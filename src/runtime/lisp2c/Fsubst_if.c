#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z141_subst_if_internal(CL_FORM *base, CL_FORM *display[]);

void Fsubst_if(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 3), nargs - 3, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	COPY(STACK(base, 2), STACK(base, 4));
	display[0] = STACK(base, 0);
	Z141_subst_if_internal(STACK(base, 4), display);
	COPY(STACK(base, 4), STACK(base, 0));
}

static void Z141_subst_if_internal(CL_FORM *base, CL_FORM *display[])
{
	COPY(&display[0][1], STACK(base, 1));
	if(CL_TRUEP(&display[0][3]))
	{
		COPY(&display[0][3], STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		Ffuncall(STACK(base, 2), 2);
		mv_count = 1;
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
	}
	Ffuncall(STACK(base, 1), 2);
	mv_count = 1;
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(&display[0][0], STACK(base, 0));
	}
	else
	{
		if(CL_ATOMP(STACK(base, 0)))
		{
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
			COPY(STACK(base, 0), STACK(base, 2));
			Fcdr(STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 3));
			Z141_subst_if_internal(STACK(base, 3), display);
			COPY(STACK(base, 2), STACK(base, 4));
			Z141_subst_if_internal(STACK(base, 4), display);
			if(EQ(STACK(base, 3), STACK(base, 1)) || CL_FLOATP(STACK(base, 3)) && GET_FLOAT(STACK(base, 3)) == GET_FLOAT(STACK(base, 1)))
			{
				bool_result = EQ(STACK(base, 4), STACK(base, 2)) || CL_FLOATP(STACK(base, 4)) && GET_FLOAT(STACK(base, 4)) == GET_FLOAT(STACK(base, 2));
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
			}
			else
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 5), 2);
				COPY(STACK(base, 3), CAR(lptr));
				COPY(STACK(base, 4), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 0));
			}
		}
	}
}
