#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void compare_component(CL_FORM *base)
{
	M14_1:;
	if(EQ(STACK(base, 0), STACK(base, 1)) || CL_FLOATP(STACK(base, 0)) && GET_FLOAT(STACK(base, 0)) == GET_FLOAT(STACK(base, 1)))
	{
		LOAD_T(STACK(base, 2));
	}
	else
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		if(CL_SMAR_P(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 4));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 5));	/* * */
			check_array_internal(STACK(base, 3));
			bool_result = CL_TRUEP(STACK(base, 3));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			if(CL_SMSTRP(STACK(base, 1)))
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
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 4));	/* PATTERN */
			struct_typep(STACK(base, 3));
			if(CL_TRUEP(STACK(base, 3)))
			{
				COPY(STACK(base, 1), STACK(base, 3));
				LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 4));	/* PATTERN */
				struct_typep(STACK(base, 3));
				if(CL_TRUEP(STACK(base, 3)))
				{
					patternE(STACK(base, 0));
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
				}
			}
			else
			{
				if(CL_CONSP(STACK(base, 0)))
				{
					if(CL_CONSP(STACK(base, 1)))
					{
						COPY(STACK(base, 0), STACK(base, 3));
						Fcar(STACK(base, 3));
						COPY(STACK(base, 1), STACK(base, 4));
						Fcar(STACK(base, 4));
						compare_component(STACK(base, 3));
						if(CL_TRUEP(STACK(base, 3)))
						{
							COPY(STACK(base, 0), STACK(base, 3));
							Fcdr(STACK(base, 3));
							COPY(STACK(base, 1), STACK(base, 4));
							Fcdr(STACK(base, 4));
							COPY(STACK(base, 3), STACK(base, 0));
							COPY(STACK(base, 4), STACK(base, 1));
							goto M14_1;
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
					LOAD_NIL(STACK(base, 0));
				}
			}
		}
	}
	goto RETURN15;
	RETURN15:;
}
