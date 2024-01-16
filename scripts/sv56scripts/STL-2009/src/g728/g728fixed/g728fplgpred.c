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

/* predict dB value (20*log base 10) of the current execitation gain */
void g728fp_lgpred(
	Float	*gp,		/* log-gain predictor coefficients */
	Float	*gstate,	/* log-gain predictor memory */
	Gain	*gain,		/* predicted gain */
	Float	*gaindb,	/* predicted gain in dB */
	int	afterfe,	/* limit gain growth after frame erasure */
	Float	ogaindb)
{
	int	i;
	Long	l;
	Float	d;

	d = 0.;
	for (i = 0; i < LPCLG; i++)
		d -= gp[i] * gstate[i];
	SAT32(d);
	l = (Long)d >> 14;
	if (l > 14336)		/* limit log-gain (-32 to 28 dB in Q9) */
		l = 14336;
	if (l < -GOFF)
		l = -GOFF;
	/* limit gain growth after frame erasure */
	if (afterfe && (l - ogaindb) > FEGAINMAX)
		l = (Long)(ogaindb + FEGAINMAX);
	*gaindb = l;
	l += GOFF;		/* add back the gain offset (32 in Q9 = 16384)*/
	g728fp_lexp10(l, &l, &gain->q);	/* gain = 10**(a0/20) */
	gain->data = l;
}
