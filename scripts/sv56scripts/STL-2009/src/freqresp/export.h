/*                                                          15.Feb.2010 v1.3 */
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
	This file contains a demonstration program of a frequency response tool, using
  the functions in fft.h

  HISTORY :
	31.Mar.05	v1.0	First Beta version
	10.Mar.08 v1.1  New option:
	                -ov   : overlap between consecutive frames for decreasing the window effect.
	   Sep.08 v1.2  Use of split radix(4,2) fft algorithm instead of DFT
                  New option:
                  -nfft : indicates the number of points used in FFT.
  15.Feb.10 v1.3  Modified maximum string length for filename, and
	                removed some macros (OVERLAP, VAR_NFFT)

  AUTHORS :
	Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com
	Pierre Berthet (v1.1) for France Telecom
	Deming Zhang (v1.2) for Huawei Technologies
  yusuke hiwasaki (v1.3) NTT
*/

/* This routine exports the input buffer to an ASCII file */
void exportASCII(
				float* buff1,		/* buffer containing the power spectrum of a first file */
				float* buff2,		/* buffer containing the power spectrum of a second file */
				long   fs,			/* sampling frequency of the input files */
				int    nNFFT,		/* number of coefficents of the fourier transform */
				char*  fileName		/* name of the ASCII ouput file */
);
