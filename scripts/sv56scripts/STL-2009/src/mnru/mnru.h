/*
  ============================================================================
   File: MNRU.H                                              V.2.0-31.JUL-1995
  ============================================================================

                            UGST/ITU-T MNRU MODULE

                          GLOBAL FUNCTION PROTOTYPES

   History:
   27.Jan.92	v1.0	First version <simao@cpqd.br>
   01.Feb.95    v1.1    Smart prototypes that work with many compilers 
                        <simao@ctd.comsat.com> 
   31.Jul.95    v2.0    MNRU conforming to new P.81. State variables changed.
  ============================================================================
*/
#ifndef MNRU_DEFINED
#define MNRU_DEFINED 200

/* Smart function prototypes: for [ag]cc, VaxC, and [tb]cc */
#if !defined(ARGS)
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(s) s
#else
#define ARGS(s) ()
#endif
#endif

#ifdef STL92_RNG
#define RANDOM_state ori_RANDOM_state
#else
#define RANDOM_state new_RANDOM_state
#endif

/* Definition of type for random_MNRU state variables */
typedef struct
{
  int             idum;
  int             inext, inextp;
  long            ma[56];	/* this is a special value; shall not be
				 * changed [1],[2] */
}               ori_RANDOM_state;

/* Definition of type for random_MNRU state variables */
typedef struct
{
  float *gauss;

}               new_RANDOM_state;

/* Definitions for the MNRU state variable */
#define MNRU_STAGE_OUT_FLT 2         /* number of 2nd-order stages in filter */

/* Definition of type for MNRU state variables */
typedef struct
{
  /* State variables related to the rnadom number generation */
  long            seed, clip;
  double          signal_gain, noise_gain;
  double         *vet, last_xk, last_yk, last_y20k_lp;
  RANDOM_state    rnd_state;	/* for random_MNRU() */
  char            rnd_mode;

  /* State variables related to the output band-pass filtering */
  double A[MNRU_STAGE_OUT_FLT][3];    /* numerator coefficients */
  double B[MNRU_STAGE_OUT_FLT][2];    /* denominator coefficients */
  double DLY[MNRU_STAGE_OUT_FLT][2];  /* delay storage elements (z-shifts) */
}               MNRU_state;

/* Prototype for MNRU and random function(s) */
#ifdef NO_PROMOTION
double *MNRU_process ARGS((char operation, MNRU_state *s, float *input, 
			   float *output, long n, long seed, char mode, 
			   double Q));
#else
double *MNRU_process ARGS((int operation, MNRU_state *s, float *input, 
			   float *output, long n, long seed, int mode, 
			   double Q));
#endif
float random_MNRU ARGS((char *mode, RANDOM_state *r, long seed));

/* Definitions for the MNRU algorithm */
#define MOD_NOISE    1
#define NOISE_ONLY   0
#define SIGNAL_ONLY -1

#define MNRU_START     1
#define MNRU_CONTINUE  0
#define MNRU_STOP     -1

/* Definitions for Knuth's subtractive Random Number Generator */
#define RANDOM_RUN 0
#define RANDOM_RESET 1

#endif
/*  ------------------------- End of MNRU.H ----------------------------- */
