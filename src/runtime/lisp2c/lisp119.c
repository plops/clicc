#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kunparse_unix_piece)
{
	MAKE_STRING(1, "*"),	/* 0 */
	MAKE_STRING(1, "?"),	/* 2 */
	MAKE_STRING(25, "Invalid pattern piece: ~S"),	/* 4 */
	MAKE_STRING(1, "["),	/* 6 */
	MAKE_STRING(1, "]"),	/* 8 */
	MAKE_STRING(25, "Invalid pattern piece: ~S"),	/* 10 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 12 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 14 */
};

void unparse_unix_piece(CL_FORM *base)
{
	if(CL_SMAR_P(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 1));
		LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 2));	/* STANDARD-CHAR */
		LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 3));	/* * */
		check_array_internal(STACK(base, 1));
		bool_result = CL_TRUEP(STACK(base, 1));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Flength(STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 2));
		LOAD_FIXNUM(0, STACK(base, 3));
		M30_1:;
		COPY(STACK(base, 3), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		Fge(STACK(base, 4), 2);
		if(CL_TRUEP(STACK(base, 4)))
		{
			goto RETURN31;
		}
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 5));
		Fschar(STACK(base, 4));
		if(CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '*')
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
			if(CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '?')
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
				bool_result = CL_CHARP(STACK(base, 4)) && GET_CHAR(STACK(base, 4)) == '[';
			}
		}
		if(bool_result)
		{
			COPY(STACK(base, 2), STACK(base, 5));
			F1plus(STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 2));
		}
		F1plus(STACK(base, 3));
		goto M30_1;
		RETURN31:;
		COPY(STACK(base, 2), STACK(base, 3));
		Fmake_string(STACK(base, 3), 1);
		LOAD_FIXNUM(0, STACK(base, 4));
		LOAD_FIXNUM(0, STACK(base, 5));
		M31_1:;
		COPY(STACK(base, 5), STACK(base, 6));
		COPY(STACK(base, 1), STACK(base, 7));
		Fge(STACK(base, 6), 2);
		if(CL_TRUEP(STACK(base, 6)))
		{
			goto RETURN32;
		}
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 7));
		Fschar(STACK(base, 6));
		if(CL_CHARP(STACK(base, 6)) && GET_CHAR(STACK(base, 6)) == '*')
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
			if(CL_CHARP(STACK(base, 6)) && GET_CHAR(STACK(base, 6)) == '?')
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
				bool_result = CL_CHARP(STACK(base, 6)) && GET_CHAR(STACK(base, 6)) == '[';
			}
		}
		if(bool_result)
		{
			LOAD_CHAR('\\', STACK(base, 7));
			COPY(STACK(base, 3), STACK(base, 8));
			COPY(STACK(base, 4), STACK(base, 9));
			Fset_schar(STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 7));
			F1plus(STACK(base, 7));
			COPY(STACK(base, 7), STACK(base, 4));
		}
		COPY(STACK(base, 6), STACK(base, 7));
		COPY(STACK(base, 3), STACK(base, 8));
		COPY(STACK(base, 4), STACK(base, 9));
		Fset_schar(STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 7));
		F1plus(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 4));
		F1plus(STACK(base, 5));
		goto M31_1;
		RETURN32:;
		COPY(STACK(base, 3), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 2));	/* PATTERN */
		struct_typep(STACK(base, 1));
		if(CL_TRUEP(STACK(base, 1)))
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
			LOAD_NIL(STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_FIXNUM(0, STACK(base, 4));
			LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 5));	/* PATTERN */
			struct_ref(STACK(base, 3));
			M32_1:;
			if(CL_ATOMP(STACK(base, 3)))
			{
				LOAD_NIL(STACK(base, 2));
				goto RETURN33;
			}
			COPY(STACK(base, 3), STACK(base, 4));
			Fcar(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 2));
			if(CL_SMAR_P(STACK(base, 2)))
			{
				COPY(STACK(base, 2), STACK(base, 4));
				LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 5));	/* STANDARD-CHAR */
				LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
				check_array_internal(STACK(base, 4));
				bool_result = CL_TRUEP(STACK(base, 4));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 2), STACK(base, 4));
				COPY(STACK(base, 1), STACK(base, 5));
				add_q(STACK(base, 4));
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 2)) || CL_NILP(STACK(base, 2)))
				{
					if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 203))	/* MULTI-CHAR-WILD */
					{
						LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[0], STACK(base, 4));	/* * */
						COPY(STACK(base, 1), STACK(base, 5));
						add_q(STACK(base, 4));
					}
					else
					{
						if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 214))	/* SINGLE-CHAR-WILD */
						{
							LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[2], STACK(base, 4));	/* ? */
							COPY(STACK(base, 1), STACK(base, 5));
							add_q(STACK(base, 4));
						}
						else
						{
							LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[4], STACK(base, 4));	/* Invalid pattern piece: ~S */
							COPY(STACK(base, 2), STACK(base, 5));
							Ferror(STACK(base, 4), 2);
						}
					}
				}
				else
				{
					if(CL_CONSP(STACK(base, 2)))
					{
						COPY(STACK(base, 2), STACK(base, 4));
						Fcar(STACK(base, 4));
						if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 196))	/* CHARACTER-SET */
						{
							LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[6], STACK(base, 5));	/* [ */
							COPY(STACK(base, 1), STACK(base, 6));
							add_q(STACK(base, 5));
							COPY(STACK(base, 2), STACK(base, 5));
							Fcdr(STACK(base, 5));
							COPY(STACK(base, 1), STACK(base, 6));
							add_q(STACK(base, 5));
							LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[8], STACK(base, 5));	/* ] */
							COPY(STACK(base, 1), STACK(base, 6));
							add_q(STACK(base, 5));
						}
						else
						{
							LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[10], STACK(base, 5));	/* Invalid pattern piece: ~S */
							COPY(STACK(base, 2), STACK(base, 6));
							Ferror(STACK(base, 5), 2);
						}
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[12], STACK(base, 4));	/* etypecase: the value ~a is not a legal value */
						COPY(STACK(base, 2), STACK(base, 5));
						Ferror(STACK(base, 4), 2);
					}
				}
			}
			Fcdr(STACK(base, 3));
			goto M32_1;
			RETURN33:;
			LOAD_GLOBFUN(&CFconcatenate, STACK(base, 2));
			LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 3));	/* SIMPLE-STRING */
			COPY(STACK(base, 1), STACK(base, 4));
			Fcar(STACK(base, 4));
			Fapply(STACK(base, 2), 3);
			COPY(STACK(base, 2), STACK(base, 0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_piece[14], STACK(base, 1));	/* etypecase: the value ~a is not a legal value */
			COPY(STACK(base, 0), STACK(base, 2));
			Ferror(STACK(base, 1), 2);
		}
	}
}
