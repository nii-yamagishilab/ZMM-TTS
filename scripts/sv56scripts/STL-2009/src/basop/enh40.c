/*
  ===========================================================================
   File: ENH40.C                                         v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T  STL  BASIC OPERATORS

            40-BIT ARITHMETIC OPERATORS

   History:
   07 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.

  ============================================================================
*/


/*****************************************************************************
 *
 *  Enhanced 40 bit operators :
 *
 *    L40_add()
 *    L40_sub()
 *    L40_abs()
 *    L40_negate()
 *    L40_max()
 *    L40_min()
 *    L40_shr()
 *    L40_shr_r()
 *    L40_shl()
 *    L40_shl_r()
 *    norm_L40()
 *    L40_mult()
 *    L40_mac()
 *    L40_msu()
 *    mac_r40()
 *    msu_r40()
 *    Mpy_32_16_ss()
 *    Mpy_32_32_ss()
 *    L40_lshl()
 *    L40_lshr()
 *    L40_round()
 *    L_saturate40()
 *    L40_set() 
 *    Extract40_H()
 *    Extract40_L()
 *    L_Extract40()
 *    L40_deposit_h()
 *    L40_deposit_l()
 *    L40_deposit32()
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *  Include-Files
 *
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "stl.h"

#if (WMOPS)
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;
#endif /* ifdef WMOPS */


/*****************************************************************************
 *
 *  Local Functions
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *  Constants and Globals
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *  Functions
 *
 *****************************************************************************/

/*****************************************************************************
 *
 *  Function Name : L40_shl
 *
 *  Purpose :
 *
 *    Arithmetically shifts left L40_var1 by var2 positions.
 *    - If var2 is negative, L40_var1 is shifted to the LSBits by (-var2)
 *      positions with extension of the sign bit.
 *    - If var2 is positive, L40_var1 is shifted to the MSBits by (var2)
 *      positions.
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
Word40 L40_shl( Word40 L40_var1, Word16 var2) {

   Word40 L40_var_out;
	Word40 L40_constant = L40_set( 0xc000000000);

   if( var2 < 0) {
      var2 = -var2;
      L40_var_out = L40_shr( L40_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L40_shr--;
      #endif /* ifdef WMOPS */
   }

    else {
      L40_var_out = L40_var1;

      for ( ; var2 > 0; var2--) {
         if( L40_var_out > 0x003fffffffff) {
            L40_var_out = L40_OVERFLOW_OCCURED( L40_var_out);
            break;
         }

         else if ( L40_var_out < L40_constant) {
            L40_var_out = L40_UNDERFLOW_OCCURED( L40_var_out);
            break;
         }

         else {
            L40_var_out = L40_var_out << 1;
         }
      }
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_set--;
   multiCounter[currCounter].L40_shl++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_shr
 *
 *  Purpose :
 *
 *    Arithmetically shifts right L40_var1 by var2 positions.
 *    - If var2 is positive, L40_var1 is shifted to the LSBits by (var2)
 *      positions with extension of the sign bit.
 *    - If var2 is negative, L40_var1 is shifted to the MSBits by (-var2)
 *      positions.
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
Word40 L40_shr( Word40 L40_var1, Word16 var2) {
   Word40 L40_var_out;

   if( var2 < 0) {
      var2 = -var2;
      L40_var_out  = L40_shl ( L40_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L40_shl--;
      #endif /* ifdef WMOPS */

   } else {
      L40_var_out = L40_var1 >> var2;
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_shr++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_negate
 *
 *  Purpose :
 *
 *    Negates L40_var1.
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
Word40 L40_negate( Word40 L40_var1) {
   Word40 L40_var_out;

   L40_var_out = L40_add( ~L40_var1, 0x01);

   #if (WMOPS)
   multiCounter[currCounter].L40_add--;
   multiCounter[currCounter].L40_negate++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_add
 *
 *  Purpose :
 *
 *    Adds L40_var1 and L40_var2 and returns the 40-bit result.
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
 *    L40_var2    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var2 <= MAX_40.
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
Word40 L40_add( Word40 L40_var1, Word40 L40_var2) {
   Word40 L40_var_out;

   L40_var_out = L40_var1 + L40_var2;

   if(  ((( L40_var1    & 0x8000000000) >> 39) != 0)
     && ((( L40_var2    & 0x8000000000) >> 39) != 0)
     && ((( L40_var_out & 0x8000000000) >> 39) == 0)) {
      L40_var_out = L40_UNDERFLOW_OCCURED( L40_var_out);

   } else if( (((L40_var1    & 0x8000000000) >> 39) == 0)
           && (((L40_var2    & 0x8000000000) >> 39) == 0)
           && (((L40_var_out & 0x8000000000) >> 39) != 0)) {
      L40_var_out = L40_OVERFLOW_OCCURED( L40_var_out);
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_add++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_sub
 *
 *  Purpose :
 *
 *    Subtracts L40_var2 from L40_var1.
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
 *    L40_var2    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var2 <= MAX_40.
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
Word40 L40_sub( Word40 L40_var1, Word40 L40_var2) {
   Word40 L40_var_out;

   L40_var_out = L40_var1 - L40_var2;

   if( (((L40_var1    & 0x8000000000) >> 39) != 0)
    && (((L40_var2    & 0x8000000000) >> 39) == 0)
    && (((L40_var_out & 0x8000000000) >> 39) == 0)) {
      L40_var_out = L40_UNDERFLOW_OCCURED( L40_var_out);

   } else if( (((L40_var1    & 0x8000000000) >> 39) == 0)
           && (((L40_var2    & 0x8000000000) >> 39) != 0)
           && (((L40_var_out & 0x8000000000) >> 39) != 0)) {
      L40_var_out = L40_OVERFLOW_OCCURED( L40_var_out);
   }
   #if (WMOPS)
    multiCounter[currCounter].L40_sub++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_abs
 *
 *  Purpose :
 *
 *    Returns the absolute value of L40_var1.
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
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L40_var_out 40 bit long signed integer (Word40) whose value falls in
 *                the range : 0x00 0000 0000 <= L40_var_out <= MAX_40.
 *
 *****************************************************************************/
Word40 L40_abs( Word40 L40_var1) {
   Word40 L40_var_out;

   if( L40_var1 < 0) {
      L40_var_out = L40_negate ( L40_var1);

      #if (WMOPS)
      multiCounter[currCounter].L40_negate--;
      #endif /* ifdef WMOPS */

   } else {
      L40_var_out = L40_var1;
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_abs++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_max
 *
 *  Purpose :
 *
 *    Compares L40_var1 and L40_var2 and returns the maximum value.
 *
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    L40_var2    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var2 <= MAX_40.
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
Word40 L40_max( Word40 L40_var1, Word40 L40_var2) {
   Word40 L40_var_out;

   if( L40_var1 < L40_var2)
      L40_var_out = L40_var2;
   else
      L40_var_out = L40_var1;

   #if (WMOPS)
   multiCounter[currCounter].L40_max++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_min
 *
 *  Purpose :
 *
 *    Compares L40_var1 and L40_var2 and returns the minimum value.
 *
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    L40_var2    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var2 <= MAX_40.
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
Word40 L40_min( Word40 L40_var1, Word40 L40_var2) {
   Word40 L40_var_out;

   if( L40_var1 < L40_var2)
      L40_var_out = L40_var1;
   else
      L40_var_out = L40_var2;

   #if (WMOPS)
   multiCounter[currCounter].L40_min++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_saturate40
 *
 *  Purpose :
 *
 *    If L40_var1 is greater than MAX_32, returns MAX_32.
 *    If L40_var1 is lower   than MIN_32, returns MIN_32.
 *    If not,                             returns L_Extract40( L40_var1).
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
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
Word32 L_saturate40( Word40 L40_var1) {
   Word32 L_var_out;

   Word40 UNDER_L40_var2 = ( Word40) ~(((( Word40) 1) << 31) - ( Word40) 1 );
   Word40 OVER_L40_var2  = ( Word40)  (((( Word40) 1) << 31) - ( Word40) 1 );

   if( L40_var1 < UNDER_L40_var2) {
    L40_var1 = UNDER_L40_var2;
    Overflow = 1;
   }

   if( L40_var1 > OVER_L40_var2) {
    L40_var1 = OVER_L40_var2;
    Overflow = 1;
   }

   L_var_out = L_Extract40( L40_var1);

   #if (WMOPS)
    multiCounter[currCounter].L_Extract40--;
    multiCounter[currCounter].L_saturate40++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}


/*****************************************************************************
 *
 *  Function Name :  Mpy_32_16_ss
 *
 *  Purpose :
 *
 *    Multiplies the 2 signed values L_var1 and var2 with saturation control
 *    on 48-bit. The operation is performed in fractional mode :
 *    - L_var1 is supposed to be in 1Q31 format.
 *    - var2   is supposed to be in 1Q15 format.
 *    - The result is produced in 1Q47 format : L_varout_h points to the
 *      32 MSBits while varout_l points to the 16 LSBits.
 *
 *  Complexity weight : 2
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    *L_varout_h 32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_varout_h <= 0x7fff ffff.
 *
 *    *varout_l   16 bit short unsigned integer (UWord16) whose value falls in
 *                the range : 0x0000 0000 <= varout_l <= 0x0000 ffff.
 *
 *  Return Value :
 *
 *    none
 *
 *****************************************************************************/
void Mpy_32_16_ss( Word32 L_var1, Word16 var2, Word32 *L_varout_h, UWord16 *varout_l) {
   Word16 var1_h;
   UWord16 uvar1_l;
   Word40 L40_var1;

   if( (L_var1 == ( Word32) 0x80000000)
    && (var2   == ( Word16) 0x8000)) {
      *L_varout_h = 0x7fffffff;
      *varout_l = ( UWord16) 0xffff;

    } else {
      uvar1_l = extract_l( L_var1);
      var1_h = extract_h( L_var1);

      /* Below line can not overflow, so we can use << instead of L40_shl.  */
      L40_var1 = (( Word40) (( Word32) var2 * ( Word32) uvar1_l)) << 1;

      *varout_l = Extract40_L( L40_var1);

      L40_var1 = L40_shr( L40_var1, 16);
      L40_var1 = L40_mac( L40_var1, var2, var1_h);

      *L_varout_h = L_Extract40( L40_var1);

      #if(WMOPS)
      multiCounter[currCounter].extract_l--;
      multiCounter[currCounter].extract_h--;
      multiCounter[currCounter].Extract40_L--;
      multiCounter[currCounter].L40_shr--;
      multiCounter[currCounter].L40_mac--;
      multiCounter[currCounter].L_Extract40--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
   multiCounter[currCounter].Mpy_32_16_ss++;
   #endif /* ifdef WMOPS */

   return;
}


/*****************************************************************************
 *
 *  Function Name :  Mpy_32_32_ss
 *
 *  Purpose :
 *
 *    Multiplies the 2 signed values L_var1 and L_var2 with saturation control
 *    on 64-bit. The operation is performed in fractional mode :
 *    - L_var1 and L_var2 are supposed to be in 1Q31 format.
 *    - The result is produced in 1Q63 format : L_varout_h points to the
 *      32 MSBits while L_varout_l points to the 32 LSBits.
 *
 *  Complexity weight : 4
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value falls in the
 *                range : 0x8000 0000  <= L_var1 <= 0x7fff ffff.
 *
 *    L_var2      32 bit long signed integer (Word32) whose value falls in the
 *                range : 0x8000 0000  <= L_var2 <= 0x7fff ffff.
 *
 *  Outputs :
 *
 *    *L_varout_h 32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_varout_h <= 0x7fff ffff.
 *
 *    *L_varout_l 32 bit short unsigned integer (UWord32) whose value falls in
 *                the range : 0x0000 0000 <= L_varout_l <= 0xffff ffff.
 *
 *
 *  Return Value :
 *
 *    none
 *
 *****************************************************************************/
void Mpy_32_32_ss( Word32 L_var1, Word32 L_var2, Word32 *L_varout_h, UWord32 *L_varout_l) {
   UWord16 uvar1_l, uvar2_l;
   Word16   var1_h,  var2_h;
   Word40 L40_var1;

   if( (L_var1 == ( Word32)0x80000000)
    && (L_var2 == ( Word32)0x80000000)) {
      *L_varout_h = 0x7fffffff;
      *L_varout_l = ( UWord32)0xffffffff;

   } else {

      uvar1_l = extract_l( L_var1);
      var1_h = extract_h( L_var1);
      uvar2_l = extract_l( L_var2);
      var2_h = extract_h( L_var2);

      /* Below line can not overflow, so we can use << instead of L40_shl.  */
      L40_var1 = (( Word40) (( UWord32) uvar2_l * ( UWord32) uvar1_l)) << 1;

      *L_varout_l = 0x0000ffff & L_Extract40( L40_var1);

      L40_var1 = L40_shr( L40_var1, 16);
      L40_var1 = L40_add( L40_var1, (( Word40) (( Word32) var2_h * ( Word32) uvar1_l)) << 1);
      L40_var1 = L40_add( L40_var1, (( Word40) (( Word32) var1_h * ( Word32) uvar2_l)) << 1);
      *L_varout_l |= (L_Extract40( L40_var1)) << 16;

      L40_var1 = L40_shr( L40_var1, 16);
      L40_var1 = L40_mac( L40_var1, var1_h, var2_h);

      *L_varout_h = L_Extract40( L40_var1);

      #if (WMOPS)
      multiCounter[currCounter].extract_l-=2;
      multiCounter[currCounter].extract_h-=2;
      multiCounter[currCounter].L_Extract40-=3;
      multiCounter[currCounter].L40_shr-=2;
      multiCounter[currCounter].L40_add-=2;
      multiCounter[currCounter].L40_mac--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
    multiCounter[currCounter].Mpy_32_32_ss++;
   #endif /* ifdef WMOPS */

   return;
}


/*****************************************************************************
 *
 *  Function Name : L40_lshl
 *
 *  Purpose :
 *
 *    Logically shifts left L40_var1 by var2 positions.
 *    - If var2 is negative, L40_var1 is shifted to the LSBits by (-var2)
 *      positions with insertion of 0 at the MSBit.
 *    - If var2 is positive, L40_var1 is shifted to the MSBits by (var2)
 *      positions.
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
Word40 L40_lshl( Word40 L40_var1, Word16 var2) {
   Word40 L40_var_out;

   if( var2 <= 0) {
      var2 = -var2;
      L40_var_out = L40_lshr  ( L40_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L40_lshr--;
      #endif /* ifdef WMOPS */

   } else {
      if( var2 >= 40)
         L40_var_out = 0x0000000000;
      else
         L40_var_out = L40_var1 << var2;

      L40_var_out = L40_set( L40_var_out);

      #if (WMOPS)
      multiCounter[currCounter].L40_set--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_lshl++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_lshr
 *
 *  Purpose :
 *
 *    Logically shifts right L40_var1 by var2 positions.
 *    - If var2 is positive, L40_var1 is shifted to the LSBits by (var2)
 *      positions with insertion of 0 at the MSBit.
 *    - If var2 is negative, L40_var1 is shifted to the MSBits by (-var2)
 *      positions.
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
Word40 L40_lshr( Word40 L40_var1, Word16 var2) {
   Word40 L40_var_out;

   if( var2 < 0) {
      var2 = -var2;
      L40_var_out = L40_lshl ( L40_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L40_lshl--;
      #endif /* ifdef WMOPS */
   } else {
      if( var2 >= 40)
         L40_var_out = 0x0000000000;
      else
         L40_var_out = (L40_var1 & 0xffffffffff) >> var2;
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_lshr++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : norm_L40
 *
 *  Purpose :
 *
 *    Produces the number of left shifts needed to normalize in 32 bit format
 *    the 40 bit variable L40_var1. This returned value can be used to scale
 *    L_40_var1 into the following intervals : 
 *    - [(MAX_32+1)/2 .. MAX_32       ] for positive values.
 *    - [ MIN_32      .. (MIN_32/2)+1 ] for negative values.
 *    - [ 0           .. 0            ] for null     values.
 *    In order to normalize the result, the following operation must be done :
 *    normelized_L40_var1 = L40_shl( L40_var1, norm_L40( L40_var1))
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
 *    var_out     16 bit short signed integer (Word16) whose value falls in
 *                the range : -8 <= var_out <= 31.
 *
 *****************************************************************************/
Word16 norm_L40( Word40 L40_var1) {
   Word16 var_out;

   var_out = 0;

   if( L40_var1 != 0) {
      while( (L40_var1 > ( Word32)0x80000000L)
          && (L40_var1 < ( Word32)0x7fffffffL)) {

         L40_var1 = L40_shl( L40_var1, 1);
         var_out++;

         #ifdef WMOPS
         multiCounter[currCounter].L40_shl--;
         #endif /* ifdef WMOPS */
      }

      while( (L40_var1 < ( Word32)0x80000000L)
          || (L40_var1 > ( Word32)0x7fffffffL)) {

         L40_var1 = L40_shl( L40_var1, -1);
         var_out--;

         #ifdef WMOPS
         multiCounter[currCounter].L40_shl--;
         #endif /* ifdef WMOPS */
      }
   }

  #ifdef WMOPS
  multiCounter[currCounter].norm_L40++;
  #endif /* ifdef WMOPS */

  return( var_out);
 }






/*****************************************************************************
 *
 *  Function Name : L40_shr_r
 *
 *  Purpose :
 *
 *    Arithmetically shifts right L40_var1 by var2 positions and rounds the
 *    result. It is equivalent to L40_shr( L40_var1, var2) except that if the
 *    last bit shifted out to the LSBit is 1, then the shifted result is
 *    incremented by 1.
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0xffff 8000 <= var2 <= 0x0000 7fff.
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
Word40 L40_shr_r( Word40 L40_var1, Word16 var2) {
   Word40 L40_var_out;

   if( var2 > 39) {
      L40_var_out = 0;

   } else {
      L40_var_out = L40_shr( L40_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L40_shr--;
      #endif /* ifdef WMOPS */

      if( var2 > 0) {
         if( ( L40_var1 & (( Word40) 1 << (var2 - 1))) != 0) {
            /* below line can not generate overflows on 40-bit */
            L40_var_out++;
         }
      }
   }

   #if (WMOPS)
   multiCounter[currCounter].L40_shr_r++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L40_shl_r
 *
 *  Purpose :
 *
 *    Arithmetically shifts left L40_var1 by var2 positions and rounds the
 *    result. It is equivalent to L40_shl( L40_var1, var2) except if var2 is
 *    negative. In that case, it does the same as
 *    L40_shr_r( L40_var1, (-var2)).
 *    Calls the macro L40_UNDERFLOW_OCCURED() in case of underflow on 40-bit.
 *    Calls the macro L40_OVERFLOW_OCCURED()  in case of overflow  on 40-bit.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    L40_var1    40 bit long signed integer (Word40) whose value falls in the
 *                range : MIN_40 <= L40_var1 <= MAX_40.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0xffff 8000 <= var2 <= 0x0000 7fff.
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
Word40 L40_shl_r( Word40 L40_var1, Word16 var2) {
   Word40 L40_var_out;

   if( var2 >= 0) {
      L40_var_out = L40_shl( L40_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L40_shl--;
      #endif /* ifdef WMOPS */

   } else {
      var2 = -var2;
      L40_var_out = L40_shr_r ( L40_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L40_shr_r--;
      #endif /* ifdef WMOPS */
   }
   #if (WMOPS)
   multiCounter[currCounter].L40_shl_r++;
   #endif /* ifdef WMOPS */

   return( L40_var_out);
}


/* end of file */
