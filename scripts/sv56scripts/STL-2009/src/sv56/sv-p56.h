/*
  ============================================================================
   File: SV-P56.H                                             01.SEP.1995 v2.2
  ============================================================================

                      UGST/ITU-T SPEECH VOLTMETER MODULE

                          GLOBAL FUNCTION PROTOTYPES

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO 
       CODING STANDARDS".
       =============================================================

   History:
   28.Feb.92	v1.0	First version <tdsimao@venus.cpqd.ansp.br>
   18.May.92	v1.1	Prototype of new init_speech_voltmeter;
                        removal of `init` parameter in speech_voltmeter;
                        macros for accessing SVP56_state statistics. 
                        <tdsimao@venus.cpqd.ansp.br>
   01.Sep.95    v2.2    Updated version number to match sv-p56.c and added 
                        smart prototypes <simao@ctd.comsat.com>

  ============================================================================
*/
#ifndef SPEECH_VOLTMETER_defined
#define SPEECH_VOLTMETER_defined 220

/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* State for speech voltmeter function */
typedef struct
{
  float           f;            /* sampling frequency, in Hz */
  unsigned long   a[15];        /* activity count */
  double          c[15];        /* threshold level; 15 is the no.of thres. */
  unsigned long   hang[15];     /* hangover count */
  unsigned long   n;            /* number of samples read since last reset */
  double          s;            /* sum of all samples since last reset */
  double          sq;           /* squared sum of samples since last reset */
  double          p;            /* intermediate quantities */
  double          q;            /* envelope */
  double          max;          /* max absolute value found since last reset */
  double          refdB;        /* 0 dB reference point, in [dB] */
  double          rmsdB;        /* rms value found since last reset */
  double          maxP, maxN;   /* maximum pos/neg.values since last reset */
  double          DClevel;      /* average level since last reset */
  double          ActivityFactor; /* Activity factor since last reset */
}               SVP56_state;

/* Speech voltmeter prototypes */
double bin_interp ARGS((double upcount, double lwcount, double upthr, 
			double lwthr, double Margin, double tol));
void init_speech_voltmeter ARGS((SVP56_state *state, double sampl_freq));
double speech_voltmeter ARGS((float *buffer, long smpno, SVP56_state *state));


/* Definitions for getting statistics from a `SVP56_state' variable */
#define SVP56_get_rms_dB(state) (state.rmsdB)
#define SVP56_get_DC_level(state) ((state).DClevel)
#define SVP56_get_activity(state) ((state.ActivityFactor) * 100.0)
#define SVP56_get_pos_max(state) ((state).maxP)
#define SVP56_get_neg_max(state) ((state).maxN)
#define SVP56_get_abs_max(state) ((state).max)
#define SVP56_get_smpno(state) ((state).n)

#endif /* SPEECH_VOLTMETER_defined */
/* ........................ End of SVP56.H .......................... */
