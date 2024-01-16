/*                                                            02.Feb.2010 v1.1 
  ============================================================================

  CVT-HEAD.C
  ~~~~~~~~~~

  Description: 
  ~~~~~~~~~~~~

  Program that converts a serial bitstream from the old STL92 format
  back/to STL96 format.

  The STL92 format for BS was SYNC_FLAG followed by softbits. The
  STL96 uses the G.192 bistream format, in which a sync HEADER is used
  (a sync header is composed of a sync flag followed by the frame
  length).

  Initial suppositions: 
  - input file has a header (either STL92 or STL96)

  Usage:
  cvt-head 
  Options:
  -n ............. number of frames to process
  -start ......... first frame to start operation
  -frame ......... payload size in no. of softbits
  -nosync ........ don't use sync headers in output file
  -sync .......... use sync headers in output file [default]
  -nofer ......... revert any frame erasure flags (0x6B20) to 0x6B21
  -q ............. quiet operation
  -help, -? ...... display help message

  Original author: 
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto, Sr.MTS  *All comments are strictly my own*
  COMSAT Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao.campos@comsat.com
                                          
  History
  ~~~~~~~
  18.Jan.99 v1.0 Created
  02.Feb.10 v1.1 Modified maximum string length for filenames (y.hiwasaki)
  ============================================================================
*/

/* ..... Generic include files ..... */
#include <stdio.h>                      /* Standard I/O Definitions */
#include <stdlib.h>                     /* for atoi(), atol() */
#include <string.h>                     /* for strstr() */
#include <math.h>                       /* for ceil() */
#include "ugstdemo.h"                   /* general UGST definitions */

/* ..... Specific include files ..... */
#if defined(VMS)		/* for checking file sizes */
#include <stat.h>
#else
#include <sys/stat.h>
#endif

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
#include "ugst-utl.h"                   /* format conversion functions */

/* ..... Module definition files ..... */
#include "softbit.h"            /* Soft bit definitions and prototypes */


/* ... Local function prototypes ... */
char check_bs_format ARGS((FILE *F, char *file, char *type));
int check_sync ARGS((FILE *F, char *file, char *bs_type, long *fr_len,
		     char *bs_format));
void display_usage ARGS((int level));
/* ......... End of local function prototypes ......... */ 


/* ... Local defines, pseudo-functions ... */
#define OVERHEAD_STL92 1 /* Overhead is sync word */
#define OVERHEAD_STL96 2 /* Overhead is sync word and length word*/


/* Operating modes */
enum STL_versions {raw, STL92, STL96};


/* ************************* AUXILIARY FUNCTIONS ************************* */

/*
  --------------------------------------------------------------------------
  char check_bs_format (FILE *F, char *file, char *type);
  ~~~~~~~~~~~~~~~~~~~~

  Function that checks the format (g192, byte, bit) in a given
  bitstream, and tries to guess the type of data (bit stream or frame
  erasure pattern)

  Parameter:
  ~~~~~~~~~~
  F ...... FILE * structure to file to be checked
  file ... name of file to be checked

  type ... pointer to guessed data type in file:
           NO_HEADER for headerless BS
           HAS_HEADER for sync header (sync flag followed by frame length) 
           HAS_FLAG_ONLY for BS with sync flag only, but no frame length)

  Returned value:
  ~~~~~~~~~~~~~~~
  Returns the data format (g192, byte, bit) found in file.

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  20.Aug.97  v.1.0  Created.
  18.Jan.99  v.1.1  Added case for HAS_FLAG_ONLY <simao>
  -------------------------------------------------------------------------- 
*/
char check_bs_format(F, file, type)
FILE *F;
char *file;
char *type;
{

  short word;
  char ret_val;

  /* Get a 16-bit word from the file */
  fread(&word, sizeof(short), 1, F);

  /* Use some heuristics to determine what type of file is this */
  switch((unsigned)word)
  {
  case 0x7F7F:
  case 0x7F81:
  case 0x817F:
  case 0x8181:
    /* Byte-oriented G.192 bitstream */ 
    *type = NO_HEADER;
    ret_val = byte;
    break;

  case 0x2020:
  case 0x2021:
  case 0x2120:
  case 0x2121:
    /* Byte-oriented G.192 syncs */
    *type = HAS_HEADER;
    ret_val = byte;
    break;

  case 0x007F:
  case 0x0081:
    /* G.192 bitstream in natural order */
    *type = NO_HEADER;
    ret_val = g192;
    break;

  case 0x6B21:
  case 0x6B20:
    /* G.192 sync header in natural order */
    *type = HAS_HEADER;
    ret_val = g192;
    break;

  case 0x7F00:
  case 0x8100:
  case 0x216B:
  case 0x206B:
    /* G.192 format that needs to be byte-swapped */
    fprintf(stderr, "File %s needs to be byte-swapped! Aborted.\n", file);
    exit(8);

  default:
    /* Assuming it is compact bit mode */
   *type = nil; /* Not possible to infer type for binary format! */
    ret_val = compact;
  }

  /* Final check to see if the input bitstream is a byte-oriented G.192
     bitstream. In this case, the first byte is 0x2n (n=0..F) and the
     second byte must be the frame length */
  if ((((unsigned)word>>8) & 0xF0) == 0x20)
  {
     *type = HAS_HEADER;
     ret_val = byte;
  }
  /* Rewind file & and return format identifier */
  fseek(F, 0l, SEEK_SET);
  return(ret_val);
}
/* ...................... End of check_bs_format() ...................... */


/* 
  ------------------------------------------------------------------------
  int check_sync (FILE *F, char *file, char *bs_type,
                 long *fr_len, *bs_format);

  Check that the bit stream has a synchronization flag and header,
  returning the length of the overhead. For headerless BS, it is 0,
  for STL96 is 2, for STL92 is 1.

  Parameter:
  ~~~~~~~~~~
  F ...... FILE * structure to file to be checked
  file ... name of file to be checked
  bs_type  .... pointer to type of bitstream (G192, BYTE, or COMPACT)
  fr_len ...... pointer to frame length, in number of (soft)bits
  bs_format ... pointer to guessed data type in file:
                - NO_HEADER for headerless BS
		- HAS_HEADER for sync header (sync flag followed by
		  frame length)
		- HAS_FLAG_ONLY for BS with sync flag only, but no
		  frame length)

  Returned value:
  ~~~~~~~~~~~~~~~

  Returns whether a synchonization HEADER was found in the
  bitstream. Also changes the bs_type, fr_len, and bs_format input
  variables. The file pointer is rewinded to the beginning of the
  file.

  Original author: <simao.campos@comsat.com>
  ~~~~~~~~~~~~~~~~

  History:
  ~~~~~~~~
  20.Aug.97  v.1.0  Created.
  18.Jan.99  v.1.1  Added case for HAS_FLAG_ONLY <simao>
  -------------------------------------------------------------------------- 
*/
int check_sync(F, file, bs_type, fr_len, bs_format)
FILE *F;
char *file;
char *bs_type, *bs_format;
long *fr_len;
{
  long i;
  char sync_header, sync_flag;

  /* Initializes expecting a non-G.192-compliant BS */
  sync_header = sync_flag = 0;

  /* Get info on bitstream */
  *bs_format = check_bs_format(F, file, bs_type);

  /* If the BS seems to have a header, search for two consecutive
     ones. If it finds, determines which is the frame size */
  if (*bs_type == HAS_HEADER)
  {
    /* It has a sync flag, but is it a full sync header? */
    sync_flag = 1;

    /* The input BS may have a G.192 synchronization header - verify */
    if (*bs_format == g192)
    {
      short tmp[2];

      /* Get presumed first G.192 sync header */
      fread(tmp, sizeof(short), 2, F);
      /* tmp[1] should have the frame length */
      i = tmp[1];
      /* advance file to the (presumed) next G.192 sync header */
      fseek(F, (long)(tmp[1])*sizeof(short), SEEK_CUR);
      /* get (presumed) next G.192 sync header */
      fread(tmp, sizeof(short), 2, F);
      /* Verify whether ... */
      if (((tmp[0] & 0xF0) == 0x20) && (i == tmp[1]))
      {
	/* ... found a sync header */
	*fr_len = i;
	sync_flag = 1;
	sync_header = 2; 
      }
      else
	sync_header = 1;
    }
    else if (*bs_format == byte)
    {
      char tmp[2];

      /* Get presumed first byte-wise G.192 sync header */
      fread(tmp, sizeof(char), 2, F);
      /* tmp[1] should have the frame length */
      i = tmp[1];
      /* advance file to the (presumed) next byte-wise G.192 sync header */
      fseek(F, (long)tmp[1], SEEK_CUR);
      /* get (presumed) next G.192 sync header */
      fread(tmp, sizeof(char), 2, F);
      /* Verify */
      if (((tmp[0] & 0xF0) == 0x20) && (i == tmp[1]))
      {
	*fr_len = i;
	sync_flag = 1;
	sync_header = 2; 
      }
      else
	sync_header = 1;
    }
    else
      sync_header = sync_flag = 0;

    /* Rewind file */
    fseek(F, 0l, SEEK_SET);
  }
  else
    /* BS type is COMPACT; return 0 */
    return(0);

  /* Check the case when it has sync flag but not sync header */
  if (sync_flag==sync_header)
  {
    int tmp = 0x20;
    long flen = -1;

    /* Search first occurence of LSB of sync flag in BS */
    while ((getc(F)&0xF0)!=tmp && !feof(F));
    /* Search next occurence of LSB of sync flag in BS */
    while ((getc(F)&0xF0)!=tmp && !feof(F))
      flen++; /* Increase counter */

    /* Use the one found; use as no.of bytes read if BYTE of no.of words
       if G192 */
    *fr_len = (*bs_format == g192)? flen/2: flen;
    *bs_type = HAS_FLAG_ONLY;

    /* Rewind file */
    fseek(F, 0l, SEEK_SET);
  }

  /* Return info on whether or not a frame sync header was found */
  return(sync_header);
}

/*
  --------------------------------------------------------------------------
  display_usage()

  Shows program usage.

  History:
  ~~~~~~~~
  20/Aug/1997  v1.0 Created <simao>
  --------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage(level)
int level;
{
}
#undef P
/* .................. End of display_usage() ....................... */ 

/*
  ------------------------------------------------------------------------
  Return a string with the version of bitstream
  ------------------------------------------------------------------------
*/
char *cvt_type(i)
int i;
{
  switch(i)
  {
  case raw:
    return "Headerless";
    break;
  case STL92:
    return "STL92";
    break;
  case STL96:
    return "STL96";
    break;
  }
  return "";
}
/* ......................... End of cvt_type() ......................... */



/* ------------------------------------------------------------------------ */
/*                              Main Program                                */
/* ------------------------------------------------------------------------ */
int main(argc, argv)
int argc;
char *argv[];
{
  long            N = 256, N1 = 1, N2 = 0, size, i;
  long            fr_len = 256;         /* Frame length in number of bits */
  long            items, bitno, cur_blk;
  char            bs_type = HAS_HEADER; /* Type for bitstream */
  char            bs_format = g192;     /* Format for bitstream */
  long            Ninp, Nout;      /* Frame length for input and input files */
  short          *bs_inp, *bs_out;	        /* bit-stream data */
  char            inpfil[MAX_STRLEN], outfil[MAX_STRLEN];
  FILE           *Fibs, *Fobs;
  long            start_byte, flags_fixed=0;
  char            sync_inp=2, sync_out=1, fer=1;
  short           overhead_inp = OVERHEAD_STL96;
  short           overhead_out = OVERHEAD_STL96;
  short           inp_type, out_type;
#ifdef VMS
  char            mrs[15];	/* for correct mrs in VMS environment */
#endif
  char quiet=0;

  /* Pointer to a function */
  long            (*read_data)() = read_g192;	/* To read input bitstream */
  long            (*save_data)() = save_g192;	/* To save output bitstream */


  /* ......... GET PARAMETERS ......... */

  /* Check options */
  if (argc < 2)
    display_usage(0);
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp (argv[1], "-start") == 0)
      {
	/* Define starting sample for s/p operation */
	N1 = atol (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-frame") == 0)
      {
	/* Define input & output encoded speech bitstream format */
	fr_len = atoi(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-n") == 0)
      {
	/* Define number of frames to process */
	N2 = atoi(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-sync") == 0)
      {
	/* Use sync header to generate output file */
	sync_out=1;
	
	/* Update arg[vc] */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-nosync") == 0)
      {
	/* Don't use sync header */
	sync_out=0;
	
	/* Update arg[vc] */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-nofer") == 0)
      {
	/* Convert any FER flags (0x6B20) to good frame indicator (0x6B21) */
	fer=0;
	
	/* Update arg[vc] */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-q") == 0)
      {
	/* quiet operation - don't print progress flag */
	quiet=1;
	
	/* Update arg[vc] */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0)
      {
	/* Display help */
	display_usage(0);
      }
      else if (strstr(argv[1], "-help"))
      {
	/* Display help */
	display_usage(1);
      }
      else if (strcmp(argv[1], "-") == 0)
      {
	/* End of option parsing */
	break;
      }
      else
      {
	fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
		argv[1]);
	display_usage(0);
      }
  }

  /* Read parameters for processing */

  GET_PAR_S(1,  "_File to be converted: ....... ", inpfil);
  GET_PAR_S(2,  "_Output File: ................ ", outfil);
  FIND_PAR_L(3, "_Softbits per frame: ......... ", N, N);
  FIND_PAR_L(4, "_Start Frame: ................ ", N1, N1);
  FIND_PAR_L(5, "_Frames to process: .......... ", N2, N2);

  /* ***** FILE OPERATIONS ***** */

#ifdef VMS
  sprintf(mrs, "mrs=%d", 512);
#endif

  /* Open input file */
  if ((Fibs = fopen(inpfil, RB)) == NULL)
    KILL(inpfil, 2);

  /* Open (create) output file */
  if ((Fobs = fopen(outfil, WB)) == NULL)
    KILL(outfil, 3);

  /* Define 1st byte to process */
  start_byte = N * (--N1) * sizeof(short);
  
  /* Move file's pointer to 1st desired block */
  if (fseek(Fibs, start_byte, 0) < 0l)
    KILL(inpfil, 4);

  /* ***** DEFINE FRAME SIZES AND FILE TYPES ***** */

  /* Gather data about input bitstream */

  /* Find the actual frame size for the serial bit stream from the
     serial bitstream. If this is a headerless bitstream, assumes that
     the frame size is based on the provided block size. */

  /* Find the number of words per frame (payload only!), 
     check whether a sync header was found, and also returns the type 
     of bitstream (g192, byte, compact) found */
  i = check_sync(Fibs, inpfil, &bs_type, &fr_len, &bs_format);

  /* If bitstream is not g192, abort */
  if (bs_format != g192)
    HARAKIRI("Bitstream needs to be in G.192 format. Aborted.\n", 7);

  /* If sync info in file differs from the users' provided
     input, warns and changes to the value found in file */
  if (sync_inp != i)
  {
    fprintf (stderr, "*** Switching sync of input BS from %d to %ld ***\n",
	     sync_inp, i);
    sync_inp = i;
  }

  switch(bs_type)
  {
  case HAS_HEADER:
    /* STL96 -> STL92 conversion */
    inp_type = STL96;
    out_type = STL92;
    /* Define overhead length */
    overhead_inp = OVERHEAD_STL96;
    overhead_out = OVERHEAD_STL92;
    sync_out = 1;
    /* Set Ninp to length of payload + header */
    Ninp = fr_len + (sync_inp ? overhead_inp : 0);
    Nout = fr_len + (sync_out ? overhead_out : 0);
    break;
  case HAS_FLAG_ONLY:
    /* STL92 -> STL96 conversion */
    inp_type = STL92;
    out_type = STL96;
    /* Define overhead length */
    overhead_inp = OVERHEAD_STL92;
    overhead_out = OVERHEAD_STL96;
    sync_out = 2;
    /* Set Ninp to length of payload + header */
    Ninp = fr_len + (sync_inp ? overhead_inp : 0);
    Nout = fr_len + (sync_out ? overhead_out : 0);
    break;
  case NO_HEADER:
    inp_type = raw;
    out_type = STL96;
    /* If input BS is headerless, any frame size will do; use default */
    fr_len = N;
    /* Define overhead length */
    overhead_inp = 0;
    overhead_out = OVERHEAD_STL96;
    /* Set Ninp to length of payload + header */
    Ninp = fr_len;
    Nout = fr_len + overhead_out;
    /* In this particular case, force sync header in the output */
    sync_inp = 0;
    sync_out = 1;
    break;
  default:
    HARAKIRI("Unrecognized bitstream type; check input file\n", 5);
  }

  /* Print info */
  fprintf(stderr, "# Input has  %ld bits/frame %s sync header\n", 
	  fr_len, sync_inp?"with":"without");
  fprintf(stderr, "# Output has %ld bits/frame %s sync header\n",
	  fr_len, sync_out?"with":"without");


  /* The following is a hook for future extensions */
  switch(bs_format)
  {
  case g192:
    size = sizeof(short);
    break;
  case byte:
    size = sizeof(char);
    break;
  case compact:
    size = sizeof(char);
    break;
  }

  /* *** FINAL INITIALIZATIONS *** */

  /* Use the proper data I/O functions; format of output file is the
     same as that of the input file */
  read_data = bs_format==byte? read_byte
              : (bs_format==g192? read_g192 : read_bit_ber);
  save_data = bs_format==byte? save_byte
              : (bs_format==g192? save_g192 : save_bit);


  /* ***** ALLOCATION OF BUFFERS ***** */

  /* Allocate memory for input and output buffers */
  if ((bs_inp = (short *) calloc(Ninp, sizeof(short))) == NULL)
    HARAKIRI("Can't allocate memory for input buffer\n", 10);
  if ((bs_out = (short *) calloc(Nout, sizeof(short))) == NULL)
    HARAKIRI("Can't allocate memory for output buffer\n", 10);


  /* Check if is to process the whole file */
  if (N2 == 0)
  {
    struct stat     st;

    stat(inpfil, &st);
    N2 = ceil((st.st_size - start_byte) / (double)(Ninp * size));
  }


  /* ***** PRINT PRELIMINARY INFORMATION ***** */
  fprintf(stderr, "# Operation: %s -> %s\n",
	  cvt_type(inp_type), cvt_type(out_type));
  fprintf(stderr, "# Bitstream format: %s\n", format_str((int)bs_format));
  fprintf(stderr, "# Input BS header:  %s\n", sync_inp? "present" : "none");
  fprintf(stderr, "# Output BS header: %s\n", sync_out? "present" : "none");
  fprintf(stderr, "# Payload/total frame size (input): %ld / %ld\n",
	  fr_len, fr_len + (sync_inp ? overhead_inp : 0) );
  fprintf(stderr, "# Payload/total frame size (output): %ld / %ld\n",
	  fr_len, fr_len + (sync_out ? overhead_out : 0) );


  /* ***** CARRY OUT CONVERSION ***** */
  
  /* Move file's pointer to 1st desired block (since the BS probing
     has changed the FILE pointer) */
  if (fseek(Fibs, start_byte, 0) < 0l)
    KILL(inpfil, 4);

  /* Inits */
  bitno=0;

  /* Performs the serial-to-parallel conversion */
  for (cur_blk = 0; cur_blk < N2; cur_blk++)
  {
    if (!quiet)
      fprintf(stderr, "\rProcessing block %ld\t", cur_blk+1);

    if ((items = read_data (bs_inp, Ninp, Fibs))<0)
      KILL(inpfil,5);

    /* Treatment */
    if (inp_type == STL92) /* out_type is STL96 */
    {
      /* Copy flag, insert length, copy payload */
      bs_out[0]=bs_inp[0];
      bs_out[1]=fr_len;
      memcpy(&bs_out[2], &bs_inp[1], fr_len * sizeof(short));
    }
    else if (inp_type == STL96) /* out_type is STL92 */
    {
      /* Copy flag, skip length, copy payload */
      bs_out[0]=bs_inp[0];
      memcpy(&bs_out[1], &bs_inp[2], fr_len * sizeof(short));
    }
    else /* headerless BS; save as STL96 BS */
    {
      /* Insert flag and length, copy payload */
      bs_out[0]= 0x6B21;
      bs_out[1]=fr_len;
      memcpy(&bs_out[2], bs_inp, fr_len * sizeof(short));
    }
 
    /* Replace FER, if selected */
    if (!fer && bs_out[0] == 0x6B20)
    {
      bs_out[0] = 0x6B21;
      flags_fixed++;
    }

    /* Save BS to file */
    if ((items = save_data (bs_out, Nout, Fobs))<0)
      KILL(outfil,6);

    /* Update number of bits processed */
    bitno += items - sync_out;
  }


  /* ***** EXITING ***** */

  /* Print info */
  fprintf(stderr, "# Bits saved to output file: %ld\n", bitno);
  if (!fer)
    fprintf(stderr, "# FER flags replaced:        %ld\n", flags_fixed);

  /* Release memory */
  free(bs_out); 
  free(bs_inp);

  /* And close files! */
  fclose(Fobs);
  fclose(Fibs);
  
  /* Return OK when not VMS */
#ifndef VMS
  return(0);
#endif
}
/* ....................... End of main() program ....................... */ 
