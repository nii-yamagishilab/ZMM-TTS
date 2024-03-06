/* ..............................................................................................30/Mar/2005*/
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
	This file contains functions used for the bitstream truncation tool.

  FUNCTIONS :
	Global (have prototype in reverb-lib.h)
		trunc		:		Frame truncation routine

  HISTORY :
	30.Mar.05	v1.0	First Beta version

  AUTHORS :
	Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com

*/

/* Routine to truncate a frame */
void trunca(syncWord, outFrameLgth, inpFrame, outFrame)
short syncWord;		/* Synchronisation word */
short outFrameLgth;	/* Length of the output frame */
short* inpFrame;	/* input frame */
short* outFrame;	/* output frame */
{
	int i;

	outFrame[0]=syncWord;
	outFrame[1]=outFrameLgth;
	for(i=0; i<outFrameLgth; i++)
	{
		outFrame[i+2]=inpFrame[i];
	}
}
