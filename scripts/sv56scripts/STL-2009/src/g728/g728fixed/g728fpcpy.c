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
#include <string.h>

void g728fp_copyf(Float *f, Float *t, int n)
{
	int i;

	for (i = 0; i < n; i++)
		t[i] = f[i];
}

void g728fp_rcopyf(Float *f, Float *t, int n)
{
	int i;

	for (i = n - 1; i >= 0; i--)
		t[i] = f[i];
}

void g728fp_zerof(Float *x, int n)
{
	int i;

	for (i = 0; i < n; i++)
		x[i] = 0.;
}
