/*                                                             v1.0 24.MAY.2005
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
	         ** This code has  (C) Copyright by AT&T Corp. **
       =============================================================

MODULE:         G711 Appendix I PLC module
                Sub-unit: Error pattern file routines

ORIGINAL BY:	AT&T Corp.

DESCRIPTION:
        This file contains procedures for reading the error pattern file.

FUNCTIONS:
  readplcmask_open ......... Open the file containing the packet loss concealment pattern.
                             Selelct the input routine based on the type of the file.
                             File type and input routine are from softbits.[ch] in G.191.

  readplcmask_erased ....... Read in a value from the file. Return 1 if erased, 0 if good.
                             if the error pattern file is shorter than the input file, roll over
                             and start from the beginning. This is useful for uniform error patterns.

  readplcmask_close ........ Clear everything out in case we call something after the close.

HISTORY:
    24.May.05 v1.0 First version <AT&T>
				   Integration in STL2005 <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>
  =============================================================================
*/

#include "plcferio.h"
#include "softbit.h"
#include "error.h"

/*
 * Open the file containing the packet loss concealment pattern.
 * Selelct the input routine based on the type of the file.
 * File type and input routine are from softbits.[ch] in G.191.
 */
void readplcmask_open(readplcmask *r, char *fname)
{
	char	streamtype;
	char	fileformat;
	short	s;

	if ((r->fp = fopen(fname, "rb")) == NULL)
		error("Can't open PLC error pattern file: %s", fname);
	/* shouldn't need to check this by check_eid_format doesn't */
	if (fread(&s, sizeof(short), 1, r->fp) != 1)
		error("Error pattern file %s is too short", fname);
	fseek(r->fp, 0L, SEEK_SET);
	fileformat = check_eid_format(r->fp, fname, &streamtype);
	if (fileformat == g192 || fileformat == byte) {
		if (streamtype == BER)
			error("File %s contains BER instead of FER", fname);
	} else if (fileformat != compact)
		error("File %s contains unknown error pattern format", fname);
	if (fileformat == g192)
		r->readfunc = read_g192;
	else if (fileformat == byte)
		r->readfunc = read_byte;
	else
		r->readfunc = read_bit_fer;
}

/*
 * Read in a value from the file. Return 1 if erased, 0 if good.
 * if the error pattern file is shorter than the input file, roll over
 * and start from the beginning. This is useful for uniform error patterns.
 */
int readplcmask_erased(readplcmask *r)
{
	short	s;
	short	h;

	if ((*r->readfunc)(&s, 1L, r->fp) != 1L) {
		/* roll over at end of file */
		fseek(r->fp, 0L, SEEK_SET);
		if ((*r->readfunc)(&s, 1L, r->fp) != 1L)
			error("Read on error pattern file failed");
	}
	if (soft2hard(&s, &h, 1L, FER) != 0L)
		error("Unexpected input value in error pattern file");
	return (int)h;
}

/*
 * clear everything out in case we call something after the close
 */
void readplcmask_close(readplcmask *r)
{
	fclose(r->fp);
	r->fp = NULL;
	r->readfunc = (long (*)(short*,long, FILE*))0;
}
