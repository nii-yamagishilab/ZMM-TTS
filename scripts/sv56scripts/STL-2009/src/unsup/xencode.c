/*                                                            V2.11-26.Feb.97
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	XENCODE.C
        ~~~~~~~~~

	Description:
	~~~~~~~~~~~~
        Program to convert binary files into ASCII file(s), in order to
        allow transmission via electronic mail.

        It follows the de-facto standard of the public-domain program
        uuencode.c, but adding the capability of automatically break a
        file pottencially too long for transmission in some networks
        (e.g., BITNET only allows 64k-byte long files).

        The encoded file will be decoded by xdecode.c or any other
        compatible with uudecode.c; using the former, the reconstruction
        of the file encoded by xencode.c will be automatic (in the case
        that file truncation happened); using the latter, reconstruction
        will have to be manual.

        The uuencode.c's basic algorithm is to access the file
        char-wisely (char-by-char), to transform the chars (0..255) read
        into printable ASCII chars but adding some error detection
        capabilities: reads 45 chars from the file (possibly except in
        the last "block" read from file, if the file size is not
        multiple of 45, i.e.,  filesize % 45 != 0.), codes the number of
        chars read and put into the file;then codes the samples, making
        3 chars into 4, and then converting them to readable ASCII
        chars. The encoding function is

                          ENC(c) = (((c) & 077) + ' ')

        The standard implies also that a header and a tail are added to
        the file. The header, for uuencode.c, is the word "begin",
        followed by the file's access mode (in octal) and the original
        file name. The tail is just the word "end", followed by a
        carriage return (necessarily).

        The extension here implemented just add some more information to
        the header: the continuation file's name. This way, if no
        continuation  file name is found in header, the program assumes
        that the presently open file is the last one.

        In this version the CRC is calculated and saved incrementally to
        the end of the xencoded file (right after the "end" line).
        Incrementally means that, when a file is broken in parts, at the end of
        each part, it will be printed the CRC from the beginning of the file
        upto that part. Three CRCs are printed: CCITT, XMODEM and ARC.
        The CRC module is based on the implementation posted on the
        comp.sources.unix by Mark G. Mendel.

        NOTE1: because some operating systems have short maximum file names
        (for example, DOS only allows 8-char-long file names and
        3-byte-long file extensions), it is advidsable to use destination
        names as short as possible, in order to prevent file overwriting.

        NOTE2: when encoding ASCII files (e.g. postscript figures or docs)
        in a DOS file system, it is advisable to use the option "-t",
        rather than the default binary option. This may cause a problem
        when decoding the file in a Unix machine, because DOS line
        termination is the pair CR+LF, while in Unix it is only LF. Option
        "-t" provides a proper encoding of the original file.

        Usage:
	~~~~~~
	$ xencode [-options] [InpFile [OutFile [BlockSize]]]

	where:
	InpFile		is the name of the file to be encoded; if not
                        specified, stdin is assumed;
	OutFile		is the name with the encoded data; if not specified,
                        stdout is assumed;
	BlockSize	is the block size, almost in kbytes; if not specified,
                        MAX_LONG is assumed, what implies no fragmentation of
			encoded file; the "almost" above means that, due to 1k
                        (ie, 1024) not being a multiple of 45, the final file's
		        size will be adjusted to the greatest multiple of 45
        		smaller than the file's size (in bytes).
        Options:
        ~~~~~~~~
        -t ............ interpret the input file/pipe as ASCII, rather 
                        than binary (the default).
        -b ............ interpret the input file/pipe as binary (default).
	-q ............ do not produce information text header


        Example:	$ xencode speech.dat speech.uue 10
        ~~~~~~~~	If speech.dat is 103424 bytes long, this will convert
                        it  into 11 (ASCII) files, each being 14336 bytes long,
                        named speech.uue,speech01.uue, ... , speech10.uue.
                        The size of the last one will in general be smaller
                        than the other files (because in general the file
                        length is not an integer multiple of 45, the "magic
                        number" used by the uuencode algorithm).

	Variables:
	~~~~~~~~~~
	in		input file;
	out		output file;
	blk		block size;

	Exit values:
	~~~~~~~~~~~~
	-1	Wrong parameters
	0 	Success (all but VAX/VMS)
	1	Success (only VAX/VMS)
	2	Error opening input file
	3	Error opening output file
	4	Error in block size specs

	Compilation under:
        ~~~~~~~~~~~~~~~~~~
        TurboC: tcc xencode
        MS-C:	cl xencode
        VAX-C:	cc xencode
        	link xencode
        SunOS:	cc -o xencode xencode.c
        Gnu-C:  gcc -o xencode xencode.c

	Author:	Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~  <tdsimao@venus.cpqd.ansp.br>

        History:
        ~~~~~~~~
        01.Aug.91 1.00 1st version to UGST <tdsimao@cpqd.ansp.br>
        27.Aug.91 1.10 1024 changed by 1024l in line that calculates
                       the number of bytes per .uue file, in the case
                       of partition of encoded file into several
                       pieces. <tdsimao@cpqd.ansp.br>
        05.Jun.92 1.20 Added function to put text information on uuencoded file
                       and parts on the total number of parts and how they
                       shall be named. <tdsimao@cpqd.ansp.br>
        21.Jul.93 1.30 Strips path from file name for the output file.
                       <tdsimao@cpqd.ansp.br>
        25.Sep.93 2.00 Calculate CRC for the input file and save it,
                       incrementally for each part. Included "-t" option
                       for safe encoding of text files. <tdsimao@cpqd.ansp.br>
        23.May.95 2.01 Added \n\n before %XENCODE-INFO, to avoid problems
                       with some e-mailers. <simao@ctd.comsat.com>
        25.Sep.95 2.10 Encoded file does not have ' ' but '`' instead. 
                       Cleaned for better compilation. Added extra header 
                       instruction message when original file is .ZIP. Added 
                       option for having user's email in message instead of 
                       my email (coordinated with the makefile). Max filesizes
                       specified in command line now reflect the size of the 
                       output file parts, not the chuncks of the input file 
                       (an old bug corrected!) <simao@ctd.comsat.com>
        26.Feb.97 2.11 Added command line option to supress printing
	               text header on how to uudecode, etc. <simao>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* Version number for XENCODE */
#define XENCODE 211

/* General includes */
#include "ugstdemo.h"		/* UGST demo macros - the 1st include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* System-dependent includes */
#if defined(VMS)
#include types
#include stat
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

/* DEFINITION FOR SMART PROTOTYPES */
#ifndef ARGS
#if defined(MSDOS) || defined(VMS)
#define ARGS(x) x
#else /* Unix: no parameters in prototype! */
#define ARGS(x) ()
#endif
#endif /* ARGS */


/* General defines */
#define MAX_LONG 2147483647
#define NO	0
#define	YES	1


/* Defines for CRC routine */
#define W       16		/* CRC width */
#define WTYPE	unsigned short	/* Basic data type */
#define B	8		/* the number of bits per char */

#ifndef USER_NAME
#define USER_NAME (char *)NULL /* "<simao@ctd.comsat.com>" OR your user name */
#endif

/* Local function prototypes */
void display_usage ARGS((void));
int encode      ARGS((FILE *in, FILE *out, long charno));
int fr          ARGS((FILE *fd, char *buf, int cnt));
int get_mode    ARGS((FILE *fp));
char           *get_extension ARGS((char *ext, char *full_name, int max_length));
char           *get_root ARGS((char *root, char *full_name));
char           *xcode_index ARGS((register char *sp, register char c));
int outenc      ARGS((char *p, FILE * f));
void put_info   ARGS((FILE * out, char *ori_file, char *root, char *ext,
                  char mode, int filno));
WTYPE updcrc    ARGS((WTYPE icrc, unsigned char *icp, int icnt, WTYPE *crctab, 
                  char swapped));

/* ENC is the basic 1 character encoding function to make a char printing */
char c_global;
#define ENC(c) ((c_global=(((c) & 077) + ' '))==' '?'`':c_global)


/* Macros related to crc calculations */
#define get_ccitt_crc(crc,buf,n)  updcrc(crc, buf, n, crctab_ccitt, 0)
#define get_arc_crc(crc,buf,n)    updcrc(crc, buf, n, crctab_arc, 1)
#define get_xmodem_crc(crc,buf,n) updcrc(crc, buf, n, crctab_xmodem, 1)

/* Global variables related to crc calculations */
WTYPE           crc_a = 0, crc_c = 0, crc_x = 0;
int             init_crc_a = 0L, init_crc_c = -1L, init_crc_x = 0L;


/* ================================================== */
int main(argc, argv)
  int             argc;
  char           *argv[];
{
  FILE           *in, *out;
  long            blk = MAX_LONG;
  int             filno, i;
  int             input_is_file = YES;
  int             output_is_file = YES;
  char            root[120], ext[5], src[120], this_file[120], next_file[120];
  char           *get_root(), *get_extension(), mode_is_binary = YES, quiet=0;
  struct stat     inpfil;
#ifdef VMS
  static char     mrs[15] = "mrs=512";	/* for correct mrs in VMS environment */
#endif

  /* Check if 1st par. is an option */
  while (argc > 1 && argv[1][0] == '-' && strlen(argv[1]) > 1)
    if (strcmp(argv[1], "-t") == 0)
    {
      /* set mode as text */
      mode_is_binary = 0;

	/* Move argv over the option to the next argument */
      argc--;
      argv++;
    }
    else if (strcmp(argv[1], "-b") == 0)
    {
      /* set mode as binary */
      mode_is_binary = 1;

      /* Move argv over the option to the next argument */
      argv++;
      argc--;
    }
    else if (strcmp(argv[1], "-q") == 0)
    {
      /* Don't print progress indicator */
      quiet = 1;

      /* Move argv over the option to the next argument */
      argv++;
      argc--;
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


  /* Check arguments ... */
  if (argc > 3)			/* , then block size specified */
  {
    blk = atol(argv[3]) * 1024l;
    blk -= blk % 45;		/* blk is in bytes */
    argc--;
    if (blk == 0)
      HARAKIRI("Bad max.size for encoded file was specified! \n", 4);
  }

  /* If 3 arguments, then output file specified */
  if (argc > 2)
  {
    argc--;
  }
  else
  {
    out = stdout;		/* output is not a file, but the std. output */
    output_is_file = NO;
  }

  /* If 2 arguments, then input file specified */
  if (argc > 1)
  {
    strcpy(src, argv[1]);
    if (mode_is_binary)
      in = fopen(src, RB);
    else 
      in = fopen(src, RT);
    if (in == NULL)
      KILL(src, 2);
    argc--;
  }
  else
  {
    in = stdin;			/* input is not a file, but the std. input */
    input_is_file = NO;
  }


  /* Warns wrong usage */
  if (argc != 1)
    display_usage();

  /* Finds out the number of files to generate, filno */
  if (input_is_file)
  {
    long tmp = blk * 3/4;
    fstat(fileno(in), &inpfil);      /* Stat structure */
    filno = inpfil.st_size / tmp;    /* Number of files; */
    if (inpfil.st_size % tmp != 0)   /* Add one to avoid truncation */
      filno++;
  }
  else
    filno = 1;			/* only one file will be generated */


  /* Remove path from input file name */
  get_root(root, src);
  get_extension(ext, src, 4);
  strcpy(src, root);
  strcat(src, ext);


  /* Print info on screen when not a pipe/redirection */
  if (output_is_file)
  {
    /* Displays blocking information */
    if (filno > 1)
    {
      fprintf(stderr, " Input file will be broken");
      fprintf(stderr, " in %3d files ", filno);
      fprintf(stderr, " of %ld bytes each.\n", blk);
    }

    /* Get the root of the file name (ie, name w/o extension) */
    get_root(root, argv[2]);

    /* Get file name's extension */
    get_extension(ext, argv[2], 4);

    /* Initializes this_file with the given output file name */
    /* PS: this is the FIRST file; srips off the path! */
    strcpy(this_file, root);
    strcat(this_file, ext);

    /* tell something ... */
    if (input_is_file)
      fprintf(stderr, " Encoding %s into ", src);
  }


  /* Initialize crc calculations */
  crc_a = init_crc_a;
  crc_c = init_crc_c;
  crc_x = init_crc_x;

  /* Process input file saving in blocks, as specified */
  for (i = 0; i < filno; i++)
  {
    /* Mounts the next output file name, if any */
    if (i < filno - 1 && output_is_file)
    {
      sprintf(next_file, "%s%02d%s\0", root, i + 1, ext);
    }
    else
      strcpy(next_file, "\0");	/* no next file! */

    /* Opens output file */
    if (output_is_file)
    {
      /* Print the name the file *shall* have after being extracted */
      if ((out = fopen(this_file, WT)) == NULL)
	KILL(this_file, 3);
      if (filno>1)
      {
	  fprintf(out, "\n\n%%XENCODE-INFO: This is ");
	  fprintf(out, "Part %02d/%02d of %s. ",
		  i + 1, filno, src);
	  fprintf(out, "Name it as \"%s\".\n\n", this_file);
      }
    }

    /* Write out information on how to xdecode files */
    if (i == 0 && !quiet)
      put_info(out, src, root, ext, mode_is_binary, filno);

    /* Complement information */
    if (input_is_file && output_is_file)
      fprintf(stderr, "%s%c", this_file, (i < filno - 1) ? ',' : '.');

    /* Mounts the encoded file's header */
    fprintf(out, "begin %o %s %s\n",
	    inpfil.st_mode & 0777,	/* access mode */
	    src,		/* original file's name */
	    next_file);		/* cont.file's name */

    /* Proceeds to encoding process */
    encode(in, out, blk);

    /* Put file's end mark */
    fprintf(out, "end\n");

    /* Print incremental CRCs */
    fprintf(out, "%%XENCODE-CRC: %d CCITT %4X ARC %4X XMODEM %4X\n",
	    (int) XENCODE, crc_c, crc_a, crc_x);

    /* Closes the present output file */
    fclose(out);

    /* updates file names */
    strcpy(this_file, next_file);
  }
  if (input_is_file && output_is_file)
    fprintf(stderr, "\n -> Done! \n");

  /* Closes input file */
  fclose(in);

  /* And exits! */
#ifndef VMS
  return 0;
#endif
}
/* ........................... end of main() ........................... */


/*
  ----------------------------------------------------------------------------

      void display_usage (void);
      ~~~~~~~~~~~~~~~~~~

      Description:
      ~~~~~~~~~~~~
      Display usage and quit.

      Parameters: none.
      ~~~~~~~~~~~

      Return:
      ~~~~~~~
      Nothing

      Log of Changes:
      ~~~~~~~~~~~~~~~
      23.Sep.93  v1.0	1st version <tdsimao@cpqd.ansp.br>

  ----------------------------------------------------------------------------
*/
#define FP(x) fprintf(stderr, x);
void display_usage()
{
  fprintf(stderr, "\nxencode Ver.%.2f - ", (float)XENCODE/100.0);
  FP("UGST file email transmission tool compatible with uuencode\n");
  FP("Usage: xencode [-options] [infile [outfile [blocksize] ] ]\n");
  FP("Options:\n");
  FP("-t ... treat input data as ASCII [default: binary]\n");
  FP("-b ... treat input data as binary [default]\n");
  FP("-q ... do not produce information text header\n");
  exit(-128);
}
/* ...................... end of display_usage() ........................ */

/*
  ----------------------------------------------------------------------------

      void put_info (FILE *out, char *ori_file, char *root, char *ext,
      ~~~~~~~~~~~~~  char mode, int filno);

      Description:
      ~~~~~~~~~~~~

      Put header on output file, telling what is in it and how to decode
      the file. Useful for new users, who does not understand what to do
      with the bunch of .UUE files, and how to correctly name the files
      out of the mail utility.

      Parameters:
      ~~~~~~~~~~~
        out ........ pointer to output FILE;
        ori_file ... name of original file;
        root ....... root of output file name;
        ext ........ extension of output file name;
        mode ....... input file is ASCII (0) or binary (1)
        filno ...... number of parts the output file will be broken.

      Return:
      ~~~~~~~
      Nothing

      Log of Changes:
      ~~~~~~~~~~~~~~~
      17.Jun.92  v1.0	1st version <tdsimao@cpqd.ansp.br>

  ----------------------------------------------------------------------------
*/
void            put_info(out, ori_file, root, ext, mode, filno)
  FILE           *out;
  char           *ori_file, *root, *ext, mode;
  int             filno;
{
  int             i;
  char           *ori_ext;

  ori_ext = strrchr(ori_file, '.');

  fprintf(out, "ATTENTION!\n\n");

  fprintf(out, "The original file %s has been xencode'd (software\n",
	  ori_file);
  fprintf(out, "tool xencode.c) and broken into %d file(s).\n", filno);
  fprintf(out, "To decode the files back to the original, please:\n\n");

  fprintf(out, "1. Extract each of the e-mails received with the names:\n");

  fprintf(out, "\tpart 01/%02d\t -> %s%s\n", filno, root, ext);

  /* Mounts the next file name of parts 2..filno */
  for (i = 1; i < filno; i++)
    fprintf(out, "\tpart %02d/%02d\t -> %s%02d%s\n",
	    i + 1, filno, root, i, ext);

  fprintf(out, "\n2. Run the utility xdecode, as follows:\n");
  if (mode)
    fprintf(out, "   $ xdecode %s%s\n", root, ext);
  else
    fprintf(out, "   $ xdecode -t %s%s\n", root, ext);
  fprintf(out, "   and it will generate the file %s.\n\n", ori_file);

  if (strcmp(ori_ext, ".zip")==0 || strcmp(ext, ".ZIP")==0)
  {
    fprintf(out, "3. Run a pkunzip-compatible unarchiver on a MSDOS, Unix,\n");
    fprintf(out, "   or VMS environment. Pkunzip is a shareware on MSDOS,\n");
    fprintf(out, "   unzip is a free software available for most operating\n");
    fprintf(out, "   systems and platforms. To get a copy of pkunzip, get\n");
    fprintf(out, "   the package from wuarchive.wustl.edu, directory\n");
    fprintf(out, "   /systems/ibmpc/simtel/zip/, file pkz204g.exe. To get\n");
    fprintf(out, "   a MSDOS version of unzip, get in the same site,\n");
    fprintf(out, "   directory /systems/ibmpc/simtel/starter, file\n");
    fprintf(out, "   unz512x.exe. For other operating systems, see your\n");
    fprintf(out, "   system guru.\n\n");
  }

  fprintf(out, "NOTE1: VMS and DOS users should explicitly use xdecode\n");
  fprintf(out, "       option -b if the original file was a binary file\n");
  fprintf(out, "       (eg, speech),and -t if it was a text file (eg a \n");
  fprintf(out, "       postscript file or a C code program). XDECODE\n");
  fprintf(out, "       default is *binary* mode.\n\n");

  fprintf(out, "NOTE2: no edition of the uuencoded files is needed.\n");
  fprintf(out, "       XDECODE will get rid of all the garbage.\n\n");

  fprintf(out, "Good luck! %s\n\n", USER_NAME);

  /* Print the next only if more than 1 part */
  if (filno > 1)
  {
    fprintf(out, "%s%s", "*************************************",
	    "************************************\n");
    fprintf(out, "%s%s",
	    "\t  ORIG.FILE  NEXT FILE XDECODE WILL LOOK FOR! ",
	    "BE SURE IT EXISTS,\n");
    fprintf(out, "%s%s",
	    "\t    NAME     OR A \"SHORT FILE\" MESSAGE ",
	    "WILL BE DISPLAYED!!!\n");
    fprintf(out, "             |           |\n");
    fprintf(out, "             V           V\n");
    fprintf(out, "%s%s", "*************************************",
	    "************************************\n\n");
  }
}
/* ........................... end of put_info() ........................... */


/*
  ---------------------------------------------------------------------------
  WTYPE updcrc (WTYPE icrc, unsigned char *icp, int icnt, WTYPE *crctab, 
  ~~~~~~~~~~~~  char swapped);
  
  Description:
  ~~~~~~~~~~~~
  Calculate, intelligently, the CRC-16 of a dataset incrementally given a
  buffer full at a time. The parameters on which the function relyes are:

  Parameter             CCITT   XMODEM  ARC
  ---------             ------  ------  ----
  the polynomial P	0x1021	0x1021	A001
  initial CRC value:	-1	0	0
  bit-order swap        No (0)  Yes (1) Yes (1)
  bits in CRC:          16	16	16

  Usage:
  ~~~~~~
  	newcrc = updcrc( oldcrc, bufadr, buflen, crctab, swapped)
  		   WTYPE oldcrc;
                   int buflen;
  		   char *bufadr;
                   WTYPE *crctab;
                   char swapped;

  Notes:
   Regards the data stream as an integer whose MSB is the MSB of the first
   byte recieved.  This number is 'divided' (using xor instead of subtraction)
   by the crc-polynomial P.
   XMODEM does things a little differently, essentially treating the LSB of
   the first data byte as the MSB of the integer.

  Original Author:
  ~~~~~~~~~~~~~~~~
  Mark G. Mendel, 7/86
  UUCP: ihnp4!umn-cs!hyper!mark, GEnie: mgm
  [ Original code available in comp.sources.unix volume 6 (1989) ]

  History:
  ~~~~~~~~
  xx.Jul.86 v1.0  Release by the author [posted to c.s.misc in 1989]
  01.Sep.93 v1.0a Adapted by Simao <simao@cpqd.ansp.br> to be used in the
 		  x{en,de}code.c.

  ---------------------------------------------------------------------------
*/

/* Table for CCITT CRCs */
static WTYPE    crctab_ccitt[1 << B] = {
  0x0, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0xa50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0xc60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0xe70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0xa1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x2b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x8e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0xaf1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0xcc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0xed1, 0x1ef0,
};

/* Table for xmodem CRCs */
static WTYPE    crctab_xmodem[1 << B] = {
  0x0, 0x17ce, 0xfdf, 0x1811, 0x1fbe, 0x870, 0x1061, 0x7af,
  0x1f3f, 0x8f1, 0x10e0, 0x72e, 0x81, 0x174f, 0xf5e, 0x1890,
  0x1e3d, 0x9f3, 0x11e2, 0x62c, 0x183, 0x164d, 0xe5c, 0x1992,
  0x102, 0x16cc, 0xedd, 0x1913, 0x1ebc, 0x972, 0x1163, 0x6ad,
  0x1c39, 0xbf7, 0x13e6, 0x428, 0x387, 0x1449, 0xc58, 0x1b96,
  0x306, 0x14c8, 0xcd9, 0x1b17, 0x1cb8, 0xb76, 0x1367, 0x4a9,
  0x204, 0x15ca, 0xddb, 0x1a15, 0x1dba, 0xa74, 0x1265, 0x5ab,
  0x1d3b, 0xaf5, 0x12e4, 0x52a, 0x285, 0x154b, 0xd5a, 0x1a94,
  0x1831, 0xfff, 0x17ee, 0x20, 0x78f, 0x1041, 0x850, 0x1f9e,
  0x70e, 0x10c0, 0x8d1, 0x1f1f, 0x18b0, 0xf7e, 0x176f, 0xa1,
  0x60c, 0x11c2, 0x9d3, 0x1e1d, 0x19b2, 0xe7c, 0x166d, 0x1a3,
  0x1933, 0xefd, 0x16ec, 0x122, 0x68d, 0x1143, 0x952, 0x1e9c,
  0x408, 0x13c6, 0xbd7, 0x1c19, 0x1bb6, 0xc78, 0x1469, 0x3a7,
  0x1b37, 0xcf9, 0x14e8, 0x326, 0x489, 0x1347, 0xb56, 0x1c98,
  0x1a35, 0xdfb, 0x15ea, 0x224, 0x58b, 0x1245, 0xa54, 0x1d9a,
  0x50a, 0x12c4, 0xad5, 0x1d1b, 0x1ab4, 0xd7a, 0x156b, 0x2a5,
  0x1021, 0x7ef, 0x1ffe, 0x830, 0xf9f, 0x1851, 0x40, 0x178e,
  0xf1e, 0x18d0, 0xc1, 0x170f, 0x10a0, 0x76e, 0x1f7f, 0x8b1,
  0xe1c, 0x19d2, 0x1c3, 0x160d, 0x11a2, 0x66c, 0x1e7d, 0x9b3,
  0x1123, 0x6ed, 0x1efc, 0x932, 0xe9d, 0x1953, 0x142, 0x168c,
  0xc18, 0x1bd6, 0x3c7, 0x1409, 0x13a6, 0x468, 0x1c79, 0xbb7,
  0x1327, 0x4e9, 0x1cf8, 0xb36, 0xc99, 0x1b57, 0x346, 0x1488,
  0x1225, 0x5eb, 0x1dfa, 0xa34, 0xd9b, 0x1a55, 0x244, 0x158a,
  0xd1a, 0x1ad4, 0x2c5, 0x150b, 0x12a4, 0x56a, 0x1d7b, 0xab5,
  0x810, 0x1fde, 0x7cf, 0x1001, 0x17ae, 0x60, 0x1871, 0xfbf,
  0x172f, 0xe1, 0x18f0, 0xf3e, 0x891, 0x1f5f, 0x74e, 0x1080,
  0x162d, 0x1e3, 0x19f2, 0xe3c, 0x993, 0x1e5d, 0x64c, 0x1182,
  0x912, 0x1edc, 0x6cd, 0x1103, 0x16ac, 0x162, 0x1973, 0xebd,
  0x1429, 0x3e7, 0x1bf6, 0xc38, 0xb97, 0x1c59, 0x448, 0x1386,
  0xb16, 0x1cd8, 0x4c9, 0x1307, 0x14a8, 0x366, 0x1b77, 0xcb9,
  0xa14, 0x1dda, 0x5cb, 0x1205, 0x15aa, 0x264, 0x1a75, 0xdbb,
  0x152b, 0x2e5, 0x1af4, 0xd3a, 0xa95, 0x1d5b, 0x54a, 0x1284,
};

/* Table for ARC CRCs */
static WTYPE    crctab_arc[1 << B] = {
  0x0, 0xc0c1, 0xc181, 0x140, 0xc301, 0x3c0, 0x280, 0xc241,
  0xc601, 0x6c0, 0x780, 0xc741, 0x500, 0xc5c1, 0xc481, 0x440,
  0xcc01, 0xcc0, 0xd80, 0xcd41, 0xf00, 0xcfc1, 0xce81, 0xe40,
  0xa00, 0xcac1, 0xcb81, 0xb40, 0xc901, 0x9c0, 0x880, 0xc841,
  0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
  0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
  0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
  0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
  0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
  0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
  0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
  0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
  0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
  0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
  0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
  0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
  0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
  0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
  0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
  0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
  0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
  0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
  0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
  0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
  0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
  0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
  0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
  0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
  0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
  0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
  0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
  0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040,
};

/* -------------- Begin of function updcrc() ---------------------- */
WTYPE
updcrc(icrc, icp, icnt, crctab, swapped)
  WTYPE           icrc, *crctab;
  unsigned char  *icp;
  int             icnt;
  char            swapped;
{
  register WTYPE  crc = icrc;
  register unsigned char *cp = icp;
  register int    cnt = icnt;

  if (swapped)
    while (cnt--)
    {
      crc = (crc >> B) ^ crctab[(crc & ((1 << B) - 1)) ^ *cp++];
    }
  else
    while (cnt--)
    {
      crc = (crc << B) ^ crctab[(crc >> (W - B)) ^ *cp++];
    }

  return (crc);
}
/* .......................... End of updcrc() ............................ */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int get_mode(FILE *fp);
        ~~~~~~~~~~~~

	Description:	get file's mode.
	~~~~~~~~~~~~

	Variables:
	~~~~~~~~~~
	fp 	pointer to file of which the mode is wanted

	Exit values:
	~~~~~~~~~~~~
	S_????	file's access mode

	Author:	Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~  <tdsimao@venus.cpqd.ansp.br>

	Date:	19/Mar/91-16:30
	~~~~~

	Version: 1.00
	~~~~~~~~

	Revision/Date/Author: none (yet)
	~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int             get_mode(fp)
  FILE           *fp;
{
  struct stat     sbuf;

  /* figure out file mode */
  fstat(fileno(fp), &sbuf);
  return (sbuf.st_mode & 0777);
}
/* ........................... end of get_mode() ........................... */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int encode (FILE *in, FILE *out, long charno);
        ~~~~~~~~~~

	Description:
	~~~~~~~~~~~~
	Copy from in to out, encoding as it goes along, using the basic
	encoding scheme:
                          ENC(c) = (((c) & 077) + ' ')

	Variables:
	~~~~~~~~~~
	in 	pointer to file from which samples are taken
	out	pointer to file to which encoded samples are put
	charno	maximum number of chars to be written

	Routines:	- outenc
	~~~~~~~~~	- ENC

	Author:	unknown
	~~~~~~~

	Revision/Date/Author: 
	~~~~~~~~~~~~~~~~~~~~~ 
        19.Mar.91 v1.0 Adapted from public domain implementation
                       <tdsimao@venus.cpqd.ansp.br>
        25.Sep.95 v1.1 Modified charno to reflect the number of bytes of the 
                       output file, instead of the number of bytes of the 
                       input file. <simao@ctd.comsat.com>

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int             encode(in, out, charno)
  FILE           *in;
  FILE           *out;
  long            charno;
{
  char            buf[80];
  int             i, n;
  long            count = 0;

  /* Adjust input bytes to output bytes */
  charno = (long)(charno * 3 / 4);

  do
  {
    /* Find out, encode and write the no.of chars/line (upto 45) */
    n = fr(in, buf, 45);
    putc(ENC(n), out);

    /* Update CRCs for the block of data */
    crc_a = get_arc_crc(crc_a,(unsigned char *) buf, n);
    crc_c = get_ccitt_crc(crc_c, (unsigned char *) buf, n);
    crc_x = get_xmodem_crc(crc_x, (unsigned char *) buf, n);

    /* Encode the data from input and save to output, 3 at a time */
    for (i = 0; i < n; i += 3)
      outenc(&buf[i], out);

    /* New line after "n" chars are out */
    putc('\n', out);

    /* Updates counter of output samples */
    count += n;

  } while (n > 0 && count < charno);	/* i.e., not EOF, ... */
  if (count >= charno)
    fprintf(out, "`\n");

  return 0;
}
/* .......................... end of encode() .......................... */

/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int outenc (char *p, FILE *f);
        ~~~~~~~~~~

	Description:
	~~~~~~~~~~~~
	Output one group of 3 bytes, pointed at by p, on file f, using
	the basic encoding scheme:

                          ENC(c) = (((c) & 077) + ' ')

	Variables:
	~~~~~~~~~~
	p	pointer to array of 3 bytes to be coded and saved as 4
		[printable] bytes;
	f	pointer to file to which encoded samples are put

	Author:	unknown
	~~~~~~~

	Revision/Date/Author: Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~~~~~~~~~~~~~~~ <tdsimao@venus.cpqd.ansp.br> -- 19/Mar/91-16:30

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int outenc(p, f)
  char           *p;
  FILE           *f;
{
  int             c1, c2, c3, c4;

  c1 = *p >> 2;
  c2 = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
  c3 = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
  c4 = p[2] & 077;
  putc(ENC(c1), f);
  putc(ENC(c2), f);
  putc(ENC(c3), f);
  putc(ENC(c4), f);

  return 0;
}
/* ........................... end of outenc() ........................... */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int fr (FILE *fd, char *buf, int cnt);
	~~~~~~

	Description:
	~~~~~~~~~~~~
	Read upto cnt bytes from stream (file/device) pointed by fd and
	save in buffer buf. Similar to fread.

	Variables:
	~~~~~~~~~~
	fd 	pointer to file from which chars are read
	buf	pointer to char
	cnt	maximum number of chars to be read

	Author:	unknown
	~~~~~~~

	Revision/Date/Author: Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~~~~~~~~~~~~~~~ <tdsimao@venus.cpqd.ansp.br> -- 19/Mar/91-16:30

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
int             fr(fd, buf, cnt)
  FILE           *fd;
  char           *buf;
  int             cnt;
{
  int             c, i;

  for (i = 0; i < cnt; i++)
  {
    c = getc(fd);
    if (c == EOF)
      return (i);
    buf[i] = c;
  }
  return (cnt);
}
/* ........................... end of fr() ........................... */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	char *xcode_index (register char *sp, register char c);
	~~~~~~~~~~~

	Description:
	~~~~~~~~~~~~
	Locates the first occurence of c in stream sp and returns it;
	if c is not found, returns NULL.

	Variables:
	~~~~~~~~~~
	sp 	pointer to first ocurrence of c in ptr
	c	character to be located;

	Author:	unknown
	~~~~~~~

	Revision/Date/Author: Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~~~~~~~~~~~~~~~ <tdsimao@venus.cpqd.ansp.br> -- 19/Mar/91-16:30

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifdef NULL
# undef NULL
#endif
#define	NULL	0
char           *xcode_index(sp, c)
  register char  *sp;
  register char   c;
{
  do
  {
    if (*sp == c)
      return (sp);
  } while (*sp++);
  return (NULL);
}
/* ........................... end of xcode_index() ........................... */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	char *get_root (char *root, char *full_name);
	~~~~~~~~~~~~~~
	Description:
	~~~~~~~~~~~~
        Deletes file extension from a file name by  locating the last
        occurence of the extension mark (".") in the file  name pointed
        by "full_name" and copying upto that position, "full_name" to
        "root".

	Variables:
	~~~~~~~~~~
	root		file name without extension;
	full_name	file's full name;

	Exit values:
	~~~~~~~~~~~~
	Filename without extension; if given file name has no extension,
        the original name is returned.

	Original Author:
	~~~~~~~~~~~~~~~~
	Simao Ferraz de Campos Neto -- CPqD/Telebras

	Revision/Date/Author:
	~~~~~~~~~~~~~~~~~~~~~
        19.Mar.91 v1.0 1st release
        21.Jul.93 v2.0 Added removal of the path from the file name.<simao>
        25.Sep.95 v2.1 Added smarter handling of filenames with multiple '.' 
                       (allowed in Unix) <simao@ctd.comsat.com>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#if defined(VMS)
#define PATH_END ']'
#elif defined(MSDOS)
#define PATH_END '\\'
#else				/* Unix */
#define PATH_END '/'
#endif

char           *get_root(root, full_name)
  char           *root;
  char           *full_name;
{
  register char  *tmp = full_name;
#if (XENCODE==200)
  int             n;
#endif

  /* copy full-name to root buffer */
  strcpy(root, full_name);

  /* finds last occurence of a pointend of the path */
  tmp = (char *) strrchr(root, PATH_END);
  if (tmp == NULL)
    tmp = root; 		/* name starts with root, i.e., no path
				 * present */
  else
    tmp++;			/* move onto start of root name */

#if (XENCODE==200)
  /* finds last occurence of a point */
  n = (long) strcspn(tmp, ".");

  /* copy root from tmp to `root' */
  strncpy(root, tmp, n);
  root[n] = '\0';
#else
  /* finds last occurence of a dot */
  tmp = strrchr(tmp, '.');
  if (tmp) 
    *tmp = 0;
#endif

  /* returns the file name without extension */
  return (root);
}
#undef PATH_END
/* ........................... end of get_root() ........................... */


/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	char *get_extension (char *ext, char *full_name, int max_length);
	~~~~~~~~~~~~~~~~~~~

	Description:
	~~~~~~~~~~~~
        Locates the extension of a file name by  locating the last
        occurence of the extension mark (".") in the file name pointed
        by "full_name" and copying from that position (thus including
        the '.')  upto the name's end (limmited to max_length chars) to
        "ext".

	Variables:
	~~~~~~~~~~
	ext		file name's extension (with point);
	full_name	file's full name;
	max_length	maximum length allowed for the extension

	Exit values:
	~~~~~~~~~~~~
	Filename's extension; if given file name has no extension,
        a null-string is returned.

	Author:	Simao Ferraz de Campos Neto -- CPqD/Telebras
	~~~~~~~  <tdsimao@venus.cpqd.ansp.br>

	Date:	19/Mar/91-16:30
	~~~~~

	Version: 1.00
	~~~~~~~~

	Revision/Date/Author: none (yet)
	~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
char           *get_extension(ext, full_name, max_length)
  char           *ext;
  char           *full_name;
  int             max_length;
{
  register char  *tmp, *end;

  /* finds last occurence of a point */
  if ((tmp = (char *) strrchr(full_name, '.')) == NULL)
  {
    /* if original file had no extension, returns a '.' */
    strcpy(ext, ".");
    return (NULL);
  }

  /* and string's end */
  end = (char *) strrchr(full_name, 0);

  /* copies the extension from the point, ... */
  strncpy(ext, tmp, max_length);

  /* ... and appends a '\0', if needed */
  if (max_length <= end - tmp)
    ext[max_length] = '\0';

  /* returns the file name's extension ('.' included!) */
  return (ext);
}
/* ......................... end of get_extension() ......................... */
