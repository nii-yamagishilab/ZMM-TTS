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
/*	decorr.c -- Decorrelation of basis vectors from given vector.*/
/**/
/*-------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

void            DECORR(vecPtr, basisPtr, numBasis)
  FTYPE          *vecPtr;
  FTYPE          *basisPtr;
  int             numBasis;
{
  FTYPE           gamma = 0.0;	/* Energy in the given vector (i.e. the
				 * vector from */
  /* which the basis set will be decorrelated) */
  FTYPE           psi;		/* Energy in the current basis vector */
  FTYPE           proj;		/* Gamma / psi */
  FTYPE          *bvPtr;	/* Points to the basis vector currently being */
  /* decorrelated */

  FTYPE          *tmpPtr, *tmpPtr2, *endPtr, *endPtr2;

/*	compute energy in the given vector*/
/**/
  tmpPtr = vecPtr;
  for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
    gamma += *tmpPtr * *tmpPtr;

/*	decorrelate each basis vector from the given vector*/
/**/
  bvPtr = basisPtr;
  for (endPtr = bvPtr + S_LEN * numBasis; bvPtr < endPtr; bvPtr += S_LEN)
  {
    /* get dot-product of basis vector and given vector */
    /**/
    psi = 0.0;
    tmpPtr = bvPtr;
    tmpPtr2 = vecPtr;
    for (endPtr2 = tmpPtr + S_LEN; tmpPtr < endPtr2; tmpPtr++)
    {
      psi += *tmpPtr * *tmpPtr2;
      tmpPtr2++;
    }

    /* subtract component of basis vector that is in the direction */
    /* of the given vector */
    /**/
    proj = psi / gamma;
    tmpPtr = bvPtr;
    tmpPtr2 = vecPtr;
    for (endPtr2 = tmpPtr + S_LEN; tmpPtr < endPtr2; tmpPtr++)
    {
      *tmpPtr = *tmpPtr - proj * *tmpPtr2;
      tmpPtr2++;
    }
  }
}
