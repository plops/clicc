#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void struct_constructor(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 1));	/* STRUCT-CONSTRUCTOR */
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	Fsymbol_plist(STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 5));	/* STRUCT-CONSTRUCTOR */
	get_prop(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 4), STACK(base, 0));
		Fcdr(STACK(base, 0));
		Fcar(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
