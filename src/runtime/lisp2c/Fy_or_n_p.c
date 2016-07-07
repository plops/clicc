#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFy_or_n_p)
{
	MAKE_STRING(4, "~&~?"),	/* 0 */
	MAKE_STRING(9, "(y or n):"),	/* 2 */
	MAKE_STRING(1, "y"),	/* 4 */
	MAKE_STRING(1, "n"),	/* 6 */
	MAKE_STRING(21, "Please answer y or n."),	/* 8 */
};

void Fy_or_n_p(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		LOAD_NIL(STACK(base, 0));
		nargs = 1;
	}
	Flist(STACK(base, 1), nargs - 1);
	LOAD_NIL(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 0)))
	{
		COPY(SYMVAL(Slisp, 62), STACK(base, 3));	/* *QUERY-IO* */
		LOAD_SMSTR((CL_FORM *)&KFy_or_n_p[0], STACK(base, 4));	/* ~&~? */
		COPY(STACK(base, 0), STACK(base, 5));
		COPY(STACK(base, 1), STACK(base, 6));
		Fformat(STACK(base, 3), 4);
		mv_count = 1;
	}
	M243_1:;
	LOAD_SMSTR((CL_FORM *)&KFy_or_n_p[2], STACK(base, 3));	/* (y or n): */
	COPY(SYMVAL(Slisp, 62), STACK(base, 4));	/* *QUERY-IO* */
	Fprinc(STACK(base, 3), 2);
	COPY(SYMVAL(Slisp, 62), STACK(base, 2));	/* *QUERY-IO* */
	Fread_line(STACK(base, 2), 1);
	mv_count = 1;
	LOAD_SMSTR((CL_FORM *)&KFy_or_n_p[4], STACK(base, 3));	/* y */
	COPY(STACK(base, 2), STACK(base, 4));
	Fstring_equal(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
		goto RETURN284;
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFy_or_n_p[6], STACK(base, 3));	/* n */
		COPY(STACK(base, 2), STACK(base, 4));
		Fstring_equal(STACK(base, 3), 2);
		if(CL_TRUEP(STACK(base, 3)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN284;
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFy_or_n_p[8], STACK(base, 3));	/* Please answer y or n. */
			COPY(SYMVAL(Slisp, 62), STACK(base, 4));	/* *QUERY-IO* */
			Fprint(STACK(base, 3), 2);
		}
	}
	goto M243_1;
	RETURN284:;
}
