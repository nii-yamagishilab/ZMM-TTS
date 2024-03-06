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

/*#include "stdlib.h"*/
#include "vparams.h"

/*
 -------------------------------------------------------------
 void FILT4(FTYPE *inPtr, int len);
 
 Description:
 High-pass filter routine.
 
 Parameters:
 inPtr ... Pointer to sample buffer to be filtered.
 len ..... Number of samples to filter.
 
 Return value:
 None.
 
 History: 
 ??.???.?? v.10 Created by Matt Hartman
 -------------------------------------------------------------
*/
void            FILT4(inPtr, len)
  FTYPE          *inPtr;
  int             len;
{
  /* coefs of HPF */
  static double   b[5] = {0.898025036, -3.59010601, 5.38416243,
                          -3.59010601, 0.898024917};   /* numerator */
  static double   a[5] = {1.0, -3.78284979, 5.37379122, -3.39733505,
                          0.806448996};                /* denominator */

  static double   x[4] = {0.0, 0.0, 0.0, 0.0};	/* numerator state */
  static double   y[4] = {0.0, 0.0, 0.0, 0.0};	/* denominator state */

  double         *endPtr, *iPtr, *oPtr, *tp1, *tp2;
  int             i, j;

  tp1 = (double *) malloc((len + 4) * sizeof(double));	/* temp buffers */
  tp2 = (double *) malloc((len + 4) * sizeof(double));
  iPtr = tp1;
  oPtr = tp2;

 /* Copy speech and states to temp buffers*/
  for (i = 0; i < len + 4; i++)
  {
    if (i < 4)
    {
      *(iPtr + i) = *(x + i);
      *(oPtr + i) = *(y + i);
    }
    else
      *(iPtr + i) = *(inPtr + i - 4);
  }

 /* Perform filter on temp speech buffer*/
  iPtr += 4;
  oPtr += 4;
  for (endPtr = oPtr + len; oPtr < endPtr; oPtr++)
  {
    *oPtr = *b * *iPtr;
    for (j = 1; j <= 4; j++)
      *oPtr += *(b + j) * *(iPtr - j) - *(a + j) * *(oPtr - j);
    *inPtr = *oPtr;
    inPtr++;
    iPtr++;
  }

 /* Save states*/
  for (i = 3; i >= 0; i--)
  {
    *(x + i) = *--iPtr;
    *(y + i) = *--oPtr;
  }
  free(tp1);
  free(tp2);
}
