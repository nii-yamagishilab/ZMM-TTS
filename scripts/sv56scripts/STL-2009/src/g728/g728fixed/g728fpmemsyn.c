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

#define SFAC	((Float)(1 << Q_FILTERCOEFF))
/*
 * lpc synthesis and filter memory update for encoder
 *	compute zero-state response(zsr) to scaled excitation vector
 *	update lpc filter memory
 *	update zero-input reponse memory of IIR weighting filter
 */
void g728fp_memsyn(
	Statelpc *et,		/* gain-scaled excitation vector */
	Float	*a,		/* LPC filter coeff. */
	Statelpc *s,		/* LPC filter memory */
	Float	*awz,		/* weighting filter all-zero coeff. */
	Float	*awp,		/* weighting filter all-pole coeff. */
	Float	*zirwiir)	/* weighting filter all-pole memory */
{
	Long	aa0;
	int	i, j, k;
	Long	mini, maxi;
	Nshift	diff;
	Float	d;
	Float	*zsrp, *wiirp;
	Float	zsr[IDIM];	/* zsr of LPC filter */
	Float	wiir[IDIM];	/* zsr of Weight filter */

	do {			/* may be executed 4 times in the worst case */
		zsrp = &zsr[IDIM];
		for (j = IDIM - 1, i = 0; i < IDIM; i++, j--, zsrp--) {
			d = et->data[j] * SFAC;
			for (k = 0; k < i; k++)
				d -= a[k] * zsrp[k];
			/*
			 * if exceeds 15 bits after assigned to zsr,
			 * right shift et by 1 and do it again.
			 */
			if (d > MAX28 || d < MIN28) {
				for (k = 0; k < IDIM; k++)
					et->data[k] = (Long)et->data[k] >> 1;
				et->q--;
				break;
			}
			zsr[j] = (Long)d >> Q_FILTERCOEFF;
		}
	} while (i < IDIM);
	if (awz) {	/* only need to do this in the encoder */
		/* weighting filter zsr */
		diff = et->q - Q_SPEECH;
		zsrp = &zsr[IDIM];
		wiirp = &wiir[IDIM];
		for (j = IDIM - 1, i = 0; i < IDIM; i++, j--, zsrp--) {
			d = zsr[j] * SFAC;
			for (k = 0; k < i; k++)
				d += awz[k] * zsrp[k] - awp[k] * wiirp[k];
			aa0 = (Long)d >> Q_FILTERCOEFF;
			SAT16(aa0);
			*--wiirp = aa0;
			if (diff > 0)		/* Convert wiir to Q_SPEECH */
				aa0 >>= diff;
			else if (diff < 0)
				aa0 <<= -diff;
			aa0 += (Long)zirwiir[j];
			SAT16(aa0);
			zirwiir[j] = aa0; /* update weighting mem */
		}
	}
	diff = s->q - et->q;		/* align statelpc and zsr */
	if (diff > 0) {
		for (i = 0; i < IDIM; i++)
			s->data[i] = (Long)s->data[i] >> diff; 
		s->q = et->q;
	} else if (diff < 0) {
		diff = -diff;
		for (i = 0; i < IDIM; i++)
			zsr[i] = (Long)zsr[i] >> diff; 
	}
	if (s->q < 0) {			/* Set clipping level */
		maxi = 4095 >> -s->q;
		mini = -maxi;
	} else {
		maxi = 4095 << s->q;
		if (maxi > MAX16) {
			maxi = MAX16;
			mini = MIN16;
		} else
			mini = -maxi;
	}
	for (i = 0; i < IDIM; i++) {
		aa0 = (Long)(s->data[i] + zsr[i]);/* update lpc filter memory */
		if (aa0 > maxi)			/* Clipping */
			aa0 = maxi;
		else if (aa0 < mini)
			aa0 = mini;
		s->data[i] = aa0;
	}
	/* re-normalize statelpc to 14 bits */
	s->q += g728fp_vnormalizef(s->data, s->data, 14, IDIM);
}
