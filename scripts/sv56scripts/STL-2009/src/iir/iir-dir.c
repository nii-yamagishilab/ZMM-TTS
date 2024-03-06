/*                                                               v1.0 31.Jul.95
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
       CODING STANDARDS".  Also, please note the original authorship
       copyrights in the body of each function, when applicable.
       =============================================================

MODULE:         IIR-DIR.C, IIR FILTER MODULE, 
                Sub-unit with direct form IIR filtering for data sampled 
                at up- and down-sampling.

ORIGINAL BY:
          Simao Ferraz de Campos Neto (C code)
	  Comsat Laboratories                  Tel:    +1-301-428-4516
	  22300 Comsat Drive                   Fax:    +1-301-428-9287
	  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

          Rosario Drogo de Iaccovo (Filter design)
          CSELT
          Audio Coding and Transm.Quality  Tel:    +39-11-228-5697
          Via G.Reiss Romoli, 274	   Fax:    +39-11-228-6207
          10148 Torino	 E-mail: Rosario.DrogodeIaccovo@cselt.stet.it
          ITALY

DESCRIPTION:

        This file contains functions for initializing and running an
        IIR-filter (direct form) with a DC removal filter (factor 1:1). 

FUNCTIONS    : - fill_iir_dc_removal = coeffs for cascade-form IIR DC removal
                                       filter (1:1 factor)
               - iir_dc_removal_init() = initialization of cascade-form IIR 
                                      DC removal filter (factor 1:1)

NOTE: DO NOT USE CODE BETWEEN THE "UNDER_HEAVY_CONSTRUCTION" BLOCKS!!!

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
void fill_iir_dc_removal ARGS((float **a_cof, float **b_cof, long *zerono, long *poleno));

/* External function prototypes - code in in iir-lib.c */
extern DIRECT_IIR *direct_iir_init ARGS((long zerono, long poleno, float *a, 
			 float *b, double gain, long idown, int hswitch));

#ifdef UNDER_HEAVY_CONSTRUCTION
/*
  ============================================================================

  void fill_iir_lp_3_to_1 (float **a_cof, float **b_cof, long *zerono, 
  ~~~~~~~~~~~~~~~~~~~~~~~  long *poleno);

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to arrays with IIR coefficients for direct-form IIR 
  3:1/1:3 rate change filter. The coefficients used were provided by Rosario 
  Drogo de Iaccovo (CSELT/Italy) the ITU-T wideband speech codec qualification
  tests. The coefficients are of a filter in the direct form.

  Parameters:
  ~~~~~~~~~~~
  float **a_cof .. pointer to numerator filter coefficients
  float **b_cof .. pointer to denominator filter coefficients
  long *zerono ... number of zeroes (output value)
  long *poleno ... number of poles (output value)

  Return value:  None.
  ~~~~~~~~~~~~~

        Author: 
        ~~~~~~~
        Filter coefficients: 
          Rosario Drogo de Iaccovo
          CSELT
          Audio Coding and Transm.Quality  Tel:    +39-11-228-5697
          Via G.Reiss Romoli, 274	   Fax:    +39-11-228-6207
          10148 Torino			   ITALY
          Copyright Notice:
          " (C) 1994 CSELT - Centro Studi E Laboratori Telecomunicazioni SpA "
          " All rights reserved "
          
        Routine:
          Simao Ferraz de Campos Neto
	  Comsat Laboratories                  Tel:    +1-301-428-4516
	  22300 Comsat Drive                   Fax:    +1-301-428-9287
	  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Created <simao@ctd.comsat.com>

 ============================================================================
*/
void fill_iir_lp_3_to_1 (a_cof, b_cof, zerono, poleno)
  float        **a_cof;
  float        **b_cof;
  long           *zerono, *poleno;
{
#define POLENO        24	  /* number of poles in direct form */
#define ZERONO        24	  /* number of zeroes in direct form */

  /* Numerator coefficients (zeroes) */
  static float a_lp_3_1[ZERONO] = 
  { 5.4968171e-04,-6.8272329e-05, 3.1475655e-03, 1.2150722e-03,
    8.6479028e-03, 7.2179878e-03, 1.7002962e-02, 1.8660073e-02, 
    2.7400730e-02, 3.1055931e-02, 3.6173885e-02, 3.8048666e-02, 
    3.8048666e-02, 3.6173885e-02, 3.1055931e-02, 2.7400730e-02, 
    1.8660073e-02, 1.7002962e-02, 7.2179878e-03, 8.6479028e-03, 
    1.2150722e-03, 3.1475655e-03,-6.8272329e-05, 5.4968171e-04
  };

  /* Denominator coefficients (poles) */
  static float  b_lp_3_1[POLENO] =
  { 1.0000000e+00,-6.0756172e+00, 2.1006057e+01,-5.0217794e+01, 
    9.2038231e+01,-1.3508820e+02, 1.6398570e+02,-1.6740764e+02, 
    1.4576856e+02,-1.0897157e+02, 7.0403019e+01,-3.9334337e+01, 
    1.9053915e+01,-7.9633036e+00, 2.8739011e+00,-8.8310070e-01, 
    2.3220561e-01,-5.0145585e-02, 9.2944661e-03,-1.2334147e-03, 
    1.7203837e-04,-3.8684472e-06, 2.3653695e-06, 2.8700440e-07
  };


  *zerono = ZERONO;
  *poleno = POLENO;
  *a_cof = (float *) a_lp_3_1;
  *b_cof = (float *) b_lp_3_1;
}
#undef POLENO
#undef ZERONO
/* ..................... End of fill_iir_lp_3_to_1() ...................... */



/*
  ============================================================================

  DIRECT_IIR *iir_down_3_to_1_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Initialize a direct-for IIR filter structure for 3:1 downsampling filtering. 
  Coefficients init'd by fill...() & gain furnished by CSELT for the
  ITU-T wideband speech coder qualification tests.

  Parameters:  none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct DIRECT_IIR;
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  30.Sep.94 v1.0 Release of 1st version <simao>

 ============================================================================
*/
DIRECT_IIR        *iir_down_3_to_1_init()
{
  float         *a_cof, *b_cof; /* pointer to numerator/denominator */
  long            poleno;	  /* number of poles */
  long            zerono;	  /* number of zeroes */


  fill_iir_lp_3_to_1		  /* get pointer to filter-coefficients */
    (&a_cof, &b_cof, &zerono, &poleno);

  return direct_iir_init(	  /* Returns: pointer to DIRECT_IIR-struct */
			 zerono, /* In: number of zeroes */
			 poleno, /* In: number of poles */
			 (float *) a_cof,	/* Zeroes */
			 (float *) b_cof,	/* Poles */
			 1.0,                   /* In: gain for filter */
			 (long) 3,   /* In: Down-sampling factor */
			 'D');	     /* In: call down-sampling routine */
}
/* ................... End of iir_down_3_to_1_init() ................... */


/*
  ============================================================================

  DIRECT_IIR *iir_up_1_to_3_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Initialize a direct-for IIR filter structure for 1:3 upsampling filtering. 
  Coefficients init'd by fill...() & gain furnished by CSELT for the
  ITU-T wideband speech coder qualification tests.

  Parameters:  none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct DIRECT_IIR;
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao>

 ============================================================================
*/
DIRECT_IIR        *iir_up_1_to_3_init()
{
  float         *a_cof, *b_cof; /* pointer to numerator/denominator */
  long            poleno;	  /* number of poles */
  long            zerono;	  /* number of zeroes */


  fill_iir_lp_3_to_1		  /* get pointer to filter-coefficients */
    (&a_cof, &b_cof, &zerono, &poleno);

  return direct_iir_init(	  /* Returns: pointer to DIRECT_IIR-struct */
			 zerono, /* In: number of zeroes */
			 poleno, /* In: number of poles */
			 (float *) a_cof,	/* Zeroes */
			 (float *) b_cof,	/* Poles */
			 1.0,                   /* In: gain for filter */
			 (long) 3,   /* In: Down-sampling factor */
			 'U');	     /* In: call up-sampling routine */
}
/* ................... End of iir_up_1_to_3_init() ................... */

#endif /* UNDER_HEAVY_CONSTRUCTION */


/*
  ============================================================================

  void fill_iir_dc_removal (float **a_cof, float **b_cof, long *zerono, 
  ~~~~~~~~~~~~~~~~~~~~~~~~  long *poleno);

  Description:
  ~~~~~~~~~~~~

  Initialize pointer to arrays with IIR coefficients for direct-form IIR 
  DC removal filter based on the RPELTP algorithm DC removal filter. 
  Alpha here is 0.985 filter. The coefficients are of a filter in the 
  direct form.
                                   -1
                              1 - z
                   H(z) = ---------------
                                      -1
                           1 - alpha*z

  Parameters:
  ~~~~~~~~~~~
  float **a_cof .. pointer to numerator filter coefficients
  float **b_cof .. pointer to denominator filter coefficients
  long *zerono ... number of zeroes (output value)
  long *poleno ... number of poles (output value)

  Return value:  None.
  ~~~~~~~~~~~~~

        Author: 
        ~~~~~~~
          Simao Ferraz de Campos Neto
	  Comsat Laboratories                  Tel:    +1-301-428-4516
	  22300 Comsat Drive                   Fax:    +1-301-428-9287
	  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Created <simao@ctd.comsat.com>

 ============================================================================
*/
void fill_iir_dc_removal (a_cof, b_cof, zerono, poleno)
  float        **a_cof;
  float        **b_cof;
  long           *zerono, *poleno;
{
#define POLENO        2	  /* number of poles in direct form + 1 */
#define ZERONO        2	  /* number of zeroes in direct form + 1 */

  /* Numerator coefficients (zeroes) */
  static float a_dc[ZERONO] = {1, -1};


  /* Denominator coefficients (poles) */
  static float  b_dc[POLENO] = {1, -0.985};

  *zerono = ZERONO;
  *poleno = POLENO;
  *a_cof = (float *) a_dc;
  *b_cof = (float *) b_dc;
}
#undef POLENO
#undef ZERONO
/* .................... End of fill_iir_dc_removal() ..................... */


/*
  ============================================================================

  DIRECT_IIR *iir_dir_dc_removal_init (void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Initialize a direct-for IIR filter structure for a 1:1 DC removal filtering. 

  Parameters:  none.
  ~~~~~~~~~~~

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct DIRECT_IIR;
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao>

 ============================================================================
*/
DIRECT_IIR        *iir_dir_dc_removal_init()
{
  float         *a_cof, *b_cof; /* pointer to numerator/denominator */
  long            poleno;	  /* number of poles */
  long            zerono;	  /* number of zeroes */


  fill_iir_dc_removal		  /* get pointer to filter-coefficients */
    (&a_cof, &b_cof, &zerono, &poleno);

  return direct_iir_init(	  /* Returns: pointer to DIRECT_IIR-struct */
			 zerono, /* In: number of zeroes */
			 poleno, /* In: number of poles */
			 (float *) a_cof,	/* Zeroes */
			 (float *) b_cof,	/* Poles */
			 1.0,                   /* In: gain for filter */
			 (long) 1,   /* In: Down-sampling factor */
			 'D');	     /* In: call up-sampling routine */
}
/* .................. End of iir_dir_dc_removal_init() .................. */


/* *************************** END OF IIR-DIR.C **************************** */
