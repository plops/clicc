#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kadd_nicknames)
{
	MAKE_STRING(35, "package with name ~S already exists"),	/* 0 */
};

void add_nicknames(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	M47_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN53;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	if(CL_SYMBOLP(STACK(base, 2)) || CL_NILP(STACK(base, 2)))
	{
		COPY(STACK(base, 2), STACK(base, 4));
		Fsymbol_name(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 2));
	}
	COPY(STACK(base, 2), STACK(base, 4));
	Ffind_package(STACK(base, 4));
	if(EQ(STACK(base, 4), STACK(base, 1)))
	{
	}
	else
	{
		if(CL_TRUEP(STACK(base, 4)))
		{
			LOAD_SMSTR((CL_FORM *)&Kadd_nicknames[0], STACK(base, 5));	/* package with name ~S already exists */
			Ferror(STACK(base, 5), 1);
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 5));
			COPY(STACK(base, 1), STACK(base, 6));
			LOAD_FIXNUM(1, STACK(base, 7));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
			struct_ref(STACK(base, 6));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 2), CAR(lptr));
				COPY(STACK(base, 6), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 6));
			}
			COPY(STACK(base, 1), STACK(base, 7));
			LOAD_FIXNUM(1, STACK(base, 8));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 9));	/* PACKAGE */
			COPY(STACK(base, 6), STACK(base, 10));
			set_struct_ref(STACK(base, 7));
		}
	}
	Fcdr(STACK(base, 3));
	goto M47_1;
	RETURN53:;
}
