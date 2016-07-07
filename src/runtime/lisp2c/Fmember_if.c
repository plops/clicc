#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z151_member_if_internal(CL_FORM *base, CL_FORM *display[]);

void Fmember_if(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	BOOL supl_flags[1];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 139),	/* KEY */
	};
	keysort(STACK(base, 2), nargs - 2, 1, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
	}
	COPY(STACK(base, 1), STACK(base, 3));
	display[0] = STACK(base, 0);
	Z151_member_if_internal(STACK(base, 3), display);
	COPY(STACK(base, 3), STACK(base, 0));
}

static void Z151_member_if_internal(CL_FORM *base, CL_FORM *display[])
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		COPY(&display[0][0], STACK(base, 1));
		if(CL_TRUEP(&display[0][2]))
		{
			COPY(&display[0][2], STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			Fcar(STACK(base, 3));
			Ffuncall(STACK(base, 2), 2);
			mv_count = 1;
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 2));
			Fcar(STACK(base, 2));
		}
		Ffuncall(STACK(base, 1), 2);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 1)))
		{
		}
		else
		{
			Fcdr(STACK(base, 0));
			Z151_member_if_internal(STACK(base, 0), display);
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
