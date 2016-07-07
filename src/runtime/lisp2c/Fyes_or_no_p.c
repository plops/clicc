#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFyes_or_no_p)
{
	MAKE_STRING(4, "~&~?"),	/* 0 */
	MAKE_STRING(12, "(yes or no):"),	/* 2 */
	MAKE_STRING(3, "yes"),	/* 4 */
	MAKE_STRING(2, "no"),	/* 6 */
	MAKE_STRING(24, "Please answer yes or no."),	/* 8 */
};

void Fyes_or_no_p(CL_FORM *base, int nargs)
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
		LOAD_SMSTR((CL_FORM *)&KFyes_or_no_p[0], STACK(base, 4));	/* ~&~? */
		COPY(STACK(base, 0), STACK(base, 5));
		COPY(STACK(base, 1), STACK(base, 6));
		Fformat(STACK(base, 3), 4);
		mv_count = 1;
	}
	M244_1:;
	LOAD_SMSTR((CL_FORM *)&KFyes_or_no_p[2], STACK(base, 3));	/* (yes or no): */
	COPY(SYMVAL(Slisp, 62), STACK(base, 4));	/* *QUERY-IO* */
	Fprinc(STACK(base, 3), 2);
	COPY(SYMVAL(Slisp, 62), STACK(base, 2));	/* *QUERY-IO* */
	Fread_line(STACK(base, 2), 1);
	mv_count = 1;
	LOAD_SMSTR((CL_FORM *)&KFyes_or_no_p[4], STACK(base, 3));	/* yes */
	COPY(STACK(base, 2), STACK(base, 4));
	Fstring_equal(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
		goto RETURN285;
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFyes_or_no_p[6], STACK(base, 3));	/* no */
		COPY(STACK(base, 2), STACK(base, 4));
		Fstring_equal(STACK(base, 3), 2);
		if(CL_TRUEP(STACK(base, 3)))
		{
			LOAD_NIL(STACK(base, 0));
			goto RETURN285;
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFyes_or_no_p[8], STACK(base, 3));	/* Please answer yes or no. */
			COPY(SYMVAL(Slisp, 62), STACK(base, 4));	/* *QUERY-IO* */
			Fprint(STACK(base, 3), 2);
		}
	}
	goto M244_1;
	RETURN285:;
}
