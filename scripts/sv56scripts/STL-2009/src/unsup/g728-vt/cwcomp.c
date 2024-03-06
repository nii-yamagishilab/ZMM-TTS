/* *********************************************************************** */
/* FORTRAN77 main program CWCOMP */
/* Task:      Compare samples in two 16 bit binary files. */
/*            Used to check encoder output codewords against reference. */
/* ----------------------------------------------------------------------- */
/* Input:     Reference file */
/*            Test file */
/* Output:    Number of different samples to terminal */
/* ----------------------------------------------------------------------- */
/* File handling:      through routines FOPEN, FREAD */
/*                     files on units 101 and 102 */
/* User communication: FORTRAN terminal I/O */
/* System interaction: none */
/* ----------------------------------------------------------------------- */
/* Subroutines called:                                       from: */
/*   FOPEN, FREAD                                            FILSUB */
/* ----------------------------------------------------------------------- */
/* Version: 1                Date: 09.10.91        By: Finn Tore Johansen */
/* *********************************************************************** */
/* cwcomp.f -- translated by f2c (version of 15 October 1990  19:58:17).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/
#include "ugstdemo.h"
#include <stdio.h>


int             main(argc, argv)
  int             argc;
  char           *argv[];
{
  /* Local variables */
  static long     ferr, nerr, n;
  static long     first;
  static char     infil1[80], infil2[80];
  static long     cw1, cw2, eof;
  short buf1[256], buf2[256];
  long            i1, i2, samples, codeno;
  FILE           *f1, *f2;


  /* User communication */
  GET_PAR_S(1, "_Reference codewords file name: ", infil1);
  GET_PAR_S(2, "_Test codewords file name: ", infil2);

  /* Initialize files */
  if ((f1 = fopen(infil1, RB)) == NULL)
    KILL(infil1, 2);
  if ((f2 = fopen(infil2, RB)) == NULL)
    KILL(infil2, 3);
  n = 1;
  nerr = 0;
  first = 1;

  while ((i1 = fread(buf1, sizeof(short), (size_t) 256, f1))>0 &&
	 (i2 = fread(buf2, sizeof(short), (size_t) 256, f2))>0)
  {
    /* Get the number of samples read in this data buffer */
    samples = (i1 <= i2)? i1 : i2;
    
    for (codeno = 0; codeno < samples; ++codeno)
    {
      /* Copy codewords to variables */
      cw1 = (long) buf1[codeno];
      cw2 = (long) buf2[codeno];

      if (cw1 != cw2)
      {
	++nerr;
	if (first == 1)
	{
	  first = 0;
	  ferr = n;
	}
      }
      ++n;
    }
  }

  printf("%12ld codewords compared\n", n - 1);
  printf("%12ld different codewords found\n", nerr);

  if (first == 0)
  {
    printf("First error position: %ld\n", ferr);
  }

  /* Finalizations */
  fclose(f1);
  fclose(f2);
#ifndef VMS
  return (0);
#endif
}
/* ....................... End of main() ................................ */
