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
/*	r_sub.h -- includes and function declarations for r_sub.c.*/
/*-------------------------------------------------------------------*/
/*	Written by: Matt Hartman*/
/*-------------------------------------------------------------*/
/*	inclusions*/
#include "vparams.h"

/*#include "stdlib.h"*/
#include <math.h>

/*	externals (defined in externDef.i, malloc'd in calcParams.c)*/
extern FTYPE    RS00, RS11, RS22;

/*-------------------------------------------------------------------------*/
/* Function declarations ...  */

/*	from filters.c*/
void            I_DIR();
/*FTYPE *inPtr, FTYPE *oPtr, FTYPE *stPtr, FTYPE *coefPtr, int len*/
void            DIR();
/*FTYPE *inPtr, FTYPE *oPtr, FTYPE *stPtr, FTYPE *coefPtr, int len*/

/*	from p_ex.c*/
void            P_EX();
/*FTYPE *oPtr, FTYPE *psPtr, int lag*/

/*	from b_con.c*/
void            B_CON();
/*int codeWord, int numBits, FTYPE *bitArray*/

/*	from v_con.c*/
void            V_CON();
/*FTYPE *basisPtr, FTYPE *bitArray, int numBasis, FTYPE *oPtr*/

/*	from rs_rr.c*/
FTYPE           RS_RR();
/*FTYPE *vecPtr, FTYPE rs*/

/*	from excite.c*/
FTYPE           EXCITE();
/*int gsp0, int lag, FTYPE rs00, FTYPE rs11, FTYPE rs22,
		FTYPE *pVecPtr, FTYPE *x1VecPtr, FTYPE *x2VecPtr,
		FTYPE *oPtr*/

/* ......................... End of file r_sub.h ........................... */
