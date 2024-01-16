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

/* LPC filter hybrid window */
static Float wnr[NSBSZ] = {
	1565,	3127,	4681,	6225,	7755,
	9266,	10757,	12223,	13661,	15068,
	16441,	17776,	19071,	20322,	21526,
	22682,	23786,	24835,	25828,	26761,
	27634,	28444,	29188,	29866,	30476,
	31016,	31486,	31884,	32208,	32460,
	32637,	32739,	32767,	32721,	32599,
	32403,	32171,	31940,	31711,	31484,
	31259,	31034,	30812,	30591,	30372,
	30154,	29938,	29724,	29511,	29299,
	29089,	28881,	28674,	28468,	28264,
	28062,	27861,	27661,	27463,	27266,
	27071,	26877,	26684,	26493,	26303,
	26114,	25927,	25742,	25557,	25374,
	25192,	25012,	24832,	24654,	24478,
	24302,	24128,	23955,	23784,	23613,
	23444,	23276,	23109,	22943,	22779,
	22616,	22454,	22293,	22133,	21974,
	21817,	21661,	21505,	21351,	21198,
	21046,	20896,	20746,	20597,	20450,
	20303,	20157,	20013,	19870,	19727
};

/* hybrid window for LPC */
int g728fp_block49(			/* 1 if ok, 0 if ill-conditioned */
	Statelpc *sb,		/* signal buffer */
	Float	*rexp,		/* contribution due to exponential window */
	Nshift	*nlsrexp,	/* Q or rexp */
	Float	*r)		/* autocorrelation coeff */
{
	Nshift nlsws, nrsh;
	int i, j, k;
	Long l, l2;
	Float ws[NSBSZ];

	/* find the minimum nls - this determines nlsws */
	nlsws = sb[0].q;
	for (i = 1; i < NSBSZ/IDIM; i++)
		if (nlsws > sb[i].q)
			nlsws = sb[i].q;
	/* now multiply sb by the hybrid window function */
	k = 0;
	for (i = 0; i < NSBSZ/IDIM; i++) {
		nrsh = sb[i].q - nlsws - 1;
		for (j = 0; j < IDIM; j++, k++) {
			l = (Long)(sb[i].data[j] *wnr[k]);
			if (nrsh < 0)	/* -1 */
				l <<= 1;
			else if (nrsh > 0) {
				if (nrsh > 30)
					l = l < 0 ? -1 : 0;
				else
					l >>= nrsh;
			}
			ROUND(l, l2);
			ws[k] = l2;
		}
	}
	/* call the core computation part shared by all 3 hybrid windows */
	return g728fp_autocor(LPC, NFRSZ, NONR, 2, ws, nlsws, rexp, nlsrexp, r);	
}
