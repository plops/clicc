#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void set_struct_constructor(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	COPY(STACK(base, 2), STACK(base, 0));
}
