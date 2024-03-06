/*
 * (c) Copyright 1990-2007 AT&T Intellectual Property.
 * All rights reserved.
 *
 * NOTICE:
 * This software source code ("Software"), and all of its derivations,
 * is subject to the "ITU-T General Public License". It can be viewed
 * in the distribution disk, or in the ITU-T Recommendation G.191 on
 * "SOFTWARE TOOLS FOR SPEECH AND AUDIO CODING STANDARDS".
 *
 * The availability of this "Software" does not provide any license by
 * implication, estoppel, or otherwise under any patent rights owned or
 * controlled by AT&T Inc. or others covering any use of the Software
 * herein.
 */
#ifndef __G728_H__
#define __G728_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER	/* Visual C++ */
#ifndef WINAPIV
#define WINAPIV __cdecl
#endif
#else		/* Unix */
#ifndef WINAPIV
#define WINAPIV
#endif
#endif

/*
 * Declaration for the floating point CCITT G.728 16 kb/s low-delay CELP coder.
 * The coder can be compiled with the following flags.
 *
 * -DUSEDOUBLES
 * Compiles the coder so it uses double-precision floating-point arithmetic.
 * Withough this option, a single-precision floating-point version is created.
 * The single-precision version runs faster, but use the double-precision
 * version for an exact match with the test vectors.
 */
#ifdef USEDOUBLES
typedef double  Float;
#endif
#ifdef USEFLOATS
typedef float   Float;
#endif
typedef short   Short;

/* G.728 constants */
#define	IDIM		5
#define	LPC		50
#define	LPCW		10
#define	LPCLG		10
#define	LPC1		(LPC + 1)
#define	LPCW1		(LPCW + 1)
#define	LPCLG1		(LPCLG + 1)
#define	NCWD		128
#define	NM		4
#define	NG		8
#define	NGBITS		3
#define	GOFF		32.
#define	NFRSZ		20
#define	NONR		35		/* Non-recursive samples for synthesis*/
#define	NONRW		30		/* Non-recursive samples for weight */
#define	NONRG		20		/* Non-recursive samples for log gain */
#define	NUPDATE		4		/* Predictor update period */
#define	WNCF		(257./256.)
#define	NSBSZ		(LPC + NONR + NFRSZ)
#define	NSBWSZ		(LPCW + NONRW + NFRSZ)
#define	NSBGSZ		(LPCLG + NONRG + NUPDATE)
#define	ATTENL		.75
#define	ATTENW		.5
#define	ATTENG		.75
#define CLAMPLPC	4095.
#define	UPDATEG		0
#define	UPDATEW		1
#define	UPDATEKP	2
#define	UPDATELPC	3
#define	SCALEINPUT	.125
#define	SCALEOUTPUT	8.

/* G.728 Post-filter constants */
#define	NPWSZ		100	/* Pitch analysis window size */
#define NDEC		4	/* Decimation factor */
#define	G728_KPMAX	140	/* Maximum allowable pitch period */
#define	G728_KPMIN	20	/* Minimum allowable pitch period */
#define	KPDELTA		6	/* Maximum deviation from previous pitch */
#define	PITCHSZ		(NPWSZ + G728_KPMAX)
#define	PITCHSZD	((NPWSZ + G728_KPMAX) / NDEC)
#define G728_LPCPF	10	/* Short-term post filter order */
#define	DECORDER	3	/* Order of low-pass filter in decimation */
#define	DECORDER1	(DECORDER + 1)
#define	PPFTH		0.6
#define	TAPTH		0.4	/* Tap threshold for pitch replacement*/
#define	AGCFAC		0.99
#define	AGCFAC1		(1. - AGCFAC)

/* G.728 Frame erasure constants */
#define	FE_FRAMESZDEF	4	/* 10 msec by default */
#define	FE_AVGLEN	40	/* Samples to average g in unvoiced erasures */
#define	FE_ATTEN	.97	/* LPC coeff attenuation factor */
#define FE_GAINMAX	2.0	/* max increase in gain after FE */
#define	FE_AFTERMAX	16	/* max number of 2.5msec frames to limit gain */

/*
 * Tables that may be placed in ROM on DSP
 */
typedef struct G728rom {
	Float gq[NG];		/* gain quantizer levels */
	Float y[NCWD][IDIM];	/* shape code book */
	Float wnr[NSBSZ];	/* LPC hybrid window */
	Float wnrg[NSBGSZ];	/* Log-gain hybrid window */
	Float wnrw[NSBWSZ];	/* Weighting filter hybrid window - TX only */
	Float facv[LPC];	/* LPC bandwidth expansion weight table(BWEWT)*/
	Float facgpv[LPCLG];	/* Log gain BWEWT */
	Float wzcfv[LPCW];	/* Weight filter all-zero BWEWT - TX only */
	Float spfpcfv[G728_LPCPF]; /* Short term post filter all-pole BWEWT */
	Float gq_db[NG];	/* Gain in dB */
	Float gb[NG];		/* gain quantizer cell boundaries */
	Float g2[NG];		/* 2*gq */
	Float gsq[NG];		/* gq*gq */
	Float y_db[NCWD];	/* Shape gain in dB */
	Float fatten[LPC];	/* frame erasure LPC coef. attenuation */
	Float *wpcfv;		/* Weight filter all-pole BWEWT  - TX only */
	Float *spfzcfv;		/* Short term post filter all-zero BWEWT */
	Float pzcf;
	Float tiltf;
} G728Rom;

/*
 * RAM needed for the encoder
 */
typedef struct g728encdata {
	/* Common to encoder and decoder */
	int	info;		/* 1 if new LPC predictors are valid */
	int	count;		/* Cycle index */
	Float	*gstate;	/* Pointer to log-gain state */
	Float	*statelpc;	/* Pointer to LPC state */
	Float	*gq;		/* Gain quantizer table */
	Float	*gq_db;		/* Gain quantizer dB table */
	Float	a[LPC];		/* LPC predictors */
	Float	atmp[LPC];	/* New LPC predictors */
	Float	gp[LPCLG];	/* Log-gain predictors */
	Float	rexp[LPC1];	/* Recursive part of LPC autocorrelation */
	Float	rexpg[LPCLG1];	/* Recursive part of log-gain autocorrelation */
	Float	sb[NSBSZ];	/* Quantized speech buffer */
	Float	sbg[NSBGSZ];	/* Log-gain buffer */
	/* Encoder only */
	Float	*s;		/* Pointer to input speech */
	Float	awp[LPCW];	/* Weighting filter all-pole coefficients */
	Float	awz[LPCW];	/* Weighting filter all-zero coefficients */
	Float	h[IDIM];	/* Impulse response of LPC + weighting filter */
	Float	rexpw[LPCW1];	/* Recursive part of weighting autocorrelation*/
	Float	sbw[NSBWSZ];	/* Input speech buffer */
	Float	wiir[LPCW];	/* Weighting filter memory for input speech */
	Float	y2[NCWD];	/* Norm square of |H*y|^2  */
	Float	zirwiir[LPCW];	/* Weighting filter memory for zero input res */
	/* bit-rate dependent */
	void	(*search)(Float*,Float*,Float*,int*,int*);
	Float	*wpcfv;		/* Weight filter all-pole BWEWT */
} G728EncData;

/*
 * RAM needed for the decoder
 */
typedef struct g728decdata {
	/* Common to encoder and decoder */
	int	info;		/* 1 if new LPC predictors are valid */
	int	count;		/* Cycle index */
	Float	*gstate;	/* Pointer to log-gain state */
	Float	*statelpc;	/* Pointer to LPC state */
	Float	*gq;		/* Gain quantizer table */
	Float	*gq_db;		/* Gain quantizer dB table */
	Float	a[LPC];		/* LPC predictors */
	Float	atmp[LPC];	/* New LPC predictors */
	Float	gp[LPCLG];	/* Log-gain predictors */
	Float	rexp[LPC1];	/* Recursive part of LPC autocorrelation */
	Float	rexpg[LPCLG1];	/* Recursive part of log-gain autocorrelation */
	Float	sb[NSBSZ];	/* Quantized speech buffer */
	Float	sbg[NSBGSZ];	/* Log-gain buffer */
	/* Decoder only */
	Float	r[LPC1];	/* Autocorrelation coefficients */
	Float	alpha;		/* Residual energy in Durbin's recursion */
	/* Postfilter only */
	Float	*resid;		/* Residual signal buffer pointer*/
	int	postf;		/* post-filter enabled - on by default */
	int	kp;		/* Pitch period */
	Float	gl;		/* long-term postfilter gain scaling factor */
	Float	glb;		/* gl * tap */
	Float	tap;		/* optimal single-tap pitch predictor coeff */
	Float	tiltz;		/* Short-term postfilter tilt-compensation */
	Float	ap[G728_LPCPF];	/* Short-term all-pole postfilter coeffs */
	Float	az[G728_LPCPF];	/* Short-term all-zero postfilter coeffs */
	Float	apf[G728_LPCPF];	/* 10th order LPC predictors */
	Float	stpffir[G728_LPCPF];	/* Short term postfilter FIR memory */	
	Float	stpfiir[G728_LPCPF];	/* Short term postfilter IIR memory */
	Float	pfmem[PITCHSZ];	/* Buffer for unfiltered decoded speech */
	Float	scalefil;
	Float	lpfiir[DECORDER];/* Decimation low-pass filter IIR memory */
	Float	pitchd[PITCHSZ]; /* Lpc residual buffer */
	Float	dec[PITCHSZD];	 /* Decimated lpc residual buffer */
	/* Frame erasure only */
	int	eraseframe;	/* input signal that frame is erased */
	int	adcount;	/* adaptation cycle (2.5 ms) counter */
	int	feframesz;	/* Number of 2.5 ms frames in a FE frame */
	int	feframevectors;	/* vectors in a FE */
	int	feframesamples;	/* samples in a FE */
	int	ferased;	/* current frame is erased */
	int	fecount;	/* contiguously erased 2.5 msec frame cnt */
	int	ofecount;	/* Old frame erasue count, for limiting gain */
	int	voiced;		/* frame is voiced */
	int	fedelay;	/* pitch for erased frame */
	Float	fescale;	/* scale factor for erased frame */
	Float	fescaleavg;	/* average scale factor for unvoiced frame */
	Float	ogaindb;	/* old gain in dB */
	Float	etpast[G728_KPMAX]; /* frame erasure old excitation storage */
	Float	vscale;
	Float	uvscale;
	/* bit-rate dependent */
	Float *spfzcfv;		/* Short term post filter all-zero BWEWT */
	Float pzcf;
	Float tiltf;
} G728DecData;


/*
 * RAM needed for the vocoder (encoder/decoder with postfilter)
 */
typedef struct g728vocdata {
	/* Common to encoder and decoder */
	int	info;		/* 1 if new LPC predictors are valid */
	int	count;		/* Cycle index */
	Float	*gstate;	/* Pointer to log-gain state */
	Float	*statelpc;	/* Pointer to LPC state */
	Float	*gq;		/* Gain quantizer table */
	Float	*gq_db;		/* Gain quantizer dB table */
	Float	a[LPC];		/* LPC predictors */
	Float	atmp[LPC];	/* New LPC predictors */
	Float	gp[LPCLG];	/* Log-gain predictors */
	Float	rexp[LPC1];	/* Recursive part of LPC autocorrelation */
	Float	rexpg[LPCLG1];	/* Recursive part of log-gain autocorrelation */
	Float	sb[NSBSZ];	/* Quantized speech buffer */
	Float	sbg[NSBGSZ];	/* Log-gain buffer */
	/* Encoder only */
	Float	*s;		/* Pointer to input speech */
	Float	awp[LPCW];	/* Weighting filter all-pole coefficients */
	Float	awz[LPCW];	/* Weighting filter all-zero coefficients */
	Float	h[IDIM];	/* Impulse response of LPC + weighting filter */
	Float	rexpw[LPCW1];	/* Recursive part of weighting autocorrelation*/
	Float	sbw[NSBWSZ];	/* Input speech buffer */
	Float	wiir[LPCW];	/* Weighting filter memory for input speech */
	Float	y2[NCWD];	/* Norm square of |H*y|^2  */
	Float	zirwiir[LPCW];	/* Weighting filter memory for zero input res */
	/* Postfilter only */
	Float	*resid;		/* Residual signal buffer pointer*/
	int	kp;		/* Pitch period */
	Float	gl;		/* long-term postfilter gain scaling factor */
	Float	glb;		/* gl * tap */
	Float	tap;		/* optimal single-tap pitch predictor coeff */
	Float	tiltz;		/* Short-term postfilter tilt-compensation */
	Float	ap[G728_LPCPF];	/* Short-term all-pole postfilter coeffs */
	Float	az[G728_LPCPF];	/* Short-term all-zero postfilter coeffs */
	Float	apf[G728_LPCPF];	/* 10th order LPC predictors */
	Float	stpffir[G728_LPCPF];	/* Short term postfilter FIR memory */	
	Float	stpfiir[G728_LPCPF];	/* Short term postfilter IIR memory */
	Float	pfmem[PITCHSZ];	/* Buffer for unfiltered decoded speech */
	Float	scalefil;
	Float	lpfiir[DECORDER];/* Decimation low-pass filter IIR memory */
	Float	pitchd[PITCHSZ]; /* Lpc residual buffer */
	Float	dec[PITCHSZD];	 /* Decimated lpc residual buffer */
	/* bit-rate dependent */
	void	(*search)(Float*,Float*,Float*,int*,int*);
	Float	*wpcfv;		/* Weight filter all-pole BWEWT */
	Float *spfzcfv;		/* Short term post filter all-zero BWEWT */
	Float pzcf;
	Float tiltf;
} G728VocData;

/* public interface to G.728 coder */
extern void g728encinit(G728EncData*);
extern void g728decinit(G728DecData*);
extern void g728encode(Short*,Float*,int,G728EncData*);
extern void g728decode(Float*,Short*,int,G728DecData*);
extern void g728setpostf(int i, G728DecData *d);
extern void g728setfesize(int sz, G728DecData *d);
extern void g728decfe(Float*,int,G728DecData*);
extern void g728vocinit(G728VocData*);
extern void g728vocode(Float*,Short*,Float*,int,int,G728VocData*);

/* private G.728 routines */
extern void g728rominit(void);
extern void g728_cbsearch2(Float*,Float*,Float[][IDIM],Float*,Float*,Float*,
	Float*,int*,int*);
extern int g728_durbin(Float*,int,int,Float*,Float*,Float*);
extern int g728_hybridw(int,int,int,Float,Float*,Float*,Float*,Float*);
extern void g728_impres(Float*,Float*,Float*,Float*);
extern void g728_lgpred(Float*,Float*,Float*,Float*,int,Float*);
extern void g728_normsq(Float y[][IDIM],Float*,Float*);
extern void g728_synmem(Float*,Float*,Float*,Float*,Float*,Float*);
extern void g728_synmemrx(Float*,Float*,Float*);
extern void g728_wfilter(Float*,Float*,Float*,Float*);
extern void g728_zirlpcf(Float*,Float*);
extern void g728_cbsearch16(Float*,Float*,Float*,int*,int*);

/* internal G.728 post-filter routines */
extern void g728_decp(int*,Float*,Float*,Float*);
extern void g728_ltpfcoef(int,Float*,Float*,Float*,Float*,Float);
extern void g728_postf(int,Float,Float,Float*,Float*,Float,Float*,Float*,
	Float*,Float*,Float*,Float*);

/* internal G.728 frame erasure routines */
extern void g728_erasedsetup(G728DecData*);
extern void g728_erasedex(Float*,Float*,G728DecData*);

/* generic operators */
extern void g728_iirblk(Float*,Float*,Float*,Float*,int,int);
extern void g728_iirblkn(Float*,Float*,Float*,int,int);
extern void g728_firblk(Float*,Float*,Float*,Float*,int,int);
extern void g728_firblkn(Float*,Float*,Float*,int,int);
extern void g728_firblknz(Float*,Float*,Float*,int,int);
extern void g728_clamp(Float*,Float,Float);
extern void g728_cpy(Float*,int,Float*);
extern void g728_rcpy(Float*,int,Float*);
extern void g728_cpyi2r(Short*,int,Float*);
extern void g728_cpyr2i(Float*,int,Short*);
extern void g728_vmult(Float*,Float*,int,Float*);
extern void g728_vscale(Float*,Float,int,Float*);
extern void g728_vsub(Float*,Float*,int,Float*);
extern Float g728_vdotp(Float*,Float*,int);
extern Float g728_vrdotp(Float*,Float*,int);
extern void g728_zero(Float*,int);
extern Float g728_vsumabs(Float*,int);

extern G728Rom g728rom;

#ifdef __cplusplus
}
#endif

#endif	/* __G728_H__ */
