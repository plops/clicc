#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFsubseq)
{
	MAKE_STRING(23, "end of sequence reached"),	/* 0 */
};

void Fsubseq(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 2:
		LOAD_NIL(STACK(base, 2));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	if(CL_TRUEP(STACK(base, 0)))
	{
		if(CL_CONSP(STACK(base, 0)))
		{
			COPY(STACK(base, 0), STACK(base, 3));
			COPY(STACK(base, 2), STACK(base, 4));
			LOAD_NIL(STACK(base, 5));
			LOAD_NIL(STACK(base, 6));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 7), 2);
				COPY(STACK(base, 5), CAR(lptr));
				COPY(STACK(base, 6), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 5));
			}
			COPY(STACK(base, 1), STACK(base, 6));
			COPY(STACK(base, 3), STACK(base, 7));
			Fnthcdr(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 3));
			if(CL_TRUEP(STACK(base, 4)))
			{
				COPY(STACK(base, 4), STACK(base, 6));
				COPY(STACK(base, 1), STACK(base, 7));
				Fminus(STACK(base, 6), 2);
				COPY(STACK(base, 6), STACK(base, 4));
				LOAD_FIXNUM(0, STACK(base, 6));
				M135_1:;
				COPY(STACK(base, 6), STACK(base, 7));
				COPY(STACK(base, 4), STACK(base, 8));
				Fge(STACK(base, 7), 2);
				if(CL_TRUEP(STACK(base, 7)))
				{
					goto RETURN149;
				}
				if(CL_ATOMP(STACK(base, 3)))
				{
					LOAD_SMSTR((CL_FORM *)&KFsubseq[0], STACK(base, 7));	/* end of sequence reached */
					Ferror(STACK(base, 7), 1);
				}
				COPY(STACK(base, 3), STACK(base, 7));
				Fcar(STACK(base, 7));
				COPY(STACK(base, 3), STACK(base, 8));
				COPY(STACK(base, 8), STACK(base, 9));
				Fcdr(STACK(base, 9));
				COPY(STACK(base, 9), STACK(base, 3));
				COPY(STACK(base, 5), STACK(base, 8));
				add_q(STACK(base, 7));
				F1plus(STACK(base, 6));
				goto M135_1;
				RETURN149:;
			}
			else
			{
				M136_1:;
				if(CL_TRUEP(STACK(base, 3)))
				{
				}
				else
				{
					goto RETURN150;
				}
				COPY(STACK(base, 3), STACK(base, 6));
				Fcar(STACK(base, 6));
				COPY(STACK(base, 3), STACK(base, 7));
				COPY(STACK(base, 7), STACK(base, 8));
				Fcdr(STACK(base, 8));
				COPY(STACK(base, 8), STACK(base, 3));
				COPY(STACK(base, 5), STACK(base, 7));
				add_q(STACK(base, 6));
				goto M136_1;
				RETURN150:;
			}
			COPY(STACK(base, 5), STACK(base, 0));
			Fcar(STACK(base, 0));
		}
		else
		{
			if(CL_VECTOR_P(STACK(base, 0)))
			{
				COPY(STACK(base, 2), STACK(base, 3));
				COPY(STACK(base, 0), STACK(base, 4));
				Flength(STACK(base, 4));
				COPY(STACK(base, 1), STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				COPY(STACK(base, 4), STACK(base, 7));
				check_seq_start_end(STACK(base, 5));
				COPY(STACK(base, 5), STACK(base, 3));
				LOAD_SYMBOL(SYMBOL(Slisp, 47), STACK(base, 5));	/* VECTOR */
				COPY(STACK(base, 0), STACK(base, 6));
				Farray_element_type(STACK(base, 6));
				Flist(STACK(base, 5), 2);
				COPY(STACK(base, 3), STACK(base, 6));
				COPY(STACK(base, 1), STACK(base, 7));
				Fminus(STACK(base, 6), 2);
				Fmake_sequence(STACK(base, 5), 2);
				COPY(STACK(base, 1), STACK(base, 6));
				LOAD_FIXNUM(0, STACK(base, 7));
				M137_1:;
				COPY(STACK(base, 6), STACK(base, 8));
				COPY(STACK(base, 3), STACK(base, 9));
				Fnumeql(STACK(base, 8), 2);
				if(CL_TRUEP(STACK(base, 8)))
				{
					COPY(STACK(base, 5), STACK(base, 0));
					goto RETURN151;
				}
				COPY(STACK(base, 0), STACK(base, 8));
				COPY(STACK(base, 6), STACK(base, 9));
				Felt(STACK(base, 8));
				COPY(STACK(base, 8), STACK(base, 9));
				COPY(STACK(base, 5), STACK(base, 10));
				COPY(STACK(base, 7), STACK(base, 11));
				Fset_elt(STACK(base, 9));
				COPY(STACK(base, 6), STACK(base, 8));
				F1plus(STACK(base, 8));
				COPY(STACK(base, 7), STACK(base, 9));
				F1plus(STACK(base, 9));
				COPY(STACK(base, 9), STACK(base, 7));
				COPY(STACK(base, 8), STACK(base, 6));
				goto M137_1;
				RETURN151:;
			}
			else
			{
				COPY(SYMVAL(Slisp, 57), STACK(base, 3));	/* WRONG_TYPE */
				COPY(STACK(base, 0), STACK(base, 4));
				LOAD_SYMBOL(SYMBOL(Slisp, 36), STACK(base, 5));	/* SEQUENCE */
				Ferror(STACK(base, 3), 3);
			}
		}
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
