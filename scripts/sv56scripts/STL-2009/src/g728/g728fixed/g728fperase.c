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

#define DIMINV	13107	/* .2 in Q16 */
#define	Q14_1	16384	/* 1. in Q14 */
#define	VTH	7022	/* .4285714 in Q14 */

#define NVOICEDFEGAIN	5
#define NUNVOICEDFEGAIN	6

/* Scaling factors for 10 msec intervals in Q15 format */
static Short voicedgain[NVOICEDFEGAIN] = {	/* voiced speech */
	/* .8,	.8,	.6,	.4,	.2 */
	26214,	26214,	19661,	13107,	6554
};
static Short unvoicedgain[NUNVOICEDFEGAIN] = {	/* unvoiced speech */
	/* 1.,	1.,	.8,	.6,	.4,	.2 */
	32767,	32767,	26214,	19661,	13107,	6554
};

/*
 * Set up the excitation paramaters for the erased frame
 */
void g728fp_erasedsetup(int kp, Float tap, int msec10, G728FpDecData *d)
{
	Long	a, a2;
	Float	f;
	int	i;

	/*
	 * decide whether its voiced or unvoiced on the 1st frame
	 * thats erased. This decision is used for the duration of
	 * the Frame Erasure.
	 */
	if (msec10 == 0) {
		if (tap > VTH) {
			d->voiced = 1;
			d->fdelay = kp - IDIM;
		} else {
			d->voiced = 0;
			/* average excitation over the last 5 msec */
			for (f = 0, i = 0; i < FE_AVGLEN; i++)
				f += abs(d->etpast[i]);
			a = (Long)f;
			d->fscaleavgq = g728fp_normalize(&a, 32) - 16 + 3 + Q_ETPAST;
			ROUND(a, a2);
			d->fscaleavg = a2;
		}
	}
	/*
	 * attenuate the FE excitation gain, depending on length of the FE.
	 * set value for cases where msec10 is so large it is set to 0.
	 * if its lower, fscale and fscaleq are be overwritten below.
	 */
	d->fscaleq = 15;
	d->fscale = 0.;
	if (d->voiced) {	/* set gain for voiced data */
		if (msec10 < NVOICEDFEGAIN)
			d->fscale = (Float)voicedgain[msec10];
	} else {	/* unvoiced */
		if (msec10 < NUNVOICEDFEGAIN) {
			a = (Long)(d->fscaleavg * unvoicedgain[msec10]);
			d->fscaleq = g728fp_normalize(&a, 32) - 16 + d->fscaleavgq +15;
			ROUND(a, a2);
			d->fscale = a2;
		}
	}
}

/*
 * Compute excitation for erased frames
 */
void g728fp_erasedex(Statelpc *et, Float *gaindb, G728FpDecData *d)
{
	int	i;
	Nshift	diff;
	Nshift	etsumq;
	Short	etsum;
	Float	scale;
	Nshift	scaleq;
	Float	f, multfac;
	Long	a, a2;
	Gain	g;

	if (d->ferased) {
		/* For a voiced signal use the gains from erasedsetup */
		if (d->voiced) {
			scale = d->fscale;
			scaleq = d->fscaleq;
		}
		/*
		 * For unvoiced signal, scale it by the average.
		 * If the erasure is so long that the fscale gain has gone
		 * to 0, skip the code below.
		 */
		else if (d->fscale != 0) {
			d->fdelay = g728fp_mrand() / 243;
			for (f = 0, i = 0; i < IDIM; i++)
				f += abs(d->etpast[d->fdelay + i]);
			a = (Long)f;
			etsumq = g728fp_normalize(&a, 32) - 16 + Q_ETPAST;
			ROUND(a, a2);
			etsum = (Short)a2;
			if (etsum == 0) {
				scaleq = 14;
				scale = Q14_1;	/* 1. in Q14 */
			} else {
				g728fp_divide((Long)d->fscale, d->fscaleq, etsum,
					etsumq, &a, &scaleq);
				scale = a;
			}
		}
		/* unvoiced and 0 */
		else {
			scale = 0;
			scaleq = 14;
		}
		for (i = 0; i < IDIM; i++) {
			a = (Long)(scale * d->etpast[d->fdelay + i]) >> scaleq;
			SAT16(a);
			et->data[i] = a;
		}
		et->q = Q_ETPAST + g728fp_vnormalizef(et->data, et->data, 15, IDIM);
		/*
		 * Need to re-calculate log-gain since default
		 * calculation based on summing codebook gains and
		 * backward adaptive gain no longer works,
		 * especially in fixed-point coder.
		 */
		f = 0;
		for (i = 0; i < IDIM; i++)
			f += et->data[i] * et->data[i];
		a = (Long)f;
		g.q = g728fp_normalize(&a, 32) + (et->q << 1) - 16;
		g.data = a >> 16;
		a = (Long)(g.data * DIMINV);	/* scale by .2 in Q16 */
		g.q += g728fp_normalize(&a, 32);	/* + 16 - 16 */
		g.data = a >> 16;
		if (g.q > 14) {			/* If < 1., clamp to 1. */
			g.q = 14;
			g.data = Q14_1;		/* 1 in Q14 */
		}
		*gaindb = g728fp_log1010(&g);
	}
	g728fp_rcopyf(d->etpast, &d->etpast[IDIM], KPMAX - IDIM);
	if ((diff = et->q - Q_ETPAST) >= 0) {
		for (i = 0; i < IDIM; i++)
			d->etpast[i] = (Long)et->data[i] >> diff;
	} else if (diff < 0) {
		multfac = 1 << -diff;
		for (i = 0; i < IDIM; i++) {
			f = et->data[i] * multfac;
			SAT16(f)
			d->etpast[i] = f;
		}
	}
}
