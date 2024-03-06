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

static Float g2[NG] = {		/* 2*gq */
	4224,	7392,	12936,	22638,
	-4224,	-7392,	-12936,	-22638
};

static Float gsq[NG] = {	/* gq*gq */
	545,	1668,	5107,	15640,
	545,	1668,	5107,	15640
};

static Float gb[] = {		/* gain quantizer cell boundaries */
	5808,	10164,	17787
};

/*
 * backward convolution of target for codebook search
 */
static void bconvolve(Statelpc *t, Float *h, Float *ct)
{
	int	j;
	Long	l;
	Nshift shift;

	for (j = 0; j < IDIM; j++)
		ct[j] = g728fp_vdotpf(&h[IDIM - 1 - j], t->data, j + 1);
	shift = Q_H + t->q - 7;
	if (shift > 30) {
		for (j = 0; j < IDIM; j++)
			ct[j] = ct[j] < 0. ? -1. : 0;
	} else {
		for (j = 0; j < IDIM; j++) {
			SAT32(ct[j]);
			l = (Long)ct[j] >> shift;
			SAT16(l);
			ct[j] = l;
		}
	}
}

/*
 * correlations between ct and codevector for code book search
 */
static void cbscorr(Float *y, Float *ct, int sz, Float *corr)
{
	int	i;

	for (i = 0; i < sz; i++, y += IDIM)
		corr[i] = g728fp_vdotpf(y, ct, IDIM);
}

/*
 * gain-shape VQ codebook search - 16kbps
 *	fast version using existing array of norm squares  || H * y || **2.
 */
void g728fp_cbsearch(
	Statelpc *t,	/* gain-normalized target */
	Float	*h,	/* impulse response of LPC + weighting filter */
	Float	*y2,	/* energy of filtered codevectors */
	int	*ig,	/* index of the best gain */
	int	*is)	/* index of the best shape codevector */
{
	int	shape, gain, bests, bestg, neg;
	Long	l;
	Float	d;		/* distortion */
	Float	dm;		/* best distortion */
	Float	cor;		/* correlation */
	Float	ysq;		/* y2 */
	Float	gb0, gb1, gb2;
	Float	corr[NCWD];
	Float	ct[IDIM];		/* backward convolution of target */

	/* backward convolution of target for codebook search */
	bconvolve(t, h, ct);
	/* correlations between ct and codevector */
	cbscorr(g728fp_y, ct, NCWD, corr);

	/* search for best shape/gain pair */
	gb0 = gb[0];
	gb1 = gb[1];
	gb2 = gb[2];
	dm = MAX32;
	for (shape = 0; shape < NCWD; shape++) {
		ysq = y2[shape];
		cor = corr[shape];
		/* find best quantized gain for shape */
		if (cor <= 0) {
			cor = -cor;
			neg = 1;
		} else
			neg = 0;
		if (cor <= gb0 * ysq)
			gain = 0;
		else if (cor <= gb1 * ysq)
			gain = 1;
		else if (cor <= gb2 * ysq)
			gain = 2;
		else
			gain = 3;
		l = (Long)cor;
		l >>= 14;
		if (l > 32767)
			l = 32767;
		cor = l;
		/* distortion for best gain */
		d = gsq[gain] * ysq - g2[gain] * cor;
		if (d < dm) {		/* update best shape/gain pair */
			dm = d;
			if (neg)
				gain += 4;
			bestg = gain;
			bests = shape;
		}
	}
	*ig = bestg;
	*is = bests;
}
