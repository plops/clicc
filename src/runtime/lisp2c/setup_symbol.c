#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void setup_symbol(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 2));
	symbol_package_index(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		Fminusp(STACK(base, 4));
		if(CL_TRUEP(STACK(base, 4)))
		{
			COPY(STACK(base, 2), STACK(base, 4));
			Fminus(STACK(base, 4), 1);
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 4));
		}
		vref(STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Fsymbol_name(STACK(base, 4));
		LOAD_FIXNUM(101, STACK(base, 5));
		string_hash(STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 5));
		COPY(STACK(base, 0), STACK(base, 6));
		set_symbol_package(STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 5));
		LOAD_FIXNUM(0, STACK(base, 6));
		Fle(STACK(base, 5), 2);
		if(CL_TRUEP(STACK(base, 5)))
		{
			COPY(STACK(base, 3), STACK(base, 5));
			LOAD_FIXNUM(3, STACK(base, 6));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
			struct_ref(STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 6));
			COPY(STACK(base, 4), STACK(base, 7));
			Fsvref(STACK(base, 6));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 0), CAR(lptr));
				COPY(STACK(base, 6), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 6));
			}
			COPY(STACK(base, 6), STACK(base, 0));
			COPY(STACK(base, 5), STACK(base, 1));
			COPY(STACK(base, 4), STACK(base, 2));
			Fset_svref(STACK(base, 0));
		}
		else
		{
			COPY(STACK(base, 3), STACK(base, 5));
			LOAD_FIXNUM(2, STACK(base, 6));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
			struct_ref(STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 6));
			COPY(STACK(base, 4), STACK(base, 7));
			Fsvref(STACK(base, 6));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 0), CAR(lptr));
				COPY(STACK(base, 6), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 6));
			}
			COPY(STACK(base, 6), STACK(base, 0));
			COPY(STACK(base, 5), STACK(base, 1));
			COPY(STACK(base, 4), STACK(base, 2));
			Fset_svref(STACK(base, 0));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
