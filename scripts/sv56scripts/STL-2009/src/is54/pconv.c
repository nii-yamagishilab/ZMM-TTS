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
/*------------------------------------------------------------------------*/
/**/
/*	paramConv.c -- conversion routines for a's to k's and vice-versa.*/
/**/
/*------------------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

/*#include "stdlib.h"*/

int             ATORC(a, k)
  FTYPE          *a;
  FTYPE          *k;
{
  FTYPE          *aNew, *aOld, *tmpPtr, *endPtr, *fp1, *fp2;
  int             i, j, unstableFlag = 0;

  fp1 = (FTYPE *) malloc(NP * sizeof(FTYPE));
  fp2 = (FTYPE *) malloc(NP * sizeof(FTYPE));
  aNew = fp1;
  aOld = fp2;

/*	move direct-form coefs to aOld*/
/**/
  for (endPtr = a + NP; a < endPtr; a++, aOld++)
    *aOld = *a;
  aOld -= NP;

/*	perform conversion from a's to k's*/
/**/
  k += NP - 1;			/* k points to k(NP) */
  *k = *(aOld + NP - 1);
  if (*k >= 1.0 || *k < -1.0)
    unstableFlag = 1;
  for (i = NP - 2; i >= 0; i--)
  {
    for (j = 0; j <= i; j++)
      *(aNew + j) = (1.0 / (1.0 - *k * *k)) * (*(aOld + j) - *k * *(aOld + i - j));
    *--k = *(aNew + i);
    tmpPtr = aNew;
    aNew = aOld;
    aOld = tmpPtr;
    if (*k >= 1.0 || *k < -1.0)
      unstableFlag = 1;
  }
  free(fp1);
  free(fp2);
  return unstableFlag;
}


int             RCTOA(k, a)
  FTYPE          *k;
  FTYPE          *a;
{
  FTYPE           ar[NP];	/* storage for direct-form coefs from
				 * recursion */
  FTYPE          *aNew;		/* pointer to new direct-form coefs in
				 * recursion */
  FTYPE          *aOld;		/* pointer to old direct-form coefs in
				 * recursion */

  FTYPE          *aTmp;
  int             i, j;

/*	initialize pointers*/
/**/
  if (NP % 2 == 0)		/* NP even... */
  {
    aNew = ar;
    aOld = a;
  }
  else
  {
    aNew = a;
    aOld = ar;
  }

/*	perform conversion recursion*/
/**/
  for (i = 0; i <= NP - 1; i++)
  {
    *(aNew + i) = *(k + i);
    for (j = 0; j <= i - 1; j++)
      *(aNew + j) = *(aOld + j) + *(k + i) * *(aOld + i - j - 1);
    aTmp = aNew;
    aNew = aOld;
    aOld = aTmp;
  }

  return 0;
}
