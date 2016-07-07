#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void host_unparse_host(CL_FORM *base)
{
	LOAD_FIXNUM(2, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 2));	/* HOST */
	struct_ref(STACK(base, 0));
}
