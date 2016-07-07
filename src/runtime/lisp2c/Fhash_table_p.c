#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fhash_table_p(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 1));	/* HASH-TABLE */
	struct_typep(STACK(base, 0));
}
