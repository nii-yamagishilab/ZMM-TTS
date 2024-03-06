/*
  ===========================================================================
   File: COUNT.H                                         v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T         STL         BASIC       OPERATORS

            PROTOTYPES & DEFINITION FOR COUNTING OPERATIONS

   History
   09.Aug.1999 V1.0.0   Input to UGST from ETSI AMR (count.h); 

   26.Jan.2000 V1.1.0   Added counter entries for G.723.1's 
                        L_mls(), div_l(), i_mult() [from basop32.c]

   05.Jul.2000 V1.2.0   Added counter entries for 32bit shiftless 
                        operators L_mult0(), L_mac0(), L_msu0()

   03 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
   March 06    v2.1     Changed to improve portability.
   Dec 06      v2.2     Changed to specify frame rate using setFrameRate()
                        Adding WMOPS_output_avg() for global average computation
                        L_mls() weight of 5.
                        div_l() weight of 32.
                        i_mult() weight of 3.
  ============================================================================
*/


/******************************************************************************
 * Functions for counting operations.
 *
 * These functions make it possible to measure the wMOPS of a codec.
 *
 * All functions in this file, and in other *.h files, update a structure so
 * that it will be possible to track how many calls to add(), sub(), L_mult()
 * ... was made by the code and to estimate the wMOPS (and MIPS) for a certain
 * part of the code.
 *
 * It is also possible to measure the wMOPS separatly for different parts
 * of the codec.
 *
 * This is done by creating a counter group (getCounterId) for each part of
 * the code that one wants a seperate measure for. Before a part of the code
 * is executed a call to the "setCounter" function is needed to identify
 * which counter group to use.
 *
 * Currently there is a limit of 255 different counter groups.
 *
 * In the end of this file, there is a piece of code illustrating how the
 * functions can be used.
 *
******************************************************************************/


#ifndef _COUNT_H
#define _COUNT_H "$Id$"
#define WMOPS 1		    /* enable WMOPS profiling features  */
/* #undef WMOPS	*/		/* disable WMOPS profiling features */
#define MAXCOUNTERS (256)

int getCounterId( char *objectName);
/*
 * Create a counter group, the "objectname" will be used when printing
 * statistics for this counter group.
 *
 * Returns 0 if no more counter groups are available.
 */


int readCounterId();
/*
 * Returns the current CounterId.
 */


void setCounter( int counterId);
/*
 * Defines which counter group to use, default is zero.
 */


char *readCounterIdName();
/*
 * Returns the current CounterId name.
 */


void incrementNbTimeObjectIsCalled( int counterId);
/*
 * This function enables to increment by 1 a counter
 * tracking the number of times the application enters a groups of functions.
 * If the counterId is not refering to a defined function counter group, then it is
 * the default function group (0) which is impacted.
 *
 */


void ClearNbTimeObjectsAreCalled();
/*
 * This function enables to clear to 0 all the counters enabling to
 * track the number of times the application enters any groups of functions.
 */


void Init_WMOPS_counter( void);
/*
 * Initiates the current counter group.
 */


void Reset_WMOPS_counter( void);
/*
 * Resets the current counter group.
 */


void WMOPS_output ( Word16 notPrintWorstWorstCase);
/*
 * Prints the statistics to the screen, if the argument is non zero
 * the statistics for worst worst case will not be printed. This is typically
 * done for dtx frames.
 *
 */

void WMOPS_output_avg (Word16 dtx_mode, Word32 *tot_wm, Word16 *num_frames);
/*
 * same as WMOPS_output + returns the total wmops counter and the number of frames 
 * to support the computation of global average.
 *
 */


Word32 fwc( void);
/*
 * worst worst case counter.
 *
 * This function calculates the worst possible case that can be reached.
 *
 * This is done by calling this function for each subpart of the calculations
 * for a frame. This function then stores the maximum wMOPS for each part.
 *
 * The WMOPS_output function add together all parts and presents the sum.
 */

void setFrameRate(int samplingFreq, int frameLength);
/*
 * This function can overwrite the value of the frameRate variable that is 
 * initialized by the FRAME_RATE constant.
 */


#define WMOPS_DATA_FILENAME	"wmops_data.txt"
/*
 * WMOPS_DATA_FILENAME is the macro defining the name of the file
 * where the Weighted Million of Operations per Second (wMOPS)
 * are appended, function group by function group.
*/


#define CODE_PROFILE_FILENAME	"code_profile.txt"
/*
 * CODE_PROFILE_FILENAME is the macro defining the name of the file
 * where the Weighted Million of Operations per Second (WMOPS)
 * are appended, basic operation by basic operation.
*/


#define WMOPS_TOTAL_FILENAME	"wmops_total.txt"
/*
 * WMOPS_TOTAL_FILENAME is the macro defining the name of the file
 * where the Weighted Million of Operations per Second (WMOPS)
 * are printed, globally for the application.
*/


#define FRAME_RATE	(0.0001F) /*in this version frame_rate can be overwriten online by the new setFrameRate function */
/* FRAME_RATE of 0.000025 is corresponding to 40ms frame.*/
/* FRAME_RATE of 0.00005 is corresponding to 20ms frame.*/
/* FRAME_RATE of 0.0001 is corresponding to 10ms frame.*/
/*
 * FRAME_RATE is the macro defining the calling rate of the
 * application to benchmark.
*/


/* Global counter variable for calculation of complexity weight */
typedef struct
{
    UWord32 add;             /* Complexity Weight of 1 */
    UWord32 sub;             /* Complexity Weight of 1 */
    UWord32 abs_s;           /* Complexity Weight of 1 */
    UWord32 shl;             /* Complexity Weight of 1 */
    UWord32 shr;             /* Complexity Weight of 1 */

    UWord32 extract_h;       /* Complexity Weight of 1 */
    UWord32 extract_l;       /* Complexity Weight of 1 */
    UWord32 mult;            /* Complexity Weight of 1 */
    UWord32 L_mult;          /* Complexity Weight of 1 */
    UWord32 negate;          /* Complexity Weight of 1 */

    UWord32 round;           /* Complexity Weight of 1 */
    UWord32 L_mac;           /* Complexity Weight of 1 */
    UWord32 L_msu;           /* Complexity Weight of 1 */
    UWord32 L_macNs;         /* Complexity Weight of 1 */
    UWord32 L_msuNs;         /* Complexity Weight of 1 */

    UWord32 L_add;           /* Complexity Weight of 1 */
    UWord32 L_sub;           /* Complexity Weight of 1 */
    UWord32 L_add_c;         /* Complexity Weight of 2 */
    UWord32 L_sub_c;         /* Complexity Weight of 2 */
    UWord32 L_negate;        /* Complexity Weight of 1 */

    UWord32 L_shl;           /* Complexity Weight of 1 */
    UWord32 L_shr;           /* Complexity Weight of 1 */
    UWord32 mult_r;          /* Complexity Weight of 1 */
    UWord32 shr_r;           /* Complexity Weight of 3 */
    UWord32 mac_r;           /* Complexity Weight of 1 */

    UWord32 msu_r;           /* Complexity Weight of 1 */
    UWord32 L_deposit_h;     /* Complexity Weight of 1 */
    UWord32 L_deposit_l;     /* Complexity Weight of 1 */
    UWord32 L_shr_r;         /* Complexity Weight of 3 */
    UWord32 L_abs;           /* Complexity Weight of 1 */

    UWord32 L_sat;           /* Complexity Weight of 4 */
    UWord32 norm_s;          /* Complexity Weight of 1 */
    UWord32 div_s;           /* Complexity Weight of 18 */
    UWord32 norm_l;          /* Complexity Weight of 1 */
    UWord32 move16;          /* Complexity Weight of 1 */

    UWord32 move32;          /* Complexity Weight of 2 */
    UWord32 Logic16;         /* Complexity Weight of 1 */
    UWord32 Logic32;         /* Complexity Weight of 2 */
    UWord32 Test;            /* Complexity Weight of 2 */
    UWord32 s_max;           /* Complexity Weight of 1 */

    UWord32 s_min;           /* Complexity Weight of 1 */
    UWord32 L_max;           /* Complexity Weight of 1 */
    UWord32 L_min;           /* Complexity Weight of 1 */
    UWord32 L40_max;         /* Complexity Weight of 1 */
    UWord32 L40_min;         /* Complexity Weight of 1 */

    UWord32 shl_r;           /* Complexity Weight of 3 */
    UWord32 L_shl_r;         /* Complexity Weight of 3 */
    UWord32 L40_shr_r;       /* Complexity Weight of 3 */
    UWord32 L40_shl_r;       /* Complexity Weight of 3 */
    UWord32 norm_L40;        /* Complexity Weight of 1 */

    UWord32 L40_shl;         /* Complexity Weight of 1 */
    UWord32 L40_shr;         /* Complexity Weight of 1 */
    UWord32 L40_negate;      /* Complexity Weight of 1 */
    UWord32 L40_add;         /* Complexity Weight of 1 */
    UWord32 L40_sub;         /* Complexity Weight of 1 */

    UWord32 L40_abs;         /* Complexity Weight of 1 */
    UWord32 L40_mult;        /* Complexity Weight of 1 */
    UWord32 L40_mac;         /* Complexity Weight of 1 */
    UWord32 mac_r40;         /* Complexity Weight of 2 */

    UWord32 L40_msu;         /* Complexity Weight of 1 */
    UWord32 msu_r40;         /* Complexity Weight of 2 */
    UWord32 Mpy_32_16_ss;    /* Complexity Weight of 2 */
    UWord32 Mpy_32_32_ss;    /* Complexity Weight of 4 */
    UWord32 L_mult0;         /* Complexity Weight of 1 */

    UWord32 L_mac0;          /* Complexity Weight of 1 */
    UWord32 L_msu0;          /* Complexity Weight of 1 */
    UWord32 lshl;            /* Complexity Weight of 1 */ 
    UWord32 lshr;            /* Complexity Weight of 1 */
    UWord32 L_lshl;          /* Complexity Weight of 1 */ 

    UWord32 L_lshr;          /* Complexity Weight of 1 */
    UWord32 L40_lshl;        /* Complexity Weight of 1 */
    UWord32 L40_lshr;        /* Complexity Weight of 1 */
    UWord32 s_and;           /* Complexity Weight of 1 */
    UWord32 s_or;            /* Complexity Weight of 1 */

    UWord32 s_xor;           /* Complexity Weight of 1 */
    UWord32 L_and;           /* Complexity Weight of 1 */
    UWord32 L_or;            /* Complexity Weight of 1 */
    UWord32 L_xor;           /* Complexity Weight of 1 */
    UWord32 rotl;            /* Complexity Weight of 3 */

    UWord32 rotr;            /* Complexity Weight of 3 */
    UWord32 L_rotl;          /* Complexity Weight of 3 */
    UWord32 L_rotr;          /* Complexity Weight of 3 */
    UWord32 L40_set;         /* Complexity Weight of 3 */
    UWord32 L40_deposit_h;   /* Complexity Weight of 1 */

    UWord32 L40_deposit_l;   /* Complexity Weight of 1 */
    UWord32 L40_deposit32;   /* Complexity Weight of 1 */
    UWord32 Extract40_H;     /* Complexity Weight of 1 */
    UWord32 Extract40_L;     /* Complexity Weight of 1 */
    UWord32 L_Extract40;     /* Complexity Weight of 1 */

    UWord32 L40_round;       /* Complexity Weight of 1 */
    UWord32 L_saturate40;    /* Complexity Weight of 1 */
    UWord32 round40;         /* Complexity Weight of 1 */
    UWord32 If;              /* Complexity Weight of 4 */
    UWord32 Goto;            /* Complexity Weight of 4 */

    UWord32 Break;           /* Complexity Weight of 4 */
    UWord32 Switch;          /* Complexity Weight of 8 */
    UWord32 For;             /* Complexity Weight of 3 */
    UWord32 While;           /* Complexity Weight of 4 */
    UWord32 Continue;        /* Complexity Weight of 4 */

    UWord32 L_mls;           /* Complexity Weight of 6 */
    UWord32 div_l;           /* Complexity Weight of 32 */
    UWord32 i_mult;          /* Complexity Weight of 3 */
}
BASIC_OP;


Word32 TotalWeightedOperation( void);
Word32 DeltaWeightedOperation( void);


void generic_WMOPS_output ( Word16 notPrintWorstWorstCase, char *test_file_name);
/*
 * This function enable to append :
 * - to WMOPS_DATA_FILENAME file, the WMOPS information related
 *   to the execution of the application, function group by function
 *   group.
 * - to CODE_PROFILE_FILENAME file, the WMOPS information related
 *   to the execution of the application, basic operation by basic
 *   operation.
 * - to WMOPS_TOTAL_FILENAME file, the total WMOPS information related
 *   to the execution of the application.
 *
 * Note that :
 * if the files exists, the data will be written at the end of file.
 *
 * test_file_name : Is a character string referencing each calls to
 *                  generic_WMOPS_output(). Usually, it is the name of a test
 *                  sequence file.
 *
 * notPrintWorstWorstCase : Same usage as in WMOPS_output().
 */


#if 0
/*
 * Example of how count.h could be used.
 *
 * In the example below it is assumed that the init_OBJECT functions
 * does not use any calls to counter.h or basic_op.h. If this is the case
 * a call to the function Reset_WMOPS_counter() must be done after each call
 * to init_OBJECT if these operations is not to be included in the statistics.
 */

int main(){ 
 int spe1Id,spe2Id,cheId;
 
 /* initiate counters and objects */
 spe1Id=getCounterId("Spe 5k8"); 
 setCounter(spe1Id); 
 Init_WMOPS_counter (); 
 init_spe1(...);
 
 spe2Id=getCounterId("Spe 12k2"); 
 setCounter(spe2Id); 
 Init_WMOPS_counter (); 
 init_spe2(...);
 
 cheId=getCounterId("Channel encoder"); 
 setCounter(cheId); 
 Init_WMOPS_counter (); 
 init_che(...); 
 ... 
 while(data){ 
    test();             /* Note this call to test(); */
    if(useSpe1)
        setCounter(spe1Id); 
    else 
        setCounter(spe2Id); 
    Reset_WMOPS_counter();
    speEncode(...);
    WMOPS_output(0);    /* Normal routine for displaying WMOPS info */
    
    setCounter(cheId); 
    Reset_WMOPS_counter();
    preChannelInter(...); fwc(); /* Note the call to fwc() for each part*/
    convolve(...); fwc();        /* of the channel encoder. */
    interleave(...); fwc();
    WMOPS_output(0);    /* Normal routine for displaying WMOPS info */
 }
}
#endif /* #if 0*/


#endif /* _COUNT_H */


/* end of file */
