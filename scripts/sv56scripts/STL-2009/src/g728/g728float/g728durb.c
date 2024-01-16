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
 *  durbin's recursion - compute filter coefficients from autocorrelation coeff.
 *	routine is structured so the recursion can be started in one call and
 *	continued in the next call. Returns 1 if successful, 0 otherwise.
 */
int g728_durbin(
	Float	*r,		/* autocorrelation coefficients */
	int	i,		/* order of filter already done */
	int	order,		/* order of filter */
	Float	*a,		/* filter coefficients of this frame */
	Float	*rc0,		/* first reflection coefficient */
	Float	*re)		/* residual energy */
{
	Float	t, rc;
	int	j, k;

	if (i == 0)
		*re = r[0];
	for ( ; i < order && *re > (Float)0.; i++) {
		t = r[i + 1] + g728_vrdotp(a, &r[1], i);
		a[i] = rc = -t / *re;		/* reflection coeff. */
		if (i == 0)			
			*rc0 = rc;		/* save rc[0] for postfilter */
		*re += rc * t;
		for (j = 0, k = i - 1; j <= k; j++, k--) {
			t = a[j] + rc * a[k];
			a[k] += rc * a[j];
			a[j] = t;
		}
	}
	return *re > (Float)0. ? 1 : 0;	/* test if ill-conditioned */
}

