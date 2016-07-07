#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void bq_attach_conses(CL_FORM *base)
{
	LOAD_GLOBFUN(&Cnull_or_quoted, STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	Fevery(STACK(base, 2), 2);
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), STACK(base, 2));
		null_or_quoted(STACK(base, 2));
		bool_result = CL_TRUEP(STACK(base, 2));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		COPY(SYMVAL(Slisp, 372), STACK(base, 2));	/* *BQ-QUOTE* */
		LOAD_GLOBFUN(&CFcadr, STACK(base, 3));
		COPY(STACK(base, 0), STACK(base, 4));
		Fmapcar(STACK(base, 3), 2);
		COPY(STACK(base, 1), STACK(base, 4));
		Fcdr(STACK(base, 4));
		Fcar(STACK(base, 4));
		Fappend(STACK(base, 3), 2);
		Flist(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	else
	{
		if(NOT(CL_TRUEP(STACK(base, 1))))
		{
			LOAD_T(STACK(base, 2));
		}
		else
		{
			LOAD_NIL(STACK(base, 2));
		}
		if(CL_TRUEP(STACK(base, 2)))
		{
			bool_result = TRUE;
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 3));
			COPY(SYMVAL(Slisp, 373), STACK(base, 4));	/* *BQ-QUOTE-NIL* */
			Fequal(STACK(base, 3));
			bool_result = CL_TRUEP(STACK(base, 3));
		}
		if(bool_result)
		{
			CL_FORM *lptr;	/* *BQ-LIST* */
			lptr = form_alloc(STACK(base, 2), 2);
			COPY(SYMVAL(Slisp, 367), CAR(lptr));
			COPY(STACK(base, 0), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
		else
		{
			if(CL_CONSP(STACK(base, 1)))
			{
				COPY(STACK(base, 1), STACK(base, 2));
				Fcar(STACK(base, 2));
				if(EQ(STACK(base, 2), SYMVAL(Slisp, 367)))	/* *BQ-LIST* */
				{
					LOAD_T(STACK(base, 2));
				}
				else
				{
					LOAD_NIL(STACK(base, 2));
				}
				if(CL_TRUEP(STACK(base, 2)))
				{
					bool_result = TRUE;
				}
				else
				{
					COPY(STACK(base, 1), STACK(base, 3));
					Fcar(STACK(base, 3));
					bool_result = EQ(STACK(base, 3), SYMVAL(Slisp, 369));	/* *BQ-LIST** */
				}
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				COPY(STACK(base, 1), STACK(base, 2));
				Fcar(STACK(base, 2));
				COPY(STACK(base, 0), STACK(base, 3));
				COPY(STACK(base, 1), STACK(base, 4));
				Fcdr(STACK(base, 4));
				Fappend(STACK(base, 3), 2);
				{
					CL_FORM *lptr;
					lptr = form_alloc(STACK(base, 4), 2);
					COPY(STACK(base, 2), CAR(lptr));
					COPY(STACK(base, 3), CDR(lptr));
					LOAD_CONS(lptr, STACK(base, 0));
				}
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 2));	/* *BQ-LIST** */
				COPY(STACK(base, 1), STACK(base, 3));
				Flist(STACK(base, 3), 1);
				Fappend(STACK(base, 2), 2);
				{
					CL_FORM *lptr;
					lptr = form_alloc(STACK(base, 3), 2);
					COPY(SYMVAL(Slisp, 369), CAR(lptr));
					COPY(STACK(base, 2), CDR(lptr));
					LOAD_CONS(lptr, STACK(base, 0));
				}
			}
		}
	}
}
