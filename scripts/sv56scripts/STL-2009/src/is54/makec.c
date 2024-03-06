/**************************************************************************

                (C) Copyright 1990, Motorola Inc., U.S.A.

Note:  Reproduction and use for the development of North American digital
       cellular standards or development of digital speech coding
       standards within the International Telecommunications Union -
       Telecommunications Standardization Sector is authorized by Motorola 
       Inc.  No other use is intended or authorized.

       The availability of this material does not provide any license
       by implication, estoppel, or otherwise under any patent rights
       of Motorola Inc. or others covering any use of the contents
       herein.

       Any copies or derivative works must incude this and all other
       proprietary notices.


Systems Research Laboratories
Chicago Corporate Research and Development Center
Motorola Inc.

**************************************************************************/
/*-------------------------------------------------------------------*/
/**/
/*	makeCoefs.c -- functions for widening and spectral smoothing*/
/*		       of coefs.*/
/**/
/*-------------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

/*#include "stdlib.h"*/

/*	from paramConv.c*/
/**/
int             ATORC();
/*FTYPE *a, FTYPE *k*/

void            widen(lambda, side)
  FTYPE           lambda;
  char            side;
{
  FTYPE           term;		/* holds lambda**i term (widening factor for
				 * Ai) */

  FTYPE          *tp, *tp2, *ep;

  if (side == 't')
  {
    tp2 = T_NEW.a;
    tp = T_NEW.widen;
  }
  else
  {
    tp2 = R_NEW.a;
    tp = R_NEW.widen;
  }
  term = lambda;
  for (ep = tp + NP; tp < ep; tp++)
  {
    *tp = *tp2 * term;
    tp2++;
    term *= lambda;
  }
}


/*------------------------------------------------------------------------*/
/*	A_SST -- performs spectral smoothing to get post-filter numerator*/
/*		 coefficients*/
/**/
/*	function declarations*/
/**/
void            ATOCOR();
/*FTYPE *k, FTYPE *ac*/
void            LEVINSON();
/*FTYPE *ac, FTYPE *a*/

/*	function definition*/
/**/
void            A_SST(wCoefPtr, ssCoefPtr)
  FTYPE          *wCoefPtr;
  FTYPE          *ssCoefPtr;
{
  FTYPE          *tmpKs;	/* points to temporary reflection coef buffer */
  FTYPE          *tmpAcs;	/* points to temp autocorrelation buffer */
  FTYPE          *tp, *tp2, *ep;

  tmpKs = (FTYPE *) malloc(NP * sizeof(FTYPE));
  tmpAcs = (FTYPE *) malloc((NP + 1) * sizeof(FTYPE));

/*	convert widened denominator coefs to reflection coefs*/
/**/
  ATORC(wCoefPtr, tmpKs);

/*	negate reflection coefficients*/
/**/
  tp = tmpKs;
  for (ep = tp + NP; tp < ep; tp++)
    *tp *= -1.0;

/*	convert to autocorrelation coefficients*/
/**/
  ATOCOR(tmpKs, tmpAcs);

/*	do spectral smoothing (apply envelope to autocorrelations)*/
/**/
  tp = tmpAcs;
  tp2 = P_SST;
  for (ep = tp + NP + 1; tp < ep; tp++, tp2++)
    *tp *= *tp2;

/*	convert smoothed ac coefs back to direct-form coefficients*/
/**/
  LEVINSON(tmpAcs, ssCoefPtr);

  free(tmpKs);
  free(tmpAcs);
}				/* end of A_SST */


void            ATOCOR(k, ac)
  FTYPE          *k;
  FTYPE          *ac;
{
  FTYPE           e;		/* error value, updated recursively */
  FTYPE          *a, *aTmp;	/* buffers for intermediate direct-form coefs */
  FTYPE          *acBegin;	/* points to beginning of autocorrelation
				 * array */
  FTYPE           sum, *temp, *tp, *tp2, *tp3, *ep, *ep2, *fp1, *fp2;

  fp1 = (FTYPE *) malloc(NP * sizeof(FTYPE));
  fp2 = (FTYPE *) malloc(NP * sizeof(FTYPE));
  a = fp1;
  aTmp = fp2;

  acBegin = ac;
  *ac = 1.0;
  ac++;
  *ac = *k;
  ac++;
  *a = -*k;
  e = 1.0 - *k * *k;
  k++;
  for (ep = ac + NP - 1; ac < ep; ac++, k++)
  {
    /* compute next autocorrelation coef */
    /**/
    sum = 0.0;
    tp = a;
    tp2 = ac - 1;
    for (; tp2 > acBegin; tp++, tp2--)
      sum += *tp * *tp2;
    *ac = *k * e - sum;

    /* update e and a array */
    /**/
    e *= 1.0 - *k * *k;

    tp2 = a;
    tp3 = aTmp;
    for (ep2 = tp; tp2 < ep2; tp2++, tp3++)
      *tp3 = *tp2 - *k * *--tp;
    *tp3 = -*k;
    temp = a;
    a = aTmp;
    aTmp = temp;
  }
  free(fp1);
  free(fp2);
}


void            LEVINSON(ac, a)
  FTYPE          *ac;
  FTYPE          *a;
{
  FTYPE          *aTmp, *bTmp;	/* buffers for intermediate direct-form coefs */
  FTYPE          *rc;		/* reflection coef array */
  FTYPE           e;		/* error value */
  FTYPE           sum;		/* temporary accumulator */
  FTYPE           gamma;	/* intermediate term in recursion */
  FTYPE          *acBegin;	/* points to beginning of autocorrelation
				 * array */
  FTYPE          *tp, *tp2, *tp3, *ep, *ep2, *temp, *fp1, *fp2, *fp3;

  fp1 = (FTYPE *) malloc(NP * sizeof(FTYPE));
  fp2 = (FTYPE *) malloc(NP * sizeof(FTYPE));
  fp3 = (FTYPE *) malloc(NP * sizeof(FTYPE));
  aTmp = fp1;
  bTmp = fp2;
  rc = fp3;

  *aTmp = -*(ac + 1) / *ac;
  *rc = *aTmp;
  e = *ac * (1.0 - *rc * *rc);
  acBegin = ac;
  ac += 2;
  rc++;
  for (ep = ac + NP - 1; ac < ep; ac++, rc++)
  {
    /* compute next reflection coef */
    /**/
    sum = 0.0;
    tp = aTmp;
    tp2 = ac - 1;
    for (; tp2 > acBegin; tp++, tp2--)
      sum += *tp * *tp2;
    gamma = *ac + sum;
    *rc = -gamma / e;

    /* update a array and e */
    /**/
    tp2 = aTmp;
    tp3 = bTmp;
    for (ep2 = tp; tp2 < ep2; tp2++, tp3++)
      *tp3 = *tp2 + *rc * *--tp;
    *tp3 = *rc;
    temp = aTmp;
    aTmp = bTmp;
    bTmp = temp;

    e *= 1.0 - *rc * *rc;
  }
/*	put direct-from coefs in output array*/
/**/
  for (ep = aTmp + NP; aTmp < ep; aTmp++, a++)
    *a = *aTmp;

  free(fp1);
  free(fp2);
  free(fp3);
}
