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

/* Weighting filter - filter memory is input + output */
void g728fp_wfilter(Float *b, Float *a, Float *bmem, Float *amem)
{
	int	i;
	Float	t;
	Float	fac;
	Long	l;
	
	fac = (1 << Q_FILTERCOEFF);
	bmem += IDIM - 1;
	for (i = 0; i < IDIM; i++, bmem--) {
		t = bmem[0] * fac +
		    g728fp_vdotpf(b, &bmem[1], LPCW) - g728fp_vdotpf(a, amem, LPCW);
		g728fp_rcopyf(amem, &amem[1], LPCW - 1);
		l = (Long)t >> Q_FILTERCOEFF;
		if (l > 32767)
			l = 32767;
		else if (l < -32768)
			l = -32768;
		amem[0] = l;
	}
}
