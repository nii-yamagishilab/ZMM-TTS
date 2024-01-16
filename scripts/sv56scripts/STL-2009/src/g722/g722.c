/*                     v3.0 - 10/Jan/2007
  ============================================================================

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
       ** This code has  (C) Copyright by CNET Lannion A TSS/CMC **
       =============================================================


MODULE:         USER-LEVEL FUNCTIONS FOR THE UGST G.722 MODULE

ORIGINAL BY:
   Simao Ferraz de Campos Neto
   COMSAT Laboratories                    Tel:    +1-301-428-4516
   22300 Comsat Drive                     Fax:    +1-301-428-9287
   Clarksburg MD 20871 - USA              E-mail: simao.campos@labs.comsat.com
    
   History:
History:
~~~~~~~~
14.Mar.95  v1.0       Released for use ITU-T UGST software package Tool
                      based on the CNET's 07/01/90 version 2.00
01.Jul.95  v2.0       Changed function declarations to work with many compilers;
                      reformated <simao@ctd.comsat.com>
10.Jan.07  v3.0       Updated with STL2005 v2.2 basic operators
                      <{balazs.kovesi,stephane.ragot}@orange-ftgroup.com>
  ============================================================================
*/
#include "g722.h"
#include "stl.h"

void g722_reset_encoder(encoder)
g722_state *encoder;
{

  Word16          xl, il;
  Word16          xh, ih, j;

  xl = xh = 0;
#ifdef WMOPS
    move16();
    move16();
#endif
  FOR (j = 0; j < 24; j++)
  {
    encoder->qmf_tx_delayx[j] = 0;
#ifdef WMOPS
    move16();
#endif
  }
  il = lsbcod (xl, 1, encoder);
  ih = hsbcod (xh, 1, encoder);
}
/* .................... end of g722_reset_encoder() ....................... */


Word32 g722_encode(incode,code,read1,encoder)
  short *incode;
  short *code;
  Word32 read1;
  g722_state     *encoder;
{
  /* Encoder variables */
  Word16          xl, il;
  Word16          xh, ih;
  Word16          xin0, xin1;

  /* Auxiliary variables */
  Word32             i;
  Word16          *p_incode;

  /* Divide sample counter by 2 to account for QMF operation */
  read1 = L_shr(read1, 1);

  /* Main loop - never reset */
	p_incode = incode;
#ifdef WMOPS
    move16();
#endif
  FOR (i = 0; i < read1; i++)
  {
    xin1 = *incode++;
    xin0 = *incode++;
#ifdef WMOPS
    move16();
    move16();
#endif

    /* Calculation of the synthesis QMF samples */
    qmf_tx (xin0, xin1, &xl, &xh, encoder);

    /* Call the upper and lower band ADPCM encoders */
    il = lsbcod (xl, 0, encoder);
    ih = hsbcod (xh, 0, encoder);

    /* Mount the output G722 codeword: bits 0 to 5 are the lower-band
     * portion of the encoding, and bits 6 and 7 are the upper-band
     * portion of the encoding */
    code[i] = s_and(add(shl(ih, 6), il), 0xFF);
#ifdef WMOPS
    move16();
#endif
  }

  /* Return number of samples read */
  return(read1);
}
/* .................... end of g722_encode() .......................... */


void g722_reset_decoder(decoder)
g722_state *decoder;
{
  Word16          il, ih;
  Word16          rl, rh;
  Word16          j;

  il = ih = 0;
#ifdef WMOPS
    move16();
    move16();
#endif
  FOR (j = 0; j < 24; j++)
  {
    decoder->qmf_rx_delayx[j] = 0;
#ifdef WMOPS
    move16();
#endif
  }
  rl = lsbdec (il, (Word16)0, 1, decoder);
  rh = hsbdec (ih, 1, decoder);
}
/* .................... end of g722_reset_decoder() ....................... */


short g722_decode(code,outcode,mode,read1,decoder)
  short *code;
  short *outcode;
  short mode;
  short read1;
  g722_state     *decoder;
{
  /* Decoder variables */
  Word16          il, ih;
  Word16          rl, rh;
  Word16          xout1, xout2;
    
  /* Auxiliary variables */
  short             i;  
    
  /* Decode - reset is never applied here */
  FOR (i = 0; i < read1; i++)
  {
    /* Separate the input G722 codeword: bits 0 to 5 are the lower-band
     * portion of the encoding, and bits 6 and 7 are the upper-band
     * portion of the encoding */
    il = s_and(code[i], 0x3F);	/* 6 bits of low SB */
    ih = s_and(lshr(code[i], 6), 0x03);/* 2 bits of high SB */

    /* Call the upper and lower band ADPCM decoders */
    rl = lsbdec (il, mode, 0, decoder);
    rh = hsbdec (ih, 0, decoder);
      
    /* Calculation of output samples from QMF filter */
    qmf_rx (rl, rh, &xout1, &xout2, decoder);

    /* Save samples in output vector */
    *outcode++ = xout1;
    *outcode++ = xout2;
#ifdef WMOPS
    move16();
    move16();
#endif      
  }
    
  /* Return number of samples read */
  return(shl(read1,1));
}
/* .................... end of g722_decode() .......................... */

