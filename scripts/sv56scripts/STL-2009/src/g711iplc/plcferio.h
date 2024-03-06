/*
  ============================================================================
   File: plcferio.h                                          V.1.0-24.MAY-2005
  ============================================================================

                     UGST/ITU-T G711 Appendix I PLC MODULE

                          GLOBAL FUNCTION PROTOTYPES

   History:
   24.May.05	v1.0	First version <AT&T>
						Integration in STL2005 <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>
  ============================================================================
*/
#include <stdio.h>

#ifndef __READPLCMASK_H__
#define __READPLCMASK_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * code to read the PLC mask in the ITU-T software tools format.
 */
typedef struct _readplcmask {
	long	(*readfunc)(short*,long, FILE*);
	FILE	*fp;
} readplcmask;

void readplcmask_open(readplcmask*, char *fname);
int readplcmask_erased(readplcmask*);
void readplcmask_close(readplcmask*);

#ifdef __cplusplus
}
#endif

#endif  /* __READPLCMASK_H__ */
