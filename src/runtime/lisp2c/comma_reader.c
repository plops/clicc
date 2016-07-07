#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Ccomma_reader = {comma_reader, 2};

CL_INIT2(Kcomma_reader)
{
	MAKE_STRING(39, "A comma appeared outside of a backquote"),	/* 0 */
};

void comma_reader(CL_FORM *base)
{
	COPY(SYMVAL(Slisp, 375), STACK(base, 2));	/* *BQ-LEVEL* */
	LOAD_FIXNUM(0, STACK(base, 3));
	Fle(STACK(base, 2), 2);
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&Kcomma_reader[0], STACK(base, 2));	/* A comma appeared outside of a backquote */
		Ferror(STACK(base, 2), 1);
	}
	COPY(SYMVAL(Slisp, 375), STACK(base, 2));	/* *BQ-LEVEL* */
	F1minus(STACK(base, 2));
	COPY(STACK(base, 2), SYMVAL(Slisp, 375));	/* *BQ-LEVEL* */
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	LOAD_NIL(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	Fpeek_char(STACK(base, 2), 5);
	if(CL_CHARP(STACK(base, 2)) && GET_CHAR(STACK(base, 2)) == '@')
	{
		COPY(STACK(base, 0), STACK(base, 3));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
		LOAD_NIL(STACK(base, 5));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
		Fread_char(STACK(base, 3), 4);
		COPY(SYMVAL(Slisp, 365), STACK(base, 2));	/* *COMMA-ATSIGN* */
	}
	else
	{
		if(CL_CHARP(STACK(base, 2)) && GET_CHAR(STACK(base, 2)) == '.')
		{
			COPY(STACK(base, 0), STACK(base, 3));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
			LOAD_NIL(STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
			Fread_char(STACK(base, 3), 4);
			COPY(SYMVAL(Slisp, 366), STACK(base, 2));	/* *COMMA-DOT* */
		}
		else
		{
			COPY(SYMVAL(Slisp, 364), STACK(base, 2));	/* *COMMA* */
		}
	}
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	LOAD_NIL(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	internal_read(STACK(base, 3));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 4), 2);
		COPY(STACK(base, 2), CAR(lptr));
		COPY(STACK(base, 3), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 2));
	}
	COPY(SYMVAL(Slisp, 375), STACK(base, 3));	/* *BQ-LEVEL* */
	F1plus(STACK(base, 3));
	COPY(STACK(base, 3), SYMVAL(Slisp, 375));	/* *BQ-LEVEL* */
	COPY(STACK(base, 2), STACK(base, 0));
}
