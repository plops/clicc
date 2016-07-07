#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFparse_integer)
{
	MAKE_STRING(18, "illegal integer ~S"),	/* 0 */
};

void Fparse_integer(CL_FORM *base, int nargs)
{
	BOOL supl_flags[4];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
		SYMBOL(Slisp, 398),	/* RADIX */
		SYMBOL(Slisp, 195),	/* JUNK-ALLOWED */
	};
	keysort(STACK(base, 1), nargs - 1, 4, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_FIXNUM(0, STACK(base, 1));
	}
	if(NOT(supl_flags[1]))
	{
		COPY(STACK(base, 0), STACK(base, 5));
		Flength(STACK(base, 5));
		COPY(STACK(base, 5), STACK(base, 2));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_FIXNUM(10, STACK(base, 3));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	LOAD_NIL(STACK(base, 7));
	M104_1:;
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 2), STACK(base, 9));
	Fge(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		goto M103_1;
	}
	COPY(STACK(base, 0), STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	Fchar(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 5));
	COPY(SYMVAL(Slisp, 382), STACK(base, 8));	/* *READTABLE* */
	LOAD_FIXNUM(0, STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 10));	/* READTABLE */
	struct_ref(STACK(base, 8));
	COPY(STACK(base, 5), STACK(base, 9));
	Fchar_code(STACK(base, 9));
	vref(STACK(base, 8));
	if(CL_SYMBOLP(STACK(base, 8)) && GET_SYMBOL(STACK(base, 8)) == SYMBOL(Slisp, 390))	/* WHITESPACE */
	{
	}
	else
	{
		goto RETURN116;
	}
	goto M104_1;
	RETURN116:;
	if(CL_CHARP(STACK(base, 5)) && GET_CHAR(STACK(base, 5)) == '-')
	{
		COPY(STACK(base, 1), STACK(base, 8));
		F1plus(STACK(base, 8));
		COPY(STACK(base, 8), STACK(base, 1));
	}
	else
	{
		if(CL_CHARP(STACK(base, 5)) && GET_CHAR(STACK(base, 5)) == '+')
		{
			COPY(STACK(base, 1), STACK(base, 8));
			F1plus(STACK(base, 8));
			COPY(STACK(base, 8), STACK(base, 1));
		}
	}
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 2), STACK(base, 9));
	Fge(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
		goto M103_1;
	}
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 1), STACK(base, 9));
	Fchar(STACK(base, 8));
	COPY(STACK(base, 3), STACK(base, 9));
	Fdigit_char_p(STACK(base, 8), 2);
	COPY(STACK(base, 8), STACK(base, 6));
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 1), STACK(base, 8));
		F1plus(STACK(base, 8));
		COPY(STACK(base, 8), STACK(base, 1));
		M105_1:;
		if(EQ(STACK(base, 1), STACK(base, 2)) || CL_FLOATP(STACK(base, 1)) && GET_FLOAT(STACK(base, 1)) == GET_FLOAT(STACK(base, 2)))
		{
			goto RETURN117;
		}
		COPY(STACK(base, 0), STACK(base, 7));
		COPY(STACK(base, 1), STACK(base, 8));
		Fchar(STACK(base, 7));
		COPY(STACK(base, 3), STACK(base, 8));
		Fdigit_char_p(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
		}
		else
		{
			goto RETURN117;
		}
		COPY(STACK(base, 1), STACK(base, 8));
		F1plus(STACK(base, 8));
		COPY(STACK(base, 8), STACK(base, 1));
		COPY(STACK(base, 3), STACK(base, 8));
		COPY(STACK(base, 6), STACK(base, 9));
		Fmult(STACK(base, 8), 2);
		COPY(STACK(base, 7), STACK(base, 9));
		Fplus(STACK(base, 8), 2);
		COPY(STACK(base, 8), STACK(base, 6));
		goto M105_1;
		RETURN117:;
	}
	else
	{
		goto M103_1;
	}
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 6), STACK(base, 8));
		COPY(STACK(base, 1), STACK(base, 9));
		COPY(STACK(base, 8), STACK(base, 0));
		COPY(STACK(base, 9), &mv_buf[0]);
		mv_count = 2;
		goto RETURN115;
	}
	else
	{
		M106_1:;
		COPY(STACK(base, 1), STACK(base, 8));
		COPY(STACK(base, 2), STACK(base, 9));
		Fge(STACK(base, 8), 2);
		if(CL_TRUEP(STACK(base, 8)))
		{
			goto RETURN118;
		}
		COPY(STACK(base, 0), STACK(base, 8));
		COPY(STACK(base, 1), STACK(base, 9));
		Fchar(STACK(base, 8));
		COPY(STACK(base, 8), STACK(base, 5));
		COPY(SYMVAL(Slisp, 382), STACK(base, 8));	/* *READTABLE* */
		LOAD_FIXNUM(0, STACK(base, 9));
		LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 10));	/* READTABLE */
		struct_ref(STACK(base, 8));
		COPY(STACK(base, 5), STACK(base, 9));
		Fchar_code(STACK(base, 9));
		vref(STACK(base, 8));
		if(CL_SYMBOLP(STACK(base, 8)) && GET_SYMBOL(STACK(base, 8)) == SYMBOL(Slisp, 390))	/* WHITESPACE */
		{
		}
		else
		{
			goto M103_2;
		}
		goto M106_1;
		RETURN118:;
		COPY(STACK(base, 6), STACK(base, 8));
		COPY(STACK(base, 1), STACK(base, 9));
		COPY(STACK(base, 8), STACK(base, 0));
		COPY(STACK(base, 9), &mv_buf[0]);
		mv_count = 2;
		goto RETURN115;
	}
	M103_1:;
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 8));
		COPY(STACK(base, 1), STACK(base, 9));
		COPY(STACK(base, 8), STACK(base, 0));
		COPY(STACK(base, 9), &mv_buf[0]);
		mv_count = 2;
		goto RETURN115;
	}
	M103_2:;
	LOAD_SMSTR((CL_FORM *)&KFparse_integer[0], STACK(base, 8));	/* illegal integer ~S */
	COPY(STACK(base, 0), STACK(base, 9));
	Ferror(STACK(base, 8), 2);
	LOAD_NIL(STACK(base, 0));
	RETURN115:;
}
