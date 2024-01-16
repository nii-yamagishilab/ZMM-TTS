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

/* weight table for the all-pole portion of the short-term postfilter - Q14 */
static Float iwpfp[] = {
	12288,	9216,	6912,	5184,	3888,
	2916,	2187,	1640,	1230,	923
};

/* weight table for the all-zero portion of the short-term postfilter - Q14 */
static Float iwpfz[] = {
	10650,	6922,	4499,	2925,	1901,
	1236,	803,	522,	339,	221
};

static Float fatten[LPC] = {	/* frame erasure LPC coef. attenuation, Q14 */
	15892,	15416,	14953,	14505,	14069,
	13647,	13238,	12841,	12456,	12082,
	11719,	11368,	11027,	10696,	10375,
	10064,	9762,	9469,	9185,	8910,
	8642,	8383,	8131,	7888,	7651,
	7421,	7199,	6983,	6773,	6570,
	6373,	6182,	5996,	5816,	5642,
	5473,	5309,	5149,	4995,	4845,
	4700,	4559,	4422,	4289,	4161,
	4036,	3915,	3797,	3683,	3573
};

/*
 * enable or disable the post-filter.
 * by default, its on.
 */
void g728fp_setpostf(int i, G728FpDecData *d)
{
	d->postf = i;
}

/*
 * Set the Frame Erasure frame size. The argument is a multiple
 * of 2.5msec. By default, it is sz is set to 4, or 10 msec.
 */
void g728fp_setfesize(int sz, G728FpDecData *d)
{
	d->feframesz = sz;
	d->adcount = d->feframesz - 1;
	d->feframevectors = d->feframesz * 4;
	d->feframesamples = d->feframevectors * IDIM;
}

/*
 * Initialize the decoder state
 */
void g728fp_decinit(G728FpDecData *d)
{
	int i;

	d->count = NUPDATE - 1;
	d->info = 0;
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
	/* post-filter */
	d->postf = 1;
	d->resid = &d->pitchd[IDIM*2];
	g728fp_zerof(d->stlpci, LPCPF);
	g728fp_zerof(d->stpfiir, LPCPF);
	g728fp_zerof(d->stpffir, LPCPF);
	g728fp_zerof(d->pfmem, PITCHSZ);
	g728fp_zerof(d->lpfiir, DECORDER);
	g728fp_zerof(d->pitchd, PITCHSZ);
	g728fp_zerof(d->dec, PITCHSZD);
	g728fp_zerof(d->ap, LPCPF);
	g728fp_zerof(d->az, LPCPF);
	g728fp_zerof(d->apf, LPCPF);
	d->kp = 50;
	d->tiltz = 0;
	d->scalefil = 16384;
	d->gl = 16384;
	d->glb = 0;
	/* frame erasure */
	d->eraseframe = 0;
	d->ferased = 0;
	d->fecount = 0;
	d->ofecount = 0;
	d->tap = 0;
	g728fp_zerof(d->etpast, KPMAX);
	d->feframesz = 4;	/* 10. msec */
	d->adcount = d->feframesz - 1;
	d->feframevectors = d->feframesz * 4;
	d->feframesamples = d->feframevectors * IDIM;
	d->lgupdate = g728fp_lgupdate;
	d->scalecv = g728fp_scalecv;
}

/*
 * decode vectors
 */
void g728fp_decode(
	Short	*speech,	/* output speech */
	Short	*index,		/* input indices */
	int	sz,		/* output size - must be multiple of IDIM */
	G728FpDecData *d)	/* RAM */
{
	int	i;
	int	ix;
	int	ig;			/* Gain index */
	int	is;			/* Shape index */
	Nshift	il;
	Long	l, l2;
	Float	*aptr;		/* For attenuating a or atmp */
	Float	gptmp[LPCLG];
	Gain	gain;
	Float	gaindb;
	Statelpc et;
	Float	multfac;
	Float	rc;
	Float	tmp;
	Float	st[IDIM];
	Float	stpf[IDIM];
	Float	ws[IDIM];
	Float	aa[LPCPF];

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
				if (d->ofecount > AFTERFEMAX)
					d->ofecount = AFTERFEMAX;
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
				g728fp_erasedsetup(d->kp, d->tap,
						   d->fecount >> 2, d);
		}
	}
	if (d->ferased)
		g728fp_zirlpcf(d->a, d->statelpc);/* zero LPC input response */
	else {
		is = (ix >> NGBITS) & (NCWD - 1);
		ig = ix & (NG - 1);
		/* predict excitation gain */
		g728fp_lgpred(d->gp, d->gstate, &gain, &gaindb, d->ofecount,
			d->ogaindb);
		g728fp_zirlpcf(d->a, d->statelpc);/* zero LPC input response */
		/* scale selected codevector */
		(*d->scalecv)(is, ig, &gain, &et);
	}
	/* handle frame erasures */
	g728fp_erasedex(&et, &gaindb, d); 
	d->ogaindb = gaindb;
	/* update the weighted lpc synthesis filter memory */
	g728fp_memsyn(&et, d->a, d->statelpc, 0, 0, 0);

	if (d->postf) {
	  /* Don't let Q of statelpc get too small from frame erasures */
	  if (d->ferased && d->statelpc->q > 16)
		d->statelpc->q = 16;
	  /* feed decoded speech thru lpc inverse filter to get lpc residual */
	  multfac = (Float)(1 << (16 - d->statelpc->q + 2));
	  for (i = 0; i < IDIM; i++) {
		tmp = d->statelpc->data[i] * multfac;
		SAT32(tmp);
		l = (Long)tmp;
		ROUND(l, l2);
		st[i] = l2;
	  }
	  g728fp_lpcinv(d->apf, st, d->stlpci, d->resid);
	  if(d->count == UPDATEKP) {	/* update the pitch estimate */
		/* pitch extraction by 4:1 decimation */
	 	d->kp = g728fp_decp(d->kp, d->pitchd, d->dec, d->lpfiir);
		/* update long term postfilter coeff */
		g728fp_ltpfcoef(d->kp, &d->gl, &d->glb, &d->tap, d->pfmem);
		d->resid = &d->pitchd[NFRSZ - IDIM];
	  } else
		d->resid -= IDIM;
	  g728fp_postf(d->kp, d->gl, d->glb, d->az, d->ap, d->tiltz, st, stpf,
		d->stpffir, d->stpfiir, &d->scalefil, d->pfmem);
	  g728fp_vlshiftf(stpf, ws, 1, IDIM);
	} else {
	  multfac = (Float)(1 << (16 - d->statelpc->q + 3));
	  for (i = 0; i < IDIM; i++) {
		tmp = d->statelpc->data[i] * multfac;
		SAT32(tmp);
		l = (Long)tmp;
		ROUND(l, l2);
		ws[i] = l2;
	  }
	}
	/* output the speech */
	for (i = 0; i < IDIM; i++)
		speech[i] = (Short)ws[IDIM - 1 - i];
	speech += IDIM;
	/* update log-gain lpc pred memory */
	if (d->ferased)
		*--d->gstate = gaindb;
	else
		*--d->gstate = (*d->lgupdate)(gaindb, ig, is);
	d->statelpc--;			/* save quantized speech */
	/* begin background analysis */
	if (d->count == UPDATELPC) {	/* calculate LPC coeff */
		d->info = 0;
		if (g728fp_block49(d->sb, d->rexp, &d->nlsrexp, d->r) &&
 		    /* lpc analysis of coded speech up to order 10 */
		    (d->info = g728fp_durbin(d->r, 0, LPCPF, d->ja, &rc,
					     &d->alpha, &d->qtmp)) &&
		     d->postf) {
			/* save 10th order lpc pred for short-term postfilter */
			il = 16 - (d->qtmp - 13);
			for (i = 0; i < LPCPF; i++) {
				l = (Long)d->ja[i] << il;
				ROUND(l, l2);
				d->apf[i] = l2;
			}
			/* short-term postfilter coefficients */
			if (g728fp_weight(d->ja, aa, LPCPF, iwpfp, d->qtmp)) {
				g728fp_copyf(aa, d->ap, LPCPF);
				/* No overflow */
				g728fp_weight(d->ja, d->az,LPCPF,iwpfz,d->qtmp);
				/* update spectral tilt compensation filter */
				l = (Long)(rc * ITILTF);
				ROUND(l, l2);
				d->tiltz = l2;
			}
		}
		for (i = NSBSZ/IDIM - 1; i >= NUPDATE; i--)
			d->sb[i] = d->sb[i - NUPDATE];
		d->statelpc = &d->sb[NUPDATE - 1];
	}
	else if (d->count == UPDATEW) {
		if (!d->ferased) {
			if (d->info)
				/* update predictors */
				g728fp_copyf(d->atmp, d->a, LPC);
		} else if ((d->fecount & 3) == 1) {
			if (d->fecount == 1 && d->info)
				aptr = d->atmp;
			else
				aptr = d->a;
			for (i = 0; i < LPC; i++) {
				l = (Long)(fatten[i]*aptr[i]* 4.);
				ROUND(l, l2);
				d->a[i]=l2;
			}
		}
	}
	/* calculate new gain predictor coeff. */
	else if (d->count == UPDATEG) {
		/* continue lpc analysis from order 11 to 50 */
		if (d->info &&
		    d->fecount <= 1 &&
		    (d->info = g728fp_durbin(d->r, LPCPF, LPC, d->ja, &rc,
					     &d->alpha, &d->qtmp)))
			d->info = g728fp_weight(d->ja, d->atmp, LPC,
						g728fp_iw, d->qtmp);
		if (g728fp_block43(d->sbg, d->rexpg, &d->nlsrexpg, d->r) &&
		    !d->ferased &&
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

/*
 * frame erasure concealment
 * input size must be equal to the frame erasure size
 */
void g728fp_eraseframe(
	Short	*speech,	/* output speech */
	int	sz,		/* output size - must be feframesamples */
	G728FpDecData *d)	/* RAM */
{
	int	i;
	Short	ix;

	if (sz != d->feframesamples)
		error("g728fp_eraseframe: incorrect frame erase size");
	if (!d->postf)
		error("g728fp_eraseframe: post-filter must be enabled");
	d->eraseframe = 1;
	for (i = 0; i < d->feframevectors; i++) {
		g728fp_decode(speech, &ix, IDIM, d);
		speech += IDIM;
	}
}
