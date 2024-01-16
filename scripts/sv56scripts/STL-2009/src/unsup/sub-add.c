/*
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	SUB-ADD.C

	Description:
	~~~~~~~~~~~~
	Program for adding of subtracting the first file from the second,
	printing the results on the screen, or in a BINARY file. Supposes
	`short' data format.

	The decision of add or subtract is taken at compilation time; the
	default option is to SUBTRACT files; however, if the symbol ADD_FILES
	is defined, then the option for addition of files is followed.

	Usage:
	~~~~~~
	$ SUB file1 file2 [BlkSiz [1stBlock [NoOfBlocks [output] ]]]
	where: 	file1 		is the first file name;
		file2 		is the second file name;
		BlkSiz 		is block size, in samples;
		1stBlock 	is the starting block;
		NoOfBlocks 	the number of blocks to be displayed;
		output          if specified, is the name of output
				file, with the difference values.

	Options:
	-delay n	delay the 1st file by n samples and then compare;
			a negative n causes the 2nd file to be delayed
	-equiv n        consider differences of upto +- to be equivalent files
                        and report as such.
	-q		run in quiet mode - only report totals

	Author:	Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~

	Revision:
	~~~~~~~~~
	15/Jan/1991 v1.0 1st release
        22/Feb/1996 v1.1 Removed compilation warnings, included headers as
                         suggested by Kirchherr (FI/DBP Telekom) to run under
			 OpenVMS/AXP <simao@ctd.comsat.com>

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/* OS definition */
#if defined(__MSDOS__) && !defined(MSDOS) 
# define MSDOS
#endif

/* includes in general */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ugstdemo.h"

/* includes for DOS specific directives */
#if defined (MSDOS)
# include <fcntl.h>
# include <io.h>    /* For read(), write(), lseek() */
# include <sys\stat.h>

/* includes for VMS specific directives */
#elif defined (VMS)
# include <perror.h>
# include <file.h>
# include <unixio.h>    /* For read(), write(), lseek() */
char mrs[15] = "mrs=512";

/* Other O.S. */
#else
# include <unistd.h>     /* For read(), write(), lseek() */
# include <sys/stat.h>
#endif



#define PRINT_RULE { for (K=0;K<4;K++) printf("-------------------");}
#define PRINT_RULE2 { for (K=0;K<4;K++) printf("===================");}
#define CR	   printf("\n");

#define YES 1
#define NO  0

/* Pseudo-functions */
#define display_usage() (\
  printf("Usage: %s [-options] file1 file2 %s\n", argv[0],\
	 "[BlkSiz [1stBlock [NoOfBlocks [output] ]]]"),\
  exit(-128) \
)

#define ABS(x) (x>0?(x):-(x))


/* ************************************************************************* */
/* ********************* Main program section ****************************** */
/* ************************************************************************* */
int main(argc,argv)
int argc;
char *argv[];
{
  char out_is_file=NO;
  int i,j,k,l,K;

  char	File1[50],File2[50];
  int fh1,fh2,fho;
  FILE *F1,*F2,*Fo;

  long int	N, N1, N2, NrDiffs=0, NrEquivs=0;
  long start_byte1, start_byte2,delay=0;
  short int a[4096],b[4096];
  char quiet = 0;
  long equiv=0;

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
      else if (strcmp(argv[1], "-equiv") == 0)
      {
	/* Compare using the concept of equivalent results: differences 
	 * not exceeding +-equiv */
	equiv = atol(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
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
  GET_PAR_S(1,  "First file? ........................... ", File1);
  GET_PAR_S(2,  "Second file? .......................... ", File2);
  FIND_PAR_L(3, "Record Length? ........................ ", N, 256);
  FIND_PAR_L(4, "Starting Record? ...................... ", N1, 1);
  FIND_PAR_L(5, "Number of Records? .................... ", N2, 0);

  if (argc>6)
  {
    if ((Fo=fopen(argv[6],WB))==NULL) KILL(argv[6],4);
    fho = fileno(Fo);
    out_is_file = YES;
  }
  else
   fho = fileno(stdout);

  /* Define 1st sample to compare */
  N1--;
  start_byte1 = start_byte2 = sizeof(short);
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

  /* Find number of blocks */
  if (N2==0)
  {
    struct stat     st;
    long            k, l;

    /* ... find the shortest of the 2 files and the number of blks from it */
    /* ... hey, need to skip the delayed samples! ... */
    stat(File1, &st);
    k = (st.st_size - start_byte1) / (N * sizeof(short));
    stat(File2, &st);
    l = (st.st_size - start_byte2) / (N * sizeof(short));
    N2 = k < l ? k : l;
    if (k != l)
      fprintf(stderr, "%%SUB-W-DIFSIZ: Files have different sizes!\n");
  }

  /* Open input files */
  if ((F1=fopen(File1,RB))==NULL) KILL(File1,2);
  if ((F2=fopen(File2,RB))==NULL) KILL(File2,3);
  fh1 = fileno(F1); fh2 = fileno(F2);

  /* Positions file to the starting of block N1 */
  N1--;	/* for the 1st block is not 1 but 0! */
  if (lseek(fh1,start_byte1,0)<0l) KILL(File1,5);
  if (lseek(fh2,start_byte2,0)<0l) KILL(File2,6);

  /* Print dump information */
  if (out_is_file==NO && !quiet)
  {
    CR; PRINT_RULE;
    printf("\n COMPARING %s and %s\n",File1,File2);
    PRINT_RULE;
  }

  /* Dumps the file to the output (file or screen) */
  for (NrDiffs=i=j=0;i<N2;i++,j=0)
  {
    if ((l=read(fh1,a,2*N)/2)>0&&(k=read(fh2,b,2*N)/2)>0)
    {
      if (out_is_file)
      {
	if (isatty(fileno(stderr)) && !quiet)
	  fprintf (stderr,"Now processing block %d\t\t\r",i+1);

	for (j=0; j<l&&j<k; j++)
	{
#ifdef ADD_FILES
	  b[j] += a[j];
#else
	  b[j] -= a[j];
#endif
	  /* Increment difference and equivalence count */
	  NrDiffs++;
	  if(ABS(b[j])<=equiv &&b[j]!=0 ) 
	    NrEquivs++;
	}
	if ((K=write(fho,b,2*N))!=2*l) KILL(argv[6],9);
      }
      else for(j=0; j<l&&j<k; j++)
      {
	if (a[j]!=b[j])
	{
	  /* Increment difference and equivalence count */
	  NrDiffs++;
#ifdef ADD_FILES
	  if(ABS(a[j]+b[j])<=equiv) 
#else
	  if(ABS(a[j]-b[j])<=equiv) 
#endif
	    NrEquivs++;
	  else 
	    if (!quiet)
	    {
	      /* Print if requested non-equivalent samples */
	      printf("\nBLOCK: %6d, SAMPLE: %4d ===>",i+1,j+1);
#ifdef ADD_FILES
	      printf("\t%6d\t%6d\t%6d",a[j],b[j],b[j]+a[j]);
#else
	      printf("\t%6d\t%6d\t%6d = %7.2f",a[j],b[j],b[j]-a[j],
		     ((double)(b[j]-a[j]))/(double)a[j]*100.);
#endif
	  }
	}
      }
    }
    else
    {
      if (l<0) KILL(File1,7);
      if (k<0) KILL(File2,8);
      break;
    }
  }

  /* Print summary */
  if (!quiet)
    printf("\n");

  printf("%s and %s: %ld different samples ", File1, File2, NrDiffs);
  if (equiv && NrDiffs) 
    printf("(%ld equivalent at a +-%ld level) ", NrEquivs, equiv);
  printf("found out of %ld.\n", N*N2);

  /* Finalizations */
  fclose(F1); 
  fclose(F2);
  if (out_is_file) 
    fclose(Fo);
#ifndef VMS
  return 0;
#endif
}
