/* ..............................................................................................02/Feb/2005*/
/*																										v1.0*/

/*=============================================================================

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
       =============================================================

  DESCRIPTION : 
	This file contains functions used for the reverberation tool.

  FUNCTIONS :
	Global (have prototype in reverb-lib.h)
		shift(...)		:		Shift coefficients of the input buffer for next block filtering
		conv(...)		:		Convolves the impulse response of a room with the input file

  HISTORY :
	02.Feb.05	v1.0	First Beta version
	10.jul.08   v1.01   Added 16 bit saturation and saturation warning


  AUTHORS :
	v1.0 Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com
    v1.01 Jonas Svedberg jonas.svedberg@ericsson.com
*/


/* this routine replaces the first N-1 samples of a buffer by the last N-1 samples */
void shift(
	short*	buff,
	long	N
);


/* this routine convolves buffIn with IR and stores the processed data into buffRvb */
/* alignFact is used to align the energy of the input file with an other file */
/* the ouput is an overflow  flag, if non_zero it indicates overflow with saturation at that sample position*/ 
long conv(
	float	*IR,		/* impulse response buffer */
	short	*buffIn,	/* input buffer */
	short	*buffRvb,	/* reverberated data */
	float	alignFact,	/* energy alignment factor */
	long	N,			/* length of the impulse response */
	long	L			/* length of the input buffer to process */
);
