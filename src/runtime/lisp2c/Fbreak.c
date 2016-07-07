#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFbreak)
{
	MAKE_STRING(44, "~&Break: call to the `break\' function.~%~?~%"),	/* 0 */
	MAKE_STRING(43, "Restart actions:~% y: continue~% n: abort~%"),	/* 2 */
};

void Fbreak(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 0:
		LOAD_NIL(STACK(base, 0));
		nargs = 1;
	}
	Flist(STACK(base, 1), nargs - 1);
	COPY(SYMVAL(Slisp, 61), STACK(base, 2));	/* *ERROR-OUTPUT* */
	LOAD_SMSTR((CL_FORM *)&KFbreak[0], STACK(base, 3));	/* ~&Break: call to the `break' function.~%~?~% */
	COPY(STACK(base, 0), STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	Fformat(STACK(base, 2), 4);
	mv_count = 1;
	LOAD_SMSTR((CL_FORM *)&KFbreak[2], STACK(base, 2));	/* Restart actions:~% y: continue~% n: abort~% */
	Fy_or_n_p(STACK(base, 2), 1);
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		LOAD_FIXNUM(-1, STACK(base, 2));
		{
			int nargs;
			nargs = 1;
			Flist(STACK(base, 2), nargs - 0);
			{
				static GLOBAL_FUNARG extern_closure = {throw_internal, -2};
				LOAD_GLOBFUN(&extern_closure, STACK(base, 0));
			}
			LOAD_SYMBOL(SYMBOL(Slisp, 437), STACK(base, 1));	/* ERROR-CATCHER */
			Fapply(STACK(base, 0), 3);
		}
	}
}
