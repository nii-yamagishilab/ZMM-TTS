/*                                                             v3.2 22.Dec.2004
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
                Sub-unit: IRS filtering

ORIGINAL BY:
                Rudolf Hofmann
                Advanced Development Digital Signal Processing
                PHILIPS KOMMUNIKATIONS INDUSTRIE AG
                Kommunikationssysteme
                Thurn-und-Taxis-Strasse 14
                D-8500 Nuernberg 10 (Germany)

                Phone : +49 911 526-2603
                FAX   : +49 911 526-3385
                EMail : hf@pkinbg.uucp

DESCRIPTION:
        This file contains procedures for IRS send part filtering for
	several sampling rates. Up and down sampling has not been made 
	part of this sub-unit.

FUNCTIONS:
  Global (have prototype in firflt.h)
         = irs_8khz_init()       :  initialize IRS weighting filter  8 kHz
         = irs_16khz_init()      :  initialize IRS weighting filter 16 kHz
         = mod_irs_16khz_init()
         = mod_irs_48khz_init()

  Local (should be used only here -- prototypes only in this file)
         = fill_irs8khz(...)     : idem, for IRS @  8 kHz
         = fill_irs16khz(...)    : idem, for IRS @ 16 kHz
         = fill_modified_irs16khz(...): idem, for mod.IRS @ 16kHz
         = fill_modified_irs48khz(...): idem, for mod.IRS @ 48kHz



HISTORY:
  16.Dec.1991 v0.1 First beta-version <hf@pkinbg.uucp>
  28.Feb.1992 v1.0 Release of 1st version to UGST <hf@pkinbg.uucp>
  20.Apr.1994 v2.0 Added new filtering routines: modified IRS at 16kHz and
                   48kHz, Delta-SM, Linear-phase band-pass.
                   <simao@cpqd.ansp.br>
  30.Sep.1994 v2.1 Updated to accomodate changes in the name of the name and
                   slitting of module in several files, for ease of expansion.
		    <simao@ctd.comsat.com>
  18.Nov.1996 v2.2 Modified gain in modified IRS initialization functions   
                   mod_irs_{48,16}k_init() from 1.0 to -1.0 to fix phase
		   inversion characteristic. <simao@ctd.comsat.com>
  02.Jul.1999 v3.0 Added modified IRS filter RECEIVE-side characteristic 
                   for 8 and 16 kHz sampling-rate, as 1:1 filters. <simao>
  13.Mar.2000 v3.1 Replaced coefficients for modified IRS receive 
                   characteristics to get a closer match to the mask
                   <simao>.
  22.Dec.2004 v3.2 The modified IRS send-side filter (48kHz) is made symmetric. <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>
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

void fill_irs8khz ARGS((float **h0, long *lenh0));
void fill_irs16khz ARGS((float **h0, long *lenh0));
void fill_modified_irs16khz ARGS((float **h0, long *lenh0));
void fill_modified_irs48khz ARGS((float **h0, long *lenh0));
void fill_rx_modified_irs8khz ARGS((float **h0, long *lenh0));
void fill_rx_modified_irs16khz ARGS((float **h0, long *lenh0));


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

        SCD_FIR *irs_8khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for IRS weighting filter for data sampled
        at 8 kHz.

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
SCD_FIR        *irs_8khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_irs8khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-filter) */
}
/* ........................ End of irs_8khz_init() ........................ */



/*
  ============================================================================

        SCD_FIR *irs_16khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for IRS weighting filter for data sampled
        at 16 kHz.

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
SCD_FIR        *irs_16khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_irs16khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ....................... End of irs_16khz_init() ....................... */


/*
  ============================================================================

        void fill_irs8khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with FIR coefficients for IRS send
        part filter, 8 kHz sampled speech, with 24 bit representation.

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
#define lenh0IRS8 151

void            fill_irs8khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{

  static float    h0IRS8[lenh0IRS8] = {
    75478. / f24, -14206. / f24, -79415. / f24, 24725. / f24, -12018. / f24,
    -6955. / f24, 6418. / f24, -9378. / f24, 4475. / f24, -2452. / f24,
    3665. / f24, 451. / f24, 2106. / f24, 614. / f24, 4112. / f24,
    1081. / f24, 1933. / f24, 1627. / f24, -272. / f24, 1998. / f24,
    -1880. / f24, 2458. / f24, -2915. / f24, -207. / f24, -39. / f24,
    -1243. / f24, 209. / f24, -1769. / f24, 755. / f24, -2845. / f24,
    3537. / f24, -3614. / f24, 918. / f24, -1447. / f24, -1570. / f24,
    3280. / f24, -7810. / f24, 9338. / f24, -7644. / f24, 2935. / f24,
    9680. / f24, -9577. / f24, 18798. / f24, -9883. / f24, 13551. / f24,
    7062. / f24, -3391. / f24, 23525. / f24, -16317. / f24, 23303. / f24,
    3769. / f24, -1411. / f24, 20093. / f24, -3115. / f24, 19200. / f24,
    3058. / f24, 22160. / f24, -5018. / f24, 19643. / f24, -15801. / f24,
    20036. / f24, -21687. / f24, -38474. / f24, 26946. / f24, -167771. / f24,
    102570. / f24, -204371. / f24, -63234. / f24, -84358. / f24, -304135. / f24,
    228893. / f24, -890014. / f24, 728671. / f24, -1951830. / f24, -609400. / f24,
    6230173. / f24, -609400. / f24, -1951830. / f24, 728671. / f24, -890014. / f24,
    228893. / f24, -304135. / f24, -84358. / f24, -63234. / f24, -204371. / f24,
    102570. / f24, -167771. / f24, 26946. / f24, -38474. / f24, -21687. / f24,
    20036. / f24, -15801. / f24, 19643. / f24, -5018. / f24, 22160. / f24,
    3058. / f24, 19200. / f24, -3115. / f24, 20093. / f24, -1411. / f24,
    3769. / f24, 23303. / f24, -16317. / f24, 23525. / f24, -3391. / f24,
    7062. / f24, 13551. / f24, -9883. / f24, 18798. / f24, -9577. / f24,
    9680. / f24, 2935. / f24, -7644. / f24, 9338. / f24, -7810. / f24,
    3280. / f24, -1570. / f24, -1447. / f24, 918. / f24, -3614. / f24,
    3537. / f24, -2845. / f24, 755. / f24, -1769. / f24, 209. / f24,
    -1243. / f24, -39. / f24, -207. / f24, -2915. / f24, 2458. / f24,
    -1880. / f24, 1998. / f24, -272. / f24, 1627. / f24, 1933. / f24,
    1081. / f24, 4112. / f24, 614. / f24, 2106. / f24, 451. / f24,
    3665. / f24, -2452. / f24, 4475. / f24, -9378. / f24, 6418. / f24,
    -6955. / f24, -12018. / f24, 24725. / f24, -79415. / f24, -14206. / f24,
  75478. / f24};


  *lenh0 = lenh0IRS8;		/* store 'number of coefficients' */
  *h0 = h0IRS8;			/* store pointer to h02[]-array */
}
#undef f24
#undef lenh0IRS8
/* ......................... End of fill_irs8khz() ......................... */


/*
  ============================================================================

        void fill_irs16khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with 24-bit FIR coefficients for IRS
        send part filter for speech sampled at 16 kHz.

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
#define lenh0IRS16 209

void            fill_irs16khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{
  static float    h0IRS16[lenh0IRS16] = {
    -6550. / f24, -19590. / f24, -31126. / f24, -26048. / f24, 1228. / f24,
    34321. / f24, 45479. / f24, 24079. / f24, -8997. / f24, -22054. / f24,
    -6923. / f24, 13991. / f24, 16071. / f24, 473. / f24, -11489. / f24,
    -6686. / f24, 5726. / f24, 9089. / f24, 547. / f24, -7804. / f24,
    -6076. / f24, 1728. / f24, 4793. / f24, -163. / f24, -5958. / f24,
    -5297. / f24, 373. / f24, 3914. / f24, 1576. / f24, -3357. / f24,
    -4996. / f24, -1334. / f24, 3716. / f24, 4539. / f24, 329. / f24,
    -3277. / f24, -841. / f24, 5094. / f24, 6459. / f24, 943. / f24,
    -3697. / f24, -83. / f24, 7579. / f24, 8650. / f24, 1424. / f24,
    -3680. / f24, 1315. / f24, 9920. / f24, 9523. / f24, -443. / f24,
    -6464. / f24, 450. / f24, 11324. / f24, 11286. / f24, 670. / f24,
    -5417. / f24, 1069. / f24, 9734. / f24, 8006. / f24, 179. / f24,
    -479. / f24, 6622. / f24, 9177. / f24, 3279. / f24, 439. / f24,
    7302. / f24, 12461. / f24, 5268. / f24, -4048. / f24, 172. / f24,
    10849. / f24, 7385. / f24, -8008. / f24, -9315. / f24, 9395. / f24,
    16551. / f24, -9249. / f24, -36445. / f24, -20593. / f24, 19917. / f24,
    15401. / f24, -48052. / f24, -84637. / f24, -26771. / f24, 52025. / f24,
    21079. / f24, -102121. / f24, -143790. / f24, -31091. / f24, 61111. / f24,
    -42320. / f24, -207655. / f24, -151256. / f24, 87076. / f24, 113228. / f24,
    -217142. / f24, -443156. / f24, -117237. / f24, 361490. / f24, 90490. / f24,
    -971447. / f24, -1510993. / f24, -311632. / f24, 1958678. / f24, 3123092. / f24,
    1958678. / f24, -311632. / f24, -1510993. / f24, -971447. / f24, 90490. / f24,
    361490. / f24, -117237. / f24, -443156. / f24, -217142. / f24, 113228. / f24,
    87076. / f24, -151256. / f24, -207655. / f24, -42320. / f24, 61111. / f24,
    -31091. / f24, -143790. / f24, -102121. / f24, 21079. / f24, 52025. / f24,
    -26771. / f24, -84637. / f24, -48052. / f24, 15401. / f24, 19917. / f24,
    -20593. / f24, -36445. / f24, -9249. / f24, 16551. / f24, 9395. / f24,
    -9315. / f24, -8008. / f24, 7385. / f24, 10849. / f24, 172. / f24,
    -4048. / f24, 5268. / f24, 12461. / f24, 7302. / f24, 439. / f24,
    3279. / f24, 9177. / f24, 6622. / f24, -479. / f24, 179. / f24,
    8006. / f24, 9734. / f24, 1069. / f24, -5417. / f24, 670. / f24,
    11286. / f24, 11324. / f24, 450. / f24, -6464. / f24, -443. / f24,
    9523. / f24, 9920. / f24, 1315. / f24, -3680. / f24, 1424. / f24,
    8650. / f24, 7579. / f24, -83. / f24, -3697. / f24, 943. / f24,
    6459. / f24, 5094. / f24, -841. / f24, -3277. / f24, 329. / f24,
    4539. / f24, 3716. / f24, -1334. / f24, -4996. / f24, -3357. / f24,
    1576. / f24, 3914. / f24, 373. / f24, -5297. / f24, -5958. / f24,
    -163. / f24, 4793. / f24, 1728. / f24, -6076. / f24, -7804. / f24,
    547. / f24, 9089. / f24, 5726. / f24, -6686. / f24, -11489. / f24,
    473. / f24, 16071. / f24, 13991. / f24, -6923. / f24, -22054. / f24,
    -8997. / f24, 24079. / f24, 45479. / f24, 34321. / f24, 1228. / f24,
  -26048. / f24, -31126. / f24, -19590. / f24, -6550. / f24};


  *lenh0 = lenh0IRS16;		/* store 'number of coefficients' */
  *h0 = h0IRS16;		/* store pointer to h02[]-array */
}
#undef f24
#undef lenh0IRS16
/* ........................ End of fill_irs16khz() ........................ */


/* ************************************************************************ */
/* ****** The functions that follow were added after the ITU-T STL92 ****** */
/* ************************************************************************ */

/*
  ============================================================================

        void fill_modified_irs16khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with 16-bit FIR coefficients for
        modified IRS send part filter for speech sampled at 16 kHz.
        The sum of the squares of the coefficient values is given to allow
        checking to be done. The coefficients are listed below in rows of 10.
        Order M = 495
        Scale factor = 1/(32768)
        Sum of squares of coefficient values = 484646208

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
#define MOD_IRS16_LEN 495

void            fill_modified_irs16khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{

  static float    mod_irs16_coef[MOD_IRS16_LEN] = {
    1. / f16, 1. / f16, 1. / f16, 1. / f16, 1. / f16,
    1. / f16, 1. / f16, 1. / f16, 1. / f16, 1. / f16,
    1. / f16, 1. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 1. / f16, 1. / f16, 1. / f16, 1. / f16,
    1. / f16, 2. / f16, 2. / f16, 2. / f16, 2. / f16,
    3. / f16, 3. / f16, 3. / f16, 3. / f16, 3. / f16,
    4. / f16, 4. / f16, 4. / f16, 4. / f16, 4. / f16,
    4. / f16, 4. / f16, 4. / f16, 4. / f16, 4. / f16,
    3. / f16, 3. / f16, 3. / f16, 3. / f16, 3. / f16,
    2. / f16, 2. / f16, 2. / f16, 1. / f16, 1. / f16,
    0. / f16, 0. / f16, 0. / f16, -1. / f16, -1. / f16,
    -1. / f16, -2. / f16, -2. / f16, -2. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -3. / f16, -3. / f16, -3. / f16, -4. / f16,
    -5. / f16, -5. / f16, -5. / f16, -6. / f16, -7. / f16,
    -8. / f16, -9. / f16, -8. / f16, -8. / f16, -10. / f16,
    -11. / f16, -12. / f16, -12. / f16, -11. / f16, -12. / f16,
    -14. / f16, -15. / f16, -14. / f16, -13. / f16, -14. / f16,
    -16. / f16, -17. / f16, -15. / f16, -13. / f16, -13. / f16,
    -15. / f16, -15. / f16, -12. / f16, -8. / f16, -7. / f16,
    -9. / f16, -12. / f16, -9. / f16, -3. / f16, 0. / f16,
    -2. / f16, -6. / f16, -6. / f16, 0. / f16, 7. / f16,
    6. / f16, -3. / f16, -5. / f16, 5. / f16, 18. / f16,
    19. / f16, 8. / f16, 3. / f16, 22. / f16, 50. / f16,
    53. / f16, 30. / f16, 21. / f16, 52. / f16, 104. / f16,
    124. / f16, 91. / f16, 71. / f16, 133. / f16, 224. / f16,
    221. / f16, 114. / f16, 34. / f16, 100. / f16, 273. / f16,
    346. / f16, 234. / f16, 169. / f16, 324. / f16, 457. / f16,
    302. / f16, -22. / f16, -145. / f16, 296. / f16, 966. / f16,
    727. / f16, -148. / f16, -55. / f16, 1326. / f16, 4457. / f16,
    4853. / f16, -6894. / f16, -17155. / f16, -6894. / f16, 4853. / f16,
    4457. / f16, 1326. / f16, -55. / f16, -148. / f16, 727. / f16,
    966. / f16, 296. / f16, -145. / f16, -22. / f16, 302. / f16,
    457. / f16, 324. / f16, 169. / f16, 234. / f16, 346. / f16,
    273. / f16, 100. / f16, 34. / f16, 114. / f16, 221. / f16,
    224. / f16, 133. / f16, 71. / f16, 91. / f16, 124. / f16,
    104. / f16, 52. / f16, 21. / f16, 30. / f16, 53. / f16,
    50. / f16, 22. / f16, 3. / f16, 8. / f16, 19. / f16,
    18. / f16, 5. / f16, -5. / f16, -3. / f16, 6. / f16,
    7. / f16, 0. / f16, -6. / f16, -6. / f16, -2. / f16,
    0. / f16, -3. / f16, -9. / f16, -12. / f16, -9. / f16,
    -7. / f16, -8. / f16, -12. / f16, -15. / f16, -15. / f16,
    -13. / f16, -13. / f16, -15. / f16, -17. / f16, -16. / f16,
    -14. / f16, -13. / f16, -14. / f16, -15. / f16, -14. / f16,
    -12. / f16, -11. / f16, -12. / f16, -12. / f16, -11. / f16,
    -10. / f16, -8. / f16, -8. / f16, -9. / f16, -8. / f16,
    -7. / f16, -6. / f16, -5. / f16, -5. / f16, -5. / f16,
    -4. / f16, -3. / f16, -3. / f16, -3. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -2. / f16, -2. / f16, -2. / f16, -1. / f16,
    -1. / f16, -1. / f16, 0. / f16, 0. / f16, 0. / f16,
    1. / f16, 1. / f16, 2. / f16, 2. / f16, 2. / f16,
    3. / f16, 3. / f16, 3. / f16, 3. / f16, 3. / f16,
    4. / f16, 4. / f16, 4. / f16, 4. / f16, 4. / f16,
    4. / f16, 4. / f16, 4. / f16, 4. / f16, 4. / f16,
    3. / f16, 3. / f16, 3. / f16, 3. / f16, 3. / f16,
    2. / f16, 2. / f16, 2. / f16, 2. / f16, 1. / f16,
    1. / f16, 1. / f16, 1. / f16, 1. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, -1. / f16, -1. / f16, -1. / f16,
    -1. / f16, -1. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, 1. / f16, 1. / f16,
    1. / f16, 1. / f16, 1. / f16, 1. / f16, 1. / f16,
    1. / f16, 1. / f16, 1. / f16, 1. / f16, 1. / f16
  };

  *lenh0 = MOD_IRS16_LEN;	/* store 'number of coefficients' */
  *h0 = mod_irs16_coef;		/* store pointer to []-array */
}
#undef f16
#undef MOD_IRS16_LEN
/* .................... End of fill_modified_irs16khz() .................... */



/*
  ============================================================================

        void fill_modified_irs48khz (float **h0, long *lenh0);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize pointer to array with 16-bit FIR coefficients for
        modified IRS send part filter for speech sampled at 48 kHz.
        Scale factor = 1/(32768)

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
		22.Dec.04 v3.2 This filter is made symmetric <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>

 ============================================================================
*/
#define f16 32768.
#define MOD_IRS48_LEN 513

void            fill_modified_irs48khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{

  static float    mod_irs48_coef[MOD_IRS48_LEN] =
  {
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -4. / f16, -4. / f16, -4. / f16, -4. / f16, -4. / f16,
    -5. / f16, -5. / f16, -5. / f16, -5. / f16, -5. / f16,
    -5. / f16, -5. / f16, -5. / f16, -5. / f16, -5. / f16,
    -5. / f16, -5. / f16, -5. / f16, -5. / f16, -6. / f16,
    -6. / f16, -6. / f16, -6. / f16, -6. / f16, -6. / f16,
    -6. / f16, -6. / f16, -6. / f16, -5. / f16, -5. / f16,
    -5. / f16, -5. / f16, -6. / f16, -6. / f16, -6. / f16,
    -6. / f16, -7. / f16, -7. / f16, -7. / f16, -7. / f16,
    -6. / f16, -6. / f16, -6. / f16, -6. / f16, -5. / f16,
    -5. / f16, -5. / f16, -5. / f16, -5. / f16, -5. / f16,
    -6. / f16, -6. / f16, -6. / f16, -6. / f16, -5. / f16,
    -5. / f16, -5. / f16, -4. / f16, -4. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -4. / f16, -4. / f16, -4. / f16, -4. / f16,
    -4. / f16, -3. / f16, -2. / f16, -2. / f16, -1. / f16,
    0. / f16, 0. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, -1. / f16, -1. / f16, -1. / f16, -2. / f16,
    -2. / f16, -1. / f16, -1. / f16, 0. / f16, 1. / f16,
    2. / f16, 3. / f16, 3. / f16, 3. / f16, 3. / f16,
    3. / f16, 2. / f16, 1. / f16, 0. / f16, 0. / f16,
    -1. / f16, -1. / f16, 0. / f16, 1. / f16, 3. / f16,
    4. / f16, 6. / f16, 7. / f16, 8. / f16, 8. / f16,
    8. / f16, 7. / f16, 5. / f16, 4. / f16, 3. / f16,
    2. / f16, 2. / f16, 4. / f16, 6. / f16, 9. / f16,
    12. / f16, 15. / f16, 18. / f16, 19. / f16, 20. / f16,
    19. / f16, 17. / f16, 14. / f16, 12. / f16, 9. / f16,
    8. / f16, 8. / f16, 10. / f16, 13. / f16, 19. / f16,
    25. / f16, 31. / f16, 36. / f16, 40. / f16, 42. / f16,
    42. / f16, 40. / f16, 36. / f16, 31. / f16, 27. / f16,
    25. / f16, 25. / f16, 29. / f16, 36. / f16, 45. / f16,
    56. / f16, 66. / f16, 75. / f16, 80. / f16, 80. / f16,
    76. / f16, 66. / f16, 53. / f16, 39. / f16, 25. / f16,
    15. / f16, 10. / f16, 12. / f16, 21. / f16, 35. / f16,
    54. / f16, 74. / f16, 92. / f16, 106. / f16, 113. / f16,
    113. / f16, 106. / f16, 94. / f16, 80. / f16, 67. / f16,
    59. / f16, 59. / f16, 67. / f16, 82. / f16, 103. / f16,
    125. / f16, 144. / f16, 154. / f16, 152. / f16, 136. / f16,
    106. / f16, 67. / f16, 23. / f16, -17. / f16, -46. / f16,
    -58. / f16, -47. / f16, -12. / f16, 43. / f16, 113. / f16,
    186. / f16, 252. / f16, 299. / f16, 317. / f16, 301. / f16,
    250. / f16, 170. / f16, 75. / f16, -20. / f16, -92. / f16,
    -120. / f16, -84. / f16, 30. / f16, 226. / f16, 500. / f16,
    830. / f16, 1182. / f16, 1503. / f16, 1721. / f16, 1745. / f16,
    1470. / f16, 789. / f16, -378. / f16, -2008. / f16, -3872. / f16,
    -5455. / f16, -6098. / f16, -5455. / f16, -3872. / f16, -2008. / f16,
    -378. / f16, 789. / f16, 1470. / f16, 1745. / f16, 1721. / f16,
    1503. / f16, 1182. / f16, 830. / f16, 500. / f16, 226. / f16,
    30. / f16, -84. / f16, -120. / f16, -92. / f16, -20. / f16,
    75. / f16, 170. / f16, 250. / f16, 301. / f16, 317. / f16,
    299. / f16, 252. / f16, 186. / f16, 113. / f16, 43. / f16,
    -12. / f16, -47. / f16, -58. / f16, -46. / f16, -17. / f16,
    23. / f16, 67. / f16, 106. / f16, 136. / f16, 152. / f16,
    154. / f16, 144. / f16, 125. / f16, 103. / f16, 82. / f16,
    67. / f16, 59. / f16, 59. / f16, 67. / f16, 80. / f16,
    94. / f16, 106. / f16, 113. / f16, 113. / f16, 106. / f16,
    92. / f16, 74. / f16, 54. / f16, 35. / f16, 21. / f16,
    12. / f16, 10. / f16, 15. / f16, 25. / f16, 39. / f16,
    53. / f16, 66. / f16, 76. / f16, 80. / f16, 80. / f16,
    75. / f16, 66. / f16, 56. / f16, 45. / f16, 36. / f16,
    29. / f16, 25. / f16, 25. / f16, 27. / f16, 31. / f16,
    36. / f16, 40. / f16, 42. / f16, 42. / f16, 40. / f16,
    36. / f16, 31. / f16, 25. / f16, 19. / f16, 13. / f16,
    10. / f16, 8. / f16, 8. / f16, 9. / f16, 12. / f16,
    14. / f16, 17. / f16, 19. / f16, 20. / f16, 19. / f16,
    18. / f16, 15. / f16, 12. / f16, 9. / f16, 6. / f16,
    4. / f16, 2. / f16, 2. / f16, 3. / f16, 4. / f16,
    5. / f16, 7. / f16, 8. / f16, 8. / f16, 8. / f16,
    7. / f16, 6. / f16, 4. / f16, 3. / f16, 1. / f16,
    0. / f16, -1. / f16, -1. / f16, 0. / f16, 0. / f16,
    1. / f16, 2. / f16, 3. / f16, 3. / f16, 3. / f16,
    3. / f16, 3. / f16, 2. / f16, 1. / f16, 0. / f16,
    -1. / f16, -1. / f16, -2. / f16, -2. / f16, -1. / f16,
    -1. / f16, -1. / f16, 0. / f16, 0. / f16, 0. / f16,
    0. / f16, 0. / f16, 0. / f16, -1. / f16, -2. / f16,
    -2. / f16, -3. / f16, -4. / f16, -4. / f16, -4. / f16,
    -4. / f16, -4. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -4. / f16,
    -4. / f16, -5. / f16, -5. / f16, -5. / f16, -6. / f16,
    -6. / f16, -6. / f16, -6. / f16, -5. / f16, -5. / f16,
    -5. / f16, -5. / f16, -5. / f16, -5. / f16, -6. / f16,
    -6. / f16, -6. / f16, -6. / f16, -7. / f16, -7. / f16,
    -7. / f16, -7. / f16, -6. / f16, -6. / f16, -6. / f16,
    -6. / f16, -5. / f16, -5. / f16, -5. / f16, -5. / f16,
    -6. / f16, -6. / f16, -6. / f16, -6. / f16, -6. / f16,
    -6. / f16, -6. / f16, -6. / f16, -6. / f16, -5. / f16,
    -5. / f16, -5. / f16, -5. / f16, -5. / f16, -5. / f16,
    -5. / f16, -5. / f16, -5. / f16, -5. / f16, -5. / f16,
    -5. / f16, -5. / f16, -5. / f16, -4. / f16, -4. / f16,
    -4. / f16, -4. / f16, -4. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -3. / f16,
    -3. / f16, -3. / f16, -3. / f16, -3. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16, -2. / f16, -2. / f16,
    -2. / f16, -2. / f16, -2. / f16 };

  *lenh0 = MOD_IRS48_LEN;	/* store 'number of coefficients' */
  *h0 = mod_irs48_coef;		/* store pointer to []-array */
}

#undef f16
#undef MOD_IRS48_LEN
/* .................... End of fill_modified_irs48khz() .................... */


/*
  ============================================================================

        SCD_FIR *mod_irs_16khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for IRS weighting filter for data sampled
        at 16 kHz.

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
        18.Nov.96 v2.0 Modified gain from 1.0 to -1.0 to fix phase
		       inversion characteristic. <simao@ctd.comsat.com>

 ============================================================================
*/
SCD_FIR        *mod_irs_16khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_modified_irs16khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       -1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ..................... End of mod_irs_16khz_init() ...................... */


/*
  ============================================================================

        SCD_FIR *mod_irs_48khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialization routine for the modified IRS weighting filter for 
        data sampled at 48 kHz.

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
        18.Nov.96 v2.0 Modified gain from 1.0 to -1.0 to fix phase
		       inversion characteristic. <simao@ctd.comsat.com>

 ============================================================================
*/
SCD_FIR        *mod_irs_48khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_modified_irs48khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       -1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ..................... End of mod_irs_48khz_init() ..................... */


/* ************************************************************************ */
/* ****** The functions that follow were added after the ITU-T STL96 ****** */
/* ************************************************************************ */

/*
  ============================================================================

  void fill_rx_modified_irs16khz (float **h0, long *lenh0);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to array with floating-point FIR coefficients for
  modified IRS receive part filter for speech sampled at 16 kHz.
  Order M = 201
  Scale factor = 1.0 [floating point coefficients]

  The coefficients are set such that the conversion curve will yield
  values similar to a dBPa/V scale.

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
   Version 1.0: Robert Dunn <bobd@voxware.com> [donated on June 1, 1998]
   Version 2.0: Simao Campos

  Routine:
   Simao Ferraz de Campos Neto
   COMSAT Laboratories                  Tel:    +1-301-428-4516
   22300 Comsat Drive                   Fax:    +1-301-428-9287
   Clarksburg MD 20871 - USA            E-mail: simao.campos@comsat.com

  History:
  ~~~~~~~~
  02.Jul.1999 v1.0 Release of 1st version <simao.campos@comsat.com>
  13.Mar.2000 v2.0 Replaced coefficients with a frequency response
                   more closely following the Mod.IRS receive
                   characteristic. Coefficients provided in floating
                   point representation, however quantized to
                   Q24. <simao>
 ============================================================================
*/
#define RX_MOD_IRS16_LEN 148

void            fill_rx_modified_irs16khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{
  static float    rx_mod_irs16_coef[RX_MOD_IRS16_LEN] = {
  (F)-7.58608110527E-05, (F)-7.45618878900E-05, (F)3.52397043534E-05, 
  (F) 6.59225768920E-05, (F)-8.37107055674E-05, (F)-1.58207551398E-04, 
  (F) 5.62956743818E-05, (F) 2.56970258294E-04, (F) 6.77430584711E-05, 
  (F)-2.00030886248E-04, (F) 4.44851821251E-05, (F) 5.53472241382E-04, 
  (F) 4.86998632219E-04, (F)-6.69370487084E-05, (F)-7.70833910586E-06, 
  (F) 8.59767144900E-04, (F) 1.20365948004E-03, (F) 4.07633497495E-04, 
  (F)-4.83212337795E-05, (F) 1.00774295131E-03, (F) 2.08612468009E-03, 
  (F) 1.33872917862E-03, (F) 5.34656421714E-05, (F) 8.17449939198E-04, 
  (F) 2.80029955459E-03, (F) 2.66869275784E-03, (F) 4.77016237903E-04, 
  (F) 1.93215167531E-04, (F) 2.85267934554E-03, (F) 4.05666481945E-03, 
  (F) 1.35463805644E-03, (F)-7.86614339958E-04, (F) 1.73439319417E-03, 
  (F) 4.84337106579E-03, (F) 2.62691593615E-03, (F)-1.84049759260E-03, 
  (F)-8.61605387770E-04, (F) 4.14629655378E-03, (F) 3.90122545031E-03, 
  (F)-2.55570237462E-03, (F)-4.82171288139E-03, (F) 1.09982019304E-03, 
  (F) 4.38664353426E-03, (F)-2.56328316368E-03, (F)-9.48298074862E-03, 
  (F)-4.80499625396E-03, (F) 2.96737931365E-03, (F)-1.76891938392E-03, 
  (F)-1.36665825591E-02, (F)-1.33800532867E-02, (F)-1.57198138490E-03, 
  (F)-5.58846793879E-04, (F)-1.58836781884E-02, (F)-2.35150240749E-02, 
  (F)-1.02340330877E-02, (F) 9.03664564506E-05, (F)-1.46306631671E-02, 
  (F)-3.31327842943E-02, (F)-2.35700471003E-02, (F)-1.41610250658E-03, 
  (F)-8.56157183619E-03, (F)-3.92227490974E-02, (F)-4.18423596350E-02, 
  (F)-7.52821834806E-03, (F) 4.01084684374E-03, (F)-3.72587785679E-02, 
  (F)-6.66875462216E-02, (F)-2.37070509946E-02, (F) 2.91713227025E-02, 
  (F)-1.47859922355E-02, (F)-1.14322570124E-01, (F)-8.49657112273E-02, 
  (F) 1.47105639110E-01, (F) 3.90180668520E-01, (F) 3.90180668520E-01, 
  (F) 1.47105639110E-01, (F)-8.49657112273E-02, (F)-1.14322570124E-01, 
  (F)-1.47859922355E-02, (F) 2.91713227025E-02, (F)-2.37070509946E-02, 
  (F)-6.66875462216E-02, (F)-3.72587785679E-02, (F) 4.01084684374E-03, 
  (F)-7.52821834806E-03, (F)-4.18423596350E-02, (F)-3.92227490974E-02, 
  (F)-8.56157183619E-03, (F)-1.41610250658E-03, (F)-2.35700471003E-02, 
  (F)-3.31327842943E-02, (F)-1.46306631671E-02, (F) 9.03664564506E-05, 
  (F)-1.02340330877E-02, (F)-2.35150240749E-02, (F)-1.58836781884E-02, 
  (F)-5.58846793879E-04, (F)-1.57198138490E-03, (F)-1.33800532867E-02, 
  (F)-1.36665825591E-02, (F)-1.76891938392E-03, (F) 2.96737931365E-03, 
  (F)-4.80499625396E-03, (F)-9.48298074862E-03, (F)-2.56328316368E-03, 
  (F) 4.38664353426E-03, (F) 1.09982019304E-03, (F)-4.82171288139E-03, 
  (F)-2.55570237462E-03, (F) 3.90122545031E-03, (F) 4.14629655378E-03, 
  (F)-8.61605387770E-04, (F)-1.84049759260E-03, (F) 2.62691593615E-03, 
  (F) 4.84337106579E-03, (F) 1.73439319417E-03, (F)-7.86614339958E-04, 
  (F) 1.35463805644E-03, (F) 4.05666481945E-03, (F) 2.85267934554E-03, 
  (F) 1.93215167531E-04, (F) 4.77016237903E-04, (F) 2.66869275784E-03, 
  (F) 2.80029955459E-03, (F) 8.17449939198E-04, (F) 5.34656421714E-05, 
  (F) 1.33872917862E-03, (F) 2.08612468009E-03, (F) 1.00774295131E-03, 
  (F)-4.83212337795E-05, (F) 4.07633497495E-04, (F) 1.20365948004E-03, 
  (F) 8.59767144900E-04, (F)-7.70833910586E-06, (F)-6.69370487084E-05, 
  (F) 4.86998632219E-04, (F) 5.53472241382E-04, (F) 4.44851821251E-05, 
  (F)-2.00030886248E-04, (F) 6.77430584711E-05, (F) 2.56970258294E-04, 
  (F) 5.62956743818E-05, (F)-1.58207551398E-04, (F)-8.37107055674E-05, 
  (F) 6.59225768920E-05, (F) 3.52397043534E-05, (F)-7.45618878900E-05, 
  (F)-7.58608110527E-05
  };

  *lenh0 = RX_MOD_IRS16_LEN;	/* store 'number of coefficients' */
  *h0 = rx_mod_irs16_coef;		/* store pointer to []-array */
}
#undef RX_MOD_IRS16_LEN
/* .................. End of fill_rx_modified_irs16khz() .................. */

/*
  ============================================================================

  SCD_FIR *rx_mod_irs_16khz_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialization routine for receive-side IRS weighting filter for
  data sampled at 16 kHz. 

  The coefficients are set such that the conversion curve will yield
  unity gain at 1000 Hz.

  Parameters:  none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct SCD_FIR;

  Author: <simao.campos@comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  02.Jul.1999 v1.0 Release of 1st version <simao.campos@comsat.com>
  13.Mar.2000 v1.1 Changed gain to fit new coefficients <simao>

  ============================================================================ 
*/
SCD_FIR        *rx_mod_irs_16khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_rx_modified_irs16khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ................... End of rx_mod_irs_16khz_init() ..................... */


/*
  ============================================================================

  void fill_rx_modified_irs8khz (float **h0, long *lenh0);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to array with floating-point FIR coefficients for
  modified IRS receive part filter for speech sampled at 8 kHz with order
  75 and unity gain at 1000 Hz.

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
   Version 1.0: Robert Dunn <bobd@voxware.com> [donated on June 1, 1998]
   Version 2.0: Simao Campos

  Routine:
   Simao Ferraz de Campos Neto
   COMSAT Laboratories                  Tel:    +1-301-428-4516
   22300 Comsat Drive                   Fax:    +1-301-428-9287
   Clarksburg MD 20871 - USA            E-mail: simao.campos@comsat.com

  History:
  ~~~~~~~~
  02.Jul.1999 v1.0 Release of 1st version <simao.campos@comsat.com>
  13.Mar.2000 v2.0 Replaced coefficients with a frequency response
                   more closely following the Mod.IRS receive
                   characteristic. Coefficients provided in floating
                   point representation, however quantized to Q24.
                   <simao>

 ============================================================================ 
*/
#define RX_MOD_IRS8_LEN 75

void            fill_rx_modified_irs8khz(h0, lenh0)
  float         **h0;
  long           *lenh0;
{
  static float    rx_mod_irs8_coef[RX_MOD_IRS8_LEN] = {
  (F)-9.29651661244e-05, (F)-5.57106193888e-05, (F) 1.03988706315e-05, 
  (F)-1.72293166049e-04, (F) 4.47217668394e-04, (F)-3.23797104316e-04, 
  (F) 1.30365157303e-03, (F)-3.55580623304e-04, (F) 2.45858167783e-03, 
  (F) 2.42818204634e-05, (F) 3.51212488897e-03, (F) 1.16618521672e-03, 
  (F) 3.83442125820e-03, (F) 3.28526605765e-03, (F) 2.76231166368e-03, 
  (F) 6.19221988419e-03, (F)-9.82572018289e-05, (F) 9.05269882274e-03, 
  (F)-4.57588144125e-03, (F) 1.03141841829e-02, (F)-9.78742034483e-03, 
  (F) 7.91486895870e-03, (F)-1.42545020007e-02, (F)-2.00938098796e-04, 
  (F)-1.63018302038e-02, (F)-1.53662101258e-02, (F)-1.46332554277e-02, 
  (F)-3.74962856889e-02, (F)-8.88267461843e-03, (F)-6.46863884253e-02, 
  (F) 8.76053917889e-05, (F)-9.33150000039e-02, (F) 1.02964712211e-02, 
  (F)-1.18710939405e-01, (F) 1.91692363192e-02, (F)-1.36240288203e-01, 
  (F) 2.43201311407e-02, (F) 8.57500000600e-01, (F) 2.43201311407e-02, 
  (F)-1.36240288203e-01, (F) 1.91692363192e-02, (F)-1.18710939405e-01, 
  (F) 1.02964712211e-02, (F)-9.33150000039e-02, (F) 8.76053917889e-05, 
  (F)-6.46863884253e-02, (F)-8.88267461843e-03, (F)-3.74962856889e-02, 
  (F)-1.46332554277e-02, (F)-1.53662101258e-02, (F)-1.63018302038e-02, 
  (F)-2.00938098796e-04, (F)-1.42545020007e-02, (F) 7.91486895870e-03, 
  (F)-9.78742034483e-03, (F) 1.03141841829e-02, (F)-4.57588144125e-03, 
  (F) 9.05269882274e-03, (F)-9.82572018289e-05, (F) 6.19221988419e-03, 
  (F) 2.76231166368e-03, (F) 3.28526605765e-03, (F) 3.83442125820e-03, 
  (F) 1.16618521672e-03, (F) 3.51212488897e-03, (F) 2.42818204634e-05, 
  (F) 2.45858167783e-03, (F)-3.55580623304e-04, (F) 1.30365157303e-03, 
  (F)-3.23797104316e-04, (F) 4.47217668394e-04, (F)-1.72293166049e-04, 
  (F) 1.03988706315e-05, (F)-5.57106193888e-05, (F)-9.29651661244e-05
  };

  *lenh0 = RX_MOD_IRS8_LEN;	/* store 'number of coefficients' */
  *h0 = rx_mod_irs8_coef;		/* store pointer to []-array */
}
#undef RX_MOD_IRS8_LEN
/* .................. End of fill_rx_modified_irs8khz() .................. */

/*
  ============================================================================

  SCD_FIR *rx_mod_irs_8khz_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialization routine for receive-side IRS weighting filter for
  data sampled at 8 kHz. Coefficients normalized to bring a unit gain
  at 1 kHz (0.50).

  Parameters:  none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct SCD_FIR;

  Author: <simao.campos@comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  02.Jul.99 v1.0 Release of 1st version <simao.campos@comsat.com>

 ============================================================================
*/
SCD_FIR        *rx_mod_irs_8khz_init()
{
  float          *h0;		/* pointer to array with FIR coeff. */
  long            lenh0;	/* number of FIR coefficients */


  /* allocate array for FIR coeff. and fill with coefficients */
  fill_rx_modified_irs8khz(&h0, &lenh0);

  return
    fir_initialization(		/* Returns: pointer to SCD_FIR-struct */
		       lenh0,	/* In: number of FIR-coefficients */
		       h0,	/* In: pointer to array with FIR-cof. */
		       1.0,	/* In: gain factor for FIR-coeffic. */
		       1l,	/* In: Down-sampling factor */
		       'D'	/* In: switch to down-sampling proc. */
    );				/* (works here as simple FIR-fil. */
}
/* ................... End of rx_mod_irs_8khz_init() ..................... */


/* *************************** END OF FIR-IRS.C *************************** */
