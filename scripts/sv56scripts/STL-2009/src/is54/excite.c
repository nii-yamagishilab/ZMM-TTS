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
/*	excite.c -- Scales and combines excitations.*/
/**/
/*-------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

FTYPE           EXCITE(gsp0, lag, rs00, rs11, rs22, pVecPtr, x1VecPtr, x2VecPtr, oPtr)
  int             gsp0;
  int             lag;
  FTYPE           rs00;
  FTYPE           rs11;
  FTYPE           rs22;
  FTYPE          *pVecPtr;
  FTYPE          *x1VecPtr;
  FTYPE          *x2VecPtr;
  FTYPE          *oPtr;
{
  FTYPE          *gsp0Ptr;	/* points into the GSP0 quantization table to */
  /* retrieve values needed to determine gains */
  FTYPE           beta;		/* the pitch excitation gain */
  FTYPE           gamma1;	/* the 1st codebook excitation gain */
  FTYPE           gamma2;	/* the 2nd codebook excitation gain */

  FTYPE          *tmpPtr, *endPtr;

/*	get beta, scale pitch, and put into combined excitation buffer*/
/*	(if no pitch, zero-out buffer)*/
/**/
/*	Note: factor of 0.5 applied to gains offsets the factor of 2 in*/
/*		the table entries*/
/**/
  gsp0Ptr = GSP0_TABLE + GSP0_TERMS * gsp0;
  if (lag)
    beta = 0.5 * rs00 * *gsp0Ptr;
  else
    beta = 0.0;
  tmpPtr = oPtr;
  for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
  {
    *tmpPtr = beta * *pVecPtr;
    pVecPtr++;
  }

/*	get gamma1, scale 1st-codebook excitation and add to combined*/
/*	excitation*/
/**/
  gsp0Ptr++;
  gamma1 = 0.5 * rs11 * *gsp0Ptr;
  tmpPtr = oPtr;
  for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
  {
    *tmpPtr += gamma1 * *x1VecPtr;
    x1VecPtr++;
  }

/*	get gamma2, scale 2nd-codebook excitation and add to combined*/
/*	excitation*/
/**/
  gsp0Ptr++;
  gamma2 = 0.5 * rs22 * *gsp0Ptr;
  tmpPtr = oPtr;
  for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
  {
    *tmpPtr += gamma2 * *x2VecPtr;
    x2VecPtr++;
  }
  return beta;
}				/* end of excite */
