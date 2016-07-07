#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cunparse_unix_enough = {unparse_unix_enough, 2};

CL_INIT2(Kunparse_unix_enough)
{
	MAKE_STRING(1, "."),	/* 0 */
	MAKE_STRING(2, ".*"),	/* 2 */
	MAKE_STRING(3, ".~D"),	/* 4 */
};

void unparse_unix_enough(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 2), CAR(lptr));
		COPY(STACK(base, 3), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 2));
	}
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_FIXNUM(2, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
	struct_ref(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	LOAD_FIXNUM(2, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 6));	/* PATHNAME */
	struct_ref(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 5));
	Flength(STACK(base, 5));
	COPY(STACK(base, 5), STACK(base, 6));
	LOAD_FIXNUM(1, STACK(base, 7));
	Fgt(STACK(base, 6), 2);
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 3), STACK(base, 6));
		Flength(STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 7));
		Fge(STACK(base, 6), 2);
		if(CL_TRUEP(STACK(base, 6)))
		{
			COPY(STACK(base, 3), STACK(base, 6));
			LOAD_FIXNUM(0, STACK(base, 7));
			COPY(STACK(base, 5), STACK(base, 8));
			Fsubseq(STACK(base, 6), 3);
			COPY(STACK(base, 4), STACK(base, 7));
			compare_component(STACK(base, 6));
			bool_result = CL_TRUEP(STACK(base, 6));
		}
		else
		{
			bool_result = FALSE;
		}
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 201), STACK(base, 6));	/* RELATIVE */
		COPY(STACK(base, 5), STACK(base, 7));
		COPY(STACK(base, 3), STACK(base, 8));
		Fnthcdr(STACK(base, 7));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 8), 2);
			COPY(STACK(base, 6), CAR(lptr));
			COPY(STACK(base, 7), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 6));
		}
	}
	else
	{
		COPY(STACK(base, 3), STACK(base, 6));
		Fcar(STACK(base, 6));
		if(CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 198))	/* ABSOLUTE */
		{
			COPY(STACK(base, 3), STACK(base, 6));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[10], STACK(base, 6));	/* ~S cannot be represented relative to ~S */
			COPY(STACK(base, 0), STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			Ferror(STACK(base, 6), 3);
		}
	}
	COPY(STACK(base, 6), STACK(base, 7));
	unparse_unix_directory_list(STACK(base, 7));
	COPY(STACK(base, 2), STACK(base, 8));
	add_q(STACK(base, 7));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_FIXNUM(5, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 5));	/* PATHNAME */
	struct_ref(STACK(base, 3));
	if(CL_TRUEP(STACK(base, 3)))
	{
		if(NOT(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 200)))	/* NEWEST */
		{
			LOAD_T(STACK(base, 4));
		}
		else
		{
			LOAD_NIL(STACK(base, 4));
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 4));
	}
	COPY(STACK(base, 0), STACK(base, 5));
	LOAD_FIXNUM(4, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 7));	/* PATHNAME */
	struct_ref(STACK(base, 5));
	if(CL_TRUEP(STACK(base, 4)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	}
	else
	{
		if(CL_TRUEP(STACK(base, 5)))
		{
			if(NOT(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 197)))	/* UNSPECIFIC */
			{
				LOAD_T(STACK(base, 6));
			}
			else
			{
				LOAD_NIL(STACK(base, 6));
			}
		}
		else
		{
			LOAD_NIL(STACK(base, 6));
		}
	}
	COPY(STACK(base, 0), STACK(base, 7));
	LOAD_FIXNUM(3, STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 9));	/* PATHNAME */
	struct_ref(STACK(base, 7));
	if(CL_TRUEP(STACK(base, 6)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
	}
	else
	{
		if(CL_TRUEP(STACK(base, 7)))
		{
			COPY(STACK(base, 7), STACK(base, 8));
			COPY(STACK(base, 1), STACK(base, 9));
			LOAD_FIXNUM(3, STACK(base, 10));
			LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 11));	/* PATHNAME */
			struct_ref(STACK(base, 9));
			compare_component(STACK(base, 8));
			if(NOT(CL_TRUEP(STACK(base, 8))))
			{
				LOAD_T(STACK(base, 8));
			}
			else
			{
				LOAD_NIL(STACK(base, 8));
			}
		}
		else
		{
			LOAD_NIL(STACK(base, 8));
		}
	}
	if(CL_TRUEP(STACK(base, 8)))
	{
		if(CL_TRUEP(STACK(base, 7)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[10], STACK(base, 9));	/* ~S cannot be represented relative to ~S */
			COPY(STACK(base, 0), STACK(base, 10));
			COPY(STACK(base, 1), STACK(base, 11));
			Ferror(STACK(base, 9), 3);
		}
		COPY(STACK(base, 7), STACK(base, 9));
		unparse_unix_piece(STACK(base, 9));
		COPY(STACK(base, 2), STACK(base, 10));
		add_q(STACK(base, 9));
	}
	if(CL_TRUEP(STACK(base, 6)))
	{
		if(NOT(CL_TRUEP(STACK(base, 5))))
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
			bool_result = CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 197);	/* UNSPECIFIC */
		}
		if(bool_result)
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[10], STACK(base, 9));	/* ~S cannot be represented relative to ~S */
			COPY(STACK(base, 0), STACK(base, 10));
			COPY(STACK(base, 1), STACK(base, 11));
			Ferror(STACK(base, 9), 3);
		}
		LOAD_SMSTR((CL_FORM *)&Kunparse_unix_enough[0], STACK(base, 9));	/* . */
		COPY(STACK(base, 2), STACK(base, 10));
		add_q(STACK(base, 9));
		COPY(STACK(base, 5), STACK(base, 9));
		unparse_unix_piece(STACK(base, 9));
		COPY(STACK(base, 2), STACK(base, 10));
		add_q(STACK(base, 9));
	}
	if(CL_TRUEP(STACK(base, 4)))
	{
		if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 202))	/* WILD */
		{
			LOAD_SMSTR((CL_FORM *)&Kunparse_unix_enough[2], STACK(base, 9));	/* .* */
			COPY(STACK(base, 2), STACK(base, 10));
			add_q(STACK(base, 9));
		}
		else
		{
			if(CL_FIXNUMP(STACK(base, 3)))
			{
				LOAD_NIL(STACK(base, 9));
				LOAD_SMSTR((CL_FORM *)&Kunparse_unix_enough[4], STACK(base, 10));	/* .~D */
				COPY(STACK(base, 3), STACK(base, 11));
				Fformat(STACK(base, 9), 3);
				mv_count = 1;
				COPY(STACK(base, 2), STACK(base, 10));
				add_q(STACK(base, 9));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KClisp[10], STACK(base, 9));	/* ~S cannot be represented relative to ~S */
				COPY(STACK(base, 0), STACK(base, 10));
				COPY(STACK(base, 1), STACK(base, 11));
				Ferror(STACK(base, 9), 3);
			}
		}
	}
	LOAD_GLOBFUN(&CFconcatenate, STACK(base, 0));
	LOAD_SYMBOL(SYMBOL(Slisp, 40), STACK(base, 1));	/* SIMPLE-STRING */
	Fcar(STACK(base, 2));
	Fapply(STACK(base, 0), 3);
}
