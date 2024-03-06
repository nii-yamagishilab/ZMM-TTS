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
/*	v_con.c -- Constructs codebook excitation vector.*/
/**/
/*-------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

void            V_CON(basisPtr, bitArray, numBasis, oPtr)
  FTYPE          *basisPtr;
  FTYPE          *bitArray;
  int             numBasis;
  FTYPE          *oPtr;
{
  FTYPE          *tmpPtr, *endPtr, *endPtr2;

/*	initialize output with contribution from first basis vector*/
/**/
  tmpPtr = oPtr;
  for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
  {
    *tmpPtr = *bitArray * *basisPtr;
    basisPtr++;
  }

/*	add contributions from other vectors*/
/**/
  bitArray++;
  for (endPtr = bitArray + numBasis - 1; bitArray < endPtr; bitArray++)
  {
    tmpPtr = oPtr;
    for (endPtr2 = tmpPtr + S_LEN; tmpPtr < endPtr2; tmpPtr++)
    {
      *tmpPtr += *bitArray * *basisPtr;
      basisPtr++;
    }
  }
}
