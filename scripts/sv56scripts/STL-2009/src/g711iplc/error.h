/*
  ============================================================================
   File: error.h                                             V.1.0-24.MAY-2005
  ============================================================================

                     UGST/ITU-T G711 Appendix I PLC MODULE

                          GLOBAL FUNCTION PROTOTYPES

   History:
   24.May.05	v1.0	First version <AT&T>
						Integration in STL2005 <Cyril Guillaume & Stephane Ragot - stephane.ragot@francetelecom.com>
  ============================================================================
*/

#ifndef __ERROR_H__
#define __ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32 	/* Windows 95, 98 or NT */
#ifndef WINAPIV
#define WINAPIV __cdecl
#endif
#else		/* Unix, Linux */
#define WINAPIV
#endif

void WINAPIV error(char *s, ...);

#ifdef __cplusplus
}
#endif

#endif  /* __ERROR_H__ */
