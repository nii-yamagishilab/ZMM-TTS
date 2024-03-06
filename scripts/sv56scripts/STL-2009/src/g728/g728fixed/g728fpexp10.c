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
 * Compute 10^(loggain/20)
 * This is equivalent to 2 ** (x = .1660964 * loggain)
 * If we break x into its integer and fractional part, then the integer
 * part can be added to the exponent and the fractional part can be
 * computed by Taylor series expansion of 2 ** xfrac.
 */
void g728fp_lexp10(Long loggain, Long *lineargain, Nshift *nlsgain)
{
	Long aa0, aa1, x, tmp;
	Nshift nls;
	static Long c[] = { 16384, 22702, 7866, 1874, 323 };

	/* SEPARATE INTEGER AND FRACTIONAL PARTS OF Q9 LOGGAIN */
	aa0 = 10 * loggain;
	aa1 = 20649 * loggain;
	aa1 <<= 1;
	ROUND(aa1, tmp);
	aa0 += tmp;
	aa1 = aa0 >> 15;
	nls = (Nshift)aa1;
	aa1 <<= 15;
	x = aa0 - aa1;

	/* COMPUTE EXPONENTIAL OF FRACTIONAL PART */
	aa0 = c[4] * x;
	aa0 <<= 1;
	aa1 = c[3] << 16;
	aa0 += aa1;
	ROUND(aa0, tmp);
	aa0 = tmp * x;
	aa0 <<= 1;
	aa1 = c[2] << 16;
	aa0 += aa1;
	ROUND(aa0, tmp);
	aa0 = tmp * x;
	aa0 <<= 1;
	aa1 = c[1] << 16;
	aa0 += aa1;
	ROUND(aa0, tmp);
	aa0 = tmp * x;
	aa1 = c[0] << 16;
	aa0 += aa1;
	ROUND(aa0, tmp);
	*lineargain = tmp;
	*nlsgain = 14 - nls;
}
