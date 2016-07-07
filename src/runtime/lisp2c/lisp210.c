#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z82_g868(CL_FORM *base);

void do_all_symbols_iterator(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	LOAD_NIL(STACK(base, 1));
	{
		GEN_CLOSURE(array, STACK(base, 2), 4, Z82_g868, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 2));
	}
	COPY(STACK(base, 2), STACK(base, 2));
	COPY(SYMVAL(Slisp, 302), STACK(base, 3));	/* *PACKAGE-ARRAY* */
	Fmap(STACK(base, 1), 3);
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z82_g868(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_FIXNUM(3, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 4));	/* PACKAGE */
	struct_ref(STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	M71_1:;
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		goto RETURN80;
	}
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fsvref(STACK(base, 6));
	M72_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN81;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Ffuncall(STACK(base, 7), 2);
	mv_count = 1;
	Fcdr(STACK(base, 6));
	goto M72_1;
	RETURN81:;
	F1plus(STACK(base, 4));
	goto M71_1;
	RETURN80:;
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 2));
	LOAD_FIXNUM(2, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 4));	/* PACKAGE */
	struct_ref(STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	M73_1:;
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN82;
	}
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fsvref(STACK(base, 6));
	M74_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN83;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Ffuncall(STACK(base, 7), 2);
	mv_count = 1;
	Fcdr(STACK(base, 6));
	goto M74_1;
	RETURN83:;
	F1plus(STACK(base, 4));
	goto M73_1;
	RETURN82:;
}
