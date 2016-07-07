#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cnil_fun = {nil_fun, 0};

void nil_fun(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 0));
}
