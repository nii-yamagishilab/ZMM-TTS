/* ldcsub.f -- translated by f2c (version of 15 October 1990  19:58:17).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include <stdio.h>
#include <math.h>
#include "ugstdemo.h"

short           round(x)
  double          x;
{
  return (short) (x + 0.5 * ((x > 0) ? 1 : -1));
}

/* Common Block Declarations */
struct
{
  double          wnc;
}               global_;

#define global_1 global_

struct
{
  double          wg[34], rhog, lambdg[10], dltmin, dltmax, doffst, ws[105], rhos,
                  lambds[50], ymin, ymax, codebk[5][1024], dcstat[290];
}               decod_;

#define decod_1 decod_

struct
{
  long            ifailg, ifails;
  double          deltaw[34], rdelta[11], sigma, e[5], yw[105], ry[51];
}               decmon_;

#define decmon_1 decmon_

struct
{
  double          wp[60], rhop, gamma1[10], gamma2[10], dmstat[143];
}               dist_;

#define dist_1 dist_

struct
{
  long            ifailp;
  double          xw[60], rx[11], q[11];
}               dismon_;

#define dismon_1 dismon_

/* Table of constant values */

static double   c_b5 = .90625;
static double   c_b9 = .98828125;
static double   c_b43 = .9;
static double   c_b47 = .6;
static long     c__0 = 0;
static long     c__1 = 1;
static long     c__10 = 10;
static double   c_b282 = 10.;
static long     c__50 = 50;

/* File:            LDCSUB.FOR */
/* Last modified:   January 6, 1992 */
/* By: 		   Finn Tore Johansen */
/* 		   Norwegian Telecom Research */
/* 		   <ftj@bilde.nta.no> */
/* Content:        Fortran subroutines for the LD-CELP verification programs: */
/* 	RESET		Constants definition and state reset subroutine */
/* 	ENCODE		LD-CELP encoder subroutine (using stupid full search) */
/* 	DISTM		LD-CELP distance measure subroutine */
/*       PWFILT          Perceptual weighting filter subroutine */
/* 	DECODE		LD-CELP decoder subroutine */
/* 	PSTFLT		Postfilter subroutine */
/* 	DURBIN		Durbin recursion subroutine */
/* 	HYBWG		Hybrid window for gain predictor */
/* 	HYBWS		Hybrid window for synthesis filter */
/* 	HYBWP		Hybrid window for perceptual weighting filter */
/* 	CBOOK		Shape codebook definition subroutine */
/* Version 06.01.92 / Ftj */


/* Reset decoder and distance measure states, assign constants */
int             reset_(n)
  long           *n;
{
  /* System generated locals */
  long            i_1, i_2, i_3;
  double          d_1;

  /* Builtin functions */
  long            s_rnge();
  double          pow_di();
  long            i_dnnt();

  /* Local variables */
#define ag ((double *)&decod_1 + 5372)
#define as ((double *)&decod_1 + 5445)
#define delta ((double *)&decod_1 + 5326)
#define dltbuf ((double *)&decod_1 + 5337)
#define q1 ((double *)&dist_1 + 81)
#define q2 ((double *)&dist_1 + 92)
#define rdexp ((double *)&decod_1 + 5383)
#define rxexp ((double *)&dist_1 + 133)
#define ryexp ((double *)&decod_1 + 5394)
#define xbuf ((double *)&dist_1 + 144)
#define xp ((double *)&dist_1 + 209)
#define ybuf ((double *)&decod_1 + 5496)
#define ybufp ((double *)&dist_1 + 103)
#define yp ((double *)&dist_1 + 118)

  extern void     cbook_();
  extern void     hybwg_();
  extern void     hybwp_(), hybws_();
  static double   g[8];
  static double   shape[640];
  static long     i, j, m;


  /* Constants */
  global_1.wnc = (double) 257. / 256.;

  /* Decoder parameters */
  *n = 0;
  for (i = 1; i <= 10; ++i)
  {
    d_1 = pow(c_b5, (double) i) * (double) 16384.;
    decod_1.lambdg[i - 1] = (double) round(d_1) / (double) 16384.;
  }

  decod_1.dltmin = (double) 0.;
  decod_1.dltmax = (double) 60.;
  decod_1.doffst = (double) 32.;
  for (i = 1; i <= 50; ++i)
  {
    d_1 = pow(c_b9, (double) i) * (double) 16384.;
    decod_1.lambds[i - 1] = (double) round(d_1) / (double) 16384.;
  }

  decod_1.ymin = (double) -4095.;
  decod_1.ymax = (double) 4095.;
  decod_1.rhog = (double) .75;
  decod_1.rhos = (double) .75;

  /* ...codebook */
  hybws_(decod_1.ws);
  hybwg_(decod_1.wg);
  g[0] = (double) .515625;
  g[1] = g[0] * 7 / (double) 4.;
  g[2] = g[1] * 7 / (double) 4.;
  g[3] = g[2] * 7 / (double) 4.;
  g[4] = -g[0];
  g[5] = -g[1];
  g[6] = -g[2];
  g[7] = -g[3];
  cbook_(shape);
  for (m = 0; m <= 4; ++m)
  {
    for (i = 0; i <= 7; ++i)
    {
      for (j = 0; j <= 127; ++j)
      {
	i_1 = (8 * j) + i;
	i_2 = (5 * j) + m;
	decod_1.codebk[m][i_1] = g[i] * shape[i_2];
      }
    }

    /* State variables */
    /* ...gain predictor */
  }
  /* ...special treatment of initial values to get correct start-up */
  for (i = -34; i <= -1; ++i)
  {
    dltbuf[(i_1 = i + 34)] = (double) 0.;
  }
  dltbuf[33] = -decod_1.doffst;
  dltbuf[32] = -decod_1.doffst;
  dltbuf[31] = -decod_1.doffst;
  for (i = -10; i <= -1; ++i)
  {
    delta[(i_1 = i + 10)] = -decod_1.doffst;
  }
  for (i = 0; i <= 10; ++i)
  {
    rdexp[i] = (double) 0.;
  }
  ag[0] = (double) 1.;
  ag[1] = (double) -1.;

  /* ...synthesis filter */
  for (i = 2; i <= 10; ++i)
  {
    ag[i] = (double) 0.;
  }

  for (i = 0; i <= 50; ++i)
  {
    ryexp[i] = (double) 0.;
  }

  as[0] = (double) 1.;

  for (i = 1; i <= 50; ++i)
  {
    as[i] = (double) 0.;
  }

  /* Perceptual filter parameters */
  /* Constants */
  for (i = -115; i <= 4; ++i)
  {
    ybuf[(i_1 = i + 115)] = (double) 0.;
  }

  for (i = 1; i <= 10; ++i)
  {
    d_1 = pow(c_b43, (double) i) * (double) 16384.;
    dist_1.gamma1[(i_1 = i - 1)] = (double) i_dnnt(&d_1) / (double) 16384.;
  }

  for (i = 1; i <= 10; ++i)
  {
    d_1 = pow(c_b47, (double) i) * (double) 16384.;
    dist_1.gamma2[(i_1 = i - 1)] = (double) round(d_1) / (double) 16384.;
  }

  /* State variables */
  /* ...Perceptual filter */
  dist_1.rhop = (double) .5;
  hybwp_(dist_1.wp);
  for (i = -60; i <= 4; ++i)
  {
    xbuf[(i_1 = i + 60)] = (double) 0.;
  }
  for (i = 0; i <= 10; ++i)
  {
    rxexp[i] = (double) 0.;
  }
  for (i = -10; i <= -1; ++i)
  {
    xp[(i_1 = i + 10)] = (double) 0.;
  }
  for (i = -10; i <= -1; ++i)
  {
    ybufp[(i_1 = i + 10)] = (double) 0.;
  }
  for (i = -10; i <= -1; ++i)
  {
    yp[(i_1 = i + 10)] = (double) 0.;
  }
  q1[0] = (double) 1.;
  for (i = 1; i <= 10; ++i)
  {
    q1[i] = (double) 0.;
  }
  q2[0] = (double) 1.;
  for (i = 1; i <= 10; ++i)
  {
    q2[i] = (double) 0.;
  }
  /* Version 03.10.91 / Ftj */
}
#undef dltbuf
#undef xp
#undef yp
#undef as
#undef ag
#undef ryexp
#undef rxexp
#undef q2
#undef q1
#undef ybufp
#undef rdexp
#undef delta
#undef ybuf
#undef xbuf
/* .......................... End of reset() ............................. */


/* LDCELP encoder */
/* Version 03.10.91 / Ftj */
int             encode_(n, x, c)
  long           *n;
  double         *x;
  long           *c;
{
  /* System generated locals */
  long            i_1, i_2;

  /* Local variables */
#define ag ((double *)&decod_1 + 5372)
#define as ((double *)&decod_1 + 5445)
#define delta ((double *)&decod_1 + 5326)
#define dltbuf ((double *)&decod_1 + 5337)
#define q1 ((double *)&dist_1 + 81)
#define q2 ((double *)&dist_1 + 92)
#define rdexp ((double *)&decod_1 + 5383)
#define rxexp ((double *)&dist_1 + 133)
#define ryexp ((double *)&decod_1 + 5394)
#define xbuf ((double *)&dist_1 + 144)
#define xp ((double *)&dist_1 + 209)
#define ybuf ((double *)&decod_1 + 5496)
#define ybufp ((double *)&dist_1 + 103)
#define yp ((double *)&dist_1 + 118)
  extern int      decode_();
  extern int      pwfilt_();
  static double   di;
  static double   dmin_;
  static double   savdec[290], yi[5];
  static double   savdis[143];
  static long     ci;
  static long     i;

  /* Save decoder and distance measure state */
  for (i = 0; i <= 289; ++i)
  {
    savdec[i] = decod_1.dcstat[i];
  }

  for (i = 0; i <= 142; ++i)
  {
    savdis[i] = dist_1.dmstat[i];
  }

  /* Search for best codeword */
  dmin_ = (double) 1e30;
  for (ci = 0; ci <= 1023; ++ci)
  {
    decode_(n, &ci, yi, &c__0);
    pwfilt_(n, x, yi, &di);
    if (di <= dmin_)
    {
      dmin_ = di;
      *c = ci;
    }

    /* Recall saved states */
    for (i = 0; i <= 289; ++i)
    {
      decod_1.dcstat[i] = savdec[i];
    }
    for (i = 0; i <= 142; ++i)
    {
      dist_1.dmstat[i] = savdis[i];
    }
  }

  /* Pass chosen codeword through decoder and dist.measure to update state */
  decode_(n, c, yi, &c__1);
  pwfilt_(n, x, yi, &di);
}
#undef dltbuf
#undef xp
#undef yp
#undef as
#undef ag
#undef ryexp
#undef rxexp
#undef q2
#undef q1
#undef ybufp
#undef rdexp
#undef delta
#undef ybuf
#undef xbuf
/* ........................... End of encode() ............................. */


/* Find distance measure D for a given codeword C and input vector X */
/* Version 21.11.91 / Ftj */
int             distm_(n, x, c, d)
  long           *n;
  double         *x;
  long           *c;
  double         *d;
{
  /* System generated locals */
  long            i_1, i_2;

  /* Builtin functions */
  long            s_rnge();

  /* Local variables */
#define ag ((double *)&decod_1 + 5372)
#define as ((double *)&decod_1 + 5445)
#define delta ((double *)&decod_1 + 5326)
#define dltbuf ((double *)&decod_1 + 5337)
#define q1 ((double *)&dist_1 + 81)
#define q2 ((double *)&dist_1 + 92)
#define rdexp ((double *)&decod_1 + 5383)
#define rxexp ((double *)&dist_1 + 133)
#define ryexp ((double *)&decod_1 + 5394)
#define xbuf ((double *)&dist_1 + 144)
#define xp ((double *)&dist_1 + 209)
#define ybuf ((double *)&decod_1 + 5496)
#define ybufp ((double *)&dist_1 + 103)
#define yp ((double *)&dist_1 + 118)
  extern int      decode_();
  extern int      pwfilt_();
  static double   dum;
  static double   savdec[290];
  static double   savdis[143];
  static double   y[5];
  static long     i;

  /* Save decoder and distance measure state */
  for (i = 0; i <= 289; ++i)
  {
    savdec[i] = decod_1.dcstat[i];
  }
  for (i = 0; i <= 142; ++i)
  {
    savdis[i] = dist_1.dmstat[i];
  }

  /* Compute distance measure */
  decode_(n, c, y, &c__0);
  pwfilt_(n, x, y, d);

  /* Recall saved states */
  for (i = 0; i <= 289; ++i)
  {
    decod_1.dcstat[i] = savdec[i];
  }
  for (i = 0; i <= 142; ++i)
  {
    dist_1.dmstat[i] = savdis[i];
  }

  /* Update decoder and weighting filter state */
  decode_(n, c, y, &c__1);
  pwfilt_(n, x, y, &dum);
}
#undef dltbuf
#undef xp
#undef yp
#undef as
#undef ag
#undef ryexp
#undef rxexp
#undef q2
#undef q1
#undef ybufp
#undef rdexp
#undef delta
#undef ybuf
#undef xbuf
/* ............................ End of distm() ........................... */


/* Compute perceptually weighted distance D between vectors X(n) and Y(n) */
/* Version 18.11.91 / Ftj */
int             pwfilt_(n, x, y, d)
  long           *n;
  double         *x, *y, *d;
{
  /* System generated locals */
  long            i_1, i_2, i_3, i_4;
  double          d_1;

  /* Local variables */
#define ag ((double *)&decod_1 + 5372)
#define as ((double *)&decod_1 + 5445)
#define delta ((double *)&decod_1 + 5326)
#define dltbuf ((double *)&decod_1 + 5337)
#define q1 ((double *)&dist_1 + 81)
#define q2 ((double *)&dist_1 + 92)
#define rdexp ((double *)&decod_1 + 5383)
#define rxexp ((double *)&dist_1 + 133)
#define ryexp ((double *)&decod_1 + 5394)
#define xbuf ((double *)&dist_1 + 144)
#define xp ((double *)&dist_1 + 209)
#define ybuf ((double *)&decod_1 + 5496)
#define ybufp ((double *)&dist_1 + 103)
#define yp ((double *)&dist_1 + 118)
  extern int      durbin_();
  static double   rc[50];
  static double   sum;
  static long     i, j, k, m;

  for (j = 0; j <= 4; ++j)
  {
    xbuf[(i_1 = j + 60)] = x[(i_2 = j)];
  }
  for (j = 0; j <= 4; ++j)
  {
    ybufp[(i_1 = j + 10)] = y[(i_2 = j)];
  }

  /* If n=4k+2 for any long k then update perceptual filter: */
  if (*n % 4 == 2)
  {
    for (i = 1; i <= 60; ++i)
    {
      i_3 = -i + 60;
      dismon_1.xw[i - 1] = dist_1.wp[i - 1] * xbuf[i_3];
    }
    for (k = 0; k <= 10; ++k)
    {
      sum = (double) 0.;
      for (i = 31; i <= 50; ++i)
      {
	sum += dismon_1.xw[(i_1 = i - 1)] * dismon_1.xw[(i_2 = i + k - 1)];
      }
      rxexp[k] = dist_1.rhop * rxexp[k] + sum;
    }

    for (k = 0; k <= 10; ++k)
    {
      sum = (double) 0.;
      for (i = 1; i <= 30; ++i)
      {
	sum += dismon_1.xw[i - 1] * dismon_1.xw[i + k - 1];
      }
      dismon_1.rx[k] = rxexp[k] + sum;
    }
    dismon_1.rx[0] = global_1.wnc * dismon_1.rx[0];
    if (dismon_1.rx[10] == 0.)
    {
      dismon_1.ifailp = 3;
    }
    else
    {
      durbin_(dismon_1.rx, &c__10, dismon_1.q, rc, &dismon_1.ifailp);
    }
    if (dismon_1.ifailp == 0)
    {
      for (k = 1; k <= 10; ++k)
      {
	q1[k] = dismon_1.q[k] * dist_1.gamma1[k - 1];
      }
      for (k = 1; k <= 10; ++k)
      {
	q2[k] = dismon_1.q[k] * dist_1.gamma2[k - 1];
      }
    }
  }

  /* Filter input signal through weighting filter: */
  for (m = 0; m <= 4; ++m)
  {
    xp[(i_1 = m + 10)] = xbuf[(i_2 = m + 60)];
    for (k = 1; k <= 10; ++k)
    {
      xp[(i_1 = m + 10)] = xp[(i_2 = m + 10)] +
	q1[k] * xbuf[(i_4 = m - k + 60)];
    }

    for (k = 1; k <= 10; ++k)
    {
      xp[(i_1 = m + 10)] = xp[(i_2 = m + 10)] -
	q2[(i_3 = k)] * xp[(i_4 = m - k + 10)];
    }
  }

  /* Filter output signal through weighting filter */
  for (m = 0; m <= 4; ++m)
  {
    yp[(i_1 = m + 10)] = ybufp[(i_2 = m + 10)];
    for (k = 1; k <= 10; ++k)
    {
      yp[(i_1 = m + 10)] = yp[(i_2 = m + 10)] +
	q1[(i_3 = k)] * ybufp[(i_4 = m - k + 10)];
    }

    for (k = 1; k <= 10; ++k)
    {
      yp[(i_1 = m + 10)] = yp[(i_2 = m + 10)] -
	q2[(i_3 = k)] * yp[(i_4 = m - k + 10)];
    }
  }

  /* Compute rms of weighted error signal: */
  *d = (double) 0.;
  for (j = 0; j <= 4; ++j)
  {
    /* Computing 2nd power */
    d_1 = yp[(i_1 = j + 10)] - xp[(i_2 = j + 10)];
    *d += d_1 * d_1;
  }

  /* Update state: */
  *d = sqrt(*d / 5);
  for (m = -60; m <= -1; ++m)
  {
    xbuf[(i_1 = m + 60)] = xbuf[(i_2 = m + 65)];
  }

  for (m = -10; m <= -1; ++m)
  {
    ybufp[(i_1 = m + 10)] = ybufp[(i_2 = m + 15)];
  }

  for (m = -10; m <= -1; ++m)
  {
    xp[(i_1 = m + 10)] = xp[(i_2 = m + 15)];
  }

  for (m = -10; m <= -1; ++m)
  {
    yp[(i_1 = m + 10)] = yp[(i_2 = m + 15)];
  }
}
#undef dltbuf
#undef xp
#undef yp
#undef as
#undef ag
#undef ryexp
#undef rxexp
#undef q2
#undef q1
#undef ybufp
#undef rdexp
#undef delta
#undef ybuf
#undef xbuf
/* ............................ End of pwfilt() ........................... */


/* LDCELP decoder */
/* Version 29.11.91 / Ftj */
int             decode_(n, c, y, mode)
  long           *n, *c;
  double         *y;
  long           *mode;
{
  /* System generated locals */
  long            i_1, i_2, i_3, i_4;
  double          d_1;

  /* Builtin functions */
  int             s_stop();
  long            s_rnge();
  double          pow_dd(), d_lg10();

  /* Local variables */
#define ag ((double *)&decod_1 + 5372)
#define as ((double *)&decod_1 + 5445)
#define delta ((double *)&decod_1 + 5326)
#define dltbuf ((double *)&decod_1 + 5337)
#define q1 ((double *)&dist_1 + 81)
#define q2 ((double *)&dist_1 + 92)
#define rdexp ((double *)&decod_1 + 5383)
#define rxexp ((double *)&dist_1 + 133)
#define ryexp ((double *)&decod_1 + 5394)
#define xbuf ((double *)&dist_1 + 144)
#define xp ((double *)&dist_1 + 209)
#define ybuf ((double *)&decod_1 + 5496)
#define ybufp ((double *)&dist_1 + 103)
#define yp ((double *)&dist_1 + 118)
  extern int      durbin_();
  static double   rc[50];
  static double   tmp;
  static long     i, j, k, m;

  /* Test if valid code file */
  if (*c > 1023 || *c < 0)
  {
    fprintf(stderr, "Illegal codeword encountered\n");
    exit(-1);
  }
  if (*n % 4 == 1)
  {
    for (i = 1; i <= 34; ++i)
    {
      decmon_1.deltaw[(i_1 = i - 1)] = decod_1.wg[(i_2 = i - 1)] *
	dltbuf[(i_3 = -i + 34)];
    }

    for (k = 0; k <= 10; ++k)
    {
      tmp = (double) 0.;
      for (i = 21; i <= 24; ++i)
      {
	tmp += decmon_1.deltaw[i - 1] * decmon_1.deltaw[i + k - 1];
      }
      rdexp[k] = decod_1.rhog * rdexp[k] + tmp;
    }

    for (k = 0; k <= 10; ++k)
    {
      tmp = (double) 0.;
      for (i = 1; i <= 20; ++i)
      {
	tmp += decmon_1.deltaw[i - 1] * decmon_1.deltaw[i + k - 1];
      }
      decmon_1.rdelta[k] = rdexp[k] + tmp;
    }

    decmon_1.rdelta[0] = global_1.wnc * decmon_1.rdelta[0];
    if (decmon_1.rdelta[10] == 0.)
    {
      decmon_1.ifailg = 3;
    }
    else
    {
      durbin_(decmon_1.rdelta, &c__10, ag, rc, &decmon_1.ifailg);
    }
    if (decmon_1.ifailg == 0)
    {
      for (k = 1; k <= 10; ++k)
      {
	ag[k] = ag[k] * decod_1.lambdg[k - 1];
      }
    }
  }

  /* Perform logarithmic gain prediction and excitation vector computation: */
  tmp = decod_1.doffst;
  for (k = 1; k <= 10; ++k)
  {
    tmp -= ag[k] * delta[-k + 10];
  }
  if (tmp < decod_1.dltmin)
  {
    tmp = decod_1.dltmin;
  }
  if (tmp > decod_1.dltmax)
  {
    tmp = decod_1.dltmax;
  }
  tmp /= (double) 20.;
  decmon_1.sigma = pow((double) 10., tmp);

  for (j = 0; j <= 4; ++j)
  {
    decmon_1.e[j] = decmon_1.sigma * decod_1.codebk[j][*c];
  }
  tmp = (double) 0.;
  for (j = 0; j <= 4; ++j)
  {
    /* Computing 2nd power */
    d_1 = decmon_1.e[j];
    tmp += d_1 * d_1;
  }
  tmp /= 5;
  if (tmp < 1.)
  {
    tmp = (double) 1.;
  }

  /* If n=4k+2 for any long k then update synthesis filter: */
  delta[10] = log10(tmp) * 10 - decod_1.doffst;
  if (*n % 4 == 2)
  {
    for (i = 1; i <= 105; ++i)
    {
      decmon_1.yw[(i_1 = i - 1)] = decod_1.ws[(i_2 = i - 1)] *
	ybuf[(i_3 = -10 - i + 115)];
    }
    for (k = 0; k <= 50; ++k)
    {
      tmp = (double) 0.;
      for (i = 36; i <= 55; ++i)
      {
	tmp += decmon_1.yw[i - 1] * decmon_1.yw[i + k - 1];
      }
      ryexp[k] = decod_1.rhos * ryexp[k] + tmp;
    }

    for (k = 0; k <= 50; ++k)
    {
      tmp = (double) 0.;
      for (i = 1; i <= 35; ++i)
      {
	tmp += decmon_1.yw[i - 1] * decmon_1.yw[i + k - 1];
      }
      decmon_1.ry[k] = ryexp[k] + tmp;
    }

    decmon_1.ry[0] = global_1.wnc * decmon_1.ry[0];
    if (decmon_1.ry[50] == 0.)
    {
      decmon_1.ifails = 3;
    }
    else
    {
      durbin_(decmon_1.ry, &c__50, as, rc, &decmon_1.ifails);
    }
    if (decmon_1.ifails == 0)
    {
      for (k = 1; k <= 50; ++k)
      {
	as[k] *= decod_1.lambds[k - 1];
      }
    }
  }

  /* Perform synthesis filtering: */
  for (m = 0; m <= 4; ++m)
  {
    ybuf[(i_1 = m + 115)] = decmon_1.e[m];
    for (k = 1; k <= 50; ++k)
    {
      ybuf[m + 115] = ybuf[(i_2 = m + 115)] - as[k] * ybuf[m - k + 115];
    }
  }

  /* Limit output signal when not in codebook search mode: */
  if (*mode == 1)
  {
    for (m = 0; m <= 4; ++m)
    {
      if (ybuf[m + 115] < decod_1.ymin)
      {
	ybuf[m + 115] = decod_1.ymin;
      }
      if (ybuf[m + 115] > decod_1.ymax)
      {
	ybuf[m + 115] = decod_1.ymax;
      }
    }
  }

  for (j = 0; j <= 4; ++j)
  {
    y[j] = ybuf[j + 115];
  }

  /* Update signal buffers: */
  for (m = -115; m <= -1; ++m)
  {
    ybuf[m + 115] = ybuf[m + 120];
  }

  for (m = -10; m <= -1; ++m)
  {
    delta[m + 10] = delta[m + 11];
  }

  dltbuf[34] = delta[10];
  for (m = -34; m <= -1; ++m)
  {
    dltbuf[m + 34] = dltbuf[m + 35];
  }
}
#undef dltbuf
#undef xp
#undef yp
#undef as
#undef ag
#undef ryexp
#undef rxexp
#undef q2
#undef q1
#undef ybufp
#undef rdexp
#undef delta
#undef ybuf
#undef xbuf
/* ........................... End of decode() ........................... */


/* Version: 21.11.91 / Ftj */
int             pstflt_(nvect, sx, sy)
  long           *nvect;
  double         *sx, *sy;
{
  /* System generated locals */
  long            i_1, i_2, i_3, i_4, i_5;
  double          d_1;

  /* Builtin functions */
  long            s_rnge();
  double          pow_di();
  long            i_dnnt();

  /* Local variables */
  static long     idim;
  static double   cmax;
  static long     kmax;
  static double   ptap, temp[5], ybuf[110];
  static long     itmp;
  static double   rhos, b, d[240];
  static long     i, j, k, m, n;
  static double   scale;
  static long     kpmin, kpmax;
  static double   ppfth, tapth, tiltf, stlpf[3];
  static long     kptmp;
  extern void     hybws_();
  static long     m1, m2, nfrsz;
  static double   tiltz, ryexp[51];
  static long     npwsz;
  static double   agcfac, al[3], bl[4], ap[11], gl, rc[10], sd[245];
  static long     ip, kp;
  static double   az[11], scalfl;
  static long     ifails;
  static double   ws[105], ry[51];
  static long     kpdlta;
  static double   yw[105], spfpcf;
  extern int      durbin_();
  static double   ppfzcf, cormax, spfzcf, stlpci[10], sumfil, sunfil, sppcfv[10];
  static long     icount;
  static double   stpfir[10], stpiir[10], spzcfv[10];
  static long     kp1;
  static double   dec[60], apf[11], tap, wnc, spf[5], tmp, sum, tap1;

  /* Preprocessing */
  if (*nvect == 0)
  {
    /* ...reset constant values */
    agcfac = (double) .99;
    idim = 5;
    kpdlta = 6;
    kpmin = 20;
    kpmax = 140;
    nfrsz = 20;
    npwsz = 100;
    ppfth = (double) .6;
    ppfzcf = (double) .15;
    spfpcf = (double) .75;
    spfzcf = (double) .65;
    tapth = (double) .4;
    /* ...reset initial values */
    tiltf = (double) .15;
    al[0] = (double) -2.34036589;
    al[1] = (double) 2.01190019;
    al[2] = (double) -.614109218;
    ap[0] = (double) 1.;
    az[0] = (double) 1.;
    for (i = 1; i < 11; ++i)
    {
      ap[i] = az[i] = (double) 0.;
    }

    for (i = -34; i <= 25; ++i)
    {
      dec[i + 34] = (double) 0.;
    }

    for (i = -139; i <= 100; ++i)
    {
      d[i + 139] = (double) 0.;
    }

    bl[0] = (double) .0357081667;
    bl[1] = (double) -.0069956244;
    bl[2] = (double) -.0069956244;
    bl[3] = (double) .0357081667;
    ip = 85;
    kp1 = 50;
    scalfl = (double) 1.;
    for (i = 0; i < 3; ++i)
    {
      stlpf[i] = (double) 0.;
    }
    for (i = 0; i < 10; ++i)
    {
      stlpci[i] = stpfir[i] = stpiir[i] = (double) 0.;
    }

    /* ...reset internal decoder state */
    gl = (double) 1.;
    wnc = (double) 1.00390625;
    rhos = (double) .75;
    hybws_(ws);
    for (i = 0; i <= 50; ++i)
    {
      ryexp[i] = (double) 0.;
    }
    for (i = -105; i <= 4; ++i)
    {
      ybuf[i + 105] = (double) 0.;
    }
    for (i = 1; i <= 10; ++i)
    {
      d_1 = pow(spfpcf, (double) i) * 16384;
      sppcfv[i - 1] = (double) round(d_1) / (double) 16384.;
      d_1 = pow(spfzcf, (double) i) * 16384;
      spzcfv[i - 1] = (double) round(d_1) / (double) 16384.;
    }
  }

  /* ...update 10'th order LPC coefficients at first vector in frame */
  if (*nvect % 4 == 0)
  {
    for (i = 1; i <= 105; ++i)
    {
      yw[i - 1] = ws[i - 1] * ybuf[-i + 105];
    }

    for (k = 0; k <= 50; ++k)
    {
      tmp = (double) 0.;
      for (i = 36; i <= 55; ++i)
      {
	tmp += yw[i - 1] * yw[i + k - 1];
      }
      ryexp[k] = rhos * ryexp[k] + tmp;
    }

    for (k = 0; k <= 50; ++k)
    {
      tmp = (double) 0.;
      for (i = 1; i <= 35; ++i)
      {
	tmp += yw[i - 1] * yw[i + k - 1];
      }
      ry[k] = ryexp[k] + tmp;
    }
    ry[0] = wnc * ry[0];

    if (ry[50] != 0.)
    {
      durbin_(ry, &c__10, apf, rc, &ifails);
    }
  }

  /* ...update decoded signal buffer */
  for (k = 0; k <= 4; ++k)
  {
    ybuf[k + 105] = sx[k];
  }

  for (m = -105; m <= -1; ++m)
  {
    ybuf[m + 105] = ybuf[m + 110];
  }

  /* ...transfer input vector */
  i_1 = idim;
  for (k = 1; k <= i_1; ++k)
  {
    sd[k + 239] = sx[k - 1];
  }

  /* Start processing according to postfilter description: */
  /* Compute the LPC prediction residual for the current decoded */
  /* speech vector. */
  icount = *nvect % 4 + 1;

  /* check & update IP */
  if (ip == npwsz)
  {
    ip = npwsz - nfrsz;
  }
  i_1 = idim;
  for (k = 1; k <= i_1; ++k)
  {
    itmp = ip + k;
    d[itmp + 13] = sd[k + 239];
    for (j = 10; j >= 2; --j)
    {
      /* FIR filtering. */
      d[itmp + 139] = d[itmp + 139] + stlpci[j - 1] * apf[j];
      /* Memory shift. */
      stlpci[j - 1] = stlpci[j - 2];
    }

    /* Handle last one. */
    d[itmp + 139] = d[itmp + 139] + stlpci[0] * apf[1];

    /* shift in input. */
    stlpci[0] = sd[k + 239];
  }

  /* Extract the pitch period from the LPC prediction residual */
  /* update IP. */
  ip += idim;

  /* lowpass filtering & 4:1 downsampling. */
  if (icount == 3)
  {
    i_1 = npwsz;
    for (k = npwsz - nfrsz + 1; k <= i_1; ++k)
    {
      /* IIR filter */
      tmp = d[k + 139] - stlpf[0] * al[0] - stlpf[1] * al[1] - stlpf[2] * al[2];
      if (k % 4 == 0)
      {
	/* do FIR filtering only if needed. */
	n = k / 4;
	dec[n + 34] = tmp * bl[0] + stlpf[0] * bl[1] +
	  stlpf[1] * bl[2] + stlpf[2] * bl[3];
      }

      /* shift lowpass filter memory. */
      stlpf[2] = stlpf[1];
      stlpf[1] = stlpf[0];
      stlpf[0] = tmp;
    }

    /* start correlation peak-picking in the decimated LPC residual domain. */
    m1 = kpmin / 4;
    m2 = kpmax / 4;
    cormax = (double) -1e30;
    i_1 = m2;
    for (j = m1; j <= i_1; ++j)
    {
      tmp = 0.;
      i_2 = npwsz / 4;
      /* tmp = correlation in decimated domain */
      for (n = 1; n <= i_2; ++n)
      {
	tmp += dec[n + 34] * dec[n - j + 34];
      }
      /* find maximum correlation and the corresponding lag. */
      if (tmp > cormax)
      {
	cormax = tmp;
	kmax = j;
      }
    }

    /* shift decimated LPC residual buffer. */
    i_1 = (npwsz - nfrsz) / 4;
    for (n = -m2 + 1; n <= i_1; ++n)
    {
      dec[n + 34] = dec[n + 39];
    }

    /* start correlation peak-picking in undecimated domain */
    m1 = (kmax << 2) - 3;
    m2 = (kmax << 2) + 3;

    /* check whether M1 out of range. */
    if (m1 < kpmin)
    {
      m1 = kpmin;
    }

    /* check whether M2 out of range. */
    if (m2 > kpmax)
    {
      m2 = kpmax;
    }

    /* correlation in undecimated domain. */
    cormax = (double) -1e30;
    i_1 = m2;
    for (j = m1; j <= i_1; ++j)
    {
      tmp = (double) 0.;
      i_2 = npwsz;
      for (k = 1; k <= i_2; ++k)
      {
	tmp += d[k] * d[k - j + 139];
      }
      if (tmp > cormax)
      {
	/* find maximum correlation and */
	cormax = tmp;
	/* the corresponding lag. */
	kp = j;
      }
    }

    /* determine the range of search around */
    m1 = kp1 - kpdlta;

    /* the pitch period of previou frame. */
    m2 = kp1 + kpdlta;

    /* KP can't be a multiple pitch if true. */
    if (kp < m2 + 1)
    {
      goto L115;
    }

    /* check whether M1 out of range. */
    if (m1 < kpmin)
    {
      m1 = kpmin;
    }

    /* check whether M2 out of range. */
    if (m2 > kpmax)
    {
      m2 = kpmax;
    }

    cmax = (double) -1e30;
    i_1 = m2;
    for (j = m1; j <= i_1; ++j)
    {
      tmp = (double) 0.;
      i_2 = npwsz;
      for (k = 1; k <= i_2; ++k)
      {
	/* correlation in undecimated domain. */
	tmp += d[k + 139] * d[k - j + 139];
      }
      if (tmp > cmax)
      {
	/* find maximum correlation and the corresponding lag. */
	cmax = tmp;
	kptmp = j;
      }
    }

    /* start computing the tap weights */
    sum = tmp = (double) 0.;
    i_1 = npwsz;
    for (k = 1; k <= i_1; ++k)
    {
      sum += d[k - kp + 139] * d[k - kp + 139];
      tmp += d[k - kptmp + 139] * d[k - kptmp + 139];
    }
    if (sum == 0.)
    {
      tap = 0.;
    }
    else
    {
      tap = cormax / sum;
    }
    if (tmp == 0.)
    {
      tap1 = 0.;
    }
    else
    {
      tap1 = cmax / tmp;
    }

    /* clamp TAP between 0 and 1 */
    if (tap > 1.)
    {
      tap = 1.;
    }
    if (tap < 0.)
    {
      tap = 0.;
    }

    /* clamp TAP1 between 0 and 1 */
    if (tap1 > 1.)
    {
      tap1 = 1.;
    }

    /* Replace KP with fundamental pitch if TAP1 is large enough. */
    if (tap1 < 0.)
    {
      tap1 = 0.;
    }
    if (tap1 > tapth * tap)
    {
      kp = kptmp;
    }

L115:
    /* update pitch period of previous frame */
    kp1 = kp;
    i_1 = npwsz - nfrsz;

    /* shift the LPC residual buffer */
    for (k = -kpmax + 1; k <= i_1; ++k)
    {
      d[k + 139] = d[k + nfrsz + 139];
    }

    /* Calculate the optimal tap weight of the single-tap pitch predictor of
     * the decoded speech. */
    sum = (double) 0.;
    tmp = (double) 0.;
    for (k = -npwsz + 1; k <= 0; ++k)
    {
      sum += sd[k - kp + 239] * sd[k - kp + 239];
      tmp += sd[k + 239] * sd[k - kp + 239];
    }
    if (sum == 0.)
    {
      ptap = 0.;
    }
    else
    {
      /* Calculate the coefficient $b$ and the scaling factor $g sub l$ of
       * the long-term postfilter. */
      ptap = tmp / sum;
    }

    /* clamp PTAP at 1. */
    if (ptap > 1.)
    {
      ptap = (double) 1.;
    }

    /* turn off pitch postfilter if PTAP smaller than threshold. */
    if (ptap < ppfth)
    {
      ptap = (double) 0.;
    }
    b = ppfzcf * ptap;
    gl = 1 / (b + 1);
  }

  /* Calculate the coefficients of the short-term postfilter. */
  if (icount == 1)
  {
    for (i = 2; i <= 11; ++i)
    {
      /* scale denominator coeff. */
      ap[i - 1] = sppcfv[i - 2] * apf[i - 1];

      /* scale numerator coeff. */
      az[i - 1] = spzcfv[i - 2] * apf[i - 1];
    }

    /* tilt compensation filter coeff. */
    tiltz = tiltf * rc[0];
  }

  /* Perform filtering operation of the long-term postfilter. */
  i_1 = idim;
  for (k = 1; k <= i_1; ++k)
  {
    /* long-term postfiltering. */
    temp[k - 1] = gl * (sd[k + 239] + b * sd[k - kp + 239]);
  }

  /* shift decoded speech buffer. */
  for (k = -npwsz - kpmax + 1; k <= 0; ++k)
  {
    sd[k + 239] = sd[k + idim + 239];
  }

  /* Perform filtering operation of the short-term postfilter. */
  i_1 = idim;
  for (k = 1; k <= i_1; ++k)
  {
    tmp = temp[k - 1];
    for (j = 10; j >= 2; --j)
    {
      /* All-zero part of the filter. */
      temp[k - 1] += stpfir[j - 1] * az[j];
      stpfir[j - 1] = stpfir[j - 2];
    }

    /* Last multiplier. */
    temp[k - 1] += stpfir[0] * az[1];
    stpfir[0] = tmp;

    /* All-pole part of the filter. */
    for (j = 10; j >= 2; --j)
    {
      temp[k - 1] -= stpiir[j - 1] * ap[j];
      stpiir[j - 1] = stpiir[j - 2];
    }

    /* Last multiplier. */
    temp[k - 1] -= stpiir[0] * ap[1];

    /* Spectral tilt compensation filter. */
    stpiir[0] = temp[k - 1];
    temp[k - 1] += stpiir[1] * tiltz;
  }

  /* Calculate the sum of absolute values of the components of the */
  /* decoded speech vector. */
  sunfil = (double) 0.;
  i_1 = idim;
  for (k = 1; k <= i_1; ++k)
  {
    sunfil + = fabs(sd[k + 239]);
  }

  /* Calculate the sum of absolute values of the components of the */
  /* short-term postfilter output vector. */
  sumfil = (double) 0.;
  i_1 = idim;
  for (k = 1; k <= i_1; ++k)
  {
    sumfil += fabs(temp[k - 1]);
  }

  /* Calculate the overall scaling factor of the postfilter */
  /* Lowpass filter the once-a-vector scaling factor and use the filtered */
  /* scaling factor to scale the short-term postfilter output vector. */
  scale = (double) 1.;
  if (sumfil > 1.)
  {
    scale = sunfil / sumfil;
  }
  i_1 = idim;
  for (k = 1; k <= i_1; ++k)
  {
    /* lowpass filtering */
    scalfl = agcfac * scalfl + (1 - agcfac) * scale;

    /* scale output. */
    spf[k - 1] = scalfl * temp[k - 1];
  }

  /* Postprocessing */
  /* ...transfer output vector */
  for (k = 1; k <= 5; ++k)
  {
    sy[k - 1] = spf[k - 1];
  }
}
/* ......................... End of pstflt() ......................... */


int             durbin_(r, n, a, rc, ifail)
  double         *r;
  long           *n;
  double         *a, *rc;
  long           *ifail;
{
  /* System generated locals */
  long            i_1, i_2;

  /* Local variables */
  static double   abuf[100];
  static long     minc, i;
  static double   alpha;
  static long     n1, ib, mh;
  static double   at;
  static long     ip;
  static double   sum;

  *ifail = 0;
  i_1 = *n + 1;
  for (i = 0; i < i_1; ++i)
  {
    abuf[i] = a[i];
  }
  if (r[0] <= 0.)
  {
    *ifail = 1;
    goto L99;
  }
  rc[0] = -(r[1] / r[0]);
  a[0] = (double) 1.;
  a[1] = rc[0];
  alpha = r[0] + r[1] * rc[0];
  if (alpha <= 0.)
  {
    *ifail = 2;
    goto L99;
  }
  i_1 = *n;
  for (minc = 2; minc <= i_1; ++minc)
  {
    sum = (double) 0.;
    i_2 = minc;
    for (ip = 1; ip <= i_2; ++ip)
    {
      n1 = minc - ip + 2;
      sum += r[n1 - 1] * a[ip - 1];
    }
    rc[minc - 1] = -(sum / alpha);
    mh = minc / 2 + 1;
    i_2 = mh;
    for (ip = 2; ip <= i_2; ++ip)
    {
      ib = minc - ip + 2;
      at = a[ip - 1] + rc[minc - 1] * a[ib - 1];
      a[ib - 1] += rc[minc - 1] * a[ip - 1];
      a[ip - 1] = at;
    }
    a[minc] = rc[minc - 1];
    alpha += rc[minc - 1] * sum;
    if (alpha <= 0.)
    {
      *ifail = 2;
      goto L99;
    }
  }
  return 0;

L99:
  /* Keep wheighting filter coefficients in case of ill-conditioning */
  i_1 = *n + 1;
  for (i = 0; i < i_1; ++i)
  {
    a[i] = abuf[i];
  }
  return (*ifail);
  /* Version: 03.10.91 / Ftj */
}
/* ........................ End of durbin() ............................. */


void            hybws_(ws)
  double         *ws;
{
  /* Initialized data */

  static long     iws[105] = {1565, 3127, 4681, 6225, 7755, 9266, 10757, 12223,
    13661, 15068, 16441, 17776, 19071, 20322, 21526, 22682, 23786, 24835, 25828,
    26761, 27634, 28444, 29188, 29866, 30476, 31016, 31486, 31884, 32208, 32460,
    32637, 32739, 32767, 32721, 32599, 32403, 32171, 31940, 31711, 31484, 31259,
    31034, 30812, 30591, 30372, 30154, 29938, 29724, 29511, 29299, 29089, 28881,
    28674, 28468, 28264, 28062, 27861, 27661, 27463, 27266, 27071, 26877, 26684,
    26493, 26303, 26114, 25927, 25742, 25557, 25374, 25192, 25012, 24832, 24654,
    24478, 24302, 24128, 23955, 23784, 23613, 23444, 23276, 23109, 22943, 22779,
    22616, 22454, 22293, 22133, 21974, 21817, 21661, 21505, 21351, 21198, 21046,
  20896, 20746, 20597, 20450, 20303, 20157, 20013, 19870, 19727};

  /* System generated locals */
  long            i_1, i_2;

  /* Builtin functions */
  long            s_rnge();

  /* Local variables */
  static long     i;

  for (i = 0; i < 105; ++i)
  {
    ws[i] = iws[i] / (double) 32768.;
  }
  /* Version: 03.10.91 / Ftj */
}
/* ............................ End of hybws() .......................... */


void            hybwg_(wg)
  double         *wg;
{
  /* Version: 03.10.91 / Ftj */
  /* Initialized data */
  static long     iwg[34] = {3026, 6025, 8973, 11845, 14615, 17261, 19759, 22088,
    24228, 26162, 27872, 29344, 30565, 31525, 32216, 32631, 32767, 32625, 32203,
    31506, 30540, 29461, 28420, 27416, 26448, 25514, 24613, 23743, 22905, 22096,
  21315, 20562, 19836, 19135};

  /* Local variables */
  static long     i;

  for (i = 0; i < 34; ++i)
    wg[i] = (double) iwg[i] / (double) 32768.;
}
/* .......................... End of hybwg() .............................. */


void            hybwp_(wp)
  double         *wp;
{
  /* Version: 03.10.91 / Ftj */
  /* Initialized data */

  static short    iwp[60] = {1957, 3908, 5845, 7760, 9648, 11502, 13314, 15079,
    16790, 18441, 20026, 21540, 22976, 24331, 25599, 26775, 27856, 28837, 29715,
    30487, 31150, 31702, 32141, 32464, 32672, 32763, 32738, 32595, 32336, 31961,
    31472, 30931, 30400, 29878, 29365, 28860, 28364, 27877, 27398, 26927, 26465,
    26010, 25563, 25124, 24693, 24268, 23851, 23442, 23039, 22643, 22254, 21872,
  21496, 21127, 20764, 20407, 20057, 19712, 19373, 19041};

  /* Local variables */
  static int      i;

  for (i = 0; i < 60; ++i)
    wp[i] = (double) iwp[i] / (double) 32768.;
}
/* .......................... End of hybwp() ............................... */


void            cbook_(y)
  double         *y;
{
  /* Initialized data */

  static short    iy[640] = {668, -2950, -1254, -1790, -2553, -5032, -4577, -1045,
    2908, 3318, -2819, -2677, -948, -2825, -4450, -6679, -340, 1482, -1276, 1262,
    -562, -6757, 1281, 179, -1274, -2512, -7130, -4925, 6913, 2411, -2478, -156,
    4683, -3873, 0, -8208, 2140, -478, -2785, 533, 1889, 2759, 1381, -6955, -5913,
    5082, -2460, -5778, 1797, 568, -2208, -3309, -4523, -6236, -7505, -2719,
    4358, -2988, -1149, 2664, 1259, 995, 2711, -2464, -10390, 1722, -7569, -2742,
    2171, -2329, 1032, 747, -858, -7946, -12843, 3106, 4856, -4193, -2541, 1035,
    1862, -960, -6628, 410, 5882, -2493, -2628, -4000, -60, 7202, -2672, 1446,
    1536, -3831, 1233, -5302, 6912, 1589, -4187, 3665, -3456, -8170, -7709, 1384,
    4698, -4699, -6209, -11176, 8104, 16830, 930, 7004, 1269, -8977, 2567, 4649,
    11804, 3441, -5657, 1199, 2542, -183, -8859, -7976, 3230, -2872, -2011,
    -9713, -8385, 12983, 3086, 2140, -3680, -9643, -2896, -7609, 6515, -2283,
    -2522, 6332, -3333, -5620, -9130, -11131, 5543, -407, -6721, -17466, -2889,
    11568, 3692, 6796, -262, -10846, -1856, 7275, 13404, -2989, -10595, 4936,
    244, -2219, 2656, 3776, -5412, -4043, -5934, 2131, 863, -2866, -3302, 1743,
    -2006, -128, -2052, -6361, 3342, -1583, -21, 1142, -3837, -1831, 6397, 2545,
    -2848, -9332, -6528, 5309, 1986, -2245, -4490, 748, 1935, -3027, -493, -9255,
    5366, 3193, -4493, 1784, 4784, -370, 1866, 1057, -1889, 7342, -2690, -2577,
    676, -611, -502, 2235, -1850, -1777, -2049, 1011, 3880, -2465, 2209, -152,
    2592, 2829, 5588, 2839, -7306, -3049, -4918, 5955, 9201, -4447, 697, 3908,
    5798, -4451, -4644, -2121, 5444, -2570, 321, -1202, 2846, -2086, 3532, 566,
    -708, -4279, 950, 4980, 3749, 452, -2484, 3502, 1719, -170, 238, -3435, 263,
    2114, -2005, 2361, -7338, -1208, 9347, -1216, -4013, -13498, -439, 8028,
    -4232, 361, -3729, 5433, 2004, -4727, -1259, -3986, 7743, 8429, -3691, -987,
    5198, -423, 1150, -1281, 816, 7409, 4109, -3949, 2690, 30, 1246, 3055, -35,
    -1370, -246, -1489, 5635, -678, -2627, 3170, 4830, -4585, 2008, -1062, 799,
    -129, 717, 4594, 14937, 10706, 417, 2759, 1850, -5057, -1153, -3887, 7361,
    -5768, 4285, 666, 1443, -938, 20, -2119, -1697, -3712, -3402, -2212, 110,
    2136, -2952, 12, -1568, -3500, -1855, -1315, -1731, 1160, -558, 1709, 88,
    -4569, 194, -454, -2957, -2839, -1666, -273, 2084, -155, -189, -2376, 1663,
    -1040, -2449, -2842, -1369, 636, -248, -2677, 1517, 79, -3013, -3669, -973,
    1913, -2493, -5312, -749, 1271, -2903, -3324, -3756, -3690, -1829, -2913,
    -1547, -2760, -1406, 1124, 1844, -1834, 456, 706, -4272, 467, -4256, -1909,
    1521, 1134, -127, -994, -637, -1491, -6494, 873, -2045, -3828, -2792, -578,
    2311, -1817, 2632, -3052, 1968, 641, 1194, 1893, 4107, 6342, -45, 1198, 2160,
    -1449, 2203, -2004, 1713, 3518, 2652, 4251, 2936, -3968, 1280, 131, -1476,
    2827, 8, -1928, 2658, 3513, 3199, -816, 2687, -1741, -1407, 2948, 4029, 394,
    -253, 1298, 4286, 51, -4507, -32, -659, 3903, 5646, -5588, -2592, 5707, -606,
    1234, -1607, -5187, 664, -525, 3620, -2192, -2527, 1707, 4297, -3251, -2283,
    812, -2264, 5765, 528, -3287, 1352, 1672, 2735, 1241, -1103, -3273, -3407,
    4033, 1648, -2965, -1174, 1444, 74, 918, 1999, 915, -1026, -2496, -1605, 2034,
    2950, 229, -2168, 2037, 15, -1264, -208, -3552, 1530, 581, 1491, 962, -2613,
    -2338, 3621, -1488, -2185, -1747, 81, 5538, 1432, -2257, -1019, 867, 214,
    -2284, -1510, -1684, 2816, -229, 2551, -1389, 2707, 504, 479, 2783, -1009,
    2517, -1487, -1596, 621, 1929, -148, 2206, -4288, 1292, -1401, -527, 1243,
    -2731, 1909, 1280, 2149, -1501, 3688, 610, -4591, 3306, -3369, 1875, 3636,
    -1217, 2574, 2513, 1449, -3074, -4979, 814, 1826, -2497, 4234, -4077, 1664,
    -220, 3418, 1002, 1115, 781, 1658, 3919, 6130, 3140, 1148, 4065, 1516, 815,
    199, 1191, 2489, 2561, 2421, 2443, 770, -5915, 5515, -368, -3199, 1190, 1047,
    3742, 6927, -2089, 292, 3099, 4308, -758, -2455, 523, 3921, 4044, 1386, 85,
    4367, 1006, -1252, -1466, -1383, 3852, 1579, -77, 2064, 868, 5109, 2919, -202,
    359, -509, 3650, 3206, 2303, 1693, 1296, 2905, -3907, 229, -1196, -2332, 5977,
    -3585, 805, 3825, -3138, 3746, -606, 53, -269, -3301, 606, 2018, -1316, 4064,
  398};

  /* Local variables */
  int             i;

  for (i = 0; i < 640; ++i)
    y[i] = (double) iy[i] / (double) 2048.;
}
/* ........................... End of cbook() ........................... */
