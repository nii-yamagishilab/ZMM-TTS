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
/*	rs_rr.c -- Computes sqrt(RS/R(x,x)).*/
/**/
/*-------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

#include <math.h>

FTYPE           RS_RR(vecPtr, rs)
  FTYPE          *vecPtr;
  FTYPE           rs;
{
  FTYPE          *endPtr, Rxx = 0.0;

/*	compute energy in excitation vector*/
/**/
  for (endPtr = vecPtr + S_LEN; vecPtr < endPtr; vecPtr++)
    Rxx += *vecPtr * *vecPtr;

/* return (rs / sqrt(Rxx)); */
  if (Rxx > 1.e-10)
    return (rs / sqrt(Rxx));
  else
    return (0.0);
}
