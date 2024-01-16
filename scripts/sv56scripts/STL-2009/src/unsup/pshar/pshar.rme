This is the read.me file for the pshar distribution kit.

Pshar is the Portable SHell ARquiver. It gets the input text files and 
pack all them in one single ASCII file, that is easy to unpack. Unpacking
can be done using pshar with the option -u (see usage), or, in a Unix
system, using sh(1).

Usage is:

 pshar [-u shar] [[-a] [-p prefix] [-d delim] [-bcv] 
       [-f shar] files [> shar]]
 where:  
    -a     all the options (v,c,b,-pXX)
    -b     extract absolute paths into current directory
    -c     check filesizes on extraction
    -d     use this EOF delimiter instead of SHAR_EOF
    -f     shar <archive>
    -p     use this as prefix to each line in archived files
    -u     unshar <archive>
    -v     verbose on extraction, incl. echoing filesizes

    files: list of files to be packed. In Unix, wildcard expansion 
           is performed by the operating system. With Borland products,
           one may use wildargs.obj to allow wildcar expansion by the 
           program. Wildcard expansion is not implemented for VMS.

    NOTE: redirection ([> shar] above) does NOT work for VMS!

In this kit you find the files:

    make-vms.com:	DCL file for VMS build-up
    makefile.tcc:       makefile for Borland make & tcc
    makefile.djc:	makefile for MSDOS port of GNU make/gcc
    makefile.unx:       makefile for Unix make
    pshar.c:          	the portable shell archive generator
    file1-o.txt:       	Input test file #1
    file2-o.txt:       	Input test file #2
    file3-o.txt:       	Input test file #3
    file4-o.txt:       	Input test file #4
    file5-o.txt:       	Input test file #5
    test.ori:      	Output test file

This has been released to public in February 95 for UGST usage, based on a 
public-domain version of shar for VMS and CPM(!) systems. Please note
that this software is provided ``AS IS'' and WITHOUT any express or
implied warranties, including, without limitation, the implied
warranties of merchantibility and fitness for a particular purpose.

Simao F. Campos Neto
<simao@ctd.comsat.com>

