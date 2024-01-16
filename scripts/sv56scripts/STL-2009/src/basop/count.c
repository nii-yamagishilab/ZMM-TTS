/*
  ===========================================================================
   File: COUNT.C                                         v.2.3 - 30.Nov.2009
  ===========================================================================

            ITU-T   STL   BASIC   OPERATORS

            COMPLEXITY EVALUATION FUNCTIONS

   History:
   03 Nov 04   v2.0     Incorporation of new 32-bit / 40-bit / control
                        operators for the ITU-T Standard Tool Library as 
                        described in Geneva, 20-30 January 2004 WP 3/16 Q10/16
                        TD 11 document and subsequent discussions on the
                        wp3audio@yahoogroups.com email reflector.
                        norm_s()      weight reduced from 15 to 1.
                        norm_l()      weight reduced from 30 to 1.
                        L_abs()       weight reduced from  2 to 1.
                        L_add()       weight reduced from  2 to 1.
                        L_negate()    weight reduced from  2 to 1.
                        L_shl()       weight reduced from  2 to 1.
                        L_shr()       weight reduced from  2 to 1.
                        L_sub()       weight reduced from  2 to 1.
                        mac_r()       weight reduced from  2 to 1.
                        msu_r()       weight reduced from  2 to 1.
                        mult_r()      weight reduced from  2 to 1.
                        L_deposit_h() weight reduced from  2 to 1.
                        L_deposit_l() weight reduced from  2 to 1.
   March 06    v2.1     Changed to improve portability.
   Dec 06      v2.2     Changed to specify frame rate using setFrameRate()
                        Adding WMOPS_output_avg() for global average computation
                        L_mls() weight of 5.
                        div_l() weight of 32.
                        i_mult() weight of 3.
  ============================================================================
*/


/*****************************************************************************
 *
 * This file contains functions for the automatic complexity calculation
 *
 *****************************************************************************/


#include <stdlib.h> 
#include <stdio.h>
#include <string.h> 
#include "stl.h"

#ifdef WMOPS
static double frameRate = FRAME_RATE; /* default value : 10 ms */ 
#endif /* ifdef WMOPS */

#ifdef WMOPS
/* Global counter variable for calculation of complexity weight */
BASIC_OP multiCounter[MAXCOUNTERS];
int currCounter=0; /* Zero equals global counter */
#endif /* ifdef WMOPS */

#ifdef WMOPS
void setFrameRate(int samplingFreq, int frameLength)
{
	if(frameLength > 0)
	{
		frameRate = samplingFreq / 1000000.0 / frameLength;
	}
	return;
}

#endif /* ifdef WMOPS */

#ifdef WMOPS
/*
 * Below list is used for displaying the code profiling information in
 * the file which name is defined by CODE_PROFILE_FILENAME.
 * For further details see generic_WMOPS_output() function.
 * Attention, the ordering in this table must be kept in synchronisation
 * with the structure definition BASIC_OP.
 */
char* BasicOperationList[] =
{
   "add",           "sub",             "abs_s",         "shl",             "shr",
   "extract_h",     "extract_l",       "mult",          "L_mult",          "negate",
   "round",         "L_mac",           "L_msu",         "L_macNs",         "L_msuNs",
   "L_add",         "L_sub",           "L_add_c",       "L_sub_c",         "L_negate",
   "L_shl",         "L_shr",           "mult_r",        "shr_r",           "mac_r",

   "msu_r",         "L_deposit_h",     "L_deposit_l",   "L_shr_r",         "L_abs",
   "L_sat",         "norm_s",          "div_s",         "norm_l",          "move16",
   "move32",        "Logic16",         "Logic32",       "Test",            "s_max",
   "s_min",         "L_max",           "L_min",         "L40_max",         "L40_min",
   "shl_r",         "L_shl_r",         "L40_shr_r",     "L40_shl_r",       "norm_L40",

   "L40_shl",       "L40_shr",         "L40_negate",    "L40_add",         "L40_sub",
   "L40_abs",       "L40_mult",        "L40_mac",       "mac_r40",
   "L40_msu",       "msu_r40",         "Mpy_32_16_ss",  "Mpy_32_32_ss",    "L_mult0",
   "L_mac0",        "L_msu0",          "lshl",          "lshr",            "L_lshl",
   "L_lshr",        "L40_lshl",        "L40_lshr",      "s_and",           "s_or",

   "s_xor",         "L_and",           "L_or",          "L_xor",           "rotl",
   "rotr",          "L_rotl",          "L_rotr",        "L40_set",         "L40_deposit_h",
   "L40_deposit_l", "L40_deposit32",   "Extract40_H",   "Extract40_L",     "L_Extract40",
   "L40_round",     "L_saturate40",    "round40",       "IF",              "GOTO",
   "BREAK",         "SWITCH",          "FOR",           "WHILE",           "CONTINUE"

 , "L_mls",         "div_l",           "i_mult"
};
#endif /* ifdef WMOPS */


#ifdef WMOPS
const BASIC_OP op_weight =
{
    1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,
    1,     1,     2,     2,     1,
    1,     1,     1,     3,     1,

    1,     1,     1,     3,     1,
    4,     1,     18,    1,     1,
    2,     1,     2,     2,     1,
    1,     1,     1,     1,     1,
    3,     3,     3,     3,     1,

    1,     1,     1,     1,     1,
    1,     1,     1,     2,
    1,     2,     2,     4,     1,
    1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,

    1,     1,     1,     1,     3,
    3,     3,     3,     3,     1,
    1,     1,     1,     1,     1,
    1,     1,     1,     4,     4,
    4,     8,     3,     4,     4
    
  , 5,     32,    3
};
#endif /* ifdef WMOPS */


Word32 TotalWeightedOperation (void);
Word32 DeltaWeightedOperation (void);


#ifdef WMOPS
/* Counters for separating counting for different objects */


static int maxCounter=0;
static char* objectName[MAXCOUNTERS+1];

static Word16 fwc_corr[MAXCOUNTERS+1];
static long int nbTimeObjectIsCalled[MAXCOUNTERS+1];

#define NbFuncMax  1024

static Word16 funcid[MAXCOUNTERS], nbframe[MAXCOUNTERS];
static Word32 glob_wc[MAXCOUNTERS], wc[MAXCOUNTERS][NbFuncMax];
static float total_wmops[MAXCOUNTERS];

static Word32 LastWOper[MAXCOUNTERS];
#endif /* ifdef WMOPS */


#ifdef WMOPS
static char* my_strdup(const char *s) {
/*
 * duplicates UNIX function strdup() which is not ANSI standard:
 * -- malloc() memory area big enough to hold the string s
 * -- copy string into new area
 * -- return pointer to new area
 *
 * returns NULL if either s==NULL or malloc() fails
 */
    char *dup;
    
    if (s == NULL)
        return NULL;

    /* allocate memory for copy of ID string (including string terminator) */
    /* NOTE: the ID strings will never be deallocated because there is no
             way to "destroy" a counter that is not longer needed          */
    if ((dup = (char *) malloc(strlen(s)+1)) == NULL)
        return NULL;

    return strcpy(dup, s);
}
#endif /* ifdef WMOPS */


int getCounterId( char *objectNameArg) {
#if WMOPS
  if(maxCounter>=MAXCOUNTERS-1) return 0;
  objectName[++maxCounter]=my_strdup(objectNameArg);
  return maxCounter;

#else /* ifdef WMOPS */
  return 0; /* Dummy */

#endif /* ifdef WMOPS */
}


#if WMOPS
int readCounterId() {
   return currCounter;
}
#endif /* ifdef WMOPS */


#ifdef WMOPS
char * readCounterIdName() {
   return objectName[currCounter];
}
#endif /* ifdef WMOPS */

void setCounter( int counterId) {
#if WMOPS
   if( (counterId > maxCounter)
    || (counterId < 0)) {
      currCounter=0;
      return;
   }
   currCounter=counterId;
   call_occurred = 1;
#endif /* ifdef WMOPS */
}


void incrementNbTimeObjectIsCalled( int counterId) {
#if WMOPS
  if( (counterId > maxCounter)
   || (counterId < 0)) {
      nbTimeObjectIsCalled[0]++;
      return;
    }
  nbTimeObjectIsCalled[counterId]++;
#endif /* ifdef WMOPS */
}


#if WMOPS
static Word32 WMOPS_frameStat() {
/* calculate the WMOPS seen so far and update the global
   per-frame maximum (glob_wc)
 */
    Word32 tot;

    tot = TotalWeightedOperation ();
    if (tot > glob_wc[currCounter])
        glob_wc[currCounter] = tot;

    /* check if fwc() was forgotten at end of last frame */
    if (tot > LastWOper[currCounter]) {
        if (!fwc_corr[currCounter]) {
            fprintf(stderr,
                    "count: operations counted after last fwc() for '%s'; "
                    "-> fwc() called\n",
                    objectName[currCounter]?objectName[currCounter]:"");
        }
        fwc();
    }
    
    return tot;
}
#endif /* ifdef WMOPS */


#ifdef WMOPS
static void WMOPS_clearMultiCounter() {
    Word16 i;
    
    Word32 *ptr = (Word32 *) &multiCounter[currCounter];
    for( i = 0; i < (sizeof (multiCounter[currCounter])/ sizeof (Word32)); i++) {
        *ptr++ = 0;
    }
}
#endif /* ifdef WMOPS */


void ClearNbTimeObjectsAreCalled() {
#if WMOPS		
    Word16 i;
    
    for (i = 0; i < (sizeof (multiCounter[currCounter])/ sizeof (Word32)); i++) {
		nbTimeObjectIsCalled[i] = 0;
    }
#endif /* ifdef WMOPS */
}

Word32 TotalWeightedOperation () {
#if WMOPS
    Word16 i;
    Word32 tot, *ptr, *ptr2;

    tot = 0;
    ptr = (Word32 *) &multiCounter[currCounter];
    ptr2 = (Word32 *) &op_weight;
    for (i = 0; i < (sizeof (multiCounter[currCounter])/ sizeof (Word32)); i++) {
        tot += ((*ptr++) * (*ptr2++));
    }

    return ((Word32) tot);

#else /* ifdef WMOPS */
    return 0; /* Dummy */

#endif /* ifdef WMOPS */

}

Word32 DeltaWeightedOperation () {
#if WMOPS
    Word32 NewWOper, delta;

    NewWOper = TotalWeightedOperation ();
    delta = NewWOper - LastWOper[currCounter];
    LastWOper[currCounter] = NewWOper;
    return (delta);

#else /* ifdef WMOPS */
    return 0; /* Dummy */

#endif /* ifdef WMOPS */
}


void Init_WMOPS_counter (void) {
#if WMOPS
    Word16 i;

    /* reset function weight operation counter variable */

    for (i = 0; i < NbFuncMax; i++)
        wc[currCounter][i] = (Word32) 0;
    glob_wc[currCounter] = 0;
    nbframe[currCounter] = 0;
    total_wmops[currCounter] = 0.0;

    /* initially clear all counters */
    WMOPS_clearMultiCounter();
    LastWOper[currCounter] = 0;
    funcid[currCounter] = 0;

    /* Following line is useful for incrIf(), see control.h */
    call_occurred = 1;
    funcId_where_last_call_to_else_occurred=MAXCOUNTERS;
#endif /* ifdef WMOPS */
}


void Reset_WMOPS_counter (void) {
#if WMOPS
    Word32 tot = WMOPS_frameStat();
        
    /* increase the frame counter --> a frame is counted WHEN IT BEGINS */
    nbframe[currCounter]++;
    /* add wmops used in last frame to count, then reset counter */
    /* (in first frame, this is a no-op                          */
    total_wmops[currCounter] += (float)( tot * frameRate );
    
    /* clear counter before new frame starts */
    WMOPS_clearMultiCounter();
    LastWOper[currCounter] = 0;
    funcid[currCounter] = 0;           /* new frame, set function id to zero */
#endif /* ifdef WMOPS */
}


Word32 fwc (void) {
/* function worst case */
#if WMOPS
    Word32 tot;

    tot = DeltaWeightedOperation ();
    if (tot > wc[currCounter][funcid[currCounter]])
        wc[currCounter][funcid[currCounter]] = tot;

    funcid[currCounter]++;
    return (tot);

#else /* ifdef WMOPS */
    return 0; /* Dummy */

#endif /* ifdef WMOPS */
}

void WMOPS_output (Word16 dtx_mode) {
#if WMOPS
    Word16 i;
    Word32 tot, tot_wm, tot_wc;

    /* get operations since last reset (or init),
       but do not update the counters (except the glob_wc[] maximum)
       so output CAN be called in each frame without problems.
       The frame counter is NOT updated!
     */
    tot = WMOPS_frameStat();
    tot_wm = (Word32)(total_wmops[currCounter] + ((float) tot) * frameRate);

    fprintf (stdout, "%10s:WMOPS=%.3f",
	     objectName[currCounter]?objectName[currCounter]:"",
	     ((float) tot) * frameRate);

    if (nbframe[currCounter] != 0)
    {
        fprintf (stdout, "  Average=%.3f",
                 tot_wm / (float) nbframe[currCounter]);
    }
    fprintf (stdout, "  WorstCase=%.3f",
             ((float) glob_wc[currCounter]) * frameRate);

    /* Worst worst case printed only when not in DTX mode */
    if (dtx_mode == 0)
    {
        tot_wc = 0L;
        for (i = 0; i < funcid[currCounter]; i++)
            tot_wc += wc[currCounter][i];
        fprintf (stdout, "  WorstWC=%.3f", ((float) tot_wc) * frameRate);
    }
    fprintf (stdout, " (%d frames)\n", nbframe[currCounter]);
    
#endif /* ifdef WMOPS */
}


void WMOPS_output_avg (Word16 dtx_mode, Word32 *tot_wm, Word16 *num_frames) {
#if WMOPS
    Word16 i;
    Word32 tot, tot_wc;

    /* get operations since last reset (or init),
       but do not update the counters (except the glob_wc[] maximum)
       so output CAN be called in each frame without problems.
       The frame counter is NOT updated!
     */
    tot = WMOPS_frameStat();
    *tot_wm = (Word32)(total_wmops[currCounter] + ((float) tot) * frameRate);
    *num_frames = nbframe[currCounter];

    fprintf (stdout, "%10s:WMOPS=%.3f",
	     objectName[currCounter]?objectName[currCounter]:"",
	     ((float) tot) * frameRate);

    if (nbframe[currCounter] != 0)
    {
        fprintf (stdout, "  Average=%.3f",
                 *tot_wm / (float) nbframe[currCounter]);
    }
    fprintf (stdout, "  WorstCase=%.3f",
             ((float) glob_wc[currCounter]) * frameRate);

    /* Worst worst case printed only when not in DTX mode */
    if (dtx_mode == 0)
    {
        tot_wc = 0L;
        for (i = 0; i < funcid[currCounter]; i++)
            tot_wc += wc[currCounter][i];
        fprintf (stdout, "  WorstWC=%.3f", ((float) tot_wc) * frameRate);
    }
    fprintf (stdout, " (%d frames)\n", nbframe[currCounter]);
    
#endif /* ifdef WMOPS */
}

void generic_WMOPS_output (Word16 dtx_mode, char *test_file_name)
{
#if WMOPS
   int		saved_value;
   Word16	i;
   Word32	tot, tot_wm, tot_wc, *ptr, *ptr2;
   Word40   grand_total;
   FILE	*WMOPS_file;

   saved_value = currCounter;

   /*Count the grand_total WMOPS so that % ratio per function group
   can be displayed. */
   grand_total = 0;
   for( currCounter = 0; currCounter <= maxCounter; currCounter++) {
      tot = WMOPS_frameStat();
      grand_total += tot;

   }


   if( (WMOPS_file=fopen(WMOPS_DATA_FILENAME,"a"))!=NULL) {

      printf( "opened file %s in order to print WMOPS for each function group.\n", WMOPS_DATA_FILENAME);

      /* Print the file header line. */
      fprintf (WMOPS_file, "Test file name\tFunction Name \tFrame\tNb Times Called\tWMOPS\t%% versus grand total");

      if (nbframe[saved_value] != 0)
         fprintf (WMOPS_file, "\tAverage");

      fprintf (WMOPS_file, "\tWorstCase");

      /* Worst worst case printed only when not in DTX mode */
      if (dtx_mode == 0)
         fprintf (WMOPS_file, "\tWorstWC");

      fprintf (WMOPS_file, "\n");

      /* Print the WMOPS for each Function Group by scanning
         all the function groups with currCounter index.*/
      for( currCounter = 0; currCounter <= maxCounter; currCounter++) {

         fprintf (WMOPS_file, "%s", test_file_name);
         fprintf (WMOPS_file, "\t%s",
                  objectName[currCounter] ? objectName[currCounter] : "");
         fprintf (WMOPS_file, "\t%d", nbframe[currCounter]);

         tot = WMOPS_frameStat();
         tot_wm = (Word32)(total_wmops[currCounter] + ((float) tot) * frameRate);

         fprintf (WMOPS_file, "\t\t%ld", nbTimeObjectIsCalled[currCounter]);
         fprintf (WMOPS_file, "\t%.6f", ((float) tot) * frameRate);
         fprintf (WMOPS_file, "\t%.3f", ((float) tot) / grand_total * 100);

         if (nbframe[currCounter] != 0)
            fprintf (WMOPS_file, "\t%.3f", tot_wm / (float) nbframe[currCounter]);

         fprintf (WMOPS_file, "\t%.3f", ((float) glob_wc[currCounter]) * frameRate);

         /* Worst worst case printed only when not in DTX mode */
         if (dtx_mode == 0) {
            tot_wc = 0L;
            for (i = 0; i < funcid[currCounter]; i++)
               tot_wc += wc[currCounter][i];
            fprintf (WMOPS_file, "\t%.3f", ((float) tot_wc) * frameRate);
         }
         fprintf (WMOPS_file, "\n");

      }

      /* Print the file Grand Total line */
      fprintf (WMOPS_file, "%s", test_file_name);
      fprintf (WMOPS_file, "\tGrand Total");
      fprintf (WMOPS_file, "\t%d", nbframe[saved_value]);
      fprintf (WMOPS_file, "\t\t%.6f", ((float) grand_total) * frameRate);
      fprintf (WMOPS_file, "\t100.000");
      fprintf (WMOPS_file, "\n");
      fclose(WMOPS_file);

   } else
      printf( "Can not open file %s for WMOPS editing\n", WMOPS_DATA_FILENAME);


   if( (WMOPS_file=fopen(WMOPS_TOTAL_FILENAME,"a"))!=NULL) {
      printf( "opened file %s in order to print application's total WMOPS.\n", WMOPS_TOTAL_FILENAME);
      fprintf (WMOPS_file, "%s", test_file_name);
      fprintf (WMOPS_file, "\tframe=%d", nbframe[currCounter]);
      fprintf (WMOPS_file, "\tWMOPS=%.6f", ((float) grand_total) * frameRate);
      fprintf (WMOPS_file, "\n");
      fclose(WMOPS_file);

   } else
      printf( "Can not open file %s for WMOPS editing.\n", WMOPS_TOTAL_FILENAME);


   if( (WMOPS_file=fopen(CODE_PROFILE_FILENAME,"a"))!=NULL) {

      printf( "opened file %s in order to print basic operation distribution statistics.\n", CODE_PROFILE_FILENAME);

      /* Print the file header line. */
      fprintf (WMOPS_file, "Test file name\tBasic Operation Name\tframe\tWMOPS\t\t%% versus grand total\n");

      /* Print the WMOPS for each Basic Operation across all the defined */
      /* Function Groups. */
      for( i = 0; i <(sizeof(op_weight) / sizeof(Word32)); i++) {
         fprintf (WMOPS_file, "%-16s", test_file_name);
         fprintf (WMOPS_file, "\t%s", BasicOperationList[i]);
         fprintf (WMOPS_file, "\t%d", nbframe[0]);

         tot = 0;
         ptr = (Word32 *) &multiCounter[0] + i;
         ptr2 = (Word32 *) &op_weight + i;
         for( currCounter = 0; currCounter <= maxCounter; currCounter++) {
            tot += ((*ptr) * (*ptr2));
            ptr += (sizeof(op_weight) / sizeof(Word32));
         }

         fprintf (WMOPS_file, "\t%.6f", ((float) tot) * frameRate);
         fprintf (WMOPS_file, "\t%.3f", ((float) tot) / grand_total * 100);
         fprintf (WMOPS_file, "\n");
      }

      /* Print the file Grand Total line */
      fprintf (WMOPS_file, "%s", test_file_name);
      fprintf (WMOPS_file, "\tGrand Total");
      fprintf (WMOPS_file, "\t%d", nbframe[saved_value]);
      fprintf (WMOPS_file, "\t%.6f", ((float) grand_total) * frameRate);
      fprintf (WMOPS_file, "\t100.000");
      fprintf (WMOPS_file, "\n");
      fclose(WMOPS_file);

   } else
      printf( "Can not open file %s for basic operations distribution statistic editing\n", CODE_PROFILE_FILENAME);

   currCounter = saved_value;

#endif /* ifdef WMOPS */
}


/* end of file */

