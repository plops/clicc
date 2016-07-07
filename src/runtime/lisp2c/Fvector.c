#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFvector = {Fvector, -1};

void Fvector(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 0), nargs - 0);
	COPY(STACK(base, 0), STACK(base, 1));
	Flength(STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 2));	/* ELEMENT-TYPE */
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
	LOAD_SYMBOL(SYMBOL(Slisp, 99), STACK(base, 4));	/* INITIAL-CONTENTS */
	COPY(STACK(base, 0), STACK(base, 5));
	Fmake_array(STACK(base, 1), 5);
	COPY(STACK(base, 1), STACK(base, 0));
}
