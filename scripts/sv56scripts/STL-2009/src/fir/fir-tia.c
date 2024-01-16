/*                                                            28.Mar.2000 v1.1 
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
       =============================================================

MODULE:         FIRFLT, HIGH QUALITY FIR UP/DOWN-SAMPLING FILTER
                Sub-unit: IRS filtering using the TIA Filter Coeefs

ORIGINAL BY:
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

DESCRIPTION:
        This file contains procedures for IRS send part filtering for
	several sampling rates. Up and down sampling has not been made 
	part of this sub-unit.

FUNCTIONS:
  Global (have prototype in firflt.h)
         = tia_irs_8khz_init()  :  initialize IRS weighting filter 8 kHz

  Local (should be used only here -- prototypes only in this file)
         = fill_tia_irs8khz(...) : init filter coeffs for TIA IRS @ 8 kHz

HISTORY:
    30.Sep.1994 v1.0 Created <simao@ctd.comsat.com>.
    28.Mar.2000 v1.1 Added type casting to float in fill_tia_irs8khz() to 
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

void fill_tia_irs8khz ARGS((float **h0, long *lenh0));


/* 
 * ..... Private function prototypes defined in other sub-unit ..... 
 */
extern SCD_FIR *fir_initialization ARGS((long lenh0, float h0[], double gain, 
                                                 long idwnup, int hswitch));


/*
 * ...................... BEGIN OF FUNCTIONS .........................
 */

/* ************************************************************************ */
/* ****** The functions that follow were added after the ITU-T STL92 ****** */
/* ************************************************************************ */

/*
  ============================================================================

        SCD_FIR *tia_irs_8khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for IRS weighting filter for data sampled
        at 8 kHz using the TIA coefficients.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <simao@ctd.comsat.com>
        ~~~~~~~

        History:
        ~~~~~~~~
        30.Sep.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
SCD_FIR        *tia_irs_8khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_tia_irs8khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ..................... End of tia_irs_8khz_init() ..................... */


/*
  ============================================================================

        void fill_tia_irs8khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with 24-bit FIR coefficients for IRS
        send part filter for speech sampled at 8 kHz.

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients

        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: <simao>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.1992 v1.0 Release of 1st version
        28.Mar.2000 v1.1 Added type cast to float to avoid compilation 
                         warnings with MS Visual C compiler <simao>

 ============================================================================
*/
#define lenh0_TIAIRS8 151

void            fill_tia_irs8khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{
  static float    h0IRS8[lenh0_TIAIRS8] = {
    (F)8.997699E-03, 	(F)-1.693536E-03, (F)-9.467031E-03, 	(F)2.947994E-03, 
    (F)-2.947994E-03, 	(F)-8.290875E-04, (F)7.650433E-04, 	(F)-1.117980E-03, 
    (F)5.334970E-04, 	(F)-2.923005E-04, (F)4.368893E-04, 	(F)5.379950E-05, 
    (F)2.410283E-04, 	(F)7.318580E-05,  (F)4.912692E-04, 	(F)1.289220E-04, 
    (F)2.303720E-04, 	(F)1.939132E-04,  (F)-3.239230E-05, 	(F)2.381581E-04, 
    (F)-2.241322E-04, 	(F)2.929805E-04,  (F)-3.475052E-04, 	(F)-2.468450E-05, 
    (F)-4.705100E-06, 	(F)-1.482234E-04, (F)2.496010E-05, 	(F)-2.108535E-04, 
    (F)9.002890E-05, 	(F)-3.390951E-04, (F)4.216802E-04, 	(F)4.306677E-04, 
    (F)1.094914E-04, 	(F)-1.725316E-04, (F)-1.871907E-04, 	(F)3.910654E-04, 
    (F)-9.310743E-04, 	(F)1.113172E-03,  (F)-9.112764E-04, 	(F)3.499053E-04, 
    (F)1.153966E-03, 	(F)-1.141615E-03, (F)2.240873E-03, 	(F)-1.178158E-03, 
    (F)1.615365E-03, 	(F)8.418237E-04,  (F)-4.042726E-04, 	(F)2.804421E-03, 
    (F)-1.945186E-03, 	(F)2.777955E-03,  (F)4.492770E-04, 	(F)-1.682586E-04, 
    (F)2.395237E-03, 	(F)-3.712968E-04, (F)2.288824E-03, 	(F)3.645405E-04, 
    (F)2.641730E-03, 	(F)-5.982397E-04, (F)2.341659E-03, 	(F)-1.883668E-03, 
    (F)2.388487E-03, 	(F)-2.585317E-03, (F)-4.586401E-03, 	(F)3.212217E-03, 
    (F)-1.999986E-02, 	(F)1.222732E-02,  (F)-2.436289E-02, 	(F)-6.538092E-03, 
    (F)-1.005630E-02, 	(F)-3.625567E-02, (F)2.728618E-02, 	(F)-1.060980E-01, 
    (F)8.686430E-02, 	(F)-2.326763E-01, (F)-7.264613E-02, 	(F)7.426945E-01, 
    (F)-7.264613E-02, 	(F)-2.326763E-01, (F)8.686430E-02, 	(F)-1.060980E-01, 
    (F)2.728618E-02, 	(F)-3.625567E-02, (F)-1.005630E-02, 	(F)-6.538092E-03, 
    (F)-2.436289E-02, 	(F)1.222732E-02,  (F)-1.999986E-02, 	(F)3.212217E-03, 
    (F)-4.586401E-03, 	(F)-2.585317E-03, (F)2.388487E-03, 	(F)-1.883668E-03, 
    (F)2.341659E-03, 	(F)-5.982397E-04, (F)2.641730E-03, 	(F)3.645405E-04, 
    (F)2.288824E-03, 	(F)-3.712968E-04, (F)2.395237E-03, 	(F)-1.682586E-04, 
    (F)4.492770E-04, 	(F)2.777955E-03,  (F)-1.945186E-03, 	(F)2.804421E-03, 
    (F)-4.042726E-04, 	(F)8.418237E-04,  (F)1.615365E-03, 	(F)-1.178158E-03, 
    (F)2.240873E-03, 	(F)-1.141615E-03, (F)1.153966E-03, 	(F)3.499053E-04, 
    (F)-9.112764E-04, 	(F)1.113172E-03,  (F)-9.310743E-04, 	(F)3.910654E-04, 
    (F)-1.871907E-04, 	(F)-1.725316E-04, (F)1.094914E-04, 	(F)4.306677E-04, 
    (F)4.216802E-04, 	(F)-3.390951E-04, (F)9.002890E-05, 	(F)-2.108535E-04, 
    (F)2.496010E-05, 	(F)-1.482234E-04, (F)-4.705100E-06, 	(F)-2.468450E-05, 
    (F)-3.475052E-04, 	(F)2.929805E-04,  (F)-2.241322E-04, 	(F)2.381581E-04, 
    (F)-3.239230E-05, 	(F)1.939132E-04,  (F)2.303720E-04, 	(F)1.289220E-04, 
    (F)4.912692E-04, 	(F)7.318580E-05,  (F)2.410283E-04, 	(F)5.379950E-05, 
    (F)4.368893E-04, 	(F)-2.923005E-04, (F)5.334970E-04, 	(F)-1.117980E-03, 
    (F)7.650433E-04, 	(F)-8.290875E-04, (F)-2.947994E-03, 	(F)2.947994E-03, 
    (F)-9.467031E-03, 	(F)-1.693536E-03, (F)8.997699E-03};


  *lenh0 = lenh0_TIAIRS8;		/* store 'number of coefficients' */
  *h0 = h0IRS8;		                /* store pointer to h02[]-array */
}
#undef lenh0_TIAIRS8
/* ...................... End of fill_tia_irs8khz() ..................... */


/* *************************** END OF FIR-TIA.C *************************** */
