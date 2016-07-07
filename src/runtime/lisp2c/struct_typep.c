#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void struct_typep(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 2));
	structp(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 0), STACK(base, 2));
		struct_type(STACK(base, 2));
		M76_1:;
		if(EQ(STACK(base, 2), STACK(base, 1)))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
			goto RETURN85;
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 3));
			COPY(STACK(base, 3), STACK(base, 4));
			Fsymbol_plist(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 5));
			LOAD_SYMBOL(SYMBOL(Slisp, 325), STACK(base, 6));	/* INCLUDED-STRUCT */
			get_prop(STACK(base, 5));
			if(CL_TRUEP(STACK(base, 5)))
			{
				COPY(STACK(base, 5), STACK(base, 6));
				COPY(STACK(base, 6), STACK(base, 2));
				Fcdr(STACK(base, 2));
				Fcar(STACK(base, 2));
			}
			else
			{
				LOAD_NIL(STACK(base, 2));
			}
			if(CL_TRUEP(STACK(base, 2)))
			{
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
				goto RETURN85;
			}
		}
		goto M76_1;
		RETURN85:;
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
