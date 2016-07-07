#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kread_token)
{
	MAKE_STRING(42, "unexpected End of File after single escape"),	/* 0 */
	MAKE_STRING(17, "illegal character"),	/* 2 */
	MAKE_STRING(42, "unexpected End of File after single escape"),	/* 4 */
	MAKE_STRING(17, "illegal Character"),	/* 6 */
	MAKE_STRING(44, "unexpected End of File after multiple escape"),	/* 8 */
	MAKE_CHAR('e'),	/* 10 CONS(10) */
	MAKE_LIST(&Kread_token[12]),
	MAKE_CHAR('s'),	/* CONS(12) */
	MAKE_LIST(&Kread_token[14]),
	MAKE_CHAR('f'),	/* CONS(14) */
	MAKE_LIST(&Kread_token[16]),
	MAKE_CHAR('d'),	/* CONS(16) */
	MAKE_LIST(&Kread_token[18]),
	MAKE_CHAR('l'),	/* CONS(18) */
	MAKE_NIL,
	MAKE_STRING(24, "ratios are not supported"),	/* 12 */
	MAKE_STRING(28, "token may not contain colons"),	/* 14 */
	MAKE_STRING(16, "illegal token ~S"),	/* 16 */
	MAKE_STRING(23, "illegal package-name ~S"),	/* 18 */
	MAKE_STRING(39, "can\'t find the external symbol ~S in ~S"),	/* 20 */
	MAKE_STRING(16, "illegal Token ~S"),	/* 22 */
	MAKE_STRING(16, "illegal token ~S"),	/* 24 */
};

static void Z87_read_digits(CL_FORM *base, CL_FORM *display[]);
static void Z88_read_sign(CL_FORM *base, CL_FORM *display[]);
static void Z89_read_float2(CL_FORM *base, CL_FORM *display[]);

void read_token(CL_FORM *base)
{
	CL_FORM *display[1];
	LOAD_NIL(STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	M88_1:;
	COPY(SYMVAL(Slisp, 382), STACK(base, 7));	/* *READTABLE* */
	COPY(STACK(base, 7), STACK(base, 8));
	LOAD_FIXNUM(0, STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 10));	/* READTABLE */
	struct_ref(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	Fchar_code(STACK(base, 8));
	vref(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 3));
	if(CL_TRUEP(STACK(base, 2)))
	{
		if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 391))	/* SINGLE-ESCAPE */
		{
			COPY(STACK(base, 0), STACK(base, 7));
			LOAD_NIL(STACK(base, 8));
			LOAD_NIL(STACK(base, 9));
			Fread_char(STACK(base, 7), 3);
			COPY(STACK(base, 7), STACK(base, 1));
			if(CL_TRUEP(STACK(base, 1)))
			{
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&Kread_token[0], STACK(base, 7));	/* unexpected End of File after single escape */
				Ferror(STACK(base, 7), 1);
			}
			COPY(STACK(base, 1), STACK(base, 7));
			COPY(SYMVAL(Slisp, 376), STACK(base, 8));	/* *TOKEN* */
			Fvector_push_extend(STACK(base, 7), 2);
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 392))	/* MULTIPLE-ESCAPE */
			{
				LOAD_NIL(STACK(base, 2));
			}
			else
			{
				if(CL_TRUEP(STACK(base, 3)))
				{
					COPY(STACK(base, 1), STACK(base, 7));
					COPY(SYMVAL(Slisp, 376), STACK(base, 8));	/* *TOKEN* */
					Fvector_push_extend(STACK(base, 7), 2);
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kread_token[2], STACK(base, 7));	/* illegal character */
					Ferror(STACK(base, 7), 1);
				}
			}
		}
	}
	else
	{
		if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 390))	/* WHITESPACE */
		{
			COPY(STACK(base, 1), STACK(base, 7));
			COPY(STACK(base, 0), STACK(base, 8));
			Funread_char(STACK(base, 7), 2);
			mv_count = 1;
			goto RETURN98;
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 391))	/* SINGLE-ESCAPE */
			{
				COPY(STACK(base, 0), STACK(base, 7));
				LOAD_NIL(STACK(base, 8));
				LOAD_NIL(STACK(base, 9));
				Fread_char(STACK(base, 7), 3);
				COPY(STACK(base, 7), STACK(base, 1));
				if(CL_TRUEP(STACK(base, 1)))
				{
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kread_token[4], STACK(base, 7));	/* unexpected End of File after single escape */
					Ferror(STACK(base, 7), 1);
				}
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
				COPY(STACK(base, 1), STACK(base, 7));
				COPY(SYMVAL(Slisp, 376), STACK(base, 8));	/* *TOKEN* */
				Fvector_push_extend(STACK(base, 7), 2);
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 392))	/* MULTIPLE-ESCAPE */
				{
					LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
					LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
				}
				else
				{
					if(CL_TRUEP(STACK(base, 3)))
					{
						if(CL_CONSP(STACK(base, 3)))
						{
							COPY(STACK(base, 3), STACK(base, 7));
							Fcdr(STACK(base, 7));
							bool_result = NOT(CL_TRUEP(STACK(base, 7)));
						}
						else
						{
							bool_result = FALSE;
						}
						if(bool_result)
						{
							COPY(STACK(base, 1), STACK(base, 7));
							COPY(STACK(base, 0), STACK(base, 8));
							Funread_char(STACK(base, 7), 2);
							mv_count = 1;
							goto RETURN98;
						}
						if(CL_CHARP(STACK(base, 1)) && GET_CHAR(STACK(base, 1)) == ':')
						{
							if(CL_TRUEP(STACK(base, 5)))
							{
								if(CL_FIXNUMP(STACK(base, 5)) && GET_FIXNUM(STACK(base, 5)) == 1)
								{
									LOAD_FIXNUM(2, STACK(base, 5));
									COPY(SYMVAL(Slisp, 376), STACK(base, 7));	/* *TOKEN* */
									Ffill_pointer(STACK(base, 7));
									F1minus(STACK(base, 7));
									if(EQ(STACK(base, 6), STACK(base, 7)) || CL_FLOATP(STACK(base, 6)) && GET_FLOAT(STACK(base, 6)) == GET_FLOAT(STACK(base, 7)))
									{
									}
									else
									{
										LOAD_FIXNUM(3, STACK(base, 5));
									}
								}
								else
								{
									LOAD_FIXNUM(3, STACK(base, 5));
								}
							}
							else
							{
								LOAD_FIXNUM(1, STACK(base, 5));
								COPY(SYMVAL(Slisp, 376), STACK(base, 7));	/* *TOKEN* */
								Ffill_pointer(STACK(base, 7));
								COPY(STACK(base, 7), STACK(base, 6));
							}
						}
						COPY(STACK(base, 1), STACK(base, 7));
						Fchar_upcase(STACK(base, 7));
						COPY(SYMVAL(Slisp, 376), STACK(base, 8));	/* *TOKEN* */
						Fvector_push_extend(STACK(base, 7), 2);
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&Kread_token[6], STACK(base, 7));	/* illegal Character */
						Ferror(STACK(base, 7), 1);
					}
				}
			}
		}
	}
	COPY(STACK(base, 0), STACK(base, 7));
	LOAD_NIL(STACK(base, 8));
	LOAD_NIL(STACK(base, 9));
	Fread_char(STACK(base, 7), 3);
	COPY(STACK(base, 7), STACK(base, 1));
	if(CL_TRUEP(STACK(base, 1)))
	{
	}
	else
	{
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_SMSTR((CL_FORM *)&Kread_token[8], STACK(base, 7));	/* unexpected End of File after multiple escape */
			Ferror(STACK(base, 7), 1);
		}
		goto RETURN98;
	}
	goto M88_1;
	RETURN98:;
	if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN97;
	}
	LOAD_FIXNUM(0, STACK(base, 7));
	COPY(SYMVAL(Slisp, 376), STACK(base, 8));	/* *TOKEN* */
	Ffill_pointer(STACK(base, 8));
	LOAD_FIXNUM(1, STACK(base, 9));
	LOAD_FIXNUM(0, STACK(base, 10));
	{
		static double local_float = 0.0;
		LOAD_FLOAT(&local_float, STACK(base, 11));
	}
	COPY(SYMVAL(Slisp, 345), STACK(base, 12));	/* *READ-BASE* */
	LOAD_NIL(STACK(base, 13));
	if(CL_TRUEP(STACK(base, 4)))
	{
		bool_result = TRUE;
	}
	else
	{
		bool_result = CL_TRUEP(STACK(base, 5));
	}
	if(bool_result)
	{
		goto M89_5;
	}
	COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
	COPY(STACK(base, 8), STACK(base, 15));
	F1minus(STACK(base, 15));
	vref(STACK(base, 14));
	if(CL_CHARP(STACK(base, 14)) && GET_CHAR(STACK(base, 14)) == '.')
	{
		LOAD_FIXNUM(10, STACK(base, 12));
	}
	display[0] = STACK(base, 0);
	Z88_read_sign(STACK(base, 14), display);
	COPY(STACK(base, 14), STACK(base, 9));
	display[0] = STACK(base, 0);
	Z87_read_digits(STACK(base, 14), display);
	COPY(STACK(base, 14), STACK(base, 10));
	if(CL_TRUEP(STACK(base, 10)))
	{
	}
	else
	{
		goto M89_1;
	}
	if(EQ(STACK(base, 7), STACK(base, 8)) || CL_FLOATP(STACK(base, 7)) && GET_FLOAT(STACK(base, 7)) == GET_FLOAT(STACK(base, 8)))
	{
		COPY(STACK(base, 9), STACK(base, 14));
		COPY(STACK(base, 10), STACK(base, 15));
		Fmult(STACK(base, 14), 2);
		COPY(STACK(base, 14), STACK(base, 0));
		goto RETURN97;
	}
	COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
	COPY(STACK(base, 7), STACK(base, 15));
	vref(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 13));
	COPY(STACK(base, 7), STACK(base, 14));
	F1plus(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 7));
	if(CL_CHARP(STACK(base, 13)) && GET_CHAR(STACK(base, 13)) == '.')
	{
		if(EQ(STACK(base, 7), STACK(base, 8)) || CL_FLOATP(STACK(base, 7)) && GET_FLOAT(STACK(base, 7)) == GET_FLOAT(STACK(base, 8)))
		{
			COPY(STACK(base, 9), STACK(base, 14));
			COPY(STACK(base, 10), STACK(base, 15));
			Fmult(STACK(base, 14), 2);
			COPY(STACK(base, 14), STACK(base, 0));
			goto RETURN97;
		}
		if(CL_FIXNUMP(STACK(base, 12)) && GET_FIXNUM(STACK(base, 12)) == 10)
		{
		}
		else
		{
			LOAD_FIXNUM(10, STACK(base, 12));
			LOAD_FIXNUM(0, STACK(base, 7));
			display[0] = STACK(base, 0);
			Z88_read_sign(STACK(base, 14), display);
			COPY(STACK(base, 14), STACK(base, 9));
			display[0] = STACK(base, 0);
			Z87_read_digits(STACK(base, 14), display);
			COPY(STACK(base, 14), STACK(base, 10));
			COPY(STACK(base, 7), STACK(base, 14));
			F1plus(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 7));
		}
		{
			static double local_float = 10.0;
			LOAD_FLOAT(&local_float, STACK(base, 14));
		}
		display[0] = STACK(base, 0);
		Z89_read_float2(STACK(base, 14), display);
		COPY(STACK(base, 14), STACK(base, 11));
		goto M89_2;
	}
	else
	{
		if(CL_CHARP(STACK(base, 13)) && GET_CHAR(STACK(base, 13)) == '/')
		{
			goto M89_4;
		}
		else
		{
			goto M89_3;
		}
	}
	M89_1:;
	LOAD_FIXNUM(0, STACK(base, 10));
	if(EQ(STACK(base, 7), STACK(base, 8)) || CL_FLOATP(STACK(base, 7)) && GET_FLOAT(STACK(base, 7)) == GET_FLOAT(STACK(base, 8)))
	{
		goto M89_5;
	}
	COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
	COPY(STACK(base, 7), STACK(base, 15));
	vref(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 13));
	COPY(STACK(base, 7), STACK(base, 14));
	F1plus(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 7));
	if(CL_CHARP(STACK(base, 13)) && GET_CHAR(STACK(base, 13)) == '.')
	{
	}
	else
	{
		goto M89_5;
	}
	if(EQ(STACK(base, 7), STACK(base, 8)) || CL_FLOATP(STACK(base, 7)) && GET_FLOAT(STACK(base, 7)) == GET_FLOAT(STACK(base, 8)))
	{
		goto M89_5;
	}
	COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
	COPY(STACK(base, 7), STACK(base, 15));
	vref(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 13));
	COPY(STACK(base, 7), STACK(base, 14));
	F1plus(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 7));
	COPY(STACK(base, 13), STACK(base, 14));
	Fdigit_char_p(STACK(base, 14), 1);
	COPY(STACK(base, 14), STACK(base, 11));
	if(CL_TRUEP(STACK(base, 11)))
	{
	}
	else
	{
		goto M89_5;
	}
	COPY(STACK(base, 11), STACK(base, 14));
	{
		static double local_float = 10.0;
		LOAD_FLOAT(&local_float, STACK(base, 15));
	}
	Fdiv(STACK(base, 14), 2);
	COPY(STACK(base, 14), STACK(base, 11));
	{
		static double local_float = 100.0;
		LOAD_FLOAT(&local_float, STACK(base, 14));
	}
	display[0] = STACK(base, 0);
	Z89_read_float2(STACK(base, 14), display);
	COPY(STACK(base, 14), STACK(base, 11));
	M89_2:;
	if(EQ(STACK(base, 7), STACK(base, 8)) || CL_FLOATP(STACK(base, 7)) && GET_FLOAT(STACK(base, 7)) == GET_FLOAT(STACK(base, 8)))
	{
		COPY(STACK(base, 9), STACK(base, 14));
		COPY(STACK(base, 10), STACK(base, 15));
		COPY(STACK(base, 11), STACK(base, 16));
		Fplus(STACK(base, 15), 2);
		Fmult(STACK(base, 14), 2);
		COPY(STACK(base, 14), STACK(base, 0));
		goto RETURN97;
	}
	COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
	COPY(STACK(base, 7), STACK(base, 15));
	vref(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 13));
	COPY(STACK(base, 7), STACK(base, 14));
	F1plus(STACK(base, 14));
	COPY(STACK(base, 14), STACK(base, 7));
	M89_3:;
	COPY(STACK(base, 13), STACK(base, 14));
	Fchar_downcase(STACK(base, 14));
	LOAD_CONS((CL_FORM *)&Kread_token[10], STACK(base, 15));
	Fmember(STACK(base, 14), 2);
	if(CL_TRUEP(STACK(base, 14)))
	{
	}
	else
	{
		goto M89_5;
	}
	if(EQ(STACK(base, 7), STACK(base, 8)) || CL_FLOATP(STACK(base, 7)) && GET_FLOAT(STACK(base, 7)) == GET_FLOAT(STACK(base, 8)))
	{
		goto M89_5;
	}
	LOAD_FIXNUM(10, STACK(base, 12));
	display[0] = STACK(base, 0);
	Z88_read_sign(STACK(base, 14), display);
	display[0] = STACK(base, 0);
	Z87_read_digits(STACK(base, 15), display);
	if(NOT(CL_TRUEP(STACK(base, 15))))
	{
		LOAD_T(STACK(base, 16));
	}
	else
	{
		LOAD_NIL(STACK(base, 16));
	}
	if(CL_TRUEP(STACK(base, 16)))
	{
		bool_result = TRUE;
	}
	else
	{
		COPY(STACK(base, 7), STACK(base, 17));
		COPY(STACK(base, 8), STACK(base, 18));
		Flt(STACK(base, 17), 2);
		bool_result = CL_TRUEP(STACK(base, 17));
	}
	if(bool_result)
	{
		goto M89_5;
	}
	COPY(STACK(base, 9), STACK(base, 16));
	COPY(STACK(base, 10), STACK(base, 17));
	COPY(STACK(base, 11), STACK(base, 18));
	Fplus(STACK(base, 17), 2);
	LOAD_FIXNUM(10, STACK(base, 18));
	COPY(STACK(base, 14), STACK(base, 19));
	COPY(STACK(base, 15), STACK(base, 20));
	Fmult(STACK(base, 19), 2);
	Fexpt(STACK(base, 18));
	Fmult(STACK(base, 16), 3);
	COPY(STACK(base, 16), STACK(base, 0));
	goto RETURN97;
	M89_4:;
	display[0] = STACK(base, 0);
	Z87_read_digits(STACK(base, 14), display);
	COPY(STACK(base, 14), STACK(base, 11));
	if(NOT(CL_TRUEP(STACK(base, 11))))
	{
		LOAD_T(STACK(base, 14));
	}
	else
	{
		LOAD_NIL(STACK(base, 14));
	}
	if(CL_TRUEP(STACK(base, 14)))
	{
		bool_result = TRUE;
	}
	else
	{
		COPY(STACK(base, 7), STACK(base, 15));
		COPY(STACK(base, 8), STACK(base, 16));
		Flt(STACK(base, 15), 2);
		bool_result = CL_TRUEP(STACK(base, 15));
	}
	if(bool_result)
	{
		goto M89_5;
	}
	LOAD_SMSTR((CL_FORM *)&Kread_token[20], STACK(base, 14));	/* ratios are not supported */
	Fwarn(STACK(base, 14), 1);
	mv_count = 1;
	COPY(STACK(base, 10), STACK(base, 14));
	COPY(STACK(base, 11), STACK(base, 15));
	Fdiv(STACK(base, 14), 2);
	COPY(STACK(base, 14), STACK(base, 0));
	goto RETURN97;
	M89_5:;
	if(CL_TRUEP(STACK(base, 5)))
	{
		if(CL_TRUEP(SYMVAL(Slisp, 377)))	/* *UNINTERNED* */
		{
			LOAD_SMSTR((CL_FORM *)&Kread_token[22], STACK(base, 14));	/* token may not contain colons */
			Ferror(STACK(base, 14), 1);
		}
		else
		{
			if(CL_FIXNUMP(STACK(base, 6)) && GET_FIXNUM(STACK(base, 6)) == 0)
			{
				if(CL_FIXNUMP(STACK(base, 5)) && GET_FIXNUM(STACK(base, 5)) == 1)
				{
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kread_token[24], STACK(base, 14));	/* illegal token ~S */
					COPY(SYMVAL(Slisp, 376), STACK(base, 15));	/* *TOKEN* */
					Ferror(STACK(base, 14), 2);
				}
				COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
				LOAD_FIXNUM(1, STACK(base, 15));
				Fsubseq(STACK(base, 14), 2);
				COPY(SYMVAL(Slisp, 303), STACK(base, 15));	/* *KEYWORD-PACKAGE* */
				Fintern(STACK(base, 14), 2);
				mv_count = 1;
				COPY(STACK(base, 14), STACK(base, 0));
				goto RETURN97;
			}
			else
			{
				COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
				LOAD_FIXNUM(0, STACK(base, 15));
				COPY(STACK(base, 6), STACK(base, 16));
				Fsubseq(STACK(base, 14), 3);
				COPY(STACK(base, 14), STACK(base, 15));
				Ffind_package(STACK(base, 15));
				COPY(SYMVAL(Slisp, 376), STACK(base, 16));	/* *TOKEN* */
				COPY(STACK(base, 6), STACK(base, 17));
				COPY(STACK(base, 5), STACK(base, 18));
				Fplus(STACK(base, 17), 2);
				Fsubseq(STACK(base, 16), 2);
				if(CL_TRUEP(STACK(base, 15)))
				{
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kread_token[26], STACK(base, 17));	/* illegal package-name ~S */
					COPY(STACK(base, 14), STACK(base, 18));
					Ferror(STACK(base, 17), 2);
				}
				if(CL_FIXNUMP(STACK(base, 5)) && GET_FIXNUM(STACK(base, 5)) == 2)
				{
					COPY(STACK(base, 16), STACK(base, 17));
					COPY(STACK(base, 15), STACK(base, 18));
					Fintern(STACK(base, 17), 2);
					mv_count = 1;
					COPY(STACK(base, 17), STACK(base, 0));
					goto RETURN97;
				}
				else
				{
					if(CL_FIXNUMP(STACK(base, 5)) && GET_FIXNUM(STACK(base, 5)) == 1)
					{
						COPY(STACK(base, 16), STACK(base, 17));
						COPY(STACK(base, 15), STACK(base, 18));
						Ffind_symbol(STACK(base, 17), 2);
						COPY(&mv_buf[0], STACK(base, 18));
						{
							int nargs;
							nargs = 2;
							mv_count = 1;
							switch(nargs)
							{
								case 0:
								LOAD_NIL(STACK(base, 17));
								case 1:
								LOAD_NIL(STACK(base, 18));
								nargs = 2;
							}
							Flist(STACK(base, 19), nargs - 2);
							if(CL_SYMBOLP(STACK(base, 18)) && GET_SYMBOL(STACK(base, 18)) == SYMBOL(Slisp, 306))	/* EXTERNAL */
							{
							}
							else
							{
								LOAD_SMSTR((CL_FORM *)&Kread_token[28], STACK(base, 20));	/* can't find the external symbol ~S in ~S */
								COPY(STACK(base, 16), STACK(base, 21));
								COPY(STACK(base, 15), STACK(base, 22));
								Ferror(STACK(base, 20), 3);
							}
							COPY(STACK(base, 17), STACK(base, 0));
							goto RETURN97;
						}
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&Kread_token[30], STACK(base, 17));	/* illegal Token ~S */
						COPY(SYMVAL(Slisp, 376), STACK(base, 18));	/* *TOKEN* */
						Ferror(STACK(base, 17), 2);
					}
				}
			}
		}
	}
	else
	{
		if(CL_TRUEP(STACK(base, 4)))
		{
		}
		else
		{
			if(CL_FIXNUMP(STACK(base, 8)) && GET_FIXNUM(STACK(base, 8)) == 1)
			{
				COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
				LOAD_FIXNUM(0, STACK(base, 15));
				vref(STACK(base, 14));
				if(CL_CHARP(STACK(base, 14)) && GET_CHAR(STACK(base, 14)) == '.')
				{
					bool_result = CL_TRUEP(SYMVAL(Slisp, 379));	/* *DOT-FLAG* */
				}
				else
				{
					bool_result = FALSE;
				}
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				LOAD_NIL(SYMVAL(Slisp, 379));	/* *DOT-FLAG* */
				LOAD_NIL(STACK(base, 0));
				goto RETURN97;
			}
			LOAD_FIXNUM(0, STACK(base, 14));
			M90_1:;
			COPY(STACK(base, 14), STACK(base, 15));
			COPY(STACK(base, 8), STACK(base, 16));
			Fge(STACK(base, 15), 2);
			if(CL_TRUEP(STACK(base, 15)))
			{
				LOAD_SMSTR((CL_FORM *)&Kread_token[32], STACK(base, 15));	/* illegal token ~S */
				COPY(SYMVAL(Slisp, 376), STACK(base, 16));	/* *TOKEN* */
				Ferror(STACK(base, 15), 2);
				goto RETURN99;
			}
			COPY(SYMVAL(Slisp, 376), STACK(base, 15));	/* *TOKEN* */
			COPY(STACK(base, 14), STACK(base, 16));
			vref(STACK(base, 15));
			if(CL_CHARP(STACK(base, 15)) && GET_CHAR(STACK(base, 15)) == '.')
			{
			}
			else
			{
				goto RETURN99;
			}
			COPY(STACK(base, 14), STACK(base, 15));
			F1plus(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 14));
			goto M90_1;
			RETURN99:;
		}
		if(CL_TRUEP(SYMVAL(Slisp, 377)))	/* *UNINTERNED* */
		{
			COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
			Fmake_symbol(STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 0));
		}
		else
		{
			COPY(SYMVAL(Slisp, 376), STACK(base, 14));	/* *TOKEN* */
			Fintern(STACK(base, 14), 1);
			mv_count = 1;
			COPY(STACK(base, 14), STACK(base, 0));
		}
		goto RETURN97;
	}
	LOAD_NIL(STACK(base, 0));
	RETURN97:;
}

static void Z87_read_digits(CL_FORM *base, CL_FORM *display[])
{
	LOAD_NIL(STACK(base, 0));
	LOAD_NIL(STACK(base, 1));
	if(EQ(&display[0][7], &display[0][8]) || CL_FLOATP(&display[0][7]) && GET_FLOAT(&display[0][7]) == GET_FLOAT(&display[0][8]))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(SYMVAL(Slisp, 376), STACK(base, 2));	/* *TOKEN* */
		COPY(&display[0][7], STACK(base, 3));
		vref(STACK(base, 2));
		COPY(&display[0][12], STACK(base, 3));
		Fdigit_char_p(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 0));
		if(CL_TRUEP(STACK(base, 0)))
		{
			COPY(&display[0][7], STACK(base, 2));
			F1plus(STACK(base, 2));
			COPY(STACK(base, 2), &display[0][7]);
			M91_1:;
			if(EQ(&display[0][7], &display[0][8]) || CL_FLOATP(&display[0][7]) && GET_FLOAT(&display[0][7]) == GET_FLOAT(&display[0][8]))
			{
				goto RETURN100;
			}
			COPY(SYMVAL(Slisp, 376), STACK(base, 1));	/* *TOKEN* */
			COPY(&display[0][7], STACK(base, 2));
			vref(STACK(base, 1));
			COPY(&display[0][12], STACK(base, 2));
			Fdigit_char_p(STACK(base, 1), 2);
			if(CL_TRUEP(STACK(base, 1)))
			{
			}
			else
			{
				goto RETURN100;
			}
			COPY(&display[0][7], STACK(base, 2));
			F1plus(STACK(base, 2));
			COPY(STACK(base, 2), &display[0][7]);
			COPY(&display[0][12], STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			Fmult(STACK(base, 2), 2);
			COPY(STACK(base, 1), STACK(base, 3));
			Fplus(STACK(base, 2), 2);
			COPY(STACK(base, 2), STACK(base, 0));
			goto M91_1;
			RETURN100:;
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
}

static void Z88_read_sign(CL_FORM *base, CL_FORM *display[])
{
	if(EQ(&display[0][7], &display[0][8]) || CL_FLOATP(&display[0][7]) && GET_FLOAT(&display[0][7]) == GET_FLOAT(&display[0][8]))
	{
		LOAD_FIXNUM(1, STACK(base, 0));
	}
	else
	{
		COPY(SYMVAL(Slisp, 376), STACK(base, 0));	/* *TOKEN* */
		COPY(&display[0][7], STACK(base, 1));
		vref(STACK(base, 0));
		if(CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == '-')
		{
			COPY(&display[0][7], STACK(base, 1));
			F1plus(STACK(base, 1));
			COPY(STACK(base, 1), &display[0][7]);
			LOAD_FIXNUM(-1, STACK(base, 0));
		}
		else
		{
			if(CL_CHARP(STACK(base, 0)) && GET_CHAR(STACK(base, 0)) == '+')
			{
				COPY(&display[0][7], STACK(base, 1));
				F1plus(STACK(base, 1));
				COPY(STACK(base, 1), &display[0][7]);
				LOAD_FIXNUM(1, STACK(base, 0));
			}
			else
			{
				LOAD_FIXNUM(1, STACK(base, 0));
			}
		}
	}
}

static void Z89_read_float2(CL_FORM *base, CL_FORM *display[])
{
	LOAD_NIL(STACK(base, 1));
	M92_1:;
	if(EQ(&display[0][7], &display[0][8]) || CL_FLOATP(&display[0][7]) && GET_FLOAT(&display[0][7]) == GET_FLOAT(&display[0][8]))
	{
		goto RETURN101;
	}
	COPY(SYMVAL(Slisp, 376), STACK(base, 2));	/* *TOKEN* */
	COPY(&display[0][7], STACK(base, 3));
	vref(STACK(base, 2));
	COPY(STACK(base, 2), &display[0][13]);
	COPY(&display[0][13], STACK(base, 1));
	Fdigit_char_p(STACK(base, 1), 1);
	if(CL_TRUEP(STACK(base, 1)))
	{
	}
	else
	{
		goto RETURN101;
	}
	COPY(&display[0][7], STACK(base, 2));
	F1plus(STACK(base, 2));
	COPY(STACK(base, 2), &display[0][7]);
	COPY(&display[0][11], STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	Fdiv(STACK(base, 3), 2);
	Fplus(STACK(base, 2), 2);
	COPY(STACK(base, 2), &display[0][11]);
	COPY(STACK(base, 0), STACK(base, 2));
	{
		static double local_float = 10.0;
		LOAD_FLOAT(&local_float, STACK(base, 3));
	}
	Fmult(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
	goto M92_1;
	RETURN101:;
	COPY(&display[0][11], STACK(base, 0));
}
