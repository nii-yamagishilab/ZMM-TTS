/* ------------------------------------------------------------------------- */
/* File: UGSTDEMO.H version 2.3                                  03-Feb-2010 */
/* ------------------------------------------------------------------------- */
/*                   Definitions for UGST demo programs                      */
/* ------------------------------------------------------------------------- */
#ifndef UGSTDEMO_defined
#define UGSTDEMO_defined 230

/* DEFINITION FOR OPERATING SYSTEMS */

#ifndef MSDOS
#if defined(__MSDOS__) || defined (__CYGWIN__) || defined (_MSC_VER)
# define MSDOS
#endif
#endif

#if defined(VMS) 
# if defined(__GNUC__)
#  define COMPILER "GCC-VMS"
# else
#  define COMPILER "VaxC-VMS"
# endif
#elif defined (__CYGWIN__)
# define COMPILER "GCC/CYGWIN-Win32"
#elif defined (_MSC_VER)
# define COMPILER "Microsoft Visual C-Win32"
#elif defined (__TURBOC__)
# define COMPILER "Borland Turbo-C-Win16"
#elif defined (__BORLANDC__)
# define COMPILER "Borland-C-Win16"
#elif defined(__GNUC__) && defined(MSDOS)
# define COMPILER "GCC/DJGPP-Win16"
#elif defined(__hpux)
# if defined(__GNUC__)
#  define COMPILER "GCC-HPUX"
# else
#  define COMPILER "Unknown compiler/HPUX"
# endif
#elif defined(sun)
# if defined(__GNUC__)
#  define COMPILER "GCC-Sun"
# elif !defined (__unix__)
#  define COMPILER "CC-Sun"
# else
#  define COMPILER "ACC-SUN"
# endif
#elif defined(__alpha)
# define COMPILER "DecC-Alpha"
#elif defined(__GNUC__)
# define COMPILER "GCC-Unknown platform"
#else
# define COMPILER "Unknown compiler/Platform"
#endif

/* DEFINITION FOR MEASURING CPU-TIME CONSUMPTION */

#include <time.h>
#include <ctype.h>
#include <string.h>

#ifndef CLOCKS_PER_SEC
#if defined(__TURBOC__)|defined(VMS)	/* For Borland and Vax C compilers */
#define CLOCKS_PER_SEC CLK_TCK
#elif defined(MSDOS)		/* Only for Microsoft C compilers */
#define CLOCKS_PER_SEC 1
#else				/* Unix */
#define CLOCKS_PER_SEC 1000000
#endif				/* if */
#endif				/* ifndef */

/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if defined(MSDOS) || defined(__STDC__) || defined(VMS) || defined(__DECC)
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif

/* Definition for maximum string length (terminator included) */
#ifndef MAX_STRLEN
#define MAX_STRLEN 1024
#endif

/* 
   ----------------------------------------------------------------------- 
	   DEFINITIONS FOR COMAND LINE INPUT OF PARAMETERS
   ----------------------------------------------------------------------- 
                     Asking User for Parameter Input                      
   ------------------------------------------------------------------------ 
   if the desired parameter has not been entered, the user is asked
   for this with a prompt string; otherwise the same prompt string is
   printed to the screen, followed by the value read for the parameter
 -------------------------------------------------------------------------- 
*/

/* Modified 02-Feb-2010 by y.hiwasaki (explicit casting) */
#define askc(m,v) {\
   fprintf(stderr,m); \
   scanf("%1s%*c",&v); \
   v=toupper((int)v);}

/* Modified 03-Feb-2010 by y.hiwasaki (check buffer overruns) */
#define asks(m,v) { \
   fprintf(stderr,m);	\
   fgets(v,sizeof(v),stdin);*(v+strlen(v)-1)='\0';}

/* Modified 03-Feb-2010 by y.hiwasaki (clear stream, including newline) */
#define askl(m,v) {\
   fprintf(stderr,m);\
   scanf("%ld%*[^%\n]",&v);getchar();}

#define asklu(m,v) {\
   fprintf(stderr,m);\
   scanf("%lu%*[^%\n]",&v);getchar();}

#define askf(m,v) {\
   fprintf(stderr,m);\
   scanf("%f%*[^%\n]",&v);getchar();}
 
#define asklf(m,v) {\
   fprintf(stderr,m);\
   scanf("%lf%*[^%\n]",&v);getchar();}
 
#define aski(m,v) {\
   fprintf(stderr,m);\
   scanf(" %d%*[^\n]",&v);getchar();}

#define GET_PAR_L(p,msg,i ) \
   { if(argc>p) { \
       i=atol(argv[p]); \
       fprintf(stderr,"%s%ld\n",msg,i);}\
     else askl(msg,i);}

#define GET_PAR_LU(p,msg,i ) \
   { if(argc>p) { \
       sscanf(argv[p],"%ul",&(i)); \
       fprintf(stderr,"%s%lu\n",msg,i);}\
     else asklu(msg,i);}

#define GET_PAR_I(p,msg,i ) \
   { if(argc>p) { \
       i=atoi(argv[p]); \
       fprintf(stderr,"%s%d\n",msg,i);}\
     else aski(msg,i);}

#define GET_PAR_D(p,msg,r ) \
   { if(argc>p) {\
       r=(double)atof(argv[p]);\
       fprintf(stderr,"%s%f\n",msg,r);}\
     else asklf(msg,r);}

#define GET_PAR_F(p,msg,r ) \
   { if(argc>p) {\
       r=atof(argv[p]);\
       fprintf(stderr,"%s%f\n",msg,r);}\
     else askf(msg,r);}

/* Modified 02-Feb-2010 by y.hiwasaki (check buffer overrun) */
/* Note: do not use this macro for dynamically allocated arrays */
#define GET_PAR_S(p,msg,sp) \
	{ if((argc>p)&&(strlen(argv[p])<sizeof(sp))) {	\
      strcpy(sp,argv[p]); \
      fprintf(stderr,"%s%s\n",msg,sp);}\
    else asks(msg,sp);}

/* Modified 02-Feb-2010 by y.hiwasaki (explicit casting) */
#define GET_PAR_C(p,msg,C) \
   { if(argc>p) {\
       C=(char)toupper((int)(argv[p][0]));			\
       fprintf(stderr,"%s%c\n",msg,C);}		\
     else askc(msg,C); }
 
/* 
   -------------------------------------------------------------------------- 
   If the desired parameter has not been entered, a default value is
   taken; otherwise the entered value is used
   -------------------------------------------------------------------------- 
*/
#define FIND_PAR_C(p,msg,C,dft) \
   { C=toupper((argc>p)?argv[p][0]:dft);\
     fprintf(stderr,"%s%c\n",msg,C);}
 
#define FIND_PAR_S(p,msg,i,dft) \
   { strcpy(i,(argc>p)?argv[p]:dft);\
     fprintf(stderr,"%s%s\n",msg,i); }
 
#define FIND_PAR_L(p,msg,i,j) \
   if(argc>p) {\
     i=atol(argv[p]);\
     fprintf(stderr,"%s%ld\n",msg,i); }\
   else {\
     i=j; \
     fprintf(stderr,"%s%ld\n",msg,i); }
 
#define FIND_PAR_I(p,msg,i,j) \
   if(argc>p) {\
     i=atoi(argv[p]);\
     fprintf(stderr,"%s%d\n",msg,i); }\
   else {\
     i=(int)j; \
     fprintf(stderr,"%s%d\n",msg,i); }
 
#define FIND_PAR_F(p,msg,i,j) \
   if(argc>p) {\
     i=atof(argv[p]);\
     fprintf(stderr,"%s%f\n",msg,i); }\
   else {\
     i=j; \
     fprintf(stderr,"%s%f\n",msg,i); }
 
#define FIND_PAR_D(p,msg,i,j) \
   if(argc>p) {\
     i=(double)atof(argv[p]);\
     fprintf(stderr,"%s%f\n",msg,i); }\
   else {\
     i=j; \
     fprintf(stderr,"%s%f\n",msg,i); }
 
/* GENERAL DEFINITIONS */

/* -------------------------------------------- */
/* ... Print error message and exit program ... */
/* -------------------------------------------- */
#define HARAKIRI(m,code) {fprintf(stderr,m); exit((int)code);}
/* #define KILL(f,code) {perror(f); exit((int)code);} */
#define KILL(f,code) perror(f), exit((int)code)
 

/* DEFINITIONS FOR OPEN/CLOSE ISSUES */

#if defined(VMS) && !defined(__GNUC__)
#   define WB  "wb",mrs,"rfm=fix","ctx=stm"
#   define RB  "rb",mrs,"rfm=fix","ctx=stm"
#   define WT  "w","mrs=256","rat=cr","rfm=var"
#   define RT  "r","mrs=256","rat=cr","rfm=var"
#   define RWT "r+","mrs=256","rat=cr","rfm=var"
#elif defined(MSDOS)|defined(__CYGWIN__)
#   define WB  "wb"
#   define RB  "rb"
#   define WT  "wt"
#   define RT  "rt"
#   define RWT "rt+"
#else /* Unix */
#   define WB  "w"
#   define RB  "r"
#   define WT  "w"
#   define RT  "r"
#   define RWT "r+"
#endif

#endif

/* ................... End of include file UGSTDEMO.H ................... */
