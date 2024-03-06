/*
  ============================================================================
   File: UGST-UTL.H                                                06.Mar.96
  ============================================================================
 
                         UGST/ITU-T UTILITIES MODULE
 
                         GLOBAL FUNCTION  PROTOTYPES
 
   History:
   28.Feb.92    v1.0    First version <tdsimao@venus.cpqd.ansp.br>
   18.May.92    v1.1    Initialization in-routine removed from serialize_*.
   27.Nov.92    v1.2    erroneous mask in "fl2sh_14bit" corrected
			<hf@pkinbg.uucp> and created sh2fl_alt(), an
			alternate to sh2fh() <bloecher@pkinbg.uucp>.
   06.Jun.95    v2.0    Updated version no. (to be inline with module) <simao>
   06.Mar.96    v3.0    Updated to reflect new parallelize_...() and 
                        serialize_...() functions which comply to the
                        bitstream definition given in Annex B of
                        G.192. Use of the STL92 functions instead of
                        the G.192-compliant functions is made by the
                        the definition of the symbol STL92 at compile
                        time <simao@ctd.comsat.com>
  ============================================================================
*/
#ifndef UGST_UTILITIES_defined
#define UGST_UTILITIES_defined 300
 
/* macros for smart prototypes */
#ifndef ARGS
#if (defined(__STDC__) || defined(VMS) || defined(__DECC)  || defined(MSDOS) || defined(__MSDOS__))
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* macros for UGST utility functions */
#define fl2sh_16bit(n,x,y,r) fl2sh(n,x,y,r?0.5:0.0,(short)0xFFFF)
#define fl2sh_15bit(n,x,y,r) fl2sh(n,x,y,r?1.0:0.0,(short)0xFFFE)
#define fl2sh_14bit(n,x,y,r) fl2sh(n,x,y,r?2.0:0.0,(short)0xFFFC)
#define fl2sh_13bit(n,x,y,r) fl2sh(n,x,y,r?4.0:0.0,(short)0xFFF8)
#define fl2sh_12bit(n,x,y,r) fl2sh(n,x,y,r?8.0:0.0,(short)0xFFF0)
 
/* -------------------------------------------------------------------------- */
/* sh2fl_alt performs only conversion to values, normalised to -1.0 ... +1.0  */
/* For compatibility reasons, "nrm" argument is kept in sh2fl_**bit           */
/*                                            <bloecher@pkinbg.uucp>          */
/* -------------------------------------------------------------------------- */
#define sh2fl_16bit(n,x,y,nrm) (nrm ? \
				sh2fl_alt(n,x,y,(short)0xFFFF) : \
				sh2fl(n,x,y,16,nrm) )
#define sh2fl_15bit(n,x,y,nrm) (nrm ? \
				sh2fl_alt(n,x,y,(short)0xFFFE) : \
				sh2fl(n,x,y,15,nrm) )
#define sh2fl_14bit(n,x,y,nrm) (nrm ? \
				sh2fl_alt(n,x,y,(short)0xFFFC) : \
				sh2fl(n,x,y,14,nrm) )
#define sh2fl_13bit(n,x,y,nrm) (nrm ? \
				sh2fl_alt(n,x,y,(short)0xFFF8) : \
				sh2fl(n,x,y,13,nrm) )
#define sh2fl_12bit(n,x,y,nrm) (nrm ? \
				sh2fl_alt(n,x,y,(short)0xFFF0) : \
				sh2fl(n,x,y,12,nrm) )

/* VAX ideosynchrasies */
#ifdef VAX11C
#define CHAR char
#define SHORT short
#else
#define CHAR int
#define SHORT int
#endif

 
/*
 * .... Choose the appropriate set of parallelize_...() and
 * .... serialize_...() routines. By default, post-STL92 functions
 * .... are used.
 */
#ifdef STL92
# define parallelize_left_justified parallelize_left_justifiedstl92
# define parallelize_right_justified parallelize_right_justifiedstl92
# define serialize_left_justified serialize_left_justifiedstl92
# define serialize_right_justified serialize_right_justifiedstl92
#else
# define parallelize_left_justified parallelize_left_justifiedstl96
# define parallelize_right_justified parallelize_right_justifiedstl96
# define serialize_left_justified serialize_left_justifiedstl96
# define serialize_right_justified serialize_right_justifiedstl96
#endif

/* Prototypes for UGST utility functions - generated automatically by cproto */
long scale ARGS((float *buffer, long smpno, double factor));
long fl2sh ARGS((long n, float *x, short *iy, double half_lsb, SHORT mask));
void sh2fl_alt ARGS((long n, short *ix, float *y, SHORT mask));
void sh2fl ARGS((long n, short *ix, float *y, long resolution, CHAR norm));
long serialize_right_justified ARGS((short *par_buf, short *bit_stm, long n, 
		long resol, CHAR sync));
long parallelize_right_justified ARGS((short *bit_stm, short *par_buf, 
		long bs_len, long resol, CHAR sync));
long serialize_left_justified ARGS((short *par_buf, short *bit_stm, long n, 
		long resol, CHAR sync));
long parallelize_left_justified ARGS((short *bit_stm, short *par_buf, 
		long bs_len, long resol, CHAR sync)); 

#define IS_SERIAL -1
#define IS_PARALLEL 1
 
#endif
/* ........................ End of UGST-UTL.H .......................... */
