#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void internal_import(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	Fsymbol_name(STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 3));
	string_hash(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	Fsymbol_package(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		set_symbol_package(STACK(base, 3));
	}
	if(EQ(STACK(base, 1), SYMVAL(Slisp, 303)))	/* *KEYWORD-PACKAGE* */
	{
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Fset(STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 3));
		set_constant_flag(STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 3));
		LOAD_FIXNUM(3, STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 5));	/* PACKAGE */
		struct_ref(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fsvref(STACK(base, 4));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 5), 2);
			COPY(STACK(base, 0), CAR(lptr));
			COPY(STACK(base, 4), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 4));
		}
		COPY(STACK(base, 4), STACK(base, 0));
		COPY(STACK(base, 3), STACK(base, 1));
		Fset_svref(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 3));
		LOAD_FIXNUM(2, STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 5));	/* PACKAGE */
		struct_ref(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fsvref(STACK(base, 4));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 5), 2);
			COPY(STACK(base, 0), CAR(lptr));
			COPY(STACK(base, 4), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 4));
		}
		COPY(STACK(base, 4), STACK(base, 0));
		COPY(STACK(base, 3), STACK(base, 1));
		Fset_svref(STACK(base, 0));
	}
}
