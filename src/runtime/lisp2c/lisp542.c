#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Knormalize_type)
{
	MAKE_STRING(40, "~S is an invalid sequence typespecifier."),	/* 0 */
};

void normalize_type(CL_FORM *base)
{
	M243_1:;
	if(CL_ATOMP(STACK(base, 0)))
	{
		if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 13))	/* ARRAY */
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
			if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 477))	/* SIMPLE-ARRAY */
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
				if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 47))	/* VECTOR */
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
					if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 41))	/* SIMPLE-VECTOR */
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
						if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 44))	/* STRING */
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
							bool_result = CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 40);	/* SIMPLE-STRING */
						}
					}
				}
			}
		}
		if(bool_result)
		{
			Flist(STACK(base, 0), 1);
			goto M243_1;
		}
		else
		{
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 1));
		LOAD_NIL(STACK(base, 2));
		LOAD_NIL(STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Fcdr(STACK(base, 4));
		COPY(STACK(base, 0), STACK(base, 5));
		Fcar(STACK(base, 5));
		if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 44))	/* STRING */
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
			bool_result = CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 40);	/* SIMPLE-STRING */
		}
		if(bool_result)
		{
			COPY(STACK(base, 0), STACK(base, 6));
			Fcar(STACK(base, 6));
			if(CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 44))	/* STRING */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 13), STACK(base, 1));	/* ARRAY */
			}
			else
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 477), STACK(base, 1));	/* SIMPLE-ARRAY */
			}
			LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 2));	/* CHARACTER */
			if(CL_ATOMP(STACK(base, 4)))
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
			}
			else
			{
				COPY(STACK(base, 4), STACK(base, 6));
				Fcar(STACK(base, 6));
				COPY(STACK(base, 4), STACK(base, 7));
				COPY(STACK(base, 7), STACK(base, 8));
				Fcdr(STACK(base, 8));
				COPY(STACK(base, 8), STACK(base, 4));
			}
			Flist(STACK(base, 6), 1);
			COPY(STACK(base, 6), STACK(base, 3));
			if(CL_TRUEP(STACK(base, 4)))
			{
				goto M244_1;
			}
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 41))	/* SIMPLE-VECTOR */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 477), STACK(base, 1));	/* SIMPLE-ARRAY */
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
				if(CL_ATOMP(STACK(base, 4)))
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
				}
				else
				{
					COPY(STACK(base, 4), STACK(base, 6));
					Fcar(STACK(base, 6));
					COPY(STACK(base, 4), STACK(base, 7));
					COPY(STACK(base, 7), STACK(base, 8));
					Fcdr(STACK(base, 8));
					COPY(STACK(base, 8), STACK(base, 4));
				}
				Flist(STACK(base, 6), 1);
				COPY(STACK(base, 6), STACK(base, 3));
				if(CL_TRUEP(STACK(base, 4)))
				{
					goto M244_1;
				}
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 47))	/* VECTOR */
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 13), STACK(base, 1));	/* ARRAY */
					if(CL_ATOMP(STACK(base, 4)))
					{
						LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 2));	/* * */
					}
					else
					{
						COPY(STACK(base, 4), STACK(base, 6));
						Fcar(STACK(base, 6));
						COPY(STACK(base, 4), STACK(base, 7));
						COPY(STACK(base, 7), STACK(base, 8));
						Fcdr(STACK(base, 8));
						COPY(STACK(base, 8), STACK(base, 4));
						COPY(STACK(base, 6), STACK(base, 2));
					}
					if(CL_ATOMP(STACK(base, 4)))
					{
						LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
					}
					else
					{
						COPY(STACK(base, 4), STACK(base, 6));
						Fcar(STACK(base, 6));
						COPY(STACK(base, 4), STACK(base, 7));
						COPY(STACK(base, 7), STACK(base, 8));
						Fcdr(STACK(base, 8));
						COPY(STACK(base, 8), STACK(base, 4));
					}
					Flist(STACK(base, 6), 1);
					COPY(STACK(base, 6), STACK(base, 3));
					if(CL_TRUEP(STACK(base, 4)))
					{
						goto M244_1;
					}
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 13))	/* ARRAY */
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
						bool_result = CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 477);	/* SIMPLE-ARRAY */
					}
					if(bool_result)
					{
						COPY(STACK(base, 0), STACK(base, 6));
						Fcar(STACK(base, 6));
						COPY(STACK(base, 6), STACK(base, 1));
						if(CL_ATOMP(STACK(base, 4)))
						{
							LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 2));	/* * */
						}
						else
						{
							COPY(STACK(base, 4), STACK(base, 6));
							Fcar(STACK(base, 6));
							COPY(STACK(base, 4), STACK(base, 7));
							COPY(STACK(base, 7), STACK(base, 8));
							Fcdr(STACK(base, 8));
							COPY(STACK(base, 8), STACK(base, 4));
							COPY(STACK(base, 6), STACK(base, 2));
						}
						if(CL_ATOMP(STACK(base, 4)))
						{
							LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 3));	/* * */
						}
						else
						{
							COPY(STACK(base, 4), STACK(base, 6));
							Fcar(STACK(base, 6));
							COPY(STACK(base, 4), STACK(base, 7));
							COPY(STACK(base, 7), STACK(base, 8));
							Fcdr(STACK(base, 8));
							COPY(STACK(base, 8), STACK(base, 4));
							COPY(STACK(base, 6), STACK(base, 3));
						}
						if(CL_TRUEP(STACK(base, 4)))
						{
							goto M244_1;
						}
					}
					else
					{
						goto M244_1;
					}
				}
			}
		}
		COPY(STACK(base, 1), STACK(base, 0));
		COPY(STACK(base, 2), STACK(base, 1));
		COPY(STACK(base, 3), STACK(base, 2));
		Flist(STACK(base, 0), 3);
		goto RETURN285;
		M244_1:;
		LOAD_SMSTR((CL_FORM *)&Knormalize_type[0], STACK(base, 5));	/* ~S is an invalid sequence typespecifier. */
		COPY(STACK(base, 0), STACK(base, 6));
		Ferror(STACK(base, 5), 2);
		LOAD_NIL(STACK(base, 0));
	}
	RETURN285:;
	goto RETURN284;
	RETURN284:;
}
