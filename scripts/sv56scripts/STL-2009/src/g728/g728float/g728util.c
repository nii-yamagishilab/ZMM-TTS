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
 * Utilities for floating point LD-CELP, G728 coder
 */

/*
 * gain-shape VQ codebook search
 *	fast version using existing array of norm squares  || H * y || **2.
 */
void g728_cbsearch16(
	Float	*t,		/* gain-normalized target */
	Float	*h,		/* impulse response of LPC + weighting filter */
	Float	*y2,		/* energy of filtered codevectors */
	int	*ig,		/* index of the best gain */
	int	*is)		/* index of the best shape codevector */
{
	int i, j;
	Float cor, cor2;
	Float d;		/* distortion */
	Float dm = (Float)1e30;	/* best distortion */
	Float ct[IDIM];		/* backward convolution of target */
	
	for (j = 0; j < IDIM; j++)
		ct[j] = g728_vdotp(&h[IDIM - 1 - j], t, j + 1);
	for (j = 0; j < NCWD; j++) {	/* search for best shape/gain pair */
		/* correlation between ct and codevector */
		cor = g728_vdotp(ct, g728rom.y[j], IDIM);
		/* find best quantized gain for shape */
		cor2 = cor / y2[j];
		if (cor2 < (Float)0.)
			cor2 = -cor2;
		for (i = 0; i < 3; i++) /* search gains */
			if (cor2 < g728rom.gb[i])
				break;
		if (cor <= (Float)0.)
			i += 4;
		/* distortion for best gain */
		d = g728rom.g2[i] * cor + g728rom.gsq[i] * y2[j];
		if (d < dm) {		/* update best shape/gain pair */
			dm = d;
			*ig = i;
			*is = j;
		}
	}
}

/*
 * hybrid window method for computing autocorrelation coefficients
 *	hybrid first-order recursive window and non-recursive window
 * 	recursion is done once per frame
 */
int g728_hybridw(
	int	lpc,	/* linear predictor order */
	int	nfrsz,	/* frame size (samples between autocor. updates) */
	int	nonr,	/* samples in the non-recursive part of the window */
	Float	atten,	/* decaying factor for recursive part */
	Float	*win,	/* hybrid window sample values */
	Float	*sb,	/* signal buffer */
	Float	*rexp,	/* recursive contribution to autocor. coeff */
	Float	*r)	/* autocorrelation coeff */
{
	int i;
	Float ws[NSBSZ];

	/* apply hybrid window */
	g728_vmult(sb, win, lpc + nfrsz + nonr, ws);
	for (i = 0; i <= lpc; ++i) {		/* autocorrelations */
		/* recursive part */
		rexp[i] = atten * rexp[i] +
			  g728_vdotp(&ws[nonr], &ws[nonr+i], nfrsz);
		/* non-recursive part */
		r[i] = rexp[i] + g728_vdotp(ws, &ws[i], nonr);
	}
	r[0] *= (Float)WNCF;
	return r[lpc] == 0. ? 0 : 1;
}

/* calculate impulse response of LPC and weighting filter */
void g728_impres(
	Float	*a,	/* LPC filter coeffs */
	Float	*awz,	/* coeff. of weighting filter all-zero (FIR) portion */ 
	Float	*awp,	/* coeff. of weighting filter all-pole (IIR) portion */
	Float	*h)	/* impulse response vector */
{
	int i, j = IDIM - 1;
	Float tmp[IDIM];

	h[j] = tmp[j] = (Float)1.;
	for (i = 1, j--; i < IDIM ; i++, j--) {
		tmp[j] = -g728_vdotp(a, &tmp[j+1], i);
		h[j] = tmp[j] + g728_vdotp(awz, &tmp[j+1], i)
			      - g728_vdotp(awp, &h[j+1], i);
	}
}

/*
 * log-gain predictor 
 *	Compute the value of the backward-predicted excitation gain
 *	range is limited between 0 and 60 dB 
 */
void g728_lgpred(
	Float	*gp,		/* log-gain predictor coefficients */
	Float	*gstate,	/* log-gain predictor memory */
	Float	*gain,		/* predicted gain */
	Float	*gaindb,	/* predicted gain in dB */
	int	afterfe,	/* first 10msec after frame erasure */
	Float	*ogaindb)	/* old gain in dB */
{
	Float t;

	t = -g728_vdotp(gp, gstate, LPCLG);
	g728_clamp(&t, (Float)28., (Float)-GOFF);
	if (afterfe && (t - *ogaindb) > FE_GAINMAX)
		t = *ogaindb + (Float)FE_GAINMAX;
	*gaindb = t;
	t = (Float)((t + GOFF) * .05);
	*gain = (Float)pow(10., t);
}

/*
 * convolve each codevector with the impulse response and compute
 * the norm square. || H*y || **2 
 */
void g728_normsq(
	Float y[][IDIM],/* excitation VQ shape codebook */
	Float *h,	/* the impulse response sequence of W(z)/A(z) */
	Float *y2	/* norm square (energy) of H*y */
	)
{
	int i, j;
	Float t;

	for (i = 0; i < NCWD; i++) {
		y2[i] = 0;
		for (j = 0; j < IDIM; j++) {
			t = g728_vrdotp(&y[i][j], &h[j], IDIM - j);
			y2[i] += t * t;
		}
	}
} 

/*
 * lpc synthesis and filter memory update for encoder
 *	compute zero-state response to scaled excitation vector
 *	update lpc filter memory
 *	update zero-input reponse memory of IIR weighting filter
 */
void g728_synmem(
	Float	*a,		/* LPC filter coeffs */
	Float	*statelpc,	/* LPC filter memory */
	Float	*et,		/* scaled excitation vector */
	Float	*awz,		/* weighting filter all-zero coeffs */
	Float	*awp,		/* weighting filter all-pole coeffs */
	Float	*zirwiir)	/* weighting filter zero impulse response mem */
{
	int i, j = IDIM - 1;
	Float wiir[IDIM], zsr[IDIM];

	for (i = 0; i < IDIM ; i++, j--) {
		zsr[j] = et[j] - g728_vdotp(a, &zsr[j+1], i);
		wiir[j] = zsr[j] + g728_vdotp(awz, &zsr[j+1], i)
				 - g728_vdotp(awp, &wiir[j+1], i);
		zirwiir[j] += wiir[j];		/* update weighting mem */
		statelpc[j] += zsr[j];		/* update lpc filter memory */
		g728_clamp(&statelpc[j], (Float)CLAMPLPC, (Float)-CLAMPLPC);
	}
}

/*
 * lpc synthesis and filter memory update for decoder
 *	same as synmem, but without the weighting filter
 */
void g728_synmemrx(
	Float	*a,		/* LPC filter coeffs */
	Float	*statelpc,	/* LPC filter memory */
	Float	*et)		/* scale excitation vector */
{
	int i, j = IDIM - 1;
	Float zsr[IDIM];

	for (i = 0; i < IDIM ; i++, j--) {
		zsr[j] = et[j] - g728_vdotp(a, &zsr[j+1], i);
		statelpc[j] += zsr[j];		/* update lpc filter memory */
		g728_clamp(&statelpc[j], (Float)CLAMPLPC, (Float)-CLAMPLPC);
	}
}

/* Weighting filter - filter memory is input + output */
void g728_wfilter(Float *b, Float *a, Float *bmem, Float *amem)
{
	int i;
	Float s;
	
	bmem += IDIM - 1;
	for (i = 0; i < IDIM; i++, bmem--) {
		s = bmem[0] + g728_vdotp(b, &bmem[1], LPCW) -
		    g728_vdotp(a, amem, LPCW);
		g728_rcpy(amem, LPCW - 1, &amem[1]);
		amem[0] =  s;
	}
}

/* zero-input response of the LPC filter  */
void g728_zirlpcf(
	Float	*a,		/* LPC filter coeffs */
	Float	*statelpc)	/* LPC filter memory */
{
	int	i;
	Float	f;

	statelpc += IDIM;
	for (i = 0; i < IDIM; i++) {
		f = -g728_vdotp(statelpc, a, LPC);
		*--statelpc = f;
	}
}
