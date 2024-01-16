/*                                                          v1.0 - 12/Apr/1994
  =============================================================================

                          U    U   GGG    SSSS  TTTTT
                          U    U  G       S       T
                          U    U  G  GG   SSSS    T
                          U    U  G   G       S   T
                           UUU     GG     SSS     T

                   ========================================
                    ITU-T - USER'S GROUP ON SOFTWARE TOOLS
                   ========================================

MODULE:         RPELTP.C - GSM 06.10 RPE-LTP SPEECH ENCODING ALGORITHM

ORIGINAL BY:

 The driving routines of this module were written by:
   Simao Ferraz de Campos Neto
   DDS/Pr11                      Tel: +55-192-39-6637
   CPqD/Telebras                 Fax: +55-192-39-6125
   13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>
   
 The true implementation was done by
   Jutta Deneger (jutta@cs.tu-berlin.de)
   Carsten Borman (cabo@cs.tu-berlin.de)
   Communications and Operating Systems Research Group (KBS) 
   Technishe Universitaet Berlin
  
 The portability changes on the code and rearrangement were done by <simao> 
  
DATE:           12/Apr/1994: 18H00 -3H00GMT

RELEASE:        1.00

PROTOTYPES:     see rpeltp.h for driving-routine prototypes, gsm.h for 
		general prototypes, and private.h for special definitions.

OBSERVATION:
  Copyright note by the original authors:
  " Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
    Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
    details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE."

FUNCTIONS: (Only UGST user-level functions!)

rpeltp_init() ......... Initializes the state variable for the encoding or
			the decoding algorithm. Each need a separate state
			variable!!!

rpeltp_delete ......... Delete the state variable allocation, etc (an alias).

rpeltp_encode ......... Entry-level function for the encoding algorithm.

rpeltp_decode ......... Entry-level function for the decoding algorithm.

============================================================================
*/
#include <stdio.h> /* For NULL */
#include "private.h"
#include "gsm.h"
#include "rpeltp.h"

/* *** Alias functions *** */
/*
  ------------------------------------------------------------------------
  void rpeltp_delete(gsm_state *st);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  Description:
  ~~~~~~~~~~~~
  Frees memory of a state variable previously initialized by a call to 
  rpeltp_init(). This is an alias to the function gsm_destroy() implemented 
  by the original authors of the module.
  
  Variables:
  ~~~~~~~~~~
  A pointer to a valid encoder or decoder state variable.
  
  Return value:
  ~~~~~~~~~~~~~
  None. 
  
  Prototype: in gsm.h
  ~~~~~~~~~~
  
  Original author:
  ~~~~~~~~~~~~~~~~
   Simao Ferraz de Campos Neto
   DDS/Pr11                      Tel: +55-192-39-6637
   CPqD/Telebras                 Fax: +55-192-39-6125
   13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>
   
  Log of changes:
  ~~~~~~~~~~~~~~~
  12.Apr.94  v.1.0	created.

  ------------------------------------------------------------------------
*/
/* #define rpeltp_delete gsm_destroy / * This is already done in rpeltp.h !!! */
/* ................... End of rpeltp_delete() ......................... */


/* *** True functions *** */

/*
  ------------------------------------------------------------------------
  gsm_state *rpeltp_init(void);
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
  Description:
  ~~~~~~~~~~~~
  Allocates memory for the state variable of the encoder or the decoder,
  and resets flags to proper initial values. This includes processing
  strictly according to the Rec. GSM 06.10.
  
  In a encoder plus decoder calling program there must exist different 
  state variables for the encoder and for the decoder.
  
  Variables:
  ~~~~~~~~~~
  None.
  
  Return value:
  ~~~~~~~~~~~~~
  Returns a pointer to a valid encoder or decoder state variable. On 
  failure, returns NULL.
  
  Prototype: in rpeltp.h
  ~~~~~~~~~~
  
  Original author:
  ~~~~~~~~~~~~~~~~
   Simao Ferraz de Campos Neto
   DDS/Pr11                      Tel: +55-192-39-6637
   CPqD/Telebras                 Fax: +55-192-39-6125
   13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>
   
  Log of changes:
  ~~~~~~~~~~~~~~~
  12.Apr.94  v.1.0	created.

  ------------------------------------------------------------------------
*/
gsm rpeltp_init()
{
  gsm rpe_state;
  int	gsm_compliant = 0;

  /* Initializa state variable */
  if (!(rpe_state = gsm_create())) 
    return((gsm)(NULL));
   
  /* The gsm_{en,de}code() routines may operate on a compliant mode, or
   * in a non-compliant, faster mode - here we use the complyant one */
  (void)gsm_option(rpe_state, GSM_OPT_FAST, &gsm_compliant);

  return(rpe_state);
}
/* ................... End of rpeltp_init() ......................... */


/*
  ------------------------------------------------------------------------
  void gsm_state *rpeltp_encode (gsm_state *rpe_state, short *inp_buf, 
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  short *rpe_frame);

  
  Description:
  ~~~~~~~~~~~~
  Process the input buffer inp_buf with left-aligned 16-bit linear-format,
  2-complement samples using 13-bit resolution, i.e., the lower 3 bits of 
  each input sample are ignored. 
  
  The rpe-ltp frame buffer rpe_frame is word-oriented (16-bit samples), 
  right-aligned i.e., the lower bits are the most significatives. 
  They follow the sequence specified by the `parameter number' in 
  Table 1.1 of the Rec.GSM-06.10.
  
  The input buffer has always 160 samples, and the generated frame has 
  always 76 samples.
  
  Variables:
  ~~~~~~~~~~
  rpe_state ... state variable initialized by a previous call to rpeltp_init()
  inp_buf ..... pointer to a buffer with 160 16-bit, left aligned samples
  rpe_frame ... pointer to a rpe-ltp encoded frame buffer. Has length of 76.
                The samples are 16-bit, right-aligned.
  
  Return value:
  ~~~~~~~~~~~~~
  None.
  
  Prototype: in rpeltp.h
  ~~~~~~~~~~
  
  Original author:
  ~~~~~~~~~~~~~~~~
   Simao Ferraz de Campos Neto
   DDS/Pr11                      Tel: +55-192-39-6637
   CPqD/Telebras                 Fax: +55-192-39-6125
   13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>
   
  Log of changes:
  ~~~~~~~~~~~~~~~
  12.Apr.94  v.1.0	created.

  ------------------------------------------------------------------------
*/
void rpeltp_encode(rpe_state, inp_buf, rpe_frame)
gsm rpe_state;
short *inp_buf, *rpe_frame;
{
    gsm_frame rpe_packed_frame;
    
    /* Run both and save decoded samples */
    gsm_encode(rpe_state, inp_buf, rpe_packed_frame);
    gsm_explode(rpe_state, rpe_packed_frame, rpe_frame);
}

/* ................... End of rpeltp_encode() ......................... */


/*
  ------------------------------------------------------------------------
  void gsm_state *rpeltp_decode (gsm_state *rpe_state, short *rpe_frame, 
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  short *out_buf);

  
  Description:
  ~~~~~~~~~~~~
  Decode the input rpe-ltp encoded frame buffer rpe_buf with 76 righ-aligned 
  16-bit samples, generating 160 decoded 16-bit, left-aligned linear 
  2's-complement samples with 13-bit resolution, i.e., the lower 3 bits of 
  each input sample are zeroed. These are saved in out_buf.
  
  The rpe-ltp frame buffer rpe_frame sample has the sequence specified by 
  the `parameter number' in Table 1.1 of the Rec.GSM-06.10.
  
  Variables:
  ~~~~~~~~~~
  rpe_state ... state variable initialized by a previous call to rpeltp_init()
  rpe_frame ... pointer to a rpe-ltp encoded frame buffer. Has length of 76.
                The samples are 16-bit, right-aligned.
  out_buf ..... pointer to the decode sample's buffer with 160 16-bit, 
                left aligned samples
  
  Return value:
  ~~~~~~~~~~~~~
  None.
  
  Prototype: in rpeltp.h
  ~~~~~~~~~~
  
  Original author:
  ~~~~~~~~~~~~~~~~
   Simao Ferraz de Campos Neto
   DDS/Pr11                      Tel: +55-192-39-6637
   CPqD/Telebras                 Fax: +55-192-39-6125
   13088-061 Campinas SP Brazil  E-mail: <simao@cpqd.ansp.br>
   
  Log of changes:
  ~~~~~~~~~~~~~~~
  12.Apr.94  v.1.0	created.

  ------------------------------------------------------------------------
*/
void rpeltp_decode(rpe_state, rpe_frame, out_buf)
gsm rpe_state;
short *out_buf, *rpe_frame;
{
    gsm_frame rpe_packed_frame;

    /* Pack frame, run only the decoder and save the decoded samples */
    gsm_implode(rpe_state, rpe_frame, rpe_packed_frame);
    gsm_decode(rpe_state, rpe_packed_frame, out_buf);
}
/* ................... End of rpeltp_decode() ......................... */

/* -------------------------- END OF RPELTP.C ------------------------ */
