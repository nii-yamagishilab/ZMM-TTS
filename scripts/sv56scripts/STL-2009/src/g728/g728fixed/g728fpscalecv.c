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

static Nshift ic2gq[NG] = {
	3, 3, 2, 1,
	3, 3, 2, 1
};

static Float gq[NG] = {
	4224,	7392,	12936,	22638,
	-4224,	-7392,	-12936,	-22638
};

/* scale the selected codebook vector by the gain */
void g728fp_scalecv(int is, int ig, Gain *gain, Statelpc *et)
{
	int	k;
	Float	a0;
	Long	l, l2;
	Nshift	a0q, ynq;
	Float	yn[IDIM];

	/*
	 * gain is normalized (16-bit numbers with no leading zeros). igq(ig)
	 * has ic2gq(ig)-1 leading zeros, so the product of igain*igq(ig) will
	 * be a 32-bit number with (ic2gq(ig)-1)+1 leading zeros (not counting
	 * sign bit).To keep all accuracy, shift left ic2gq(ig) bits and round.
	 */
	l = (Long)(gain->data * gq[ig]) << ic2gq[ig];
	ROUND(l, l2);
	a0 = l2;
	a0q = gain->q + Q_GQ + ic2gq[ig] - 16;

	/* normalize the selected shape codevector */
	ynq = g728fp_vnormalizef(&g728fp_y[is*IDIM], yn, 16, IDIM);

	/*
	 * scale the selected shape codevector. a0 is a 16-bit normalized
	 * number, yn is a 16-bit normalized array so the product
	 * has 1 leading 0, and the high word is a 15-bit number.
	 */
	for (k = 0; k < IDIM; k++) {
		l = (Long)(a0 * yn[k]);
		ROUND(l, l2);
		et->data[k] = l2;
	}
	et->q = a0q + Q_Y + ynq - 16;
}
