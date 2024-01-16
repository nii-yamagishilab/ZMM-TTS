/*
 ============================================================================
   File: EID_IO.H                                                   22.FEB.96
 ============================================================================
 
		  UGST/ITU-T ERROR INSERTION MODULE
 
	  PROTOTYPES FOR THE STATE VARIABLE FILE I/O FUNCTIONS
 
   AUTHOR:
   ~~~~~~~ 
   Gerhard Schroeder
   Deutsche Bundespost TELEKOM    Tel +49 6151 833973  
   Postfach 100003                FAX +49 6151 895234
   64276 Darmstadt                Email: gerhard.schroeder@ties.itu.ch
   Germany
  
   HISTORY:
   ~~~~~~~~
   29.Jul.93 v1.0  Created <gerhard.schroeder@ties.itu.ch>
   22.Feb.96 v1.1  Inserted smart prototypes <simao@ctd.comsat.com>
 ============================================================================
*/
#ifndef EID_IO_defined
#define EID_IO_defined 110

/* ......... Smart prototypes .......... */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(x) x
#else
#define ARGS(x) ()
#endif
#endif

/* Definitions for G.192 mode */
#define G192_ZERO       (short)0X007F
#define G192_ONE	(short)0X0081
#define G192_SYNC	(short)0x6B21
#define G192_FER	(short)0x6B20

/* 
 * ... Local function prototypes for saving/retrieving EID states in file ...
 */
long save_EID_to_file ARGS((SCD_EID *EID, char *EIDfile, double ber,
			    double gamma));
long save_burst_eid_to_file ARGS((BURST_EID *burst_eid, char *state_file));
SCD_EID *recall_eid_from_file ARGS((char *EIDfile, double *ber,
			    double *gamma));
BURST_EID *recall_burst_eid_from_file ARGS((char *state_file, long index));
long READ_L ARGS((FILE *fp, long n, long *longary));
long READ_lf ARGS((FILE *fp, long n, double *doubleary));
long READ_c ARGS((FILE *fp, long n, char *chr));

#endif
/* ........................... End of EID.H ........................... */
