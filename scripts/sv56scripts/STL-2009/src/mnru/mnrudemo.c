/*                                                Version: 2.2 - 02.Feb.2010
  --------------------------------------------------------------------------

  MNRUDEMO.C
  ~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Demonstration program for generating files with modulated noise
  added based on UGST's MNRU module, which is based in the ITU-T
  Recommendation P.81 (September/1995).

  Usage:
  ~~~~~~
  $ MNRUDEMO (no parameters) --> to display a help message,
  or
  $ MNRUDEMO [-options] filein fileout blk 1stblk blkno desiredQ [mode]

  where:
  filein          input filename; samples are supposed
                  to be in the 16 bit, 2-complement format.
  fileout         output filename; samples are saved
                  in the 16 bit, 2-complement format.
  blk             block-size, in samples [default: 256 samples/block]
  1stblk          number of the first block to process [default: 1st]
  blkno           total blocks to process [default: all]
  desiredQ        is the signal-to-modulated-noise ratio desired
                  for the output file' samples; supposed to be
                  in dB [default is 100dB].
  mode            is the operation mode of the MNRU: signal-
                  only, noise-only, or modulated-noise. If not
                  specified, the default is modulated-noise.
  Options:
  ~~~~~~~~
  -q              quiet operation; don't print progress info
  -Q xdB          define Q value as xdB [default: 100dB]
  -noise          define MNRU mode as noise-only
  -signal         define MNRU mode as signal-only
  -mod            define MNRU mode as modulated noise (default)

  History:
  ~~~~~~~~
  25.Sep.1991  1.0F Release of Fortran Version to UGST
  23.Jan.1992  1.0C Compatibility of Fortran and C versions
  27.Jan.1992  1.1  Modular version
  18.May.1992  1.2  Use of `sh2fl' and `fl2sh' with float data 
                    in the normalized range <tdsimao@venus.cpqd.ansp.br>
  01.Feb.1995  2.0  Options and help added <simao@ctd.comsat.com>
  21.Mar.2000  2.1  changed memory alllocation of floating point buffers
                    in[] and out[] from static to dynamic, to prevent
                    memory invasion when block sizes larger than 256
                    are specified. <simao.campos@labs.comsat.com>
  02.Feb.2010  2.2  Modified maximum string length, implicit casting of
                    toupper() argument removed (y.hiwasaki)
  --------------------------------------------------------------------------
*/

#include "ugstdemo.h"		/* defs. for UGSTdemo programs; always is 1st
				 * include */

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for str...() */
#include <ctype.h>   /* for toupper() */
#include <math.h>

#if defined(VMS)
#include <stat.h>
#else
#include <sys/stat.h>
#endif

/* ... Include MNRU module ... */
#include "mnru.h"

/* ... Include of utilities ... */
#include "ugst-utl.h"


/*
 -------------------------------------------------------------------------
 void display_usage(void);
 ~~~~~~~~~~~~~~~~~~
 Display proper usage for the demo program. Generated automatically from
 program documentation.

 History:
 ~~~~~~~~
 01.Feb.95 v1.0 Created.
 -------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage()
{
  P(("MNRU.C - Version 2.2 of 02.Feb.2010 \n"));
  P(("Demonstration program for generating files with modulated\n"));
  P(("noise added based on UGST's MNRU module, which is based in the\n"));
  P(("Recommendation P.81 (Blue Book).\n"));
  P(("\n"));
  P(("Usage:\n"));
  P(("$ MNRUDEMO (no parameters) --> to display this help message,\n"));
  P(("or\n"));
  P(("$ MNRUDEMO [-options] filin filout blk 1stblk blkno desiredQ [mode]\n"));
  P(("\n"));
  P(("where:\n"));
  P((" filin      input filename [sample format: 16 bit, 2-complement]\n"));
  P((" filout     output filename [sample format: 16 bit, 2-complement]\n"));
  P((" blk        block-size, in samples [default: 256 samples/block]\n"));
  P((" 1stblk     number of first block to process [default: 1st]\n"));
  P((" blkno      total of blocks to process [default: all]\n"));
  P((" desiredQ   desired signal-to-modulated-noise ratio in dB [def=100]\n"));
  P((" mode       MNRU operation mode: `S'ignal-only, `N'oise-only,\n"));
  P(("            or `M'odulated-noise [default: modulated-noise]\n"));

  P(("Options:\n"));
  P((" -q         quiet operation; don't print progress info\n"));
  P((" -Q xdB     define Q value as xdB [default: 100dB]\n"));
  P((" -noise     define MNRU mode as noise-only\n"));
  P((" -signal    define MNRU mode as signal-only\n"));
  P((" -mod       define MNRU mode as modulated noise (default)\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* .................... End of display_usage() ........................... */



/*
   **************************************************************************
   ***                                                                    ***
   ***        Demo-Program for testing the correct implementation         ***
   ***               and to show how to use the programs                  ***
   ***                                                                    ***
   **************************************************************************
*/
int main(argc, argv)
  int             argc;
  char           *argv[];
{
  /* DECLARATIONS */

/* File variables */
  char            FileIn[80], FileOut[80];
  FILE           *Fi, *Fo;
  int             fhi, fho;
#ifdef VMS
  char            mrs[15];
#endif

/* Algorithm variables */
  MNRU_state      state;

  short           *Buf;
  float           *inp, *out;
  double          QdB=100; /* defaults to a high value */
  long            cur_frame, l, N, N1, N2;
  char            MNRU_mode=MOD_NOISE, operation;
  long            size, over=0;
  char quiet=0;
  long start_byte;


  /* PREAMBLE */
  fprintf(stderr, "Compiling options: \n\t%s\n\t%s\n\t%s\n\n",
#ifdef STL92_RNG
	  "- Using STL92 random number generator",
#else
	  "- Using new random number generator",
#endif
#ifdef NO_DC_REMOVAL
	  "- DC-removal filter disabled",
#else
	  "- DC-removal filter enabled",
#endif
#ifdef NO_OUT_FILTER
	  "- No output filter used");
#else
	  "- Low-pass filter in the output");
#endif

  /* Getting options */

  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1], "-Q") == 0)
      {
	/* No reset */
	QdB = atof(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-q") == 0)
      {
	/* Don't print progress indicator */
	quiet = 1;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
      }
      else if (strncmp(argv[1], "-noise",6) == 0)
      {
	/* Noise-only mode */
        MNRU_mode = NOISE_ONLY;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
      }
      else if (strncmp(argv[1], "-signal",7) == 0)
      {
	/* Signal-only mode */
        MNRU_mode = SIGNAL_ONLY;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
      }
      else if (strncmp(argv[1], "-mod",4) == 0)
      {
	/* Modulated noise, the default mode */
	MNRU_mode = MOD_NOISE;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "-help") == 0)
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

  /* Now get regular parameters */
  GET_PAR_S(1,  "_Input File: ................. ", FileIn);
  GET_PAR_S(2,  "_Output File: ................ ", FileOut);
  FIND_PAR_L(3, "_Block Length: ............... ", N, 256);
  FIND_PAR_L(4, "_Start Block: ................ ", N1, 1);
  FIND_PAR_L(5, "_No. of Blocks: .............. ", N2, 0);
  FIND_PAR_D(6, "_Desired Q: .................. ", QdB, QdB);

  /* Check for parameter 7 to change MNRU operation mode - OBSOLETE! */
  if (argc > 7)
  {
    MNRU_mode = toupper((int)argv[7][0]);
    if (MNRU_mode == 'S')	/* Signal-only mode */
      MNRU_mode = SIGNAL_ONLY;
    else if (MNRU_mode == 'M')	/* Modulated noise, the default mode */
      MNRU_mode = MOD_NOISE;
    else if (MNRU_mode == 'N')	/* Noise-only mode */
      MNRU_mode = NOISE_ONLY;
    else
    {
      fprintf(stderr, "Bad mode chosen; use M,N,or S \n");
      exit(2);
    }
  }

  /* Inform mode under use */
  fprintf(stderr, "MNRU mode is ");
  if (MNRU_mode == MOD_NOISE)
    fprintf(stderr, "Modulated Noise\n");
  else if (MNRU_mode == SIGNAL_ONLY)
    fprintf(stderr, "Signal-only\n");
  else if (MNRU_mode == NOISE_ONLY)
    fprintf(stderr, "Noise only\n");

  /* Find starting byte in file */
  start_byte = sizeof(short) * (long) (--N1) * (long) N;

  /* Check if is to process the whole file */
  if (N2 == 0)
  {
    struct stat     st;

    /* ... find the input file size ... */
    stat(FileIn, &st);
    N2 = (st.st_size - start_byte) / (N * sizeof(short));
  }

  /* Allocate memory for data vectors */
  if ((inp=(float *)calloc(N,sizeof(float)))==NULL)
    KILL("Error allocating input buffer\n",10);
  if ((out=(float *)calloc(N,sizeof(float)))==NULL)
    KILL("Error allocating output buffer\n",10);

  /* Opening input file; abort if there's any problem */
#ifdef VMS
  sprintf(mrs, "mrs=%d", 2 * N);
#endif
  if ((Fi = fopen(FileIn, RB)) == NULL)
    KILL(FileIn, 2);
  fhi = fileno(Fi);

  /* Creates output file */
  if ((Fo = fopen(FileOut, WB)) == NULL)
    KILL(FileOut, 3);
  fho = fileno(Fo);

  /* Move pointer to 1st block of interest */
  if (fseek(Fi, start_byte, 0) < 0l)
    KILL(FileIn, 4);

  /* INSERTION OF MODULATED NOISE ACCORDING TO P.810 */

  size = N;

  if ((Buf = (short *) calloc(size, sizeof(short))) == NULL)
    KILL("Can't allocate memory for intermediate data buffer\n", 10);


  /* Read samples ... */
  for (cur_frame = 0; cur_frame < N2; cur_frame++)
  {
    /* Read samples ... */
    if ((l = fread(Buf, sizeof(short), N, Fi)) < 0)
      KILL(FileIn, 5);

    /* Information on processing phase */
    if (!quiet) 
      fprintf(stderr, "\rProcessing frame %ld of %ld\t", cur_frame + 1, N2);

    /* Convert data from short to float */
      sh2fl_16bit((long) l, Buf, inp, 1);

    /* Choose operation mode: START, CONTINUE, or STOP */
    if (cur_frame == 0)
      operation = MNRU_START;
    else if (cur_frame == N2 - 1)
      operation = MNRU_STOP;
    else
      operation = MNRU_CONTINUE;

    /* MNRU processing */
    MNRU_process(operation, &state, inp, out, (long) l,
		 (long) 314159265, MNRU_mode, QdB);

    /* Convert from float to short with hard clip and truncation */
    over += fl2sh_16bit((long) l, out, Buf, 1);

    /* Save data to file */
    if ((l = fwrite(Buf, sizeof(short), l, Fo)) <= 0)
      KILL(FileOut, 4);
  }


  /*
   * ........ FINALIZATIONS .........
   */
  fprintf(stderr, "\nOverflow samples: %ld", over);
  fprintf(stderr, "\nClipped noise samples: %ld", state.clip);
  fprintf(stderr, "\n");
  fclose(Fi);
  fclose(Fo);
#ifndef VMS
  return (0);
#endif
}
