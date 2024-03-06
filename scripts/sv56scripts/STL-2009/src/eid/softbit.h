/*
  ============================================================================
   File: SOFTBIT.H                                                   10.OCT.97
  ============================================================================

			  UGST/ITU-T UTILITY MODULE

	  PROTOTYPES FOR SOFTBIT CONVERSION AND FILE I/O OPERATIONS

   History:
   10.Oct.97     1.00   Created
  ============================================================================
*/
#ifndef SOFTBIT_DEFINED
#define SOFTBIT_DEFINED 100

/* ......... Smart prototypes .......... */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(x) x
#else
#define ARGS(x) ()
#endif
#endif


/* ..... Definitions for softbit operations ..... */

/* Operating modes */
enum BS_formats {byte, g192, compact, nil};
enum BS_types {NO_HEADER, HAS_HEADER, HAS_FLAG_ONLY};
enum EP_types {BER, FER};

/* Definitions for byte mode */
#define BYTE_ZERO       (char)0X7F
#define BYTE_ONE	(char)0X81
#define BYTE_SYNC	(char)0x21
#define BYTE_FER	(char)0x20

/* Definitions for G.192 mode */
#define G192_ZERO       (short)0X007F
#define G192_ONE	(short)0X0081
#define G192_SYNC	(short)0x6B21
#define G192_FER	(short)0x6B20

/* softbit.c */
long read_g192 ARGS((short *patt, long n, FILE *F));
long read_bit_ber ARGS((short *patt, long n, FILE *F));
long read_bit_fer ARGS((short *patt, long n, FILE *F));
long read_bit ARGS((short *patt, long n, FILE *F, char type));
long read_byte ARGS((short *patt, long n, FILE *F));
long save_g192 ARGS((short *patt, long n, FILE *F));
long save_bit ARGS((short *patt, long n, FILE *F));
long save_byte ARGS((short *patt, long n, FILE *F));
char *format_str ARGS((int fmt));
char *type_str ARGS((int type));
char check_eid_format ARGS((FILE *F, char *file, char *type));
long soft2hard ARGS((short *soft, short *hard, long n, char type));

#endif /* SOFTBIT_DEFINED */

/* ************************* END OF SOFTBIT.H ************************* */
