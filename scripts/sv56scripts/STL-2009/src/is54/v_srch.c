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
/*	v_srch.c -- VSELP codebook search.*/
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

int             V_SRCH(wiPtr, wBasisPtr, numBasis)
  FTYPE          *wiPtr;
  FTYPE          *wBasisPtr;
  int             numBasis;
{
  FTYPE          *R;		/* array of Rm values, the cross correlations
				 * between */
  /* the weighted speech and weighted basis vectors */
  FTYPE          *D;		/* matrix of Dmj values, the cross
				 * correlations */
  /* between the weighted basis vectors */
  FTYPE           Dcurrent;	/* temporary storage of Dmj value */
  FTYPE           C;		/* cross correlation between current weighted */
  /* codebook vector and weighted speech */
  FTYPE           G;		/* energy in current weighted codebook vector */
  FTYPE           cSave;	/* C value corresponding to current best C**2
				 * / G */
  FTYPE           cSqrdBest;	/* C**2 value corresponding to current best
				 * C**2 / G */
  FTYPE           gBest;	/* G value corresponding to current best C**2
				 * / G */

  int             Ddim;		/* dimension of Dmj array */
  int             codeWord;	/* current codeword in codebook search */
  int             bitChanged;	/* bit postion that changed from the last
				 * codeword */
  int             mask;		/* mask of bit that changed from the last
				 * codeword */
  int             theta;	/* 0 if bit that changed is now 0, 1 if 1 */
  int             wordSave;	/* current best codeword */
  int             tmpMask;	/* temporary mask used in G update */

  FTYPE          *tmpPtr, *tmpPtr2, *endPtr, *endPtr2, *fp;
  int            *intPtr, m, j, *iePtr;

  Ddim = numBasis + 1;
  fp = (FTYPE *) calloc(Ddim, sizeof(FTYPE));	/* some of these */
  R = fp;			/* aren't used. */
  D = (FTYPE *) calloc(Ddim * Ddim, sizeof(FTYPE));	/**/

/*	calculate correlations between weighted basis vectors and weighted*/
/*	speech vector (Rm's), calculate C0, and calculate 0.25 * sum of Djj*/
/*	for G0.*/
/**/
  C = 0.0;
  G = 0.0;
  R++;
  tmpPtr = wBasisPtr;
  for (endPtr = tmpPtr + numBasis * S_LEN; tmpPtr < endPtr;)
  {
    tmpPtr2 = wiPtr;
    for (endPtr2 = tmpPtr2 + S_LEN; tmpPtr2 < endPtr2; tmpPtr2++)
    {
      *R += *tmpPtr * *tmpPtr2;
      G += *tmpPtr * *tmpPtr;
      tmpPtr++;
    }
    C -= *R;
    *R *= 2.0;
    R++;
  }
  R -= Ddim;

/*	calculate all Dmj (no diagonal terms since they aren't used in*/
/*	recursion); finish calculating G0*/
/**/
  for (m = 1; m < numBasis; m++)
  {
    for (j = m + 1; j <= numBasis; j++)
    {
      Dcurrent = 0.0;
      tmpPtr = wBasisPtr + (m - 1) * S_LEN;
      tmpPtr2 = wBasisPtr + (j - 1) * S_LEN;
      for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++, tmpPtr2++)
	Dcurrent += *tmpPtr * *tmpPtr2;	/* Dmj */
      G += 2.0 * Dcurrent;
      *(D + m * Ddim + j) = 4.0 * Dcurrent;
    }
  }

/*	initialize best vector to be code vector zero and perform search*/
/**/
  cSave = C;
  cSqrdBest = C * C;
  gBest = G;
  wordSave = 0;
  intPtr = TABLE;
  for (iePtr = intPtr + (1 << numBasis) - 2; intPtr < iePtr; intPtr++)
  {
    codeWord = *intPtr;
    bitChanged = *++intPtr;	/* bitChanged is in [0,numBasis-1] */
    mask = 0x1 << bitChanged;	/* mask set to correct value */
    bitChanged++;		/* bitChanged is in [1,numBasis] */

    /* update C */
    /**/
    if (theta = !(!(codeWord & mask)))	/* theta is 0 or 1 */
      C += *(R + bitChanged);
    else
      C -= *(R + bitChanged);

    /* update G */
    /**/
    tmpMask = 0x1;
    for (j = 1; j < bitChanged; j++)
    {
      if (theta == !(!(codeWord & tmpMask)))
	G += *(D + j * Ddim + bitChanged);
      else
	G -= *(D + j * Ddim + bitChanged);
      tmpMask <<= 1;
    }
    tmpMask = mask << 1;
    for (j = bitChanged + 1; j <= numBasis; j++)
    {
      if (theta == !(!(codeWord & tmpMask)))
	G += *(D + bitChanged * Ddim + j);
      else
	G -= *(D + bitChanged * Ddim + j);
      tmpMask <<= 1;
    }

    /* check for maximum */
    /**/
    if (C * C * gBest > cSqrdBest * G)
    {
      cSqrdBest = C * C;
      gBest = G;
      wordSave = codeWord;
      cSave = C;
    }
  }

/*	if Cbest is negative, invert codeword*/
/**/
  if (cSave < 0.0)
    wordSave = wordSave ^ ((1 << numBasis) - 1);

  free(fp);
  free(D);
  return wordSave;
}
