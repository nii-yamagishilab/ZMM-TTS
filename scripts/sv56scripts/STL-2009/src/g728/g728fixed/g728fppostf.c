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

#define AGCFAQ	16220
#define AGCFAQ1	20972

/* post filter with both long and short delay sections. Blocks 71 thru 77. */
void g728fp_postf(
	int	kp,	/* pitch period */
	Float	gl,	/* long-term postfilter gain scaling factor */
	Float	glb,	/* gl*b, where b is the long-term pf coeff. */
	Float	*az,	/* short-term postfilter all-zero coefficients*/
	Float	*ap,	/* short-term postfilter all-pole coefficients*/
	Float	tiltz,	/* spectral tilt compensation coefficient */
	Float	*st,	/* unfiltered decoded speech */
	Float	*stpf,	/* postfiltered decoded speech */
	Float	*stpffir,
	Float	*stpfiir,
	Float	*scalefil,
	Float	*pfmem)
{
	Long a0, a1, scale;
	Long num, den;
	Float d0, d1;
	Nshift nlsden, nlsnum, nlsscale, ir;
	int j, k;
	Long agcscale;

	g728fp_rcopyf(pfmem, &pfmem[IDIM], PITCHSZ - IDIM);/* shift the speech buffer*/
	/* pfmem is Q0, while st is Q2 */
	for (j = 0; j < IDIM; j++)
		pfmem[j] = (Long)st[j] >> 2;
	for (k = IDIM - 1; k >= 0; k--) {
		/* long-term postfilter */
		d1 = d0 = gl * st[k] + glb * pfmem[kp + k];
		/* short-term postfilter */
		for (j = LPCPF - 1; j > 0; j--) {
			d1 += az[j] * stpffir[j] - ap[j] * stpfiir[j];
			stpffir[j] = stpffir[j - 1];
			stpfiir[j] = stpfiir[j - 1];
		}
		d1 += az[0] * stpffir[0] - ap[0] * stpfiir[0];
		d0 *= 4.;
		SAT32(d0);
		a0 = (Long)d0;
		ROUND(a0, a1);
		stpffir[0] = a1;
		a0 = (Long)(Long)d1 >> 14;
		SAT16(a0);
		stpfiir[0] = a0;
		/* spectral tilt compensation */
		d1 += stpfiir[1] * tiltz;
		a0 = (Long)(Long)d1 >> 14;
		SAT16(a0);
		stpf[k] = a0;
	}
	/* sum magnitudes of unfiltered and postfiltered speech */
	d1 = d0 = 0;
	for (k = 0; k < IDIM; k++) {
		d0 += abs(st[k]);
		d1 += abs(stpf[k]);
	}
	a0 = (Long)d0;
	a1 = (Long)d1;
	/* scaling factor */
	if (a1 > 4) {	/* 1. in Q2 */
		nlsden = g728fp_normalize(&a1, 32);
		ROUND(a1, den);
		nlsnum = g728fp_normalize(&a0, 32);
		ROUND(a0, num);
		g728fp_divide(num, nlsnum, den, nlsden,&scale,&nlsscale);
	} else {
		scale = 16384;
		nlsscale = 14;
	}
	/* low-pass filter the scale factor and scale the postfilter output */
	agcscale = AGCFAQ1 * scale;
	ir = nlsscale - 14 + (21 - 14);
	if (ir > 0) {
		if (ir > 30)
			agcscale = 0;
		else
			agcscale >>= ir;
	} else if (ir < 0) {
		d0 = agcscale * (Float)(1 << -ir);
		SAT32(d0);
		agcscale = (Long)d0;
	}
	for (k = IDIM - 1; k >= 0; k--) {
		d0 = (agcscale + AGCFAQ * *scalefil) * 4.;
		SAT32(d0);
		a0 = (Long)d0;
		ROUND(a0, a1);
		*scalefil = a1;
		d0 = *scalefil * stpf[k] * 4.;
		SAT32(d0);
		a0 = (Long)d0;
		ROUND(a0, a1);
		stpf[k] = a1;
	}
}
