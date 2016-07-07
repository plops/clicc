#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void do_external_symbols_iterator(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_FIXNUM(3, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 4));	/* PACKAGE */
	struct_ref(STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	M69_1:;
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		LOAD_NIL(STACK(base, 0));
		goto RETURN78;
	}
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fsvref(STACK(base, 6));
	M70_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN79;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Ffuncall(STACK(base, 7), 2);
	mv_count = 1;
	Fcdr(STACK(base, 6));
	goto M70_1;
	RETURN79:;
	F1plus(STACK(base, 4));
	goto M69_1;
	RETURN78:;
}
