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

/* LPC inverse filter. Block 81 of G728 */
void g728fp_lpcinv(
	Float	*apf,		/* 10th order LPC coefficients, Q14 */
	Float	*st,		/* Un-postfiltered decoded speech, Q2 */
	Float	*stlpci,
	Float	*resid)		/* Residual buffer pointer, Q1 */
{
	int	j, k;
	Long	l, l2;
	Float	a0;
	Float	mfac;

	mfac = 1 << 13;
	for (k = IDIM - 1; k >= 0; k--) {
		a0 = st[k] * mfac;
		for (j = LPCPF - 1; j > 0; j--) {
			a0 += stlpci[j] * apf[j];
			stlpci[j] = stlpci[j - 1];
		}
		a0 += stlpci[0] * apf[0];
 		a0 *= 4.;
		stlpci[0] = st[k];
		SAT32(a0);
		l = (Long)a0;
		ROUND(l, l2);
		resid[k] = l2;
	}
}
