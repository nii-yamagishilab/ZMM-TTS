/*                                                        24.May.2005 v1.0
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  g711iplc.c

  Description:
  ~~~~~~~~~~~~
  Example-program simulating G.711 Appendix I Packet Loss Concealment.

  Usage:
  ~~~~~~

	$ g711iplc [options] plcpattern speechin speechout

	Options:
		-noplc		simulate silence insertion instead of concealment
		-stats		print out concealed frame statistics

	File Formats:
		plcpattern	G.192 FER file
		speechin	Headerless binary 8kHz 16-bit PCM file
		speechout	Headerless binary 8kHz 16-bit PCM file

	Example:
	$ g711iplc plcpattern speechin speechout

	  The command above takes the samples in file "speechin" and 
	  simulate PLC according to the file "plcpattern". The output file
	  is "speechout".

  Prototypes:
  ~~~~~~~~~~~
  Needs error.h, plcferio.h and lowcfe.h.

  Authors:	AT&T Corp.
  ~~~~~~~~

  History:
  ~~~~~~~~
  24.May.2005 v1.0 Release of 1st demo program for G711 PLC module <AT&T>.
				   Integration of this module in STL2005 <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
#include "plcferio.h"
#include "lowcfe.h"

char usage[] = "\
G711IPLC Version 1.0 of 24/May/2005\n\
  UGST/ITU-T G.711 Appendix I Packet Loss Concealment module\n\
  (*) G711IPLC module: COPYRIGTH 1997-2001 AT&T Corp.\n\
ANSI C Version\n\
\n\
Usage:\n\
	g711iplc_c [options] plcpattern speechin speechout\n\
Options:\n\
	-noplc		simulate silence insertion instead of concealment\n\
	-stats		print out concealed frame statistics\n\
File Formats:\n\
	plcpattern	G.192 FER file\n\
	speechin	Headerless binary 8kHz 16-bit PCM file\n\
	speechout	Headerless binary 8kHz 16-bit PCM file\
";

int WINAPIV main(int argc, char *argv[])
{
	int		i;
	int		dostats = 0;	/* if set print out erasure stats */
	int		dofe = 1;	/* if not set use silence insertion */
	int		nframes;	/* processed frame count */
	int		nerased;	/* erased frame count */
	char		*arg;
	FILE		*fi;		/* input file */
	FILE		*fo;		/* output file */
	LowcFE_c	lc;		/* PLC simulation data */
	readplcmask	mask;		/* error pattern file reader */
	short		in[FRAMESZ];	/* i/o buffer */

	argc--; argv++;
	while (argc > 0 && argv[0][0] == '-') {
		arg = argv[0];
		if (!strcmp("-noplc", arg))
			dofe = 0;
		else if (!strcmp("-stats", arg))
			dostats = 1;
		else
			error(usage);
		argc--; argv++;
	}
	if (argc != 3)
		error(usage);
	readplcmask_open(&mask, argv[0]);		/* PLC pattern file */
	if ((fi = fopen(argv[1], "rb")) == NULL)	/* input file */
		error("Can't open input file: %s", argv[1]);
	if ((fo = fopen(argv[2], "wb")) == NULL)	/* output file */
		error("Can't open output file: %s", argv[2]);
	nframes = nerased = 0;
	g711plc_construct(&lc);
	while (fread(in, sizeof(short), FRAMESZ, fi) == FRAMESZ) {
		nframes++;
		if (readplcmask_erased(&mask)) {
			nerased++;	/* frame is erased */
			if (dofe)	/* simulate concealment */
				g711plc_dofe(&lc, in);
			else {		/* simulate silence insertion */
				for (i = 0; i < FRAMESZ; i++)
					in[i] = 0;
				g711plc_addtohistory(&lc, in);
			}
		} else
			g711plc_addtohistory(&lc, in); /* frame is not erased */
		/*
		 * The concealment algorithm delays the signal by
		 * POVERLAPMAX samples. Remove the delay so the output
		 * file is time-aligned with the input file.
		 */
		if (nframes == 1)
			fwrite(&in[POVERLAPMAX], sizeof(short),
				FRAMESZ - POVERLAPMAX, fo);
		else
			fwrite(in, sizeof(short), FRAMESZ, fo);
	}
	/*
	 * the following code outputs the delayed speech in the history buffer
	 * so the length of the output file is an integral multiple of
	 * the frame size.
	 */
	if (nframes) {
		for (i = 0; i < FRAMESZ; i++)
			in[i] = 0;
		g711plc_addtohistory(&lc, in);
		fwrite(in, sizeof(short), POVERLAPMAX, fo);
	}
	if (dostats && nframes)
		printf("%d of %d frames concealed = %.2f%%\n", nerased, nframes,
			(double)nerased / nframes * 100.);
	/* cleanup */
	fclose(fo);
	fclose(fi);
	readplcmask_close(&mask);
	return 0;
}
