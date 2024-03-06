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

#define TUNED_FFT

/* Number of points of the FFT */
#define NFFT_MAX 8192
#define pi 3.141592654

/* This routine generate a hanning window */
void genHanning(
				int n,			/* number of coeffients of the hanning window */
				float* hanning	/* buffer containing the coefficients of the hanning window */
);

#ifndef TUNED_FFT
/* This routine computes the positive part of the spectrum, using Real Discrete Fourier Transform */
void rdft(
		  int m,		/* number of coefficients of the fourier transform */
		  float *x1,	/* input real signal */
		  float *x2,	/* output real part of the DFT */
		  float *y2		/* output imaginary part of the DFT */
);


/* This routine compute the power spectrum of the DFT of a signal */
void powSpect(
			  float* real,		/* input buffer containing the real part of the DFT */
			  float* imag,		/* input buffer containing the imaginary part of the DFT */
			  float* pws,		/* output buffer containing the power spectrum */
			  int n				/* length of the input buffers */
);

#else
void powSpect(int m,float *x1, float *x2);
#endif
