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

/* normalize the target vector */
void g728fp_normtarv(
	Gain	*gain,		/* linear gain */
	Statelpc *target)	/* target vector, Q2 */
{
	int k;
	Long igaininv;
	Nshift nlsgaininv;
	Float gaininv;

	/* 1/gain */
	g728fp_divide(16384, 14, (Long)gain->data, gain->q, &igaininv, &nlsgaininv);
	/*
	 * a shift of 15 is used rather than 16 since the result of a multiply
	 * will never exceed 31 bits (except for the special case -32768*-32768
	 * and this never happens here since gain is positive.
	 */
	gaininv = igaininv;
	for (k = 0; k < IDIM; k++)
		 target->data[k] = (Short)((Long)(gaininv*target->data[k])>>15);
	target->q += nlsgaininv - 15;		/* update Q of target */
	/* renormalize */
	target->q += g728fp_vnormalizef(target->data, target->data, 16, IDIM);
}
