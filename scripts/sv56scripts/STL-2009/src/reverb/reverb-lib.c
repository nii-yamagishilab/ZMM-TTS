/* ..............................................................................................02/Feb/2005*/
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
	This file contains functions used for the reverberation tool.

  FUNCTIONS :
	Global (have prototype in reverb-lib.h)
		shift(...)		:		Shift coefficients of the input buffer for next block filtering
		conv(...)		:		Convolves the impulse response of a room with the input file

  HISTORY :
	02.Feb.05	v1.0	First Beta version
    10.jul.08   v1.01   Added 16 bit saturation and saturation warning

  AUTHORS :
	v1.0 Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com
	v1.01 Jonas Svedberg jonas.svedberg@ericsson.com

*/



/* this routine replaces the first N-1 samples of a buffer by the last N-1 samples */
void shift(short* buff, long N)
{
	long k;

	for(k=0; k<N-1; k++)
	{
		buff[k]=buff[k+N];
	}
}


/* this routine convolves buffIn with IR and stores the processed data into buffRvb */
/* alignFact is used to align the energy of the input file with an other file */
/* the ouput sat_warning is used to provide a warning of there is 16 bit saturation, 
  a positive value indicates position of overflow */ 
long conv(float* IR, short* buffIn, short* buffRvb, float alignFact, long N, long L)
{
	long k,j;
	float tmpRvb;
    long sat_warning;
   
    sat_warning=-1;
	for(k=0; k<L; k++) {
		tmpRvb=0;
		for(j=0;j<N; j++)
		{
			tmpRvb+=buffIn[N-1+k-j]*IR[j];
		}     
        tmpRvb=(float)(alignFact*tmpRvb + 0.5); /* +0.5 : rounding for the 'short' truncation */

        /* perform 16 bit saturation */
        if( tmpRvb < -32768.0 ){ 
          buffRvb[k] = -32768;
          sat_warning=k;
        } else{
            if( tmpRvb > 32767.0 ){ 
              buffRvb[k]=32767;
              sat_warning=k; 
            } else {
              buffRvb[k]=(short)tmpRvb;   
            }
        }
    }
    return sat_warning;
}

/* this routine convolves buffIn with IR and stores the processed data into buffRvb */
/* alignFact is used to align the energy of the input file with an other file */
void conv_old(float* IR, short* buffIn, short* buffRvb, float alignFact, long N, long L)
{
	long k,j;
	float tmpRvb;

	for(k=0; k<L; k++) {
		tmpRvb=0;
		for(j=0;j<N; j++)
		{
			tmpRvb+=buffIn[N-1+k-j]*IR[j];
		}
		buffRvb[k]=(short) (alignFact*tmpRvb + 0.5);  /* +0.5 : rounding during the 'short' truncation */
	}
}

