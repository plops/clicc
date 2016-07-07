#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fshadowing_import(CL_FORM *base, int nargs)
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
	M58_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN67;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	Fsymbol_name(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	Ffind_symbol(STACK(base, 4), 2);
	COPY(&mv_buf[0], STACK(base, 5));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 4));
			case 1:
			LOAD_NIL(STACK(base, 5));
			nargs = 2;
		}
		Flist(STACK(base, 6), nargs - 2);
		COPY(STACK(base, 2), STACK(base, 7));
		Fsymbol_name(STACK(base, 7));
		LOAD_FIXNUM(101, STACK(base, 8));
		string_hash(STACK(base, 7));
		if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 305))	/* INTERNAL */
		{
			COPY(STACK(base, 4), STACK(base, 8));
			COPY(STACK(base, 1), STACK(base, 9));
			LOAD_FIXNUM(2, STACK(base, 10));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 11));	/* PACKAGE */
			struct_ref(STACK(base, 9));
			COPY(STACK(base, 7), STACK(base, 10));
			del_pack_sym(STACK(base, 8));
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 306))	/* EXTERNAL */
			{
				COPY(STACK(base, 4), STACK(base, 8));
				COPY(STACK(base, 1), STACK(base, 9));
				LOAD_FIXNUM(3, STACK(base, 10));
				LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 11));	/* PACKAGE */
				struct_ref(STACK(base, 9));
				COPY(STACK(base, 7), STACK(base, 10));
				del_pack_sym(STACK(base, 8));
			}
		}
	}
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	internal_import(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	LOAD_FIXNUM(4, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
	struct_ref(STACK(base, 5));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 6), 2);
		COPY(STACK(base, 2), CAR(lptr));
		COPY(STACK(base, 5), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 5));
	}
	COPY(STACK(base, 1), STACK(base, 6));
	LOAD_FIXNUM(4, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
	COPY(STACK(base, 5), STACK(base, 9));
	set_struct_ref(STACK(base, 6));
	Fcdr(STACK(base, 3));
	goto M58_1;
	RETURN67:;
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
}
