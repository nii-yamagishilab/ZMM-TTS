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

/* calculate long-term postfilter coefficients. Blocks 83 and 84 */
void g728fp_ltpfcoef(
	int	kp,	/* pitch period */
	Float	*gl,	/* long-term postfilter scaling factor */
	Float	*glb,	/* gl*b, where b is long-term postfilter coeff. */
	Float	*rtap,	/* optimal single-tap pitch predictor */
	Float	*pfmem)
{
	int	i;
	Long	aa0, aa1, ptap, b;
	Long	gll;
	Float	a0, a1, t0;
	Float	*f0, *f1;
	Long	den, num;
	Nshift	nlsnum, nlsden, nls, ir;
static	Short	ppfth = 9830, ppfzcf = 9830;

	/* CALCULATE NUMERATOR AND DENOMINATOR TERMS FOR PITCH PREDICTOR TAP */
	a0 = a1 = 0.;
	f0 = &pfmem[kp];
	f1 = pfmem;
	for (i = NPWSZ; i; i--) {
		t0 = *f0++;
		a0 += t0 * t0;
		a1 += t0 * *f1++;
	}
	if (a0 > MAX32 || a0 < MIN32)
		error("g728fp_ltpfcoef: a0 overflow");
	if (a1 >= MAX32 || a1 < MIN32)
		error("g728fp_ltpfcoef: aq overflow");
	aa0 = (Long)a0;
	aa1 = (Long)a1;

	/* CALCULATE PITCH PREDICTOR TAP AND CONVERT TO Q14 */
	if (aa0 <= 0 || aa1 <= 0)
		ptap = 0;
	else if (aa1 >= aa0)
		ptap = 16384;
	else {
		nlsnum = g728fp_normalize(&aa1, 32);
		nlsden = g728fp_normalize(&aa0, 32);
		ROUND(aa1, num);
		ROUND(aa0, den);
		g728fp_divide(num, nlsnum, den, nlsden, &ptap, &nls);
		ir = nls - 14;
		if (ir > 30)
			ptap = 0;
		else
			ptap >>= ir;
	}
	*rtap = ptap;
	/*
	 * CALCULATE LONG-TERM POSTFILTER COEFFICIENT; ppfzcf=0.15=9830 IN Q16
	 * b IS IN Q14.  AFTER THE DIVIDE, nls OF gl IS EITHER 15 OR 14 BECAUSE
	 * 0 <= b <= 0.15, SO 0.87 <= gl = 1/(1+b) <= 1.
	 * SO nls-14 IS NON-NEGATIVE THE FINAL gl RETURNED IS IN Q14.
	 * glb is Q16.
	 */
	if (ptap < ppfth)
		ptap = 0;
	aa0 = ppfzcf * ptap;
	b = aa0 >> 14;
	aa0 >>= 16;
	den = aa0 + 16384;
	g728fp_divide(16384, 14, den, 14, &gll, &nls);
	aa0 = gll * b;
	*glb = aa0 >> nls;
	*gl = gll >> (nls - 14);
}
