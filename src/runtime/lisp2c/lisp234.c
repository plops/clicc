#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cbackquote_reader = {backquote_reader, 2};

void backquote_reader(CL_FORM *base)
{
	COPY(SYMVAL(Slisp, 375), STACK(base, 2));	/* *BQ-LEVEL* */
	F1plus(STACK(base, 2));
	COPY(STACK(base, 2), SYMVAL(Slisp, 375));	/* *BQ-LEVEL* */
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
	LOAD_NIL(STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	internal_read(STACK(base, 2));
	bq_process(STACK(base, 2));
	bq_simplify(STACK(base, 2));
	bq_remove_tokens(STACK(base, 2));
	COPY(SYMVAL(Slisp, 375), STACK(base, 3));	/* *BQ-LEVEL* */
	F1minus(STACK(base, 3));
	COPY(STACK(base, 3), SYMVAL(Slisp, 375));	/* *BQ-LEVEL* */
	COPY(STACK(base, 2), STACK(base, 0));
}
