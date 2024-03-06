/*                                                            28.Mar.2000 v2.2
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
       CODING STANDARDS". Also, please note the original authorship
       copyrights in the body of each function, when applicable.
       =============================================================

MODULE:         FIRFLT, HIGH QUALITY FIR UP/DOWN-SAMPLING FILTER
                Sub-unit: Psophometric filter function

ORIGINAL BY:
          
          Simao Ferraz de Campos Neto (C code)
	  Comsat Laboratories                  Tel:    +1-301-428-4516
	  22300 Comsat Drive                   Fax:    +1-301-428-9287
	  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

          Paolo Usai (Filter design)
          CSELT
          Audio Coding and Transm.Quality  Tel:    +39-11-228-5697
          Via G.Reiss Romoli, 274	   Fax:    +39-11-228-6207
          10148 Torino			   E-mail: Paolino.Usai@cselt.stet.it
          ITALY

DESCRIPTION:
        This file contains procedures for flat-weighting low-pass and 
	high-pass filters, with rate factors of 2 and 3 for both up-
	and down-sampling.

FUNCTIONS:
  Global (have prototype in firflt.h)

  Local (should be used only here -- prototypes only in this file)

  Global (have prototype in firflt.h)
         = psophometric_8khz_init ();

  Local (should be used only here -- prototypes only in this file)
         = fill_psophometric_8khz(...): idem, for the psophometric filter at


HISTORY:
    16.Dec.1991 v0.1 First beta-version <hf@pkinbg.uucp>
    28.Feb.1992 v1.0 Release of 1st version to UGST <hf@pkinbg.uucp>
    20.Apr.1994 v2.0 Added new filtering routines: modified IRS at 16kHz and
                     48kHz, Delta-SM, Linear-phase band-pass.
                     <simao@cpqd.ansp.br>
    30.Sep.1994 v2.1 Updated to accomodate changes in the name of the name and
                     slitting of module in several files, for ease of expansion.
    28.Mar.2000 v2.2 Added type casting to float in fill_psophometric_8khz() to 
                     eliminate compilation warnings in MS Visual C 
                     compiler <simao.campos@labs.comsat.com>

  =============================================================================
*/


/*
 * ......... INCLUDES .........
 */
#include <stdio.h>
#ifndef VMS
#include <stdlib.h>		/* General utility definitions */
#endif

#include "firflt.h"		/* Global definitions for FIR-FIR filter */

#define F float

/*
 * ......... Local function prototypes .........
 */
void fill_psophometric_8khz ARGS((float **h0, long *lenh0));


/* 
 * ..... Private function prototypes defined in other sub-unit ..... 
 */
extern SCD_FIR *fir_initialization ARGS((long lenh0, float h0[], double gain, 
                                                 long idwnup, int hswitch));


/*
 * ...................... BEGIN OF FUNCTIONS .........................
 */

/*
  ============================================================================

        void fill_psophometric_8khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients

        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: 
        ~~~~~~~
        Filter coefficients: 
          Paolo Usai
          CSELT
          Audio Coding and Transm.Quality  Tel:    +39-11-228-5697
          Via G.Reiss Romoli, 274	   Fax:    +39-11-228-6207
          10148 Torino			   E-mail: Paolino.Usai@cselt.stet.it
          ITALY
          Copyright Notice:
          " (C) 1994 CSELT - Centro Studi E Laboratori Telecomunicazioni SpA "
          " All rights reserved "
          
        Routine:
          Simao Ferraz de Campos Neto
          DDS/Pr11                      Tel: +55-192-39-1396
          CPqD/Telebras                 Fax: +55-192-53-4754
          13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>

        History:
        ~~~~~~~~
        10.Apr.1994 v1.0 Release of 1st version <simao@cpqd.ansp.br>
        28.Mar.2000 v1.1 Added type cast to float to avoid compilation 
                         warnings with MS Visual C compiler <simao>

 ============================================================================
*/
#define PSOPHOMETRIC_8K_LEN 156

void fill_psophometric_8khz (h0, lenh0)
float **h0;
long *lenh0;
{
  static float psophometric_8khz_coeff[PSOPHOMETRIC_8K_LEN] =  {
    (F) 0.445651, (F)-0.067161, (F)-0.399779, (F)-0.204052, (F)-0.116012, (F) 0.037295,
    (F) 0.087390, (F) 0.104088, (F) 0.077870, (F) 0.053150, (F) 0.023256, (F) 0.008227,
    (F) 0.000316, (F) 0.001940, (F) 0.004631, (F)-0.000713, (F)-0.003866, (F)-0.007453,
    (F)-0.011206, (F)-0.010320, (F)-0.011344, (F)-0.005345, (F)-0.003364, (F)-0.001533,
    (F)-0.003264, (F)-0.002365, (F)-0.002630, (F)-0.001813, (F)-0.001708, (F)-0.000045,
    (F) 0.001985, (F) 0.001365, (F) 0.000407, (F) 0.000064, (F)-0.000007, (F) 0.001172,
    (F) 0.000454, (F) 0.001754, (F) 0.001959, (F) 0.002148, (F) 0.000446, (F) 0.000080,
    (F)-0.000131, (F) 0.000325, (F)-0.000118, (F)-0.000141, (F) 0.000667, (F) 0.000480,
    (F)-0.000450, (F)-0.001116, (F)-0.001299, (F)-0.000208, (F)-0.000598, (F)-0.000241,
    (F)-0.000117, (F) 0.000588, (F)-0.000117, (F)-0.000450, (F)-0.000475, (F) 0.000493,
    (F) 0.000698, (F) 0.000337, (F) 0.000496, (F) 0.000613, (F) 0.000290, (F)-0.000152,
    (F)-0.000521, (F) 0.000581, (F) 0.000635, (F) 0.000593, (F)-0.000024, (F) 0.000294,
    (F)-0.000100, (F)-0.000460, (F)-0.000922, (F)-0.000259, (F) 0.000140, (F)-0.000472,
    (F) 0.000588, (F) 0.001611, (F) 0.000140, (F)-0.001721, (F)-0.002363, (F)-0.002238,
    (F)-0.001453, (F)-0.000586, (F) 0.000137, (F) 0.000576, (F) 0.000804, (F) 0.000850,
    (F) 0.000819, (F) 0.000732, (F) 0.000682, (F) 0.000600, (F) 0.000512, (F) 0.000382,
    (F) 0.000261, (F) 0.000143, (F) 0.000051, (F)-0.000028, (F)-0.000069, (F)-0.000092,
    (F)-0.000104, (F)-0.000129, (F)-0.000138, (F)-0.000142, (F)-0.000129, (F)-0.000125,
    (F)-0.000104, (F)-0.000084, (F)-0.000060, (F)-0.000051, (F)-0.000042, (F)-0.000035,
    (F)-0.000014, (F)-0.000002, (F) 0.000013, (F) 0.000024, (F) 0.000033, (F) 0.000032,
    (F) 0.000026, (F) 0.000023, (F) 0.000023, (F) 0.000018, (F) 0.000018, (F) 0.000011,
    (F) 0.000012, (F) 0.000003, (F)-0.000005, (F)-0.000011, (F)-0.000010, (F)-0.000008,
    (F)-0.000010, (F)-0.000010, (F)-0.000006, (F)-0.000003, (F)-0.000001, (F) 0.000000,
    (F) 0.000007, (F) 0.000013, (F) 0.000014, (F) 0.000014, (F) 0.000018, (F) 0.000018,
    (F) 0.000014, (F) 0.000015, (F) 0.000016, (F) 0.000016, (F) 0.000013, (F) 0.000012,
    (F) 0.000010, (F) 0.000005, (F)-0.000003, (F)-0.000008, (F)-0.000011, (F)-0.000011
  };

  *lenh0 = PSOPHOMETRIC_8K_LEN;/* store 'number of coefficients' */
  *h0 = psophometric_8khz_coeff;		/* store pointer to []-array */
}
#undef PSOPHOMETRIC_8K_LEN
/* ................... End of fill_psophometric_8khz() ................... */


/*
  ============================================================================

        SCD_FIR *psophometric_8khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for psophometric weighting filter for data 
        sampled at 16 kHz.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <simao@cpqd.ansp.br>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <simao>

 ============================================================================
*/
SCD_FIR *psophometric_8khz_init ()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_psophometric_8khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* .................... End of psophometric_8khz_init() .................... */

/* *************************** END OF FIR-PSO.C *************************** */
