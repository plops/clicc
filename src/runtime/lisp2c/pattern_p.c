#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cpattern_p = {pattern_p, 1};

void pattern_p(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 1));	/* PATTERN */
	struct_typep(STACK(base, 0));
}
