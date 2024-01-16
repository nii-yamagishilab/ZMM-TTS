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

#define	LENG	17
static
long	ary[LENG] =
{
	0x4B14EA50L,
	0x53C4A8E0L,
	0x67B1FA98L,
	0x9CFB3BB5L,
	0x18761AF1L,
	0x7970CD66L,
	0xDBAFE136L,
	0x3C31FC3EL,
	0x697B37DEL,
	0x07BC568BL,
	0xCAFD3967L,
	0xA8F48722L,
	0x4AB26824L,
	0xA479EE47L,
	0x5C7246E2L,
	0x954BF297L,
	0x20A713ADL,
};
static
int	i1 = 0;
static
int	i2 = 12;

int g728fp_mrand(void)
{

	if(++i1 >= LENG)
		i1 = 0;
	if(++i2 >= LENG)
		i2 = 0;
	ary[i1] += ary[i2];
	return((ary[i1] >> 15) & 0x7FFF);
}
