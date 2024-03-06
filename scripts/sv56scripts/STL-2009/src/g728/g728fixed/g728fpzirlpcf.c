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

/* zero-input response of LPC filter. Block 9 of g728 */
void g728fp_zirlpcf(
	Float	 *a,	/* LPC filter coefficients (in Q14 or Q15) */
	Statelpc *s)	/* state variables (memory) of the LPC filter */
{
	int	i, j, k, shift;
	Long	t;
	Float	tf;
	Float	*ap, *sp;

	s[0].q = s[1].q;		/* find max exponent in statelpc */
	for (i = 2; i <= LPCVEC; i++)
		if (s[i].q < s[0].q)
			s[0].q = s[i].q;
	/* compute zero-input response of the LPC filter */
	for (k = IDIM, i = 0; i < IDIM; i++)  {
		ap = a;
		/* first partial */
		sp = &s[0].data[k];
		tf = 0.;
		for (j = i; j; j--)
			tf -= *sp++ * *ap++;
		t = (Long)tf;
		/* middle 9 vectors */
		for (j = 1; j < LPCVEC; j++) {
			shift = s[j].q - s[0].q;
			sp = s[j].data;
			tf = -(*sp++ * *ap++);
			tf -= *sp++ * *ap++;
			tf -= *sp++ * *ap++;
			tf -= *sp++ * *ap++;
			tf -= *sp++ * *ap++;
			if (shift > 30) {
				if (tf < 0.)
					t -= 1;
			} else
				t += (Long)tf >> shift;
		}
		/* last partial */
		if ((shift = s[LPCVEC].q - s[0].q) < 30) {
			tf = 0.;
			sp = s[LPCVEC].data;
			for (j = k; j; j--)
				tf -= *sp++ * *ap++;
			t += (Long)tf >> shift;
		}
		t >>= 14;
		SAT16(t);
		s[0].data[--k] = t;
	}
	/* normalize new state vector to 15 bits */
	s[0].q += g728fp_vnormalizef(s[0].data, s[0].data, 15, IDIM);
}
