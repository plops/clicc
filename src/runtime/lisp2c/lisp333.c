#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_sequence)
{
	MAKE_STRING(40, "~S is an invalid sequence typespecifier."),	/* 0 */
	MAKE_STRING(63, "The size ~S is inconsistent with the specified size ~S in type."),	/* 2 */
};

void Fmake_sequence(CL_FORM *base, int nargs)
{
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 123),	/* INITIAL-ELEMENT */
	};
	keysort(STACK(base, 2), nargs - 2, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
		LOAD_NIL(STACK(base, 3));
	}
	else
	{
		LOAD_T(STACK(base, 3));
	}
	COPY(STACK(base, 0), STACK(base, 4));
	normalize_type(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	if(CL_ATOMP(STACK(base, 4)))
	{
		if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 28))	/* LIST */
		{
			if(CL_TRUEP(STACK(base, 3)))
			{
				COPY(STACK(base, 1), STACK(base, 0));
				LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 1));	/* INITIAL-ELEMENT */
				Fmake_list(STACK(base, 0), 3);
			}
			else
			{
				COPY(STACK(base, 1), STACK(base, 0));
				Fmake_list(STACK(base, 0), 1);
			}
			goto RETURN157;
		}
		else
		{
			goto M143_1;
		}
	}
	else
	{
		COPY(STACK(base, 4), STACK(base, 6));
		Fcar(STACK(base, 6));
		if(CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 477))	/* SIMPLE-ARRAY */
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
			bool_result = CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 13);	/* ARRAY */
		}
		if(bool_result)
		{
			COPY(STACK(base, 4), STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 7));
			Fcdr(STACK(base, 7));
			Fcar(STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 8));
			Fcdr(STACK(base, 8));
			Fcdr(STACK(base, 8));
			Fcar(STACK(base, 8));
			COPY(STACK(base, 8), STACK(base, 9));
			Fcdr(STACK(base, 9));
			if(CL_TRUEP(STACK(base, 9)))
			{
				goto M143_1;
			}
			COPY(STACK(base, 8), STACK(base, 9));
			COPY(STACK(base, 9), STACK(base, 10));
			Fcar(STACK(base, 10));
			COPY(STACK(base, 10), STACK(base, 5));
			if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 48))	/* * */
			{
				bool_result = FALSE;
			}
			else
			{
				COPY(STACK(base, 1), STACK(base, 9));
				COPY(STACK(base, 5), STACK(base, 10));
				Fnumneql(STACK(base, 9), 2);
				bool_result = CL_TRUEP(STACK(base, 9));
			}
			if(bool_result)
			{
				goto M143_2;
			}
			if(CL_SYMBOLP(STACK(base, 7)) && GET_SYMBOL(STACK(base, 7)) == SYMBOL(Slisp, 18))	/* CHARACTER */
			{
				if(CL_TRUEP(STACK(base, 3)))
				{
					COPY(STACK(base, 1), STACK(base, 0));
					LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 1));	/* INITIAL-ELEMENT */
					Fmake_string(STACK(base, 0), 3);
				}
				else
				{
					COPY(STACK(base, 1), STACK(base, 0));
					Fmake_string(STACK(base, 0), 1);
				}
			}
			else
			{
				LOAD_GLOBFUN(&CFmake_array, STACK(base, 9));
				COPY(STACK(base, 1), STACK(base, 10));
				if(CL_TRUEP(STACK(base, 3)))
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 11));	/* INITIAL-ELEMENT */
					COPY(STACK(base, 2), STACK(base, 12));
					Flist(STACK(base, 11), 2);
				}
				else
				{
					LOAD_NIL(STACK(base, 11));
				}
				if(CL_SYMBOLP(STACK(base, 7)) && GET_SYMBOL(STACK(base, 7)) == SYMBOL(Slisp, 48))	/* * */
				{
					LOAD_NIL(STACK(base, 12));
				}
				else
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 12));	/* ELEMENT-TYPE */
					COPY(STACK(base, 7), STACK(base, 13));
					Flist(STACK(base, 12), 2);
				}
				Fappend(STACK(base, 11), 2);
				Fapply(STACK(base, 9), 3);
				COPY(STACK(base, 9), STACK(base, 0));
			}
			goto RETURN157;
		}
		else
		{
			goto M143_1;
		}
	}
	M143_1:;
	LOAD_SMSTR((CL_FORM *)&KFmake_sequence[0], STACK(base, 6));	/* ~S is an invalid sequence typespecifier. */
	COPY(STACK(base, 0), STACK(base, 7));
	Ferror(STACK(base, 6), 2);
	M143_2:;
	LOAD_SMSTR((CL_FORM *)&KFmake_sequence[2], STACK(base, 6));	/* The size ~S is inconsistent with the specified size ~S in type. */
	COPY(STACK(base, 1), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Ferror(STACK(base, 6), 3);
	LOAD_NIL(STACK(base, 0));
	RETURN157:;
}
