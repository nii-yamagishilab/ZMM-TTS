/*
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  bin2asc.c

  Description:
  ~~~~~~~~~~~~
  
  Program for dumping a binary (short) file onto stdout, skipping the
  initial `skip' samples. This feature is useful for skipping the
  header of binary files. The default is no skip (0).

  Usage:
  ~~~~~~
  $ bin2asc [-s skip] file [block_size [start_block [no_of_blocks]]]
  Where:
  file 		is the name of the file with binary (short) data;
  block_size 	is the block (record) size, in (short) samples;
			default is 256;
  start_block 	is the first block to dump; default is the 1st one;
  no_of_blocks	is the number of blocks to dump; default is all.

  Options:
  -h            print output as a hex number [default: decimal]
  -H            print output as a hex number preceded by 0x
  -o            print output as an octal number preceded by 0       
  -s		is the number of initial samples to skip. Default: none

  Example:
  ~~~~~~~~
  $ bin2asc -s 20 speech.fil 256 1 1 
  Will produce a dump on the screen of 256 samples from the file
  `speech.fil', starting at its sample number 21 (skip 20!) of its 
  1st block (block number 1). Note that the block size is 256 samples.

  $ bin2asc -s 13 speech.fil 160 10 2 
  Will produce a dump on the screen of 320 (160*2) samples from the file
  `speech.fil', starting at its sample number 14 (skip 13!) of its 
  10th block (block number 10). Note that the block size is 160 samples.

  Author:	Simao Ferraz de Campos Neto -- CPqD/Telebras
  ~~~~~~~

  Revision/Date: none (yet)
  ~~~~~~~~~~~~~~
  11.Nov.92 v1.0 created
  19.Nov.97 v1.1 added hex and octal outputs <simao.campos@comsat.com>
  08.Aug.98 v1.2 fixed bug in that the output file has less samples 
                 than it should when the input file size is not an
                 integer multiple of the block size <simao>.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/* OS definition */
#ifdef __MSDOS__
#define MSDOS
#endif

/* includes in general */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include <file.h>
/* includes for other OS (Unix) specific directives */
#else
#include <sys/stat.h>
#endif

/* ---------------------------- Display Usage ---------------------------- */
#define FP(x) fprintf(stderr, x)
void display_usage()
{
  FP(" *** bin2asc.c --- 08/Aug/1998 --- Version: 1.2 *** \n\n");

  FP(" Program for dumping a binary (short) file onto stdout, \n");
  FP(" skipping the initial `skip' samples. This feature is \n");
  FP(" useful for skipping the header of binary files. The default \n");
  FP(" is no skip (0).\n\n");

  FP(" *** Usage: ***\n");
  FP(" $ bin2asc [Options] file [blk_size [start_blk[no_of_blks]]]\n");
  FP(" Where:\n");
  FP(" file	is the name of the file with binary (short) data;\n");
  FP(" blk_size 	is the block (record) size, in (short) samples;\n");
  FP(" 		default is 256;\n");
  FP(" start_blk   is the 1st block to dump; default is the 1st one;\n");
  FP(" no_of_blks  is the number of blocks to dump; default is all.\n");

  FP(" Options:\n");
  FP(" -h          print numbers as Hex [Default: decimal]\n");
  FP(" -H          print numbers as Hex (with a leading 0x)\n");
  FP(" -o          print numbers as octal (with a leading zero)\n");
  FP(" -s          is the number of initial samples to skip [Default: none]\n");

  FP(" *** Examples: ***\n");
  FP(" $ bin2asc -s 20 speech.fil 256 1 1 \n");
  FP(" Will produce a dump on the screen of 256 samples from the file\n");
  FP(" `speech.fil', starting at its sample number 21 (skip 20!) of \n");
  FP(" its 1st block (block number 1). Note that the block size is\n");
  FP(" 256 samples.\n\n");

  FP(" $ bin2asc -s 13 speech.fil 160 10 2 \n");
  FP(" Will produce a dump on the screen of 320 (160*2) samples from \n");
  FP(" `speech.fil', starting at its sample number 14 (skip 13!) of \n");
  FP(" its 10th block (block number 10). Note that the block size is \n");
  FP(" 160 samples.\n\n");

  FP(" Author:	Simao Ferraz de Campos Neto -- CPqD/Telebras\n\n");

  exit(1);

}
#undef FP

/* ---------------------------- Main Routine ---------------------------- */
int main(argc, argv)
  int             argc;
  char           *argv[];
{
  int             i, j, l;

  long            N, N1, N2, skip=0;
  char            File1[50];
  short int       a[4096];
  FILE           *f1;
  static char     fmt[20]="%d\n";

  /* Get parameters */
  if (argc < 2)
    display_usage ();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp (argv[1], "-skip") == 0)
      {
	/* Get skip length */
	skip = atol(argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-o") == 0)
      {
	/* Output mode is octal, not decimal */
	strcpy(fmt,"%#o\n");

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-h") == 0)
      {
	/* Output mode is hexadecimal, not decimal */
	strcpy(fmt,"%04X\n");

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-H") == 0)
      {
	/* Output mode is hexadecimal, not decimal */
	strcpy(fmt,"0x%04X\n");

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp (argv[1], "-?") == 0)
      {
	display_usage();
      }
      else if (strstr(argv[1], "-help"))
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

  GET_PAR_S(1,  "First file? ............ ", File1);
  FIND_PAR_L(2, "Record Length? ......... ", N, 256L);
  FIND_PAR_L(3, "Starting Record? ....... ", N1, 1L);
  FIND_PAR_L(4, "Number of Records? ..... ", N2, 0L);

  /* Open input files */
  if ((f1 = fopen(File1, RB)) == NULL)
    KILL(File1, 2);

  /* Check the number of records */
  if (N2 == 0)
  {
    struct stat     F1stat;
    stat(File1, &F1stat);
    N2 = ceil(F1stat.st_size / (double)(sizeof(short) * N));
  }

  /* Positions file to the starting of block N1 */
  N1--;				/* because the 1st block is not 1 but 0! */
  if (fseek(f1, sizeof(short) * (N1 * N + skip), 0))
    KILL(File1, 3);

  /* Dumps the file to the screen */
  for (i = j = 0; i < N2; i++, j = 0)
  {
    if ((l = fread(a, sizeof(short), N, f1)) > 0)
      while (j < l)
      {
        printf(fmt, a[j]);
	j++;
      }
    else
    {
      if (l<0)
	KILL(File1, 4);
    }
  }
  fclose(f1);
#ifndef VMS
  return(0);
#endif
}
