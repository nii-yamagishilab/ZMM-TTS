/*                                                          02.Feb.2010 v3.0
============================================================================

DECG722.C 
~~~~~~~~~

Description: 
~~~~~~~~~~~~

Demonstration program for UGST/ITU-T G.722 module with the G.722 
decoding function.

The default input bitsstream format follows the G.192 16 bit format. 
G.722 bits in a  G.192 bistream  are ordered so that the frame can be 
truncated at 56 and 48 kbps.
The legacy non-g192 7kHz ADPCM bitstream is left-adjusted, 
i.e., the codewords are located in the lower 8-bits of the encoded bitstream file. 
The MSB is always 0 for the legacy bitstream file.

Output data is aligned at word boundaries, i.e.,
organized in 16-bit words, following the operating system normal
organization (low-byte first for VMS and DOS; high byte first for most
Unix systems). Output linear samples are supposed to be 16-bit right-adjusted.

Usage:
~~~~~~
$ DECG722 [-options] InpFile OutFile

where:
InpFile     is the name of the bit stream file ;
OutFile     is the name of the file  with synthesized speech;

Options:

-mode #     is the operation mode  for the G.722 decoder.
            Default is mode 1/64 kbit/s.
            others are mode 2/56 kbps and mode3/48 kbps
-fsize #    Number of samples per frame  
            Default is 160, 16 kHz samples (or 10ms) 
-frames #   Number of frames to process
-plc #      (0 = zero index insertion, no decoder state reset)
            (1 = zero index insertion, decoder state reset)
            (2 = previous frame repetition, no decoder state reset)
            (3 = previous frame repetition, a few zero_indeces in first good frame ,no decoder state reset)
            
-byte       Use legacy nonG192 G.722 format (byte oriented) without frame/synch headers.
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
                      <{balazs.kovesi,stephane.ragot}@orange-ftgroup.com>
02.Feb.10  v3.1       Implicit casting of toupper() argument removed.
                      (y.hiwasaki)
============================================================================
*/

/* Standard prototypes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* local prototypes */
void display_usage ARGS((void));

short g192_to_byte(short nb_bits, short *code_byte, short *inp_bits, short N_samples);
void set_index(short value, short *code,short n_bytes);
void copy_index (short *from, short *to, short n_bytes);

short g192_to_byte(short nb_bits, short *code_byte, short *inp_bits, short N_samples){
   long i,j,k;
   short bit,mask,mode=-1;;
   set_index(0,code_byte,(short)(N_samples/2));

   /* convert soft bits value [-127 ... +127] range to hard bits (G192_ONE or G192_ZERO) */
   for(i=0;i < nb_bits;i++){      
      if(inp_bits[i]&0x0080){ /* look at sign bit only */
         inp_bits[i] = G192_ONE;
      } else {
         inp_bits[i] = G192_ZERO;
      }
   } 

   if(nb_bits==(N_samples/2)*8) { mode=1;};
   if(nb_bits==(N_samples/2)*7) { mode=2;};
   if(nb_bits==(N_samples/2)*6) { mode=3;};

   if(nb_bits==0){
      mode = -1; /* flag for NO_DATA or BAD SPEECH frame, without payload */
   } else {
      /* special ordering for scalability reasons */
      /* write [ b2*n, b3*n, b4*n, b5*n, b6*n, b7*n, b1*n, b0*n]  to enable truncation of G.722 g192 frames */ 
      /*  "core"/non-scalable layer is read first b2s-b3s-b4s-b5s */
      /* b6s,b7s are semi scalable, (b6,b7) scalability is not part of the g.722 standard */
      j=0;
      for (bit=2;bit<8;bit++){
         mask=(0x0001<<(bit));
         for(i=0; i < (N_samples/2); i++,j++){
            if(inp_bits[j] == G192_ONE){
               code_byte[i] |= mask; /* set bit */  
            } 
            /* leave bit zero */
         }
      }

      /* embedded layers last in G.192 frame */
      /* read b1s followed by b0s if available */
      k=0;                /* 64 kbps */
      if(mode==2) {k=1;}; /* 56 kbps */
      if(mode==3) {k=2;}; /* 48 kbps*/
      for (bit=1;bit>=k;bit--){
         mask=(0x0001<<(bit)); 
         for(i=0; i < (N_samples/2); i++,j++){
            if(inp_bits[j] == G192_ONE){
               code_byte[i] |= mask;
            } 
         }
      }
   }
   return mode;
}

void set_index(short value,short *code,short n_shorts){
   long i;
   FOR(i=0;i<n_shorts;i++){
      code[i]=value;
#ifdef WMOPS
    move16();
#endif
   }
}

void copy_index (short *from, short *to, short n_shorts){
   long i;
   FOR(i=0;i<n_shorts;i++){
      to[i]=from[i];
#ifdef WMOPS
    move16();
#endif
   }
}


#define P(x) printf x
void display_usage()
{
   /* Print Message */
   printf ("\n\n");
   printf ("\n***************************************************************");
   printf ("\n* PROCESSING THE DECODER OF ITU-T G.722 WIDEBAND SPEECH CODER *");
   printf ("\n* COPYRIGHT CNET LANNION A TSS/CMC Date 24/Aug/90             *");
   printf ("\n* COPYRIGHT Ericsson AB.           Date 22/May/06             *");
   printf ("\n* COPYRIGHT France Telecom R&D     Date 23/Aug/06             *");
   printf ("\n***************************************************************\n\n");

   /* Quit program */
   P(("USAGE: \n"));
   P(("  decg722 [-options] file.adp file.outp \n"));
   P(("or \n"));
   P(("  decg722 [-mode ] [-plc] [-fsize N] [-byte] [-frames N2] file.adp.g192 file.outp \n\n"));

   exit(-128);
}
#undef P



/*
**************************************************************************
***                                                                    ***
***        Demo-Program for decoding G.722 frames with errors          ***
***                                                                    ***
**************************************************************************
*/
int             main (argc, argv)
int argc; 
char *argv[];
{
   /* Local Declarations */
   short  mode_arg = 1;  /* input argument mode*/
   short  local_mode = 1; /* actual decoder synthesis mode*/

   g722_state      decoder;

   /* Encode and decode operation specification */
   /* Sample buffers */
   short  code[MAX_BYTESTREAM_BUFFER];   /* byte stream buffer , 1 byte(8 bit) at  8 kHz */
   short  outcode[MAX_OUTPUT_SP_BUFFER]; /* speech buffer */
   short  inp_frame[2+MAX_BITLEN_SIZE]; /* g192 inpFrame , 1 bit is stored per 16 bit Word*/
   short  mem_code[PLC_MEM_SIZE][MAX_BYTESTREAM_BUFFER];   /* index history byte stream buffer , 1 byte(8 bit) at  8 kHz */
   short  mem_mode[PLC_MEM_SIZE];   /* decoding mode memory */

   long   debug=0;
   long   g192_inp=1;
   long   plc_alg=0;
   long   frames=1; /* frame counter */

   /* File variables */
   char            FileIn[MAX_STR], FileOut[MAX_STR];
   FILE            *F_cod, *F_out;
   long            iter=0;
   short            N=DEF_FR_SIZE, N2=0, smpno=0;
   long            i=0;
   char            bs_format = g192;
   char            tmp_type;   /* for input type checking*/
   short           header[2];  /* g192 header size*/
   long            fr_size[N_MODES+1]; /* analysis frame sizes in bits for every mode */
   short           mode_frame; /* mode for the current input frame */
   long            fer_cnt=0;  /* counter of consecutive FERs */
   long            good_cnt=0; /* counter of consecutive SYNCH frames  */
   long            mem_zero=0; /* nb_samples to mute in initial part of good frame after erasure*/ 

#ifdef WMOPS
    short spe1Id = -1;
    short spe2Id = -1;
#endif

#ifdef VMS
   char            mrs[15];
#endif

   /* Progress flag indicator */
   static char     quiet=0, funny[9] = "|/-\\|/-\\";


   /* *** ......... PARAMETERS FOR PROCESSING ......... *** */
   /* GETTING OPTIONS */
   if (argc < 2)
      display_usage();
   else {
      while (argc > 1 && argv[1][0] == '-')
         if (strcmp(argv[1], "-plc") == 0){
            /* Define plc algorithm */
            plc_alg = atoi(argv[2]);
            if(( plc_alg < 0) || (plc_alg > MAX_PLC)) {
               plc_alg = 0;
               fprintf(stderr, "Warning! Invalid plc \"%s\" in command line\n\n using \"-plc 0\"\n",argv[2]);	
            }
            /* Move argv over the option to the next argument */
            argv+=2;
            argc-=2;
         }  else if (strcmp(argv[1], "-frames") == 0){
            /* Define number of frames to process */
            N2 = atoi(argv[2]);
            if(( N2 <= 0)) {
               N2 = 0;
            }
            /* Move argv over the option to the next argument */
            argv+=2;
            argc-=2;
         } else if (strcmp(argv[1], "-byte") == 0) {
            /* g.192 input operation or not */
            g192_inp = 0;
            /* Move argv over the option to the next argument */
            argv++;
            argc--;
         }  else if (strcmp(argv[1], "-mode") == 0) {
            /* Define decoder (b0, b1 truncation) operation mode*/
            mode_arg = atoi(argv[2]);
            switch (mode_arg){
               case 1:
               case 64: mode_arg=1; break;
               case 2:
               case 56: mode_arg=2; break;
               case 3:
               case 48: mode_arg=3; break; 
               default:
                  fprintf(stderr, "ERROR! Invalid mode \"%s\" in command line\n\n",argv[2]);
                  display_usage();
            }
            /* Move argv over the option to the next argument */
            argv+=2;
            argc-=2;
         } else if (strcmp(argv[1], "-debug") == 0) {
            /*Debug printouts */
            debug = 1;
            /* Move argv over the option to the next argument */
            argv++;
            argc--;
         } else if (strcmp(argv[1], "-fsize") == 0){
            /* Define Frame size for g192 operation and file reading */
            N = atoi(argv[2]);
            if(( N > MAX_OUTPUT_SP_BUFFER) || (N <=0) || (N&0x0001)) {
               fprintf(stderr, "ERROR! Invalid frame size \"%s\" in command line\n\n",argv[2]);
               display_usage();
            }
            /* Move argv over the option to the next argument */
            argv+=2;
            argc-=2;
         } else if (strcmp(argv[1], "-q") == 0) {
            /* Don't print progress indicator */
            quiet = 1;

            /* Move argv over the option to the next argument */
            argv++;
            argc--;
         } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0){
            /* Print help */
            display_usage();
         } else {
            fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
               argv[1]);
            display_usage();
         }
   }

   /* Now get regular file parameters */
   GET_PAR_S(1, "_Input File: .................. ", FileIn);
   GET_PAR_S(2, "_Output File: ................. ", FileOut);

   if(debug) {
      fprintf(stderr, " fsize=%ld\n",(long)N);
      fprintf(stderr, " mode=%ld\n",(long)mode_arg);
      fprintf(stderr, " g192=%ld\n",g192_inp);
      fprintf(stderr, " frames=%ld\n",(long)N2);
      fprintf(stderr, " plc=%ld\n",plc_alg);
   }

   /* check decompression mode_arg, */
   if (mode_arg < 1 || mode_arg > N_MODES) {
      HARAKIRI("Bad mode specified; aborting\n", 2);
   }
   /* Open input file */
   if ((F_cod = fopen (FileIn, RB)) == NULL){
      KILL(FileIn, -2);
   }
   /* Open output file */
   if ((F_out = fopen (FileOut, WB)) == NULL) {
      KILL(FileOut, -2);
   }

   /* initialization */
   /* set frame sizes in bits for the possibe  modes(1(64kbps),2(56 kbps),3(48 kbps)) */
   for(i=1;i<(N_MODES+1);i++){
       fr_size[i]=(N/2)*(8+1-i);
   }
   fr_size[0] = -1; /* unused entry */

   /* init complete PLC index memory */
   for (i=0;i<PLC_MEM_SIZE;i++){
      set_index(ZERO_INDEX,mem_code[i],(short)(fr_size[1]/8));
      mem_mode[i]=1;
   }

   /* Reset lower and upper band encoders */
   g722_reset_decoder(&decoder);

#ifdef WMOPS
    setFrameRate(16000, N);
    spe1Id = getCounterId("lost frame processing");
    setCounter(spe1Id);
    Init_WMOPS_counter();
    spe2Id = getCounterId("received frame processing");
    setCounter(spe2Id);
    Init_WMOPS_counter();
#endif

   /* Read an analysis frame of bits from input bit stream file and decode */
   if(g192_inp){
      /* for g192 inputs, the synch header and frame size has to be read first */
      /* then the soft bits may be read if they are available */
      /* bits are stored b2s,b3s,b4s,b5s,b6s,b7s and b1s,b0s  to allow frame truncation*/

      /* Do preliminary inspection in the INPUT BITSTREAM FILE to check
      that it has a correct format (g192) */
      i = check_eid_format(F_cod, FileIn, &tmp_type);
      /* Check whether the  required input format matches with the one in the file */
      if (i != bs_format) {
         /* The input bitstream format is not g192 */
         fprintf (stderr, "*** Illegal input bitstream format: %s (should be %s) ***\n",
            format_str(i),format_str((int)bs_format));
         HARAKIRI("\nExiting...\n\n",1);
      }
      {  /* check input file for valid initial G.192 synchronism headers */
         short sync_header=0;
         fread(header, sizeof(short), 2, F_cod); /* Get presumed first G.192 sync header */
         i = header[1]; /* header[1] should have the frame length */
         /* advance file to the (presumed) next G.192 sync header */
         fseek(F_cod, (long)(header[1])*sizeof(short), SEEK_CUR);
         fread(header, sizeof(short), 2, F_cod);  /* get (presumed) next G.192 sync header */

         if ((header[0] & 0xFFF0) == (G192_FER & 0xFFF0)) { /* Verify */
            sync_header = 1;
         }    
         fseek(F_cod, 0l, SEEK_SET); /* Rewind BP file */
         if(sync_header==0){
            HARAKIRI("Error::Input bitstream MUST have valid G192 sync_headers \n\n",1);
         }
      }
      /* start actual g.192 frame loop */
      while (fread (header, sizeof (short), 2, F_cod) == 2) {
         if(!((short)fread (inp_frame, sizeof (short), header[1], F_cod) == header[1])){
            HARAKIRI("Error::Could not read complete frame of input bitstream  \n\n",1);
            break;
         } else if (!((header[1]==0) || (header[1]==fr_size[1]) || (header[1]==fr_size[2])  || (header[1]==fr_size[3]))) {
            HARAKIRI("Error::invalid input frame size in bitstream, only (0,48,56,64) kbps are allowed)\n\n",1);
            break;
         } else {          /* normal decoding */
            if (!quiet && !debug){ /* progress character*/
               fprintf(stderr, "%c\r", funny[(iter/N/2) % 8]);
            } 
            if(debug){
               fprintf(stderr, "frame=%ld\n", frames);
            }
            if(debug){
               fprintf(stderr,"read %d samples from frame\n", header[1]);
            }

            mode_frame = g192_to_byte(header[1],code,inp_frame,N);

            if(mode_frame == -1){
               local_mode = mode_arg; /* use command line synthesis mode for zero frame length frames*/ 
            } else {
               local_mode = mode_frame;
            }

            /* PLC pre decoding index operations */
            if(header[0] != G192_SYNC){ /* bad frame, (with zero length or valid G.722 length) */ 
#ifdef WMOPS
              setCounter(spe1Id);
              fwc();
              Reset_WMOPS_counter();
#endif
              fer_cnt++;
              good_cnt=0;
              if(debug){ fprintf(stderr,"PLC-FER fer_cnt=%ld \n",fer_cnt); };
              
              switch(plc_alg){
              case 0:  /* zero index   */ 
              case 1:  /* fall through */
                set_index(ZERO_INDEX,code,(short)(fr_size[1]/8));
                local_mode=1;
#ifdef WMOPS
    move16();
#endif
                break;
              case 2: /* frame buffer repeat */
              case 3: /* fall through        */ 
                IF(fer_cnt <= PLC_MEM_SIZE){
                  copy_index(mem_code[fer_cnt-1],code,(short)(fr_size[mem_mode[fer_cnt-1]]/8));
                  local_mode=mem_mode[fer_cnt-1];
                } else {
                  set_index(ZERO_INDEX,code,(short)(fr_size[1]/8));
                  local_mode=1;
                }
#ifdef WMOPS
    move16();
#endif
                break;
              default:
                break;
              }
              /* index memory buffer mem_code is frozen during erasures */
              /* but for longer bursts all memory indexes are set to zero */
              
              IF(L_sub(fer_cnt, PLC_MEM_SIZE) > 0){
                /*fprintf(stderr,"PLC: long burst, setting [fer_cnt==%ld] setting code_mem[*] to ZEROINDEX\n",fer_cnt); */
                FOR (i=0;i<PLC_MEM_SIZE;i++){
                  set_index(ZERO_INDEX,mem_code[i],(short)(fr_size[1]/8));
                  mem_mode[i]=1;
#ifdef WMOPS
    move16();
#endif
                }
              }
            } else {  /* good frame, update index memory mem_code and mode memory mem_mode */
#ifdef WMOPS
                setCounter(spe2Id);
                fwc();
                Reset_WMOPS_counter();
#endif
              good_cnt++;
              fer_cnt=0;
              FOR (i=(PLC_MEM_SIZE-1);i>=1;i--){
                set_index(ZERO_INDEX,mem_code[i],(short)(fr_size[1]/8));  /*reset all bytes*/
                copy_index(mem_code[i-1],mem_code[i],(short)(fr_size[mem_mode[i-1]]/8));
                mem_mode[i]= mem_mode[i-1];
#ifdef WMOPS
    move16();
#endif
              }
              copy_index(code,mem_code[0],(short)(fr_size[local_mode]/8));               /* save incoming good frame */
              mem_mode[0] = local_mode;
#ifdef WMOPS
    move16();
#endif
              
              set_index(ZERO_INDEX,code,(short)(mem_zero/2)); /* handle potential initial mute in first good frame */
#ifdef WMOPS
    move16();
#endif
              mem_zero=0; 
            }
         }

         smpno = g722_decode(code, outcode, local_mode, (short)(N/2), &decoder);  
         
         /* PLC post frame synthesis operations   */
         /* preparations for upcoming good frame  */
         if(header[0] != G192_SYNC){
#ifdef WMOPS
              setCounter(spe1Id);
#endif
           mem_zero = 0;
           switch(plc_alg){
           case 1: 
             g722_reset_decoder(&decoder); 
             break; 
           case 2:
             mem_zero=0;
#ifdef WMOPS
    move16();
#endif
             break;
           case 3:
             IF(fer_cnt <= (PLC_MEM_SIZE)){
               mem_zero = 10;  /* nb of silence samples in initial part of next good frame*/
#ifdef WMOPS
    move16();
#endif
             }
             break;
           default:
             break;
           }
         }
         /* Update sample counter */
         iter += smpno;
         /* Save a frame of decoded speech samples */
         if ((short)fwrite ((char *) outcode, sizeof (Word16), N, F_out) != N) {
            KILL(FileOut,-4);
         }
         if (debug) { fprintf(stderr, "*****************\n");};
         frames++;
         if( (N2!=0) && (frames > N2) ){
            break;
         }
      } /* end while fread */
#ifdef WMOPS
      setCounter(spe1Id);
      fwc();
      WMOPS_output(0);
      setCounter(spe2Id);
      fwc();
      WMOPS_output(0);
#endif
   } else {
      /* for non g192 input, simply  read input bits in chunks of bytes of N/2 bytes stored in words, 
      always read 64 kbps */
      while ((short)fread ((char *) code, sizeof (short), (N/2), F_cod) == (N/2)) {
         /* print progress flag */
         if (!quiet && !debug){
            fprintf(stderr, "%c\r", funny[(iter/N/2) % 8]);
         } 
         if(debug){ fprintf(stderr, "frame=%ld\n", frames);};

         /* No ecu algorithm possible since there are no FER flags */

         /* always decode N samples */
         local_mode = mode_arg; /* mode from command line */ 
         smpno = g722_decode(code, outcode, local_mode, (short)(N/2), &decoder);  
         /* Update sample counter */
         iter += smpno;

         /* Save decoded samples */
         if ((short)fwrite ((char *) outcode, sizeof (Word16), N, F_out) != N) {
            KILL(FileOut,-4);
         }
         if (debug) { fprintf(stderr, "*****************\n");};
         frames++; 
         if( (N2!=0) && (frames>N2) ){
            break;
         }
      }
   }

   /* Close input and output files */
   fclose(F_out);
   fclose(F_cod);

   /* Exit with success for non-vms systems */
#ifndef VMS
   return (0);
#endif
}
