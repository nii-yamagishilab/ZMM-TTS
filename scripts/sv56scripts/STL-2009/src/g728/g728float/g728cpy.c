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

void g728_zero(Float *b, int n)
{
	int i;

	for (i = 0; i < n; i++)
		b[i] = (Float)0.;
}

/* copy from beginning to end */
void g728_cpy(Float *x, int n, Float *y)
{
	int i;

	for (i = 0; i < n; i++)
		y[i] = x[i];
}

/* copy from end to beginning */
void g728_rcpy(Float *x, int n, Float *y)
{
	int i;

	for (i = n - 1; i >= 0; i--)
		y[i] = x[i];
}

/*  convert Short to Float */
void g728_cpyi2r(Short *x, int n, Float *y)
{
	int i;

	for (i = 0; i < n; i++)
		y[i] = x[i];
}

/*  convert Float to Short with rounding and saturation */
void g728_cpyr2i(Float *x, int n, Short *y)
{
	int i;

	for (i = 0; i < n; i++) {
		if (x[i] > 32767.)
			y[i] = 32767;
		else if (x[i] < -32768.)
			y[i] = -32768;
		else if (x[i] > 0.)
			y[i] = (Short)(x[i] + 0.5);
		else
			y[i] = (Short)(x[i] - 0.5);
	}
}
