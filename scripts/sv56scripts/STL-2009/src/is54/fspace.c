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
/**/
/*	freeSpace.c -- frees allocated memory.*/
/**/
/*------------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

/*#include "stdlib.h"*/

void            freeSpace()
{
  free(inBuf);
  free(I_CBUFF);
  free(RS_BUFF);
  free(codeBuf);
  free(*r0kAddr);
  free(sst);
  free(T_STATE);
  free(T_VEC);
  free(P);
  free(P_VEC);
  free(W_P_VEC);
  free(BASIS);
  free(BASIS_A);
  free(W_BASIS);
  free(BITS);
  free(X_VEC);
  free(W_X_VEC);
  free(X_A_VEC);
  free(W_X_A_VEC);
  free(T_P_STATE);
  free(TABLE);
  free(GSP0_TABLE);
  free(P_SST);
  free(R_P_STATE);
  free(outBuf);
}
