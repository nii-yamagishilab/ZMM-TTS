/*		This is a new file called "fir-msin.c"   v1.0 27.3.1998 (KAF)
                      based on/used to be "fir-flat.c"   v2.1 30.SEP.94 18H00
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
       it read in the distribution  disk, or in ITU-T Recommendation
       G.191   on  "SOFTWARE  TOOLS  FOR  SPEECH  AND  AUDIO  CODING 
       STANDARDS".
       =============================================================


ORIGINAL BY:

   Simao Ferraz de Campos Neto (Integration in the STL)
   Comsat Laboratories                  Tel:    +1-301-428-4516
   22300 Comsat Drive                   Fax:    +1-301-428-9287
   Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

   Kyrill Fisher (Filter design, original C code)
   Deutshe Telekom/Berkom
   k.fischer@berkom.de

DESCRIPTION:

   This file contains the coefficients of an fir-filter that simulates
   the transfer function of background noise for a mobile
   station. MSIN stands for "mobile station in". The purpose of this
   filter is to effectively suppress low-frequency components
   (i.e. ca. 50Hz...300Hz) of background noise. The transfer function
   has been defined based on measurements by BT. The transfer function
   is valid to roughly describe the spectral modifications of
   background noise; they do not, however, apply to speech input,
   which is much closer to the MS's microphone. However, at the ETSI
   SMG11 # 8 Plenary (Jan. 1999; Helsinki, Finland) it has been
   discussed that this filter may also be applied to the speech
   signal.

   This module is intended to be used in combination with the program
   "filter.c" provided by ITU. The new filter defined here has to be
   made available from "filter.c". The filter-option to be specified
   when calling "filter.c" has been named "msin"/"MSIN" in this
   implementation.  This unique name should be added to the list of
   legal filter type descriptors (Flt_type/F_type in filter.c).

   Then, the initialization of the msin 1:1-filter should be done in
   this way:

filter.c:
    ----snip snip --- part of "filter.c" starts here:
    selection of filter to be used:
     *
     * Filter type: msin - Linear-phase highpass
     * added by KAF 27Mar98
     *
    else if ((strncmp(F_type, "msin", 4) == 0 || 
              strncmp(F_type, "msin", 4) == 0) && 
	      strlen(F_type)==4)               added by KAF 27Mar98 
    {
      if (upsample) { 
           HARAKIRI("Error: cannot upsample using msin filter!\n",20);
      }
      printf("MSIN: MSIN (mobile station input) Filter with SR=1:1\n");
      fir_state = msin_init();
    }    
    (...)
    ----snip snip --- part of "filter.c" ends here.

        The function msin_init() is provided in this file "fir-msin.c". 
        Now, the filter is initialized and will be used in the subsequent 
        call to hq_kernel(...) (which is already realized in "filter.c").

	In addition to this change, the prototype of msin_init(...) has to be 
        added to "firflt.h" (just on line):          

firflt.h:
    ----snip snip --- part of "firflt.h" starts here:
    SCD_FIR *msin_init ARGS((void)); 
    ----snip snip --- part of "firflt.h" ends here.

        This completes the adaptions to be made to include the msin 1:1 
        filter. 


    ================================= Mar. 1998; KAF == <k.fischer@berkom.de>

FUNCTIONS:
  Global (have prototype in firflt.h)
	= msin_init(): initialize msin-filter 1:1


  Local (should be used only here -- prototypes only in this file)

	  fill_msin_filter(...) :  filling filter coeffs into msin 
				    filter, 
				    sampling rate change factor 1:1


HISTORY:
    27.Mar.98 v1.0 New Module "fir-msin.c" defined for 
                   msin filtering at 16kHz sampling frequency, 
                   no change of sampling frequency. KAF
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

void fill_msin_16khz ARGS((float *h0[], long *lenh0));


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

  void fill_msin_16khz (float **h0, long *lenh0);
  ~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to array with of FIR coefficients of a GSM mobile
  station input filtering characteristic (MSIN) without downsampling.

  A summary of the filter characteristics is as follows:
  - high pass filter
  - linear phase
  - 185 coefficients
  - cuttoff frequency: ca. 200Hz (@16kHz) 
  - passband is flat above ca. 400Hz (@16kHz)

  @16kHz:
  - 200Hz: ca. - 3dB attenuation
  - 100Hz: ca. -12dB attenuation
  -  50Hz: ca. -30dB attenuation

   
  Parameters:
  ~~~~~~~~~~~
  h0:    (Out) pointer to array with FIR coefficients
  lenh0: (Out) pointer to number of coefficients


  Return value:
  ~~~~~~~~~~~~~
  None.

  Author: <k.fischer@berkom.de>
  ~~~~~~~

  History:
  ~~~~~~~~
  27.Feb.98 v1.0 Created. 
 ============================================================================
*/

#define msin_LEN 185
#define lenh02 msin_LEN

void            fill_msin_16khz (h0, lenh0)
float          *h0[];
long           *lenh0;
{
  static float msin_coef[msin_LEN] =
  {
        0./32768,    0./32768,     1./32768,    1./32768,    1./32768,
        1./32768,    2./32768,     2./32768,    3./32768,    3./32768,
        4./32768,    4./32768,     5./32768,    6./32768,    7./32768,
        7./32768,    8./32768,     9./32768,   10./32768,   11./32768,
       12./32768,   13./32768,    14./32768,   15./32768,   16./32768,
       16./32768,   17./32768,    17./32768,   18./32768,   18./32768,
       18./32768,   18./32768,    17./32768,   16./32768,   15./32768,
       14./32768,   12./32768,     9./32768,    7./32768,    3./32768,
       -1./32768,   -5./32768,   -10./32768,  -15./32768,  -21./32768,
      -28./32768,  -35./32768,   -43./32768,  -52./32768,  -62./32768,
      -72./32768,  -82./32768,   -94./32768, -106./32768, -119./32768,
     -133./32768, -147./32768,  -161./32768, -177./32768, -193./32768,
     -209./32768, -226./32768,  -243./32768, -261./32768, -279./32768,
     -297./32768, -315./32768,  -334./32768, -353./32768, -371./32768,
     -390./32768, -408./32768,  -426./32768, -444./32768, -462./32768,
     -479./32768, -495./32768,  -511./32768, -527./32768, -541./32768,
     -555./32768, -568./32768,  -580./32768, -591./32768, -601./32768,
     -610./32768, -618./32768,  -625./32768, -630./32768, -635./32768,
     -638./32768, -640./32768, 32128./32768, -640./32768, -638./32768,
     -635./32768, -630./32768,  -625./32768, -618./32768, -610./32768,
     -601./32768, -591./32768,  -580./32768, -568./32768, -555./32768,
     -541./32768, -527./32768,  -511./32768, -495./32768, -479./32768,
     -462./32768, -444./32768,  -426./32768, -408./32768, -390./32768,
     -371./32768, -353./32768,  -334./32768, -315./32768, -297./32768,
     -279./32768, -261./32768,  -243./32768, -226./32768, -209./32768,
     -193./32768, -177./32768,  -161./32768, -147./32768, -133./32768,
     -119./32768, -106./32768,   -94./32768,  -82./32768,  -72./32768,
      -62./32768,  -52./32768,   -43./32768,  -35./32768,  -28./32768,
      -21./32768,  -15./32768,   -10./32768,   -5./32768,   -1./32768,
        3./32768,    7./32768,     9./32768,   12./32768,   14./32768,
       15./32768,   16./32768,    17./32768,   18./32768,   18./32768,
       18./32768,   18./32768,    17./32768,   17./32768,   16./32768,
       16./32768,   15./32768,    14./32768,   13./32768,   12./32768,
       11./32768,   10./32768,     9./32768,    8./32768,    7./32768,
        7./32768,    6./32768,     5./32768,    4./32768,    4./32768,
        3./32768,    3./32768,     2./32768,    2./32768,    1./32768,
        1./32768,    1./32768,     1./32768,    0./32768,    0./32768 
  }; 

  *lenh0 = lenh02;		/* store 'number of coefficients' */
  *h0 = msin_coef;		/* store pointer to h02[]-array */
}
#undef lenh02
#undef msin_LEN
/* ................... End of fill_msin_16khz() ................... */


/*
  ============================================================================

  SCD_FIR *msin_16khz_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialization routine for GSM mobile station input signal
  filtering characteristic (MSIN) with factor 1:1 (here defined
  as downsampling factor of 1).

  Parameters:  none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct SCD_FIR;

  Author: <k.fischer@berkom.de>
  ~~~~~~~

  History:
  ~~~~~~~~
  27.Feb.98 v1.0 Created new module msin (msin without dwnsmpl.) 
  ============================================================================
*/
SCD_FIR        *msin_16khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_msin_16khz (&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor 1, (no DwnSpl) */
		       'D'	/* In: switch to down-sampling kernel */
    );
}
/* ...................... End of msin_16khz_init() ....................... */


/* ************************** END OF FIR-MSIN.C ************************** */


