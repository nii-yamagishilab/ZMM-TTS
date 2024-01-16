/*                                  v1.1 (part of basop V.2.3) - 14/Sep/2009 */
/*=============================================================================

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

  DESCRIPTION : 
  Basicop counter utility. 

  This file contains a program ROM estimation tool for softwares
  written using STL basic operators.

  Usage : - Check blacklist array in this file and edit it if
            adaptation is needed
          - Compile basop_cnt.c
          - Call basop_cnt for each C source file of the studied
            software: basop_cnt source.c [res.xls]

          - The result is printed on the screen. If the optional
            argument res.xls is also given, a summary of all calls is
            available in res.xls, it can be opened in Excel or in any
            text editor.  Res.xls contains 4 columns:
            C file name, # of basic operators,
            # of user functions, and # of blacklist functions

  HISTORY :
  13.Oct.06	v1.0	First version used in G.722 PLC standardisation

  14.Sep.09 v1.1  Cleaned blacklist, more comments, possibility to save
                  results in a file (STL2009)

  AUTHORS :
	v1.0  Balazs Kovesi France Telecom R&D/TECH <balazs.kovesi@orange-ftgroup.com>
	v1.1  Balazs Kovesi France Telecom R&D/TECH <balazs.kovesi@orange-ftgroup.com>
*/


#include <stdio.h>
#include <stdlib.h>

/** / #define VERBOSE_BASOP          /*print on stdout each basicOp found*/
/** / #define VERBOSE_FUNC           /*print on stdout each user defined function call found*/
/** / #define VERBOSE_BLACKLIST_FUNC /*print on stdout each blacklist function call found (not counted in complexity)*/

/* #define OLD_STL  */
/* switch to old STL (before STL2005): "if", "do", are not counted directly as they should be 
   coupled by test() that is counted as basicOp.
   See ITU-T Software Tool Library 2000 User's Manual: 
   "There is no complexity counted for any loops, subroutine calls, etc., 
   except for the complexity for arithmetic test on data in program control statement 
   (e.g. do while)." 
*/

const char basops[][20] = { /* all basicop commands */
"abs_s",
"add",
#ifndef OLD_STL
"break",
#endif
"BREAK",
#ifndef OLD_STL
"continue",
#endif
"CONTINUE",
"div_l",
"div_s",
#ifndef OLD_STL
"do",
#endif
"DO",
#ifndef OLD_STL
"else",
#endif
"ELSE",
"extract_h",
"extract_l",
"Extract40_H",
"Extract40_L",
#ifndef OLD_STL
"for",
#endif
"FOR",
#ifndef OLD_STL
"goto",
#endif
"GOTO",
"i_mult",
#ifndef OLD_STL
"if",
#endif
"IF",
"L_abs",
"L_add",
"L_add_c",
"L_and",
"L_deposit_h",
"L_deposit_l",
"L_Extract40",
"L_lshl",
"L_lshr",
"L_mac",
"L_mac0",
"L_macNs",
"L_max",
"L_min",
"L_mls",
"L_msu",
"L_msu0",
"L_msuNs",
"L_mult",
"L_mult0",
"L_negate",
"L_or",
"L_rotl",
"L_rotr",
"L_sat",
"L_saturate40",
"L_shl",
"L_shl_r",
"L_shr",
"L_shr_r",
"L_sub",
"L_sub_c",
"L_xor",
"L40_abs",
"L40_add",
"L40_deposit_h",
"L40_deposit_l",
"L40_deposit32",
"L40_lshl",
"L40_lshr",
"L40_mac",
"L40_max",
"L40_min",
"L40_msu",
"L40_mult",
"L40_negate",
"L40_round",
"L40_set",
"L40_shl",
"L40_shl_r",
"L40_shr",
"L40_shr_r",
"L40_sub",
"logic16",
"logic32",
"lshl",
"lshr",
"mac_r",
"mac_r40",
"move16",
"move32",
"Mpy_32_16_ss",
"Mpy_32_32_ss",
"msu_r",
"msu_r40",
"mult",
"mult_r",
"negate",
"norm_l",
"norm_L40",
"norm_s",
"rotl",
"rotr",
"round",
"round40",
"s_and",
"s_max",
"s_min",
"s_or",
"s_xor",
"shl",
"shl_r",
"shr",
"shr_r",
"sub",
#ifndef OLD_STL
"switch",
#endif
"SWITCH",
"test",
#ifndef OLD_STL
"while",
#endif
"WHILE",
"*"
};

const char blacklist[][20] = { /* to recognize standard C functions (memory allocation, 
  printings...) and other auxilary user defined functions that should not be counted in the program ROM*/
  /*this part can be edited to add new functions if needed*/
"malloc",
"calloc",
"free",
"return",
"sizeof",
"exit",
"fopen",
"fclose",
#ifdef OLD_STL
"for",
"if",
"switch",
"while",
#endif
"fwc",
"WMOPS_output",
"Reset_WMOPS_counter",
"Init_WMOPS_counter",
"setCounter",
"getCounterId",
"printf",
"fwrite",
"fread",
"*"
};

/*reads a character from a file with end-of-file control*/
char read_char(fp)
FILE * fp;
{
  char ch;
  if(!feof(fp))
  {
    fscanf(fp, "%c", &ch);
  }
  else
  {
    ch = 0;
  }
  return(ch);
}

/*search a token in a list[][20], returns one if found or 0 otherwise*/
int search_token(list, token, length)
char list[][20];
char token[];
int length; /*length of the searched token*/
{
  int ind2, eqcnt, i, found_flag;
  ind2 = 0;
  found_flag = 0;
  while(list[ind2][0] != '*') /*search for all basicops*/
  {
    /* verify if token is in list of basic ops */
    eqcnt = 0;
    for(i = 0; i < length; i++)
    {
      if(token[i] == list[ind2][i])
      {
        eqcnt++;
      }
      else
      {
        break;
      }
    }
    if((eqcnt == length) && (list[ind2][i] == 0))
    {
      found_flag = 1; /*basicop found --> cancel init: not a user function*/
      break;
    }
    ind2++;
  }
  return(found_flag);
}

int             main (argc, argv)
int argc; 
char *argv[];
{
  FILE *fp, *fpo, *fpr;
  char inchar;                         /* for the read input char */
  char words[200];                     /* contains an alphanumerical word */
  char last_word[200];                 /* contains the last alphanumerical word */
  char prepoc_file[200];
  int ind, countops, i, ind_lastword;
  int func_cnt, func_flag, blacklist_cnt;
  char twochar[3] = {"  \0"};          /* contains a string of two chars to search for comment separators */
  int incomment = 0;                   /* flag to signal that the processing is in a commented part */
  int infunction = 0;                  /* flag to signal that the processing is in a function, counts the opened {-s */
  int line = 1;

  if ((argc != 2) && (argc != 3))
  {
     printf("usage: %s <c_filename> [<result filename in append mode>]\n\n", argv[0]);
     exit(-1);
  }
  
  /* set preprocessed filename to argv[1] + "_pre" */
  ind = 0;
  while(argv[1][ind] != 0)
  {
    prepoc_file[ind] = argv[1][ind];
    ind++;
  }
  prepoc_file[ind++] = '_';
  prepoc_file[ind++] = 'p';
  prepoc_file[ind++] = 'r';
  prepoc_file[ind++] = 'e';
  prepoc_file[ind++] = 0;

  /**********************************************************
   * preprocess (remove comments, both C and C++ style)     *
   **********************************************************/
  /* Attention: preprocessor directives like #if 0 or #ifdef are not checked */

  /* open input and output files */
  if((fp = fopen(argv[1], "r")) == NULL)
  {
    printf("Error opening %s\n\n", argv[1]);
    exit(-1);
  }
  if((fpo = fopen(prepoc_file, "w")) == NULL) /*preprocessed file */
  {
    printf("Error opening %s\n\n", prepoc_file);
    exit(-1);
  }
  if(argc ==3 )
  {
    if((fpr = fopen(argv[2], "a")) == NULL) /*result summary file, one line for each processed C file*/
    {
      printf("Error opening %s\n\n", argv[2]);
      exit(-1);
    }
  }

  /* feed char buffer */
  twochar[1] = read_char(fp);

  /* loop over file */
  while(!feof(fp)) 
  {
    /* shift and read new char */
    twochar[0] = twochar[1];
    twochar[1] = read_char(fp);

    /* check for C comment start */
    if((twochar[0]=='/') && (twochar[1]=='*')) /*beginning of comment*/
    {
      incomment = 1;
    }

    /* check for C comment end */
    if((twochar[0]=='*') && (twochar[1]=='/')) /*end of comment*/
    {
      incomment = 0;
      /*feed buffer*/
      twochar[1] = read_char(fp);
      continue; /*not to write out '*' in twochar[0]*/
    }

    /* check for C++ comment start */
    if((twochar[0]=='/') && (twochar[1]=='/') && (incomment == 0)) /* not ansi C comment*/
    {
      /*ignore the rest of the line*/
      while(!feof(fp) && (getc(fp) != '\n') );

      /*feed buffer*/
      twochar[1] = read_char(fp);
      continue; /*not to write out '/' in twochar[0] */
    }

    /* write char to output file */
    if(incomment == 0)
    {
      fprintf(fpo, "%c", twochar[0]);
    }
  }
    /* flush buffer */
  if((incomment == 0) && (twochar[1] != 0))
  {
    fprintf(fpo, "%c", twochar[1]);
  }
  fclose(fp);
  fclose(fpo);

  /* print summary */
  printf("Input file:                %s\n", argv[1]);
  printf("Output pre-processed file: %s\n\n", prepoc_file);

  /**************************************
   * count basic ops and function calls  *
   ***************************************/

  /* open processed file */
  fp = fopen(prepoc_file, "r");

  ind = 0;
  countops = 0;
  func_cnt = 0;
  blacklist_cnt = 0;
  words[ind] = 0;
  ind_lastword = 0;
  last_word[ind_lastword] = 0;

  /* loop over file */
  while(!feof(fp))
  {
    /* read char */
    inchar = read_char(fp);

    /* update line counter */
    if(inchar=='\n')
    {
      line++;
    }

    /* check for opening bracket */
    if(inchar=='{')
    {
      /* close string */
      words[ind] = 0;

      /* save last token */
      for(i = 0; i <= ind; i++)
      {
        last_word[i] = words[i];
      }
      ind_lastword = ind;

      /* increment bracket counter (opening -> +1) */
      infunction++;
    }

    /* check for closing bracket */
    if(inchar=='}')
    {
      /* close string */
      words[ind] = 0;

      /* save last token */
      for(i = 0; i <= ind; i++)
      {
        last_word[i] = words[i];
      }
      ind_lastword = ind;

      /* decrement bracket counter (closing -> -1) */
      infunction--;

      /* check for bracket problem */
      if(infunction < 0)
      {
        printf("{ } mismatch, EXIT processing \n");
        exit(-1);
      }
    }

    /* parse token only after an opening bracket i.e. process only inside functions */
    if(infunction > 0)
    {
      /* check for alphanumerical char */      
      if( ((inchar>='0') && (inchar<='9')) || ((inchar>='A') && (inchar<='Z')) || ((inchar>='a') && (inchar<='z')) || (inchar=='_') || (inchar=='#'))
      {
        words[ind++] = inchar;
      }
      else
      {
        /* close string and save token */
        words[ind] = 0;
        if(ind > 0)
        {
          for(i = 0; i <= ind; i++)
          {
            last_word[i] = words[i];
          }
          ind_lastword = ind;
        }

        /* do not delete current token if current char is a white space or opening parenthesis */
        /* otherwise last word is surely not a function name, can be deleted */
        /* do not count neither preprocessor directives like #ifdef(X) as a function*/
        if(((inchar!=' ') && (inchar!='\t') && (inchar!='(')) || (last_word[0]=='#'))
        {
          ind_lastword = 0;
          last_word[0] = 0;
        }

        /* check for opening parenthesis */
        if(inchar=='(')
        {
          /* init: assume a user-defined function */
          func_flag = 1;

          if(ind_lastword == 0) /*not a valid token (alphanumerical string) before ( */
          {
            func_flag = 0; /* cancel init (token is not a user-defined function) */
          }

          /* check for basic op function (if found, cancel assumption of user-defined function) */
          /* but basicops are counted later as they not always have function form*/
          if(search_token(basops, last_word, ind_lastword) == 1)
          {
            func_flag = 0; /*basicop found --> cancel init: not a user function*/
          }

          /* check for standard C functions */
          if(search_token(blacklist, last_word, ind_lastword) == 1)
          {
            func_flag = 0; /*C function found --> not a user function*/
            blacklist_cnt++;
#ifdef VERBOSE_BLACKLIST_FUNC
            printf("line %4d: blacklist #%4d:            %30s \n", line, blacklist_cnt, last_word);
#endif
          }

          /* if init was not cancelled at this stage, we found a user-defined function */
          if(func_flag == 1)
          {
            func_cnt++;
#ifdef VERBOSE_FUNC
              printf("line %4d: User defined function #%4d: %30s \n", line, func_cnt, last_word);
#endif
          }
        } /* end of if(inchar=='(') */

        /* check for basic ops (functions or simply keyword like CONTINUE) */
        if(search_token(basops, words, ind) == 1)
        {
          countops++;
#ifdef VERBOSE_BASOP
          printf("line %4d: Basicop #%4d:               %30s \n", line, countops, words);
#endif
        }
        ind = 0;
      }
    }
  }
  
  printf("%4d STL basicops\n", countops);
  printf("%4d calls to user-defined functions\n", func_cnt);
  printf("%4d blacklist tokens\n\n", blacklist_cnt);

  fclose(fp);
  if(argc ==3 )
  {
    fprintf(fpr, "%s\t%d\t%d\t%d\n", argv[1], countops, func_cnt, blacklist_cnt);
    fclose(fpr);
  }

  return(0);
}
