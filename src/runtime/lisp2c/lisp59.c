#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void rmi_to_subscripts(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		Fcdr(STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		{
			static GLOBAL_FUNARG extern_closure = {Fmult, -1};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 4));
		}
		COPY(STACK(base, 2), STACK(base, 5));
		Fapply(STACK(base, 4), 2);
		mv_count = 1;
		floor_internal(STACK(base, 3));
		COPY(&mv_buf[0], STACK(base, 4));
		{
			int nargs;
			nargs = 2;
			mv_count = 1;
			switch(nargs)
			{
				case 0:
				LOAD_NIL(STACK(base, 3));
				case 1:
				LOAD_NIL(STACK(base, 4));
				nargs = 2;
			}
			Flist(STACK(base, 5), nargs - 2);
			COPY(STACK(base, 2), STACK(base, 6));
			COPY(STACK(base, 4), STACK(base, 7));
			rmi_to_subscripts(STACK(base, 6));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 3), CAR(lptr));
				COPY(STACK(base, 6), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 0));
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
