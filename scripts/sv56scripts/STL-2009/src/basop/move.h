/*
  ===========================================================================
   File: MOVE.H                                          v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T  STL  BASIC  OPERATORS

            MOVE & MISC LEGACY OPERATORS

   History:
   07 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
   March 06   v2.1      Changed to improve portability.                        

  ============================================================================
*/


#ifndef _MOVE_H
#define _MOVE_H


#include "stl.h"

#if (WMOPS)
extern BASIC_OP multiCounter[MAXCOUNTERS];
extern int currCounter;
#endif /* ifdef WMOPS */


static __inline void move16( void) {
#if WMOPS
    multiCounter[currCounter].move16++;
#endif /* ifdef WMOPS */
}






static __inline void move32( void) {
#if WMOPS
    multiCounter[currCounter].move32++;
#endif /* ifdef WMOPS */
}






static __inline void test( void) {
#if WMOPS
    multiCounter[currCounter].Test++;
#endif /* ifdef WMOPS */
}


static __inline void logic16( void) {
#if WMOPS
    multiCounter[currCounter].Logic16++;
#endif /* ifdef WMOPS */
}


static __inline void logic32( void) {
#if WMOPS
    multiCounter[currCounter].Logic32++;
#endif /* ifdef WMOPS */
}


/*-------- legacy ----------*/
#define data_move()          move16()
#define L_data_move()        move32()
#define data_move_external() move16()
#define compare_zero()       test()
/*-------- end legacy ----------*/


#endif /* _MOVE_H */


/* end of file */
