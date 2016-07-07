#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fpathnamep(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 1));	/* PATHNAME */
	struct_typep(STACK(base, 0));
}
