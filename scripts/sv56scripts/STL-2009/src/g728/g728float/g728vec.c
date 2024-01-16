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

void g728_vsub(Float *a, Float *b, int n, Float *c)
{
	int i;

	for (i = 0; i < n; i++)
		c[i] = a[i] - b[i];
}

void g728_vmult(Float *a, Float *b, int n, Float *c)
{
	int i;

	for (i = 0; i < n; i++)
		c[i] = a[i] * b[i];
}

void g728_vscale(Float *a, Float b, int n, Float *c)
{
	int i;

	for (i = 0; i < n; i++)
                c[i] = b * a[i];
}

Float g728_vsumabs(Float *a, int n)
{
	int i;
	Float sum = (Float)0.;

	for (i = 0; i < n; i++)
		if (a[i] >= (Float)0.)
			sum += a[i];
		else
			sum -= a[i];
	return sum;
}

Float g728_vdotp(Float *a, Float *b, int n)
{
	int i;
	Float sum = (Float)0.;

	for (i = 0; i < n; i++)
		sum += a[i] * b[i];
	return sum;
}

Float g728_vrdotp(Float *a, Float *b, int n)
{
	int i;
	Float sum = (Float)0.;

	b += n - 1;
	for (i = 0; i < n; i++)
		sum += a[i] * *b--;
	return sum;
}

void g728_clamp(Float *x, Float h, Float l)
{
	if (*x > h)
		*x = h;
	else if (*x < l)
		*x = l;
}
