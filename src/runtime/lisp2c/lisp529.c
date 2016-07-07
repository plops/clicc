#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFFI_lisp_integer)
{
	MAKE_STRING(50, "The evaluated value ~S is not of type c-<integer>."),	/* 0 */
};

void FFI_lisp_integer(CL_FORM *base)
{
	if(CL_C_SHORT_P(STACK(base, 0)))
	{
		LOAD_T(STACK(base, 1));
	}
	else
	{
		LOAD_NIL(STACK(base, 1));
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		bool_result = CL_TRUEP(STACK(base, 1));
	}
	else
	{
		if(CL_C_INT_P(STACK(base, 0)))
		{
			LOAD_T(STACK(base, 2));
		}
		else
		{
			LOAD_NIL(STACK(base, 2));
		}
		if(CL_TRUEP(STACK(base, 2)))
		{
			bool_result = CL_TRUEP(STACK(base, 2));
		}
		else
		{
			if(CL_C_LONG_P(STACK(base, 0)))
			{
				LOAD_T(STACK(base, 3));
			}
			else
			{
				LOAD_NIL(STACK(base, 3));
			}
			if(CL_TRUEP(STACK(base, 3)))
			{
				bool_result = CL_TRUEP(STACK(base, 3));
			}
			else
			{
				if(CL_C_UNSIGNED_SHORT_P(STACK(base, 0)))
				{
					LOAD_T(STACK(base, 4));
				}
				else
				{
					LOAD_NIL(STACK(base, 4));
				}
				if(CL_TRUEP(STACK(base, 4)))
				{
					bool_result = CL_TRUEP(STACK(base, 4));
				}
				else
				{
					if(CL_C_UNSIGNED_INT_P(STACK(base, 0)))
					{
						LOAD_T(STACK(base, 5));
					}
					else
					{
						LOAD_NIL(STACK(base, 5));
					}
					if(CL_TRUEP(STACK(base, 5)))
					{
						bool_result = CL_TRUEP(STACK(base, 5));
					}
					else
					{
						bool_result = CL_C_UNSIGNED_LONG_P(STACK(base, 0));
					}
				}
			}
		}
	}
	if(bool_result)
	{
		make_lisp_integer(STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFFI_lisp_integer[0], STACK(base, 1));	/* The evaluated value ~S is not of type c-<integer>. */
		COPY(STACK(base, 0), STACK(base, 2));
		Flist(STACK(base, 2), 1);
		LOAD_SMSTR((CL_FORM *)&KClisp[20], STACK(base, 3));	/* ~&Error in ~A: ~?~% */
		LOAD_SYMBOL(SYMBOL(Slisp, 606), STACK(base, 4));	/* LISP-INTEGER */
		COPY(STACK(base, 1), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		Ferror(STACK(base, 3), 4);
	}
}
