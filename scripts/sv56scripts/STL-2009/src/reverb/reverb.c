/*                                                         02/Feb/2010 v1.02 */
/*=============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

       =============================================================
       COPYRIGHT NOTE: This source code, and all of its derivations,
       is subject to the "ITU-T General Public License". Please have
       it  read  in    the  distribution  disk,   or  in  the  ITU-T
       Recommendation G.191 on "SOFTWARE TOOLS FOR SPEECH AND  AUDIO
       CODING STANDARDS".
       =============================================================

  DESCRIPTION : 
	This file contains a demonstration program of a reverberation tool, using the functions in reverb-lib.h

  HISTORY :
	02.Feb.05	v1.0	First Beta version
	10.Jul.08 v1.01 Added 16 bit saturation and saturation warning
	02.Feb.10 v1.02 Modified maximum string length to avoid buffer overrun

  AUTHORS :
	v1.0  Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com
  v1.01 Jonas Svedberg jonas.svedberg@ericsson.com
  v1.02 yusuke hiwasaki
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* UGST modules */
#include "ugstdemo.h"

#include "reverb-lib.h"

#define P(x) printf x
static void display_usage()
{
  P(("REVERB.C - Version 1.02 of 02.Feb.2010 \n\n"));
 
  P((" Program to add reverberation to a signal\n"));
  P((" This program convolves a signal with the impulse response of a room\n"));
  P((" Make sure that the impulse response and the input file are sampled at the same rate\n"));
  P((" If output saturation occurs in processing a warning message is displayed on stderr. \n"));

  P(("\n"));
  P((" Usage:\n"));
  P((" $ reverb   [-options] FileIn FileIR FileOut\n"));
  P((" where:\n"));
  P(("  FileIn       is the file to be processed;\n"));
  P(("  FileIR       is the file containing the impulse response;\n"));
  P(("  FileOut      is the file with the processed data;\n"));
  P(("\n"));
  P((" Options:\n"));
  P(("  -align A...... multiplicative factor to apply to the reverberated sound\n"));
  P(("				   in order to align its energy level with a second file\n"));
  P(("\n"));
}
#undef P


#define tmpIRlength	512

int main(argc, argv)
int argc;
char *argv[];
{



	/* .......... DECLARATIONS ............*/

	/* File variables */
	FILE* ptr_fileIn;
	FILE* ptr_fileOut;
	FILE* ptr_fileIR;
	char FileIn[MAX_STRLEN];
	char FileIR[MAX_STRLEN];
	char FileOut[MAX_STRLEN];

	/* buffers */
	float *IR;		/* buffer for the impulse response */
	short *buffRvb;	/* buffer for the reverberated Sound */
	short *buffIn;	/* buffer for the input sound file */
	float tmpIR[tmpIRlength];	/* temporary buffer for the impulse response reading */

	/* Algorithm variables */
	float alignFact=1.0;/* multiplicative factor for the reverberated sound (energy alignment with another file to compare) */
	long  N;			/* length of the impulse response */
	long  count,global_count;
    long  local_sat_pos;
 
    global_count   = 0; 
    local_sat_pos  = -1; /* local position of last saturation */ 


	/* ......... GET PARAMETERS ......... */

	/* Check options */
	if (argc < 4)
		display_usage();
	else
	{
		while (argc > 1 && argv[1][0] == '-')
			if (strcmp(argv[1],"-align")==0)
			{
				/* Set the energy alignment factor */
				alignFact = (float) atof(argv[2]);

				/* Move arg{c,v} over the option to the next argument */
				argc -=2;
				argv +=2;
			}
			else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-?") == 0)
			{
				/* Display help message */
				display_usage();
				exit(2);
			}
			else
			{
				fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",argv[1]);
				display_usage();
				exit(-1);
			}
	}

	/* Read parameters for processing */
	GET_PAR_S(1, "_Input File: .................. ", FileIn);
	GET_PAR_S(2, "_Impulse Response File: ....... ", FileIR);
	GET_PAR_S(3, "_Output File: ................. ", FileOut);







	/* ......... PREPARING FILES ......... */

	/* Load the Impulse Response */
	ptr_fileIR=fopen(FileIR,"rb");
	if (ptr_fileIR == NULL)
	{
		fprintf(stderr, "\nUnable to open Input file\n");
		exit(-1);
	}
		/* determine the length of the impulse response */
	N=0;
	while(!feof(ptr_fileIR))
	{
		N+=fread(tmpIR,sizeof(float),tmpIRlength,ptr_fileIR);
	}
		/* allocate memory for the impulse response buffer */
	IR = (float *) calloc(N, sizeof(float));
	rewind(ptr_fileIR);
		/* read the impulse response */
	fread(IR,sizeof(float),N,ptr_fileIR);
		/* close file */
	fclose(ptr_fileIR);

	/* open the input file */
	ptr_fileIn=fopen(FileIn,"rb");
	if (ptr_fileIn==NULL) 
	{
		fprintf(stderr, "\nUnable to open Input file\n");
		exit(-1);
	}

	/* open the output file */
	ptr_fileOut=fopen(FileOut,"wb");
	if (ptr_fileOut==NULL) 
	{
		fprintf(stderr, "\nUnable to open Output file\n");
		exit(-1);
	}
	/* allocate memory for the buffers */
	buffIn =(short*)  calloc(2*N-1,sizeof(short));	/* allocate memory for a block of the input file */
	buffRvb=(short *) malloc(N*sizeof(short));		/* allocate memory for the processed block */

	/* check consistency */
	if((buffIn==NULL)||(buffRvb==NULL))
	{
		fprintf(stderr, "\nUnable to allocate enough memory\n");
		exit(-1);
	}





	/* .......FILTERING OPERATION ........*/

	/* Filter the sound File */
	while( !feof(ptr_fileIn) )
	{
		count= (long) fread(buffIn+N-1,sizeof(short),N,ptr_fileIn);	/* read a block of the input file */
        
		local_sat_pos = conv(IR,buffIn,buffRvb,alignFact,N,count);		    /* convolves a block of the input file with the impulse response */
        if(local_sat_pos >= 0){
           fprintf(stderr, "\nWarning warning!! Saturation(s) in output file.  In  sample %ld\n", 
                   local_sat_pos + global_count);
        }
        global_count += count;
		fwrite(buffRvb,sizeof(short),count,ptr_fileOut);			/* output the processed block */
		shift(buffIn,N);											/* shift a part of the input buffer (to keep the N-1 
																	   last samples of the input file for the next processing) */
	}





	/* FINALIZATIONS */
	
	/* free allocated memory */
	free(buffIn);
	free(buffRvb);
	free(IR);
	/* close the opened files */
	fclose(ptr_fileIn);
	fclose(ptr_fileOut);


	return(0);

}
