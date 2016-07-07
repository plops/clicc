#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void do_symbols_iterator(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_FIXNUM(3, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 4));	/* PACKAGE */
	struct_ref(STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	M62_1:;
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		goto RETURN71;
	}
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fsvref(STACK(base, 6));
	M63_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN72;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Ffuncall(STACK(base, 7), 2);
	mv_count = 1;
	Fcdr(STACK(base, 6));
	goto M63_1;
	RETURN72:;
	F1plus(STACK(base, 4));
	goto M62_1;
	RETURN71:;
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 2));
	LOAD_FIXNUM(2, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 4));	/* PACKAGE */
	struct_ref(STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	M64_1:;
	COPY(STACK(base, 4), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fge(STACK(base, 5), 2);
	if(CL_TRUEP(STACK(base, 5)))
	{
		goto RETURN73;
	}
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 2), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fsvref(STACK(base, 6));
	M65_1:;
	if(CL_ATOMP(STACK(base, 6)))
	{
		LOAD_NIL(STACK(base, 5));
		goto RETURN74;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 5));
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Ffuncall(STACK(base, 7), 2);
	mv_count = 1;
	Fcdr(STACK(base, 6));
	goto M65_1;
	RETURN74:;
	F1plus(STACK(base, 4));
	goto M64_1;
	RETURN73:;
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(5, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 6));	/* PACKAGE */
	struct_ref(STACK(base, 4));
	M66_1:;
	if(CL_ATOMP(STACK(base, 4)))
	{
		LOAD_NIL(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 0));
		goto RETURN75;
	}
	COPY(STACK(base, 4), STACK(base, 5));
	Fcar(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 3));
	COPY(STACK(base, 5), STACK(base, 6));
	LOAD_FIXNUM(2, STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 8));	/* PACKAGE */
	struct_ref(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 2));
	LOAD_FIXNUM(101, STACK(base, 5));
	LOAD_FIXNUM(0, STACK(base, 6));
	M67_1:;
	COPY(STACK(base, 6), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Fge(STACK(base, 7), 2);
	if(CL_TRUEP(STACK(base, 7)))
	{
		goto RETURN76;
	}
	LOAD_NIL(STACK(base, 7));
	COPY(STACK(base, 2), STACK(base, 8));
	COPY(STACK(base, 6), STACK(base, 9));
	Fsvref(STACK(base, 8));
	M68_1:;
	if(CL_ATOMP(STACK(base, 8)))
	{
		LOAD_NIL(STACK(base, 7));
		goto RETURN77;
	}
	COPY(STACK(base, 8), STACK(base, 9));
	Fcar(STACK(base, 9));
	COPY(STACK(base, 9), STACK(base, 7));
	COPY(STACK(base, 0), STACK(base, 9));
	COPY(STACK(base, 7), STACK(base, 10));
	Ffuncall(STACK(base, 9), 2);
	mv_count = 1;
	Fcdr(STACK(base, 8));
	goto M68_1;
	RETURN77:;
	F1plus(STACK(base, 6));
	goto M67_1;
	RETURN76:;
	Fcdr(STACK(base, 4));
	goto M66_1;
	RETURN75:;
}
