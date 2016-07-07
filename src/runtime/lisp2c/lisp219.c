#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_hash_table_count = {Fset_hash_table_count, 2};

void Fset_hash_table_count(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_FIXNUM(1, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 4));	/* HASH-TABLE */
	COPY(STACK(base, 0), STACK(base, 5));
	set_struct_ref(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}
