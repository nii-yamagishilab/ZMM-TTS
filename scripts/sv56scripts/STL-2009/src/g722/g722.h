/*
  ============================================================================
   File: G722.H                                  v3.0 - 10/Jan/2007
  ============================================================================

                            UGST/ITU-T G722 MODULE

                          GLOBAL FUNCTION PROTOTYPES

  History:
14.Mar.95  v1.0       Released for use ITU-T UGST software package Tool
                      based on the CNET's 07/01/90 version 2.00
01.Jul.95  v2.0       Changed function declarations to work with many compilers;
                      reformated <simao@ctd.comsat.com>
10.Jan.07  v3.0       Updated with STL2005 v2.1 basic operators
                      <{balazs.kovesi,stephane.ragot}@orange-ftgroup.com>
  ============================================================================
*/
#ifndef G722_H
#define G722_H 200

/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__)) || defined (__CYGWIN__) || defined (_MSC_VER)
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* Include function prototypes for G722 operators and type definitions */
/* #include "operg722.h" */
#include "stl.h"

/* Define type for G.722 state structure */
typedef struct
{
  Word16          al[3];
  Word16          bl[7];
  Word16          detl;
  Word16          dlt[7]; /* dlt[0]=dlt */
  Word16          nbl;
  Word16          plt[3]; /* plt[0]=plt */
  Word16          rlt[3];
  Word16          ah[3];
  Word16          bh[7];
  Word16          deth;
  Word16          dh[7]; /* dh[0]=dh */
  Word16          ph[3]; /* ph[0]=ph */
  Word16          rh[3];
  Word16          sl;
  Word16          spl;
  Word16          szl;
  Word16          nbh;
  Word16          sh;
  Word16          sph;
  Word16          szh;
  Word16          qmf_tx_delayx[24];
  Word16          qmf_rx_delayx[24];
}          g722_state;

/* Include function prototypes for G722 functions */
#include "funcg722.h"

/* High-level (UGST) function prototypes for G722 functions */
void g722_reset_encoder ARGS((g722_state *encoder));
Word32 g722_encode ARGS((short *incode, short *code, Word32 nsmp, 
		       g722_state *encoder));
void g722_reset_decoder ARGS((g722_state *decoder));
short g722_decode ARGS((short *code, short *outcode, short mode, 
		       short nsmp, g722_state *decoder));

#endif /* G722_H */
/* ................. End of file g722.h .................................. */
