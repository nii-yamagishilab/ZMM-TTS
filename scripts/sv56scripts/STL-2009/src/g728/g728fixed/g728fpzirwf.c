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

/* zero input response of the weighting filter.  Block 10 of g728 */
void g728fp_zirwf(
	Float	*awz,		/* weighting filter all-zero coefficients */
	Float	*awp,		/* weighting filter all-pole coefficients */
	Statelpc *s,		/* zero-input response vector of LPC filter */
	Float	*zirwiir)	/* memory, all-pole part of weighting filter */
{
	int i, j;
	Nshift ir;
	Float stmp[LPCW + IDIM];

	/* Convert statelpc to Q_SPEECH */
	for (j = 0, i = 0; i < 3; i++, j += IDIM) {
		if ((ir = s[i].q - Q_SPEECH) >= 0)
			g728fp_vrshiftf(s[i].data, &stmp[j], ir, IDIM);
		else
			g728fp_vlshiftf(s[i].data, &stmp[j], -ir, IDIM);
	}
	g728fp_wfilter(awz, awp, stmp, zirwiir);	/* weighting filter */
}
