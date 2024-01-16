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
                Sub-unit: Half-tilt IRS filtering within P.48 send mask

ORIGINAL BY:
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

DESCRIPTION:
        This file contains procedure for IRS send part filtering for
	the 16kHz sampling rate. Up and down sampling has not been made 
	part of this sub-unit. 

FUNCTIONS:
  Global (have prototype in firflt.h)
         = ht_irs_16khz_init()  :  initialize half-tilt IRS filter @ 16kHz

  Local (should be used only here -- prototypes only in this file)
         = fill_ht_irs16khz(...) : init filter coeffs for half-tilt IRS @ 16kHz

HISTORY:
    12.Oct.1994 v1.0 Created <simao@ctd.comsat.com>.
    28.Mar.2000 v1.1 Added type casting to float in fill_ht_irs16khz() to 
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
void fill_ht_irs16khz ARGS((float **h0, long *lenh0));


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

        SCD_FIR *ht_irs_16khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for IRS weighting filter for data sampled
        at 16 kHz with a half-tilt inclination within the P.48 mask.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Original Author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

        History:
        ~~~~~~~~
        12.Oct.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
SCD_FIR        *ht_irs_16khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_ht_irs16khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.08,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ..................... End of ht_irs_16khz_init() ..................... */


/*
  ============================================================================

        void fill_ht_irs16khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with floating point FIR coefficients 
	for IRS send part filter for speech sampled at 16 kHz with a 
	half-tilt inclination within the P.48 sending mask.

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients

        Return value:
        ~~~~~~~~~~~~~
        None.

        Original Author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

        History:
        ~~~~~~~~
        12.Oct.1994 v1.0 Release of 1st version <simao>
        28.Mar.2000 v1.1 Added type cast to float to avoid compilation 
                         warnings with MS Visual C compiler <simao>

 ============================================================================
*/
#define lenh0_HT_IRS16 200

void            fill_ht_irs16khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{
  static float    h0_HT_IRS16[lenh0_HT_IRS16] = {
    (F) 6.48926390136748E-04,  (F) 5.08374974537000E-04,  (F)-1.93741975090558E-03, 
    (F)-4.32906858044298E-03,  (F)-3.07160188232933E-03,  (F) 4.07292104955264E-04, 
    (F) 1.65823167544808E-03,  (F) 1.84627757467645E-04,  (F)-6.98491018003273E-04, 
    (F) 2.93671432395676E-04,  (F) 1.29895857659601E-03,  (F) 9.85530592932959E-04, 
    (F) 1.28412444450720E-05,  (F)-2.68239465467643E-04,  (F) 6.23250695474507E-04, 
    (F) 1.45903311532251E-03,  (F) 8.28128689658387E-04,  (F)-3.82648486845963E-04, 
    (F)-1.51274635413981E-04,  (F) 1.22475032666453E-03,  (F) 1.49679653573058E-03, 
    (F) 1.83030468467793E-04,  (F)-6.94793562606839E-04,  (F) 1.96991788880095E-04, 
    (F) 1.47941687349052E-03,  (F) 1.07191910980744E-03,  (F)-6.00606295828205E-04, 
    (F)-9.96309944874409E-04,  (F) 5.87957389664349E-04,  (F) 1.61310240551938E-03, 
    (F) 2.46312244042134E-04,  (F)-1.49099518920482E-03,  (F)-8.25871406365821E-04, 
    (F) 1.23921422343711E-03,  (F) 1.37017689528132E-03,  (F)-7.89870858252723E-04, 
    (F)-1.85500138025001E-03,  (F) 6.09513843543533E-06,  (F) 1.91632941594184E-03, 
    (F) 7.31477114761637E-04,  (F)-1.73050023438375E-03,  (F)-1.41874373744582E-03, 
    (F) 1.32624454174905E-03,  (F) 2.15725404773305E-03,  (F)-2.68785705896557E-04, 
    (F)-2.01762072421080E-03,  (F)-1.61536962526210E-04,  (F) 2.45447169715076E-03, 
    (F) 1.70004940206756E-03,  (F)-1.18538751279461E-03,  (F)-1.53701744684482E-03, 
    (F) 1.24807152129252E-03,  (F) 2.75498855887683E-03,  (F) 6.26880221412733E-04, 
    (F)-1.76908829000979E-03,  (F)-7.46282716211742E-04,  (F) 2.01070727842972E-03, 
    (F) 2.23954280271022E-03,  (F)-3.64463845247026E-04,  (F)-1.86200539609239E-03, 
    (F)-7.20518341140038E-05,  (F) 2.31257169346809E-03,  (F) 1.77288407332229E-03, 
    (F)-9.86927300202547E-04,  (F)-2.05416265779151E-03,  (F) 2.35961912775498E-04, 
    (F) 2.77797367874245E-03,  (F) 1.72865401483635E-03,  (F)-1.85181770038746E-03, 
    (F)-2.73930144121175E-03,  (F) 9.93157603413731E-04,  (F) 4.04125805634181E-03, 
    (F) 9.67964466020143E-04,  (F)-4.56946360514922E-03,  (F)-3.85271947019125E-03, 
    (F) 2.95416340725047E-03,  (F) 4.85673979256079E-03,  (F)-3.18414254189099E-03, 
    (F)-9.89096935324451E-03,  (F)-3.92410709098466E-03,  (F) 5.87951535298261E-03, 
    (F) 1.72696515775198E-03,  (F)-1.32913805254163E-02,  (F)-1.60850533705738E-02, 
    (F)-6.03422809814603E-04,  (F) 7.47331665499820E-03,  (F)-1.01150890721100E-02, 
    (F)-2.94983622700486E-02,  (F)-1.81037308316360E-02,  (F) 8.33210007317934E-03, 
    (F) 2.40801819645080E-03,  (F)-3.68067589352497E-02,  (F)-4.93613296015322E-02, 
    (F)-6.32008351138828E-03,  (F) 2.64708948779254E-02,  (F)-2.55667904031717E-02, 
    (F)-1.12672370244213E-01,  (F)-9.04911237929400E-02,  (F) 8.77094780592204E-02, 
    (F) 2.71732906586285E-01,  (F) 2.71732906586285E-01,  (F) 8.77094780592204E-02, 
    (F)-9.04911237929400E-02,  (F)-1.12672370244213E-01,  (F)-2.55667904031717E-02, 
    (F) 2.64708948779254E-02,  (F)-6.32008351138828E-03,  (F)-4.93613296015322E-02, 
    (F)-3.68067589352497E-02,  (F) 2.40801819645080E-03,  (F) 8.33210007317934E-03, 
    (F)-1.81037308316360E-02,  (F)-2.94983622700486E-02,  (F)-1.01150890721100E-02, 
    (F) 7.47331665499820E-03,  (F)-6.03422809814603E-04,  (F)-1.60850533705738E-02, 
    (F)-1.32913805254163E-02,  (F) 1.72696515775198E-03,  (F) 5.87951535298261E-03, 
    (F)-3.92410709098466E-03,  (F)-9.89096935324451E-03,  (F)-3.18414254189099E-03, 
    (F) 4.85673979256079E-03,  (F) 2.95416340725047E-03,  (F)-3.85271947019125E-03, 
    (F)-4.56946360514922E-03,  (F) 9.67964466020143E-04,  (F) 4.04125805634181E-03, 
    (F) 9.93157603413731E-04,  (F)-2.73930144121175E-03,  (F)-1.85181770038746E-03, 
    (F) 1.72865401483635E-03,  (F) 2.77797367874245E-03,  (F) 2.35961912775498E-04, 
    (F)-2.05416265779151E-03,  (F)-9.86927300202547E-04,  (F) 1.77288407332229E-03, 
    (F) 2.31257169346809E-03,  (F)-7.20518341140038E-05,  (F)-1.86200539609239E-03, 
    (F)-3.64463845247026E-04,  (F) 2.23954280271022E-03,  (F) 2.01070727842972E-03, 
    (F)-7.46282716211742E-04,  (F)-1.76908829000979E-03,  (F) 6.26880221412733E-04, 
    (F) 2.75498855887683E-03,  (F) 1.24807152129252E-03,  (F)-1.53701744684482E-03, 
    (F)-1.18538751279461E-03,  (F) 1.70004940206756E-03,  (F) 2.45447169715076E-03, 
    (F)-1.61536962526210E-04,  (F)-2.01762072421080E-03,  (F)-2.68785705896557E-04, 
    (F) 2.15725404773305E-03,  (F) 1.32624454174905E-03,  (F)-1.41874373744582E-03, 
    (F)-1.73050023438375E-03,  (F) 7.31477114761637E-04,  (F) 1.91632941594184E-03, 
    (F) 6.09513843543533E-06,  (F)-1.85500138025001E-03,  (F)-7.89870858252723E-04, 
    (F) 1.37017689528132E-03,  (F) 1.23921422343711E-03,  (F)-8.25871406365821E-04, 
    (F)-1.49099518920482E-03,  (F) 2.46312244042134E-04,  (F) 1.61310240551938E-03, 
    (F) 5.87957389664349E-04,  (F)-9.96309944874409E-04,  (F)-6.00606295828205E-04, 
    (F) 1.07191910980744E-03,  (F) 1.47941687349052E-03,  (F) 1.96991788880095E-04, 
    (F)-6.94793562606839E-04,  (F) 1.83030468467793E-04,  (F) 1.49679653573058E-03, 
    (F) 1.22475032666453E-03,  (F)-1.51274635413981E-04,  (F)-3.82648486845963E-04, 
    (F) 8.28128689658387E-04,  (F) 1.45903311532251E-03,  (F) 6.23250695474507E-04, 
    (F)-2.68239465467643E-04,  (F) 1.28412444450720E-05,  (F) 9.85530592932959E-04, 
    (F) 1.29895857659601E-03,  (F) 2.93671432395676E-04,  (F)-6.98491018003273E-04, 
    (F) 1.84627757467645E-04,  (F) 1.65823167544808E-03,  (F) 4.07292104955264E-04, 
    (F)-3.07160188232933E-03,  (F)-4.32906858044298E-03,  (F)-1.93741975090558E-03, 
    (F) 5.08374974537000E-04,  (F) 6.48926390136748E-04};


  *lenh0 = lenh0_HT_IRS16;		/* store 'number of coefficients' */
  *h0 = h0_HT_IRS16;                    /* store pointer to h02[]-array */
}
#undef lenh0_HT_IRS16
/* ...................... End of fill_ht_irs16khz() ..................... */


/* *************************** END OF FIR-HIRS.C *************************** */
