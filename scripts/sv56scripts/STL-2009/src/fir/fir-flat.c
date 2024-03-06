/*                                                         v2.2 16.MAY.97 18H00
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
                Sub-unit: Flat-weighting filters, LP and BP

ORIGINAL BY:
 	Chris Tate <c.n.tate@bnr.co.uk>
	John Barnes <G.J.P.Barnes@bnr.co.uk>
	Rudolf Hofmann <hf@pkinbg.uucp>
        Simao Ferraz de Campos Neto <simao@ctd.comsat.com>

DESCRIPTION:
        This file contains procedures for flat-weighting low-pass and 
	high-pass filters, with rate factors of 2 and 3 for both up-
	and down-sampling.

FUNCTIONS:
  Global (have prototype in firflt.h)
         = hq_down_2_to_1_init() :  initialize down-sampling filter 2:1
         = hq_down_3_to_1_init() :  initialize down-sampling filter 3:1
         = hq_up_1_to_2_init()   :  initialize   up-sampling filter 1:2
         = hq_up_1_to_3_init()   :  initialize   up-sampling filter 1:3
         = linear_phase_pb_2_to_1_init()
         = linear_phase_pb_1_to_2_init()
         = linear_phase_pb_1_to_1_init()

  Local (should be used only here -- prototypes only in this file)
         = fill_lp_2_to_1(...)   : filling filter coefficients into
                                   array for flat low pass, factor 2:1
         = fill_lp_3_to_1(...)   : idem, for flat low pass, factor 3:1
         = fill_flat_band_pass(...): idem, for linear-phase band-pass 



HISTORY:
    16.Dec.91 v0.1 First beta-version <hf@pkinbg.uucp>
    28.Feb.92 v1.0 Release of 1st version to UGST <hf@pkinbg.uucp>
    20.Apr.94 v2.0 Added new filtering routines: modified IRS at 16kHz and
                   48kHz, Delta-SM, Linear-phase band-pass.
                   <simao@cpqd.ansp.br>
    30.Sep.94 v2.1 Updated to accomodate changes in the name of the name and
                   slitting of module in several files, for ease of expansion.
    ** THIS FILE WAS SPLIT FROM THE ORIGINAL .C FILE **
    16.May.97 v2.2 Added function for HQ flat bandpass keeping the sampling
                   rate (1:1 factor)
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

void fill_lp_2_to_1 ARGS((float *h0[], long *lenh0));
void fill_lp_3_to_1 ARGS((float **h0, long *lenh0));
void fill_flat_band_pass ARGS((float **h0, long *lenh0));


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

        SCD_FIR *hq_down_2_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality FIR downsampling filter
        by factor of 2.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR        *hq_down_2_to_1_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_2_to_1(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       2l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling kernel */
    );
}
/* ..................... End of hq_down_2_to_1_init() ..................... */



/*
  ============================================================================

        SCD_FIR *hq_up_1_to_2_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality upsampling FIR filter
        by factor of 2.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR        *hq_up_1_to_2_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_2_to_1(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       2.0,	/* In: gain factor for FIR-coeffic. */
		       2l,	/* In: Up-sampling factor */
		       'U'	/* In: Switch to upsampling procedure */
    );
}
/* ..................... End of hq_up_1_to_2_init() ..................... */



/*
  ============================================================================

        SCD_FIR *hq_down_3_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality FIR downsampling filter
        by factor of 3.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR        *hq_down_3_to_1_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_3_to_1(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       3l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );
}
/* .................... End of hq_down_3_to_1_init() ..................... */



/*
  ============================================================================

        SCD_FIR *hq_up_1_to_3_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for high quality FIR upsampling filter
        by factor of 3.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_FIR        *hq_up_1_to_3_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_lp_3_to_1(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       3.0,	/* In: gain factor for FIR-coeffic. */
		       3l,	/* In: Up-sampling factor */
		       'U'	/* In: switch to upsampling procedure */
    );
}
/* ...................... End of hq_up_1_to_3_init() ...................... */


/*
  ============================================================================

        void fill_lp_2_to_1 (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with of FIR coefficients of a low
        pass filter for up/down sampling filter with factor 2:1 or 1:2
        and 24-bit representation.

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients


        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
#define f24 (float)0x00800000
#define lenh02 118

void            fill_lp_2_to_1(h0, lenh0)
  float          *h0[];
  long           *lenh0;
{
  static float    h02[lenh02] = {
    1584. / f24, 805. / f24, -4192. / f24, -8985. / f24,
    -5987. / f24, 2583. / f24, 4657. / f24, -3035. / f24,
    -7004. / f24, 1542. / f24, 8969. / f24, 567. / f24,
    -10924. / f24, -3757. / f24, 12320. / f24, 7951. / f24,
    -12793. / f24, -13048. / f24, 11923. / f24, 18793. / f24,
    -9331. / f24, -24802. / f24, 4694. / f24, 30570. / f24,
    2233. / f24, -35439. / f24, -11526. / f24, 38680. / f24,
    23114. / f24, -39474. / f24, -36701. / f24, 36999. / f24,
    51797. / f24, -30419. / f24, -67658. / f24, 18962. / f24,
    83318. / f24, -1927. / f24, -97566. / f24, -21284. / f24,
    108971. / f24, 51215. / f24, -115837. / f24, -88430. / f24,
    116130. / f24, 133716. / f24, -107253. / f24, -188497. / f24,
    85497. / f24, 255795. / f24, -44643. / f24, -342699. / f24,
    -28185. / f24, 468096. / f24, 167799. / f24, -696809. / f24,
    -519818. / f24, 1446093. / f24, 3562497. / f24, 3562497. / f24,
    1446093. / f24, -519818. / f24, -696809. / f24, 167799. / f24,
    468096. / f24, -28185. / f24, -342699. / f24, -44643. / f24,
    255795. / f24, 85497. / f24, -188497. / f24, -107253. / f24,
    133716. / f24, 116130. / f24, -88430. / f24, -115837. / f24,
    51215. / f24, 108971. / f24, -21284. / f24, -97566. / f24,
    -1927. / f24, 83318. / f24, 18962. / f24, -67658. / f24,
    -30419. / f24, 51797. / f24, 36999. / f24, -36701. / f24,
    -39474. / f24, 23114. / f24, 38680. / f24, -11526. / f24,
    -35439. / f24, 2233. / f24, 30570. / f24, 4694. / f24,
    -24802. / f24, -9331. / f24, 18793. / f24, 11923. / f24,
    -13048. / f24, -12793. / f24, 7951. / f24, 12320. / f24,
    -3757. / f24, -10924. / f24, 567. / f24, 8969. / f24,
    1542. / f24, -7004. / f24, -3035. / f24, 4657. / f24,
    2583. / f24, -5987. / f24, -8985. / f24, -4192. / f24,
  805. / f24, 1584. / f24};


  *lenh0 = lenh02;		/* store 'number of coefficients' */
  *h0 = h02;			/* store pointer to h02[]-array */
}
#undef f24
#undef lenh02
/* ....................... End of fill_lp_2_to_1() ....................... */



/*
  ============================================================================

        void fill_lp_3_to_1 (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with FIR coefficients for up/down
        sampling low-pass filter with factor 3:1 or 1:3 and 24 bit
        representation.

        Parameters:
        ~~~~~~~~~~~
        h0:    (Out) pointer to array with FIR coefficients
        lenh0: (Out) pointer to number of coefficients


        Return value:
        ~~~~~~~~~~~~~
        None.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
#define f24 (float)0x00800000
#define lenh03 168

void            fill_lp_3_to_1(h0, lenh0)
  float         **h0;
  long           *lenh0;
{
  static float    h03[lenh03] = {
    877. / f24, 3745. / f24, 6479. / f24, 8447. / f24, 7307. / f24,
    3099. / f24, -2223. / f24, -5302. / f24, -3991. / f24, 766. / f24,
    5168. / f24, 5362. / f24, 731. / f24, -5140. / f24, -7094. / f24,
    -2830. / f24, 4611. / f24, 8861. / f24, 5584. / f24, -3260. / f24,
    -10326. / f24, -8887. / f24, 888. / f24, 11145. / f24, 12532. / f24,
    2617. / f24, -10961. / f24, -16207. / f24, -7257. / f24, 9442. / f24,
    19522. / f24, 12931. / f24, -6288. / f24, -22007. / f24, -19398. / f24,
    1280. / f24, 23148. / f24, 26290. / f24, 5704. / f24, -22403. / f24,
    -33102. / f24, -14655. / f24, 19237. / f24, 39196. / f24, 25404. / f24,
    -13162. / f24, -43824. / f24, -37610. / f24, 3766. / f24, 46146. / f24,
    50752. / f24, 9264. / f24, -45243. / f24, -64134. / f24, -26137. / f24,
    40124. / f24, 76873. / f24, 46957. / f24, -29705. / f24, -87899. / f24,
    -71773. / f24, 12729. / f24, 95920. / f24, 100661. / f24, 12412. / f24,
    -99329. / f24, -133927. / f24, -48113. / f24, 95967. / f24, 172563. / f24,
    98654. / f24, -82409. / f24, -219347. / f24, -173208. / f24, 51783. / f24,
    282060. / f24, 295863. / f24, 14257. / f24, -387590. / f24, -556360. / f24,
    -195882. / f24, 696028. / f24, 1767624. / f24, 2494432. / f24, 2494432. / f24,
    1767624. / f24, 696028. / f24, -195882. / f24, -556360. / f24, -387590. / f24,
    14257. / f24, 295863. / f24, 282060. / f24, 51783. / f24, -173208. / f24,
    -219347. / f24, -82409. / f24, 98654. / f24, 172563. / f24, 95967. / f24,
    -48113. / f24, -133927. / f24, -99329. / f24, 12412. / f24, 100661. / f24,
    95920. / f24, 12729. / f24, -71773. / f24, -87899. / f24, -29705. / f24,
    46957. / f24, 76873. / f24, 40124. / f24, -26137. / f24, -64134. / f24,
    -45243. / f24, 9264. / f24, 50752. / f24, 46146. / f24, 3766. / f24,
    -37610. / f24, -43824. / f24, -13162. / f24, 25404. / f24, 39196. / f24,
    19237. / f24, -14655. / f24, -33102. / f24, -22403. / f24, 5704. / f24,
    26290. / f24, 23148. / f24, 1280. / f24, -19398. / f24, -22007. / f24,
    -6288. / f24, 12931. / f24, 19522. / f24, 9442. / f24, -7257. / f24,
    -16207. / f24, -10961. / f24, 2617. / f24, 12532. / f24, 11145. / f24,
    888. / f24, -8887. / f24, -10326. / f24, -3260. / f24, 5584. / f24,
    8861. / f24, 4611. / f24, -2830. / f24, -7094. / f24, -5140. / f24,
    731. / f24, 5362. / f24, 5168. / f24, 766. / f24, -3991. / f24,
    -5302. / f24, -2223. / f24, 3099. / f24, 7307. / f24, 8447. / f24,
  6479. / f24, 3745. / f24, 877. / f24};


  *lenh0 = lenh03;		/* store 'number of coefficients' */
  *h0 = h03;			/* store pointer to h02[]-array */
}
#undef lenh03
#undef f24
/* ........................ End of fill_lp_3_to_1() ........................ */


/* ************************************************************************ */
/* ****** The functions that follow were added after the ITU-T STL92 ****** */
/* ************************************************************************ */

/*
  ============================================================================

        void fill_flat_band_pass (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        These are the filter coefficients describing the
        impulse response of the "flat" filter used in the
        "Talker Dependency Experiment" of the Selection Tests of the
        ITU-TSS 8kbit/s speech coder. A summary of the filter
        characteristics is as follows:

        - linear phase
        - 168 taps
        - band-pass filter with cut-off of very low frequency (a la
          G.712) for the telephony range [NOTE: can be used for other
          sampling frequencies, but the lower cut-off will change.
          Initially designed for 16kHz sampling rate!]
        - coefficients quantised and scaled so +/-1 gives +/-32768
          (ie a factor of 32768)
        - |Pass-band ripple| < 0.2dB (over approx. 143Hz -> 3407Hz)
        - 3dB points at approx. 98Hz and 3462Hz
        - stopband attenuation > 50dB (over approx. 0Hz -> 8Hz)
          and > 50dB (over approx. 3600Hz -> 8000Hz)


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
          John Barnes <G.J.P.Barnes@bnr.co.uk>
          Chris Tate <c.n.tate@bnr.co.uk>
          
        Routine:
          Simao Ferraz de Campos Neto
          DDS/Pr11                      Tel: +55-192-39-1396
          CPqD/Telebras                 Fax: +55-192-53-4754
          13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>

        History:
        ~~~~~~~~
        10.Apr.94 v1.0 Release of 1st version <simao@cpqd.ansp.br>

 ============================================================================
*/
#define f16 32768.
#define FLAT_BAND_PASS_LEN 168

void            fill_flat_band_pass(h0, lenh0)
  float         **h0;
  long           *lenh0;
{

  static float    flat_coef[FLAT_BAND_PASS_LEN] =
  {
    -43. / f16, -168. / f16, -156. / f16, -90. / f16, -1. / f16,
    -7. / f16, -78. / f16, -107. / f16, -55. / f16, -13. / f16,
    -55. / f16, -120. / f16, -108. / f16, -39. / f16, -31. / f16,
    -108. / f16, -154. / f16, -97. / f16, -29. / f16, -70. / f16,
    -168. / f16, -170. / f16, -72. / f16, -36. / f16, -134. / f16,
    -221. / f16, -159. / f16, -45. / f16, -74. / f16, -215. / f16,
    -251. / f16, -120. / f16, -34. / f16, -148. / f16, -296. / f16,
    -242. / f16, -67. / f16, -61. / f16, -255. / f16, -349. / f16,
    -186. / f16, -24. / f16, -140. / f16, -372. / f16, -349. / f16,
    -97. / f16, -24. / f16, -274. / f16, -465. / f16, -278. / f16,
    -1. / f16, -96. / f16, -443. / f16, -491. / f16, -139. / f16,
    54. / f16, -257. / f16, -605. / f16, -414. / f16, 43. / f16,
    17. / f16, -503. / f16, -699. / f16, -207. / f16, 214. / f16,
    -164. / f16, -803. / f16, -652. / f16, 137. / f16, 298. / f16,
    -547. / f16, -1107. / f16, -360. / f16, 636. / f16, 175. / f16,
    -1265. / f16, -1354. / f16, 468. / f16, 1485. / f16, -570. / f16,
    -3275. / f16, -1493. / f16, 5831. / f16, 12895. / f16, 12895. / f16,
    5831. / f16, -1493. / f16, -3275. / f16, -570. / f16, 1485. / f16,
    468. / f16, -1354. / f16, -1265. / f16, 175. / f16, 636. / f16,
    -360. / f16, -1107. / f16, -547. / f16, 298. / f16, 137. / f16,
    -652. / f16, -803. / f16, -164. / f16, 214. / f16, -207. / f16,
    -699. / f16, -503. / f16, 17. / f16, 43. / f16, -414. / f16,
    -605. / f16, -257. / f16, 54. / f16, -139. / f16, -491. / f16,
    -443. / f16, -96. / f16, -1. / f16, -278. / f16, -465. / f16,
    -274. / f16, -24. / f16, -97. / f16, -349. / f16, -372. / f16,
    -140. / f16, -24. / f16, -186. / f16, -349. / f16, -255. / f16,
    -61. / f16, -67. / f16, -242. / f16, -296. / f16, -148. / f16,
    -34. / f16, -120. / f16, -251. / f16, -215. / f16, -74. / f16,
    -45. / f16, -159. / f16, -221. / f16, -134. / f16, -36. / f16,
    -72. / f16, -170. / f16, -168. / f16, -70. / f16, -29. / f16,
    -97. / f16, -154. / f16, -108. / f16, -31. / f16, -39. / f16,
    -108. / f16, -120. / f16, -55. / f16, -13. / f16, -55. / f16,
    -107. / f16, -78. / f16, -7. / f16, -1. / f16, -90. / f16,
    -156. / f16, -168. / f16, -43. / f16
  };

  *lenh0 = FLAT_BAND_PASS_LEN;	/* store 'number of coefficients' */
  *h0 = flat_coef;		/* store pointer to []-array */
}
#undef f16
#undef FLAT_BAND_PASS_LEN
/* .................... End of fill_flat_band_pass() .................... */


/*
  ============================================================================

        SCD_FIR *linear_phase_pb_2_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Initialization routine for the linear-phase, FIR implementation of
        the band-pass filter characteristic for data sampled at 16 kHz and 
        down-sampling of 2:1. The mask resembles that of ITU-T Rec.G.712.

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
SCD_FIR        *linear_phase_pb_2_to_1_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_flat_band_pass(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       2l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ............... End of linear_phase_pb_2_to_1_init() ............... */


/*
  ============================================================================

        SCD_FIR *linear_phase_pb_1_to_2_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Initialization routine for the linear-phase, FIR implementation of
        the band-pass filter characteristic for data sampled at 16 kHz and 
        down-sampling of 2:1. The mask resembles that of ITU-T Rec.G.712.

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
SCD_FIR        *linear_phase_pb_1_to_2_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_flat_band_pass(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       2.0,	/* In: gain factor for FIR-coeffic. */
		       2l,	/* In: Down-sampling factor */
		       'U'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ................ End of linear_phase_pb_1_to_2_init() ................ */

/* ************************************************************************ */
/* ****** The functions that follow were added after the ITU-T STL96 ****** */
/* ************************************************************************ */



/*
  ============================================================================

        SCD_FIR *linear_phase_pb_1_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Initialization routine for the linear-phase, FIR implementation of
        the band-pass filter characteristic for data sampled at 16 kHz and 
        no downsampling (factor is 1:1). The output signal, although at the
	16 kHz sampling rate, will have a 4 kHz *effective* bandwidth.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_FIR;

        Author: <simao.campos@comsat.com>
        ~~~~~~~
        
        History:
        ~~~~~~~~
        16.May.97 v1.0 Release of 1st version <simao>
        06.Oct.99 v2.0 Fixed wrong gain in function (was 2.0) <simao>

 ============================================================================
*/
SCD_FIR        *linear_phase_pb_1_to_1_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_flat_band_pass(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil.) */
}
/* ................ End of linear_phase_pb_1_to_1_init() ................ */


/* ************************* END OF FIR-FLAT.C ************************** */
