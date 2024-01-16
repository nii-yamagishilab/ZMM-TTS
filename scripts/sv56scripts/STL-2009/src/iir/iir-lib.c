/*                                                           v3.1 - 22/Feb/1996
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

MODULE:         IIR-LIB.C, IIR FILTER MODULE
		Sub-unit with basic filtering routines

ORIGINAL BY:
          Rudolf Hofmann (Parallel form)
	  PHILIPS KOMMUNIKATIONS INDUSTRIE 
	  Kommunikationssysteme         Phone : +49 911 526-2603
	  Thurn-und-Taxis-Strasse 14    FAX   : +49 911 526-3385
	  D-8500 Nuernberg 10 (Germany) EMail : hf@pkinbg.uucp

          Simao Ferraz de Campos Neto (Cascade and direct form)
	  Comsat Laboratories                  Tel:    +1-301-428-4516
	  22300 Comsat Drive                   Fax:    +1-301-428-9287
	  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

DESCRIPTION:

        This file contains functions for running an IIR-filter with 
        coefficients in the parallel or the cascade (bi-quad) form.

FUNCTIONS    : - stdpcm_kernel(...)   =  parallel-form IIR filter (kernel)
               - stdpcm_reset(...)    =  clear parallel state variables
                                         (needed only if another signal
                                          should be processed with the
                                          same filter)
               - stdpcm_free(...)     =  deallocate parallel filter memory
	       - cascade_iir_kernel(...) = cascade-form IIR filter (kernel)
	       - cascade_iir_free(...) = deallocate cascade filter memory
	       - cascade_iir_reset(...) = clear cascade state variables
	       - direct_iir_kernel(...) = direct-form IIR filter (kernel)
	       - direct_iir_free(...) = deallocate direct filter memory
	       - direct_iir_reset(...) = clear direct state variables
HISTORY:

    16.Dec.91 v0.1 First beta-version <hf@pkinbg.uucp>
    28.Feb.92 v1.0 Release of 1st version to UGST <hf@pkinbg.uucp>
    30.Oct.94 v2.0 Broken into sub-units; added cascade-form IIR filtering.
                   <simao@ctd.comsat.com>
    31.Jul.95 v3.0 Added direct-form IIR filtering <simao@ctd.comsat.com>
    22.Feb.96 v3.1 Changed inclusion of stdlib.h to inconditional, as
                   suggested by Kirchherr (FI/DBP Telekom) to run under
		   OpenVMS/AXP <simao@ctd.comsat.com>

  =============================================================================
*/


/*
 * ......... GENERAL INCLUDES .........
 */

#include <stdlib.h>		  /* General utility definitions */
#include <math.h>		  /* RTL Math Function Declarations */

/* Definitions for IIR filters */
#include "iirflt.h"		  



/*
 * ......... Local function *smart* prototypes .........
 */
/* Parallel-form filtering basic function prototypes */
static long scd_parallel_form_iir_down_kernel ARGS((long lenx, float *x, 
			 float *y, long *k0, long idown, long nblocks, 
                         double direct_cof, double gain, float (*b)[3], 
                         float (*c)[2], float (*T)[2]));
static long scd_parallel_form_iir_up_kernel ARGS((long lenx, float *x, 
			 float *y, long iup, long nblocks, double direct_cof, 
			 double gain, float (*b)[3], float (*c)[2], 
			 float (*T)[2]));

SCD_IIR *scd_stdpcm_init ARGS((long nblocks, float (*b)[3], float (*c)[2], 
                         double direct_cof, double gain, long idown, 
                         int hswitch));

long cascade_iir_kernel ARGS((long lseg, float *x_ptr, CASCADE_IIR *iir_ptr, 
                         float *y_ptr));

/* Cascade-form filtering basic function prototypes */
static long cascade_form_iir_down_kernel ARGS((long lenx, float *x, float *y, 
                         long *k0, long idown, long nblocks, double gain, 
                         float (*a)[2], float (*b)[2], float (*T)[4]));

static long cascade_form_iir_up_kernel ARGS((long lenx, float *x, float *y, 
                         long iup, long nblocks, double gain, float (*a)[2], 
                         float (*b)[2], float (*T)[4]));

CASCADE_IIR *cascade_iir_init ARGS((long nblocks, float (*a)[2], float (*b)[2],
                         double gain, long idown, int hswitch));


/* Direct-form filtering basic function prototypes */
long direct_iir_kernel ARGS((long lseg, float *x_ptr, DIRECT_IIR *iir_ptr, 
                         float *y_ptr));

static long direct_form_iir_down_kernel ARGS((long lenx, float *x, float *y, 
                         long *k0, long idown, long zno, long pno, 
			 double gain, float *a, float *b, float (*T)[2]));

static long direct_form_iir_up_kernel ARGS((long lenx, float *x, float *y, 
                         long iup, long zno, long pno, double gain, 
                         float *a, float *b, float (*T)[2]));

DIRECT_IIR *direct_iir_init ARGS((long zno, long pno, float *a, float *b,
                         double gain, long idown, int hswitch));


/*
 * ...................... BEGIN OF FUNCTIONS .........................
 */

/*
  ============================================================================

        void stdpcm_free (SCD_IIR *iir_ptr);
        ~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Deallocate memory, which was allocated by an earlier call to one
        of the initilization routines. `SCD_IIR *iir_prt' must not be
        NULL.

        Parameters:
        ~~~~~~~~~~~
        SCD_IIR *iir_ptr: ... pointer to struct SCD_IIR previously
                              initialized by a call to one of the
                              initialization routines.

        Return value:
        ~~~~~~~~~~~~~
        Nothing.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
void            stdpcm_free(iir_ptr)
  SCD_IIR        *iir_ptr;
{
  free(iir_ptr->T);		  /* free state variables */
  free(iir_ptr);		  /* free allocated struct */
}
/* ....................... End of stdpcm_free() ....................... */



/*
  ============================================================================

        void stdpcm_reset (SCD_IIR *iir_ptr);
        ~~~~~~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Clear state variables in SCD_IIR structure, which have been
        initialized by a previous call to one of the init functions.

        Parameters:
        ~~~~~~~~~~~
        SCD_IIR *iir_ptr: ... pointer to struct SCD_IIR previously
                              initialized by a call to one of the
                              initialization routines.

        Return value:
        ~~~~~~~~~~~~~
        Nothing.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
void            stdpcm_reset(iir_ptr)
  SCD_IIR        *iir_ptr;
{
  long            n;
  float           (*T_ptr)[2];


  T_ptr = iir_ptr->T;
  for (n = 0; n < iir_ptr->nblocks; n++)
  {
    T_ptr[n][0] = 0.0;
    T_ptr[n][1] = 0.0;
  }

  iir_ptr->k0 = iir_ptr->idown;	  /* modulo counter for down-sampling */
}
/* ....................... End of stdpcm_reset() ....................... */



/*
  ============================================================================

        SCD_IIR *scd_stdpcm_init (long nblocks, float (*b)[3],
        ~~~~~~~~~~~~~~~~~~~~~~~~  float (*c)[2], double direct_cof,
                                  double gain, long idwnup,
                                  char hswitch);

        Description:
        ~~~~~~~~~~~~
        Allocate&initialize struct for parallel IIR down/up-sampling functions.

        Parameters:
        ~~~~~~~~~~~
        long nblocks: ....... number of 2'nd order blocks
        float (*b)[3]: ...... pointer to numerator coeffic.
        float (*c)[3]: ...... pointer to denominator coeffic.
        double direct_cof: .. direct path gain factor
        double gain: ........ gain factor for filter
        long idown: ......... Down-sampling factor
        char hswitch: ....... to up-/downsampling routine


        Return value:
        ~~~~~~~~~~~~~
        Returns a pointer to struct SCD_IIR;

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>
        30.Oct.94 v2.0 Made visible (not static) after module broken into 
                       several sub-units <simao@ctd.comsat.com>

 ============================================================================
*/
SCD_IIR *scd_stdpcm_init(nblocks, b, c, direct_cof, gain, idown, hswitch)
  long            nblocks;
  float           (*b)[3], (*c)[2];
  double          direct_cof, gain;
  long            idown;
  char            hswitch;
{
  static SCD_IIR *ptrIIR;	  /* pointer to the new struct */
  float           fak;
  float           (*T_ptr)[2];
  long            n;


  /* Allocate memory for a new struct */
  if ((ptrIIR = (SCD_IIR *) malloc((long) sizeof(SCD_IIR))) == (SCD_IIR *) 0L)
  {
    return 0;
  }


  /* Allocate memory for state variables */
  if ((ptrIIR->T = (float (*)[2]) malloc((nblocks * 2) * sizeof(fak)))
      == (float (*)[2]) 0)
  {
    free(ptrIIR);
    return 0;
  }


  /* fill coefficient sets */
  ptrIIR->nblocks = nblocks;	  /* store number of 2'nd order blocks */
  ptrIIR->b = b;
  ptrIIR->c = c;


  /* store down-sampling factor/gain/direct-path coefficient */
  ptrIIR->idown = idown;
  ptrIIR->gain = gain;
  ptrIIR->direct_cof = direct_cof;


  /* Store switch to IIR-kernel procedure */
  ptrIIR->hswitch = hswitch;

  /* Clear state variables */
  T_ptr = ptrIIR->T;
  for (n = 0; n < nblocks; n++)
  {
    T_ptr[n][0] = 0.0;
    T_ptr[n][1] = 0.0;
  }

  ptrIIR->k0 = idown;		  /* modulo counter for down-sampling */


  /* Exit returning pointer to struct */
  return (ptrIIR);
}
/* ....................... End of scd_stdpcm_init() ....................... */


/*
  ============================================================================

        long stdpcm_kernel (long lseg, float *x_ptr, SCD_IIR *iir_ptr,
        ~~~~~~~~~~~~~~~~~~  float *y_ptr);

        Description:
        ~~~~~~~~~~~~

        Standard PCM-filter.

        Parameters:
        ~~~~~~~~~~~
        lseg: ...... number of input samples
        x_ptr: ..... array with input samples
        iir_ptr: ... pointer to IIR-struct (SCD_IIR *)
        y_ptr: ..... output samples

        Return value:
        ~~~~~~~~~~~~~
        Returns the number of output samples.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
long            stdpcm_kernel(lseg, x_ptr, iir_ptr, y_ptr)
  long            lseg;
  float          *x_ptr;
  SCD_IIR        *iir_ptr;
  float          *y_ptr;
{

  if (iir_ptr->hswitch == 'U')
    return
      scd_parallel_form_iir_up_kernel(	/* returns number of output samples */
				      lseg,	/* In   : length of input
						 * signal */
				      x_ptr,	/* In   : array with input
						 * samples */
				      y_ptr,	/* Out  : array with output
						 * samples */
				      iir_ptr->idown,	/* In  : downsampling
							 * factor */
				      iir_ptr->nblocks,	/* In  : number of
							 * IIR-coefficients */
				      iir_ptr->direct_cof,/* In: direct path
							   * coefficient */
				      iir_ptr->gain,	/* In  : gain factor */
				      iir_ptr->b,	/* In  : direct path
							 * coefficient */
				      iir_ptr->c,	/* In  : array with
							 * IIR-coefficients */
				      iir_ptr->T	/* InOut: state
							 * variables */
      );
  else
    return
      scd_parallel_form_iir_down_kernel(	/* returns number of output
						 * samples */
					lseg,	/* In   : length of input
						 * signal */
					x_ptr,	/* In   : array with input
						 * samples */
					y_ptr,	/* Out  : array with output
						 * samples */
					&(iir_ptr->k0),	/* InOut:starting index
							 * in x-array */
					iir_ptr->idown,	/* In   : downsampling
							 * factor */
					iir_ptr->nblocks,/* In   : number of
							  * IIR-coefficients */
					iir_ptr->direct_cof,/* In  : direct path
							     * coefficient */
					iir_ptr->gain,	/* In   : gain factor */
					iir_ptr->b,	/* In   : direct path
							 * coefficient */
					iir_ptr->c,	/* In   : array w/
							 * IIR-coefficients */
					iir_ptr->T	/* InOut: state
							 * variables */
      );
}
/* ....................... End of stdpcm_kernel() ....................... */



/*
  ============================================================================

        long scd_parallel_form_iir_down_kernel(long lenx, float *x,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ float *y, long *k0,
                                               long idown, long nblocks,
                                               double direct_cof,
                                               double gain, float (*b)[3],
                                               float (*c)[2], float (*T)[2]);

        Description:
        ~~~~~~~~~~~~

        Function for filtering a sequence of input samples by a
        parallel-form IIR-filter with down-sampling.

        Parameters:
        ~~~~~~~~~~~
        lenx: ........ (In) length of input array x[]
        x: ........... (In) array with input samples
        y: ........... (Out) array with output samples
        k0: .......... (In/Out) pointer to modulo counter
        idown: ....... (In) down-sampling factor
        nblocks: ..... (In) number of coeff. sets
        direct_cof: .. (In) direct path coefficient
        gain: ........ (In) gain factor
        b: ........... (In) numerator coefficients
        c: ........... (In) denominator coefficients
        T: ........... (In/Out) state variables

        Return value:
        ~~~~~~~~~~~~~
        Returns the number of samples filtered.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
static long     scd_parallel_form_iir_down_kernel(lenx, x, y, k0, idown, 
                                         nblocks, direct_cof, gain, b, c, T)
  long            lenx;
  float          *x, *y;
  long           *k0, idown, nblocks;
  double          direct_cof, gain;
  float           (*b)[3], (*c)[2], (*T)[2];
{
  long            kx, ky, n;
  float           Ttmp;


  ky = 0;			  /* starting index in output array (y) */
  for (kx = 0; kx < lenx; kx++)	  /* loop over all input samples */
  {
    if (*k0 % idown == 0)	  /* compute output only every "idown"
				   * samples */
    {
      y[ky] = direct_cof * x[kx]; /* direct path */
      for (n = 0; n < nblocks; n++)	/* loop over all second order filter */
      {
	Ttmp = 2. * (x[kx] - c[n][0] * T[n][0] - c[n][1] * T[n][1]);
	y[ky] += b[n][2] * Ttmp + b[n][1] * T[n][1] + b[n][0] * T[n][0];
	T[n][0] = T[n][1];
	T[n][1] = Ttmp;
      }
      y[ky] *= gain;
      ky++;
    }
    else
    {
      for (n = 0; n < nblocks; n++)
      {
	Ttmp = 2. * (x[kx] - c[n][0] * T[n][0] - c[n][1] * T[n][1]);
	T[n][0] = T[n][1];
	T[n][1] = Ttmp;
      }
    }
    (*k0)++;
  }
  *k0 %= idown;			  /* avoid overflow by (*k0)++ */
  return ky;
}
/* .............. End of scd_parallel_form_iir_down_kernel() .............. */



/*
  ============================================================================

        long scd_parallel_form_iir_up_kernel(long lenx, float *x,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ float *y, long *k0,
                                               long idown, long nblocks,
                                               double direct_cof,
                                               double gain, float (*b)[3],
                                               float (*c)[2], float (*T)[2]);

        Description:
        ~~~~~~~~~~~~

        Function for filtering a sequence of input samples by a
        parallel-form IIR-filter with up-sampling.

        Parameters:
        ~~~~~~~~~~~
        lenx: ........ (In) length of input array x[]
        x: ........... (In) array with input samples
        y: ........... (Out) array with output samples
        iup: ......... (In) up-sampling factor
        nblocks: ..... (In) number of coeff. sets
        direct_cof: .. (In) direct path coefficient
        gain: ........ (In) gain factor
        b: ........... (In) numerator coefficients
        c: ........... (In) denominator coefficients
        T: ........... (In/Out) state variables

        Return value:
        ~~~~~~~~~~~~~
        Returns the number of samples filtered.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
static long     scd_parallel_form_iir_up_kernel(lenx, x, y, iup, nblocks,
				                  direct_cof, gain, b, c, T)
  long            lenx;
  float          *x, *y;
  long            iup, nblocks;
  double          direct_cof, gain;
  float           (*b)[3], (*c)[2], (*T)[2];
{
  long            kx, ky, n;
  float           Ttmp;


  kx = 0;			  /* starting index in input array (x) */
  for (ky = 0; ky < iup * lenx; ky++)	/* loop over all input samples */
  {
    if (ky % iup == 0)		  /* compute output only every "iup" */
    {				  /* samples by taking one input sample */
      y[ky] = direct_cof * x[kx]; /* direct path */
      for (n = 0; n < nblocks; n++)	/* loop over all second order filter */
      {
	Ttmp = 2. * (x[kx] - c[n][0] * T[n][0] - c[n][1] * T[n][1]);
	y[ky] += b[n][2] * Ttmp + b[n][1] * T[n][1] + b[n][0] * T[n][0];
	T[n][0] = T[n][1];
	T[n][1] = Ttmp;
      }
      y[ky] *= gain;
      kx++;
    }
    else
    {
      y[ky] = 0.0;		  /* at other instants feed zero-valued
				   * samples */
      for (n = 0; n < nblocks; n++)
      {
	Ttmp = 2. * (0.0 - c[n][0] * T[n][0] - c[n][1] * T[n][1]);
	y[ky] += b[n][2] * Ttmp + b[n][1] * T[n][1] + b[n][0] * T[n][0];
	T[n][0] = T[n][1];
	T[n][1] = Ttmp;
      }
      y[ky] *= gain;
    }
  }
  return ky;
}
/* ............... End of scd_parallel_form_iir_up_kernel() ............... */



/* *************************************************************************
   ******** THE ROUTINES TO FOLLOW HAVE BEEN ADDED AFTER THE STL92 *********
 * ************************************************************************* */

/*
  ============================================================================

  void cascade_iir_reset (CASCADE_IIR *iir_ptr);
  ~~~~~~~~~~~~~~~~~~~~~~
  
  Description:
  ~~~~~~~~~~~~

  Clear state variables in CASCADE_IIR structure, what has been
  initialized by a previous call to one of the init functions.

  Parameters:
  ~~~~~~~~~~~
  CASCADE_IIR *iir_ptr: ... pointer to struct CASCADE_IIR previously 
                            initialized by a call to one of the initialization 
                            routines.

  Return value:
  ~~~~~~~~~~~~~
  Nothing.

  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  30.Oct.94 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
void            cascade_iir_reset(iir_ptr)
  CASCADE_IIR        *iir_ptr;
{
  long            n;
  float           (*T_ptr)[4];


  T_ptr = iir_ptr->T;
  for (n = 0; n < iir_ptr->nblocks; n++)
  {
    T_ptr[n][0] = 0.0;
    T_ptr[n][1] = 0.0;
    T_ptr[n][2] = 0.0;
    T_ptr[n][3] = 0.0;
  }

  iir_ptr->k0 = iir_ptr->idown;	  /* modulo counter for down-sampling */
}
/* .................... End of cascade_iir_reset() ...................... */


/*
  ============================================================================

  long cascade_iir_kernel (long lseg, float *x_ptr, CASCADE_IIR *iir_ptr,
  ~~~~~~~~~~~~~~~~~~~~~~~  float *y_ptr);

  Description:
  ~~~~~~~~~~~~

  Basic cascade-form IIR filtering routine, for both up- and 
  down-sampling.

  Parameters:
  ~~~~~~~~~~~
  lseg: ...... number of input samples
  x_ptr: ..... array with input samples
  iir_ptr: ... pointer to IIR-struct (CASCADE_IIR *)
  y_ptr: ..... output samples

  Return value:
  ~~~~~~~~~~~~~
  Returns the number of output samples.
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  30.Oct.94 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
long            cascade_iir_kernel(lseg, x_ptr, iir_ptr, y_ptr)
  long            lseg;
  float          *x_ptr;
  CASCADE_IIR    *iir_ptr;
  float          *y_ptr;
{
  if (iir_ptr->hswitch == 'U')
    return
      cascade_form_iir_up_kernel( /* returns number of output samples */
				  lseg,	          /* In : input signal leng. */
				  x_ptr,	  /* In : input sample array */
				  y_ptr,	  /* Out: outp. sample array */
				  iir_ptr->idown, /* In : dwnsmpl.factor */
				  iir_ptr->nblocks, /*In: no.IIR-coeffs */
				  iir_ptr->gain,  /* In : gain factor*/
				  iir_ptr->a,	  /* In : num.coeffs */
				  iir_ptr->b,	  /* In : denom.coeffs */
				  iir_ptr->T	  /* I/O: state vars */
      );
  else
    return
      cascade_form_iir_down_kernel(/* returns number of output samples */
				   lseg,	  /* In : input signal leng. */
				   x_ptr,	  /* In : input sample array */
				   y_ptr,	  /* Out: outp. sample array */
				   &(iir_ptr->k0), /*I/O: start idx,x-array */
				   iir_ptr->idown, /*In : dwnsmpl.factor */
				   iir_ptr->nblocks, /*In:no.of IIR-coeffs */
				   iir_ptr->gain, /* In : gain factor */
				   iir_ptr->a,	  /* In : numerator coeffs */
				   iir_ptr->b,	  /* In : denom.coeffs */
				   iir_ptr->T	  /* I/O: state vars */
      );
}
/* .................... End of cascade_iir_kernel() ....................... */


/*
  ============================================================================

  long cascade_form_iir_down_kernel(long lenx, float *x,
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ float *y, long *k0,
                                    long idown, long nblocks,
                                    double gain, float (*a)[2],
                                    float (*b)[2], float (*T)[4]);
  
  Description:
  ~~~~~~~~~~~~

  Function for filtering a sequence of input samples by a
  cascade-form IIR-filter with down-sampling.
  
  Parameters:
  ~~~~~~~~~~~
  lenx: ........ (In) length of input array x[]
  x: ........... (In) array with input samples
  y: ........... (Out) array with output samples
  k0: .......... (In/Out) pointer to modulo counter
  idown: ....... (In) down-sampling factor
  nblocks: ..... (In) number of coeff. sets
  gain: ........ (In) gain factor
  b: ........... (In) numerator coefficients
  c: ........... (In) denominator coefficients
  T: ........... (In/Out) state variables
  
  Return value:
  ~~~~~~~~~~~~~
  Returns the number of samples filtered.
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  30.Oct.94 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
static long     cascade_form_iir_down_kernel(lenx, x, y, k0, idown, 
                                             nblocks, gain, a, b, T)
  long            lenx;
  float          *x, *y;
  long           *k0, idown, nblocks;
  double          gain;
  float           (*a)[2], (*b)[2], (*T)[4];
{
  long            kx, ky, n;
  double   xj,yj;


  ky = 0;			  /* starting index in output array (y) */
  for (kx = 0; kx < lenx; kx++)	  /* loop over all input samples */
  {
    xj = x[kx]; /* direct path */
    for (n = 0; n < nblocks; n++)	/* loop over all second order filter */
    {
      yj =  xj + a[n][0] * T[n][0] + a[n][1] * T[n][1];
      yj -= (b[n][0] * T[n][2] + b[n][1] * T[n][3]);

      /* Save samples in memory */
      T[n][1] = T[n][0];
      T[n][0] = xj;
      T[n][3] = T[n][2];
      T[n][2] = yj;

      /* The yj of this stage is the xj of the next */
      xj = yj;
    }

    if (*k0 % idown == 0)	  /* compute output only every "idown"
				   * samples */
    {
      /* Apply gain and update y-samples' counter */
      y[ky] = yj * gain;
      ky++;
    }
    (*k0)++;
  }
  *k0 %= idown;			  /* avoid overflow by (*k0)++ */
  return ky;
}
/* .............. End of cascade_form_iir_down_kernel() .............. */


/*
  ============================================================================

  long cascade_form_iir_up_kernel(long lenx, float *x,
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ float *y, long *k0,
                                  long idown, long nblocks,
                                  double gain, float (*a)[2],
                                  float (*b)[2], float (*T)[4]);

  Description:
  ~~~~~~~~~~~~

  Function for filtering a sequence of input samples by a
  cascade-form IIR-filter with up-sampling.

  Parameters:
  ~~~~~~~~~~~
  lenx: ........ (In) length of input array x[]
  x: ........... (In) array with input samples
  y: ........... (Out) array with output samples
  iup: ......... (In) up-sampling factor
  nblocks: ..... (In) number of coeff. sets
  gain: ........ (In) gain factor
  b: ........... (In) numerator coefficients
  c: ........... (In) denominator coefficients
  T: ........... (In/Out) state variables
  
  Return value:
  ~~~~~~~~~~~~~
  Returns the number of samples filtered.
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  30.Oct.94 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
static long     cascade_form_iir_up_kernel(lenx, x, y, iup, nblocks,
					   gain, a, b, T)
  long            lenx;
  float          *x, *y;
  long            iup, nblocks;
  double          gain;
  float           (*a)[2], (*b)[2], (*T)[4];
{
  long            kx, ky, n;
  double xj, yj;

  kx = 0;			  /* starting index in input array (x) */
  for (ky = 0; ky < iup * lenx; ky++)	/* loop over all input samples */
  {
    /* Compute output only every "iup" compute output only every "iup" 
     * samples by taking one input sample direct path OR by using a 
     * zero-valued sample */
    if (ky % iup == 0)		  
      xj = x[kx];                
    else                          
      xj = 0.;

    /* Filter samples through all cascade stages */
    for (n = 0; n < nblocks; n++)    
    {
      yj =  xj + a[n][0] * T[n][0] + a[n][1] * T[n][1];
      yj -= (b[n][0] * T[n][2] + b[n][1] * T[n][3]);

      /* Save samples in memory */
      T[n][1] = T[n][0];
      T[n][0] = xj;
      T[n][3] = T[n][2];
      T[n][2] = yj;

      /* The yj of this stage is the xj of the next */
      xj = yj;
    }
 
    /* Apply the gain and update x counter if needed */
    y[ky] = yj * gain;
    if (ky % iup == 0) kx++;
  }
  return ky;
}
/* ............... End of cascade_form_iir_up_kernel() ............... */


/*
  ============================================================================

  CASCADE_IIR *cascade_iir_init (long nblocks, float (*a)[2],
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  float (*b)[2], double gain, 
	                         long idwnup, char hswitch);

  Description:
  ~~~~~~~~~~~~
  Allocate & initialize struct for down/up-sampling procedures.

  Parameters:
  ~~~~~~~~~~~
  long nblocks: ....... number of 2'nd order blocks
  float (*a)[2]: ...... pointer to numerator coeffic.
  float (*b)[2]: ...... pointer to denominator coeffic.
  double gain: ........ gain factor for filter
  long idown: ......... Down-sampling factor
  char hswitch: ....... to up-/downsampling routine

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct CASCADE_IIR;

  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  30.Oct.94 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
CASCADE_IIR *cascade_iir_init(nblocks, a, b, gain, idown, hswitch)
  long            nblocks;
  float           (*a)[2], (*b)[2];
  double          gain;
  long            idown;
  char            hswitch;
{
  static CASCADE_IIR *ptrIIR;	  /* pointer to the new struct */
  float           fak;
  float           (*T_ptr)[4];
  long            n;


  /* Allocate memory for a new struct */
  ptrIIR = (CASCADE_IIR *) malloc((long) sizeof(CASCADE_IIR));
  if (ptrIIR == (CASCADE_IIR *) 0L)
  {
    return 0;
  }


  /* Allocate memory for state variables */
  if ((ptrIIR->T = (float (*)[4]) malloc((nblocks * 4) * sizeof(fak)))
      == (float (*)[4]) 0)
  {
    free(ptrIIR);
    return 0;
  }


  /* fill coefficient sets */
  ptrIIR->nblocks = nblocks;	  /* store number of 2'nd order blocks */
  ptrIIR->a = a;
  ptrIIR->b = b;


  /* store down-sampling factor/gain/direct-path coefficient */
  ptrIIR->idown = idown;
  ptrIIR->gain = gain;


  /* Store switch to IIR-kernel procedure */
  ptrIIR->hswitch = hswitch;

  /* Clear state variables */
  T_ptr = ptrIIR->T;
  for (n = 0; n < nblocks; n++)
  {
    T_ptr[n][0] = 0.0;
    T_ptr[n][1] = 0.0;
    T_ptr[n][2] = 0.0;
    T_ptr[n][3] = 0.0;
  }

  ptrIIR->k0 = idown;		  /* modulo counter for down-sampling */


  /* Exit returning pointer to struct */
  return (ptrIIR);
}
/* ....................... End of cascade_iir_init() ....................... */


/*
  ============================================================================

  void cascade_free (CASCADE_IIR *iir_ptr);
  ~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Deallocate memory, which was allocated by an earlier call to one
  of the initilization routines. `CASCADE_IIR *iir_prt' must not be
  NULL.

  Parameters:
  ~~~~~~~~~~~
  CASCADE_IIR *iir_ptr: ... pointer to struct CASCADE_IIR previously
                            initialized by a call to one of the
                            initialization routines.

  Return value:
  ~~~~~~~~~~~~~
  Nothing.

  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  30.Oct.94 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
void            cascade_iir_free(iir_ptr)
  CASCADE_IIR        *iir_ptr;
{
  free(iir_ptr->T);		  /* free state variables */
  free(iir_ptr);		  /* free allocated struct */
}
/* ....................... End of cascade_iir_free() ....................... */


/* *********************************************************************** */

/*
  ============================================================================

  void direct_reset (DIRECT_IIR *iir_ptr);
  ~~~~~~~~~~~~~~~~~
  
  Description:
  ~~~~~~~~~~~~

  Clear state variables in DIRECT_IIR structure, what has been
  initialized by a previous call to one of the init functions.

  Parameters:
  ~~~~~~~~~~~
  DIRECT_IIR *iir_ptr: ... pointer to struct DIRECT_IIR previously 
                           initialized by a call to one of the initialization 
                           routines.

  Return value:
  ~~~~~~~~~~~~~
  Nothing.

  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
void            direct_reset(iir_ptr)
  DIRECT_IIR        *iir_ptr;
{
  long            n;
  register float  (*T_ptr)[2];
  long nblocks = (iir_ptr->poleno > iir_ptr->zerono)
                     ? iir_ptr->poleno 
                     : iir_ptr->zerono;

  T_ptr = iir_ptr->T;
  for (n = 0; n < nblocks; n++)
  {
    T_ptr[n][0] = 0.0;
    T_ptr[n][1] = 0.0;
  }

  iir_ptr->k0 = iir_ptr->idown;	  /* modulo counter for down-sampling */
}
/* ....................... End of direct_reset() ....................... */


/*
  ============================================================================

  long direct_iir_kernel (long lseg, float *x_ptr, DIRECT_IIR *iir_ptr,
  ~~~~~~~~~~~~~~~~~~~~~~~  float *y_ptr);

  Description:
  ~~~~~~~~~~~~

  Basic direct-form IIR filtering routine, for both up- and 
  down-sampling.

  Parameters:
  ~~~~~~~~~~~
  lseg: ...... number of input samples
  x_ptr: ..... array with input samples
  iir_ptr: ... pointer to IIR-struct (DIRECT_IIR *)
  y_ptr: ..... output samples

  Return value:
  ~~~~~~~~~~~~~
  Returns the number of output samples.
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
long            direct_iir_kernel(lseg, x_ptr, iir_ptr, y_ptr)
  long            lseg;
  float          *x_ptr;
  DIRECT_IIR    *iir_ptr;
  float          *y_ptr;
{
  if (iir_ptr->hswitch == 'U')
    return
      direct_form_iir_up_kernel( /* returns number of output samples */
				lseg,	          /* In : input signal leng. */
				x_ptr,	  /* In : input sample array */
				y_ptr,	  /* Out: outp. sample array */
				iir_ptr->idown, /* In : dwnsmpl.factor */
				iir_ptr->zerono, /*In: no.IIR-zeroes */
				iir_ptr->poleno, /*In: no.IIR-poles */
				iir_ptr->gain,  /* In : gain factor*/
				iir_ptr->a,	  /* In : num.coeffs */
				iir_ptr->b,	  /* In : denom.coeffs */
				iir_ptr->T	  /* I/O: state vars */
				);
  else
    return
      direct_form_iir_down_kernel(/* returns number of output samples */
				  lseg,	  /* In : input signal leng. */
				  x_ptr,	  /* In : input sample array */
				  y_ptr,	  /* Out: outp. sample array */
				  &(iir_ptr->k0), /*I/O: start idx,x-array */
				  iir_ptr->idown, /*In : dwnsmpl.factor */
				  iir_ptr->zerono, /*In: no.IIR-zeroes */
				  iir_ptr->poleno, /*In: no.IIR-poles */
				  iir_ptr->gain, /* In : gain factor */
				  iir_ptr->a,	  /* In : numerator coeffs */
				  iir_ptr->b,	  /* In : denom.coeffs */
				  iir_ptr->T	  /* I/O: state vars */
				  );
}
/* .................... End of direct_iir_kernel() ....................... */


/*
  ============================================================================

  long direct_form_iir_down_kernel(long lenx, float *x,
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ float *y, long *k0,
                                    long idown, long zerono, long poleno,
                                    double gain, float *a,
                                    float *b, float (*T)[2]);
  
  Description:
  ~~~~~~~~~~~~

  Function for filtering a sequence of input samples by a
  direct-form IIR-filter with down-sampling.
  
  Parameters:
  ~~~~~~~~~~~
  lenx: ........ (In) length of input array x[]
  x: ........... (In) array with input samples
  y: ........... (Out) array with output samples
  k0: .......... (In/Out) pointer to modulo counter
  idown: ....... (In) down-sampling factor
  zerono: ...... (In) number of zeroes
  poleno: ...... (In) number of poles
  gain: ........ (In) gain factor
  b: ........... (In) numerator coefficients
  c: ........... (In) denominator coefficients
  T: ........... (In/Out) past sample memory: T[][0] store past numerator (x) 
                      samples and T[][1] stores past denominator (y) samples. 
                      T[0][0],T[0][1] are the most recent samples, 
                      T[zerono-1][0],T[poleno-1][1] are the oldest.

  Return value:
  ~~~~~~~~~~~~~
  Returns the number of samples filtered.

  Author: <simao@ctd.comsat.com>
  ~~~~~~~

  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
static long     direct_form_iir_down_kernel(lenx, x, y, k0, idown, 
                                             zerono, poleno, gain, a, b, T)
  long            lenx;
  float          *x, *y;
  long           *k0, idown, zerono, poleno;
  double          gain;
  float           *a, *b, (*T)[2];
{
  long            kx, ky, n;
  double   yj;


  ky = 0;			  /* starting index in output array (y) */
  for (kx = 0; kx < lenx; kx++)	  /* loop over all input samples */
  {
    /* Save xk in memory */
    T[0][0] = x[kx]; 

    /* Filter samples through numerator (zero) part */
    for (yj=0, n = 0; n < zerono; n++)    
      yj +=  a[n] * T[n][0];

    /* Filter samples through denominator (pole) part */
    for (n = 1; n < poleno; n++)    
      yj -= b[n] * T[n-1][1];

    /* Shift samples in memory (to the right) for next step */
    for (n = zerono-1; n >0; n--)    
      T[n][0] = T[n-1][0];
    for (n = poleno-1; n >0; n--)    
      T[n][1] = T[n-1][1];
    T[0][1] = yj;

    /* Save to output only every "idown" samples */
    if (*k0 % idown == 0)
    {
      /* Apply gain and update y-samples' counter */
      y[ky] = yj * gain;
      ky++;
    }
    (*k0)++;
  }
  *k0 %= idown;			  /* avoid overflow by (*k0)++ */
  return ky;
}
/* .............. End of direct_form_iir_down_kernel() .............. */


/*
  ============================================================================

  long direct_form_iir_up_kernel(long lenx, float *x,
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ float *y, long *k0,
                                  long idown, long zerono, long poleno,
                                  double gain, float *a,
                                  float *b, float (*T)[2]);

  Description:
  ~~~~~~~~~~~~

  Function for filtering a sequence of input samples by a
  direct-form IIR-filter with up-sampling. Samples are taken from the 
  input vector x only once-in-idown times; in the other times, the input 
  sample is zero (interpolation process). 

  Parameters:
  ~~~~~~~~~~~
  lenx: ........ (In) length of input array x[]
  x: ........... (In) array with input samples
  y: ........... (Out) array with output samples
  iup: ......... (In) up-sampling factor
  zerono: ...... (In) number of zeroes
  poleno: ...... (In) number of poles
  gain: ........ (In) gain factor
  b: ........... (In) numerator coefficients
  c: ........... (In) denominator coefficients
  T: ........... (In/Out) past sample memory: T[][0] store past numerator (x) 
                      samples and T[][1] stores past denominator (y) samples. 
                      T[0][0],T[0][1] are the most recent samples, 
                      T[zerono-1][0],T[poleno-1][1] are the oldest.

  Return value:
  ~~~~~~~~~~~~~
  Returns the number of samples filtered.
  
  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
static long     direct_form_iir_up_kernel(lenx, x, y, iup, zerono, poleno,
					   gain, a, b, T)
  long            lenx;
  float          *x, *y;
  long            iup, zerono, poleno;
  double          gain;
  float           *a, *b, (*T)[2];
{
  long            kx, ky, n;
  register double yj;

  kx = 0;			  /* starting index in input array (x) */
  for (ky = 0; ky < iup * lenx; ky++)	/* loop over all input samples */
  {
    /* Compute output only every "iup" compute output only every "iup" 
     * samples by taking one input sample direct path OR by using a 
     * zero-valued sample; already save on memory array */
    T[0][0] = (ky % iup == 0) ? x[kx] : 0;

    /* Filter samples through numerator (zero) part */
    for (yj=0, n = 0; n < zerono; n++)    
      yj +=  a[n] * T[n][0];

    /* Filter samples through denominator (pole) part */
    for (n = 1; n < poleno; n++)    
      yj -= b[n] * T[n-1][1];

    /* Shift samples in memory (to the right) for next step */
    for (n = zerono-1; n >0; n--)    
      T[n][0] = T[n-1][0];
    for (n = poleno-1; n >0; n--)    
      T[n][1] = T[n-1][1];
    T[0][1] = yj;

    /* Apply the gain and update x counter if needed */
    y[ky] = yj * gain;
    if (ky % iup == 0) kx++;
  }
  return ky;
}
/* ............... End of direct_form_iir_up_kernel() ............... */


/*
  ============================================================================

  DIRECT_IIR *direct_iir_init (long nblocks, float *a,
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  float *b, double gain, 
	                         long idwnup, char hswitch);

  Description:
  ~~~~~~~~~~~~
  Allocate & initialize struct for down/up-sampling procedures.

  Parameters:
  ~~~~~~~~~~~
  long zerono: ........ number of zeroes
  long poleno: ........ number of poles
  float *a: ........... pointer to numerator coeffic.
  float *b: ........... pointer to denominator coeffic.
  double gain: ........ gain factor for filter
  long idown: ......... Down-sampling factor
  char hswitch: ....... to up-/downsampling routine

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to struct DIRECT_IIR;

  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
DIRECT_IIR *direct_iir_init(zerono, poleno, a, b, gain, idown, hswitch)
  long            zerono, poleno;
  float           *a, *b;
  double          gain;
  long            idown;
  char            hswitch;
{
  static DIRECT_IIR *ptrIIR;	  /* pointer to the new struct */
  float           fak;
  float           (*T_ptr)[2];
  long            n;
  long nblocks;

  /* Get greater of zero and pole */
  nblocks = zerono > poleno? zerono: poleno;

  /* Allocate memory for a new struct */
  ptrIIR = (DIRECT_IIR *) malloc((long) sizeof(DIRECT_IIR));
  if (ptrIIR == (DIRECT_IIR *) 0L)
  {
    return 0;
  }


  /* Allocate memory for state variables */
  if ((ptrIIR->T = (float (*)[2]) malloc((nblocks * 2) * sizeof(fak)))
      == (float (*)[2]) 0)
  {
    free(ptrIIR);
    return 0;
  }


  /* fill coefficient sets */
  ptrIIR->poleno = poleno;	  /* store number of poles */
  ptrIIR->zerono = zerono;	  /* store number of zeroes */
  ptrIIR->a = a;
  ptrIIR->b = b;


  /* store down-sampling factor/gain/direct-path coefficient */
  ptrIIR->idown = idown;
  ptrIIR->gain = gain;


  /* Store switch to IIR-kernel procedure */
  ptrIIR->hswitch = hswitch;

  /* Clear state variables */
  T_ptr = ptrIIR->T;
  for (n = 0; n < nblocks; n++)
  {
    T_ptr[n][0] = 0.0; /* Numerator memory */
    T_ptr[n][1] = 0.0; /* Denominator memory */
  }

  ptrIIR->k0 = idown;		  /* modulo counter for down-sampling */


  /* Exit returning pointer to struct */
  return (ptrIIR);
}
/* ....................... End of direct_iir_init() ....................... */


/*
  ============================================================================

  void direct_free (DIRECT_IIR *iir_ptr);
  ~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Deallocate memory, which was allocated by an earlier call to one
  of the initilization routines. `DIRECT_IIR *iir_prt' must not be
  NULL.

  Parameters:
  ~~~~~~~~~~~
  DIRECT_IIR *iir_ptr: ... pointer to struct DIRECT_IIR previously
                            initialized by a call to one of the
                            initialization routines.

  Return value:
  ~~~~~~~~~~~~~
  Nothing.

  Author: <simao@ctd.comsat.com>
  ~~~~~~~
  
  History:
  ~~~~~~~~
  31.Jul.95 v1.0 Release of 1st version <simao@ctd.comsat.com>

 ============================================================================
*/
void            direct_iir_free(iir_ptr)
  DIRECT_IIR        *iir_ptr;
{
  free(iir_ptr->T);		  /* free state variables */
  free(iir_ptr);		  /* free allocated struct */
}
/* ....................... End of direct_iir_free() ....................... */


/* **************************** END OF IIR-LIB.C ************************ */
