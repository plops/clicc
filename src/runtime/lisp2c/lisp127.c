#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KPenumerate_files)
{
	MAKE_STRING(1, "."),	/* 0 */
	MAKE_STRING(1, "."),	/* 2 */
};

void Penumerate_files(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 4));
	Fpathname_name(STACK(base, 4), 1);
	mv_count = 1;
	COPY(STACK(base, 1), STACK(base, 5));
	Fpathname_type(STACK(base, 5), 1);
	mv_count = 1;
	COPY(STACK(base, 1), STACK(base, 6));
	Fpathname_version(STACK(base, 6));
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 7));	/* STRING */
		COPY(STACK(base, 0), STACK(base, 8));
		COPY(STACK(base, 4), STACK(base, 9));
		Fconcatenate(STACK(base, 7), 3);
		if(NOT(CL_TRUEP(STACK(base, 5))))
		{
			LOAD_T(STACK(base, 8));
		}
		else
		{
			LOAD_NIL(STACK(base, 8));
		}
		if(CL_TRUEP(STACK(base, 8)))
		{
			bool_result = TRUE;
		}
		else
		{
			bool_result = CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 197);	/* UNSPECIFIC */
		}
		if(bool_result)
		{
		}
		else
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 8));	/* STRING */
			COPY(STACK(base, 7), STACK(base, 9));
			LOAD_SMSTR((CL_FORM *)&KPenumerate_files[0], STACK(base, 10));	/* . */
			COPY(STACK(base, 5), STACK(base, 11));
			Fconcatenate(STACK(base, 8), 4);
			COPY(STACK(base, 8), STACK(base, 7));
		}
		if(NOT(CL_TRUEP(STACK(base, 6))))
		{
			LOAD_T(STACK(base, 8));
		}
		else
		{
			LOAD_NIL(STACK(base, 8));
		}
		if(CL_TRUEP(STACK(base, 8)))
		{
			bool_result = TRUE;
		}
		else
		{
			bool_result = CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 200);	/* NEWEST */
		}
		if(bool_result)
		{
		}
		else
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 8));	/* STRING */
			COPY(STACK(base, 7), STACK(base, 9));
			LOAD_SMSTR((CL_FORM *)&KPenumerate_files[2], STACK(base, 10));	/* . */
			COPY(STACK(base, 6), STACK(base, 11));
			quick_integer_to_string(STACK(base, 11));
			Fconcatenate(STACK(base, 8), 4);
			COPY(STACK(base, 8), STACK(base, 7));
		}
		COPY(STACK(base, 7), STACK(base, 8));
		unix_file_kind(STACK(base, 8), 1);
		if(CL_TRUEP(STACK(base, 8)))
		{
			COPY(STACK(base, 3), STACK(base, 0));
			COPY(STACK(base, 7), STACK(base, 1));
			Ffuncall(STACK(base, 0), 2);
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 7));
		unix_file_kind(STACK(base, 7), 1);
		if(CL_TRUEP(STACK(base, 7)))
		{
			COPY(STACK(base, 3), STACK(base, 7));
			COPY(STACK(base, 0), STACK(base, 8));
			Ffuncall(STACK(base, 7), 2);
			COPY(STACK(base, 7), STACK(base, 0));
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
}
