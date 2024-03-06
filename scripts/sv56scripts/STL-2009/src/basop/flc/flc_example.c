/*===================================================================*
*  Complexity evaluation tool for floating point C Code
*  Software Release 1.0 (2009-09)
*
* (C) 2009 VoiceAge Corporation. All rights reserved.
*
* VoiceAge Corp. makes no representation nor warranty in regard to
* the accuracy, completeness or sufficiency of The Software, nor
* shall VoiceAge Corp. be held liable for any damages whatsoever
* relating to use of said Software.
*===================================================================*/


/*
 * flc_example.c
 *
 * Examples and guidelines for the use of the computational complexity
 * and Program ROM estimation tool.
 *
 * The Levinson-Durbin and autocorrelation routines of the G.722.2
 * floating-point ANSI C source code are adopted as examples.
 *
 */

#include <stdlib.h>
#include <math.h>
#include "flc.h"

#define  M         16
#define  L_WINDOW  256
#define  PI2       6.283185307F

typedef float      Float32;
typedef long int   Word32;

Float32 E_ROM_hamming_cos[L_WINDOW];


void E_UTIL_autocorr(Float32 *x, Float32 *r);
void E_LPC_lev_dur(Float32 *a, Float32 *r, Word32 m);
void Set_Zero(Float32 *p, int c);


int main()
{
   Float32 input[L_WINDOW];
   Float32 r[M+1], a[M+1];
   int n;

   /* Data initialisation, which is not counted for this example
    * as normally this data would be in ROM and input data. */
   for (n = 0; n < L_WINDOW; n++)
   {
      E_ROM_hamming_cos[n] =
         0.54F - 0.46F * (float)cos((PI2/(L_WINDOW-1)) * (float)n);
      input[n] = 32768.0F * ((2*rand()/(float)RAND_MAX)-1.0F);
   }
   /* FLC_init() must be called before any wmops counters
    * to initialise internal data structures. */
                                            FLC_init();

   for (n = 0; n < 100; n++)
   {
      E_UTIL_autocorr( input, r );          FUNC(2);
      E_LPC_lev_dur( a, r, M );             FUNC(3);

      /* The FLC tool can keep track of per-frame maxima to evaluate
       * the worst-case conditions. The function FLC_frame_update
       * must be called at the end of the frame loop. */
                                            FLC_frame_update();
   }

   /* FLC_end() is the routine that computes and prints the complexity
    * of the program. */
                                            FLC_end();
   return(0);
}

/*
 * E_UTIL_autocorr
 * ===============
 * Parameters:
 *    x           I: input signal
 *    r           O: autocorrelations
 *
 * Function:
 *    Compute the autocorrelations of windowed speech signal.
 *    order of LP filter is M. Window size is L_WINDOW.
 *    Analysis window is "window".
 *
 * Returns:
 *    void
 */
void E_UTIL_autocorr(Float32 *x, Float32 *r)
{
   Float32 t[L_WINDOW + M];
   Word32 i, j;
                                            FLC_sub_start("Autocorr");

                                            BRANCH(1);TEST(1);
   if (x == NULL || r == NULL)
   {
      fprintf(stderr, "x and r pointer must be non-NULL\n");
   }

   /* Initialise pointers into t[], x[], and E_ROM_hamming_cos[]. */
                                            LOOP(1);PTR_INIT(3);
   for (i = 0; i < L_WINDOW; i += 4)
   {
      /* No INDIRECT is needed since access is via pointers.  This is
       * an unrolled loop, but see below for a more extreme example.
       * Note that in contrast to the one below, the unrolling provides
       * no savings as far as the wmops count is concerned, since it is
       * only a partial unrolling.

       * Free pointer updates are limited to increment, decrement, increment
       * by a constant, and increment by a constant with modulo. Other,
       * more complex pointer operations require arithmetic instructions,
       * that is, an accumulator and are charged accordingly. */

      t[i] = x[i] * E_ROM_hamming_cos[i];              MULT(4);STORE(4);
      t[i + 1] = x[i + 1] * E_ROM_hamming_cos[i + 1];
      t[i + 2] = x[i + 2] * E_ROM_hamming_cos[i + 2];
      t[i + 3] = x[i + 3] * E_ROM_hamming_cos[i + 3];
   }

   /* Note that the memset function in the original AMR-WB code clear
    * M and M+1 locations. Internal system functions should be avoided.
    * Instead, new functions should be called for complexity evaluation
    * with the complexity counters included.

    * However, the equivalent count can be gotten also from
    *
    *   memset(&t[L_WINDOW], 0, M*sizeof(Float32));
    *           FUNC(2);LOOP(1);PTR_INIT(1);MOVE(1);STORE(M-1);
    *   memset(r, 0, (M + 1)*sizeof(Float32));
    *           FUNC(2);LOOP(1);PTR_INIT(1);MOVE(1);STORE(M);

    * Also, note that the expressions &t[L_WINDOW] and M+1 evaluate to
    * constants at compile-time and thus are not counted at runtime. */

   Set_Zero(&t[L_WINDOW], M);               FUNC(2);
   Set_Zero(r, M + 1);                      FUNC(2);

   /* Initialise the t[j] pointer. */
                                            LOOP(1);PTR_INIT(1);
   for (j = 0; j < L_WINDOW; j++)
   {
      /* This is an example of a fully unrolled loop saving saving
       * a few instructions. r[] and t[] must still be initialised,
       * since they will be handled as autoincrementing pointers. */
                                            PTR_INIT(2);
      r[0] += t[j] * t[j];                  MAC(17);STORE(17);
      r[1] += t[j] * t[j + 1];
      r[2] += t[j] * t[j + 2];
      r[3] += t[j] * t[j + 3];
      r[4] += t[j] * t[j + 4];
      r[5] += t[j] * t[j + 5];
      r[6] += t[j] * t[j + 6];
      r[7] += t[j] * t[j + 7];
      r[8] += t[j] * t[j + 8];
      r[9] += t[j] * t[j + 9];
      r[10] += t[j] * t[j + 10];
      r[11] += t[j] * t[j + 11];
      r[12] += t[j] * t[j + 12];
      r[13] += t[j] * t[j + 13];
      r[14] += t[j] * t[j + 14];
      r[15] += t[j] * t[j + 15];
      r[16] += t[j] * t[j + 16];
   }

   /* For branches, comparisons are always against zero; thus x<1 is
    * thought as (x-1)<0.  Note also a read from r[0]. */

                                            INDIRECT(1);ADD(1);BRANCH(1);
   if (r[0] < 1.0F)
   {
      r[0] = 1.0F;                          MOVE(1);
   }
                                            FLC_sub_end();
   return;
}

/*
 * E_LPC_lev_dur
 * =============
 * Parameters:
 *    a           O: LP coefficients (a[0] = 1.0)
 *    r           I: vector of autocorrelations
 *    m           I: number of coefficients
 *
 * Function:
 *    Wiener-Levinson-Durbin algorithm to compute
 *    the LPC parameters from the autocorrelations of speech.
 *
 * Returns:
 *    void
 */
void E_LPC_lev_dur(Float32 *a, Float32 *r, Word32 m)
{
   Float32 buf[M];
   Float32 *rc;          /* reflection coefficients 0,...,m-1 */
   Float32 s, at, err;
   Word32  i, j;
                                            FLC_sub_start("Lev_dur");

   /* rc is a pointer getting initialised. Access via this pointer
    * will not be an INDIRECT anymore, and writes to it become
    * STORE rather than MOVE. */
   rc = &buf[0];                            PTR_INIT(1);

   /* Access to r[] requires an INDIRECT. Note that a negation
    * is a multiplication by -1.0. */
   rc[0] = (-r[1]) / r[0];                  MULT(1);INDIRECT(2);DIV(1);STORE(1);

   /* Since there are no other operations, these are MOVEs. */
   a[0] = 1.0F;                             MOVE(1);
   a[1] = rc[0];                            MOVE(1);

   /* Since err is a local variable, no MOVE or STORE is required
    * when some other operations are done. */
   err = r[0] + r[1] * rc[0];               INDIRECT(2);MAC(1);

   /* Note that the PTR_INIT for rc[] was done above. */
                                            LOOP(1);
   for (i = 2; i <= m; i++)
   {
      s = 0.0F;                             MOVE(1);

      /* Two pointers used: one into r[] and one into a[]. */
                                            LOOP(1);PTR_INIT(2);
      for (j = 0; j < i; j++)
      {
         s += r[i - j] * a[j];              MAC(1);
      }

      /* This is a STORE since the pointer was initialised. */
      rc[i - 1]= (-s) / (err);              MULT(1);DIV(1);STORE(1);

      /* Two extra pointers required into a[j] and a[i-j]. */
                                            LOOP(1);SHIFT(1);PTR_INIT(2);
      for (j = 1; j <= (i >> 1); j++)
      {
         at = a[j] + rc[i - 1] * a[i-j];    MAC(1);
         a[i-j] += rc[i - 1] * a[j];        MAC(1);STORE(1);
         a[j] = at;                         MOVE(1);
      }

      a[i] = rc[i - 1];                     MOVE(1);
      err += rc[i - 1] * s;                 MAC(1);

                                            BRANCH(1);
      if (err <= 0.0F)
      {
         err = 0.01F;                       MOVE(1);
      }
   }
                                            FLC_sub_end();
   return;
}

/*
 * Set_Zero
 * ========
 * Parameters:
 *    p           O: array to be zeroed
 *    c           I: number of elements
 *
 * Function:
 *    A simple replacment of the memset function specifically
 *    for Float32.
 *
 * Returns:
 *    void
 */
void Set_Zero(Float32 *p, int c)
{
   int n;
                                            FLC_sub_start("Set_Zero");
                                            LOOP(1);PTR_INIT(1);
   for (n = 0; n < c; n++)
   {
      p[n] = 0.0F;                          MOVE(1);
   }
                                            FLC_sub_end();
}
