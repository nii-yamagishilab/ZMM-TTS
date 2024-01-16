#ifndef __G728FP_H__
#define __G728FP_H__
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

typedef short	Short;
typedef long	Long;
typedef	double	Float;
typedef double	Double;
typedef	short	Nshift;

#define	Q_FILTERCOEFF	14
#define	Q_SPEECH	2
#define	Q_GAINDB	9
#define	Q_GQ		13
#define	Q_GQ_DB		11
#define	Q_H		13
#define	Q_Y		11
#define	Q_Y2		5
#define	Q_Y_DB		11
#define	Q_YH		10

#define	IDIM		5		/* Signal samples in a vector */
#define	LPC		50		/* Synthesis filter order */
#define	LPCW		10		/* Perceptual weighting filter order */
#define	LPCLG		10		/* Log-gain predictor order */
#define	LPC1		(LPC + 1)
#define	LPCW1		(LPCW + 1)
#define	LPCLG1		(LPCLG + 1)
#define	LPCVEC		(LPC/IDIM)
#define	NCWD		128		/* Shape codebook size */
#define	NM		4
#define	NA		(NCWD * IDIM)
#define	NG		8
#define	GOFF		16384		/* Log-gain offset, 32. in Q9 */
#define	NGBITS		3
#define	NCWDBITS	7
#define	NFRSZ		20
#define	NONR		35		/* Non-recursive samples for synthesis*/
#define	NONRW		30		/* Non-recursive samples for weight */
#define	NONRG		20		/* Non-recursive samples for log gain */
#define	NUPDATE		4		/* Predictor update period */
#define	NSBSZ		(NONR + NFRSZ + LPC)
#define	NSBWSZ		(LPCW + NONRW + NFRSZ)
#define	NSBGSZ		(LPCLG + NONRG + NUPDATE)

#define	UPDATEG		0
#define	UPDATEW		1
#define	UPDATEKP	2
#define	UPDATELPC	3

/* Postfilter  constants */
#define	NPWSZ		100	/* Pitch analysis window size */
#define NDEC		4	/* Decimation factor */
#define	KPMAX		140	/* Maximum allowable pitch period */
#define	KPMIN		20	/* Minimum allowable pitch period */
#define	KPDELTA		6	/* Maximum deviation from previous pitch */
#define	PITCHSZ		(NPWSZ + KPMAX)
#define	PITCHSZD	((NPWSZ + KPMAX) / NDEC)
#define LPCPF		10	/* Short-term post filter order */
#define LPCPF1		(LPCPF + 1)
#define	DECORDER	3	/* Order of low-pass filter in decimation */
#define	DECORDER1	(DECORDER + 1)
#define	ITILTF		4915	/* Spectal tilt compensation factor */

/* Frame erasure constants */
#define	FE_FRAMESZ	4	/* 10 msec */
#define	FE_ETSCALEV	26214	/* Scale factor for voiced et, .8 in Q15 */
#define	FE_AVGLEN	40	/* Samples to average g in unvoiced erasures */
#define	AFTERFEMAX	16	/* max number of 2.5msec frames to limit gain */
#define	FEGAINMAX	1024	/* maximum gain growth after FE in dB, 2. Q9 */
#define	Q_ETPAST	2

typedef struct Statelpc {
	Nshift	q;
	Float	data[IDIM];
} Statelpc;

typedef struct Gain {
	Nshift	q;
	Float	data;
} Gain;

/*
 * Encoder state info
 */
typedef struct g728fpencdata {
	int	count;		/* cycle index */
	int	info;		/* 1 if new LPC predictors are valid */
	Float	*s;		/* input speech pointer */
	Statelpc *statelpc;	/* LPC state pointer */
	Float	*gstate;	/* log-gain state pointer */
	Float	a[LPC];
	Float	atmp[LPC];
	Float	gp[LPCLG];
	Float	rexp[LPC1];
	Nshift	nlsrexp;
	Float	rexpg[LPCLG1];
	Nshift	nlsrexpg;
	Statelpc sb[NSBSZ/IDIM];
	Float	sbg[NSBGSZ];
	Float	awp[LPCW];
	Float	awz[LPCW];
	Float	h[IDIM];
	Float	rexpw[LPCW1];
	Nshift	nlsrexpw;
	Float	sbw[NSBWSZ];
	Float	wiir[LPCW];
	Float	zirwiir[LPCW];
	Float	y2[NCWD];
	Float	alpha;
	Nshift	qtmp;
	Float	ja[LPC1];
	Float	r[LPC1];
	void	(*cbsearch)(Statelpc*,Float*,Float*,int*,int*);
	Float	(*lgupdate)(Float,int,int);
	void	(*scalecv)(int,int,Gain*,Statelpc*);
} G728FpEncData;

/*
 * Decoder state info
 */
typedef struct g728fpdecdata {
	int	count;		/* cycle index */
	int	info;		/* 1 if new LPC predictors are valid */
	Statelpc *statelpc;	/* LPC state pointer */
	Float	*gstate;	/* log-gain state pointer */
	Float	a[LPC];
	Float	atmp[LPC];
	Float	gp[LPCLG];
	Float	rexp[LPC1];
	Nshift	nlsrexp;
	Float	rexpg[LPCLG1];
	Nshift	nlsrexpg;
	Statelpc sb[NSBSZ/IDIM];
	Float	sbg[NSBGSZ];
	Float	alpha;
	Nshift	qtmp;
	Float	ja[LPC1];
	Float	r[LPC1];
	/* post-filter */
	int	postf;		/* post-filter enabled - on by default */
	int	kp;
	Float	*resid;		/* Residual signal buffer pointer */
	Float	gl;
	Float	glb;
	Float	tiltz;
	Float	az[LPCPF];
	Float	ap[LPCPF];
	Float	apf[LPCPF];
	Float	stpffir[LPCPF];
	Float	stpfiir[LPCPF];
	Float	stlpci[LPCPF];
	Float	pfmem[PITCHSZ];	/* Buffer for unfiltered decoded speech */
	Float	scalefil;
	Float	lpfiir[DECORDER];
	Float	pitchd[PITCHSZ];	/* Lpc residual buffer */
	Float	dec[PITCHSZD];
	/* frame erasure */
	int	eraseframe;	/* input signal that frame is erased */
	int	adcount;	/* adaptation cycle (2.5 ms) counter */
	int	feframesz;	/* Number of 2.5 ms frames in a FE frame */
	int	feframevectors;	/* vectors in a FE */
	int	feframesamples;	/* samples in a FE */
	int	ferased;	/* current frame is erased */
	int	fecount;	/* contiguously erased 2.5 msec frame cnt */
	int	ofecount;	/* Old frame erasue count, for limiting gain */
	int	voiced;		/* frame is voiced */
	int	fdelay;		/* pitch for erased frame */
	Float	tap;		/* optimal single-tap pitch predictor coeff */
	Float	etpast[KPMAX];	/* FE storage for old excitation, Q2 */
	Float	fscale;		/* scale factor for erased frame */
	Nshift	fscaleq;
	Float	fscaleavg;	/* average scale factor for unvoiced frame */
	Nshift	fscaleavgq;
	Float	ogaindb;	/* old gain, for limiting gain growth */
	Float	(*lgupdate)(Float,int,int);
	void	(*scalecv)(int,int,Gain*,Statelpc*);
} G728FpDecData;

/*
 * public inteface
 */
extern void g728fp_encinit(G728FpEncData *d);
extern void g728fp_encode(Short *ix, Short *in, int sz, G728FpEncData *d);
extern void g728fp_decinit(G728FpDecData *d);
extern void g728fp_decode(Short *out, Short *ix, int sz, G728FpDecData *d);
extern void g728fp_eraseframe(Short *out, int sz, G728FpDecData *d);
extern void g728fp_setpostf(int on, G728FpDecData *d);
extern void g728fp_setfesize(int sz, G728FpDecData *d);

/*
 * private interface
 * These functions should not be called directly by the user.
 */
int g728fp_autocor(int,int,int,Nshift,Float*,Nshift,Float*,Nshift*,Float*);
int g728fp_block36(Float*,Float*,Nshift*,Float*);
int g728fp_block43(Float*,Float*,Nshift*,Float*);
int g728fp_block49(Statelpc*,Float*,Nshift*,Float*);
void g728fp_cbsearch(Statelpc*,Float*,Float*,int*,int*);
void g728fp_divide(Long,Nshift,Long,Nshift,Long*,Nshift*);
void g728fp_lexp10(Long,Long*,Nshift*);
void g728fp_impres(Float*,Float*,Float*,Float*);
void g728fp_lgpred(Float*,Float*,Gain*,Float*,int,Float);
Float g728fp_lgupdate(Float,int,int);
void g728fp_memsyn(Statelpc*,Float*,Statelpc*,Float*,Float*,Float*);
void g728fp_normsq(Float*,Float*);
void g728fp_normtarv(Gain*,Statelpc*);
int g728fp_durbin(Float*,int,int,Float*,Float*,Float*,Nshift*);
void g728fp_scalecv(int,int,Gain*,Statelpc*);
int g728fp_weight(Float*,Float*,int,Float*,Nshift);
void g728fp_wfilter(Float*,Float*,Float*,Float*);
void g728fp_zirlpcf(Float*,Statelpc*);
void g728fp_zirwf(Float*,Float*,Statelpc*,Float*);
void g728fp_copyf(Float*,Float*,int);
void g728fp_rcopyf(Float*,Float*,int);
void g728fp_zerof(Float*,int);
Nshift g728fp_normalize(Long*,int);
Nshift g728fp_vnormalizeshiftsf(Float*,int,int);
Nshift g728fp_vnormalizef(Float*,Float*,int,int);
extern void g728fp_vrshiftf(Float*,Float*,int,int);
extern void g728fp_vlshiftf(Float*,Float*,int,int);
extern Float g728fp_vdotpf(Float*,Float*,int);
extern Float g728fp_vrdotpf(Float*,Float*,int);
int g728fp_decp(int,Float*,Float*,Float*);
void g728fp_lpcinv(Float*,Float*,Float*,Float*);
void g728fp_ltpfcoef(int,Float*,Float*,Float*,Float*);
void g728fp_postf(int,Float,Float,Float*,Float*,Float,Float*,Float*,Float*,
	Float*,Float*,Float*);
extern	void g728fp_erasedsetup(int kp, Float tap, int msec10,G728FpDecData *d);
extern	void g728fp_erasedex(Statelpc*,Float*, G728FpDecData *d);
extern	Short g728fp_log1010(Gain*);
extern	int g728fp_mrand(void);

void WINAPIV error(char*, ...);

#define abs(x) ((x) > 0 ? (x) : -(x))

extern	Float g728fp_y[];	/* shape code book */
extern	Float g728fp_iw[], g728fp_iwg[];

#define	MAX32	2147483647.	/* Maximum value in 32 bits */
#define	MIN32	-2147483648.	/* minimum value in 32 bits */
#define	MAX28	268435455.	/* Maximum value in 28 bits */
#define	MIN28	-268435456.	/* Minimum value in 28 bits */
#define	MAX16	32767		/* Maximum value in 26 bits */
#define	MIN16	-32768		/* Minimum value in 26 bits */
#define	MASK15	0x8000

#define	SAT32(x) { if (x > MAX32) x = MAX32; else if (x < MIN32) x = MIN32; }
#define	SAT16(x) { if (x > MAX16) x = MAX16; else if (x < MIN16) x = MIN16; }
#define ROUND(a,b)	b = a >> 16; if ((a & MASK15) && b != MAX16) b++

#ifdef __cplusplus
}
#endif

#endif	/* __G728FP_H__ */
