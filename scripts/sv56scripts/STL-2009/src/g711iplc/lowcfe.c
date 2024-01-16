/*                                                          24.May.2005 v.1.0
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


MODULE:         G.711 Appendix I PLC module.

ORIGINAL BY:    AT&T Corp.

FUNCTIONS:

g711plc_construct: ...... LowcFE Constructor.

g711plc_dofe: ........... Generate the synthetic signal.
						  At the beginning of an erasure determine the pitch, and extract
						  one pitch period from the tail of the signal. Do an OLA for 1/4
						  of the pitch to smooth the signal. Then repeat the extracted signal
						  for the length of the erasure. If the erasure continues for more than
						  10 msec, increase the number of periods in the pitchbuffer. At the end
						  of an erasure, do an OLA with the start of the first good frame.
						  The gain decays as the erasure gets longer.

g711plc_addtohistory: ... A good frame was received and decoded.
						  If right after an erasure, do an overlap add with the synthetic signal.
						  Add the frame to history buffer.

HISTORY:

  24.May.05  v1.0  Release of 1st G711 PLC module <AT&T>.
				   Integration of this module in STL2005 <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>.
=============================================================================
*/

#include <math.h>
#include "lowcfe.h"

static void g711plc_scalespeech(LowcFE_c*, short *out);
static void g711plc_getfespeech(LowcFE_c*, short *out, int sz);
static void g711plc_savespeech(LowcFE_c*, short *s);
static int g711plc_findpitch(LowcFE_c*);
static void g711plc_overlapadd(Float *l, Float *r, Float *o, int cnt);
static void g711plc_overlapadds(short *l, short *r, short *o, int cnt);
static void g711plc_overlapaddatend(LowcFE_c*, short *s, short *f, int cnt);
static void g711plc_convertsf(short *f, Float *t, int cnt);
static void g711plc_convertfs(Float *f, short *t, int cnt);
static void g711plc_copyf(Float *f, Float *t, int cnt);
static void g711plc_copys(short *f, short *t, int cnt);
static void g711plc_zeros(short *s, int cnt);

void g711plc_construct(LowcFE_c *lc)
{
	lc->erasecnt = 0;
	lc->pitchbufend = &lc->pitchbuf[HISTORYLEN];
	g711plc_zeros(lc->history, HISTORYLEN);
}

/*
 * Get samples from the circular pitch buffer. Update poffset so
 * when subsequent frames are erased the signal continues.
 */
static void g711plc_getfespeech(LowcFE_c *lc, short *out, int sz)
{
	while (sz) {
		int cnt = lc->pitchblen - lc->poffset;
		if (cnt > sz)
			cnt = sz;
		g711plc_convertfs(&lc->pitchbufstart[lc->poffset], out, cnt);
		lc->poffset += cnt;
		if (lc->poffset == lc->pitchblen)
			lc->poffset = 0;
		out += cnt;
		sz -= cnt;
	}
}

static void g711plc_scalespeech(LowcFE_c *lc, short *out)
{
	int	i;
	Float	g = (Float)1. - (lc->erasecnt - 1) * ATTENFAC;
	for (i = 0; i < FRAMESZ; i++) {
		out[i] = (short)(out[i] * g);
		g -= ATTENINCR;
	}
}

/*
 * Generate the synthetic signal.
 * At the beginning of an erasure determine the pitch, and extract
 * one pitch period from the tail of the signal. Do an OLA for 1/4
 * of the pitch to smooth the signal. Then repeat the extracted signal
 * for the length of the erasure. If the erasure continues for more than
 * 10 msec, increase the number of periods in the pitchbuffer. At the end
 * of an erasure, do an OLA with the start of the first good frame.
 * The gain decays as the erasure gets longer.
 */
void g711plc_dofe(LowcFE_c *lc,short *out)
{
	if (lc->erasecnt == 0) {
		 /* get history */
		g711plc_convertsf(lc->history, lc->pitchbuf, HISTORYLEN);
		lc->pitch = g711plc_findpitch(lc);	/* find pitch */
		lc->poverlap = lc->pitch >> 2;		/* OLA 1/4 wavelength */
		/* save original last poverlap samples */
		g711plc_copyf(lc->pitchbufend - lc->poverlap, lc->lastq,
			lc->poverlap);
		lc->poffset = 0;	/* create pitch buffer with 1 period */
		lc->pitchblen = lc->pitch;
		lc->pitchbufstart = lc->pitchbufend - lc->pitchblen;
		g711plc_overlapadd(lc->lastq, lc->pitchbufstart - lc->poverlap,
			lc->pitchbufend - lc->poverlap, lc->poverlap);
		/* update last 1/4 wavelength in history buffer */
		g711plc_convertfs(lc->pitchbufend - lc->poverlap,
			&lc->history[HISTORYLEN-lc->poverlap], lc->poverlap);
		/* get synthesized speech */
		g711plc_getfespeech(lc, out, FRAMESZ);
	} else if (lc->erasecnt == 1 || lc->erasecnt == 2) {
		/* tail of previous pitch estimate */
		short tmp[POVERLAPMAX];
		int saveoffset = lc->poffset;	/* save offset for OLA */
		/* continue with old pitchbuf */
		g711plc_getfespeech(lc, tmp, lc->poverlap);
		/* add periods to the pitch buffer */
		lc->poffset = saveoffset;
		while (lc->poffset > lc->pitch)
			lc->poffset -= lc->pitch;
		lc->pitchblen += lc->pitch;		/* add a period */
		lc->pitchbufstart = lc->pitchbufend - lc->pitchblen;
		g711plc_overlapadd(lc->lastq, lc->pitchbufstart - lc->poverlap,
			lc->pitchbufend - lc->poverlap, lc->poverlap);
		/* overlap add old pitchbuffer with new */
		g711plc_getfespeech(lc, out, FRAMESZ);
		g711plc_overlapadds(tmp, out, out, lc->poverlap);
		g711plc_scalespeech(lc, out);
	} else if (lc->erasecnt > 5) {
		g711plc_zeros(out, FRAMESZ);
	} else {
		g711plc_getfespeech(lc, out, FRAMESZ);
		g711plc_scalespeech(lc, out);
	}
	lc->erasecnt++;
	g711plc_savespeech(lc, out);
}

/*
 * Save a frames worth of new speech in the history buffer.
 * Return the output speech delayed by POVERLAPMAX.
 */
static void g711plc_savespeech(LowcFE_c *lc, short *s)
{
	/* make room for new signal */
	g711plc_copys(&lc->history[FRAMESZ], lc->history, HISTORYLEN - FRAMESZ);
	/* copy in the new frame */
	g711plc_copys(s, &lc->history[HISTORYLEN - FRAMESZ], FRAMESZ);
	/* copy out the delayed frame */
	g711plc_copys(&lc->history[HISTORYLEN - FRAMESZ - POVERLAPMAX], s,
		FRAMESZ);
}

/*
 * A good frame was received and decoded.
 * If right after an erasure, do an overlap add with the synthetic signal.
 * Add the frame to history buffer.
 */
void g711plc_addtohistory(LowcFE_c *lc, short *s)
{
	if (lc->erasecnt) {
		short overlapbuf[FRAMESZ];
		/*
		 * longer erasures require longer overlaps
		 * to smooth the transition between the synthetic
		 * and real signal.
		 */
		int olen = lc->poverlap + (lc->erasecnt - 1) * EOVERLAPINCR;
		if (olen > FRAMESZ)
			olen = FRAMESZ;
		g711plc_getfespeech(lc, overlapbuf, olen);
		g711plc_overlapaddatend(lc, s, overlapbuf, olen);
		lc->erasecnt = 0;
	}
	g711plc_savespeech(lc, s);
}

/*
 * Overlapp add the end of the erasure with the start of the first good frame
 * Scale the synthetic speech by the gain factor before the OLA.
 */
static void g711plc_overlapaddatend(LowcFE_c *lc, short *s, short *f, int cnt)
{
	int	i;
	Float	incrg;
	Float	lw, rw;
	Float	t;
	Float	incr = (Float)1. / cnt;
	Float	gain = (Float)1. - (lc->erasecnt - 1) * ATTENFAC;
	if (gain < 0.)
		gain = (Float)0.;
	incrg = incr * gain;
	lw = ((Float)1. - incr) * gain;
	rw = incr;
	for (i = 0; i < cnt; i++) {
		t = lw * f[i] + rw * s[i];
		if (t > 32767.)
			t = (Float)32767.;
		else if (t < -32768.)
			t = (Float)-32768.;
		s[i] = (short)t;
		lw -= incrg;
		rw += incr;
	}
}

/*
 * Overlapp add left and right sides
 */
static void g711plc_overlapadd(Float *l, Float *r, Float *o, int cnt)
{
	int	i;
	Float	incr, lw, rw, t;

	if (cnt == 0)
		return;
	incr = (Float)1. / cnt;
	lw = (Float)1. - incr;
	rw = incr;
	for (i = 0; i < cnt; i++) {
		t = lw * l[i] + rw * r[i];
		if (t > (Float)32767.)
			t = (Float)32767.;
		else if (t < (Float)-32768.)
			t = (Float)-32768.;
		o[i] = t;
		lw -= incr;
		rw += incr;
	}
}

/*
 * Overlapp add left and right sides
 */
static void g711plc_overlapadds(short *l, short *r, short *o, int cnt)
{
	int	i;
	Float	incr, lw, rw, t;

	if (cnt == 0)
		return;
	incr = (Float)1. / cnt;
	lw = (Float)1. - incr;
	rw = incr;
	for (i = 0; i < cnt; i++) {
		t = lw * l[i] + rw * r[i];
		if (t > (Float)32767.)
			t = (Float)32767.;
		else if (t < (Float)-32768.)
			t = (Float)-32768.;
		o[i] = (short)t;
		lw -= incr;
		rw += incr;
	}
}

/*
 * Estimate the pitch.
 * l - pointer to first sample in last 20 msec of speech.
 * r - points to the sample PITCH_MAX before l
 */
static int g711plc_findpitch(LowcFE_c *lc)
{
	int	i, j, k;
	int	bestmatch;
	Float	bestcorr;
	Float	corr;		/* correlation */
	Float	energy;		/* running energy */
	Float	scale;		/* scale correlation by average power */
	Float	*rp;		/* segment to match */
	Float	*l = lc->pitchbufend - CORRLEN;
	Float	*r = lc->pitchbufend - CORRBUFLEN;

	/* coarse search */
	rp = r;
	energy = (Float)0.;
	corr = (Float)0.;
	for (i = 0; i < CORRLEN; i += NDEC) {
		energy += rp[i] * rp[i];
		corr += rp[i] * l[i];
	}
	scale = energy;
	if (scale < CORRMINPOWER)
		scale = CORRMINPOWER;
	corr = corr / (Float)sqrt(scale);
	bestcorr = corr;
	bestmatch = 0;
	for (j = NDEC; j <= PITCHDIFF; j += NDEC) {
		energy -= rp[0] * rp[0];
		energy += rp[CORRLEN] * rp[CORRLEN];
		rp += NDEC;
		corr = 0.f;
		for (i = 0; i < CORRLEN; i += NDEC)
			corr += rp[i] * l[i];
		scale = energy;
		if (scale < CORRMINPOWER)
			scale = CORRMINPOWER;
		corr /= (Float)sqrt(scale);
		if (corr >= bestcorr) {
			bestcorr = corr;
			bestmatch = j;
		}
	}
	/* fine search */
	j = bestmatch - (NDEC - 1);
	if (j < 0)
		j = 0;
	k = bestmatch + (NDEC - 1);
	if (k > PITCHDIFF)
		k = PITCHDIFF;
	rp = &r[j];
	energy = 0.f;
	corr = 0.f;
	for (i = 0; i < CORRLEN; i++) {
		energy += rp[i] * rp[i];
		corr += rp[i] * l[i];
	}
	scale = energy;
	if (scale < CORRMINPOWER)
		scale = CORRMINPOWER;
	corr = corr / (Float)sqrt(scale);
	bestcorr = corr;
	bestmatch = j;
	for (j++; j <= k; j++) {
		energy -= rp[0] * rp[0];
		energy += rp[CORRLEN] * rp[CORRLEN];
		rp++;
		corr = 0.f;
		for (i = 0; i < CORRLEN; i++)
			corr += rp[i] * l[i];
		scale = energy;
		if (scale < CORRMINPOWER)
			scale = CORRMINPOWER;
		corr = corr / (Float)sqrt(scale);
		if (corr > bestcorr) {
			bestcorr = corr;
			bestmatch = j;
		}
	}
	return PITCH_MAX - bestmatch;
}

static void g711plc_convertsf(short *f, Float *t, int cnt)
{
	int	i;
	for (i = 0; i < cnt; i++)
		t[i] = (Float)f[i];
}

static void g711plc_convertfs(Float *f, short *t, int cnt)
{
	int	i;
	for (i = 0; i < cnt; i++)
		t[i] = (short)f[i];
}

static void g711plc_copyf(Float *f, Float *t, int cnt)
{
	int	i;
	for (i = 0; i < cnt; i++)
		t[i] = f[i];
}

static void g711plc_copys(short *f, short *t, int cnt)
{
	int	i;
	for (i = 0; i < cnt; i++)
		t[i] = f[i];
}

static void g711plc_zeros(short *s, int cnt)
{
	int	i;
	for (i = 0; i < cnt; i++)
		s[i] = 0;
}
