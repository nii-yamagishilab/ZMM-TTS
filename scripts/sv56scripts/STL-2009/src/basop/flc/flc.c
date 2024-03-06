/*                                  v1.0 (part of basop V.2.3) - Sep/2009 */
/*===================================================================*
*  ITU-T Floating point operation counter STL 2009 tool
*  Software Release 1.0 (2009-09)
*
*      COPYRIGHT NOTE: This source code, and all of its derivations,
*      is subject to the "ITU-T General Public License". Please have
*      it  read  in    the  distribution  disk,   or  in  the  ITU-T
*      Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO 
*      CODING STANDARDS".
*
* The tool was provided by VoiceAge Corporation. VoiceAge Corp. makes no
* representation nor warranty in regard to the accuracy, completeness or
* sufficiency of The Software, nor shall VoiceAge Corp. be held liable
* for any damages whatsoever relating to use of said Software.
*===================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "flc.h"

#ifndef DONT_COUNT

/* printing behavioral defines */
#define FLC_MAXPREFIXLEN  8
#define FLC_MAXTAGLEN     17
#define FLC_OPSCNT_LEN    17
#define FLC_OPCODE_LEN    6
#define FLC_PTR_CMP

/* data structures for storing the data */
struct FLC_Ops_Data
{
   /* list and sublist references */
   struct FLC_Ops_Data *next;
   struct FLC_Ops_Data *subfirst;
   struct FLC_Ops_Data *sublast;
   struct FLC_Ops_Data *parent;
   /* counting data fields */
   char name[FLC_MAXTAGLEN+1];
   unsigned long invocations;
   double fn_total;
   unsigned long fn_max;
   unsigned long op_cnt;
   unsigned long optable[FLC_OPEND];
};

struct FLC_Mem_Data
{
   struct FLC_Mem_Data *next;
   int ref;
   int type;
   int count;
   char *context;
};

typedef struct FLC_Ops_Data FLC_Ops_Data;
typedef struct FLC_Mem_Data FLC_Mem_Data;

static char *Op_Names[FLC_OPEND] = {
   "NOP",
   "ADD", "MULT", "MAC",
   "MOVE", "STORE", "LOGIC",
   "SHIFT", "BRNCH", "DIV",
   "SQRT", "TRANC", "FUNC",
   "LOOP", "IND", "PTR",
   "MISC", "TEST",
   "POW", "LOG"
};
static int Mem_Weights[FLC_OPEND] = {
   0,
   1, 1, 1,                     /*   "ADD", "MULT", "MAC",      */
   1, 0, 1,                     /*   "MOVE", "STORE", "LOGIC",  */
   1, 2, 2,                     /*   "SHIFT", "BRNCH", "DIV",   */
   2, 2, 2,                     /*   "SQRT", "TRANC", "FUNC",   */
   1, 2, 1,                     /*   "LOOP", "IND", "PTR",      */
   1, 1,                        /*   "MISC", "TEST",            */
   2, 2                         /*   "POW", "LOG"               */
};
static int Ops_Weights[FLC_OPEND] = {
   0,
   1, 1, 1,                     /*   "ADD", "MULT", "MAC",      */
   1, 1, 1,                     /*   "MOVE", "STORE", "LOGIC",  */
   1, 4, 18,                    /*   "SHIFT", "BRNCH", "DIV",   */
   10, 25, 2,                   /*   "SQRT", "TRANC", "FUNC",   */
   3, 2, 1,                     /*   "LOOP", "IND", "PTR",      */
   1, 2,                        /*   "MISC", "TEST",            */
   25, 25                       /*   "POW", "LOG"               */
};

/* This structure is only used for printing the memory sums. */
struct FLC_Mem_Summaries
{
   struct FLC_Mem_Summaries *next;
   char *name;
   int counts[FLC_OPEND];
};
typedef struct FLC_Mem_Summaries FLC_Mem_Summaries;
static FLC_Mem_Summaries *FLC_Mem_find_function(FLC_Mem_Summaries *top,
                                                char *name);

/* global variables */
FLC_Ops_Data *ops_top;
FLC_Ops_Data *ops_current;
FLC_Ops_Data *ops_total;
FLC_Mem_Data *mem_top;
FILE *FLC_output;
unsigned long totals[FLC_OPEND];

static FLC_Ops_Data *search_Ops_list(FLC_Ops_Data *top, char *name);

static void FLC_Ops_header();
static void FLC_Ops_print(char *prefix, FLC_Ops_Data *pL);
static void FLC_Ops_line(char *p, FLC_Ops_Data *pL);

static void FLC_Mem_print();

static void FLC_print_sum();
static void FLC_print_frame_sum();

/*
 * FLC initialisation. Must be called at start, sets up all the
 * structures and resets counters.
 *
 * Called externally.
 */
void FLC_init()
{
   FLC_output = stderr;

   ops_top = (FLC_Ops_Data *) malloc(sizeof(struct FLC_Ops_Data));
   if (!ops_top)
      perror("Allocation mem (ops_top) for flc");
   ops_total = (FLC_Ops_Data *) malloc(sizeof(struct FLC_Ops_Data));
   if (!ops_total)
      perror("Allocation mem (ops_total) for flc");

   ops_current = ops_top;

   /* clear the structures */
   memset(ops_current, 0, sizeof(struct FLC_Ops_Data));
   strcpy(ops_current->name, "ROOT");
   ops_current->invocations++;
   memset(ops_total, 0, sizeof(struct FLC_Ops_Data));
   strcpy(ops_total->name, "TOTAL");
   ops_total->invocations = 0;  /* this field is actually the frame count */

   /* the mem count structure is initialised a NULL pointer */
   mem_top = NULL;
}

/*
 * FLC finishing routine.  Adds up totals, prints results.
 *
 * Called externally.
 */

static void FLC_free_node(FLC_Ops_Data *node) 
{
   if (node->next)
   {
      FLC_free_node(node->next);
      free(node->next);
   }
   if (node->subfirst)
   {
      FLC_free_node(node->subfirst);
      free(node->subfirst);
   }
}

void FLC_end()
{
   fprintf(FLC_output, "\n===== Call Graph and total ops per function =====\n\n");
   FLC_Ops_header();
   FLC_Ops_print("", ops_top);

   fprintf(FLC_output, "\n===== Program Memory Usage by Function =====\n\n");
   FLC_Mem_print();

   FLC_print_sum();
   if (ops_total->invocations != 0)
   {
      FLC_print_frame_sum();
   }

   FLC_free_node(ops_top);
   free(ops_total);
   free(ops_top);
}

/*
 * FLC Subroutine or subsection start.  This changes the context so
 * that separate counts are kept for each subroutine and subsection.
 *
 * Called externally, must be matched with a FLC_sub_end()
 */

void FLC_sub_start(char *name)
{
   FLC_Ops_Data *newleaf;
   FLC_Ops_Data *temp;

   temp = search_Ops_list(ops_current->subfirst, name);

   if (!temp)
   {

      /* No context of that type in this function found. */

      newleaf = (FLC_Ops_Data *) malloc(sizeof(struct FLC_Ops_Data));
      memset(newleaf, 0, sizeof(struct FLC_Ops_Data));
      if (!newleaf)
         perror("Allocation mem for flc");

      newleaf->next = NULL;
      newleaf->subfirst = NULL;
      newleaf->sublast = NULL;
      newleaf->parent = ops_current;

      if (ops_current->subfirst == NULL)
      {
         ops_current->subfirst = newleaf;
         ops_current->sublast = newleaf;
      }
      else
      {
         ops_current->sublast->next = newleaf;
         ops_current->sublast = newleaf;
      }

      /* now switch contexts to new function */
      ops_current = newleaf;

      /* set the name pointer */
      if (strlen(name) > FLC_MAXTAGLEN)
      {
         perror("The counter name is too long");
         exit(1);
      }
      strncpy(ops_current->name, name, FLC_MAXTAGLEN);
      ops_current->invocations++;

      /* clear some fields */
      ops_current->fn_max = 0;
   }
   else
   {

      /* this context has been previously initialised */

      ops_current = temp;
      ops_current->invocations++;
   }
}

/*
 * Match to the above.  Finishes up a subroutine context,
 * and computes totals
 */

void FLC_sub_end()
{
   if (ops_current->parent == NULL)
   {
      fprintf(stderr, "ERROR: fell off stack in FLC_sub_end!\n");
   }
   else
   {
      /* Handle max counts per subroutine */
      if (ops_current->op_cnt > ops_current->fn_max)
         ops_current->fn_max = ops_current->op_cnt;

      /* add current counts to per-fn total */
      ops_current->fn_total += ops_current->op_cnt;

      /* add the current counts to the total program count */
      ops_total->op_cnt += ops_current->op_cnt;

      /* reset "current" counts */
      ops_current->op_cnt = 0;

      /* finally, switch context back to higher frame */
      ops_current = ops_current->parent;
   }
}

void FLC_frame_update()
{
   ops_total->invocations++;

   if (ops_total->op_cnt > ops_total->fn_max)
      ops_total->fn_max = ops_total->op_cnt;

   ops_total->fn_total += ops_total->op_cnt;

   ops_total->op_cnt = 0;
}

/* the ops count routine */
void FLC_ops(int op, int c)
{
   if (op == FLC_FUNC)
   {
      /* the "FUNC" opcode is special in the handling of the argument */
      ops_current->op_cnt += Ops_Weights[op] + c;
      ops_current->optable[op]++;
   }
   else
   {
      ops_current->op_cnt += Ops_Weights[op] * c;
      ops_current->optable[op] += c;
   }
}

/* the memory count routine */
void FLC_mem(int op, int c)
{
   FLC_Mem_Data *temp;

   /* c is the no. or args */
   if (op == FLC_FUNC)
      c = 1;

   temp = malloc(sizeof(struct FLC_Mem_Data));
   if (!temp)
   {
      perror("Allocating memory for mem counter");
      exit(1);
   }
   temp->next = mem_top;
   temp->count = c;
   temp->type = op;
   temp->context = ops_current->name;

   mem_top = temp;

   return;
}


/**************************************************************************
 * From here, there are only internal (static) functions                  *
 **************************************************************************/

/*
 * search a FLC_Ops_Data list for an item according to name pointer
 */

static FLC_Ops_Data *search_Ops_list(FLC_Ops_Data *top, char *name)
{
   FLC_Ops_Data *pL;

   /* walk through the list until we find the match or fall off the
      end */
   for (pL = top; pL; pL = pL->next)
   {
      /* Note: use one of the two below */
      if (!strncmp(pL->name, name, FLC_MAXTAGLEN )) return pL;
   }

   return pL;
}

/**************************************************************
 * below this are functions for printing the summaries        *
 **************************************************************/

#define FLC_PFIELDS 3
static char *FLC_pfields[FLC_PFIELDS] = {
  "Calls", "Ops", "Ops/Call"
};

/* print the header for the output */
static void FLC_Ops_header()
{
   char formatstring[10];
   int n;

   /* first, print a header for the tags, FLC_MAXTAGLEN long padded */
   sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
   fprintf(FLC_output, formatstring, "Function");

   /* now print the opcode names, padded to FLC_OPCODE_LEN */
   sprintf(formatstring, "%%%ds", FLC_OPSCNT_LEN);
   for (n = 0; n < FLC_PFIELDS; n++)
   {
      fprintf(FLC_output, formatstring, FLC_pfields[n]);
   }
   fputs("\n-----------\n", FLC_output);
}

static void FLC_Ops_print(char *prefix, FLC_Ops_Data *pL)
{
   char namestr[FLC_MAXTAGLEN + 1] = { 0 };
   char newprefix[FLC_MAXPREFIXLEN + 1] = { 0 };

   do
   {
      strncpy(namestr, prefix, FLC_MAXPREFIXLEN);
      strncat(namestr, pL->name, FLC_MAXTAGLEN - strlen(namestr));

      FLC_Ops_line(namestr, pL);

      /* check to see if there is a sublist */
      if (pL->subfirst != NULL)
      {
         strncpy(newprefix, prefix, FLC_MAXPREFIXLEN - 1);
         strcat(newprefix, "-");
         FLC_Ops_print(newprefix, pL->subfirst);
      }

      pL = pL->next;
   }
   while (pL != NULL);
}

static void FLC_Ops_line(char *pre, FLC_Ops_Data *pL)
{
   char formatstring[10];

   /* first, print a header for the tags, FLC_MAXTAGLEN long padded */
   sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
   fprintf(FLC_output, formatstring, pre);

   /* now print the opcode names, padded to FLC_OPCODE_LEN */
   sprintf(formatstring, "%%%dld", FLC_OPSCNT_LEN);
   fprintf(FLC_output, formatstring, pL->invocations);
   sprintf(formatstring, "%%%dlg", FLC_OPSCNT_LEN);
   fprintf(FLC_output, formatstring, pL->fn_total);
   fprintf(FLC_output, formatstring, pL->fn_total/(float)pL->invocations);

   fputc('\n', FLC_output);
}

/****** Memory usage printing *************/

static void FLC_Mem_print()
{
   FLC_Mem_Data *pL;
   FLC_Mem_Summaries *sums, *s_top;
   char formatstring[10];
   int n;

   s_top = NULL;
   memset(totals, 0, sizeof(totals));

   /* First compute sums per function */
   for (pL = mem_top; pL; pL = pL->next)
   {
      sums = FLC_Mem_find_function(s_top, pL->context);
      totals[pL->type] += pL->count;
      if (sums)
      {
         sums->counts[pL->type] += pL->count;
      }
      else
      {
         sums = malloc(sizeof(struct FLC_Mem_Summaries));
         memset(sums, 0, sizeof(struct FLC_Mem_Summaries));
         sums->counts[pL->type] += pL->count;
         sums->name = pL->context;
         sums->next = s_top;
         s_top = sums;
      }
   }


   /* Print summaries header ADD - DIV */

   sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
   fprintf(FLC_output, formatstring, "Function");
   sprintf(formatstring, "%%%ds", FLC_OPCODE_LEN);
   for (n = FLC_ADD; n <= FLC_DIV; n++)
   {
      fprintf(FLC_output, formatstring, Op_Names[n]);
   }

   fputs("\n-----------\n", FLC_output);

   for (sums = s_top; sums; sums = sums->next)
   {
      sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
      fprintf(FLC_output, formatstring, sums->name);
      sprintf(formatstring, "%%%dd", FLC_OPCODE_LEN);
      for (n = FLC_ADD; n <= FLC_DIV; n++)
      {
         fprintf(FLC_output, formatstring, sums->counts[n]);
      }
      fprintf(FLC_output, "\n");
   }

   fputs("\n-----------\n", FLC_output);

   sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
   fprintf(FLC_output, formatstring, "totals");
   sprintf(formatstring, "%%%dd", FLC_OPCODE_LEN);
   for (n = FLC_ADD; n <= FLC_DIV; n++)
   {
      fprintf(FLC_output, formatstring, totals[n]);
   }

   fputs("\n\n--------------------------------\n\n", FLC_output);

   /* Print summaries header SQRT - MISC */

   sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
   fprintf(FLC_output, formatstring, "Function");
   sprintf(formatstring, "%%%ds", FLC_OPCODE_LEN);
   for (n = FLC_SQRT; n <= FLC_MISC; n++)
   {
      fprintf(FLC_output, formatstring, Op_Names[n]);
   }

   fputs("\n-----------\n", FLC_output);

   for (sums = s_top; sums; sums = s_top)
   {
      sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
      fprintf(FLC_output, formatstring, sums->name);
      sprintf(formatstring, "%%%dd", FLC_OPCODE_LEN);
      for (n = FLC_SQRT; n <= FLC_MISC; n++)
      {
         fprintf(FLC_output, formatstring, sums->counts[n]);
      }
      fprintf(FLC_output, "\n");
      s_top = sums->next;
      free(sums);
   }

   fputs("\n-----------\n", FLC_output);

   sprintf(formatstring, "%%-%ds", FLC_MAXTAGLEN);
   fprintf(FLC_output, formatstring, "totals");
   sprintf(formatstring, "%%%dd", FLC_OPCODE_LEN);
   for (n = FLC_SQRT; n <= FLC_MISC; n++)
   {
      fprintf(FLC_output, formatstring, totals[n]);
   }
}

static FLC_Mem_Summaries *FLC_Mem_find_function(FLC_Mem_Summaries *top,
                                                char *name)
{
   FLC_Mem_Summaries *pL;

   for (pL = top; pL; pL = pL->next)
      if (pL->name == name)
         return pL;

   return pL;
}

static void FLC_print_sum()
{
   unsigned long gt, n;

   fprintf(FLC_output, "\n\n\n===== SUMMARY =====\n\n");

   /* Print total ops */
   fprintf(FLC_output, "Total Ops: %lg\n", ops_total->fn_total);

   /* Print overall total Program Memory Usage */
   gt = 0;
   for (n = 1; n < FLC_OPEND; n++)
      gt += totals[n] * Mem_Weights[n];

   fprintf(FLC_output, "Total Program ROM usage: %ld  (word)\n", gt);
}

static void FLC_print_frame_sum()
{
   fprintf(FLC_output, "\n===== Per Frame Summary =====\n");

   fprintf(FLC_output,
           "Number of Frames: %ld \nAverage Ops/frame: %8.2f   Max Ops/frame: %ld\n\n",
           ops_total->invocations,
           ops_total->fn_total / (float) ops_total->invocations,
           ops_total->fn_max);

   fprintf(FLC_output, "\n===== ESTIMATED COMPLEXITY (Frame length is %4.2f ms) =====\n", FLC_FRAMELEN );
   fprintf(FLC_output, "Maximum complexity: %f WMOPS\n",
           ops_total->fn_max / (1000.0f * (float) FLC_FRAMELEN) );
   fprintf(FLC_output, "Average complexity: %f WMOPS\n",
           (ops_total->fn_total / 
            (float) ops_total->invocations) / 
           (1000.0f * (float)FLC_FRAMELEN));

   fprintf(FLC_output, "\nEstimated fixed point complexity with %2.1f scaling factor: \n",
           FLC_SCALEFAC );
   fprintf(FLC_output, "Maximum complexity: %f WMOPS\n",
           FLC_SCALEFAC * ops_total->fn_max / (1000.0f * (float) FLC_FRAMELEN) );
   fprintf(FLC_output, "Average complexity: %f WMOPS\n\n",
           (FLC_SCALEFAC * ops_total->fn_total / 
            (float) ops_total->invocations) / 
           (1000.0f * (float)FLC_FRAMELEN));

}

#else   /* else of DONT_COUNT */

void FLC_init() {}
void FLC_end() {}
void FLC_sub_start(char *name) {}
void FLC_sub_end() {}
void FLC_frame_update() {}

#endif   /* end of DONT_COUNT */
