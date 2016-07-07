#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z145_sublis_internal(CL_FORM *base, CL_FORM *display[]);
static void Z146_g1694(CL_FORM *base, int nargs);

void Fsublis(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	BOOL supl_flags[3];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 2), nargs - 2, 3, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 5));
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(CL_TRUEP(STACK(base, 2)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 3))))
		{
			GEN_CLOSURE(array, STACK(base, 5), 4, Z146_g1694, -1);
			COPY(STACK(base, 3), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 5));
			COPY(STACK(base, 5), STACK(base, 2));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 2));
		}
	}
	COPY(STACK(base, 1), STACK(base, 5));
	display[0] = STACK(base, 0);
	Z145_sublis_internal(STACK(base, 5), display);
	COPY(STACK(base, 5), STACK(base, 0));
}

static void Z145_sublis_internal(CL_FORM *base, CL_FORM *display[])
{
	COPY(STACK(base, 0), STACK(base, 1));
	COPY(&display[0][0], STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 3));	/* TEST */
	COPY(&display[0][2], STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 5));	/* KEY */
	COPY(&display[0][4], STACK(base, 6));
	Fassoc(STACK(base, 1), 6);
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 0));
		Fcdr(STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fcar(STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		Fcdr(STACK(base, 3));
		COPY(STACK(base, 2), STACK(base, 4));
		Z145_sublis_internal(STACK(base, 4), display);
		COPY(STACK(base, 3), STACK(base, 5));
		Z145_sublis_internal(STACK(base, 5), display);
		if(EQ(STACK(base, 4), STACK(base, 2)) || CL_FLOATP(STACK(base, 4)) && GET_FLOAT(STACK(base, 4)) == GET_FLOAT(STACK(base, 2)))
		{
			bool_result = EQ(STACK(base, 5), STACK(base, 3)) || CL_FLOATP(STACK(base, 5)) && GET_FLOAT(STACK(base, 5)) == GET_FLOAT(STACK(base, 3));
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
			lptr = form_alloc(STACK(base, 6), 2);
			COPY(STACK(base, 4), CAR(lptr));
			COPY(STACK(base, 5), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
}

static void Z146_g1694(CL_FORM *base, int nargs)
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