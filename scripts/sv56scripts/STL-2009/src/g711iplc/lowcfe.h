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
#ifndef __LOWCFE_C_H__
#define __LOWCFE_C_H__

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct _LowcFE_c {
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
} LowcFE_c;

/* public functions */
void g711plc_construct(LowcFE_c*);	/* constructor */
void g711plc_dofe(LowcFE_c*, short *s);	/* synthesize speech for erasure */
void g711plc_addtohistory(LowcFE_c*, short *s);
		/* add a good frame to history buffer */

#ifdef __cplusplus
}
#endif

#endif  /* __LOWCFE_C_H__ */
