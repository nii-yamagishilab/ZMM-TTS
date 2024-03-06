/*
  ============================================================================
   File: FIRFLT.H                                           v.2.5 -  31.Dec.2008
  ============================================================================

	    ITU-T STL HIGH QUALITY FIR UP/DOWN-SAMPLING FILTER

 		      GLOBAL FUNCTION PROTOTYPES

   History:
   28.Feb.92	v1.0	First version <hf@pkinbg.uucp>
   20.Apr.94	v2.0	Added smart prototypes & new functions for HQFLT v.2.0
   30.Sep.94    v2.1    Changed name of module to FIR and split different 
                        filters to different files, to ease expansion.
   02.Jul.99    v2.2    Added new prototypes
   10.Feb.05	v2.3	Added protoype for the [50Hz-14kHz] filter 
						and the LP filters	<Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>
   15.May.07	v2.4+	Added protoype for the [20Hz-20kHz] filter 
						and the 1.5kHz, 14kHz. 20kHz LP filters	<Ericsson>
   31.Dec.2008  v2.5    Added LP filters (12kHz) for fs=48kHz < huawei >

  ============================================================================
*/
 
#ifndef FIRFLT_FIRstruct_defined
#define FIRFLT_FIRstruct_defined 220


/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* 
 * ..... State variable structure for FIR filtering ..... 
 */

typedef struct {
        long  lenh0;                    /* number of FIR coefficients        */
        long  dwn_up;                   /* down sampling factor              */
        long  k0;                       /* start index in next segment       */
                                        /* (needed in segmentwise filtering) */
        float *h0;                      /* pointer to array with FIR coeff.  */
        float *T;                       /* pointer to delay line             */
        char  hswitch;                  /* switch to FIR-kernel              */
} SCD_FIR;


/* 
 * ..... Global function prototypes ..... 
 */

long hq_kernel ARGS((long lseg, float *x_ptr, SCD_FIR *fir_ptr, float *y_ptr));
SCD_FIR *hq_down_2_to_1_init ARGS((void));
SCD_FIR *hq_up_1_to_2_init ARGS((void));
SCD_FIR *hq_down_3_to_1_init ARGS((void));
SCD_FIR *hq_up_1_to_3_init ARGS((void));
SCD_FIR *irs_8khz_init ARGS((void));
SCD_FIR *irs_16khz_init ARGS((void));
SCD_FIR *mod_irs_16khz_init ARGS((void));
SCD_FIR *mod_irs_48khz_init ARGS((void));
SCD_FIR *tia_irs_8khz_init ARGS((void));
SCD_FIR *ht_irs_16khz_init ARGS((void));
SCD_FIR *rx_mod_irs_16khz_init ARGS((void));
SCD_FIR *rx_mod_irs_8khz_init ARGS((void));
SCD_FIR *delta_sm_16khz_init ARGS((void));
SCD_FIR *linear_phase_pb_2_to_1_init ARGS((void));
SCD_FIR *linear_phase_pb_1_to_2_init ARGS((void));
SCD_FIR *linear_phase_pb_1_to_1_init ARGS((void));
SCD_FIR *psophometric_8khz_init ARGS((void));
SCD_FIR *msin_16khz_init ARGS((void));
SCD_FIR *p341_16khz_init ARGS((void));
SCD_FIR *bp5k_16khz_init ARGS((void));
SCD_FIR *bp100_5k_16khz_init ARGS((void));
SCD_FIR *bp14k_32khz_init ARGS((void));
SCD_FIR *bp20k_48khz_init ARGS((void));
SCD_FIR *LP1p5_48kHz_init ARGS((void));
SCD_FIR *LP35_48kHz_init ARGS((void));
SCD_FIR *LP7_48kHz_init ARGS((void));
SCD_FIR *LP10_48kHz_init ARGS((void));
SCD_FIR *LP14_48kHz_init ARGS((void));
SCD_FIR *LP20_48kHz_init ARGS((void));
// FILTER_12k48k_HW
SCD_FIR *LP12_48kHz_init ARGS((void));
// FILTER_12k48k_HW
void hq_free ARGS((SCD_FIR *fir_ptr));
void hq_reset ARGS((SCD_FIR *fir_ptr));

#endif /* FIRFLT_FIRstruct_defined */

/* *************************** End of firflt.h *************************** */
