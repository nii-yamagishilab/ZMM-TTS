/*                                                            v2.0 - 01.Jul.95
  ============================================================================

  TSTCG722.C 
  ~~~~~~~~~~

  Description: 
  ~~~~~~~~~~~~
  
  Demonstration program to pass the official set of test vectors
  through the G.722 encoder. This code *bypasses* the QMF filters, as
  required by the test vector.

  The test vectors are supposed to be in binary format, 16-bit, 2s'
  complement data.

  Usage:
  ~~~~~~
  $ tstcg722 InpFile OutFile 

  where:
  InpFile     is the name of the file to be processed;
  OutFile     is the name with the processed data;

  Original author:
  ~~~~~~~~~~~~~~~~
  J-P PETIT 
  CNET - Centre Lannion A
  LAA-TSS                         Tel: +33-96-05-39-41
  Route de Tregastel - BP 40      Fax: +33-96-05-13-16
  F-22301 Lannion CEDEX           Email: petitjp@lannion.cnet.fr
  FRANCE
    
  History:
  14.Mar.95    v1.0    Released for use ITU-T UGST software package Tool
                       based on the CNET's 07/01/90 version 2.00
  01.Jul.95    v2.0    Changed function declarations to work with 
                       many compilers; reformated <simao@ctd.comsat.com>
  ============================================================================ 
*/



/*********************************/
/*  Stantard Function prototypes */
/*********************************/
#include <stdio.h>
#include <stdlib.h>

/***********************/
/* Function prototypes */
/***********************/
#include "g722.h"
#include "operg722.h"
#include "funcg722.h"


/*************************************/
/* Test of the G.722 coder algorithm */
/*************************************/
int             main (argc, argv)
int argc; 
char *argv[];
{

/* declarations locales */
/************************/

  Word16          xl, inl, inh, xh, il, ih, incode;
  Word16          rs, codeC, code;
  g722_state      encoder;
  FILE           *xmt, *cod;
  int             read1;
  long            iter;

  /* debut du code executable */
	/****************************/

  if (argc != 3)
  {
    printf ("\n Bad use of TSTCG722");
    printf ("\n USE: TSTG722 fichx.XMT fichy.COD ... EXIT");
    exit (0);
  }

  if ((xmt = fopen (argv[1], "r+b")) == NULL)
  {
    printf ("TSTG722 ne peut pas ouvrir %s \n", argv[1]);
    exit (0);
  }

  if ((cod = fopen (argv[2], "r+b")) == NULL)
  {
    printf ("TSTG722 ne peut pas ouvrir %s \n", argv[2]);
    exit (0);
  }



  printf ("\n\n  BEGINING OF PROCESSING INPUT FILE %s : REFERENCE %s \n",
	  argv[1], argv[2]);

  xl = xh = rs = 1;
  il = lsbcod (xl, rs, &encoder);
  ih = hsbcod (xl, rs, &encoder);
  iter = 0L;

  while ((read1 = fread (&incode, sizeof (Word16), 1, xmt)) == 1)
  {

    if (read1 != 1)
    {
      printf ("\n Error read file XMT");
      exit (0);
    }

    rs = incode & 1;

    inl = incode;
    inh = incode;

    xl = shr (inl, 1);
    xh = shr (inh, 1);

    il = lsbcod (xl, rs, &encoder);
    ih = hsbcod (xh, rs, &encoder);

    if (rs == 1)
    {
      codeC = 1;
    }
    else
    {
      codeC = (((ih << 6) + il) << 8) & 0xFF00;
    }


    /* lecture du code de reference */
		/********************************/

    if (fread (&code, sizeof (Word16), 1, cod) != 1)
    {
      printf ("\n Error Read File COD");
      exit (0);
    }

    /* comparaison avec le code de reference */
		/*****************************************/

    if (code != codeC)
    {
      printf ("\nError iter=%ld C = %04X  REF = %04X  ", iter, codeC, code);
    }

    iter++;

    if (iter % 512L == 0L)
    {
      printf ("\r OK ITERATION %ld ", iter);
    }
  }

  printf ("TEST OK\n");
  fclose (xmt);
  fclose (cod);
  return (0);
}
