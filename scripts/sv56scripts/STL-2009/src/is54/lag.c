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
/*-------------------------------------------------------------*/
/**/
/*	lag_search.c -- Long-term predictor lag determination.*/
/**/
/*-------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

/*#include "stdlib.h"*/

/*	function declarations*/
/**/
static void     i_resp();
/*FTYPE *htPtr*/


int             LAG_SEARCH()
{
  FTYPE          *ht;		/* Points to truncated impulse response */
  FTYPE          *zl;		/* Points to zl vector, the weighted section
				 * of the */
  /* ltp state that will be overlapped and added to */
  /* get bl, the weighted pitch excitation for lag l */
  FTYPE          *bl;		/* Points to the weighted pitch excitation
				 * for lag l */
  FTYPE          *r;		/* Points one past the end of the ltp state
				 * (for */
  /* correct indexing of the r array) */
  FTYPE           Cl;		/* Holds the cross correlation between bl and
				 * P, the */
  /* weighted speech, for lag l */
  FTYPE           Gl;		/* Holds the energy in bl */
  FTYPE           E;		/* Holds the partial energy of bl for lags
				 * greater */
  /* than S_LEN */
  FTYPE           ClBest = 0.0;	/* Holds the value of Cl for the current best */
  /* Cl**2 / Gl */
  FTYPE           GlBest = 1.0;	/* Holds the value of Gl for the current best */
  /* Cl**2 / Gl */
  FTYPE           rVal;		/* Temporary storage place for r(-L) */
  int             lag;		/* the current lag */
  int             lagBest = 0;	/* holds the value of l for the current best */
  /* Cl**2 / Gl */

  FTYPE          *tmpPtr, *tmpPtr2, *tmpPtr3, *endPtr;
  int             n, i;

  ht = (FTYPE *) malloc(PLEN * sizeof(FTYPE));
  zl = (FTYPE *) calloc(S_LEN, sizeof(FTYPE));
  bl = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  r = T_P_STATE + LMAX;

/*	compute truncated impulse response of H(z)*/
/**/
  i_resp(ht);

/*	compute initial zl array*/
/**/
  for (n = 0; n < LMIN; n++)
  {
    for (i = 0; i <= n; i++)
      *(zl + n) += *(r + i - LMIN) * *(ht + n - i);
  }
  for (n = LMIN; n < PLEN; n++)
  {
    for (i = 0; i < LMIN; i++)
      *(zl + n) += *(r + i - LMIN) * *(ht + n - i);
  }
  for (n = PLEN; n < PLEN + LMIN - 1; n++)
  {
    for (i = n - PLEN + 1; i < LMIN; i++)
      *(zl + n) += *(r + i - LMIN) * *(ht + n - i);
  }
  if (LMIN + PLEN - 1 < S_LEN)
  {
    for (n = PLEN + LMIN - 1; n < S_LEN; n++)
      *(zl + n) = 0.0;
  }

/*	lag search loop*/
/**/
  for (lag = LMIN; lag <= LMAX; lag++)
  {
    /* get bl from zl (overlap and add) */
    /**/
    endPtr = bl + S_LEN;

    tmpPtr = bl;		/* copy zl into bl */
    tmpPtr2 = zl - 1;
    for (; tmpPtr < endPtr; tmpPtr++)
      *tmpPtr = *++tmpPtr2;

    tmpPtr = bl + lag;		/* compute bl elements which have */
    tmpPtr2 = zl - 1;		/* at least two terms (if any) */
    for (; tmpPtr < endPtr; tmpPtr++)
      *tmpPtr += *++tmpPtr2;

    tmpPtr = bl + 2 * lag;	/* compute bl elements which have */
    tmpPtr2 = zl - 1;		/* three terms (if any) */
    for (; tmpPtr < endPtr; tmpPtr++)
      *tmpPtr += *++tmpPtr2;

    /* compute cross-correlation between weighted speech and bl */
    /**/
    Cl = 0.0;
    tmpPtr = bl;
    tmpPtr2 = P;
    for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++, tmpPtr2++)
      Cl += *tmpPtr * *tmpPtr2;

    /* compute energy in bl (with recursion, if appilcable) */
    /**/
    Gl = 0.0;
    if (lag < S_LEN)
    {
      tmpPtr = bl;
      for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
	Gl += *tmpPtr * *tmpPtr;
    }
    else if (lag > S_LEN)
    {
      Gl = E;
      tmpPtr = bl;
      for (endPtr = tmpPtr + PLEN; tmpPtr < endPtr; tmpPtr++)
	Gl += *tmpPtr * *tmpPtr;
      E += *(zl + PLEN - 1) * *(zl + PLEN - 1) - *(zl + S_LEN - 1) * *(zl + S_LEN - 1);
    }
    else
    {
      /* lag = S_LEN: initialize partial energy E, and calculate */
      /* energy in bl. */
      /**/
      E = 0.0;
      tmpPtr = bl + PLEN;
      for (endPtr = bl + S_LEN; tmpPtr < endPtr; tmpPtr++)
	E += *tmpPtr * *tmpPtr;
      Gl = E;
      tmpPtr = bl;
      for (endPtr = tmpPtr + PLEN; tmpPtr < endPtr; tmpPtr++)
	Gl += *tmpPtr * *tmpPtr;
      E += *(zl + PLEN - 1) * *(zl + PLEN - 1) - *(zl + S_LEN - 1) * *(zl + S_LEN - 1);
    }

    /* compare C**2/G and save max */
    /**/
    if (Cl > 0.0)
    {
      if (Cl * Cl * GlBest > ClBest * ClBest * Gl)
      {
	ClBest = Cl;
	GlBest = Gl;
	lagBest = lag;
      }
    }

    /* update zl array unless done */
    /**/
    if (lag == LMAX)
      break;
    rVal = *(r - lag - 1);
    tmpPtr = zl + S_LEN - 1;
    tmpPtr2 = tmpPtr;
    for (endPtr = zl + PLEN; tmpPtr >= endPtr; tmpPtr--)
      *tmpPtr = *--tmpPtr2;
    tmpPtr3 = ht + PLEN;
    for (endPtr = zl + 1; tmpPtr >= endPtr; tmpPtr--)
      *tmpPtr = *--tmpPtr2 + rVal * *--tmpPtr3;
    *zl = rVal * *ht;
  }

  free(ht);
  free(zl);
  free(bl);
  return lagBest;
}


/*	truncated impulse response function*/
/**/
static void     i_resp(htPtr)
  FTYPE          *htPtr;
{
  FTYPE          *cP, *tmpPtr, *endPtr, *freePtr, impulse = 1.0;
  int             i;

  freePtr = (FTYPE *) calloc(PLEN + NP, sizeof(FTYPE));
  tmpPtr = freePtr;
  tmpPtr += NP;

/*	calculate response*/
/**/
  for (endPtr = tmpPtr + PLEN; tmpPtr < endPtr; tmpPtr++)
  {
    cP = W_COEF - 1;
    for (i = 1; i <= NP; i++)
      *tmpPtr -= *(cP + i) * *(tmpPtr - i);
    *tmpPtr += impulse;
    impulse = 0.0;
    *htPtr = *tmpPtr;
    htPtr++;
  }

  free(freePtr);
}
