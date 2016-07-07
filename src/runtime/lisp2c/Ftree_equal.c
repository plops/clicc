#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z134_tree_equal_internal(CL_FORM *base, CL_FORM *display[]);
static void Z135_g1635(CL_FORM *base, int nargs);

void Ftree_equal(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	BOOL supl_flags[2];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 479),	/* TEST-NOT */
	};
	keysort(STACK(base, 2), nargs - 2, 2, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 4));
	if(CL_TRUEP(STACK(base, 2)))
	{
	}
	else
	{
		if(CL_TRUEP(INDIRECT(STACK(base, 3))))
		{
			GEN_CLOSURE(array, STACK(base, 4), 4, Z135_g1635, -1);
			COPY(STACK(base, 3), &array[3]);
			LOAD_CLOSURE(array, STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 2));
		}
		else
		{
			static GLOBAL_FUNARG extern_closure = {Feql, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 2));
		}
	}
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	display[0] = STACK(base, 0);
	Z134_tree_equal_internal(STACK(base, 4), display);
	COPY(STACK(base, 4), STACK(base, 0));
}

static void Z134_tree_equal_internal(CL_FORM *base, CL_FORM *display[])
{
	if(CL_ATOMP(STACK(base, 0)))
	{
		if(CL_ATOMP(STACK(base, 1)))
		{
			COPY(&display[0][2], STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			COPY(STACK(base, 1), STACK(base, 4));
			Ffuncall(STACK(base, 2), 3);
			COPY(STACK(base, 2), STACK(base, 0));
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
	else
	{
		if(CL_ATOMP(STACK(base, 1)))
		{
			LOAD_NIL(STACK(base, 0));
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 3));
			Fcar(STACK(base, 3));
			Z134_tree_equal_internal(STACK(base, 2), display);
			mv_count = 1;
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				Fcdr(STACK(base, 2));
				COPY(STACK(base, 1), STACK(base, 3));
				Fcdr(STACK(base, 3));
				Z134_tree_equal_internal(STACK(base, 2), display);
				COPY(STACK(base, 2), STACK(base, 0));
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
	}
}

static void Z135_g1635(CL_FORM *base, int nargs)
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
