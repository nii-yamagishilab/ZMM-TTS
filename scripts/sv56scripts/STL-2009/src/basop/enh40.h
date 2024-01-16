/*
  ===========================================================================
   File: ENH40.H                                         v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T  STL  BASIC OPERATORS

            40-BIT ARITHMETIC OPERATORS

   History:
   07 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
   March 06   v2.1      Changed to improve portability.                        

  ============================================================================
*/


#ifndef _ENH40_H
#define _ENH40_H


#include "stl.h"


#ifdef WMOPS
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;
#endif /* ifdef WMOPS */


/*****************************************************************************
 *
 *  Constants and Globals
 *
 *****************************************************************************/
#ifdef _MSC_VER
#define MAX_40 (0x0000007fffffffff)
#define MIN_40 (0xffffff8000000000)
#endif /* ifdef _MSC_VER */



/*****************************************************************************
 *
 *  Macros for 40 bit arithmetic overflow management :
 *  Upon 40-bit overflow beyond MAX_40 or underflow beyond MIN_40,
 *  the application will exit.
 *
 *****************************************************************************/
#define L40_OVERFLOW_OCCURED(  L40_var1) (Overflow = 1, exit(1), L40_var1) 
#define L40_UNDERFLOW_OCCURED( L40_var1) (Overflow = 1, exit(2), L40_var1)


 
 /*****************************************************************************
 *
 *  Prototypes for enhanced 40 bit arithmetic operators
 *
 *****************************************************************************/
Word40 L40_shr(   Word40 L40_var1, Word16 var2);
Word40 L40_shr_r( Word40 L40_var1, Word16 var2);
Word40 L40_shl(   Word40 L40_var1, Word16 var2);
Word40 L40_shl_r( Word40 L40_var1, Word16 var2);

static __inline Word40 L40_mult(      Word16 var1,   Word16 var2);

static __inline Word40 L40_mac(      Word40 L40_var1, Word16 var1,   Word16 var2);
static __inline Word16 mac_r40(      Word40 L40_var1, Word16 var1,   Word16 var2);

static __inline Word40 L40_msu(      Word40 L40_var1, Word16 var1,   Word16 var2);
static __inline Word16 msu_r40(      Word40 L40_var1, Word16 var1,   Word16 var2);


void Mpy_32_16_ss( Word32 L_var1, Word16 var2,   Word32 *L_varout_h, UWord16 *varout_l);
void Mpy_32_32_ss( Word32 L_var1, Word32 L_var2, Word32 *L_varout_h, UWord32 *L_varout_l);


Word40 L40_lshl(  Word40 L40_var1, Word16 var2);
Word40 L40_lshr(  Word40 L40_var1, Word16 var2);

static __inline Word40 L40_set(      Word40 L40_var1);
static __inline UWord16 Extract40_H( Word40 L40_var1);
static __inline UWord16 Extract40_L( Word40 L40_var1);
static __inline UWord32 L_Extract40(  Word40 L40_var1);

static __inline Word40 L40_deposit_h( Word16 var1);
static __inline Word40 L40_deposit_l( Word16 var1);
static __inline Word40 L40_deposit32( Word32 L_var1);

static __inline Word40 L40_round(     Word40 L40_var1);
static __inline Word16 round40(       Word40 L40_var1);


Word40 L40_add( Word40 L40_var1, Word40 L40_var2);
Word40 L40_sub( Word40 L40_var1, Word40 L40_var2);
Word40 L40_abs( Word40 L40_var1);
Word40 L40_negate( Word40 L40_var1);
Word40 L40_max( Word40 L40_var1, Word40 L40_var2);
Word40 L40_min( Word40 L40_var1, Word40 L40_var2);
Word32 L_saturate40( Word40 L40_var1);
Word16 norm_L40(     Word40 L40_var1);


/*****************************************************************************
 *
 *  Functions
 *
 *****************************************************************************/

/*****************************************************************************
 *
 *  Function Name : L40_set
 *
 *  Purpose :
 *
 *    Assigns a 40 constant to a Word40 with adequate initialization depending
 *    on underlying architecture constraints (for example to keep consistency
 *    of sign bits). Current implementation only validated on MSVC++6.0.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
/*#ifdef _MSC_VER*/
static __inline Word40 L40_set( Word40 L40_var1) {
   Word40 L40_var_out;
   
   L40_var_out =  L40_var1 & 0x000000ffffffffff;

   if( L40_var1 & 0x8000000000)
      L40_var_out = L40_var_out | 0xffffff0000000000;

   #ifdef WMOPS
   multiCounter[currCounter].L40_set++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}
/*#endif*/ /* ifdef _MSC_VER */



/*****************************************************************************
 *
 *  Function Name : Extract40_H
 *
 *  Purpose :
 *
 *    Returns the bits [31-16] of L40_var1.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short unsigned integer (UWord16) whose value falls in
 *                the range : MIN_U_16 <= var_out <= MAX_U_16.
 *
 *****************************************************************************/
static __inline UWord16 Extract40_H( Word40 L40_var1) {
   UWord16 var_out;

   var_out = ( UWord16)( L40_var1 >> 16);

   #if (WMOPS)
   multiCounter[currCounter].Extract40_H++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : Extract40_L
 *
 *  Purpose :
 *
 *    Returns the bits [15-0] of L40_var1.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short unsigned integer (UWord16) whose value falls in
 *                the range : MIN_U_16 <= var_out <= MAX_U_16.
 *
 *****************************************************************************/
static __inline UWord16 Extract40_L( Word40 L40_var1) {
   UWord16 var_out;

   var_out = ( UWord16)( L40_var1);

   #if (WMOPS)
   multiCounter[currCounter].Extract40_L++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_Extract40
 *
 *  Purpose :
 *
 *    Returns the bits [31-0] of L40_var1.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long unsigned integer (UWord32) whose value falls in
 *                range : MIN_U_32 <= L_var_out <= MAX_U_32.
 *
 *****************************************************************************/
static __inline UWord32 L_Extract40( Word40 L40_var1) {
   UWord32 L_var_out;

   L_var_out = ( UWord32) L40_var1;

   #if (WMOPS)
   multiCounter[currCounter].L_Extract40++;
   #endif /* ifdef WMOPS */

   return(L_var_out);
} 


/*****************************************************************************
 *
 *  Function Name : L40_deposit_h
 *
 *  Purpose :
 *
 *    Deposits var1 in the bits [31-16] in a 40-bit number. The 16 LSBits of
 *    the output are zeroed and the 8 MSBits sign extend var1 sign bit.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var1 <= MAX_16.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
static __inline Word40 L40_deposit_h( Word16 var1) {
   Word40 L40_var_out;

   L40_var_out = (( Word40) var1) << 16;

   if( var1 & 0x8000) {
      L40_var_out = L40_set( L40_var_out | 0xff00000000);

      #if (WMOPS)
      multiCounter[currCounter].L40_set--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_deposit_h++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}   


/*****************************************************************************
 *
 *  Function Name : L40_deposit_l
 *
 *  Purpose :
 *
 *    Deposits var1 in the bits [15-0] in a 40-bit number. The 24 MSBits sign
 *    extend var1 sign bit.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var1 <= MAX_16.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
static __inline Word40 L40_deposit_l( Word16 var1) {
   Word40 L40_var_out;

   L40_var_out = var1;

   if( var1 & 0x8000) {
      L40_var_out = L40_set( L40_var_out | 0xffffff0000);

      #if (WMOPS)
      multiCounter[currCounter].L40_set--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_deposit_l++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}   


/*****************************************************************************
 *
 *  Function Name : L40_deposit32
 *
 *  Purpose :
 *
 *    Deposits L_var1 in the bits [31-0] in a 40-bit number. The 8 MSBits sign
 *    extend L_var1 sign bit.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value falls in the
 *                range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
static __inline Word40 L40_deposit32( Word32 L_var1) {
   Word40 L40_var_out;

   L40_var_out = ( Word40) L_var1;

   if( L_var1 & 0x80000000) {
      L40_var_out = L40_set( L40_var_out | 0xff00000000);

      #if (WMOPS)
      multiCounter[currCounter].L40_set--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_deposit32++;
   #endif /* ifdef WMOPS */
   
   return( L40_var_out);
}








/*****************************************************************************
 *
 *  Function Name : L40_round
 *
 *  Purpose :
 *
 *    Rounds the lower 16 bits of the 40 bit input number. Returns the 40 bit
 *    result with bits 15-0 cleared.
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
static __inline Word40 L40_round( Word40 L40_var1) {
   Word40 L40_var_out;
   Word40 L40_constant = L40_set( 0xffffff0000);

   L40_var_out = L40_add( 0x8000, L40_var1);
   L40_var_out = L40_var_out & L40_constant;

   #if (WMOPS)
   multiCounter[currCounter].L40_set--;
   multiCounter[currCounter].L40_add--;
   multiCounter[currCounter].L40_round++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}   


/*****************************************************************************
 *
 *  Function Name : round40
 *
 *  Purpose :
 *
 *    Rounds the lower 16 bits of the 40 bit input number. Saturates the 40
 *    bit result to 32-bit and returns the resulting higher 16-bit.
 *    round40( L40_var1) = extract_h( L_saturate40( L40_round( L40_var1)))
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
static __inline Word16 round40( Word40 L40_var1) {
   Word16 var_out;

   var_out = extract_h( L_saturate40( L40_round( L40_var1)));

   #if (WMOPS)
   multiCounter[currCounter].L40_round--;
   multiCounter[currCounter].L_saturate40--;
   multiCounter[currCounter].extract_h--;
   multiCounter[currCounter].round40++;
   #endif /* ifdef WMOPS */

   return( var_out);
}   


/*****************************************************************************
 *
 *  Function Name : L40_mult
 *
 *  Purpose :
 *
 *    Multiplies var1 by var2 and shifts the result left by 1. Returns the
 *    full precision result on 40-bit.
 *    L40_mult( var1, var2) = shiftleft(( var1 times var2), 1)
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var1 <= MAX_16.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var2 <= MAX_16.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
static __inline Word40 L40_mult( Word16 var1, Word16 var2) {
   Word32 L_var_out;
   Word40 L40_var_out;

   L_var_out = ( Word32) var1 * ( Word32) var2;
   L40_var_out = ( Word40) L_var_out;

   /* Below line can not overflow, so we can use << instead of L40_shl. */
   L40_var_out = L40_var_out << 1;

   #if (WMOPS)
   multiCounter[currCounter].L40_mult++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}












/*****************************************************************************
 *
 *  Function Name : L40_mac
 *
 *  Purpose :
 *
 *    Multiplies var2 by var3. Shifts left the 40-bit result by 1 and adds
 *    the result to L40_var1. Returns a 40 bit result.
 *    L40_mac( L40_var1, var2, var3)
 *    = L40_add( L40_var1, L40_mult( var2, var3))
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var2 <= MAX_16.
 *
 *    var3        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var3 <= MAX_16.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
static __inline Word40 L40_mac( Word40 L40_var1, Word16 var2, Word16 var3) {
   Word40 L40_var_out;

   L40_var_out = L40_mult( var2, var3);
   L40_var_out = L40_add( L40_var1, L40_var_out);

   #if (WMOPS)
   multiCounter[currCounter].L40_mult--;
   multiCounter[currCounter].L40_add--;
   multiCounter[currCounter].L40_mac++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}   






/*****************************************************************************
 *
 *  Function Name : mac_r40
 *
 *  Purpose :
 *
 *    Multiplies var2 by var3. Shifts left the 40-bit result by 1 and adds
 *    the result to L40_var1. Rounds the lower 16 bits of the 40 bit result.
 *    Saturates the 40 bit result to 32-bit and returns the resulting higher
 *    16-bit.
 *    mac_r40( L40_var1, var2, var3)
 *    = round40( L40_mac( L40_var1, var2, var3))
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *  Complexity weight : 2
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var2 <= MAX_16.
 *
 *    var3        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var3 <= MAX_16.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in
 *                the range : MIN_16 <= var_out <= MAX_16.
 *
 *****************************************************************************/
static __inline Word16 mac_r40( Word40 L40_var1, Word16 var2, Word16 var3) {
   Word40 L40_var_out;
   Word16 var_out;

   L40_var_out = L40_mac( L40_var1, var2, var3);
   var_out = round40( L40_var_out);

   #if (WMOPS)
   multiCounter[currCounter].L40_mac--;
   multiCounter[currCounter].round40--;
   multiCounter[currCounter].mac_r40++;
   #endif /* ifdef WMOPS */

   return( var_out);
}   






/*****************************************************************************
 *
 *  Function Name : L40_msu
 *
 *  Purpose :
 *
 *    Multiplies var2 by var3. Shifts left the 40-bit result by 1 and
 *    subtracts the result from L40_var1. Returns a 40 bit result.
 *    L40_msu( L40_var1, var2, var3)
 *    = L40_sub( L40_var1, L40_mult( var2, var3))
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var2 <= MAX_16.
 *
 *    var3        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var3 <= MAX_16.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : MIN_40 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
static __inline Word40 L40_msu( Word40 L40_var1, Word16 var2, Word16 var3) {
   Word40 L40_var_out;

   L40_var_out = L40_mult( var2, var3);
   L40_var_out = L40_sub( L40_var1, L40_var_out);

   #if (WMOPS)
   multiCounter[currCounter].L40_mult--;
   multiCounter[currCounter].L40_sub--;
   multiCounter[currCounter].L40_msu++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}   






/*****************************************************************************
 *
 *  Function Name : msu_r40
 *
 *  Purpose :
 *
 *    Multiplies var2 by var3. Shifts left the 40-bit result by 1 and
 *    subtracts the result from L40_var1. Rounds the lower 16 bits of the
 *    40 bit result. Saturates the 40 bit result to 32-bit and returns the
 *    resulting higher 16-bit.
 *    msu_r40( L40_var1, var2, var3)
 *    = round40( L40_msu( L40_var1, var2, var3))
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *  Complexity weight : 2
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var2 <= MAX_16.
 *
 *    var3        16 bit short signed integer (Word16) whose value falls in 
 *                the range : MIN_16 <= var3 <= MAX_16.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in
 *                the range : MIN_16 <= var_out <= MAX_16.
 *
 *****************************************************************************/
static __inline Word16 msu_r40( Word40 L40_var1, Word16 var2, Word16 var3) {
   Word40 L40_var_out;
   Word16 var_out;

   L40_var_out = L40_msu( L40_var1, var2, var3);
   var_out = round40( L40_var_out);

   #if (WMOPS)
   multiCounter[currCounter].L40_msu--;
   multiCounter[currCounter].round40--;
   multiCounter[currCounter].msu_r40++;
   #endif /* ifdef WMOPS */

   return( var_out);
}   










































#endif /*_ENH40_H*/


/* end of file */


