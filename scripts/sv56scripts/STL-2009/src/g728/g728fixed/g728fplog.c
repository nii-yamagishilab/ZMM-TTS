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

#define	CONST34		24576	/* .75 in Q15 */
#define LOG2OVERLOG10	24660	/* 3.0102999566 in Q13 */

/*
 * Log2 - returns log2(x)
 *	Input - normalized 16-bit number with exponent range +- 32;
 *	Calculated by putting the mantissa in the range (.5 <= x < .1)
 *	and taking the Taylor series of Log2 around the point .75.
 *	When finished, the exponent is added back in.
 *	c[0] is just log2(.75) and the other coefficients may be calculated
 * 	as follows:
 *		fac = 1./log(2.);
 *		c[0] = log(.75) * fac;
 *		for (i = 1; i < 10; i++) {
 *			c[i] = fac * 1. / (pow(.75, (double)i) * (double)i);
 *			fac = -fac;
 *		}
 *	Output: Q10
 *
 */
static Short llog2(		/* Returned in Q10 */
	Gain	*x)
{
	int	i;
	Short	s;	/* Q13 */
	Short	r;	/* Q10 */
	Short	xa;	/* x - .75, in Q16 */
	/*
	 * c[] =-0.4150374993,	1.9235933879,	-1.2823955919,
	 *	1.1399071928,	-1.1399071928,	1.2159010057
	 */
static	Short	c[6] = {	/* Q13 */
		-3400,		15758,		-10505,
		9338,		-9338,		9961
	};

	xa = ((Short)x->data - CONST34)<<1; /* Value now -.25 <= xa < .25, Q16*/
	r = (15 - x->q) << 10;		/* Exponent portion of result */
	s = 0;
	for (i = 5; i >= 1; i--) {
		s += c[i];
		s = (Short)((Long)s * xa >> 16);	/* Q13 = Q13 * Q16 */
	}
	r += (s + c[0]) >> 3;		/* Mantissa portion of result */
	return r;
}

/*
 * Log1010 - return 10. * log10(x)
 *	calculated with log2(x)
 *	log10(x) = log2(x) * log10(2.)
 *		 = log2(x) * log(2) / log(10);
 */
Short g728fp_log1010(		/*  Returned in Q9 */
	Gain	*x)
{
	Double a;

	a = (Long)llog2(x) * LOG2OVERLOG10;	/* Q10 * Q13 = Q23 */	
	a -= (Long)GOFF << 14;			/* Q9 -> Q23 */
	return (Short)((Long)a >> 14);		/* Q23 -> Q9 */
}
