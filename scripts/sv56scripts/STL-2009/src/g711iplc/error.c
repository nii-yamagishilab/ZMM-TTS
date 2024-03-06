/*                                                             v1.0 24.MAY.2005
  =============================================================================

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
	         ** This code has  (C) Copyright by AT&T Corp. **
       =============================================================

MODULE:         G711 Appendix I PLC module
                Sub-unit: Error message routine

ORIGINAL BY:	AT&T Corp.

DESCRIPTION:
        This file contains procedures for printing error messages.

FUNCTIONS:
  error ......... Print an error message.

HISTORY:
    24.May.05 v1.0 First version <AT&T>
		Integration in STL2005 <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>
  =============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

void WINAPIV error(char *s, ...)
{
        va_list ap;

        va_start(ap, s);
        vfprintf(stderr, s, ap);
        va_end(ap);
        fprintf(stderr, "\n");
        exit(1);
}
