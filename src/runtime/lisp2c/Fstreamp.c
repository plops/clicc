#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fstreamp(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 1));	/* STREAM */
	struct_typep(STACK(base, 0));
}
