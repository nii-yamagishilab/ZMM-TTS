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

#define LROUND(a,b)	b = a >> 16; if ((a & mask15) && b != max16) b++

/*
 * Durbin's recursion
 * convert autocorrelation coefficients to predictor coeffecients.
 * the residual energy is returned so higher order recursions may be
 * continued in later calls.
 */
int g728fp_durbin(		/* 1 if normal, 0 if ill-conditioned */
	Float	*r,	/* autocorrelation coefficients */
	int	i,	/* order of filter already done */
	int	order,	/* order of filter */
	Float	*a,	/* filter coefficients of this frame */
	Float	*rc1,	/* first reflection coefficient */
	Float	*alphaf,	/* residual energy */
	Nshift	*iaq)	/* Q of a, either 13, 14, or 15 */
{
	int	j, n;
	Long	num, a0h;
	Long	a0, a1, a2;
	Long	alpha;
	Nshift	nrs;
	Float	da0, da1;
	Float	rc;
	Float	multfac;
	Long	max16, mask15;
	Float	min32, max32, shift16;
	Float	*f, *g;

	mask15 = MASK15;
	max16 = MAX16;
	max32 =  MAX32;
	min32 = MIN32;
	shift16 = 65536.;
	if (i == 0) {
		if (r[0] <= 0)		/* test if ill-conditioned */
			return 0;
		nrs = 0;		/* calculate the 1st order predictor */
		multfac = 2.;
		a2 = 0;
		num = (Long)r[1];
		if (num < 0)
			num = -num;
		alpha = (Long)r[0];
		for (n = 16; n; n--) {	/* a2 = pkidiv(num, alpha) << 15 */
			a2 <<= 1;
			num <<= 1;
			if (num >= alpha) {
				num -= alpha;
				a2++;
		    	}
		}
		a2 <<= 15;
		LROUND(a2, a1);
		rc = a1;
		if (r[1] > 0)
			rc = -rc;
		a[0] = *rc1 = rc;
		a0 = (Long)(r[0] * shift16 + rc * r[1] * 2.);
		LROUND(a0, a1);
		alpha = a1;
		i = 1;
	} else {
		nrs = 15 - *iaq;
		multfac = 2. * (1 << nrs);
		alpha = (Long)*alphaf;
	}
	for ( ; i < order; i++) {
		da0 = 0.;
		f = &r[1];
		g = a + i - 1;
		for (n = i; n; n--)
			da0 += *f++ * *g--;
		a0 = (Long)(*f * shift16 + da0 * multfac);
		LROUND(a0, a1);
		num = a0h = a1;	/* iround(a0) */
		if (num < 0)
			num = -num;
		if (num >= alpha)
			return 0;
		a2 = 0;
		for (n = 16; n; n--) {	/* a2 = pkidiv(num, alpha) << 15 */
			a2 <<= 1;
			num <<= 1;
			if (num >= alpha) {
				num -= alpha;
				a2++;
		    	}
		}
		a2 <<= 15;
		LROUND(a2, a1);
		rc = a1 << 1;		/* iround(a1) << 1 */
		if (a0h > 0)
			rc = -rc;
		a1 = (alpha << 16) + (Long)(rc * a0h);
		if (a1 <= 0)		/* ill conditioned ? */
			return 0;
		LROUND(a1, a0);
		alpha = a0;		/* update residual energy */
	
		/* update the predictor coeffficients */
		for (f = a, g = a + i - 1; f <= g; f++, g--) {
			da0 = *f * shift16 + rc * *g;
			da1 = *g * shift16 + rc * *f;
			if (da0 > max32 || da0 < min32 || 
			    da1 > max32 || da1 < min32) {
				if (++nrs > 2)	/* Q < 13 */
					return 0;
				multfac *= 2.;
				for (j = 0; j < i; j++)
					a[j] = (Long)a[j] >> 1;
				da0 = *f * shift16 + rc * *g;
				da1 = *g * shift16 + rc * *f;
			}
			a0 = (Long)da0;
			LROUND(a0, a1);
			*f = a1;	/* iround(da0) */
			a0 = (Long)da1;
			LROUND(a0, a1);
			*g = a1;	/* iround(da1) */
		}
		a2 >>= nrs;
		LROUND(a2, a1);
		a[i] = a1;	/* iround(a2 >> nrs) */
		if (a0h > 0)
			a[i] = -a[i];
	}
	*alphaf = alpha;
	*iaq = 15 - nrs;
	return 1;
}
