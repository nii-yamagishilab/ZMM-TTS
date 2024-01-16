/* *********************************************************************** */
/* FORTRAN77 main program SNR */
/* Task:      Compute signal to noise ratio, SNR, between two signal files. */
/*            Global, segmental and minimum SNRs are computed */
/* ----------------------------------------------------------------------- */
/* Input:     Reference signal file */
/*            Test signal file */
/* Output:    SNR values to terminal */
/* ----------------------------------------------------------------------- */
/* File handling:      through routines FOPEN, FREAD */
/*                     files on units 101 and 102 */
/* User communication: FORTRAN terminal I/O */
/* System interaction: none */
/* ----------------------------------------------------------------------- */
/* Subroutines called:                                       from: */
/*   FOPEN, FREAD                                            FILSUB */
/*   GLBSNR,SEGSNR,MINSNR                                   this file */
/* ----------------------------------------------------------------------- */
/* Version: 1                Date: 14.10.91        By: Finn Tore Johansen */
/* *********************************************************************** */
/* snr.f -- translated by f2c (version of 15 October 1990  19:58:17).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/
#include <stdio.h>
#include <math.h>
#include "ugstdemo.h"


/* Table of constant values */

static long     c__256 = 256;


int             main(argc, argv)
  int             argc;
  char           *argv[];
{
  /* Initialized data */

  static long     nlow = 0;
  static long     nseg = 0;
  static double   mn[7] = {200., 200., 200., 200., 200., 200., 200.};
  static long     imn[7] = {0, 0, 0, 0, 0, 0, 0};

  /* System generated locals */
  long            i_1;

  /* Builtin functions */
  long            s_wsfe(), do_fio(), e_wsfe(), s_rsfe(), e_rsfe(), s_wsle(), e_wsle(),
                  do_lio(), f_clos();

  /* Local variables */
  static long     i;
  static double   x[256], y[256];
  static char     infil1[80], infil2[80];
  extern int      glbsnr_();
  extern void     segsnr_();
  extern void     minsnr_();
  static long     ix1[256], ix2[256];
  static double   glb;
  static long     eof;
  static double   seg;
  static long     ifr;
  static short    buf1[256], buf2[256];
  long            samples;
  FILE           *f1, *f2;


  /* User communication */
  GET_PAR_S(1, "_Reference signal file name: ", infil1);
  GET_PAR_S(2, "_Test signal file name: ", infil2);

  /* Initialize files */
  if ((f1 = fopen(infil1, RB)) == NULL)
    KILL(infil1, 2);
  if ((f2 = fopen(infil2, RB)) == NULL)
    KILL(infil2, 3);

  ifr = 0;

  while ((i = fread(buf1, sizeof(short), (size_t) 256, f1)) > 0 &&
	 (i_1 = fread(buf2, sizeof(short), (size_t) 256, f2)) > 0)
  {
    /* Get the number of samples read in this data buffer */
    samples = (i <= i_1) ? i : i_1;

    /* Copy input signals to double buffer */
    for (i = 0; i < samples; ++i)
    {
      x[i] = (double) buf1[i];
      y[i] = (double) buf2[i] - (double) buf1[i];
    }
    /* Compute different SNRs, global, segmental (256 sample segments) and */
    /* minimum for block lengths 256,128,64,32,16,8 and 4 */
    ++ifr;
    glbsnr_(x, y, &samples, &glb);
    segsnr_(x, y, &samples, &seg, &nlow, &nseg);
    for (i = 0; i <= 6; ++i)
    {
      minsnr_(x, y, &samples, &i, &ifr, &mn[i], &imn[i]);
    }
  }

  /* Write results to terminal */
  if (glb == -1.)
  {
    printf("Zero input signal\n");
  }
  else if (glb == -2.)
  {
    printf("Files are identical\n");
  }
  else
  {
    printf("%s%s", "   SEG256  GLOB    MIN256  MIN128",
	   "  MIN64   MIN32   MIN16   MIN8    MIN4\n");
    printf("%8.2lf", seg);
    printf("%8.2lf", glb);
    for (i = 0; i <= 6; ++i)
    {
      printf("%8.2lf", mn[i]);
    }
    printf("\n   Min. segment:");
    for (i = 0; i < 7; ++i)
    {
      printf("%8ld", imn[i]);
    }
  }

  printf("\n   Active segments :%12ld", nseg);
  printf("\n   Total segments  :%12ld\n", nseg + nlow);

  /* Finalizations */
  fclose(f1);
  fclose(f2);
#ifndef VMS
  return (0);
#endif
}
/* ....................... End of main() ................................ */


/* Compute global SNR for file */
/* Return 0 if SNR, -1 if silence (no signal) and -2 if snr->oo */
int             glbsnr_(x, y, nfr, glb)
  double         *x, *y;
  long           *nfr;
  double         *glb;
{
  /* Initialized data */

  static double   xp = 0.;
  static double   yp = 0.;

  /* System generated locals */
  long            i_1;
  double          d_1;

  /* Local variables */
  static long     i;

  i_1 = *nfr - 1;
  for (i = 0; i <= i_1; ++i)
  {
    /* Computing 2nd power */
    d_1 = x[i];
    xp += d_1 * d_1 / *nfr;
    /* Computing 2nd power */
    d_1 = y[i];
    yp += d_1 * d_1 / *nfr;
    /* L10: */
  }

  /* Check limit values and calculate SNR */
  if (xp <= 0.)
  {
    /* No signal - silence */
    *glb = (double) -1.;
    return -1;
  }
  else if (yp <= 0.)
  {
    /* Signals are identical: snr -> oo */
    *glb = (double) -2.;
    return -2;
  }
  else
  {
    /* OK, they are "regular" signals */
    d_1 = xp / yp;
    *glb = log10(d_1) * 10;
  }

  /* Return value */
  return 0;
}
/* .......................... End of glbsnr() ........................... */


/* Compute average segmental SNR for segments above threshold */
void            segsnr_(x, y, nfr, seg, nlow, nseg)
  double         *x, *y;
  long           *nfr;
  double         *seg;
  long           *nlow, *nseg;
{
  /* Initialized data */
  static double   sum = 0.;

  /* System generated locals */
  long            i_1;
  double          d_1;

  /* Local variables */
  static long     i;
  static double   xp, yp, maxsnr, xth;

  xth = (double) 1e4;
  maxsnr = (double) 200.;
  xp = (double) 0.;
  yp = (double) 0.;
  i_1 = *nfr - 1;
  for (i = 0; i <= i_1; ++i)
  {
    /* Computing 2nd powers */
    d_1 = x[i];
    xp += d_1 * d_1;
    d_1 = y[i];
    yp += d_1 * d_1;
  }
  xp /= *nfr;
  yp /= *nfr;
  if (xp <= xth)
  {
    ++(*nlow);
  }
  else
  {
    ++(*nseg);
    if (yp > 0.)
    {
      d_1 = xp / yp;
      sum += log10(d_1) * 10;
    }
    else
    {
      sum += maxsnr;
    }
    *seg = sum / *nseg;
  }
}
/* ........................... End of segsnr() ........................... */


/* Compute minimum block SNR for blocks with energy above threshold */
void            minsnr_(x, y, nfr, nsub, ifr, mn, imn)
  double         *x, *y;
  long           *nfr, *nsub, *ifr;
  double         *mn;
  long           *imn;
{
  /* System generated locals */
  long            i_1, i_2;
  double          d_1;

  /* Local variables */
  static long     segl, i, n, i0;
  static double   xp, yp, maxsnr, snr, xth;

  xth = (double) 1e4;
  maxsnr = (double) 200.;
  d_1 = pow((double) 2., (double) (*nsub));
  segl = *nfr / d_1;
  i_1 = d_1 - 1;
  for (n = 0; n <= i_1; ++n)
  {
    i0 = n * segl;
    xp = (double) 0.;
    yp = (double) 0.;
    i_2 = i0 + segl - 1;
    for (i = i0; i <= i_2; ++i)
    {
      /* Computing 2nd powers */
      d_1 = x[i];
      xp += d_1 * d_1;
      d_1 = y[i];
      yp += d_1 * d_1;
    }
    xp /= segl;
    yp /= segl;

    if (xp > xth)
    {
      if (yp <= 0.)
      {
	snr = maxsnr;
      }
      else
      {
	d_1 = xp / yp;
	snr = log10(d_1) * 10;
      }
      if (snr < *mn)
      {
	*mn = snr;
	*imn = *ifr;
      }
    }
  }
}
/* ........................... End of minsnr() ........................... */
