#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z71_maplist_internal(CL_FORM *base, CL_FORM *display[]);

void Fmaplist(CL_FORM *base, int nargs)
{
	CL_FORM *display[2];
	Flist(STACK(base, 2), nargs - 2);
	display[0] = STACK(base, 0);
	Z71_maplist_internal(STACK(base, 3), display);
	COPY(STACK(base, 3), STACK(base, 0));
}

static void Z72_get_rest_args(CL_FORM *base, CL_FORM *display[]);

static void Z71_maplist_internal(CL_FORM *base, CL_FORM *display[])
{
	CONTENV new_cont;
	CL_FORM *caller_base;
	new_cont.bind_top = bind_top;
	new_cont.last = last_cont;
	LOAD_UNIQUE_TAG(STACK(base, 0));
	caller_base = (CL_FORM *)SETJMP(new_cont.jmp_buf);
	if(caller_base == NULL)
	{
		last_cont = &new_cont;
		if(CL_ATOMP(&display[0][1]))
		{
			LOAD_NIL(STACK(base, 0));
		}
		else
		{
			COPY(&display[0][0], STACK(base, 1));
			COPY(&display[0][1], STACK(base, 2));
			COPY(&display[0][1], STACK(base, 3));
			COPY(STACK(base, 3), STACK(base, 4));
			Fcdr(STACK(base, 4));
			COPY(STACK(base, 4), &display[0][1]);
			COPY(&display[0][2], STACK(base, 3));
			display[1] = STACK(base, 0);
			Z72_get_rest_args(STACK(base, 3), display);
			Fapply(STACK(base, 1), 3);
			mv_count = 1;
			Z71_maplist_internal(STACK(base, 2), display);
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 3), 2);
				COPY(STACK(base, 1), CAR(lptr));
				COPY(STACK(base, 2), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 0));
			}
		}
		RETURN41:;
		last_cont = new_cont.last;
	}
	else
	{
		last_cont = new_cont.last;
		if(EQ(STACK(caller_base, 0), STACK(base, 0)))
		{
			COPY(STACK(caller_base, 1), STACK(base, 0));
		}
		else
		{
			call_cont(caller_base);
		}
	}
}

static void Z72_get_rest_args(CL_FORM *base, CL_FORM *display[])
{
	if(CL_ATOMP(STACK(base, 0)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(CL_ATOMP(STACK(base, 1)))
		{
			COPY(&display[1][0], STACK(base, 1));
			LOAD_NIL(STACK(base, 2));
			call_cont(STACK(base, 1));
		}
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 1));
		Fcdr(STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		Fset_car(STACK(base, 2));
		COPY(&display[0][1], STACK(base, 1));
		COPY(STACK(base, 0), STACK(base, 2));
		Fcdr(STACK(base, 2));
		Z72_get_rest_args(STACK(base, 2), display);
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 3), 2);
			COPY(STACK(base, 1), CAR(lptr));
			COPY(STACK(base, 2), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
