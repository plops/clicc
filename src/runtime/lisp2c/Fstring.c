#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFstring)
{
	MAKE_STRING(42, "It could not be generated a string from ~S"),	/* 0 */
};

void Fstring(CL_FORM *base)
{
	if(CL_STRING_P(STACK(base, 0)))
	{
	}
	else
	{
		if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
		{
			Fsymbol_name(STACK(base, 0));
		}
		else
		{
			if(CL_CHARP(STACK(base, 0)))
			{
				LOAD_FIXNUM(1, STACK(base, 1));
				LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 2));	/* INITIAL-ELEMENT */
				COPY(STACK(base, 0), STACK(base, 3));
				Fmake_string(STACK(base, 1), 3);
				COPY(STACK(base, 1), STACK(base, 0));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&KFstring[0], STACK(base, 1));	/* It could not be generated a string from ~S */
				COPY(STACK(base, 0), STACK(base, 2));
				Ferror(STACK(base, 1), 2);
			}
		}
	}
}
