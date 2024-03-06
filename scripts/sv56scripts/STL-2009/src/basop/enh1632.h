/*
  ===========================================================================
   File: ENH1632.H                                       v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T        STL        BASIC       OPERATORS

            ENHANCED 16-BIT & 32-BIT ARITHMETIC OPERATORS

   History:
   07 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
   March 06   v2.1      Changed to improve portability.
                        Some counters incrementations were missing (s_and, 
                        s_or, s_xor).                        
   30 Nov 09   v2.3     saturate() removed

  ============================================================================
*/


#ifndef _ENH1632_H
#define _ENH1632_H


/*****************************************************************************
 *
 *  Constants and Globals
 *
 *****************************************************************************/


#include "stl.h"


#if (WMOPS)
#include "count.h"
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;
#endif /* ifdef WMOPS */


/*****************************************************************************
 *
 *  Prototypes for enhanced 16/32 bit arithmetic operators
 *
 *****************************************************************************/
Word16 shl_r(   Word16 var1,   Word16 var2);
Word32 L_shl_r( Word32 L_var1, Word16 var2);


Word16 lshl(    Word16 var1,   Word16 var2);
Word16 lshr(    Word16 var1,   Word16 var2);
Word32 L_lshl(  Word32 L_var1, Word16 var2);
Word32 L_lshr(  Word32 L_var1, Word16 var2);

Word16 rotr(    Word16 var1,   Word16 var2, Word16 *var3);
Word16 rotl(    Word16 var1,   Word16 var2, Word16 *var3);
Word32 L_rotr(  Word32 var1,   Word16 var2, Word16 *var3);
Word32 L_rotl(  Word32 var1,   Word16 var2, Word16 *var3);



/*****************************************************************************
 *
 *  Functions
 *
 *****************************************************************************/

/*****************************************************************************
 *
 *  Function Name : s_max
 *
 *  Purpose :
 *
 *    Compares var1 and var2 and returns the maximum value.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0x8000 <= var1 <= 0x7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in
 *                the range : 0x8000 <= var2 <= 0x7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in
 *                the range : 0x8000 <= L_var_out <= 0x7fff.
 *
 *****************************************************************************/
static __inline Word16 s_max( Word16 var1, Word16 var2) {
   Word16 var_out;

   if( var1 >= var2)
      var_out = var1;
   else
      var_out = var2;

   #if (WMOPS)
   multiCounter[currCounter].s_max++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : s_min
 *
 *  Purpose :
 *
 *    Compares var1 and var2 and returns the minimum value.
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0x8000  <= var1 <= 0x7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0x8000  <= var2 <= 0x7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value falls in 
 *                the range : 0x8000 <= var_out <= 0x7fff.
 *
 *****************************************************************************/
static __inline Word16 s_min( Word16 var1, Word16 var2) {
   Word16 var_out;

   if( var1 <= var2)
      var_out = var1;
   else
      var_out = var2;

   #if (WMOPS)
   multiCounter[currCounter].s_min++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_max
 *
 *  Purpose :
 *
 *    Compares L_var1 and L_var2 and returns the maximum value.
 *
 *  Complexity weight : 1
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
 *    none
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in the
 *                range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
static __inline Word32 L_max( Word32 L_var1, Word32 L_var2) {
   Word32 L_var_out;

   if( L_var1 >= L_var2)
      L_var_out = L_var1;
   else
      L_var_out = L_var2;

   #if (WMOPS)
   multiCounter[currCounter].L_max++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_min
 *
 *  Purpose :
 *
 *    Compares L_var1 and L_var2 and returns the minimum value.
 *
 *  Complexity weight : 1
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
 *    none
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value falls in the
 *                range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
static __inline Word32 L_min( Word32 L_var1, Word32 L_var2) {
   Word32 L_var_out;

   if( L_var1 <= L_var2)
      L_var_out = L_var1;
   else
      L_var_out = L_var2;

   #if (WMOPS)
   multiCounter[currCounter].L_min++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}










/*****************************************************************************
 *
 *  Function Name : s_and
 *
 *  Purpose :
 *
 *    Performs logical AND of the two 16 bit input variables.
 *    var_out = var1 & var2
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
static __inline Word16 s_and( Word16 var1, Word16 var2) {
   Word16 var_out;

   var_out = var1 & var2;

   #if (WMOPS)
   multiCounter[currCounter].s_and++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_and
 *
 *  Purpose :
 *
 *    Performs logical AND of the two 32 bit input variables.
 *    L_var_out = L_var1 & L_var2
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    L_var2      32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
static __inline Word32 L_and( Word32 L_var1, Word32 L_var2) {
   Word32 L_var_out;

   L_var_out = L_var1 & L_var2;

   #if (WMOPS)
   multiCounter[currCounter].L_and++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}


/*****************************************************************************
 *
 *  Function Name : s_or
 *
 *  Purpose :
 *
 *    Performs logical OR of the two 16 bit input variables.
 *    var_out = var1 | var2
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
static __inline Word16 s_or( Word16 var1, Word16 var2) {
   Word16 var_out;

   var_out = var1 | var2;

   #if (WMOPS)
   multiCounter[currCounter].s_or++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_or
 *
 *  Purpose :
 *
 *    Performs logical OR of the two 32 bit input variables.
 *    L_var_out = L_var1 | L_var2
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    L_var2      32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
static __inline Word32 L_or( Word32 L_var1, Word32 L_var2) {

   Word32 L_var_out;

   L_var_out = L_var1 | L_var2;

   #if (WMOPS)
   multiCounter[currCounter].L_or++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}


/*****************************************************************************
 *
 *  Function Name : s_xor
 *
 *  Purpose :
 *
 *    Performs logical XOR of the two 16 bit input variables.
 *    var_out = var1 ^ var2
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    var1        16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var1 <= 0x0000 7fff.
 *
 *    var2        16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var2 <= 0x0000 7fff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    var_out     16 bit short signed integer (Word16) whose value
 *                falls in the range 0xffff 8000 <= var_out <= 0x0000 7fff.
 *
 *****************************************************************************/
static __inline Word16 s_xor( Word16 var1, Word16 var2) {
   Word16 var_out;

   var_out = var1 ^ var2;

   #if (WMOPS)
   multiCounter[currCounter].s_xor++;
   #endif /* ifdef WMOPS */

   return( var_out);
}


/*****************************************************************************
 *
 *  Function Name : L_xor
 *
 *  Purpose :
 *
 *    Performs logical OR of the two 32 bit input variables.
 *    L_var_out = L_var1 ^ L_var2
 *
 *  Complexity weight : 1
 *
 *  Inputs :
 *
 *    L_var1      32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var1 <= 0x7fff ffff.
 *
 *    L_var2      32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var2 <= 0x7fff ffff.
 *
 *  Outputs :
 *
 *    none
 *
 *  Return Value :
 *
 *    L_var_out   32 bit long signed integer (Word32) whose value
 *                falls in the range 0x8000 0000 <= L_var_out <= 0x7fff ffff.
 *
 *****************************************************************************/
static __inline Word32 L_xor( Word32 L_var1, Word32 L_var2) {
   Word32 L_var_out;

   L_var_out = L_var1 ^ L_var2;

   #if (WMOPS)
   multiCounter[currCounter].L_xor++;
   #endif /* ifdef WMOPS */

   return( L_var_out);
}



































#endif /*_ENH1632_H*/

/* end of file */
