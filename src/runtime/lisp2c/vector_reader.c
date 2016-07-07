#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cvector_reader = {vector_reader, 3};

CL_INIT2(Kvector_reader)
{
	MAKE_STRING(42, "Vector longer than specified length: #~S~S"),	/* 0 */
	MAKE_STRING(52, "#~s( syntax is not allowed in backquoted expressions"),	/* 2 */
};

void vector_reader(CL_FORM *base)
{
	COPY(STACK(base, 0), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	cons_reader(STACK(base, 3));
	if(CL_TRUEP(SYMVAL(Slisp, 346)))	/* *READ-SUPPRESS* */
	{
		LOAD_NIL(STACK(base, 0));
	}
	else
	{
		COPY(SYMVAL(Slisp, 375), STACK(base, 4));	/* *BQ-LEVEL* */
		Fzerop(STACK(base, 4));
		if(CL_TRUEP(STACK(base, 4)))
		{
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(STACK(base, 3), STACK(base, 4));
				Flength(STACK(base, 4));
				COPY(STACK(base, 4), STACK(base, 5));
				COPY(STACK(base, 2), STACK(base, 6));
				Fgt(STACK(base, 5), 2);
				if(CL_TRUEP(STACK(base, 5)))
				{
					LOAD_SMSTR((CL_FORM *)&Kvector_reader[0], STACK(base, 5));	/* Vector longer than specified length: #~S~S */
					COPY(STACK(base, 2), STACK(base, 6));
					COPY(STACK(base, 3), STACK(base, 7));
					Ferror(STACK(base, 5), 3);
				}
				COPY(STACK(base, 2), STACK(base, 5));
				Fmake_array(STACK(base, 5), 1);
				COPY(STACK(base, 3), STACK(base, 6));
				Freplace(STACK(base, 5), 2);
				COPY(STACK(base, 3), STACK(base, 6));
				Flast(STACK(base, 6), 1);
				Fcar(STACK(base, 6));
				LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 7));	/* START */
				COPY(STACK(base, 4), STACK(base, 8));
				Ffill(STACK(base, 5), 4);
				COPY(STACK(base, 5), STACK(base, 0));
			}
			else
			{
				LOAD_GLOBFUN(&CFvector, STACK(base, 0));
				COPY(STACK(base, 3), STACK(base, 1));
				Fapply(STACK(base, 0), 2);
			}
		}
		else
		{
			if(CL_TRUEP(STACK(base, 2)))
			{
				LOAD_SMSTR((CL_FORM *)&Kvector_reader[2], STACK(base, 0));	/* #~s( syntax is not allowed in backquoted expressions */
				COPY(STACK(base, 2), STACK(base, 1));
				Ferror(STACK(base, 0), 2);
			}
			else
			{
				CL_FORM *lptr;	/* *BQ-VECTOR* */
				lptr = form_alloc(STACK(base, 4), 2);
				COPY(SYMVAL(Slisp, 374), CAR(lptr));
				COPY(STACK(base, 3), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 0));
			}
		}
	}
}
