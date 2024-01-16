/*                                                             v2.3 19.MAY.05
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


MODULE:         SV-P56.C, FUNCTIONS RELATED TO ACTIVE LEVEL CALCULATIONS

ORIGINAL BY:    
   Simao Ferraz de Campos Neto   CPqD/Telebras Brazil

DATE:           19/May/2005

RELEASE:        2.00

PROTOTYPES:     see sv-p56.h.

FUNCTIONS:

init_speech_voltmeter ......... initialization of the speech voltmeter state
                                variables in a structure of type SVP56_state.

speech_voltmeter .............. measurement of the active speech level of
                                data in a buffer according to P.56. Other 
				relevant statistics are also available.

HISTORY:

   07.Oct.91 v1.0 Release of 1st version to UGST.
   28.Feb.92 v2.0 Correction of bug in speech_voltmeter; inclusion of test
                  for extremes in bin_interp; use of structure to keep  
                  state variables.   <simao@cpqd.br>
   18.May.92 v2.1 Creation of init_speech_voltmeter and consequent changes;
                  speech_voltmeter changed to operate with float data in the 
                  normalized range. <simao@cpqd.br>
   01.Sep.95 v2.2 Added very small constant to avoid problems first detected
                  in a DEC Alpha VMS workstation with log(0) by
                  <gerhard.schroeder@fz13.fz.dbp.de>; generalized to all
		  platforms <simao@ctd.comsat.com>
   19.May.05 v2.3 Bug correction in bin_interp() routine, based on changes 
				  suggested by Mr Kabal. 
				  Upper and lower bounds are updated during the interpolation.
						<Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com>

=============================================================================
*/

/*
 * .................... INCLUDES .................... 
 */

/* System includes ... */
#include <math.h>

/* Specific includes ... */
#ifndef SPEECH_VOLTMETER_defined
#include "sv-p56.h"
#endif

/*
 * .................... FUNCTIONS .................... 
 */

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        double bin_interp (double upcount, double lwcount,
        ~~~~~~~~~~~~~~~~~  double upthr, double lwthr, double Margin)

        Description:
        ~~~~~~~~~~~~

        Makes the binary interpolation between upcount and lwcount (In
        this program, the upper and lower bounds inside which the active
        speech  level, asl, shall fall), considering the quantization
        thresholds upthr (upper threshold) and lwthr (lower threshold).
        First it tests whether the extreme points are not already close
        enough to `asl'. If not, then it guesses that the  initial value
        for asl falls in the middle of the bounds, with a `tol' [dB]
        tolerance. If this difference is higher than tol, the new 
        bounds are taken as the upper and the middle activities and
        thresholds. If it is less than `-tol' [dB], the new bounds are
        taken to be the middle and the lower activities and thresholds.
        The process goes  until the `tol' [dB] tolerance is reached.

        Variables:
        ~~~~~~~~~~
        Name:        I/0: Type:  Use:
        upcount       I   double Upper activity bound to interpolate
        lwcount       I   double Lower activity bound to interpolate
        upthr         I   double Upper threshold level
        lwthr         I   double Lower threshold level
        Margin        I   double Margin between activity bound and threshold
        tol           I   double Desired tolerance to the interpolation;
                                      normally it is 0.5 [dB].

        Return value:
        ~~~~~~~~~~~~~
        Returns the interpolated value, which falls in a range of `tol' dB.

        Functions used:      NONE
        ~~~~~~~~~~~~~~~

        Prototype:   in sv-p56.h
        ~~~~~~~~~~

        Author:        Simao Ferraz de Campos Neto - CPqD-TELEBRAS
        ~~~~~~~                 [tdsimao@venus.cpqd.ansp.br]

        Log of changes:
        ~~~~~~~~~~~~~~~
        Dates        Version        Description
        19.Nov.90     1.0F        Fortran version of the routine
        07.Oct.91     1.0C        Translation to C.
        28.Feb.92     2.0         Included check for the extremes as
                                  interpolated values.
		19.May.05     2.3         Bug correction, based on changes suggested 
								  by Mr Kabal. upper and lower bounds are 
								  updated during the interpolation. 
									 <Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
double          bin_interp(upcount, lwcount, upthr, lwthr, Margin, tol)
  double          upcount, lwcount, upthr, lwthr, Margin, tol;
{
  double          midcount, midthr, diff;
  register long   iterno;

  /* Consistency check */
  if (tol < 0.)
    tol = -tol;

  /* Check if extreme counts are not already the true active value */
  iterno = 1;
  if ((diff = fabs((upcount - upthr) - Margin)) < tol)
    return upcount;
  if ((diff = fabs((lwcount - lwthr) - Margin)) < tol)
    return lwcount;

  /* Initialize first middle for given (initial) bounds */
  midcount = (upcount + lwcount) / 2.0;
  midthr = (upthr + lwthr) / 2.0;

  /* Repeats loop until `diff' falls inside the tolerance (-tol<=diff<=tol) */
  while ((diff = (midcount - midthr) - Margin, fabs(diff)) > tol)
  {
    /* if tolerance is not met up to 20 iteractions, then relax the 
       tolerance by 10 % */
    if (++iterno>20) 
      tol *= 1.1; 

    if (diff > tol)             /* then new bounds are ... */
    {
      midcount = (upcount + midcount) / 2.0; /* upper and middle activities */
      midthr = (upthr + midthr) / 2.0;	     /* ... and thresholds */
	  lwcount = midcount;
	  lwthr = midthr;
    }
    else if (diff < -tol)	/* then new bounds are ... */
    {
      midcount = (midcount + lwcount) / 2.0; /* middle and lower activities */
      midthr = (midthr + lwthr) / 2.0;       /* ... and thresholds */
	  upcount = midcount;
	  upthr = midthr;
    }
  }

  /* Since the tolerance has been satisfied, midcount is selected 
   * as the interpolated value with a tol [dB] tolerance. */

  return (midcount);
}
/* ....................... End of bin_interp() .......................... */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        void init_speech_voltmeter (SVP56_state *state, double sampl_freq);
        ~~~~~~~~~~~~~~~~~~~~~~~~~~     

        Description:
        ~~~~~~~~~~~~

        Initializes state variables of a structure of type SVP56_state,
        for use by the speech_voltmeter() routine. 

        Other variables:
        ~~~~~~~~~~~~~~~~
        Name:         Type:   Use:
        buffer         I/O       input samples vector
        sampl_freq      I        input signal's sampling frequency


        Value returned:
        ~~~~~~~~~~~~~~~
        None.

        Prototype:   in sv-p56.h
        ~~~~~~~~~~

        Author: Simao Ferraz de Campos Neto -- CPqD/Telebras
        ~~~~~~~          <tdsimao@cpqd.ansp.br>

        Log of changes:
        ~~~~~~~~~~~~~~~
        18.May.92     1.0   Release of first version <tdsimao@cpqd.ansp.br>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define T        0.03	/* in [s] */
#define H        0.20	/* in [s] */
#define M        15.9	/* in [dB] */
#define THRES_NO 15     /* number of thresholds in the speech voltmeter */

void            init_speech_voltmeter(state, sampl_freq)
  SVP56_state    *state;
  double          sampl_freq;
{
  double          x;
  long            I, j;


  /* First initializations */
  state->f = sampl_freq;
  I = floor(H * state->f + 0.5);

  /* Inicialization of threshold vector */
    for (x = 0.5, j = 1; j <= THRES_NO; j++, x /= 2.0)
      state->c[THRES_NO - j] = x;

  /* Inicialization of activity and hangover count vectors */
    for (j = 0; j < THRES_NO; j++)
    {
      state->a[j] = 0;
      state->hang[j] = I;
    }

    /* Inicialization for the quantities used in the two P.56's processes */
    state->s = state->sq = state->n = state->p = state->q = 0;

    /* Inicialization of other quantities referring to state variables */
    state->max = 0;
    state->maxP = -32768.;
    state->maxN = 32767.;

    /* Defining the 0 dB reference level in terms of normalized values */
    state->refdB = 0 /* dBov */;

}
#undef THRES_NO 
/* .................. End of init_speech_voltmeter() ..................... */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        double speech_voltmeter (float *buffer,long smpno,SVP56_state *state);
        ~~~~~~~~~~~~~~~~~~~~~~~  

        Description:
        ~~~~~~~~~~~~

        Calculates the activity factor and the active speech level
        (conforming to ITU-T P.56) as main results; side
        results are:
        > average level;
        > max & min values;
        > rms power [dB];
        > maximum dB level to normalize without causing clipping
        > rms and active peak factor for the file

        Follows the ITU-T Recommendation P.56 with the
        following notation for the variables (the ones marked
        `DEFINITION' are not true vars, but #define's instead):

          f:          sampling frequency;
          T:          time constant of smoothing, in seconds (DEFINITION);
          g:          coefficient of smoothing;
          H:          hangover in seconds (DEFINITION);
          I:          size of hangover normalized by the sampling
                      frequency;
          M:          margin in dB of the difference between threshold
                      and active speech level (DEFINITION);
          q:          envelope;
          p:          intermediate quantity;
          c:          vector with thresholds from one quantizing level
                      up to half the maximum code, at a step of 2;
          refdB:      reference value to which 0 dB is assigned.
                      For the STL92, it is the peak of a digital system, which
                      is defined as 1.0 (0dBov).

        Variables:
        ~~~~~~~~~~
        Name:         Type:   Use:
        buffer          I        input samples vector
        smpno           I        number of samples in vector `buffer'
        state          I/O       state variable associated with `buffer'


        Value returned:
        ~~~~~~~~~~~~~~~
        Returns the active speech level, in dBov, as a double.

        Functions used:
        ~~~~~~~~~~~~~~~
        > bin_interp, from this module;
        > exp, fabs, log10, pow, from standard library <math.h>;

        Prototype:   in sv-p56.h
        ~~~~~~~~~~

        Author: Simao Ferraz de Campos Neto -- CPqD/Telebras
        ~~~~~~~          <tdsimao@venus.cpqd.ansp.br>

        Log of changes:
        ~~~~~~~~~~~~~~~
        07.Mar.91     0.0       Release of first version of C speech voltmeter.
        08.Oct.91     1.0       Release of speech_voltmeter as a module.
        19.Feb.91     2.0       Use of one structure with all state variables,
                                instead of passing a bunch of variables.
        18.May.92     2.1       Does not carry out initialization (see init_
                                speech_voltmeter above); input data in 
                                "buffer" is supposed to be in the range 
                                -1.0 .. 1.0.
        01.Sep.95     2.2       Added a small constant to all log10() calls 
                                to avoid problems with log(0), first detected
                                by <gerhard.schroeder@fz13.fz.dbp.de> on a
				DEC Alpha VMS workstation and extended
                                to ther platforms as well. Exceptions are 
                                VMS and gcc on PC. <simao@ctd.comsat.com>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define T        0.03	/* in [s] */
#define H        0.20	/* in [s] */
#define M        15.9	/* in [dB] */
#define THRES_NO 15     /* number of thresholds in the speech voltmeter */

/* Hooked to eliminate sigularity with log(0.0) (happens w/all-0 data blocks */
#define MIN_LOG_OFFSET 1.0e-20

double          speech_voltmeter(buffer, smpno, state)
  float          *buffer;
  long            smpno;
  SVP56_state    *state;
{
  int             I, j;
  long            k;
  double          g, x, AdB, CdB, AmdB, CmdB, ActiveSpeechLevel;
  double          LongTermLevel, Delta[15];


  /* Some initializations */
  I = floor(H * state->f + 0.5);
  g = exp(-1.0 / (state->f * T));

  /* Calculates statistics for all given data points */
  for (k = 0; k < smpno; k++)
  {
    x = (double) buffer[k];
    /* Compares the sample with the max. already found for the file */
    if (fabs(x) > state->max)
      state->max = fabs(x);
    /* Check for the max. pos. value */
    if (x > state->maxP)
      state->maxP = x;
    /* Check for the max. neg. value */
    if (x < state->maxN)
      state->maxN = x;

    /* Implements Process 1 of P.56 */
    (state->sq) += x * x;
    (state->s) += x;
    (state->n)++;

    /* Implements Process 2 of P.56 */
    state->p = g * (state->p) + (1 - g) * ((x > 0) ? x : -x);
    state->q = g * (state->q) + (1 - g) * (state->p);

    /* Applies threshold to the envelope q */
    for (j = 0; j < THRES_NO; j++)
    {
      if ((state->q) >= state->c[j])
      {
	state->a[j]++;
	state->hang[j] = 0;
      }
      if (((state->q) < state->c[j]) && (state->hang[j] < I))
      {
	state->a[j]++;
	state->hang[j] += 1;
      }
   /* if (((state->q)<state->c[j])&&(state->hang[j]=I)), do nothing */
    }		   /* [j] */
  }		   /* [k] */

  /* Computes the statistics */
  state->DClevel = (state->s) / (state->n);
  LongTermLevel = 10 * log10((state->sq) / (state->n) + MIN_LOG_OFFSET);
  state->rmsdB = LongTermLevel - state->refdB;
  state->ActivityFactor = 0;
  ActiveSpeechLevel = -100.0;

  /* Test the lower active counter; if 0, is silence */
  if (state->a[0] == 0) 
    return(ActiveSpeechLevel);
  else
    AdB = 10 * log10(((state->sq) / state->a[0]) + MIN_LOG_OFFSET);

  /* Test if the lower act.counter is below the margin: if yes, is silence */
  CdB = 20 * log10((double) state->c[0]);
  if (AdB - CdB < M) 
    return(ActiveSpeechLevel);

  /* Proceed serially for steps 2 and up -- this is the most common case*/
  for (j = 1; j < THRES_NO; j++)
  {
    if (state->a[j] != 0)
    {
      AdB = 10 * log10(((state->sq) / state->a[j]) + MIN_LOG_OFFSET);
      CdB = 20 * log10(((double) state->c[j]) + MIN_LOG_OFFSET);
      Delta[j] = AdB - CdB;
      if (Delta[j] <= M)	/* then interpolates to find the active */
	/* level and the activity factor and exits */
      {
	/* AmdB is AdB for j-1, CmdB is CdB for j-1 */
	AmdB = 10 * log10(((state->sq) / state->a[j - 1]) + MIN_LOG_OFFSET);
	CmdB = 20 * log10(((double) state->c[j - 1] + MIN_LOG_OFFSET));

	ActiveSpeechLevel = bin_interp(AdB, AmdB, CdB, CmdB, M, 0.5 /* dB */ );

	state->ActivityFactor =
	  pow(10.0, ((LongTermLevel - ActiveSpeechLevel) / 10));
	ActiveSpeechLevel -= (state->refdB);
        break;
      }
    }
  }

  return (ActiveSpeechLevel);
}
#undef MIN_LOG_OFFSET
#undef M
#undef H
#undef T
#undef THRES_NO 
/* .................... End of speech_voltmeter() ........................ */
