/*                                               Version 2.2 - 01.May.2000
  ========================================================================

  ASTRIP.C
  ~~~~~~~~

  Description:
  ~~~~~~~~~~~~ 
  Program for stripping a segment from a given 16-bit word sample
  file, e.g. a speech file. To smooth the transition, a windowing
  may be applied.

  Usage:
  ~~~~~~
  $ ASTRIP [-options] file1 file2 
           [BlockSize [1stBlock [NoOfBlocks [DataType]]]]]
  where:
   file1       is the first file name;
   file2       is the second file name;
   BlockSize   is the Block size, in samples [default: 256 samples];
   1stBlock    is the starting block [default: 1st block];
   NoOfBlocks  the number of blocks to be segmented [default: all];

  Options:
   -q           operate quietly - only report total no.of differences
   -blk #       number of samples per block [default: 256]
   -start #     starting block (*) or sample to be extracted
   -n #         number of blocks (*) or samples to be extracted from
                file1 (default: all)
   -end #       last block (*) or sample to be extracted from
                file1 (default: last)
   -sample      parameters specified are number of samples instead of
                number of blocks.
   -smooth      applies smoothing to the beginning and end of the 
                file segment being stripped. Default: no smoothing.
                If specified, then Hanning window for WLEN samples
                will be used by default. The default WLEN is defined
                at compilation time, but set "on factory" to 1600. 
   -window #    Parameter for windowing. Default is 0.50 (Hanning).
   -wlen #      Length for window smoothing. "Factory" default is 
                WLEN=1600 samples (100 ms at a 16 kHz sampling rate) 
   (*) indicate that the default option is to work with block numbers,
       rather than with sample numbers.

  Original Author:
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto
  COMSAT Labs                   Tel: +1-301-428-4516
  22300 Comsat Drive            Fax: +1-301-428-4534
  Clarksburg Md 20874 - USA     E-mail: <simao@ctd.comsat.com>

  History:
  ~~~~~~~~
  25/Oct/1995  v1.0  Created
  06/Sep/1996  v2.0  Added window smoothing for start/end segments <simao>
  10/Feb/1999  v2.1  Improved user messages <simao>
  01/May/2000  v2.2  Solved file truncation problem when file size not a 
                     multiple of the block size; solved a problem when
                     in sample mode that the end of the file was NOT
                     being smoothed out <simao.campos@labs.comsat.com>.

  ======================================================================== */
#include "ugstdemo.h"

/* OS definition */
#if defined(__MSDOS__) && !defined(MSDOS)
#define MSDOS
#endif

/* includes in general */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> /* for memset() */
#include <ctype.h>

/* ... Includes for O.S. specific headers ... */
#if defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#elif defined(VMS)
#include <perror.h>
#include <file.h>
#include <stat.h>
#else		       /* Unix */
#include <sys/stat.h>
#include <unistd.h>    /* Specific for read/write/lseek */
#endif

/* Function prototypes */
void display_usage ARGS((void));

/* Global definition */
#define WLEN 1600 /* Window length: 100 ms at 16000 Hz sampling rate */


/*
  ----------------------------------------------------------------------
  Print window type
  Simao -- 10.Feb.99
  ----------------------------------------------------------------------
*/
#define EPS 1E-5
char *window_type(alpha)
double alpha;
{
  if (alpha-0.5<EPS)
    return "Hanning";
  else if (alpha-0.54<EPS) 
    return "Hamming";
  else if (alpha-1.00<EPS)
    return "Rectangular";
  else
    return "Unknown";
}
#undef EPS
/* ........................ End of window_type() ........................ */


/*
 * --------------------------------------------------------------------------
 * ... Display usage of program ...
 *     Simao 
 * --------------------------------------------------------------------------
 */
#define P(x) printf x
void display_usage()
{
  P(("ASTRIP Version 2.2 of 01/May/2000 \n"));
  P((" Program for stripping a segment from a given 16-bit word sample\n"));
  P((" file, e.g. a speech file. Smoothing can be applied to file edges.\n\n"));
  P(("Usage:\n"));
  P(("$ ASTRIP [-options] file1 file2 \n"));
  P(("         [BlockSize [1stBlock [NoOfBlocks [DataType]]]]]\n"));
  P((" where:\n"));
  P(("  file1       is the first file name;\n"));
  P(("  file2       is the second file name;\n"));
  P(("  BlockSize   is the Block size, in samples [default: 256 samples];\n"));
  P(("  1stBlock    is the starting block [default: 1st block];\n"));
  P(("  NoOfBlocks  the number of blocks to be segmented [default: all];\n"));
  P(("\n"));
  P(("Hit <enter> to continue ..."));
  fgetc(stdin);

  P((" Options:\n"));
  P(("  -q           operate quietly, only report total no.of differences\n"));
  P(("  -blk #       number of samples per block [default: 256]\n"));
  P(("  -start #     starting block (*) or sample to be extracted\n"));
  P(("  -n #         number of blocks (*) or samples to be extracted from\n"));
  P(("               file1 (default: all)\n"));
  P(("  -end #       last block (*) or sample to be extracted from\n"));
  P(("               file1 (default: last)\n"));
  P(("  -sample      parameters specified are number of samples instead\n"));
  P(("               of number of blocks.\n"));
  P(("  -smooth      applies smoothing to the beginning and end of the \n"));
  P(("               file segment being stripped. Default: no smoothing.\n"));
  P(("               If specified, then Hanning window for WLEN samples\n"));
  P(("               will be used by default (see -wlen below).\n"));
  P(("  -window #    Parameter for windowing. Default is 0.50 (Hanning).\n"));
  P(("  -wlen #      Length for window smoothing. Default is %ld samples\n", WLEN));
  P(("               (%f ms at a 16 kHz sampling rate)\n", WLEN/16.0));
  P(("  (*) indicates that the default option is to work with block \n"));
  P(("      numbers, rather than with sample numbers.\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ...................... End of display_usage() ..........................*/




/*============================== */
int main(argc, argv)
  int             argc;
  char           *argv[];
/*============================== */
{
  /* segment start/end-smoothing counters */
  long            start_count = 0, end_count = 0;
  /* block size, block/sample counters */
  long            N=256, N1=1, N2=0, count, smpno, tot_samples=0, to_process;
  long            start_byte, samplesize;
  /* smoothing function variables */
  float           w_alpha = 0.50; /* Hanning window's alpha */
  float           w_len = WLEN;   /* No. of samples in the beg/end to smooth */
  float           factor, pi;     /* Constant pi */
  /* sample buffer pointer, to be initialized */
  short          *buf;
  /* file-related variables */
  char            File1[100], File2[100];
  FILE           *fi, *fo;
#ifdef VMS
  char            mrs[15] = "mrs=";
#endif
  /* flags/complementary variables*/
  char            smooth = 0, quiet=0;
  static char     funny[9] = "|/-\\|/-\\";


  /* ......... GET PARAMETERS ......... */

  /* Check options */
  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1],"-blk")==0)
      {
	/* Get skip length */
	N = atol(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-start") == 0)
      {
	/* Define starting block */
	N1 = atol(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-end") == 0)
      {
	/* Define last block */
	N2= atol(argv[2])-N1+1;

	/* Move arg{c,v} over the option to the next argument */
	argc-=2;
	argv+=2;
      }
      else if (strcmp(argv[1], "-n") == 0)
      {
	/* Define number of samples to extract */
	N2= atol(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strncmp(argv[1], "-sample", 7) == 0)
      {
	/* Use sample counts instead of block counts */
	N = 1;
	quiet = 1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-smooth") == 0)
      {
	/* Enable smooth option */
	smooth = 1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;	
      }
      else if (strcmp(argv[1], "-window") == 0)
      {
	/* Define alpha value for window equation */
	/* 0.50 for Hanning window */
	/* 0.54 for Hamming Window */
	/* 1.00 for rectangular window */
	w_alpha = atof(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-wlen") == 0)
      {
	/* Define no. of samples to smooth in the beginning/end of segment */
	w_len= atof(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-") == 0)
      {
	/* No more options; Move arg{c,v} to the next argument and break */
	argc--;
	argv++;
	break;
      }
      else if (strcmp(argv[1], "-q") == 0)
      {
	/* Set quiet compare - only log the total of samples extracted */
	quiet = 1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-?") == 0 || strstr(argv[1], "-help"))
      {
	/* Print help */
	display_usage();
      }
      else
      {
	fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
		argv[1]);
	display_usage();
      }
  }

  /* Read parameters for processing */
  GET_PAR_S(1,  "Full file? ............................ ", File1);
  GET_PAR_S(2,  "Sub-file? ............................. ", File2);
  if (N>1)
  {
    FIND_PAR_L(3, "Block Length? ......................... ", N, N);
    FIND_PAR_L(4, "Starting Record? ...................... ", N1, N1);
    FIND_PAR_L(5, "Number of Records? .................... ", N2, N2);
  }
  else
  {
    /* Use samples counts instead */
    FIND_PAR_L(3, "First Sample? ......................... ", N1, N1);
    FIND_PAR_L(4, "Number of Samples? .................... ", N2, N2);
    N=1;
  }


  /* ......... SOME INITIALIZATIONS ......... */

  /* Get Pi value & window cosine multiplication factor */
  pi = 4 * atan(1.0);
  factor = pi/w_len;

  /* Adjust block/sample start counter: 1st sample is 0, not 1 */
  --N1;

  /* Allocate sample buffer */
  if ((buf = (short *)calloc(N, sizeof(short)))==NULL)
    HARAKIRI("Error allocating memory for data buffer - aborting\n", 5);

  /* Define sample size */
  samplesize = sizeof(short);
  
  /* Define 1st sample to compare */
  start_byte = samplesize * N1 * N;

  /* Check if is to process the whole file */
  if (N2 == 0)
  {
    struct stat     st;

    /* ... find the size of the full file and discount the number */
    /* ... of samples to skip in the beginning of the file ... */
    stat(File1, &st);
    to_process = (st.st_size - start_byte)/samplesize;
    N2 = ceil((st.st_size - start_byte)/ (double)(N * samplesize));
  }
  else
    to_process = N * N2;

  /* Opening test file; abort if there's any problem */
#ifdef VMS
  sprintf(mrs, "mrs=%d", 512l);
#endif

  /* Open input files */
  if ((fi=fopen(File1,RB))==NULL) KILL(File1,3);
  if ((fo=fopen(File2,WB))==NULL) KILL(File2,4);

  /* Move pointer to 1st block of interest */
  if (fseek(fi, start_byte, 0l) < 0l)
    KILL(File1, 3);

  /* Some preliminaries */
  N1++;


  /* Print some user info */
  if (smooth)
  {
    printf("#Using edge smoothing\n");
    printf("#Window: length= %.0f samples; alpha= %.3f (%s window)\n",
	   w_len, w_alpha, window_type(w_alpha));
  }
  else
    printf("#Edge smoothing not used (same as rectangular window)\n");
  if (N==1)
  {
    printf("#Strip operation is sample-based\n#Start sample %ld, ", N1);
    printf("end sample %ld (%ld samples)\n", N1+N2-1, N2);
  }
  else
  {
    printf("#Strip operation is block-based\n#Start block %ld, ", N1);
    printf("end block %ld (%ld blocks)\n", N1+N2-1, N2);
  }


  /* Operates on the file to the screen */
  for (smpno = tot_samples = count=0; count<N2;count++)
  {
    /* Print progress flag */
    if (!quiet)
      fprintf(stderr, "%c\r", funny[count % 8]);

    /* Read samples from input buffer */
    if ((smpno = fread(buf, sizeof(short), N, fi))==0)
      break;
    else if (smpno<0)
      KILL(File1, 2);

    /* ** Apply windowing if necessary in the beg/end of segment */
    /* Window initial samples accounting that tot_samples is */
    /*   updated only in the end of the loop */
    if (smooth)
    {
      if (tot_samples < w_len)
      {
	/* Local temporary variables */
	long n;
	float w_alpha_1 = 1.0 - w_alpha;

	/* Smooth for the whole block or until w_len samples are reached */
	for (n=0; n<N && start_count < w_len; n++, start_count++)
	{
	  buf[n] = (short)((float)buf[n]*
		     (w_alpha - w_alpha_1 * cos((start_count) * factor)));
	}
#ifdef DEBUG
	n = -1; /* Dummy step point for debugging */
#endif
      }
      /* Window final samples accounting that tot_samples is */
      /*   updated only in the end of the loop */
#ifdef ORI_WAY
      else if (tot_samples + smpno > N2* N - w_len + 1)
      {
	/* Local temporary variables */
	long n, abs_n;
	float w_alpha_1 = 1.0 - w_alpha;
	long up_lim = N2 * N - w_len;

	/* Smooth for the remainder of first final block or */
	/*   whole block til end of segment */
	for (n=0, abs_n=up_lim>tot_samples?up_lim:tot_samples;
	     n+1<N;
	     abs_n++, end_count++)
	{
	  n = abs_n % N;
	  buf[n] = (short)((float)buf[n]*
		     (w_alpha - w_alpha_1*cos((w_len + end_count) * factor)));
	}
#else
      else if (tot_samples + smpno > to_process - w_len)
      {
	/* Local temporary variables */
	long n, abs_n;
	float w_alpha_1 = 1.0 - w_alpha;
	long up_lim = to_process - w_len;

	/* Smooth for the remainder of first final block or */
	/*   whole block til end of segment */
	n=0; abs_n=up_lim>tot_samples?up_lim:tot_samples;
        do
	{
	  n = abs_n % N;
	  buf[n] = (short)((float)buf[n]*
			   (w_alpha - w_alpha_1*cos((w_len + end_count +1) * factor)));
	  abs_n++; end_count++;
	} while (n+1<N);
#endif /* ORI_WAY */
#ifdef DEBUG
	n = -1; /* Dummy step point for debugging */
#endif
      }
    }
    tot_samples += fwrite(buf, sizeof(short), smpno, fo);
  }

  /* ..... FINALIZATIONS ..... */
  fprintf(stderr, "> Total %ld samples extracted\n", tot_samples);
  fclose(fi);
  fclose(fo);
  free(buf);
#ifndef VMS
  return(0);
#endif
}
