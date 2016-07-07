/* This file was generated by CLiCC 0.6.4 [obrep 1] */
#define __OBREP 1
#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT Kpeek_char1[] =
{
	MAKE_STRING(22, "unexpected end of file"),	/* 0 */
};

void peek_char1(CL_FORM *base)
{
	if(CL_TRUEP(ARG(1)))
	{
		if(CL_SYMBOLP(ARG(1)) && GET_SYMBOL(ARG(1)) == SYMBOL(Slisp, 48))	/* T */
		{
			COPY(SYMVAL(Slisp, 59), ARG(1));	/* *TERMINAL-IO* */
		}
	}
	else
	{
		COPY(SYMVAL(Slisp, 60), ARG(1));	/* *STANDARD-INPUT* */
	}
	COPY(ARG(1), ARG(5));
	LOAD_NIL(ARG(6));
	LOAD_NIL(ARG(7));
	COPY(ARG(4), ARG(8));
	read_char1(ARG(5));
	if(CL_SYMBOLP(ARG(0)) && GET_SYMBOL(ARG(0)) == SYMBOL(Slisp, 48))	/* T */
	{
		M1_1:;
		if(CL_TRUEP(ARG(5)))
		{
		}
		else
		{
			goto RETURN1;
		}
		COPY(SYMVAL(Slisp, 454), ARG(6));	/* *READTABLE* */
		COPY(ARG(6), ARG(7));
		LOAD_SYMBOL(SYMBOL(Slisp, 420), ARG(8));	/* READTABLE */
		rt_struct_typep(ARG(7));
		if(CL_TRUEP(ARG(7)))
		{
			COPY(OFFSET(AR_BASE(GET_FORM(ARG(6))), 0 + 1), ARG(6));
		}
		else
		{
			COPY(SYMVAL(Slisp, 352), ARG(7));	/* NO_STRUCT */
			COPY(ARG(6), ARG(8));
			LOAD_SYMBOL(SYMBOL(Slisp, 420), ARG(9));	/* READTABLE */
			Ferror(ARG(7), 3);
		}
		if(CL_CHARP(ARG(5)))
		{
		}
		else
		{
			COPY(SYMVAL(Slisp, 58), ARG(7));	/* WRONG_TYPE */
			COPY(ARG(5), ARG(8));
			LOAD_SYMBOL(SYMBOL(Slisp, 18), ARG(9));	/* CHARACTER */
			Ferror(ARG(7), 3);
		}
		COPY(ARG(5), ARG(7));
		rt_char_code(ARG(7));
		LOAD_BOOL(CL_SMVECP(ARG(6)), ARG(8));
		if(CL_TRUEP(ARG(8)))
		{
			goto THEN1;
		}
		else
		{
			COPY(ARG(6), ARG(9));
			LOAD_SYMBOL(SYMBOL(Slisp, 150), ARG(10));	/* COMPLEX-VECTOR */
			rt_struct_typep(ARG(9));
		}
		if(CL_TRUEP(ARG(9)))
		{
			THEN1:;
		}
		else
		{
			COPY(SYMVAL(Slisp, 58), ARG(8));	/* WRONG_TYPE */
			COPY(ARG(6), ARG(9));
			LOAD_SYMBOL(SYMBOL(Slisp, 47), ARG(10));	/* VECTOR */
			Ferror(ARG(8), 3);
		}
		Frow_major_aref(ARG(6));
		if(CL_SYMBOLP(ARG(6)) && GET_SYMBOL(ARG(6)) == SYMBOL(Slisp, 462))	/* WHITESPACE */
		{
		}
		else
		{
			goto RETURN1;
		}
		COPY(ARG(1), ARG(6));
		COPY(ARG(4), ARG(7));
		COPY(ARG(6), ARG(8));
		LOAD_NIL(ARG(9));
		LOAD_NIL(ARG(10));
		COPY(ARG(7), ARG(11));
		read_char1(ARG(8));
		COPY(ARG(8), ARG(5));
		goto M1_1;
		RETURN1:;
	}
	else
	{
		if(CL_CHARP(ARG(0)))
		{
			M2_1:;
			if(CL_TRUEP(ARG(5)))
			{
			}
			else
			{
				goto RETURN2;
			}
			if(EQL(ARG(0), ARG(5)))
			{
			}
			else
			{
				goto RETURN2;
			}
			COPY(ARG(1), ARG(6));
			COPY(ARG(4), ARG(7));
			COPY(ARG(6), ARG(8));
			LOAD_NIL(ARG(9));
			LOAD_NIL(ARG(10));
			COPY(ARG(7), ARG(11));
			read_char1(ARG(8));
			COPY(ARG(8), ARG(5));
			goto M2_1;
			RETURN2:;
		}
	}
	if(CL_TRUEP(ARG(5)))
	{
	}
	else
	{
		if(CL_TRUEP(ARG(2)))
		{
			LOAD_SMSTR((CL_FORM *)&Kpeek_char1[0], ARG(6));	/* unexpected end of file */
			Ferror(ARG(6), 1);
		}
	}
	COPY(ARG(5), ARG(6));
	COPY(ARG(1), ARG(7));
	unread_char1(ARG(6));
	mv_count = 1;
	COPY(ARG(5), ARG(0));
}