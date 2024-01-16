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
/* put[CodesEt]c.c -- Coded bit-stream and performance info */
/* 		output routines. */
/*-------------------------------------------------------------*/
/* Written by: Matt Hartman */
/*-------------------------------------------------------------*/
/* inclusions */
#include "vparams.h"

/* function declarations */
/* from cunpack0_.c */
/* void            unpack0_(); */
/*int *wrds, int *blen, char *packed */

/* static externals */
static int      size8000[28] = {27, 6, 5, 5, 4, 4, 3, 3, 3, 3, 2, 5, 7, 8, 
				7, 7, 7, 8, 7, 7, 7, 8, 7, 7, 7, 8, 7, 7};

/* ------------------------------------------------------------------------ */
long            putCodesHex(fpstream, paramP)
  FILE		 *fpstream;
  int            *paramP;
{
  unsigned int    param, code, temp;
  int            *sizeP, *arrangeP, sizeOfTemp, size, mask, count, 
                 *tip1, *tip2,*eip;

  arrangeP = (int *) malloc(numCodes * sizeof(int));

 /* rearrange coded parameters and set pointer to array of sizes */
  tip1 = paramP + 1;
  tip2 = arrangeP;
  for (eip = tip1 + NP; tip1 < eip; tip1++, tip2++)
    *tip2 = *tip1;		/* k's */
  *tip2 = *paramP;		/* r0 */
  tip2++;

  for (eip = arrangeP + numCodes; tip2 < eip; tip1 += 4)
  {
    *tip2 = *tip1;
    *++tip2 = *(tip1 + 3);
    *++tip2 = *(tip1 + 1);
    *++tip2 = *(tip1 + 2);
    tip2++;
  }
  sizeP = size8000 + 1;

  sizeOfTemp = 0;
  mask = 0xf;
  count = 0;
  tip1 = arrangeP;
  for (eip = tip1 + numCodes; tip1 < eip;)
  {
    while (sizeOfTemp < 4)
    {
      size = *sizeP;
      param = *tip1;
      temp = (temp << size) | param;
      sizeOfTemp += size;
      sizeP++;
      tip1++;
      if (tip1 == eip)
      {
	while (sizeOfTemp % 4 != 0)
	{
	  temp <<= 1;
	  sizeOfTemp++;
	}
      }
    }
    while (sizeOfTemp >= 4)
    {
      sizeOfTemp -= 4;
      code = temp & (mask << sizeOfTemp);
      temp -= code;
      code >>= sizeOfTemp;
      fprintf(fpstream, "%X", code);
    }
  }
  fprintf(fpstream, "\n");
  free(arrangeP);
  return((long)numCodes);
}				
/* .......................... end of putCodesHex() ......................... */


/* 
 --------------------------------------------------------------------------- 
 long putCodesBin(FILE *fpstream);
 
  Description:
  Save IS54 encoded frame into a binary file.
 
  Parameter:
  fpstream ... FILE pointer.

  Return value:
  The number of saved parameters. If shorter than IS54_FRAME_LEN, there was
  a write error.
  
  History:
  02.May.94 v1.0 Created <simao@cpqd.ansp.br>
  
 --------------------------------------------------------------------------- 
*/
#define IS54_FRAME_LEN 27
long putCodesBin(fpstream, codePtr)
  FILE		 *fpstream;
  int            *codePtr;
{
  int            *tip, *eip, index, code;
  short           bs[IS54_FRAME_LEN], idx = 0;

  for (idx=0; idx<IS54_FRAME_LEN; idx++)
  {
    bs[idx] = (short)codePtr[idx];
  }

  /* Save and return number of saved samples */
  return(fwrite(bs, sizeof(short), IS54_FRAME_LEN, fpstream));
}
#undef IS54_FRAME_LEN
/* .......................... end of putCodesBin() ......................... */


/* ------------------------------------------------------------------------ */
#define	MAXLINE	133
long            getCodesHex(fpcode, codePtr)
  FILE 		 *fpcode;
  int            *codePtr;
{
  char           *packedLine;
  int            *unpackedCodes, *tip, *eip;

  packedLine = (char *) malloc(MAXLINE * sizeof(char));

 /* get line from packed ascii-hex code file */
  if ((packedLine = fgets(packedLine, MAXLINE, fpcode)) == NULL)
  {
    /* printf("number of frames received is %d\n", frCnt - 1); */
    return(0);
  }

 /* unpack codes, then rearrange and put into codeBuf */
  unpackedCodes = (int *) malloc(28 * sizeof(int));
  *unpackedCodes = 27;
  unpack0_(unpackedCodes, size8000, packedLine);

 /* rearrange */
  tip = unpackedCodes + 1;
  eip = tip + 10;
  *codePtr = *eip;		/* r0 */
  codePtr++;
  for (; tip < eip; tip++, codePtr++)
    *codePtr = *tip;		/* k's */
  tip++;
  for (eip = tip + 16; tip < eip; tip += 4)
  {
    *codePtr = *tip;
    *++codePtr = *(tip + 2);
    *++codePtr = *(tip + 3);
    *++codePtr = *(tip + 1);
    codePtr++;
  }

  free(packedLine);
  free(unpackedCodes);
  return(27);
}				
#undef MAXLINE
/* ...................... end of getCodesHex() ......................... */


/* ------------------------------------------------------------------------ */
#define IS54_FRAME_LEN 27
long getCodesBin(fpcode, codePtr)
  FILE 		 *fpcode;
  int            *codePtr;
{
  int count, gotten;
  short codes[IS54_FRAME_LEN];
  
  /* Read samples from a 16-bit-samples' file */
  gotten = fread(codes, sizeof(short), IS54_FRAME_LEN, fpcode);
  
  /* Convert to local representation */
  for (count=0;count<gotten;count++)
    codePtr[count] = (int)codes[count];
    
  /* Return number of gotten samples */
  return(gotten);
}
#undef IS54_FRAME_LEN
/* ...................... end of getCodesHex() ......................... */


/* ------------------------------------------------------------------------ */
void            putCodesLog()
{
  int            *tip, *eip, index, code;

 /* print out codes for one frame. */
  tip = codeBuf;
  fprintf(fplog, "%d    ", frCnt);
  fprintf(fplog, "%d    ", *tip);	/* r0 */
  tip++;
  for (eip = tip + NP; tip < eip; tip++)	/* k's */
    fprintf(fplog, "%d ", *tip);
  fprintf(fplog, "\n");
  for (index = 0; index < N_SUB; index++)
  {
    fprintf(fplog, "    ILAG = %3d", *tip);	/* lag. */
    tip++;
    code = *tip;
    tip++;
    fprintf(fplog, "     CODE1 = %3d", code);	/* code1. */
    code = *tip;
    tip++;
    fprintf(fplog, "     CODE2 = %3d", code);	/* code2. */
    fprintf(fplog, "     IGSP0 = %3d\n", *tip);	/* gsp0. */
    tip++;
  }
  fprintf(fplog, "\n");
}
/* ....................... end of putCodesLog() ............................ */
