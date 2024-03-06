/*                                                         v1.0 30.OCT.94 18H00
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

MODULE:         CASCG712.C, IIR FILTER MODULE, 
                Sub-unit with the cascade-form standard PCM filter (G.712)

ORIGINAL BY:
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

DESCRIPTION:
        This file contains functions for initializing an
        IIR-filter (cascade form) with a standard PCM frequency
        response for only the sampling frequency 8 kHz.

FUNCTIONS    : - iir_G712_8khz_init = initialization of cascade-form IIR G.712
	       - fill_iir_G712_8khz = coeffs for cascade-form IIR G.712

HISTORY:

    30.Oct.94 v1.0 Release of 1st version <simao@ctd.comsat.com>

  =============================================================================
*/
#ifndef VMS
#include <stdlib.h>		  /* General utility definitions */
#endif
#include <stdio.h>		  /* UNIX Standard I/O Definitions */
#include <math.h>		  /* RTL Math Function Declarations */

#include "iirflt.h"

/* Local function prototypes */
void fill_iir_G712_8khz ARGS((float ***a_cof, float ***b_cof, long *nblocks));

/* External function prototypes - code in in iir-lib.c */
extern CASCADE_IIR *cascade_iir_init ARGS((long nblocks, float (*a)[2], 
			 float (*b)[2], double gain, long idown, int hswitch));

/*
  ============================================================================

  void fill_iir_G712_8khz ();
  ~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to arrays with IIR coefficients for IIR G712
  filter, running at 8 kHz. The coefficients used were given
  by John Rosenberger (Bellcore, USA), what are of a filter 
  in the cascade form. 

  Parameters:
  ~~~~~~~~~~~
  float ***a_cof .. pointer to numerator filter coefficients
  float ***b_cof .. pointer to denominator filter coefficients
  long *nblocks ... number of coefficients (output value)

  Return value:  None.
  ~~~~~~~~~~~~~

  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  30.Sep.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
void            fill_iir_G712_8khz(a_cof, b_cof, nblocks)
  float        ***a_cof;
  float        ***b_cof;
  long           *nblocks;
{
#define nblocks_8khz        2	  /* number of 2'nd order blocks */

  /* Numerator coefficients */
  static float a_G712_8khz[nblocks_8khz][2] = 
                           { /*       T[L]1   ,      T[L]2 */
			     {0.197140840E+01, 0.100000000E+01},
			     {-0.199301310E+01, 0.100000000E+01}
			   };

  /* Denominator coefficients */
  static float  b_G712_8khz[nblocks_8khz][2] =
                           { /*  T[L]3   ,      T[L]4 */
			     {0.156814950E+01, 0.690445310E+00},
			     {-0.179704400E+01, 0.830129300E+00}
                           };


  *nblocks = nblocks_8khz;
  *a_cof = (float **) a_G712_8khz;
  *b_cof = (float **) b_G712_8khz;
}
#undef nblocks_8khz
/* ..................... End of fill_iir_G712_8khz() ...................... */



/*
  ============================================================================

  CASCADE_IIR *iir_G712_8khz_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  Description:
  ~~~~~~~~~~~~
  Initialize a 8 kHz IIR filter structure for G712 (P.48) filtering. 
  Coefficients init'd by fill...() and gain furnished by Bellcore. The factor 
  for this function is 1:1.

  Parameters:  none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct CASCADE_IIR;

  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  30.Sep.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
CASCADE_IIR        *iir_G712_8khz_init()
{
  float         **a_cof, **b_cof; /* pointer to numerator/denominator */
  long            nblocks;	  /* number of 2'nd order blocks */


  fill_iir_G712_8khz		  /* get pointer to filter-coefficients */
    (&a_cof, &b_cof, &nblocks);

  return cascade_iir_init(	  /* Returns: pointer to CASCADE_IIR-struct */
			 nblocks, /* In: number of 2'nd order blocks */
			 (float (*)[2]) a_cof,	/* In: 24-bit repres. of
						 * numer. coef. */
			 (float (*)[2]) b_cof,	/* In: 24-bit repres. of
						 * denom. coef. */
			 0.695296250,  /* In: gain factor for filter */
			 (long) 1,/* In: Down-sampling factor */
			 'D');	  /* -> call down-sampling routine */

}
/* ...................... End of iir_G712_8khz_init() ...................... */


/* ************************* END OF FILE CASCG712.C ************************ */
