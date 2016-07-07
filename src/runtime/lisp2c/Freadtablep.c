#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Freadtablep(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 1));	/* READTABLE */
	struct_typep(STACK(base, 0));
}
