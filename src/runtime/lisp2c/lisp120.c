#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kunparse_unix_directory_list)
{
	MAKE_STRING(1, "/"),	/* 0 */
	MAKE_STRING(40, "ecase: the value ~a is not a legal value"),	/* 2 */
	MAKE_STRING(3, "../"),	/* 4 */
	MAKE_STRING(43, ":BACK cannot be represented in namestrings."),	/* 6 */
	MAKE_STRING(1, "/"),	/* 8 */
	MAKE_STRING(31, "Invalid directory component: ~S"),	/* 10 */
};

void unparse_unix_directory_list(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 1));
	LOAD_NIL(STACK(base, 2));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 3), 2);
		COPY(STACK(base, 1), CAR(lptr));
		COPY(STACK(base, 2), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 1));
	}
	if(CL_TRUEP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 4));
		Fcdr(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 0));
		if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 198))	/* ABSOLUTE */
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_directory_list[0], STACK(base, 3));	/* / */
			COPY(STACK(base, 1), STACK(base, 4));
			add_q(STACK(base, 3));
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 201))	/* RELATIVE */
			{
				LOAD_T(STACK(base, 3));
			}
			else
			{
				LOAD_NIL(STACK(base, 3));
			}
			if(CL_TRUEP(STACK(base, 3)))
			{
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&Kunparse_unix_directory_list[2], STACK(base, 4));	/* ecase: the value ~a is not a legal value */
				COPY(STACK(base, 2), STACK(base, 5));
				Ferror(STACK(base, 4), 2);
			}
		}
		LOAD_NIL(STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		M33_1:;
		if(CL_ATOMP(STACK(base, 3)))
		{
			LOAD_NIL(STACK(base, 2));
			goto RETURN34;
		}
		COPY(STACK(base, 3), STACK(base, 4));
		Fcar(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 2));
		if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 205))	/* UP */
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_directory_list[4], STACK(base, 4));	/* ../ */
			COPY(STACK(base, 1), STACK(base, 5));
			add_q(STACK(base, 4));
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 199))	/* BACK */
			{
				LOAD_SMSTR((CL_FORM *)&Kunparse_unix_directory_list[6], STACK(base, 4));	/* :BACK cannot be represented in namestrings. */
				Ferror(STACK(base, 4), 1);
			}
			else
			{
				if(CL_SMAR_P(STACK(base, 2)))
				{
					COPY(STACK(base, 2), STACK(base, 4));
					LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 5));	/* STANDARD-CHAR */
					LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
					check_array_internal(STACK(base, 4));
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
					COPY(STACK(base, 2), STACK(base, 5));
					LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 6));	/* PATTERN */
					struct_typep(STACK(base, 5));
					bool_result = CL_TRUEP(STACK(base, 5));
				}
				if(bool_result)
				{
					COPY(STACK(base, 2), STACK(base, 4));
					unparse_unix_piece(STACK(base, 4));
					COPY(STACK(base, 1), STACK(base, 5));
					add_q(STACK(base, 4));
					LOAD_SMSTR((CL_FORM *)&Kunparse_unix_directory_list[8], STACK(base, 4));	/* / */
					COPY(STACK(base, 1), STACK(base, 5));
					add_q(STACK(base, 4));
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kunparse_unix_directory_list[10], STACK(base, 4));	/* Invalid directory component: ~S */
					COPY(STACK(base, 2), STACK(base, 5));
					Ferror(STACK(base, 4), 2);
				}
			}
		}
		Fcdr(STACK(base, 3));
		goto M33_1;
		RETURN34:;
	}
	LOAD_GLOBFUN(&CFconcatenate, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 3));	/* SIMPLE-STRING */
	COPY(STACK(base, 1), STACK(base, 4));
	Fcar(STACK(base, 4));
	Fapply(STACK(base, 2), 3);
	COPY(STACK(base, 2), STACK(base, 0));
}
