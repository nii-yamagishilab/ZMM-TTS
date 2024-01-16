/*
 *  MODIFIED TO READ ALL THE PARAMETERS FROM FILE   PK 09/06/91
 */

/**************************************************************************

                (C) Copyright 1990, Motorola Inc., U.S.A.

Note:  Reproduction and use for the development of North American digital
       cellular standards or development of digital speech coding
       standards within the International Telecommunications Union -
       Telecommunications Standardization Sector is authorized by Motorola 
       Inc.  No other use is intended or authorized.

       The availability of this material does not provide any license
       by implication, estoppel, or otherwise under any patent rights
       of Motorola Inc. or others covering any use of the contents
       herein.

       Any copies or derivative works must incude this and all other
       proprietary notices.


Systems Research Laboratories
Chicago Corporate Research and Development Center
Motorola Inc.

************************************************************************* */
/*---------------------------------------------------------------------------*/
/*	getParams.c -- Reads parameters from parameter file (.prm extension) */
/*			or prompts the user.                                 */
/*---------------------------------------------------------------------------*/
/*	Written by: Matt Hartman    */
/*	Date: Sept 11, 1990.        */
/*----------------------------------*/
/* Includes */
#include <stdio.h>
#include "vparams.h"
#include "ugstdemo.h"

void            getParams(fpget)
  FILE           *fpget;
{
  int             choice;
  char            filename[128];
#ifdef VMS
  static char     mrs[8] = "mrs=320";
#endif

  fprintf(stderr, "\nSystem options? [Encoder/decoder = 0, Decoder only = 1]: ");
  fscanf(fpget, "%d", &choice);
  decode_only = choice; 

  if (!decode_only)
  {
    fprintf(stderr, "\nPut out packed ASCII hex coded stream? [0=no 1=yes]: .. ");
    fscanf(fpget, "%d", &choice);
    packedStream = choice;

    fprintf(stderr, "\nCalculate performance measures? [0=no 1=yes]: ......... ");
    fscanf(fpget, "%d", &choice);
    performMeas = choice;
  }

  fprintf(stderr, "\nCreate log file? [0=no 1=yes]: ........................ ");
  fscanf(fpget, "%d", &choice);
  makeLog = choice;

  fprintf(stderr, "\nApply post-filter to synthesis? [0=no 1=yes]: ......... ");
  fscanf(fpget, "%d", &choice);
  apply_postfilter = choice;

  if (!decode_only)
  {
    fprintf(stderr, "\nEnter input speech file: ");
    fscanf(fpget, "%s", filename);
    if ((fpin = fopen(filename, RB)) == NULL)
      HARAKIRI("***ERROR Input File Specification\n", 2);
  }

  fprintf(stderr, "\nEnter output speech file: ");
  fscanf(fpget, "%s", filename);
  if ((fpout = fopen(filename, WB)) == NULL)
    HARAKIRI("***ERROR Output File Specification\n", 1);

  if (decode_only && fpcode == NULL)
  {
    fprintf(stderr, "\nEnter input bit-stream file: ");
    fscanf(fpget, "%s", filename);
    if ((fpcode = fopen(filename, "r")) == NULL)
      HARAKIRI("***ERROR Code File Specification\n", 1);
  }

  if (makeLog || performMeas && fplog == NULL)
  {
    fprintf(stderr, "\nEnter output filename for diagnostics: ");
    fscanf(fpget, "%s", filename);
    fplog = fopen(filename, "w");
  }

  if (packedStream && fpstream == NULL)
  {
    fprintf(stderr, "\nEnter packed, coded output filename: ");
    fscanf(fpget, "%s", filename);
    fpstream = fopen(filename, "w");
  }
  fprintf(stderr, "\n OK: all options read; Processing now ... \n");
}
/* ........................... end of getParams() ........................... */
