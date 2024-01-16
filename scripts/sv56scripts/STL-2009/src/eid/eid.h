/*
  ============================================================================
   File: EID.H                                                      11.AUG.97
  ============================================================================

                      UGST/ITU-T ERROR INSERTION MODULE

                          GLOBAL FUNCTION PROTOTYPES

   History:
   28.Feb.92	v1.0	First version <hf@pkinbg.uucp>
   20.Apr.92	v2.0	Changes due to eid.c v2.0 <hf@pkinbg.uucp>
   30.Apr.92	v2.2	Update from UGST formats <tdsimao@cpqd.ansp.br>
   04.Aug.93	v2.3	Add FER_generator_random and Bellcore EID 
   11.Aug.97    v2.31   Updated smart prototype list as per 
                        <Morgan.Lindqvist@era-t.ericsson.se> comments for the
		        cc compiler in a DEC Alpha Unix machine.
   10.Oct.97    v2.4    Added prototype for reset_burst_eid() <simao>   
  ============================================================================
*/
 
#include <math.h>               /* RTL Math Function Declarations */
#include <stdlib.h>             /* General utility definitions */

#ifndef EID_defined
#define EID_defined 300

/* ......... Smart prototypes .......... */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(x) x
#else
#define ARGS(x) ()
#endif
#endif

#define MODEL_SIZE 11
#define MODEL_NUMBER 5

#if defined(VMS) | defined(MSDOS) | defined (__MSDOS__) | defined (__STDC__)
#define CONST const
#else
#define CONST static
#endif

/* 
 * ......... Structure for the channel model ......... 
 */
typedef struct {
        unsigned long seed;             /* seed for random no.generator */
        long nstates;                   /* number of states of the channel */
                                        /* model */
        long current_state;             /* index of current channel state */
        double *ber;                    /* bit error rate in each state */
        double **matrix;                /* matrix containing the probabilit. */
                                        /* for changing from one state to */
                                        /* another one */
        double usrber;                  /* user defined bit error rate */
        double usrgamma;                /* user defined correlation factor */
               } SCD_EID;

typedef struct {
	unsigned long	seedptr;
	long	internal[MODEL_SIZE];
	long	s_new;
	long	index;
	} BURST_EID;

/* 
 * ......... Definitions for G.192-compliant or non-compliant  ......... 
 * ......... handling of soft bitstreams (post- and pre-STL92) ......... 
 */
#ifdef STL92 /* Non-compliant */
# define BER_insertion BER_insertion_stl92
# define FER_module FER_module_stl92
#else /* Compliant */
# define BER_insertion BER_insertion_stl96
# define FER_module FER_module_stl96
#endif

/* 
 * ......... Global function prototypes ......... 
 */
SCD_EID         *open_eid ARGS((double ber, double gamma));
BURST_EID	*open_burst_eid ARGS((long index));
void            set_RAN_seed ARGS((SCD_EID *EID, unsigned long seed));
unsigned long   get_RAN_seed ARGS((SCD_EID *EID));
void            set_GEC_matrix ARGS((SCD_EID *EID, double threshold, 
                               int /* char */ current_state, int /* char */ next_state));
double          get_GEC_matrix ARGS((SCD_EID *EID, int /* char */ current_state, 
                               int /* char */ next_state));
void            set_GEC_current_state ARGS((SCD_EID *EID, int /* char */ current_state));
char            get_GEC_current_state ARGS((SCD_EID *EID));
void            BER_insertion ARGS((long lseg, short *xbuff, short *ybuff, 
                               short *error_pattern));
double          BER_generator ARGS((SCD_EID *EID, long lseg, short *EPbuff));
double          FER_generator_random ARGS((SCD_EID *EID));
double          FER_generator_burst ARGS((BURST_EID *state));
double          FER_module ARGS((SCD_EID *EID, long lseg, short *xbuff, 
                                short *ybuff));
double 		FER_generator_burst ARGS((BURST_EID *state));
BURST_EID      *reset_burst_eid ARGS((BURST_EID *burst_eid));
#endif
/* ........................... End of EID.H ........................... */
