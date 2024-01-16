/*                                                         02.Feb.2010 v.2.2
   =========================================================================

   ep-stats.c
   ~~~~~~~~~~

   Program Description:
   ~~~~~~~~~~~~~~~~~~~~

   This example program calculates the number of occurences of errors/
   frame erasures in a pattern file, as well as a short-term histogram
   to see the number of burst frame erasures.

   The file containing the error pattern will be in one of three
   possible formats: G.192 16-bit softbit format (without synchronism
   header for bit errors), byte-oriented version of the G.192 format,
   and compact, hard-bit binary (bit) mode. These are described in the
   following.

   The headerless G.192 serial bitstream format is as described in
   G.192, with the exceptions listed below. The main feature is that
   the softbits and frame erasure indicators are right-aligned at
   16-bit word boundaries (unsigned short): 
   '0'=0x007F and '1'=0x0081, and good/bad frame = 0x6B21/0x6B20

   In the byte-oriented softbit serial bitstream, only the lower byte
   of the softbits defined in G.192 are used. Hence:
   '0'=0x7F and '1'=0x81, and good/bad frame = 0x21/0x20

   In the compact (bit) mode, only hard bits are present. Each byte will
   have information about eight bits or frames. The LBbs will refer to
   bits or frames that occur first in time. Here, '1' means that a bit
   is in error or that a frame should be erased, and a '0', otherwise.

   Usage:
   ~~~~~
   ep-stats [Options] err_patt_file  burst_len
   Where:
   err_pat .... error pattern bitstream file
   burst_len .. maximum bust length to report

   Options:
   -burst imax . Define max burst length to report
   -frame # .... Set the frame size to #.
   -start # .... First frame to start processing
   -ep format .. Format for error pattern (g192, byte, or bit)
   -ber ........ Error pattern type is bit error pattern (important
                 for bit format)
   -fer ........ Error pattern type is frame erasure pattern (important
                 for bit format)
   -q .......... Quiet operation
   -? .......... Displays this message
   -help ....... Displays a complete help message

   Original Author:
   ~~~~~~~~~~~~~~~~
   Simao Ferraz de Campos Neto
   Comsat Laboratories                  Tel:    +1-301-428-4516
   22300 Comsat Drive                   Fax:    +1-301-428-9287
   Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

   History:
   ~~~~~~~~
   10.Oct.1997 v.1.0 Created <simao.campos@comsat.com>

   12.Apr.2000 v.2.0 Modified code to produce min, max, average
                     distance between error/erasure events
                     <simao.campos@labs.comsat.com>
    1.Nov.2008 v.2.1 Modified code ti use the -start option properly
                     <Ericsson>
    2.Feb.2010 v.2.2 Modified maximum string length for filename to avoid
                     buffer overruns (y.hiwasaki)
   ========================================================================= */

/* ..... Generic include files ..... */
#include "ugstdemo.h"		/* general UGST definitions */
#include <stdio.h>		/* Standard I/O Definitions */
#include <math.h>
#include <stdlib.h>
#include <string.h>		/* memset */
#include <ctype.h>		/* toupper */

/* ..... OS-specific include files ..... */
#ifdef VMS
#include <stat.h>
#else
#include <sys/stat.h>
#endif

/* This strange construction is necessary for DJGPP, because "unix" 
                   is defined, even it being MSDOS! */
#if defined (unix) && !defined(MSDOS)
/*                 ^^^^^^^^^^^^^^^^^^ */
#if defined(__ALPHA)
#include <unistd.h>		/* for SEEK_... definitions used by fseek() */
#else
#include <sys/unistd.h>		/* for SEEK_... definitions used by fseek() */
#endif
#endif



/* ..... Module definition files ..... */
#ifdef IF_NEEDED
#include "eid.h"		/* EID functions */
#include "eid_io.h"		/* EID I/O functions */
#endif
#include "softbit.h"            /* Soft bit definitions and prototypes */

/* ..... Definitions used by the program ..... */

/* Buffer size definitions */
#define EID_BUFFER_LENGTH 256
#define OUT_RECORD_LENGTH 512

/* Local type definitions */
typedef struct {
  long burst_len;  /* Maximum burst leng.that will be individually observed */
  long *hist;      /* EP histogram array */
  long count;      /* Count number of occurences */
  long in_event;   /* Flags to indicate if in a burst event */
  long unexpected; /* Number of unexpected conversions */
  char first_time; /* Flag for general initializations */
  short *ep;       /* Buffer for hard-bit version of error pattern */
  long processed, disturbed;  /* No. of processed/disturbed bits */
  float event_distance; /* Sum of error/erasure event distances */
  float event_distance_sq; /* Squared sum of error/erasure event distances */
  long min_distance; /* Minimum distance between events */
  long max_distance; /* Maximum distance between events */
  long event_started; /* Starting point of error/erasure event */
  long last_event; /* Position of last error/erasure event */
  long event_no;   /* Number of error/erasure events */
} ep_histogram_state;

/* Local function prototypes */
int init_ep_histogram ARGS((ep_histogram_state *state, long burst_len));
long compute_ep_histogram ARGS((short *pattern, long items, int ep_type,
				ep_histogram_state *state, int reset));
void free_ep_histogram ARGS((ep_histogram_state *state));

/* 
  ---------------------------------------------------------------------------
  int init_ep_histogram (ep_histogram_state *state, long burst_len);
  ~~~~~~~~~~~~~~~~~~~~~

  Function to set initial values in the EP histogram state variable
  and allocate memory for EP histogram buffer.

  Parameters:
  ~~~~~~~~~~~
  state ..... pointer to state-variable for error pattern histogram
  burst_len . longest burst length to look for

  Returned value: 
  ~~~~~~~~~~~~~~~
  0: OK
  -1: cound't allocate histogram array

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  19.Nov.97  v.1.0  Created.

  ---------------------------------------------------------------------------
*/
int init_ep_histogram(state, burst_len)
ep_histogram_state *state;
long burst_len;
{
  long i;

  /* Reset some variables */
  state->in_event=0;
  state->first_time=1;
  state->ep=0;
  state->burst_len = burst_len;
  state->min_distance=2147483647;
  state->max_distance=0;
  state->event_distance=0.0;
  state->event_distance_sq=0.0;
  state->event_started=0;
  state->event_no=0;
  state->unexpected=0;
  state->processed=0;
  state->disturbed=0;
  state->count=0;
  state->last_event=0;

  /* Allocate memory for histograms */
  state->hist = (long *)calloc(burst_len+2, sizeof(long));
  if (state->hist == NULL)
    return(-1);

  /* Reset values in histogram */
  for (i=0; i<=burst_len+1; i++)
    state->hist[i] = 0;

  /* Return OK */
 return(0);
}
/* ..................... End of init_ep_histogram() ..................... */


/* 
  ---------------------------------------------------------------------------
  void free_ep_histogram (ep_histogram_state *state);
  ~~~~~~~~~~~~~~~~~~~~~~

  Deallocate memory for EP histogram buffer.

  Parameters:
  ~~~~~~~~~~~
  state ..... pointer to state-variable for error pattern histogram

  Returned value: void
  ~~~~~~~~~~~~~~~

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  19.Nov.97  v.1.0  Created.

  ---------------------------------------------------------------------------
*/
void free_ep_histogram(state)
ep_histogram_state *state;
{
  free(state->hist);
}
/* ..................... End of free_ep_histogram() ..................... */


/*
  ---------------------------------------------------------------------------
  long compute_ep_histogram (short *pattern, long items, int ep_type,
  ~~~~~~~~~~~~~~~~~~~~~~~~~  long *ep_hist, long burst_len,
			     long *processed, long *disturbed, int reset);

  Function that computes the short term histogram for an input error
  pattern. This function performs complete initialization at the first
  time it is called, and at any later time if parameter reset is
  non-zero. The function performs accumulative calculations since stat
  or since the last reset. If the number of softbits in the pattern is
  zero, the function assumes that this is the last step of a series of
  measurements and flushes any necessary updates, followed by a
  partial reset of some of the static variables.

  Parameters:
  ~~~~~~~~~~~
  pattern ..... headerless G.192 bit error/frame erasure pattern array.
  items ....... Number of softbits/erasure flags in pattern. When 'items' 
                is null, the function assumes that this is the last
                computation of a series (like when the EOF is found),
                and the update of the counters is followed by a
                partial reset of the state variables.
  ep_type ..... Error pattern type (BER or FER).
  state ....... State variable for the error pattern histogram. 
                This function will allocate memory for the temporary
                error-pattern buffer, and reset a number of variable
                in the first time the function is invoked. In the
                state variable, hist[0] will contain the number of
                undisturbed bits, and hist[burst_len] will contain the
                number of bursts longer than 'burst_len' bits/frames.
  reset ....... Flag to force the initialization of the static variables 
                as well as the associated counters. A reset is always
                performed the first time that the function is invoked.

  Returned value:
  ~~~~~~~~~~~~~~~
  Returns the number of samples in the pattern which had an unexpected
  value. Unexpected values would be values other than 0x6b2? (?=0..F)
  for FER patterns, or 0x007F/0x0081 for BER patterns. 

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  10.Oct.97  v1.0  Created.
  19.Nov.97  v1.1  Changed to use a state variable rather than local 
                   static variables. Necessary for processing multiple
                   EP at the same time. <simao>
                       
  ---------------------------------------------------------------------------
*/
long compute_ep_histogram(pattern, items, ep_type, s, reset)
short *pattern;
long items;
char ep_type, reset;
ep_histogram_state *s;
{
  long i;
  long this_event;
  float delta;

  /* Initial reset */
  if (reset || s->first_time || items == 0)
  {
    /* Free temporary storage if previously allocated */
    if (s->ep)
      free(s->ep);

    /* Allocate memory for temporary buffer */
    if (s->ep==NULL && items!=0)
      if ((s->ep = (short *)calloc(items, sizeof(short))) == NULL)
	HARAKIRI("Can't allocate memory for counter. Aborted.\n",6);

    /* Flush possible error counts from last block, update counters */
    if (s->in_event)
    {
      s->hist[s->count <= s->burst_len? s->count : s->burst_len+1]++;
      s->disturbed += s->count;
      s->count = 0;
      s->in_event = 0;

      s->event_no++;
      s->last_event = s->event_started;
    }

    /* Reset error event counter array */
    if (reset || s->first_time)
    {
      for (i=0; i<=s->burst_len+1; i++)
	s->hist[i] = 0;
      s->processed = s->disturbed = 0;
    }

    /* Set variables to a known state */
    s->count = s->in_event = s->first_time = 0;
  }

  /* Stop if no items to be processed */
  if (items==0)
    return(0);

  /* Update counter */
  s->processed += items;

  /* Convert EP to hardbit notation */
  s->unexpected += soft2hard(pattern, s->ep, items, ep_type);

  /* Search for errors/erasures */
  for (i=0; i<items; i++)
  {
    if (s->ep[i])
    {
      s->count++;
      if (!s->in_event) 
      {
        s->event_started = s->processed - items + i;
        delta=s->event_started - s->last_event;
        if (delta > s->max_distance)
          s->max_distance = delta;
        if (delta < s->min_distance)
          s->min_distance = delta;
        s->event_distance+= delta;
        s->event_distance_sq+= delta * delta ;
#ifdef DEBUG
        fprintf(stderr, "Event at bit/frame: %ld\n", s->event_started);
#endif
      }
      s->in_event = 1;
    }
    else
    {
      /* No longer in an error/erasure event. Flush the info to the EP
	 counter array, update disturb.counter and reset variables/flags */
      if (s->in_event)
      {
        this_event = (s->count <= s->burst_len)? s->count : s->burst_len+1; 
	s->hist[this_event]++;
	s->disturbed += s->count;
	s->count = 0;
	s->in_event = 0;

	s->event_no++;
        s->last_event = s->event_started;
      }
    }
  }

  /* Set number of undisturbed bits/frames */
  s->hist[0] = s->processed - s->disturbed;

  /* Return number of unexpected values */
  return (s->unexpected); 
}
/* ...................... End of compute_ep_histogram() ................... */


/*
  file .... file name
  format .. error pattern format (g192, byte, compact
  start ... first item to process (bit or frame, depending on error
            pattern type)
  limit ... max number of items to process
*/
long get_max_items(file, format, start, limit)
char *file;
char format;
long start, limit;
{
  struct stat fileinfo;
  long bytes, max_items;

  stat(file, &fileinfo);
  switch(format)
  {
  case g192:
    bytes = fileinfo.st_size - start*2;
    break;
  case byte:
    bytes = fileinfo.st_size - start;
    break;
  case compact:
    bytes = fileinfo.st_size - start/8;
    break;
  }
  

  switch(format)
  {
  case g192:
    max_items = bytes/2;
    break;
  case byte:
    max_items = bytes;
    break;
  case compact:
    max_items = bytes * 8;
    break;
  }
  if (limit>0 && max_items > limit) 
    max_items = limit;

  return (max_items);
}
/* ......................... End of get_max_items() ....................... */


/* 
    *********************************************************
    ***** FROM HERE ON, IT IS A CONDITIONAL COMPILATION ***** 
    ********** TO GENERATE THE EXECUTABLE PROGRAM ***********
    *********************************************************
*/


#ifdef RUN
/*
   --------------------------------------------------------------------------
   display_usage(int level);

   Shows program usage.

   History:
   ~~~~~~~~
   11/Aug/1997  v1.0 Created <simao>
   --------------------------------------------------------------------------
 */
#define P(x) printf x
void            display_usage (level)
int level;
{
  P(("ep-stats.c - Version 2.2 of 02.Feb.2010 \n\n"));

  if (level)
  { 
  P(("Program Description:\n"));
  P(("~~~~~~~~~~~~~~~~~~~~\n"));
  P(("\n"));
  P(("This example program calculates the number of occurences of errors/\n"));
  P(("frame erasures in a pattern file, as well as a short-term histogram\n"));
  P(("to see the number of burst frame erasures.\n"));
  P(("\n"));
  P(("The file containing the error pattern will be in one of three\n"));
  P(("possible formats: G.192 16-bit softbit format (without synchronism\n"));
  P(("header for bit errors), byte-oriented version of the G.192 format,\n"));
  P(("and compact, hard-bit binary (bit) mode. These are described in the\n"));
  P(("following.\n"));
  P(("\n"));
  P(("The headerless G.192 serial bitstream format is as described in\n"));
  P(("G.192, with the exceptions listed below. The main feature is that\n"));
  P(("the softbits and frame erasure indicators are right-aligned at\n"));
  P(("16-bit word boundaries (unsigned short): \n"));
  P(("'0'=0x007F and '1'=0x0081, and good/bad frame = 0x6B21/0x6B20\n"));
  P(("\n"));
  P(("In the byte-oriented softbit serial bitstream, only the lower byte\n"));
  P(("of the softbits defined in G.192 are used. Hence:\n"));
  P(("'0'=0x7F and '1'=0x81, and good/bad frame = 0x21/0x20\n"));
  P(("\n"));
  P(("In the compact (bit) mode, only hard bits are present. Each byte will\n"));
  P(("have information about eight bits or frames. The LBbs will refer to\n"));
  P(("bits or frames that occur first in time. Here, '1' means that a bit\n"));
  P(("is in error or that a frame should be erased, and a '0', otherwise.\n"));
  }
  else
  {
  P(("Program that calculates the number of occurences of errors/frame\n"));
  P(("erasures in a pattern file, as well as a short-term histogram.\n"));
  }

  P(("Usage:\n"));
  P((" ep-stats [Options] err_patt_file  burst_len\n"));
  P(("Where:\n"));
  P((" err_pat .... error pattern bitstream file\n"));
  P((" burst_len .. maximum bust length to report\n"));
  P(("\n"));
  P(("Options:\n"));
  P((" -frame # .... Set the frame size to #\n"));
  P((" -start # .... First frame to start processing\n"));
  P((" -burst max .. Define max burst length to report\n"));
  P((" -ep format .. Format for error pattern (g192, byte, or bit)\n"));
  P((" -ber ........ Pattern type is bit error pattern\n"));
  P((" -fer ........ Pattern type is frame erasure pattern\n"));
  P((" -q .......... Quiet operation\n"));
  P((" -? .......... Displays this message\n"));
  P((" -help ....... Displays a complete help message\n"));

  /* Quit program */
  exit (-128);
}
#undef P
/* .................... End of display_usage() ........................... */


/* ************************************************************************* */
/* ************************** MAIN_PROGRAM ********************************* */
/* ************************************************************************* */
int             main (argc, argv)
  int             argc;
  char           *argv[];
{
  /* Command line parameters */
  char            ep_type = BER;      /* Type of error pattern: FER or BER */
  char            ep_format = g192;   /* Error pattern format */
  char            ep_file[MAX_STRLEN]; /* Error pattern file */
  long            fr_len = EID_BUFFER_LENGTH;  /* Frame length in bits */
  long            fr_no=0, max_items; /* Number of frames/items to process */
  long            ep_len;             /* EP lengths */
  long            burst_len = 10;      /* Max burst length to count */
  long            start_item = 1;     /* Start analyzing errors from 1st one */
  long            preamble_items=0;
  /* File I/O parameter */
  FILE           *Fep;      /* Pointer to error pattern file */

  /* Data arrays and structures */
  short          *ep;	               /* Error pattern buffer */
  ep_histogram_state eps;

  /* Aux. variables */
  char            tmp_type;
  long            i;
  long            items;	       /* Samples read from file */
#if defined(VMS)
  char            mrs[15] = "mrs=512";
#endif
  char            quiet = 0;
  float           ftmp;

  /* Pointer to a function */
  long            (*read_patt)() = read_g192;	/* To read error pattern */


  /* ......... GET PARAMETERS ......... */

  /* Check options */
  if (argc < 2)
    display_usage (0);
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp (argv[1], "-start") == 0)
      {
	/* Define starting sample/frame for error insertion */
	start_item = atol (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-frame") == 0)
      {
	/* Define frame size for processing */
	fr_len = atoi(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-n") == 0)
      {
	/* Define number of frames to process */
	fr_no = atoi(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-burst") == 0)
      {
	/* Define how long to search for burst sequences */
	/* The + 1 position will hold how many bursts longer than the
           max specified were observed */
	burst_len = atoi(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-ep") == 0)
      {
	/* Define error pattern format */
	for (i=0; i<nil; i++)
	{
	  if (strstr(argv[2], format_str(i)))
	    break;
	}
	if (i==nil)
	{
	  HARAKIRI("Invalid error pattern format type. Aborted\n",5);
	}
	else
	  ep_format = i;

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-ber") == 0 || strcmp (argv[1], "-BER") == 0)
      {
	/* Error pattern type: BER */
	ep_type = BER;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-fer") == 0 ||
	       strcmp (argv[1], "-FER") == 0 ||
	       strcmp (argv[1], "-bfer") == 0 ||
	       strcmp (argv[1], "-BFER") == 0)
      {
	/* Error pattern type: FER */
	ep_type = FER;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-q") == 0)
      {
	/* Set quiet mode */
	quiet = 1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-?") == 0)
      {
	display_usage(0);
      }
      else if (strstr(argv[1], "-help"))
      {
	display_usage(1);
      }
      else
      {
	fprintf (stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
		 argv[1]);
	display_usage (0);
      }
  }

  /* Get command line parameters */
  GET_PAR_S (1, "_Error pattern file ........: ", ep_file);
  FIND_PAR_L (2, "_Burst length ..............: ", burst_len, burst_len);


  /* Open files */
  if ((Fep= fopen (ep_file, RB)) == NULL)
    HARAKIRI ("Could not open error pattern file\n", 1);


  /* *** CHECK TYPE OF ERROR PATTERN *** */

  /* Do preliminary inspection in the ERROR PATTERN FILE to check
     its format (byte, bit, g192) */
  i = check_eid_format(Fep, ep_file, &tmp_type);

  /* Check whether the specified EP format matches with the one in the file */
  if (i != ep_format)
  {
    /* The error pattern format is not the same as specified */
    fprintf (stderr, "*** Switching error pattern format from %s to %s ***\n",
	     format_str((int)ep_format), format_str(i));
    ep_format = i;
  }

  /* Check whether the specified EP type matches with the one in the file */
  if (tmp_type != ep_type)
  {
    /* The error pattern type is not the same as specified */

    if (ep_format == compact)
    {
      fprintf (stderr, "*** Cant infer error pattern type. Using %s ***\n",
	       type_str((int)ep_type));
    }
    else
    {
      fprintf (stderr, "*** Switching error pattern type from %s to %s ***\n",
	       type_str((int)ep_type), type_str((int)tmp_type));
      ep_type = tmp_type;
    }
  }


  /* *** FINAL INITIALIZATIONS *** */

  /* Use the proper data I/O functions */
  read_patt = ep_format==byte? read_byte
              : (ep_format==g192? read_g192 :
		 (ep_type==BER? read_bit_ber : read_bit_fer));

  /* Define how many samples are read for each frame */
  /* Bitstream may have sync headers, which are 2 samples-long */
  ep_len = fr_len;


  /* Allocate memory for data buffers */
  /* ... error pattern from file */
  if ((ep = (short *)calloc(ep_len, sizeof(short))) == NULL)
    HARAKIRI("Can't allocate memory for error pattern. Aborted.\n",6);

  /* Initiaize histogram state variable */
  init_ep_histogram(&eps, burst_len);

  /* Starting frame is from 0 to number_of_items-1 */
  start_item--;

  /* Define maximum no. of items to process */
  max_items = get_max_items(ep_file, ep_format, start_item, fr_no);

   /* *** START ACTUAL WORK *** */

  /* first skip the part [0-start_item] */
  /* Read preamble from EP file */
  preamble_items=0;
  while(preamble_items < start_item)
  {
      if((ep_format==byte) || (ep_format==g192)){ 	
        items = read_patt (ep, 1, Fep); /*one item at a time consumed, for g.192 and byte */
      } else if ((ep_format==compact)){
          if ((start_item%8)==0){
            items = read_patt (ep, 8, Fep); /*8 items time consumed, for bit/compact packed fer */
          } else {
              fprintf(stderr,"Error: the start/preamble segment must be byte-aligned (divisble by 8), start=%ld, ( %ld%%8 != 0)\n", 
                    start_item+1, start_item);    
              KILL(ep_file, 7);
          }
      }
 
   
    /* Aborts on error */
      if (items < 0){
           KILL(ep_file, 7);
       }
       preamble_items += items;
  }

  /* now finaly analyze target part*/
  while(1)
  {
    /* Read a block from EP file */
    items = read_patt (ep, ep_len, Fep);

    /* Aborts on error */
    if (items < 0)
      KILL(ep_file, 7);

    /* Adjusts no of items if number of processed items exceed user limit */
    if (eps.processed+items > max_items)
    {
      items = max_items - eps.processed;
    }

    /* Stop when reaches end-of-file or top processing */
    if (items==0)
      break;

    /* Computes histogram */
    compute_ep_histogram(ep, items, ep_type, &eps, 0);
  }

  /* Flushes any pending processing */
  compute_ep_histogram(ep, items, ep_type, &eps, 0);


  /* ***  PRINT SUMMARY OF OPTIONS & RESULTS ON SCREEN *** */

  /* Calculate the std.dev. of the distance between events */
  ftmp = pow(eps.event_distance,2) / eps.event_no;
  ftmp = sqrt((eps.event_distance_sq - ftmp)/(float)(eps.event_no-1));

  /* Print summary */
  fprintf (stderr, "# Error pattern file ................... : %s\n",
	   ep_file);
  fprintf (stderr, "# Burst length ......................... : %ld\n",
	   eps.burst_len);
  fprintf (stderr, "# Pattern format %s....... : %s\n",
	   ep_type == FER? "(Frame erasure) " : "(Bit error) ....", 
	   format_str((int)ep_format));
  if (ep_type==BER)
    fprintf (stderr, "# Frame size ............................: %ld\n",
	     fr_len);
  if (ep_type!=BER)
     fprintf (stderr, "# Starting  %s...................... : %ld  \n",
	                   "frame ", start_item+1);

  fprintf (stderr, "# Processed %s..................... : %ld \n",
	   ep_type==BER? "bits .." : "frames ", eps.processed);
  fprintf (stderr, "# Disturbed %s..................... : %ld \n",
	   ep_type==BER? "bits .." : "frames ", eps.disturbed);
  fprintf (stderr, "# Overall %s............ : %f %%\n",
	   ep_type==BER? "bit error rate ..." : "frame erasure rate", 
	   100.0 * eps.disturbed / (float)eps.processed);
  if (eps.event_no > 0)
  {
    fprintf (stderr, "# Average event distance %s : %.0f +- %.0f %s\n",
             "...............", eps.event_distance/eps.event_no, ftmp,
             ep_type==BER? "bits" : "frames ");
    fprintf (stderr, "# Min/Max event distance %s : %ld / %ld %s\n",
             "...............", eps.min_distance, eps.max_distance, 
             ep_type==BER? "bits" : "frames ");
  }
  fprintf (stderr, "#  Error-free %s:\t%ld\n",
	   ep_type==BER? "bits" : "frames", eps.hist[0]);
  fprintf(stderr, "#  Single events:\t%ld\n", eps.hist[1]);
  for (i=2; i<=eps.burst_len; i++)
  {
    fprintf(stderr, "#  %ld events:    \t%ld\n", i, eps.hist[i]);
  }
  fprintf(stderr, "#  >%ld events:    \t%ld\n", eps.burst_len, eps.hist[i]);
  if (eps.unexpected)
    fprintf(stderr, "#*** %ld unexpected samples found in EP file\n",
	    eps.unexpected);

  /* *** FINALIZATIONS *** */

  /* Free memory allocated */
  free_ep_histogram(&eps);
  free(ep);

  /* Close the output file and quit *** */
  fclose (Fep);

#ifndef VMS			/* return value to OS if not VMS */
  return 0;
#endif
}

#endif /* RUN -- conditional compilation directive to create executable */
