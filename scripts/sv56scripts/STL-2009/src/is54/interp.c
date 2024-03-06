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
/*	interpolate.c -- direct-form coef interpolation routine.*/
/*-------------------------------------------------------------------*/
/*	Written by: Matt Hartman*/
/*-------------------------------------------------------------*/
/*	inclusions*/
#include <assert.h>
#include <math.h>
#include "vparams.h"

int             ATORC();   /* from paramConv.c*/
                /*FTYPE *a, FTYPE *k*/
void            I_MOV();    /* from here*/
                /*struct coefSet defSet, int numSets, FTYPE rq0*/
FTYPE           RES_ENG();
                /*FTYPE rq0, FTYPE *k*/

static FTYPE   *aPtr;		/* points (in I_CBUFF) to the beginning of
				 * the interpolated direct-form coefs for 
				 * the current subframe */
static FTYPE   *rsPtr;		/* points (in rsBuf) to the next spot to be
				 * filled by an rs value */



/*	
 *      INTERPOLATE interpolates between coef sets based on the subframe
 *	index.
 */
int             INTERPOLATE(defCoefs, numSets, oCoefs, i, rq0)
  struct coefSet  defCoefs;
  int             numSets;
  struct coefSet  oCoefs;
  int             i;
  FTYPE           rq0;
{
  static FTYPE   *intCoefPtr;	/* points (in I_CBUFF) to the next spot for */
  /* an interpolated coef */
  FTYPE           defPct;	/* percentage that defCoefs contribute to */
  /* interpolated coefficients */
  FTYPE           oPct;		/* percentage that oCoefs contribute to
				 * interpolated */
  /* coefficients */
  FTYPE          *defPtr;	/* points to the direct-form coefficients in
				 * the */
  /* default set */
  FTYPE          *oPtr;		/* points to the direct-form coefficients in
				 * the */
  /* other set */
  FTYPE          *kPtr;		/* points to buffer that gets k's from
				 * atorc() */
  int             unstableFlag;	/* set if interpolated coefs yield unstable */
  /* filter */

  FTYPE          *endPtr, fTmp;

  kPtr = (FTYPE *) malloc(NP * sizeof(FTYPE));

/*	get interpolation percentages to use from last and current coefs*/
  fTmp = (FTYPE) (i + 1) / (FTYPE) N_SUB;
  defPct = (fTmp >= 0.5) ? fTmp : 1.0 - fTmp;
  oPct = 1.0 - defPct;

/*	interpolate coef sets*/
  if (i == 0)
  {
    intCoefPtr = I_CBUFF;
    aPtr = I_CBUFF;
    rsPtr = RS_BUFF;
  }

  defPtr = defCoefs.a;
  oPtr = oCoefs.a;
  for (endPtr = defPtr + numSets * NP; defPtr < endPtr; defPtr++)
  {
    *intCoefPtr = *defPtr * defPct + *oPtr * oPct;
    intCoefPtr++;
    oPtr++;
  }

/*	convert to k's and check stability; if unstable, move default set*/
/*	(i.e., the last or current set) and calculate residual estimate*/
/*	based on that set.  If stable, calculate residual based on*/
/*	interpolated set.*/
  if ((unstableFlag = ATORC(aPtr, kPtr)) == 1)
    I_MOV(defCoefs, numSets, rq0);
  else
  {
    *rsPtr = RES_ENG(rq0, kPtr);
    rsPtr += 2;
  }
  aPtr += numSets * NP;
  free(kPtr);
  return unstableFlag;
}


/*	I_MOV moves a set of coefs into the I_CBUFF buffer and calculates*/
/*	an RS value based on that set*/
void            I_MOV(defSet, numSets, rq0)
  struct coefSet  defSet;
  int             numSets;
  FTYPE           rq0;
{
  FTYPE          *tmpPtr, *tmpPtr2, *endPtr;

/*	calculate residual estimate*/
  *rsPtr = RES_ENG(rq0, defSet.k);
  rsPtr += 2;

/*	move default coef set to interpolated set buffer.*/
  tmpPtr = aPtr;
  tmpPtr2 = defSet.a - 1;
  for (endPtr = tmpPtr + numSets * NP; tmpPtr < endPtr; tmpPtr++)
    *tmpPtr = *++tmpPtr2;
}


/*	RES_ENG calculates a residual energy estimate*/
FTYPE           RES_ENG(rq0, k)
  FTYPE           rq0;
  FTYPE          *k;
{
  FTYPE          *endPtr /* , tmp = 1.0 */, ftmp;
  double tmp=1.0;

/*	perform canonic product, take square root, multiply by rq0*/
  for (endPtr = k + NP; k < endPtr; k++)
    tmp *= 1.0 - *k * *k;
  ftmp = tmp;
  assert(tmp>=0);
  tmp = sqrt(tmp);
  tmp *= rq0;
  return (FTYPE)tmp;
}


