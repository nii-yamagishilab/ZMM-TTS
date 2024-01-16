/* ..............................................................................................31/Mar/2005*/
/*																										v1.0*/

/*=============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

  DESCRIPTION : 
	This file contains a function used for the frequency response measure tool.

  FUNCTION :
	Global (have prototype in export.h)
		exportASCII(...)	:	Exports average power spectrum vectors in an ASCII file

  HISTORY :
	31.Mar.05	v1.0	First Beta version (STL2005)
	Dec.09		v1.0.1  Header correction (STL2009)

  AUTHORS :
	Cyril Guillaume Stephane Ragot -- stephane.ragot@francetelecom.com

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* This routine exports the input buffer to an ASCII file */
void exportASCII(
				float* buff1,		/* buffer containing the power spectrum of a first file */
				float* buff2,		/* buffer containing the power spectrum of a second file */
				long   fs,			/* sampling frequency of the input files */
				int    NFFT,		/* number of coefficents of the fourier transform */
				char*  fileName		/* name of the ASCII ouput file */
				)
{
	FILE* fp;
	int i;

	fp=fopen(fileName,"wb");
	if(fp==NULL) {
		fprintf(stderr,"Error opening output ASCII file!");
		exit(-1);
	}
	fprintf(fp,"                   +------------------------------------------+\n");
	fprintf(fp,"                   |  Average Amplitude Spectra (in dB)       |\n");
	fprintf(fp,"+------------------+------------------------------------------+\n");
	fprintf(fp,"| frequencies (Hz) | Input of the codec | Output of the codec |\n");
	fprintf(fp,"+------------------+------------------------------------------+\n");
	
	for(i=0; i<NFFT/2; i++) {
	  fprintf(fp,"|      %5d       |       %3.2f       |        %3.2f       |\n", (int)(i*fs/NFFT), 10*log10(buff1[i]), 10*log10(buff2[i]));
	}
	fprintf(fp,"+------------------+------------------------------------------+\n");
	
	fclose(fp);

}
