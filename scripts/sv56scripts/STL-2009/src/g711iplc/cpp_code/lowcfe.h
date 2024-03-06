/*
  ============================================================================
   File: lowcfe.h                                            V.1.0-24.MAY-2005
  ============================================================================

                     UGST/ITU-T G711 Appendix I PLC MODULE

                          GLOBAL FUNCTION PROTOTYPES

   History:
   24.May.05	v1.0	First version <AT&T>
						Integration in STL2005 <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>
  ============================================================================
*/

#ifndef __LOWCFE_H__
#define __LOWCFE_H__

#ifdef USEDOUBLES
typedef double Float;		/* likely to be bit-exact between machines */
#else
typedef float Float;
#endif

#define	PITCH_MIN	40		/* minimum allowed pitch, 200 Hz */
#define PITCH_MAX	120		/* maximum allowed pitch, 66 Hz */
#define	PITCHDIFF	(PITCH_MAX - PITCH_MIN)
#define	POVERLAPMAX	(PITCH_MAX >> 2)/* maximum pitch OLA window */
#define	HISTORYLEN	(PITCH_MAX * 3 + POVERLAPMAX) /* history buffer length*/
#define	NDEC		2		/* 2:1 decimation */
#define	CORRLEN		160		/* 20 msec correlation length */
#define	CORRBUFLEN	(CORRLEN + PITCH_MAX) /* correlation buffer length */
#define	CORRMINPOWER	((Float)250.)	/* minimum power */
#define	EOVERLAPINCR	32		/* end OLA increment per frame, 4ms */
#define	FRAMESZ		80		/* 10 msec at 8kHz */
#define	ATTENFAC	((Float).2)	/* attenuation factor per 10ms frame */
#define	ATTENINCR	(ATTENFAC/FRAMESZ) /* attenuation per sample */

class LowcFE {
public:
		LowcFE();
	void	dofe(short *s);		/* synthesize speech for erasure */
	void	addtohistory(short *s);	/* add a good frame to history buffer */
protected:
	int	erasecnt;		/* consecutive erased frames */
	int	poverlap;		/* overlap based on pitch */
	int	poffset;		/* offset into pitch period */
	int	pitch;			/* pitch estimate */
	int	pitchblen;		/* current pitch buffer length */
	Float	*pitchbufend;		/* end of pitch buffer */
	Float	*pitchbufstart;		/* start of pitch buffer */
	Float	pitchbuf[HISTORYLEN];	/* buffer for cycles of speech */
	Float	lastq[POVERLAPMAX];	/* saved last quarter wavelengh */
	short	history[HISTORYLEN];	/* history buffer */

	void	scalespeech(short *out);
	void	getfespeech(short *out, int sz);
	void	savespeech(short *s);
	int	findpitch();
	void	overlapadd(Float *l, Float *r, Float *o, int cnt);
	void	overlapadd(short *l, short *r, short *o, int cnt);
	void	overlapaddatend(short *s, short *f, int cnt);
	void	convertsf(short *f, Float *t, int cnt);
	void	convertfs(Float *f, short *t, int cnt);
	void	copyf(Float *f, Float *t, int cnt);
	void	copys(short *f, short *t, int cnt);
	void	zeros(short *s, int cnt);
};

#endif  /* __LOWCFE_H__ */
