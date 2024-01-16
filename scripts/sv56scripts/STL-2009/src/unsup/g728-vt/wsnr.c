/* *********************************************************************** */
/* FORTRAN77 main program WSNR */
/* Task:      Compute LD-CELP distortion measure (weighted signal to noise */
/*            ratio, WSNR) for a codeword sequence. */
/*            Codewords and input signal vectors are read from files, */
/*            the codewords are put through the LD-CELP decoder and the */
/*            distortion measure is calculated for each output vector. */
/*            The average segmental WSNR is then computed on five-sample */
/*            segments. */
/* Input:     Signal file */
/*            Codeword file */
/* Output:    WSNR value to terminal */
/* ----------------------------------------------------------------------- */
/* File handling:      through routines FOPEN, FREAD */
/*                     files on units 101 and 102 */
/* System interaction: none */
/* ----------------------------------------------------------------------- */
/* Subroutines called:                                       from: */
/*   RESET, DISTM                                            LDCSUB */
/*   FOPEN, FREAD                                            FILSUB */
/* ----------------------------------------------------------------------- */
/* Version: 1                Date: 23.10.91        By: Finn Tore Johansen */
/* *********************************************************************** */
/* wsnr.f -- translated by f2c (version of 15 October 1990  19:58:17).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/
#include <stdio.h>
#include <math.h>
#include "ugstdemo.h"



/* Table of constant values */

static long     c__1 = 1;
static long     c__101 = 101;
static long     c__0 = 0;
static long     c__102 = 102;
static long     c__3 = 3;
static long     c__9 = 9;
static long     c__4 = 4;



int             main(argc, argv)
  int             argc;
  char           *argv[];
{
  /* System generated locals */
  float           r_1;
  double          d_1;

  /* Local variables */
  static double   dmin_;
  static long     nseg;
  static double   spow;
  static long     c;
  static double   d;
  static long     i, n;
  static double   x[5];
  extern int      distm_(), reset_();
  static char     infil1[80], infil2[80];
  static double   sth, snr;
  static long     pos1, pos2;
  static short    ix[5], eof;
  static short    buf1[256], buf2[256], s_tmp;
  long            samples;
  FILE           *f1, *f2;
  int             i1, i2;


  /* Initializations */
  dmin_ = (double) .01;
  sth = (double) 10.;

  /* User communication */
  GET_PAR_S(1, "_Input signal file name:   ", infil1);
  GET_PAR_S(2, "_Input codeword file name: ", infil2);

  /* Initialize files */
  if ((f1 = fopen(infil1, RB)) == NULL)
    KILL(infil1, 2);
  if ((f2 = fopen(infil2, RB)) == NULL)
    KILL(infil2, 3);

  pos1 = 0;
  pos2 = 0;

  /* Reset reference model parameters */
  reset_(&n);
  snr = (double) 0.;

  /* Read signal and codewords from file */
  while ((i1 = fread(ix, sizeof(short), (size_t) 5, f1)) > 0 &&
	 (i2 = fread(&s_tmp, sizeof(short), (size_t) 1, f2)) > 0)
  {
    /* Convert to long */
    c = s_tmp;

    /* Compute signal power, distortion measure and vector WSNR */
    spow = (double) 0.;
    for (i = 0; i < 5; ++i)
    {
      d_1 = ix[i] / (double) 8.;
      spow += d_1 * d_1;
    }
    spow = sqrt(spow / (double) 5.);

    /* Calculate distance */
    distm_(&n, x, &c, &d);
    if (d < dmin_)
    {
      d = dmin_;
    }
    if (spow > sth)
    {
      d_1 = spow / d;
      snr += log10(d_1) * 20;
      ++nseg;
    }
    ++n;
  }

  /* Compute segmental WSNR */
  if (nseg > 0)
  {
    snr /= nseg;
  }

  /* Print results */
  printf("%12ld segments above threshold, WSNR =%f\n", nseg, snr);

  /* Finalizations */
  fclose(f1);
  fclose(f2);
#ifndef VMS
  return (0);
#endif
}
/* ....................... End of main() ................................ */
