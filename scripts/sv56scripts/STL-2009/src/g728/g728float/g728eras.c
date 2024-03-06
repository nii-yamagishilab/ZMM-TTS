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

static int	vlimit = 1;
static int	uvlimit = 1;
static Float	vgain = (Float).8;
static Float	vdecay = (Float).2;
static Float	uvgain = (Float)1.;
static Float	uvdecay = (Float).2;

/*
 * Set up the excitation paramaters for the erased frame
 */
void g728_erasedsetup(G728DecData *d)
{
	int	msec10;

	msec10 = d->fecount >> 2;
	/* Determine mode on first erased frame */
	if (msec10 == 0) {
		if (d->tap > PPFTH/1.4)	{	/* voiced */
			d->voiced = 1;
			d->fedelay = d->kp - IDIM;
		} else {			/* not voiced */
			d->voiced = 0;
			d->fescaleavg = g728_vsumabs(d->etpast,
				FE_AVGLEN) / ((Float)FE_AVGLEN/IDIM);
		}
	}
	if (d->voiced) {
		d->vscale = vgain;
		if (msec10 > vlimit) {
			d->vscale -= vdecay * (msec10 - vlimit);
			if (d->vscale < 0.)
				d->vscale = (Float)0.;
		}
	} else {
		d->uvscale = uvgain;
		if (msec10 > uvlimit) {
			d->uvscale -= uvdecay * (msec10 - uvlimit);
			if (d->uvscale < 0.)
				d->uvscale = (Float)0.;
		}
	}
}

/*
 * Compute excitation for erased frames
 */
void g728_erasedex(Float *et, Float *gaindb, G728DecData *d)
{
	Float	sum;

	if (d->voiced)
		d->fescale = d->vscale;
	else {
		d->fedelay = (rand() & 0x7FFF) / 243;
		sum = g728_vsumabs(&d->etpast[d->fedelay], IDIM);
		if (sum == 0.)
			d->fescale = (Float)1.;
		else
			d->fescale = d->uvscale * d->fescaleavg / sum;
	}
	g728_vscale(&d->etpast[d->fedelay], d->fescale, IDIM, et);
	/*
	 * Need to re-calculate log-gain since default
	 * calculation based on summing codebook gains and
	 * backward adaptive gain no longer works,
	 * especially in fixed-point coder.
	 */
	sum = g728_vdotp(et, et, IDIM) / IDIM;
	if (sum < 1.)
		sum = (Float)1.;
	*gaindb = (Float)(10. * log10(sum) - GOFF);
}
