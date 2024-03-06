/*
  ===========================================================================
   File: CONTROL.H                                       v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T STL BASIC OPERATORS

            CONTROL   FLOW  OPERATORS

   History:
   07 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
   March 06   v2.1      Changed to improve portability.                        

  ============================================================================
*/


#ifndef _CONTROL_H
#define _CONTROL_H

#include "stl.h"

/*****************************************************************************
 *
 *  Constants and Globals
 *
 *****************************************************************************/
#if (WMOPS)
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;

  /* Technical note :
   * The following 3 variables are only used for correct complexity
   * evaluation of the following structure :
   *   IF{
   *     ...
   *   } ELSE IF {
   *     ...
   *   } ELSE IF {
   *     ...
   *   }
   *   ...
   *   } ELSE {
   *     ...
   *   }
   */
extern int  funcId_where_last_call_to_else_occurred;
extern long funcid_total_wmops_at_last_call_to_else;
extern int  call_occurred;
#endif /* ifdef WMOPS */


/*****************************************************************************
 *
 *  Function Macros
 *
 *****************************************************************************/

/*****************************************************************************
 *
 *  Function Name : FOR
 *
 *  Purpose :
 *
 *    The macro FOR should be used instead of the 'for' C statement.
 *    The complexity is independent of the number of loop iterations that are
 *    performed.
 *
 *  Complexity weight : 3 (regardless of number of iterations).
 *
 *****************************************************************************/
#ifndef WMOPS
#define FOR( a) for( a)

#else /* ifndef WMOPS */
#define FOR( a) if( incrFor(), 0); else for( a)

static __inline void incrFor( void) {
   multiCounter[currCounter].For++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : WHILE
 *
 *  Purpose :
 *
 *    The macro WHILE should be used instead of the 'while' C statement.
 *    The complexity is proportional to the number of loop iterations that
 *    are performed.
 *
 *  Complexity weight : 4 x 'number of loop iterations'.
 *
 *****************************************************************************/
#ifndef WMOPS
#define WHILE( a) while( a)

#else /* ifndef WMOPS */
#define WHILE( a) while( incrWhile(), a)

static __inline void incrWhile( void) {
   multiCounter[currCounter].While++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : DO
 *
 *  Purpose :
 *
 *    The macro DO should be used instead of the 'do' C statement.
 *
 *  Complexity weight : 0 (complexity counted by WHILE macro).
 *
 *****************************************************************************/
#ifndef WMOPS
#define DO do

#else /* ifndef WMOPS */
#define DO do

#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : IF
 *
 *  Purpose :
 *
 *    The macro IF should :
 *
 *    - not be used when :
 *      - the 'if' structure does not have any 'else if' nor 'else' statement
 *      - and it conditions only one DSP basic operations.
 *
 *    - be used instead of the 'if' C statement in every other case :
 *      - when there is an 'else' or 'else if' statement,
 *      - or when the 'if' conditions several DSP basic operations,
 *      - or when the 'if' conditions a function call.
 *
 *  Complexity weight : 4
 *
 *****************************************************************************/
#ifndef WMOPS
#define IF( a) if( a)

#else /* ifndef WMOPS */
#define IF( a) if( incrIf(), a)

static __inline void incrIf( void) {
   /* Technical note :
    * If the "IF" operator comes just after an "ELSE", its counter
    * must not be incremented.
    */
   if ( (currCounter != funcId_where_last_call_to_else_occurred)
     || (TotalWeightedOperation() != funcid_total_wmops_at_last_call_to_else)
     || (call_occurred == 1))
     multiCounter[currCounter].If++;

   call_occurred = 0;
   funcId_where_last_call_to_else_occurred = MAXCOUNTERS;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : ELSE
 *
 *  Purpose :
 *
 *    The macro ELSE should be used instead of the 'else' C statement.
 *
 *  Complexity weight : 4
 *
 *****************************************************************************/
#ifndef WMOPS
#define ELSE else

#else /* ifndef WMOPS */
#define ELSE else if( incrElse(), 0) ; else

static __inline void incrElse( void) {
   multiCounter[currCounter].If++;

   /* We keep track of the funcId of the last function
    * which used ELSE {...} structure.
    */
   funcId_where_last_call_to_else_occurred = currCounter;

   /* We keep track of the number of WMOPS of this funcId
    * when the ELSE macro was called.
    */
   funcid_total_wmops_at_last_call_to_else = TotalWeightedOperation();

   /* call_occurred is set to 0, in order to count the next IF (if necessary)
    */
   call_occurred=0;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : SWITCH
 *
 *  Purpose :
 *
 *    The macro SWITCH should be used instead of the 'switch' C statement.
 *
 *  Complexity weight : 8
 *
 *****************************************************************************/
#ifndef WMOPS
#define SWITCH( a) switch( a)

#else /* ifndef WMOPS */
#define SWITCH( a) switch( incrSwitch(), a)

static __inline void incrSwitch( void) {
   multiCounter[currCounter].Switch++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : CONTINUE
 *
 *  Purpose :
 *
 *    The macro CONTINUE should be used instead of the 'continue' C statement.
 *
 *  Complexity weight : 4
 *
 *****************************************************************************/
#ifndef WMOPS
#define CONTINUE continue

#else /* ifndef WMOPS */
#define CONTINUE if( incrContinue(), 0); else continue

static __inline void incrContinue( void) {
   multiCounter[currCounter].Continue++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : BREAK
 *
 *  Purpose :
 *
 *    The macro BREAK should be used instead of the 'break' C statement.
 *
 *  Complexity weight : 4
 *
 *****************************************************************************/
#ifndef WMOPS
#define BREAK break

#else /* ifndef WMOPS */
#define BREAK if( incrBreak(), 0); else break

static __inline void incrBreak( void) {
   multiCounter[currCounter].Break++;
}
#endif /* ifndef WMOPS */


/*****************************************************************************
 *
 *  Function Name : GOTO
 *
 *  Purpose :
 *
 *    The macro GOTO should be used instead of the 'goto' C statement.
 *
 *  Complexity weight : 4
 *
 *****************************************************************************/
#ifndef WMOPS
#define GOTO goto

#else /* ifndef WMOPS */
#define GOTO if( incrGoto(), 0); else goto

static __inline void incrGoto( void) {
   multiCounter[currCounter].Goto++;
}
#endif /* ifndef WMOPS */


#endif /* _CONTROL_H */


/* end of file */
