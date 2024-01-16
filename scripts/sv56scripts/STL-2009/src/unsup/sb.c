/*                                                           13.Mar.2000  V2.0
  ============================================================================

  SB.C
  ~~~~

  Program to swap low and high bytes of the words in a binary file, in
  order to allow proper usage on Unix workstations.

  Usage:
  ~~~~~~
  $ sb filein fileout

  where:
  filein 	is the original file.
  fileout	is the swaped byte file.

  Return
  ~~~~~~
  0	OK status (all systems but VMS)
  1	OK status (VMS)
  2	Bad usage
  3	Can't open input file
  4	Can't create output file
  5	can't allocate memory
  6	error reading data from filein
  7	error writing data to fileout

  Compilation under:
  ~~~~~~~~~~~~~~~~~~
  TurboC: $ tcc sb
  MS-C:	$ cl sb
  VAX-C:	$ cc sb
        	$ link sb
  SunOS:	$ cc -o sb sb.c

  Author: Simao Ferraz de Campos Neto
  ~~~~~~~ <tdsimao@venus.cpqd.ansp.br>

  History:
  ~~~~~~~~
  02.Oct.1991  V1.0 First release
  13.Mar.2000  V2.0 Corrected ideosynchrasy for MS Visual C compiler; cleaned 
                    up code; added option for conditional byteswapping when 
                    overwriting (depending on whether the system is little
                    or big-endian). <simao.campos@labs.comsat.com>
=============================================================================
*/
#if defined(__MSDOS__) && !defined(MSDOS) /* def. for autom. compil.,TurboC */
#define MSDOS			/* this is already defined for Microsoft C */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ugstdemo.h"

#ifdef VMS
#include <stat.h>
#include <perror.h>
#else
#include <sys/stat.h>
#endif				/* VMS */


/* defines properly the OK status value */
#ifdef VMS
#define OK 1
#else
#define OK 0
#endif

/* Global variables */
char isILS = 0;

/* Define cases for overwrite condition */
#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#endif

enum OVER {ANY=0, BIG_ENDIAN=1, LITTLE_ENDIAN=2};

/*
 -----------------------------------------------------------------------------
 Display usage
 -----------------------------------------------------------------------------
*/
void display_usage(prog)
char *prog;
{
  printf("SB.C Version 2.0 of 13.Mar.2000 \n");
  printf("Usage is: %s [-ILS] [-over [-if [big|little|anyway]] filein fileout\n",
          prog);
  exit(2);
}


/* 
  -----------------------------------------------------------------------------
  Detects whether the system is little or big endian
  -----------------------------------------------------------------------------
*/ 
int endian_type()
{
  /* Hex version of the string ABCD */
  unsigned long tmp = 0x41424344; 
  
  /* compare the hex version of the 4 characters with the ascii version */
  return(strncmp("ABCD",(char *)&tmp,4) ? LITTLE_ENDIAN : BIG_ENDIAN);
}
/* ......................... is_little_endian() ......................... */


/*
  -----------------------------------------------------------------------------
  Swap considering MSDOS limitations
  -----------------------------------------------------------------------------
*/
long msdos_approach(i,o)
FILE *i;
FILE *o;
{
  char            ch[512], ch2;
  long            j, k, total;
  short          *sp0, *sp2, s0, s2;
  struct stat     s;

  /* initialize */
  total = 0;

  /* get input file size, among other informations */
  fstat(fileno(i), &s);

  /* read chunk of file in array ch */
  while((k = fread(ch, sizeof(char), sizeof(ch), i))!=0)
  {
    /* swap the bytes */
    for (j = 0; j < k; j += 2)
    {
      ch2 = ch[j + 1];
      ch[j + 1] = ch[j];
      ch[j] = ch2;
    }

    /* treat ILS header, if informed by the user */
    if (isILS)
    {
      /* Pass here only the 1st time */
      isILS=0;

      /* Some more swapping to swap long words */
      for (k = 0; k < 512; k += 4)
      {
	sp0 = (short *) (&ch[k + 0]);
	sp2 = (short *) (&ch[k + 2]);
	s0 = *sp0;
	s2 = *sp2;
	*sp0 = s2;
	*sp2 = s0;
      }
    }

    /* write the swaped data from the array ch into output file */
    if (fwrite(ch, sizeof(char), k, o) != k)
    {
      fprintf(stderr, "Canna write file, cap!\n");
      exit(7);
    }
    else
      total += k;
  }
  return(total);
}
/* ................ End of msdos_approach() .......................... */


/*
  -----------------------------------------------------------------------------
  Swap in a more efficient way
  -----------------------------------------------------------------------------
*/
long decent_approach(i, o)
FILE *i;
FILE *o;
{
  char           *ch, *ch1, ch2;
  long            total, k = 1;
  short          *sp0, *sp2, s0, s2;
  struct stat     s;
#ifdef VMS
  char mrs[10] = "mrs=512"
#endif

  /* get input file size, among other informations */
  fstat(fileno(i), &s);

  /* allocate memory to hold all the file */
  if ((ch = (char *) malloc(s.st_size)) == NULL)
  {
    fprintf(stderr, "Canna allocate memory, cap!\n");
    exit(6);
  }

  /* read all file in array ch */
  for (ch1 = ch, k = 0; (long)ch1 - (long)ch < s.st_size; ch1 += k)
  {
    total = k = fread(ch, sizeof(char), s.st_size, i);
    if (k <= 0l)
    {
      fprintf(stderr, "Canna read file, cap!\n");
      exit(6);
    }
  }

  /* swap the bytes */
  for (k = 0; k < s.st_size; k += 2)
  {
    ch2 = ch[k + 1];
    ch[k + 1] = ch[k];
    ch[k] = ch2;
  }

  /* treat ILS header, if informed by the user */
  if (isILS)
  {
    for (k = 0; k < 512; k += 4)
    {
      sp0 = (short *) (&ch[k + 0]);
      sp2 = (short *) (&ch[k + 2]);
      s0 = *sp0;
      s2 = *sp2;
      *sp0 = s2;
      *sp2 = s0;
    }
  }

  /* write the swaped data from the array ch into output file */
  if (fwrite(ch, sizeof(char), s.st_size, o) != s.st_size)
  {
    fprintf(stderr, "Canna write file, cap!\n");
    exit(7);
  }

  /* exiting stuf */
  free(ch);
  return(total);
}
/* ..................... End of decent_approach() .......................... */


/*
  -----------------------------------------------------------------------------
  Get a temporary file name, handling ideosynchrasies from different compilers
  -----------------------------------------------------------------------------
*/
void get_tmp_name(fileout)
char *fileout;
{
#if defined (unix) || defined (__GNUC__) || defined(_MSC_VER)
  char *ch;
#endif

  tmpnam(fileout);

#if defined (unix) || defined (__GNUC__)
  ch = strrchr(fileout, '/'); ch++;
  strcpy(fileout, ch);
#endif
#ifdef _MSC_VER
  ch = strrchr(fileout, '\\'); ch++;
  strcpy(fileout, ch);
#endif

#ifdef DEBUG
 printf("Temporary file name: %s\n", fileout);
#endif
}
/* ..................... End of get_tmp_name() .......................... */


int main(argc, argv)
  int             argc;
  char           *argv[];
{
  char            overwrite = 0, conditional_swap=0, byte_order=ANY, fileout[150];
  char           *prog=argv[0];
  FILE           *i, *o;
  long            total, k = 1;
  struct stat     s;
#ifdef VMS
  char mrs[10] = "mrs=512"
#endif

  /* Check bad usage */
  if (argc == 1)
    display_usage(prog);

  /* Look for option(s) */
  while (argv[1][0] == '-')	/* yes, it's an option! */
  {
    if (!strcmp(argv[1], "-ILS") || !strcmp(argv[1], "-ils"))
    {				/* valid option: ILS header */
      isILS = 1;
      argv++; argc--;
      fprintf(stderr, "Aya, sir; it's and ILS file!\n");
    }
    else if (!strcmp(argv[1], "-over"))
    {
      get_tmp_name(fileout);
      argv++; argc--;
      overwrite = 1;
    }
    else if (!strcmp(argv[1], "-if"))
    {
      if (strncmp(argv[2],"big",3)==0)
        conditional_swap=BIG_ENDIAN;
      else if (strncmp(argv[2],"lit",3)==0)
        conditional_swap=LITTLE_ENDIAN;
      else if (strncmp(argv[2],"any",3)==0)
        conditional_swap=ANY;
      else
      {
        fprintf(stderr, "Invalid conditional swap option: %s\n", argv[2]);
        display_usage(prog);
      }

      /* Move pointer to next parameter */
      argv+=2; argc-=2;
    }
    else if (strcmp(argv[1], "-?") == 0 || strstr(argv[1], "-help"))
    {
      /* Print help */
      display_usage(prog);
    }
    else
    {
      fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
              argv[1]);
      display_usage(prog);
      }
  }

  /* Check consistency: conditional swap only if overwriting */
  if (conditional_swap && !overwrite)
    HARAKIRI("Conditional swapping only available when overwriting! Aborted.\n", 2);

  /* open input file */
  if ((i = fopen(argv[1], RB)) == NULL)
    KILL(argv[1],3);

  /* Print message if overwriting, perform checks before opening out file */
  if(overwrite)
  {
    if (conditional_swap == endian_type() || conditional_swap == ANY) 
      fprintf(stderr, "Aya, sir; I'll overwrite file %s!\n",argv[1]);
    else
    {
      fprintf(stderr, "%s: Skipping byte swapping, system is %s\n",
              argv[1],
	      endian_type()==BIG_ENDIAN? "big endian" : "little endian");
      exit(0);
    }
  }

  /* open output file */
  if ((o = fopen(overwrite ? fileout : argv[2], WB)) == NULL)
    KILL(argv[k + 1],4);

  /* get input file size, among other informations */
  fstat(fileno(i), &s);

  /* do the byte swapping */
  total =
#if defined(MSDOS) && !defined (__GNUC__)
  msdos_approach(i,o);
#else
  decent_approach(i,o);
#endif

  /* test file size and converted samples */
  if (s.st_size!=total)
    fprintf(stderr,
	    "Problem: input file size is %ld, converted bytes are %ld\n",
	    s.st_size, total);

  /* close files */
  fclose(i);
  fclose(o);
  
  /* check if overwrite */
  if (overwrite)
  {
#ifdef OLD_WAY
# ifdef MSDOS
    if(chmod(argv[1], (int)0700) ||
       unlink(argv[1]) ||
       rename(fileout, argv[1]))
      KILL(argv[1],10);
# else
    char            cmd[200];
    sprintf(cmd, "\\mv %s %s", fileout, argv[1]);
    system(cmd);
# endif
#else
    if (chmod(argv[1], (int)0700) ||
	remove(argv[1]) ||
	rename(fileout, argv[1]))
      KILL(argv[1],10);
#endif
  }


#ifndef VMS
  return(0);
#endif
}
