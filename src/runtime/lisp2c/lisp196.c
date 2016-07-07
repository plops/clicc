#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Funuse_package(CL_FORM *base, int nargs)
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
	M61_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN70;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	coerce_to_package(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	LOAD_FIXNUM(5, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
	struct_ref(STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 310), STACK(base, 7));	/* COUNT */
	LOAD_FIXNUM(1, STACK(base, 8));
	Fdelete(STACK(base, 5), 4);
	COPY(STACK(base, 1), STACK(base, 6));
	LOAD_FIXNUM(5, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
	COPY(STACK(base, 5), STACK(base, 9));
	set_struct_ref(STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	LOAD_FIXNUM(5, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
	struct_ref(STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 310), STACK(base, 7));	/* COUNT */
	LOAD_FIXNUM(1, STACK(base, 8));
	Fdelete(STACK(base, 5), 4);
	COPY(STACK(base, 2), STACK(base, 6));
	LOAD_FIXNUM(6, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
	COPY(STACK(base, 5), STACK(base, 9));
	set_struct_ref(STACK(base, 6));
	Fcdr(STACK(base, 3));
	goto M61_1;
	RETURN70:;
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
}
