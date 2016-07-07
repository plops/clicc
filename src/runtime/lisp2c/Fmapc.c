#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z73_get_rest_args(CL_FORM *base, CL_FORM *display[]);

void Fmapc(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	Flist(STACK(base, 2), nargs - 2);
	{
		CONTENV new_cont;
		CL_FORM *caller_base;
		new_cont.bind_top = bind_top;
		new_cont.last = last_cont;
		LOAD_UNIQUE_TAG(STACK(base, 3));
		caller_base = (CL_FORM *)SETJMP(new_cont.jmp_buf);
		if(caller_base == NULL)
		{
			last_cont = &new_cont;
			LOAD_NIL(STACK(base, 4));
			COPY(STACK(base, 1), STACK(base, 5));
			M38_1:;
			if(CL_ATOMP(STACK(base, 5)))
			{
				LOAD_NIL(STACK(base, 4));
				goto RETURN42;
			}
			COPY(STACK(base, 5), STACK(base, 6));
			Fcar(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 4));
			COPY(STACK(base, 0), STACK(base, 6));
			COPY(STACK(base, 4), STACK(base, 7));
			COPY(STACK(base, 2), STACK(base, 8));
			display[0] = STACK(base, 0);
			Z73_get_rest_args(STACK(base, 8), display);
			Fapply(STACK(base, 6), 3);
			mv_count = 1;
			COPY(STACK(base, 5), STACK(base, 6));
			Fcdr(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 5));
			goto M38_1;
			RETURN42:;
			last_cont = new_cont.last;
		}
		else
		{
			last_cont = new_cont.last;
			if(EQ(STACK(caller_base, 0), STACK(base, 3)))
			{
			}
			else
			{
				call_cont(caller_base);
			}
		}
	}
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z73_get_rest_args(CL_FORM *base, CL_FORM *display[])
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
			COPY(&display[0][3], STACK(base, 1));
			LOAD_NIL(STACK(base, 2));
			call_cont(STACK(base, 1));
		}
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 4));
		Fcdr(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 1));
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Fset_car(STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 1));
		COPY(STACK(base, 0), STACK(base, 2));
		Fcdr(STACK(base, 2));
		Z73_get_rest_args(STACK(base, 2), display);
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 3), 2);
			COPY(STACK(base, 1), CAR(lptr));
			COPY(STACK(base, 2), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}
