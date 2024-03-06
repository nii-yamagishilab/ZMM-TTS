/*                                                         31.Jan.2001  V.2.0
  ===========================================================================
  CONCAT.C
  ~~~~~~~~
  
  Concatenates a series of files in binary mode, irrespective of
  actual content (ASCII/binary, char, short, long, float,
  whatever). Optionally, generates a undo script that can be used 
  to separate the concatenated files, assuming that the data is
  organized in 16-bit words. 

  NOTE 1: If the concatenated files are byte-oriented, however, and
          the size of at least one of the files is odd, then the
          concatenated file cannot be separated using the default STL
          command, astrip, and the program will print a warning in the
          undo script file.

  NOTE 2: Since the output file is created before any data is read, using
          something like

            concat a b c a

          will actually create a file a = b+c since the output file is
          reset upon creation. The original contents of file a are
          permanently lost! However, you will be prompted for
          overwriting in this case when the option -f is *not* used.

  Usage:
  ~~~~~~
  concat [options] file1 [file2 file3 ...] fileout
  where 
  file1,2 ... filename of input file(s) to be concatenated
  fileout ... output filename

  Options:
  -f ........ force overwriting output file, if it already exists
  -i ........ asks before overwriting output file
  -undo f ... saves undo commands into file f

  Original author:
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto  
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao.campos@labs.comsat.com

  History:
  ~~~~~~~~
  12.Jul.2000  V.1.0    Created
  31.Jan.2001  V.2.0    Added undo script creation <simao>
  =========================================================================== */
#include "ugstdemo.h"

/* Include generic headers */
#include <stdio.h>
#include <string.h> /* For strstr() */
#include <stdlib.h>

/* ... Includes for O.S. specific headers ... */
#if defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#elif defined(VMS)
#include <perror.h>
#include <file.h>
#include <stat.h>
#else		       /* Unix */
#include <sys/stat.h>
#include <unistd.h>    /* Specific for read/write/lseek */
#endif

/* Function prototypes */
void display_usage ARGS((void));

/* Definitions, pseudo-functions */
#define KILL(f,code) perror(f), exit((int)code)
#define BUFF_SIZE 1024

/* Define other symbols */
#define SEP "astrip -sample" /* Separation command */

/*
  --------------------------------------------------------------------------
  display_usage()

  Shows program usage.

  History:
  ~~~~~~~~
  12/Jul/2000  v1.0 Created <simao>
  --------------------------------------------------------------------------
*/
#define P(x) printf x
void display_usage()
{
  P(("CONCAT.C - Version 2.0 of 30/Jan/2001 \n\n"));
 
  P(("  Concatenates a series of files in binary mode, irrespective of\n"));
  P(("  actual content (ASCII/binary, char, short, long, float,\n"));
  P(("  whatever). Optionally, generates a undo script that can be used \n"));
  P(("  to separate the concatenated files, assuming that the data is\n"));
  P(("  organized in 16-bit words. \n"));
  P(("  NOTE1: The script will NOT work properly if data is byte-oriented\n"));
  P(("         and at least one of the files to concatenate has an odd size.\n"));
  P(("\n"));
  P(("  NOTE2: Since the output file is created before any data is read, \n"));
  P(("         using something like\n"));
  P(("          $ concat a b c a\n"));
  P(("         will actually create a file a = b+c since the output file\n"));
  P(("         is reset upon creation. The original contents of file 'a' \n"));
  P(("         are permanently lost! However, you will be prompted for\n"));
  P(("         overwriting in this case provided that the option -f is *not* used.\n"));
  P(("Hit <enter> to continue ..."));
  fgetc(stdin);
  P(("\n"));
  P(("  Usage:\n"));
  P(("   $ concat [options] file1 [file2 file3 ...] fileout\n"));
  P(("  where \n"));
  P(("   file1,2 ... filename of input file(s) to be concatenated\n"));
  P(("   fileout ... output filename\n"));
  P(("\n"));
  P(("  Options:\n"));
  P(("   -f ........ force overwriting output file, if it already exists\n"));
  P(("   -i ........ asks before overwriting output file\n"));
  P(("   -undo f ... saves undo commands into file f\n"));

  /* Quit program */
  exit(-128);
}
#undef P
/* ....................... End of display_usage() ....................... */


int main(argc,argv)
int argc;
char *argv[];
{
  long nsam;  /* Number of bytes saved */
  long cur=1; /* Current offset in shorts in concatenated file (for undo) */
  char buffer[BUFF_SIZE];
  FILE *Fi=0, *Fo=0, *Fundo=0;
  char *cur_file, *file_out=argv[argc-1], interactive=1, *undo=0;
  struct stat     st;
  int i,  bad_undo=0;

  /* Options */
  while (argc > 1 && argv[1][0] == '-')
    if (strcmp(argv[1],"-")==0)
    {
      /* Input is stdin -  just go to filename parsing section*/
      break;
    }
    else if (strcmp(argv[1], "-f") == 0)
    {
      /* Force overwriting without asking */
      interactive=0;

      /* Move argv over the option to the next argument */
      argv++;
      argc--;
    }
    else if (strcmp(argv[1], "-i") == 0)
    {
      /* Ask before overwriting */
      interactive=1;

      /* Move argv over the option to the next argument */
      argv++;
      argc--;
    }
    else if (strcmp(argv[1], "-undo") == 0)
    {
      /* Ask before overwriting */
      undo=argv[2];

      /* Move argv over the option to the next argument */
      argv+=2;
      argc-=2;
    }
    else if (strcmp(argv[1], "-?") == 0 || strstr(argv[1], "-help"))
    {
      /* Print help */
      display_usage();
    }
    else
    {
      fprintf(stderr, "ERROR! Invalid option \"%s\" in command line\n\n",
	      argv[1]);
      display_usage();
    }

  
  /* Check usage */
  if (argc<3)
  {
    fprintf(stderr, "Bad number of parameters\n");
    display_usage();
  }

  /* Check whether any of the input files are stdin; abort in that case */
  for (i=1; i<argc-1; i++)
    if (strcmp(argv[i],"-")==0)
    {
      fprintf(stderr, "This program will not read from stdin. Aborted.\n");
      exit(2);      
    }

  /* Check whether output file exists; skip if -f or abort if stdout */
  memset(&st, 0, sizeof(st)); 
  stat(file_out, &st);
  if (strcmp(file_out,"-")==0)
  {
    fprintf(stderr, "This program will not write data to stdout. Aborted.\n");
    exit(2);
  }
  else if (interactive && st.st_size > 0)
  {
    fprintf(stderr, "Output file %s exists. Overwrite? (Y,n) [Y]\n", file_out);
    interactive = fgetc(stdin);
    if (interactive=='N' || interactive=='n')
    {
      fprintf(stderr, "Will not overwrite file. Please try again.\n");
      exit(1);
    }
  }

  /* Create/overwrite output file */  
  if ((Fo = fopen(file_out,WB))==NULL)
    KILL(file_out, 4);

  /* Create/overwrite undo script file */
  if (undo && (Fundo = fopen(undo,"w"))==NULL)
    KILL(undo, 5);

  /* Print info */
  if (undo)
    fprintf(stderr, "Undo script saved in %s \n", undo);
  fprintf(stderr, "Creating %s from: ", file_out);

  while (argc>2)
  {
    /* Inits */
    cur_file=argv[1]; argv++; argc--;

    /* Save undo command for this file in undo script file */
    /* Assumes that the file is 16-bit (e.g. short) oriented */
    if (undo)
    {
      /* Get info for current (input) file */
      stat(cur_file, &st);

      /* If the filesize is odd, then the file is organized in
         byte-boundaries, not 16- or 32-bit words, and the
         concatenated file cannot be separated using the default STL
         command, astrip. Hence, prints a warning to the user. */
      if (st.st_size%2)
      {
	fprintf(Fundo, "echo WARNING! File %s has an odd no.of samples. \n",cur_file);
        fprintf(Fundo, "echo %s%s", "The commands below are offset (late) ",
		"by (at least) one byte.\n");
        fprintf(Fundo, "echo %s%s %s!\n", "From this point on, you cannot ",
		"use astrip to separate", file_out);
	bad_undo++;
      }
      fprintf(Fundo, "%s -start %ld -n %ld\t%s\t%s\n",
	      SEP, cur, (long)(st.st_size)/2, file_out, cur_file);
      cur += st.st_size/2;
    }

    /* Print some more info */
    fprintf(stderr, "%s ", cur_file);

    /* Open current input file */
    if ((Fi = fopen(cur_file, RB))==NULL)
      KILL(cur_file, 5);

    /* Read & write until finish in chuncks of 1kByte */
    while (!feof(Fi))
    {
      if ((nsam = fread(buffer, sizeof(char), sizeof(buffer), Fi))<0)
        KILL(cur_file, 5);
      if (nsam && (nsam = fwrite(buffer, sizeof(char), nsam, Fo))<0)
        KILL(file_out, 6);      
    }

    /* Close file and goes to next */
    fclose(Fi);
  }

  /* Close output file and exit */
  fprintf(stderr, "\n");
  if (undo)
  {
    fclose(Fundo);
    if (bad_undo)
      fprintf(stderr, "WARNING: found %d file(s) with odd size. %s\a\n",
	      bad_undo, "Undo script will not work properly as created");
  }
  fclose(Fo);
#ifndef VMS
  return(0);
#endif
}
