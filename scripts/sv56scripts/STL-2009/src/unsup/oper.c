/*                                                 Version 1.5 - 05.Mar.10
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  OPER.C
  ~~~~~~

  Description:
  ~~~~~~~~~~~~
  Program for implementing simple arithmetic operations over two
  binary speech files (+, -, *, and /):
            A*file1 + B*file2 + C
            A*file1 - B*file2 + C
            A*file1 * B*file2 + C
            A*file1 / B*file2 + C
 
  Supported data types are short (in both decimal and hex formats) and,
  in the future, float. Both files have to be of the same type (short,
  float, etc).

  This program can also add the files with a delay between them.  This
  delay may be fixed or random, as specified in the command line.  The
  default is no delay.

  *** NOTE: ***
  Not fully tested: need to be thoroughly tested with -, *, and /.

  Usage:
  ~~~~~~
  $ OPER [-options] A file1 op B file2 C resfil
         [BlockSize [1stBlock [NoOfBlocks [DataType]]]]]
  where:
  A           is the multiplier for the first file samples (dB or linear)
  file1       is the first file name;
  B           is the multiplier for the 2nd file samples (dB or linear)
  file2       is the second file name;
  C           is the constant to add (linear *only*!)
  BlockSize   is the Block size, in samples [default: 256 samples];
  1stBlock    is the stating block [default: 1st block];
  NoOfBlocks  the number of blocks to be operated [default: all];
  DataType    if the file data type (float, long, ...) [default: short];

  Options:
  -q          operate quietly - only report total no.of differences
  -delay n    n is the number of samples to delay. If n>0,
              the first file should be delayed in relation to
	      the second file (samples from file 1 should be skipped). 
	      n<0, on the other hand, means that the 2nd file 
	      is who should be delayed (i.e., have samples skipped).
  -random n   instructs the program to apply a random delay to the 
              files (as defined for -delay) but no longer than 
	      n *seconds*. It is internally converted to number of
	      samples. Default is no delay.
  -gain s     Specify that gains A, B are given either in dB (s=dB)
	      *power* or in linear format (s=linear). Default is 
	      linear. NOTE THAT the offset C is *always* linear. 
  -size #     trim output filesize by the size of 1st file (#=1), 
              of the 2nd file (#=2), the longest of both (#=3) 
	      or the shortest of both (#=0) [default: 0]
  -round      Uses rounding after integer operations [default]
  -trunc      Uses truncation after integer operations
  -float      display float numbers
  -double     display double numbers
  -short      display short numbers [default]
  -long       display long numbers
        
  Original Author:
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto
  COMSAT Labs                   Tel: +1-301-428-4516
  22300 Comsat Drive            Fax: +1-301-428-4534
  Clarksburg Md 20874 - USA     E-mail: <simao@ctd.comsat.com>

  History:
  ~~~~~~~~
  06/Jul/1995 v1.0 Created
  30/Aug/1995 v1.1 RAND_MAX is not available in non-ANSI C 
                   compilers; added a "hard-definition" if not 
		   defined by stdlib.h <simao>.
  25/Sep/1995 v1.2 Added option to trim by the size of the largest 
                   of the input files. Added overflow protection to 
		   trim case.
  27/May/1997 v1.3 Increased maximum file name size <simao>
  02/Jul/1999 v1.4 Added hook to allow compilation with MS Visual
                   C Compiler V.6 [aliased sleep to _sleep()] and
                   added option for truncation instead of rounding
                   <simao>
  05/Mar/2010 v1.5 Fixed problem of Oper caused by change in GET_PAR_S()
                   macro (yusuke hiwasaki)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/* includes in general */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> /* for memset() */
#include <ctype.h>
#include "ugstdemo.h"

/* ... Includes for O.S. specific headers ... */
#if defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#elif defined(VMS)
#include <perror.h>
#include <file.h>
#include <stat.h>
#else				/* Unix */
#include <sys/stat.h>
#include <unistd.h>    /* Specific for read/write/lseek */
#endif

/* Define specific function call */
#ifdef _MSC_VER /* Dirty trick to circumvent problems in MSCVC */
#define sleep _sleep
#endif

/* Define maximum random number generator output, if not defined */
#ifndef RAND_MAX
#if defined(sun) || defined(vms)
#define RAND_MAX 2147483647 /* 2**31-1, as per the man page */
#else
#define RAND_MAX 32767      /* Def. valid for gcc, although gcc has RAND_MAX */
#endif
#endif

/* Function prototypes */
void display_usage ARGS((void));

/* Choose delay to improve seed to random number generator */
#ifndef USE_IT
#define USE_IT 1
#endif

/* Pseudo-functions */
#define dB2linear(x) pow((double)10., (x)/(double)20.);


/*
 * --------------------------------------------------------------------------
 * ... Display usage of program ...
 *     Simao 
 * --------------------------------------------------------------------------
 */
#define P(x) printf x
void display_usage()
{
  P(("OPER.C - Version 1.5 of 5/Mar/2010 \n\n"));
  P(("Program for implementing simple arithmetic operations over two\n"));
  P(("*binary* speech files (+, -, *, and /):\n"));
  P(("      A*file1 [+-*/] B*file2 + C\n"));
  P(("Supported data types are short (in both decimal and hex formats) \n"));
  P(("and, in the future, float. Both files have to be of the same \n"));
  P(("type (short, float, etc).\n"));
  P(("This program can also add the files with a fixed or a random delay\n"));
  P(("between them, as specified in the command line [Default: no delay]\n"));
  P(("\n"));
  P(("Usage:\n"));
  P(("$ OPER [-options] A file1 op B file2 C resfil\n"));
  P(("           [BlockSize [1stBlock [NoOfBlocks [DataType]]]]]\n"));
  P((" A           multiplier for the first file samples (dB or linear)\n"));
  P((" file1       first file name;\n"));
  P((" B           multiplier for the second file samples (dB or linear)\n"));
  P((" file2       second file name;\n"));
  P((" C           constant to add (linear *only*!)\n"));
  P((" BlockSize   block size, in samples [default: 256 samples];\n"));
  P((" 1stBlock    starting block [default: 1st block];\n"));
  P((" NoOfBlocks  number of blocks to be operated [default: all];\n"));
  P((" DataType    file data type (float, long, ...)[default: short];\n"));
  P(("\n"));
  P(("Options:\n"));
  P(("-q           operate quietly\n"));
  P(("-delay n     n is the number of samples to delay. If n>0,\n"));
  P(("             the first file should be delayed in relation to the\n"));
  P(("             second file (samples from file 1 should be skipped). \n"));
  P(("             n<0, on the other hand, means that the 2nd file \n"));
  P(("             is who should be delayed (i.e., have samples skipped).\n"));
  P(("-random n    instructs the program to apply a random delay to the \n"));
  P(("             files (as defined for -delay) but no longer than \n"));
  P(("             n *seconds*. It is internally converted to number of\n"));
  P(("             samples. Default is no delay.\n"));
  P(("-gain s      specify that gains A, B are given either in dB (s=dB) \n"));
  P(("             *power* or in linear format (s=linear). Default is \n"));
  P(("             linear. NOTE THAT the offset C is *always* linear. \n"));
  P(("-size #      trim output filesize by the size of 1st file (#=1), \n"));
  P(("             of the 2nd file (#=2), the longest of both (#=3) \n"));
  P(("             or the shortest of both (#=0) [default: 0]\n"));
  P(("-round       Uses rounding after integer operations [default]\n"));
  P(("-trunc       Uses truncation after integer operations\n"));
  P(("-float       display float numbers\n"));
  P(("-double      display double numbers\n"));
  P(("-short       display short numbers [default]\n"));
  P(("-long        display long numbers\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ....................... end of display_usage() ...........................*/


/* .............. definition of the operation functions ................... */
#define FUNCS 4
double add(x,y)
double x,y;
{
  return x+y;
}

double sub(x,y)
double x,y;
{
  return x-y;
}


double mult(x,y)
double x,y;
{
  return x*y;
}

double divide(x,y)
double x,y;
{
  return y<1E-38? 1E+38:x/y;
}
/* ................... operation functions ......................... */


/*
  -------------------------------------------------------------------------
  Operate over short int files 
  -------------------------------------------------------------------------
 */
long operate_shorts(File1,File2,File3,fh1,fh2,fh3,N,N1,N2,
		    A,B,C,oper_f,trim_by,round)
char *File1,*File2,*File3, trim_by;
int fh1,fh2,fh3;
long N,N1,N2;
double A, B, C, round, (*oper_f)();
{
  long i,j,l,k,saved=0;
  short *a, *b;
  register double tmp;

  /* Allocate memory for data vectors */
  if ((a=(short *)calloc(N,sizeof(short)))==NULL)
    return -1;
  if ((b=(short *)calloc(N,sizeof(short)))==NULL) {
    free(a);
    return -1;
  }

  /* Read data in chuncks, procecess & save */
  for (i = j = 0; i < N2; i++, j = 0)
  {
    /* Reset buffers */
    memset(a,0,N*sizeof(short));
    memset(b,0,N*sizeof(short));

    if ((l = read(fh1, a, sizeof(short) * N) / sizeof(short)) >= 0 &&
	(k = read(fh2, b, sizeof(short) * N) / sizeof(short)) >= 0)
      while (j < l && j < k)
      {
	tmp =  oper_f(A * (double)a[j], B * (double)b[j]) + C + round;
	b[j] = (short)(tmp > 32767 ? 32767 : (tmp < -32768 ? -32768 : tmp));
	j++;
      }
    else
    {
      if (l < 0)
      {
	KILL(File1, 5);
      }
      else if (k<0)
      {
	KILL(File2, 6);
      }
      else
	break;
    }
    /* Flush if it is the case */
    if (trim_by==1) 
      for (;j<l;j++) 
      {
	tmp = (A * (double)a[j] + C + round);
	b[j] = (short)(tmp > 32767 ? 32767 : (tmp < -32768 ? -32768 : tmp));
      }
    else if (trim_by==2)
      for (;j<k;j++) 
      {
	tmp = (B * (double)b[j] + C + round);
	b[j] = (short)(tmp > 32767 ? 32767 : (tmp < -32768 ? -32768 : tmp));
      }

    saved += write(fh3, b, sizeof(short) * j) / sizeof(short);
  }
  return(saved);
}
/* ................ end of operate_shorts() ................... */



/*============================== */
int main(argc, argv)
  int             argc;
  char           *argv[];
/*============================== */
{
  char            c[1], Oper;
  int             fh1, fh2, fhr;

  long            N, N1, N2, Prcd=0;
  long            delay=0, start_byte1, start_byte2, samplesize;
  char            File1[150], File2[150], RFile[150];
  char            TypeOfData = 'I', quiet=0, gain_in_dB = 0;
  char            better_seed=USE_IT, trim_by = 0;
  static char    *trim_str[4]={"shortest", "first", "second", "longest"};
  double          A=0, B=0, C=0, (*oper_f)(), round=0.5;
  FILE           *f1, *f2, *fr;
#ifdef VMS
  char            mrs[15] = "mrs=";
#endif


  /* ......... GET PARAMETERS ......... */

  /* Check options */
  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1],"-delay")==0)
      {
	/* Get skip length */
	delay = atol(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-random") == 0)
      {
	/* Define a random delay between the given boundary (seconds) */
	double rnd_max = atof(argv[2]);

	/* Wait a second */
	if (better_seed)
	  sleep((unsigned int)1);

	/* Convert seconds to samples: 8000 samples/s for 8kHz */
	rnd_max *= 8000; 

	srand((unsigned int)time(NULL));
	
	delay = 2*(((double)rand()/(double)RAND_MAX)-0.5)*rnd_max;

	fprintf(stderr, "Delay used is %ld samples\n", delay);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-gain") == 0)
      {
	/* Set gains in dB power, or as linear gains */
	if (strcmp(argv[2], "dB") == 0 || strcmp(argv[2], "DB") == 0 ||
	    strcmp(argv[2], "db") == 0)
	{ 
	  gain_in_dB = 1;
	}
	else if (strcmp(argv[2], "linear") == 0)
	  gain_in_dB = 0;
	else
	  HARAKIRI("Invalid gain type (use dB or linear)\n",5);

	/* Move arg{c,v} over the option to the next argument */
	argc-=2;
	argv+=2;
      }
      else if (strcmp(argv[1], "-round") == 0)
      {
	/* Round samples after integer operations */
	round = 0.5;

	/* Update argc/argv to next valid option/argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-trunc") == 0)
      {
	/* Truncate samples after integer operations */
	round = 0;

	/* Update argc/argv to next valid option/argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-size") == 0)
      {
	/* Order to trim output file size by the size of file 1, 2, 
	   or a compromise of both [default] */
	trim_by = atoi(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-float") == 0)
      {
	/* Set data type as real */
	TypeOfData = 'R';

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-double") == 0)
      {
	/* Set data type as double real */
	TypeOfData = 'D';

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-short") == 0)
      {
	/* Set data type as real */
	TypeOfData = 'I';

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-long") == 0)
      {
	/* Set data type as real */
	TypeOfData = 'L';

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
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
	/* Set quiet compare - only log the total differences */
	quiet = 1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
     else
      {
	fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
		argv[1]);
	display_usage();
      }
  }

  /* Read parameters for processing */
  GET_PAR_D(1,   "A? .................................... ", A);
  GET_PAR_S(2,   "First file? ........................... ", File1);
  GET_PAR_C(3,   "Operation (+,-,*,/)? .................. ", Oper);
  GET_PAR_D(4,   "B? .................................... ", B);
  GET_PAR_S(5,   "Second file? .......................... ", File2);
  GET_PAR_D(6,   "C? .................................... ", C);
  GET_PAR_S(7,   "Result file? .......................... ", RFile);
  FIND_PAR_L(8,  "Record Length? ........................ ", N, 256);
  FIND_PAR_L(9,  "Starting Record? ...................... ", N1, 1);
  FIND_PAR_L(10, "Number of Records? .................... ", N2, 0);
  FIND_PAR_C(11, "Short(I), long(L), float (R) or double (D) data? .. ", 
                c[0], TypeOfData);
  TypeOfData = c[0];


  /* ......... SOME INITIALIZATIONS ......... */
  --N1;
  fprintf(stderr, "Trimming by the size of the %s file %s\n", 
	          trim_str[(int)trim_by], "(if different sizes)");

  /* Classify operation */
  switch(Oper)
  {
  case '+':
    oper_f = add;
    break;
  case '-': 
    oper_f = sub;
    break;
  case '*': case 'x': case 'X':
    oper_f = mult;
    break;
  case '/':
    oper_f = divide;
    break;
  default:
    HARAKIRI("Oper: Undefined operation - aborted\n",5);
  }

  /* Convert gain if necessary */
  if(gain_in_dB)
  {
    A = dB2linear(A);
    B = dB2linear(B);
  }

  /* Define sample size */
  switch(TypeOfData)
  {
    case 'R':
      samplesize = sizeof(float);
      break;
    case 'D':
      samplesize = sizeof(double);
      break;
    case 'I':
      samplesize = sizeof(short);
      break;
    case 'L':
      samplesize = sizeof(long);
      break;
    default:
      HARAKIRI("++==++==++ UNSUPPORTED DATA TYPE ++==++==++\007\n",7);
  }
  
  /* Define 1st sample to compare */
  start_byte1 = start_byte2 = samplesize;
  if (delay>=0)
  {
    start_byte1 *= (N1*N + delay);
    start_byte2 *= (N1*N);
  }
  else
  {
    start_byte1 *= (N1*N);
    start_byte2 *= (N1*N - delay);
  }

  /* Check if is to process the whole file */
  if (N2 == 0)
  {
    struct stat     st;
    long            k, l;

    /* ... find the size of the 2 files and the number of blks from it */
    /* ... hey, need to skip the delayed samples! ... */
    stat(File1, &st);
    k = (st.st_size - start_byte1) / (N * samplesize);
    stat(File2, &st);
    l = (st.st_size - start_byte2) / (N * samplesize);

    if (trim_by==1)
      N2 = k;
    else if (trim_by==2)
      N2 = l;
    else if (trim_by==3)
    {
      /* Trim by the size of the longest file */
      N2 = k > l ? k : l;

      /* Convert the trim flag to the equivalent of trimming by the size 
       * of file 1 or file 2, the longest of them; this eases implementation */
      trim_by = (k>l)? 1 : 2;
    }
    else
    {
      /* Trim by the size of the shortest file */
      N2 = k < l ? k : l;
    }

    if (k != l)
      fprintf(stderr, "WARNING: Files have different sizes!\n");
  }

  /* Opening test file; abort if there's any problem */
#ifdef VMS
  sprintf(mrs, "mrs=%d", sizeof(short) * N);
#endif

  /* Open input files */
  if ((f1=fopen(File1,RB))==NULL) KILL(File1,3);
  if ((f2=fopen(File2,RB))==NULL) KILL(File2,4);
  if ((fr=fopen(RFile,WB))==NULL) KILL(RFile,5);
  fh1= fileno(f1); fh2= fileno(f2); fhr= fileno(fr);

  /* If samples of the primary files are to be skipped, dump them into the 
     output file */
  if (delay>0)
  {
    long i, dump = start_byte1/samplesize-delay;
    short *a=(short *)calloc(sizeof(short),delay);
    double register tmp;

    if (lseek(fh1, dump * samplesize, 0l) < 0l)
      KILL(File1, 3);

    if (read(fh1, a, delay * samplesize)!= samplesize*delay) KILL(File1,6);
    for (i=0;i<delay;i++)
    {
      tmp = (A * (double)a[i] + C + round);
      a[i] = (short)(tmp > 32767 ? 32767 : (tmp < -32768 ? -32768 : tmp));
    }
    write(fhr, a, delay * samplesize);
    free(a);
  }

  /* Move pointer to 1st block of interest */
  if (lseek(fh1, start_byte1, 0l) < 0l)
    KILL(File1, 3);
  if (lseek(fh2, start_byte2, 0l) < 0l)
    KILL(File2, 4);

  /* Some preliminaries */
  N1++;

  /* Operates on the file to the screen */
  switch (TypeOfData)
  {
  case 'I': /* short data */
    Prcd = operate_shorts(File1,File2,RFile,fh1,fh2,fhr,
			  N,N1,N2,A,B,C,oper_f,trim_by,round);
    break;

#ifdef DATA_TYPE_DEFINED
  case 'L': /* long data */
    NrDiffs = operate_longs();
    break;

  case 'R': /* float data */
    NrDiffs = operate_floats();
    break;

  case 'D': /* double data */
    NrDiffs = operate_doubles();
    break;
#endif
  }


  /* Finalizations */
  close(fh1);
  close(fh2);
#ifndef VMS
  return(0);
#endif
}
