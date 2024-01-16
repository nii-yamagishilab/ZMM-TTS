/*                                                               v1.0 31.JUL.95
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

MODULE:         IIR-FLAT.C, IIR FILTER MODULE, 
		Sub-unit with flat low-pass IIR filtering

ORIGINAL BY:
  Simao Ferraz de Campos Neto
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

DESCRIPTION:

        This file contains functions for initializing and running an
        IIR-filter (cascade form) with a low-pass characteristic for 3:1 
        and 1:3 rate change factors. 

FUNCTIONS    : - iir_casc_lp_3_to_1_init = init. of cascade-form IIR LP
	       - fill_casc_iir_lp_3_to_1 = coeffs for cascade-form IIR LP

HISTORY:

    31.Jul.95 v1.0 Created <simao@ctd.comsat.com>

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
void fill_casc_iir_lp_3_to_1 ARGS((float ***a_cof, float ***b_cof, long *nblocks));

/* External function prototypes - code in in iir-lib.c */
extern CASCADE_IIR *cascade_iir_init ARGS((long nblocks, float (*a)[2], 
			 float (*b)[2], double gain, long idown, int hswitch));


/*
  ============================================================================

  void fill_casc_iir_lp_3_to_1 (float ***a_cof, float ***b_cof, long *nblocks);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to arrays with IIR coefficients for IIR flat band-pass 
  filter for rate change factor of 1:3/3:1. The coefficients used were 
  generated using Hypersignal for a ellyptic design and implemented in the 
  direct form II cascade. The filter has order 13 and have 7 sections.

  Filtering is implemented using:
                N
             _______              -1        -2
               | |  {  1  +  A1i z  +  A2i z  }
       H(z)= K | |   -------------------------- 
               i=1                -1        -2
                    {  1  +  B1i z  +  B2i z  }


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
void            fill_casc_iir_lp_3_to_1_ravi2(a_cof, b_cof, nblocks)
  float        ***a_cof;
  float        ***b_cof;
  long           *nblocks;
{
#define nblocks_3_1        12	  /* number of 2'nd order blocks */

  /* Numerator coefficients */
  static float a_iir_3_1[nblocks_3_1][2] = 
                           { /*       z-1 ,      z-2 */
			     {-1, 0},
			     {-1.78633, 1},
			     {-1.27455, 1},
			     {-0.697086, 1},
			     {-0.195802, 1},
			     {0.191821, 1.00008},
			     {0.476488, 1.00072},
			     {0.684385, 0.99976},
			     {0.814775, 0.993167},
			     {1.01303, 1.00853},
			     {0.955143, 1.03644},
			     {0.980876, 0.98494},
			   };

  /* Denominator coefficients */
  static float  b_iir_3_1[nblocks_3_1][2] =
                           { /*  T[L]3   ,      T[L]4 */
			     {-14.5254, 0},
			     {-4.13681, 39.6786},
			     {-0.128857, 12.0835},
			     {0.751788, 5.96406},
			     {1.07085, 1.38994},
			     {1.0612, 3.72064},
			     {1.08917, 1.05086},
			     {1.14703, 2.0356},
			     {1.22446, 2.71153},
			     {1.24262, 1.03462},
			     {1.46873, 1.38843},
			     {1.49368, 1.94557},
                           };

#define gain_ravi2 4.0774e-06

  *nblocks = nblocks_3_1;
  *a_cof = (float **) a_iir_3_1;
  *b_cof = (float **) b_iir_3_1;
}
#undef nblocks_3_1

void            fill_casc_iir_lp_3_to_1_ravi1(a_cof, b_cof, nblocks)
  float        ***a_cof;
  float        ***b_cof;
  long           *nblocks;
{
#define nblocks_3_1        12	  /* number of 2'nd order blocks */

  /* Numerator coefficients */
  static float a_iir_3_1[nblocks_3_1][2] = 
                           { /*       z-1 ,      z-2 */
			     {-1, 0},
			     {-1.78633, 0.999999},
			     {-1.27455, 0.999999},
			     {-0.697086, 1},
			     {-0.195802, 1},
			     {0.191806, 0.999921},
			     {0.476144, 0.999278},
			     {0.684549, 1.00024},
			     {0.820381, 1.00688},
			     {1.00447, 0.991547},
			     {0.921565, 0.964845},
			     {0.995874, 1.01529},
			   };

  /* Denominator coefficients */
  static float  b_iir_3_1[nblocks_3_1][2] =
                           { /*  T[L]3   ,      T[L]4 */
			     {-14.5254, 0},
			     {-0.104258, 0.0252025},
			     {-0.0106639, 0.0827577},
			     {0.126053, 0.167671},
			     {0.770429, 0.719456},
			     {0.28522, 0.268771},
			     {1.03645, 0.951598},
			     {0.563482, 0.491255},
			     {0.451575, 0.368796},
			     {1.20104, 0.966538},
			     {1.05783, 0.720236},
			     {0.767732, 0.513987}
                           };


  *nblocks = nblocks_3_1;
  *a_cof = (float **) a_iir_3_1;
  *b_cof = (float **) b_iir_3_1;
}
#undef nblocks_3_1

void            fill_casc_iir_lp_3_to_1(a_cof, b_cof, nblocks)
  float        ***a_cof;
  float        ***b_cof;
  long           *nblocks;
{
#define nblocks_3_1        7	  /* number of 2'nd order blocks */

  /* Numerator coefficients */
  static float a_iir_3_1[nblocks_3_1][2] = 
                           {  /*       z-1 ,      z-2 */
			     {  1.00000000000,  0.00000000000 },
			     { -1.08311748126,  1.00000000000 },
			     { -1.04805345999,  1.00000000000 },
			     { -0.956974481706, 1.00000000000 },
			     {  0.859312578283, 1.00000000000 },
			     { -0.748981579387, 1.00000000000 },
			     { -0.261004187144, 1.00000000000 }
			   };

  /* Denominator coefficients */
  static float  b_iir_3_1[nblocks_3_1][2] =
                           { /*       z-1 ,      z-2 */
			     { -0.741393933361, 0.000000000000 },
			     { -1.44635266594,  0.599869375555 },
			     { -1.36536866685,  0.711998423234 },
			     { -1.28688150301,  0.822500869328 },
			     { -1.23342027831,  0.902175203027 },
			     { -1.20604048405,  0.952767955206 },
			     { -1.20077042454,  0.986028477813 }
                          };


  *nblocks = nblocks_3_1;
  *a_cof = (float **) a_iir_3_1;
  *b_cof = (float **) b_iir_3_1;
}
#undef nblocks_3_1
/* ..................... End of fill_casc_iir_lp_3_to_1() ..................... */



/*
  ============================================================================

  CASCADE_IIR *iir_casc_lp_3_to_1_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
CASCADE_IIR        *iir_casc_lp_3_to_1_init()
{
  float         **a_cof, **b_cof; /* pointer to numerator/denominator */
  long            nblocks;	  /* number of 2'nd order blocks */


  fill_casc_iir_lp_3_to_1	      /* get pointer to filter-coefficients */
    (&a_cof, &b_cof, &nblocks);

  return cascade_iir_init(	  /* Returns: pointer to CASCADE_IIR-struct */
			 nblocks, /* In: number of 2'nd order blocks */
			 (float (*)[2]) a_cof,  /* Numerator. coef. */
			 (float (*)[2]) b_cof,	/* Denom. coef. */
			 2.552013334E-04,    /* In: gain factor for filter */
			 (long) 3,           /* In: Down-sampling factor */
			 'D');	             /* In: call down-sampl. routine */

}
/* ................... End of iir_casc_lp_3_to_1_init() ................... */



/*
  ============================================================================

  CASCADE_IIR *iir_casc_lp_1_to_3_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
CASCADE_IIR        *iir_casc_lp_1_to_3_init()
{
  float         **a_cof, **b_cof; /* pointer to numerator/denominator */
  long            nblocks;	  /* number of 2'nd order blocks */


  fill_casc_iir_lp_3_to_1	      /* get pointer to filter-coefficients */
    (&a_cof, &b_cof, &nblocks);

  return cascade_iir_init(	  /* Returns: pointer to CASCADE_IIR-struct */
			 nblocks, /* In: number of 2'nd order blocks */
			 (float (*)[2]) a_cof,  /* Numerator. coef. */
			 (float (*)[2]) b_cof,	/* Denom. coef. */
			 3. * 2.552013334E-04, /* In: gain factor for filter */
			 (long) 3,/* In: Up-sampling factor */
			 'U');	  /* -> call up-sampling routine */

}
/* ................... End of iir_casc_lp_1_to_3_init() ................... */


/* ************************** END OF IIR-FLAT.C *************************** */
