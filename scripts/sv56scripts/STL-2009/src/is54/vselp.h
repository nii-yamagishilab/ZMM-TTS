/* *************************************************************************

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

  ************************************************************************* */
/*-------------------------------------------------------------------------*/
/*	vselp.h -- function declarations, external variable declarations */
/*			and definitions for vselp.c. */
/*	Written by: Matt Hartman */
/*	Date: July 3, 1990. */
/*-------------------------------------------------------------------------*/

#ifndef VSELP_H
#define VSELP_H 100

/* Inclusions */
#include <math.h>
#include "vparams.h"		/* needed for struct definitions.  */

/*-------------------------------------------------------------------------*/
/* Function declarations ...  */
/* ... from getParams.c */
void            getParams();
/*FILE *fpget */

/* ... from calcParams.c */
void            calcParams();

/* ... from initTables.c */
void            initTables();

/* ... from filt4.c */
void            FILT4();
/*FTYPE *inPtr, int len */

/* ... from flatv.c */
void            FLATV();
FTYPE           lookup();
/*int i */

/* ... from paramConv.c */
int             ATORC();
/*FTYPE *a, FTYPE *k */
int             RCTOA();
/*FTYPE *k, FTYPE *a */

/* ... from interpolate.c */
int             INTERPOLATE();
/*struct coefSet defCoefs,
		int numSets,
		struct coefSet oCoefs,
		int i,
		FTYPE rq0 */

void            I_MOV();
/*struct coefSet defSet,
	  int numSets,
	  FTYPE	rq0 */

FTYPE           RES_ENG();
/*FTYPE rq0, FTYPE *k */

/* ... from t_sub.c */
void            T_SUB();
/*int sfIndex */

/* ... from putCodesEtc.c */
long            putCodesHex();
long            putCodesBin();
/*int *paramP */
long            getCodesHex();
long            getCodesBin();
/*int *codePtr */
void            putCodesLog();

/* ... from makeCoefs.c */
void            widen();
/*FTYPE lambda, char side */
void            A_SST();
/*FTYPE *wCoefPtr, FTYPE *ssCoefPtr */

/* ... from r_sub.c */
void            R_SUB();

/* ... from weightedSnr.c */
void            printSnr();

/* ... from freeSpace.c */
void            freeSpace();

/* external definitions */
#include "edef.i"

#endif /* VSELP_H */
/* ......................... End of file vselp.h ........................... */
