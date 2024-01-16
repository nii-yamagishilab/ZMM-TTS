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
 * 	g728fp_divide: this subroutine performs a 16-bit precision g728fp_divide operation
 * 		on two numbers in "normalized floating-point representation".
 */
void g728fp_divide(
	Long	num,	/* normalized numerator mantissa */
	Nshift	numq,	/* Q of num */
	Long	den,	/* normalized denominator mantissa */
	Nshift	denq,	/* Q of den */
	Long	*quo,	/* normalized quotient mantissa */
	Nshift	*quoq)	/* Q of quo */
{
	Long	n, d, q;
	int	i, isneg;
	Nshift	qq;

	qq = numq - denq + 14;		/* set exp */
	isneg = num * den < 0;		/* sign */
	n = abs(num);
	d = abs(den);
	if (n < d) {
		qq++;
		n <<= 1;		/* left shift num so q is normalized */
	}
	q = 0;
	for (i = 0; i < 15; i++) {	/* binary long divison */
		q <<= 1;		
		if(n > d) {
			q++;
			n -= d;
		}
		n <<= 1;
	}
	if(n > d)			/* rounding */
		q++;
	if(isneg)			/* restore sign */
		q = -q;
	*quoq = qq;
	*quo = q;
}
