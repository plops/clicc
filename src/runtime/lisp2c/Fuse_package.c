#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFuse_package)
{
	MAKE_STRING(12, "can\'t use ~S"),	/* 0 */
	MAKE_STRING(32, "~S in ~S conflicts with ~S in ~S"),	/* 2 */
};

static void Z80_g836(CL_FORM *base);

void Fuse_package(CL_FORM *base, int nargs)
{
	if(nargs > 2)
	{
		goto ALL_OPT_SUPPLIED;
	}
	switch(nargs)
	{
		case 2:
		ALL_OPT_SUPPLIED:;
		GEN_HEAPVAR(STACK(base, 1), STACK(base, 0 + nargs));
	}
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 272), STACK(base, 1));	/* *PACKAGE* */
		GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(INDIRECT(STACK(base, 1)), STACK(base, 2));
	coerce_to_package(STACK(base, 2));
	COPY(STACK(base, 2), INDIRECT(STACK(base, 1)));
	if(CL_LISTP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Flist(STACK(base, 2), 1);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	LOAD_NIL(STACK(base, 2));
	GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 3));
	M60_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(INDIRECT(STACK(base, 2)));
		goto RETURN69;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), INDIRECT(STACK(base, 2)));
	coerce_to_package(STACK(base, 4));
	COPY(STACK(base, 4), INDIRECT(STACK(base, 2)));
	if(EQ(INDIRECT(STACK(base, 2)), SYMVAL(Slisp, 303)))	/* *KEYWORD-PACKAGE* */
	{
		LOAD_SMSTR((CL_FORM *)&KFuse_package[0], STACK(base, 4));	/* can't use ~S */
		COPY(INDIRECT(STACK(base, 2)), STACK(base, 5));
		Ferror(STACK(base, 4), 2);
	}
	COPY(INDIRECT(STACK(base, 2)), STACK(base, 4));
	COPY(INDIRECT(STACK(base, 1)), STACK(base, 5));
	LOAD_FIXNUM(5, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
	struct_ref(STACK(base, 5));
	Fmember(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
	}
	else
	{
		GEN_CLOSURE(array, STACK(base, 4), 5, Z80_g836, 1);
		COPY(STACK(base, 2), &array[3]);
		COPY(STACK(base, 1), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 4));
		COPY(INDIRECT(STACK(base, 2)), STACK(base, 5));
		do_external_symbols_iterator(STACK(base, 4));
		COPY(INDIRECT(STACK(base, 1)), STACK(base, 4));
		COPY(INDIRECT(STACK(base, 1)), STACK(base, 5));
		LOAD_FIXNUM(5, STACK(base, 6));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
		struct_ref(STACK(base, 5));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 6), 2);
			COPY(INDIRECT(STACK(base, 2)), CAR(lptr));
			COPY(STACK(base, 5), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 5));
		}
		COPY(INDIRECT(STACK(base, 1)), STACK(base, 6));
		LOAD_FIXNUM(5, STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
		COPY(STACK(base, 5), STACK(base, 9));
		set_struct_ref(STACK(base, 6));
		COPY(INDIRECT(STACK(base, 2)), STACK(base, 4));
		COPY(INDIRECT(STACK(base, 2)), STACK(base, 5));
		LOAD_FIXNUM(6, STACK(base, 6));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
		struct_ref(STACK(base, 5));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 6), 2);
			COPY(INDIRECT(STACK(base, 1)), CAR(lptr));
			COPY(STACK(base, 5), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 5));
		}
		COPY(INDIRECT(STACK(base, 2)), STACK(base, 6));
		LOAD_FIXNUM(6, STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
		COPY(STACK(base, 5), STACK(base, 9));
		set_struct_ref(STACK(base, 6));
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcdr(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 3));
	goto M60_1;
	RETURN69:;
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
}

static void Z80_g836(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	Fsymbol_name(STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 3));
	Ffind_symbol(STACK(base, 2), 2);
	COPY(&mv_buf[0], STACK(base, 3));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 2));
			case 1:
			LOAD_NIL(STACK(base, 3));
			nargs = 2;
		}
		Flist(STACK(base, 4), nargs - 2);
		if(CL_TRUEP(STACK(base, 3)))
		{
			if(EQ(STACK(base, 1), STACK(base, 2)))
			{
				bool_result = FALSE;
			}
			else
			{
				COPY(STACK(base, 2), STACK(base, 5));
				COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 6));
				LOAD_FIXNUM(4, STACK(base, 7));
				LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
				struct_ref(STACK(base, 6));
				Fmember(STACK(base, 5), 2);
				bool_result = NOT(CL_TRUEP(STACK(base, 5)));
			}
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			LOAD_SMSTR((CL_FORM *)&KFuse_package[2], STACK(base, 5));	/* ~S in ~S conflicts with ~S in ~S */
			COPY(STACK(base, 2), STACK(base, 6));
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 9));
			Ferror(STACK(base, 5), 5);
		}
	}
	LOAD_NIL(STACK(base, 0));
}
