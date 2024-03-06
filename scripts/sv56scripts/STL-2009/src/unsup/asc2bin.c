/*                                                        v1.0 - 20.Mar.94
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  asc2bin.c

  Description:
  ~~~~~~~~~~~~
  Program for converting samples in ascii format to its binary 
  representation. 

  Usage:
  ~~~~~~
  $ asc2bin [-options] ascfile binfile
  Where:
  ascfile         is the name of the input file in ascii format;
                   if ommited or if equal to "-", uses stdin
  binfile         is the name of the output, binary file;
                   if ommited or if equal to "-", uses stdout

  Options:
  -h              data is in hex format
  -d              data is in decimal format (int) [default]
  -short          data is short int
  -long           data is long int
  -float          data is float
  -double         data is long double

  Original Author:
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto -- CPqD/Telebras
  <simao@cpqd.ansp.br>

  History:
  ~~~~~~~~
  20.Mar.94 v.1.0 Created.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/* OS definition */
#ifdef __MSDOS__
#define MSDOS
#endif

/* includes in general */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* includes for DOS specific directives */
#if defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
/* includes for VMS specific directives */
#elif defined(VMS)
#include <perror.h>
#include <file.h>
/* includes for other OS (Unix) specific directives */
#else
#include <sys/stat.h>
#endif

/* ---------------------------- Display Usage ---------------------------- */
#define P(x) printf x
void display_usage()
{
  P(("  asc2bin: version 1.0 of 20.Mar.94 - <simao@cpqd.ansp.br>\n\n"));

  P(("  Program for converting samples in ascii format to its binary\n"));
  P(("  representation.\n"));
  P(("\n"));
  P(("  Usage:\n"));
  P(("  $ asc2bin [-options] ascfile binfile\n"));
  P(("  Where:\n"));
  P(("  ascfile         is the name of the input file in ascii format;\n"));
  P(("                  if ommited or if equal to \"-\", uses stdin\n"));
  P(("  binfile         is the name of the output, binary file;\n"));
  P(("                  if ommited or if equal to \"-\", uses stdout\n"));
  P(("\n"));
  P(("  Options:\n"));
  P(("  -h  data is in hex format\n"));
  P(("  -d              data is in decimal format (int) [default]\n"));
  P(("  -short          data is short int\n"));
  P(("  -long           data is long int\n"));
  P(("  -float          data is float\n"));
  P(("  -double         data is long double\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* .................. end of display_usage() .......................... */


/* ---------------------------- Main Routine ---------------------------- */
int main(argc, argv)
  int             argc;
  char           *argv[];
{
  int		  d;
  short           hd;
  unsigned short  X;
  long		  ld;
  float		  f;
  double	  lf;
  long 		  data_size = sizeof(hd);
  long		  count = 0;
  char NumberFormat = 'd';
  char DataType = 'h';
  void *data = &hd;
  char fmt[10]; /* scanf format string */
  FILE           *fi, *fo;
  static char *cvt_type_str[] = {"hexadecimal", "decimal", "float", "double", 
			  "short", "long"};
  enum conv {HEX, DEC, FLOAT, DOUBLE, SHORT, LONG};
  int cvt_type=DEC; /* Default is decimal format */

  /* Check options */
  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1], "-") == 0)
      {
	/* Stop processing options */
        break;
      }
      else if (strcmp(argv[1], "-h") == 0)
      {
	/* Set dump as hex type */
	NumberFormat = 'X';
	DataType = 'h';
        data = &X;
	data_size = sizeof(X);
	cvt_type = HEX;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-d") == 0)
      {
	/* Set dump in decimal format */
	NumberFormat = 'd';
	DataType = 0;
        data = &d;
	data_size = sizeof(d);
	cvt_type = DEC;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-float") == 0)
      {
	/* Set data type as real */
	NumberFormat = 'f';
	DataType = 0;
	data = &f;
	data_size = sizeof(f);
	cvt_type = FLOAT;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-double") == 0)
      {
	/* Set data type as double real */
	NumberFormat = 'f';
	DataType = 'l';
	data = &lf;
	data_size = sizeof(lf);
	cvt_type = DOUBLE;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-short") == 0)
      {
	/* Set data type as short */
	NumberFormat = 'd';
	DataType = 'h';
	data = &hd;
	data_size = sizeof(hd);
	cvt_type = SHORT;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-long") == 0)
      {
	/* Set data type as long */
	NumberFormat = 'd';
	DataType = 'l';
	data = &ld;
	data_size = sizeof(ld);
	cvt_type = LONG;

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

  /* Parse file names */
  switch(argc)
  {
  case 3:
     fi = strcmp(argv[1],"-")==0 ? stdin : fopen(argv[1],"rt");
     fo = strcmp(argv[2],"-")==0 ? stdout : fopen(argv[2],"wb");
     break;
  case 2:
     fi = fopen(argv[1],"rt");
     fo = stdout;
     break;
  case 1:
     fi = stdin;
     fo = stdout;
     break;
  }

  /* Check for file opening errors */
  if (fi==NULL)
    return(perror(argv[1]),2);
  if (fo==NULL)
    return(perror(argv[2]),3);

  /* Setup string conversion rules */
  if (DataType)
    sprintf(fmt, "%%%c%c", DataType, NumberFormat);
  else
    sprintf(fmt, "%%%c", NumberFormat);

  /* Print info */
  fprintf(stderr, "Data format is %s, each sample having %ld bytes\n", 
	  cvt_type_str[cvt_type], data_size);

  /* Convert ascii input to binary output */
  while(fscanf(fi, fmt, data)==1)
  {
    fwrite(data, data_size, 1L, fo);
    count++;
  }



  /* Closing program */
  fprintf(stderr, "Converted %ld samples\n", count);
  fclose(fi);
  fclose(fo);
#ifndef VMS
  return(0);
#endif
}
