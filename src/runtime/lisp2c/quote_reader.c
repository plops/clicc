#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cquote_reader = {quote_reader, 2};

void quote_reader(CL_FORM *base)
{
	LOAD_SYMBOL(SYMBOL(Slisp, 383), STACK(base, 2));	/* QUOTE */
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	LOAD_NIL(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	internal_read(STACK(base, 3));
	Flist(STACK(base, 2), 2);
	COPY(STACK(base, 2), STACK(base, 0));
}
