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

/* weighting filter hybrid window */
static Float wnrw[NSBWSZ] = {
	1957,	3908,	5845,	7760,	9648,
	11502,	13314,	15079,	16790,	18441,
	20026,	21540,	22976,	24331,	25599,
	26775,	27856,	28837,	29715,	30487,
	31150,	31702,	32141,	32464,	32672,
	32763,	32738,	32595,	32336,	31961,
	31472,	30931,	30400,	29878,	29365,
	28860,	28364,	27877,	27398,	26927,
	26465,	26010,	25563,	25124,	24693,
	24268,	23851,	23442,	23039,	22643,
	22254,	21872,	21496,	21127,	20764,
	20407,	20057,	19712,	19373,	19041
};

/* weighting filter hybrid window */
int g728fp_block36(			/* 1 if ok, 0 if ill-conditioned */
	Float	*sb,		/* signal buffer */
	Float	*rexp,		/* contribution due to exponential window */
	Nshift	*nlsrexp,	/* Q or rexp */
	Float	*r)		/* autocorrelation coeff */
{
	int	i, il;
	Long	l, l2;
	Nshift	nls;
	Float	ws[NSBWSZ];

	nls = g728fp_vnormalizeshiftsf(sb, 15, NSBWSZ);
	for (i = 0; i < NSBWSZ; i++) {
		l = (Long)(sb[i] * wnrw[i]) << nls;
		ROUND(l, l2);
		ws[i] = l2;
	}
	il = g728fp_autocor(LPCW, NFRSZ, NONRW, 1, ws, nls, rexp, nlsrexp, r);
	if (*nlsrexp > 41)
		*nlsrexp = 41;
	return il;
}
