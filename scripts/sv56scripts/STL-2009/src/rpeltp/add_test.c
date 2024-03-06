/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 *
 * Updated by KIRCHHERR (FI/DBP Telekom) for Alpha VMS/Apx: included unixio.h
 */

/* $Header: /home/kbs/jutta/src/gsm/gsm-1.0/add-test/RCS/add_test.c,v 1.1 1992/10/28 00:09:10 jutta Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef VMS
#include <unixio.h> 
#endif

#define	GSM_IMPLEMENTATION
#include "private.h"
#include "gsm.h"

#include "add.c"

int		interactive = 1;

char		* opname;
longword	L_op1, L_op2, L_expect;
word		op1, op2, expect;
int		do_expect;

void help()
{
puts( "  add a b      sub a b     mult a b   div    a b" );
puts( "L_add A B    L_sub A B   L_mult A B   mult_r a b" );
puts( "" );
puts( "abs   a      norm  a        >> a b      << a b" );
puts( "                          L_>> A B    L_<< A B" );

}

char * strtek P2((str, sep), char * str, char * sep) {

	static char     * S = (char *)0;
	char		* c, * base;

	if (str) S = str;

	if (!S || !*S) return (char *)0;

	/*  Skip delimiters.
	 */
	while (*S) {
		for (c = sep; *c && *c != *S; c++) ;
		if (*c) *S++ = 0;
		else break;
	}

	base = S;

	/*   Skip non-delimiters.
	 */
	for (base = S; *S; S++) {

		for (c = sep; *c; c++)
			if (*c == *S) {
				*S++ = 0;
				return base;
			}
	}

	return base == S ? (char *)0 : base;
}

long value P1((s), char * s)
{
	switch (*s) {
	case '-': switch (s[1]) {
		  case '\0': return MIN_WORD;
		  case '-':  return MIN_LONGWORD;
		  default:   break;
		  }
		  break;

	case '+': switch (s[1]) {
		  case '\0': return MAX_WORD;
		  case '+':  return MAX_LONGWORD;
		  default:   break;
		  }
	default:  break;
	}

	return strtol(s, (char **)0, 0);
}

char * parse P1((buf), char * buf)
{
	char  * s, * a;
	long	l;

	if ((a = strchr(buf, '='))!=NULL)
	  *a++ = 0;

	opname = s = strtek(buf, " \t("); 
	if (!s) return (char *)0;

	op1 = op2 = L_op1 = L_op2 = 0;

	if ((s = strtek( (char *)0, "( \t,"))!=NULL) {
		op1 = L_op1 = value(s);
		if ((s = strtek( (char *)0, ", \t)"))!=NULL)
		  op2 = L_op2 = value(s);
	}

	if (a) {
		do_expect = 1;
		while (*a == ' ' || *a == '\t') a++;
		expect = L_expect = value(a);
	}

	return opname;
}

void fprint_word P2((f, w), FILE * f,  word w)
{
	if (!w) putc('0', f);
	else fprintf(f, "0x%4.4x (%d%s)",
		(unsigned int)w,
		(int)w,
		w == MIN_WORD? "/-" : (w == MAX_WORD ? "/+" : ""));
}

void print_word P1((w), word w)
{
	fprint_word( stdout, w );
}

void fprint_longword P2((f, w), FILE * f, longword w)
{
	if (!w) putc('0', f);
	else fprintf(f, "0x%8.8x (%ld%s)",
		w, w, w == MIN_WORD ? "/-"
		: (w == MAX_WORD ? "/+"
		: (w == (longword)MIN_LONGWORD ? "/--"
		: (w == (longword)MAX_LONGWORD ? "/++" : ""))));
}

void print_longword P1((w),longword w)
{
	fprint_longword(stdout, w);
}

void do_longword P1((w), longword w)
{
	if (interactive) print_longword(w);
	if (do_expect) {
		if (w != L_expect) {
			if (!interactive) fprint_longword(stderr, w);
			fprintf(stderr, " != %s (%ld, %ld) -- expected ",
				opname, L_op1, L_op2 );
			fprint_longword(stderr, L_expect);
			putc( '\n', stderr );
		}
	} else if (interactive) putchar('\n');
}

void do_word P1((w), word w )
{
	if (interactive) print_word(w);
	if (do_expect) {
		if (w != expect) {
			if (!interactive) fprint_word(stderr, w);
			fprintf(stderr, " != %s (%ld, %ld) -- expected ",
				opname, L_op1, L_op2 );
			fprint_word(stderr, expect);
			putc('\n', stderr);
		}
	} else if (interactive) putchar('\n');
}

int main(argc,argv)
int argc;
char *argv[];
{
	char	buf[299];
	char	* c;
	FILE *datain;

	if (argc>1)
	  datain = fopen(argv[1], "r");
	else
	  datain = stdin;

	if (datain==NULL)
	{
	  perror("Invalid input file!\n");
	  exit(1);
	}

	interactive = isatty(fileno(datain));

	for (;;) {
		if (interactive) fprintf(stderr, "? ");

		if (!fgets(buf, sizeof(buf), datain)) exit(0);
		if ((c = strchr(buf, '\n'))!=NULL) *c = 0;

		if (*buf == ';' || *buf == '#') continue;
		if (*buf == '\'') {
			puts(buf + 1);
			continue;
		}
		if (*buf == '\"') {
			fprintf(stderr,  "%s\n", buf + 1);
			continue;
		}

		c = parse(buf);

		if (!c) continue;
		if (!strcmp(c,   "add")) {
			do_word(    gsm_add( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "sub")) {
			do_word(    gsm_sub( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "mult")) {
			do_word(    gsm_mult( op1, op2 ));
			continue;
		}
		if (!strcmp(c, "mult_r")) {
			do_word(    gsm_mult_r(op1, op2));
			continue;
		}
		if (!strcmp(c, "abs" )) {
			do_word(    gsm_abs(op1) );
			continue;
		} 
		if (!strcmp(c, "div" )) {
			do_word(    gsm_div( op1, op2 ));
			continue;
		}
		if (!strcmp(c,  "norm" )) {
			do_word(	gsm_norm(L_op1));
			continue;
		} 
		if (!strcmp(c,  "<<" )) {
			do_word(    gsm_asl( op1, op2));
			continue;
		} 
		if (!strcmp(c,  ">>" )) {
			do_word(    gsm_asr( op1, op2 ));
			continue;
		}
		if (!strcmp(c,  "L_mult")) {
			do_longword( gsm_L_mult( op1, op2 ));
			continue;
		}
		if (!strcmp(c,  "L_add" )) {
			do_longword( gsm_L_add( L_op1, L_op2 ));
			continue;
		}
		if (!strcmp(c,  "L_sub" )) {
			do_longword( gsm_L_sub( L_op1, L_op2 ));
			continue;
		}
		if (!strcmp(c,  "L_<<" )) {
			do_longword(    gsm_L_asl( L_op1, L_op2 ));
			continue;
		}
		if (!strcmp(c,  "L_>>")) {
			do_longword(    gsm_L_asr( L_op1, L_op2 ));
			continue;
		}
		help();
	}
}
