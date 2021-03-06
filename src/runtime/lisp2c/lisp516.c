/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

GEN_GLOBAL_FUNARG(Cnull_or_quoted, null_or_quoted, 1);

void null_or_quoted(CL_FORM *base)
{
	if(CL_TRUEP(ARG(0)))
	{
		LOAD_NIL(ARG(1));
	}
	else
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(1));	/* T */
	}
	if(CL_TRUEP(ARG(1)))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(0));	/* T */
	}
	else
	{
		if(CL_CONSP(ARG(0)))
		{
			COPY(GET_CAR(ARG(0)), ARG(2));
			LOAD_BOOL(EQ(ARG(2), SYMVAL(Slisp, 434)), ARG(0));	/* *BQ-QUOTE* */
		}
		else
		{
			LOAD_NIL(ARG(0));
		}
	}
}
