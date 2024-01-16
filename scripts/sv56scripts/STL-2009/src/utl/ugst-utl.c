/*                                                            v3.0  06.Mar.96
=============================================================================
 
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
 
 
MODULE:         UGST-UTL.C, UGST UTILITY FUNCTIONS
 
ORIGINAL BY:    <tdsimao@venus.cpqd.ansp.br> (editor),
                <hf@pkinbg.uucp>
 
PROTOTYPE:     in ugst-utl.h
 
FUNCTIONS:
 
    fl2sh_16bit: .... conversion of an array from float to 16 bit (*)
    fl2sh_15bit: .... conversion of an array from float to 15 bit (*)
    fl2sh_14bit: .... conversion of an array from float to 14 bit (*)
    fl2sh_13bit: .... conversion of an array from float to 13 bit (*)
    fl2sh_12bit: .... conversion of an array from float to 12 bit (*)
    fl2sh: .......... generic function for conversion from float to short
 
    scale: .......... gain/loss insertion algorithm.
 
    sh2fl_16bit: .... conversion of an array from 16 bit to float (*)
    sh2fl_15bit: .... conversion of an array from 15 bit to float (*)
    sh2fl_14bit: .... conversion of an array from 14 bit to float (*)
    sh2fl_13bit: .... conversion of an array from 13 bit to float (*)
    sh2fl_12bit: .... conversion of an array from 12 bit to float (*)
    sh2fl: .......... generic function for conversion from short to float
 
    serialize_left_justified ....... serialization for left-justified data
    serialize_right_justified ...... serialization for right-justified data
    parallelize_left_justified ..... parallelization for left-justified data
    parallelize_right_justified .... parallelization for right-justified data

    There are two families of serialize...() and parallelize_...()
    functions. Ones dates from the STL92 release, and the other was
    generated for the STL96 release. The difference between them is
    that in the STL96, frame boundaries, when marked, follow the
    description in Annex B of ITU-T Rec. G.192, while for the STL92,
    frame boundaries were be marked with a sync word only. In Annex B
    of G.192, frame boundaries are marked by a sync word followed by a
    two-complent 16-bit word with the number of softbits in the
    frame. By default, the G.192-compliant functions are used in the
    STL96. The STL92 version will be used if the symbol STL92 is
    defined during compilation.
 
    --------------------------------------------------------------------
    NB: all marked with (*) are implemented as macros (#defines), rather
        than true functions! For their definition, please see ugst-utl.h
    --------------------------------------------------------------------
 
HISTORY:
 
  28.Feb.92 v1.0 Release of 1st assembled file of UGST utilities.
  10.Apr.92 v1.1 Added ser/par routines
                 <tdsimao@cpqd.ansp.br>
  18.May.92 v1.2 Correction of `sh2fl' normalization factor calculation;
                 `fl2sh' modified to use floats in the normalizaed range;
                 removed built-in initialization of the `serialize_*'.
                 <tdsimao@cpqd.ansp.br>
  27.Nov.92 v1.3 "fl2sh()" for negative values corrected <hf@pkinbg.uucp>
                 "sh2fl()" fixed for resolution!=16 <bloecher@pkinbg.uucp>
		 created sh2fl_alt()" as an alternate to sh2fl() of the
		 STL92 manual <bloecher@pkinbg.uucp>
  06.Jun.95 v2.0 Fixed erroneous definition of softbits '1' and '0' in the
                 serial<->parallel conversion routines, to make
                 them inline with the EID module, the hardware 8kbit/s codec 
                 host lab EID and G.192. New definitions are '0'=0x7F and 
                 '1'=0x81. <simao@ctd.comsat.com>
  06.Mar.96 v3.0 Created new parallelize_...() and serialize_...() functions 
                 which comply to the bitstream definition given in Annex B 
                 of G.192. <simao@ctd.comsat.com>
=============================================================================
*/


/*
 * .................... INCLUDES ....................
 */
#include <string.h> /* For memset() */
#include "ugst-utl.h" /* Module Function prototypes */
 
 
/*
 * .................... FUNCTIONS ....................
 */
 
/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
        long scale (float *buffer,long smpno,double factor)
        ~~~~~~~~~~
 
        Description:
        ~~~~~~~~~~~~
 
        Gain/loss insertion algorithm that scales the input buffer data
        by a given factor. If the factor is greater than 1.0, it means a
        gain; if less than 1.0, a loss. The basic algorithm is simply:
                              y(k)= x(k) * factor
 
        Please note that:
        > the scaled data is put into the same location of the
          original data, in order to save memory space;
        > input data buffer is an array of floats;
        > scaling precision is single (rather than double).
 
        Variables:
        ~~~~~~~~~~
        Name:        I/0: Type:   Use:
        buffer       I/O  float*  Data vector to be scaled.
        smpno         I   long    Number of samples in "buffer"
        factor        I   float   scaling factor
 
        Internal variables are declared as "register" to improve
        processing velocity.
 
        Return value:
        ~~~~~~~~~~~~~
        Returns the number of scaled samples, as a long.
 
        Functions used:      NONE
        ~~~~~~~~~~~~~~~
 
        Prototype:    in ugst-utl.h
        ~~~~~~~~~~
 
        Author:
        ~~~~~~~
        Simao Ferraz de Campos Neto
        DDS/Pr11                      Tel: +55-192-39-1396
        CPqD/Telebras                 Fax: +55-192-53-4754
        13085 Campinas SP Brazil      E-mail: <tdsimao@venus.cpqd.ansp.br>
 
        Log of changes:
        ~~~~~~~~~~~~~~~
        Dates        Version        Description
        11.Oct.91      1.0        First release in C.
 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
long            scale(buffer, smpno, factor)
  float          *buffer;
  double          factor;
  long            smpno;
{
  register long   j;
  register float  f;
 
  /* scales all of the samples */
  for (f = (float) factor, j = 0; j < smpno; j++)
    buffer[j] *= f;
 
  /* and return the number of scaled samples */
  return (j);
}                  /* ......... end of scale() ......... */
 
 
/*
  --------------------------------------------------------------------------
 
        long fl2sh (long  n, float *x, short *iy,
        ~~~~~~~~~~  double half_lsb, unsigned mask);
 
        Description:
        ~~~~~~~~~~~~
 
        Common quantisation routine. The conversion routine expect the
        floating point data to be in the range between -1 .. +1,
        values outside this range are limited to -1 or +32767.0.
        Quantization is done by taking into account only the
        most significant bits. So the quantized (or converted) data are
        located left justified within the 16-bit word, and the results
        are in the range:
                  -32768,( 1),+32767, if quantized to 16 bit
                  -32768,( 2),+32766, if quantized to 15 bit
                  -32768,( 4),+32763, if quantized to 14 bit
                  -32768,( 8),+32760, if quantized to 13 bit
                  -32768,(16),+32752, if quantized to 12 bit
 
        In some cases one needs truncated data. For example, at the
        input of A-Law encoding, truncation to 12 bit is neccessary, not
        rounding. On the other hand within recursive filters rounding is
        essential. So, for both cases functions have been designed.
 
        Concerning the location of the fixed-point data within one 16
        bit word, it's most practical to have the decimal point
        immediateley after the sign bit (between bit 15 and 14, if the
        bits are ordered from 0 ... 15). Since this is well defined, a
        software, which processes the quantized data  needs no knowledge
        about the resolution of the data. It's not important,  whether
        tha data come out from A- or u-Law decoding routines or from
        12-bit (13,14,16-bit) A/D converters.
 
 
        Parameters:
        ~~~~~~~~~~~
        n .......... is the number of samples in x[];
        x .......... is input float array's pointer;
        iy ......... is output short array's pointer;
        half_lsb ... is the float representation of 0.5 lsb for the
                     desired resolution (quantization);
        mask ....... unsigned masking of the lower (right) bits.
 
        Returns value:
        ~~~~~~~~~~~~~~
        Returns the number of overflows that happened.
 
        Prototype:  in ugst-utl.h
        ~~~~~~~~~~
 
        Original author:
        ~~~~~~~~~~~~~~~~
              Rudolf Hofmann
              Advanced Development Digital Signal Processing
              PHILIPS KOMMUNIKATIONS INDUSTRIE AG
              Kommunikationssysteme
              Thurn-und-Taxis-Strasse 14
              D-8500 Nuernberg 10 (Germany)
 
              Phone : +49 911 526-2603
              FAX   : +49 911 526-3385
              EMail : hf@pkinbg.uucp
 
        History:
        ~~~~~~~~
        10.Dec.91 v1.0 Release of 1st version with callable routines for
                       conversion from float to short. <hf@pkinbg.uucp>
        25.Feb.92 v1.1 Change of specific functions to macros (#defines)
                       <tdsimao@venus.cpqd.ansp.br>
        28.Feb.92 v1.2 Change of call from short to unsigned for parameter
                       mask, needed in Unix <tdsimao@venus.cpqd.ansp.br>
        18.May.92 v1.3 Change to make it operate with input data in the
                       normalized range (-1.0 .. +1.0), instead of floats
                       in the integer range (-32768.0 .. 32767.0).
        27.Nov.92 v1.4 fl2sh() corrected for negative values 
                       <hf@pkinbg.uucp>
 
  --------------------------------------------------------------------------
*/
 
long            fl2sh(n, x, iy, half_lsb, mask)
  long            n;
  float          *x;
  short          *iy;
  double          half_lsb;
  short           mask;
{
  register long   iOvrFlw, k;
  register double y;

  /* Reset overflow counter */
  iOvrFlw = 0;

  /* Loop over all input samples: assume result left justified in array */

  /* ------------------------------------------------------------------------ */
  /* Perform 2's complement truncation if "no rounding" is selected           */
  /* ------------------------------------------------------------------------ */
  if (half_lsb == 0.0)
  {
    for (k = 0; k < n; k++)
    {
      /* Convert input data from normalized to 16-bit range (still float) */
      y = x[k] * 32768;

      /* Amplitude clip */
      if (y > 32767.0)		/* limitation to positive maximum */
      {
	y = 32767.0;
	iOvrFlw += 1;
      }
      else if (y < -32768.0)	/* limitation to negative maximum */
      {
	y = -32768.0;
	iOvrFlw += 1;
      }

      iy[k] = (short) (y);
      iy[k] &= mask;
    }
  }

  /* ---------------------------------------------------------------------- */
  /* Perform Magnitude Rounding                                             */
  /* ---------------------------------------------------------------------- */
  else
  {
    for (k = 0; k < n; k++)
    {
      /* Convert input data from normalized to 16-bit range (still float) */
      y = x[k] * 32768;
      if (y >= 0.0)
	y = y + half_lsb;
      else
	y = y - half_lsb;

      /* Amplitude clip */
      if (y > 32767.0)
      {
	/* limitation to positive maximum */
	y = 32767.0;
	iOvrFlw += 1;
      }
      else if (y < -32768.0)
      {
	/* limitation to negative maximum */
	y = -32768.0;
	iOvrFlw += 1;
      }

      if (y >= 0.0)
      {
	/* conversion to short (16 bit) */
	iy[k] = (short) (y);
	iy[k] &= mask;
      }
      else
      {
	/* if (y < 0.0) */
	iy[k] = (short) (-y);	/* iy will be 0x8000 even if y = -32768.0   */
	iy[k] &= mask;
	iy[k] = -iy[k];
      }
    }
  }

  /* Return number of overflows */
  return iOvrFlw;
}                   /* ......... end of fl2sh() ......... */
 
/*
  --------------------------------------------------------------------------
 
        void sh2fl_alt (long  n, short *ix, float *y, short mask)
        ~~~~~~~~~~~~~~
 
        Description:
        ~~~~~~~~~~~~
 
	Alternate common conversion routine. This conversion routine
	expects the fixed-point data to be in the range -32768..32767.
	Conversion to float is done by taking into account only the
	most significant bits, indicated by 'mask'.  Conversion to
	float results *NECESSARILY* in normalised values in the range
	-1.0 <= y[*] < +1.0. This is an alternate version of the
	sh2fl() as in the STL92 Manual.
 
        Parameters:
        ~~~~~~~~~~~
        n ........... is the number of samples in ix[];
        ix .......... is input short array's pointer;
        y ........... is output float array's pointer;
        mask ........ mask for truncation of the input values
                      to the desired resolution:
                      0xFFFF: take 16 MSBs
                      0xFFFE: take 15 MSBs
                      0xFFFC: take 14 MSBs
                      0xFFF8: take 13 MSBs
                      0xFFF0: take 12 MSBs
 
 
 
        Returns value:  none
        ~~~~~~~~~~~~~~
 
        Prototype:  in ugst-utl.h
        ~~~~~~~~~~
 
        Original authors:
        ~~~~~~~~~~~~~~~~~
         V1.0 <bloecher@pkinbg.uucp>
 
        History:
        ~~~~~~~~
        27.Nov.92 v1.0 Version based in sh2fl() v1.1 where output 
		       samples are always normalised to -1.0 ... +1.0.
		       The input vector is not changed.
		       <bloecher@pkinbg.uucp>
 
  --------------------------------------------------------------------------
*/
 
void            sh2fl_alt(n, ix, y, mask)
  long            n;
  short          *ix, mask;
  float          *y;
{
  register long   k;
  register float  factor;


  for (k = 0, factor = (1. / 32768.); k < n; k++)
    *y++ = factor * ((*ix++) & mask);

}              /* ......... end of sh2fl_alt() ......... */

 
/*
  -------------------------------------------------------------------------- 
 
        void sh2fl (long  n, short *ix, float *y,
        ~~~~~~~~~~  long resolution, char norm);
 
        Description:
        ~~~~~~~~~~~~
 
        Common conversion routine. The conversion routine expect the
        fixed point data to be in the range between -32768..32767.
        Conversion to float is done by taking into account only the
        most significant bits, which are right-shifted before the
        conversion to float using the user-specified `resolution',
        and normalized to the range -1..+1 if `norm' is 1.
 
        Parameters:
        ~~~~~~~~~~~
        n ........... is the number of samples in y[];
        ix .......... is input short array's pointer;
        y ........... is output float array's pointer;
        resolution .. is the resolution (number of bits) desired
                      for the input data in the floating point
                      representation.
        norm ........ flag for normalization:
                      1 ... normalize float data to the range -1..+1;
                      0 ... only convert from short to float,
                            leaving the data in the range:
                            -32768>>resolution .. 32767>>resolution.
 
        Returns value:  none
        ~~~~~~~~~~~~~~
 
        Prototype:  in ugst-utl.h
        ~~~~~~~~~~
 
        Original author:
        ~~~~~~~~~~~~~~~~
              tdsimao@venus.cpqd.ansp.br
 
        History:
        ~~~~~~~~
        25.Feb.92 v1.0 Release of 1st version.
                       <tdsimao@venus.cpqd.ansp.br>
        18.May.92 v1.1 Corrected bug in norm.factor calculation
                       <tdsimao@venus.cpqd.ansp.br>
        27.Nov.92 v1.2 Corrected bug when left-adjusting to the 
                       desired resolution <bloecher@pkinbg.uucp>
 
  --------------------------------------------------------------------------
*/
 
void            sh2fl(n, ix, y, resolution, norm)
  long            n;
  short          *ix;
  float          *y;
  long            resolution;
  char            norm;
{
  register long   k;
  float           factor;
 
  /* Shift of left-adjusted samples to the desired resolution */
  if (resolution != 16)
  { /* Block been correct as per suggestion from <bloecher@pkinbg.uucp> */
    register long tmp;
    tmp = 16 - resolution;
    for (k = 0; k < n; k++)
      ix[k] >>= tmp;
  }

  /* Factor for normalization */
  if (norm)
    for (factor = 32768.0, k = 16 - resolution; k > 0; k--)
      factor /= 2;
 
  /* Convert all samples */
  for (k = 0; k < n; k++)
    y[k] = (float) ix[k];
 
  /* Normalize samples, fi requested, to the range -1..+1 */
  if (norm)
    for (k = 0; k < n; k++)
      y[k] /= factor;
 
}                  /* ......... end of sh2fl() ......... */
 
 
 
/*
 ============================================================================
 
  long serialize_right_justifiedstl92 (short *par_buf,
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  short *bit_stm, long n, long resol,
                                        char sync);
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a frame of `n' right-justified samples with a
        resolution `resol' into a bitstream of length n*resol. The bit-
        stream is a sequence of 16-bit words whose least significant
        bits are either EID_ONE or EID_ZERO, depending if the
        correspondent bit in the input word was 1 or 0. (Therefore,
        the bitstream 16-bit words are always right-justified!)
 
        If a synchronism (SYNC_WORD) word is to be included at frame
        boundaries, then the length of the bit stream is increased to
        (n+1)*resol.
 
        ----------------------------------------------------------------
        NOTE! Be sure that the memory reserved (allocated) for "bit_stm"
              is big enough to accomodate all the data:
               - with sync word: ...... (n+1)*resol
               - without sync word: ...  n*resol
        ----------------------------------------------------------------
 
        Please note that the less significant bits of the input word are
        serialized first, such that the bitstream is a stream with less
        significant bits coming first! (Please see example below).
 
        Example: ADPCM input paralell sequence, in blocks of 256
                 samples, for 10 blocks (note that for each frame an
                 individual call to this function is needed!):
 
        Input contents:
 
        frame no.:  |   1st          | | 2nd ...| ... | last one  |
        sample no.:    1    2 ...  256  257 ...         (last one)
        sample:     1011,0101,...,1001 1111 ...          1010
 
        Output:
 
        Word no.:
          1   2 3 4 5 6  ...  *       1026  ...     ...        x
        +----+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+ ... +-+-+-+-+
        |Sync|1|1|0|1|1| ... |1|0|0|1|Sync|1|1|1|1| ... |0|1|0|1|
        +----+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+ ... +-+-+-+-+
        * = word no.1022
        x = word no.10250
 
        Here, the bitstream value `1' is defined by EID_ONE, `0' as
        EID_ZERO, and the sync word  by SYNC_WORD:
 
                      Bit '0' is represented as '0x007F'
                      Bit '1' is represented as '0x0081'
                      SYNC-word is represented as '0x6B21'
 
 
        Parameters:
        ~~~~~~~~~~~
        par_buf ... input buffer with right-adjusted samples to be serialized.
        bit_stm ... output buffer with bitstream.
        n ......... number of words in the input buffer.
        resol ..... resolution (number of bits) of the right-adjusted samples
                    in par_buf.
        sync ...... flag to say whether a sync word is (1) to be used
                    (appended) in the boundaries of each frame of the
                    bitstream, or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~
 
        On success, returns the number of bits of the output bitstream
        (including sync word). If the value returned is 0, the number of
        converted samples is zero.
 
        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        CPqD/Telebras
        DDS/Pr.11
        Rd. Mogi Mirim-Campinas Km.118
        13.085 - Campinas - SP (Brazil)
 
        Phone : +55-192-39-6396
        FAX   : +55-192-53-4754
        EMail : tdsimao@venus.cpqd.ansp.br
 
        History
        ~~~~~~~
        20.Mar.92 v1.0 1st release to UGST.
        18.May.92 v1.1 Initialization in-routine removed.
        06.Jun.95 v2.0 Exchanged definition of softbits '1' and '0' for
                       compatibitilty with EID module, HW, and Rec.G.192
                       <simao@ctd.comsat.com>
 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE   0x0081
#define SYNC_WORD 0x6B21
 
long serialize_right_justifiedstl92 (par_buf, bit_stm, n, resol, sync)
        short *par_buf, *bit_stm;
        long n, resol;
        char sync;
{
  unsigned short tmp, *bs;
  long bs_length;
  long j,k;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate size of bitstream */
  bs_length = n * resol + (sync==1?1:0);
 
 
/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 
  /* Put a sync word at beginning of the frame, if requested */
    if (sync) *bs++ = SYNC_WORD;
 
  /* Convert every sample in parallel buffer into a bitstream,
     including a sync word if requested */
  for (j=0; j<n; j++)
  {
    /* Convert input word to unsigned */
    tmp = (unsigned short)par_buf[j];
 
    /* Serialize all sample's bits ... */
    for (k=0; k<resol; k++)
    {
      *bs++ = (tmp&0x0001)?EID_ONE:EID_ZERO;
      tmp >>= 1;
    }
  }
 
 
/*
  * ......... RETURN bitstream length (include sync word) .........
  */
 
   return((long)((short *)bs-bit_stm));
 
}
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* ............... End of serialize_right_justifiedstl92() ............... */
 
 
 
/*
============================================================================
 
        long parallelize_right_justifiedstl92 (short *bit_stm, short *par_buf,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  long bs_len, long resol, char sync);
 
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a bitstream of length `bs_len' into a frame
        of bs_len/resol(*) right-justified samples with a resolution
        `resol'. The bitstream is a sequence of 16-bit words whose
        least significant bits are either EID_ONE or EID_ZERO, depending
        if the correspondent bit in the originating word was 1 or 0
        (Therefore, the bitstream 16-bit words are always right-justified!).
        Note that the bits that come first are less significant than the
        next ones, when referring to a same word. Then, when mounting a
        word from the bitstream, bits from the bitstream that comes
        first are allocated to lower significant bits (see example
        below).
 
              (*)PS: if there are sync words in the bitstream, the
                     number of samples per frame is (bs_len-1)/resol.
 
        If a synchronism (SYNC_WORD) word is included at frame
        boundaries (parameter sync==1), then sync word is removed from
        the bitstream and not copied to the output buffer.
 
        Example: ADPCM input paralell sequence, in blocks of 256
                 samples, for 10 blocks (note that for each frame an
                 individual call to this function is needed!):
 
        Input contents:
 
        Word no.:
          1   2 3 4 5 6  ...  *       1026  ...     ...        x
        +----+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+ ... +-+-+-+-+
        |Sync|1|1|0|1|1| ... |1|0|0|1|Sync|1|1|1|1| ... |0|1|0|1|
        +----+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+ ... +-+-+-+-+
        * = word no.1022
        x = word no.10250
 
        Output:
 
        frame no.:  |   1st          | | 2nd ...| ... | last one  |
        sample no.:    1    2 ...  256  257 ...         (last one)
        sample:     1011,0101,...,1001 1111 ...          1010
 
        Here, the bitstream value `1' is defined by EID_ONE, `0' as
        EID_ZERO, and the sync word  by SYNC_WORD:
 
                      Bit '0' is represented as '0x007F'
                      Bit '1' is represented as '0x0081'
                      SYNC-word is represented as '0x6B21'
 
        ----------------------------------------------------------------
          NOTE! This routine assumes that memory HAS already been
                allocated to BOTH the bit stream bit_stm and to the
                parallel samples' buffer par_buf.
          ALSO! This routine considers that the first `bit' of the bit-
                stream corresponds to the least significant bit of the
                original word!
        ----------------------------------------------------------------
 
        Parameters:
        ~~~~~~~~~~~
        bit_stm ... input buffer with bitstream to be parallelized.
        par_buf ... output buffer with right-adjusted samples.
        bs_len .... number of bits per frame (ie, size of input buffer).
        resol ..... resolution (number of bits) of the right-adjusted samples
                    in par_buf.
        sync ...... flag to say whether a sync word is (1) to be used
                    (appended) in the boundaries of each frame of the
                    bitstream, or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~
        On success, returns the number of parallel words converted from the
        input bitstream (The sync word is removed and not copied to the
        output buffer).
 
        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        CPqD/Telebras
        DDS/Pr.11
        Rd. Mogi Mirim-Campinas Km.118
        13.085 - Campinas - SP (Brazil)
 
        Phone : +55-192-39-6396
        FAX   : +55-192-53-4754
        EMail : tdsimao@venus.cpqd.ansp.br
 
        History
        ~~~~~~~
        20.Mar.92 v1.0 1st release to UGST.
        06.Jun.95 v2.0 Exchanged definition of softbits '1' and '0' for
                       compatibitilty with EID module, HW, and Rec.G.192
                       <simao@ctd.comsat.com>

 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE   0x0081
#define SYNC_WORD 0x6B21
 
long parallelize_right_justifiedstl92 (bit_stm, par_buf, bs_len, resol, sync)
        short *bit_stm, *par_buf;
        long bs_len, resol;
        char sync;
{
  unsigned short tmp, *bs;
  long n,j,k;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate size of bitstream */
  n = (bs_len - (sync==1?1:0))/resol;
 
/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 
  /* Convert every sample in parallel buffer into a bitstream,
     including a sync word if requested */
  for (j=0; j<n; j++)
  {
    /* Skip sync word if present */
    if (*bs == SYNC_WORD) bs++;
 
    /* Get 1st bit ... */
    tmp = (unsigned short)(*bs++==EID_ONE)?1:0;
 
    /* Parallelize all the other bits ... */
    for (k=1; k<resol; k++)
    {
      tmp += (unsigned short)( ( (*bs++) == EID_ONE ? 1 : 0) << k);
    }
 
    /* Save word as short */
    par_buf[j] = (short)tmp;
  }
 
 
/*
  * ......... RETURN .........
  */
 
   return((long)n);
}
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* ............... End of parallelize_right_justifiedstl92() .............. */
 
 
/*
 ============================================================================
 
        long serialize_left_justifiedstl92  (short *par_buf,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  short *bit_stm, long n, long resol,
                                        char sync);
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a frame of `n' left-justified samples with a
        resolution `resol' into a bitstream of length n*resol. Input
        data is converted first from left-adjusted to right-adjusted
        (both in 2's complement), according to the resolution specified,
        and then converted into a bitstream vector. The bitstream is a
        sequence of 16-bit words whose least significant bits are either
        EID_ONE or EID_ZERO (defined below), depending if the
        correspondent bit in the input word was 1 or 0. (Therefore,
        the bitstream 16-bit words are always right-justified!)
 
        If a synchronism (SYNC_WORD) word is to be included at frame
        boundaries, then the length of the bit stream is increased to
        (n+1)*resol.
 
        ----------------------------------------------------------------
        NOTE! Be sure that the memory reserved (allocated) for "bit_stm"
              is big enough to accomodate all the data:
               - with sync word: ...... (n+1)*resol
               - without sync word: ...  n*resol
        ----------------------------------------------------------------
 
        Please note that the MOST SIGNIFICANT BITS of the left-
        justified input word are SERIALIZED FIRST, such that the
        bitstream is a stream with most significant bits coming first,
        complementary to the right-justified routines above
	(see example there).
 
        Here, the bitstream value `1' is defined by EID_ONE, `0' as
        EID_ZERO, and the sync word  by SYNC_WORD:
 
                      Bit '0' is represented as '0x007F'
                      Bit '1' is represented as '0x0081'
                      SYNC-word is represented as '0x6B21'
  
         Parameters:
        ~~~~~~~~~~~
        par_buf ... input buffer with left-adjusted samples to be serialized.
        bit_stm ... output buffer with bitstream.
        n ......... number of words in the input buffer.
        resol ..... resolution (number of bits) of the left-adjusted samples
                    in par_buf.
        sync ...... flag to say whether a sync word is (1) to be used
                    (appended) in the boundaries of each frame of the
                    bitstream, or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~
        On success, returns the number of bits of the output bitstream
        (including sync word). 
 
        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        CPqD/Telebras
        DDS/Pr.11
        Rd. Mogi Mirim-Campinas Km.118
        13.085 - Campinas - SP (Brazil)
 
        Phone : +55-192-39-6396
        FAX   : +55-192-53-4754
        EMail : tdsimao@venus.cpqd.ansp.br
 
        History
        ~~~~~~~
        20.Mar.92 v1.0 1st release to UGST.
        18.May.92 v1.1 Initialization in-routine removed.
        06.Jun.95 v2.0 Exchanged definition of softbits '1' and '0' for
                       compatibitilty with EID module, HW, and Rec.G.192
                       <simao@ctd.comsat.com>
 
 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE  0x0081
#define SYNC_WORD 0x6B21
 
long serialize_left_justifiedstl92 (par_buf, bit_stm, n, resol, sync)
        short *par_buf, *bit_stm;
        long n, resol;
        char sync;
{
  unsigned short tmp, *bs;
  long bs_length;
  long j,k,l;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate size of bitstream */
  bs_length = n * resol + (sync==1?1:0);
 
 
/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 
  /* Put a sync word at beginning of the frame, if requested */
  if (sync) *bs++ = SYNC_WORD;
 
  /* Convert samples from left- to right-justified in input buffer */
  l = 16-resol;
 
  /* Convert every sample in parallel buffer into a bitstream,
     including a sync word if requested */
  for (j=0; j<n; j++)
  {
    /* Convert input word to unsigned */
    tmp = (unsigned short)(par_buf[j] >> l);
 
    /* Serialize all sample's bits ... */
    for (k=0; k<resol; k++)
    {
      *bs++ = (tmp&0x0001)?EID_ONE:EID_ZERO;
      tmp >>= 1;
    }
  }
 
 
/*
  * ......... RETURN bitstream length (include sync word) .........
  */
 
   return((long)((short *)bs-bit_stm));
 
}
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* ............... End of serialize_left_justifiedstl92() ............... */
 
 
 
/*
============================================================================
 
        long parallelize_left_justifiedstl92 (short *bit_stm, short *par_buf,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ long bs_len, long resol, char sync);
 
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a bitstream of length `bs_len' into a frame
        of bs_len/resol(*) left-justified samples with a resolution
        `resol'. The bitstream is a sequence of 16-bit words whose
        least significant bits are either EID_ONE or EID_ZERO, depending
        if the correspondent bit in the originating word was 1 or 0.
        (Therefore, the bitstream 16-bit words are always right-justified!)
        Note that the bits that come first are less significant than the
        next ones, when referring to a same word. Then, when mounting a
        word from the bitstream, bits from the bitstream that comes
        first are allocated to lower significant bits, as for the
        right-justified routines above (see example there). Output data
        is converted from right-adjusted to left-adjusted (both in 2's
        complement) before returning, according to the resolution
        specified.
 
              (*)PS: if there are sync words in the bitstream, the
                     number of samples per frame is (bs_len-1)/resol.
 
        If a synchronism (SYNC_WORD) word is included at frame
        boundaries (parameter sync==1), then sync word is removed from
        the bitstream and not copied to the output buffer.
 
        Here, the bitstream value `1' is defined by EID_ONE, `0' as
        EID_ZERO, and the sync word  by SYNC_WORD:
 
                      Bit '0' is represented as '0x007F'
                      Bit '1' is represented as '0x0081'
                      SYNC-word is represented as '0x6B21'
 
        ----------------------------------------------------------------
          NOTE! This routine assumes that memory HAS already been
                allocated to BOTH the bit stream bit_stm and to the
                parallel samples' buffer par_buf.
          ALSO! This routine considers that the first `bit' of the bit-
                stream corresponds to the least significant bit of the
                original word!
        ----------------------------------------------------------------
 
        Parameters:
        ~~~~~~~~~~~
        bit_stm ... input buffer with bitstream to be parallelized.
        par_buf ... output buffer with left-adjusted samples.
        bs_len .... number of bits per frame (ie, size of input buffer).
        resol ..... resolution (number of bits) of the left-adjusted samples
                    in par_buf.
        sync ...... flag to say whether a sync word is (1) to be used
                    (appended) in the boundaries of each frame of the
                    bitstream, or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~
        On success, returns the number of parallel words extracted from the
        input bitstream (The sync word is removed and not copied to the
        output buffer).
 
        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        CPqD/Telebras
        DDS/Pr.11
        Rd. Mogi Mirim-Campinas Km.118
        13.085 - Campinas - SP (Brazil)
 
        Phone : +55-192-39-6396
        FAX   : +55-192-53-4754
        EMail : tdsimao@venus.cpqd.ansp.br
 
        History
        ~~~~~~~
        20.Mar.92 v1.0 1st release to UGST.
        06.Jun.95 v2.0 Exchanged definition of softbits '1' and '0' for
                       compatibitilty with EID module, HW, and Rec.G.192
                       <simao@ctd.comsat.com>

 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE   0x0081
#define SYNC_WORD 0x6B21
 
long parallelize_left_justifiedstl92 (bit_stm, par_buf, bs_len, resol, sync)
        short *bit_stm, *par_buf;
        long bs_len, resol;
        char sync;
{
  unsigned short tmp, *bs;
  long n,j,k;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate size of bitstream */
  n = (bs_len - (sync==1?1:0))/resol;
 
/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 
  /* Convert every sample in parallel buffer into a bitstream,
     including a sync word if requested */
  for (j=0; j<n; j++)
  {
    /* Skip sync word if present */
    if (*bs == SYNC_WORD) bs++;
 
    /* Get 1st bit ... */
    tmp = (unsigned)(*bs++==EID_ONE)?1:0;
 
    /* Parallelize all the other bits ... */
    for (k=1; k<resol; k++)
    {
      tmp += (unsigned)( ( (*bs++) == EID_ONE ? 1 : 0) << k);
    }
 
    /* Sign extension is needed if last bit was a `1' ... */
    if (*(bs-1) == EID_ONE)
      for (;k<16;k++) tmp += (1 << k);
 
    /* Save word as short */
    par_buf[j] = (short)tmp;
  }
 
 
/*
  * Convert samples from right- to left-justified in output buffer
  *               (only needed if resol is not 16!)
  */
 
  if ((resol = 16-resol)!=0)
  for (j=0; j<n; j++)
  {
    /* Shift up */
    k = (long)par_buf[j] << resol;
 
    /* Check for overflow and save back to output vector */
    par_buf[j] = (k > 32767) ? 32767 :
                               (k < -32768? (-32768) : (short)k );
  }
 
 
/*
  * ......... RETURN .........
  */
 
   return((long)n);
}
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* .............. End of parallelize_left_justifiedstl92() .............. */
 



/* ********************************************************************** */
/* ********************************************************************** */
/* ********************************************************************** */
/* ********* THE FUNCTIONS BELOW ARE NEW/MODIFIED FOR THE STL96 ********* */
/* ********************************************************************** */
/* ********************************************************************** */
/* ********************************************************************** */


/*
 ============================================================================
 
        long serialize_right_justifiedstl96 (short *par_buf,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  short *bit_stm, long n, long resol,
                                        char sync);
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a frame of `n' right-justified samples with a
        resolution `resol' into a bitstream of length n*resol. The bit-
        stream is a sequence of 16-bit words whose least significant
        bits are either EID_ONE or EID_ZERO, depending if the
        correspondent bit in the input word was 1 or 0. (Therefore,
        the bitstream 16-bit words are always right-justified!)
 
        If the G.192 Annex B bitstream format is to be used, a
        synchronism (SYNC_WORD) word will be included at frame
        boundaries, followed by a 16-bit word indicating the frame
        length, i.e., the number of softbits in the frame. In this
        case, the the length of the bit stream is increased to
        (n+2)*resol. The option of adding only the sync word, as
        implemented in the STL92, is no longer available with this
        function.
 
        ----------------------------------------------------------------
        NOTE! Be sure that the memory reserved (allocated) for "bit_stm"
              is big enough to accomodate all the data:
               - with sync word + frame length: ...... (n+2)*resol shorts
               - without sync word: ..................  n*resol shorts
        ----------------------------------------------------------------
 
        Please note that the LEAST SIGNIFICANT BITS of the
        right-justified input words are serialized FIRST, such that
        the bitstream is a stream with less significant bits coming
        first! (Please see example below).
 
        Example: ADPCM input paralell sequence, in blocks of 256
                 samples, for 10 blocks (note that for each frame an
                 individual call to this function is needed!):
 
        Input contents:
 
        frame no.:  |   1st          | | 2nd ...| ... | last one  |
        sample no.:    1    2 ...  256  257 ...         (last one)
        sample:     1011,0101,...,1001 1111 ...          1010
 
        Output:
 
        Word no. (in output bitstream array):
          1   2 3 4 5 6 7  ...  *       1027  ...       ...        x
        +----+-+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+-+ ... +-+-+-+-+
        |Sync|#|1|1|0|1|1| ... |1|0|0|1|Sync|#|1|1|1|1| ... |0|1|0|1|
        +----+-+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+-+ ... +-+-+-+-+
        # = 1024 (i.e., 256 samples/frame * 4 bits/sample = 1024 bits/frame)
        * = word no.1022
        x = word no.10260
 
        Here, the bitstream value `1' is defined by the softbit
        EID_ONE, `0' as the softbit EID_ZERO, and the sync word by
        SYNC_WORD:
	    Hardbit '0' is represented as the softbit '0x007F'
	    Hardbit '1' is represented as the softbit '0x0081'
	    SYNC-word is represented as the word '0x6B21'
	    # is represented in two-complement notation
 
 
        Parameters:
        ~~~~~~~~~~~
        par_buf ... input buffer with right-adjusted samples to be serialized.
        bit_stm ... output buffer with bitstream.
        n ......... number of words in the input buffer, i.e., the number of
	            parallel samples/frame.
        resol ..... resolution (number of bits) of the right-adjusted samples
                    in par_buf.
        sync ...... flag to say whether the G.192 Annex B bitstream 
                    format (1) is to be used (i.e., a sync word and a 
                    bitstream length word are added at frame boundaries), 
                    or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~
 
        On success, returns the total number of softbits in the output
        bitstream, including the including sync word and frame length.
        If the value returned is 0, the number of converted samples is
        zero.
 
        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com
 
        History
        ~~~~~~~
        06.Mar.96 v1.0 Created based on the STL92 version, however using 
                       the bitstream definition in Annex B of G.192.
                       <simao@ctd.comsat.com>
 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE   0x0081
#define SYNC_WORD 0x6B21
 
long serialize_right_justifiedstl96 (par_buf, bit_stm, n, resol, sync)
        short *par_buf, *bit_stm;
        long n, resol;
        char sync;
{
  register unsigned short tmp, *bs;
  register unsigned short bs_length;
  long j,k;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate size of softbits in bitstream */
  bs_length = n * resol;
 
  /* Initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 
 
/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* Put a sync and length word at beginning of the frame, if requested */
  if (sync) 
  {
    /* Frame boundaries have the sync word and the number of
       softbits per frame */
    *bs++ = SYNC_WORD;
    *bs++ = bs_length;
  }
 
  /* Convert every sample in parallel buffer into a bitstream,
     including a sync word if requested */
  for (j=0; j<n; j++)
  {
    /* Convert input right-justified word to unsigned */
    tmp = (unsigned short)par_buf[j];
 
    /* Serialize all sample's bits ... */
    for (k=0; k<resol; k++)
    {
      *bs++ = (tmp&0x0001)?EID_ONE:EID_ZERO;
      tmp >>= 1;
    }
  }
 
 
/*
  * ......... RETURN bitstream length (include sync word) .........
  */
 
   return((long)((short *)bs-bit_stm));
 
}
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* ............... End of serialize_right_justifiedstl96() ............... */
 
 
 
/*
============================================================================
 
        long parallelize_right_justifiedstl96 (short *bit_stm, short *par_buf,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  long bs_len, long resol, char sync);
 
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a bitstream of length `bs_len' into a frame
        of bs_len/resol(*) right-justified samples with a resolution
        `resol'. The bitstream is a sequence of 16-bit words whose
        least significant bits are either EID_ONE or EID_ZERO, depending
        if the correspondent bit in the originating word was 1 or 0
        (Therefore, the bitstream 16-bit words are always right-justified!).
        Note that the bits that come first are less significant than the
        next ones, when referring to the same word. Then, when mounting a
        word from the bitstream, bits from the bitstream that comes
        first are converted to lower significant bits (see example
        below).
 
              (*)PS: if the bitstream is compatible with G.192 Annex B, 
                     the number of parallel samples per frame  
                     is (bs_len-2)/resol.
 
        If the G.192 Annex B bitstream format is used (parameter
        sync==1), a synchronism (SYNC_WORD) word followed by a 16-bit
        word (indicating the number of softbits per frame) are
        present at frame boundaries in the input buffer. In this
        case, the sync and frame lengthwords are removed from the
        bitstream and not copied to the output buffer.

        Note that all parallel samples are supposed to have a constant
        number of bits for the whole frame. This means that, by
        construction, the number of softbits divided by the resolution
        must be an integer number, or (bs_len-2)%resol==0. If this
        does not happen, probably the serial bitstream was not
        generated by one of the serialize_...() routines, and cannot
        be parallelized by this function. As an example, this is the
        case of the RPE-LTP bitstream: the 260 bits of the encoded
        bitstream are not divided equally among the 76 parameters of
        the bitstream. In cases like this, the user must write its own
        parallelization function when the number of bits is not
        constant for all parallel samples in the frame.
 
        Example: ADPCM input paralell sequence, in blocks of 256
                 samples, for 10 blocks (note that for each frame an
                 individual call to this function is needed!):
 
        Input contents:
 
        Word no. (in input bitstream array):
          1   2 3 4 5 6 7  ...  *       1027  ...       ...        x
        +----+-+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+-+ ... +-+-+-+-+
        |Sync|#|1|1|0|1|1| ... |1|0|0|1|Sync|#|1|1|1|1| ... |0|1|0|1|
        +----+-+-+-+-+-+-+ ... +-+-+-+-+----+-+-+-+-+-+ ... +-+-+-+-+
        # = 1024 (i.e., 256 samples/frame * 4 bits/sample = 1024 bits/frame)
        * = word no.1022
        x = word no.10260
 
        Output:
 
        frame no.:  |   1st          | | 2nd ...| ... | last one  |
        sample no.:    1    2 ...  256  257 ...         (last one)
        sample:     1011,0101,...,1001 1111 ...          1010
 
        Here, the bitstream value `1' is defined by the softbit
        EID_ONE, `0' as the softbit EID_ZERO, and the sync word by
        SYNC_WORD:
	    Hardbit '0' is represented as the softbit '0x007F'
	    Hardbit '1' is represented as the softbit '0x0081'
	    SYNC-word is represented as the word '0x6B21'
	    # is represented in two-complement notation

 
        ----------------------------------------------------------------
          NOTE! This routine assumes that memory HAS already been
                allocated to BOTH the bit stream bit_stm and to the
                parallel samples' buffer par_buf.
          ALSO! This routine considers that the first `bit' of the bit-
                stream corresponds to the least significant bit of the
                original word!
        ----------------------------------------------------------------
 
        Parameters:
        ~~~~~~~~~~~
        bit_stm ... input buffer with bitstream to be parallelized.
        par_buf ... output buffer with right-adjusted samples.
        bs_len .... number of bits per frame (ie, size of input buffer, 
                    which includes sync/length words if sync==1).
        resol ..... resolution (number of bits) of the right-adjusted samples
                    in par_buf.
        sync ...... flag to say whether the G.192 Annex B bitstream 
                    format (1) is to be used (i.e., a sync word and a 
                    bitstream length word are added at frame boundaries), 
                    or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~ 

        On success, returns the number of parallel words extracted
        from the input bitstream (the sync and legth words are removed
        and not copied to the output buffer). If the number of sofbits
        (plus sync and legth words) expected by the user (as specified
        in parameter bs_len) does not match with the bitstream length
        word (bit_stm[1]), a negative number (-bs_len[1]) is returned,
        and all samples in the output frame are set to 0. If the number
        of softbits in the frame is not an integer multiple of the
        number parallel samples, then -1 is returned (all samples in
        par_buf are also set to -1).  If a bad-frame indicator
        (0x6B20) is present, no valid samples are returned and the
        output buffer will contain just zero samples.

        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com
 
        History
        ~~~~~~~
        06.Mar.96 v1.0 Created based on the STL92 version, however using 
                       the bitstream definition in Annex B of G.192.
                       <simao@ctd.comsat.com>

 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE   0x0081
#define SYNC_WORD 0x6B21
#define BAD_FRAME 0x6B20
 
long parallelize_right_justifiedstl96 (bit_stm, par_buf, bs_len, resol, sync)
        short *bit_stm, *par_buf;
        long bs_len, resol;
        char sync;
{
  unsigned short tmp, *bs;
  long n,j,k;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate size of bitstream */
  n = (bs_len - (sync==1?2:0))/resol;

  /* Initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 
  /* Check if number of softbits in frame is consistent with the resolution */
  if (n*resol + (sync==1?2:0) != bs_len)
  {
    /* The bitstream does not have an integer number of parallel samples
       of width resol ... */

    /* ... set all samples in bitstream to -1 */
    memset(par_buf, -1, n * sizeof(short));

    /* ... and return error code */
    return(-1);
  }
  else
  {
    /* No problems - reset samples in parallel sample buffer */
    memset(par_buf, 0, n * sizeof(short));
  }


/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* Convert every softbit in serial buffer to a parallel sample format */
  for (j=0; j<n; j++)
  {
    /* If bad frame indicator present, no valid samples are returned.
       The output buffer will contain just zero samples */
    if (*bs == BAD_FRAME)
      return(0l);

    /* Skip sync word if present */
    if (*bs == SYNC_WORD)
    {
      /* Skip sync word */
      bs++;
      /* Check frame length for consistency */
      if (*bs++ != (bs_len-2))
	return(-bs_len);
    }
 
    /* Get 1st bit ... */
    tmp = (unsigned short)(*bs++==EID_ONE)?1:0;
 
    /* Parallelize all the other bits ... */
    for (k=1; k<resol; k++)
    {
      tmp += (unsigned short)( ( (*bs++) == EID_ONE ? 1 : 0) << k);
    }
 
    /* Save word as short */
    par_buf[j] = (short)tmp;
  }
 
 
/*
  * ......... RETURN .........
  */
 
   return((long)n);
}
#undef BAD_FRAME
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* .............. End of parallelize_right_justifiedstl96() .............. */
 
 
/*
 ============================================================================
 
        long serialize_left_justifiedstl96  (short *par_buf,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  short *bit_stm, long n, long resol,
                                        char sync);
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a frame of `n' left-justified samples with a
        resolution `resol' into a bitstream of length n*resol. Input
        data is converted first from left-adjusted to right-adjusted
        (both in 2's complement), according to the resolution specified,
        and then converted into a bitstream vector. The bitstream is a
        sequence of 16-bit words whose least significant bits are either
        EID_ONE or EID_ZERO (defined below), depending if the
        correspondent bit in the input word was 1 or 0. (Therefore,
        the bitstream 16-bit words are always right-justified!)
 
        If the G.192 Annex B bitstream format is to be used, a
        synchronism (SYNC_WORD) word will be included at frame
        boundaries, followed by a 16-bit word indicating the frame
        length, i.e., the number of softbits in the frame. In this
        case, the the length of the bit stream is increased to
        (n+2)*resol. The option of adding only the sync word, as
        implemented in the STL92, is no longer available with this
        function.
 
        ----------------------------------------------------------------
        NOTE! Be sure that the memory reserved (allocated) for "bit_stm"
              is big enough to accomodate all the data:
               - with sync word + frame length: ...... (n+2)*resol
               - without sync word: ..................  n*resol
        ----------------------------------------------------------------
 
        Please note that the LEAST SIGNIFICANT BITS of the
        right-justified input words are serialized FIRST, such that
        the bitstream is a stream with less significant bits coming
        first! (Please see example in serialize_right_justified() above).

        The bitstream value `1' is defined by the softbit
        EID_ONE, `0' as the softbit EID_ZERO, and the sync word by
        SYNC_WORD:
	    Hardbit '0' is represented as the softbit '0x007F'
	    Hardbit '1' is represented as the softbit '0x0081'
	    SYNC-word is represented as the word '0x6B21'
	    # is represented in two-complement notation
  
        Parameters:
        ~~~~~~~~~~~
        par_buf ... input buffer with left-adjusted samples to be serialized.
        bit_stm ... output buffer with bitstream.
        n ......... number of words in the input buffer, i.e., the number of
	            parallel samples/frame.
        resol ..... resolution (number of bits) of the left-adjusted samples
                    in par_buf.
        sync ...... flag to say whether the G.192 Annex B bitstream 
                    format (1) is to be used (i.e., a sync word and a 
                    bitstream length word are added at frame boundaries), 
                    or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~
        On success, returns the number of bits of the output bitstream
        (including sync word).
 
        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com
 
        History
        ~~~~~~~
        06.Mar.96 v1.0 Created based on the STL92 version, however using 
                       the bitstream definition in Annex B of G.192.
                       <simao@ctd.comsat.com>

 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE  0x0081
#define SYNC_WORD 0x6B21
 
long serialize_left_justifiedstl96 (par_buf, bit_stm, n, resol, sync)
        short *par_buf, *bit_stm;
        long n, resol;
        char sync;
{
  unsigned short tmp, *bs;
  long bs_length;
  long j,k,l;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate number of softbits in bitstream frame*/
  bs_length = n * resol;

  /* Initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 

/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* Put a sync word at beginning of the frame, if requested */
  if (sync) 
  {
    /* Frame boundaries have the sync word and the number of
       softbits per frame */
    *bs++ = SYNC_WORD;
    *bs++ = bs_length;
  }
 
  /* Convert samples from left- to right-justified in input buffer */
  l = 16-resol;
 
  /* Convert every sample in parallel buffer into a bitstream,
     including a sync word if requested */
  for (j=0; j<n; j++)
  {
    /* Convert input word to unsigned */
    tmp = (unsigned short)(par_buf[j] >> l);
 
    /* Serialize all sample's bits ... */
    for (k=0; k<resol; k++)
    {
      *bs++ = (tmp&0x0001)?EID_ONE:EID_ZERO;
      tmp >>= 1;
    }
  }
 
 
/*
  * ......... RETURN bitstream length (include sync word) .........
  */
 
   return((long)((short *)bs-bit_stm));
 
}
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* ............... End of serialize_left_justifiedstl96() ............... */
 
 
 
/*
============================================================================
 
        long parallelize_left_justifiedstl96 (short *bit_stm, short *par_buf,
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ long bs_len, long resol, char sync);
 
 
        Description:
        ~~~~~~~~~~~~
 
        Routine to convert a bitstream of length `bs_len' into a frame
        of bs_len/resol(*) left-justified samples with a resolution
        `resol'. The bitstream is a sequence of 16-bit words whose
        least significant bits are either EID_ONE or EID_ZERO, depending
        if the correspondent bit in the originating word was 1 or 0.
        (Therefore, the bitstream 16-bit words are always right-justified!)
        Note that the bits that come first are less significant than the
        next ones, when referring to a same word. Then, when mounting a
        word from the bitstream, bits from the bitstream that comes
        first are allocated to lower significant bits, as for the
        right-justified routines above (see example there). Output data
        is converted from right-adjusted to left-adjusted (both in 2's
        complement) before returning, according to the resolution
        specified.
 
              (*)PS: if there are sync words in the bitstream, the
                     number of samples per frame is (bs_len-2)/resol.
 
        If the G.192 Annex B bitstream format is used (parameter
        sync==1), a synchronism (SYNC_WORD) word followed by a 16-bit
        word (indicating the number of softbits per frame) are
        present at frame boundaries in the input buffer. In this
        case, the sync and frame lengthwords are removed from the
        bitstream and not copied to the output buffer.

        Note that all parallel samples are supposed to have a constant
        number of bits for the whole frame. This means that, by
        construction, the number of softbits divided by the resolution
        must be an integer number, or (bs_len-2)%resol==0. If this
        does not happen, probably the serial bitstream was not
        generated by one of the serialize_...() routines, and cannot
        be parallelized by this function. As an example, this is the
        case of the RPE-LTP bitstream: the 260 bits of the encoded
        bitstream are not divided equally among the 76 parameters of
        the bitstream. In cases like this, the user must write its own
        parallelization function when the number of bits is not
        constant for all parallel samples in the frame.

        The bitstream value `1' is defined by the softbit
        EID_ONE, `0' as the softbit EID_ZERO, and the sync word by
        SYNC_WORD:
	    Hardbit '0' is represented as the softbit '0x007F'
	    Hardbit '1' is represented as the softbit '0x0081'
	    SYNC-word is represented as the word '0x6B21'
	    # is represented in two-complement notation
 
        ----------------------------------------------------------------
          NOTE! This routine assumes that memory HAS already been
                allocated to BOTH the bit stream bit_stm and to the
                parallel samples' buffer par_buf.
          ALSO! This routine considers that the first `bit' of the bit-
                stream corresponds to the least significant bit of the
                original word!
        ----------------------------------------------------------------
 
        Parameters:
        ~~~~~~~~~~~
        bit_stm ... input buffer with bitstream to be parallelized.
        par_buf ... output buffer with left-adjusted samples.
        bs_len .... number of bits per frame (ie, size of input buffer, 
                    which includes sync/length words if sync==1).
        resol ..... resolution (number of bits) of the left-adjusted samples
                    in par_buf.
        sync ...... flag to say whether the G.192 Annex B bitstream 
                    format (1) is to be used (i.e., a sync word and a 
                    bitstream length word are added at frame boundaries), 
                    or (0) not.
 
        Return value:
        ~~~~~~~~~~~~~
        On success, returns the number of parallel words extracted
        from the input bitstream (the sync and legth words are removed
        and not copied to the output buffer). If the number of sofbits
        (plus sync and legth words) expected by the user (as specified
        in parameter bs_len) does not match with the bitstream length
        word (bit_stm[1]), a negative number (-bs_len[1]) is returned,
        and all samples in the output frame are set to 0. If the number
        of softbits in the frame is not an integer multiple of the
        number parallel samples, then -1 is returned (all samples in
        par_buf are also set to -1).  If a bad-frame indicator
        (0x6B20) is present, no valid samples are returned and the
        output buffer will contain just zero samples.

        Original author:
        ~~~~~~~~~~~~~~~~
        Simao Ferraz de Campos Neto
        Comsat Laboratories                  Tel:    +1-301-428-4516
        22300 Comsat Drive                   Fax:    +1-301-428-9287
        Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com
 
        History
        ~~~~~~~
        06.Mar.96 v1.0 Created based on the STL92 version, however using 
                       the bitstream definition in Annex B of G.192.
                       <simao@ctd.comsat.com>

 ============================================================================
*/
#define EID_ZERO  0x007F
#define EID_ONE   0x0081
#define SYNC_WORD 0x6B21
#define BAD_FRAME 0x6B20
 
long parallelize_left_justifiedstl96 (bit_stm, par_buf, bs_len, resol, sync)
        short *bit_stm, *par_buf;
        long bs_len, resol;
        char sync;
{
  unsigned short tmp, *bs;
  long n,j,k;
 
 
/*
  * ......... INITIALIZATIONS .........
  */
 
  /* Calculate size of bitstream */
  n = (bs_len - (sync==1?2:0))/resol;
 
  /* Initialize unsigned pointer to input (potentially signed) and
     output buffers */
  bs = (unsigned short *)bit_stm;
 
  /* Check if number of softbits in frame is consistent with the resolution */
  if (n*resol + (sync==1?2:0) != bs_len)
  {
    /* The bitstream does not have an integer number of parallel samples
       of width resol ... */

    /* ... set all samples in bitstream to -1 */
    memset(par_buf, -1, n * sizeof(short));

    /* ... and return error code */
    return(-1);
  }
  else
  {
    /* No problems - reset samples in parallel sample buffer */
    memset(par_buf, 0, n * sizeof(short));
  }


/*
  * ......... PROCEEDS NORMAL PROCESSING .........
  */
 
  /* Convert every sample in parallel buffer into a bitstream,
     including a sync word if requested */
  for (j=0; j<n; j++)
  {
    /* If bad frame indicator present, no valid samples are returned.
       The output buffer will contain just zero samples */
    if (*bs == BAD_FRAME)
      return(0l);

    /* Skip sync word if present */
    if (*bs == SYNC_WORD)
    {
      /* Skip sync word */
      bs++;
      /* Check frame length for consistency */
      if (*bs++ != (bs_len-2))
	return(-bs_len);
    }
 
    /* Get 1st bit ... */
    tmp = (unsigned)(*bs++==EID_ONE)?1:0;
 
    /* Parallelize all the other bits ... */
    for (k=1; k<resol; k++)
    {
      tmp += (unsigned)( ( (*bs++) == EID_ONE ? 1 : 0) << k);
    }
 
    /* Sign extension is needed if last bit was a `1' ... */
    if (*(bs-1) == EID_ONE)
      for (;k<16;k++) tmp += (1 << k);
 
    /* Save word as short */
    par_buf[j] = (short)tmp;
  }
 
 
/*
  * Convert samples from right- to left-justified in output buffer
  *               (only needed if resol is not 16!)
  */
 
  if ((resol = 16-resol)!=0)
  for (j=0; j<n; j++)
  {
    /* Shift up */
    k = (long)par_buf[j] << resol;
 
    /* Check for overflow and save back to output vector */
    par_buf[j] = (k > 32767) ? 32767 :
                               (k < -32768? (-32768) : (short)k );
  }
 
 
/*
  * ......... RETURN .........
  */
 
   return((long)n);
}
#undef BAD_FRAME
#undef EID_ONE
#undef EID_ZERO
#undef SYNC_WORD
/* ............... End of parallelize_left_justifiedstl96() ............... */
 
/* ......................... END OF UGST-UTL.C .......................... */

