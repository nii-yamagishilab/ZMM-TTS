/*                                                         v1.0 28.FEB.92 18H00
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

MODULE:         IIR-G712.C, IIR FILTER MODULE, 
                Sub-unit with the parallel-form standard PCM filter (G.712)

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

        This file contains functions for initializing an
        IIR-filter (parallel form) with a standard PCM frequency
        response (sampling frequency = 8 and16 kHz).
        (see also ETSI-document TD 91/23, 4.2).

FUNCTIONS    : - stdpcm_16khz_init()  =  initialization of standard
                                         PCM-filter for sf=16kHz
               - stdpcm_1_to_2_init() =  as "stdpcm_16khz_init()", but
                                         needs input with sf=8kHz and
                                         returns data with sf=16 kHz
               - stdpcm_2_to_1_init() =  as "stdpcm_16khz_init()", but
                                         needs input with sf=16kHz and
                                         returns data with sf=8 kHz

HISTORY:

    16.Dec.91 v0.1 First beta-version <hf@pkinbg.uucp>
    28.Feb.92 v1.0 Release of 1st version to UGST <hf@pkinbg.uucp>
    30.Oct.94 v2.0 Broken into sub-units. <simao@ctd.comsat.com>

  =============================================================================
*/


/*
 * ......... INCLUDES .........
 */
#ifndef VMS
#include <stdlib.h>		  /* General utility definitions */
#endif
#include <math.h>		  /* RTL Math Function Declarations */

/* Definitions for IIR filters */
#include "iirflt.h"		  



/*
 * ......... Local function *smart* prototypes .........
 */
void fill_iir_cof_16khz ARGS((float ***b_cof, float ***c_cof, 
			      double *K, long *nblocks));

/* External function prototypes - code in in iir-lib.c */
extern SCD_IIR *scd_stdpcm_init ARGS((long nblocks, float (*b)[3], 
			    float (*c)[2], double direct_cof, double gain, 
			    long idown, int hswitch));


/*
 * ...................... BEGIN OF FUNCTIONS .........................
 */


/*
  ============================================================================

        SCD_IIR *stdpcm_16khz_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize a 16 kHz IIR filter structure for standard PCM (G712)
        filtering.

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_IIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_IIR        *stdpcm_16khz_init()
{
  float         **b_cof, **c_cof; /* pointer to numerator/denominator */
  double          K;		  /* coefficients; direct path coef. */
  long            nblocks;	  /* number of 2'nd order blocks */


  /* get pointer to filter-coefficients */
  fill_iir_cof_16khz(&b_cof, &c_cof, &K, &nblocks);

  /* Returns: pointer to SCD_IIR-struct */
  return scd_stdpcm_init(nblocks, /* In: number of 2'nd order blocks */
			 (float (*)[3]) b_cof,	/* In: 24-bit repres. of
						 * numer. coef. */
			 (float (*)[2]) c_cof,	/* In: 24-bit repres. of
						 * denom. coef. */
			 K,	  /* 'K' (direct path coefficient) */
			 1.0,	  /* In: gain factor for filter */
			 (long) 1,/* In: Down-sampling factor */
			 'D');	  /* -> call down-sampling routine */

}
/* ....................... End of stdpcm_16khz_init() ....................... */


/*
  ============================================================================

        SCD_IIR *stdpcm_2_to_1_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize Standard PCM-Filter sampling frequencies for
        "stdpcm_kernel":
         -  input signal: 16 kHz
         -  output signal: 8 kHz

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_IIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_IIR        *stdpcm_2_to_1_init()
{
  float         **b_cof, **c_cof; /* pointer to numerator/denominator */
  double          K;		  /* coefficients; direct path coef. */
  long            nblocks;	  /* number of 2'nd order blocks */


  fill_iir_cof_16khz		  /* get pointer to filter-coefficients */
    (&b_cof, &c_cof, &K, &nblocks);	/* */

  return
    scd_stdpcm_init(		  /* Returns: pointer to SCD_IIR-struct */
		    nblocks,	  /* In: number of 2'nd order blocks */
		    (float (*)[3]) b_cof,	/* In: 24-bit repres. of
						 * numer. coef. */
		    (float (*)[2]) c_cof,	/* In: 24-bit repres. of
						 * denom. coef. */
		    K,		  /* 'K' (direct path coefficient) */
		    1.0,	  /* In: gain factor for filter */
		    (long) 2,	  /* In: Down-sampling factor */
		    'D'		  /* -> call down-sampling routine */
    );
}
/* ..................... End of stdpcm_2_to_1_init() ..................... */



/*
  ============================================================================

        SCD_IIR *stdpcm_1_to_2_init (void);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Initialize Standard PCM-Filter sampling frequencies for
        "stdpcm_kernel":
         -  input signal:   8 kHz
         -  output signal: 16 kHz

        Parameters:  none.
        ~~~~~~~~~~~

        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_IIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
SCD_IIR        *stdpcm_1_to_2_init()
{
  float         **b_cof, **c_cof; /* pointer to numerator/denominator */
  double          K;		  /* coefficients; direct path coef. */
  long            nblocks;	  /* number of 2'nd order blocks */


  /* get pointer to filter-coefficients */
  fill_iir_cof_16khz(&b_cof, &c_cof, &K, &nblocks);

  return
    scd_stdpcm_init(		  /* Returns: pointer to SCD_IIR-struct */
		    nblocks,	  /* In: number of 2'nd order blocks */
		    (float (*)[3]) b_cof,	/* In: 24-bit repres. of
						 * numer. coef. */
		    (float (*)[2]) c_cof,	/* In: 24-bit repres. of
						 * denom. coef. */
		    K,		  /* 'K' (direct path coefficient) */
		    2.0,	  /* In: gain factor for filter */
		    (long) 2,	  /* In: Up-sampling factor */
		    'U'		  /* -> call up-sampling routine */
    );
}
/* ...................... End of stdpcm_1_to_2_init() ...................... */



/*
  ============================================================================

        SCD_IIR *fill_iir_cof_16khz ();
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Return pointer to arrays with IIR coefficients for standard PCM
        filter, running at 16 kHz.

        Parameters:
        ~~~~~~~~~~~
        float ***b_cof .. pointer to numerator filter coefficients
        float ***c_cof .. pointer to denominator filter coefficients
        double *K ....... 'K' (direct path coefficient)
        long *nblocks ... number of coefficients (output value)

        Return value:  None.
        ~~~~~~~~~~~~~

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
void            fill_iir_cof_16khz(b_cof, c_cof, K, nblocks)
  float        ***b_cof;
  float        ***c_cof;
  double         *K;
  long           *nblocks;
{
#define nblocks_16khz        4	  /* number of 2'nd order blocks */
#define f24                  (float)0x00800000
#define K_direct_path_16khz  -0.620901	/* 'K' (direct path coefficient) */


  /* Numerator coefficients */
  static float b_16khz[4][3] = 
                           { /*       T[L]1   ,      T[L]2 */
                            { 0.,  1990214./f24, -4831226./f24},
                            { 0., - 891214./f24,   390775./f24},
                            { 0.,   188047./f24, - 190086./f24},
                            { 0., -7026995./f24,  7461919./f24}
                           };


  /* (Denominator coefficients) */
  static float  c_16khz[4][2] =
                           { /*  T[L]3   ,      T[L]4 */
                            { 1599397./f24, -2059789./f24},
                            { 3376264./f24, -1250280./f24},
                            { 4020626./f24, -8195142./f24},
                            { 1243410./f24, -5159623./f24}
                           };


  *nblocks = nblocks_16khz;
  *b_cof = (float **) b_16khz;
  *c_cof = (float **) c_16khz;
  *K = K_direct_path_16khz;
}
#undef K_direct_path_16khz
#undef f24
#undef nblocks_16khz
/* ...................... End of fill_iir_cof_16khz() ...................... */

/* ************************** END OF IIR-G712.C *************************** */


