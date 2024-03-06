/*                                                 Version 1.0 20/Apr/2000
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  FDELAY.C
  ~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  A Flexible Delay program to add a specified delay (using short
  samples) to the beginning or the end of a speech file using
  short's. Samples can be a default linear, A-law, or u-law value, or
  a user-specified one. The number of samples added defaults to 43,
  but can be changed by the user. Alternatively, it can also add the
  delay from a user-provided file, which is equivalent to
  concatenating the two files in the specified order.

  Values can be specified in decimal or hex formats.

  Usage:
  ~~~~~~
  $ fdelay [-options] filein fileout law
           [BlockSize [1stBlock [NoOfBlocks]]]]
  where:
  filein      is the input file name;
  fileout     is the output file name;
  law         is the coding law: l, a, or u (linear, A-law, or u-law)
  BlockSize   is the Block size, in samples [default: 256 samples];
  1stBlock    is the starting block [default: 1st block];
  NoOfBlocks  the number of blocks to be operated [default: all];

  Options:
  -q           Operate quietly
  -delay n     n is the number of samples to delay (default: 43) 
  -tail        Add samples to the end of the file, rather than the beginning
  -law [l|A|u] Coding law (default: linear). The default value for linear
               samples is 0, for A-law is 0xD5 (0x80 without bit
               inversion), and for u-law is 0xFF.
  -dec z       Uses the specified decimal value z
  -hex z       Uses the specified hexadecimal value z 
  -file fff    Uses the provided file fff as the delay
  -blk #       Sample block size
  -n #         Number of blocks to process
  -start #     First block to process
  -end #       Last block to process (n-start+1)
  -help        Displays usage
        
  Original Author:
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto
  COMSAT Labs                   Tel: +1-301-428-4516
  22300 Comsat Drive            Fax: +1-301-428-4534
  Clarksburg Md 20874 - USA     E-mail: <simao.campos@labs.comsat.com>

  History:
  ~~~~~~~~
  20/Apr/2000 v1.0 Created
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/* includes in general */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> /* for memset() */
#include <ctype.h>
#include "ugstdemo.h"

#if defined(VMS)
#include <stat.h>
#else				/* Unix/DOS */
#include <sys/stat.h>
#endif


/*
  -----------------------------------------------------------------------------

  long add_delay (short *smp_buf, short sample, short delay, FILE *Fdel,
  ~~~~~~~~~~~~~~  FILE *Fout, char *OutFile);

  Add delay to output file, either from a user-specified file or
  using the user-specified values.  

  Return value:
  ~~~~~~~~~~~~~
  On success, the number of samples delayed. Aborts on error saving to file.

  History:
  ~~~~~~~~
  17.Apr.2000 V.1.0  Implemented <simao>

  -----------------------------------------------------------------------------
*/

long add_delay(smp_buf, sample, delay, Fdel, Fout, OutFile)
short *smp_buf, sample, delay;
FILE *Fdel, *Fout;
char *OutFile;
{
  long saved=0, smpno, i;

  if (Fdel)
  {
    /* Delay comes from a file */
    while ((smpno=fread(smp_buf, sizeof(short), delay, Fdel))>0)
    {
      if ((smpno=fwrite(smp_buf, sizeof(short), smpno, Fout)) < 0)
        KILL(OutFile, 7);
      saved += smpno;
    }
  }
  else
  {
    /* Delay created here */
    for(i = 0; i < delay; i++)
      smp_buf[i]=sample;
    if ((saved=fwrite(smp_buf, sizeof(short), delay, Fout)) < delay)
      KILL(OutFile, 7);
  }

  return saved;
}
/* ......................... End of add_delay() ......................... */


/*
  --------------------------------------------------------------------------
  display_usage()

  Shows program usage.

  History:
  ~~~~~~~~
  20/Apr/2000  v1.0 Created <simao>
  --------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage()
{
  P(("FDELAY.C Version 1.0 of 20/Apr/2000 \n\n"));

  P(("A Flexible Delay program to add a specified delay (using short\n"));
  P(("samples) to the beginning or the end of a speech file using\n"));
  P(("short's. Samples can be a default linear, A-law, or u-law value, or\n"));
  P(("a user-specified one. The number of samples added defaults to 43,\n"));
  P(("but can be changed by the user. Alternatively, it can also add the\n"));
  P(("delay from a user-provided file, which is equivalent to\n"));
  P(("concatenating the two files in the specified order.\n"));
  P(("Values can be specified in decimal or hex formats.\n\n"));

  P(("Usage:\n"));
  P(("$ fdelay [-options] filein fileout law\n"));
  P(("         [BlockSize [1stBlock [NoOfBlocks]]]]\n"));
  P(("where:\n"));
  P((" filein      is the input file name;\n"));
  P((" fileout     is the output file name;\n"));
  P((" law         is the coding law: l, a, or u (linear, A-law, or u-law)\n"));
  P((" BlockSize   is the Block size, in samples [default: 256 samples];\n"));
  P((" 1stBlock    is the stating block [default: 1st block];\n"));
  P((" NoOfBlocks  the number of blocks to be operated [default: all];\n"));
  P(("\n"));
  P(("Options:\n"));
  P((" -q           operate quietly\n"));
  P((" -delay n     n is the number of samples to delay (default: 43) \n"));
  P((" -tail        add samples to the end of the file, rather than the beginning\n"));
  P((" -law [l|A|u] coding law (default: linear). The default value for linear\n"));
  P(("              samples is 0, for A-law is 0xD5 (0x80 without bit\n"));
  P(("              inversion), and for u-law is 0xFF.\n"));
  P((" -dec z       Uses the specified decimal value z\n"));
  P((" -hex z       Uses the specified hexadecimal value z \n"));
  P((" -file fff    Uses the provided file fff as the delay\n"));
  P((" -blk #       Sample block size\n"));
  P((" -n #         Number of blocks to process\n"));
  P((" -start #     First block to process\n"));
  P((" -end #       Last block to process (n-start+1)\n"));
  P((" -help        Displays usage\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ....................... End of display_usage() ....................... */


/*============================== */
int main(argc, argv)
  int             argc;
  char           *argv[];
/*============================== */
{
  long            N=256, N1=1, N2=0, start_byte;
  long            i, smpno, saved=0;
  long            delay=43;
  char            InpFile[150], OutFile[150], *delay_file=0;
  char            quiet=0, append=0;
  short           sample=0;
  FILE           *Finp, *Fout, *Fdel=0;
#ifdef VMS
  char            mrs[15] = "mrs=";
#endif

   int frame = 0;
   short *smp_buf;



  /* ......... GET PARAMETERS ......... */

  /* Check options:
   -q           operate quietly
   -delay n     n is the number of samples to delay (default: 43) 
   -tail        add samples to the end of the file, rather than the beginning
   -law [l|A|u] coding law (default: linear). The default value for linear
               samples is 0, for A-law is 0xD5 (0x80 without bit
               inversion), and for u-law is 0xFF.
   -dec z       Uses the specified decimal value z. Overrides -law!
   -hex z       Uses the specified hexadecimal value z. Overrides -law!
   -file fff    Uses the provided file fff as the delay
   -help        Displays usage
  */
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
      else if (strcmp(argv[1], "-tail") == 0)
      {
	/* Add samples to the end of the file, rather than the beginning */
	append=1;

	/* Move arg{c,v} over the option to the next argument */
	argc--;
	argv++;
      }
      else if (strcmp(argv[1], "-law") == 0)
      {
	/* Define coding law default value */
        switch(argv[2][0])
	{
	case 'A':
	case 'a':
	  sample=0x00D5;
	  break;
	case 'U':
	case 'u':
	  sample=0x00FF;
	  break;
	case 'l':
	case 'L':
	  sample=0;
	  break;
	default:
	  fprintf(stderr, "Invalid law: %s", argv[2]);
	  display_usage();
	}

	/* Move arg{c,v} over the option to the next argument */
	argc-=2;
	argv+=2;
      }
      else if (strcmp(argv[1], "-dec") == 0)
      {
	/* Uses the specified decimal value. Overrides -law! */
	sample = atoi(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argc-=2;
	argv+=2;
      }
      else if (strcmp(argv[1], "-hex") == 0)
      {
	/* Uses the specified hexadecimal value. Overrides -law! */
	sscanf(argv[2],"%hx", &sample);

	/* Update argc/argv to next valid option/argument */
	argc-=2;
	argv+=2;
      }
      else if (strcmp(argv[1], "-file") == 0)
      {
	/* Truncate samples after integer operations */
	delay_file = argv[2];

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-blk") == 0)
      {
	/* Change default block size */
	N = atol(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-start") == 0)
      {
	/* Change default starting block */
	N1 = atol(argv[2]);

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
      else if (strcmp(argv[1], "-n") == 0)
      {
	/* Change default number of blocks */
	N2 = atol(argv[2]);

	/* Update argc/argv to next valid option/argument */
	argv+=2;
	argc-=2;
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
  GET_PAR_S(1,  "First file? ........................... ", InpFile);
  GET_PAR_S(2,  "Second file? .......................... ", OutFile);
  FIND_PAR_L(3, "Record Length? ........................ ", N, N);
  FIND_PAR_L(4, "Starting Record? ...................... ", N1, N1);
  FIND_PAR_L(5, "Number of Records? .................... ", N2, N2);

  /* Print information */
  if (delay_file)
    fprintf(stderr, "Using delay from file %s.\n", delay_file);
  else
    fprintf(stderr, "# %s %ld samples of value %ld (0x%04X).\n",
            append? "Appending":"Delaying", delay, sample, sample);

  /* Open files; abort on error */
  if ((Finp=fopen(InpFile,RB))==NULL)
    KILL(InpFile,3);
  if ((Fout=fopen(OutFile,WB))==NULL)
    KILL(OutFile,4);
  if (delay_file)
  {
    /* Delay comes from a file - open delay file */
    if ((Fdel=fopen(delay_file, RB))==NULL)
      KILL(delay_file, 5);
  }

  /* Allocate memory */
  if ((smp_buf=(short *)calloc(N>delay? N : delay, sizeof(short)))==NULL)
    HARAKIRI("Cannot allocate memory for buffer. Aborted.\n",6);

  /* Find starting byte in file */
  start_byte = sizeof(short) * (long) (--N1) * (long) N;

  /* Check if is to process the whole file */
  if (N2 == 0)
  {
    struct stat     st;

    /* ... find the input file size ... */
    stat(InpFile, &st);
    N2 = ceil((st.st_size - start_byte) / (double)(N * sizeof(short)));
  }

  /* Move pointer to 1st block of interest */
  if (fseek(Finp, start_byte, 0) < 0l)
    KILL(InpFile, 4);

  /* Put delay samples in begining of file, if *not* appending */
  if (!append)
    saved += add_delay(smp_buf, sample, delay, Fdel, Fout, OutFile);

  /* Copy input file to output file */
  for(i=0;i<N2;i++) 
  {
    /* Read a block of samples */
    if ((smpno = fread(smp_buf, sizeof(short), N, Finp)) < 0)
      KILL(InpFile, 6);

    /* Frame counter */
    frame++;

    /* Write output words */
    if ((smpno = fwrite(smp_buf, sizeof(short), smpno, Fout)) < 0)
      KILL(OutFile, 7);

    /* Update sample counter */
    saved += smpno;
  }

  /* Put delay samples in the end of the file, if appending */
  if (append)
    saved += add_delay(smp_buf, sample, delay, Fdel, Fout, OutFile);

  /* Print report */
  fprintf(stderr, "# A total of %ld samples were saved to %s\n", saved, OutFile);

  /* Free memory, close files */
  free(smp_buf);
  fclose(Finp);
  fclose(Fout);
  if (Fdel)
    fclose(Fdel);
  return(0);
}
/* .............................. End of main() .............................. */


