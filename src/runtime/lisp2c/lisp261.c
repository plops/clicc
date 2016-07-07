#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cfunction_reader = {function_reader, 3};

void function_reader(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[4], STACK(base, 3));	/* extra argument for #~S */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 25), STACK(base, 3));	/* FUNCTION */
	COPY(STACK(base, 0), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	LOAD_NIL(STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 7));	/* T */
	internal_read(STACK(base, 4));
	Flist(STACK(base, 3), 2);
	COPY(STACK(base, 3), STACK(base, 0));
}
