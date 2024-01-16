/* Version 0.3    Last modified: July 26 2006, Jonas Svedberg */

/* ------------------------------------------------------------------------ */
/*                             MAIN PROGRAM                                 */
/* ------------------------------------------------------------------------

g729e_convert_synch

Background:
 The G729E reference decoder does not accept other synchs than SYNCH_WORD(G192_SYNC)
 in the G192 word oriented bitstream. The STL tools EID-XOR and EID-EV write a 
 G192_FER tag for erased frames.
 Also to make G729(Main,A,D) accept EID-EV output, a zeroed payload 
 has to be created when the corrupted frame has zero payload length.

----------------------------------
 G729E Bitstream information - all parameters start with msb
 The bits are defined as follows:
 #define SYNC_WORD (short)0x6b21   (=G192_SYNC)
 #define BIT_0     (short)0x007f 
 #define BIT_1     (short)0x0081 
 A bad frame is indicated by setting all payload bits to zero. 

---Actual g729E c-code--- 
 if(serial[0] != SYNC_WORD ) {
   printf("error reading synchro word file %s frame %hd\n",
   argv[1], frame);
   exit(-1);
}
------------------------------------
This program converts G.192 word oriented analysis frames without SYNCH_WORD 
into a G.192 word oriented file with SYNCH_WORD and zeroed payload bits.
Also zero length frames (with header G192_FER or G192_SYNC) 
are converted to non-zero length G192_SYNC frames with zeroed payload.  

Note: G729D and G729/G729A/G729B does not check the synch_word, 
they merely check that all analysis bits in the frame are set to zero
to declare a frame erasure(FER). I.e. G729D/G729/G729A works ok with eid-xor.
however they do not work with eid-ev, since eid-ev has a zero payload for the G192_FER frames

Compilation (in the STL eid directory):
 gcc g729e_convert_synch.c ../eid/softbit.c -I../eid -I ../utl -o g729e_convert_synch

Example execution:
 ./g729e/fix/codere  file.smp file.g729e  1
 # optional error patter creation 
 #\rm sta; ./gen-patt -reset -fer -g192 -gamma 0 -rate 0.10 -n 609  f.10.g192
 ./eid-xor -fer -ep g192 -bs g192 file.g729e f.10.g192 file.g729e.f.10
 ./g729e_convert_synch file.g729e.f.10 file.g729e.f.10.all_synch quiet
 ./g729e/fix/decodere file.g729e.f.10.all_synch file.g729e.f.10.all_synch.smp
-----------------------------------------------------------------       

Note for DTX operation and different Error insertion devices(EID-XOR/EID-EV):
   When inserting Errors EID-XOR tags NO_DATA frames(G192_SYNC,length 0) as FER frames (G192_FER,length 0),
   When inserting errors EID-EV maintains NO_DATA frames(G192_SYNC,length 0) as NO_DATA frames (G192_SYNC,length 0).
   ( This causes problems because the G729 decoders does not handle these frametypes  exactly the same way. 
      causing minor synthesis differences.)

*/

#include <stdlib.h>
#include <stdio.h>
/* ..... OS-specific include files ..... */
#if defined (unix) && !defined(MSDOS)
/*  ^^^^^^^^^^^^^^^^^^ This strange construction is necessary
for DJGPP, because "unix" is defined, even it being MSDOS! */
#if defined(__ALPHA)
#include <unistd.h>		/* for SEEK_... definitions used by fseek() */
#else
#include <sys/unistd.h>		/* for SEEK_... definitions used by fseek() */
#endif
#endif

#include "softbit.h"

#define NB_BITS_E      118
#define NB_BITS_A      80
#define NB_BITS_D      64
#define NB_BITS_B      16
#define SERIAL_SIZE_E (NB_BITS_E+2)  
#define SYNC_WORD      G192_SYNC


/*-----------------------------------------------------------------*
*            Main routine                                 *
*-----------------------------------------------------------------*/

int main(int argc, char *argv[] )
{
   /* short rate; */
   short  serial[SERIAL_SIZE_E];            /* Serial stream     */
   short  serial_out[SERIAL_SIZE_E];        /* Serial stream     */
   char   type,tmp_type;

   long  i, zeroed,fer,quiet;
   long  frame,nb_fer_in,nb_fer_zeroed_in,nb_no_data_in,nb_fer_out;
   short  serial_size;
   short  serial_size_mem;
   FILE    *f_serial_out, *f_serial;

   frame=0;
   nb_fer_in=0;         /* G192_FER header */
   nb_fer_zeroed_in=0;  /* zeroed payload */
   nb_no_data_in=0;     /* zero length payload and G192_SYNC */
   nb_fer_out=0;        /*  G192_FER or zeroed payload or (zero length payload and G192_SYNC) */
   quiet = 0;
   serial_size_mem=-1;

   printf("\n");
   printf("------------------- g729e_convert_synch  -----------------\n");
   printf("-------------------     Version 0.3      -----------------\n");
   printf("(Transforms output of EID-XOR/EID-EV for G729(Main,A,B,D,E))\n");
   printf("Supports: 0,1.6(G729B),6.4(G729D),8.0(G729),11.8(G729E)kb/s\n");
   printf("Note: non-transmitted frames(G192_SYNC,length 0) are tagged\n");
   printf("      differently by EID-XOR and EID-EV, causing slightly  \n");
   printf("      differing synthesis output for error cases\n");
   printf("\n");

   /* Passed arguments */
   if ( (argc != 3) && (argc !=4 )) {
      printf("Usage : g729E_convert_synch  bitstream_file bitstream_file_out [quiet] \n");
      printf("\n");
      printf("Format for bitstream_file:\n");
      printf("  One (2-byte) synchronization word,\n");
      printf("  One (2-byte) bit-rate word,\n");
      printf("  N   (2-byte) softbit words, where N=bit-rate word\n");
      printf("\n");
      printf("Format for bitstream_file_out:\n");
      printf("  (Same as bitstream_file)\n");
      printf("Options:  \n");
      printf("   Supplying the 'quiet' argument, reduces debug printouts\n\n");
      exit( 1 );
   }


   /* Open file for synthesis and packed serial stream */
   if( (f_serial = fopen(argv[1],"rb") ) == NULL ) {
      printf("%s - Error opening file  %s !!\n", argv[0], argv[1]);
      exit(0);
   }
   if( (f_serial_out = fopen(argv[2], "wb") ) == NULL ) {
      printf("%s - Error opening file  %s !!\n", argv[0], argv[2]);
      exit(0);
   }

   printf("Input bitstream file    :   %s\n",argv[1]);
   printf("Output bistream file    :   %s\n",argv[2]);
   if (argc==4) {
      quiet=1;
      printf("Quite operation mode  \n\n");
   }

   /*-----------------------------------------------------------------*
   *           Initialization                             *
   *-----------------------------------------------------------------*/

   /* Do preliminary inspection in the INPUT BITSTREAM FILE to check
   its format (byte, bit, g192) */
   type = check_eid_format(f_serial, argv[1], &tmp_type);

   /* Check whether the specified BS format matches with the one in the file */
   if (type != g192) {
      /* The input bitstream format is not g192 */
      fprintf (stdout, "*** bitstream format is not \"%s\" (seems like \"%s\" format ...) ***\nExiting\n",
         format_str((int)g192), format_str(type));
      exit(0);
   }

   /*-----------------------------------------------------------------*
   *            Loop for each FRAME                                  *
   *-----------------------------------------------------------------*/
   while( fread(serial, sizeof(short), 2, f_serial) == 2) {
      frame++;
      fer=0;
      zeroed=1;
      for (i=0; i < SERIAL_SIZE_E; i++){
         serial_out[i] = 0;
      }

      serial_size = serial[1];

      if (fread(&serial[2], sizeof(short), serial_size, f_serial) != (size_t)serial_size) {
         printf("Error reading input bitstream file %s, frame %ld\n",
            argv[1], frame);
         exit(-1);
      }

      if(serial_size==0){
	if(serial[0]==G192_SYNC){
	  /* non-transmitted DTX frame */
          /* allow G192_SYNCH with zero payload length ("NO_DATA frame")*/
          if(!quiet){
            printf("\nDTX::zero length payload, synch word(%x) in file %s , frame %ld ",
		   serial[0], argv[1], frame);
	  }
	}  else { /* not synch */
	  /* allow G192_FER with zero payload length   (alternative "FER-frame")   */
	  /* but convert these to a G192_SYNC frame with zeroed payload bits) */
	  if(serial_size_mem < 0){
             serial_size = NB_BITS_A; /* assume G729A/main frame size, when unknown */
	  } else {
             serial_size = serial_size_mem; /* use old speech frame size, FILE  SIZE MAY CHANGE !! */
	  }
	}
      } else { /* payload exists */
	if( (serial_size != NB_BITS_E) && (serial_size != NB_BITS_A) && (serial_size != NB_BITS_D) && (serial_size != NB_BITS_B)){
	   printf("Error:: incorrect value of serial_size (%d) in %s frame %ld\n",
		 serial_size, argv[1], frame);
	   exit(-1);
	}
	if(serial_size >= NB_BITS_D){
	   serial_size_mem = serial_size; /* memorize speech frame size */
	}
      }
      serial_out[1] = serial_size;

      /* compute input statistics  and set fer_flag for output */
      if(serial[0] != G192_SYNC ) {
         nb_fer_in++;
         if(!quiet){
            printf("\nFER::non synch word(%x) in file %s , frame %ld ",
               serial[0], argv[1], frame);
         }
         fer=1;
      }

      if((serial[0] == G192_SYNC ) && (serial[1]==0))  {
         nb_no_data_in++;
      }

      if ((serial[0]==G192_SYNC ) && (serial[1]!=0)) {
         zeroed = 1; /*assume zeroed payload */
         for(i=0;i<(serial[1]);i++){
            if (serial[2+i] != 0 ) {
               zeroed = 0; /* frame contains different soft info, is not a FER tagged frame */
               break;
            } ;
         }
         if(zeroed){
            nb_fer_zeroed_in++;
            if(!quiet){
               printf("\nFER::zeroed payload, synch word(%x) in file %s , frame %ld ",
                  serial[0], argv[1], frame);
            }
            fer=1;
         }
      }

      /* apply fer using zeroed payload */
      if(fer){
         serial_out[0]=G192_SYNC;
         serial_out[1]=serial_size;
         for (i=2; i < (serial_size+2); i++){
            serial_out[i] = 0; /* zero payload bits ( uncertainty frame with synch) */
         }
         if(!quiet){
            printf("-> Erased output frame[%ld]: size=%d ", frame,serial_size);
         }
         nb_fer_out++;
      } else {
         serial_out[0]=G192_SYNC;
         serial_out[1]=serial_size; 
         for (i=2; i < (serial_size+2); i++){
            serial_out[i] = serial[i]; /* copy frame */
         }
         if(!quiet){
	   if(serial_size > 0){
	     printf("\nSP/SID frame[%ld]: insize=%d, outsize=%d ", frame,serial[1],serial_size);
	   }
	 }
      }

      if(fwrite(serial_out, sizeof(short), serial_size+2, f_serial_out)!=(serial_size+2)){
         printf("Error writing output bitstream file %s, frame %ld\n",
            argv[1], frame);
         exit(-1);
      }
   }
   printf("\n--finished\n");
   printf("\n processed_frames =%ld",frame);
   if(!quiet){
      printf("\n nb_fer_in         =%ld",nb_fer_in);
      printf("\n nb_fer_zeroed_in  =%ld",nb_fer_zeroed_in);
      printf("\n nb_no_data_in(DTX)=%ld",nb_no_data_in);
      printf("\n Sum(fer_*_in)     =%ld",nb_fer_in+nb_fer_zeroed_in);
      printf("\n *****************");
   }
   printf("\n nb_fer_out       =%ld\n",nb_fer_out);   
   return(0);
}

