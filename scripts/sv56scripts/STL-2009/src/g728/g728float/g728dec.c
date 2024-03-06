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
 * enable or disable the post-filter.
 * by default, its on.
 */
void g728setpostf(int i, G728DecData *d)
{
	d->postf = i;
}

/*
 * Set the Frame Erasure frame size. The argument is a multiple
 * of 2.5msec. By default, it is set to 4, or 10 msec.
 */
void g728setfesize(int sz, G728DecData *d)
{
	d->feframesz = sz;
	d->adcount = d->feframesz - 1;
	d->feframevectors = d->feframesz * 4;
	d->feframesamples = d->feframevectors * IDIM;
}

/*
 * Initialize the decoder RAM
 */
void g728decinit(G728DecData *d)
{
	int i;

	g728rominit();
	/* Encoder and decoder */
	d->info = 0;
	d->count = NUPDATE - 1;
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
	/* Postfilter only */
	d->postf = 1;
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
	/* Frame erasure only */
	d->eraseframe = 0;
	d->ferased = 0;
	d->fecount = 0;
	d->ofecount = 0;
	g728_zero(d->etpast, G728_KPMAX);
	d->feframesz = FE_FRAMESZDEF;	/* 10 msec */
	d->adcount = d->feframesz - 1;
	d->feframevectors = d->feframesz << 2;
	d->feframesamples = d->feframevectors * IDIM;
	/* set the seed for the random number generator used in FE */
	srand(657);
	/* bit-rate related */
	d->gq = g728rom.gq;
	d->gq_db = g728rom.gq_db;
	d->spfzcfv = g728rom.spfzcfv;
	d->pzcf = g728rom.pzcf;
	d->tiltf = g728rom.tiltf;
}

/*
 * G.728 Decoder
 * Take several indices as input and output the speech.
 * This version also contains the frame erasure concealment code.
 */
void g728decode(
	Float	*speech,	/* output speech */
	Short	*index,		/* input indices */
	int	sz,		/* output size - must be multiple of IDIM */
	G728DecData *d)		/* RAM */
{
	int	i;
	int	ig;		/* Gain index */
	int	is;		/* Shape index */
	Short	ix;
	Float	gain;		/* Predicted gain */
	Float	gaindb;		/* Predicted gain in dB */
	Float	rc;		/* Reflection coefficient 1 */
	Float	*fp;
	/*
	 * The statics below could be locals, but are made static so they
	 * do not get placed on the stack.
	 */
	Float	et[IDIM];	/* Gain scaled excitation vector, shareable */
	Float	gptmp[LPCLG];	/* Temp buffer for gain predictors, shareable */
	/* Postfilter only */
	Float	stpf[IDIM];	/* Buffer for postfiltered speech, shareable */

	for ( ; sz >= IDIM; sz -= IDIM) {	/* loop through vectors */
	ix = *index++;
	if (++d->count == NUPDATE) {
		d->count = 0;
		/*
		 * If clamping the gain after an FE and the current frame
		 * is good, decrease the clamping time
		 */
		if (d->ofecount > 0 && !d->ferased)
			d->ofecount--;
		/*
		 * If the adaptation cycle counter is equal to the
		 * the FE frame size, check to see if the frame is erased
		 */
		if (++d->adcount == d->feframesz) {
			d->adcount = 0;
			/* determine if next frame is erased */
			d->ferased = d->eraseframe;
			if (d->ferased)
				d->eraseframe = 0;	/* clear input flag */
			/*
			 * At first good frames following a FE,
			 * limit the gain growth to decrease
			 * artifacts caused by the decoder filter
			 * memory not matching the encoder.
			 * The time to limit depends on the
			 * length of the erasure.
			 */
			else if (d->fecount > 0) {
				d->ofecount += d->fecount;
				/* clamp gain limit at 40 msec */
				if (d->ofecount > FE_AFTERMAX)
					d->ofecount = FE_AFTERMAX;
				d->fecount = 0;
			}
		}
		if (d->ferased) {
			d->fecount++;
			/*
			 * call the FE setup routine at the start of a FE and
			 * every 10msec there after
			 */
			if ((d->fecount & 3) == 1)
				g728_erasedsetup(d);
		}
	}
	/* zero input response of LPC */
	g728_zirlpcf(d->a, d->statelpc);
	if (d->ferased)	/* handle frame erasures */
		g728_erasedex(et, &gaindb, d);
	else {
		/* predict excitation gain */
		g728_lgpred(d->gp, d->gstate, &gain, &gaindb, d->ofecount,
			    &d->ogaindb);
		/* scale selected codevector */
		is = (ix >> NGBITS) & (NCWD - 1);
		ig = ix & (NG - 1);
		g728_vscale(g728rom.y[is], gain * d->gq[ig], IDIM, et);
	}
	/* update the weighted lpc synthesis filter memory */
	g728_synmemrx(d->a, d->statelpc, et);
	if (d->postf) {
		/* save et in etpast and old gain for FE */
		g728_rcpy(d->etpast, G728_KPMAX - IDIM, &d->etpast[IDIM]);
		g728_cpy(et, IDIM, d->etpast);
		d->ogaindb = gaindb;
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
	/* update log-gain lpc pred. mem */
	if (d->ferased)
		*--d->gstate = gaindb;
	else {
		*--d->gstate = d->gq_db[ig] + g728rom.y_db[is] + gaindb;
		if (*d->gstate < -GOFF)	
			*d->gstate = (Float)-GOFF;
	}
	d->statelpc -= IDIM;		/* save quantized speech */
	/* begin background analysis */
	if (d->count == UPDATELPC) {	/* calculate LPC coeff */
		d->info = 0;
		if (g728_hybridw(LPC, NFRSZ, NONR, (Float)ATTENL, g728rom.wnr,
				 d->sb, d->rexp,d->r) &&
		    /* lpc analysis of coded speech up to order 10 */
		    (d->info = g728_durbin(d->r, 0, LPCW, d->atmp, &rc,
					   &d->alpha))){
			if (d->postf) {
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
		}
		g728_rcpy(d->sb, LPC + NONR, &d->sb[NFRSZ]);
		d->statelpc = &d->sb[NFRSZ - IDIM];
	}
	else if (d->count == UPDATEW) {
		if (!d->ferased) {
			if (d->info)
				/* update predictors */
				g728_cpy(d->atmp, LPC, d->a);
		} else {
			if ((d->fecount & 3) == 1) {
				if (d->fecount == 1 && d->info)
					fp = d->atmp;
				else
					fp = d->a;
				g728_vmult(fp, g728rom.fatten, LPC, d->a);
			}
		}
	}
	else if (d->count == UPDATEG) { /* calculate new gain predictor coeff */
		/* continue lpc analysis from order 11 to 30 */
		if (d->info &&
		    d->fecount <= 1 &&
		    (d->info = g728_durbin(d->r, LPCW, LPC, d->atmp, &rc,
					   &d->alpha)))
			g728_vmult(d->atmp, g728rom.facv, LPC, d->atmp);
		if (g728_hybridw(LPCLG, NUPDATE, NONRG, (Float)ATTENG,
				 g728rom.wnrg, d->sbg, d->rexpg, d->r) &&
		    !d->ferased &&
		    g728_durbin(d->r, 0, LPCLG, gptmp, &rc, &d->alpha))
			/* update gain pred */
			g728_vmult(gptmp, g728rom.facgpv, LPCLG, d->gp);
		g728_rcpy(d->sbg, LPCLG + NONRG, &d->sbg[NUPDATE]);
		d->gstate = &d->sbg[NUPDATE];
	}	/* end of background analysis */
	}	/* end of vector loop */
}

/*
 * G.728 Decoder Frame Erasure(FE) Concealment
 * The post-filter must be on for FE concealment. If its off, turn it on.
 */
void g728decfe(
	Float	*speech,	/* output speech */
	int	sz,		/* output size - must be multiple of IDIM */
	G728DecData *d)		/* RAM */
{
	int	i;
	Short	ix;

	if (!d->postf)
		d->postf = 1;
	for ( ; sz >= d->feframesamples; sz -= d->feframesamples) {
		d->eraseframe = 1;
		for (i = 0; i < d->feframevectors; i++) {
			g728decode(speech, &ix, IDIM, d);
			speech += IDIM;
		}
	}
}
