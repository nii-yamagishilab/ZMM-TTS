/* 
    gen_rate_profile.c   

    Program for generating random rate/layer switching file.
 
    For every "period" a layer rate is randomly selected among a set of layer rates 
    defined on the command line e.g. -layers 32000,36000,40000   (bps). 
	(The output rate file is a stream of long integers quantized with 1 bps resolution.)

	If desired the lowest rate can be set to a minimum value "minrate",
	(e.g. to avoid switching to lower synthesis rates for some scalable codecs.)   
    50 Hz framerate is default. 

    Note: the layer rates  and minrate value can only be entered with  steps accordng to the framerate, 
 	      e.g. minrate=37001 creates the layer setup [37000,37000,40000], 
          with the set of layers described above 
		   
    Command line 
      gen_rate_profile -layers A,B,C... [-framerate X] ratefilename [period  minrate [frames [seed] ] ]

    where
     -layers A,B,C.. is the layer setup, in bps, in a monotonically increasing list
     -framerate X   is the frame rate (default is X=50 Hz)
      ratefilename  is the output file (stream of long integers) (no default)
      period        is the repetition period in frames(default 10)
	  minrate       is a minumum rate in integer bps in the output 
      frames        is the total number of frames to produce (default 15000 frames, 300 seconds)
      seed          is the random number generator seed(default is 314159265). 

  Compile in STL eid subdirectory :
    gcc gen_rate_profile.c  -I ../utl -o gen_rate_profile.exe 

  Example execution
  >  ./gen_rate_profile.exe -layers 32000,36000,40000 -framerate 50 ratefile.long 2 37000 10 5 
*/

#include "ugstdemo.h"   /* general UGST definitions */
#include <stdio.h>		/* Standard I/O Definitions */
#include <stdio.h>		/* Standard I/O Definitions */
#include <math.h>
#include <stdlib.h>

/* ..... Definitions used by the program ..... */
#define DEF_PERIOD     10
#define MAX_PERIOD     2000
#define DEF_FRAMERATE  50  /* Hz */
#define DEF_FRAMES     300*DEF_FRAMERATE
#define MAX_STR        2048
#define MAX_LAYERS     256
#define BPS_STEP       DEF_FRAMERATE
#define DEF_SEED       314159265
#define DEF_MINRATE    32000
#define VERSION        "v2.1 02 Feb. 2010"

/* local functions */

/*-------------------------------------------------------------------------
display_usage(int level);  Shows program usage.
-------------------------------------------------------------------------*/
#define P(x) printf x
void            display_usage (level)
int level;
{
	P(("gen_rate_profile - %s\n\n",VERSION));

    if (level)
    { 
        P(("Program Description:\n"));
        P(("\n"));
        P(("Program generates a rate profile\n"));
        P(("The rate profile may be used with e.g the STL truncate tool\n"));

        P(("Command line\n")); 
        P((" gen_rate_profile -layers A,B,C... [-framerate X] ratefilename [period  minrate [frames [seed] ] ]\n\n"));

        P(("where\n"));
        P((" -layers A,B,C.. is the layer setup, in bps, in a monotonically increasing list\n"));
        P((" -framerate X   is the frame rate (default is X=50 Hz)\n"));
        P((" ratefilename  is the output file (stream of long integers) (no default)\n"));
        P((" period        is the repetition period in frames(default 10)\n"));
        P((" minrate       is a minumum rate in integer bps in the output \n")); 
        P((" frames        is the total number of frames to produce (default 15000 frames, 300 seconds)\n"));
        P((" seed          is the random number generator seed(default is 314159265).\n")); 
        P(("\n"));
    }
	else
	{
		P(("Program generates a rate profile\n"));
        P(("The rate profile may be used with e.g the STL truncate tool\n"));
        P(("Command line\n")); 
        P((" gen_rate_profile -layers A,B,C... [-framerate X] ratefilename [period  minrate [frames [seed] ] ]\n"));
	}

	P(("Usage:\n"));
	
	/* Quit program */
	exit (-128);
}
#undef P
/* .................... End of display_usage() ........................... */

/*========================================================================== 
double EID_random (unsigned long *seed); inherited from STL/eid.h, 
output is in the range 0 to .99
============================================================================ */
double EID_random(seed)
unsigned long *seed;
{
	/* Size in bits (=size in bytes * 8) for long variables*/
	static double bits_in_long = sizeof(long) * 8;
	/* Update RNG */
	*seed = ((unsigned long)69069L * (*seed) +1L);
	/* Return random number as a double */
#ifdef WAS
	return(pow((double)2.0, (double)-32.0) * (double)(*seed));
#else
	return(pow((double)2.0, -bits_in_long) * (double)(*seed));
#endif
}

/* ************************* AUXILIARY FUNCTIONS ************************* */
long parse_layers(char *inp_str, long *bound)
{
	/* 
	valid layer string format is %d,%d,%d or  just %d 
	no spaces allowed !!,
	function returns number of read layers or returns -1 on failure   
	*/

	long i,index=0;
	long status = -1;
	char *pch;
	char tmp_str[MAX_STR];

	strncpy(tmp_str,inp_str,MAX_STR);
	pch = (char*)strtok (tmp_str," ,.");
	/*printf ("(%s)\n",pch);*/
	while (pch != NULL){
		bound[index]=atoi(pch); 
		pch = (char*)strtok (NULL,",");
		index++;
	}
	for(i=0;i<index;i++){
		if(bound[i] < 0){
			fprintf(stderr,"Illegal layer string values %s\n\n",inp_str);
			return -1;
		}
	}
	/* check logical ordering */
	if (index > 0){
		for(i=1;i<index;i++){
			if(bound[i-1]>bound[i]){
				fprintf(stderr,"Illegal unordered layer string %s\n\n",inp_str);
				return -1;
			}	
		}
	}
	/* parse printout */
	/*for(i=0;i<index;i++){
		fprintf(stderr,"read layer boundary[%ld]=%ld\n",i,bound[i]);
	}*/
	status=index;

	return(status);
}
/* ....................... End of parse_layers() ....................... */


int   main (int argc, char *argv[]){
	FILE          *Forf;                   /* Pointer to output rate file */
	char          o_file[MAX_STRLEN];      /* Output bitstream file */
	long          n_layers;                /*number of active layers */  
	long	        layer_b[MAX_LAYERS];     /*layering boundaries information*/	
  char          layer_str[MAX_STR];      /* layering string for parsing*/
	long          l_bps_buffer[MAX_PERIOD];/* long values quantized in 1 bps */
	long          stats[MAX_LAYERS];       /* probability for each layer */
	long          cnt_frames;
	long          l_cur_bps;
	long          n_frames=DEF_FRAMES;
	long          period=DEF_PERIOD;
	long          minrate=DEF_MINRATE;
	long          framerate=DEF_FRAMERATE; 
	unsigned long seed;
	long          i,j,layer,items,sum_bps;
	double        avg_bps; 

	sum_bps=0;

	fprintf(stdout,"\n Command line:\n gen_rate_profile -layers=A,B,C,... ratefilename [period minrate [frames [seed] ] ]\n");

    if (argc < 2){
        display_usage (1);
    } else {
        while (argc > 1 && argv[1][0] == '-')
            /* read the -layers A,B,C,... option */
            if (strcmp (argv[1], "-layers") == 0 || strcmp (argv[1], "-LAYERS") == 0){
                /* find layer setup from command line */
                strncpy (layer_str,argv[2],MAX_STR);
                /*fprintf(stderr,"Layer(Rate) string:\"%s\"\n",layer_str);*/

                n_layers=parse_layers(layer_str, &(layer_b[0]));

                if(n_layers <= 0) {
                    HARAKIRI("Illegal layer string ",5);
                    exit(-1);
                }
                /* Move arg{c,v} over the option to the next argument */
                argc -=2;
                argv +=2;
            } else   if (strcmp (argv[1], "-framerate") == 0 || strcmp (argv[1], "-FRAMERATE") == 0){
                /* find frame rate from command line */
                
                framerate=atoi(argv[2]);

                if(framerate <= 1) {
                    HARAKIRI("Illegal framerate specified must be higher than 0\n",5);
                    exit(-1);
                }
                /* Move arg{c,v} over the option to the next argument */
                argc -=2;
                argv +=2;
            } else {
              fprintf(stderr,"Illegal option found on the command line. Exiting.\n");
              display_usage (0);
            }

    }

    if (n_layers<=0){
           HARAKIRI("No -layers A,B.C, ... option found on the command line. Exiting.\n",6);
    } else {
      /*display starting layer setup*/ 
      fprintf(stdout,"\n Starting layer_setup=[");
	  for(i=0;i<n_layers;i++){
	  	fprintf(stdout," %ld,",layer_b[i]);
      }
	  fprintf(stdout,"](bps)\n");
    }

    fprintf(stdout,"framerate =%ld (Hz)\n",framerate); 
     

	GET_PAR_S (1, "_Output rate file ...............: ", o_file);
	if ((Forf= fopen (o_file, WB)) == NULL){ HARAKIRI ("Could not create output rate file\n", 1);}

	FIND_PAR_L(2,"_Period .........................: ",period,DEF_PERIOD);
	FIND_PAR_L(3,"_Minrate ........................: ",minrate,layer_b[0]);
	FIND_PAR_L(4,"_Number of frames ...............: ",n_frames,DEF_FRAMES);
	FIND_PAR_L(5,"_Seed ...........................: ",seed,DEF_SEED);

	if(period>MAX_PERIOD){ HARAKIRI ("Too long period !!\n", 1); }

    /* truncate minrate according to frame rate*/
    if(minrate!=(minrate/(framerate))*framerate){
   		  fprintf(stderr,"Warning minrate  %ld, not divisible into %ld Hz frames, truncating \n", minrate,framerate);
        }
    minrate=(minrate/(framerate))*framerate;
	if(minrate>layer_b[n_layers-1]){ 
        fprintf(stderr,"Warning, very high minimum rate, all layers will have the same rate !!\n"); 
    }
    if(minrate < layer_b[0]){        
        fprintf(stderr,"Warning, very low minimum rate , has no effect!!\n");
    }

	/* set up/modify(quantize)/show layers according to minrate and framerate */
	fprintf(stdout,"Program: gen_rate_profile \n final layer_setup=[");
	for(i=0;i<n_layers;i++){
        if(layer_b[i] < (minrate)){
            fprintf(stderr,"Warning layer[%ld] rate %ld, is lower than minrate, increasing to minrate=%ld \n",i, layer_b[i],minrate);
            layer_b[i]=minrate ;
        };
        if(layer_b[i]!=(layer_b[i]/(framerate))*framerate){
   		  fprintf(stderr,"Warning layer[%ld] rate  %ld, not divisible into %ld Hz frames, truncating \n",i, layer_b[i],framerate);
        }
        layer_b[i]=layer_b[i]/(framerate);
		fprintf(stdout," %ld,",layer_b[i]*framerate);
		stats[i]=0;
    }
	fprintf(stdout,"](bps)\n");

	/* start of actual rate generation*/
	for(i=0; i < n_frames/period;i++){	
		layer = (long)((n_layers)*EID_random(&seed)); /*randomly pick a "layer" mode  among 0...(n_layers-1) */
		l_cur_bps = (layer_b[layer]*framerate); 
		for(j=0;j<period;j++){
			l_bps_buffer[j]=l_cur_bps;
		}
		/* Write output for the period */
		items = fwrite(l_bps_buffer,sizeof(l_cur_bps),period, Forf); /* long format as used by STL2005 truncate */		
		stats[layer]++;
		if (items < period) {KILL(o_file, 7); } /* Abort on write error */

		sum_bps += l_cur_bps; /* Av. stats may saturate for huge number of frames*/
		avg_bps = (double)sum_bps/(double)(i+1);
		fprintf(stderr,"\n segment[%ld], layer=%2ld, bps=%5ld, avg_bps=%10.2f",i,layer,l_cur_bps, avg_bps);
	}
    /* present stats */
	cnt_frames=0;
 	fprintf(stdout,"\nOverall Layer/(rate) stats:\n");
	for(i=0;i<n_layers;i++){
		fprintf(stdout,"  Number of L%ld(%ld bps) frames = %ld, (%4.2f %%)\n",i+1,(layer_b[i]*framerate),stats[i]*period,(double)stats[i]*period*100.0/(double)n_frames);
		cnt_frames += stats[i]*period;
	}
 
	fprintf(stderr,"\n (Total %ld frames, Avg bps=%10.2f)\n-- finished --\n", cnt_frames,avg_bps);
	fclose (Forf);
	return 0;
}
