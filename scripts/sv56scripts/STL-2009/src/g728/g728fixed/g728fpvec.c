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

void g728fp_vrshiftf(Float *a, Float *b, int cnt, int n)
{
	int i;

	if (cnt > 14)
		for (i = 0; i < n; i++)
			b[i] = a[i] < 0. ? -1. : 0.;
	else
		for (i = 0; i < n; i++)
			b[i] = (Short)a[i] >> cnt;
}

void g728fp_vlshiftf(Float *a, Float *b, int cnt, int n)
{
	int i;
	Float l;
	Float fac;

	if (cnt > 14) {
		for (i = 0; i < n; i++) {
			if (a[i] == 0.)
				b[i] = 0.;
			else {
				if (a[i] > 0.)
					b[i] = 32767.;
				else
					b[i] = -32768.;
			}
		}
	} else {
		fac = 1 << cnt;
		for (i = 0; i < n; i++) {
			l = a[i] * fac;
			if (l > 32767. ||  l < -32768.) {
				if (l < 0.)
					l = -32768.;
				else
					l = 32767.;
			}
			b[i] = l;
		}
	}
}

Float g728fp_vdotpf(Float *a, Float *b, int n)
{
	int i;
	Float r = 0.;

	for (i = 0; i < n; i++)
		r += a[i] * b[i];
	return r;
}

Float g728fp_vrdotpf(Float *a, Float *b, int n)
{
	Float r = 0.;
	int i;

	b += n - 1;
	for (i = 0; i < n; i++)
		r += a[i] * *b--;
	return r;
}
