/*
  ============================================================================
   File: G726.H                                        28-Feb-1991 (18:00:00)
  ============================================================================

                            UGST/ITU-T G726 MODULE

                          GLOBAL FUNCTION PROTOTYPES

   History:
   28.Feb.92	v1.0	First version <simao@cpqd.br>
   06.May.94    v2.0    Smart prototypes that work with many compilers <simao> 
  ============================================================================
*/
#ifndef G726_defined
#define G726_defined 200

/* Smart function prototypes: for [ag]cc, VaxC, and [tb]cc */
#if !defined(ARGS)
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__)) || defined (__CYGWIN__) || defined (_MSC_VER)
#define ARGS(s) s
#else
#define ARGS(s) ()
#endif
#endif


/* State for G726 encoder and decoder */
typedef struct
{
  short           sr0, sr1;	/* Reconstructed signal with delays 0 and 1 */
  short           a1r, a2r;	/* Triggered 2nd order predictor coeffs. */
  short           b1r;		/* Triggered 6nd order predictor coeffs */
  short           b2r;
  short           b3r;
  short           b4r;
  short           b5r;
  short           b6r;
  short           dq5;		/* Quantized difference signal with delays 5
				 * to 0 */
  short           dq4;
  short           dq3;
  short           dq2;
  short           dq1;
  short           dq0;
  short           dmsp;		/* Short term average of the F(I) sequence */
  short           dmlp;		/* Long term average of the F(I) sequence */
  short           apr;		/* Triggered unlimited speed control
				 * parameter */
  short           yup;		/* Fast quantizer scale factor */
  short           tdr;		/* Triggered tone detector */
  short           pk0, pk1;	/* sign of dq+sez with delays 0 and 1 */
  long            ylp;		/* Slow quantizer scale factor */
}               G726_state;

#ifdef VAXC
#  define SHORT short
#else
#  define SHORT int
#endif

/* Function prototypes */
void G726_encode ARGS((short *inp_buf, short *out_buf, long smpno, char *law, 
	SHORT rate, SHORT r, G726_state *state));
void G726_decode ARGS((short *inp_buf, short *out_buf, long smpno, char *law, 
	SHORT rate, SHORT r, G726_state *state));
void G726_expand ARGS((short *s, char *law, short *sl));
void G726_subta ARGS((short *sl, short *se, short *d));
void G726_log ARGS((short *d, short *dl, short *ds));
void G726_quan ARGS((SHORT rate, short *dln, short *ds, short *i));
void G726_subtb ARGS((short *dl, short *y, short *dln));
void G726_adda ARGS((short *dqln, short *y, short *dql));
void G726_antilog ARGS((short *dql, short *dqs, short *dq));
void G726_reconst ARGS((SHORT rate, short *i, short *dqln, short *dqs));
void G726_delaya ARGS((short *r, short *x, short *y));
void G726_delayb ARGS((short *r, short *x, short *y));
void G726_delayc ARGS((short *r, long *x, long *y));
void G726_delayd ARGS((short *r, short *x, short *y));
void G726_filtd ARGS((short *wi, short *y, short *yut));
void G726_filte ARGS((short *yup, long *yl, long *ylp));
void G726_functw ARGS((SHORT rate, short *i, short *wi));
void G726_limb ARGS((short *yut, short *yup));
void G726_mix ARGS((short *al, short *yu, long *yl, short *y));
void G726_filta ARGS((short *fi, short *dms, short *dmsp));
void G726_filtb ARGS((short *fi, short *dml, short *dmlp));
void G726_filtc ARGS((short *ax, short *ap, short *app));
void G726_functf ARGS((SHORT rate, short *i, short *fi));
void G726_lima ARGS((short *ap, short *al));
void G726_subtc ARGS((short *dmsp, short *dmlp, short *tdp, short *y, short *ax));
void G726_triga ARGS((short *tr, short *app, short *apr));
void G726_accum ARGS((short *wa1, short *wa2, short *wb1, short *wb2, short *wb3, short *wb4, short *wb5, short *wb6, short *se, short *sez));
void G726_addb ARGS((short *dq, short *se, short *sr));
void G726_addc ARGS((short *dq, short *sez, short *pk0, short *sigpk));
void G726_floata ARGS((short *dq, short *dq0));
void G726_floatb ARGS((short *sr, short *sr0));
void G726_fmult ARGS((short *An, short *SRn, short *WAn));
void G726_limc ARGS((short *a2t, short *a2p));
void G726_limd ARGS((short *a1t, short *a2p, short *a1p));
void G726_trigb ARGS((short *tr, short *ap, short *ar));
void G726_upa1 ARGS((short *pk0, short *pk1, short *a1, short *sigpk, short *a1t));
void G726_upa2 ARGS((short *pk0, short *pk1, short *pk2, short *a2, short *a1, short *sigpk, short *a2t));
void G726_upb ARGS((SHORT rate, short *u, short *b, short *dq, short *bp));
void G726_xor ARGS((short *dqn, short *dq, short *u));
void G726_tone ARGS((short *a2p, short *tdp));
void G726_trans ARGS((short *td, long *yl, short *dq, short *tr));
void G726_compress ARGS((short *sr, char *law, short *sp));
void G726_sync ARGS((SHORT rate, short *i, short *sp, short *dlnx, short *dsx, char *law, short *sd));

/* Definitions for better user interface (?!) */
#ifndef IS_LOG 
#  define IS_LOG   0
#endif

#ifndef IS_LIN
#  define IS_LIN   1
#endif

#ifndef IS_ADPCM
#  define IS_ADPCM 2
#endif

#endif
/* .......................... End of G726.H ........................... */
