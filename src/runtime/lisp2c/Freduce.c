#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Freduce(CL_FORM *base, int nargs)
{
	BOOL supl_flags[4];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 229),	/* FROM-END */
		SYMBOL(Slisp, 161),	/* START */
		SYMBOL(Slisp, 162),	/* END */
		SYMBOL(Slisp, 478),	/* INITIAL-VALUE */
	};
	keysort(STACK(base, 2), nargs - 2, 4, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_FIXNUM(0, STACK(base, 3));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_NIL(STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
	}
	else
	{
		LOAD_T(STACK(base, 6));
	}
	COPY(STACK(base, 1), STACK(base, 7));
	Flength(STACK(base, 7));
	COPY(STACK(base, 3), STACK(base, 8));
	COPY(STACK(base, 4), STACK(base, 9));
	COPY(STACK(base, 7), STACK(base, 10));
	check_seq_start_end(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 4));
	COPY(STACK(base, 7), STACK(base, 8));
	Fzerop(STACK(base, 8));
	if(CL_TRUEP(STACK(base, 8)))
	{
		if(CL_TRUEP(STACK(base, 6)))
		{
			COPY(STACK(base, 5), STACK(base, 0));
		}
		else
		{
			Ffuncall(STACK(base, 0), 1);
		}
	}
	else
	{
		COPY(STACK(base, 7), STACK(base, 8));
		LOAD_FIXNUM(1, STACK(base, 9));
		Fnumeql(STACK(base, 8), 2);
		if(CL_TRUEP(STACK(base, 8)))
		{
			bool_result = NOT(CL_TRUEP(STACK(base, 6)));
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			COPY(STACK(base, 1), STACK(base, 0));
			LOAD_FIXNUM(0, STACK(base, 1));
			Felt(STACK(base, 0));
		}
		else
		{
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(STACK(base, 4), STACK(base, 8));
				F1minus(STACK(base, 8));
				COPY(STACK(base, 8), STACK(base, 4));
				if(CL_TRUEP(STACK(base, 6)))
				{
				}
				else
				{
					COPY(STACK(base, 1), STACK(base, 8));
					COPY(STACK(base, 4), STACK(base, 9));
					Felt(STACK(base, 8));
					COPY(STACK(base, 8), STACK(base, 5));
					COPY(STACK(base, 4), STACK(base, 8));
					F1minus(STACK(base, 8));
					COPY(STACK(base, 8), STACK(base, 4));
				}
				COPY(STACK(base, 5), STACK(base, 8));
				M156_1:;
				COPY(STACK(base, 3), STACK(base, 9));
				COPY(STACK(base, 4), STACK(base, 10));
				Fge(STACK(base, 9), 2);
				if(CL_TRUEP(STACK(base, 9)))
				{
					COPY(STACK(base, 8), STACK(base, 0));
					goto RETURN170;
				}
				COPY(STACK(base, 0), STACK(base, 9));
				COPY(STACK(base, 1), STACK(base, 10));
				COPY(STACK(base, 4), STACK(base, 11));
				Felt(STACK(base, 10));
				COPY(STACK(base, 8), STACK(base, 11));
				Ffuncall(STACK(base, 9), 3);
				mv_count = 1;
				COPY(STACK(base, 9), STACK(base, 8));
				COPY(STACK(base, 4), STACK(base, 9));
				F1minus(STACK(base, 9));
				COPY(STACK(base, 9), STACK(base, 4));
				goto M156_1;
				RETURN170:;
			}
			else
			{
				if(CL_TRUEP(STACK(base, 6)))
				{
				}
				else
				{
					COPY(STACK(base, 1), STACK(base, 8));
					COPY(STACK(base, 3), STACK(base, 9));
					Felt(STACK(base, 8));
					COPY(STACK(base, 8), STACK(base, 5));
					COPY(STACK(base, 3), STACK(base, 8));
					F1plus(STACK(base, 8));
					COPY(STACK(base, 8), STACK(base, 3));
				}
				COPY(STACK(base, 5), STACK(base, 8));
				M157_1:;
				COPY(STACK(base, 3), STACK(base, 9));
				COPY(STACK(base, 4), STACK(base, 10));
				Fge(STACK(base, 9), 2);
				if(CL_TRUEP(STACK(base, 9)))
				{
					COPY(STACK(base, 8), STACK(base, 0));
					goto RETURN171;
				}
				COPY(STACK(base, 0), STACK(base, 9));
				COPY(STACK(base, 8), STACK(base, 10));
				COPY(STACK(base, 1), STACK(base, 11));
				COPY(STACK(base, 3), STACK(base, 12));
				Felt(STACK(base, 11));
				Ffuncall(STACK(base, 9), 3);
				mv_count = 1;
				COPY(STACK(base, 9), STACK(base, 8));
				COPY(STACK(base, 3), STACK(base, 9));
				F1plus(STACK(base, 9));
				COPY(STACK(base, 9), STACK(base, 3));
				goto M157_1;
				RETURN171:;
			}
		}
	}
}
