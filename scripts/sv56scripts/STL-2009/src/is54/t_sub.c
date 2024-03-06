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
/**/
/*	t_sub.c -- transmit subframe processing.*/
/**/
/*-------------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "t_sub.h"

void            T_SUB(sfIndex)
  int             sfIndex;
{
  static FTYPE   *I_PTR;	/* points to beginning of current subframe */
  /* of input speech */
  static FTYPE    W_STATE_N[NP];/* speech weighting filter numerator state */
  static FTYPE    W_STATE_D[NP];/* speech weighting filter denominator state */
  static FTYPE    SYN_STATE_W[NP];	/* weighted synthesis filter state */
  int             T_LAG;	/* holds the chosen lag */
  int             T_CODE;	/* holds the chosen codeword from the 1st */
  /* codebook */
  int             T_CODE_A;	/* holds the chosen codeword from the 2nd */
  /* codebook */
  int             T_GSP0;	/* holds the code for the chosen GSP0 */
  /* centriod */

  FTYPE          *tmpPtr, *tmpPtr2, *tmpPtr3, *endPtr, *endPtr2;

  static FTYPE    tState[NP];	/* needed for performance analysis */
  FTYPE          *tVec;		/* needed for performance analysis */

  if (performMeas)
    tVec = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));

  if (sfIndex == 0)
    I_PTR = inBuf;
  else
    I_PTR += S_LEN;

/*	apply W(z) to input speech, and get the zero-input response of H(z)*/
/**/
  tmpPtr = T_STATE;
  tmpPtr2 = SYN_STATE_W - 1;
  for (endPtr = tmpPtr + NP; tmpPtr < endPtr; tmpPtr++)
    *tmpPtr = *++tmpPtr2;
  I_DIR(I_PTR, T_VEC, W_STATE_N, COEF, S_LEN);
  DIR(T_VEC, P, W_STATE_D, W_COEF, S_LEN);
  ZI_DIR(T_VEC, T_STATE, W_COEF, S_LEN);

/*	subtract zero-input response from weighted speech*/
/**/
  tmpPtr = P;
  tmpPtr2 = T_VEC - 1;
  for (endPtr = tmpPtr + S_LEN; tmpPtr < endPtr; tmpPtr++)
    *tmpPtr -= *++tmpPtr2;

/*	do lag search; if lag is not zero, contruct pitch vector excitation*/
/*	and send it thru weighting filter*/
/**/
  if ((T_LAG = LAG_SEARCH()) != 0)
  {
    P_EX(P_VEC, T_P_STATE, T_LAG);
    tmpPtr = T_STATE;
    for (endPtr = tmpPtr + NP; tmpPtr < endPtr; tmpPtr++)
      *tmpPtr = 0.0;
    DIR(P_VEC, W_P_VEC, T_STATE, W_COEF, S_LEN);
  }

/*	weight the basis vectors*/
/**/
  tmpPtr = BASIS;
  tmpPtr2 = W_BASIS;
  for (endPtr = tmpPtr + S_LEN * C_BITS; tmpPtr < endPtr; tmpPtr += S_LEN)
  {
    tmpPtr3 = T_STATE;
    for (endPtr2 = tmpPtr3 + NP; tmpPtr3 < endPtr2; tmpPtr3++)
      *tmpPtr3 = 0.0;
    DIR(tmpPtr, tmpPtr2, T_STATE, W_COEF, S_LEN);
    tmpPtr2 += S_LEN;
  }

/*	if there is a pitch vector, decorrelate the weighted basis vectors*/
/*	from it, and put them back in W_BASIS*/
/**/
  if (T_LAG)
    DECORR(W_P_VEC, W_BASIS, C_BITS);

/*	do the VSELP codebook search on the weighted, decorrelated basis*/
/*	vectors.*/
/**/
  T_CODE = V_SRCH(P, W_BASIS, C_BITS);

/*	construct the chosen VSELP code vector from the basis vectors*/
/**/
  B_CON(T_CODE, C_BITS, BITS);
  V_CON(BASIS, BITS, C_BITS, X_VEC);

/*	construct the weighted, decorrelated, 1st-codebook vector*/
/**/
  V_CON(W_BASIS, BITS, C_BITS, W_X_VEC);

/*	weight the 2nd basis vector set*/
/**/
  tmpPtr = BASIS_A;
  tmpPtr2 = W_BASIS;
  for (endPtr = tmpPtr + S_LEN * C_BITS_A; tmpPtr < endPtr; tmpPtr += S_LEN)
  {
    tmpPtr3 = T_STATE;
    for (endPtr2 = tmpPtr3 + NP; tmpPtr3 < endPtr2; tmpPtr3++)
      *tmpPtr3 = 0.0;
    DIR(tmpPtr, tmpPtr2, T_STATE, W_COEF, S_LEN);
    tmpPtr2 += S_LEN;
  }

/*	if there is a pitch vector, decorrelate the 2nd set of weighted basis*/
/*	vectors from it, and put them back in W_BASIS*/
/*    */
  if (T_LAG)
    DECORR(W_P_VEC, W_BASIS, C_BITS_A);

/*	decorrelate the 2nd set of weighted, decorrelated-from-pitch-vector*/
/*	basis vectors from the weighted, 1st-codebook excitation*/
/**/
  DECORR(W_X_VEC, W_BASIS, C_BITS_A);

/*	do the VSELP codebook search on the weighted, decorrelated,*/
/*	2nd-codebook vectors*/
/**/
  T_CODE_A = V_SRCH(P, W_BASIS, C_BITS_A);

/*	construct the chosen VSELP code vector from the 2nd set of*/
/*	basis vectors*/
/**/
  B_CON(T_CODE_A, C_BITS_A, BITS);
  V_CON(BASIS_A, BITS, C_BITS_A, X_A_VEC);

/*	weight the 1st-codebook vector*/
/**/
  tmpPtr = T_STATE;
  for (endPtr = tmpPtr + NP; tmpPtr < endPtr; tmpPtr++)
    *tmpPtr = 0.0;
  DIR(X_VEC, W_X_VEC, T_STATE, W_COEF, S_LEN);

/*	weight the 2nd-codebook vector*/
/**/
  tmpPtr = T_STATE;
  for (endPtr = tmpPtr + NP; tmpPtr < endPtr; tmpPtr++)
    *tmpPtr = 0.0;
  DIR(X_A_VEC, W_X_A_VEC, T_STATE, W_COEF, S_LEN);

/*	if there is a pitch vector, get sqrt(rs/energy in pitch)*/
/**/
  if (T_LAG)
    RS00 = RS_RR(P_VEC, RS);

/*	get sqrt(rs/energy in 1st-codebook excitation)*/
/**/
  RS11 = RS_RR(X_VEC, RS);

/*	get sqrt(rs/energy in 2nd-codebook excitation)*/
/**/
  RS22 = RS_RR(X_A_VEC, RS);

/*	do gain quantization*/
/**/
  T_GSP0 = G_QUANT(T_LAG, RS00, RS11, RS22);

/*	put codes into code buffer*/
/**/
  if (T_LAG)
    *codes = (T_LAG - LMIN + 1) & 0xff;
  else
    *codes = 0;
  codes++;
  *codes = T_CODE & 0xff;
  codes++;
  *codes = T_CODE_A & 0xff;
  codes++;
  *codes = T_GSP0 & 0xff;
  codes++;

/*	perform one subframe's worth of delay on ltp state, T_P_STATE*/
/**/
  tmpPtr = T_P_STATE;
  tmpPtr2 = T_P_STATE + S_LEN;
  for (endPtr = T_P_STATE + LMAX; tmpPtr2 < endPtr; tmpPtr2++)
  {
    *tmpPtr = *tmpPtr2;
    tmpPtr++;
  }

/*	scale and combine excitations, put result at end of ltp state*/
/**/
  EXCITE(T_GSP0, T_LAG, RS00, RS11, RS22, P_VEC, X_VEC, X_A_VEC, xmtExPtr);

/*	perform weighting filter, H(z), only to get state*/
/**/
  DIR(xmtExPtr, T_VEC, SYN_STATE_W, W_COEF, S_LEN);
  if (performMeas)
  {
    tmpPtr = T_STATE;
    for (endPtr = tmpPtr + NP; tmpPtr < endPtr; tmpPtr++)
      *tmpPtr = 0.0;
    DIR(xmtExPtr, T_VEC, T_STATE, W_COEF, S_LEN);	/* weight combined excit */
    DIR(xmtExPtr, tVec, tState, COEF, S_LEN);	/* synthesize speech */
    runningSnr(I_PTR, tVec, P, T_VEC);	/* compute performance */
    free(tVec);
  }
}				/* end of T_SUB */
