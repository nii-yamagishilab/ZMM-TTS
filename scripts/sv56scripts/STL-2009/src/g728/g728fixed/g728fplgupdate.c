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

/* shape code book gain in dB */
static Short y_db[NCWD] = {
	-227,	10308,	6549,	7753,	7597,
	16563,	6406,	11933,	13569,	10569,
	16328,	6536,	15803,	11673,	21318,
	9100,	12245,	12018,	2503,	14690,
	18190,	28801,	16803,	20331,	18019,
	24920,	16159,	17618,	23072,	28075,
	19169,	25723,	8670,	10069,	503,
	8647,	11165,	18447,	4264,	17381,
	3531,	10543,	-2392,	2266,	14527,
	18788,	13030,	6238,	1825,	9090,
	211,	1888,	18088,	22557,	10893,
	18156,	3426,	13400,	-4375,	7970,
	7754,	25270,	5313,	15615,	-6296,
	4510,	2202,	-7229,	3146,	-2818,
	-2674,	-1567,	1841,	5803,	7824,
	319,	1815,	1765,	6949,	2484,
	2808,	9714,	-4215,	6678,	2634,
	3509,	871,	2190,	5546,	15337,
	3708,	2406,	5750,	7538,	3912,
	3543,	-10104,	303,	-6161,	-1142,
	3867,	5935,	-7201,	-759,	-2093,
	-2863,	2217,	-3243,	6161,	5853,
	7599,	6747,	-2001,	10218,	-54,
	1912,	11495,	10575,	4517,	4279,
	1813,	566,	4569,	4153,	3368,
	11179,	1694,	761
};

/* gain code book gain in dB */
static Short gq_db[NG] = {
	-11783,	-1828,	8127,	18082,
	-11783, -1828,	8127,	18082
};

Float g728fp_lgupdate(Float gaindb, int ig, int is)
{
	Long sum;

	sum = (Long)gaindb << (16 - Q_GAINDB);
	sum += (Long)gq_db[ig] << (16 - Q_GQ_DB);
	sum += (Long)y_db[is] << (16 - Q_Y_DB);
	sum >>= 16 - Q_GAINDB;
	if (sum < -GOFF)
		sum = -GOFF;
	return (Float)sum;
}
