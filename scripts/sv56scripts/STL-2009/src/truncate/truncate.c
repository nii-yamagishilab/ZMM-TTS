/*                                                          02/Feb/2010 v1.3 */
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
	This file contains a demonstration program of a bitstream truncation tool, 
	using the functions in trunc-lib.h

  FUNCTIONS :
	Global (have prototype in reverb-lib.h)
		trunc		:		Bitstream truncation routine

  HISTORY :
	30.Mar.05	v1.0	First Beta version
	25.May.05	v1.1	Bug correction - unjustified "Warning: incomplete last frame, ignored"
	31.May.05	v1.2	Truncate operates on binary and G.192 byte-oriented bitstreams, 
                  using softbit.c routines.
                  <Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com>

  02.Feb.10 v1.3  Temporary bitstream file is now multi-thread safe, and
                  modified maximum string length to avoid buffer overruns
                  (y.hiwasaki)

  AUTHORS :
	Cyril Guillaume & Stephane Ragot -- stephane.ragot@francetelecom.com

*/

#define TMPFILE_FIX /* temporary; only to checkout v1.3 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* UGST modules */
#include "../utl/ugstdemo.h"
#include "../eid/softbit.h"
#include "trunc-lib.h"

/* G.192 constants */
#define SYNC_WORD_MIN	0x6B21
#define SYNC_WORD_MAX	0x6B2F
#define BAD_FRAME		0x6B20

#define MAX_BST_LENGTH 2560

#define P(x) printf x
static void display_usage()
{
  P(("TRUNCATE.C - Version 1.3 of 02.Feb.2010 \n\n"));
 
  P((" Bitstream truncation program\n"));
  P((" This program truncates a bitstream to obtain intermediate bitrates\n"));
  P((" This tool operates on :\n"));
  P(("       - G.192 (with or without sync header) bitstreams (16-bits words),\n"));
  P(("       - G.192 byte-oriented (with or without sync header) bitstreams,\n"));
  P(("       - binary (compact) bitstreams.\n"));
  P(("\n"));
  P((" Usage:\n"));
  P((" $ truncate   [-options] BstIn BstOut\n"));
  P((" where:\n"));
  P(("  BstIn        is the input bitstream;\n"));
  P(("  BstOut       is the output bitstream;\n"));
  P(("\n"));
  P((" Options:\n"));
  P(("  -fl fl.......... fl is the frame length of the input bistream in ms (default is 20ms);\n"));
  P(("  -b btr.......... btr is the constant bitrate of the output bistream file;\n"));
  P(("  -bf btrF........ btrF is a bitrate file containing the bitrate;\n"));
  P(("                   for each frame of the input bitstream (disabled for binary bitstreams);\n"));
  P(("  -ib btr......... btr is the constant bitrate of the input bitstream file (for binary bitstreams only);\n"));
  P(("  -q ............. quiet processing (no progress flag)\n"));
  P(("\n"));
}
#undef P

#define N 1024

/* 
	Routine converting bitstreams in the G.192 (with sync header) format.
	This routine uses the functions in softbit.c

	Authors : Cyril Guillaume & Stephane Ragot -- stephane.ragot@rd.francetelecom.com
*/
static int conv_inpBst(char type, int n, char sync_header, FILE * pfile, FILE * pfile_conv)
{
	short *patt;
	short sync=G192_SYNC;
	int nbread;

	patt=malloc(sizeof(short)*n);
	switch (type)
	{
	case byte:
		if (!sync_header)
			while(nbread=read_byte(patt,n,pfile)) {
				fwrite(&sync, sizeof(short), 1, pfile_conv);
				fwrite(&n, sizeof(short), 1, pfile_conv);
				save_g192(patt,nbread,pfile_conv);
			}
		else
			while(nbread=read_byte(patt,n,pfile)) {
				save_g192(patt,nbread,pfile_conv);
			}

		break;
	case compact:
		while(nbread=read_bit(patt,n,pfile,BER)) {
			 fwrite(&sync, sizeof(short), 1, pfile_conv);
			 fwrite(&n, sizeof(short), 1, pfile_conv);
			 save_g192(patt,nbread,pfile_conv);
		}
		break;
	case g192:
		if(!sync_header) {
			while(nbread=read_g192(patt,n,pfile)) {
				fwrite(&sync, sizeof(short), 1, pfile_conv);
				fwrite(&n, sizeof(short), 1, pfile_conv);
				save_g192(patt,nbread,pfile_conv);
			}
		}
		break;
	}

	return 0;
}

int main(argc, argv)
int argc;
char *argv[];
{
	/* .......... DECLARATIONS ............*/

	/* File variables */
	  FILE *pfilin;				/* input bitsream file */
	  FILE *pfilout;			/* output bitstream file */
	  FILE *pfilrate = NULL;	/* bitrate file */
	  FILE *pfiltmp;			/* temporary bitstream file */
	  char filin[MAX_STRLEN];  /* name of the input bitstream file */
	  char filout[MAX_STRLEN]; /* name of the output bitstream file */
#ifndef TMPFILE_FIX
	  char filtmp[9]="~bst.tmp";	/* name of the temporary bistream file */
#endif

	/* buffers */
	  short  bstIn[MAX_BST_LENGTH];	/*input frame*/
	  short* outFrame;				/*output frame*/

	/* Algorithm variables */
	  char type;				/* type of the input bitstream */
	  char conv=0;
	  char type_ER;
	  char sync_header=0;
	  short n;
	  int nubread;
	  int mode=-1;
	  long rate, inprate=-1, framerate, prev_bitrate;
	  int nbframe =0;
	  int quiet = 0;
	  double framelength= 0.02;
	  short sync;
	  short nbWords;
	  short nbBitsOut;
	  int nbWrd;


	/* ......... GET PARAMETERS ......... */

	/* Check options */
	if (argc < 5) {
		display_usage();
		exit(-1);
	}
	else
	{
		while (argc > 1 && argv[1][0] == '-')
			if (strcmp(argv[1],"-fl")==0)
			{
				/* Set the input framelength */
				  framelength = (float) atoi(&argv[2][0]);
				  framelength /= 1000;

				/* Move arg{c,v} over the option to the next argument */
				argc -=2;
				argv +=2;
			}
			else if (strcmp(argv[1],"-b")==0)
			{
				/* Set the output bitrate */
			      mode = 1;
				  rate = atoi(&argv[2][0]);

				/* Move arg{c,v} over the option to the next argument */
				argc -=2;
				argv +=2;
			}
			else if (strcmp(argv[1],"-ib")==0)
			{
				/* Set the input bitrate (for binary bitstreams (without headers))*/
				  inprate = atoi(&argv[2][0]);

				/* Move arg{c,v} over the option to the next argument */
				argc -=2;
				argv +=2;
			}
			else if (strcmp(argv[1],"-bf")==0)
			{
				/* Set the bitrate file */
				mode = 0;
		        if ((pfilrate=fopen(argv[2],"rb")) == NULL) {
					fprintf(stderr,"Error opening bitrate file %s\n",argv[2]);
					exit(-1);
				}

				/* Move arg{c,v} over the option to the next argument */
				argc -=2;
				argv +=2;
			}
			else if (strcmp(argv[1],"-q")==0)
			{
				/* Set the quiet mode flag */
				quiet=1;

				/* Move arg{c,v} over the option to the next argument */
				argc --;
				argv ++;
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
	GET_PAR_S(1, "_Input Bitstream File: .................. ", filin);
	GET_PAR_S(2, "_Output Bitstream File: ................. ", filout);


	/* check parameters */
	/* check input bistream file */
	if ((pfilin=fopen(filin,"rb")) == NULL) {
	  fprintf(stderr,"Error opening input bitstream file %s\n",filin);
	  exit(-1);
	}

	/* check input bitstream format */
	type = check_eid_format(pfilin, filin, &type_ER);

	/* check whether the bitstream has a sync header */
	sync_header = (type_ER == FER) ? 1 : 0;

	/* check input bitstream format, and, if needed, convert it to G.192 (with sync header) */
	if ( (type!=g192) || ((type==g192)&&(!sync_header)) ) {

		/* For bitstreams without sync header, the constant input bitrate need to be specified */
		if((sync_header==0)&&(inprate==-1)) {
			fprintf(stderr,"Error: Bitstreams without sync header need to have the input bitrate (-ib option) specified.\n");
			exit(-1);
		}

		/* Open temporary bitstream file */
#ifdef TMPFILE_FIX
		if ( (pfiltmp = tmpfile()) == NULL) {
			fprintf(stderr,"Error creating temporary bitstream file.\n");
			exit(-1);
		}
#else
		if ( (pfiltmp=fopen(filtmp,"wb")) == NULL) {
			fprintf(stderr,"Error creating temporary bitstream file %s\n",filtmp);
			exit(-1);
		}
#endif

		/* set block length for conversion */
		n = (sync_header) ? N : (short) (framelength*inprate);

		/* conversion */
		conv_inpBst(type, n, sync_header, pfilin,pfiltmp);
		conv=1;

		/* close files */
		fclose(pfilin);
#ifdef TMPFILE_FIX
		fclose(pfiltmp);
#endif

		/* open converted bitstream as input bitstream file */
#ifdef TMPFILE_FIX
		pfilin = pfiltmp;
		rewind(pfilin);
#else
		if( (pfilin=fopen(filtmp,"rb")) == NULL) {
			fprintf(stderr,"Error opening converted bitstream file %s\n",filtmp);
			exit(-1);
		}
#endif
	}

	/* check output bistream file */
	if ((pfilout=fopen(filout,"wb")) == NULL) {
	  fprintf(stderr,"Error opening output bitstream file %s\n",filout);
	  exit(-1);
	}

	if(mode == -1)
	{
	  fprintf(stderr,"Error : no bitrate option is given;\n");
	  exit(-1);
	}
  


  
  /*......... PROCESSING ..........*/

  /* loop over bitstream file */
  while((fread(&sync, 2, 1, pfilin) != 0) && (fread(&nbWords,2,1,pfilin) !=0) )  /* read the sync and framelength words */
  {

	  /* check sync word */
	  if ( !((sync<=SYNC_WORD_MAX)&&(sync>=SYNC_WORD_MIN)||(sync==BAD_FRAME)) )
	  {
		  fprintf(stderr, "Error: Bad Bitstream format");
		  exit(-1);
	  }

	  nbWrd=(int)nbWords;

	  /* read frame */
	  if(((int)fread(bstIn, 2, nbWrd, pfilin)) != nbWrd)
	  {
		  fprintf(stderr,"Warning: Incomplete last frame, ignored");
	  }
	  else{      
		  if(mode == 0)
		  {
			  /* read bitrate file */
			  if( (nubread = fread(&framerate, sizeof(framerate), 1, pfilrate)) != 1) 
			  {
				  printf("Warning : bitrate file too short, previous bitrate is used for the rest \n");
				  framerate = prev_bitrate;
			  }
			  prev_bitrate=framerate;
	  		  if (!quiet)
				  printf(" frame %d  rate  %d  \n",nbframe, framerate);
		  }
		  else if(mode == 1)
		  {
			  /* constant bitrate */
			  framerate = rate;
		  }

		  /* compute the output framelength */
		  nbBitsOut = (short) (framelength*framerate);

		  /* check output framelength consistency */
		  if (nbBitsOut>nbWrd)
		  {
			  nbBitsOut=nbWrd;
			  fprintf(stderr,"Warning: Desired bitrate is greater than input bitrate, input bitrate is chosen;\n");
		  }
		  /*allocate memory for the output frame */
		  outFrame=malloc(sizeof(outFrame[0])*(nbBitsOut+2));

		  /*truncate the frame according to the desired bitrate*/
		  trunca(sync, nbBitsOut, bstIn, outFrame);

		  /* write output bitstream */
		  fwrite(outFrame,sizeof(outFrame[0]),nbBitsOut+2,pfilout);

		  /*free memory for next iteration*/
		  free(outFrame);
		  
		  nbframe++;
	  }
  }/* end loop */
  
  printf("\nNumber of processed frames: %d\n",nbframe);
  


  /* FINALIZATIONS */
	
  /* close the opened files */
#ifndef TMPFILE_FIX
  fclose(pfilin);
#endif
  fclose(pfilout);
  if(pfilrate != NULL) fclose(pfilrate);
#ifndef TMPFILE_FIX
  if(conv)				/* remove temporary bitstream file */
	remove(filtmp);
#endif

  return(0);
}
