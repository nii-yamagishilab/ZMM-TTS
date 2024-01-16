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


MODULE:         G722.C 7kHz ADPCM AT 64 KBIT/S MODULE ENCODER AND 
                DECODER FUNCTIONS

ORIGINAL BY:
    J-P PETIT 
    CNET - Centre Lannion A
    LAA-TSS                         Tel: +33-96-05-39-41
    Route de Tregastel - BP 40      Fax: +33-96-05-13-16
    F-22301 Lannion CEDEX           Email: petitjp@lannion.cnet.fr
    FRANCE
    
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

#include <stdio.h>

/* Include state variable definition, function and operator prototypes */
#include "g722.h"

/*___________________________________________________________________________
                                                                            
    Function Name : lsbcod                                                  
                                                                            
    Purpose :                                                               
                                                                            
     Encode lower subband of incomung speech/music.                         
                                                                            
    Inputs :                                                                
      xh - Low sub-band portion of the input sample                         
      rs - reset flag (read-only)                                           
      s  - pointer to state variable (read/write)                           
                                                                            
    Return Value :                                                          
     Encoded sample as a 16-bit word                                        
 ___________________________________________________________________________
*/
#define AL   s->al
#define BL   s->bl
#define DETL s->detl
#define DLT  s->dlt
#define NBL  s->nbl
#define PLT  s->plt
#define RLT  s->rlt
#define SL   s->sl
#define SPL  s->spl
#define SZL  s->szl

Word16 
lsbcod (xl, rs, s)
Word16 xl;
Word16 rs;
g722_state *s;
{
  Word16          el, nbpl, il;

  IF (sub(rs, 1) == 0)
  {
    DETL = 32;
    SL = SPL = SZL = NBL = 0;
    AL[1] = AL[2] = 0;
    BL[1] = BL[2] = BL[3] = BL[4] = BL[5] = BL[6] = 0;
    DLT[0] = DLT[1] = DLT[2] = DLT[3] = DLT[4] = DLT[5] = DLT[6] = 0;
    PLT[0] = PLT[1] = PLT[2] = 0;
    RLT[0] = RLT[1] = RLT[2] = 0;
#ifdef WMOPS
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif
    return (0);
  }

  el = sub (xl, SL);           /* subtra */
  il = quantl (el, DETL);
  DLT[0] = invqal (il, DETL);
  nbpl = logscl (il, NBL);
  NBL = nbpl;
  DETL = scalel (nbpl);
  PLT[0] = add (DLT[0], SZL);   /* parrec */
  RLT[0] = add (SL, DLT[0]);    /* recons */
  upzero (DLT, BL);
  uppol2 (AL, PLT);
  uppol1 (AL, PLT);
  SZL = filtez (DLT, BL);
  SPL = filtep (RLT, AL);
  SL = add (SPL, SZL);          /* predic */
#ifdef WMOPS
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif

  /* Return encoded sample */
  return (il);
}
#undef AL
#undef BL
#undef DETL
#undef DLT
#undef NBL
#undef PLT
#undef RLT
#undef SL
#undef SPL
#undef SZL
/* ........................ End of lsbcod() ........................ */


/*___________________________________________________________________________
                                                                            
    Function Name : hsbcod                                                  
                                                                            
    Purpose :                                                               
                                                                            
     Encode upper sub-band of incoming speech or music.                     
                                                                            
    Inputs :                                                                
      xh - High sub-band portion of the input sample                        
      rs - reset flag (read-only)                                           
      s  - pointer to state variable (read/write)                           
                                                                            
    Return Value :                                                          
     ADPCM code for the high-band portion of the input sample as a          
     16-bit word.                                                           
                                                                            
 ___________________________________________________________________________
*/
#define AH   s->ah
#define BH   s->bh
#define DETH s->deth
#define DH  s->dh 
#define NBH  s->nbh
#define PH  s->ph 
#define RH  s->rh 
#define SH   s->sh
#define SPH  s->sph 
#define SZH  s->szh

Word16 
hsbcod (xh, rs, s)
Word16 xh;
Word16 rs;
g722_state *s;
{
  Word16          eh, nbph, ih;

  IF (sub(rs, 1) == 0)
  {
    DETH = 8;
    SH = SPH = SZH = NBH = 0;
    AH[1] = AH[2] = 0;
    BH[1] = BH[2] = BH[3] = BH[4] = BH[5] = BH[6] = 0;
    DH[0] = DH[1] = DH[2] = DH[3] = DH[4] = DH[5] = DH[6] = 0;
    PH[0] = PH[1] = PH[2] = 0;
    RH[0] = RH[1] = RH[2] = 0;
#ifdef WMOPS
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif

    return (0);
  }

  eh = sub (xh, SH);         /* subtra */
  ih = quanth (eh, DETH);
  DH[0] = invqah (ih, DETH);
  nbph = logsch (ih, NBH);
  NBH = nbph;
  DETH = scaleh (nbph);
  PH[0] = add (DH[0], SZH);   /* parrec */
  RH[0] = add (SH, DH[0]);    /* recons */
  upzero (DH, BH);
  uppol2 (AH, PH);
  uppol1 (AH, PH);
  SZH = filtez (DH, BH);
  SPH = filtep (RH, AH);
  SH = add (SPH, SZH);        /* predic */
#ifdef WMOPS
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif

  return (ih);

}
#undef AH
#undef BH
#undef DETH
#undef DH
#undef NBH
#undef PH
#undef RH
#undef SH
#undef SPH
#undef SZH
/* ........................ End of hsbcod() ........................ */



/*___________________________________________________________________________
                                                                            
    Function Name : lsbdec                                                  
                                                                            
    Purpose :                                                               
                                                                            
     Decode lower subband of incomung speech/music.                         
                                                                            
    Inputs :                                                                
      ilr - ADPCM encoding of the low sub-band                              
      mode - G.722 operation mode                                           
      rs  - reset flag (read-only)                                          
      s   - pointer to state variable (read/write)                          
                                                                            
    Return Value :                                                          
     Decoded low-band portion of the recovered sample as a 16-bit word      
 ___________________________________________________________________________
*/
#define AL   s->al
#define BL   s->bl
#define DETL s->detl
#define DLT  s->dlt
#define NBL  s->nbl
#define PLT  s->plt
#define RLT  s->rlt
#define SL   s->sl
#define SPL  s->spl
#define SZL  s->szl

Word16 
lsbdec (ilr, mode, rs, s)
Word16 ilr;
Word16 mode;
Word16 rs;
g722_state *s;
{
  Word16          dl, rl, nbpl, yl;

  IF (sub(rs,1) == 0)
  {
    DETL = 32;
    SL = SPL = SZL = NBL = 0;
    AL[1] = AL[2] = 0;
    BL[1] = BL[2] = BL[3] = BL[4] = BL[5] = BL[6] = 0;
    DLT[0] = DLT[1] = DLT[2] = DLT[3] = DLT[4] = DLT[5] = DLT[6] = 0;
    PLT[0] = PLT[1] = PLT[2] = 0;
    RLT[0] = RLT[1] = RLT[2] = 0;
#ifdef WMOPS
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif

    return (0);
  }

  dl = invqbl (ilr, DETL, mode);
  rl = add (SL, dl);              /* recons */
  yl = limit (rl);
  DLT[0] = invqal (ilr, DETL);
  nbpl = logscl (ilr, NBL);
  NBL = nbpl;
  DETL = scalel (nbpl);
  PLT[0] = add (DLT[0], SZL);     /* parrec */
  RLT[0] = add (SL, DLT[0]);      /* recons */
  upzero (DLT, BL);
  uppol2 (AL, PLT);
  uppol1 (AL, PLT);
  SZL = filtez (DLT, BL);
  SPL = filtep (RLT, AL);
  SL = add (SPL, SZL);            /* predic */
#ifdef WMOPS /*for structures and arrays only*/
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif

  return (yl);

}
#undef AL
#undef BL
#undef DETL
#undef DLT
#undef NBL
#undef PLT
#undef RLT
#undef SL
#undef SPL
#undef SZL
/* ........................ End of lsbdec() ........................ */


/*___________________________________________________________________________
                                                                            
    Function Name : hsbdec                                                  
                                                                            
    Purpose :                                                               
                                                                            
     Decode lower subband of incomung speech/music.                         
                                                                            
    Inputs :                                                                
      ih  - ADPCM encoding of the high sub-band                             
      rs  - reset flag (read-only)                                          
      s   - pointer to state variable (read/write)                          
                                                                            
    Return Value :                                                          
     Decoded high-band portion of the recovered sample as a 16-bit word     
 ___________________________________________________________________________
*/
#define AH   s->ah
#define BH   s->bh
#define DETH s->deth
#define DH  s->dh 
#define NBH  s->nbh
#define PH  s->ph 
#define RH  s->rh 
#define SH   s->sh
#define SPH  s->sph 
#define SZH  s->szh

Word16 
hsbdec (ih, rs, s)
Word16 ih;
Word16 rs;
g722_state *s;
{
  Word16          nbph, yh;

  IF (sub(rs, 1) == 0)
  {
    DETH = 8;
    SH = SPH = SZH = NBH = 0;
    AH[1] = AH[2] = 0;
    BH[1] = BH[2] = BH[3] = BH[4] = BH[5] = BH[6] = 0;
    DH[0] = DH[1] = DH[2] = DH[3] = DH[4] = DH[5] = DH[6] = 0;
    PH[0] = PH[1] = PH[2] = 0;
    RH[0] = RH[1] = RH[2] = 0;
#ifdef WMOPS
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif
    return (0);
  }

  DH[0] = invqah (ih, DETH);
  nbph = logsch (ih, NBH);
  NBH = nbph;
  DETH = scaleh (nbph);
  PH[0] = add (DH[0], SZH);    /* add */
  RH[0] = add (SH, DH[0]);     /* recons */
  upzero (DH, BH);
  uppol2 (AH, PH);
  uppol1 (AH, PH);
  SZH = filtez (DH, BH);
  SPH = filtep (RH, AH);
  SH = add (SPH, SZH);         /* predic */
  yh = limit (RH[0]);
#ifdef WMOPS
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
    move16();
#endif

  return (yh);
}
#undef AH
#undef BH
#undef DETH
#undef DH
#undef NBH
#undef PH
#undef RH
#undef SH
#undef SPH
#undef SZH
/* ........................ End of hsbdec() ........................ */


/*___________________________________________________________________________
                                                                            
    Function Name : quantl                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
quantl (el, detl)
Word16 el;
Word16 detl;
{
 /* table to read IL frorm SIL and MIL: misil(sil(0,1),mil(1,31)) */

  static Word16   misil[2][32] =
  {
    {0x0000, 0x003F, 0x003E, 0x001F, 0x001E, 0x001D, 0x001C, 0x001B,
     0x001A, 0x0019, 0x0018, 0x0017, 0x0016, 0x0015, 0x0014, 0x0013,
     0x0012, 0x0011, 0x0010, 0x000F, 0x000E, 0x000D, 0x000C, 0x000B,
     0x000A, 0x0009, 0x0008, 0x0007, 0x0006, 0x0005, 0x0004, 0x0000},
    {0x0000, 0x003D, 0x003C, 0x003B, 0x003A, 0x0039, 0x0038, 0x0037,
     0x0036, 0x0035, 0x0034, 0x0033, 0x0032, 0x0031, 0x0030, 0x002F,
     0x002E, 0x002D, 0x002C, 0x002B, 0x002A, 0x0029, 0x0028, 0x0027,
     0x0026, 0x0025, 0x0024, 0x0023, 0x0022, 0x0021, 0x0020, 0x0000}
  };

 /* 6 levels quantizer level od decision */
  static Word16   q6[31] =
  {
    0, 35, 72, 110, 150, 190, 233, 276,
    323, 370, 422, 473, 530, 587, 650, 714,
    786, 858, 940, 1023, 1121, 1219, 1339, 1458,
    1612, 1765, 1980, 2195, 2557, 2919, 3200
  };



  Word16          sil, mil, wd, val;

  sil = shr (el, 15);
  wd = sub (MAX_16, s_and(el, MAX_16));
  if(sil == 0)
  {
    wd = el;
#ifdef WMOPS
    move16();
#endif
  }

  mil = 0;
#ifdef WMOPS
    move16();
#endif
  val = mult (shl(q6[mil], 3), detl);
  WHILE (sub(val,wd) <= 0)
  {
    IF (sub(mil, 30) == 0)
    {
      BREAK;
    }
    ELSE
    {
      mil = add(mil,1);
      val = mult (shl(q6[mil],3), detl);
    }
  }

  sil = add(sil,1);
#ifdef WMOPS
    move16(); /*for the 2 dimensional array addressing */
#endif
  return (misil[sil][mil]);
}
/* ..................... End of quantl() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name quanth:                                                   
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
quanth (eh, deth)
Word16 eh;
Word16 deth;
{
  static Word16   misih[2][3] =
  {
    {0, 1, 0},
    {0, 3, 2}
  };
  static Word16   q2 = 564;

  Word16          sih, mih, wd;

  sih = shr (eh, 15);
  wd = sub (MAX_16, s_and(eh, MAX_16));
  if(sih == 0)
  {
    wd = eh;
  }
  mih = 1;
#ifdef WMOPS
  move16();
#endif
  if (sub(wd, mult (shl (q2, 3), deth)) >= 0)
  {
    mih = 2;
#ifdef WMOPS
    move16();
#endif
  }

  sih = add(sih, 1);
#ifdef WMOPS
    move16(); /*for the 2 dimensional array addressing */
#endif
  return (misih[sih][mih]);
}
/* ..................... End of quanth() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : filtep                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
filtep (rlt, al)
Word16 rlt [];
Word16 al [];
{

  Word16          wd1, wd2, spl;

  /* shift of rlt */
  rlt[2] = rlt[1];		
  rlt[1] = rlt[0];		
#ifdef WMOPS
    move16();
    move16();
#endif

  wd1 = add (rlt[1], rlt[1]);
  wd1 = mult (al[1], wd1);
  wd2 = add (rlt[2], rlt[2]);
  wd2 = mult (al[2], wd2);
  spl = add (wd1, wd2);

  return (spl);
}
/* ..................... End of filtep() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : filtez                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
filtez (dlt, bl)
Word16 dlt [];
Word16 bl [];
{
  Word16          szl, wd;
  Word16          i;

  szl = 0;
#ifdef WMOPS
    move16();
#endif
  FOR (i = 6; i > 0; i--)
  {
    wd = add (dlt[i], dlt[i]);
    wd = mult (wd, bl[i]);
    szl = add (szl, wd);
  }

  return (szl);
}
/* ..................... End of filtez() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : invqal                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
invqal (il, detl)
Word16 il;
Word16 detl;
{

  /* inverse quantizer 4 bits for encoder or decoder */
  static Word16   ril4[16] =
  {0, 7, 6, 5, 4, 3, 2, 1, 7, 6, 5, 4, 3, 2, 1, 0};
  static Word16   risil[16] =
  {0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0};
  static Word16   oq4[8] =
  {0, 150, 323, 530, 786, 1121, 1612, 2557};

  /* Local variables */
  Word16          ril, wd1, wd2;


  ril = shr (il, 2);
  wd1 = shl (oq4[ril4[ril]], 3);
#ifdef WMOPS
    move16(); /*for the 2 dimensional array addressing */
#endif
  wd2 = negate (wd1);
#ifdef WMOPS
    move16(); /*for the test on array variable*/
#endif
  if(risil[ril] == 0)
  {
    wd2 = wd1;
#ifdef WMOPS
    move16();
#endif
  }

  return (mult (detl, wd2));
}
/* ..................... End of invqal() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : invqbl                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
invqbl (ilr, detl, mode)
Word16 ilr;
Word16 detl;
Word16 mode;
{

  /* Inverse quantizer 4, 5, and 6 bit tables for the decoder */
  static Word16   ril4[16] = {0, 7, 6, 5, 4, 3, 2, 1, 7, 6, 5, 4, 3, 2, 1, 0};
  static Word16   risi4[16] = {0, -1, -1, -1, -1, -1, -1, -1, 
                               0, 0, 0, 0, 0, 0, 0, 0};
  static Word16   oq4[8] = {0, 150, 323, 530, 786, 1121, 1612, 2557};
  static Word16   ril5[32] = {
    1, 1, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1};

  static Word16   risi5[32] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1};

  static Word16   oq5[16] = {
    0, 35, 110, 190, 276, 370, 473, 587,
    714, 858, 1023, 1219, 1458, 1765, 2195, 2919};

  static Word16   ril6[64] = {
    1, 1, 1, 1, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3,
    30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 1};

  static Word16   risi6[64] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1};

  static Word16   oq6[31] = {
    0, 17, 54, 91, 130, 170, 211, 254, 300, 347, 396, 447, 501,
    558, 618, 682, 750, 822, 899, 982, 1072, 1170, 1279, 1399,
    1535, 1689, 1873, 2088, 2376, 2738, 3101};

  /* Local variables */
  Word16          ril, wd1, wd2;

  SWITCH (mode)
  {
  case 0:
  case 1:
    ril = ilr;
#ifdef WMOPS
    move16();
#endif
    wd1 = shl (oq6[ril6[ril]], 3);
#ifdef WMOPS
    move16(); /*for double array indexing*/
#endif
    wd2 = sub (0, wd1);
#ifdef WMOPS
    move16(); /*for the test on array variable*/
#endif
    if(risi6[ril] == 0)
    {
      wd2 = add (0, wd1);
    }
    BREAK;

  case 2:
    ril = shr (ilr, 1);
    wd1 = shl (oq5[ril5[ril]], 3);
#ifdef WMOPS
    move16(); /*for double array indexing*/
#endif
    wd2 = sub (0, wd1);
#ifdef WMOPS
    move16(); /*for the test on array variable*/
#endif
    if(risi5[ril] == 0)
    {
      wd2 = add (0, wd1);
    }
    BREAK;

  case 3:
    ril = shr (ilr, 2);
    wd1 = shl (oq4[ril4[ril]], 3);
#ifdef WMOPS
    move16(); /*for double array indexing*/
#endif
    wd2 = sub (0, wd1);
#ifdef WMOPS
    move16(); /*for the test on array variable*/
#endif
    if(risi4[ril] == 0)
    {
      wd2 = add (0, wd1);
    }
    BREAK;

  default:
    /* default to mode 3 */
    ril = shr (ilr, 2);
    wd1 = shl (oq4[ril4[ril]], 3);
#ifdef WMOPS
    move16(); /*for double array indexing*/
#endif
    wd2 = sub (0, wd1);
#ifdef WMOPS
    move16(); /*for the test on array variable*/
#endif
    if(risi4[ril] == 0)
    {
      wd2 = add (0, wd1);
    }
    BREAK;
  }

  return (mult (detl, wd2));
}
/* ..................... End of invqbl() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : invqah                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
invqah (ih, deth)
Word16 ih;
Word16 deth;
{
  static Word16   ih2[4] = {2, 1, 2, 1};
  static Word16   sih[4] = {-1, -1, 0, 0};
  static Word16   oq2[3] = {0, 202, 926};

  Word16          wd1, wd2;

  wd1 = shl (oq2[ih2[ih]], 3);
#ifdef WMOPS
    move16(); /*for double array indexing*/
#endif
  wd2 = negate (wd1);
#ifdef WMOPS
    move16(); /*for the test on array variable*/
#endif
  if(sih[ih] == 0)
  {
    wd2 = wd1;
#ifdef WMOPS
    move16();
#endif
  }

  return mult (wd2, deth);
}
/* ..................... End of invqah() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : limit                                                   
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
limit (rl)
Word16 rl;
{

  if(sub(rl, 16383) > 0)
  {
    rl = 16383;
#ifdef WMOPS
    move16();
#endif
  }
  
  if(sub(rl, -16384) < 0)
  {
    rl = -16384;
#ifdef WMOPS
    move16();
#endif
  }

  return (rl);
}
/* ..................... End of limit() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : logsch                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
logsch (ih, nbh)
Word16 ih;
Word16 nbh;
{
  static Word16   ih2[4] = {2, 1, 2, 1};
  static Word16   wh[3] =  {0, -214, 798};

  Word16          wd, nbph;
  
  wd = mult (nbh, 32512);
  nbph = add (wd, wh[ih2[ih]]);
#ifdef WMOPS
    move16(); /*for double array indexing*/
#endif
  if(nbph < 0)
  {
    nbph = 0;
#ifdef WMOPS
    move16();
#endif
  }
  if(sub(nbph, 22528) > 0)
  {
    nbph = 22528;
#ifdef WMOPS
    move16();
#endif
  }
  
  return (nbph);
}
/* ..................... End of logsch() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : logscl                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
logscl (il,nbl )
Word16 il;
Word16 nbl;
{
  static Word16   ril4[16] = {0, 7, 6, 5, 4, 3, 2, 1, 7, 6, 5, 4, 3, 2, 1, 0};
  static Word16   wl[8] =    {-60, -30, 58, 172, 334, 538, 1198, 3042};

  Word16          ril, wd, nbpl, il4;


  ril = shr (il, 2);
  wd = mult (nbl, 32512);
  il4 = ril4[ril];
  nbpl = add (wd, wl[il4]);
  if(nbpl < 0)
  {
    nbpl = 0;
#ifdef WMOPS
    move16();
#endif
  }
  if(sub(nbpl, 18432) > 0)
  {
    nbpl = 18432;
#ifdef WMOPS
    move16();
#endif
  }

  return (nbpl);

}
/* ..................... End of logscl() ..................... */


/* ************** Table ILA used by scalel and scaleh ******************** */
static Word16   ila[353] =
  {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 4, 4, 4, 4, 4,
    4, 4, 4, 5, 5, 5, 5, 5,
    5, 5, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 8, 8,
    8, 8, 8, 9, 9, 9, 9, 10,
    10, 10, 10, 11, 11, 11, 11, 12,
    12, 12, 13, 13, 13, 13, 14, 14,
    15, 15, 15, 16, 16, 16, 17, 17,
    18, 18, 18, 19, 19, 20, 20, 21,
    21, 22, 22, 23, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 28, 29, 30,
    31, 31, 32, 33, 33, 34, 35, 36,
    37, 37, 38, 39, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 50, 51,
    52, 54, 55, 56, 57, 58, 60, 61,
    63, 64, 65, 67, 68, 70, 71, 73,
    75, 76, 78, 80, 82, 83, 85, 87,
    89, 91, 93, 95, 97, 99, 102, 104,
    106, 109, 111, 113, 116, 118, 121, 124,
    127, 129, 132, 135, 138, 141, 144, 147,
    151, 154, 157, 161, 165, 168, 172, 176,
    180, 184, 188, 192, 196, 200, 205, 209,
    214, 219, 223, 228, 233, 238, 244, 249,
    255, 260, 266, 272, 278, 284, 290, 296,
    303, 310, 316, 323, 331, 338, 345, 353,
    361, 369, 377, 385, 393, 402, 411, 420,
    429, 439, 448, 458, 468, 478, 489, 500,
    511, 522, 533, 545, 557, 569, 582, 594,
    607, 621, 634, 648, 663, 677, 692, 707,
    723, 739, 755, 771, 788, 806, 823, 841,
    860, 879, 898, 918, 938, 958, 979, 1001,
    1023, 1045, 1068, 1092, 1115, 1140, 1165, 1190,
    1216, 1243, 1270, 1298, 1327, 1356, 1386, 1416,
    1447, 1479, 1511, 1544, 1578, 1613, 1648, 1684,
    1721, 1759, 1797, 1837, 1877, 1918, 1960, 2003,
    2047, 2092, 2138, 2185, 2232, 2281, 2331, 2382,
    2434, 2488, 2542, 2598, 2655, 2713, 2773, 2833,
    2895, 2959, 3024, 3090, 3157, 3227, 3297, 3370,
    3443, 3519, 3596, 3675, 3755, 3837, 3921, 4007,
    4095};
/* ************************* End of Table ILA **************************** */


/*___________________________________________________________________________
                                                                            
    Function Name : scalel                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
scalel (nbpl)
Word16 nbpl;
{
  Word16          wd1, wd2;

  wd1 = s_and(shr(nbpl, 6), 511);
  wd2 = add(wd1, 64);
  return (shl(add(ila[wd2], 1), 2));
}
/* ..................... End of scalel() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : scaleh                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
                                                                            
 ___________________________________________________________________________
*/
Word16 
scaleh (nbph)
Word16 nbph;
{
  Word16          wd;

  wd = s_and(shr(nbph, 6), 511);

  return (shl(add(ila[wd], 1), 2));
}
/* ..................... End of scaleh() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : uppol1                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
     None.                                                                  
 ___________________________________________________________________________
*/
void 
uppol1 (al, plt)
Word16 al [];
Word16 plt [];
{
  Word16          sg0, sg1, wd1, wd2, wd3, apl1;

  sg0 = shr (plt[0], 15);
  sg1 = shr (plt[1], 15);
  wd1 = -192;
#ifdef WMOPS
  move16();
#endif
  if(sub(sg0, sg1) == 0)
  {
    wd1 = 192;
#ifdef WMOPS
    move16();
#endif
  }
  wd2 = mult (al[1], 32640);
  apl1 = add (wd1, wd2);
  wd3 = sub (15360, al[2]);
  IF(sub(apl1, wd3) > 0)
  {
    apl1 = wd3;
#ifdef WMOPS
    move16();
#endif
  }
  ELSE
  {
    if(add(apl1, wd3) < 0)
    {
      apl1 = negate (wd3);
    }
  }
/*  apl1 = (apl1 > wd3) ? wd3 : ((apl1 < -wd3) ? negate (wd3) : apl1);*/

  /* Shift of the plt signals */
  plt[2] = plt[1];
  plt[1] = plt[0];
  al[1] = apl1;
#ifdef WMOPS
    move16();
    move16();
    move16();
#endif
}
/* ..................... End of uppol1() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : uppol2                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
     None.                                                                  
 ___________________________________________________________________________
*/
void 
uppol2 (al, plt)
Word16 al [];
Word16 plt [];
{
  Word16          sg0, sg1, sg2, wd1, wd2, wd3, wd4, wd5, apl2;

  sg0 = shr (plt[0], 15);
  sg1 = shr (plt[1], 15);
  sg2 = shr (plt[2], 15);
  wd1 = shl (al[1], 2);
  wd2 = add (0, wd1);
  if(sub(sg0, sg1) == 0)
  {
    wd2 = sub (0, wd1);
  }
  wd2 = shr (wd2, 7);
  wd3 = -128;
#ifdef WMOPS
  move16();
#endif
  if(sub(sg0, sg2) == 0)
  {
    wd3 = 128;
#ifdef WMOPS
    move16();
#endif
  }
  wd4 = add (wd2, wd3);
  wd5 = mult (al[2], 32512);
  apl2 = add (wd4, wd5);
  if(sub(apl2, 12288) > 0)
  {
    apl2 = 12288;
#ifdef WMOPS
    move16();
#endif
  }
  if(sub(apl2, -12288) < 0)
  {
    apl2 = -12288;
#ifdef WMOPS
    move16();
#endif
  }
  al[2] = apl2;
#ifdef WMOPS
  move16();
#endif
}
/* ..................... End of uppol2() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : upzero                                                  
                                                                            
    Purpose :                                                               
                                                                            
     .                  
                                                                            
    Inputs :                                                                
                                                                            
                                                                            
    Outputs :                                                               
                                                                            
     none                                                                   
                                                                            
    Return Value :                                                          
     None.                                                                  
                                                                            
 ___________________________________________________________________________
*/
void 
upzero (dlt, bl)
Word16 dlt [];
Word16 bl [];
{

  Word16          sg0, sgi, wd1, wd2, wd3;
  Word16          i;

  /* shift of the dlt line signal and update of bl */

  wd1 = 128;
#ifdef WMOPS
  move16();
#endif
  if(dlt[0] == 0)
  {
    wd1 = 0;
#ifdef WMOPS
    move16();
#endif
  }
  sg0 = shr (dlt[0], 15);

  FOR (i = 6; i > 0; i--)
  {
    sgi = shr (dlt[i], 15);
    wd2 = sub (0, wd1);
    if(sg0 == sgi)
    {
      wd2 = add (0, wd1);
    }
    wd3 = mult (bl[i], 32640);
    bl[i] = add (wd2, wd3);
    dlt[i] = dlt[i - 1];
#ifdef WMOPS
    move16();
    move16();
#endif
  }
}
/* ..................... End of upzero() ..................... */


/* **** Coefficients for both transmission and reception QMF **** */
static Word16   coef_qmf[24] =
  {
    3 * 2, -11 * 2, -11 * 2, 53 * 2, 12 * 2, -156 * 2,
    32 * 2, 362 * 2, -210 * 2, -805 * 2, 951 * 2, 3876 * 2,
    3876 * 2, 951 * 2, -805 * 2, -210 * 2, 362 * 2, 32 * 2,
    -156 * 2, 12 * 2, 53 * 2, -11 * 2, -11 * 2, 3 * 2};
/* ..................... End of table coef_qmf[] ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : qmf_tx                                                  
                                                                            
    Purpose :                                                               
                                                                            
     G722 QMF analysis (encoder) filter. Uses coefficients in array         
     coef_qmf[] defined above.                                              
                                                                            
    Inputs :                                                                
     xin0 - first sample for the QMF filter (read-only)                     
     xin1 - secon sample for the QMF filter (read-only)                     
     xl   - lower band portion of samples xin0 and xin1 (write-only)        
     xh   - higher band portion of samples xin0 and xin1 (write-only)       
     s    - pointer to state variable structure (read/write)                
                                                                            
    Return Value :                                                          
     None.                                                                  
 ___________________________________________________________________________
*/
#define delayx s->qmf_tx_delayx
void 
qmf_tx (xin0, xin1, xl, xh, s)
Word16 xin0;
Word16 xin1;
Word16 *xl;
Word16 *xh;
g722_state *s;
{

  /* Local variables */
  Word16             i;
  Word32          accuma, accumb;
  Word32          comp_low, comp_high;
  Word16 *pcoef, *pdelayx;

  /* Saving past samples in delay line */
  delayx[1] = xin1;
  delayx[0] = xin0;
#ifdef WMOPS
  move16();
  move16();
#endif

  /* QMF filtering */
  pcoef = coef_qmf;
  pdelayx = delayx;
#ifdef WMOPS
  move16();
  move16();
#endif
  accuma = L_mult0(*pcoef++, *pdelayx++);
  accumb = L_mult0(*pcoef++, *pdelayx++);
  FOR (i = 1; i < 12; i++)
  {
    accuma = L_mac0(accuma, *pcoef++, *pdelayx++);
    accumb = L_mac0(accumb, *pcoef++, *pdelayx++);
  }

  /* Descaling and shift of the delay line */
  FOR (i = 0; i < 22; i++)
  {
    delayx[23 - i] = delayx[21 - i];
#ifdef WMOPS
    move16();
#endif
  }

  comp_low = L_add (accuma, accumb);
  comp_low = L_add (comp_low, comp_low);
  comp_high = L_sub (accuma, accumb);
  comp_high = L_add (comp_high, comp_high);
  *xl = limit ((Word16) L_shr (comp_low, (Word16) 16));
  *xh = limit ((Word16) L_shr (comp_high, (Word16) 16));
}
#undef delayx
/* ..................... End of qmf_tx() ..................... */


/*___________________________________________________________________________
                                                                            
    Function Name : qmf_rx                                                  
                                                                            
     G722 QMF synthesis (decoder) filter. Uses coefficients in array        
     coef_qmf[] defined above.                                              
                                                                            
    Inputs :                                                                
     xout0 - first sample out of the QMF filter (write-only)                
     xout1 - secon sample out of the QMF filter (write-only)                
     rl    - lower band portion of a sample (read-only)                     
     rh    - higher band portion of a sample (read-only)                    
     s     - pointer to state variable structure (read/write)               
                                                                            
    Return Value :                                                          
     None.                                                                  
 ___________________________________________________________________________
*/
#define delayx s->qmf_rx_delayx
void 
qmf_rx (rl, rh, xout1, xout2, s)
Word16 rl;
Word16 rh;
Word16 *xout1;
Word16 *xout2;
g722_state *s;
{
  Word16             i;
  Word32          accuma, accumb;
  Word32          comp_low, comp_high;
  Word16 *pcoef, *pdelayx;

  /* compute sum and difference from lower-band (rl) and higher-band (rh) signals */
  /* update delay line */
  delayx[1] = add (rl, rh);
  delayx[0] = sub (rl, rh);
#ifdef WMOPS
    move16();
    move16();
#endif

  /* qmf_rx filtering */
  pcoef = coef_qmf;
  pdelayx = delayx;
#ifdef WMOPS
  move16();
  move16();
#endif
  accuma = L_mult0(*pcoef++, *pdelayx++);
  accumb = L_mult0(*pcoef++, *pdelayx++);
  FOR (i = 1; i < 12; i++)
  {
    accuma = L_mac0(accuma, *pcoef++, *pdelayx++);
    accumb = L_mac0(accumb, *pcoef++, *pdelayx++);
  }

  /* shift delay line */
  FOR(i = 0; i < 22; i++)
  {
    delayx[23 - i] = delayx[21 - i];
#ifdef WMOPS
    move16();
#endif
  }
  comp_low = L_shl(accuma,4);
  comp_high = L_shl(accumb,4);

  /* compute output samples */
  *xout1 = extract_h(comp_low);
  *xout2 = extract_h(comp_high);
}

#undef delayx
/* ..................... End of qmf_rx() ..................... */

/* ******************** End of funcg722.c ***************************** */
