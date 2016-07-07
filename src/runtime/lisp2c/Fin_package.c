#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fin_package(CL_FORM *base, int nargs)
{
	BOOL supl_flags[2];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 304),	/* NICKNAMES */
		SYMBOL(Slisp, 311),	/* USE */
	};
	keysort(STACK(base, 1), nargs - 1, 2, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 2));
		LOAD_NIL(STACK(base, 3));
	}
	else
	{
		LOAD_T(STACK(base, 3));
	}
	COPY(STACK(base, 0), STACK(base, 4));
	Ffind_package(STACK(base, 4));
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 1), STACK(base, 5));
		COPY(STACK(base, 4), STACK(base, 6));
		add_nicknames(STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 5));
		COPY(STACK(base, 4), STACK(base, 6));
		Fuse_package(STACK(base, 5), 2);
		COPY(STACK(base, 4), SYMVAL(Slisp, 272));	/* *PACKAGE* */
		COPY(SYMVAL(Slisp, 272), STACK(base, 0));	/* *PACKAGE* */
	}
	else
	{
		if(CL_TRUEP(STACK(base, 3)))
		{
			COPY(STACK(base, 0), STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Slisp, 304), STACK(base, 6));	/* NICKNAMES */
			COPY(STACK(base, 1), STACK(base, 7));
			LOAD_SYMBOL(SYMBOL(Slisp, 311), STACK(base, 8));	/* USE */
			COPY(STACK(base, 2), STACK(base, 9));
			Fmake_package(STACK(base, 5), 5);
			COPY(STACK(base, 5), SYMVAL(Slisp, 272));	/* *PACKAGE* */
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Slisp, 304), STACK(base, 6));	/* NICKNAMES */
			COPY(STACK(base, 1), STACK(base, 7));
			Fmake_package(STACK(base, 5), 3);
			COPY(STACK(base, 5), SYMVAL(Slisp, 272));	/* *PACKAGE* */
		}
		COPY(SYMVAL(Slisp, 272), STACK(base, 0));	/* *PACKAGE* */
	}
}
