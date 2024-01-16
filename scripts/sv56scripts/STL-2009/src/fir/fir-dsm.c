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
       CODING STANDARDS".
       =============================================================

MODULE:         FIRFLT, HIGH QUALITY FIR UP/DOWN-SAMPLING FILTER
                Sub-unit: Delta-SM filtering characteristic for 
		          far-field-to-near-field speech characteristics
			  conversion. To be used when adding backgound 
			  noise to speech.

ORIGINAL BY:
          Simao Ferraz de Campos Neto
	  Comsat Laboratories                Tel:    +1-301-428-4516
	  22300 Comsat Drive                 Fax:    +1-301-428-9287
	  Clarksburg MD 20871 - USA          E-mail: simao@ctd.comsat.com

          Robin Cross
          Acoustics and Vibration Laboratory Tel: +44-473-642956
          BT Laboratories - Rm.27 Bldg.B48   Fax: +44-473-649866
          Martlesham Heath, Ipswich	     Email: cross_r_c@bt-web.bt.co.uk
          Suffolk, IP5 7RE - United Kingdom

DESCRIPTION:
        This file contains procedures for applying the Delta-SM weighting 
	to 16kHz sampled speech. No rate conversion is performed.

FUNCTIONS:
  Global (have prototype in firflt.h)
         = delta_sm_16khz_init(): init coeffs, gain, rate cvt. for Delta-SM

  Local (should be used only here -- prototypes only in this file)
         = fill_delta_sm_16khz(...): init coeffs for Delta-SM filter


HISTORY:
    16.Dec.1991 v0.1 First beta-version <hf@pkinbg.uucp>
    28.Feb.1992 v1.0 Release of 1st version to UGST <hf@pkinbg.uucp>
    20.Apr.1994 v2.0 Added new filtering routines: modified IRS at 16kHz and
                     48kHz, Delta-SM, Linear-phase band-pass.
                     <simao@cpqd.ansp.br>
    30.Sep.1994 v2.1 Updated to accomodate changes in the name of the name and
                     slitting of module in several files, for ease of expansion.
    28.Mar.2000 v2.2 Added type casting to float in fill_delta_sm_16khz() to 
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


/*
 * ......... Local function prototypes .........
 */
void fill_delta_sm_16khz ARGS((float **h0, long *lenh0));


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

        void fill_delta_sm_16khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        The following are the coefficients for the 207 tap DSM filter. 
        The gain needed is 2.7dB, but I think it should be checked. e.g. 
        a sinusoid just below 1000Hz should be 0dB from in to out (see 
        the upper curve on page 124 of the Handbook on Telephonometry 
        Geneva 1987). [Comments by Robin Cross]

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
          Robin Cross
          Acoustics and Vibration Laboratory Tel: +44-473-642956
          BT Laboratories - Rm.27 Bldg.B48   Fax: +44-473-649866
          Martlesham Heath, Ipswich	     Email: <cross_r_c@bt-web.bt.co.uk>
          Suffolk, IP5 7RE
          United Kingdom
          
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
#define DELTA_SM_LEN 207

#define F float
void            fill_delta_sm_16khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{
  static float    dsm_fil_coef[DELTA_SM_LEN] =
  {
    (F)-2.69880325793689E-002, (F)9.76699781947926E-004,  (F)3.73016531198107E-004,
    (F)2.36061209990078E-004,  (F)5.60162996216139E-006,  (F)-9.03137877180616E-005,
    (F)-5.24176083216893E-005, (F)-2.53542270960832E-004, (F)-5.41713941354720E-004,
    (F)-7.55423766058210E-004, (F)-8.67675033272794E-004, (F)-8.53658346869226E-004,
    (F)-7.15408802672506E-004, (F)-5.54895680169466E-004, (F)-4.97482641711827E-004,
    (F)-4.48553262341146E-004, (F)-2.79409101149253E-004, (F)-5.19695177863672E-005,
    (F)1.99771617048017E-004,  (F)4.78375113418250E-004,  (F)6.86483053608730E-004,
    (F)8.80656033466992E-004,  (F)1.10582670272701E-003,  (F)1.34375020583181E-003,
    (F)1.61921608174516E-003,  (F)1.84612735647147E-003,  (F)1.97946476877689E-003,
    (F)1.93656760518496E-003,  (F)1.76230904270421E-003,  (F)1.60885448626217E-003,
    (F)1.40261436746897E-003,  (F)1.19651473235190E-003,  (F)1.07049214861627E-003,
    (F)8.08352845380886E-004,  (F)4.19405162312910E-004,  (F)1.50965878042073E-004,
    (F)7.51792228051498E-006,  (F)2.19621770164560E-006,  (F)4.81460539050409E-005,
    (F)-1.92054739650773E-005, (F)-1.73141861459014E-004, (F)-3.96846124978692E-004,
    (F)-3.73471061585074E-004, (F)-1.52619474213404E-004, (F)2.63442996474272E-004,
    (F)8.71743233151986E-004,  (F)1.28848831479795E-003,  (F)1.50947450623765E-003,
    (F)1.82678852795534E-003,  (F)2.02173224397282E-003,  (F)2.08625432328207E-003,
    (F)2.27705046087903E-003,  (F)2.33954806560429E-003,  (F)2.21046393112245E-003,
    (F)1.98686438827823E-003,  (F)1.70205852832272E-003,  (F)1.50980359223393E-003,
    (F)1.24216128628559E-003,  (F)7.79382700158117E-004,  (F)-1.31074758488880E-004,
    (F)-1.36307996310321E-003, (F)-2.30215984191042E-003, (F)-3.10762956017788E-003,
    (F)-3.64138674279795E-003, (F)-3.58960251637397E-003, (F)-3.87566324525222E-003,
    (F)-4.70519008462187E-003, (F)-5.16315519015880E-003, (F)-5.28365079601592E-003,
    (F)-5.11447151039184E-003, (F)-4.82052612725036E-003, (F)-4.86405940768248E-003,
    (F)-4.98414841743199E-003, (F)-4.92919314382592E-003, (F)-4.09218298472986E-003,
    (F)-2.67640746215187E-003, (F)-9.57169836955837E-004, (F)1.24637220221356E-003,
    (F)1.99848129623332E-003,  (F)9.33658189813553E-004,  (F)-3.15777788578298E-004,
    (F)-2.72402625563398E-003, (F)-5.62851552997196E-003, (F)-6.19723020007827E-003,
    (F)-5.99224680524037E-003, (F)-6.62684508618650E-003, (F)-8.15167176304365E-003,
    (F)-1.01372391888131E-002, (F)-1.04208458084723E-002, (F)-1.02956557902670E-002,
    (F)-1.21727812936274E-002, (F)-2.03361456714826E-002, (F)-3.30125070070071E-002,
    (F)-3.82954298216963E-002, (F)-3.70416885274482E-002, (F)-2.88705952443952E-002,
    (F)-5.52791056297395E-003, (F)8.27735355493049E-003,  (F)-3.98316979165397E-003,
    (F)-4.48874477419406E-003, (F)4.82721687735318E-003,  (F)1.29528980311788E-002,
    (F)7.43610439968511E-002,  (F)4.09324809954693E-001,  (F)7.43610439968511E-002,
    (F)1.29528980311788E-002,  (F)4.82721687735318E-003,  (F)-4.48874477419406E-003,
    (F)-3.98316979165397E-003, (F)8.27735355493049E-003,  (F)-5.52791056297395E-003,
    (F)-2.88705952443952E-002, (F)-3.70416885274482E-002, (F)-3.82954298216963E-002,
    (F)-3.30125070070071E-002, (F)-2.03361456714826E-002, (F)-1.21727812936274E-002,
    (F)-1.02956557902670E-002, (F)-1.04208458084723E-002, (F)-1.01372391888131E-002,
    (F)-8.15167176304365E-003, (F)-6.62684508618650E-003, (F)-5.99224680524037E-003,
    (F)-6.19723020007827E-003, (F)-5.62851552997196E-003, (F)-2.72402625563398E-003,
    (F)-3.15777788578298E-004, (F)9.33658189813553E-004,  (F)1.99848129623332E-003,
    (F)1.24637220221356E-003,  (F)-9.57169836955837E-004, (F)-2.67640746215187E-003,
    (F)-4.09218298472986E-003, (F)-4.92919314382592E-003, (F)-4.98414841743199E-003,
    (F)-4.86405940768248E-003, (F)-4.82052612725036E-003, (F)-5.11447151039184E-003,
    (F)-5.28365079601592E-003, (F)-5.16315519015880E-003, (F)-4.70519008462187E-003,
    (F)-3.87566324525222E-003, (F)-3.58960251637397E-003, (F)-3.64138674279795E-003,
    (F)-3.10762956017788E-003, (F)-2.30215984191042E-003, (F)-1.36307996310321E-003,
    (F)-1.31074758488880E-004, (F)7.79382700158117E-004,  (F)1.24216128628559E-003,
    (F)1.50980359223393E-003,  (F)1.70205852832272E-003,  (F)1.98686438827823E-003,
    (F)2.21046393112245E-003,  (F)2.33954806560429E-003,  (F)2.27705046087903E-003,
    (F)2.08625432328207E-003,  (F)2.02173224397282E-003,  (F)1.82678852795534E-003,
    (F)1.50947450623765E-003,  (F)1.28848831479795E-003,  (F)8.71743233151986E-004,
    (F)2.63442996474272E-004,  (F)-1.52619474213404E-004, (F)-3.73471061585074E-004,
    (F)-3.96846124978692E-004, (F)-1.73141861459014E-004, (F)-1.92054739650773E-005,
    (F)4.81460539050409E-005,  (F)2.19621770164560E-006,  (F)7.51792228051498E-006,
    (F)1.50965878042073E-004,  (F)4.19405162312910E-004,  (F)8.08352845380886E-004,
    (F)1.07049214861627E-003,  (F)1.19651473235190E-003,  (F)1.40261436746897E-003,
    (F)1.60885448626217E-003,  (F)1.76230904270421E-003,  (F)1.93656760518496E-003,
    (F)1.97946476877689E-003,  (F)1.84612735647147E-003,  (F)1.61921608174516E-003,
    (F)1.34375020583181E-003,  (F)1.10582670272701E-003,  (F)8.80656033466992E-004,
    (F)6.86483053608730E-004,  (F)4.78375113418250E-004,  (F)1.99771617048017E-004,
    (F)-5.19695177863672E-005, (F)-2.79409101149253E-004, (F)-4.48553262341146E-004,
    (F)-4.97482641711827E-004, (F)-5.54895680169466E-004, (F)-7.15408802672506E-004,
    (F)-8.53658346869226E-004, (F)-8.67675033272794E-004, (F)-7.55423766058210E-004,
    (F)-5.41713941354720E-004, (F)-2.53542270960832E-004, (F)-5.24176083216893E-005,
    (F)-9.03137877180616E-005, (F)5.60162996216139E-006,  (F)2.36061209990078E-004,
    (F)3.73016531198107E-004,  (F)9.76699781947926E-004,  (F)-2.69880325793689E-002
  };

  *lenh0 = DELTA_SM_LEN;	/* store 'number of coefficients' */
  *h0 = dsm_fil_coef;		/* store pointer to []-array */
}
#undef DELTA_SM_LEN
/* .................... End of fill_delta_sm_16khz() .................... */


/*
  ============================================================================

        SCD_FIR *delta_sm_16khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Initialization routine for the delta-SM filter characteristic for 
        data sampled at 16 kHz.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <simao@cpqd.ansp.br>
        ~~~~~~~

        History:
        ~~~~~~~~
        20.Apr.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
SCD_FIR        *delta_sm_16khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_delta_sm_16khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ..................... End of delta_sm_16khz_init() ..................... */


/* *************************** END OF FIR-DSM.C *************************** */
