#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kcomponents_match)
{
	MAKE_SYMREF(SYMBOL(Slisp, 203)),	/* 0 CONS(0) MULTI-CHAR-WILD */
	MAKE_NIL,
};

void components_match(CL_FORM *base)
{
	M23_1:;
	if(EQ(STACK(base, 0), STACK(base, 1)))
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
			COPY(STACK(base, 1), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 4));	/* PATTERN */
			struct_typep(STACK(base, 3));
			if(CL_TRUEP(STACK(base, 3)))
			{
				COPY(STACK(base, 1), STACK(base, 3));
				COPY(STACK(base, 0), STACK(base, 4));
				pattern_matches(STACK(base, 3));
				mv_count = 1;
				COPY(STACK(base, 3), STACK(base, 0));
			}
			else
			{
				if(CL_SMAR_P(STACK(base, 1)))
				{
					COPY(STACK(base, 1), STACK(base, 3));
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
					FstringE(STACK(base, 0), 2);
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
				}
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
						components_match(STACK(base, 3));
						if(CL_TRUEP(STACK(base, 3)))
						{
							COPY(STACK(base, 0), STACK(base, 3));
							Fcdr(STACK(base, 3));
							COPY(STACK(base, 1), STACK(base, 4));
							Fcdr(STACK(base, 4));
							COPY(STACK(base, 3), STACK(base, 0));
							COPY(STACK(base, 4), STACK(base, 1));
							goto M23_1;
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
					if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 199))	/* BACK */
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
						if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 205))	/* UP */
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
							if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 197))	/* UNSPECIFIC */
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
								bool_result = NOT(CL_TRUEP(STACK(base, 0)));
							}
						}
					}
					if(bool_result)
					{
						COPY(STACK(base, 1), STACK(base, 3));
						LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 4));	/* PATTERN */
						struct_typep(STACK(base, 3));
						if(CL_TRUEP(STACK(base, 3)))
						{
							COPY(STACK(base, 1), STACK(base, 0));
							LOAD_FIXNUM(0, STACK(base, 1));
							LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 2));	/* PATTERN */
							struct_ref(STACK(base, 0));
							LOAD_CONS((CL_FORM *)&Kcomponents_match[0], STACK(base, 1));
							Fequal(STACK(base, 0));
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
	}
	goto RETURN24;
	RETURN24:;
}
