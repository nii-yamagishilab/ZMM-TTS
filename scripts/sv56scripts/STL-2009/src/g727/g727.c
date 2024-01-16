/*                                                          19.May.2000 v1.02
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


MODULE: G727.C, G.727 embedded ADPCM algorithm operating with
                      5-, 4-, 3-, and 2-bits per sample

ORIGINAL BY:

     Nikolai L. Birioukov
     Maxim A. Lagerev

     Ukrainian Research Institut of Telecommunications

     P.O. 252650
     13, Solomenskaya St.
     Kiev, Ukraine

     Phone : +380 44 276 51 01
     FAX   : +380 44 226 26 46
     EMail : INFO@UNIIS,KIEV,UA

FUNCTIONS:

  G727_reset ...... G727 reset function;
  G727_encode ..... G727 encoder function;
  G727_decode ..... G727 decoder function;

HISTORY:
  01.Apr.1995  0.98  Version of the G727 module in C++ code
  10.Jul.1996  0.99  C code version released to UGST <info@uniis.kiev.ua>
  10.Mar.1997  1.00  Conversion to UGST format <simao.campos@comsat.com>
  04.Aug.1997  1.01  Eliminated compilation warning about unused variables
                     as per revision from <Morgan.Lindqvist@era-t.ericsson.se>
  19.May.2000  v1.02 Corrected self-documentation of functions. <simao>
=============================================================================
*/

#include <assert.h>
#include "g727.h"

/* Local function prototypes */
Int16 g727_get_d ARGS((Int8 s, Int16 se, short law));
Int16 g727_expand ARGS((Int8 sp, short law));
Int16 g727_subta ARGS((Int16 sl, Int16 se));
Int8 g727_get_in ARGS((Int16 d, Int16 y, short rate));
void g727_log ARGS((Int16 *dl, Int8 *ds, Int16 d));
Int8 g727_quan ARGS((Int16 dln, Int8 ds, short rate));
Int16 g727_subtb ARGS((Int16 dl, Int16 y));
Int8 g727_get_ic ARGS((Int8 in, short enh));
Int16 g727_get_dq ARGS((Int16 y, Int8 in, short rate));
Int16 g727_antilog ARGS((Int16 dql, Int8 dqs));
Int16 g727_adda ARGS((Int16 dqln, Int16 y));
void g727_reconst ARGS((Int16 *dqln, Int8 *dqs, Int8 in, short rate));
void g727_qsfa_reset ARGS((g727_qsfa_state *st));
Int16 g727_get_y ARGS((g727_qsfa_state *st, Int8 al));
Int32 g727_get_yl ARGS((g727_qsfa_state *st));
void g727_qsfa_transit ARGS((g727_qsfa_state *st, Int8 in, Int16 y,
			     short rate));
Int16 g727_filtd ARGS((Int16 wi, Int16 y));
Int32 g727_filte ARGS((g727_qsfa_state *st, Int16 yup));
Int16 g727_functw ARGS((Int8 in, short rate));
Int16 g727_limb ARGS((Int16 yut));
Int16 g727_mix ARGS((g727_qsfa_state *st, Int8 al));
void g727_asc_reset ARGS((g727_asc_state *st));
Int8 g727_get_al ARGS((g727_asc_state *st));
void g727_asc_transit ARGS((g727_asc_state *st, Int8 in, Int16 y, Int8 tr,
			    Int8 tdp, short rate));
Int16 g727_filta ARGS((g727_asc_state *st, Int8 fi));
Int16 g727_filtb ARGS((g727_asc_state *st, Int8 fi));
Int16 g727_filtc ARGS((g727_asc_state *st, Int8 ax));
Int8 g727_functf ARGS((Int8 in, short rate));
Int8 g727_lima ARGS((g727_asc_state *st));
Int8 g727_subtc ARGS((Int16 dmsp, Int16 dmlp, Int8 tdp, Int16 y));
Int16 g727_triga ARGS((Int8 tr, Int16 app));
void g727_aprsc_reset ARGS((g727_aprsc_state *st));
Int16 g727_get_sr ARGS((Int16 dq, Int16 se));
void g727_get_se_sez ARGS((g727_aprsc_state *st, Int16 *se, Int16 *sez));
Int16 g727_get_a2p ARGS((g727_aprsc_state *st, Int16 dq, Int16 sez,
			 Int8 *pk0, Int8 *sigpk));
void g727_aprsc_transit ARGS((g727_aprsc_state *st, Int16 dq, Int8 tr,
			      Int16 sr, Int16 a2p, Int8 pk0, Int8 sigpk));
void g727_accum ARGS((Int16 *se, Int16 *sez, Int16 wa1, Int16 wa2, Int16 wb1,
		      Int16 wb2, Int16 wb3, Int16 wb4, Int16 wb5, Int16 wb6));
Int16 g727_addb ARGS((Int16 dq, Int16 se));
void g727_addc ARGS((Int8 *pk0, Int8 *sigpk, Int16 dq, Int16 sez));
Int8 g727_makexp ARGS((Int16 mag));
Int16 g727_floata ARGS((Int16 dq));
Int16 g727_floatb ARGS((Int16 sr));
Int16 g727_fmult ARGS((Int16 an, Int16 srn));
Int16 g727_limc ARGS((Int16 a2t));
Int16 g727_limd ARGS((Int16 a1t, Int16 a2p));
Int16 g727_aprsc_trigb ARGS((Int8 tr, Int16 anp));
Int16 g727_upa1 ARGS((g727_aprsc_state *st, Int8 pk0, Int8 sigpk));
Int16 g727_upa2 ARGS((g727_aprsc_state *st, Int8 pk0, Int8 sigpk));
Int16 g727_upb ARGS((Int8 un, Int16 bn, Int16 dq));
Int8 g727_xor ARGS((Int16 dqn, Int16 dq));
Int16 g727_get_srff ARGS((Int16 dqff, Int16 se));
void g727_ttd_reset ARGS((g727_ttd_state *st));
Int8 g727_get_tr ARGS((g727_ttd_state *st, Int32 yl, Int16 dq));
Int8 g727_get_tdp ARGS((Int16 a2p));
void g727_ttd_transit ARGS((g727_ttd_state *st, Int8 tr, Int8 tdp));
Int8 g727_tone ARGS((Int16 a2p));
Int8 g727_trans ARGS((g727_ttd_state *st, Int32 yl, Int16 dq));
Int8 g727_ttd_trigb ARGS((Int8 tr, Int8 tdp));
Int8 g727_get_sd ARGS((Int16 sr, Int16 se, Int8 in, Int16 y, short law,
		       short rate));
Int8 g727_compress ARGS((Int16 sr, short law));
Int8 g727_sync ARGS((Int8 in, Int8 sp, Int16 dlnx, short law, Int8 ds,
		     short rate));




/*---------- Constant tables ----------*/

static Int16 g727_dqlnTable40[32] = {
    3832, 4035,   34,   97,  145,  184,  217,  246,
     273,  298,  321,  344,  367,  391,  419,  456,
     456,  419,  391,  367,  344,  321,  298,  273,
     246,  217,  184,  145,   97,   34, 4035, 3832
};

static Int16 g727_dqlnTable32[16] = {
    3961,   68,  165,  232,  285,  332,  377,  428,
     428,  377,  332,  285,  232,  165,   68, 3961
};

static Int16 g727_dqlnTable24[8] = {
    4085,  199,  307,  395,  395,  307,  199, 4085
};

static Int16 g727_dqlnTable16[4] = {
    116, 365, 365, 116
};

static Int16 g727_wiTable32[8] = {
    4084, 4, 27, 50, 98, 184, 340, 1108
};

static Int16 g727_wiTable24[4] = {
    4092, 30, 137, 582
};

static Int16 g727_wiTable16[2] = {
    4074, 439
};

static Int8 g727_fiTable32[8] = {
    0, 0, 0, 1, 1, 1, 3, 7
};

static Int8 g727_fiTable24[4] = {
    0, 1, 2, 7
};

static Int8 g727_fiTable16[2] = {
    0, 7
};


/*---------- G.727 ADPCM Encoder & Decoder ----------*/


/*
  ----------------------------------------------------------------------------

  void g727_reset (g727_state *st);

  Description:
  ~~~~~~~~~~~~

  Reset ITU-T G.727 embedded ADPCM encoder or decoder state variable.
  The state variable is defined in g727.h.

  Return value:
  ~~~~~~~~~~~~~
  None.

  Prototype:      in file g727.h
  ~~~~~~~~~~

  History:
  ~~~~~~~~
  10.Jul.96  0.99  C code version released to UGST <info@uniis.kiev.ua>
  10.Mar.97  1.00  Conversion to UGST format <simao.campos@comsat.com>
 ----------------------------------------------------------------------------
*/
void
g727_reset(g727_state *st)
{
    g727_qsfa_reset(&st->qsfa);
    g727_asc_reset(&st->asc);
    g727_aprsc_reset(&st->aprsc);
    g727_ttd_reset(&st->ttd);
}
/* ........................ End of G727_reset() ........................ */



/*
  ----------------------------------------------------------------------------

  void g727_encode_block (short *src, short *dst, short n, 
  ~~~~~~~~~~~~~~~~~~~~~~  short law, short cbits, short ebits, g727_state *st);

  Description:
  ~~~~~~~~~~~~

  Simulation of the ITU-T G.727 embedded ADPCM encoder. Takes the A or
  mu law input array of shorts `src' (16 bit, right- justified,
  without sign extension) of length `n', and saves the encoded samples
  in the array of shorts `dst', with the same number of samples and
  right-justified. The ADPCM samples will have cbits core bits, and
  ebits enhancement bits.

  The state variables are saved in the structure `g727_state'. The law
  is A if `law'=='1', and mu law if `law'=='0'.
	
  Parameters:
  ~~~~~~~~~~~
  src .......... A- or u-law 16-bit right justified sample to encode
  dst .......... 16-bit right justified ADPCM-encoded sample with cbits
                 core bits and ebits enhancement bits
  n ............ Number of samples to encode.
  law .......... encoding law (character '1'=A-law, character '0'=u-law).
  cbits ........ number of core bits
  ebits ........ number of enhancement bits
  g727_state ... G.727 state variable structure

  Return value:
  ~~~~~~~~~~~~~
  None.

  Prototype:      in file g727.h
  ~~~~~~~~~~

  History:
  ~~~~~~~~
  10.Jul.96  0.99  C code version released to UGST <info@uniis.kiev.ua>
  10.Mar.97  1.00  Conversion to UGST format <simao.campos@comsat.com>
 ----------------------------------------------------------------------------
*/
void
g727_encode_block(short *src, short *dst, short n, short law, short cbits,
                                                              short ebits,
                                                              g727_state *st)
{
    short *eptr = src + n;

    /* Fix for compatibility with g726_encode() definitions */
    if (law == '1')
      law = 1;
    else if (law == '0')
      law = 0;

    /* Encode all samples in buffer */
    while (src < eptr)
        *dst++ = g727_encode_sample(*src++, law, cbits, ebits, st);
}
/* ..................... End of G727_encode_block() ..................... */


/*
  ----------------------------------------------------------------------------

  void g727_decode_block (short *src, short *dst, short n,
  ~~~~~~~~~~~~~~~~~~~~~~  short law, short cbits, short ebits, g727_state *st);

  Description:
  ~~~~~~~~~~~~

  Simulation of the ITU-T G.727 embedded ADPCM decoder. Takes the
  ADPCM input array of shorts `src' (16 bit, right- justified, without
  sign extension) of length `n', and saves the decoded samples (A or
  mu law) in the array of shorts `dst', with the same number of
  samples and right-justified. The ADPCM samples must have cbits core
  bits, and ebits enhancement bits.

  The state variables are saved in the structure `g727_state'. The law
  is A if `law'=='1', and mu law if `law'=='0'.

	
  Parameters:
  ~~~~~~~~~~~
  src .......... 16-bit right justified ADPCM-encoded sample with cbits
                 core bits and ebits enhancement bits
  dst .......... A- or u-law 16-bit right justified sample to encode
  n ............ Number of samples to encode.
  law .......... encoding law (character '1'=A-law, character '0'=u-law).
  cbits ........ number of core bits
  ebits ........ number of enhancement bits
  g727_state ... G.727 state variable structure

  Return value:
  ~~~~~~~~~~~~~
  None.

  Prototype:      in file g727.h
  ~~~~~~~~~~

  History:
  ~~~~~~~~
  10.Jul.96  0.99  C code version released to UGST <info@uniis.kiev.ua>
  10.Mar.97  1.00  Conversion to UGST format <simao.campos@comsat.com>
 ----------------------------------------------------------------------------
*/
void
g727_decode_block(short *src, short *dst, short n, short law, short cbits,
                                                              short ebits,
                                                              g727_state *st)
{
    short *eptr = src + n;

    /* Fix for compatibility with g726_decode() definitions */
    if (law == '1')
      law = 1;
    else if (law == '0')
      law = 0;

    /* Decode all samples in buffer */
    while (src < eptr)
        *dst++ = g727_decode_sample(*src++, law, cbits, ebits, st);
}
/* ...................... End of G727_decode_block() ..................... */


/*
  ----------------------------------------------------------------------------

  short g727_encode_sample (short code, short law, short cbits, 
  ~~~~~~~~~~~~~~~~~~~~~~~~  short ebits, g727_state *st);

  Description:
  ~~~~~~~~~~~~

  Simulation of the ITU-T G.727 embedded ADPCM encoder. Takes one A or
  mu law (16 bit, right- justified, without sign extension) sample in
  'code' and returns the ADPCM-encoded sample using the law and number
  of core and enhancement bits specified by the user.  Samples are
  returned right-justified.

  The state variables are saved in the structure `g727_state'. The law
  is A if `law'=='1', and mu law if `law'=='0'.
	
  Parameters:
  ~~~~~~~~~~~
  code ......... A- or u-law 16-bit right justified sample to encode
  law .......... encoding law (character '1'=A-law, character '0'=u-law).
  cbits ........ number of core bits
  ebits ........ number of enhancement bits
  g727_state ... G.727 state variable structure

  Return value:
  ~~~~~~~~~~~~~
  16-bit right justified ADPCM-encoded sample with cbits core bits and
  ebits enhancement bits

  Prototype:      in file g727.h
  ~~~~~~~~~~

  History:
  ~~~~~~~~
  10.Jul.96  0.99  C code version released to UGST <info@uniis.kiev.ua>
  10.Mar.97  1.00  Conversion to UGST format <simao.campos@comsat.com>
 ----------------------------------------------------------------------------
*/
short g727_encode_sample(short code, short law, short cbits, short ebits, g727_state *st)
{
  Int16 se;
  Int16 sez;
  Int8 al;
  Int16 y;
  Int32 yl;
  Int16 d;
  Int8 in;
  Int8 ic;
  Int16 dq;
  Int16 sr;
  Int8 pk0;
  Int8 sigpk;
  Int16 a2p;
  Int8 tr;
  Int8 tdp;

  assert(2 <= cbits && cbits <= 4 && ebits >= 0 && cbits + ebits <= 5);

    g727_get_se_sez(&st->aprsc, &se, &sez);

    al = g727_get_al(&st->asc);

    y = g727_get_y(&st->qsfa, al);
    yl = g727_get_yl(&st->qsfa);

    d = g727_get_d((Int8)code, se, law);

    in = g727_get_in(d, y, cbits + ebits);
    ic = g727_get_ic(in, ebits);

    dq = g727_get_dq(y, ic, cbits);

    sr = g727_get_sr(dq, se);
    a2p = g727_get_a2p(&st->aprsc, dq, sez, &pk0, &sigpk);

    tr = g727_get_tr(&st->ttd, yl, dq);
    tdp = g727_get_tdp(a2p);

    g727_qsfa_transit(&st->qsfa, ic, y, cbits);
    g727_asc_transit(&st->asc, ic, y, tr, tdp, cbits);
    g727_aprsc_transit(&st->aprsc, dq, tr, sr, a2p, pk0, sigpk);
    g727_ttd_transit(&st->ttd, tr, tdp);

    return in;
}
/* ..................... End of G727_encode_sample() ..................... */


/*
  ----------------------------------------------------------------------------

  void g727_decode_sample (short code, short law, short cbits, 
  ~~~~~~~~~~~~~~~~~~~~~~~  short ebits, g727_state *st);

  Description:
  ~~~~~~~~~~~~

  Simulation of the ITU-T G.727 embedded ADPCM decoder. Takes the
  ADPCM short sample in 'code' (16 bit, right- justified, without sign
  extension), and returns the decoded sample (A or mu law),
  right-justified.

  The state variables are saved in the structure `g727_state'. The law
  is A if `law'=='1', and mu law if `law'=='0'.
	
  Parameters:
  ~~~~~~~~~~~
  code ......... 16-bit right justified ADPCM-encoded sample with cbits
                 core bits and ebits enhancement bits
  law .......... encoding law (character '1'=A-law, character '0'=u-law).
  cbits ........ number of core bits
  ebits ........ number of enhancement bits
  g727_state ... G.727 state variable structure

  Return value:
  ~~~~~~~~~~~~~
  16-bit right justified decoded A- or u-law sample

  Prototype:      in file g727.h
  ~~~~~~~~~~

  History:
  ~~~~~~~~
  10.Jul.96  0.99  C code version released to UGST <info@uniis.kiev.ua>
  10.Mar.97  1.00  Conversion to UGST format <simao.campos@comsat.com>
 ----------------------------------------------------------------------------
*/
short
g727_decode_sample(short code, short law, short cbits, short ebits, g727_state *st)
{
    Int8 ic;
    Int8 al;
    Int16 y;
    Int32 yl;
    short rt;
    Int16 dqfb;
    Int16 dqff;
    Int16 se;
    Int16 sez;
    Int16 srfb;
    Int8 pk0;
    Int8 sigpk;
    Int16 a2p;
    Int16 srff;
    Int8 tr;
    Int8 tdp;
    Int8 sd;

    assert(2 <= cbits && cbits <= 4 && ebits >= 0 && cbits + ebits <= 5);


    ic = g727_get_ic((Int8)code, ebits);

    al = g727_get_al(&st->asc);

    y = g727_get_y(&st->qsfa, al);
    yl = g727_get_yl(&st->qsfa);

    rt = cbits + ebits;

    dqfb = g727_get_dq(y, ic, cbits);
    dqff = g727_get_dq(y, (Int8)code, rt);

    g727_get_se_sez(&st->aprsc, &se, &sez);
    srfb = g727_get_sr(dqfb, se);
    a2p = g727_get_a2p(&st->aprsc, dqfb, sez, &pk0, &sigpk);

    srff = g727_get_srff(dqff, se);

    tr = g727_get_tr(&st->ttd, yl, dqfb);
    tdp = g727_get_tdp(a2p);

    sd = g727_get_sd(srff, se, (Int8)code, y, law, rt);

    g727_qsfa_transit(&st->qsfa, ic, y, cbits);
    g727_asc_transit(&st->asc, ic, y, tr, tdp, cbits);
    g727_aprsc_transit(&st->aprsc, dqfb, tr, srfb, a2p, pk0, sigpk);
    g727_ttd_transit(&st->ttd, tr, tdp);

    return sd;
}
/* ..................... End of G727_decode_sample() ..................... */




/* ********************************************************************** *
 * ******** PRIVATE FUNCTIONS - DO NOT GO BEYOND THIS POINT ... ********* *
 * ********************************************************************** */

/*---------- Input PCM format conversion and difference ----------*/
/*---------- signal computation (G.727 6.2.1) --------------------*/

Int16
g727_get_d(Int8 s, Int16 se, short law)
{
    Int8 sp = (law == 1) ? (0xd5 ^ s) : (0xff ^ s);

    return g727_subta(g727_expand(sp, law), se);
}


Int16
g727_expand(Int8 sp, short law)
{
    Int16 ss = 0x21 | ((sp & 0xf) << 1);
    Int8 sss;
    Int16 ssq;
    Int16 sl;

    if (law == 1) {
        if ((sp & 0x70) == 0)
            ss ^= 0x20;
        else
            ss <<= ((sp & 0x70) >> 4) - 1;

        ss |= (sp & 0x80) << 5;
    }
    else if (law == 0) {
        ss <<= (sp & 0x70) >> 4;
        ss -= 33;
        ss |= (sp & 0x80) << 6;
    }
    else
        assert(0);

    if (law == 0) {
        sss = ss >> 13;
        ssq = ss & 8191;
    } else {
        Int16 ssm;

        sss = ss >> 12;
        ssm = ss & 4095;
        ssq = ssm << 1;
    }

    if (sss == 0)
        sl = ssq;
    else if (sss == 1)
        sl = (16384 - ssq) & 16383;
    else
        assert(0);

    return sl;
}


Int16
g727_subta(Int16 sl, Int16 se)
{
    Int8 sls = sl >> 13;
    Int16 sli;
    Int8 ses;
    Int16 sei;

    if (sls == 0)
        sli = sl;
    else if (sls == 1)
        sli = 49152 + sl;
    else
        assert(0);

    ses = se >> 14;

    if (ses == 0)
        sei = se;
    else if (ses == 1)
        sei = 32768 + se;
    else
        assert(0);

    return (sli + 65536UL - sei) & 65535;
}


/*---------- Adaptive quantizer (G.727 6.2.2) ----------*/

Int8
g727_get_in(Int16 d, Int16 y, short rate)
{
    Int16 dl;
    Int8 ds;

    g727_log(&dl, &ds, d);

    return g727_quan(g727_subtb(dl, y), ds, rate);
}


void
g727_log(Int16 *dl, Int8 *ds, Int16 d)
{
    Int16 dqm;
    Int8 exp;
    Int8 mant;

    *ds = d >> 15;

    if (*ds == 0)
        dqm = d;
    else if (*ds == 1)
        dqm = (65536 - d) & 32767;
    else
        assert(0);

    if (16384 <= dqm)
        exp = 14;
    else if (8192 <= dqm)
        exp = 13;
    else if (4096 <= dqm)
        exp = 12;
    else if (2048 <= dqm)
        exp = 11;
    else if (1024 <= dqm)
        exp = 10;
    else if (512 <= dqm)
        exp = 9;
    else if (256 <= dqm)
        exp = 8;
    else if (128 <= dqm)
        exp = 7;
    else if (64 <= dqm)
        exp = 6;
    else if (32 <= dqm)
        exp = 5;
    else if (16 <= dqm)
        exp = 4;
    else if (8 <= dqm)
        exp = 3;
    else if (4 <= dqm)
        exp = 2;
    else if (2 <= dqm)
        exp = 1;
    else
        exp = 0;

    mant = (((Int32)dqm << 7) >> exp) & 127;

    *dl = ((Int16)exp << 7) + mant;
}


Int8
g727_quan(Int16 dln, Int8 ds, short rate)
{
    Int8 in;

    switch (rate) {
        case 5:
            if (4096 <= dln)
                assert(0);
            else if (4089 <= dln)
                in = 2;
            else if (3961 <= dln)
                in = 1;
            else if (2048 <= dln)
                in = 0;
            else if (439 <= dln)
                in = 15;
            else if (405 <= dln)
                in = 14;
            else if (380 <= dln)
                in = 13;
            else if (356 <= dln)
                in = 12;
            else if (333 <= dln)
                in = 11;
            else if (310 <= dln)
                in = 10;
            else if (286 <= dln)
                in = 9;
            else if (261 <= dln)
                in = 8;
            else if (233 <= dln)
                in = 7;
            else if (202 <= dln)
                in = 6;
            else if (166 <= dln)
                in = 5;
            else if (123 <= dln)
                in = 4;
            else if (69 <= dln)
                in = 3;
            else
                in = 2;

            in = (ds == 1) ? ((~in) & 0x1f) : in;
            break;

        case 4:
            if (4096 <= dln)
                assert(0);
            else if (4089 <= dln)
                in = 1;
            else if (2048 <= dln)
                in = 0;
            else if (405 <= dln)
                in = 7;
            else if (356 <= dln)
                in = 6;
            else if (310 <= dln)
                in = 5;
            else if (261 <= dln)
                in = 4;
            else if (202 <= dln)
                in = 3;
            else if (123 <= dln)
                in = 2;
            else
                in = 1;

            in = (ds == 1) ? ((~in) & 0xf) : in;
            break;

        case 3:
            if (4096 <= dln)
                assert(0);
            else if (2048 <= dln)
                in = 0;
            else if (356 <= dln)
                in = 3;
            else if (261 <= dln)
                in = 2;
            else if (123 <= dln)
                in = 1;
            else
                in = 0;

            in = (ds == 1) ? ((~in) & 0x7) : in;
            break;

        case 2:
            if (4096 <= dln)
                assert(0);
            else if (2048 <= dln)
                in = 0;
            else if (261 <= dln)
                in = 1;
            else
                in = 0;

            in = (ds == 1) ? ((~in) & 0x3) : in;
            break;

        default:
            assert(0);
    }

    return in;
}


Int16
g727_subtb(Int16 dl, Int16 y)
{
    return (dl + 4096 - (y >> 2)) & 4095;
}


/*---------- Bit masking (G.727 6.2.3) ----------*/

Int8
g727_get_ic(Int8 in, short enh)
{
    return in >> enh;
}


/*---------- Inverse Adaptive Quantizer (G.727 6.2.4) ----------*/

Int16
g727_get_dq(Int16 y, Int8 in, short rate)
{
    Int16 dqln;
    Int8 dqs;

    g727_reconst(&dqln, &dqs, in, rate);

    return g727_antilog(g727_adda(dqln, y), dqs);
}


Int16
g727_antilog(Int16 dql, Int8 dqs)
{
    Int8 ds = dql >> 11;
    Int8 dex = (dql >> 7) & 15;
    Int8 dmn = dql & 127;

    Int8 dqt = (1 << 7) + dmn;
    Int16 dqmag;

    if (ds == 0)
        dqmag = (dqt << 7) >> (14 - dex);
    else if (ds == 1)
        dqmag = 0;
    else
        assert(0);

    return (dqs << 14) + dqmag;
}


Int16
g727_adda(Int16 dqln, Int16 y)
{
    return (dqln + (y >> 2)) & 4095;
}


void
g727_reconst(Int16 *dqln, Int8 *dqs, Int8 in, short rate)
{
    switch (rate) {
        case 5:
            *dqs = in >> 4;
            *dqln = g727_dqlnTable40[in];
            break;

        case 4:
            *dqs = in >> 3;
            *dqln = g727_dqlnTable32[in];
            break;

        case 3:
            *dqs = in >> 2;
            *dqln = g727_dqlnTable24[in];
            break;

        case 2:
            *dqs = in >> 1;
            *dqln = g727_dqlnTable16[in];
            break;

        default:
            assert(0);
    }
}


/*---------- Quantizer Scale Factor Adaptation (G.727 6.2.5) ----------*/

void
g727_qsfa_reset(g727_qsfa_state *st)
{
    st->yl = 34816;
    st->yu = 544;
}


Int16
g727_get_y(g727_qsfa_state *st, Int8 al)
{
    return g727_mix(st, al);
}


Int32
g727_get_yl(g727_qsfa_state *st)
{
    return st->yl;
}


void
g727_qsfa_transit(g727_qsfa_state *st, Int8 in, Int16 y, short rate)
{
    Int16 yup = g727_limb(g727_filtd(g727_functw(in, rate), y));
    Int32 ylp = g727_filte(st, yup);

    st->yu = yup;
    st->yl = ylp;
}


Int16
g727_filtd(Int16 wi, Int16 y)
{
    Int32 dif = (((Int32)wi << 5) + 131072 - y) & 131071;
    Int8 difs = dif >> 16;
    Int16 difsx;

    if (difs == 0)
        difsx = dif >> 5;
    else if (difs == 1)
        difsx = (dif >> 5) + 4096;
    else
        assert(0);

    return (y + difsx) & 8191;
}


Int32
g727_filte(g727_qsfa_state *st, Int16 yup)
{
    Int16 dif = (yup + ((1048576 - st->yl) >> 6)) & 16383;
    Int8 difs = dif >> 13;
    Int32 difsx;

    if (difs == 0)
        difsx = dif;
    else if (difs == 1)
        difsx = dif + 507904;
    else
        assert(0);

    return (st->yl + difsx) & 524287;
}


Int16
g727_functw(Int8 in, short rate)
{
    Int8 is;
    Int8 im;
    Int16 wi;

    switch (rate) {
        case 4:
            is = in >> 3;

            if (is == 0)
                im = in & 7;
            else if (is == 1)
                im = (15 - in) & 7;
            else
                assert(0);

            wi = g727_wiTable32[im];
            break;

        case 3:
            is = in >> 2;

            if (is == 0)
                im = in & 3;
            else if (is == 1)
                im = (7 - in) & 3;
            else
                assert(0);

            wi = g727_wiTable24[im];
            break;

        case 2:
            is = in >> 1;

            if (is == 0)
                im = in & 1;
            else if (is == 1)
                im = (3 - in) & 1;
            else
                assert(0);

            wi = g727_wiTable16[im];
            break;

        default:
            assert(0);
    }

    return wi;
}


Int16
g727_limb(Int16 yut)
{
    Int8 geul = ((yut + 11264) & 16383) >> 13;
    Int8 gell = ((yut + 15840) & 16383) >> 13;
    Int16 yup;

    if (gell == 1)
        yup = 544;
    else if (geul == 0)
        yup = 5120;
    else
        yup = yut;

    return yup;
}


Int16
g727_mix(g727_qsfa_state *st, Int8 al)
{
    Int16 dif = (st->yu + 16384 - (st->yl >> 6)) & 16383;
    Int8 difs = dif >> 13;
    Int16 difm;
    Int16 prodm;
    Int16 prod;

    if (difs == 0)
        difm = dif;
    else if (difs == 1)
        difm = (16384 - dif) & 8191;
    else
        assert(0);

    prodm = ((Int32)difm*al) >> 6;

    if (difs == 0)
        prod = prodm;
    else if (difs == 1)
        prod = (16384 - prodm) & 16383;
    else
        assert(0);

    return ((st->yl >> 6) + prod) & 8191;
}


/*---------- Adaptation Speed Control (G.727 6.2.6) ----------*/

void
g727_asc_reset(g727_asc_state *st)
{
    st->dms = 0;
    st->dml = 0;
    st->ap = 0;
}


Int8
g727_get_al(g727_asc_state *st)
{
    return g727_lima(st);
}


void
g727_asc_transit(g727_asc_state *st, Int8 in, Int16 y, Int8 tr, Int8 tdp,
                                                                short rate)
{
    Int8 fi = g727_functf(in, rate);
    Int16 dmsp = g727_filta(st, fi);
    Int16 dmlp = g727_filtb(st, fi);
    Int16 apr = g727_triga(tr, g727_filtc(st, g727_subtc(dmsp, dmlp, tdp, y)));

    st->dms = dmsp;
    st->dml = dmlp;
    st->ap = apr;
}


Int16
g727_filta(g727_asc_state *st, Int8 fi)
{
    Int16 dif = ((fi << 9) + 8192 - st->dms) & 8191;
    Int8 difs = dif >> 12;
    Int16 difsx;

    if (difs == 0)
        difsx = dif >> 5;
    else if (difs == 1)
        difsx = (dif >> 5) + 3840;
    else
        assert(0);

    return (difsx + st->dms) & 4095;
}


Int16
g727_filtb(g727_asc_state *st, Int8 fi)
{
    Int16 dif = ((fi << 11) + 32768 - st->dml) & 32767;
    Int8 difs = dif >> 14;
    Int16 difsx;

    if (difs == 0)
        difsx = dif >> 7;
    else if (difs == 1)
        difsx = (dif >> 7) + 16128;
    else
        assert(0);

    return (difsx + st->dml) & 16383;
}


Int16
g727_filtc(g727_asc_state *st, Int8 ax)
{
    Int16 dif = ((ax << 9) + 2048 - st->ap) & 2047;
    Int8 difs = dif >> 10;
    Int16 difsx;

    if (difs == 0)
        difsx = dif >> 4;
    else if (difs == 1)
        difsx = (dif >> 4) + 896;
    else
        assert(0);

    return (difsx + st->ap) & 1023;
}


Int8
g727_functf(Int8 in, short rate)
{
    Int8 is;
    Int8 im;
    Int8 fi;

    switch (rate) {
        case 4:
            is = in >> 3;

            if (is == 0)
                im = in & 7;
            else if (is == 1)
                im = (15 - in) & 7;
            else
                assert(0);

            fi = g727_fiTable32[im];
            break;

        case 3:
            is = in >> 2;

            if (is == 0)
                im = in & 3;
            else if (is == 1)
                im = (7 - in) & 3;
            else
                assert(0);

            fi = g727_fiTable24[im];
            break;

        case 2:
            is = in >> 1;

            if (is == 0)
                im = in & 1;
            else if (is == 1)
                im = (3 - in) & 1;
            else
                assert(0);

            fi = g727_fiTable16[im];
            break;

        default:
            assert(0);
    }

    return fi;
}


Int8
g727_lima(g727_asc_state *st)
{
    return (st->ap >= 256) ? 64 : (st->ap >> 2);
}


Int8
g727_subtc(Int16 dmsp, Int16 dmlp, Int8 tdp, Int16 y)
{
    Int16 dif = ((dmsp << 2) + 32768 - dmlp) & 32767;
    Int8 difs = dif >> 14;
    Int16 difm;
    Int16 dthr;

    if (difs == 0)
        difm = dif;
    else if (difs == 1)
        difm = (32768 - dif) & 16383;
    else
        assert(0);

    dthr = dmlp >> 3;

    return (y >= 1536 && difm < dthr && tdp == 0) ? 0 : 1;
}


Int16
g727_triga(Int8 tr, Int16 app)
{
    Int16 apr;

    if (tr == 0)
        apr = app;
    else if (tr == 1)
        apr = 256;
    else
        assert(0);

    return apr;
}


/*---------- Adaptive Predictor and Reconstructed  ----------*/
/*---------- Signal Calculator (G.727 6.2.7) ----------------*/

void
g727_aprsc_reset(g727_aprsc_state *st)
{
    st->pk1 = 0;
    st->pk2 = 0;
    st->sr1 = 32;
    st->sr2 = 32;
    st->dq1 = 32;
    st->dq2 = 32;
    st->dq3 = 32;
    st->dq4 = 32;
    st->dq5 = 32;
    st->dq6 = 32;
    st->b1 = 0;
    st->b2 = 0;
    st->b3 = 0;
    st->b4 = 0;
    st->b5 = 0;
    st->b6 = 0;
    st->a1 = 0;
    st->a2 = 0;
}


Int16
g727_get_sr(Int16 dq, Int16 se)
{
    return g727_addb(dq, se);
}


void
g727_get_se_sez(g727_aprsc_state *st, Int16 *se, Int16 *sez)
{
    g727_accum(se, sez, g727_fmult(st->a1, st->sr1),
                        g727_fmult(st->a2, st->sr2),
                        g727_fmult(st->b1, st->dq1),
                        g727_fmult(st->b2, st->dq2),
                        g727_fmult(st->b3, st->dq3),
                        g727_fmult(st->b4, st->dq4),
                        g727_fmult(st->b5, st->dq5),
                        g727_fmult(st->b6, st->dq6));
}


Int16
g727_get_a2p(g727_aprsc_state *st, Int16 dq, Int16 sez, Int8 *pk0,
                                                        Int8 *sigpk)
{
    g727_addc(pk0, sigpk, dq, sez);

    return g727_limc(g727_upa2(st, *pk0, *sigpk));
}


void
g727_aprsc_transit(g727_aprsc_state *st, Int16 dq, Int8 tr, Int16 sr,
                                                            Int16 a2p,
                                                            Int8 pk0,
                                                            Int8 sigpk)
{
    Int16 sr0;
    Int16 dq0;
    Int16 a1r;
    Int16 a2r;
    Int16 b1r;
    Int16 b2r;
    Int16 b3r;
    Int16 b4r;
    Int16 b5r;
    Int16 b6r;

    sr0 = g727_floatb(sr);

    dq0 = g727_floata(dq);

    a1r = g727_aprsc_trigb(tr, g727_limd(g727_upa1(st, pk0, sigpk), a2p));
    a2r = g727_aprsc_trigb(tr, a2p);

    b1r = g727_aprsc_trigb(tr, g727_upb(g727_xor(st->dq1, dq), st->b1, dq));
    b2r = g727_aprsc_trigb(tr, g727_upb(g727_xor(st->dq2, dq), st->b2, dq));
    b3r = g727_aprsc_trigb(tr, g727_upb(g727_xor(st->dq3, dq), st->b3, dq));
    b4r = g727_aprsc_trigb(tr, g727_upb(g727_xor(st->dq4, dq), st->b4, dq));
    b5r = g727_aprsc_trigb(tr, g727_upb(g727_xor(st->dq5, dq), st->b5, dq));
    b6r = g727_aprsc_trigb(tr, g727_upb(g727_xor(st->dq6, dq), st->b6, dq));

    st->pk2 = st->pk1;
    st->pk1 = pk0;

    st->sr2 = st->sr1;
    st->sr1 = sr0;

    st->a1 = a1r;
    st->a2 = a2r;

    st->b1 = b1r;
    st->b2 = b2r;
    st->b3 = b3r;
    st->b4 = b4r;
    st->b5 = b5r;
    st->b6 = b6r;

    st->dq6 = st->dq5;
    st->dq5 = st->dq4;
    st->dq4 = st->dq3;
    st->dq3 = st->dq2;
    st->dq2 = st->dq1;
    st->dq1 = dq0;
}


void
g727_accum(Int16 *se, Int16 *sez, Int16 wa1, Int16 wa2, Int16 wb1,
                                                        Int16 wb2,
                                                        Int16 wb3,
                                                        Int16 wb4,
                                                        Int16 wb5,
                                                        Int16 wb6)
{
    Int16 sezi = (((((((((wb1 + wb2) & 65535) + wb3) & 65535) +
                 wb4) & 65535) + wb5) & 65535) + wb6) & 65535;
    Int16 sei = (((sezi + wa2) & 65535) + wa1) & 65535;

    *sez = sezi >> 1;
    *se = sei >> 1;
}


Int16
g727_addb(Int16 dq, Int16 se)
{
    Int8 dqs = dq >> 14;
    Int16 dqi;
    Int8 ses;
    Int16 sei;

    if (dqs == 0)
        dqi = dq;
    else if (dqs == 1)
        dqi = (65536 - (dq & 16383)) & 65535;
    else
        assert(0);

    ses = se >> 14;

    if (ses == 0)
        sei = se;
    else if (ses == 1)
        sei = (1U << 15) + se;
    else
        assert(0);

    return (dqi + sei) & 65535;
}


void
g727_addc(Int8 *pk0, Int8 *sigpk, Int16 dq, Int16 sez)
{
    Int8 dqs = dq >> 14;
    Int16 dqi;
    Int8 sezs;
    Int16 sezi;
    Int16 dqsez;

    if (dqs == 0)
        dqi = dq;
    else if (dqs == 1)
        dqi = (65536 - (dq & 16383)) & 65535;
    else
        assert(0);

    sezs = sez >> 14;

    if (sezs == 0)
        sezi = sez;
    else if (sezs == 1)
        sezi = (1U << 15) + sez;
    else
        assert(0);

    dqsez = (dqi + sezi) & 65535;
    *pk0 = dqsez >> 15;
    *sigpk = (dqsez == 0) ? 1 : 0;
}


Int8
g727_makexp(Int16 mag)
{
    if (16384 <= mag)
        return 15;
    if (8192 <= mag)
        return 14;
    if (4096 <= mag)
        return 13;
    if (2048 <= mag)
        return 12;
    if (1024 <= mag)
        return 11;
    if (512 <= mag)
        return 10;
    if (256 <= mag)
        return 9;
    if (128 <= mag)
        return 8;
    if (64 <= mag)
        return 7;
    if (32 <= mag)
        return 6;
    if (16 <= mag)
        return 5;
    if (8 <= mag)
        return 4;
    if (4 <= mag)
        return 3;
    if (2 <= mag)
        return 2;

    return mag;
}


Int16
g727_floata(Int16 dq)
{
    Int8 dqs = dq >> 14;
    Int16 mag = dq & 16383;
    Int8 exp = g727_makexp(mag);
    Int8 mant = (mag == 0) ? (1 << 5) : (((Int32)mag << 6) >> exp);

    return (dqs << 10) + (exp << 6) + mant;
}


Int16
g727_floatb(Int16 sr)
{
    Int8 srs = sr >> 15;
    Int16 mag;
    Int8 exp;
    Int8 mant;

    if (srs == 0)
        mag = sr;
    else if (srs == 1)
        mag = (65536 - sr) & 32767;
    else
        assert(0);

    exp = g727_makexp(mag);
    mant = (mag == 0) ? (1 << 5) : (((Int32)mag << 6) >> exp);

    return (srs << 10) + (exp << 6) + mant;
}


Int16
g727_fmult(Int16 an, Int16 srn)
{
    Int8 ans = an >> 15;
    Int16 anmag;
    Int8 anexp;
    Int8 anmant;
    Int8 srns;
    Int8 srnexp;
    Int8 srnmant;
    Int8 wans;
    Int8 wanexp;
    Int8 wanmant;
    Int16 wanmag;
    Int16 wan;

    if (ans == 0)
        anmag = an >> 2;
    else if (ans == 1)
        anmag = (16384 - (an >> 2)) & 8191;
    else
        assert(0);

    anexp = g727_makexp(anmag);
    anmant = (anmag == 0) ? (1 << 5) : (((Int32)anmag << 6) >> anexp);

    srns = srn >> 10;
    srnexp = (srn >> 6) & 15;
    srnmant = srn & 63;

    wans = srns ^ ans;
    wanexp = srnexp + anexp;
    wanmant = ((srnmant*anmant) + 48) >> 4;
    wanmag = (wanexp <= 26 ) ?
             (((Int16)wanmant << 7) >> (26 - wanexp)) :
             ((((Int16)wanmant << 7) << (wanexp - 26)) & 32767);

    if (wans == 0)
        wan = wanmag;
    else if (wans == 1)
        wan = (65536 - wanmag) & 65535;
    else
        assert(0);

    return wan;
}


Int16
g727_limc(Int16 a2t)
{
    Int16 a2ul = 12288;
    Int16 a2ll = 53248;

    return (32768 <= a2t && a2t <= a2ll) ? a2ll :
           ((a2ul <= a2t && a2t <= 32767) ? a2ul : a2t);
}


Int16
g727_limd(Int16 a1t, Int16 a2p)
{
    Int16 ome = 15360;
    Int16 a1ul = (ome + 65536UL - a2p) & 65535;
    Int16 a1ll = (a2p + 65536UL - ome) & 65535;

    return (32768 <= a1t && a1t <= a1ll) ? a1ll :
           ((a1ul <= a1t && a1t <= 32767) ? a1ul : a1t);
}


Int16
g727_aprsc_trigb(Int8 tr, Int16 anp)
{
    Int16 anr;

    if (tr == 0)
        anr = anp;
    else if (tr == 1)
        anr = 0;
    else
        assert(0);

    return anr;
}


Int16
g727_upa1(g727_aprsc_state *st, Int8 pk0, Int8 sigpk)
{
    Int8 pks = pk0 ^ st->pk1;
    Int16 uga1;
    Int8 a1s;
    Int16 ula1;
    Int16 ua1;

    if (pks == 0 && sigpk == 0)
        uga1 = 192;
    else if (pks == 1 && sigpk == 0)
        uga1 = 65344;
    else if (sigpk == 1)
        uga1 = 0;
    else
        assert(0);

    a1s = st->a1 >> 15;

    if (a1s == 0)
        ula1 = (65536 - (st->a1 >> 8)) & 65535;
    else if (a1s == 1)
        ula1 = (65536 - ((st->a1 >> 8) + 65280UL)) & 65535;
    else
        assert(0);

    ua1 = (uga1 + ula1) & 65535;

    return (st->a1 + ua1) & 65535;
}


Int16
g727_upa2(g727_aprsc_state *st, Int8 pk0, Int8 sigpk)
{
    Int8 pks1 = pk0 ^ st->pk1;
    Int8 pks2 = pk0 ^ st->pk2;
    Int32 uga2a;
    Int8 a1s;
    Int32 fa1;
    Int32 fa;
    Int32 uga2b;
    Int8 uga2s;
    Int16 uga2;
    Int8 a2s;
    Int16 ula2;
    Int16 ua2;

    if (pks2 == 0)
        uga2a = 16384;
    else if (pks2 == 1)
        uga2a = 114688;
    else
        assert(0);

    a1s = st->a1 >> 15;

    if (a1s == 0)
        fa1 = (st->a1 <= 8191) ? ((Int32)st->a1 << 2) : (8191U << 2);
    else if (a1s == 1)
        fa1 = (st->a1 >= 57345) ? (((Int32)st->a1 << 2) & 131071) :
              (24577UL << 2);
    else
        assert(0);

    if (pks1 == 1)
        fa = fa1;
    else if (pks1 == 0)
        fa = (131072 - fa1) & 131071;
    else
        assert(0);

    uga2b = (uga2a + fa) & 131071;
    uga2s = uga2b >> 16;

    if (uga2s == 0 && sigpk == 0)
        uga2 = uga2b >> 7;
    else if (uga2s == 1 && sigpk == 0)
        uga2 = (uga2b >> 7) + 64512;
    else if (sigpk == 1)
        uga2 = 0;
    else
        assert(0);

    a2s = st->a2 >> 15;

    if (a2s == 0)
        ula2 = (65536 - (st->a2 >> 7)) & 65535;
    else if (a2s == 1)
        ula2 = (65536 - ((st->a2 >> 7) + 65024)) & 65535;
    else
        assert(0);

    ua2 = (uga2 + ula2) & 65535;

    return (st->a2 + ua2) & 65535;
}


Int16
g727_upb(Int8 un, Int16 bn, Int16 dq)
{
    Int16 dqmag = dq & 16383;
    Int16 ugbn;
    Int8 bns;
    Int16 ulbn;
    Int16 ubn;

    if (un == 0 && dqmag != 0)
        ugbn = 128;
    else if (un == 1 && dqmag != 0)
        ugbn = 65408;
    else if (dqmag == 0)
        ugbn = 0;
    else
        assert(0);

    bns = bn >> 15;

    if (bns == 0)
        ulbn = (65536 - (bn >> 8)) & 65535;
    else if (bns == 1)
        ulbn = (65536 - ((bn >> 8) + 65280)) & 65535;
    else
        assert(0);

    ubn = (ugbn + ulbn) & 65535;

    return (bn + ubn) & 65535;
}


Int8
g727_xor(Int16 dqn, Int16 dq)
{
    Int8 dqs = dq >> 14;
    Int8 dqns = dqn >> 10;

    return dqs ^ dqns;
}


/*---------- Reconstructed signal calculator ----------*/
/*---------- Feed-Forward Path (G.727 6.2.8) ----------*/

Int16
g727_get_srff(Int16 dqff, Int16 se)
{
    return g727_addb(dqff, se);
}


/*---------- Tone and Transition Detector (G.727 6.2.9) ----------*/

void
g727_ttd_reset(g727_ttd_state *st)
{
    st->td = 0;
}


Int8
g727_get_tr(g727_ttd_state *st, Int32 yl, Int16 dq)
{
    return g727_trans(st, yl, dq);
}


Int8
g727_get_tdp(Int16 a2p)
{
    return g727_tone(a2p);
}


void
g727_ttd_transit(g727_ttd_state *st, Int8 tr, Int8 tdp)
{
    st->td = g727_ttd_trigb(tr, tdp);
}


Int8
g727_tone(Int16 a2p)
{
    return (32768 <= a2p && a2p < 53760) ? 1 : 0;
}


Int8
g727_trans(g727_ttd_state *st, Int32 yl, Int16 dq)
{
    Int16 dqmag = dq & 16383;
    Int8 ylint = yl >> 15;
    Int8 ylfrac = (yl >> 10) & 31;

    Int32 thr1 = (32 + ylfrac) << ylint;
    Int32 thr2 = (ylint > 8) ? (31 << 9) : thr1;

    Int32 dqthr = (thr2 + (thr2 >> 1)) >> 1;

    return (dqmag > dqthr && st->td == 1) ? 1 : 0;
}


Int8
g727_ttd_trigb(Int8 tr, Int8 tdp)
{
    Int8 tdr;

    if (tr == 0)
        tdr = tdp;
    else if (tr == 1)
        tdr = 0;
    else
        assert(0);

    return tdr;
}


/*---------- Output PCM Format Conversion and Synchronous -----------*/
/*---------- Coding Adjustment (G.727 6.2.9) ------------------------*/

Int8
g727_get_sd(Int16 sr, Int16 se, Int8 in, Int16 y, short law, short rate)
{
    Int8 sp = g727_compress(sr, law);

    Int16 dl;
    Int8 ds;
    g727_log(&dl, &ds, g727_subta(g727_expand(sp, law), se));

    return g727_sync(in, sp, g727_subtb(dl, y), law, ds, rate);
}


Int8
g727_compress(Int16 sr, short law)
{
    Int8 is = sr >> 15;
    Int16 im;
    Int16 imag;
    Int16 sp;

    if (is == 0)
        im = sr;
    else if (is == 1)
        im = (65536 - sr) & 32767;
    else
        assert(0);

    if (law == 0)
        imag = im;
    else if (is == 0)
        imag = im >> 1;
    else if (is == 1)
        imag = (im + 1) >> 1;
    else
        assert(0);

    if (law == 1) {         /* A law */
        if (imag == 0)
            sp = 0;
        else {
            if (is == 1)
                imag -= 1;

            if (4095 < imag)
                sp = 0x7f;      /* Absolute maximum of PCM signal. */
            else if (2047 < imag)
                sp = 0x70 | ((imag >> 7) & 0xf);
            else if (1023 < imag)
                sp = 0x60 | ((imag >> 6) & 0xf);
            else if (511 < imag)
                sp = 0x50 | ((imag >> 5) & 0xf);
            else if (255 < imag)
                sp = 0x40 | ((imag >> 4) & 0xf);
            else if (127 < imag)
                sp = 0x30 | ((imag >> 3) & 0xf);
            else if (63 < imag)
                sp = 0x20 | ((imag >> 2) & 0xf);
            else if (31 < imag)
                sp = 0x10 | ((imag >> 1) & 0xf);
            else
                sp = (imag >> 1) & 0xf;
        }
    }
    else {                  /* U law */
        imag += 33;

        if (8191 < imag)
            sp = 0x7f;      /* Absolute maximum of PCM signal. */
        else if (4095 < imag)
            sp = 0x70 | ((imag >> 8) & 0xf);
        else if (2047 < imag)
            sp = 0x60 | ((imag >> 7) & 0xf);
        else if (1023 < imag)
            sp = 0x50 | ((imag >> 6) & 0xf);
        else if (511 < imag)
            sp = 0x40 | ((imag >> 5) & 0xf);
        else if (255 < imag)
            sp = 0x30 | ((imag >> 4) & 0xf);
        else if (127 < imag)
            sp = 0x20 | ((imag >> 3) & 0xf);
        else if (63 < imag)
            sp = 0x10 | ((imag >> 2) & 0xf);
        else
            sp = (imag >> 1) & 0xf;
    }

    sp |= is << 7;

    return sp;
}


Int8
g727_sync(Int8 in, Int8 sp, Int16 dlnx, short law, Int8 ds, short rate)
{
    Int8 is;
    Int8 im;
    Int8 id;
    Int8 sd;

    switch (rate) {
        case 5:
            is = in >> 4;

            if (is == 0)
                im = in + 16;
            else if (is == 1)
                im = in & 15;
            else
                assert(0);

            if (4096 <= dlnx)
                assert(0);
            else if (4089 <= dlnx)
                id = 18;
            else if (3961 <= dlnx)
                id = 17;
            else if (2048 <= dlnx)
                id = 16;
            else if (439 <= dlnx)
                id = 31;
            else if (405 <= dlnx)
                id = 30;
            else if (380 <= dlnx)
                id = 29;
            else if (356 <= dlnx)
                id = 28;
            else if (333 <= dlnx)
                id = 27;
            else if (310 <= dlnx)
                id = 26;
            else if (286 <= dlnx)
                id = 25;
            else if (261 <= dlnx)
                id = 24;
            else if (233 <= dlnx)
                id = 23;
            else if (202 <= dlnx)
                id = 22;
            else if (166 <= dlnx)
                id = 21;
            else if (123 <= dlnx)
                id = 20;
            else if (69 <= dlnx)
                id = 19;
            else
                id = 18;

            id = (ds == 1) ? ((~id) & 0x1f) : id;
            break;

        case 4:
            is = in >> 3;

            if (is == 0)
                im = in + 8;
            else if (is == 1)
                im = in & 7;
            else
                assert(0);

            if (4096 <= dlnx)
                assert(0);
            else if (4089 <= dlnx)
                id = 9;
            else if (2048 <= dlnx)
                id = 8;
            else if (405 <= dlnx)
                id = 15;
            else if (356 <= dlnx)
                id = 14;
            else if (310 <= dlnx)
                id = 13;
            else if (261 <= dlnx)
                id = 12;
            else if (202 <= dlnx)
                id = 11;
            else if (123 <= dlnx)
                id = 10;
            else
                id = 9;

            id = (ds == 1) ? ((~id) & 0xf) : id;
            break;

        case 3:
            is = in >> 2;

            if (is == 0)
                im = in + 4;
            else if (is == 1)
                im = in & 3;
            else
                assert(0);

            if (4096 <= dlnx)
                assert(0);
            else if (2048 <= dlnx)
                id = 4;
            else if (356 <= dlnx)
                id = 7;
            else if (261 <= dlnx)
                id = 6;
            else if (123 <= dlnx)
                id = 5;
            else
                id = 4;

            id = (ds == 1) ? ((~id) & 0x7) : id;
            break;

        case 2:
            is = in >> 1;

            if (is == 0)
                im = in + 2;
            else if (is == 1)
                im = in & 1;
            else
                assert(0);

            if (4096 <= dlnx)
                assert(0);
            else if (2048 <= dlnx)
                id = 2;
            else if (261 <= dlnx)
                id = 3;
            else
                id = 2;

            id = (ds == 1) ? ((~id) & 0x3) : id;
            break;

        default:
            assert(0);
    }

    if (id < im) {
        if (0xff < sp)
            assert(0);
        else if (0x80 < sp)
            sd = sp - 1;
        else if (sp == 0x80)
            sd = (law == 0) ? 1 : 0;
        else if (sp == 0x7f)
            sd = 0x7f;
        else
            sd = sp + 1;
    }
    else if (id > im) {
        if (0xff < sp)
            assert(0);
        else if (sp == 0xff)
            sd = sp;
        else if (0x7f < sp)
            sd = sp + 1;
        else if (0 < sp)
            sd = sp - 1;
        else
            sd = (law == 0) ? 0x81 : 0x80;
    }
    else
        sd = sp;

    return (law == 1) ? (0xd5 ^ sd) : (0xff ^ sd);
}

