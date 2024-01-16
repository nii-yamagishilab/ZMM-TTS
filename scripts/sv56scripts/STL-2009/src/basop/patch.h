/*
  ===========================================================================
   File: PATCH.H                                         v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T STL BASIC OPERATORS

            OPERATOR   NAME  PATCHING

   History:
   07 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.

  ============================================================================
*/


#ifndef _PATCH_H
#define _PATCH_H


#if 1
/*****************************************************************************
 * start of Patch1 :
 * It is enabled.
 *
 * This patch enables to keep compatibility with old ITU DSP operator names
 * following naming conventions as proposed to ITU-T Standard Tool Library
 * definition group in Geneva, 20-30 January 2004 WP 3/16 Q10/16
 * TD 11 document.
 *
 *****************************************************************************/
#define shift_r(   var1,   var2)   shl_r(   var1,   var2)
#define L_shift_r( L_var1, var2)   L_shl_r( L_var1, var2)


#endif /* end of Patch1 */










#endif /* end of _PATCH_H */
