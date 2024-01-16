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
 * Filtering routines for floating point LD-CELP, G728 coder
 */

/* fir filtering of a block */
void g728_firblk(Float *coeff, Float *in, Float *out, Float *mem, int m, int n)
{
	int i;
	Float t;

	for (i = n - 1; i >= 0; i--) {
		t = in[i] + g728_vdotp(coeff, mem, m);
		g728_rcpy(mem, m - 1, &mem[1]);
		mem[0] = in[i];
		out[i] = t;
	}
}

/* fir filtering of a block. Input is filter memory. */
void g728_firblkn(Float *coeff, Float *in, Float *out, int m, int n)
{
	int i;

	for (i = 0; i < n; i++)
		out[i] = in[i] + g728_vdotp(coeff, &in[i+1], m);
}

/* fir filtering of a block. Input is filter memory. Coeff[0] != 1. */
void g728_firblknz(Float *coeff, Float *in, Float *out, int m, int n)
{
	int i;

	for (i = 0; i < n; i++)
		out[i] = g728_vdotp(coeff, &in[i], m + 1);
}

/* iir filtering of a block  */
void g728_iirblk(Float *coeff, Float *in, Float *out, Float *mem, int m, int n)
{
	int i;
	Float t;

	for (i = n - 1; i >= 0; i--) {
		t = in[i] - g728_vdotp(coeff, mem, m);
		g728_rcpy(mem, m - 1, &mem[1]);
		mem[0] = out[i] = t;
	}
}

/* iir filtering of a block. Filter memory is only output. */
void g728_iirblkn(Float *coeff, Float *in, Float *mem, int m, int n)
{
	int i;
	Float t;

	for (i = n - 1; i >= 0; i--) {
		t = in[i] - g728_vdotp(coeff, mem, m);
		g728_rcpy(mem, m - 1, &mem[1]);
		mem[0] = t;
	}
}
