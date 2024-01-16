/*                                                           03.Feb.2010 v1.4
  ============================================================================

  G726DEMO.C
  ~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Demonstration program for UGST/ITU-T G.726 module (the same as the Blue 
  Book G.726). Takes the input file and processes by the G.726 codec, 
  depending on user's option: for encoding, input must be in either A 
  or mu law (G711), for decoding, in ADPCM format. The modules called have 
  been originally written in Fortran, and were translated into C by the 
  converter f2c, version of October 15, 1990 at 19:58:17.

  Input data is supposed to be aligned at word boundaries, i.e.,
  organized in 16-bit words, following the operating system normal
  organization (low-byte first for VMS and DOS; high byte first
  for most Unix systems). G711 compressed data is supposed to be
  in the 8 LEAST significant bits of the word and the ADPCM data
  is in the LEAST 5 bits. Both are without sign extension.

  Output data will be generated in the same format as decribed
  above for the input data.

  Usage:
  ~~~~~~
  $ G726demo [-options] Law Transf Rate InpFile OutFile 
  [BlockSize [1stBlock [NoOfBlocks [Reset]]]]
  where:
   Law       is the law desired (either A or u)
  Transf     is the desired conversion on the input file:
              [lolo], (A/u)log -> ADPCM -> (A/u) log
	      [load], (A/u)log -> ADPCM
	      [adlo], ADPCM    -> (A/u) log
  Rate       is the number of ADPCM bits per sample:
              [5],[40] -> 5 bits per sample (40 kbit/s)
	      [4],[32] -> 4 bits per sample (32 kbit/s)
	      [3],[24] -> 3 bits per sample (24 kbit/s)
	      [2],[16] -> 2 bits per sample (16 kbit/s)
  InpFile     is the name of the file to be processed;
  OutFile     is the name with the processed data;
  BlockSize   is the block size, in number of samples
  1stBlock    is the number of the first block of the input file
              to be processed;
  NoOfBlocks  is the number of blocks to be processed, starting on
              block "1stBlock"
  Reset       is the optional reset. If specified as 1, the
              coder and decoder will be reset at the very beginning of
	      the processings. If 0, the processing will start with
	      the variables at a unknown state. It defaults to 1
	      (reset ON).
  Options:
  -noreset    don't apply reset to the encoder/decoder
  -?/-help    print help message
        

  Example:
  $ G726demo u lolo  4 voice.ref voice.rel 256 3 45 *OR*
  $ G726demo u lolo 32 voice.ref voice.rel 256 3 45
  
  The command above takes the samples in file "voice.ref", already
  in mu law format, processes the data through the G726 encoder
  and decoder at a rate of 32 bkit/s,  saving them into the file
  "voice.rel". The processing starts  at block 3 for 45 blocks,
  each block being 256 samples wide.

  Original authors:
  ~~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto   EMail : simao@cpqd.ansp.br (32k)
  Fernando Tofolli Queiroz      EMail : tdfernan@cpqd.ansp.br (extension)

  CPqD/Telebras                 Rd. Mogi Mirim-Campinas Km.118
  DDS/Pr.11                     13.088-061 - Campinas - SP (Brazil)

  History:
  ~~~~~~~~
  28/Feb/1994 v1.0 Release of 1st version of this demo program
                   based on the g721demo of the STL92. G.726
                   functionality added by Fernando Toffoli Queiroz
                   <tdfernan@cpqd.ansp.br>; usage routine and
                   portability tests by Simao.
  22/Feb/1996 v1.1 Removed compilation warnings, included headers as
                   suggested by Kirchherr (FI/DBP Telekom) to run under
		   OpenVMS/AXP <simao@ctd.comsat.com>
  21/Mar/2000 v1.2 Changed memory allocation of floating point buffers
                   tmp_buf[], inp_buf[] and out_buf[] from static to
                   dynamic, to prevent memory invasion
                   when block sizes larger than 256 are specified. 
                   Corrected bug that made incorrect calculation on
                   total number of blocks to process when the block
                   size is not a multiple of the file
                   size. <simao.campos@labs.comsat.com>
  20/Oct/2005 v1.3 Filtering of the rate field (if rate is different 
                   of 2/3/4/5 or 16/24/32/40, the program stops).
                   <Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com>
  03/Feb/2010 v1.4 Modified maximum string length, removed implicit
                   casting of toupper(), and type of "rate" is int
                   (y.hiwasaki)
============================================================================ 
*/


/* ..... General definitions for UGST demo programs ..... */
#include "ugstdemo.h"

/* ..... General include ..... */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#if defined(VMS)
#include <stat.h>
#else				/* Unix/DOS */
#include <sys/stat.h>
#endif

/* ..... G.726 module as include functions ..... */
#include "g726.h"

/*
 -------------------------------------------------------------------------
 void display_usage(void);
 ~~~~~~~~~~~~~~~~~~
 Display proper usage for the demo program. Generated automatically from
 program documentation.

 History:
 ~~~~~~~~
 8.Mar.94 v1.0 Created.
 -------------------------------------------------------------------------
*/
#define P(x) printf x
void            display_usage()
{
  P(("G726DEMO - Version 1.4 of 03.Feb.2010 \n\n"));

  P(("> Description:\n"));
  P(("   Demonstration program for UGST/ITU-T G.726 module. Takes the\n"));
  P(("   input file and processes by the G.726 codec,\n"));
  P(("   depending on user's option: for encoding, input must be in\n"));
  P(("   either A or mu law (G711), for decoding, in ADPCM format. The\n"));
  P(("   modules called have been originally written in Fortran, and were\n"));
  P(("   translated into C by the converter f2c, version of October 15,1990\n"));
  P(("\n"));
  P(("   Input data is supposed to be aligned at word boundaries, i.e.,\n"));
  P(("   organized in 16-bit words, following the operating system normal\n"));
  P(("   organization (low-byte first for VMS and DOS; high byte first\n"));
  P(("   for most Unix systems). G711 compressed data is supposed to be\n"));
  P(("   in the 8 LEAST significant bits of the word and the ADPCM data\n"));
  P(("   is in the LEAST 5 bits. Both are without sign extension.\n"));
  P(("\n"));
  P(("   Output data will be generated in the same format as decribed\n"));
  P(("   above for the input data.\n"));
  P(("\n"));

  P(("> Usage:\n"));
  P(("$ G726demo [-options] Law Transf Rate InpFile OutFile \n"));
  P(("           [BlockSize [1stBlock [NoOfBlocks [Reset]]]]\n"));
  P((" where:\n"));
  P((" Law         is the law desired (either A or u)\n"));
  P((" Transf      is the desired conversion on the input file:\n"));
  P(("              [lolo], (A/u)log -> ADPCM -> (A/u) log\n"));
  P(("              [load], (A/u)log -> ADPCM  \n"));
  P(("              [adlo], ADPCM    -> (A/u) log\n"));
  P((" Rate        is the number of ADPCM bits per sample:\n"));
  P(("              [5],[40] -> 5 bits per sample (40 kbit/s)\n"));
  P(("              [4],[32] -> 4 bits per sample (32 kbit/s)\n"));
  P(("              [3],[24] -> 3 bits per sample (24 kbit/s)\n"));
  P(("              [2],[16] -> 2 bits per sample (16 kbit/s)\n"));
  P((" InpFile     is the name of the file to be processed;\n"));
  P((" OutFile     is the name with the processed data;\n"));
  P((" BlockSize   is the block size, in number of samples\n"));
  P((" 1stBlock    is the number of the first block of the input file\n"));
  P(("             to be processed;\n"));
  P((" NoOfBlocks  is the number of blocks to be processed, starting on\n"));
  P(("             block \"1stBlock\"\n"));
  P((" Reset       is the optional reset. If specified as 1, the\n"));
  P(("             coder and decoder will be  reset at the very\n"));
  P(("             beginning of the processings. If 0, the\n"));
  P(("             processing will start with the variables  at a\n"));
  P(("             unknown state. It defaults to 1 (reset ON). \n"));
  P((" Options: \n"));
  P((" -noreset    don't apply reset to the encoder/decoder\n"));
  P((" -? or -help print this help message\n"));
  P(("\n"));

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
int main(argc, argv)
  int             argc;
  char           *argv[];
{
  G726_state      encoder_state, decoder_state;
  long            N = 256, N1 = 1, N2 = 0, cur_blk, smpno;
  short           *tmp_buf, *inp_buf, *out_buf, reset=1;
  short           inp_type, out_type;
	int             rate;

  /* Progress indication */
  static char     quiet=0, funny[9] = "|/-\\|/-\\";

/* File variables */
  char            FileIn[MAX_STRLEN], FileOut[MAX_STRLEN], law[4], lilo[8];
  FILE           *Fi, *Fo;
  int             inp, out;
  long            start_byte;
#ifdef VMS
  char            mrs[15];
#endif

/*
 * ......... PARAMETERS FOR PROCESSING .........
 */

  /* GETTING OPTIONS */

  if (argc < 2)
    display_usage();
  else
  {
    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1], "-noreset") == 0)
      {
	/* No reset */
	reset = 0;

	/* Update argc/argv to next valid option/argument */
	argv++;
	argc--;
      }
      else if (strcmp(argv[1], "-q") == 0)
      {
	/* Don't print progress indicator */
	quiet = 1;

	/* Move argv over the option to the next argument */
	argv++;
	argc--;
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

  /* Now get regular parameters */
  GET_PAR_S(1, "_Law (A,u): ................... ", law);
  GET_PAR_S(2, "_Operation (lolo,load,adlo): .. ", lilo);
  GET_PAR_I(3, "_Rate or bits/ADPCM sample: ... ", rate);
  GET_PAR_S(4, "_Input File: .................. ", FileIn);
  GET_PAR_S(5, "_Output File: ................. ", FileOut);
  FIND_PAR_L(6, "_Block Size: .................. ", N, 256);
  FIND_PAR_L(7, "_Starting Block: .............. ", N1, 1);
  FIND_PAR_L(8, "_No. of Blocks: ............... ", N2, 0);
  FIND_PAR_I(9, "_Reset (YES=1, NO=0): ......... ", reset, 1);

  /* Find starting byte in file */
  start_byte = sizeof(short) * (long) (--N1) * (long) N;

  /* Check if is to process the whole file */
  if (N2 == 0)
  {
    struct stat     st;

    /* ... find the input file size ... */
    stat(FileIn, &st);
    N2 = ceil((st.st_size - start_byte) / (double)(N * sizeof(short)));
  }

  /* Classification of the conversion desired */
  inp_type = toupper((int)lilo[1]) == 'O' ? IS_LOG : IS_ADPCM;
  out_type = toupper((int)lilo[3]) == 'O' ? IS_LOG : IS_ADPCM;
  if ((out_type == IS_ADPCM) && (inp_type == IS_ADPCM))
    HARAKIRI("Bad conversion chosen (lolo,load,adlo)! Aborted...\n", 8);

  /* Classification of law */
  if (toupper((int)law[0]) == (char) 'A')
    law[0] = '1';
  else if (toupper((int)law[0]) == (char) 'U')
    law[0] = '0';
  else
    HARAKIRI(" Invalid law (A or u)! Aborted...\n", 7);

  /* Classification of rate */
  if (rate == 40)
      rate = 5;
  else if (rate == 32)
      rate = 4;
  else if (rate == 24)
      rate = 3;
  else if (rate == 16)
      rate = 2;
  else if ((rate != 2)&&(rate != 3)&&(rate != 4)&&(rate != 5))
  {
      HARAKIRI(" Invalid rate (5/4/3/2) or (40/32/24/16)! Aborted...\n", 9);
  }

/*
 * ...... MEMORY ALLOCATION .........
 */

  if ((inp_buf = (short *) calloc(N, sizeof(short))) == NULL) 
     HARAKIRI("Error in memory allocation!\n",1);
  if ((out_buf = (short *) calloc(N, sizeof(short))) == NULL) 
     HARAKIRI("Error in memory allocation!\n",1);
  if ((tmp_buf = (short *) calloc(N, sizeof(short))) == NULL) 
     HARAKIRI("Error in memory allocation!\n",1);

/*
 * ......... FILE PREPARATION .........
 */

  /* Opening input file; abort if there's any problem */
  if ((Fi = fopen(FileIn, "rb")) == NULL)
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
 * ......... PROCESSING ACCORDING TO ITU-T G.726 .........
 */

  for (cur_blk = 0; cur_blk < N2; cur_blk++)
  {
    /* Print progress flag */
    if (!quiet)
      fprintf(stderr, "%c\r", funny[cur_blk % 8]);

    /* Read a block of samples */
    if ((smpno = fread(inp_buf, sizeof(short), N, Fi)) < 0)
      KILL(FileIn, 5);

    /* Check if reset is needed */
    reset = (reset == 1 && cur_blk == 0) ? 1 : 0;

    /* Carry out the desired operation */
    if (inp_type == IS_LOG && out_type == IS_ADPCM)
    {
      G726_encode(inp_buf, out_buf, smpno, law, (short)rate, reset, &encoder_state);
    }
    else if (inp_type == IS_ADPCM && out_type == IS_LOG)
    {
      G726_decode(inp_buf, out_buf, smpno, law, (short)rate, reset, &decoder_state);
    }
    else if (inp_type == IS_LOG && out_type == IS_LOG)
    {
      G726_encode(inp_buf, tmp_buf, smpno, law, (short)rate, reset, &encoder_state);
      G726_decode(tmp_buf, out_buf, smpno, law, (short)rate, reset, &decoder_state);
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
/* ............................. end of main() ............................. */
