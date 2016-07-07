#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFcoerce)
{
	MAKE_STRING(19, "cannot coerce to ~S"),	/* 0 */
};

void Fcoerce(CL_FORM *base)
{
	if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 28))	/* LIST */
	{
		if(CL_LISTP(STACK(base, 0)))
		{
		}
		else
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 28), STACK(base, 2));	/* LIST */
			COPY(STACK(base, 0), STACK(base, 3));
			Fconcatenate(STACK(base, 2), 2);
			COPY(STACK(base, 2), STACK(base, 0));
		}
	}
	else
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 44))	/* STRING */
		{
			if(CL_STRING_P(STACK(base, 0)))
			{
			}
			else
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 44), STACK(base, 2));	/* STRING */
				COPY(STACK(base, 0), STACK(base, 3));
				Fconcatenate(STACK(base, 2), 2);
				COPY(STACK(base, 2), STACK(base, 0));
			}
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 47))	/* VECTOR */
			{
				if(CL_VECTOR_P(STACK(base, 0)))
				{
				}
				else
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 47), STACK(base, 2));	/* VECTOR */
					COPY(STACK(base, 0), STACK(base, 3));
					Fconcatenate(STACK(base, 2), 2);
					COPY(STACK(base, 2), STACK(base, 0));
				}
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 18))	/* CHARACTER */
				{
					Fcharacter(STACK(base, 0));
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Ssys, 1))	/* T */
					{
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&KFcoerce[0], STACK(base, 0));	/* cannot coerce to ~S */
						Ferror(STACK(base, 0), 2);
					}
				}
			}
		}
	}
}
