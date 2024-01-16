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

/*
 * Simulate the fixed point CCITT G.728 16 kb/s low-delay CELP coder.
 */
#include "g728fp.h"

/* weight table for the all-pole portion of the weighting filter - Q14 */
static Float iwwp[] = {
	9830,	5898,	3539,	2123,	1274,
	764,	459,	275,	165,	99
};

/* weight table for the all-zero portion of the weighting filter - Q14 */
static Float iwwz[] = {
	14746,	13271,	11944,	10750,	9675,
	8707,	7836,	7053,	6347,	5713
};

void g728fp_encinit(G728FpEncData *d)
{
	int i;

	d->count = NUPDATE - 1;
	d->info = 0;
	d->s = &d->sbw[IDIM];
	d->statelpc = &d->sb[NUPDATE - 1];
	d->gstate = &d->sbg[1];
	for (i = 0; i < NSBSZ/IDIM; i++) {
		d->sb[i].q = 16;
		g728fp_zerof(d->sb[i].data, IDIM);
	}
	g728fp_zerof(d->rexp, LPC1);
	d->nlsrexp = 31;
	g728fp_zerof(d->sbg, NSBGSZ);
	for (i = 0; i < NUPDATE; i++)
		d->sbg[i] = -GOFF;
	g728fp_zerof(d->rexpg, LPCLG1);
	d->nlsrexpg = 31;
	g728fp_zerof(d->a, LPC);
	g728fp_zerof(d->atmp, LPC);
	g728fp_zerof(d->gp, LPCLG);
	d->gp[0] = -(1 << Q_FILTERCOEFF);
	g728fp_zerof(d->sbw, NSBWSZ);
	g728fp_zerof(d->rexpw, LPCW1);
	d->nlsrexpw = 31;
	g728fp_zerof(d->zirwiir, LPCW);
	g728fp_zerof(d->wiir, LPCW);
	g728fp_zerof(d->awz, LPCW);
	g728fp_zerof(d->awp, LPCW);
	g728fp_zerof(d->h, IDIM);
	d->h[IDIM - 1] = (1 << Q_H);
	g728fp_normsq(d->h, d->y2);
	d->cbsearch = g728fp_cbsearch;
	d->lgupdate = g728fp_lgupdate;
	d->scalecv = g728fp_scalecv;
}

/*
 * G.728 Encoder.
 * Take several frames of speech input and output the codebook indices.
 * Input should be floating-point numbers that are scaled so the maximum
 * sample is 4015.5, which is the bases for choosing GOFF. In the input array
 * the oldest sample should be first.
 */
void g728fp_encode(
	Short	*index,		/* output indices */
	Short	*input,		/* input speech */
	int	sz,		/* input size - must be multiple of IDIM */
	G728FpEncData *d)	/* RAM */
{
	int		i;
	int		ig;	/* Gain index */
	int		is;	/* Shape index */
	Long		l;
	Gain		gain;
	Float		gaindb;
	Float		rc;
	Statelpc	et;
	Statelpc	target;
	Float		awptmp[LPCW];
	Float		awztmp[LPCW];
	Float		jawztmp[LPCW];
	Float		gptmp[LPCLG];

	for ( ; sz >= IDIM; sz -= IDIM) {	/* loop through vectors */
	for (i = IDIM - 1; i >= 0; i--)
		d->s[i] = (Float)(*input++ >> 1);
	if (++d->count == NUPDATE)
		d->count = 0;
	/* predict excitation gain */
	g728fp_lgpred(d->gp, d->gstate, &gain, &gaindb, 0, 0);
	g728fp_zirlpcf(d->a, d->statelpc);	/* zero input response of LPC */
	/* weight zero input response */
	g728fp_zirwf(d->awz, d->awp, d->statelpc, d->zirwiir);
	/* weight the input speech */
	g728fp_wfilter(d->awz, d->awp, d->s, d->wiir);
	target.q = Q_SPEECH;			/* target vector for VQ */
	for (i = 0; i < IDIM; i++) {
		l = (Long)(d->wiir[i] - d->zirwiir[i]);
		SAT16(l);
		target.data[i] = l;
	}
	g728fp_normtarv(&gain, &target);	/* normalize target with gain */
	/* codebook search */
	(*d->cbsearch)(&target, d->h, d->y2, &ig, &is);
	*index++ = (is << NGBITS) + ig;
	d->s -= IDIM;				/* save input speech */
	(*d->scalecv)(is, ig, &gain, &et);
	/* update the weighted lpc synthesis filter memory */
	g728fp_memsyn(&et, d->a, d->statelpc, d->awz, d->awp, d->zirwiir);
	/* update log-gain lpc pred memory */
	*--d->gstate = (*d->lgupdate)(gaindb, ig, is);
	d->statelpc--;			/* save quantized speech */
	/* begin background analysis */
	if (d->count == UPDATELPC) {	/* calculate LPC coeff */
		d->info = 0;
		if (g728fp_block49(d->sb, d->rexp, &d->nlsrexp, d->r))
			/* lpc analysis of coded speech up to order 10 */
			d->info = g728fp_durbin(d->r, 0, LPCPF, d->ja, &rc,
				&d->alpha, &d->qtmp);
		for (i = NSBSZ/IDIM - 1; i >= NUPDATE; i--)
			d->sb[i] = d->sb[i - NUPDATE];
		d->statelpc = &d->sb[NUPDATE - 1];
	}
	else if (d->count == UPDATEW) {
		if (d->info)
			g728fp_copyf(d->atmp, d->a, LPC);/* update predictors */
		/* update weighting filter coeff. */
		if (g728fp_block36(d->sbw, d->rexpw, &d->nlsrexpw, d->r) &&
 		    g728fp_durbin(d->r, 0, LPCW, jawztmp, &rc,
				  &d->alpha, &d->qtmp) &&
		    g728fp_weight(jawztmp, awptmp, LPCW, iwwp, d->qtmp) &&
		    g728fp_weight(jawztmp, awztmp, LPCW, iwwz, d->qtmp)) {
			g728fp_copyf(awztmp, d->awz, LPCW);
			g728fp_copyf(awptmp, d->awp, LPCW);
		}
		g728fp_rcopyf(d->sbw, &d->sbw[NFRSZ], NSBWSZ - NFRSZ);
		d->s = &d->sbw[NFRSZ - IDIM];
		g728fp_impres(d->a, d->awz, d->awp, d->h);/* impulse response */
 		g728fp_normsq(d->h, d->y2);		/* codevector energy */
	}
	/* calculate new gain predictor coeff. */
	else if (d->count == UPDATEG) {
		/* continue lpc analysis from order 11 to 50 */
		if (d->info &&
		    (d->info = g728fp_durbin(d->r, LPCPF, LPC, d->ja, &rc,
					     &d->alpha, &d->qtmp)))
			d->info = g728fp_weight(d->ja, d->atmp, LPC,
						g728fp_iw, d->qtmp);
		if (g728fp_block43(d->sbg, d->rexpg, &d->nlsrexpg, d->r) &&
		    g728fp_durbin(d->r, 0, LPCLG, d->ja, &rc, &d->alpha,
				  &d->qtmp) &&
		    g728fp_weight(d->ja, gptmp, LPCLG, g728fp_iwg, d->qtmp))
			/* update the gain pred */
			g728fp_copyf(gptmp, d->gp, LPCLG);
		g728fp_rcopyf(d->sbg, &d->sbg[NUPDATE], NSBGSZ - NUPDATE);
		d->gstate = &d->sbg[NUPDATE];
	}	/* end of background analysis */
	}	/* end of vector loop */
}

