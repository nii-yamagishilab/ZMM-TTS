/* 
  ===========================================================================
  CHR2SH.C
  ~~~~~~~~
  Convert input file specified in 1st command line argument to a 16-bit word 
  file with the original bytes of the input file in the LSB of the output 
  file. Uses unsigned conversion for the transfer.

  Input file:  ...XXYY...
  Output file: ...00XX00YY...

  Usage:
  ~~~~~~
  chr2sh filein fileout 
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

  /* Check arguments */
  if (argc!=3)
    HARAKIRI("Usage is chr2sh chrfile shfile\n",1);

  /* Open files */
  if (strcmp(argv[1],"-"))
  {
    if ((Fchr=fopen(argv[1],RB))==NULL)
      KILL(argv[1],2);
  }
  else
    Fchr = stdin;

  if (strcmp(argv[2],"-"))
  {
    if ((Fsh=fopen(argv[2],WB))==NULL)
      KILL(argv[2],2);
  }
  else
    Fsh = stdout;

  /* Copy char input to short output as unsigned */
  while (1)
  {
    sh=fgetc(Fchr);
    if (feof(Fchr))
      break;
    fwrite(&sh, sizeof(sh), 1, Fsh);
  }

  /* Close and quit */
  fclose(Fchr);
  fclose(Fsh);
#ifndef VMS
  return(0);
#endif
}
