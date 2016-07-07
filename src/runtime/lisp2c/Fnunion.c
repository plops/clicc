#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z155_union_internal(CL_FORM *base, CL_FORM *display[]);
static void Z156_g1708(CL_FORM *base, int nargs);

void Fnunion(CL_FORM *base, int nargs)
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
			GEN_CLOSURE(array, STACK(base, 5), 4, Z156_g1708, -1);
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
	COPY(STACK(base, 0), STACK(base, 5));
	display[0] = STACK(base, 0);
	Z155_union_internal(STACK(base, 5), display);
	COPY(STACK(base, 5), STACK(base, 0));
}

static void Z155_union_internal(CL_FORM *base, CL_FORM *display[])
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_TRUEP(&display[0][4]))
		{
			COPY(&display[0][4], STACK(base, 1));
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
			Ffuncall(STACK(base, 1), 2);
			mv_count = 1;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
		}
		COPY(&display[0][1], STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 3));	/* TEST */
		COPY(&display[0][2], STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 5));	/* KEY */
		COPY(&display[0][4], STACK(base, 6));
		Fmember(STACK(base, 1), 6);
		if(CL_TRUEP(STACK(base, 1)))
		{
			Fcdr(STACK(base, 0));
			Z155_union_internal(STACK(base, 0), display);
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
			COPY(STACK(base, 0), STACK(base, 2));
			Fcdr(STACK(base, 2));
			Z155_union_internal(STACK(base, 2), display);
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 3), 2);
				COPY(STACK(base, 1), CAR(lptr));
				COPY(STACK(base, 2), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 0));
			}
		}
	}
	else
	{
		COPY(&display[0][1], STACK(base, 0));
	}
}

static void Z156_g1708(CL_FORM *base, int nargs)
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