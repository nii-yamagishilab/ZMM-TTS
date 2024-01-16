#include "g728.h"

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
 * Simulate the floating point CCITT G.728 16 kb/s low-delay CELP coder.
 */

/*
 * Initialize the vocoder RAM
 */
void g728vocinit(G728VocData *d)
{
	int i;

	g728rominit();
	/* Encoder and decoder */
	d->info = 0;
	d->count = -1;
	d->gstate = &d->sbg[1];
	d->statelpc = &d->sb[NFRSZ - IDIM];
	g728_zero(d->sb, NSBSZ);
	g728_zero(d->rexp, LPC1);
	g728_zero(d->sbg, NSBGSZ);
	g728_zero(d->rexpg, LPCLG1);
	g728_zero(d->a, LPC);
	g728_zero(d->atmp, LPC);
	g728_zero(d->gp, LPCLG);
	d->gp[0] = (Float)-1.;
	for (i = 0; i < NUPDATE; i++)
		d->sbg[i] = (Float)-GOFF;
	/* Encoder only */
	d->s = &d->sbw[IDIM];
	g728_zero(d->sbw, NSBWSZ);
	g728_zero(d->rexpw, LPCW1);
	g728_zero(d->zirwiir, LPCW);
	g728_zero(d->wiir, LPCW);
	g728_zero(d->awz, LPCW);
	g728_zero(d->awp, LPCW);
	g728_zero(d->h, IDIM);
	d->h[IDIM - 1] = (Float)1.;
	g728_normsq(g728rom.y, d->h, d->y2);
	/* Postfilter only */
	d->resid = &d->pitchd[IDIM*2];
	g728_zero(d->stpfiir, G728_LPCPF);
	g728_zero(d->stpffir, G728_LPCPF);
	g728_zero(d->pfmem, PITCHSZ);
	g728_zero(d->lpfiir, DECORDER);
	g728_zero(d->pitchd, PITCHSZ);
	g728_zero(d->dec, PITCHSZD);
	g728_zero(d->ap, G728_LPCPF);
	g728_zero(d->az, G728_LPCPF);
	g728_zero(d->apf, G728_LPCPF);
	d->kp = 50;
	d->tiltz = (Float)0.;
	d->scalefil = (Float)1.;
	d->gl = (Float)1.;
	d->glb = (Float)0.;
	d->tap = (Float)0.;
	/* bit-rate related */
	d->gq = g728rom.gq;
	d->gq_db = g728rom.gq_db;
	d->search = g728_cbsearch16;
	d->wpcfv = g728rom.wpcfv;
	d->spfzcfv = g728rom.spfzcfv;
	d->pzcf = g728rom.pzcf;
	d->tiltf = g728rom.tiltf;
}

/*
 * G.728 Vocoder - Combined Encoder/Decoder with option Postfilter.
 * Take several frames of speech input and output the codebook indices
 * and synthesized speech.
 * Input should be floating-point numbers that are scaled so the maximum
 * sample is 4015.5, which is the bases for choosing GOFF. In the input array
 * the oldest sample should be first.
 */
void g728vocode(
	Float	*speech,	/* output speech */
	Short	*index,		/* output indices */
	Float	*input,		/* input speech */
	int	sz,		/* input size - must be multiple of IDIM */
	int	postfflag,	/* post-filter flag */
	G728VocData *d)		/* RAM */
{
	int	i;
	int	ig;		/* Gain index */
	int	is;		/* Shape index */
	Float	gain;		/* Predicted gain */
	Float	gaindb;		/* Predicted gain in dB */
	Float	rc;		/* Reflection coefficient 1 */
	Float	alpha;		/* Residual energy in Durbin's recursion */
	Float	*fp;
	/*
	 * The statics below could be locals, but are made static so they
	 * do not get placed on the stack.
	 */
	Float	et[IDIM];	/* Gain scaled excitation vector, shareable */
	Float	gptmp[LPCLG];	/* Temp buffer for gain predictors, shareable */
	Float	r[LPC1];	/* Autocorrelation coefficients, shareable */
	/* Encoder only */
	Float	awztmp[LPCW];	/* Temp buffer for weight preds, shareable */
	Float	target[IDIM];	/* Target vector for VQ, shareable */
	/* Postfilter only */
	Float	stpf[IDIM];	/* Buffer for postfiltered speech, shareable */

	for ( ; sz >= IDIM; sz -= IDIM) {	/* loop through vectors */
	fp = d->s;
	for (i = IDIM - 1; i >= 0; i--)
		fp[i] = *input++ * (Float)SCALEINPUT;
	if (++d->count == NUPDATE)
		d->count = 0;
	/* predict excitation gain */
	g728_lgpred(d->gp, d->gstate, &gain, &gaindb, 0, (Float*)0);
	/* zero input response of LPC */
	g728_zirlpcf(d->a, d->statelpc);
	/* weight zero input response */
	g728_wfilter(d->awz, d->awp, d->statelpc, d->zirwiir);
	/* weight the input speech */
	g728_wfilter(d->awz, d->awp, d->s, d->wiir);
	/* target vector for VQ */
	g728_vsub(d->wiir, d->zirwiir, IDIM, target);
	/* normalize target with gain */
	g728_vscale(target, (Float)1.0/gain, IDIM, target);
	/* codebook search */
	(*d->search)(target, d->h, d->y2, &ig, &is);
	*index++ = (is << NGBITS) + ig;
	/* save input speech */
	d->s -= IDIM;
	/* scale selected codevector */
	g728_vscale(g728rom.y[is], gain * d->gq[ig], IDIM, et);
	/* update the weighted lpc synthesis filter memory */
	g728_synmem(d->a, d->statelpc, et, d->awz, d->awp, d->zirwiir);
	if (postfflag) {
		/* lpc inverse filter to get lpc resid for pitch extraction */
		g728_firblkn(d->apf, d->statelpc, d->resid, G728_LPCPF, IDIM);
		if (d->count == UPDATEKP) {	/* update pitch estimate */
			/* pitch extraction by 4:1 decimation */
			g728_decp(&d->kp, d->dec, d->pitchd, d->lpfiir);	
			/* update long term postfilter coeff */
			g728_ltpfcoef(d->kp, &d->gl, &d->glb, &d->tap,d->pfmem,
					d->pzcf);
			d->resid = &d->pitchd[NFRSZ - IDIM];
		} else
			d->resid -= IDIM;
		g728_postf(d->kp, d->gl, d->glb, d->az, d->ap, d->tiltz,
			   d->statelpc, stpf, d->pfmem, d->stpffir,
			   d->stpfiir, &d->scalefil);
		fp = stpf;
	} else
		fp = d->statelpc;
	for (i = IDIM - 1; i >= 0; i--)
		*speech++ = fp[i] * (Float)SCALEOUTPUT;
	*--d->gstate = d->gq_db[ig] + g728rom.y_db[is] + gaindb;
	if (*d->gstate < -GOFF)		/* update log-gain lpc pred. mem */
		*d->gstate = (Float)-GOFF;
	d->statelpc -= IDIM;		/* save quantized speech */
	/* begin background analysis */
	if (d->count == UPDATELPC) {	/* calculate LPC coeff */
		d->info = 0;
		if (g728_hybridw(LPC, NFRSZ, NONR, (Float)ATTENL, g728rom.wnr,
				 d->sb, d->rexp,r) &&
		    /* lpc analysis of coded speech up to order 10 */
		    g728_durbin(r, 0, LPCW, d->atmp, &rc, &alpha)) {
			if (postfflag) {
				/*
				 * save 10th order lpc pred for short-term
				 * postfilter
				 */
				g728_cpy(d->atmp, G728_LPCPF, d->apf);
				d->tiltz = rc * d->tiltf;
				/* short-term postfilter coefficients */
				g728_vmult(d->apf, d->spfzcfv, G728_LPCPF,
					d->az);
				g728_vmult(d->apf, g728rom.spfpcfv, G728_LPCPF,
					d->ap);
			}
			/* continue lpc analysis from order 11 to 30 */
			if (g728_durbin(r, LPCW, LPC, d->atmp, &rc, &alpha)) {
				g728_vmult(d->atmp, g728rom.facv, LPC, d->atmp);
				d->info = 1;
			}
		}
		g728_rcpy(d->sb, LPC + NONR, &d->sb[NFRSZ]);
		d->statelpc = &d->sb[NFRSZ - IDIM];
	}
	else if (d->count == UPDATEW) {
		if (d->info)
			g728_cpy(d->atmp, LPC, d->a);	/* update predictors */
		/* update weighting filter coeff. */
		if (g728_hybridw(LPCW, NFRSZ, NONRW, (Float)ATTENW,
				 g728rom.wnrw, d->sbw, d->rexpw, r) &&
		    g728_durbin(r, 0, LPCW, awztmp, &rc, &alpha)) {
			g728_vmult(awztmp, g728rom.wpcfv, LPCW, d->awp);
			g728_vmult(awztmp, g728rom.wzcfv, LPCW, d->awz);
		}
		g728_rcpy(d->sbw, LPCW + NONRW, &d->sbw[NFRSZ]);
		d->s = &d->sbw[NFRSZ - IDIM];
		g728_impres(d->a, d->awz, d->awp, d->h); /* impulse response */
		g728_normsq(g728rom.y, d->h, d->y2);	/* codevector energy */
	}
	else if (d->count == UPDATEG) { /* calculate new gain predictor coeff */
		if (g728_hybridw(LPCLG, NUPDATE, NONRG, (Float)ATTENG,
				 g728rom.wnrg, d->sbg, d->rexpg, r) &&
		    g728_durbin(r, 0, LPCLG, gptmp, &rc, &alpha))
			/* update gain pred */
			g728_vmult(gptmp, g728rom.facgpv, LPCLG, d->gp);
		g728_rcpy(d->sbg, LPCLG + NONRG, &d->sbg[NUPDATE]);
		d->gstate = &d->sbg[NUPDATE];
	}	/* end of background analysis */
	}	/* end of vector loop */
}
