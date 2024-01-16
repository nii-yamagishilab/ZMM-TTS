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
/*	p_ex.c -- Generation of pitch excitation vector from ltp state.*/
/**/
/*-------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

void            P_EX(oPtr, psPtr, lag)
  FTYPE          *oPtr;
  FTYPE          *psPtr;
  int             lag;
{
  FTYPE          *tmpPtr, *savePtr, *savePtr2, *endPtr;

  savePtr = psPtr + LMAX;	/* savePtr set past end of LTP state */
  tmpPtr = savePtr - lag;	/* tmpPtr set to pickoff point */
  savePtr2 = tmpPtr;		/* savePtr2 set to pickoff point */
  for (endPtr = oPtr + S_LEN; oPtr < endPtr; oPtr++)
  {
    *oPtr = *tmpPtr;
    tmpPtr++;
    if (tmpPtr == savePtr)
      tmpPtr = savePtr2;
  }
}
