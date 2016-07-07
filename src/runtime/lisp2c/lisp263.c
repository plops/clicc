#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_dispatch_macro_character)
{
	MAKE_STRING(89, "no ~S dispatch function defined for subchar ~S ~\n                           (with arg ~S)"),	/* 0 */
};

static void Z86_g1013(CL_FORM *base);

void Fmake_dispatch_macro_character(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		COPY(SYMVAL(Slisp, 382), STACK(base, 2));	/* *READTABLE* */
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	LOAD_FIXNUM(256, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 4));	/* INITIAL-ELEMENT */
	LOAD_NIL(STACK(base, 5));
	Fmake_array(STACK(base, 3), 3);
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 4));
	LOAD_FIXNUM(0, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 6));	/* READTABLE */
	struct_ref(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fchar_code(STACK(base, 5));
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z86_g1013, 2);
		COPY(STACK(base, 3), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 7), 2);
		COPY(STACK(base, 6), CAR(lptr));
		COPY(STACK(base, 1), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 7));
	COPY(STACK(base, 4), STACK(base, 8));
	COPY(STACK(base, 5), STACK(base, 9));
	Fset_aref(STACK(base, 7), 3);
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	LOAD_FIXNUM(1, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 7));	/* READTABLE */
	struct_ref(STACK(base, 5));
	Fassoc(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 5));
		COPY(INDIRECT(STACK(base, 3)), STACK(base, 6));
		if(CL_CONSP(STACK(base, 4)))
		{
			COPY(INDIRECT(STACK(base, 3)), CDR(GET_FORM(STACK(base, 4))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[6], STACK(base, 7));	/* ~a is not a cons */
			COPY(STACK(base, 4), STACK(base, 8));
			Ferror(STACK(base, 7), 2);
		}
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 5));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 6), 2);
			COPY(STACK(base, 0), CAR(lptr));
			COPY(INDIRECT(STACK(base, 3)), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 6));
		}
		COPY(STACK(base, 2), STACK(base, 7));
		LOAD_FIXNUM(1, STACK(base, 8));
		LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 9));	/* READTABLE */
		struct_ref(STACK(base, 7));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 8), 2);
			COPY(STACK(base, 6), CAR(lptr));
			COPY(STACK(base, 7), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 6));
		}
		COPY(STACK(base, 2), STACK(base, 7));
		LOAD_FIXNUM(1, STACK(base, 8));
		LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 9));	/* READTABLE */
		COPY(STACK(base, 6), STACK(base, 10));
		set_struct_ref(STACK(base, 7));
	}
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
}

static void Z86_g1013(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	LOAD_NIL(STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 7));	/* T */
	Fread_char(STACK(base, 4), 4);
	COPY(STACK(base, 4), STACK(base, 5));
	Fdigit_char_p(STACK(base, 5), 1);
	LOAD_NIL(STACK(base, 6));
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 5), STACK(base, 3));
		M87_1:;
		COPY(STACK(base, 1), STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
		LOAD_NIL(STACK(base, 9));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 10));	/* T */
		Fread_char(STACK(base, 7), 4);
		COPY(STACK(base, 7), STACK(base, 4));
		Fdigit_char_p(STACK(base, 7), 1);
		COPY(STACK(base, 7), STACK(base, 5));
		if(CL_TRUEP(STACK(base, 5)))
		{
		}
		else
		{
			if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
			{
				LOAD_NIL(STACK(base, 3));
			}
			goto RETURN96;
		}
		COPY(STACK(base, 5), STACK(base, 7));
		LOAD_FIXNUM(10, STACK(base, 8));
		COPY(STACK(base, 3), STACK(base, 9));
		Fmult(STACK(base, 8), 2);
		Fplus(STACK(base, 7), 2);
		COPY(STACK(base, 7), STACK(base, 3));
		goto M87_1;
		RETURN96:;
	}
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fchar_upcase(STACK(base, 7));
	Fchar_code(STACK(base, 7));
	vref(STACK(base, 6));
	if(CL_TRUEP(STACK(base, 6)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_dispatch_macro_character[0], STACK(base, 7));	/* no ~S dispatch function defined for subchar ~S ~
                           (with arg ~S) */
		COPY(STACK(base, 2), STACK(base, 8));
		COPY(STACK(base, 4), STACK(base, 9));
		COPY(STACK(base, 3), STACK(base, 10));
		Ferror(STACK(base, 7), 4);
	}
	COPY(STACK(base, 6), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 4), STACK(base, 9));
	COPY(STACK(base, 3), STACK(base, 10));
	Ffuncall(STACK(base, 7), 4);
	COPY(STACK(base, 7), STACK(base, 0));
}
