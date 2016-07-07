#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFequal = {Fequal, 2};

void Fequal(CL_FORM *base)
{
	M233_1:;
	if(CL_CONSP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 1)))
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 3));
			Fcar(STACK(base, 3));
			Fequal(STACK(base, 2));
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				Fcdr(STACK(base, 2));
				COPY(STACK(base, 1), STACK(base, 3));
				Fcdr(STACK(base, 3));
				COPY(STACK(base, 2), STACK(base, 0));
				COPY(STACK(base, 3), STACK(base, 1));
				goto M233_1;
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
	else
	{
		if(CL_ARRAY_P(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 2));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
			check_array_internal(STACK(base, 2));
			bool_result = CL_TRUEP(STACK(base, 2));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			if(CL_STRING_P(STACK(base, 1)))
			{
				FstringE(STACK(base, 0), 2);
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
		else
		{
			if(EQ(STACK(base, 0), STACK(base, 1)) || CL_FLOATP(STACK(base, 0)) && GET_FLOAT(STACK(base, 0)) == GET_FLOAT(STACK(base, 1)))
			{
				LOAD_T(STACK(base, 0));
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
	}
	goto RETURN273;
	RETURN273:;
}
