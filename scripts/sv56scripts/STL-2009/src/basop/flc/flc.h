/*                                  v1.0 (part of basop V.2.3) - Sep/2009 */
/*===================================================================*
*  ITU-T Floating point operation counter STL 2009 tool
*  Software Release 1.0 (2009-09)
*
*      COPYRIGHT NOTE: This source code, and all of its derivations,
*      is subject to the "ITU-T General Public License". Please have
*      it  read  in    the  distribution  disk,   or  in  the  ITU-T
*      Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO 
*      CODING STANDARDS".
*
* The tool was provided by VoiceAge Corporation. VoiceAge Corp. makes no
* representation nor warranty in regard to the accuracy, completeness or
* sufficiency of The Software, nor shall VoiceAge Corp. be held liable
* for any damages whatsoever relating to use of said Software.
*===================================================================*/

#ifndef FLC_H
#define FLC_H

#include <stdio.h>


/* NOTE: CHANGE THIS VALUE ACCORDING TO THE FRAME LENGTH OF THE CODEC */
/* Frame lenght is defined in milliseconds */
#define FLC_FRAMELEN 20.0F

/* define DONT_COUNT switch only if you want to SUPPRESS the tool in instrumented code */
/* #define DONT_COUNT */

/* Scaling factor to estimate corresponding complexity in fixed point implementation */
#define FLC_SCALEFAC 1.1F

/* opcodes - this list must match the names in flc.c */
enum flc_fields
{
   FLC_NOP = 0,
   FLC_ADD,
   FLC_MULT,
   FLC_MAC,
   FLC_MOVE,
   FLC_STORE,
   FLC_LOGIC,
   FLC_SHIFT,
   FLC_BRANCH,
   FLC_DIV,
   FLC_SQRT,
   FLC_TRANS,                   /* transcendal fn (sin. cos) */
   FLC_FUNC,
   FLC_LOOP,
   FLC_INDIRECT,                /* indirect ref */
   FLC_PTR_INIT,
   FLC_MISC,                    /* For all other ops (eg. ABS) - weight 1 mem, 1 op */
   FLC_TEST,                    /* For extra conditionnal test (used in conjunction with BRANCH) */
   FLC_POWER,                   /* pow, 1.0/x, e^n */
   FLC_LOG,                     /* log2, log10, Ln */
   FLC_OPEND
};

#ifndef DONT_COUNT

/* the most important fn */
void FLC_ops(int op, int count);
void FLC_mem(int op, int count);

#define _FLC(o,c)  {static int f=0; FLC_ops((o),(c)); if (!f) {f=1;FLC_mem((o),(c));}}
#define ADD(c)          _FLC( FLC_ADD,      (c) )
#define MULT(c)         _FLC( FLC_MULT,     (c) )
#define MAC(c)          _FLC( FLC_MAC,      (c) )
#define MOVE(c)         _FLC( FLC_MOVE,     (c) )
#define STORE(c)        _FLC( FLC_STORE,    (c) )
#define LOGIC(c)        _FLC( FLC_LOGIC,    (c) )
#define SHIFT(c)        _FLC( FLC_SHIFT,    (c) )
#define BRANCH(c)       _FLC( FLC_BRANCH,   (c) )
#define DIV(c)          _FLC( FLC_DIV,      (c) )
#define SQRT(c)         _FLC( FLC_SQRT,     (c) )
#define TRANS(c)        _FLC( FLC_TRANS,    (c) )
#define POWER(c)        _FLC( FLC_POWER,    (c) )
#define LOG(c)          _FLC( FLC_LOG,      (c) )
#define FUNC(c)         _FLC( FLC_FUNC,     (c) )
#define LOOP(c)         _FLC( FLC_LOOP,     (c) )
#define INDIRECT(c)     _FLC( FLC_INDIRECT, (c) )
#define PTR_INIT(c)     _FLC( FLC_PTR_INIT, (c) )
#define MISC(c)         _FLC( FLC_MISC,     (c) )
#define TEST(c)         _FLC( FLC_TEST,     (c) )

/* Double Ops count as double the operations but same memory */
#define _FLCD(o,c)  {static int f=0; FLC_ops((o),2*(c)); if (!f) {f=1;FLC_mem((o),(c));}}
#define DADD(c)         _FLCD( FLC_ADD,     (c) )
#define DMULT(c)        _FLCD( FLC_MULT,    (c) )
#define DMOVE(c)        _FLCD( FLC_MOVE,    (c) )
#define DDIV(c)         _FLCD( FLC_DIV,     (c) )

#else

#define ADD(c)      
#define MULT(c)     
#define MAC(c)      
#define MOVE(c)     
#define STORE(c)    
#define LOGIC(c)    
#define SHIFT(c)    
#define BRANCH(c)   
#define DIV(c)      
#define SQRT(c)       
#define TRANS(c)    
#define POWER(c)    
#define LOG(c)      
#define FUNC(c)     
#define LOOP(c)     
#define INDIRECT(c) 
#define PTR_INIT(c) 
#define MISC(c)     
#define TEST(c)     

#define DADD(c) 
#define DMULT(c) 
#define DMOVE(c)     
#define DDIV(c)     

#endif

/* external function prototypes */
void FLC_init();
void FLC_end();
void FLC_sub_start(char *name);
void FLC_sub_end();
void FLC_frame_update();

#endif
