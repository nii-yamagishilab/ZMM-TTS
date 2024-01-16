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

/* log-gain hybrid window */
static Float wnrg[NSBGSZ] = {
	3026,	6025,	8973,	11845,	14615,
	17261,	19759,	22088,	24228,	26162,
	27872,	29344,	30565,	31525,	32216,
	32631,	32767,	32625,	32203,	31506,
	30540,	29461,	28420,	27416,	26448,
	25514,	24613,	23743,	22905,	22096,
	21315,	20562,	19836,	19135
};

/* Log-gain hybrid window */
int g728fp_block43(			/* 1 if ok, 0 if ill-conditioned */
	Float	*sb,		/* signal buffer */
	Float	*rexp,		/* contribution due to exponential window */
	Nshift	*nlsrexp,	/* Q or rexp */
	Float	*r)		/* autocorrelation coeff */
{
	int	i;
	Long	l, l2;
	Nshift	nls;
	Float	ws[NSBGSZ];

	nls = g728fp_vnormalizeshiftsf(sb, 15, NSBGSZ);
	for (i = 0; i < NSBGSZ; i++) {
		l = (Long)(sb[i] * wnrg[i]);
		if (nls > 0)
			l <<= nls;
		else if (nls < 0)
			l >>= -nls;
		ROUND(l, l2);
		ws[i] = l2;
	}
	return g728fp_autocor(LPCLG, NUPDATE, NONRG, 2, ws, nls, rexp, nlsrexp, r);
}
