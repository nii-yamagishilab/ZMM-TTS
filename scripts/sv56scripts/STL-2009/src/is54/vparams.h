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
/*------------------------------------------------------------- */
/*	vselpParams.h -- Version-specific parameters for vselp. */
/*------------------------------------------------------------- */
/*	Written by: Matt Hartman */
/*------------------------------------------------------------- */

#ifndef VPARAMS_H
#define VPARAMS_H

/* Define floating point numbers as float */
typedef FLOAT_TYPE FTYPE;

/* Inclusions */
#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------------------*/
/* Parameter definitions */

/* LPC parameters common to all systems*/
#define	NP	10		/* Order of prediction filter */
#define	SST_BEQ	80		/* Bandwidth widening factor used to smooth 
                                 * spectrum in FLATV() */
#define	ALENPL	10		/* Amount added to frame length to get
				 * analysis length used in FLATV() */
#define	PREEMPH	0.0		/* Input speech pre-emphasis coefficient,
				 * used in FLATV() */
#define	SRATE	8000.0		/* The sampling rate in Hertz */

/*---------------------------------------------------------------*/
/*	structure definitions */
#ifndef DONE_STRUCTS
struct coefSet
{
  FTYPE           rq0;
  FTYPE          *k;
  FTYPE          *a;
  FTYPE          *widen;
  FTYPE          *sst;
};
#define DONE_STRUCTS
#endif

/*-------------------------------------------------------------*/
/* System flags*/
extern int      apply_postfilter, encode_only, decode_only, makeLog, 
                packedStream, performMeas;

/* File pointers */
extern FILE    *fpin, *fpout, *fpcode, *fplog, *fpstream, *fpxtest;

/* Constant externals */
extern int      N_SUB, C_BITS, C_BITS_A, PLEN, LMIN, LMAX;
extern FTYPE    F_TIME, W_ALPHA, POST_W_D, POST_BEQ_N, POST_EMPH, POST_AGC_COEF;

/* These are also constant, but must be calculated. */
extern int      GSP0_TERMS, GSP0_NUM, F_LEN, A_LEN, S_LEN, INBUFSIZ, numCodes;

/* Other externals. */
extern int     *codes, *codeBuf;
extern FTYPE   *inBuf, *I_CBUFF, *RS_BUFF, *COEF, *W_COEF, *N_COEF, RS, *xmtExPtr;
extern struct coefSet T_NEW, T_OLD, R_NEW, R_OLD;

/* External declarations for allocations to be freed */
extern FTYPE   *sst, *r0kAddr[]; /* from flatv.c */
extern FTYPE   *T_STATE, *T_VEC, *P, *P_VEC, *W_P_VEC, *BASIS, *BASIS_A, *W_BASIS,
               *BITS, *X_VEC, *W_X_VEC, *X_A_VEC, *W_X_A_VEC, *T_P_STATE,
               *GSP0_TABLE, *R_P_STATE;
extern int     *TABLE;
extern FTYPE   *P_SST;		/* from A_SST */
extern FTYPE   *outBuf;

/* DIAGnostic variables for debugging */
extern int      frCnt, sfCnt;
extern char     printAll, showr0k, showlag, showcode, showgsp0;

#endif /* VPARAMS_H */
/* ........................ End of file VPARAMS.H ........................ */

