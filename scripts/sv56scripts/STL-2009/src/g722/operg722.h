/*
  **************************************************************************
   TELECOM/CNET ITU-T ANSI C G.722 program to be included in
   ITU-T UGST software package Tool
  **************************************************************************
   Version 2.00 du 07/01/90 ; Compatible C ANSI ; TURBO C V2
   Copyright CNET Lannion A TSS/CMC

   Author:
   J-P PETIT Tel 33 96 05 39 41
   Fax 33 96 05 13 16
   Email petitjp@lannion.cnet.fr

   History:
   ??/??/??  v1.0 Created
   12.Jun.95 v2.0 Adapted to UGST format <simao@ctd.comsat.com> 
  **************************************************************************
*/
#ifndef OPERG722_H
#define OPERG722_H 200

/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__)) || defined (__CYGWIN__) || defined (_MSC_VER)
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/*   Types definitions */
#if defined(__BORLANDC__) || defined (__WATCOMC__) || defined(_MSC_VER) || defined(__TURBOC__)
typedef  long  int   Word32;
typedef  short int   Word16;
#elif defined(__sun) || defined (__GNUC__) || defined (hpux) || defined(sun)
typedef short  Word16;
typedef long  Word32;
#elif defined(__unix__)
typedef short Word16;
typedef int   Word32;
#endif

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

/*   Operator prototypes */
#ifdef OLD_WAY
Word16 add();     /* Short add,           1 */
Word16 sub();     /* Short sub,           1 */
Word16 shl();     /* Short shift left,    1 */
Word16 shr();     /* Short shift right,   1 */
Word16 mult();    /* Short mult,          1 */
Word16 negate();               /* Short negate,        1 */
Word32 L_add(); /* Long add with sat  2 */
Word32 L_shr();   /* L_shr             var2 */
Word32 L_shl();   /* L_shl             var2 */
#endif

Word16 sature ARGS((Word32 L_var1));
Word16 extract_l ARGS((Word32 L_var1));
Word16 add ARGS((Word16 var1, Word16 var2));
Word16 sub ARGS((Word16 var1, Word16 var2));
Word16 shl ARGS((Word16 var1, Word16 var2));
Word16 shr ARGS((Word16 var1, Word16 var2));
Word16 mult ARGS((Word16 var1, Word16 var2));
Word16 negate ARGS((Word16 var1));
Word32 L_add ARGS((Word32 L_var1, Word32 L_var2));
Word32 L_shr ARGS((Word32 L_var1, Word16 var2));
Word32 L_shl ARGS((Word32 L_var1, Word16 var2));

#endif /* OPERG722_H */
/* .................... End of include file oper.h ...................... */
