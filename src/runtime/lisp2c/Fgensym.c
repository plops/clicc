#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFgensym)
{
	MAKE_STRING(35, "positive integer or string expected"),	/* 0 */
};

void Fgensym(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		LOAD_NIL(STACK(base, 0));
		case 1:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 0)))
	{
		LOAD_FIXNUM(0, STACK(base, 1));
		if(CL_FIXNUMP(STACK(base, 0)) && GET_FIXNUM(STACK(base, 0)) >= GET_FIXNUM(STACK(base, 1)))
		{
			COPY(STACK(base, 0), SYMVAL(Slisp, 590));	/* *GENSYM-COUNTER* */
		}
		else
		{
			if(CL_ARRAY_P(STACK(base, 0)))
			{
				COPY(STACK(base, 0), STACK(base, 1));
				LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 2));	/* STANDARD-CHAR */
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 3));	/* * */
				check_array_internal(STACK(base, 1));
				bool_result = CL_TRUEP(STACK(base, 1));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 0), SYMVAL(Slisp, 595));	/* *GENSYM-PREFIX* */
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFgensym[0], STACK(base, 1));	/* positive integer or string expected */
				Ferror(STACK(base, 1), 1);
			}
		}
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 1));	/* STRING */
	COPY(SYMVAL(Slisp, 595), STACK(base, 2));	/* *GENSYM-PREFIX* */
	COPY(SYMVAL(Slisp, 590), STACK(base, 3));	/* *GENSYM-COUNTER* */
	Fmake_string_output_stream(STACK(base, 4), 0);
	COPY(STACK(base, 3), STACK(base, 5));
	COPY(STACK(base, 4), STACK(base, 6));
	Fprin1(STACK(base, 5), 2);
	COPY(STACK(base, 4), STACK(base, 3));
	Fget_output_stream_string(STACK(base, 3));
	Fconcatenate(STACK(base, 1), 3);
	Fmake_symbol(STACK(base, 1));
	COPY(SYMVAL(Slisp, 590), STACK(base, 2));	/* *GENSYM-COUNTER* */
	F1plus(STACK(base, 2));
	COPY(STACK(base, 2), SYMVAL(Slisp, 590));	/* *GENSYM-COUNTER* */
	COPY(STACK(base, 1), STACK(base, 0));
}
