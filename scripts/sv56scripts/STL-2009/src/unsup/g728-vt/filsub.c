/* filsub.f -- translated by f2c (version of 15 October 1990  19:58:17).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/


/* Table of constant values */

static long c__1 = 1;

/* File:           FILSUB.FOR */

/* Last modified:  January 10, 1992 */

/* By:             Finn Tore Johansen */
/*                 Norwegian Telecom Research */
/*                 <ftj@bilde.nta.no> */

/* Content:        This file contains subroutines for binary file handlin */
/* g: */

/*                 FOPEN   opens binary file for read or write */
/*                 FREAD   read sample from binary file */
/*                 FWRITE  write sample to binary file */

/* NOTE: These subroutines are not strictly FORTRAN77 and */
/*       should be modified according to the Fortran dialect used. */
/*       IMPLICIT NONE */
/* Subroutine */ int fopen_(lun, name, mode, name_len)
long *lun;
char *name;
long *mode;
ftnlen name_len;
{
    /* System generated locals */
    olist o_1;

    /* Builtin functions */
    /* Subroutine */ int s_copy();
    long f_open();

    /* Local variables */
    static char stat[8];

    s_copy(stat, "OLD", 8L, 3L);
/* VAX fortran (VMS): */
    if (*mode == 1) {
	s_copy(stat, "UNKNOWN", 8L, 7L);
    }
/* Microsoft fortran (DOS): */
/* 	OPEN(UNIT=LUN,FILE=NAME,STATUS=STAT,FORM='BINARY') */
/* Lahey fortran (DOS): */
/*       OPEN(UNIT=LUN,FILE=NAME,STATUS=STAT, */
/*    -       ACCESS='TRANSPARENT',FORM='UNFORMATTED') */
    o_1.oerr = 0;
    o_1.ounit = *lun;
    o_1.ofnmlen = name_len;
    o_1.ofnm = name;
    o_1.orl = 512;
    o_1.osta = stat;
    o_1.oacc = 0;
    o_1.ofm = "UNFORMATTED";
    o_1.oblnk = 0;
    f_open(&o_1);
} /* fopen_ */

/*       IMPLICIT NONE */
/* Subroutine */ int fread_(lun, buf, pos, x, eof)
long *lun, *buf, *pos, *x, *eof;
{
    /* System generated locals */
    long i_1;

    /* Builtin functions */
    long s_rsue(), do_uio(), e_rsue();

    /* Local variables */
    static long i;
    static shortint ix[256];

    /* Fortran I/O blocks */
    static cilist io__2 = { 0, 0, 1, 0, 0 };


    if (*pos == 0) {
	io__2.ciunit = *lun;
	i_1 = s_rsue(&io__2);
	if (i_1 != 0) {
	    goto L999;
	}
	for (i = 0; i <= 255; ++i) {
	    i_1 = do_uio(&c__1, (char *)&ix[i], (ftnlen)sizeof(shortint));
	    if (i_1 != 0) {
		goto L999;
	    }
	}
	i_1 = e_rsue();
	for (i = 0; i <= 255; ++i) {
	    buf[i] = ix[i];
/* L10: */
	}
    }
    *x = buf[*pos];
    ++(*pos);
    if (*pos > 255) {
	*pos = 0;
    }
    return 0;
L999:
    *eof = 1;
} /* fread_ */

/*       IMPLICIT NONE */
/* Subroutine */ int fwrite_(lun, buf, pos, x)
long *lun, *buf, *pos, *x;
{
    /* System generated locals */
    long i_1, i_2;

    /* Builtin functions */
    long s_wsue(), do_uio(), e_wsue();

    /* Local variables */
    static long i;
    static shortint ix[256];

    /* Fortran I/O blocks */
    static cilist io__7 = { 0, 0, 0, 0, 0 };


    buf[*pos] = *x;
    if (*pos == 255) {
	for (i = 0; i <= 255; ++i) {
/* Computing MAX */
/* Computing MIN */
	    i_2 = buf[i];
	    i_1 = min(i_2,32767);
	    ix[i] = max(i_1,-32768);
/* L10: */
	}
	io__7.ciunit = *lun;
	s_wsue(&io__7);
	for (i = 0; i <= 255; ++i) {
	    do_uio(&c__1, (char *)&ix[i], (ftnlen)sizeof(shortint));
	}
	e_wsue();
	*pos = 0;
    } else {
	++(*pos);
    }
} /* fwrite_ */

