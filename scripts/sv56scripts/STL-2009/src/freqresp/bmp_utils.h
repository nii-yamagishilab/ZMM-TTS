/* ..............................................................................................31/Mar/2005*/
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
	This file contains prototypes of the following functions used for the frequency response measure tool.
		draw_linesdB(...)	:	draw input vectors in an image
		sav_bmp(...)		:	save an image to a bitmap file

  HISTORY :
	31.Mar.05	v1.0	First Beta version (STL2005)
	Dec.09		v1.0.1  Header correction (STL2009)

  AUTHORS :
	Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com

*/

void sav_bmp (int width, int height,char* image,char * bmpFileName, int little_endian);

void draw_linesdB(char* image, float* avg1PowSp, int lgth, int im_wdth, int im_hght, int border, float absMax, float absMin, float ordMax, float ordMin,int n);


