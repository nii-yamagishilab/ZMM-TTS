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

int				/* returns 0 if ill-conditioned, else 1 */
g728fp_autocor(
	int	lpc,		/* filter order */
	int	nfrsz,		/* # of new samples (frame size) */
	int	nonr,		/* number of non-recursive samples */
	Nshift	iatt,		/* right shift factors for attenuation
				 *  2 for 50th-order LPC and log-gain predictor
				 *  1 for weighting filter */
	Float	*ws,		/* windowed signal (scaled to 2 bits headroom)*/
	Nshift	nlsws,		/* Q of ws */
	Float	*rexp,		/* recursive component of autocorrelation */
	Nshift	*nlsrexp,	/* Q of rexp */
	Float	*r)		/* output autocorrelation coefficients */
{
	Nshift nlsa0, nls;
	Nshift ir0, ir1, ir2, ir3;
	Long	a0, a1;
	int	i;
	Float	d, fac;
	Float	refac, rrfac;
	Float	min32, max32;
	Float	tre;		/* Temp recursive dot product */
	Float	tnr;		/* Temp non-recursive dot product */
	Long	max16, mask15;

	mask15 = MASK15;
	max16 = MAX16;
	max32 =  MAX32;
	min32 = MIN32;
	nlsa0 = nlsws << 1;
	fac = (1 << 16) - (1 << (16 - iatt));
	ir1 = ir0 = 1;
	if (*nlsrexp > nlsa0) {		/* new bigger, right shift rexp */
		ir1 += *nlsrexp - nlsa0;
		*nlsrexp = nlsa0;
	} else if (*nlsrexp < nlsa0)	/* right shift new contribution */
		ir0 += nlsa0 - *nlsrexp;
	a0 = ((Long)(rexp[0] * fac)) >> ir1;
	d = g728fp_vdotpf(&ws[nonr], &ws[nonr], nfrsz);
	a0 += (Long)d >> ir0;
	nls = g728fp_normalize(&a0, 32);
	LROUND(a0, a1);
	rexp[0] = a1;
	if (nls == 31)
		refac = MAX32 + 1.;
	else
		refac = 1 << nls;
	*nlsrexp += nls - 1;

	ir3 = ir2 = 1;
	if(*nlsrexp > nlsa0) /* non-recursive part bigger, right shift rexp */
		ir3 += *nlsrexp - nlsa0;
	else if (*nlsrexp < nlsa0)	/* right shift non-recursive part */
		ir2 += nlsa0 - *nlsrexp;
	a0 = (a1 << 16) >> ir3;
	d = g728fp_vdotpf(ws, ws, nonr);
	a0 += (Long)d >> ir2;
	a0 += a0 >> 8;
	nls = g728fp_normalize(&a0, 32);
	LROUND(a0, a1);
	r[0] = a1;
	if (nls == 31)
		rrfac = MAX32 + 1.;
	else
		rrfac = 1 << nls;

	for (i = 1; i <= lpc; i++) {
		tre = g728fp_vdotpf(&ws[nonr], &ws[nonr+i], nfrsz);
		tnr = g728fp_vdotpf(ws, &ws[i], nonr);
		/* recursive part */
		a0 = ((Long)(rexp[i] * fac)) >> ir1;
		a0 += (Long)tre >> ir0;
		d = a0 * refac;
		if (d > max32)
			d = max32;
		else if (d < min32)
			d = min32;
		a0 = (Long)d;
		LROUND(a0, a1);
		rexp[i] = a1;
		/* non-recursive part */
		a0 = ((a1 << 16) >> ir3) + ((Long)tnr >> ir2);
		d = a0 * rrfac;
		if (d > max32)
			d = max32;
		else if (d < min32)
			d = min32;
		a0 = (Long)d;
		LROUND(a0, a1);
		r[i] = a1;
	}
	/* check for ill-conditioning (r[norder] == 0) */
	return a0 != 0;
}
