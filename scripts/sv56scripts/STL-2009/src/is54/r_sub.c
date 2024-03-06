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

/*-------------------------------------------------------------------*/
/*	r_sub.c -- receive subframe processing.*/
/*-------------------------------------------------------------------*/
/*	Written by: Matt Hartman*/
/*-------------------------------------------------------------*/
 /* inclusions*/
#include "r_sub.h"

void            R_SUB()
{
  static FTYPE    SYN_STATE[NP];/* synthesis filter state */
  static FTYPE    POST_STATE_N[NP];	/* post-filter numerator state */
  static FTYPE    POST_STATE_D[NP];	/* post-filter denominator state */
  static FTYPE    POST_STATE_E;	/* post-emphasis filter state */
  static FTYPE    runningGain;	/* post-filter agc gain applied on 
                                 * sample-by-sample basis */
  FTYPE           beta;		/* pitch excitation gain */
  FTYPE           preEnergy;	/* energy before filter (both agc's) */
  FTYPE           postEnergy;	/* energy after filter (both agc's) */
  FTYPE           gain;		/* post-filter agc gain for subframe */
  int             R_LAG;	/* received lag */
  int             R_CODE;	/* received codeword for 1st codebook */
  int             R_CODE_A;	/* received codeword for 2nd codebook */
  int             R_GSP0;	/* received GSP0(P1) centroid */

  FTYPE          *tmpPtr, *tmpPtr2, *endPtr, temp1, temp2;

  /* retrieve codes from code buffer */
  if (*codes)
    R_LAG = *codes + LMIN - 1;
  else
    R_LAG = 0;
  codes++;
  R_CODE = *codes;
  codes++;
  R_CODE_A = *codes;
  codes++;
  R_GSP0 = *codes;
  codes++;

  /* construct pitch vector */
  if (R_LAG)
    P_EX(P_VEC, R_P_STATE, R_LAG);

  /* construct 1st-codebook excitation */
  B_CON(R_CODE, C_BITS, BITS);
  V_CON(BASIS, BITS, C_BITS, X_VEC);

  /* construct 2nd-codebook excitation */
  B_CON(R_CODE_A, C_BITS_A, BITS);
  V_CON(BASIS_A, BITS, C_BITS_A, X_A_VEC);

  /* if there is a pitch vector, get sqrt(rs/energy in pitch) */
  if (R_LAG)
    RS00 = RS_RR(P_VEC, RS);

  /* get sqrt(rs/energy in 1st-codebook excitation) */
  RS11 = RS_RR(X_VEC, RS);

  /* get sqrt(rs/energy in 2nd-codebook excitation) */
  RS22 = RS_RR(X_A_VEC, RS);

  /* scale and combine excitations, put result in T_VEC */
  beta = EXCITE(R_GSP0, R_LAG, RS00, RS11, RS22, P_VEC, X_VEC, X_A_VEC, T_VEC);

  /* perform one subframe's worth of delay on R_P_STATE */
  tmpPtr = R_P_STATE;
  tmpPtr2 = R_P_STATE + S_LEN;
  for (endPtr = R_P_STATE + LMAX; tmpPtr2 < endPtr; tmpPtr2++, tmpPtr++)
    *tmpPtr = *tmpPtr2;

  /* update the last subframe's worth of data in R_P_STATE with excitation */
  tmpPtr2 = T_VEC;
  for (; tmpPtr < endPtr; tmpPtr++, tmpPtr2++)
    *tmpPtr = *tmpPtr2;

  /* synthesize speech and put in output buffer */
  DIR(T_VEC, outBuf, SYN_STATE, COEF, S_LEN);

  /* adaptive postfilter */
  /* compute original energy in output speech for agc */
  preEnergy = 0.0;
  tmpPtr = outBuf;
  for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
    preEnergy += *tmpPtr * *tmpPtr;

  if (apply_postfilter)
  {
    /* implement spectral postfilter  */
        I_DIR(outBuf, outBuf, POST_STATE_N, N_COEF, S_LEN);
    DIR(outBuf, outBuf, POST_STATE_D, W_COEF, S_LEN);

    /* first order emphasis filter (boosts high frequencies)  */
        tmpPtr = outBuf;
    tmpPtr2 = tmpPtr;
    temp1 = *tmpPtr - POST_EMPH * POST_STATE_E;
    tmpPtr++;
    for (endPtr = outBuf + S_LEN; tmpPtr < endPtr; tmpPtr++, tmpPtr2++)
    {
      temp2 = *tmpPtr - POST_EMPH * *tmpPtr2;
      *tmpPtr2 = temp1;
      temp1 = temp2;
    }
    POST_STATE_E = *tmpPtr2;
    *tmpPtr2 = temp1;

    /* compute energy in post-filtered speech, compute new gain, scale */
    /* speech, and leave in outBuf */
        postEnergy = 0.0;
    tmpPtr = outBuf;
    for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
      postEnergy += *tmpPtr * *tmpPtr;

    gain = (postEnergy == 0.0) ? 0.0 : sqrt(preEnergy / postEnergy);

    temp1 = 1.0 - POST_AGC_COEF;
    tmpPtr = outBuf;
    for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
    {
      runningGain = gain * temp1 + runningGain * POST_AGC_COEF;
      *tmpPtr *= runningGain;
    }
  }
}
/* ........................... end of R_SUB ............................... */
