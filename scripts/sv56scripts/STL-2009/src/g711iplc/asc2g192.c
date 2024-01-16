/*
 * Packet Loss Concealment code for G.711 Appendix I
 *
 * (c) Copyright 1997-2001 AT&T Corp.
 * All rights reserved.
 *
 * NOTICE
 *
 * AT&T hereby grants you ("User") under its copyright rights the right to 
 * reproduce and prepare derivative works of this software source code 
 * ("Software") solely for internal research and evaluation purposes.  No other
 * rights are granted and no other use is intended or authorized.
 *
 * The availability of the Software does not provide any license by implication,
 * estoppel, or otherwise under any patent rights owned or controlled by
 * AT&T Corp. or others covering any use of the Software herein.
 *
 * USER UNDERSTANDS THAT AT&T ASSUMES NO LIABILITY FOR ANY DAMAGES, WHETHER
 * DIRECT OR OTHERWISE, WHICH THE USER MAY SUFFER DUE TO USER'S USE OF THE
 * SOFTWARE.  AT&T MAKES NO EXPRESS OR IMPLIED REPRESENTATIONS OR WARRANTIES
 * OF ANY TYPE, AND EXPRESSLY DISCLAIMS THE WARRANTIES OF FITNESS FOR A
 * PARTICULAR PURPOSE, MERCHANTABILITY, TITLE, AND INFRINGEMENT.  USER
 * ACKNOWLEDGES THAT IT ASSUMES TOTAL RESPONSIBILITY AND RISK FOR USER'S USE
 * OF THE SOFTWARE. 
 *
 * The foregoing terms and conditions shall constitute the entire agreement
 * between User and AT&T Corp. with respect to the subject matter hereof and
 * shall not be modified, superceded, or rescinded by any other agreement
 * without the express written authorization of AT&T Corp.
 *
 * Any copies or derivative works must include this and all other
 * proprietary notices.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "softbit.h"

char usage[] = "\
asc2g192: convert ASCII erasure pattern to G.192 file\n\
\n\
Copyright (c) 1997-2000. AT&T Corp.\n\
All rights reserved.\n\
\n\
Usage:\n\
	asc2g192 [options] mask.in mask.out\n\
Options:\n\
	-b		use the G.192 byte oriented format\n\
File Formats:\n\
	mask.in		ASCII file with series of 0s(good) and 1s(erasured)\n\
	mask.out	G.192 erasure pattern file\
";

static FILE	*ferasefp;
static int	frameerased(int *r);
static char	*fefileopen(char *name);

int WINAPIV main(int argc, char *argv[])
{
	short		s;
	char		c;
	int		dostats = 0;	/* if set print out erasure stats */
	int		g192byte = 0;	/* byte G.192 foramt */
	int		nframes;	/* processed frame count */
	int		nerased;	/* erased frame count */
	int		erased;
	char		*err, *arg;
	FILE		*fo;		/* output file */

	argc--; argv++;
	while (argc > 1 && argv[0][0] == '-') {
		arg = argv[0];
		if (!strcmp("-b", arg))
			g192byte = 1;
		else if (!strcmp("-stats", arg))
			dostats = 1;
		else
			error(usage);
		argc--; argv++;
	}
	if (argc != 2)
		error(usage);
	if ((err = fefileopen(argv[0])) != 0)	/* frame erasure mask file */
		error(err);
	if ((fo = fopen(argv[1], "wb")) == NULL)	/* output file */
		error("Can't open output file: %s", argv[2]);
	nframes = nerased = 0;
	while (frameerased(&erased)) {
		nframes++;
		if (erased) {
			nerased++;
		}
		if (g192byte) {
			c = erased ? BYTE_FER : BYTE_SYNC;
			fputc(c, fo);
		} else {
			s = erased ? G192_FER : G192_SYNC;
			fwrite(&s, sizeof(short), 1, fo);
		}
	}
	if (dostats && nframes)
		printf("%d of %d frames erased = %.2f%%\n", nerased, nframes,
			(double)nerased / nframes * 100.);
	fclose(fo);
	fclose(ferasefp);
	return 0;
}

static char *fefileopen(char *name)
{
	int	c;
	char	*err;

	if ((ferasefp = fopen(name, "rb")) == NULL)
		err = "Can't open Frame Erasure Mask file";
	else {
		err = 0;
		c = getc(ferasefp);
		if (c != '0' && c != '1') {
			err = "Not a valid Frame Erasure Mask file";
			fclose(ferasefp);
		} else
			fseek(ferasefp, 0, SEEK_SET);
	}
	return err;
}

/*
 * Read in an entry from frame erasure concealment mask file.
 * Ignore new lines and CRs. At the end of the file roll over
 * to the beginning. 1 means frame is erasured. 0 means frame is good.
 * For example for a uniform 10% loss with 10 msec erasures create a
 * file containing:
 *	0000000001\n
 * A uniform 10% loss file with 20msec erasures looks like:
 *	00000000000000000011\n
 */
static int frameerased(int *r)
{
	int	i, c;

	for (i = 0; i < 4; i++) {
		c = getc(ferasefp);
		switch (c) {
		case EOF:
			return 0;
		case '\r':
		case '\n':
			break;
		case '0':
			*r = 0;
			return 1;
		case '1':
			*r = 1;
			return 1;
		default:
			error("Non 0/1 in Frame Erasure file");
			break;	/* never reached */
		}
	}
	error("Frame Erasure file contains illegal data");
	return 0;	/* never reached */			
}
