#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z139_subst_internal(CL_FORM *base, CL_FORM *display[]);
static void Z140_g1686(CL_FORM *base, int nargs);

void Fsubst(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	BOOL supl_flags[3];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 3), nargs - 3, 3, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	GEN_HEAPVAR(STACK(base, 4), STACK(base, 6));
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	if(CL_TRUEP(STACK(base, 3)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 4))))
		{
			GEN_CLOSURE(array, STACK(base, 6), 4, Z140_g1686, -1);
			COPY(STACK(base, 4), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 3));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 3));
		}
	}
	COPY(STACK(base, 2), STACK(base, 6));
	display[0] = STACK(base, 0);
	Z139_subst_internal(STACK(base, 6), display);
	COPY(STACK(base, 6), STACK(base, 0));
}

static void Z139_subst_internal(CL_FORM *base, CL_FORM *display[])
{
	COPY(&display[0][3], STACK(base, 1));
	COPY(&display[0][1], STACK(base, 2));
	if(CL_TRUEP(&display[0][5]))
	{
		COPY(&display[0][5], STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Ffuncall(STACK(base, 3), 2);
		mv_count = 1;
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 3));
	}
	Ffuncall(STACK(base, 1), 3);
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
			Z139_subst_internal(STACK(base, 3), display);
			COPY(STACK(base, 2), STACK(base, 4));
			Z139_subst_internal(STACK(base, 4), display);
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

static void Z140_g1686(CL_FORM *base, int nargs)
{
	Flist(STACK(base, 1), nargs - 0);
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	Fapply(STACK(base, 2), 2);
	mv_count = 1;
	if(NOT(CL_TRUEP(STACK(base, 2))))
	{
		LOAD_T(STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
