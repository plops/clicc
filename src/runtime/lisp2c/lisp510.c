/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

GEN_GLOBAL_FUNARG(Cbackquote_reader, backquote_reader, 2);

void backquote_reader(CL_FORM *base)
{
	COPY(SYMVAL(Slisp, 437), ARG(2));	/* *BQ-LEVEL* */
	F1plus(ARG(2));
	COPY(ARG(2), SYMVAL(Slisp, 437));	/* *BQ-LEVEL* */
	COPY(ARG(0), ARG(2));
	LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(3));	/* T */
	LOAD_NIL(ARG(4));
	LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(5));	/* T */
	internal_read(ARG(2));
	bq_process(ARG(2));
	bq_simplify(ARG(2));
	bq_remove_tokens(ARG(2));
	mv_count = 1;
	COPY(SYMVAL(Slisp, 437), ARG(3));	/* *BQ-LEVEL* */
	F1minus(ARG(3));
	COPY(ARG(3), SYMVAL(Slisp, 437));	/* *BQ-LEVEL* */
	COPY(ARG(2), ARG(0));
}
