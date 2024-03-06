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

MODULE:         IIR-IRS.C, IIR FILTER MODULE, 
		Sub-unit with IRS IIR filtering for data sampled at 8kHz.

ORIGINAL BY:
  Simao Ferraz de Campos Neto
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

DESCRIPTION:

        This file contains functions for initializing and running an
        IIR-filter (cascade form) with a standard IRS weighting
        (sampling frequency = 8 kHz). 

FUNCTIONS    : - iir_irs_8khz_init = initialization of cascade-form IIR IRS
	       - fill_iir_irs_8khz = coeffs for cascade-form IIR IRS

HISTORY:

    30.Oct.94 v1.0 Created <simao@ctd.comsat.com>

  =============================================================================
*/

/* General includes */
#ifndef VMS
#include <stdlib.h>		  /* General utility definitions */
#endif
#include <stdio.h>		  /* UNIX Standard I/O Definitions */
#include <math.h>		  /* RTL Math Function Declarations */

/* This module's prototypes */
#include "iirflt.h"

/* Local function prototypes */
void fill_iir_irs_8khz ARGS((float ***a_cof, float ***b_cof, long *nblocks));

/* External function prototypes - code in in iir-lib.c */
extern CASCADE_IIR *cascade_iir_init ARGS((long nblocks, float (*a)[2], 
			 float (*b)[2], double gain, long idown, int hswitch));


/*
  ============================================================================

  void fill_iir_irs_8khz (float ***a_cof, float ***b_cof, long *nblocks);
  ~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to arrays with IIR coefficients for IIR IRS filter,
  running at 8 kHz. The coefficients used were given by John Rosenberger
  (Bellcore, USA) for the ITU-T 8 kbit/s speech coder qualification tests.
  The coefficients are of a filter in the cascade form.

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
void            fill_iir_irs_8khz(a_cof, b_cof, nblocks)
  float        ***a_cof;
  float        ***b_cof;
  long           *nblocks;
{
#define nblocks_8khz        8	  /* number of 2'nd order blocks */

  /* Numerator coefficients */
  static float a_irs_8khz[nblocks_8khz][2] = 
                           { /*       T[L]1   ,      T[L]2 */
			     { 0.181181010E+01, -0.278533980E+01},
			     {-0.341588540E+01,  0.241742110E+01},
			     { 0.106290280E+01,  0.637687860E+00},
			     {-0.114114690E+02,  0.105993240E+02},
			     { 0.533929010E+01,  0.431985380E+01},
			     {-0.174458730E+01, -0.110326430E+02},
			     { 0.122531430E-01, -0.102518530E+01},
			     {-0.288089440E-01, -0.995417300E+00}
			   };

  /* Denominator coefficients */
  static float  b_irs_8khz[nblocks_8khz][2] =
                           { /*  T[L]3   ,      T[L]4 */
			     { 0.117483180E+00, -0.315495510E-01},
			     {-0.176085970E+01,  0.803684710E+00},
			     { 0.736577630E+00,  0.447875650E+00},
			     {-0.110519770E+01,  0.425873550E+00},
			     { 0.154399700E+01,  0.761976480E+00},
			     {-0.189045870E+01,  0.910008010E+00},
			     { 0.806640680E+00,  0.132415190E+00},
			     { 0.105510470E+01,  0.522127990E+00}
                           };


  *nblocks = nblocks_8khz;
  *a_cof = (float **) a_irs_8khz;
  *b_cof = (float **) b_irs_8khz;
}
#undef nblocks_8khz
/* ..................... End of fill_iir_irs_8khz() ...................... */



/*
  ============================================================================

  CASCADE_IIR *iir_irs_8khz_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Initialize a 8 kHz IIR filter structure for IRS (P.48) filtering. 
  Coefficients init'd by fill...() & gain furnished by Bellcore for the
  ITU-T 8 kbit/s speech coder qualification tests.

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
CASCADE_IIR        *iir_irs_8khz_init()
{
  float         **a_cof, **b_cof; /* pointer to numerator/denominator */
  long            nblocks;	  /* number of 2'nd order blocks */


  fill_iir_irs_8khz		  /* get pointer to filter-coefficients */
    (&a_cof, &b_cof, &nblocks);

  return cascade_iir_init(	  /* Returns: pointer to CASCADE_IIR-struct */
			 nblocks, /* In: number of 2'nd order blocks */
			 (float (*)[2]) a_cof,	/* In: 24-bit repres. of
						 * numer. coef. */
			 (float (*)[2]) b_cof,	/* In: 24-bit repres. of
						 * denom. coef. */
			 1.05133050E-04,  /* In: gain factor for filter */
			 (long) 1,/* In: Down-sampling factor */
			 'D');	  /* -> call down-sampling routine */

}
/* ...................... End of iir_irs_8khz_init() ...................... */


/* *************************** END OF IIR_IRS.C **************************** */
