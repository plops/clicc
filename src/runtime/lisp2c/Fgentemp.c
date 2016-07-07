#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFgentemp)
{
	MAKE_STRING(1, "T"),	/* 0 */
	MAKE_STRING(15, "string expected"),	/* 2 */
};

void Fgentemp(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		LOAD_SMSTR((CL_FORM *)&KFgentemp[0], STACK(base, 0));	/* T */
		case 1:
		COPY(SYMVAL(Slisp, 272), STACK(base, 1));	/* *PACKAGE* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_STRING_P(STACK(base, 0)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFgentemp[2], STACK(base, 2));	/* string expected */
		Ferror(STACK(base, 2), 1);
	}
	M242_1:;
	COPY(SYMVAL(Slisp, 596), STACK(base, 2));	/* *GENTEMP-COUNTER* */
	F1plus(STACK(base, 2));
	COPY(STACK(base, 2), SYMVAL(Slisp, 596));	/* *GENTEMP-COUNTER* */
	LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 2));	/* STRING */
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(SYMVAL(Slisp, 596), STACK(base, 4));	/* *GENTEMP-COUNTER* */
	Fmake_string_output_stream(STACK(base, 5), 0);
	COPY(STACK(base, 4), STACK(base, 6));
	COPY(STACK(base, 5), STACK(base, 7));
	Fprin1(STACK(base, 6), 2);
	COPY(STACK(base, 5), STACK(base, 4));
	Fget_output_stream_string(STACK(base, 4));
	Fconcatenate(STACK(base, 2), 3);
	COPY(STACK(base, 2), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Fintern(STACK(base, 3), 2);
	COPY(&mv_buf[0], STACK(base, 4));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 3));
			case 1:
			LOAD_NIL(STACK(base, 4));
			nargs = 2;
		}
		Flist(STACK(base, 5), nargs - 2);
		if(CL_TRUEP(STACK(base, 4)))
		{
		}
		else
		{
			COPY(STACK(base, 3), STACK(base, 0));
			goto RETURN283;
		}
	}
	goto M242_1;
	RETURN283:;
}
