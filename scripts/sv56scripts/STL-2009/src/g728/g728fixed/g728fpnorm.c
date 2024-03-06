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

/*
 * Normalize a Long into an N bit number
 */
Nshift g728fp_normalize(Long *l, int bits)
{
	Long aa0, mini, maxi;
	Nshift nls;

	aa0 = *l;
	if (aa0 == 0)
		return bits - 1;
	nls = 0;
	if (aa0 < 0) {			/* negative number */
		maxi = -(1 << (bits - 2));
		mini =  maxi << 1;
		if (aa0 < mini)
			do {
				aa0 >>= 1;
				nls--;
			} while (aa0 < mini);
		else
			while (aa0 >= maxi) {
				aa0 <<= 1;
				nls++;
			}
 	} else {			/* positive numbers */
		mini = 1 << (bits - 2);
		maxi = mini - 1;
		maxi += mini;
		if (aa0 > maxi)
			do {
			 	aa0 >>= 1;
				nls--;
			} while (aa0 > maxi);
		else
			while (aa0 < mini) {
				aa0 <<= 1;
				nls++;
			}
	}
	*l = aa0;
	return nls;
}

/*
 * Returns the number of left shifts needed to normalize a vector to N bits
 */
Nshift g728fp_vnormalizeshiftsf(/* returns # of shifts needed, + is left, - right */
	Float	*in,	/* input array */
	int	bits,	/* Bits to normalize to */
	int	len)	/* length of in/out */
{
	int	i;
	Long	l;
	Float	plus, minus;

	/* find largest postive and negative numbers */
	plus = minus = 0;
	for (i = 0; i < len; i++) {
		if (in[i] < minus)
			minus = in[i];
		if (in[i] > plus)
			plus = in[i];
	}
	if (minus < -plus)
		l = (Long)minus;
	else
		l = (Long)plus;
	return g728fp_normalize(&l, bits);
}

/*
 * Normalize a vector to N bits.
 *	Returns the number of left shifts needed to normalize it.
 *	If the array is all zero, it returns N - 1.
 */
Nshift g728fp_vnormalizef(	/* returns # of shifts needed, + is left, - right */
	Float	*in,	/* input array */
	Float	*out,	/* output array */
	int	bits,	/* Bits to normalize to */
	int	len)	/* length of in/out */
{
	int i;
	Nshift nls;

	nls = g728fp_vnormalizeshiftsf(in, bits, len);
	if (nls < 0)			/* scale the vector */
		for (i = 0; i < len; i++)
		  	out[i] = (Long)in[i] >> -nls;
	else
		for (i = 0; i < len; i++)
			out[i] = (Long)in[i] << nls;
	return nls;
}
