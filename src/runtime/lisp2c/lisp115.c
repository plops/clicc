#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kremove_backslashes)
{
	MAKE_STRING(23, "Backslash in bad place."),	/* 0 */
};

void remove_backslashes(CL_FORM *base)
{
	COPY(STACK(base, 2), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Fminus(STACK(base, 3), 2);
	Fmake_string(STACK(base, 3), 1);
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	M24_1:;
	COPY(STACK(base, 6), STACK(base, 7));
	COPY(STACK(base, 2), STACK(base, 8));
	Fnumeql(STACK(base, 7), 2);
	if(CL_TRUEP(STACK(base, 7)))
	{
		goto RETURN25;
	}
	if(CL_TRUEP(STACK(base, 5)))
	{
		COPY(STACK(base, 0), STACK(base, 7));
		COPY(STACK(base, 6), STACK(base, 8));
		Fschar(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 8));
		COPY(STACK(base, 3), STACK(base, 9));
		COPY(STACK(base, 4), STACK(base, 10));
		Fset_schar(STACK(base, 8));
		LOAD_NIL(STACK(base, 5));
		COPY(STACK(base, 4), STACK(base, 7));
		F1plus(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 4));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 7));
		COPY(STACK(base, 6), STACK(base, 8));
		Fschar(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 8));
		LOAD_CHAR('\\', STACK(base, 9));
		FcharE(STACK(base, 8), 2);
		if(CL_TRUEP(STACK(base, 8)))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
		}
		else
		{
			COPY(STACK(base, 7), STACK(base, 8));
			COPY(STACK(base, 3), STACK(base, 9));
			COPY(STACK(base, 4), STACK(base, 10));
			Fset_schar(STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 8));
			F1plus(STACK(base, 8));
			COPY(STACK(base, 8), STACK(base, 4));
		}
	}
	F1plus(STACK(base, 6));
	goto M24_1;
	RETURN25:;
	if(CL_TRUEP(STACK(base, 5)))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 225), STACK(base, 6));	/* NAMESTRING-PARSE-ERROR */
		LOAD_SYMBOL(SYMBOL(Slisp, 226), STACK(base, 7));	/* COMPLAINT */
		LOAD_SMSTR((CL_FORM *)&Kremove_backslashes[0], STACK(base, 8));	/* Backslash in bad place. */
		LOAD_SYMBOL(SYMBOL(Slisp, 227), STACK(base, 9));	/* NAMESTRING */
		COPY(STACK(base, 0), STACK(base, 10));
		LOAD_SYMBOL(SYMBOL(Slisp, 228), STACK(base, 11));	/* OFFSET */
		COPY(STACK(base, 2), STACK(base, 12));
		F1minus(STACK(base, 12));
		Ferror(STACK(base, 6), 7);
	}
	COPY(STACK(base, 3), STACK(base, 0));
	COPY(STACK(base, 4), STACK(base, 1));
	shrink_vector(STACK(base, 0));
}
