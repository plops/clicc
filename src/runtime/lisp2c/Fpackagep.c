#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fpackagep(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 1));	/* PACKAGE */
	struct_typep(STACK(base, 0));
}
