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

/*
 * Computed weighted filter coefficients. Output is Q14.
 * Returns 1 if successful, 0 if output overlows
 */
int g728fp_weight(
	Float	*a,	/* filter coeff */
	Float	*wta,	/* weighted filter coeff */
	int	m,	/* filter order */
	Float	*wgt,	/* weights */
	Nshift	aq)	/* Q of a */
{
	int	i;
	Long	l, l2;
	Float	d;
	Float	fac;

	if (aq == 13) {
		for (i = 0; i < m; i++) {
			d = a[i] * wgt[i] * 8.;
			if (d > MAX32 || d < MIN32) {
				printf("Q14 overflow in weight!\n");
				return 0;
			}
			l = (Long)d;
			ROUND(l, l2);
			wta[i] = l2;
		}
	} else {
		if (aq == 14)
			fac = 4.;
		else			/* aq == 15 */
			fac = 2.;
		for (i = 0; i < m; i++) {
			l = (Long)(a[i] * wgt[i] * fac);
			ROUND(l, l2);
			wta[i] = l2;
		}
	}
 	return 1;
}
