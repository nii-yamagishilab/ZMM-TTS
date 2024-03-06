/*
==============================================================================

  MEASURE.C
  ~~~~~~~~~

  Program to calculate the max, min, and average values for a file, as 
  well as 16-bit CRCs (CCITT, XMODEM, ARC), if requested.

  Usage:
  ~~~~~~
  $ measure file|- [other files ...]
  where:
  file          is the name of the file to measured; - is stdin
                process all the files specified
        
  Options:
  ~~~~~~~~
  -h|-?         show program usage
  -blk size     change the number of samples per block [default 256]
  -skip n       number of blocks to skip
  -crc          also calculate crc for files
  -hex          carry-out hex dump

  Compile:
  ~~~~~~~~
  SunC: cc -o measure measure.c -lm # or #
        acc -o measure measure.c -lm
  GNUC: gcc -o measure measure.c -lm
  VMS:  cc measure.c
        link measure
 
  History:
  ~~~~~~~~
  10.Mar.94   1.0   1st release by simao@cpqd.ansp.br
  21.Aug.95   1.1   Fixed bug with zero-length files <simao@ctd.comsat.com>.
==============================================================================
*/

/* OS definition */
#if defined(__MSDOS__) && !defined(MSDOS) /* def. for autom. compil.,TurboC */
# define MSDOS	  /* this is already defined for Microsoft C 5.1 */
#endif


/* includes in general */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ugstdemo.h"


/* Generic defines */
#define YES 1
#define NO  0

/* Defines for CRC routine */
#define W       16		/* CRC width */
#define WTYPE	unsigned short	/* Basic data type */
#define B	8		/* the number of bits per char */

/* Local function prototypes */
WTYPE updcrc    ARGS((WTYPE icrc, unsigned char *icp, int icnt, WTYPE *crctab, 
                char swapped));

/* Macros related to crc calculations */
#define get_ccitt_crc(crc,buf,n)  updcrc(crc, buf, n, crctab_ccitt, 0)
#define get_arc_crc(crc,buf,n)    updcrc(crc, buf, n, crctab_arc, 1)
#define get_xmodem_crc(crc,buf,n) updcrc(crc, buf, n, crctab_xmodem, 1)

/* Global variables related to crc calculations */
WTYPE           crc_a = 0, crc_c = 0, crc_x = 0;
int             init_crc_a = 0L, init_crc_c = -1L, init_crc_x = 0L;


/* 
 * --------------------------------------------------------------------------
 * ... Statistics routines ... 
 *     Simao 07.Mar.94
 * --------------------------------------------------------------------------
 */

double get_sum(samples, count)
short *samples;
long count;
{
  double sum = 0;
  
  while(count>0)
    sum += samples[--count];
  return(sum);
}

double get_square(samples, count)
short *samples;
long count;
{
  double sum = 0, tmp;
  
  while(count>0)
  {
    tmp = samples[--count];
    sum += tmp * tmp;
  }
  return(sum);
}

/* 
  --------------------------------------------------------------------------
  Find maximum in a short array; return as a double; uses parameter 
  'max' as a default, or as the so-far found, maximum value.
  --------------------------------------------------------------------------
*/
double get_max(max, samples, count)
double max;
short *samples;
long count;
{
  double tmp;
   
  while(count>0)
  {
    count--;
    tmp = samples[count];
    max = tmp>max? tmp : max;
  }
  return max;
}

/* 
  --------------------------------------------------------------------------
  Find minimum in a short array; return as a double; uses parameter 
  'min' as a default, or as the so-far found, minimum value.
  --------------------------------------------------------------------------
*/
double get_min(min, samples, count)
double min;
short *samples;
long count;
{
  double tmp;
   
  while(count>0)
  {
    count--;
    tmp = samples[count];
    min = tmp<min? tmp : min;
  }
  return min;
}

double get_var(rms_x, avg_x, count)
double rms_x, avg_x;
long count;
{
 return ((rms_x - avg_x * avg_x / (double)count) / (double)count);
}
/* ....................... end of get_...() ...........................*/


/*
  ---------------------------------------------------------------------------
  WTYPE updcrc (WTYPE icrc, unsigned char *icp, int icnt, WTYPE *crctab, 
  ~~~~~~~~~~~~  char swapped);
  
  Description:
  ~~~~~~~~~~~~
  Calculate, intelligently, the CRC-16 of a dataset incrementally given a
  buffer full at a time. The parameters on which the function relyes are:

  Parameter             CCITT   XMODEM  ARC
  ---------             ------  ------  ----
  the polynomial P	0x1021	0x1021	A001
  initial CRC value:	-1	0	0
  bit-order swap        No (0)  Yes (1) Yes (1)
  bits in CRC:          16	16	16

  Usage:
  ~~~~~~
  	newcrc = updcrc( oldcrc, bufadr, buflen, crctab, swapped)
  		   WTYPE oldcrc;
                   int buflen;
  		   char *bufadr;
                   WTYPE *crctab;
                   char swapped;

  Notes:
   Regards the data stream as an integer whose MSB is the MSB of the first
   byte recieved.  This number is 'divided' (using xor instead of subtraction)
   by the crc-polynomial P.
   XMODEM does things a little differently, essentially treating the LSB of
   the first data byte as the MSB of the integer.

  Original Author:
  ~~~~~~~~~~~~~~~~
  Mark G. Mendel, 7/86
  UUCP: ihnp4!umn-cs!hyper!mark, GEnie: mgm
  [ Original code available in comp.sources.unix volume 6 (1989) ]

  History:
  ~~~~~~~~
  xx.Jul.86 v1.0  Release by the author [posted to c.s.misc in 1989]
  01.Sep.93 v1.0a Adapted by Simao <simao@cpqd.ansp.br> to be used in the
 		  x{en,de}code.c.

  ---------------------------------------------------------------------------
*/

/* Table for CCITT CRCs */
static WTYPE    crctab_ccitt[1 << B] = {
  0x0, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0xa50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0xc60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0xe70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0xa1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x2b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x8e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0xaf1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0xcc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0xed1, 0x1ef0,
};

/* Table for xmodem CRCs */
static WTYPE    crctab_xmodem[1 << B] = {
  0x0, 0x17ce, 0xfdf, 0x1811, 0x1fbe, 0x870, 0x1061, 0x7af,
  0x1f3f, 0x8f1, 0x10e0, 0x72e, 0x81, 0x174f, 0xf5e, 0x1890,
  0x1e3d, 0x9f3, 0x11e2, 0x62c, 0x183, 0x164d, 0xe5c, 0x1992,
  0x102, 0x16cc, 0xedd, 0x1913, 0x1ebc, 0x972, 0x1163, 0x6ad,
  0x1c39, 0xbf7, 0x13e6, 0x428, 0x387, 0x1449, 0xc58, 0x1b96,
  0x306, 0x14c8, 0xcd9, 0x1b17, 0x1cb8, 0xb76, 0x1367, 0x4a9,
  0x204, 0x15ca, 0xddb, 0x1a15, 0x1dba, 0xa74, 0x1265, 0x5ab,
  0x1d3b, 0xaf5, 0x12e4, 0x52a, 0x285, 0x154b, 0xd5a, 0x1a94,
  0x1831, 0xfff, 0x17ee, 0x20, 0x78f, 0x1041, 0x850, 0x1f9e,
  0x70e, 0x10c0, 0x8d1, 0x1f1f, 0x18b0, 0xf7e, 0x176f, 0xa1,
  0x60c, 0x11c2, 0x9d3, 0x1e1d, 0x19b2, 0xe7c, 0x166d, 0x1a3,
  0x1933, 0xefd, 0x16ec, 0x122, 0x68d, 0x1143, 0x952, 0x1e9c,
  0x408, 0x13c6, 0xbd7, 0x1c19, 0x1bb6, 0xc78, 0x1469, 0x3a7,
  0x1b37, 0xcf9, 0x14e8, 0x326, 0x489, 0x1347, 0xb56, 0x1c98,
  0x1a35, 0xdfb, 0x15ea, 0x224, 0x58b, 0x1245, 0xa54, 0x1d9a,
  0x50a, 0x12c4, 0xad5, 0x1d1b, 0x1ab4, 0xd7a, 0x156b, 0x2a5,
  0x1021, 0x7ef, 0x1ffe, 0x830, 0xf9f, 0x1851, 0x40, 0x178e,
  0xf1e, 0x18d0, 0xc1, 0x170f, 0x10a0, 0x76e, 0x1f7f, 0x8b1,
  0xe1c, 0x19d2, 0x1c3, 0x160d, 0x11a2, 0x66c, 0x1e7d, 0x9b3,
  0x1123, 0x6ed, 0x1efc, 0x932, 0xe9d, 0x1953, 0x142, 0x168c,
  0xc18, 0x1bd6, 0x3c7, 0x1409, 0x13a6, 0x468, 0x1c79, 0xbb7,
  0x1327, 0x4e9, 0x1cf8, 0xb36, 0xc99, 0x1b57, 0x346, 0x1488,
  0x1225, 0x5eb, 0x1dfa, 0xa34, 0xd9b, 0x1a55, 0x244, 0x158a,
  0xd1a, 0x1ad4, 0x2c5, 0x150b, 0x12a4, 0x56a, 0x1d7b, 0xab5,
  0x810, 0x1fde, 0x7cf, 0x1001, 0x17ae, 0x60, 0x1871, 0xfbf,
  0x172f, 0xe1, 0x18f0, 0xf3e, 0x891, 0x1f5f, 0x74e, 0x1080,
  0x162d, 0x1e3, 0x19f2, 0xe3c, 0x993, 0x1e5d, 0x64c, 0x1182,
  0x912, 0x1edc, 0x6cd, 0x1103, 0x16ac, 0x162, 0x1973, 0xebd,
  0x1429, 0x3e7, 0x1bf6, 0xc38, 0xb97, 0x1c59, 0x448, 0x1386,
  0xb16, 0x1cd8, 0x4c9, 0x1307, 0x14a8, 0x366, 0x1b77, 0xcb9,
  0xa14, 0x1dda, 0x5cb, 0x1205, 0x15aa, 0x264, 0x1a75, 0xdbb,
  0x152b, 0x2e5, 0x1af4, 0xd3a, 0xa95, 0x1d5b, 0x54a, 0x1284,
};

/* Table for ARC CRCs */
static WTYPE    crctab_arc[1 << B] = {
  0x0, 0xc0c1, 0xc181, 0x140, 0xc301, 0x3c0, 0x280, 0xc241,
  0xc601, 0x6c0, 0x780, 0xc741, 0x500, 0xc5c1, 0xc481, 0x440,
  0xcc01, 0xcc0, 0xd80, 0xcd41, 0xf00, 0xcfc1, 0xce81, 0xe40,
  0xa00, 0xcac1, 0xcb81, 0xb40, 0xc901, 0x9c0, 0x880, 0xc841,
  0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
  0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
  0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
  0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
  0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
  0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
  0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
  0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
  0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
  0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
  0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
  0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
  0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
  0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
  0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
  0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
  0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
  0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
  0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
  0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
  0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
  0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
  0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
  0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
  0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
  0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
  0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
  0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040,
};

/* -------------- Begin of function updcrc() ---------------------- */
WTYPE
updcrc(icrc, icp, icnt, crctab, swapped)
  WTYPE           icrc, *crctab;
  unsigned char  *icp;
  int             icnt;
  char            swapped;
{
  register WTYPE  crc = icrc;
  register unsigned char *cp = icp;
  register int    cnt = icnt;

  if (swapped)
    while (cnt--)
    {
      crc = (crc >> B) ^ crctab[(crc & ((1 << B) - 1)) ^ *cp++];
    }
  else
    while (cnt--)
    {
      crc = (crc << B) ^ crctab[(crc >> (W - B)) ^ *cp++];
    }

  return (crc);
}
/* .......................... End of updcrc() ............................ */


/* 
 * --------------------------------------------------------------------------
 * ... Display usage of program ... 
 *     Simao 10.Mar.94
 * --------------------------------------------------------------------------
 */
#define P(x) printf x
void display_usage()
{
  P(("MEASURE Version 1.0 of 10/Mar/1994 \n\n"));

  P((" Program to calculate the max, min, and average values for a file.\n"));
  P((" Usage:\n"));
  P(("  $ measure file \n"));
  P(("  where:\n"));
  P(("  file          is the name of the file to measured\n"));
  P(("\n"));
  P((" Options:\n"));
  P((" ~~~~~~~~\n"));
  P((" -h or -?      show program usage\n"));
  P((" -blk size     change the number of samples per block [default 256]\n"));
  P((" -skip n       number of blocks to skip [default: none]\n"));
  P((" -crc          also calculate crc for files\n"));
  P((" -hex          carry-out hex dump\n"));
  P(("\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ....................... end of display_usage() ...........................*/


/*============================== */
int main(argc,argv)
int argc;
char *argv[];
/*============================== */
{
  char display_crc = 0, print_hex = 0;
  char inpfil[50];
  long count,skip=0;
  unsigned short us_max, us_min;
  long smpno,blk;
  short *samples;
#ifdef VMS
  static char     mrs[15] = "mrs=512";	/* for correct mrs, VMS environment */
#endif

  double  avg_x, rms_x, stddev_x, max_x, min_x;

  FILE *Fi;


  /* DEFAULT OPTIONS AND INITIAL VALUES */

  skip = 0;
  blk = 256;

  /* GETTING OPTIONS */

    while (argc > 1 && argv[1][0] == '-')
      if (strcmp(argv[1],"-?")==0 || strcmp(argv[1],"-h")==0 )
      {
	/* Display usage */
	display_usage();
      }
      else if (strcmp(argv[1],"-blk")==0)
      {
        /* Change block size for averages */
	blk = atol(argv[2]);
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1],"-skip")==0)
      {
        /* Define number of blocks (defined by blk) to skip */
	skip = atol(argv[2]);
	argc -= 2;
	argv += 2;
      }
      else if (strcmp(argv[1],"-crc")==0)
      {
        /* Change block size for averages */
	display_crc = 1;
	argc--;
	argv++;
      }
      else if (strcmp(argv[1],"-hex")==0)
      {
        /* Change block size for averages */
	print_hex = 1;
	argc--;
	argv++;
      }
      else if (strcmp(argv[1],"-")==0)
      {
	break;
      }
      else
      {
	fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
		argv[1]);
	display_usage();
      }

  /* REPEAT FOR ALL FILES IN THE COMMAND LINE */
  while(argc>1)
  {
    /* Get new file name and update argument line pointer/counter */
    strcpy(inpfil, argv[1]);
    argv++;
    argc--;
    
    /* Reset statistics */
    avg_x = rms_x = 0; 
    smpno = 0;
    max_x = -1E38; 
    min_x =  1E38;

    /* Initialize crc calculations */
    crc_a = init_crc_a;
    crc_c = init_crc_c;
    crc_x = init_crc_x;
    
    /* Open files */
    if (strcmp(inpfil,"-") == 0)
      Fi = stdin;
    else
      if ((Fi=fopen(inpfil,RB))==NULL) KILL(inpfil,2);

    /* Move onto start */
    fseek(Fi, skip * blk * sizeof(short), 0);

    /* Allocate memory */
    samples = (short *) calloc(blk, sizeof(short));
    if (!samples)
      HARAKIRI("Cannot allocate memory - aborted\n", 1);
    
    /* Find statistics */
    while((count = fread(samples, sizeof(short), blk, Fi))!=0)
    {
      /* Update statistics */
      avg_x += get_sum(samples, count);
      rms_x += get_square(samples, count);
      max_x = get_max(max_x, samples, count);
      min_x = get_min(min_x, samples, count);
    
      /* Get CRCs on demand */
      if (display_crc)
      {
	crc_a = get_arc_crc(crc_a, (char *)samples, 2 * count);
	crc_c = get_ccitt_crc(crc_c, (char *)samples, 2 * count);
	crc_x = get_xmodem_crc(crc_x, (char *)samples, 2 * count);
      }
    
      /* Update sample counter */
      smpno+= count;
    }

    /* Check if empty file */
    if (smpno==0)
    {
      printf("Samples:     0 Min: ------ Max: ----- DC: ------- AC: -------");
      if (display_crc)
	printf(" CRC:[ ---- CCITT ---- ARC ---- XMODEM]");
      printf("\t%s\n", inpfil);
      continue;
    }

    /* Final adjustements do statistics */
    stddev_x = sqrt(get_var(rms_x, avg_x, smpno));
    avg_x /= (double)smpno;
    rms_x = sqrt(rms_x/(double)smpno);

    /* Print results */
    if (print_hex)
    {
      us_max = (unsigned short)max_x;
      us_min = (unsigned short)min_x;
      printf("Samples: %5ld Min: %04X Max: %04X ", 
	     smpno, us_min, us_max);
    }
    else
      printf("Samples: %5ld Min: %5.0f Max: %05.0f ", 
	     smpno, min_x, max_x);

    printf("DC: %7.2f AC: %7.2f", avg_x, stddev_x);
    if (display_crc)
      printf(" CRC:[ %04X CCITT %04X ARC %04X XMODEM]", crc_c, crc_a, crc_x);
    printf("\t%s\n", inpfil);

    /* Close file */
    fclose(Fi);
  }
  
  /* EXITING */
  free(samples);
#ifndef VMS
  exit(0);
#endif
}

