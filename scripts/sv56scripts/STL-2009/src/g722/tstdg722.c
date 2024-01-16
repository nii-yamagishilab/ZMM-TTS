/*                                                            v2.0 - 01.Jul.95
  ============================================================================

  TSTCG722.C 
  ~~~~~~~~~~

  Description: 
  ~~~~~~~~~~~~
  
  Demonstration program to pass the official set of test vectors
  through the G.722 decoder. This code *bypasses* the QMF filters, as
  required by the test vector.

  The test vectors are supposed to be in binary format, 16-bit, 2s'
  complement data.

  Usage:
  ~~~~~~
  $ tstcg722 InpFile OutFile ModeFile

  where:
  InpFile     bitstream to be decoded
  OutFile     decoded bitstream filename
  ModeFile    G.722 mode file

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
#include <string.h>

#include "ugstdemo.h"

/***********************/
/* Function prototypes */
/***********************/
#include "g722.h"
#include "operg722.h"
#include "funcg722.h"

#if defined MSDOS
#define PATH_SEP '\\'
#elif defined VMS
#define PATH_SEP ']'
#else
#define PATH_SEP '/'
#endif


/***************************************/
/* Test of the G.722 decoder algorithm */
/***************************************/
int             main (argc, argv)
int argc; 
char *argv[];
{

  /* declarations locales */
  /************************/

  Word16          il, ih, mode, rl, rh, rs, incode;
  FILE           *cod, *rcl, *rch;
  int             read1;
  long            iter, error;
  char           *usr_mode, *c;
  g722_state      decoder;

  /* debut du code executable */
  /****************************/

  if (argc != 4)
  {
    printf ("\n Mauvais usage de TESTD64K");
    printf ("\n USE: TESTD64K fichx.COD fichy.RCx. fichy.RC0.. EXIT");
    exit (0);
  }

  if ((cod = fopen (argv[1], "r+b")) == NULL)
  {
    printf ("TESTD64K ne peut pas ouvrir %s \n", argv[1]);
    exit (0);
  }

  if ((rcl = fopen (argv[2], "r+b")) == NULL)
  {
    printf ("TESTD64K ne peut pas ouvrir %s \n", argv[2]);
    exit (0);
  }

  if ((rch = fopen (argv[3], "r+b")) == NULL)
  {
    printf ("TESTD64K ne peut pas ouvrir %s \n", argv[3]);
    exit (0);
  }

  /* determination du mode du decodeur bande basse */
  /*************************************************/

  /* parse directories in name */
  c = argv[2];
  usr_mode = (char *)NULL;
  while ((c=strpbrk(c, "]\\/"))!=NULL)
  { 
    usr_mode = c++;
  }

  switch (usr_mode == NULL ? argv[2][8] : *(usr_mode + 9))
  {
  case '1':
    mode = 1;
    break;

  case '2':
    mode = 2;
    break;

  case '3':
    mode = 3;
    break;

  default:
    fprintf (stderr, "\n BAD CONVERSION OF MODE OF SB DECODER... EXIT\n");
    exit (0);
  }

  iter = error = 0L;

  /* boucle de lecture d'un mot ? coder puis comparaison avec reference */
  /**********************************************************************/

  printf ("\n\n");
  printf ("\n***************************************************************");
  printf ("\n* TESTING OF THE DECODER OF ITU-T G722 WIDE BAND SPECH CODER  *");
  printf ("\n***************************************************************");
  printf ("\n\n  BEGINING OF PROCESSING INPUT FILE %s : REF_L %s  REF_H %s\n",
	  argv[1], argv[2], argv[3]);

  while ((read1 = fread (&incode, sizeof (Word16), 1, cod)) == 1)
  {

    if (read1 != 1)
    {
      printf ("\n Erreur de lecture fichier COD");
      exit (0);
    }

    /* calcul du reset : !! Attention RESET ITU-T ACTIF = 1  */
		/*********************************************************/

    rs = (incode & 1);

    /* cadrage des entrees */
		/***********************/

    il = (incode >> 8) & 0x003F;	/* 6 bits de code SBL */
    ih = ((incode >> 8) >> 6) & 0x0003;		/* 2 bits de code SBH */

    /* appel des decodeurs SBL et SBH */
		/**********************************/

    rl = lsbdec (il, mode, rs, &decoder);
    rh = hsbdec (ih, rs, &decoder);

    /* mise en forme des codes de sortie ; si rs actif alors code = 1 */
		/****************************************************************/

    if (rs == 1)
    {
      rl = rh = 1;
    }
    else
    {
      rl = (rl << 1) & 0xFFFE;	/* decalage des sorties  */
      rh = (rh << 1) & 0xFFFE;	/* idem bande basse      */
    }

    /* lecture du code de reference bande basse */
		/********************************************/

    if (fread (&incode, sizeof (Word16), 1, rcl) != 1)
    {
      printf ("\n Erreur de lecture fichier RCx");
      exit (0);
    }

    /* comparaison avec le code de reference bande basse */
		/*****************************************************/

    if (incode != rl)
    {
      printf ("\nError : sbl_C = %04X  sbh_CCITT = %04X  ", rl, incode);
      error++;
      printf ("\n ITERATION %ld", iter + 1);
      printf (" CONT ?");
      /* getch(); */
    }

    /* lecture du code de reference bande haute */
		/********************************************/

    if (fread (&incode, sizeof (Word16), 1, rch) != 1)
    {
      printf ("\n Erreur de lecture fichier RC0");
      exit (0);
    }

    /* comparaison avec le code de reference bande haute */
		/*****************************************************/

    if (incode != rh)
    {
      printf ("\nError : sbh_C = %04X  sbh_CCITT = %04X  ", rl, incode);
      error++;
      printf ("\n ITERATION %ld", iter + 1);
      printf (" CONT ?");
      /* getch(); */
    }

    iter++;

    if (iter % 512L == 0L)
    {
      printf ("\r OK ITERATION %ld ", iter);
    }

  }
  fclose (cod);
  fclose (rcl);
  fclose (rch);
  printf ("\n\n  END OF PROCESSING INPUT FILE %s : REF_L %s  REF_H %s\n",
	  argv[1], argv[2], argv[3]);


  printf ("\n NUMBER OF ERROR(s) = %ld ", error);
  printf ("  ON %ld SAMPLES; SB decoder mode = %1d", iter, mode);

  printf (" \n END OF PROCESS");

  printf ("\n*************************************************************\n");

  return ((int) error);

}
