/*                                                           02.Feb.2010 v1.1
  =========================================================================== 

   EID-INT.C
   ~~~~~~~~~

   Program to interpolate an error-pattern file (EP). Usefull to
   generate time-aligned error pattern files for codecs with a frame
   size shorter than the frame size of the reference codec.

   Usage:
   ~~~~~
   ep-stats [Options] master_file interpolated_EP
   Where:
   master_file  ........ master error pattern bitstream file
   interpolated_EP ... interpolated error pattern bitstream file

   Options:
   -frame # .... Set the frame size to #.
   -start # .... First frame to start processing
   -ep format .. Format for error pattern (g192, byte, or bit)
   -factor # ... Interpolation factor to be used [default: 2]
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
   18.Nov.97 v.1.0  Created
   02.Feb.10 v.1.1  Modified maximum string length for filenames to
                    avoid buffer overruns (y.hiwasaki)
  ===========================================================================
*/

/* ..... Generic include files ..... */
#include "ugstdemo.h"		/* general UGST definitions */
#include <stdio.h>		/* Standard I/O Definitions */
#include <math.h>
#include <stdlib.h>
#include <string.h>		/* memset */
#include <ctype.h>		/* toupper */

/* ..... OS-specific include files ..... */
#if defined (unix) && !defined(MSDOS)
/*                 ^^^^^^^^^^^^^^^^^^ This strange construction is necessary
                                      for DJGPP, because "unix" is defined,
				      even it being MSDOS! */
#if defined(__ALPHA)
#include <unistd.h>		/* for SEEK_... definitions used by fseek() */
#else
#include <sys/unistd.h>		/* for SEEK_... definitions used by fseek() */
#endif
#endif


/* ..... Module definition files ..... */
#include "softbit.h"            /* Soft bit definitions and prototypes */

/* ..... Include C code from another file, for EP histogram calculations */
#include "ep-stats.c"

/* ..... Definitions used by the program ..... */

/* Buffer size definitions */
#define EID_BUFFER_LENGTH 256
#define OUT_RECORD_LENGTH 512

/* Local function prototypes */


/*
  --------------------------------------------------------------------------
  Abort execution when input error pattern is not a frame erasure one.
  --------------------------------------------------------------------------
*/
void not_fer()
{
  fprintf(stderr, "%s%s%s",
	  "ERROR! This program is not intended for ",
	  "bit error patterns,\n",
	  "but only for frame erasure patterns! ABORTED.\n");
  exit(5);
}
/* ......................... End of not_fer() ......................... */


/*
  --------------------------------------------------------------------------
  display_usage()

  Shows program usage.

  History:
  ~~~~~~~~
  18/Nov/1997  v1.0 Created <simao>
  --------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage()
{
  P(("EID-INT.C Version 1.1 of 02.Feb.2010 \n"));
  P(("  Program to interpolate an error-pattern (EP) file. Usefull to generate\n"));
  P(("  time-aligned error pattern files for codecs with a frame size\n"));
  P(("  shorter than the frame size of the reference codec.\n"));
  P(("\n"));
  P(("  Usage:\n"));
  P(("   ep-stats [Options] master_file  interp_file\n"));
  P(("  Where:\n"));
  P(("   master_file  ........ master error pattern bitstream file\n"));
  P(("   interpolated_EP ... interpolated error pattern bitstream file\n"));
  P(("\n"));
  P(("  Options:\n"));
  P(("   -frame # .... Set the frame size to #.\n"));
  P(("   -start # .... First frame to start processing\n"));
  P(("   -ep format .. Format for error pattern (g192, byte, or bit)\n"));
  P(("   -factor # ... Interpolation factor to be used [default: 2]\n"));
  P(("   -ber ........ Error pattern type is bit error pattern (important\n"));
  P(("                 for bit format)\n"));
  P(("   -fer ........ Error pattern type is frame erasure pattern (important\n"));
  P(("                 for bit format)\n"));
  P(("   -q .......... Quiet operation\n"));
  P(("   -? .......... Displays this message\n"));
  P(("   -help ....... Displays a complete help message\n"));
  P(("\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ....................... End of display_usage() ....................... */


/* ************************************************************************* */
/* ************************** MAIN_PROGRAM ********************************* */
/* ************************************************************************* */
int             main (argc, argv)
  int             argc;
  char           *argv[];
{
  /* Command line parameters */
  char            ep_type = FER;      /* Type of error pattern: FER or BER */
  char            ep_format = g192;   /* Error pattern format */
  char            master_file[MAX_STRLEN]; /* Master error pattern file */
  char            interp_file[MAX_STRLEN]; /* Interpolated error pattern file */
  long            fr_len = EID_BUFFER_LENGTH;  /* Frame length in bits */
  long            fr_no=0, max_items; /* Number of frames/items to process */
  long            ep_len;              /* EP lengths */
  long            burst_len = 10;      /* Max burst length to count */
  long            start_item = 1;     /* Start inserting error from 1st one */
  ep_histogram_state master_eps;       /* Histogram for master EP */
  ep_histogram_state interp_eps;       /* Histogram for interpolated EP */
  long            factor=2;            /* Interpolation factor for EP */

  /* File I/O parameter */
  FILE           *Fep;      /* Pointer to error pattern file */
  FILE           *Fepi;      /* Pointer to error pattern file */

  /* Data arrays */
  short          *ep;	               /* Master error pattern buffer */
  short          *epi;           /* Interpolated EP buffer */

  /* Aux. variables */
  char            tmp_type;
  long            i;
  long            items;	       /* Samples/block read from file */
  long            items_up;	       /* Samples/block saved to file */
#if defined(VMS)
  char            mrs[15] = "mrs=512";
#endif
  char            quiet = 0;

  /* Pointer to a function */
  long            (*read_patt)() = read_g192;	/* To read error pattern */
  long            (*save_patt)() = save_g192;	/* To write error pattern */


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
      else if (strcmp (argv[1], "-factor") == 0)
      {
	/* Define interpolation factor for EP */
	factor = atoi(argv[2]);

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
	/* ABORT: BER not supported */
	not_fer();
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
  GET_PAR_S (1, "_Master error pattern file ........: ", master_file);
  GET_PAR_S (2, "_Interpolated error pattern file ..: ", interp_file);
  FIND_PAR_L (3, "_Interpolation factor [2] .........: ", factor, factor);


  /* Open files */
  if ((Fep= fopen (master_file, RB)) == NULL)
    KILL (master_file, 1);
  if ((Fepi= fopen (interp_file, WB)) == NULL)
    KILL (interp_file, 1);

  /* *** CHECK TYPE OF ERROR PATTERN *** */

  /* Do preliminary inspection in the ERROR PATTERN FILE to check
     its format (byte, bit, g192) */
  i = check_eid_format(Fep, master_file, &tmp_type);

  /* Check whether the specified EP format matches with the one in the file */
  if (i != ep_format)
  {
    /* The error pattern format is not the same as specified */
    fprintf (stderr, "*** Switching error pattern format from %s to %s ***\n",
	     format_str((int)ep_format), format_str(i));
    ep_format = i;
  }

  /* If the pattern is not FER, abort */
  if (tmp_type != FER)
    not_fer();


  /* *** FINAL INITIALIZATIONS *** */

  /* Use the proper data I/O functions */
  switch(ep_format)
  {
  case byte:
    read_patt = read_byte; 
    save_patt = save_byte;
    break;

  case g192:
    read_patt = read_g192; 
    save_patt = save_g192;
    break;

  default: 
    read_patt = (ep_type==BER? read_bit_ber : read_bit_fer);
    save_patt = save_bit;
    break;
  }

  /* Define how many samples are read for each frame */
  /* Bitstream may have sync headers, which are 2 samples-long */
  ep_len = fr_len;

  /* Allocate memory for data buffers */
  /* ... master/interpolated error pattern */
  if ((ep = (short *)calloc(ep_len, sizeof(short))) == NULL)
    HARAKIRI("Can't allocate memory for master error pattern. Aborted.\n",6);
  if ((epi = (short *)calloc(factor * ep_len, sizeof(short))) == NULL)
    HARAKIRI("Can't allocate memory for interpolated EP. Aborted.\n",6);

  /* Initiaize histogram state variable */
  init_ep_histogram(&master_eps, burst_len);
  init_ep_histogram(&interp_eps, burst_len);

  /* Starting frame is from 0 to number_of_frames-1 */
  start_item--;

  /* Define maximum no. of items to process */
  max_items = get_max_items(master_file, ep_format, start_item, fr_no);


  /* *** START ACTUAL WORK *** */
  while(1)
  {
    /* Read a block from EP file */
    items = read_patt (ep, ep_len, Fep);

    /* Define the number of length of the interpolated EP */
    items_up = items * factor;

    /* Aborts on error */
    if (items < 0)
      KILL(master_file, 7);

    /* Adjusts no of items if number of processed items exceed user limit */
    if (master_eps.processed+items > max_items)
    {
      items = max_items - master_eps.processed;
      items_up = items * factor;
    }

    /* Stop when reaches end-of-file */
    if (items==0)
      break;

    /* Interpolates the master EP */
    for (i=0; i<items_up; i++)
	epi[i] = ep[i/factor];

    /* Save the interpolated EP to file */
    if (save_patt (epi, items_up, Fepi) != items_up)
      KILL(interp_file, 3);

    /* Update histograms for both error patterns */
    compute_ep_histogram(ep,  items,    ep_type, &master_eps, 0);
    compute_ep_histogram(epi, items_up, ep_type, &interp_eps, 0);
  }


  /* Flush possible error counts from last block, update counters */
  compute_ep_histogram(ep, items,    ep_type, &master_eps, 0);
  compute_ep_histogram(epi, items_up, ep_type, &interp_eps, 0);


  /* ***  PRINT SUMMARY OF OPTIONS & RESULTS ON SCREEN *** */
  /* Print summary */
  fprintf (stderr, "# Burst length ......................... : %ld\n",
	   burst_len);
  fprintf (stderr, "# Pattern format %s....... : %s\n",
	   ep_type == FER? "(Frame erasure) " : "(Bit error) ....", 
	   format_str((int)ep_format));
  if (ep_type==BER)
    fprintf (stderr, "# Frame size ............................: %ld\n",
	     fr_len);
  fprintf (stderr, "# Master EP file ....................... : %s\n",
	   master_file);
  fprintf (stderr, "# Interpolated EP file ................. : %s\n",
	   interp_file);
  fprintf (stderr, "#                         MasterEP   \tInterp.EP\n");
  fprintf (stderr, "# Processed %s.... : %8ld  \t%8ld\n",
	   ep_type==BER? "bits .." : "frames ",
	   master_eps.processed, interp_eps.processed);
  fprintf (stderr, "# Disturbed %s.... : %8ld  \t%8ld\n",
	   ep_type==BER? "bits .." : "frames ",
	   master_eps.disturbed, interp_eps.disturbed);
  fprintf (stderr, "# Overall %s...........: %8.3f %%\t%8.3f %%\n",
	   ep_type==BER? "BER" : "FER", 
	   100.0 * master_eps.disturbed / (float)master_eps.processed,
	   100.0 * interp_eps.disturbed / (float)interp_eps.processed);
  fprintf (stderr, "#  Error-free %s:     %8ld  \t%8ld\n",
	   ep_type==BER? "bits" : "frames",
	   master_eps.hist[0], interp_eps.hist[0]);
  fprintf (stderr, "#  Single events:         %8ld  \t%8ld\n",
	   master_eps.hist[1], interp_eps.hist[1]);
  for (i=2; i<=burst_len; i++)
  {
    fprintf(stderr, "#  %2ld events:             %8ld  \t%8ld\n",
	    i, master_eps.hist[i], interp_eps.hist[i]);
  }
  fprintf (stderr, "#  >%2ld events:            %8ld  \t%8ld\n",
	   burst_len, master_eps.hist[i], interp_eps.hist[i]);


  /* *** FINALIZATIONS *** */

  /* Free memory allocated */
#ifdef NONO
  free(interp_hist);
  free(master_hist);
#endif
  free_ep_histogram(&interp_eps);
  free_ep_histogram(&master_eps);
  free(epi);
  free(ep);

  /* Close the output file and quit *** */
  fclose (Fep);
  fclose (Fepi);

#ifndef VMS			/* return value to OS if not VMS */
  return 0;
#endif
}
