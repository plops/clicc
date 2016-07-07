#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cuninterned_reader = {uninterned_reader, 3};

CL_INIT2(Kuninterned_reader)
{
	MAKE_STRING(30, "illegal value (~S) followed #:"),	/* 0 */
};

void uninterned_reader(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[4], STACK(base, 3));	/* extra argument for #~S */
		COPY(STACK(base, 1), STACK(base, 4));
		Ferror(STACK(base, 3), 2);
	}
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
	BIND_SPECIAL(SYMBOL(Slisp, 377), STACK(base, 3));	/* *UNINTERNED* */
	COPY(STACK(base, 0), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	LOAD_NIL(STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 7));	/* T */
	Fread(STACK(base, 4), 4);
	bool_result = CL_TRUEP(SYMVAL(Slisp, 346));	/* *READ-SUPPRESS* */
	if(bool_result)
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		if(CL_SYMBOLP(STACK(base, 4)) || CL_NILP(STACK(base, 4)))
		{
			COPY(STACK(base, 4), STACK(base, 0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kuninterned_reader[0], STACK(base, 0));	/* illegal value (~S) followed #: */
			COPY(STACK(base, 4), STACK(base, 1));
			Ferror(STACK(base, 0), 2);
		}
	}
	RESTORE_SPECIAL;
}
