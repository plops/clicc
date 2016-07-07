#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fupgraded_array_element_type(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 23))	/* FIXNUM */
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 23), STACK(base, 0));	/* FIXNUM */
	}
	else
	{
		if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 43))	/* STANDARD-CHAR */
		{
			LOAD_T(STACK(base, 1));
		}
		else
		{
			LOAD_NIL(STACK(base, 1));
		}
		if(CL_TRUEP(STACK(base, 1)))
		{
			bool_result = TRUE;
		}
		else
		{
			bool_result = CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 18);	/* CHARACTER */
		}
		if(bool_result)
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 0));	/* CHARACTER */
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 24))	/* FLOAT */
			{
				LOAD_T(STACK(base, 1));
			}
			else
			{
				LOAD_NIL(STACK(base, 1));
			}
			if(CL_TRUEP(STACK(base, 1)))
			{
				bool_result = TRUE;
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 37))	/* SHORT-FLOAT */
				{
					LOAD_T(STACK(base, 2));
				}
				else
				{
					LOAD_NIL(STACK(base, 2));
				}
				if(CL_TRUEP(STACK(base, 2)))
				{
					bool_result = TRUE;
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 42))	/* SINGLE-FLOAT */
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
						if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 22))	/* DOUBLE-FLOAT */
						{
							LOAD_T(STACK(base, 4));
						}
						else
						{
							LOAD_NIL(STACK(base, 4));
						}
						if(CL_TRUEP(STACK(base, 4)))
						{
							bool_result = TRUE;
						}
						else
						{
							bool_result = CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 29);	/* LONG-FLOAT */
						}
					}
				}
			}
			if(bool_result)
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 24), STACK(base, 0));	/* FLOAT */
			}
			else
			{
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
			}
		}
	}
}
