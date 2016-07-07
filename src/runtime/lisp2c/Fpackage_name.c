#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fpackage_name(CL_FORM *base)
{
	LOAD_FIXNUM(0, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 2));	/* PACKAGE */
	struct_ref(STACK(base, 0));
}
