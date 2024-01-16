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
/**/
/*	weightedSnr.c -- Computes weighted segmental and total SNR.*/
/**/
/*-------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/
#include "vparams.h"

#include <math.h>
/*#include "stdlib.h"*/

/*	static externals*/
/**/
static FTYPE    sEngTotal, weEngTotal, eEngTotal, sumwSegSnr, sumSegSnr;
static int      numSegs;


/*----------------------------------------*/
/*	snr and weighted prediction gains*/
/**/
void            runningSnr(speech, syn, wSpeech, wsyn)
  FTYPE          *speech;
  FTYPE          *syn;
  FTYPE          *wSpeech;
  FTYPE          *wsyn;
{
  FTYPE           sEng, weEng, eEng, temp, *tfp1, *tfp2, *tfp3, *tfp4,
                 *efp;

/*	update subframe energies*/
/**/
  sEng = 0.0;
  weEng = 0.0;
  eEng = 0.0;
  tfp1 = speech;
  tfp2 = wSpeech;
  tfp3 = wsyn;
  tfp4 = syn;
  for (efp = tfp1 + S_LEN; tfp1 < efp; tfp1++, tfp2++, tfp3++, tfp4++)
  {
    sEng += *tfp1 * *tfp1;
    temp = *tfp1 - *tfp4;
    eEng += temp * temp;
    temp = *tfp2 - *tfp3;
    weEng += temp * temp;
  }

  sEngTotal += sEng;
  weEngTotal += weEng;
  eEngTotal += eEng;
  if (sEng > 0.0)
  {
    sumwSegSnr += 10.0 * log10(sEng / weEng);
    sumSegSnr += 10.0 * log10(sEng / eEng);
    numSegs++;
  }
}				/* end of runningSnr */


/*------------------------------------------------*/
/*	print final results*/
/**/
void            printSnr(fplog)
FILE *fplog;
{
  fprintf(fplog, "\nNUMBER OF FRAMES IN SNR COMPUTATION: %d\n", numSegs);
  fprintf(fplog, "\nAVG WEIGHTED SEG SNR          -> %8.5f\n",
	  sumwSegSnr / numSegs);
  fprintf(fplog, "TOTAL WEIGHTED SNR            -> %8.5f\n",
	  10.0 * log10(sEngTotal / weEngTotal));
  fprintf(fplog, "AVG SEG SNR                   -> %8.5f\n", 
  	  sumSegSnr / numSegs);
  fprintf(fplog, "TOTAL SNR                     -> %8.5f\n",
	  10.0 * log10(sEngTotal / eEngTotal));
}
