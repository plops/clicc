#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void stream_writec(CL_FORM *base)
{
	LOAD_FIXNUM(4, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 2));	/* STREAM */
	struct_ref(STACK(base, 0));
}
