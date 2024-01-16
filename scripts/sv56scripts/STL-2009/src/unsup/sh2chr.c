/* 
  ===========================================================================
  SH2CHR.C
  ~~~~~~~~
  Convert input file specified in 1st command line argument from a 16-bit word 
  file (whose upper MSBs are supposed to be all 0's) saving with LSBs in a
  unsigned byte-oriented file. Uses unsigned conversion for the transfer.

  Input file: ...00XX00YY...
  Output file: ...XXYY...

  Usage:
  ~~~~~~
  sh2chr filein fileout
  where 
  filein ... is the input file name or "-" for the standard input (pipe)
  fileout .. is the output file name or "-" for the standard output (pipe)

  Original author:
  ~~~~~~~~~~~~~~~~
  Simao Ferraz de Campos Neto  
  Comsat Laboratories                  Tel:    +1-301-428-4516
  22300 Comsat Drive                   Fax:    +1-301-428-9287
  Clarksburg MD 20871 - USA            E-mail: simao@ctd.comsat.com

  ===========================================================================
 */
#include <stdio.h>
#include <string.h>
#include "ugstdemo.h"

int main(argc,argv)
int argc;
char *argv[];
{
  FILE *Fchr,*Fsh;
  unsigned short sh;
  register long MSB_set=0;

  /* Check arguments */
  if (argc!=3)
    HARAKIRI("Usage is sh2chr shfile chrfile\n",1);

  /* Open files */
  if (strcmp(argv[1],"-"))
  {
    if ((Fsh=fopen(argv[1],RB))==NULL)
      KILL(argv[1],2);
  }
  else
    Fsh = stdin;

  if (strcmp(argv[2],"-"))
  {
    if ((Fchr=fopen(argv[2],WB))==NULL)
      KILL(argv[2],2);
  }
  else
    Fchr = stdout;

  /* Copy char input to short output as unsigned */
  while (1)
  {
    fread(&sh, sizeof(sh), 1, Fsh);
    if (feof(Fsh))
      break;
    if (sh&0xFF00)
      MSB_set++;
    sh=fputc((int)(sh&0x00FF), Fchr);
  }

  /* Warn if non-zero MSB were found */
  if (MSB_set)
    fprintf(stderr, "WARNING! %ld non-zero MSB samples found in file %s\n", 
	    (long int)MSB_set, argv[1]);

  /* Close and quit */
  fclose(Fsh);
  fclose(Fchr);
#ifndef VMS
  return(0);
#endif
}
