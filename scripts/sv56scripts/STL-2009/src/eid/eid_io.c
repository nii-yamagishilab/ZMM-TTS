/*                                                        02.Feb.2010 v2.02
  =========================================================================

   EID_IO.C
   ~~~~~~~~

   Routines for performing the EID-state I/O - used by the demo
   program eid8k.c, gen-patt.c, and eid-xor.c
 
   ORIGINAL AUTHOR:
   ~~~~~~~~~~~~~~~~
   Gerhard Schroeder
   Deutsche Bundespost TELEKOM    Tel +49 6151 833973  
   Postfach 100003                FAX +49 6151 895234
   64276 Darmstadt                Email: gerhard.schroeder@ties.itu.ch
   Germany
 
   HISTORY:
   ~~~~~~~~
   29.Jul.93 v1.0  Created <gerhard.schroeder@ties.itu.ch>
   19.Apr.94 v1.1  Functions recall_burst_eid_from_file() and
                   save_burst_eid_to_file() had the local variable state_ptr
		   changed to pointer.
   15.Aug.97 v2.0  - Added magic number to save/recall eid (random/burst)
                     state functions; 
		   - Added functions to save bitstreams in different
                     formats <simao.campos@comsat.com>
   13.Jan.98 v2.01 Clarified ambigous syntax in save_EID_to_file() <simao>
   02.Feb.10 v2.02 Correct casting of arguments to "toupper()" <y.hiwasaki>
  =========================================================================
*/


/*
**
**  INCLUDE FILES
**
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ugstdemo.h"
#include "eid.h"
#include "eid_io.h"

#define MAGIC_EID "EID"
#define MAGIC_BURST 0x42464552 /* Long int encoding for ASCII string "BFER" */

/*
  ===========================================================================

  long save_EID_to_file (SCD_EID *EID, char *EIDfile, double BER,
  ~~~~~~~~~~~~~~~~~~~~~  double GAMMA);

  Description:
  ~~~~~~~~~~~~

  Save current states of EID to file. Data are stored on an
  ASCII file. May be that on some platforms this function must be
  slightly modified, but has worked nicely for the all tested.

  The contents of the EID-struct are stored on an ASCII file to
  allow the user observation or control or what  ever. The file
  should look like:
                         BER           = 0.020000
                         GAMMA         = 0.500000
                         RAN-seed      = 0x1db85ea6
                         Current State = G
                         GOOD->GOOD    = 0.980000
                         GOOD->BAD     = 1.000000
                         BAD ->GOOD    = 0.480000
                         BAD ->BAD     = 1.000000

  A magic number is saved to prevent that other EID modes read this
  state variable file as valid.

  Parameters:
  ~~~~~~~~~~~

  SCD_EID *EID .......... EID-structure
  char *EIDfile ......... filename for saving the state
  double BER ............ bit error rate
  double GAMMA .......... burst factor

  Return value:
  ~~~~~~~~~~~~~
  Returns 1 if EID-state was successfully saved to file
  and 0 if failed (couldn't create or write to the file).

  Author: <hf@pkinbg.uucp>
  ~~~~~~~

  History:
  ~~~~~~~~
  28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>
  14.Aug.97 v1.1 Added magic number <simao.campos@comsat.com>

  ===========================================================================
*/
long            save_EID_to_file(EID, EIDfile, BER, GAMMA)
  SCD_EID        *EID;
  char           *EIDfile;
  double          BER, GAMMA;
{
  FILE           *EIDfileptr;

  /* open specified ASCII file for "overwriting": */
  EIDfileptr = fopen(EIDfile, RWT);

  /* If failed, create new file: */
  if (EIDfileptr == NULL)
  {
    if ((EIDfileptr = fopen(EIDfile, WT)) == NULL)
      return (0L);
  }

  /* otherwise: set filepointer to beginning of file for overwriting */
    else
    {
      fseek(EIDfileptr, 0L, 0);
    }

  /* Save EID magic number into state variable file */
  fprintf(EIDfileptr, "%s\n", MAGIC_EID);

  /* Since the selected bit error rate and burst factor cannot be seen from
   * the transition matrix, these values are also stored in file (only for
   * documentation purposes). */
  fprintf(EIDfileptr, "BER           = %f\n", BER);
  fprintf(EIDfileptr, "GAMMA         = %f\n", GAMMA);

  /* current state of random generator: */
  fprintf(EIDfileptr, "RAN-seed      = 0x%08lx\n", get_RAN_seed(EID));

  /* current state of GEC-model: */
  fprintf(EIDfileptr, "Current State = %c\n", get_GEC_current_state(EID));

  /* Save contents of Transition Matrix: */
  fprintf(EIDfileptr, "GOOD->GOOD    = %f\n", get_GEC_matrix(EID, 'G', 'G'));
  fprintf(EIDfileptr, "GOOD->BAD     = %f\n", get_GEC_matrix(EID, 'G', 'B'));
  fprintf(EIDfileptr, "BAD ->GOOD    = %f\n", get_GEC_matrix(EID, 'B', 'G'));
  fprintf(EIDfileptr, "BAD ->BAD     = %f\n", get_GEC_matrix(EID, 'B', 'B'));

  fclose(EIDfileptr);
  return (1L);
}
/* ....................... End of save_EID_to_file() ....................... */


/*
  ============================================================================

  SCD_EID *recall_eid_from_file (char *EIDfile,
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  double *ber, double *gamma);

  Description:
  ~~~~~~~~~~~~

  Allocate memory for EID struct and load EID-states from
  previous call (which is saved on file) into a struct SCD_EID.

  Data are read from an ASCII file. May be that  on some platforms
  this function must be slightly modified, but has worked nicely
  for all the ones tested.

  A magic number prevents that state variable files saved by other
  modes of the EID be read. If a magic number is not found, the
  function returns as having failed.

  Parameters:
  ~~~~~~~~~~~
  char *EIDfile ... file with EID states
  double *ber ..... pointer to "bit error rate"
  double *gamma ... pointer to burst factor

  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to a EID-state data structure; if failed, it will
  be a null pointer (couldn't find the file, or the file did not have
  a valid magic number, or could not allocate memory for the state
  variable).

  Author: <hf@pkinbg.uucp>
  ~~~~~~~

  History:
  ~~~~~~~~
  28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>
  14.Aug.97 v1.1 Added magic number <simao.campos@comsat.com>

 ============================================================================
*/
SCD_EID        *recall_eid_from_file(EIDfile, ber, gamma)
  char           *EIDfile;
  double         *ber;
  double         *gamma;
{
  SCD_EID        *EID;
  FILE           *EIDfileptr;
  char            chr;
  double          thr;
  long            seed;
  char            magic[8];

  /* Open ASCII file with EID states */
  if ((EIDfileptr = fopen(EIDfile, RT)) == NULL)
    return ((SCD_EID *) 0);

  /* Look for EID magic number into state variable file */
  /* If the magic number is not found, returns a NULL pointer */
  fgets(magic, 5, EIDfileptr);
  if (strncmp(magic, MAGIC_EID, strlen(MAGIC_EID)))
    return ((SCD_EID *) 0);

  /* Load channel model parameters ber and gamma */
  READ_lf(EIDfileptr, 1L, ber);
  READ_lf(EIDfileptr, 1L, gamma);

  /* Now open EID with default values and update states afterwards from file */
  if ((EID = open_eid(*ber, *gamma)) == (SCD_EID *) 0)
    return ((SCD_EID *) 0);

  /* update EID-struct from file: seed for random generator */
  READ_L(EIDfileptr, 1L, &seed);
  set_RAN_seed(EID, (unsigned long) seed);	/* store into struct */

  /* update EID-struct from file: current state */
  READ_c(EIDfileptr, 1L, &chr);
  set_GEC_current_state(EID, chr);

  /* update EID-struct from file: threshold GOOD->GOOD */
  READ_lf(EIDfileptr, 1L, &thr);
  set_GEC_matrix(EID, thr, 'G', 'G');

  /* update EID-struct from file: threshold GOOD->BAD */
  READ_lf(EIDfileptr, 1L, &thr);
  set_GEC_matrix(EID, thr, 'G', 'B');

  /* update EID-struct from file: threshold BAD ->GOOD */
  READ_lf(EIDfileptr, 1L, &thr);
  set_GEC_matrix(EID, thr, 'B', 'G');

  /* update EID-struct from file: threshold BAD ->BAD */
  READ_lf(EIDfileptr, 1L, &thr);
  set_GEC_matrix(EID, thr, 'B', 'B');

  /* Finalizations */
  fclose(EIDfileptr);
  return (EID);
}
/* ..................... End of recall_eid_from_file() ..................... */


/*
  ============================================================================

        long READ_L (FILE *fp, long n, long *ary);
        ~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~

        Read `n' longs from an EID-state file onto an array.

        Return value:
        ~~~~~~~~~~~~~
        Returns the number of longs read.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
long
                READ_L(fp, n, longary)
  FILE           *fp;
  long            n;
  long           *longary;
{
  long            i, ic;
  char            c;
  char            ch[16];


  while ((c = getc(fp)) != '=');
  for (i = 0; i < n; i++)
  {
    while (((c = getc(fp)) == 32) || (c == 9));

    ic = 0;
    while ((c != 32) && (c != 9) && (c != '\n') && (ic < 15))
    {
      ch[ic++] = c;
      c = getc(fp);
    }
    ch[ic] = (char) 0;
    if ((ch[0] == '0') && (toupper((int)ch[1]) == 'X'))
    {
      sscanf(&ch[2], "%lx", &longary[i]);
    }
    else
    {
      sscanf(ch, "%ld", &longary[i]);
    }
  }
  return (n);
}
/* ....................... End of READ_L() ....................... */


/*
  ============================================================================

        long READ_lf (FILE *fp, long n, double *doubleary);
        ~~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Read `n' doubles from an EID-state file onto an array.

        Return value:
        ~~~~~~~~~~~~~
        Returns the number of doubles read.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
long
                READ_lf(fp, n, doubleary)
  FILE           *fp;
  long            n;
  double         *doubleary;
{
  long            i, ic;
  char            c;
  char            ch[64];


  while ((c = getc(fp)) != '=');
  for (i = 0; i < n; i++)
  {
    while (((c = getc(fp)) == 32) || (c == 9));

    ic = 0;
    while ((c != 32) && (c != 9) && (c != '\n') && (ic < 63))
    {
      ch[ic++] = c;
      c = getc(fp);
    }
    ch[ic] = (char) 0;
    sscanf(ch, "%lf", &doubleary[i]);
  }
  return (n);
}
/* ....................... End of READ_lf() ....................... */


/*
  ============================================================================

        long READ_c (FILE *fp, long n, char *chr);
        ~~~~~~~~~~~

        Description:
        ~~~~~~~~~~~~
        Read `n' doubles from an EID-state file onto an array.

        Return value:
        ~~~~~~~~~~~~~
        Returns the number of chars read.

        Author: <hf@pkinbg.uucp>
        ~~~~~~~

        History:
        ~~~~~~~~
        28.Feb.92 v1.0 Release of 1st version <hf@pkinbg.uucp>

 ============================================================================
*/
long
                READ_c(fp, n, chr)
  FILE           *fp;
  long            n;
  char           *chr;
{
  long            i;
  char            c;


  while ((c = getc(fp)) != '=');
  for (i = 0; i < n; i++)
  {
    while (((c = getc(fp)) == 32) || (c == 9));
    *chr = c;
    while ((c = getc(fp)) != '\n');
  }
  return (n);
}
/* ....................... End of READ_c() ....................... */


/*
  ============================================================================

  BURST_EID *recall_burst_eid_from_file (char *state_file, long index);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Recall burst eid state variable from a binary file (Bellcore model)
  A magic number prevents that state variable files saved by other modes
  of the EID be read. If a magic number is not found, the function
  returns as having failed.

  Formal Parameters:
  ~~~~~~~~~~~~~~~~~~
  state_file:
          Data type:   char string
          Access:      read only
          Mechanism:   pointer
          Description: Name of the burst eid state file
       
  Return Value:
  ~~~~~~~~~~~~~
  Pointer to BURST_EID; NULL on error (couldn't find the file, or the
  file did not have a valid magic number, or could not allocate memory
  for the state variable).

  Side Effects:
  ~~~~~~~~~~~~~
  None
      
  History:
  ~~~~~~~~
  29.Jul.93 v1.0 Routine created <gerhard.schroeder@ites.itu.ch>
  19.Apr.94 v1.1 Fixed error in declaration of local var. state_ptr, that 
                 has to be a pointer. <simao>
  14.Aug.97 v1.2 Added magic number <simao.campos@comsat.com>
 ============================================================================
*/
BURST_EID *recall_burst_eid_from_file (state_file,index)
    char *state_file;
    long index;
{
    FILE       *state_ptr;
    BURST_EID  *burst_eid,
	       *eid_in;
    long	items, magic;
#if defined(VMS)
    char mrs[15] = "mrs=512";
#endif

    /*	  
    **  Open BURST EID state file
    **	Return a null pointer, if the file doesn't exists
    */	  
    if ((state_ptr = fopen (state_file, RB)) == NULL)
    	 return ((BURST_EID *) 0);
    	 
    /* 
    ** Read Magic number for Burst EID State Files.
    ** If it does not find the magic number, it returns 0
    */
    items = fread (&magic, 1, sizeof (long), state_ptr);
    if (items != sizeof(long) || magic != MAGIC_BURST)
    	return ((BURST_EID *) 0);

    /*	  
    **  Allocate memory for the burst eid from file
    */	  
    if ((eid_in = (BURST_EID *)malloc (sizeof (BURST_EID))) == 0L)
	    return ((BURST_EID *) 0);
    /*	  
    **  Read state from file
    */	  
    items = fread (eid_in, 1, sizeof (BURST_EID),  state_ptr);
    if (items != sizeof(BURST_EID))
   	return ((BURST_EID *) 0);
   	
    /*	  
    **  Open internal eid
    */	  
    if ((burst_eid = open_burst_eid(index)) == (BURST_EID *) 0)
	{
	    return ((BURST_EID *) 0);
	} 		

    /*	  
    **  Update burst eid
    */	  
    if ((index-1) == eid_in->index)
    {
    	burst_eid->seedptr = eid_in->seedptr;
	burst_eid->s_new   = eid_in->s_new;
    }
    
    /* Close and return */
    fclose (state_ptr);
    return (burst_eid);
}
/* ................ End of recall_burst_eid_from_file() ................. */


/*
 ============================================================================

  long save_burst_eid_to_file (BURST_EID *burst_eid, char *state_file);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Description:
  ~~~~~~~~~~~~
  Save the burst eid state variable onto a binary file (Bellcore model).
  A magic number is saved to prevent that other EID modes read this
  state variable file as valid.

  Formal Parameters:
  ~~~~~~~~~~~~~~~~~~
  burst_eid:
          Data type:   BURST_EID
          Access:      read only
          Mechanism:   pointer
          Description: Contains of the state variable
       
  state_file:
          Data type:   char string
          Access:      read only
          Mechanism:   pointer
          Description: Name of the binary file which should get the state

  Return Value:
  ~~~~~~~~~~~~~
  Return 1 if OK and 0 on failure (couldn't create or write to the file).

  Side Effects:
  ~~~~~~~~~~~~~
  None
      
  History:
  ~~~~~~~~
  29.Jul.93 v1.0 Routine created <gerhard.schroeder@ties.itu.ch>
  19.Apr.94 v1.1 Fixed error in declaration of local var. state_ptr, that 
                 has to be a pointer. <simao>
  14.Aug.97 v1.2 Added magic number <simao.campos@comsat.com>
 ============================================================================
*/
long save_burst_eid_to_file (burst_eid,state_file)
    BURST_EID	*burst_eid;
    char	*state_file;
{
#if defined(VMS)
    char mrs[15] = "mrs=512";
#endif
    FILE	*state_ptr;
    long	items, magic = MAGIC_BURST;


    /*	  
    **  Open BURST EID state file
    **	Return zero, if the file doesn't exist
    */	  
    if ((state_ptr = fopen (state_file, WB)) == NULL)
    	 return (0);

    /* 
    ** Save Magic number into Burst EID State Files.
    */
    items = fwrite (&magic, 1, sizeof (long), state_ptr);
    if (items != sizeof(long))
    	return (0);

    /*	  
    ** Write state to file
    */	  
    items = fwrite (burst_eid, 1, sizeof (BURST_EID), state_ptr);
    if (items != sizeof(BURST_EID))
    	return (0);

    /*	  
    **  Close state file
    */	  
    fclose (state_ptr);
    return(1); /* OK status */
}
/* ................ End of save_burst_eid_to_file() ................. */


/* ********************************************************************* */
/* ********************************************************************* */
/* ************************* ADDED AFTER STL96 ************************* */
/* ********************************************************************* */
/* ********************************************************************* */

/* ************************* END OF eid_io.c ************************* */
