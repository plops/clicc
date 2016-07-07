#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z58_g284(CL_FORM *base);
static GLOBAL_FUNARG CZ58_g284 = {Z58_g284, 2};

void patternE(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 4));	/* PATTERN */
	struct_ref(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 5));	/* PATTERN */
	struct_ref(STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	Flength(STACK(base, 4));
	COPY(STACK(base, 3), STACK(base, 5));
	Flength(STACK(base, 5));
	Fnumeql(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_GLOBFUN(&CZ58_g284, STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		COPY(STACK(base, 3), STACK(base, 6));
		Fevery(STACK(base, 4), 3);
		COPY(STACK(base, 4), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}

static void Z58_g284(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
	LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
	check_simple_array(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
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
		if(CL_CONSP(STACK(base, 0)))
		{
			if(CL_CONSP(STACK(base, 1)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				Fcar(STACK(base, 2));
				COPY(STACK(base, 1), STACK(base, 3));
				Fcar(STACK(base, 3));
				if(EQ(STACK(base, 2), STACK(base, 3)))
				{
					COPY(STACK(base, 0), STACK(base, 2));
					Fcdr(STACK(base, 2));
					COPY(STACK(base, 1), STACK(base, 3));
					Fcdr(STACK(base, 3));
					FstringE(STACK(base, 2), 2);
					COPY(STACK(base, 2), STACK(base, 0));
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
			if(EQ(STACK(base, 0), STACK(base, 1)))
			{
				LOAD_T(STACK(base, 0));
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
	}
}
