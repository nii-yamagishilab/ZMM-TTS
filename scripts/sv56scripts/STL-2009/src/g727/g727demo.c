/*                                                          02.Feb.2010  v1.11
  ============================================================================

  G727DEMO.C
  ~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Demonstration program for UGST/ITU-T G.727 module. Takes the input
  file and processes by the G.727 codec, depending on user's option: 
  for encoding, input must be in either A or mu law (G711), for 
  decoding, in (embedded) ADPCM format.

  Input data is supposed to be aligned at word boundaries, i.e.,
  organized in 16-bit words, following the operating system normal
  organization (low-byte first for VMS and DOS; high byte first for
  most Unix systems). G711 compressed data is supposed to be in the 8
  LEAST significant bits of the word and the ADPCM data is in the
  LEAST 5 bits. Both are without sign extension.

  Output data will be generated in the same format as decribed
  above for the input data.

  Usage:
  ~~~~~~
  $ G727demo [-options] InpFile OutFile 
             [BlockSize [1stBlock [NoOfBlocks [Law [Nc [Ne]]]]]]
  where:
  InpFile     file to be processed;
  OutFile     output file with the processed data;
  BlockSize   block size, in number of samples [default: 256]
  1stBlock    first block of the input file to be processed [dft: 1]
  NoOfBlocks  number of blocks to process starting at "1stBlock" [dft: all]
  Law         is the law desired: A, u or l(linear) [default: A]
  Nc          number of core bits to be used [default: 4]
  Ne          number of enhancement bits to be used [default: 0]

  Options:
  -enc ...... Encode-only operation [default: encode & decode]
  -dec ...... Decode-only operation [default: encode & decode]
  -e_d ...... Encode and decode operation [the default]
  -law l .... Law to use: A, u, l(inear) [default: A-law]
  -core # ... Number of core bits in sample (2, 3, 4, 5) [dft: 4]
  -enh # .... Number of enhancement bits (0, 1, 2, 3) [dft: 0]
  -blk # .... Number of samples per processing block [def: 256]
  -frame # .. same as block size
  -start # ..  starting block to measure [default: first]
  -n # ......  number of blocks to be measured [default: all]
  -end # ....  last block to be measured [default: last]
  -q ........ Quiet operation (don't print progress flag)
  -?/-help .. Display program usage
        
  Original authors:
  ~~~~~~~~~~~~~~~~~
  Nikolai L. Birioukov
  Maxim A. Lagerev
  Ukrainian Research Institut of Telecommunications
  P.O. 252650                          Phone : +380-44-276-51-01
  13, Solomenskaya St.                 FAX   : +380-44-226-26-46
  Kiev, Ukraine                        EMail : info@uniis.kiev.ua

  Adapted to UGST style:
  ~~~~~~~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

  History:
  ~~~~~~~~
  10.Jul.1996  0.99  C code version released to UGST <info@uniis.kiev.ua>
  10.Mar.1997  1.00  Conversion to UGST format <simao.campos@comsat.com>
  04.Aug.1997  1.01  Eliminated compilation warning about unused variables
                     as per revision from <Morgan.Lindqvist@era-t.ericsson.se>
  21.Mar.2000  1.10  changed memory allocation of short buffers
                     inp_buf[], out_buf[] and tmp_buf[] from static to
                     dynamic, to prevent memory invasion when block
                     sizes larger than 256 are specified. 
                     <simao.campos@labs.comsat.com>
  02.Feb.2010  1.11  Modified maximum string length, and implicit
                     casting of toupper() argument removed. (y.hiwasaki)
  ============================================================================
*/

/* ..... General definitions for UGST demo programs ..... */
#include "ugstdemo.h"

/* ..... General include ..... */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* For strcmp(), strstr() */
#include <ctype.h>
#include <assert.h>

#if defined(VMS)
#include <stat.h>
#else				/* Unix/DOS */
#include <sys/stat.h>
#endif


/* ..... G.727 module as include functions ..... */
#include "g727.h"
#include "../g711/g711.h"


/* Global variables */
FILE *in, *out;

/* Local function prototypes */
#ifdef ORIGINAL_IO
int getWord(int wordLen);
void putWord(unsigned char word, int wordLen);
#endif


/*
  --------------------------------------------------------------------------
  display_usage()

  Shows program usage.

  History:
  ~~~~~~~~
  10/Mar/1997  v1.0 Created <simao>
  --------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage()
{
  P(("G727DEMO.C Version %.2f of 10/Mar/1997 \n\n", G727_H/100.0));
 
  P(("  Demonstration program for UGST/ITU-T G.727 embedded ADPCM codec.\n"));
  P(("  Files must be in linear, A- or mu-law (G711), or in ADPCM format.\n"));
  P(("  Input data is supposed to be aligned at word boundaries, i.e.,\n"));
  P(("  organized in 16-bit words. G711 data is supposed to be in the\n"));
  P(("  8 LEAST significant bits of the word and the ADPCM data is in the\n"));
  P(("  LEAST 5 bits. Both are without sign extension. Linear is 16-bit\n"));
  P(("  linear PCM. Output data format will be as decribed above\n"));
  P(("\n"));
  P(("  Usage:\n"));
  P(("  $ G727demo [-options] InpFile OutFile \n"));
  P(("             [BlockSize [1stBlock [NoOfBlocks [Law [Nc [Ne]]]]]]\n"));
  P(("  where:\n"));
  P(("  InpFile     file to be processed;\n"));
  P(("  OutFile     output file with the processed data;\n"));
  P(("  BlockSize   block size, in number of samples [default: 256]\n"));
  P(("  1stBlock    first block of the input file to be processed [dft: 1]\n"));
  P(("  NoOfBlocks  number of blocks to process starting at \"1stBlock\" [dft: all]\n"));
  P(("  Law         is the law desired: A, u or l(linear) [default: A]\n"));
  P(("  Nc          number of core bits to be used [default: 4]\n"));
  P(("  Ne          number of enhancement bits to be used [default: 0]\n"));
  P(("\n"));
  P(("  Options:\n"));
  P(("  -enc ...... Encode-only operation [default: encode & decode]\n"));
  P(("  -dec ...... Decode-only operation [default: encode & decode]\n"));
  P(("  -e_d ...... Encode and decode operation [the default]\n"));
  P(("  -law l .... Law to use: A, u, l(inear) [default: A-law]\n"));
  P(("  -core # ... Number of core bits in sample (2, 3, 4, 5) [dft: 4]\n"));
  P(("  -enh # .... Number of enhancement bits (0, 1, 2, 3) [dft: 0]\n"));
  P(("  -blk # .... Number of samples per processing block [def: 256]\n"));
  P(("  -frame # .. same as block size\n"));
  P(("  -start # ..  starting block to measure [default: first]\n"));
  P(("  -n # ......  number of blocks to be measured [default: all]\n"));
  P(("  -end # ....  last block to be measured [default: last]\n"));
  P(("  -q ........ Quiet operation (don't print progress flag)\n"));
  P(("  -?/-help .. Display program usage\n"));

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
int main(int argc, char **argv)
{
  short nc = 4, ne = 0; /* Core and enhancement bits, default to 32 kbit/s */
  int wordLen;
  char law = 'A';
#ifdef STATIC_ALLOCATION
  short           tmp_buf[256], inp_buf[256], out_buf[256];
#else
  short           *tmp_buf, *inp_buf, *out_buf;
#endif
  short           inp_type, out_type;
  g727_state      enc_state, dec_state;
  long            N = 256, N1 = 1, N2 = 0, cur_blk, smpno;

  /* General-purpose, progress indication */
  static char     quiet=0, funny[9] = "|/-\\|/-\\";
  char            encode=1, decode=1; /* Default: encode+decode operation */

  /* File variables */
  char            FileIn[MAX_STRLEN], FileOut[MAX_STRLEN];
  FILE           *Fi, *Fo;
  int             inp, out;
  long            start_byte;
#ifdef VMS
  char            mrs[15];
#endif

  /*
   * ......... PARAMETERS FOR PROCESSING .........
   */

#ifdef ORIGINAL_CODE
  if (argc < 7)
    display_usage();

  nc = atoi(argv[1]);
  ne = atoi(argv[2]);

  if (!(2 <= nc && nc <= 4 && ne >= 0 && nc + ne <= 5))
    error(badParamMsg);

  wordLen = nc + ne;
  sh += 2;
  mode = *argv[1 + sh];
  l = *argv[2 + sh];


  if ((mode != 'e' && mode != 'd' && mode != 'c') ||
      (l != 'a' && l != 'u' && l != 'A' && l != 'U'))
  {
    error(badParamMsg);
  }

  if ((in = fopen(argv[3 + sh], "rb")) == NULL ||
      (out = fopen(argv[4 + sh], "wb")) == NULL)
  {
    error("Can't open file");
  }

#endif
  /* GETTING OPTIONS */
  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1], "-frame") == 0 ||
	  strcmp(argv[1], "-blk") == 0)
      {
	/* Get skip length */
	N = atol (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-start") == 0)
      {
	/* Define starting block */
	N1 = atol (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-end") == 0)
      {
	/* Define last block */
	N2 = atol (argv[2]) - N1 + 1;

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp (argv[1], "-n") == 0)
      {
	/* Define number of samples to extract */
	N2 = atol (argv[2]);

	/* Move arg{c,v} over the option to the next argument */
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1], "-enc") == 0)
      {
	/* Encoder-only operation */
	encode = 1;
	decode = 0;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-dec") == 0)
      {
	/*Decoder-only operation */
	encode = 0;
	decode = 1;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-e_d") == 0)
      {
	/* Encode-and-decode operation */
	encode = 1;
	decode = 1;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-law") == 0)
      {
	/* Define law for operation: A, u, or linear */
	law = toupper((int)argv[2][0]);

	/* Move argv over the option to the next argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-frame") == 0 ||
	       strcmp(argv[1], "-blk") == 0)
      {
	/* Define Frame size for rate change during operation */
        N = atoi(argv[2]);

	/* Move argv over the option to the next argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-core") == 0)
      {
	/*Define number of core bits for operation */
        nc = atoi(argv[2]);

	/* Move argv over the option to the next argument */
	argv+=2;
	argc-=2;
      }
      else if (strcmp(argv[1], "-enh") == 0)
      {
	/*Define number of enhancement bits for operation */
        ne =  atoi(argv[2]);

	/* Move argv over the option to the next argument */
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

  /* Now get regular parameters */
  GET_PAR_S(1, "_Input File: .................. ", FileIn);
  GET_PAR_S(2, "_Output File: ................. ", FileOut);
  FIND_PAR_L(3, "_Block Size: .................. ", N, N);
  FIND_PAR_L(4, "_Starting Block: .............. ", N1, N1);
  FIND_PAR_L(5, "_No. of Blocks: ............... ", N2, N2);
  FIND_PAR_C(6, "_Law (A,u): ................... ", law, law); 
  FIND_PAR_I(7, "_Core bits: ................... ", nc, nc);
  FIND_PAR_I(8, "_Enhancement bits: ............ ", ne, ne);

  /* INITIALIZATIONS */

  /* Check consistency for number of core & enhancement bits */
  if (!(2 <= nc && nc <= 4 && ne >= 0 && nc + ne <= 5))
  {
    fprintf(stderr, "%s: (%d,%d)-Aborted\n",
	    "Inconsistent number of core and enhancement bits", nc, ne);
    exit(5);
  }

  /* Compose word length */
  wordLen = nc + ne;

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

  
  /* Convert law letter to number */
  switch (law)
  {
  case 'A':
	  law = '1';
	  break;
  case 'U':
	  law = '0';
	  break;
  case 'L':
	  law = '2';
	  break;
  default:
	  HARAKIRI(" Invalid law (A, u, or L)! Aborted...\n", 7);
  }
  
  /* Define correct data I/O types */
  if (encode && decode) 
  {  
    inp_type = out_type = (law == '2'? IS_LIN : IS_LOG);
  }
  else if (encode)     
  {
    inp_type = law == '2'? IS_LIN : IS_LOG;
    out_type = IS_ADPCM;
  }
  else     
  {
    inp_type = IS_ADPCM;
    out_type = law == '2'? IS_LIN : IS_LOG;
  }



  /* Force law to be used *by the ADPCM* to A-law, if input is linear */
  if (law=='2')
    law='1';

  /* Report codec operation mode */
  fprintf(stderr, "Operation: %s; Nc=%d; Ne=%d\n",
	  encode && decode? "Enc+Dec":
	  (encode? "Encode-only": "Decode-only"), nc, ne);

/*
 * ...... MEMORY ALLOCATION .........
 */
#ifndef STATIC_ALLOCATION
  if ((inp_buf = (short *) calloc(N, sizeof(short))) == NULL) 
     HARAKIRI("Error in memory allocation!\n",1);
  if ((out_buf = (short *) calloc(N, sizeof(short))) == NULL) 
     HARAKIRI("Error in memory allocation!\n",1);
  if ((tmp_buf = (short *) calloc(N, sizeof(short))) == NULL) 
     HARAKIRI("Error in memory allocation!\n",1);
#endif

/*
 * ......... FILE PREPARATION .........
 */

  /* Opening input file; abort if there's any problem */
  if ((Fi = fopen(FileIn, RB)) == NULL)
    KILL(FileIn, 2);
  inp = fileno(Fi);

  /* Creates output file */
#ifdef VMS
  sprintf(mrs, "mrs=%d", 512);
#endif
  if ((Fo = fopen(FileOut, WB)) == NULL)
    KILL(FileOut, 3);
  out = fileno(Fo);

  /* Move pointer to 1st block of interest */
  if (fseek(Fi, start_byte, 0) < 0l)
    KILL(FileIn, 4);

/*
 * ......... PROCESSING ACCORDING TO ITU-T G.727 .........
 */

  /* Reset state variables */
  g727_reset(&enc_state);
  g727_reset(&dec_state);

  /* Process all blocks defined by user */
  for (cur_blk = 0; cur_blk < N2; cur_blk++)
  {
    /* Print progress flag */
    if (!quiet)
#ifdef DISPLAY_CURRENT_RATE
      fprintf(stderr, "%d-", 8 * rate[rate_idx]);
#else
      fprintf(stderr, "%c\r", funny[cur_blk % 8]);
#endif

    /* Read a block of samples */
    if ((smpno = fread(inp_buf, sizeof(short), N, Fi)) < 0)
      KILL(FileIn, 5);

    /* Compress linear input samples */
    if (inp_type == IS_LIN)
    {
      /* Compress using A-law */
      alaw_compress(smpno, inp_buf, tmp_buf);

      /* copy temporary buffer over input buffer */
      memcpy(inp_buf, tmp_buf, sizeof(short) * smpno);
    }

    /* Carry out the desired operation */
    if (encode && ! decode)
      g727_encode(inp_buf, out_buf, smpno, law, 
		  nc, ne, &enc_state);
    else if (decode && !encode)
      g727_decode(inp_buf, out_buf, smpno, law, 
		  nc, ne, &dec_state);
    else if (encode && decode)
    {
      g727_encode(inp_buf, tmp_buf, smpno, law, 
		  nc, ne, &enc_state);
      g727_decode(tmp_buf, out_buf, smpno, law, 
		  nc, ne, &dec_state);
    }

    /* Expand linear input samples */
    if (out_type == IS_LIN)
    {
      /* Compress using A-law */
      alaw_expand(smpno, out_buf, tmp_buf);

      /* copy temporary buffer over input buffer */
      memcpy(out_buf, tmp_buf, sizeof(short) * smpno);
    }

    /* Write ADPCM output word */
    if ((smpno = fwrite(out_buf, sizeof(short), smpno, Fo)) < 0)
      KILL(FileOut, 6);
  }


/*
 * ......... FINALIZATIONS .........
 */

  /* Close input and output files */
  fclose(Fi);
  fclose(Fo);

  /* Exit with success for non-vms systems */
#ifndef VMS
  return (0);
#endif
}
/* ............................ End of main() ............................. */

