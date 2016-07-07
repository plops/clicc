#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cfeature_plus_reader = {feature_plus_reader, 3};

void feature_plus_reader(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[4], STACK(base, 3));	/* extra argument for #~S */
		LOAD_CHAR('+', STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	COPY(SYMVAL(Slisp, 303), STACK(base, 3));	/* *KEYWORD-PACKAGE* */
	COPY(STACK(base, 3), STACK(base, 4));
	BIND_SPECIAL(SYMBOL(Slisp, 272), STACK(base, 4));	/* *PACKAGE* */
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	LOAD_NIL(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	Fread(STACK(base, 3), 4);
	RESTORE_SPECIAL;
	COPY(STACK(base, 3), STACK(base, 4));
	eval_feature(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
		LOAD_NIL(STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
		internal_read(STACK(base, 0));
	}
	else
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
		BIND_SPECIAL(SYMBOL(Slisp, 346), STACK(base, 4));	/* *READ-SUPPRESS* */
		COPY(STACK(base, 0), STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		LOAD_NIL(STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
		internal_read(STACK(base, 5));
		LOAD_NIL(STACK(base, 0));
		mv_count = 0;
		RESTORE_SPECIAL;
	}
}
