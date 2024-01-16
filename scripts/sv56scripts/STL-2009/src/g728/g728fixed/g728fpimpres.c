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

/* calculate impulse response of LPC and weighting filter */
void g728fp_impres(
	Float	*a,	/* LPC filter coeff. */
	Float	*awz,	/* weighting filter all-zero (FIR) coeff. */
	Float	*awp,	/* weighting filter all-pole (IIR) coeff. */
	Float	*h)	/* impulse response */
{
	int	i, k;
	Float	d0, d1, *tp;
	Long	a0;
	Float	tmp[IDIM];

	h += IDIM - 1;
	tp = &tmp[IDIM - 1];
	*h = *tp = 1 << Q_H;
	for (i = 1; i < IDIM ; i++) {
		d1 = d0 = 0.;
		for (k = 0; k < i; k++) {
			d0 -= a[k] * tp[k];
			d1 += awz[k] * tp[k] - awp[k] * h[k];
		}
		d1 += d0;
		a0 = (Long)d0 >> 14;
		SAT16(a0);
		*--tp = a0;
		a0 = (Long)d1 >> 14;
		SAT16(a0);
		*--h = a0;
	}
}
