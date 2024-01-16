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

#include <stdio.h>
/*#include "stdlib.h"*/
#include "vparams.h"

/*
 -------------------------------------------------------------
  void calcParams(void);

  calcParams initializes those constant external variables which
  depend on other constant variables.
  
  Parameters:
  None.
  
  Return value:
  None.

  History: 
  9.May.90 v.1.0  Created by Matt Hartman
 -------------------------------------------------------------
*/
void            calcParams()
{
  F_TIME = 0.02;
  N_SUB = 4;
  W_ALPHA = 0.8;
  C_BITS = 7;
  C_BITS_A = 7;
  PLEN = 21;
  POST_W_D = 0.8;
  POST_BEQ_N = 1200;
  POST_EMPH = 0.4;
  POST_AGC_COEF = 0.9875;
  GSP0_NUM = 256;

  GSP0_TERMS = 9;

  F_LEN = F_TIME * SRATE + 0.5;
  A_LEN = F_LEN + ALENPL;
  S_LEN = F_LEN / N_SUB;

  INBUFSIZ = F_LEN + A_LEN / 2 - S_LEN / 2;
  numCodes = 1 + NP + N_SUB * (1 + 2 + 1);
}
/* ...................... End of calcParams() ............................. */
