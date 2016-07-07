#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFimport)
{
	MAKE_STRING(44, "a symbol ~S is already present in package ~S"),	/* 0 */
};

void Fimport(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 272), STACK(base, 1));	/* *PACKAGE* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	coerce_to_package(STACK(base, 1));
	if(CL_LISTP(STACK(base, 0)))
	{
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Flist(STACK(base, 2), 1);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	M57_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN66;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	Fsymbol_name(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	Ffind_symbol(STACK(base, 4), 2);
	COPY(&mv_buf[0], STACK(base, 5));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 4));
			case 1:
			LOAD_NIL(STACK(base, 5));
			nargs = 2;
		}
		Flist(STACK(base, 6), nargs - 2);
		if(CL_TRUEP(STACK(base, 5)))
		{
			if(EQ(STACK(base, 2), STACK(base, 4)))
			{
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFimport[0], STACK(base, 7));	/* a symbol ~S is already present in package ~S */
				COPY(STACK(base, 4), STACK(base, 8));
				COPY(STACK(base, 1), STACK(base, 9));
				Ferror(STACK(base, 7), 3);
			}
			if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 312))	/* INHERITED */
			{
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
				goto RETURN65;
			}
		}
		COPY(STACK(base, 2), STACK(base, 7));
		COPY(STACK(base, 1), STACK(base, 8));
		internal_import(STACK(base, 7));
	}
	Fcdr(STACK(base, 3));
	goto M57_1;
	RETURN66:;
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	RETURN65:;
}
