/*                                                          v2.2 - 22/Feb/1996
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	DUMPFILE.C

	Description:
	~~~~~~~~~~~~
	Program for dumping a file to the screen as integers or hex
	numbers; the option for dumping float files has not been
	implemented yet.

	Usage:
	~~~~~~
	$ DUMPFILE [-t Type] [-k Kind] file BlkSize 1stBlk BlkNo Dump Type
	where: 	file 		is the file name;
		BlkSize 	is the Block size, in samples;
		1stBlk 		is the stating block;
		BlkNo 		the number of blocks to be displayed;
		Dump	 	if the dump is in integer/real [I] format 
				or if it is hexa[H];
		Type		is the Data type: if the file contains 
				integer [I] or real [R] data;
        Options:
	~~~~~~~~
	-delay       is the number of samples to delay. If `delay'>0,
		     the first file should be delayed in relation to
                     the second file (samples from file 1 should be skipped). 
                     `delay'<0, on the other hand, means that the 2nd file 
                     is who should be delayed (i.e., have samples skipped).
        -h           set dump in hex mode - valid for integer data types
        -i           set dump in decimal mode for integer data types [default]
        -float       display float numbers
        -double      display double numbers
	-short       display short numbers [default]
        -long        display long numbers

	Original Author:  Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~~~~~~~~~~  <tdsimao@cpqd.ansp.br>

	History:
	~~~~~~~~
	08/Mar/1990 V1.0	Original release, for VMS/DOS/SunOS
	11/Nov/1992 V2.0	From blocksize on, the parameters are optional;
				kind of dump and data type may be specified by
				command line options (Unix style). Simao
        10/Jan/1995 V2.1        Fixing options that don't work.
        22/Feb/1996 v2.2        Removed compilation warnings, included headers 
                                as suggested by Kirchherr (FI/DBP Telekom) to
                                run under OpenVMS/AXP <simao@ctd.comsat.com>
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#if defined(__MSDOS__) && !defined(MSDOS)
# define MSDOS
#endif

/* includes in general */
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ugstdemo.h"


/* includes for DOS specific directives */
#if defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
/* includes for VMS specific directives */
#elif defined(VMS)
#include <perror.h>
#include <stat.h>
#include <file.h>
/* includes for other OS (e.g. Unix) specific directives */
#else
#include <sys/stat.h>
#endif


/* definitions for the program itself */
#define PRINT_RULE { for (k=0;k<4;k++) printf("-------------------");}
#define CR	   printf("\n");

/* ---------------------------- display usage ------------------------------ */
#define P(x) printf(x)
void display_usage()
{
  P(("\t\t\t*** DUMPFILE -- 10/Jan/1994 V2.1 ***\n\n"));

  P((" Description:\n"));
  P((" Program for dumping a file to the screen as integers or hex\n"));
  P((" numbers; the option for dumping float files has not been\n"));
  P((" implemented yet.\n\n"));

  P((" Usage:\n"));
  P((" $ DUMPFILE [-options] file BlkSize 1stBlk BlkNo Dump Type\n"));
  P((" where:\n"));
  P(("  file        is the file name;\n"));
  P(("  BlkSize     is the Block size, in samples;\n"));
  P(("  1stBlk      is the stating block;\n"));
  P(("  BlkNo       the number of blocks to be displayed;\n"));
  P(("  Dump        if the dump is in decinal [D] format \n"));
  P(("              or if it is hexa[H];\n"));
  P(("  Type        is the Data type: if the file contains \n"));
  P(("              integer [I] or real [R] data;\n"));
  P((" Options:\n"));
  P((" -delay       is the number of samples to delay.\n"));
  P((" -h           set dump in hex mode - valid for integer data types\n"));
  P((" -d           set dump in decimal mode for integer data types [default]\n"));
  P((" -float       display float numbers\n"));
  P((" -double      display double numbers\n"));
  P((" -short       display short numbers [default]\n"));
  P((" -long        display long numbers\n"));

  P((" Original Author:  Simao Ferraz de Campos Neto -- Comsat Labs\n"));
  P(("                   <simao@ctd.comsat.com>\n\n"));

  /* Quit program */
  exit(1);
}
#undef FP


/* ---------------------------- main routine ------------------------------ */
int main(argc,argv)
int argc;
char *argv[];
{ 
  char c,C[2];
  int i,j,k,l;
  FILE *fi;

  long            delay=0, start_byte1, samplesize;
  int N,N1,N2;
  char	FileIn[50],KindOfDump={'D'},TypeOfData={'I'};
  short int ilet[4096];
  char quiet;
#ifdef VMS
  char mrs[11]="mrs=512";
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
      else if (strcmp(argv[1], "-h") == 0)
      {
	/* Set dump as hex type */
	KindOfDump = 'H';

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-d") == 0)
      {
	/* Set dump in decimal format */
	KindOfDump = 'D';

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
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

#ifdef OLD
  if (argc < 2)
    display_usage();
  else
  {
    while (argv[1][0] == '-')
      switch(tolower(argv[1][1]))
      {
      case 'k':
	/* Get kind of dump */
	KindOfDump = toupper(argv[2][0]);

	/* Move argv over the option to the 1st mandatory argument */
	argv += 2;

	/* Update type of data */
	argc -= 2;
        break;

      case 't':
	/* Get skip length */
	TypeOfData = toupper(argv[2][0]);

	/* Move argv over the option to the 1st mandatory argument */
	argv += 2;

	/* Update argc */
	argc -= 2;
        break;

     default:
      	fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
		argv[1]);
	display_usage();
    }
  }
#endif /* OLD */

  GET_PAR_S (1," Input file? ....................................... ",FileIn);
  FIND_PAR_I(2," Record Length? .................................... ",N,256);
  FIND_PAR_I(3," Starting Record? .................................. ",N1,1);
  FIND_PAR_I(4," Number of Records? ................................ ",N2,0);
  FIND_PAR_C(5," Decimal or Hex dump? (D,H) [D] .................... ",
	       C[0],KindOfDump);
  KindOfDump=(toupper(C[0])!='H')?'D':'H';
  FIND_PAR_C(6," Short(I), long(L), float (R) or double (D) data? .. ", 
                C[0], TypeOfData);
  TypeOfData = C[0];

  /* ......... SOME INITIALIZATIONS ......... */
  --N1;
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
  start_byte1 = samplesize;
  if (delay>=0)
  {
    start_byte1 *= (N1*N + delay);
  }
  else
  {
    start_byte1 *= (N1*N);
  }

  /* Check the number of records */
  if (N2 == 0)
  {
    struct stat     st;
    stat(FileIn, &st);
    N2 = ceil((st.st_size - start_byte1) / (double)(N * samplesize));
  }

  /* Open input file */
  if ((fi=fopen(FileIn,RB))==NULL) KILL(FileIn,1);

  /* Move file pointer to start of block N1 */
  if (fseek(fi,start_byte1,0)<0l) 
    KILL(FileIn,2);

  /* Dumps the file to the screen */
  CR; c=0; N1++;	    /* some preliminaries */
  switch(TypeOfData)
  {
  case 'I':
    for (i=j=0;i<N2;i++,j=0)
    {
      if(!(c=='Q'||c=='X'||c==27||c=='S'))
      {
	if ((l=fread(ilet,samplesize,N,fi))>0)
	  while(j<l)
	  { 
	    CR; PRINT_RULE;
	    printf("\n FILE: %s - BLOCK SIZE: %d - Block %d, part %d of %d\n",
		   FileIn,l,N1+i,j/200+1,N/201+1);
	    PRINT_RULE;
	    do
	    {
	      if (j%10==0) printf("\n%3d | ",j+1);
	      if (KindOfDump=='D')
		printf("%6d ",ilet[j++]);
	      else
		/* printf("%6.4X ",ilet[j++]); */
		printf("%04hX ",ilet[j++]);
	    } while(j%200!=0&&j<l);
	    CR; PRINT_RULE;
	    if ((c=toupper(getchar()))=='Q'||c=='X'||c==27||c=='S') break;
	  }
	else
	{
	  KILL(FileIn,3);
	}
      }
    }
    break;


  case 'R':
    fprintf(stderr, "This program doesn't handle REALs yet ...");
    HARAKIRI(" sorry and aborting!\n\a",4);
  }
  /* Finalizations */
  PRINT_RULE;
  fclose(fi);
#ifndef VMS
  return(0);
#endif
}
