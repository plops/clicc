#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Freverse(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			LOAD_NIL(STACK(base, 1));
			LOAD_NIL(STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 3));
			M138_1:;
			if(CL_ATOMP(STACK(base, 3)))
			{
				LOAD_NIL(STACK(base, 2));
				goto RETURN152;
			}
			COPY(STACK(base, 3), STACK(base, 4));
			Fcar(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 2));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 4), 2);
				COPY(STACK(base, 2), CAR(lptr));
				COPY(STACK(base, 1), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 1));
			}
			Fcdr(STACK(base, 3));
			goto M138_1;
			RETURN152:;
			COPY(STACK(base, 1), STACK(base, 0));
		}
		else
		{
			if(CL_VECTOR_P(STACK(base, 0)))
			{
				COPY(STACK(base, 0), STACK(base, 1));
				Flength(STACK(base, 1));
				LOAD_SYMBOL(SYMBOL(Slisp, 47), STACK(base, 2));	/* VECTOR */
				COPY(STACK(base, 0), STACK(base, 3));
				Farray_element_type(STACK(base, 3));
				Flist(STACK(base, 2), 2);
				COPY(STACK(base, 1), STACK(base, 3));
				Fmake_sequence(STACK(base, 2), 2);
				LOAD_FIXNUM(0, STACK(base, 3));
				COPY(STACK(base, 1), STACK(base, 4));
				F1minus(STACK(base, 4));
				M139_1:;
				COPY(STACK(base, 3), STACK(base, 5));
				COPY(STACK(base, 1), STACK(base, 6));
				Fnumeql(STACK(base, 5), 2);
				if(CL_TRUEP(STACK(base, 5)))
				{
					COPY(STACK(base, 2), STACK(base, 0));
					goto RETURN153;
				}
				COPY(STACK(base, 0), STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				Felt(STACK(base, 5));
				COPY(STACK(base, 5), STACK(base, 6));
				COPY(STACK(base, 2), STACK(base, 7));
				COPY(STACK(base, 4), STACK(base, 8));
				Fset_elt(STACK(base, 6));
				COPY(STACK(base, 3), STACK(base, 5));
				F1plus(STACK(base, 5));
				COPY(STACK(base, 4), STACK(base, 6));
				F1minus(STACK(base, 6));
				COPY(STACK(base, 6), STACK(base, 4));
				COPY(STACK(base, 5), STACK(base, 3));
				goto M139_1;
				RETURN153:;
			}
			else
			{
				COPY(SYMVAL(Slisp, 57), STACK(base, 1));	/* WRONG_TYPE */
				COPY(STACK(base, 0), STACK(base, 2));
				LOAD_SYMBOL(SYMBOL(Slisp, 36), STACK(base, 3));	/* SEQUENCE */
				Ferror(STACK(base, 1), 3);
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
