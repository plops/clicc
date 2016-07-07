#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fremprop(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	Fsymbol_plist(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	remf_internal(STACK(base, 2));
	COPY(&mv_buf[0], STACK(base, 3));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 2));
			case 1:
			LOAD_NIL(STACK(base, 3));
			nargs = 2;
		}
		Flist(STACK(base, 4), nargs - 2);
		if(CL_TRUEP(STACK(base, 3)))
		{
			COPY(STACK(base, 2), STACK(base, 5));
			COPY(STACK(base, 0), STACK(base, 6));
			Fset_symbol_plist(STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
		}
		else
		{
			LOAD_NIL(STACK(base, 0));
		}
	}
}
