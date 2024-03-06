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
/*	g_quant.c -- Does GSP0 quantization.*/
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

/*	function declarations*/
/**/
static FTYPE    corr();
/*FTYPE *vec1Ptr, FTYPE *vec2Ptr*/


int             G_QUANT(lag, rs00, rs11, rs22)
  int             lag;
  FTYPE           rs00;
  FTYPE           rs11;
  FTYPE           rs22;
{
  FTYPE           Rpc0;		/* correlation between the weighted speech
				 * and the */
  /* weighted pitch excitation vector */
  FTYPE           Rpc1;		/* correlation between the weighted speech
				 * and the */
  /* weighted 1st codebook excitation vector */
  FTYPE           Rpc2;		/* correlation between the weighted speech
				 * and the */
  /* weighted 2nd codebook excitation vector */
  FTYPE           Rcc00;	/* Rcc(mn) -- correlation between excitation
				 * vector */
  /* m and n, where vector 0 is the pitch excitation, */
  /* vector 1 is the 1st codebook excitation, and */
  /* vector 2 is the 2nd codebook excitation */
  FTYPE           Rcc01;	/* */
  FTYPE           Rcc02;	/* */
  FTYPE           Rcc11;	/* */
  FTYPE           Rcc12;	/* */
  FTYPE           Rcc22;	/* */
  FTYPE          *errCoefs;	/* array of calculated error coefficients
				 * which will */
  /* be multiplied by the table terms to get val */
  FTYPE           val;		/* error value which is maximized */
  /* (this is the portion of the error equation */
  /* which would be subtracted from Rpp, which is a */
  /* constant and thus not included) */
  FTYPE           maxVal;	/* the current best (maximum) error value */
  int             code;		/* the best GSP0 centroid, returned to
				 * T_SUB() */

  FTYPE          *savePtr, *tmpPtr, *tmpPtr2, *endPtr, *endPtr2;

  errCoefs = (FTYPE *) malloc(GSP0_TERMS * sizeof(FTYPE));

/* calculate correlations*/
/**/
  if (lag)
    Rpc0 = corr(P, W_P_VEC);
  Rpc1 = corr(P, W_X_VEC);
  Rpc2 = corr(P, W_X_A_VEC);
  if (lag)
  {
    Rcc01 = corr(W_P_VEC, W_X_VEC);
    Rcc02 = corr(W_P_VEC, W_X_A_VEC);
  }
  Rcc12 = corr(W_X_VEC, W_X_A_VEC);
  if (lag)
    Rcc00 = corr(W_P_VEC, W_P_VEC);
  Rcc11 = corr(W_X_VEC, W_X_VEC);
  Rcc22 = corr(W_X_A_VEC, W_X_A_VEC);

/* compute error coefficients (factor of 2 for some coefs is figured*/
/* into the table terms)*/
/**/
  *errCoefs = Rpc0 * rs00;
  *(errCoefs + 1) = Rpc1 * rs11;
  *(errCoefs + 2) = Rpc2 * rs22;
  *(errCoefs + 3) = Rcc01 * rs00 * rs11;
  *(errCoefs + 4) = Rcc02 * rs00 * rs22;
  *(errCoefs + 5) = Rcc12 * rs11 * rs22;
  *(errCoefs + 6) = Rcc00 * rs00 * rs00;
  *(errCoefs + 7) = Rcc11 * rs11 * rs11;
  *(errCoefs + 8) = Rcc22 * rs22 * rs22;
  if (!lag)
  {
    *errCoefs = 0.0;
    *(errCoefs + 3) = 0.0;
    *(errCoefs + 4) = 0.0;
    *(errCoefs + 6) = 0.0;
  }

/*	minimum error search loop*/
/**/
  maxVal = -50.0;
  tmpPtr = GSP0_TABLE;
  for (endPtr = tmpPtr + GSP0_TERMS * GSP0_NUM; tmpPtr < endPtr;)
  {
    /* combine error coefficients and table terms to get value to be */
    /* maximized */
    /**/
    val = 0.0;
    tmpPtr2 = errCoefs;
    for (endPtr2 = tmpPtr2 + GSP0_TERMS; tmpPtr2 < endPtr2; tmpPtr2++)
    {
      val += *tmpPtr * *tmpPtr2;
      tmpPtr++;
    }

    /* test against reference code */
    /* */
    /* save pointer if max */
    /**/
    if (val > maxVal)
    {
      maxVal = val;
      savePtr = tmpPtr;
    }
  }

/*	get code for centroid from pointer and return*/
/**/
  code = (savePtr - GSP0_TERMS - GSP0_TABLE) / GSP0_TERMS;
  free(errCoefs);
  return code;
}				/* end of G_QUANT */


static FTYPE    corr(vec1Ptr, vec2Ptr)
  FTYPE          *vec1Ptr;
  FTYPE          *vec2Ptr;
{
  FTYPE          *endPtr, R = 0.0;

  for (endPtr = vec1Ptr + S_LEN; vec1Ptr < endPtr; vec1Ptr++)
  {
    R += *vec1Ptr * *vec2Ptr;
    vec2Ptr++;
  }
  return R;
}
