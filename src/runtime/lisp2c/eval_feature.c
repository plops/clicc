#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Keval_feature)
{
	MAKE_STRING(29, "illegal feature expression ~s"),	/* 0 */
};

void eval_feature(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
		COPY(SYMVAL(Slisp, 347), STACK(base, 1));	/* *FEATURES* */
		Fmember(STACK(base, 0), 2);
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 394))	/* NOT */
		{
			COPY(STACK(base, 0), STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 2));
			Fcdr(STACK(base, 2));
			Fcar(STACK(base, 2));
			eval_feature(STACK(base, 2));
			if(NOT(CL_TRUEP(STACK(base, 2))))
			{
				LOAD_T(STACK(base, 0));
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 395))	/* AND */
			{
				LOAD_NIL(STACK(base, 2));
				COPY(STACK(base, 0), STACK(base, 3));
				Fcdr(STACK(base, 3));
				M96_1:;
				if(CL_ATOMP(STACK(base, 3)))
				{
					LOAD_NIL(STACK(base, 2));
					goto RETURN107;
				}
				COPY(STACK(base, 3), STACK(base, 4));
				Fcar(STACK(base, 4));
				COPY(STACK(base, 4), STACK(base, 2));
				eval_feature(STACK(base, 4));
				if(CL_TRUEP(STACK(base, 4)))
				{
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
					goto RETURN106;
				}
				Fcdr(STACK(base, 3));
				goto M96_1;
				RETURN107:;
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 396))	/* OR */
				{
					LOAD_NIL(STACK(base, 2));
					COPY(STACK(base, 0), STACK(base, 3));
					Fcdr(STACK(base, 3));
					M97_1:;
					if(CL_ATOMP(STACK(base, 3)))
					{
						LOAD_NIL(STACK(base, 2));
						COPY(STACK(base, 2), STACK(base, 0));
						goto RETURN108;
					}
					COPY(STACK(base, 3), STACK(base, 4));
					Fcar(STACK(base, 4));
					COPY(STACK(base, 4), STACK(base, 2));
					eval_feature(STACK(base, 4));
					if(CL_TRUEP(STACK(base, 4)))
					{
						LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
						goto RETURN106;
					}
					Fcdr(STACK(base, 3));
					goto M97_1;
					RETURN108:;
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Keval_feature[0], STACK(base, 2));	/* illegal feature expression ~s */
					COPY(STACK(base, 0), STACK(base, 3));
					Ferror(STACK(base, 2), 2);
				}
			}
		}
	}
	RETURN106:;
}
