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

#include "g728fp.h"

#define	KPMAXD		(KPMAX / NDEC)
#define KPMIND		(KPMIN / NDEC)
#define NPWSZD		(NPWSZ / NDEC)
#define NFRSZD		(NFRSZ / NDEC)
#define NDECM1		(NDEC - 1)

#define	ITAPTH	26214

/* Find maximum autocorrelation lag in range from lo to hi */
static int maxautocorr(int lo, int hi, int sz, Float *f, Long *maxl)
{
	int	i, kp;
	Float	a0, a1;

	a1 = MIN32;
	for (i = lo; i <= hi; i++) {
		a0 = g728fp_vdotpf(f, f + i, sz);
		if (a0 > MAX32 || a0 < MIN32)
			error("decp: correlation overflow");
		if (a0 > a1) {
			a1 = a0;
			kp = i;
		}
	}
	*maxl = (Long)a1;
	return kp;
}

/* decimation pitch detector. Block 82 */
int g728fp_decp(		/* returns pitch period of current frame */
	int	kpold,	/* pitch period of previous frame */
	Float	*pitchd,
	Float	*dec,
	Float	*lpfiir)
{
	Long aa0, aa1, sum, tmp, cormax, cmax;
	int i, j;
	int kp, kpdec, kptmp;
	Float	t1, t2;
	Nshift nls;

	static Float al[DECORDER] = { -19172,16481,-5031 };
	static Float bl[DECORDER1] = { 18721,-3668,-3668,18721 };

	/* lowpass filtering and 4:1 decimation */
	for (i = NFRSZ - 1; i >= 0; i--) {
		/*
		 * do FIR part first or the IIR part will amplify the LPC
		 * residual and cause overflows.
		 */
		t1 = g728fp_vdotpf(&pitchd[i], bl, DECORDER1);
		t1 = ((Long)t1 >> 6);
		t1 -= g728fp_vdotpf(lpfiir, al, DECORDER);
		lpfiir[2] = lpfiir[1];
		lpfiir[1] = lpfiir[0];
		t1 *= 8.;
		SAT32(t1);
		aa0 = (Long)t1;
		ROUND(aa0, aa1);
		lpfiir[0] = aa1;
		if ((i & NDECM1) == 0)			/* 4:1 decimation */
			dec[i>>2] = lpfiir[0];
	}

	/* find time lag of max correlation of decimated lpc residual */
	kpdec = maxautocorr(KPMIND, KPMAXD, NPWSZD, dec, &aa1);

	/* find time lag of max correlation in undecimated domain */
	if ((i = NDEC * kpdec - NDECM1) < KPMIN)
		i = KPMIN;
	if ((j = NDEC * kpdec + NDECM1) > KPMAX)
		j = KPMAX;
	kp = maxautocorr(i, j, NPWSZ, pitchd, &cormax);

	/* search neighborhood of previous pitch for max correlation */
	if ((i = kpold - KPDELTA) < KPMIN)
		i = KPMIN;
	if ((j = kpold + KPDELTA) >= kp)
		goto out;	/*  it can't be a multiple pitch */
	kptmp = maxautocorr(i, j, NPWSZ, pitchd, &cmax);

	/* compute energy at kp and kptmp delay */
	t1 = g728fp_vdotpf(&pitchd[kptmp], &pitchd[kptmp], NPWSZ);
	t2 = g728fp_vdotpf(&pitchd[kp], &pitchd[kp], NPWSZ);
	SAT32(t1);
	SAT32(t2);
	aa0 = (Long)t2;
	aa1 = (Long)t1;

	/* clip tap weights */
	if (cormax > aa0)
		cormax = aa0;
	else if (cormax < 0)
		cormax = 0;
	if (cmax > aa1)
		cmax = aa1;
	else if (cmax < 0)
		cmax = 0;

	/*TEST TAP WEIGHT THRESHOLDS TO DETERMINE WHETHER kptmp SHOULD BE USED*/
	if(aa0 > aa1) {
		nls = g728fp_normalize(&aa0, 32);
		aa1 <<= nls;
	} else {
		nls = g728fp_normalize(&aa1, 32);
		aa0 <<= nls;
	}
	sum = aa0 >> 16;
	tmp = aa1 >> 16;
 	cormax <<= nls;
 	cormax >>= 16;
 	cmax <<= nls;
 	cmax >>= 16;
	aa1 = cormax * tmp;
	aa1 >>= 16;
	aa1 = aa1 * ITAPTH;
	aa0 = cmax * sum;
	if (aa0 > aa1)
		kp = kptmp;
out:	g728fp_rcopyf(dec, &dec[NFRSZD], PITCHSZD - NFRSZD);
	g728fp_rcopyf(pitchd, &pitchd[NFRSZ], PITCHSZ - NFRSZ);
	return kp;
}
