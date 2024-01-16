/* *********************************************************************** */
/* FORTRAN77 main program LDCDEC */
/* Task:      Reference LD-CELP decoder. */
/*            A binary codeword file is read, the codewords are put through */
/*            the LD-CELP decoder, and a 16 bit output signal is written */
/*            to the output binary file. */
/* ----------------------------------------------------------------------- */
/* Input:     Codeword file */
/* Output:    Signal file */
/* ----------------------------------------------------------------------- */
/* File handling:      through routines FOPEN, FREAD, FWRITE */
/*                     files on units 101 and 102 */
/* User communication: FORTRAN terminal I/O */
/* System interaction: none */
/* ----------------------------------------------------------------------- */
/* Subroutines called:                                       from: */
/*   RESET, DECODE                                           LDCSUB */
/*   FOPEN, FREAD, FWRITE                                    FILSUB */
/* ----------------------------------------------------------------------- */
/* Version: 1                Date: 08.10.91        By: Finn Tore Johansen */
/* *********************************************************************** */
/* ldcdec.f -- translated by f2c (version of 15 October 1990  19:58:17).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/
#include <stdio.h>
#include "ugstdemo.h"


/* Table of constant values */
static long     c__1 = 1;


int             main(argc, argv)
  int             argc;
  char           *argv[];
{
  /* System generated locals */
  double          d_1;

  /* Local variables */
  extern short    round();
  extern int      reset_(), decode_();
  extern int      pstflt_();
  static char     infile[80];
  static char     outfil[80];
  static double   y[5], ypp[5];
  static long     c, i, n, codes, codeno;
  short           buf[256];
  int             ipf;
  static short    yi[5];
  FILE           *fi, *fo;


  /* User communication */
  GET_PAR_S(1, "_Input codeword file name: ", infile);
  GET_PAR_S(2, "_Output signal file name: ", outfil);
  GET_PAR_I(3, "_Include adaptive postfilter (0/1): ", ipf);

  /* Initialize files */
  if ((fi = fopen(infile, RB)) == NULL)
    KILL(infile, 2);
  if ((fo = fopen(outfil, WB)) == NULL)
    KILL(outfil, 3);

  /* Reset reference model parameters */
  reset_(&n);

  /* Read input codeword from file */
  while ((codes=fread(buf, sizeof(short), (size_t) 256, fi))>0)
  {
    for (codeno = 0; codeno < codes; ++codeno)
    {
      /* Copy code to variable */
      c = (long) buf[codeno];

      /* Pass signal through LD-CELP decoder */
      decode_(&n, &c, y, &c__1);

      /* Pass signal through adaptive postfilter */
      if (ipf == 1)
      {
	/* Copy decoded samples to scratch */
	for (i = 0; i <= 4; ++i)
	  ypp[i] = y[i];
	pstflt_(&n, ypp, y);
      }

      /* Scale output to 16 bit file level */
      for (i = 0; i <= 4; ++i)
      {
	d_1 = y[i] * (double) 8.;
	yi[i] = round(d_1);
      }

      /* Write 5 samples to file */
      if (fwrite(yi, sizeof(short), (size_t) 5, fo) != 5)
	HARAKIRI("Error saving decoded samples to file", 12);
    }
    ++n;
  }

  fclose(fi);
  fclose(fo);
}
/* ......................... End of main() ................................ */
