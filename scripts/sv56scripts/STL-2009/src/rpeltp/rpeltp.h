/* -------------------------------------------------------------------------- */
/* File: RPELTP.H version 1.0                           2-APR-1994 (18:00)    */
/* -------------------------------------------------------------------------- */
/*                 Definitions for the rpeltp module                          */
/* -------------------------------------------------------------------------- */
#ifndef RPELTP_defined
#define RPELTP_defined 100

#ifndef ARGS
#if defined(MSDOS) || defined(VMS) || defined (__STDC__)
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* Some basic definitions */
#define RPE_FRAME_SIZE 76L
#define RPE_WIND_SIZE 160L

/* Function prototypes */
gsm rpeltp_init ARGS((void));
void rpeltp_encode ARGS((gsm rpe_state, short *inp_buf, short *rpe_frame));
void rpeltp_decode ARGS((gsm rpe_state, short *rpe_frame, short *out_buf));

/* Alias-function definition */
#define rpeltp_delete gsm_destroy

#endif /* RPELTP_defined */

/* ------------------------- End of file rpeltp.h ------------------------- */
