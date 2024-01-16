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
/*	filters.c -- Filter routines.*/
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

/*--------------------------------------------------------------------------*/
/*	inverse (analysis, all-zero) filter*/
/**/
void            I_DIR(inPtr, oPtr, stPtr, coefPtr, len)
  FTYPE          *inPtr;
  FTYPE          *oPtr;
  FTYPE          *stPtr;
  FTYPE          *coefPtr;
  int             len;
{
  FTYPE          *cP, *iPtr, *endPtr, *freePtr;
  int             i;

/*	copy states and input into temp buffer*/
/**/
  freePtr = (FTYPE *) malloc((len + NP) * sizeof(FTYPE));
  iPtr = freePtr;
  iPtr--;
  for (endPtr = stPtr + NP; stPtr < endPtr; stPtr++)
    *++iPtr = *stPtr;
  for (endPtr = inPtr + len; inPtr < endPtr; inPtr++)
    *++iPtr = *inPtr;
  iPtr -= len - 1;

/*	perform filter*/
/**/
  for (endPtr = iPtr + len; iPtr < endPtr; iPtr++)
  {
    *oPtr = 0.0;
    cP = coefPtr - 1;
    for (i = 1; i <= NP; i++)
      *oPtr += *(cP + i) * *(iPtr - i);
    *oPtr += *iPtr;
    oPtr++;
  }

/*	update states*/
/**/
  stPtr--;
  for (endPtr = stPtr - NP; stPtr > endPtr; stPtr--)
    *stPtr = *--iPtr;

  free(freePtr);
}


/*--------------------------------------------------------------------------*/
/*	synthesis (all-pole) filter*/
/**/
void            DIR(inPtr, oPtr, stPtr, coefPtr, len)
  FTYPE          *inPtr;
  FTYPE          *oPtr;
  FTYPE          *stPtr;
  FTYPE          *coefPtr;
  int             len;
{
  FTYPE          *cP, *tmpPtr, *endPtr, *freePtr;
  int             i;

/*	copy states into temp output buffer*/
/**/
  freePtr = (FTYPE *) malloc((len + NP) * sizeof(FTYPE));
  tmpPtr = freePtr;
  for (endPtr = stPtr + NP; stPtr < endPtr; stPtr++)
  {
    *tmpPtr = *stPtr;
    tmpPtr++;
  }

/*	perform filter*/
/**/
  for (endPtr = tmpPtr + len; tmpPtr < endPtr; tmpPtr++)
  {
    *tmpPtr = 0.0;
    cP = coefPtr - 1;
    for (i = 1; i <= NP; i++)
      *tmpPtr -= *(cP + i) * *(tmpPtr - i);
    *tmpPtr += *inPtr;
    *oPtr = *tmpPtr;
    oPtr++;
    inPtr++;
  }

/*	update states*/
/**/
  stPtr--;
  for (endPtr = stPtr - NP; stPtr > endPtr; stPtr--)
    *stPtr = *--tmpPtr;

  free(freePtr);
}


void            ZI_DIR(oPtr, stPtr, coefPtr, len)
  FTYPE          *oPtr;
  FTYPE          *stPtr;
  FTYPE          *coefPtr;
  int             len;
{
  FTYPE          *cP, *tmpPtr, *endPtr, *freePtr;
  int             i;

/*	copy states into temp output buffer*/
/**/
  freePtr = (FTYPE *) malloc((len + NP) * sizeof(FTYPE));
  tmpPtr = freePtr;
  for (endPtr = stPtr + NP; stPtr < endPtr; stPtr++)
  {
    *tmpPtr = *stPtr;
    tmpPtr++;
  }

/*	perform filter without input*/
/**/
  for (endPtr = tmpPtr + len; tmpPtr < endPtr; tmpPtr++)
  {
    *tmpPtr = 0.0;
    cP = coefPtr - 1;
    for (i = 1; i <= NP; i++)
      *tmpPtr -= *(cP + i) * *(tmpPtr - i);
    *oPtr = *tmpPtr;
    oPtr++;
  }

  free(freePtr);
}
