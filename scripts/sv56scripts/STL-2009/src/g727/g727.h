/********************************************************************
 *
 * File :        g727.h
 * Description : G.727 embedded ADPCM algorithm operating with
 *               5-, 4-, 3-, and 2-bits per sample.
 *ORIGINAL BY:

     Nikolai L. Birioukov
     Maxim A. Lagerev

     Ukrainian Research Institut of Telecommunications

     P.O. 252650
     13, Solomenskaya St.
     Kiev, Ukraine

     Phone : +380 44 276 51 01
     FAX   : +380 44 226 26 46
     EMail : INFO@UNIIS,KIEV,UA

 *  HISTORY:
    01.Apr.95  0.99 Version of the G727 module in C++ code
    10.Jul.96  1.00 This version of the G727 module in C code
    04.Aug.97  1.01 Updated smart prototype list as per 
                    <Morgan.Lindqvist@era-t.ericsson.se> comments for the
		    cc compiler in a DEC Alpha Unix machine.
    02.Feb.2010 1.11  Modified maximum string length, and implicit
                      casting of toupper() argument removed. (y.hiwasaki)
 *
 *******************************************************************/

#ifndef G727_H
#define G727_H 111

/* Smart function prototypes: for [ag]cc, VaxC, and [tb]cc */
#if !defined(ARGS)
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(s) s
#else
#define ARGS(s) ()
#endif
#endif


/* Data types for the G.727 module */
typedef unsigned long  Int32;
typedef unsigned short Int16;
typedef unsigned char  Int8;


/* INTERNAL State variable for G727 encoder and decoder */
typedef struct {
    Int8 td;
} g727_ttd_state;

typedef struct {
    Int32 yl;
    Int16 yu;
} g727_qsfa_state;

typedef struct {
    Int16 dms;
    Int16 dml;
    Int16 ap;
} g727_asc_state;

typedef struct {
    Int8 pk1, pk2;
    Int16 sr1, sr2;
    Int16 dq1, dq2, dq3, dq4, dq5, dq6;
    Int16 b1, b2, b3, b4, b5, b6;
    Int16 a1, a2;
} g727_aprsc_state;

/* PUBLIC State variable for G727 encoder and decoder */
typedef struct {
    g727_ttd_state ttd;
    g727_qsfa_state qsfa;
    g727_asc_state asc;
    g727_aprsc_state aprsc;
} g727_state;


#ifdef ORIGINAL_PROTOS
void g727_reset(g727_state *st);
short g727_encode(short code, short law, short cbits, short ebits,
                                                      g727_state *st);
short g727_decode(short code, short law, short cbits, short ebits,
                                                      g727_state *st);
void g727_encode_block(short *src, short *dst, short n, short law,
                                                        short cbits,
                                                        short ebits,
                                                        g727_state *st);
void g727_decode_block(short *src, short *dst, short n, short law,
                                                        short cbits,
                                                        short ebits,
                                                        g727_state *st);

Int16 g727_get_d(Int8 s, Int16 se, short law);
Int16 g727_expand(Int8 sp, short law);
Int16 g727_subta(Int16 sl, Int16 se);

Int8 g727_get_in(Int16 d, Int16 y, short rate);
void g727_log(Int16 *dl, Int8 *ds, Int16 d);
Int8 g727_quan(Int16 dln, Int8 ds, short rate);
Int16 g727_subtb(Int16 dl, Int16 y);

Int8 g727_get_ic(Int8 in, short enh);

Int16 g727_get_dq(Int16 y, Int8 in, short rate);
Int16 g727_antilog(Int16 dql, Int8 dqs);
Int16 g727_adda(Int16 dqln, Int16 y);
void g727_reconst(Int16 *dqln, Int8 *dqs, Int8 in, short rate);

void g727_qsfa_reset(g727_qsfa_state *st);
Int16 g727_get_y(g727_qsfa_state *st, Int8 al);
Int32 g727_get_yl(g727_qsfa_state *st);
void g727_qsfa_transit(g727_qsfa_state *st, Int8 in, Int16 y, short rate);
Int16 g727_filtd(Int16 wi, Int16 y);
Int32 g727_filte(g727_qsfa_state *st, Int16 yup);
Int16 g727_functw(Int8 in, short rate);
Int16 g727_limb(Int16 yut);
Int16 g727_mix(g727_qsfa_state *st, Int8 al);

void g727_asc_reset(g727_asc_state *st);
Int8 g727_get_al(g727_asc_state *st);
void g727_asc_transit(g727_asc_state *st, Int8 in, Int16 y, Int8 tr,
                                                            Int8 tdp,
                                                            short rate);
Int16 g727_filta(g727_asc_state *st, Int8 fi);
Int16 g727_filtb(g727_asc_state *st, Int8 fi);
Int16 g727_filtc(g727_asc_state *st, Int8 ax);
Int8 g727_functf(Int8 in, short rate);
Int8 g727_lima(g727_asc_state *st);
Int8 g727_subtc(Int16 dmsp, Int16 dmlp, Int8 tdp, Int16 y);
Int16 g727_triga(Int8 tr, Int16 app);

void g727_aprsc_reset(g727_aprsc_state *st);
Int16 g727_get_sr(Int16 dq, Int16 se);
void g727_get_se_sez(g727_aprsc_state *st, Int16 *se, Int16 *sez);
Int16 g727_get_a2p(g727_aprsc_state *st, Int16 dq, Int16 sez, Int8 *pk0,
                                                              Int8 *sigpk);
void g727_aprsc_transit(g727_aprsc_state *st, Int16 dq, Int8 tr, Int16 sr,
                                                                 Int16 a2p,
                                                                 Int8 pk0,
                                                                 Int8 sigpk);
void g727_accum(Int16 *se, Int16 *sez, Int16 wa1, Int16 wa2, Int16 wb1,
                                                             Int16 wb2,
                                                             Int16 wb3,
                                                             Int16 wb4,
                                                             Int16 wb5,
                                                             Int16 wb6);
Int16 g727_addb(Int16 dq, Int16 se);
void g727_addc(Int8 *pk0, Int8 *sigpk, Int16 dq, Int16 sez);
Int8 g727_makexp(Int16 mag);
Int16 g727_floata(Int16 dq);
Int16 g727_floatb(Int16 sr);
Int16 g727_fmult(Int16 an, Int16 srn);
Int16 g727_limc(Int16 a2t);
Int16 g727_limd(Int16 a1t, Int16 a2p);
Int16 g727_aprsc_trigb(Int8 tr, Int16 anp);
Int16 g727_upa1(g727_aprsc_state *st, Int8 pk0, Int8 sigpk);
Int16 g727_upa2(g727_aprsc_state *st, Int8 pk0, Int8 sigpk);
Int16 g727_upb(Int8 un, Int16 bn, Int16 dq);
Int8 g727_xor(Int16 dqn, Int16 dq);

Int16 g727_get_srff(Int16 dqff, Int16 se);

void g727_ttd_reset(g727_ttd_state *st);
Int8 g727_get_tr(g727_ttd_state *st, Int32 yl, Int16 dq);
Int8 g727_get_tdp(Int16 a2p);
void g727_ttd_transit(g727_ttd_state *st, Int8 tr, Int8 tdp);
Int8 g727_tone(Int16 a2p);
Int8 g727_trans(g727_ttd_state *st, Int32 yl, Int16 dq);
Int8 g727_ttd_trigb(Int8 tr, Int8 tdp);

Int8 g727_get_sd(Int16 sr, Int16 se, Int8 in, Int16 y, short law, short rate);
Int8 g727_compress(Int16 sr, short law);
Int8 g727_sync(Int8 in, Int8 sp, Int16 dlnx, short law, Int8 ds, short rate);
#else

/* Aliases to original function names ("soft"-functions)*/
#define g727_encode g727_encode_block
#define g727_decode g727_decode_block

/* UGST-style SMART prototypes ("hard"-functions) */
void g727_reset ARGS((g727_state *st));
void g727_encode_block ARGS((short *src, short *dst, short n, short law,
			     short cbits, short ebits, g727_state *st));
void g727_decode_block ARGS((short *src, short *dst, short n, short law,
			     short cbits, short ebits, g727_state *st));
short g727_encode_sample ARGS((short code, short law, short cbits,
			       short ebits, g727_state *st));
short g727_decode_sample ARGS((short code, short law, short cbits,
			       short ebits, g727_state *st));
#endif /* Smart prototypes */

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

#endif /* G727_H */




