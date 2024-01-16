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

**************************************************************************/
/*-----------------------------------------------------------------*/
/**/
/*	externDef.i -- external variable definitions for vselp.c.*/
/**/
/*-----------------------------------------------------------------*/
/**/
/*	Written by: Matt Hartman*/
/**/
/*-------------------------------------------------------------*/
/*	inclusions*/
/**/

/*-------------------------------------------------------------*/
/*	external variables, declared in vselpParams.h*/
/**/
/*	system flags*/
/**/
int	SPF_FLAG, RCV_EXTERN_CODES, makeLog, packedStream,
	performMeas;

/*	file pointers*/
/**/
FILE	*fpin, *fpout, *fpcode, *fplog, *fpstream, *fpxtest;

/*	parameters which are determined by the system rate (set in*/
/*	calcParams())*/
/**/
FTYPE	F_TIME;		/* Frame length in seconds*/
int	N_SUB;		/* Number of subframes per frame*/
FTYPE	W_ALPHA;	/* Weighting coefficient used to get*/
			/* weighted direct-form coefficients from*/
			/* direct-form coefficients in widen()*/
int	C_BITS;		/* Number of bits in codeword of 1st codebook*/
int	C_BITS_A;	/* Number of bits in codeword of 2nd codebook*/
int	PLEN;		/* Number of samples in truncated impulse*/
			/* response, used in LAG_SEARCH()*/
FTYPE	POST_W_D;	/* Weighting coefficient used to get weighted*/
			/* denominator coefficients from direct-form*/
			/* coefficients in R_SUB()*/
FTYPE	POST_BEQ_N;	/* Bandwidth widening factor used to get*/
			/* post-filter numerator coefficients from*/
			/* denominator coefficients in R_SUB()*/
FTYPE	POST_EMPH;	/* Post-emphasis coefficient used in R_SUB()*/
FTYPE	POST_AGC_COEF;	/* Coefficient which determines the agc time*/
			/* constant in R_SUB()*/
int	GSP0_NUM;	/* Number of GSP0 centriods*/

/*	long-term predictor parameters*/
/**/
int	LMIN = 20;	/* Minimum lag value, used in LAG_SEARCH()*/
int	LMAX = 146;	/* Maximum lag value, used in LAG_SEARCH()*/

/*	parameters which will be calculated once, just after main()*/
/**/
int	GSP0_TERMS;	/* Number of terms in GSP0 quantizer error equation,*/
			/* used in G_QUANT()*/
int	F_LEN;		/* Frame length in samples*/
int	A_LEN;		/* Analysis length in samples, used in FLATV()*/
int	S_LEN;		/* Subframe length in samples*/
int	INBUFSIZ;	/* The length of the input speech*/
			/* buffer*/
int	numCodes;	/* Number of coded parameters in one frame*/

/*	other externals*/
/**/
int		*codes;		/* Points to the next spot in the code buffer*/
int		*codeBuf;	/* Points to the beginning of the code buffer*/
FTYPE		*inBuf;		/* Points to the beginning of the input data*/
				/* buffer*/
FTYPE		*I_CBUFF;	/* Points to the interpolated coef buffer*/
FTYPE		*RS_BUFF;	/* points to the residual energy estimate*/
				/* (RS) buffer*/
FTYPE		*COEF;		/* Points to direct form coefficients to be*/
				/* used in subframe processing*/
FTYPE		*W_COEF;	/* Points to weighted coefficients to be used*/
				/* in subframe processing on transmit side,*/
				/* denominator coefficients for the post-*/
				/* filter on the receive side*/
FTYPE		*N_COEF;	/* Points to numerator coefficients for the*/
				/* post-filter on the receive side*/
FTYPE		RS;		/* holds the RS value to be used in the*/
				/* subframe processing*/
struct coefSet	T_NEW;	/* points to the new coefs and Rq on xmit side*/
struct coefSet	T_OLD;	/* points to the old coefs and Rq on xmit side*/
struct coefSet	R_NEW;	/* points to the new coefs and Rq on rcv side*/
struct coefSet	R_OLD;	/* points to the old coefs and Rq on rcv side*/

/*	externals used in initTables() and QUANT()*/
/**/
FTYPE	*r0kAddr[NP+2];	/* points to the quantization tables for R0 and*/
			/* reflection coefficients*/
FTYPE	*sst;		/* points to the table of sst coefficients for*/
			/* FLATV()*/

/*	externals used in both T_SUB() and R_SUB()*/
/**/
FTYPE	*T_STATE;	/* Temporary filter state buffer, used in T_SUB()*/
FTYPE	*T_VEC;		/* Temporary vector buffer*/
FTYPE	*P_VEC;		/* Pitch excitation vector*/
FTYPE	*X_VEC;		/* 1st codebook excitation vector*/
FTYPE	*X_A_VEC;	/* 2nd codebook excitation vector*/
FTYPE	*BASIS;		/* 1st codebook basis vectors*/
FTYPE	*BASIS_A;	/* 2nd codebook basis vectors*/
FTYPE	*BITS;		/* Array of values equal to 1.0 or -1.0 for*/
			/* constructing codebook excitation*/
FTYPE	RS00;		/* Sqrt(residual energy / energy in pitch excitation)*/
			/* used in G_QUANT and EXCITE*/
FTYPE	RS11;		/* Sqrt(res eng / energy in 1st codebook excitation)*/
FTYPE	RS22;		/* Sqrt(res eng / energy in 2nd codebook excitation)*/
FTYPE	*GSP0_TABLE;	/* Pointer to beginning of GSP0 quantization table*/

/*	externals used only in T_SUB()*/
/**/
FTYPE	*P;		/* Weighted speech, used in T_SUB()*/
FTYPE	*W_P_VEC;	/* Weighted pitch excitation vector*/
FTYPE	*W_BASIS;	/* Weighted basis vectors*/
FTYPE	*W_X_VEC;	/* Weighted 1st codebook excitation vector*/
FTYPE	*W_X_A_VEC;	/* Weighted 2nd codebook excitation vector*/
FTYPE	*T_P_STATE;	/* Long-term predictor state*/
FTYPE	*xmtExPtr;	/* Pointer to the current subframe's excitation in*/
			/* T_P_STATE*/
int	*TABLE;		/* Gray code table for V_SRCH()*/

/*	externals used in A_SST*/
/**/
FTYPE	*P_SST;		/*  Table of sst coefficients for post-filter*/

/*	externals used only in R_SUB()*/
/**/
FTYPE	*R_P_STATE;	/* Long-term predictor state*/
FTYPE	*outBuf;	/* Output synthesized speech buffer*/

/*	DIAGnostic variables for debugging*/
/**/
int	frCnt = 1, sfCnt;
/*	end DIAG*/
					
