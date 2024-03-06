/*                                                            02.Feb.2010 v1.1
  ============================================================================

  IRSDEMO.C
  ~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Example program for testing the correct implementation of the
  IIR IRS filtering without rate conversion using the IIR-IRS module.
  Input signal must be at 8kHz; output signal will be at 8kHz.

  Usage:
  ~~~~~~
  $ IRSDEMO ! ---> HELP text is printed to screen
  or
  $ IRSDEMO [-options] ifile ofile [lseg]
  where:
  ifile: .. INPUT  FILE with short data (binary files)
  ofile: .. OUTPUT FILE with short data (binary files)
  lseg: ... number of samples per processing block
            (default is LSEG0=256)

  Options:
  ~~~~~~~~
  -skip no ... skips saving to file the first `no' processed samples 
  -lseg l .... defines as `l' the number of samples per processing block

  Compilation:
  ~~~~~~~~~~~

  The test program is implemented for segment-wise filtering. To
  test the  dependency of the segment length, the user must enter
  a value for the  segment length (from 1 ... LSEGMAX).

  VAX/VMS:
  $ CC IRSDEMO
  $ link IRSDEMO
  $ IRSDEMO :== $irsdemo_Disk:[irsdemo_Dir]IRSDEMO
  $ IRSDEMO ifile ofile 0 1_2 2_1 133

  The data from file "ifile" are processed in the
  following way, being equivalent to the asynchronous filtering
  needed to simulating the return from digital to analogue
  domains, and the way back:

  1) segments with 133 samples are read from file "ifile"
  2) the first filtering just keep the input data as is;
  3) the second stage will take the data of the previous stage
     as sampled at 8 kHz, filter by the std.PCM and
     upsample to 16 kHz;
  4) the output of the second stage is taken as 16 kHz data,
     which is filtered by the std.PCM filter and down-sampled
     back to 8 kHz.
  5) the output of the third stage is rounded and saved to
     file `ofile'.


  Turbo-C, Turbo-C++:
  > tcc irsdemo
  > irsdemo ifile ofile 1_1 1_1 2_1 1024

  The data from file "ifile" are processed in the following way:
  1) segments with 1024 samples are read from file "ifile"
  2) PCM filtering for 1st stage keeping the rate at 16 kHz;
  3) the same for the second;
  4) the output of the second step is then filtered and
     down-sampled (decimated) to 8 kHz.
  5) the output of the third filter is then saved to file using rounding.


  HighC (MetaWare, version R2.32):
  > hc386 -stack 16384 irsdemo.c
  > Run386 irsdemo ifile ofile 2_1 1_2 0 133

  In our test some C-implementations have shown errors, which
  could be eliminated by increasing the stack size (at compile time).


  SunC (BSD Unix)
  # cc -o irsdemo irsdemo.c
  # irsdemo


  Original author:
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

  History:
  ~~~~~~~~
  22.Sep.1994 v1.0 Created
  02.Feb.2010 v1.1 Modified maximum string length (y.hiwasaki)

  ============================================================================
*/

/*
 * ......... INCLUDES .........
 */

#include <stdio.h>		  /* UNIX Standard I/O Definitions */
#include <stdlib.h>		  /* atoi(), atol() */
#include "ugstdemo.h"		  /* private defines for user interface */
#include "ugst-utl.h"		  /* conversion from float -> short */
#include "iirflt.h"		  /* IRS IIR filtering functions */

/*
 * ......... Definitions for this test program .........
 */

#define LSEG0    256	/* default segment length for segment-wise filtering */
#define LSEGMAX 2048	/* max. number of samples to be proc. */


/*
  ============================================================================

        void display_usage (void);
        ~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Display usage of this demo program and exit;

        Return value:
        ~~~~~~~~~~~~~
        Returns the number of longs read.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        26.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>
        30.Oct.94 v2.0 Revamped <simao@ctd.comsat.com>

 ============================================================================
*/
#define P(x) printf x
void display_usage()
{
  P(("IRSDEMO.C - Version 1.1 of 02.Feb.2010 \n\n"));
 
  P((" Example program for testing the correct implementation of theIIR\n"));
  P((" IRS filtering without rate conversion using the IIR-IRS module.\n"));
  P((" Input signal must be at 8kHz; output signal will be at 8kHz\n"));
  P(("\n"));
  P(("  Usage:\n"));
  P(("  ~~~~~~\n"));
  P(("  $ IRSDEMO ! ---> HELP text is printed to screen\n"));
  P(("  or\n"));
  P(("  $ IRSDEMO [-options] ifile ofile [lseg]\n"));
  P(("  where:\n"));
  P(("  ifile: .. INPUT  FILE with short data (binary files)\n"));
  P(("  ofile: .. OUTPUT FILE with short data (binary files)\n"));
  P(("  lseg: ... number of samples per processing block\n"));
  P(("            (default is LSEG0=%d)\n",LSEG0));
  P(("\n"));
  P(("  Options:\n"));
  P(("  ~~~~~~~~\n"));
  P(("  -skip no ... skip saving to file first `no' processed samples \n"));
  P(("  -lseg l .... set `l' as the no.of samples per processing block\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ...................... End of display_usage() ........................... */



/* ......................... Begin of main() .............................. */
/*
 **************************************************************************
 **************************************************************************
 ***                                                                    ***
 ***        Test-Program for testing the correct implementation         ***
 ***               and to show how to use the programs                  ***
 ***                                                                    ***
 **************************************************************************
 **************************************************************************
*/
int main(argc, argv)
  int             argc;
  char           *argv[];
{
/*
 * ......... Define symbols of type  CASCADE_IIR for each filter .........
 */
  CASCADE_IIR        *typ1_ptr;

  /* ......... signal arrays ......... */
  short           sh_buff[8 * LSEGMAX];	/* 16-bit buffer */
  float           fl_buff[LSEGMAX];	/* float buffer */
  float           buff1[2 * LSEGMAX];	/* output of 1. filter */

  /* ......... File related variables ......... */
  char            inpfil[MAX_STRLEN], outfil[MAX_STRLEN];
  FILE           *inpfilptr, *outfilptr;
#if defined(VMS)
  static char     mrs[15] = "mrs=";
#endif

  /* ......... other auxiliary variables ......... */
  clock_t         t1, t2;	  /* aux. for CPU-time measurement */
  long            lseg=LSEG0, lsegx, lseg1;
  long            noverflows1 = 0;
  long            nsam = 0;
  long skip=0;

  /* ......... PRINT INFOS ......... */

  printf("%s%s", "*** V1.1 DEMO-Program: IRS Up/Down Sampling ",
                 "Filter, with IIR - 30-Sep-1994 ***\n");

/*
 * ......... PARAMETERS FOR PROCESSING .........
 */

  /* GETTING OPTIONS */

  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1], "-skip") == 0)
      {
	/* No reset */
	skip = atoi(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-len") == 0)
      {
	lseg = atoi(argv[2]);
	/* If max.seg.length is exceeded, display warning */
	if (lseg > LSEGMAX)
	{
	  lseg = LSEGMAX;
	  fprintf(stderr, "Warning! lseg limited to max of %ld\n", lseg);
	}

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
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


  /* ......... GETTING PARAMETERS ......... */
#ifdef VMS
  sprintf(&mrs[4], "%d", 2 * 256);/* mrs definition for VMS */
#endif

  GET_PAR_S(1, "_BIN-File to be processed: ............... ", inpfil);
  if ((inpfilptr = fopen(inpfil, RB)) == NULL)
    HARAKIRI("\n   Error opening input file", 1);

  GET_PAR_S(2, "_BIN-Output File: ........................ ", outfil);
  if ((outfilptr = fopen(outfil, WB)) == NULL)
    HARAKIRI("\n   Error opening output file", 1);

  FIND_PAR_L(3, "_Segment Length for Filtering: ........... ", lseg, lseg);
  if (lseg > LSEGMAX)
  {
    /* If max.seg.length is exceeded, display warning */
    lseg = LSEGMAX;
    fprintf(stderr, "Warning! lseg limited to max of %ld\n", lseg);
  }


/*
   * ... INITIALIZE SELECTED IIR-STRUCTURE FOR UP-/DOWNSAMPLING ...
   */

  if ((typ1_ptr = iir_irs_8khz_init()) == 0)
    HARAKIRI("Filter 1: initialization failure iir_irs_8khz()", 1);


/*
   * ......... CARRY OUT FILTERING .........
   */

  /* measure CPU-time */
  t1 = clock();

  lsegx = lseg;
  while (lsegx == lseg)
  {
    /* Read input buffer */
    lsegx = fread(sh_buff, sizeof(short), lseg, inpfilptr);

    /* convert short data to float in normalized range */
    sh2fl_16bit(lsegx, sh_buff, fl_buff, 1);


    /* IIR filtering */
    lseg1 =			  /* Returned: number of output samples */
	cascade_iir_kernel(		  /* cascade form IIR filter */
		      lsegx,	  /* In   : number of input samples */
		      fl_buff,	  /* In   : array with input samples */
		      typ1_ptr,	  /* InOut: pointer to IIR struct */
		      buff1	  /* Out  : array with output samples */
	);

    /* Convert to integer for testing overflows -- do not save! */
    noverflows1 += fl2sh_16bit(lseg1, buff1, sh_buff, (int) 1);

    /* Skip samples if requested */
    if (lseg1 > skip)
    {
      /* Write samples to output file */
      nsam += fwrite(&sh_buff[skip], sizeof(short), (lseg1-skip), outfilptr);
      skip=0;
    }
    else
      skip -= lseg1;
  }


/*
   * ......... FINALIZATIONS .........
   */

  /* Print time statistics - Include file I/O! */
  t2 = clock();
  printf("\nDONE: %f sec CPU-time for %ld generated samples\n",
	 (t2 - t1) / (double) CLOCKS_PER_SEC, nsam);

  /* Print overflow statistics */
  if (noverflows1 == 0)
    printf("\t # NO overflows occurred\n");
  else
  {
    printf("\t # Overflow in %ld samples\n", noverflows1);
  }

  /* Release memory allocated to IIR structures */
  if (typ1_ptr != (CASCADE_IIR *) NULL)
    cascade_iir_free(typ1_ptr);

  /* Close files */
  fclose(outfilptr);
  fclose(inpfilptr);

#ifndef VMS
  return 0;
#endif
}
/* ......................... End of main() ......................... */
