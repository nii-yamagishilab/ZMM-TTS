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
/*-----------------------------------------------------------------*/
/**/
/*	grayTable.i -- generates (half of) a gray-code table.  Does not*/
/*			include the first entry (i.e. 0).*/
/**/
/*-----------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
downCnt = (1<<C_BITS) - 1;
upCnt = 1;
intPtr = TABLE;
grayVal = 0;
for (iePtr = intPtr + (1<<C_BITS) - 2; intPtr < iePtr; intPtr++)
{
    grayMask = downCnt & upCnt;
    grayVal ^= grayMask;
    for(bit = 0; grayMask != 1; grayMask >>= 1)
    	bit++;
    *intPtr = grayVal;
    intPtr++;
    *intPtr = bit;
    downCnt--;
    upCnt++;
}
