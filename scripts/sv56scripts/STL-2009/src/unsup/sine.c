/*                                                         21.Mar.2000 v1.2
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  SINE.C

  Description:
  ~~~~~~~~~~~~
  Program for generating a file with a tone, given the amplitude, the
  frequency, the sampling rate, the DC level and the phase.  In this
  implementation, a 16 bit resolution A/D,D/A system is the default
  (changeable by the optional parameter #9)

  Usage:
  ~~~~~~
  $ sine [-options] file
         [BlockSize [NoOfBlocks [ACampl [f [fs [DCampl [phase [res]]]]]]]]
  where:
  file ........ is the file name;
  BlockSize ... is the Block size, in samples;
  NoOfBlocks .. the number of blocks to be displayed;
  ACampl ...... AC amplitude of the tone; if positive, it is understood
                as the PCM value; if negative, it considers the value
                given as a dB value. So, if it is -20, the program
                will generate a tone with AC amplitude equal to -20
                dB, ie, 1638 in PCM;
  f ........... tone frequency, in Hz;
  fs .......... sampling frequency, in Hz;
  DCampl ...... DC level of the tone, in PCM;
  phase	....... the desired phase for the tone, in degrees;
  res ......... digital system resolution; if not specified, the program
                assumes 16 bits;

  Options:
  ~~~~~~~~
  -dB ........... AC level is given in dB [default: linear PCM]
  -ac ........... define AC tone level, in dB or PCM
  -f ............ define tone frequency in Hertz 
  -dc ........... define DC level for tone, as a linear PCM value [default: 0]
  -sf ........... define sampling frequency in Hertz [default: 8000 Hz]
  -phase ........ set phase in degrees for tone [default: 0 degrees]
  -blk .......... define file block size [default: 256]
  -n ............ define no.of blocks to generate
  -res .......... define no.of bits per sample [default: 16]
  -q ............ quiet operation
  -? or -help ... display on-line help message

  Author:
  ~~~~~~~
  Simao Ferraz de Campos Neto -- CPqD/Telebras

  History:
  ~~~~~~~~
  08.Mar.1990 v1.0 created
  07.Nov.1997 v1.1 command-line options added; PCM<->dB conversions 
                   corrected, in particular for tones (sqrt(2) factor
		   between level & peak). <simao>
  21.Mar.2000 v1.2 changed memory allocation of short buffer
                   data[] from static to dynamic, to prevent
		   memory invasion when block sizes larger than
		   256 are specified. <simao.campos@labs.comsat.com>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/* OS definition */
#ifndef MSDOS
#if defined(__MSDOS__) || defined (__CYGWIN__) || defined (_MSC_VER)
# define MSDOS
#endif
#endif

/* includes in general */
#include "ugstdemo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* includes for OS specific directives */
#if defined(MSDOS)         /* ... DOS ... */
#include <fcntl.h>
#include <io.h>
#elif defined(VMS)         /* ... VMS ... */
#include <stdlib.h>
#include <perror.h>
#include <file.h>
#else /* ... others ... */
#include <unistd.h>
#endif

#ifdef USE_FUNCTION
#define refdB refdB_f
#define v_to_dB(x) v_to_dB_f(x)

double refdB_f=0;

double v_to_dB_f(double x)
{
  return (20.0*log10(x) - refdB);
}
#else
#define v_to_dB(x) (20.0*log10(x)-refdB)
#endif

/* Pseudo-function definitions for the program */
#define TWO_PI (8*atan(1.0))
#define DEGREE_TO_RADIAN(x) (x*atan(1.0)/45.0)
#define dB_to_v(x) (pow(10.0,(x+refdB)/10.0))


/*
  --------------------------------------------------------------------------
  display_usage()

  Shows program usage.

  History:
  ~~~~~~~~
  7/Nov/1997  v1.0 Created <simao>
  --------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage()
{
  P(("Version 1.2 of 22/Mar/2000 \n\n"));
 
  P(("  SINE.C\n"));
  P(("\n"));
  P(("  Program for generating a file with a tone, given the amplitude, the\n"));
  P(("  frequency, the sampling rate, the DC level and the phase.  In this\n"));
  P(("  implementation, a 16 bit resolution A/D,D/A system is the default\n"));
  P(("  (changeable by the optional parameter #9)\n"));
  P(("\n"));
  P(("  Usage:\n"));
  P(("  ~~~~~~\n"));
  P(("  $ sine [-options] file\n"));
  P(("         [BlockSize [NoOfBlocks [ACampl [f [fs [DCampl [phase [res]]]]]]]]\n"));
  P(("  where:\n"));
  P(("  file ........ is the file name;\n"));
  P(("  BlockSize ... is the Block size, in samples;\n"));
  P(("  NoOfBlocks .. the number of blocks to be displayed;\n"));
  P(("  ACampl ...... AC amplitude of the tone; if positive, it is understood\n"));
  P(("                as the PCM value; if negative, it considers the value\n"));
  P(("                given as a dB value. So, if it is -20, the program\n"));
  P(("                will generate a tone with AC amplitude equal to -20\n"));
  P(("                dB, ie, 1638 in PCM;\n"));
  P(("  f ........... tone frequency, in Hz;\n"));
  P(("  fs .......... sampling frequency, in Hz;\n"));
  P(("  DCampl ...... DC level of the tone, in PCM;\n"));
  P(("  phase	....... the desired phase for the tone, in degrees;\n"));
  P(("  res ......... digital system resolution; if not specified, the program\n"));
  P(("                assumes 16 bits;\n"));
  P(("\n"));
  P(("  Options:\n"));
  P(("  ~~~~~~~~\n"));
  P(("  -dB ........... AC level is given in dB [default: linear PCM]\n"));
  P(("  -ac ........... define AC tone level, in dB or PCM\n"));
  P(("  -f ............ define tone frequency in Hertz \n"));
  P(("  -dc ........... define DC level for tone, as a linear PCM value [default: 0]\n"));
  P(("  -sf ........... define sampling frequency in Hertz [default: 8000 Hz]\n"));
  P(("  -phase ........ set phase in degrees for tone [default: 0 degrees]\n"));
  P(("  -blk .......... define file block size [default: 256]\n"));
  P(("  -n ............ define no.of blocks to generate\n"));
  P(("  -res .......... define no.of bits per sample [default: 16]\n"));
  P(("  -q ............ quiet operation\n"));
  P(("  -? or -help ... display on-line help message\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ..................... End of display_usage() ......................... */

/* ********************************************************************** */
/* ********************************************************************** */
/* ************************* MAIN PROGRAM ******************************* */
/* ********************************************************************** */
/* ********************************************************************** */

int main(argc,argv)
int argc;
char *argv[];
{
  int 	i,j,n;
  double t;
#ifndef USE_FUNCTION
  double refdB=0;
#endif

  FILE *Fo;
  static char	FileOut[150];
  int	N=256, blk_no=0, DClevel=0, AD_resolution=16;
#ifdef STATIC_ALLOCATION
  short data[512];
#else
  short *data;
#endif
  double f=-1000, fs=8000,AClevel=-100000,phi=0,step,OverFlow;
  char quiet=0, use_dB = 0;
#ifdef VMS
  char 		mrs[15]="mrs=";
#endif

  /* Check options */
  if (argc < 2)
    display_usage ();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp (argv[1], "-n") == 0)
      {
	/* Define no. of blocks to generate */
	blk_no = atol (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-blk") == 0)
      {
	/* Define input & output encoded speech bitstream format */
	N = atoi(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-dB") == 0 ||
	       strcmp (argv[1], "-db") == 0 ||
	       strcmp (argv[1], "-DB") == 0)
      {
	/* AC level is in dB */
	use_dB = 1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-ac") == 0)
      {
	/* Define AC level */
	AClevel = atof (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-dc") == 0)
      {
	/* Define DC level */
	DClevel = atof (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-f") == 0)
      {
	/* Define tone frequency */
	f = atof (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-sf") == 0)
      {
	/* Define sampling frequency */
	fs = atof (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-phase") == 0)
      {
	/* Define phase offset, in degrees */
	phi = atof (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-res") == 0)
      {
	/* Define phase offset, in degrees */
	AD_resolution = atoi (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-q") == 0)
      {
	/* Quiet more */
	quiet = 1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-?") == 0 || strstr(argv[1], "-help"))
      {
	display_usage();
      }
      else
      {
	fprintf (stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
		 argv[1]);
	display_usage ();
      }
  }

  /* Get parameters */
  GET_PAR_S(1,  "_Output file? ......................... ", FileOut);
  if (AClevel==-100000)
  {
    if(use_dB)
    {
      GET_PAR_D(2, "_AC level? [dBov] ..................... ", AClevel);
    }
    else
    {
      GET_PAR_D(2, "_Tone peak value? [PCM] ............... ", AClevel);
    }
  }
  else
  {
    argc++;argv++;
    fprintf(stderr, "%s%-8.2f\n", use_dB?
	    "_AC level? [dBov] ..................... " :
	    "_Tone peak value? [PCM] ............... ", AClevel);
  }
  if (f<0)
  {
    GET_PAR_D(3, "_Desired tone frequency? [Hz] ......... ", f);
  }
  else
  {
    argc++;argv++;
    fprintf(stderr, "_Desired tone frequency? [Hz] ......... %-8.2f\n", f);
  }
  if (blk_no==0)
  {
    GET_PAR_I(4, "_Number of blocks? [].................. ", blk_no);
  }
  else
  {
    argc++;argv++;
  }
  FIND_PAR_F(5, "_Sampling frequency? [Hz] ............. ", fs, fs);
  FIND_PAR_I(6, "_DC level? [PCM] ...................... ", DClevel, DClevel);
  FIND_PAR_F(7, "_Tone phase? [-90..90 degrees] ........ ", phi, phi);
  FIND_PAR_I(8, "_Block length? [samples] .............. ", N, N);
  FIND_PAR_I(9, "_A/D resolution [bits] ................ ",
	        AD_resolution, AD_resolution);

  /* Initializations */
  OverFlow=pow((double)2.0, (double)(AD_resolution-1))-1.0;
  refdB= v_to_dB(pow(2,AD_resolution-1));

#ifdef DEBUG
  printf("Ovf=%f\n",OverFlow);
  printf("RefdB=%f\n",refdB);
  printf("ACL=%f\n",AClevel);
#endif

  if (use_dB)
  {
    AClevel = OverFlow  * sqrt(2.0) * 
      pow((double)10.0, (double)(AClevel)/(double)20.0); 
  }
#ifdef DEBUG
  printf("ACL=%f\n",AClevel);
#endif

  /* Allocates memory */
#ifndef STATIC_ALLOCATION
  if ((data = (short *) calloc(N, sizeof(short))) == NULL) 
    HARAKIRI("Error in memory allocation!\n",10);
#endif

  /* Creates output file */
#ifdef VMS
  sprintf(&mrs[4], "%d", 2 * N);
#endif
  if ((Fo = fopen(FileOut, WB)) == NULL)
    KILL(FileOut, 1);

  /* Report of what is happening ... */
  if (!quiet)
  {
    printf("\n ----------------------------------------------------");
    printf("\n  ---> TONE FILE GENERATION REPORT");
    printf("\n ----------------------------------------------------");
    printf("\n Input file: ................ %8s",FileOut);
    printf("\n Block Length: .............. %8d []",N);
    printf("\n Number of blocks: .......... %8d []",blk_no);
    printf("\n ----------------------------------------------------");
    printf("\n Tone peak (before clipping): %8.0f [PCM]",AClevel);
    printf("\n Tone level (AC): ........... %8.2f [dBov]",
	   v_to_dB(AClevel/sqrt(2.0)));
    printf("\n DC level: .................. %8d [PCM]",DClevel);
    printf("\n Tone frequency: ............ %8.2f [Hz]",f);
    printf("\n Sampling frequency: ........ %8.2f [Hz]",fs);
    printf("\n Tone phase: ................ %8.0f [degrees]",phi);
    printf("\n         or: ................ %8.5f [radians]",
	   (phi=DEGREE_TO_RADIAN(phi)));
    printf("\n ----------------------------------------------------\n");
  }

  /* Checks for overflow and aliasing; info only, no action taken */
  if(floor(AClevel+DClevel) >  OverFlow || 
     ceil(DClevel-AClevel)  < -OverFlow-1)
  { 
    printf("%%SNF-W-OVERFLOW, value for the tone amplitude plus the ");
    printf("DC level exceeds \n  the maximum value for your digital ");
    printf("system: clipping will occur.\n");
  }
  if(f>fs/2)
  {
    printf("%%SNF-W-ALIASING, sampling frequency less than twice the ");
    printf("tone frequency: aliasing\t  will occur.\n");
  }

  /* Generation of a sine wave */
  step= TWO_PI*f/fs;
  for (i=0;i<blk_no;i++)
  {
    n=i*N;
    for(j=0;j<N;j++)
    {
      /* Get the frequency argument */
      t=(step*(j+n)+phi);

      /* Get the tone amplitude, as a float */
      t=AClevel*sin(t)+DClevel;

      /* Round and saturate at the overflow point, if necessary */
      t+=0.5*((t>0)?1.0:((t==0)?0.0:-1.0));
      data[j]=(t>OverFlow)?OverFlow:(t<-OverFlow-1)?-OverFlow-1:t;
    }

    /* Saving the sine; aborts program on failure */
    if (fwrite(data,sizeof(short), N, Fo) != N)
      KILL(FileOut, 2);
  }

  /* Finalizations */
  fclose(Fo);
#ifndef VMS
  return(0);
#endif
}
