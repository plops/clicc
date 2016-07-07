#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void bq_attach_append(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 3));
	null_or_quoted(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		COPY(STACK(base, 2), STACK(base, 3));
		null_or_quoted(STACK(base, 3));
		bool_result = CL_TRUEP(STACK(base, 3));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(SYMVAL(Slisp, 372), STACK(base, 3));	/* *BQ-QUOTE* */
		COPY(STACK(base, 1), STACK(base, 4));
		Fcdr(STACK(base, 4));
		Fcar(STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fcdr(STACK(base, 5));
		Fcar(STACK(base, 5));
		Fappend(STACK(base, 4), 2);
		Flist(STACK(base, 3), 2);
		COPY(STACK(base, 3), STACK(base, 0));
	}
	else
	{
		if(NOT(CL_TRUEP(STACK(base, 2))))
		{
			LOAD_T(STACK(base, 3));
		}
		else
		{
			LOAD_NIL(STACK(base, 3));
		}
		if(CL_TRUEP(STACK(base, 3)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 4));
			COPY(SYMVAL(Slisp, 373), STACK(base, 5));	/* *BQ-QUOTE-NIL* */
			Fequal(STACK(base, 4));
			bool_result = CL_TRUEP(STACK(base, 4));
		}
		if(bool_result)
		{
			COPY(STACK(base, 1), STACK(base, 3));
			bq_splicing_frob(STACK(base, 3));
			if(CL_TRUEP(STACK(base, 3)))
			{
				Flist(STACK(base, 0), 2);
			}
			else
			{
				COPY(STACK(base, 1), STACK(base, 0));
			}
		}
		else
		{
			if(CL_CONSP(STACK(base, 2)))
			{
				COPY(STACK(base, 2), STACK(base, 3));
				Fcar(STACK(base, 3));
				bool_result = EQ(STACK(base, 3), STACK(base, 0));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 2), STACK(base, 3));
				Fcar(STACK(base, 3));
				COPY(STACK(base, 1), STACK(base, 4));
				COPY(STACK(base, 2), STACK(base, 5));
				Fcdr(STACK(base, 5));
				FlistX(STACK(base, 3), 3);
				COPY(STACK(base, 3), STACK(base, 0));
			}
			else
			{
				Flist(STACK(base, 0), 3);
			}
		}
	}
}
