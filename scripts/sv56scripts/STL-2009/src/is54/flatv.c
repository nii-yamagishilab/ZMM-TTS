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
/*------------------------------------------------------------------*/
/**/
/*	flatv.c -- FLAT algorithm for determining reflection coefs,*/
/*			quantization routines and tables.*/
/**/
/*------------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"
#include <assert.h>

#include <stdio.h>
/*#include "stdlib.h"*/
#include <math.h>

/*------------------------------------------------------------------------*/
/*	FLATV -- calculates Rq0 and k's.*/
/**/
static FTYPE    QUANT();
/*FTYPE val, int i*/

void            FLATV()
{
  FTYPE          *sd;		/* points to pre-emphasized input */
  /* speech buffer */
  FTYPE           ac[NP + 1][NP + 1];	/* autocorrelation matrix */
  FTYPE          *s;		/* pointer to analysis frame of input speech */
  FTYPE           avgR0;	/* pre-quantzd val.of sqrt(S_LEN*average R0) */
  FTYPE           f[NP][NP];	/* f matrix */
  FTYPE           b[NP][NP];	/* b matrix */
  FTYPE           c[NP][NP];	/* c matrix */
  FTYPE           temp;		/* temporarily holds denominator of
				 * reflection coef */
  FTYPE           temp2;	/* temporarily holds numerator of reflection
				 * coef */
  FTYPE          *kPtr;		/* points to current reflection coef in
				 * buffer */
  FTYPE           termf;	/* temporary f[i][k] term in matrix update
				 * loop */
  FTYPE           termb;	/* temporary b[i][k] term in matrix update
				 * loop */
  FTYPE           termc1;	/* temporary c[i][k] term in matrix update
				 * loop */
  FTYPE           termc2;	/* temporary c[k][i] term in matrix update
				 * loop */
  FTYPE           term;		/* temporary rj(~ + ~) term in matrix update
				 * loop */
  FTYPE           ksqrd;	/* current reflection coef squared */

  FTYPE          *freePtr;
  FTYPE          *tp, *ePtr;
  int             i, j, k, end;

/*	calculate autocorrelation matrix*/
/**/
/*	The ac array is a square matrix, but only the upper triangular*/
/*	is filled and updated.  A first-row entry, ac[0][i], is calculated*/
/*	first, then all entries in the same diagonal are calculated in turn*/
/*	from the previous value.*/
/**/
/*	first pre-emphasize one analysis length of speech, and put into*/
/*	double-precision buffer*/
/**/
  s = inBuf + INBUFSIZ - A_LEN;
  tp = s - 1;
  freePtr = (FTYPE *) malloc(A_LEN * sizeof(FTYPE));
  sd = freePtr;
  for (ePtr = s + A_LEN; s < ePtr; s++, tp++, sd++)
    *sd = *s - PREEMPH * *tp;
  sd -= A_LEN;

/*	now calculate ac matrix*/
/**/
  for (i = 0; i <= NP; i++)
  {
    ac[0][i] = 0.0;
    for (k = NP; k < A_LEN; k++)
      ac[0][i] += sd[k] * sd[k - i];
    for (j = 1; j <= NP - i; j++)
      ac[j][i + j] = ac[j - 1][i + j - 1] - sd[A_LEN - j] * sd[A_LEN - (i + j)]
	+ sd[NP - j] * sd[NP - (i + j)];
  }

/*	calculate and quantize sqrt(S_LEN * avg R0), assign to external*/
/*	variable.*/
/**/
  avgR0 = sqrt((S_LEN * (ac[0][0] + ac[NP][NP])) / (2.0 * (A_LEN - NP)));
  T_NEW.rq0 = QUANT(avgR0, 0);

/*	initialize F, B, and C matricies from the ac matrix*/
/**/
/*	The f and b arrays are square matricies; however, since they are*/
/*	symmetric, only the upper triangulars are filled and updated.*/
/*	The symmetry of the c matrix along diagonal below the main*/
/*	diagonal is utilized in the initialization.  Spectral*/
/*	smoothing is applied to the ac matrix before it is used to*/
/*	initialize the f, b, and c matricies.*/
/**/
  for (i = 0; i <= NP; i++)
  {
    for (j = 0; j <= NP - i; j++)
    {
      k = j + i;
      temp = *(sst + i) * ac[j][k];
      if (j < NP && k < NP)
	f[j][k] = temp;
      if (j > 0 && k > 0)
	b[j - 1][k - 1] = temp;
      if (j < NP && k > 0)
      {
	c[j][k - 1] = temp;
	if (k < NP && j > 0)
	  c[k][j - 1] = temp;
      }
    }
  }

/*	reflection coef loop*/
/**/
/*	Calculates rj (checking for zero denominator and unstable result),*/
/*	quantizes, and updates the f, b, and c matricies.  Matrix entries*/
/*	f[i][k], b[i-1][k-1], c[i][k-1], and c[k][i-1], are updated*/
/*	simultaneously using common terms.  The updates are done in place.*/
/**/
  kPtr = T_NEW.k;
  for (j = 1; j <= NP; j++)
  {
    /* calculate denominator */
    /**/
    temp = (f[0][0] + b[0][0] + f[NP - j][NP - j] + b[NP - j][NP - j]);
    if (temp == 0.0)
    {
      /* denominator is zero, set energy to lowest level and do nothing */
      /* with the rc's */
      /**/
      codes = codeBuf;
      T_NEW.rq0 = QUANT(0.0, 0);
      codes = codeBuf + NP + 1;
      break;
    }

    /* calculate numerator */
    /**/
    temp2 = -2.0 * (c[0][0] + c[NP - j][NP - j]);
    if (fabs(temp2) >= temp)
    {
      /* reflection coef >= 1.0, fill all rc's with zero */
      /**/
      for (; codes < codeBuf + NP + 1; kPtr++)
	*kPtr = QUANT(0.0, kPtr - T_NEW.k + 1);
      fprintf(stderr,
	      "WARNING FROM FLATV: k's unstable in frame %d\n", frCnt);
      break;
    }

    /* calculate and quantize reflection coef */
    /**/
    temp = temp2 / temp;
    *kPtr = QUANT(temp, j);
    if (j == NP)		/* exit here if done */
      break;

    /* update matricies */
    /**/
    end = NP - j;
    for (i = 0; i <= end; i++)
    {
      for (k = i; k <= end; k++)
      {
	termf = f[i][k];
	termb = b[i][k];
	termc1 = c[i][k];
	termc2 = c[k][i];
	term = *kPtr * (termc1 + termc2);
	ksqrd = *kPtr * *kPtr;

	if (i <= NP - j - 1 && k <= NP - j - 1)
	  f[i][k] = termf + term + ksqrd * termb;
	if (i > 0 && k > 0)
	  b[i - 1][k - 1] = termb + term + ksqrd * termf;
	term = *kPtr * (termb + termf);
	if (i <= NP - j - 1 && k > 0)
	  c[i][k - 1] = termc1 + term + ksqrd * termc2;
	if (k <= NP - j - 1 && i > 0)
	  c[k][i - 1] = termc2 + term + ksqrd * termc1;
      }
    }
    kPtr++;
  }
  free(freePtr);
}				/* end of FLATV */


/*------------------------------------------------------------------------*/
/*	QUANT -- quantization routine*/
/**/
/*	Note: if value to be quantized equals boundary value, this routine*/
/*		rounds up*/
/**/
static FTYPE    QUANT(val, i)
  FTYPE           val;
  int             i;
{
  int             offset;	/* holds the address offset for the next
				 * binary search update */

  FTYPE          *tmpPtr, *tmpPtr2, *begin;

  tmpPtr = *(r0kAddr + i) - 1;
  begin = tmpPtr;
  offset = (*(r0kAddr + i + 1) - tmpPtr) / 2;
  tmpPtr2 = tmpPtr + offset;
  offset /= 2;

  while ((tmpPtr2 - tmpPtr) != 1)
  {
    if (val >= *tmpPtr2)
      tmpPtr = tmpPtr2;
    tmpPtr2 = tmpPtr + offset;
    offset /= 2;
  }
  *codes = ((tmpPtr2 - begin) / 2) & 0xff;

  codes++;
  return *tmpPtr2;
}				/* end of quant */


/*------------------------------------------------------------------------*/
/*	lookup -- routine for decoding r0 and k's*/
/**/
FTYPE           lookup(i)
  int             i;
{
  FTYPE           rVal;

  rVal = *(*(r0kAddr + i) + 2 * *codes);
  codes++;
  return rVal;
}				/* end of lookup */
