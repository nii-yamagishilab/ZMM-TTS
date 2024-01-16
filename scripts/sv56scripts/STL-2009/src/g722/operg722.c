/*                                                   Version: 2.00 - 01.Jul.95
  ============================================================================

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
       ** This code has  (C) Copyright by CNET Lannion A TSS/CMC **
       =============================================================


MODULE:         G722.C 7kHz ADPCM AT 64 KBIT/S MODULE ENCODER AND 
                DECODER BASIC OPERATORS

ORIGINAL BY:
    J-P PETIT 
    CNET - Centre Lannion A
    LAA-TSS                         Tel: +33-96-05-39-41
    Route de Tregastel - BP 40      Fax: +33-96-05-13-16
    F-22301 Lannion CEDEX           Email: petitjp@lannion.cnet.fr
    FRANCE
    
   History:
   14.Mar.95    v1.0    Released for use ITU-T UGST software package Tool
                        based on the CNET's 07/01/90 version 2.00
   01.Jul.95    v2.0    Changed function declarations to work with 
                        many compilers; reformated <simao@ctd.comsat.com>
  ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "operg722.h"

/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if defined(MSDOS) || defined(__MSDOS__) || defined(__STDC__) || defined(VMS) || defined (__CYGWIN__) || defined (_MSC_VER)
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* Local function prototypes */
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


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : sature                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Limit the 32 bit input to the range of a 16 bit word.                  |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
*/
Word16 
sature (L_var1)
Word32 L_var1;
{
  Word16          var_out;

  if (L_var1 > 0X00007fffL)
  {
    var_out = MAX_16;
  }
  else
  {
    if (L_var1 < (Word32) 0xffff8000L)
    {
      var_out = MIN_16;
    }
    else
    {
      var_out = extract_l (L_var1);
    }
  }
  return (var_out);
}
/* ....................... end of sature() ............................ */

/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : extract_l                                               |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Return the 16 LSB of L_var1.                                            |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1                                                                 |
 |             32 bit long signed integer (Word32 ) whose value falls in the |
 |             range : 0x8000 0000 <= L_var1 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
 */

Word16 
extract_l (L_var1)
Word32 L_var1;
{
  Word16          var_out;

  var_out = (Word16) L_var1;
  return (var_out);
}
/* ....................... end of extract_l() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : add                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the addition (var1+var2) with overflow control and saturation;|
 |    the 16 bit result is set at +32767 when overflow occurs or at -32768   |
 |    when underflow occurs.                                                 |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
 */

Word16 
add (var1, var2)
Word16 var1;
Word16 var2;
{
  Word16          var_out;
  Word32          L_somme;

  L_somme = (Word32) var1 + (Word32) var2;
  var_out = sature (L_somme);
  return (var_out);
}
/* ....................... end of add() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : sub                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the subtraction (var1+var2) with overflow control and satu-   |
 |    ration; the 16 bit result is set at +32767 when overflow occurs or at  |
 |    -32768 when underflow occurs.                                          |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
 */

Word16 
sub (var1, var2)
Word16 var1;
Word16 var2;
{
  Word16          var_out;
  Word32          L_diff;

  L_diff = (Word32) var1 - (Word32) var2;
  var_out = sature (L_diff);
  return (var_out);
}
/* ....................... end of sub() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shl                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 left var2 positions.Zero fill|
 |   the var2 LSB of the result. If var2 is negative, arithmetically shift   |
 |   var1 right by -var2 with sign extension. Saturate the result in case of |
 |   underflows or overflows.                                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
 */

Word16 
shl (var1, var2)
Word16 var1;
Word16 var2;
{
  Word16          var_out;
  Word32          L_result;

  if (var2 < 0)
  {
    var_out = shr (var1, -var2);
  }
  else
  {
    L_result = (Word32) var1 *((Word32) 1 << var2);
    if (((var2 > 15) && (var1 != 0)) ||
	(L_result != (Word32) ((Word16) L_result)))
    {
      var_out = (var1 > 0) ? MAX_16 : MIN_16;
    }
    else
    {
      var_out = extract_l (L_result);
    }
  }

  return (var_out);
}
/* ....................... end of shl() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : shr                                                     |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 16 bit input var1 right var2 positions with    |
 |   sign extension. If var2 is negative, arithmetically shift var1 left by  |
 |   -var2 with sign extension. Saturate the result in case of underflows or |
 |   overflows.                                                              |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
 */

Word16 
shr (var1, var2)
Word16 var1;
Word16 var2;
{
  Word16          var_out;

  if (var2 < 0)
  {
    var_out = shl (var1, -var2);
  }
  else
  {
    if (var2 >= 15)
    {
      var_out = (var1 < (Word16) 0) ? (Word16) - 1 : (Word16) 0;
    }
    else
    {
      if (var1 < 0)
      {
	var_out = ~((~var1) >> var2);
      }
      else
      {
	var_out = var1 >> var2;
      }
    }
  }

  return (var_out);
}
/* ....................... end of shr() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : mult                                                    |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |    Performs the multiplication of var1 by var2 and gives a 16 bit result  |
 |    which is scaled i.e.:                                                  |
 |             mult(var1,var2) = shr((var1 times var2),15) and               |
 |             mult(-32768,-32768) = 32767.                                  |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
 */

Word16 
mult (var1, var2)
Word16 var1;
Word16 var2;
{
  Word16          var_out;
  Word32          L_produit;

  L_produit = (Word32) var1 *(Word32) var2;

  L_produit = (L_produit & (Word32) 0xffff8000L) >> 15;

  if (L_produit & (Word32) 0x00010000L)
  {
    L_produit |= (Word32) 0xffff0000L;
  }
  var_out = sature (L_produit);
  return (var_out);
}
/* ....................... end of mult() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : negate                                                  |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Negate var1 with saturation, saturate in the case where input is -32768:|
 |                negate(var1) = sub(0,var1).                                |
 |                                                                           |
 |   Complexity weight : 1                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    var1                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    var_out                                                                |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var_out <= 0x0000 7fff.                |
 |___________________________________________________________________________|
 */

Word16 
negate (var1)
Word16 var1;
{
  Word16          var_out;

  var_out = (var1 == MIN_16) ? MAX_16 : -var1;
  return (var_out);
}
/* ....................... end of negate() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_add                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   32 bits addition of the two 32 bits variables (L_var1+L_var2) with      |
 |   overflow control and saturation; the result is set at +214783647 when   |
 |   overflow occurs or at -214783648 when underflow occurs.                 |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    L_var2   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
 */

Word32 
L_add (L_var1, L_var2)
Word32 L_var1;
Word32 L_var2;
{
  Word32          L_var_out;

  L_var_out = L_var1 + L_var2;

  if (((L_var1 ^ L_var2) & MIN_32) == 0L)
  {
    if ((L_var_out ^ L_var1) & MIN_32)
    {
      L_var_out = (L_var1 < 0L) ? MIN_32 : MAX_32;
    }
  }
  return (L_var_out);
}
/* ....................... end of L_add() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shr                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 right var2 positions with  |
 |   sign extension. If var2 is negative, arithmetically shift L_var1 left   |
 |   by -var2 and zero fill the var2 LSB of the result. Saturate the result  |
 |   in case of underflows or overflows.                                     |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
 */

Word32 
L_shr (L_var1, var2)
Word32 L_var1;
Word16 var2;
{
  Word32          L_var_out;

  if (var2 < (Word16) 0)
  {
    L_var_out = L_shl (L_var1, -var2);
  }
  else
  {
    if (var2 >= (Word16) 31)
    {
      L_var_out = (L_var1 < 0L) ? -1L : 0L;
    }
    else
    {
      if (L_var1 < 0L)
      {
	L_var_out = ~((~L_var1) >> var2);
      }
      else
      {
	L_var_out = L_var1 >> var2;
      }
    }
  }
  return (L_var_out);
}
/* ....................... end of L_shr() ............................ */


/*___________________________________________________________________________
 |                                                                           |
 |   Function Name : L_shl                                                   |
 |                                                                           |
 |   Purpose :                                                               |
 |                                                                           |
 |   Arithmetically shift the 32 bit input L_var1 left var2 positions. Zero  |
 |   fill the var2 LSB of the result. If var2 is negative, L_var1 right by   |
 |   -var2 arithmetically shift with sign extension. Saturate the result in  |
 |   case of underflows or overflows.                                        |
 |                                                                           |
 |   Complexity weight : 2                                                   |
 |                                                                           |
 |   Inputs :                                                                |
 |                                                                           |
 |    L_var1   32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var3 <= 0x7fff ffff.                 |
 |                                                                           |
 |    var2                                                                   |
 |             16 bit short signed integer (Word16) whose value falls in the |
 |             range : 0xffff 8000 <= var1 <= 0x0000 7fff.                   |
 |                                                                           |
 |   Outputs :                                                               |
 |                                                                           |
 |    none                                                                   |
 |                                                                           |
 |   Return Value :                                                          |
 |                                                                           |
 |    L_var_out                                                              |
 |             32 bit long signed integer (Word32) whose value falls in the  |
 |             range : 0x8000 0000 <= L_var_out <= 0x7fff ffff.              |
 |___________________________________________________________________________|
 */

Word32 
L_shl (L_var1, var2)
Word32 L_var1;
Word16 var2;
{
  Word32          L_var_out = 0L;

  if (var2 <= (Word16) 0)
  {
    L_var_out = L_shr (L_var1, -var2);
  }
  else
  {
    for (; var2 > (Word16) 0; var2--)
    {
      if (L_var1 > (Word32) 0X3fffffffL)
      {
	L_var_out = MAX_32;
	break;
      }
      else
      {
	if (L_var1 < (Word32) 0xc0000000L)
	{
	  L_var_out = MIN_32;
	  break;
	}
      }
      L_var1 *= 2L;
      L_var_out = L_var1;
    }
  }
  return (L_var_out);
}
/* ....................... end of L_shl() ............................ */

