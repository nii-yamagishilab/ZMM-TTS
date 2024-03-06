/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/*$Header: /home/kbs/jutta/src/gsm/gsm-1.0/inc/RCS/gsm.h,v 1.5 1994/01/25 22:20:48 jutta Exp $*/

#ifndef	GSM_H
#define	GSM_H

#ifndef NeedFunctionPrototypes
#if defined(__cplusplus) || defined(__STDC__) || defined(VMS)
#	define	NeedFunctionPrototypes	1
#endif
#endif

/* The above 5 lines used to be several #if ... it was changed to avoid 
*   silly warnings and sintax errors of some compilers and to automate
*   symbol definition for VMS!
* #ifdef __cplusplus
* #	define	NeedFunctionPrototypes	1
* #endif
* 
* #if __STDC__
* #	define	NeedFunctionPrototypes	1
* #endif
*/

#ifdef _NO_PROTO
#	undef	NeedFunctionPrototypes
#endif

#ifdef GSM_P
#undef	GSM_P	/* somewhere else may have defined this... 	*/
#endif

#ifdef NeedFunctionPrototypes
#	define	GSM_P( protos )	protos
#else
#	define  GSM_P( protos )	( /* protos */ )
#endif

#ifdef NeedFunctionPrototypes
#   include	<stdio.h>		/* for FILE * 	*/
#endif

/*
 *	Interface
 */

typedef struct gsm_state * 	gsm;
typedef short		   	gsm_signal;		/* signed 16 bit */
typedef unsigned char		gsm_byte;
typedef gsm_byte 		gsm_frame[33];		/* 33 * 8 bits	 */

#define	GSM_MAGIC	0xD			  	/* 13 kbit/s RPE-LTP */

#define	GSM_PATCHLEVEL	3
#define	GSM_MINOR	0
#define	GSM_MAJOR	1

#define	GSM_OPT_VERBOSE	1
#define	GSM_OPT_FAST	2


/* 
 *	Prototypes generated automatically by cproto ...
 */

/* Prototypes for functions defined in file add.c */
extern word gsm_add GSM_P((word a, word b));
word gsm_sub GSM_P((word a, word b));
word gsm_mult GSM_P((word a, word b));
word gsm_mult_r GSM_P((word a, word b));
word gsm_abs GSM_P((word a));
longword gsm_L_mult GSM_P((word a, word b));
longword gsm_L_add GSM_P((longword a, longword b));
longword gsm_L_sub GSM_P((longword a, longword b));
word gsm_norm GSM_P((longword a));
longword gsm_L_asl GSM_P((longword a, int n));
word gsm_asl GSM_P((word a, int n));
longword gsm_L_asr GSM_P((longword a, int n));
word gsm_asr GSM_P((word a, int n));
word gsm_div GSM_P((word num, word denum));

/* Prototypes for functions defined in file code.c */
void Gsm_Coder GSM_P((struct gsm_state *S, word *s, word *LARc, word *Nc, word *bc, word *Mc, word *xmaxc, word *xMc));

/* Prototypes for functions defined in file decode.c */
void Gsm_Decoder GSM_P((struct gsm_state *S, word *LARcr, word *Ncr, word *bcr, word *Mcr, word *xmaxcr, word *xMcr, word *s));

/* Prototypes for functions defined in file long_term.c */
void Gsm_Long_Term_Predictor GSM_P((struct gsm_state *S, word *d, word *dp, word *e, word *dpp, word *Nc, word *bc));
void Gsm_Long_Term_Synthesis_Filtering GSM_P((struct gsm_state *S, word Ncr, word bcr, register word *erp, register word *drp));

/* Prototypes for functions defined in file lpc.c */
void Gsm_LPC_Analysis GSM_P((struct gsm_state *S, word *s, word *LARc));

/* Prototypes for functions defined in file preprocess.c */
void Gsm_Preprocess GSM_P((struct gsm_state *S, word *s, word *so));

/* Prototypes for functions defined in file rpe.c */
void Gsm_RPE_Encoding GSM_P((struct gsm_state *S, word *e, word *xmaxc, word *Mc, word *xMc));
void Gsm_RPE_Decoding GSM_P((struct gsm_state *S, word xmaxcr, word Mcr, word *xMcr, word *erp));

/* Prototypes for functions defined in file gsm_destroy.c */
void gsm_destroy GSM_P((gsm S));

/* Prototypes for functions defined in file gsm_decode.c */
int gsm_decode GSM_P((gsm s, gsm_byte *c, gsm_signal *target));

/* Prototypes for functions defined in file gsm_encode.c */
void gsm_encode GSM_P((gsm s, gsm_signal *source, gsm_byte *c));

/* Prototypes for functions defined in file gsm_explode.c */
int gsm_explode GSM_P((gsm s, gsm_byte *c, gsm_signal *target));

/* Prototypes for functions defined in file gsm_implode.c */
void gsm_implode GSM_P((gsm s, gsm_signal *source, gsm_byte *c));

/* Prototypes for functions defined in file gsm_create.c */
gsm gsm_create GSM_P((void));

/* Prototypes for functions defined in file gsm_print.c */
int gsm_print GSM_P((FILE *f, gsm s, gsm_byte *c));

/* Prototypes for functions defined in file gsm_option.c */
int gsm_option GSM_P((gsm r, int opt, int *val));

/* Prototypes for functions defined in file short_term.c */
void Gsm_Short_Term_Analysis_Filter GSM_P((struct gsm_state *S, word *LARc, word *s));
void Gsm_Short_Term_Synthesis_Filter GSM_P((struct gsm_state *S, word *LARcr, word *wt, word *s));

/* These prototypes only when debugging is enabled! */
#ifndef NDEBUG
void gsm_debug_words GSM_P((char *name, int from, int to, word *ptr));
void gsm_debug_longwords GSM_P((char *name, int from, int to, longword *ptr));
void gsm_debug_longword GSM_P((char *name, longword value));
void gsm_debug_word GSM_P((char *name, word value));
#endif

#undef	GSM_P

#endif	/* GSM_H */
