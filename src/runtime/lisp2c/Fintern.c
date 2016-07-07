#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fintern(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 272), STACK(base, 1));	/* *PACKAGE* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	coerce_to_package(STACK(base, 1));
	COPY(STACK(base, 0), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Ffind_symbol(STACK(base, 2), 2);
	COPY(&mv_buf[0], STACK(base, 3));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 2));
			case 1:
			LOAD_NIL(STACK(base, 3));
			nargs = 2;
		}
		Flist(STACK(base, 4), nargs - 2);
		if(CL_TRUEP(STACK(base, 3)))
		{
			COPY(STACK(base, 2), STACK(base, 5));
			COPY(STACK(base, 3), STACK(base, 6));
			COPY(STACK(base, 5), STACK(base, 0));
			COPY(STACK(base, 6), &mv_buf[0]);
			mv_count = 2;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 5));
			Fmake_symbol(STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 6));
			internal_import(STACK(base, 5));
			COPY(STACK(base, 2), STACK(base, 5));
			LOAD_NIL(STACK(base, 6));
			COPY(STACK(base, 5), STACK(base, 0));
			COPY(STACK(base, 6), &mv_buf[0]);
			mv_count = 2;
		}
	}
}
