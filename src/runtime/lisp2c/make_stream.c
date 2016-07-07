#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cmake_stream = {make_stream, -1};

void make_stream(CL_FORM *base, int nargs)
{
	BOOL supl_flags[10];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 79),	/* TYPE */
		SYMBOL(Slisp, 80),	/* EXTRA */
		SYMBOL(Slisp, 81),	/* READC */
		SYMBOL(Slisp, 82),	/* UNREADC */
		SYMBOL(Slisp, 83),	/* WRITEC */
		SYMBOL(Slisp, 84),	/* COLUMN */
		SYMBOL(Slisp, 85),	/* TELL */
		SYMBOL(Slisp, 86),	/* SEEK */
		SYMBOL(Slisp, 87),	/* LENGTH */
		SYMBOL(Slisp, 88),	/* CLOSE */
	};
	keysort(STACK(base, 0), nargs - 0, 10, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 0));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 2));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 3));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 4));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_GLOBFUN(&Cnil_fun, STACK(base, 5));
	}
	if(NOT(supl_flags[6]))
	{
		LOAD_GLOBFUN(&Cnil_fun, STACK(base, 6));
	}
	if(NOT(supl_flags[7]))
	{
		LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 7));
	}
	if(NOT(supl_flags[8]))
	{
		LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 8));
	}
	if(NOT(supl_flags[9]))
	{
		LOAD_NIL(STACK(base, 9));
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 10));	/* STREAM */
	COPY(STACK(base, 0), STACK(base, 11));
	COPY(STACK(base, 1), STACK(base, 12));
	COPY(STACK(base, 2), STACK(base, 13));
	COPY(STACK(base, 3), STACK(base, 14));
	COPY(STACK(base, 4), STACK(base, 15));
	COPY(STACK(base, 5), STACK(base, 16));
	COPY(STACK(base, 6), STACK(base, 17));
	COPY(STACK(base, 7), STACK(base, 18));
	COPY(STACK(base, 8), STACK(base, 19));
	COPY(STACK(base, 9), STACK(base, 20));
	make_struct(STACK(base, 10), 11);
	COPY(STACK(base, 10), STACK(base, 0));
}
