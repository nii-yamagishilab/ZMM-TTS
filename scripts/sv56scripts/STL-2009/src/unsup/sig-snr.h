/*
  ============================================================================
   File: SIG-SNR.H                                            21.MAY.1997 v1.0
  ============================================================================

                      UGST/ITU-T UNSUPPORTED MODULE - SNR MEASUREMENT

                          GLOBAL FUNCTION PROTOTYPES

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO 
       CODING STANDARDS".
       =============================================================

   History:
   21.May.97	v1.0	First version <simao@ctd.comsat.com>

  ============================================================================
*/
#ifndef SIG_SNR_DEFINED
#define SIG_SNR_DEFINED 100

/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if defined(MSDOS) || defined(__MSDOS__) || defined(__STDC__) || defined(VMS)
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* Pseudo-functions & defs to avoid domain errors */
#define EPS 10E-20
#define SQRT(x) sqrt((double)(x)+EPS)
#define LOG10(x) log10((double)(x)+EPS)
#define power2dBov(x) (10*LOG10(x)-20*LOG10(32768.0)) /* for integer numbers */
#define norm_power2dBov(x) (10*LOG10(x))            /* for +-1 norm'd signals*/
#define amplitude2dBov(x) (20*LOG10(x/32768.0))     /* for integer numbers */
#define norm_amplitude2dBov(x) (20*LOG10(x))        /* for +-1 norm'd signals*/

/* Define type for level structure */
typedef struct
{
  double          power;
  double          dc;
  double          rms_dc;
  double          rms_ac;
  double          n;
  double          variance;
}
level_t;

/* sig-snr.c */
double get_min ARGS((double min, double *samples, long count,
		     double not_under));
double get_max ARGS((double max, double *samples, long count,
		     double not_above));
double get_dc_rms_level ARGS((short *buf, long smpno));
double get_ac_rms_level ARGS((short *buf, long smpno));
double d_average ARGS((double *buf, long smpno));
void get_all_levels ARGS((short *buf, long smpno, level_t *level, int reset));
double get_snr_levels ARGS((double *seg_pwr, double dB_thres,
			    double dB_margin, long n, double *tot_SNR,
			    double *pwr_above, double *pwr_below,
			    long *n_a, long *n_b));

#endif /* SIG_SNR_DEFINED */
/* ......................... End of sig-snr.h ......................... */
