#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fhash_table_size(CL_FORM *base)
{
	LOAD_FIXNUM(0, STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 2));	/* HASH-TABLE */
	struct_ref(STACK(base, 0));
}
