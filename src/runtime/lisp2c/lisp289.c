#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z91_read_rest(CL_FORM *base, CL_FORM *display[]);

void Fread_delimited_list(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	switch(nargs)
	{
		case 1:
		LOAD_NIL(STACK(base, 1));
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 1)))
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Ssys, 1))	/* T */
		{
			COPY(SYMVAL(Slisp, 58), STACK(base, 1));	/* *TERMINAL-IO* */
		}
	}
	else
	{
		COPY(SYMVAL(Slisp, 59), STACK(base, 1));	/* *STANDARD-INPUT* */
	}
	display[0] = STACK(base, 0);
	Z91_read_rest(STACK(base, 3), display);
	COPY(STACK(base, 3), STACK(base, 0));
}

static void Z91_read_rest(CL_FORM *base, CL_FORM *display[])
{
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	COPY(&display[0][1], STACK(base, 1));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 2));	/* T */
	LOAD_NIL(STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
	Fpeek_char(STACK(base, 0), 5);
	if(EQ(&display[0][0], STACK(base, 0)) || CL_FLOATP(&display[0][0]) && GET_FLOAT(&display[0][0]) == GET_FLOAT(STACK(base, 0)))
	{
		COPY(&display[0][1], STACK(base, 0));
		Fread_char(STACK(base, 0), 1);
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(&display[0][1], STACK(base, 0));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
		LOAD_NIL(STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
		Fread(STACK(base, 0), 4);
		Z91_read_rest(STACK(base, 1), display);
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 2), 2);
			COPY(STACK(base, 0), CAR(lptr));
			COPY(STACK(base, 1), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
