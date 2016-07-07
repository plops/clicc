#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFpeek_char)
{
	MAKE_STRING(22, "unexpected end of file"),	/* 0 */
};

void Fpeek_char(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		LOAD_NIL(STACK(base, 0));
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
		case 3:
		LOAD_NIL(STACK(base, 3));
		case 4:
		LOAD_NIL(STACK(base, 4));
		case 5:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Ssys, 1))	/* T */
		{
			COPY(SYMVAL(Slisp, 58), STACK(base, 1));	/* *TERMINAL-IO* */
		}
	}
	else
	{
		COPY(SYMVAL(Slisp, 59), STACK(base, 1));	/* *STANDARD-INPUT* */
	}
	COPY(STACK(base, 1), STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	LOAD_NIL(STACK(base, 7));
	COPY(STACK(base, 4), STACK(base, 8));
	Fread_char(STACK(base, 5), 4);
	if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Ssys, 1))	/* T */
	{
		M101_1:;
		if(CL_TRUEP(STACK(base, 5)))
		{
		}
		else
		{
			goto RETURN113;
		}
		COPY(SYMVAL(Slisp, 382), STACK(base, 6));	/* *READTABLE* */
		LOAD_FIXNUM(0, STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 8));	/* READTABLE */
		struct_ref(STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 7));
		Fchar_code(STACK(base, 7));
		vref(STACK(base, 6));
		if(CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 390))	/* WHITESPACE */
		{
		}
		else
		{
			goto RETURN113;
		}
		COPY(STACK(base, 1), STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
		LOAD_NIL(STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 8));
		Fread_char(STACK(base, 5), 4);
		goto M101_1;
		RETURN113:;
	}
	else
	{
		if(CL_CHARP(STACK(base, 0)))
		{
			M102_1:;
			if(CL_TRUEP(STACK(base, 5)))
			{
			}
			else
			{
				goto RETURN114;
			}
			if(EQ(STACK(base, 0), STACK(base, 5)) || CL_FLOATP(STACK(base, 0)) && GET_FLOAT(STACK(base, 0)) == GET_FLOAT(STACK(base, 5)))
			{
			}
			else
			{
				goto RETURN114;
			}
			COPY(STACK(base, 1), STACK(base, 5));
			LOAD_NIL(STACK(base, 6));
			LOAD_NIL(STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 8));
			Fread_char(STACK(base, 5), 4);
			goto M102_1;
			RETURN114:;
		}
	}
	if(CL_TRUEP(STACK(base, 5)))
	{
	}
	else
	{
		if(CL_TRUEP(STACK(base, 2)))
		{
			LOAD_SMSTR((CL_FORM *)&KFpeek_char[0], STACK(base, 6));	/* unexpected end of file */
			Ferror(STACK(base, 6), 1);
		}
	}
	COPY(STACK(base, 5), STACK(base, 6));
	COPY(STACK(base, 1), STACK(base, 7));
	Funread_char(STACK(base, 6), 2);
	mv_count = 1;
	COPY(STACK(base, 5), STACK(base, 0));
}
