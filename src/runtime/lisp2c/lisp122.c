#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cunparse_unix_file = {unparse_unix_file, 1};

CL_INIT2(Kunparse_unix_file)
{
	MAKE_STRING(42, "Cannot specify the type without a file: ~S"),	/* 0 */
	MAKE_STRING(1, "."),	/* 2 */
	MAKE_STRING(45, "Cannot specify the version without a type: ~S"),	/* 4 */
	MAKE_STRING(2, ".*"),	/* 6 */
	MAKE_STRING(3, ".~D"),	/* 8 */
};

void unparse_unix_file(CL_FORM *base)
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
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_FIXNUM(3, STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 4));	/* PATHNAME */
	struct_ref(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_FIXNUM(4, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
	struct_ref(STACK(base, 3));
	if(NOT(CL_TRUEP(STACK(base, 3))))
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
		bool_result = CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 197);	/* UNSPECIFIC */
	}
	if(NOT(bool_result))
	{
		LOAD_T(STACK(base, 4));
	}
	else
	{
		LOAD_NIL(STACK(base, 4));
	}
	COPY(STACK(base, 0), STACK(base, 5));
	LOAD_FIXNUM(5, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
	struct_ref(STACK(base, 5));
	if(NOT(CL_TRUEP(STACK(base, 5))))
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
		bool_result = CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 200);	/* NEWEST */
	}
	if(NOT(bool_result))
	{
		LOAD_T(STACK(base, 6));
	}
	else
	{
		LOAD_NIL(STACK(base, 6));
	}
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 2), STACK(base, 7));
		unparse_unix_piece(STACK(base, 7));
		COPY(STACK(base, 1), STACK(base, 8));
		add_q(STACK(base, 7));
	}
	if(CL_TRUEP(STACK(base, 4)))
	{
		if(CL_TRUEP(STACK(base, 2)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_file[0], STACK(base, 7));	/* Cannot specify the type without a file: ~S */
			COPY(STACK(base, 0), STACK(base, 8));
			Ferror(STACK(base, 7), 2);
		}
		LOAD_SMSTR((CL_FORM *)&Kunparse_unix_file[2], STACK(base, 7));	/* . */
		COPY(STACK(base, 1), STACK(base, 8));
		add_q(STACK(base, 7));
		COPY(STACK(base, 3), STACK(base, 7));
		unparse_unix_piece(STACK(base, 7));
		COPY(STACK(base, 1), STACK(base, 8));
		add_q(STACK(base, 7));
	}
	if(CL_TRUEP(STACK(base, 6)))
	{
		if(CL_TRUEP(STACK(base, 4)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_file[4], STACK(base, 7));	/* Cannot specify the version without a type: ~S */
			COPY(STACK(base, 0), STACK(base, 8));
			Ferror(STACK(base, 7), 2);
		}
		if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 202))	/* WILD */
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_file[6], STACK(base, 7));	/* .* */
		}
		else
		{
			LOAD_NIL(STACK(base, 7));
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_file[8], STACK(base, 8));	/* .~D */
			COPY(STACK(base, 5), STACK(base, 9));
			Fformat(STACK(base, 7), 3);
			mv_count = 1;
		}
		COPY(STACK(base, 1), STACK(base, 8));
		add_q(STACK(base, 7));
	}
	LOAD_GLOBFUN(&CFconcatenate, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 3));	/* SIMPLE-STRING */
	COPY(STACK(base, 1), STACK(base, 4));
	Fcar(STACK(base, 4));
	Fapply(STACK(base, 2), 3);
	COPY(STACK(base, 2), STACK(base, 0));
}
