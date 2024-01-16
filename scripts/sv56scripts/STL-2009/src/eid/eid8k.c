/*                                                           02.Feb.2010 v3.3
  ============================================================================

  eid8k.c
  ~~~~~~~
  
  Program Description:
  ~~~~~~~~~~~~~~~~~~~~
  This example program produces the files which are necessary to 
  generate all error pattern for performing the Experiment 1 and 3 for 
  the ITU-T 8 kbit/s speech coding selection test. There, files
  that are CHAR oriented are used by the host laboratory hardware.
  Therefore, the output of this program are *NOT* the UGST-usually 
  used word format!!!

  This program generates a bitstream error pattern file that can be 
  x-ored with, for example, the coded bitstream of a test or reference 
  algorithm. The error pattern can be generated according to 3 models: 
  Random bit errors (BER), and random (FER) and (BFER) burst frame 
  erasure. 

  Valid range for the BER and FER is [0..0.5], and for the BFER is one of
  the 3 values 1%, 3%, and 5%.

  Usage:
  ~~~~~
  eid8k err_pat_bs mode frno startfr state_f rate
  Where:
  err_pat_bs ... name of error pattern bitstream file
  mode ......... a letter representing one of 4 operating modes: 
                 R -> Random Bit Errors
                 F -> (Random) Frame Erasure
                 B -> Burst Frame Erasure
  frno ......... the total number of error-producing frames
  startfr ...... the first frame to have bits that may introduce errors
  state_f ...... name of a state variable file
  rate ......... desired BER [R], FER [F], or BFER[B]

  Original Author:
  ~~~~~~~~~~~~~~~~
  Gerhard Schroeder 		Tel: +49 6151 833973  
  Deutsche Bundespost TELEKOM	Fax: +49 6151 895234
  Postfach 100003		Email: gerhard.schroeder@ties.itu.ch
  64276 Darmstadt
  Germany
    
  ITU-T recommendation:      
  ~~~~~~~~~~~~~~~~~~~~~
  Software tools UGST G.191

  History:
  ~~~~~~~~
  28.Jul.1993 v1.B  Created by <gerhard.schroeder@ties.itu.ch>
  19.Apr.1994 v1.0  Released to UGST; Added help and 
                    documentation changed by <simao@cpqd.ansp.br>
  06.Oct.1997 v3.1  Removed some compilation warnings <simao.campos@comsat.com>
  28.Mar.2000 v3.2  Added warning if module compiled in portability test
                    mode <simao.campos@labs.comsat.com>
  02.Feb.2010 v3.3  Modified maximum string length for filenames to avoid
                    buffer overruns (y.hiwasaki)
  ============================================================================
*/

/* ..... Generic include files ..... */
#include "ugstdemo.h"		/* general UGST definitions */
#include <stdio.h>		/* Standard I/O Definitions */
#include <ctype.h>		/* */

/* ..... Module definition files ..... */
#include "eid.h"		/* EID functions */
#include "eid_io.h"		/* EID I/O functions */

/* ..... Definitions used by the program ..... */
#define SYNC_WORD (short)0x6B21
#define EID_NULL (char)0X7F	
#define EID_ONE	(char)0X81
#define EID_FER	(char)0x00
#define EID_BUFFER_LENGTH 228
#define EID_MULTIPLE_BUFFER_LENGTH 2*EID_BUFFER_LENGTH
#define OUT_RECORD_LENGTH 512
#define ZERO_VECTOR_LENGTH OUT_RECORD_LENGTH-EID_MULTIPLE_BUFFER_LENGTH


/*
 -------------------------------------------------------------------------
 void display_usage(void);
 ~~~~~~~~~~~~~~~~~~
 Display proper usage for the demo program. Generated automatically from
 program documentation.

 History:
 ~~~~~~~~
 14.Apr.94 v1.0 Created. <simao>
 -------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage()
{
  P(("eid8k.c: Version 3.3 of 02.Feb.2010 \n\n"));

  P(("  This example program produces the files which are necessary to \n"));
  P(("  generate all error pattern for performing the Experiment 1 and 3 of\n"));
  P(("  the ITU-T 8 kbit/s speech coding selection test.\n"));
  P(("\n"));
  P(("  This program generates a bitstream error pattern file that can be \n"));
  P(("  x-ored with, for example, the coded bitstream of a test or reference \n"));
  P(("  algorithm. The error pattern can be generated according to 3 models: \n"));
  P(("  Random bit errors (BER), and random (FER) and (BFER) burst frame \n"));
  P(("  erasure. \n"));
  P(("\n"));
  P(("  Valid range for the BER and FER is [0..0.5], and for the BFER is one of\n"));
  P(("  the 3 values 1%%, 3%%, and 5%%.\n"));
  P(("\n"));
  P(("  Usage:\n"));
  P(("  eid8k err_pat_bs mode startfr frno state_f rate\n"));
  P(("  Where:\n"));
  P(("  err_pat_bs ... name of error pattern bitstream file\n"));
  P(("  mode ......... a letter representing one of 4 operating modes: \n"));
  P(("                 R -> Random Bit Errors\n"));
  P(("                 F -> (Random) Frame Erasure\n"));
  P(("                 B -> Burst Frame Erasure\n"));
  P(("  frno ......... the total number of error-producing frames\n"));
  P(("  startfr ...... the first frame to have bits that may introduce errors\n"));
  P(("  state_f ...... name of a state variable file\n"));
  P(("  rate ......... desired BER [R], FER [F], or BFER[B]\n"));
  P(("\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* .................... End of display_usage() ........................... */


/* ************************************************************************* */
/* ************************** MAIN_PROGRAM ********************************* */
/* ************************************************************************* */
int main (argc, argv)
int argc; 
char *argv[];
{   
    /*	  
    **  Command line parameters
    */	  
    char mode;			                  /* Processing mode Random,Frame,Burst */
    char data_file_name[MAX_STRLEN];  /* Name of the output file*/
    long number_of_frames=0;	        /* */
    long start_frame=0;		            /* */
    double ber_rate=0.0;	            /* Bit error rate. Depense on the mode*/
    char ber_file_name[MAX_STRLEN];   /* File name for saving BER state */
    char fer_file_name[MAX_STRLEN];   /* File name for saving FER state */
    char burst_file_name[MAX_STRLEN]; /* File name for saving FER burst state */
    /*	  
    **  File I/O parameter
    */	  
    FILE    *out_file_ptr;
    /*	  
    **  EID parameter UGST
    */	  
    SCD_EID	*BEReid,	    /* Pointer to BER EID structure */
		*FEReid;	    /* Pointer to FER EID structure */
    /*	  
    **  EID parameter Bellcore
    */	  
    BURST_EID	*burst_eid;	    /* Pointer to FER burst EID structure */
    /*	  
    **  Data arrays
    */
    char	zero_vector[ZERO_VECTOR_LENGTH],
		frame_erased[EID_BUFFER_LENGTH],
		frame_okay[EID_BUFFER_LENGTH];    	
    char	ybuff[512];	    /* Output data buffer with 512 entries */
    short  	*EPbuff;	    /* Bit error buffer */
    /*	  
    **  Aux. variables
    */	  
    double          FER;		/* frame erasure rate */
    double          BER;		/* bit error rate */	
    double          BER_gamma = 0.0, 
		    FER_gamma = 0.0;	/* burst factors. NOT NEEDED */

    double          ber1;	    /* returns values from BER_generator */
    double          ersfrms;	    /* distorted frames */
    double          prcfrms;	    /* number of processed frames */
    double	    genfrms;	    /* number of generated frames */
    double          dstbits;	    /* distorted bits */
    double	    genbits;	    /* number of generated bits */
    double          prcbits;	    /* number of processed bits */
    double	    percentage;
    char	    percent = '%';
    long	    i,j,k;
    long	    items;	    /* Number of output elements */
    long	    itot;
    long	    index;
#if defined(VMS)
    char mrs[15] = "mrs=512";
#endif


#ifdef PORT_TEST
    extern int PORTABILITY_TEST_OPERATION;
    if (PORTABILITY_TEST_OPERATION)
      fprintf(stderr, "WARNING! %s: compiled for PORTABILITY tests!\n\a", argv[0]);
#endif


    /* ***** PARSE INPUT DATA ***** */
    if (argc==1)
      display_usage();
    
    /*	  
    **  Get command line parameters
    */	  
    GET_PAR_S(1,"_Output bit stream file ...........................: ",
	      data_file_name);
    GET_PAR_C(2,"_Processing mode (Random,Frame,Burst) [RFB] .......: ",
	      mode);
    GET_PAR_L(3,"_Number of frames .................................: ",
	      number_of_frames);
    FIND_PAR_L(4,"_Start frame ......................................: ", 
	      start_frame, 1);
    
    /* Check consistency */
    if (start_frame > number_of_frames)
    {
    	printf (" Start frame is higher than number of frames\n");
	exit (1);
    }
    
    /*	  
    **  Open output file
    */
    if ((out_file_ptr = fopen(data_file_name, WB)) == NULL)
	HARAKIRI("Could not create output file\n", 1);
    
    /*	  
    **  Select mode
    */	  
    switch (mode)
    {
	case 'R':
	case 'r':
	    printf (" Generate Random Bit Errors: UGST model \n");
	    GET_PAR_S(5,"_File for BER state variable ...............: ",
			ber_file_name);
            /*	  
	    **  Try to open EID-States file.
	    */	  
            BEReid = recall_eid_from_file(&ber_file_name[0],
					  &BER,
					  &BER_gamma);
	    if (BEReid == (SCD_EID *) 0)
	    {
	    	printf (" File with EID-states doesn't exist. Creating one.\n"); 
		GET_PAR_D(6,"_Bit error rate (0.0 .. 0.5) ...............: ",
				ber_rate);
		BER = ber_rate;
                /*	  
		**  Setup new EID
		*/	  
                if ((BEReid = open_eid(BER,BER_gamma)) == (SCD_EID *) 0)
		{
		    HARAKIRI("Couldn't create EID for bit errors\n",1);
		}
	    }
	    ber_rate = BER;
	    printf (" ==> Error rate = %f  \n", ber_rate);
	    break;

	case 'F': 
	case 'f': 
	    printf (" Generate Random Frame Erasures: UGST model \n");
	    GET_PAR_S(5,"_File for FER state variable ...............: ",
			fer_file_name);
            /*	  
	    **  Try to open EID-States file.
	    */	  
            FEReid = recall_eid_from_file(&fer_file_name[0],
					  &FER,
					  &FER_gamma);
	    if (FEReid == (SCD_EID *) 0)
	    {
	      printf (" File with EID-states doesn't exist. Create one\n"); 
	      GET_PAR_D(6,"_Frame erasure rate (0.0 .. 0.5) ..............: ",
			ber_rate);
	      FER = ber_rate;      
	      /*	  
	       **  Setup new EID
	       */	  
	      if ((FEReid = open_eid(FER,FER_gamma)) == (SCD_EID *) 0)
	      {
		HARAKIRI("Couldn't create EID for frame errors\n",1);
	      }
	    }
	    ber_rate = FER;
	    printf (" ==> Frame erasure rate = %f \n", ber_rate);
	    break;

	case 'B': 
	case 'b': 
	    printf (" Generate Burst Frame Erasures: Bellcore model \n");
	    GET_PAR_S(5,"_File for FER state variable ............: ",
			burst_file_name);
	    GET_PAR_D(6,"_Burst erasure rate (1,3,5 pct) .........: ",
				ber_rate);
	    index  = (long)ber_rate;
            /*	  
	    **  Try to open burst eid state file
	    */	  
	    burst_eid = recall_burst_eid_from_file(&burst_file_name[0],index);
	    if (burst_eid == (BURST_EID *) 0)
	    {
	    	printf(" File with burst eid states doesn't exist.");
		printf (" Create one\n"); 
                /*	  
		**  Setup new EID
		*/	  
                if ((burst_eid = open_burst_eid(index)) == (BURST_EID *) 0)
		{
		    HARAKIRI("Couldn't create EID for burst frame errors\n",1);
		}
	    }
#ifdef DEBUG
	    printf (" Initial seed = %ld\n", burst_eid->seedptr);
	    printf (" Get another seed ? 0 = no ");
	    scanf ("%ld", &new_seed);
	    if (new_seed != 0)
	    {
	    	burst_eid->seedptr = (unsigned) new_seed;
		printf (" Initial seed = %ld\n", burst_eid->seedptr);
	    }
#endif
	    break;
    	default:
            HARAKIRI("Wrong mode parameter. Allowed values are R,F,B,A\n", 1);
	    break;
    }
    
    /*	  
    **  Allocate memory for the data buffers
    */	  
    EPbuff = (short *)calloc (EID_BUFFER_LENGTH, sizeof (short) );
    if (EPbuff == (short *) 0)
    {
        HARAKIRI("Could not allocate memory for error pattern buffer\n", 1);
    }

    /*	  
    **  Generate the bit streams
    */
    for (i=0;i < ZERO_VECTOR_LENGTH; i++)
      zero_vector[i] = 0;
    for (i=0;i < EID_BUFFER_LENGTH; i++) 
    {
      frame_erased[i] = EID_FER;	    /* 0x00 */
      frame_okay[i]   = EID_NULL;	    /* 0x7f */
    }
    
    memcpy (&ybuff[0],&frame_okay[0],EID_BUFFER_LENGTH);
    memcpy (&ybuff[EID_BUFFER_LENGTH],&frame_okay[0],EID_BUFFER_LENGTH);
    memcpy (&ybuff[2*EID_BUFFER_LENGTH],&zero_vector[0],ZERO_VECTOR_LENGTH);
    ber1 = 0.0;	dstbits = 0.0; prcbits = 0.0;
    ersfrms = 0.0; prcfrms = 0.0;
    genbits = 0.0; genfrms = 0.0;
    switch (mode)
    {
    	case 'R':
    	case 'r':
	    {
		for (i=1;  i<start_frame;  i++)
		{
		    items = fwrite (&ybuff[0],sizeof (char),512,out_file_ptr);
		    genbits += (double) 2*EID_BUFFER_LENGTH;
		}
		for (i=start_frame;  i <= number_of_frames;  i++)
		{
		    for (j=0;  j<=1;  j++)
		    {
		    	ber1 = BER_generator(BEReid,EID_BUFFER_LENGTH,EPbuff);
			dstbits += ber1;
			prcbits += (double) EID_BUFFER_LENGTH;
			genbits += (double) EID_BUFFER_LENGTH;
			for (k=0;  k<EID_BUFFER_LENGTH;  k++)
			{
		    	if (EPbuff[k] == (short) EID_NULL)
			    ybuff[k+j*EID_BUFFER_LENGTH] = EID_NULL;
			else
			    ybuff[k+j*EID_BUFFER_LENGTH] = EID_ONE;
			}
		    } 
		    memcpy (&ybuff[2*EID_BUFFER_LENGTH],
			    &zero_vector[0],
			    ZERO_VECTOR_LENGTH);
		    items = fwrite (&ybuff[0], sizeof (char), 512, 
				    out_file_ptr);
		}
	    	break;
	    }

	case 'F':
	case 'f':
	    {		
		for (i=1;  i<start_frame;  i++)
		{
		    items = fwrite (&ybuff[0],sizeof (char),512,out_file_ptr);
		    genfrms += 2.0;
		}
		for (i=start_frame;  i <= number_of_frames;  i++)
		{
		    for (j=0;  j<=1;  j++)
		    {
		    	ber1 = FER_generator_random(FEReid);
			ersfrms += ber1;
			prcfrms += 1.0;
			genfrms += 1.0;
			if (ber1 == 0.0)
			{
			    memcpy (&ybuff[j*EID_BUFFER_LENGTH],
				    &frame_okay[0],
				    EID_BUFFER_LENGTH );
			}
			else
			{
			    memcpy (&ybuff[j*EID_BUFFER_LENGTH],
				    &frame_erased[0],
				    EID_BUFFER_LENGTH );
			}
		    } 
		    memcpy (&ybuff[2*EID_BUFFER_LENGTH],
			    &zero_vector[0],
			    ZERO_VECTOR_LENGTH);
		    items = fwrite (&ybuff[0], sizeof (char), 512, 
				    out_file_ptr);
		}
		break;
	    }

	case 'B':
	case 'b':
	    {
		for (i=1;  i<start_frame;  i++)
		{
		    items = fwrite (&ybuff[0],sizeof (char),512,out_file_ptr);
		    genfrms += 2.0;
		}
		for (i=start_frame;  i<=number_of_frames;  i++)
		{
		    for (j=0;  j<=1;  j++)
		    {
		    	ber1 = FER_generator_burst(burst_eid);
			ersfrms += ber1;
			prcfrms += 1.0;
			genfrms += 1.0;
			if (ber1 == 0.0)
			{
			    memcpy (&ybuff[j*EID_BUFFER_LENGTH],
				    &frame_okay[0],
				    EID_BUFFER_LENGTH );
			}
			else
			{
			    memcpy (&ybuff[j*EID_BUFFER_LENGTH],
				    &frame_erased[0],
				    EID_BUFFER_LENGTH );
			}
		    } 
		    memcpy (&ybuff[2*EID_BUFFER_LENGTH],
			    &zero_vector[0],
			    ZERO_VECTOR_LENGTH);
		    items = fwrite (&ybuff[0], sizeof (char), 512, 
				    out_file_ptr);
		}
		break;
	    }
    }

    /*
    ** .. Save EID-status to file and print some statistics ...
    */
    switch (mode)
    {
      case 'R':
      case 'r':
      	  {
	      save_EID_to_file(BEReid, &ber_file_name[0], BER, BER_gamma);
	      printf (" Results for the Bit Error Random Generation \n");
	      printf ("  Data file name    : %s \n", data_file_name);
	      printf ("  State variable in : %s \n", ber_file_name);
	      printf ("  Generated bits    : %f \n", genbits);
	      printf ("  Processed bits    : %f \n", prcbits);
	      printf ("  Distored bits     : %f \n", dstbits);
	      percentage = 100.0*dstbits/prcbits;
	      printf ("  Bit Error Rate    : %f %c\n", percentage,percent);
	      break;
	  }

      case 'F':
      case 'f':
	  {
	      if (FER != 0.0) 
		save_EID_to_file(FEReid, &fer_file_name[0], FER, FER_gamma);
	      printf (" Results for the Random Frame Erasure Generation \n");
	      printf ("  Data file name    : %s \n", data_file_name);
	      printf ("  State variable in : %s \n", fer_file_name);
	      printf ("  Generated frames  : %f \n", genfrms);
	      printf ("  Processed frames  : %f \n", prcfrms);
	      printf ("  Distored frames   : %f \n", ersfrms);
	      percentage = 100.0*ersfrms/prcfrms;
	      printf ("  Frame Erasure Rate: %f %c\n", percentage,percent);
	      break;
	  }

      case 'B':
      case 'b':
      	  {
            /*	  
	    **  save burst eid state variable onto a file
	    */	  
            save_burst_eid_to_file(burst_eid,&burst_file_name[0]);
	    printf (" Results for the Burst Frame Erasure Generation \n");
	    printf ("  Data file name    : %s \n", data_file_name);
	    printf ("  State variable in : %s \n", burst_file_name);
	    printf ("  Generated frames  : %f \n", genfrms);
	    printf ("  Processed frames  : %f \n", prcfrms);
	    printf ("  Distored frames   : %f \n", ersfrms);
	    percentage = 100.0*ersfrms/prcfrms;
	    printf ("  Frame Erasure Rate  : %f %c\n", percentage,percent);
	    /* Display contents of state counters */
	    for (j=0; j < MODEL_SIZE; j++) 
		printf("%ld   ",burst_eid->internal[j]);
	    /* itot = number of good frames */
	    itot = burst_eid->internal[0];                   
	    for (i=1; i < MODEL_SIZE; i++)  /* plus number of bad frames */
		itot =  itot +  i * burst_eid->internal[i];
	    /* Total frames processed should equal number of trials */
	    printf("Total frames = %ld\n", itot);  
	    break;
	  }
    }    /*  End switch */

    /*	  
    **  Close the output file and quit
    */	  
    fclose (out_file_ptr);

#ifndef VMS			/* return value to OS if not VMS */
  return 0;
#endif
}
