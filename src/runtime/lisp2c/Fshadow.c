#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fshadow(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 272), STACK(base, 1));	/* *PACKAGE* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	coerce_to_package(STACK(base, 1));
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
	COPY(STACK(base, 0), STACK(base, 3));
	M59_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN68;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	Fsymbol_name(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	Ffind_symbol(STACK(base, 5), 2);
	COPY(&mv_buf[0], STACK(base, 6));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 5));
			case 1:
			LOAD_NIL(STACK(base, 6));
			nargs = 2;
		}
		Flist(STACK(base, 7), nargs - 2);
		if(CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 305))	/* INTERNAL */
		{
			LOAD_T(STACK(base, 8));
		}
		else
		{
			LOAD_NIL(STACK(base, 8));
		}
		if(CL_TRUEP(STACK(base, 8)))
		{
			bool_result = TRUE;
		}
		else
		{
			bool_result = CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 306);	/* EXTERNAL */
		}
		if(bool_result)
		{
		}
		else
		{
			COPY(STACK(base, 4), STACK(base, 8));
			Fmake_symbol(STACK(base, 8));
			COPY(STACK(base, 8), STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 9));
			internal_import(STACK(base, 8));
		}
		COPY(STACK(base, 1), STACK(base, 8));
		COPY(STACK(base, 1), STACK(base, 9));
		LOAD_FIXNUM(4, STACK(base, 10));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 11));	/* PACKAGE */
		struct_ref(STACK(base, 9));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 10), 2);
			COPY(STACK(base, 2), CAR(lptr));
			COPY(STACK(base, 9), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 9));
		}
		COPY(STACK(base, 1), STACK(base, 10));
		LOAD_FIXNUM(4, STACK(base, 11));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 12));	/* PACKAGE */
		COPY(STACK(base, 9), STACK(base, 13));
		set_struct_ref(STACK(base, 10));
	}
	Fcdr(STACK(base, 3));
	goto M59_1;
	RETURN68:;
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
}
