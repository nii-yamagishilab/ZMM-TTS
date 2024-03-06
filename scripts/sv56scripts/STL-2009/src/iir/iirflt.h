/*
  ============================================================================
   File: IIRFLT.H                                     Version: 3.0 - 31.JUL.95
  ============================================================================

                            UGST/ITU-T IIR FILTERS

                          GLOBAL FUNCTION PROTOTYPES

   History:
   28.Feb.92	v1.0	First version <hf@pkinbg.uucp>
   30.Oct.94	v2.0	Name changed to iirflt.h/included cascade-form 
                        IIR filters <simao@ctd.comsat.com>
   31.Jul.95	v3.0	Added direct-form IIR filters <simao@ctd.comsat.com>

  ============================================================================
*/

#ifndef IIRFLT_IIRstruct_defined
#define IIRFLT_IIRstruct_defined  300


/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/*
 * ..... State variable structure for IIR filtering, parallel form  .....
 */

typedef struct
{
  long            nblocks;	/* number of coefficient sets         */
  long            idown;	/* down sampling factor               */
  long            k0;		/* start index in next segment        */
  double          gain;		/* gain factor                        */
  double          direct_cof;	/* In     : direct path coefficient   */
  float           (*b)[3];	/* In     : numerator coefficients    */
  float           (*c)[2];	/* In     : denominator coefficients  */
  float           (*T)[2];	/* In/Out : state variables           */
  char            hswitch;	/* "U": upsampling; else downsampling */
}               SCD_IIR;


/*
 * ..... State variable structure for IIR filtering, cascade form  .....
 */
typedef struct
{
  long            nblocks;	/* number of stages in cascade        */
  long            idown;	/* down sampling factor               */
  long            k0;		/* start index in next segment        */
  double          gain;		/* gain factor                        */
  float           (*a)[2];	/* In     : numerator coefficients    */
  float           (*b)[2];	/* In     : denominator coefficients  */
  float           (*T)[4];	/* In/Out : state variables, 1 for each stage*/
  char            hswitch;	/* "U": upsampling; else downsampling */
}               CASCADE_IIR;


/*
 * ..... State variable structure for IIR filtering, direct form  .....
 */

typedef struct
{
  long            poleno;	/* number of poles                    */
  long            zerono;	/* number of zeores                   */
  long            idown;	/* down sampling factor               */
  long            k0;		/* start index in next segment        */
  double          gain;		/* gain factor                        */
  float           *a;	        /* In     : numerator coefficients    */
  float           *b;	        /* In     : denominator coefficients  */
  float           (*T)[2];      /* In/Out : state variables           */
  char            hswitch;	/* "U": upsampling; else downsampling */
}               DIRECT_IIR;


/*
 * ..... Global function prototypes .....
 */

/* Originals of the STL92: parallel IIR basic functions */
long stdpcm_kernel ARGS((long lseg, float *x_ptr, SCD_IIR *iir_ptr, 
			 float *y_ptr));
void stdpcm_free ARGS((SCD_IIR *iir_ptr));
void stdpcm_reset ARGS((SCD_IIR *iir_ptr));

/* Originals of the STL92: parallel IIR filter initialization */
SCD_IIR *stdpcm_16khz_init ARGS((void));
SCD_IIR *stdpcm_2_to_1_init ARGS((void));
SCD_IIR *stdpcm_1_to_2_init ARGS((void));


/* Additions to the STL92: cascade IIR basic functions */
long cascade_iir_kernel ARGS((long lseg, float *x_ptr, CASCADE_IIR *iir_ptr, 
			      float *y_ptr));
void cascade_iir_reset ARGS((CASCADE_IIR *iir_ptr));
void cascade_iir_free ARGS((CASCADE_IIR *iir_ptr));

/* Additions to the STL92: cascade IIR filter initialization */
CASCADE_IIR *iir_G712_8khz_init ARGS((void));
CASCADE_IIR *iir_irs_8khz_init ARGS((void));
CASCADE_IIR *iir_casc_lp_3_to_1_init ARGS((void));
CASCADE_IIR *iir_casc_lp_1_to_3_init ARGS((void));


/* Additions to the STL92: direct IIR basic functions */
long direct_iir_kernel ARGS((long lseg, float *x_ptr, DIRECT_IIR *iir_ptr, 
			      float *y_ptr));
void direct_reset ARGS((DIRECT_IIR *iir_ptr));
void direct_iir_free ARGS((DIRECT_IIR *iir_ptr));

/* Additions to the STL92: direct IIR filter initialization */
DIRECT_IIR *iir_dir_dc_removal_init ARGS((void));

#endif
/* ........................... End of IIRFLT.H ........................... */
