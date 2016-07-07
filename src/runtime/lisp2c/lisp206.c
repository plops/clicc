#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void check_array_internal(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 48))	/* * */
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
		COPY(STACK(base, 0), STACK(base, 4));
		Farray_element_type(STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Fupgraded_array_element_type(STACK(base, 5));
		Fequal(STACK(base, 4));
		bool_result = CL_TRUEP(STACK(base, 4));
	}
	if(bool_result)
	{
		if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 48))	/* * */
		{
			LOAD_T(STACK(base, 3));
		}
		else
		{
			LOAD_NIL(STACK(base, 3));
		}
		if(CL_TRUEP(STACK(base, 3)))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
		}
		else
		{
			if(CL_ATOMP(STACK(base, 2)))
			{
				COPY(STACK(base, 0), STACK(base, 4));
				Farray_rank(STACK(base, 4));
				COPY(STACK(base, 2), STACK(base, 5));
				Fnumeql(STACK(base, 4), 2);
				COPY(STACK(base, 4), STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 4));
				Farray_rank(STACK(base, 4));
				COPY(STACK(base, 2), STACK(base, 5));
				Flength(STACK(base, 5));
				Fnumeql(STACK(base, 4), 2);
				if(CL_TRUEP(STACK(base, 4)))
				{
					LOAD_NIL(STACK(base, 4));
					COPY(STACK(base, 2), STACK(base, 5));
					Flength(STACK(base, 5));
					LOAD_FIXNUM(0, STACK(base, 6));
					M75_1:;
					COPY(STACK(base, 6), STACK(base, 7));
					COPY(STACK(base, 5), STACK(base, 8));
					Fge(STACK(base, 7), 2);
					if(CL_TRUEP(STACK(base, 7)))
					{
						LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
						goto RETURN84;
					}
					COPY(STACK(base, 2), STACK(base, 7));
					Fcar(STACK(base, 7));
					COPY(STACK(base, 2), STACK(base, 8));
					COPY(STACK(base, 8), STACK(base, 9));
					Fcdr(STACK(base, 9));
					COPY(STACK(base, 9), STACK(base, 2));
					COPY(STACK(base, 7), STACK(base, 4));
					if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 48))	/* * */
					{
						LOAD_T(STACK(base, 7));
					}
					else
					{
						LOAD_NIL(STACK(base, 7));
					}
					if(CL_TRUEP(STACK(base, 7)))
					{
						bool_result = TRUE;
					}
					else
					{
						COPY(STACK(base, 0), STACK(base, 8));
						COPY(STACK(base, 6), STACK(base, 9));
						Farray_dimension(STACK(base, 8));
						COPY(STACK(base, 4), STACK(base, 9));
						Fnumeql(STACK(base, 8), 2);
						bool_result = CL_TRUEP(STACK(base, 8));
					}
					if(bool_result)
					{
					}
					else
					{
						LOAD_NIL(STACK(base, 0));
						goto RETURN84;
					}
					F1plus(STACK(base, 6));
					goto M75_1;
					RETURN84:;
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
				}
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
