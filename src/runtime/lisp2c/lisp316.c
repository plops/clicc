#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kcheck_seq_start_end)
{
	MAKE_STRING(51, "The START value ~S is greater than the END value ~S"),	/* 0 */
};

void check_seq_start_end(CL_FORM *base)
{
	if(CL_FIXNUMP(STACK(base, 0)))
	{
		LOAD_FIXNUM(0, STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fle(STACK(base, 3), 3);
		bool_result = CL_TRUEP(STACK(base, 3));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 476), STACK(base, 3));	/* NOT_IN_RANGE */
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 1));
	}
	if(CL_FIXNUMP(STACK(base, 1)))
	{
		LOAD_FIXNUM(0, STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fle(STACK(base, 3), 3);
		bool_result = CL_TRUEP(STACK(base, 3));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
	}
	else
	{
		COPY(SYMVAL(Slisp, 476), STACK(base, 3));	/* NOT_IN_RANGE */
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Fgt(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		LOAD_SMSTR((CL_FORM *)&Kcheck_seq_start_end[0], STACK(base, 3));	/* The START value ~S is greater than the END value ~S */
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Ferror(STACK(base, 3), 3);
	}
	COPY(STACK(base, 1), STACK(base, 0));
}
