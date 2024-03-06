/*
  ===========================================================================
   File: ENH1632.C                                       v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T        STL        BASIC       OPERATORS

            ENHANCED 16-BIT & 32-BIT ARITHMETIC OPERATORS

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
 *  Enhanced 16/32 bit operators :
 *    s_max()
 *    s_min()
 *    L_max()
 *    L_min()
 *    shl_r()
 *    L_shl_r()
 *    L_mac0()
 *    L_mult0()
 *    L_msu0()
 *    s_and()
 *    s_or()
 *    s_xor()
 *    L_and()
 *    L_or()
 *    lshl()
 *    lshr()
 *    L_lshl()
 *    L_lshr()
 *    rotr()
 *    rotl()
 *    L_rotr()
 *    L_rotl()
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
 *   Constants and Globals
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *   Functions
 *
 *****************************************************************************/


/*****************************************************************************
 *
 *  Function Name : lshl
 *
 *  Purpose :
 *
 *    Logically shifts left var1 by var2 positions.
 *    - If var2 is negative, var1 is shifted to the LSBits by (-var2)
 *      positions with insertion of 0 at the MSBit.
 *    - If var2 is positive, var1 is shifted to the MSBits by (var2)
 *      positions.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value:
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
Word16 lshl( Word16 var1, Word16 var2) {
   Word16 var_out=0;

   if( var2 < 0) {
      var2 = -var2;
      var_out = lshr( var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].lshr--;
      #endif /* ifdef WMOPS */

   } else {
      if( var2 == 0 || var1 == 0) {
         var_out = var1;
      } else if( var2 >= 16) {
         var_out = 0;
      } else {
         var_out = var1 << var2;
      }
   }
   #if (WMOPS)
   multiCounter[currCounter].lshl++;
   #endif /* ifdef WMOPS */

   return( var_out);
}

/*****************************************************************************
 *
 *  Function Name : lshr
 *
 *  Purpose :
 *
 *    Logically shifts right var1 by var2 positions.
 *    - If var2 is positive, var1 is shifted to the LSBits by (var2)
 *      positions with insertion of 0 at the MSBit.
 *    - If var2 is negative, var1 is shifted to the MSBits by (-var2)
 *      positions.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value:
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
Word16 lshr( Word16 var1, Word16 var2) {
   Word16 var_out;

   if( var2 < 0) {
      var2 = -var2;
      var_out = lshl( var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].lshl--;
      #endif /* ifdef WMOPS */

   } else {
      if( var2 == 0 || var1 == 0) {
         var_out = var1;
      } else if( var2 >= 16) {
         var_out = 0;
      } else {
         var_out = var1 >> 1;
         var_out = var_out & 0x7fff;
         var_out =  var_out >> ( var2-1);
      }
   }
   #if (WMOPS)
   multiCounter[currCounter].lshr++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_lshl
 *
 *  Purpose :
 *
 *    Logically shifts left L_var1 by var2 positions.
 *    - If var2 is negative, L_var1 is shifted to the LSBits by (-var2)
 *      positions with insertion of 0 at the MSBit.
 *    - If var2 is positive, L_var1 is shifted to the MSBits by (var2)
 *      positions.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value falls in 
 *                the range 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value:
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in 
 *                the range 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
Word32 L_lshl( Word32 L_var1, Word16 var2) {
   Word32 L_var_out=0;

   if( var2 < 0) {
      var2 = -var2;
      L_var_out = L_lshr( L_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L_lshr--;
      #endif /* ifdef WMOPS */

   } else {
      if( var2 == 0 || L_var1 == 0) {
         L_var_out = L_var1;
      } else if( var2 >= 32) {
         L_var_out = 0;
      } else {
         L_var_out = L_var1 << var2;
      }
   }
   #if (WMOPS)
   multiCounter[currCounter].L_lshl++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_lshr
 *
 *  Purpose :
 *
 *    Logically shifts right L_var1 by var2 positions.
 *    - If var2 is positive, L_var1 is shifted to the LSBits by (var2)
 *      positions with insertion of 0 at the MSBit.
 *    - If var2 is negative, L_var1 is shifted to the MSBits by (-var2)
 *      positions.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value falls in 
 *                the range 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value:
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in 
 *                the range 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
Word32 L_lshr( Word32 L_var1, Word16 var2) {
   Word32   L_var_out;

   if( var2 < 0) {
      var2 = -var2;
      L_var_out = L_lshl( L_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L_lshl--;
      #endif /* ifdef WMOPS */

   } else {
      if( var2 == 0 || L_var1 == 0) {
         L_var_out = L_var1;
      } else if( var2 >= 32) {
         L_var_out = 0;
      } else {
         L_var_out = L_var1 >> 1;
         L_var_out = L_var_out & 0x7fffffff;
         L_var_out =  L_var_out >> (var2 - 1);
      }
   }
   #if (WMOPS)
   multiCounter[currCounter].L_lshr++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}


/*****************************************************************************
 *
 *  Function Name : shl_r
 *
 *  Purpose :
 *
 *    Identical to shl( var1, var2) but with rounding. Saturates the result
 *    in case of underflows or overflows.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0xffff 8000 <= var1 <= 0x0000 7fff.
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
 *    var_out     16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
Word16 shl_r( Word16 var1, Word16 var2){
   Word16 var_out;

   if( var2 >= 0) {
      var_out = shl( var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].shl--;
      #endif /* ifdef WMOPS */

   } else {
      var2 = -var2;
      var_out = shr_r( var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].shr_r--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
   multiCounter[currCounter].shl_r++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_shl_r
 *
 *  Purpose :
 *
 *   Same as L_shl( var1, var2) but with rounding. Saturates the result in
 *   case of underflows or overflows.
 *
 *  Complexity weight : 3
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
 *    none
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in 
 *                the range : 0x8000 0000 <= var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
Word32 L_shl_r( Word32 L_var1, Word16 var2) {
   Word32 var_out;

   if( var2 >= 0) {
      var_out = L_shl( L_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L_shl--;
      #endif /* ifdef WMOPS */

   } else {
      var2 = -var2;
      var_out = L_shr_r( L_var1, var2);

      #if (WMOPS)
      multiCounter[currCounter].L_shr_r--;
      #endif /* ifdef WMOPS */
   }

   #if (WMOPS)
   multiCounter[currCounter].L_shl_r++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : rotr
 *
 *  Purpose :
 *
 *    Performs a 16-bit logical rotation of var1 by 1 bit to the LSBits. The
 *    MSBit is set to var2 bit 0. The LSBit of var1 is kept in *var3 bit 0.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in
 *                the range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value must be 0
 *                or 1.
 *
 *  Outputs :
 *
 *    *var3       Points on a 16 bit short signed integer (Word16) whose 
 *                value will be 0 or 1.
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in
 *                the range : 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
Word16 rotr( Word16 var1, Word16 var2, Word16 *var3) {
   Word16 var_out;

   *var3 = s_and( var1, 0x1);
   var_out = s_or( lshr( var1, 1),
                   lshl( var2, 15));

   #if (WMOPS)
   multiCounter[currCounter].s_and--;
   multiCounter[currCounter].lshl--;
   multiCounter[currCounter].lshr--;
   multiCounter[currCounter].s_or--;
   multiCounter[currCounter].rotr++;
   #endif /* ifdef WMOPS */

  return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : rotl
 *
 *  Purpose :
 *
 *    Performs a 16-bit logical rotation of var1 by 1 bit to the MSBits. The
 *    LSBit is set to var2 bit 0. The MSBit of var1 is kept in *var3 bit 0.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in
 *                the range : 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value must be 0
 *                or 1.
 *
 *  Outputs :
 *
 *    *var3       Points on a 16 bit short signed integer (Word16) whose 
 *                value will be 0 or 1.
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in
 *                the range : 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
Word16 rotl( Word16 var1, Word16 var2, Word16 *var3) {
   Word16 var_out;

   *var3 = lshr( var1, 15);

   var_out = s_or( lshl( var1, 1),
                   s_and( var2, 0x1));

   #if (WMOPS)
   multiCounter[currCounter].lshr--;
   multiCounter[currCounter].s_and--;
   multiCounter[currCounter].lshl--;
   multiCounter[currCounter].s_or--;
   multiCounter[currCounter].rotl++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_rotr
 *
 *  Purpose :
 *
 *    Performs a 32-bit logical rotation of L_var1 by 1 bit to the LSBits. The
 *    MSBit is set to var2 bit 0. The LSBit of L_var1 is kept in *var3 bit 0.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    var2        16 bit short signed integer (Word16) whose value must be 0
 *                or 1.
 *
 *  Outputs :
 *
 *    *var3       Points on a 16 bit short signed integer (Word16) whose 
 *                value will be 0 or 1.
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
Word32 L_rotr( Word32 L_var1, Word16 var2, Word16 *var3) {
   Word32 L_var_out;

   *var3 = s_and( extract_l( L_var1), 0x1);

   L_var_out = L_or( L_lshr( L_var1, 1),
                     L_lshl( L_deposit_l( var2), 31));

   #if (WMOPS)
   multiCounter[currCounter].extract_l--;
   multiCounter[currCounter].s_and--;
   multiCounter[currCounter].L_deposit_l--;
   multiCounter[currCounter].L_lshl--;
   multiCounter[currCounter].L_lshr--;
   multiCounter[currCounter].L_or--;
   multiCounter[currCounter].L_rotr++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_rotl
 *
 *  Purpose :
 *
 *    Performs a 32-bit logical rotation of L_var1 by 1 bit to the MSBits. The
 *    LSBit is set to var2 bit 0. The MSBit of L_var1 is kept in *var3 bit 0.
 *
 *  Complexity weight : 3
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    var2        16 bit short signed integer (Word16) whose value must be 0
 *                or 1.
 *
 *  Outputs :
 *
 *    *var3       Points on a 16 bit short signed integer (Word16) whose 
 *                value will be 0 or 1.
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in
 *                the range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
Word32 L_rotl( Word32 L_var1, Word16 var2, Word16 *var3) {
   Word32 L_var_out;

   *var3 = extract_l( L_lshr( L_var1, 31));

   L_var_out = L_or( L_lshl( L_var1, 1),
                     L_deposit_l( s_and( var2, 0x1)));

   #if (WMOPS)
   multiCounter[currCounter].L_lshr--;
   multiCounter[currCounter].extract_l--;
   multiCounter[currCounter].s_and--;
   multiCounter[currCounter].L_deposit_l--;
   multiCounter[currCounter].L_lshl--;
   multiCounter[currCounter].L_or--;
   multiCounter[currCounter].L_rotl++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}
































/* end of file */
