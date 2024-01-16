/*                                                              V2.4 10.Jun.98
  ============================================================================

  ACTLEVEL.C
  ~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Example program that calculates the active speech level of a file 
  relative to a 0 dB reference. Here, the default is 0 dB below 
  system overload.

  The calculation of speech file's active power uses the algorithm
  in ITU-T Recommendation P.56, and as a by-product of
  this mudule, also  calculates the activity [%], RMS power,
  positive and negative peaks for the file, and peak factors.

  In general, input files are in integer representation,
  16-bit words, 2's complement. In UGST convention, this data must be
  left-adjusted, RATHER than right-adjusted. Since the speech voltmeter
  uses `float' input data, it is necessary to convert from short (in the
  mentioned format) to float; this is carried out by the function
  `sh2fl()'. This function has the option of `normalize' the input data
  to the range -1..+1, what is done. 

  The default values for the AD,DA systems resolution is 16 bits, for
  the sampling rate is 16000 Hz. To change this on-line, just specify
  the proper options (see below) in the command line. For example, 14
  bits and 8000 Hz:

  $ actlevel -blk 256 -star 1 -n 100 -fs 8000 -bits 14 filein

  Usage:
  ~~~~~~
  $ actlevel [-?] (displays the help message message!) 
  or
  $ actlevel [-options] file [file ...]
  where:
  file ....... is the input file to be measured; if not specified, no actions 
               are taken.

  Options: 
  ~~~~~~~~
  -blk len  .. is the block size in number of samples;
               this parameter is optional, and the default is block size
	       of 256 samples;
  -start sb .. define `sb' as the first block to be measured 
               [default: first block of the file] 
  -n nb ...... define `nb' as the number of blocks to be measured 
               [default: whole file]
  -end eb .... define `eb' as the last block to be measured 
               [default: till end of file]
  -sf f ...... sets the sampling rate to `f' Hz [default: 16000Hz]
  -bits n .... sets the digital system resolution (AD,DA systems) to `n', 
               in number of bits [default: 16 bits]
  -lev ndB ... causes the program to CALCULATE the gain necessary to
               equalize the *active* level of the input file(s) to
               ndB. However, the program will NOT level-equalize the file.
  -rms ....... when -lev is selected, reports the gain necessary to be
               applied to the input file(s) to normalizes to the long
               term level, instead of the active speech level.
  -log file .. print the statistics log into file rather than stdout
  -q ......... quiet operation; don't print progress flag, results are
               printed all in one line.


  Modules used:
  ~~~~~~~~~~~~~
  > sv-P56.c:   contains the functions related to active speech
	        level measurement according to P.56,
	        init_speech_voltmeter(), speech_voltmeter() and 
	        bin_interp(). Their prototypesare in `sv-p56.h'.
  > ugst-utl.c: utility functions; here are used the gain/loss
	        (scaling) algorithm of scale() and the data type
		conversion functions sh2fl() and fl2sh(). Prototypes 
		are in `ugst-utl.h'.

  Exit values:
  ~~~~~~~~~~~~
  0      success (all but VMS);
  1      success (only in VMS);
  2      error opening input file;
  3      error creating output file;
  4      error moving pointer to desired start of conversion;
  5      error reading input file;
  6      error writing to file;

  Compilation:
  ~~~~~~~~~~~~
  VaxC:   cc actlevel.c 
          link actlevel
  TurboC: tcc actlevel.c sv-p56.c ugst-utl.c
  Sun-OS: cc -o actlevel actlevel.c sv-p56.c ugst-utl.c -lm 
  gcc:    cc -o actlevel actlevel.c sv-p56.c ugst-utl.c -lm 

  Author: 
  ~~~~~~~ 
  Simao Ferraz de Campos Neto
  DDS/Pr11                      Tel: +55-192-39-1396                
  CPqD/Telebras                 Fax: +55-192-53-4754                
  13085 Campinas SP Brazil      E-mail: <tdsimao@venus.cpqd.ansp.br>

  Log of changes:
  ~~~~~~~~~~~~~~~
  01.Jun.92     1.0        1st version, based on the sv56demo.c version 2.1.
  01.Feb.95     2.0        Added options and multi-file measurement option.
  21.Aug.95     2.1        Fixed bug with zero-sample files.
  06.Dec.96     2.2        Fixed small bug in the calculation of peak 
                           factors: the variable holding the absolute
                           maximum in dB was never initialized,
                           causing a wrong peak factor to be reported
                           when the -q option was *not* used (peak
                           factor wasn't reported with -q) . Took the
                           advantage to add the reporting of the peak
                           factor when option -q is used. Updated
                           display_usage().
  06.Apr.98     2.3        Solved small bug that occurred when the file size 
                           was not a multiple of the frame size. The
                           program was truncating the output file size
                           to a multiple of the current block
                           size. Solved also a bug with long file
                           names which were causing memory invasion by
                           increasing max file name length to 150
                           characters and changing strcpy() to
                           strncpy() in the filename copy process.
                           <simao>
  ============================================================================
*/

/* ... Include for all UGST demos -- should be the 1st include ... */
#include "ugstdemo.h"

/* ... Includes in general ... */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strstr() */
#include <math.h>

/* ... Includes for O.S. specific headers ... */
#if defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#elif defined(VMS)
#include <perror.h>
#include <file.h>
#include <stat.h>
#else
#include <sys/stat.h>
#endif

/* ... Include of speech-voltmeter-related routines ... */
#include "sv-p56.h"

/* ... Include of utilities ... */
#include "ugst-utl.h"

/* ... Local definitions ... */
#define DEF_BLK_LEN 256 /* samples per block */
#define MIN_LOG_OFFSET 1.0e-20 /*To avoid sigularity with log(0.0) */


/* 
  ============================================================================

        void display_usage (void)
        ~~~~~~~~~~~~~~~~~~

        Display usage information and quit program.

        Parameter:
        ~~~~~~~~~~
        None.

        Returns
        ~~~~~~~
        To calling program, nothing; to calling environment, returns OK
        (1 in VAX/VMS or 0, otherwise).

        Original author
        ~~~~~~~~~~~~~~~
        simao@ctd.comsat.com

        Log of changes
        ~~~~~~~~~~~~~~
        01.Feb.94	v1.0	Creation.

  ============================================================================
*/
#define P(x) printf x
void display_usage()
{
  P(("ACTLEVEL.C - Version 2.0 of 01/Feb/1995 \n"));
  P((" Calculate the active speech level of a file, relative to the\n"));
  P((" system overload point [dBov], using the P.56 algorithm.\n"));
  P((" Reports positive and negative peaks, RMS and active level, \n"));
  P((" activity factor [%%], and long-term and active peak factors \n"));
  P((" for the file\n"));
  P(("\n"));
  P((" Usage:\n"));
  P(("  $ actlevel [-options] file [file ...]\n"));
  P((" where:\n"));
  P(("  file ....... is the input file(s) to be measured\n"));
  P((" Options: \n"));
  P(("  -blk len  .. is the block size in number of samples;\n"));
  P(("               this parameter is optional, and the default is  \n"));
  P(("               block size of 256 samples;\n"));
  P(("  -start sb .. define `sb' as the first block to be measured \n"));
  P(("               [default: first block of the file] \n"));
  P(("  -end eb .... define `eb' as the last block to be measured\n"));
  P(("  -n nb ...... define `nb' as the number of blocks to be measured \n"));
  P(("               [default: whole file]\n"));
  P(("  -sf f ...... sets the sampling rate to `f' Hz [default: 16000Hz]\n"));
  P(("  -bits n .... sets the digital system resolution (AD,DA systems) \n"));
  P(("               to `n' bits [default: 16 bits]\n"));
  P(("  -lev ndB ... CALCULATES the gain necessary to equalize the\n"));
  P(("               *active* level of the input file(s) to ndB.\n"));
  P(("               The program will NOT level-equalize the file.\n"));
  P(("  -rms ....... when -lev is selected, reports the gain necessary\n"));
  P(("               to normalizes to the long term level, instead of the\n"));
  P(("               active speech level. Does NOT change the file(s).\n"));
  P(("  -log file ... log statistics into file rather than stdout\n"));
  P(("  -q ......... quiet operation; don't print progress flag, results\n"));
  P(("               are printed all in one line.\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ....................... End of display_usage() .......................... */


/* 
  ============================================================================

       void print_act_long_summary(char *file, SVP56_state state, double al_dB,
       ~~~~~~~~~~~~~~~~~~~~~~~~~~~ double NdB, double gain, double ratio,
                                   long N, long N1, long N2, long bitno);

       Print full summary of P.56 statistics

       Parameter:
       ~~~~~~~~~~
       file ..... File name
       state .... P.56 state variable
       al_dB .... active level in dB
       NdB ...... desired dB level for output
       gain ..... gain to equalize file to desired level
       ratio .... ratio of maximum number representable in the system (based on
                  the number of bits, or resolution, of the input speech file)
		  to the range of the input signal. E.g., for a 16 bit system
		  with a +-1 signal range, ratio = 32760 / 1.0.
       N ........ number of samples per block
       N1 ....... number of first block processed in the file
       N2 ....... total number of blocks processed from the file
       bitno .... number of bits per sample (input signal resolution)

       Returns
       ~~~~~~~
       None

       Original author
       ~~~~~~~~~~~~~~~
       simao@ctd.comsat.com

       Log of changes
       ~~~~~~~~~~~~~~
       01.Nov.96	v1.0	Creation.

  ============================================================================
*/
void print_act_long_summary(out, file, state, al_dB, NdB, ratio, gain,
			    N, N1, N2, bitno)
  FILE *out;
  SVP56_state     state;
  char *file;
  double ratio, gain, al_dB, NdB;
  long N, N1, N2, bitno;
{
  static char     unity[5] = "dBov";
  double          abs_max_dB, max_gain, new_max;

  /* Calculate peak in dB */
  abs_max_dB = 20 * log10(SVP56_get_abs_max(state) + MIN_LOG_OFFSET)
               - state.refdB;

  /* Computes the maximum gain that will not cause saturation */
  max_gain = al_dB - 20 * log10(SVP56_get_abs_max(state));

  /* Print the relevant messages */
  fprintf(out, "%s%s", " ---------------------------",
	  "----------------------------");
  fprintf(out, "\n  Input file: ................... %s, ", file);
  fprintf(out, "%2ld bits, fs=%5.0f Hz", bitno, state.f);
  fprintf(out, "\n  Block Length: ................. %7ld [samples]", N);
  fprintf(out, "\n  Starting Block: ............... %7ld []", N1 + 1);
  fprintf(out, "\n  Number of Blocks: ............. %7ld []", N2);
  if (gain>0)
    fprintf(out, "\n  %s desired for output: ...... %7.3f [%s]",
	    unity, NdB, unity);

  /* Skip if filesize is zero */
  if (state.n==0)
  {
    fprintf(out, "%s%s", "\n -***-----------------------",
	    "----------------------------\n");
    return;
  }

  /* If the activity factor is 0, don't report many things */
  if (SVP56_get_activity(state) == 0)
  {
    fprintf(out, "\n  Activity factor is ZERO -- the file is silence");
    fprintf(out, " or idle noise");
    fprintf(out, "%s%s", "\n ---------------------------",
	    "----------------------------");
    fprintf(out, "\n  DC level: ..................... %7.0f [PCM]", 
	    ratio * SVP56_get_DC_level(state));
    fprintf(out, "\n  Maximum positive value: ....... %7.0f [PCM]", 
	    ratio * SVP56_get_pos_max(state));
    fprintf(out, "\n  Maximum negative value: ....... %7.0f [PCM]", 
	    ratio * SVP56_get_neg_max(state));
    fprintf(out, "%s%s", "\n ---------------------------",
	    "----------------------------");
    fprintf(out, "\n  Noise/silence energy (rms): ... %7.3f [dB]", 
	    SVP56_get_rms_dB(state));
  }
  else
  {
    if (gain>0)
    {
      fprintf(out, "\n  Norm factor desired is: ....... %7.3f [times]", gain);
      fprintf(out, "\n  Max norm WITHOUT saturation: .. %7.3f [%s]",
	      max_gain, unity);
    }

    fprintf(out, "%s%s", "\n ---------------------------",
	    "----------------------------");
    fprintf(out, "\n  DC level: ..................... %7.0f [PCM]", 
	    ratio * SVP56_get_DC_level(state));
    fprintf(out, "\n  Maximum positive value: ....... %7.0f [PCM]", 
	    ratio * SVP56_get_pos_max(state));
    fprintf(out, "\n  Maximum negative value: ....... %7.0f [PCM]", 
	    ratio * SVP56_get_neg_max(state));
    fprintf(out, "%s%s", "\n ---------------------------",
	    "----------------------------");
    fprintf(out, "\n  Long term energy (rms): ....... %7.3f [%s]", 
	    SVP56_get_rms_dB(state), unity);
    fprintf(out, "\n  Active speech level: .......... %7.3f [%s]",
	   al_dB, unity);
    fprintf(out, "\n  RMS peak-factor found: ........ %7.3f [dB]",
	    abs_max_dB - SVP56_get_rms_dB(state));
    fprintf(out, "\n  Active peak factor found: ..... %7.3f [dB]",
	    abs_max_dB - al_dB);
    fprintf(out, "\n  Activity factor: .............. %7.3f [%%]",
	    SVP56_get_activity(state));
  }
  fprintf(out, "%s%s", "\n ---------------------------",
	  "----------------------------");

  /* Tests if the equalization will cause saturation */
  new_max = SVP56_get_abs_max(state) * gain;
  if (gain > 0 && new_max > 1.0)
  {
    /* Print message */
    fprintf(out, "\n%%SV-W-SAT, the dB level chosen causes SATURATION: ");
    fprintf(out, "old max=%5.0f; new max=%6.0f",
	    ratio * SVP56_get_abs_max(state), 
	    ratio * new_max);
    fprintf(out, "\n%%SV-I-MAXLEVDB, the maximum norm factor ");
    fprintf(out, "to PREVENT clipping is %7.3fdB; ", max_gain);
    fprintf(out, "\n -------------------------------------------------------");
  }
}
/* .................... End of print_act_long_summary() ................... */


/* 
  ============================================================================

       void print_act_short_summary (char *file, SVP56_state state,
       ~~~~~~~~~~~~~~~~~~~~~~~~  double al_dB, double ratio);

       Print one-line summary of P.56 statistics

       Parameter:
       ~~~~~~~~~~
       file ..... File name
       state .... P.56 state variable
       al_dB .... active level in dB
       ratio .... ratio of maximum number representable in the system (based on
                  the number of bits, or resolution, of the input speech file)
		  to the range of the input signal. E.g., for a 16 bit system
		  with a +-1 signal range, ratio = 32760 / 1.0.

       Returns
       ~~~~~~~
       None

       Original author
       ~~~~~~~~~~~~~~~
       simao@ctd.comsat.com

       Log of changes
       ~~~~~~~~~~~~~~
       01.Nov.96	v1.0	Creation.

  ============================================================================
*/
void print_act_short_summary(out, file, state, al_dB, ratio, gain)
  FILE *out;
  SVP56_state     state;
  char *file;
  double ratio, al_dB, gain;
{
  double          abs_max_dB;

  /* Calculate peak in dB */
  abs_max_dB = 20 * log10(SVP56_get_abs_max(state) + MIN_LOG_OFFSET)
               - state.refdB;

  /* Report number of samples */
  fprintf(out, "Samples: %5ld ", state.n);

  /* Skip if filesize is zero */
  if (state.n==0)
  {
    fprintf(out, "%s%s%s%s", "Min: ------ Max: ----- DC: ------- ",
	    "RMSLev[dB]: ------- ActLev[dB]: ------- %Active:  ------ ",
	    "RMSPkF[dB]: ------- ActPkF[dB]: -------",
	    gain>0? "Gain:  ------ ":"");
  }
  else
  {
    fprintf(out, "Min: %5.0f ",        ratio * state.maxN);
    fprintf(out, "Max: %05.0f ",       ratio * state.maxP);
    fprintf(out, "DC: %7.2f ",         ratio * state.DClevel);
    fprintf(out, "RMSLev[dB]: %7.3f ", state.rmsdB);
    fprintf(out, "ActLev[dB]: %7.3f ", al_dB);
    fprintf(out, "%%Active: %7.3f ",   state.ActivityFactor * 100);
    fprintf(out, "RMSPkF[dB]: %7.3f ", abs_max_dB - SVP56_get_rms_dB(state));
    fprintf(out, "ActPkF[dB]: %7.3f",  abs_max_dB - al_dB);
    if (gain>0)
      fprintf(out, " Gain[]: %7.3f",    gain);
  }
  fprintf(out, "\t%s\n", file);
}
/* ................... End of print_act_short_summary() .................... */


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

  /* Parameters for operation */
  double          Overflow;	/* Max.positive value for AD_resolution bits */
  long            N=DEF_BLK_LEN, N1=1, N2=0, N2_ori, i, l;

  /* Intermediate storage variables for speech voltmeter */
  SVP56_state     state;
#ifdef LOCAL_PRINT
  double          abs_max_dB;
#endif

  /* File-related variables */
  char            FileIn[150];
  FILE           *Fi;	/* input file pointer */
  FILE           *out=stdout;   /* where to print the statistical results */
#ifdef VMS
  char            mrs[15];
#endif

  /* Other variables */
  short           buffer[4096];
  float           Buf[4096];
  long            start_byte, bitno = 16;
  double          sf=16000; /* Hz */
  double          ActiveLeveldB, level=0, gain=0;
  static char     funny[] = "|/-\\|/-\\", funny_size = sizeof(funny), quiet=0;
#ifdef LOCAL_PRINT
  static char     unity[5] = "dBov";
#endif
  char            use_active_level = 1;

  /* ......... GET PARAMETERS ......... */
 
  /* Getting options */
  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1], "-sf") == 0)
      {
	/* Change default sampling frequency */
	sf = atof(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-lev") == 0)
      {
	/* Set a desired level */
	level = atof(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-rms") == 0)
      {
	/* Use the RMS level to normalize instead of the active level */
	use_active_level = 0;

	/* Update argc/argv to next valid option/argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-bits") == 0)
      {
	/* Change default sampling frequency */
	bitno = atoi(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-blk") == 0)
      {
	/* Change default sampling frequency */
	N = atoi(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-start") == 0)
      {
	/* Change default sampling frequency */
	N1 = atoi(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-n") == 0)
      {
	/* Change default sampling frequency */
	N2 = atoi(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-end") == 0)
      {
	/* Change number of blocks based on the last block */
	N2 = atol(argv[2]) - N1 + 1;

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-log") == 0)
      {
	/* Log statistics into a file */
	if ((out=fopen(argv[2], WT)) == NULL)
	  KILL(argv[2], 2);
	else
	  fprintf(stderr, "Statistics will be logged in %s\n", argv[2]);

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
      else if (strcmp(argv[1],"--")==0)
      {
	/* No more options: */
	/* Move argv over the option to the next argument and quit loop */
	argv++;
	argc--;
	break;
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


  /* ......... SOME INITIALIZATIONS ......... */
  /* funny_size = strlen(funny); */
  start_byte = --N1;
  start_byte *= N * sizeof(short);
  N2_ori = N2;

  /* Overflow (saturation) point */
  Overflow = pow((double)2.0, (double)(bitno-1));


  /* REPEAT FOR ALL FILES IN THE COMMAND LINE */
  while(argc>1)
  {
    /* Get new file name and update argument line pointer/counter */
    strncpy(FileIn, argv[1], sizeof(FileIn));
    argv++;
    argc--;
    
    /* Reset variables for speech level measurements */
    init_speech_voltmeter(&state, sf);

    /* ......... FILE PREPARATION ......... */

    /* Opening input file; abort if there's any problem */
#ifdef VMS
    sprintf(mrs, "mrs=%d", 2 * N);
#endif
    if ((Fi = fopen(FileIn, RB)) == NULL)
      KILL(FileIn, 2);

    /* Reinitialize number of blocks as specified initially */
    N2 = N2_ori;

    /* Check if is to process the whole file */
    if (N2 == 0)
    {
      struct stat st;
      stat(FileIn, &st);
      N2 = ceil(st.st_size / (double)(N * sizeof(short)));
    }

    /* Move pointer to 1st block of interest */
    if (fseek(Fi, start_byte, 0) < 0l)
      KILL(FileIn, 4);


    /* ... MEASUREMENT OF ACTIVE SPEECH LEVEL ACCORDING P.56 ... */

    /* Read samples ... */
    if (!quiet)
      fprintf(stderr, "  Processing \r");
    for (i = 0; i < N2; i++)
    {
      if ((l = fread(buffer, sizeof(short), N, Fi)) > 0)
      {
	/* ... Convert samples to float */
	sh2fl((long) l, buffer, Buf, bitno, 1);

	/* ... Get the active level */
	ActiveLeveldB = speech_voltmeter(Buf, (long) l, &state);

	/* Print progress flag */
	if (!quiet) 
	  fprintf(stderr, "%c\r", funny[i % funny_size]);
      }
      else
      {
	KILL(FileIn, 5);
      }
    }
    if (!quiet) 
      fprintf(stderr, "\n");

#ifdef LOCAL_PRINT
    /* Convert absolute maximum sample to dB */
    abs_max_dB = 20 * log10(SVP56_get_abs_max(state)) - state.refdB;

    /* ... PRINT-OUT OF RESULTS ... */
    if (!quiet) 
    {
      fprintf(stderr, "%s%s", " ---------------------------",
	                      "----------------------------");
      fprintf(stderr, "\n  Input file: ................... %s, ", FileIn);
      fprintf(stderr, "%2ld bits, fs=%5.0f Hz", bitno, sf);
      fprintf(stderr, "\n  Block Length: ................. %7ld [samples]", N);
      fprintf(stderr, "\n  Starting Block: ............... %7ld []", N1 + 1);
      fprintf(stderr, "\n  Number of Blocks: ............. %7ld []", N2);

      /* Skip if filesize is zero */
      if (state.n==0)
      {
	fprintf(stderr, "%s%s", "\n -***-----------------------",
	                        "----------------------------\n");
	continue;
      }

      /* If the activity factor is 0, don't report many things */
      if (SVP56_get_activity(state) == 0)
      {
	fprintf(stderr, "\n  Activity factor is ZERO -- the file is silence");
	fprintf(stderr, " or idle noise");
	fprintf(stderr, "%s%s", "\n ---------------------------",
	                        "----------------------------");
	fprintf(stderr, "\n  DC level: ..................... %7.0f [PCM]", 
		Overflow * SVP56_get_DC_level(state));
	fprintf(stderr, "\n  Maximum positive value: ....... %7.0f [PCM]", 
		Overflow * SVP56_get_pos_max(state));
	fprintf(stderr, "\n  Maximum negative value: ....... %7.0f [PCM]", 
		Overflow * SVP56_get_neg_max(state));
	fprintf(stderr, "%s%s", "\n ---------------------------",
	                        "----------------------------");
	fprintf(stderr, "\n  Noise/silence energy (rms): ... %7.3f [dB]", 
		SVP56_get_rms_dB(state));
      }
      else
      {
	fprintf(stderr, "%s%s", "\n ---------------------------",
	                        "----------------------------");
	fprintf(stderr, "\n  DC level: ..................... %7.0f [PCM]", 
		Overflow * SVP56_get_DC_level(state));
	fprintf(stderr, "\n  Maximum positive value: ....... %7.0f [PCM]", 
		Overflow * SVP56_get_pos_max(state));
	fprintf(stderr, "\n  Maximum negative value: ....... %7.0f [PCM]", 
		Overflow * SVP56_get_neg_max(state));
	fprintf(stderr, "%s%s", "\n ---------------------------",
	                        "----------------------------");
	fprintf(stderr, "\n  Long term energy (rms): ....... %7.3f [%s]", 
		SVP56_get_rms_dB(state), unity);
	fprintf(stderr, "\n  Active speech level: .......... %7.3f [%s]",
		ActiveLeveldB, unity);
	fprintf(stderr, "\n  RMS peak-factor found: ........ %7.3f [dB]",
		abs_max_dB - SVP56_get_rms_dB(state));
	fprintf(stderr, "\n  Active peak factor found: ..... %7.3f [dB]",
		abs_max_dB - ActiveLeveldB);
	fprintf(stderr, "\n  Activity factor: .............. %7.3f [%%]",
		SVP56_get_activity(state));
      }
      fprintf(stderr, "%s%s", "\n ---------------------------",
		              "----------------------------\n");
    }
    else
    {
      printf("Samples: %5ld ", state.n);

      /* Skip if filesize is zero */
      if (state.n==0)
      {
	printf("%s%s", "Min: ------ Max: ----- DC: ------- ",
	       "RMSLev[dB]: ------- ActLev[dB]: ------- %Active:  ------");
      }
      else
      {
	printf("Min: %-5.0f ",       Overflow * state.maxN);
	printf("Max: %-5.0f ",       Overflow * state.maxP);
	printf("DC: %-7.2f ",        Overflow * state.DClevel);
	printf("RMSLev[dB]: %7.3f ", state.rmsdB);
	printf("ActLev[dB]: %7.3f ", ActiveLeveldB);
	printf("%%Active: %7.3f ",    state.ActivityFactor * 100);
	printf("RMSPkF[dB]: %7.3f ",  abs_max_dB - SVP56_get_rms_dB(state));
 	printf("ActPkF[dB]: %7.3f",  abs_max_dB - ActiveLeveldB);
     }
      printf("\t%s\n", FileIn);
    }
#else
    if (level!=0)
    {
      /* Computes the equalization factor to be used in the output file */
      if (use_active_level)
	gain = pow(10.0, (level-ActiveLeveldB) / 20.0);
      else
	gain = pow(10.0, (level-SVP56_get_rms_dB(state)) / 20.0);
    }

    /* ... PRINT-OUT OF RESULTS ... */
    if (!quiet)
      print_act_long_summary(out, FileIn, state, ActiveLeveldB, level,
			     Overflow, gain, N, N1, N2, bitno);
    else
      print_act_short_summary(out, FileIn, state, ActiveLeveldB, 
			      Overflow, gain);
#endif /* LOCAL_PRINT */

    /* Close current file */
    fclose(Fi);
  }

  /* FINALIZATIONS */
  /* ... Close log file, if it is the case */
  if (out != stdout)
    fclose(out);

  /* ... Exit cleanly */
#if !defined(VMS)
  return (0);
#endif
}
