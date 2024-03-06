#include "g728.h"
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

/*
 * Postfilter routines for floating point LD-CELP, G728 coder
 */

#define	KPMAXD		(G728_KPMAX / NDEC)
#define KPMIND		(G728_KPMIN / NDEC)
#define NPWSZD		(NPWSZ / NDEC)
#define NFRSZD		(NFRSZ / NDEC)
#define NDECM1		(NDEC - 1)

static Float al[DECORDER] =	/* Decimation low-pass filter IIR coeffs. */
	{ (Float)-2.34036589, (Float)2.01190019, (Float)-.614109218 };
static Float bl[DECORDER1] =	/* Decimation low-pass filter FIR coeffs. */
	{ (Float).0357081667, (Float)-.0069956244,
	  (Float)-.0069956244, (Float).0357081667 };

/* post filter with both long delay and short delay sections */
void g728_postf(
	int	kp,	/* pitch period */
	Float	gl,	/* long-term postfilter gain scaling factor */
	Float	glb,	/* gl*b, where b is the long-term pf coeff. */
	Float	*az,
	Float	*ap,
	Float	tiltz,	/* spectral tilt compensation filter coefficient */
	Float	*st,	/* unfiltered decoded speech */
	Float	*stpf,	/* postfiltered decoded speech */
	Float	*pfmem,
	Float	*stpffir,
	Float	*stpfiir,
	Float	*scalefil0)
{
	int	k;
	Float	efil, eunf;
	Float	scale;
	Float	scalefil;

	g728_rcpy(pfmem, PITCHSZ - IDIM, &pfmem[IDIM]);
	g728_cpy(st, IDIM, pfmem);
	for (k = 0; k < IDIM; ++k)	/* long-term (pitch) postfilter */
		stpf[k] = gl * st[k] + glb * pfmem[kp + k];
	/* short-term (lpc) postfilter  */
	g728_firblk(az, stpf, stpf, stpffir, G728_LPCPF, IDIM);
	g728_iirblkn(ap, stpf, stpfiir, G728_LPCPF, IDIM);
	/* 1st-order filter to compensate for spectral tilt */
	for (k = 0; k < IDIM; k++)
		stpf[k] = stpfiir[k] + tiltz * stpfiir[k + 1];
	/* sum magnitudes of unfiltered and postfiltered speech */
	eunf = g728_vsumabs(st, IDIM);
	efil = g728_vsumabs(stpf, IDIM);
	if (efil <= (Float)1.)			/* scaling factor */
		scale = (Float)1.;
	else
		scale = eunf / efil;
	scale *= (Float)AGCFAC1;
	scalefil = *scalefil0;
	/* low-pass filter the scale factor and scale the postfilter output */
	for (k = IDIM - 1; k >= 0; k--) {
		scalefil = (Float)AGCFAC * scalefil + scale;
		stpf[k] *= scalefil;
		g728_clamp(&stpf[k], (Float)32767., (Float)-32767.);
	}
	*scalefil0 = scalefil;
}

/* find the optimal first-order pitch predictor tap  */
void g728_ltpfcoef(
	int	kp,	/* detected pitch period */
	Float	*gl,	/* long-term postfilter gain scaling factor */
	Float	*glb,	/* gl*b, where b is the long-term pf coeff. */
	Float	*rtap,	/* optimal single-tap pitch predictor */
	Float	*pfmem,
	Float	pzcf)
{
	Float tmp, sum;
	Float tap;	/* optimal single-tap pitch predictor coefficient */

	sum = g728_vdotp(&pfmem[kp], &pfmem[kp], NPWSZ);
	tmp = g728_vdotp(pfmem, &pfmem[kp], NPWSZ);
	if (sum != 0.) {
		tap = tmp / sum;
		if (tap > 1.)
			tap = (Float)1.;
		else if (tap < 0.)
			tap = (Float)0.;
	} else
		tap = (Float)0.;
	*rtap = tap;
	if (tap < PPFTH)
		tap = (Float)0.;
	tap *= pzcf;
	/* long-term postfilter coefficients */
	*gl = (Float)1. / (tap + (Float)1.);
	*glb = *gl * tap;
}

/* Find maximum autocorrelation lag in range from lo to hi */
static Float maxautocorr(int lo, int hi, int sz, Float *sig, int *kp)
{
	int i;
	Float sum, max;

	max = (Float)-1e20;
	for (i = lo; i <= hi; i++)
		if ((sum = g728_vdotp(sig, sig + i, sz)) > max) {
			max = sum;
			*kp = i;
		}
	return max;
}

/*  decimation pitch detector */
void g728_decp(
	int	*kp, /* pitch period of previous(in) and current(out) frame */
	Float	*dec,
	Float	*pitchd,
	Float	*lpfiir)
{
	int i, j;
	int kmax, kpnew, kptmp;
	Float cmax, cormax, tmp, sum, tap, tap1;
	Float sout[NFRSZ];

	/* lowpass filter */
	g728_firblknz(bl, pitchd, sout, DECORDER, NFRSZ);
	g728_iirblk(al, sout, sout, lpfiir, DECORDER, NFRSZ);
	for (i = 0; i < NFRSZ; i += NDEC)		/* 4:1 decimation */
		dec[i>>2] = sout[i];

	/* find time lag of max correlation of decimated lpc residual */
	maxautocorr(KPMIND, KPMAXD, NPWSZD, dec, &kmax);

	/* find time lag of max correlation in undecimated domain */
	if ((i = NDEC * kmax - NDECM1) < G728_KPMIN)
		i = G728_KPMIN;
	if ((j = NDEC * kmax + NDECM1) > G728_KPMAX)
		j = G728_KPMAX;
	cormax = maxautocorr(i, j, NPWSZ, pitchd, &kpnew);
	
	/* search neighborhood of previous pitch for max correlation */
	if ((i = *kp - KPDELTA) < G728_KPMIN)
		i = G728_KPMIN;
	if ((j = *kp + KPDELTA) >= kpnew)
		goto out;	/*  it can't be a multiple pitch */
	cmax = maxautocorr(i, j, NPWSZ, pitchd, &kptmp);

	/* optimal pitch predictor taps for the two candidate lags */
	tmp = g728_vdotp(&pitchd[kptmp], &pitchd[kptmp], NPWSZ);
	sum = g728_vdotp(&pitchd[kpnew], &pitchd[kpnew], NPWSZ);
	tap = (sum == 0.) ? (Float)0. : cormax / sum;
	tap1 = (tmp == 0.) ? (Float)0. : cmax / tmp;
	g728_clamp(&tap, (Float)1., (Float)0.);
	g728_clamp(&tap1, (Float)1., (Float)0.);
	if (tap1 > TAPTH * tap)		/* output fundamental pitch */
		kpnew = kptmp;
out:	*kp = kpnew;
	g728_rcpy(dec, PITCHSZD - NFRSZD, &dec[NFRSZD]);
	g728_rcpy(pitchd, PITCHSZ - NFRSZ, &pitchd[NFRSZ]);
}
