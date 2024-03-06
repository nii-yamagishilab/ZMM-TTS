/*                     v3.0 - 10/Jan/2007
============================================================================

ENCG722.C 
~~~~~~~~~

Description: 
~~~~~~~~~~~~

Demonstration program for UGST/ITU-T G.722 module with the G.722 
encoding function.

Input data is supposed to be aligned at word boundaries, i.e.,
organized in 16-bit words, following the operating system normal
organization (low-byte first for VMS and DOS; high byte first for most
Unix systems). Input linear samples are supposed to be 16-bit right-adjusted.

The default bit stream is a G.192 compatible 16 bit output file 
is provided using the  frame size specified by the -fsize command.
Bits in a G.192 bitstream frame are ordered so that the frame can be 
truncated at 56 and 48 kbps.

A legacy g722 byte stream can be obtained using the option -byte.
The legacy byte g722 ADPCM bitstream is left-adjusted, i.e., the codewords are 
located in the lower 8-bits of the encoded bitstream file. The MSB is
always 0 for the 16 bit bitstream file.

If the number of input samples are non divisible by the frame size,
the output codeword stream is truncated at the last frame boundary.

Usage:
~~~~~~
$ ENCG722 [-options] InpFile OutFile  
where:
InpFile     is the name of the file to be processed;
OutFile     is the name with the processed data;

Options:
-fsize  #   Number of 16 kHz input samples per frame (must be an even number). 
            Default is 160 samples(16 kHz) (10 ms)
            (NB!! -fsize must be the same on the decoder side, for g192 format.)  
-mode   #   Operating mode (1,2,3) (or rate 64, 56, 48 in kbps) . Default is mode 1 (= 64 kbps)
-frames #   number of frames to process (values -1 or 0 processes the whole file )
-byte       Provide encoder output data in legacy byte oriented format (default is g192). 			   
-h/-help    print help message

Original author:
~~~~~~~~~~~~~~~~
J-P PETIT 
CNET - Centre Lannion A
LAA-TSS                         Tel: +33-96-05-39-41
Route de Tregastel - BP 40      Fax: +33-96-05-13-16
F-22301 Lannion CEDEX           Email: petitjp@lannion.cnet.fr
FRANCE

History:
~~~~~~~~
14.Mar.95  v1.0       Released for use ITU-T UGST software package Tool
                      based on the CNET's 07/01/90 version 2.00
01.Jul.95  v2.0       Changed function declarations to work with many compilers;
                      reformated <simao@ctd.comsat.com>
22.May.06  v2.3       Updated with g192 format reading and basic index domain PLC functionality. 
                      <{nicklas.sandgren,jonas.svedberg}@ericsson.com>
10.Jan.07  v3.0       Updated with STL2005 v2.2 basic operators and G.729.1 methodology
                      Added some castings to avoid warnings
                      <{balazs.kovesi,stephane.ragot}@orange-ftgroup.com>
============================================================================
*/


/* Standard prototypes */
#include <stdio.h>
#include <stdlib.h>
/* ..... Generic include files ..... */
#include <string.h>		    /* memset */

#ifdef VMS
#include <stat.h>
#else
#include <sys/stat.h>
#endif

/* G.722- and UGST-specific prototypes */
#include "g722.h"
#include "ugstdemo.h"
#include "g722_com.h"

#include "stl.h"

void display_usage ARGS((void));

#define P(x) printf x
void display_usage()
{
	/* Print Message */
	printf ("\n\n");
	printf ("\n***************************************************************");
  printf ("\n* COPYRIGHT CNET LANNION A TSS/CMC Date 24/Aug/90             *");
  printf ("\n* COPYRIGHT Ericsson AB.           Date 22/May/06             *");
  printf ("\n* COPYRIGHT France Telecom R&D     Date 23/Aug/06             *");
	printf ("\n***************************************************************\n");

	/* Quit program */
	P(("USAGE: encg722 file.inp file.adp (all binary files).\n"));
	P(("or \n"));
	P(("       encg722 [-mode #] [-byte] [-fsize N] [-frames N2]  file.inp file.adp.g192 \n\n"));

	exit(-128);
}
#undef P
/* .................... End of display_usage() ........................... */


/*
**************************************************************************
***                                                                    ***
***        Demo-Program for supplying a g192 bit stream                ***
***                                                                    ***
**************************************************************************
*/
int             main (argc, argv)
int argc; 
char *argv[];
{
	/* Local Declarations */
	g722_state      encoder;

	/* operation specification operation specification */
	long            g192_outp = 1; 
	long            debug = 0;
	long			    mode =1;           /* operating mode encoding rate 64 kbps  */
	long            N=DEF_FR_SIZE;     /*frame size to use N=2...; */
	long            N2=0;             /* number of frames to process,  0,-1 => all */


	/* Sample buffers */
	short   code[MAX_BYTESTREAM_BUFFER];     /* analysis data output buffer */
	short   incode[MAX_INPUT_SP_BUFFER];     /* speech input buffer */
	short   outp_frame[2+MAX_BITLEN_SIZE]; /* g.192 output bit buffer */

	/* File variables */
	char    FileIn[MAX_STR], FileOut[MAX_STR];
	FILE    *F_inp, *F_cod;
	long    iter=0;
	long            frames=1; /* number of processed frames  */

	long            i=0; /* generic counter */
	long            j=0; /* generic counter */
	long            k=0; /* generic counter */
	long            bit=0; /* generic counter */
    short           mask=(short)0xFFFF; /* mode 2, mode 3 bit-mask */

#ifdef VMS
	char            mrs[15];
#endif

	/* Progress flag indicator */
	static char     quiet=0, funny[9] = "|/-\\|/-\\";

#ifdef WMOPS
    short spe1Id = -1;
#endif

	/* *** ......... PARAMETERS FOR PROCESSING ......... *** */

	/* GETTING OPTIONS */
	if (argc < 2)
		display_usage();
	else
	{
		while (argc > 1 && argv[1][0] == '-')
			if (strcmp(argv[1], "-byte") == 0) {
				/* g.192 output operation or not*/
				g192_outp = 0;
				/* Move argv over the option to the next argument */
				argv++;
				argc--;
			} else if (strcmp(argv[1], "-debug") == 0){
				/* debug printouts operation */
				debug=1; 		   
				/* Move argv over the option to the next argument */
				argv++;
				argc--;
			} else if (strcmp(argv[1], "-mode") == 0){
				/* Define operating mode  */
				mode = atoi(argv[2]);
				switch (mode){
					case 1:
					case 64: mode=1; break;
					case 2:
					case 56: mode=2; break;
					case 3:
					case 48: mode=3; break; 
					default:
						fprintf(stderr, "ERROR! Invalid mode \"%s\" in command line\n\n",argv[2]);
						display_usage();
				}
				/* Move argv over the option to the next argument */
				argv+=2;
				argc-=2;
			} else if (strcmp(argv[1], "-fsize") == 0){
				/* define frame size for g192 operation and file reading */
				N = atoi(argv[2]);
				if(( N > MAX_INPUT_SP_BUFFER) || (N <=0) || (N&0x0001)) {
					fprintf(stderr, "ERROR! Invalid frame size \"%s\" in command line\n\n",argv[2]);
					display_usage();
				}
				/* Move argv over the option to the next argument */
				argv+=2;
				argc-=2;
			}  else if (strcmp(argv[1], "-frames") == 0){
				/* Define number of frames  */
				N2 = atoi(argv[2]);
				if(( N2 <= 0)) {
					N2 = 0;
				}
				/* Move argv over the option to the next argument */
				argv+=2;
				argc-=2;
			} 
			else if (strcmp(argv[1], "-q") == 0){
				/* Don't print progress indicator */
				quiet = 1;
				/* Move argv over the option to the next argument */
				argv++;
				argc--;
			} else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
				/* Print help */
				display_usage();
			} else {
				fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
					argv[1]);
				display_usage();
			}
	}

	/* Now get regular parameters */
	GET_PAR_S(1, "_Input File: .................. ", FileIn);
	GET_PAR_S(2, "_Output File: ................. ", FileOut);
	/* end of Req parameters */
	if(debug) {
		fprintf(stderr, " fsize=%ld\n",N);
		fprintf(stderr, " mode=%ld\n",mode);
		fprintf(stderr, " g192=%ld\n",g192_outp);
		fprintf(stderr, " frames=%ld\n",N2);
	}

	/* Open input file */
	if ((F_inp = fopen (FileIn, RB)) == NULL){
		fprintf(stderr, "Could not open %s\n",FileIn);
		KILL(FileIn, -2);
	}

	/* Open output file */
	if ((F_cod = fopen (FileOut, WB)) == NULL) {
		fprintf(stderr, "Could not open %s\n",FileOut);
		KILL(FileOut, -2);
	}

	/* Reset lower and upper band encoders */
	g722_reset_encoder(&encoder);

#ifdef WMOPS
    setFrameRate(16000, N);
    spe1Id = getCounterId("encoding");
    setCounter(spe1Id);
    Init_WMOPS_counter();
#endif

	/* Read one frame of samples from input file and process */
	while ((fread ((char *) incode, sizeof (short), N, F_inp)) == (size_t)N) {
#ifdef WMOPS
                setCounter(spe1Id);
                fwc();
                Reset_WMOPS_counter();
#endif
		/* print progress flag */
		if (!quiet && !debug) {
			fprintf(stderr, "%c\r", funny[(iter/N) % 8]);
		}
		if (debug) {
			fprintf(stderr, "Processing frame(%ld)\n",frames);
		}
		/* Encode */
		iter += g722_encode(incode, code, N, &encoder);


		/* Save frame of N encoded samples */
		if(!g192_outp){
			/* every 16 kHz input sample yields an output 8 bit 8 kHz sample */
			if(mode != 1){
  			   fprintf(stderr, "Warning Non-g192 byet analysis stream, and (mode != 1) is only experimental !!\n");
            /* e.g. setting b0,b1 to zeros does not yield the same result as setting mode=3 in decoder */
            if (mode==2) { /* mask embedded bits to zeros, note actual rate is not reduced */
               mask=(short)0xFFFE; /*b0*/
            } else {
               mask=(short)0xFFFC; /*b0, b1*/
            }
            for(i=0;i<(N/2);i++){
               code[i] = (code[i]&mask); 
            }
			}
         if (fwrite ((char *) code, sizeof (short), N/2, F_cod) != (size_t)(N/2)){
					KILL(FileOut,-4);
			}
			if (debug) {
				fprintf(stderr, "NonG192: Wrote %ld data bits in  %ld output words\n",N/2*8,N/2);
			}
		} else { 
			/* Write a complete analysis bitstream for the frame in G.192 16 bit soft format */
			outp_frame[0]=G192_SYNC;
			switch (mode) {
					case 2: outp_frame[1]=(N*7)/2; break;
					case 3: outp_frame[1]=(N*6)/2; break;
					default:
						outp_frame[1]=(N*8)/2;
			}

			/* special ordering for scalability reasons */
			/* write [ b2*n, b3*n, b4*n, b5*n, b6*n, b7*n, b1*n, b0*n]  to enable truncation of G.722 g192 frames */ 
			/* always write "core"/non-scalable layer first b2s-b3s-b4s-b5s then b6s,b7s */
			/* b6s,b7s are semi scalable NB bits, however b6,b7 scalability is not part of the g.722 standard */
			j=0;
			for (bit=2;bit<8;bit++){
				for(i=0; i < (N/2); i++,j++){	     			
					if((code[i]>>bit)&0x0001){
						outp_frame[2+j]=G192_ONE; 
					} else {
						outp_frame[2+j]=G192_ZERO;
					}
				}
			}
			/* write b1s followed by b0s */
			k=0;                /* 64 kbps */
			if(mode==2) {k=1;}; /* 56 kbps */
			if(mode==3) {k=2;}; /* 48 kbps*/
			for (bit=1;bit>=k;bit--){
				for(i=0; i < (N/2); i++,j++){
					if((code[i]>>bit)&0x0001){
						outp_frame[2+j]=G192_ONE; 
					} else {
						outp_frame[2+j]=G192_ZERO;
					}
				}
			}

			if (fwrite ((char *) outp_frame, sizeof (short), (2+outp_frame[1]), F_cod) != (size_t)(2+outp_frame[1])){
				KILL(FileOut,-4);
			}
			if (debug) {
				fprintf(stderr, "Wrote  %d data bits for %ld input samples\n",outp_frame[1],N);
			}
		}


		if (debug) {
			fprintf(stderr, "*****************\n");
		}
		frames++;
		if( (N2!=0) && (frames>N2) ){
			break;
		}
	}/* while */
#ifdef WMOPS
      setCounter(spe1Id);
      fwc();
      WMOPS_output(0);
#endif

	/* Close input and output files */
	fclose(F_inp);
	fclose(F_cod);

	/* Exit with success for non-vms systems */
#ifndef VMS
	return (0);
#endif
}
/* ............................. end of main() ............................. */

