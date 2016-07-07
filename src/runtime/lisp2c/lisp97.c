#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z62_add(CL_FORM *base, CL_FORM *display[]);

void merge_directories(CL_FORM *base)
{
	CL_FORM *display[1];
	COPY(STACK(base, 0), STACK(base, 3));
	Fcar(STACK(base, 3));
	if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 198))	/* ABSOLUTE */
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
		bool_result = NOT(CL_TRUEP(STACK(base, 1)));
	}
	if(bool_result)
	{
	}
	else
	{
		LOAD_NIL(STACK(base, 3));
		LOAD_NIL(STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		COPY(STACK(base, 2), STACK(base, 6));
		maybe_diddle_case(STACK(base, 5));
		mv_count = 1;
		M19_1:;
		if(CL_ATOMP(STACK(base, 5)))
		{
			LOAD_NIL(STACK(base, 4));
			goto RETURN20;
		}
		COPY(STACK(base, 5), STACK(base, 6));
		Fcar(STACK(base, 6));
		COPY(STACK(base, 6), STACK(base, 4));
		display[0] = STACK(base, 0);
		Z62_add(STACK(base, 6), display);
		COPY(STACK(base, 5), STACK(base, 6));
		Fcdr(STACK(base, 6));
		COPY(STACK(base, 6), STACK(base, 5));
		goto M19_1;
		RETURN20:;
		LOAD_NIL(STACK(base, 4));
		COPY(STACK(base, 0), STACK(base, 5));
		Fcdr(STACK(base, 5));
		M20_1:;
		if(CL_ATOMP(STACK(base, 5)))
		{
			LOAD_NIL(STACK(base, 4));
			goto RETURN21;
		}
		COPY(STACK(base, 5), STACK(base, 6));
		Fcar(STACK(base, 6));
		COPY(STACK(base, 6), STACK(base, 4));
		display[0] = STACK(base, 0);
		Z62_add(STACK(base, 6), display);
		COPY(STACK(base, 5), STACK(base, 6));
		Fcdr(STACK(base, 6));
		COPY(STACK(base, 6), STACK(base, 5));
		goto M20_1;
		RETURN21:;
		COPY(STACK(base, 3), STACK(base, 4));
		Freverse(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 0));
	}
}

static void Z62_add(CL_FORM *base, CL_FORM *display[])
{
	if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 199))	/* BACK */
	{
		if(CL_TRUEP(&display[0][3]))
		{
			COPY(&display[0][3], STACK(base, 1));
			Fcar(STACK(base, 1));
			bool_result = NOT(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 199));	/* BACK */
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
		COPY(&display[0][3], STACK(base, 1));
		Fcar(STACK(base, 1));
		COPY(&display[0][3], STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 3));
		Fcdr(STACK(base, 3));
		COPY(STACK(base, 3), &display[0][3]);
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 1), 2);
		COPY(STACK(base, 0), CAR(lptr));
		COPY(&display[0][3], CDR(lptr));
		LOAD_CONS(lptr, &display[0][3]);
		COPY(&display[0][3], STACK(base, 0));
	}
}
