#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fpackage_used_by_list(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 1));
	coerce_to_package(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
	LOAD_FIXNUM(6, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 2));	/* PACKAGE */
	struct_ref(STACK(base, 0));
}
