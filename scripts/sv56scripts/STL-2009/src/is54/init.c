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
/*------------------------------------------------------------------*/
/*	initTables.c -- Table allocation and initialization.*/
/*------------------------------------------------------------------*/
/*	Written by: Matt Hartman*/
/*-------------------------------------------------------------*/
/*	Modified by: Simao F. Campos Neto - crazy but more efficient */
/*                    in run-time! UNIX/VMS allow big tables, but */
/*                    DOS doesn't; so, treat accordingly! */
/*-------------------------------------------------------------*/
#if (defined(MSDOS) || defined(__MSDOS__)) && !defined(__GNUC__)
/* ------------------------------------------------------
 *  You need to customize this, poor 16-bit DOS user ... 
 *  be smart and do the definitions in the makefile! 
 * ------------------------------------------------------
 */
# ifndef K_TABLE_FILE
#  define K_TABLE_FILE "./k_table.i"
# endif
# ifndef BASIS_FILE
#  define BASIS_FILE "./basis.i"
# endif
# define READ_COEFF_IN_RUN_TIME /* include "init-dos.c" */

#else 
/* -----------------------------------------
 *  OK, lucky guy, not a 16-bit C compiler! 
 * -----------------------------------------
 */
# ifdef READ_COEFF_IN_RUN_TIME
#  undef READ_COEFF_IN_RUN_TIME
# endif
#endif

/* inclusions */
#include "vparams.h"

/* #include "stdlib.h" */
#include <math.h>

#define	MAXLINE	133

/*------------------------------------------------------------------------*/
/* initTables -- allocates buffers and fills tables */
/* note: this routine assumes that NP is 10 */
/* define statements for number of bits in quantization of R0 and rc's */
#define R_BITS	5		/* number of bits in R0 quantizer */
#define STEP	2.0		/* dB step between quant levels for R0 */

/* initTables initializes lookup tables and allocates storage for some */
/* arrays */
void            initTables()
{
  FTYPE           DB = -66.0;	/* db value of R0 quantization step */
  int             numDecisionVals;	/* number of entries in R0 and rc
					 * quant tables */
  int             nb;		/* used to create table of sst coefs for
				 * FLATV() */
  int             K_BITS[11];	/* bit allocations for refl. coefs, read from
				 * file */
  int             downCnt;	/* used to generate gray code table */
  int             upCnt;	/* used to generate gray code table */
  int             grayMask;	/* used to generate gray code table */
  int             grayVal;	/* used to generate gray code table */
  int             bit;		/* used to generate gray code table */

  FTYPE          *tmpPtr, *endPtr, temp;
  int             i, *intPtr, *iePtr;
  FILE           *fptmp;
  char           *line;

  line = (char *) malloc(MAXLINE * sizeof(char));

 /* allocate input speech buffer, interpolated coef buffer, and residual */
 /* energy estimate buffer */
  inBuf = (FTYPE *) malloc(INBUFSIZ * sizeof(FTYPE));
  I_CBUFF = (FTYPE *) malloc((NP * 3 * N_SUB) * sizeof(FTYPE));
  RS_BUFF = (FTYPE *) malloc((2 * N_SUB) * sizeof(FTYPE));

 /* initialize codes to zero (in case 1st denominator in FLAT is zero) */
  codeBuf = (int *) calloc(numCodes, sizeof(int));

 /* FLATV() and QUANT() allocations */
 /* numDecisionVals is the amount of storage needed for the r0 and rc */
 /* quantization tables or... */
 /* 			(1<<(R_BITS+1))-1 + (1<<(K_BITS_1+1))-1 + */
 /* 			(1<<(K_BITS_2+1))-1 + (1<<(K_BITS_3+1))-1 + */
 /* 			(1<<(K_BITS_4+1))-1 + (1<<(K_BITS_5+1))-1 + */
 /* 			(1<<(K_BITS_6+1))-1 + (1<<(K_BITS_7+1))-1 + */
 /* 			(1<<(K_BITS_8+1))-1 + (1<<(K_BITS_9+1))-1 + */
 /* 			(1<<(K_BITS_10+1))-1; */
  numDecisionVals = 445;
  *r0kAddr = (FTYPE *) malloc(numDecisionVals * sizeof(FTYPE));
  sst = (FTYPE *) malloc((NP + 1) * sizeof(FTYPE));

 /* T_SUB() and R_SUB() allocations */
  T_STATE = (FTYPE *) malloc(NP * sizeof(FTYPE));
  T_VEC = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  P = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  P_VEC = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  W_P_VEC = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  BASIS = (FTYPE *) malloc(S_LEN * C_BITS * sizeof(FTYPE));
  BASIS_A = (FTYPE *) malloc(S_LEN * C_BITS_A * sizeof(FTYPE));
  W_BASIS = (FTYPE *) malloc(S_LEN * C_BITS * sizeof(FTYPE));
  BITS = (FTYPE *) malloc(C_BITS * sizeof(FTYPE));
  X_VEC = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  W_X_VEC = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  X_A_VEC = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  W_X_A_VEC = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));
  T_P_STATE = (FTYPE *) calloc(LMAX, sizeof(FTYPE));
  xmtExPtr = T_P_STATE + LMAX - S_LEN;
  TABLE = (int *) malloc(((1 << C_BITS) - 2) * sizeof(int));
  GSP0_TABLE = (FTYPE *) malloc(GSP0_TERMS * GSP0_NUM * sizeof(FTYPE));
  R_P_STATE = (FTYPE *) calloc(LMAX, sizeof(FTYPE));
  outBuf = (FTYPE *) malloc(S_LEN * sizeof(FTYPE));

 /* fill r0 quantization table */
  temp = sqrt((double) S_LEN);	/* sqrt (S_LEN * max amplitude squared) */
  tmpPtr = *r0kAddr;
  *tmpPtr = 0.0;		/* set lowest level to zero */
  tmpPtr++;
  *tmpPtr = temp * pow(10.0, (-72.0 / 20.0));
  tmpPtr++;
  DB += STEP / 2.0;
  DB += STEP / 2.0;
  for (endPtr = tmpPtr + (1 << (R_BITS + 1)) - 3; tmpPtr < endPtr; tmpPtr++)
  {
    *tmpPtr = temp * pow(10.0, (DB / 20.0));
    DB += STEP / 2.0;
  }

 /* include quantization tables for reflection coefs */
#ifndef READ_COEFF_IN_RUN_TIME
#include "k_table.i"
#else
  if ((fptmp = fopen(K_TABLE_FILE, "r")) == NULL)
  {
    printf("File not found: k_table.i\n");
    exit(1111);
  }

  for (i = 1; i <= 10; i++)
  {
    fgets(line, MAXLINE, fptmp);
    while (sscanf(line, "/* K_BITS_%*d EQU %d */", &K_BITS[i]) != 1)
      fgets(line, MAXLINE, fptmp);

  }

  for (i = 1; i <= 10; i++)
  {
    r0kAddr[i] = tmpPtr;
    for (endPtr = tmpPtr + (1 << (K_BITS[i] + 1)) - 1; tmpPtr < endPtr; tmpPtr++)
    {
      fgets(line, MAXLINE, fptmp);
      while (sscanf(line, "%*s = %e; /* ;", tmpPtr) != 1)
	fgets(line, MAXLINE, fptmp);
    }
  }
  r0kAddr[11] = tmpPtr;

  fclose(fptmp);
#endif

 /* store SST bandwidth widening factors for FLATV() */
  nb = log(0.5) / (2.0 * log(cos((4.0 * atan(1.0) * SST_BEQ) / (2.0 * SRATE))));
  temp = 1.0;
  *sst = temp;
  *++sst = 0.999644;
  *++sst = 0.998577;
  *++sst = 0.996802;
  *++sst = 0.994321;
  *++sst = 0.991141;
  *++sst = 0.987268;
  *++sst = 0.982710;
  *++sst = 0.977478;
  *++sst = 0.971581;
  *++sst = 0.965032;
  sst -= NP;

 /* include basis vectors, GSP0 quantization table, P0 sqrt table, */
 /* and gray-code table */
#ifndef READ_COEFF_IN_RUN_TIME
# include "basis.i"
#else
  if ((fptmp = fopen(BASIS_FILE, "r")) == NULL)
  {
    printf("File not found: basis.i\n");
    exit(1112);
  }

  tmpPtr = BASIS;
  for (endPtr = tmpPtr + S_LEN * C_BITS; tmpPtr < endPtr; tmpPtr++)
  {
    fgets(line, MAXLINE, fptmp);
    while (sscanf(line, "*++tmpPtr = %e;    /* SAMPLE", tmpPtr) != 1)
      fgets(line, MAXLINE, fptmp);
  }

  tmpPtr = BASIS_A;
  for (endPtr = tmpPtr + S_LEN * C_BITS_A; tmpPtr < endPtr; tmpPtr++)
  {
    fgets(line, MAXLINE, fptmp);
    while (sscanf(line, "*++tmpPtr = %e;    /* SAMPLE", tmpPtr) != 1)
      fgets(line, MAXLINE, fptmp);
  }

  tmpPtr = GSP0_TABLE;
  for (endPtr = tmpPtr + GSP0_TERMS * GSP0_NUM; tmpPtr < endPtr; tmpPtr++)
  {
    fgets(line, MAXLINE, fptmp);
    while (sscanf(line, "%*s = %e;    /* ET", tmpPtr) != 1)
      fgets(line, MAXLINE, fptmp);
  }

  fclose(fptmp);
#endif

#include "gray.i"		/* gray-code table */

 /* store SST bandwidth widening factors for A_SST() */
  P_SST = (FTYPE *) malloc((NP + 1) * sizeof(FTYPE));

  nb = log(0.5) / (2.0 * log(cos((4.0 * atan(1.0) * POST_BEQ_N) / (2.0 * SRATE))));
  temp = 1.0;
  *P_SST = temp;
  *++P_SST = 0.923077;
  *++P_SST = 0.725275;
  *++P_SST = 0.483516;
  *++P_SST = 0.271978;
  *++P_SST = 0.127990;
  *++P_SST = 0.049774;
  *++P_SST = 0.015718;
  *++P_SST = 0.003930;
  *++P_SST = 0.000748;
  *++P_SST = 0.000102;
  P_SST -= NP;

  free(line);
}
/* ........................ End of initTables() ............................ */
