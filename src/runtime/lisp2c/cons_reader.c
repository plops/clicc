#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Ccons_reader = {cons_reader, 2};

CL_INIT2(Kcons_reader)
{
	MAKE_STRING(40, "Nothing appears before the dot in a list"),	/* 0 */
	MAKE_STRING(39, "Nothing appears after the dot in a list"),	/* 2 */
	MAKE_STRING(46, "More than one object found after dot in a list"),	/* 4 */
};

void cons_reader(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	BIND_SPECIAL(SYMBOL(Slisp, 379), STACK(base, 5));	/* *DOT-FLAG* */
	LOAD_CHAR(')', STACK(base, 6));
	BIND_SPECIAL(SYMBOL(Slisp, 380), STACK(base, 6));	/* *PARENTHESIS-OPEN* */
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
	LOAD_NIL(STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 11));	/* T */
	internal_read(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 4));
	bool_result = CL_TRUEP(SYMVAL(Slisp, 380));	/* *PARENTHESIS-OPEN* */
	if(bool_result)
	{
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
		RESTORE_SPECIAL;
		RESTORE_SPECIAL;
		goto RETURN102;
	}
	bool_result = CL_TRUEP(SYMVAL(Slisp, 379));	/* *DOT-FLAG* */
	if(bool_result)
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&Kcons_reader[0], STACK(base, 7));	/* Nothing appears before the dot in a list */
		Ferror(STACK(base, 7), 1);
	}
	LOAD_NIL(STACK(base, 7));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 8), 2);
		COPY(STACK(base, 4), CAR(lptr));
		COPY(STACK(base, 7), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 3));
	}
	COPY(STACK(base, 3), STACK(base, 2));
	M93_1:;
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
	LOAD_NIL(STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 11));	/* T */
	internal_read(STACK(base, 8));
	COPY(STACK(base, 8), STACK(base, 4));
	bool_result = CL_TRUEP(SYMVAL(Slisp, 380));	/* *PARENTHESIS-OPEN* */
	if(bool_result)
	{
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN103;
	}
	bool_result = CL_TRUEP(SYMVAL(Slisp, 379));	/* *DOT-FLAG* */
	if(bool_result)
	{
		COPY(STACK(base, 3), STACK(base, 7));
		LOAD_NIL(STACK(base, 8));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 9), 2);
			COPY(STACK(base, 4), CAR(lptr));
			COPY(STACK(base, 8), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 8));
		}
		COPY(STACK(base, 8), STACK(base, 9));
		COPY(STACK(base, 7), STACK(base, 10));
		COPY(STACK(base, 9), CDR(GET_FORM(STACK(base, 10))));
		COPY(STACK(base, 9), STACK(base, 3));
	}
	else
	{
		COPY(STACK(base, 3), STACK(base, 7));
		COPY(STACK(base, 0), STACK(base, 8));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
		LOAD_NIL(STACK(base, 10));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 11));	/* T */
		internal_read(STACK(base, 8));
		COPY(STACK(base, 8), CDR(GET_FORM(STACK(base, 3))));
		bool_result = CL_TRUEP(SYMVAL(Slisp, 380));	/* *PARENTHESIS-OPEN* */
		if(bool_result)
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kcons_reader[2], STACK(base, 7));	/* Nothing appears after the dot in a list */
			Ferror(STACK(base, 7), 1);
		}
		COPY(STACK(base, 0), STACK(base, 7));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 8));	/* T */
		LOAD_NIL(STACK(base, 9));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 10));	/* T */
		internal_read(STACK(base, 7));
		bool_result = CL_TRUEP(SYMVAL(Slisp, 380));	/* *PARENTHESIS-OPEN* */
		if(bool_result)
		{
			LOAD_SMSTR((CL_FORM *)&Kcons_reader[4], STACK(base, 7));	/* More than one object found after dot in a list */
			Ferror(STACK(base, 7), 1);
		}
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN103;
	}
	goto M93_1;
	RETURN103:;
	RESTORE_SPECIAL;
	RESTORE_SPECIAL;
	RETURN102:;
}
