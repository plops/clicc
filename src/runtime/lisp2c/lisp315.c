#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_sequence)
{
	MAKE_STRING(40, "~S is an invalid sequence typespecifier."),	/* 0 */
	MAKE_STRING(40, "~S is an invalid sequence typespecifier."),	/* 2 */
	MAKE_STRING(63, "The size ~S is inconsistent with the specified size ~S in type."),	/* 4 */
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
	M141_1:;
	if(CL_ATOMP(STACK(base, 4)))
	{
		if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 13))	/* ARRAY */
		{
			LOAD_T(STACK(base, 5));
		}
		else
		{
			LOAD_NIL(STACK(base, 5));
		}
		if(CL_TRUEP(STACK(base, 5)))
		{
			bool_result = TRUE;
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 477))	/* SIMPLE-ARRAY */
			{
				LOAD_T(STACK(base, 6));
			}
			else
			{
				LOAD_NIL(STACK(base, 6));
			}
			if(CL_TRUEP(STACK(base, 6)))
			{
				bool_result = TRUE;
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 47))	/* VECTOR */
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
					if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 41))	/* SIMPLE-VECTOR */
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
						if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 44))	/* STRING */
						{
							LOAD_T(STACK(base, 9));
						}
						else
						{
							LOAD_NIL(STACK(base, 9));
						}
						if(CL_TRUEP(STACK(base, 9)))
						{
							bool_result = TRUE;
						}
						else
						{
							bool_result = CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 40);	/* SIMPLE-STRING */
						}
					}
				}
			}
		}
		if(bool_result)
		{
			Flist(STACK(base, 4), 1);
			goto M141_1;
		}
		else
		{
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
		LOAD_NIL(STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 8));
		Fcdr(STACK(base, 8));
		COPY(STACK(base, 4), STACK(base, 9));
		Fcar(STACK(base, 9));
		if(CL_SYMBOLP(STACK(base, 9)) && GET_SYMBOL(STACK(base, 9)) == SYMBOL(Slisp, 44))	/* STRING */
		{
			LOAD_T(STACK(base, 10));
		}
		else
		{
			LOAD_NIL(STACK(base, 10));
		}
		if(CL_TRUEP(STACK(base, 10)))
		{
			bool_result = TRUE;
		}
		else
		{
			bool_result = CL_SYMBOLP(STACK(base, 9)) && GET_SYMBOL(STACK(base, 9)) == SYMBOL(Slisp, 40);	/* SIMPLE-STRING */
		}
		if(bool_result)
		{
			COPY(STACK(base, 4), STACK(base, 10));
			Fcar(STACK(base, 10));
			if(CL_SYMBOLP(STACK(base, 10)) && GET_SYMBOL(STACK(base, 10)) == SYMBOL(Slisp, 44))	/* STRING */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 13), STACK(base, 5));	/* ARRAY */
			}
			else
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 477), STACK(base, 5));	/* SIMPLE-ARRAY */
			}
			LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 6));	/* CHARACTER */
			if(CL_ATOMP(STACK(base, 8)))
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 10));	/* * */
			}
			else
			{
				COPY(STACK(base, 8), STACK(base, 10));
				Fcar(STACK(base, 10));
				COPY(STACK(base, 8), STACK(base, 11));
				COPY(STACK(base, 11), STACK(base, 12));
				Fcdr(STACK(base, 12));
				COPY(STACK(base, 12), STACK(base, 8));
			}
			Flist(STACK(base, 10), 1);
			COPY(STACK(base, 10), STACK(base, 7));
			if(CL_TRUEP(STACK(base, 8)))
			{
				goto M142_1;
			}
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 9)) && GET_SYMBOL(STACK(base, 9)) == SYMBOL(Slisp, 41))	/* SIMPLE-VECTOR */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 477), STACK(base, 5));	/* SIMPLE-ARRAY */
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
				if(CL_ATOMP(STACK(base, 8)))
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 10));	/* * */
				}
				else
				{
					COPY(STACK(base, 8), STACK(base, 10));
					Fcar(STACK(base, 10));
					COPY(STACK(base, 8), STACK(base, 11));
					COPY(STACK(base, 11), STACK(base, 12));
					Fcdr(STACK(base, 12));
					COPY(STACK(base, 12), STACK(base, 8));
				}
				Flist(STACK(base, 10), 1);
				COPY(STACK(base, 10), STACK(base, 7));
				if(CL_TRUEP(STACK(base, 8)))
				{
					goto M142_1;
				}
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 9)) && GET_SYMBOL(STACK(base, 9)) == SYMBOL(Slisp, 47))	/* VECTOR */
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 13), STACK(base, 5));	/* ARRAY */
					if(CL_ATOMP(STACK(base, 8)))
					{
						LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
					}
					else
					{
						COPY(STACK(base, 8), STACK(base, 10));
						Fcar(STACK(base, 10));
						COPY(STACK(base, 8), STACK(base, 11));
						COPY(STACK(base, 11), STACK(base, 12));
						Fcdr(STACK(base, 12));
						COPY(STACK(base, 12), STACK(base, 8));
						COPY(STACK(base, 10), STACK(base, 6));
					}
					if(CL_ATOMP(STACK(base, 8)))
					{
						LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 10));	/* * */
					}
					else
					{
						COPY(STACK(base, 8), STACK(base, 10));
						Fcar(STACK(base, 10));
						COPY(STACK(base, 8), STACK(base, 11));
						COPY(STACK(base, 11), STACK(base, 12));
						Fcdr(STACK(base, 12));
						COPY(STACK(base, 12), STACK(base, 8));
					}
					Flist(STACK(base, 10), 1);
					COPY(STACK(base, 10), STACK(base, 7));
					if(CL_TRUEP(STACK(base, 8)))
					{
						goto M142_1;
					}
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 9)) && GET_SYMBOL(STACK(base, 9)) == SYMBOL(Slisp, 13))	/* ARRAY */
					{
						LOAD_T(STACK(base, 10));
					}
					else
					{
						LOAD_NIL(STACK(base, 10));
					}
					if(CL_TRUEP(STACK(base, 10)))
					{
						bool_result = TRUE;
					}
					else
					{
						bool_result = CL_SYMBOLP(STACK(base, 9)) && GET_SYMBOL(STACK(base, 9)) == SYMBOL(Slisp, 477);	/* SIMPLE-ARRAY */
					}
					if(bool_result)
					{
						COPY(STACK(base, 4), STACK(base, 10));
						Fcar(STACK(base, 10));
						COPY(STACK(base, 10), STACK(base, 5));
						if(CL_ATOMP(STACK(base, 8)))
						{
							LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
						}
						else
						{
							COPY(STACK(base, 8), STACK(base, 10));
							Fcar(STACK(base, 10));
							COPY(STACK(base, 8), STACK(base, 11));
							COPY(STACK(base, 11), STACK(base, 12));
							Fcdr(STACK(base, 12));
							COPY(STACK(base, 12), STACK(base, 8));
							COPY(STACK(base, 10), STACK(base, 6));
						}
						if(CL_ATOMP(STACK(base, 8)))
						{
							LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 7));	/* * */
						}
						else
						{
							COPY(STACK(base, 8), STACK(base, 10));
							Fcar(STACK(base, 10));
							COPY(STACK(base, 8), STACK(base, 11));
							COPY(STACK(base, 11), STACK(base, 12));
							Fcdr(STACK(base, 12));
							COPY(STACK(base, 12), STACK(base, 8));
							COPY(STACK(base, 10), STACK(base, 7));
						}
						if(CL_TRUEP(STACK(base, 8)))
						{
							goto M142_1;
						}
					}
					else
					{
						goto M142_1;
					}
				}
			}
		}
		COPY(STACK(base, 5), STACK(base, 4));
		COPY(STACK(base, 6), STACK(base, 5));
		COPY(STACK(base, 7), STACK(base, 6));
		Flist(STACK(base, 4), 3);
		goto RETURN157;
		M142_1:;
		LOAD_SMSTR((CL_FORM *)&KFmake_sequence[0], STACK(base, 9));	/* ~S is an invalid sequence typespecifier. */
		COPY(STACK(base, 4), STACK(base, 10));
		Ferror(STACK(base, 9), 2);
		LOAD_NIL(STACK(base, 4));
	}
	RETURN157:;
	goto RETURN156;
	RETURN156:;
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
			goto RETURN155;
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
			goto RETURN155;
		}
		else
		{
			goto M143_1;
		}
	}
	M143_1:;
	LOAD_SMSTR((CL_FORM *)&KFmake_sequence[2], STACK(base, 6));	/* ~S is an invalid sequence typespecifier. */
	COPY(STACK(base, 0), STACK(base, 7));
	Ferror(STACK(base, 6), 2);
	M143_2:;
	LOAD_SMSTR((CL_FORM *)&KFmake_sequence[4], STACK(base, 6));	/* The size ~S is inconsistent with the specified size ~S in type. */
	COPY(STACK(base, 1), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Ferror(STACK(base, 6), 3);
	LOAD_NIL(STACK(base, 0));
	RETURN155:;
}
