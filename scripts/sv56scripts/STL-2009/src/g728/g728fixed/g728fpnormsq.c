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

#define RSHFT1	(Q_H + Q_Y - Q_YH)
#define RSHFT2	(Q_YH + Q_YH - Q_Y2)

/*
 * convolve each codevector with the impulse response and compute
 * the norm square. || H*y || **2 
 */
void g728fp_normsq(
	Float *h,	/* the impulse response sequence of W(z)/A(z) */
	Float *y2	/* norm square (energy) of H*y */
	)
{
	int	i, j;
	Float	*y;
	Float	d, s;

	y = g728fp_y;
	for (i = 0; i < NCWD; i++) {
		s = 0.;
		for (j = 0; j < IDIM; j++) {
			d = g728fp_vrdotpf(y++, &h[j], IDIM - j);
			if (abs(d) > 2147483647.)
				error("g728fp_normsq: t guard bits");
			d = (Float)((Long)d >> RSHFT1);
			if (abs(d) > 32767.)
				error("g728fp_normsq: t overflow");
			s += d * d;
		}
		if (abs(s) > 2147483647.)
			error("g728fp_normsq: y2 guard bits");
		s = (Float)((Long)s >> RSHFT2);
		if (abs(s) > 32767.)
			error("g728fp_normsq: y2 overflow");
		y2[i] = s;
	}
}
