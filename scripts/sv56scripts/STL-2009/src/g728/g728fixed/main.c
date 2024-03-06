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

#include <stdarg.h>
#include <string.h>
#include "g728fp.h"

static char usage[] = "\
G.728 LD-CELP Fixed-Point Coder\n\
\n\
Usage:\n\
g728fp [options] enc speechin bstreamout		# encoder\n\
g728fp [options] dec bstreamin speechout		# decoder\n\
g728fp [options] encdec speechin bstreamout speechout	# encoder+decoder\n\
g728fp [options] plc bstreamin plcmask speechout	# decoder with PLC\n\
\n\
Options:\n\
 -plcsize msec	set the packet loss size (default: 10 ms)\n\
 -stats		print out packet loss statistics (default: off)\n\
 -little	input/output files are little endian (default: machine order)\n\
 -big		input/output files are big endian (default: machine order)\n\
 -nopostf	turn off post-filter (default: on)\n\
\n\
File formats:\n\
 speechin, speechout - binary, 16-bit linear PCM, no headers\n\
 bstreamin, bstreamout - binary, one codebook entry per 16-bit word\n\
 plcmask - erasure mask - ASCII series of 1s(erasures) and 0s(good frames).\n\
\n\
(c) Copyright 1990-2007 AT&T Intellectual Property.\n\
All Rights Reserved.\
";

#define M_ENC	0
#define M_DEC	1
#define	M_PLC	2
#define M_ED	3	/* Encode/Decode */

static FILE	*indexf;
static FILE	*speechoutf;
static FILE	*speechinf;
static FILE	*ferasef;
static int	mode;
static int	postfflag = 1;		/* Set to 1 to turn post-filter on */
static int	dostats = 0;
static double	plcsize = 10.;		/* frame erasure size in msec */
static int	machineendian;		/* machine is little endian */
static int	fileendian;		/* files are little endian */

void openfiles(int argc, char **argv);
void closefiles(void);
int ferasedin(void);
void swapshort(Short *s, int cnt);

int WINAPIV main(int argc, char **argv)
{
	size_t		fsz;		/* frame size, samples */
	size_t		vsz;		/* frame size, vectors */
	int		plc25msec;	/* frame size, in 2.5 msecs */
	int		nframes;
	int		nerased;
	Short		*ix;		/* codewords */
	Short		*ds;		/* speech buffer */
	G728FpEncData	ed;
	G728FpDecData	dd;


	openfiles(argc, argv);
	fsz = (int)((plcsize * .001) * 8000.);
	vsz = fsz / IDIM;
	if ((fsz % IDIM) != 0 || (vsz % 4) != 0)
		error("FE size must be a multiple of 2.5 msec");
	plc25msec = vsz >> 2;
	if (plc25msec < 1 || plc25msec > 8)
		error("FE size must be between 2.5 and 20 msec");
	ds = (Short *)malloc(fsz * sizeof(Short));
	ix = (Short *)malloc(vsz * sizeof(Short));
	if (!ds || !ix)
		error("Malloc failed");
	if (mode == M_ENC) {
		g728fp_encinit(&ed);
		while (fread(ds, sizeof(Short), fsz, speechinf) == fsz) {
			if (machineendian != fileendian)
				swapshort(ds, fsz);
			g728fp_encode(ix, ds, fsz, &ed);
			if (machineendian != fileendian)
				swapshort(ix, vsz);
			if (fwrite(ix, sizeof(Short), vsz, indexf) != vsz)
				error("can't write index file");
		}
	} else if (mode == M_DEC) {
		g728fp_decinit(&dd);
		g728fp_setpostf(postfflag, &dd);
		while (fread(ix, sizeof(Short), vsz, indexf) == vsz) {
			if (machineendian != fileendian)
				swapshort(ix, vsz);
			g728fp_decode(ds, ix, fsz, &dd);
			if (machineendian != fileendian)
				swapshort(ds, fsz);
			if (fwrite(ds, sizeof(Short), fsz, speechoutf) != fsz)
				error("can't write speech output file");
		}
	} else if (mode == M_PLC) {
		nframes = 0;
		nerased = 0;
		g728fp_decinit(&dd);
		g728fp_setfesize(plc25msec, &dd);
		while (fread(ix, sizeof(Short), vsz, indexf) == vsz) {
			if (machineendian != fileendian)
				swapshort(ix, vsz);
			nframes++;
			if (ferasedin()) {
				nerased++;
				g728fp_eraseframe(ds, fsz, &dd);
			} else
				g728fp_decode(ds, ix, fsz, &dd);
			if (machineendian != fileendian)
				swapshort(ds, fsz);
			if (fwrite(ds, sizeof(Short), fsz, speechoutf) != fsz)
				error("can't write speech output file");
		}
		if (dostats && nframes)
		  printf("%d of %d frames erased = %.2f%%\n", nerased, nframes,
			(double)nerased / nframes * 100.);
	} else if (mode == M_ED) {
		g728fp_encinit(&ed);
		g728fp_decinit(&dd);
		g728fp_setpostf(postfflag, &dd);
		while (fread(ds, sizeof(Short), fsz, speechinf) == fsz) {
			if (machineendian != fileendian)
				swapshort(ds, fsz);
			g728fp_encode(ix, ds, fsz, &ed);
			g728fp_decode(ds, ix, fsz, &dd);
			if (machineendian != fileendian)
				swapshort(ix, vsz);
			if (fwrite(ix, sizeof(Short), vsz, indexf) != vsz)
				error("can't write index file");
			if (machineendian != fileendian)
				swapshort(ds, fsz);
			if (fwrite(ds, sizeof(Short), fsz, speechoutf) != fsz)
				error("can't write speech output file");
		}
	}
	free(ds);
	free(ix);
	closefiles();
	return 0;
}

void WINAPIV error(char *s, ...)
{
	va_list ap;
	char *s2;

	va_start(ap, s);
	s2 = va_arg(ap, char *);
	va_end(ap);
	fprintf(stderr, s, s2);
	fprintf(stderr, "\n");
	exit(1);
}

/* Return 1 if the current frame is erased, 0 otherwise */
int ferasedin(void)
{
	int	i, c;

	for (i = 0; i < 4; i++) {
		c = getc(ferasef);
		switch (c) {
		case EOF:
			/* Roll over if too short */
			fseek(ferasef, 0, SEEK_SET);
			break;
		case '\r':
		case '\n':
			break;
		case '0':
			return 0;
		case '1':
			return 1;
		default:
			error("Non 0/1 in Frame Erasure file");
			break;	/* never reached */
		}
	}
	error("Frame Erasure file contains illegal data");
	return 0;	/* not reached */			
}

void closefiles(void)
{
	if (mode == M_ENC || mode == M_ED)
		fclose(speechinf);
	if (mode != M_ENC)
		fclose(speechoutf);
	if (mode == M_PLC)
		fclose(ferasef);
	fclose(indexf);
}

FILE *openf(char *name, char *mode)
{
	FILE *f;

	if ((f = fopen(name, mode)) == NULL)
		error("Can't open file %s", name);
	return f;
}

/*
 * return 1 if machine is little endian, 0 otherwise
 */
int littleendian()
{
	Short s = 1;
	unsigned char *p = (unsigned char *)&s;
	if (*p == 1)
		return 1;
	else
		return 0;
}

/* byte swap an array of shorts */
void swapshort(Short *s, int cnt)
{
	unsigned char t, *p;
	int	i;

	p = (unsigned char *)s;
	for (i = 0; i < cnt; i++) {
		t = p[0];
		p[0] = p[1];
		p[1] = t;
		p += 2;
	}
}

void openfiles(int argc, char **argv)
{
	char	*cmd;

	argv++; argc--;
	machineendian = littleendian();
	fileendian = machineendian;
	while (argc && argv[0][0] == '-') {
		if (!strcmp("-plcsize", argv[0])) {
			plcsize = atof(argv[1]);
			argv++; argc--;
		} else if (!strcmp("-nopostf", argv[0]))
			postfflag = 0;
		else if (!strcmp("-stats", argv[0]))
			dostats = 1;
		else if (!strcmp("-little", argv[0]))
			fileendian = 1;
		else if (!strcmp("-big", argv[0]))
			fileendian = 0;
		else
			error(usage);
		argv++; argc--;
	}
	if (argc < 1)
		error(usage);
	cmd = argv[0];
	argv++; argc--;
	if (!strcmp("enc", cmd)) {
		if (argc != 2)
			error(usage);
		mode = M_ENC;
		speechinf = openf(argv[0], "rb");
		indexf = openf(argv[1], "wb");
	} else if (!strcmp("dec", cmd)) {
		if (argc != 2)
			error(usage);
		mode = M_DEC;
		indexf = openf(argv[0], "rb");
		speechoutf = openf(argv[1], "wb");
	} else if (!strcmp("plc", cmd)) {
		if (argc != 3)
			error(usage);
		mode = M_PLC;
		postfflag = 1;
		indexf = openf(argv[0], "rb");
		ferasef = openf(argv[1], "rb");
		speechoutf = openf(argv[2], "wb");
	} else if (!strcmp("encdec", cmd)) {
		if (argc != 3)
			error(usage);
		mode = M_ED;
		speechinf = openf(argv[0], "rb");
		indexf = openf(argv[1], "wb");
		speechoutf = openf(argv[2], "wb");
	} else
		error(usage);
}
