#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cbq_remove_tokens = {bq_remove_tokens, 1};

void bq_remove_tokens(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
		if(EQ(STACK(base, 0), SYMVAL(Slisp, 367)))	/* *BQ-LIST* */
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 28), STACK(base, 0));	/* LIST */
		}
		else
		{
			if(EQ(STACK(base, 0), SYMVAL(Slisp, 368)))	/* *BQ-APPEND* */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 385), STACK(base, 0));	/* APPEND */
			}
			else
			{
				if(EQ(STACK(base, 0), SYMVAL(Slisp, 370)))	/* *BQ-NCONC* */
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 386), STACK(base, 0));	/* NCONC */
				}
				else
				{
					if(EQ(STACK(base, 0), SYMVAL(Slisp, 369)))	/* *BQ-LIST** */
					{
						LOAD_SYMBOL(SYMBOL(Slisp, 387), STACK(base, 0));	/* LIST* */
					}
					else
					{
						if(EQ(STACK(base, 0), SYMVAL(Slisp, 372)))	/* *BQ-QUOTE* */
						{
							LOAD_SYMBOL(SYMBOL(Slisp, 383), STACK(base, 0));	/* QUOTE */
						}
						else
						{
						}
					}
				}
			}
		}
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(EQ(STACK(base, 1), SYMVAL(Slisp, 371)))	/* *BQ-CLOBBERABLE* */
		{
			Fcdr(STACK(base, 0));
			Fcar(STACK(base, 0));
			bq_remove_tokens(STACK(base, 0));
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
			if(EQ(STACK(base, 1), SYMVAL(Slisp, 369)))	/* *BQ-LIST** */
			{
				COPY(STACK(base, 0), STACK(base, 1));
				Fcdr(STACK(base, 1));
				Fcdr(STACK(base, 1));
				if(CL_CONSP(STACK(base, 1)))
				{
					COPY(STACK(base, 0), STACK(base, 1));
					Fcdr(STACK(base, 1));
					Fcdr(STACK(base, 1));
					Fcdr(STACK(base, 1));
					bool_result = NOT(CL_TRUEP(STACK(base, 1)));
				}
				else
				{
					bool_result = FALSE;
				}
			}
			else
			{
				bool_result = FALSE;
			}
			if(bool_result)
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 21), STACK(base, 1));	/* CONS */
				LOAD_GLOBFUN(&Cbq_remove_tokens, STACK(base, 2));
				COPY(STACK(base, 0), STACK(base, 3));
				Fcdr(STACK(base, 3));
				maptree(STACK(base, 2));
				mv_count = 1;
				{
					CL_FORM *lptr;
					lptr = form_alloc(STACK(base, 3), 2);
					COPY(STACK(base, 1), CAR(lptr));
					COPY(STACK(base, 2), CDR(lptr));
					LOAD_CONS(lptr, STACK(base, 0));
				}
			}
			else
			{
				LOAD_GLOBFUN(&Cbq_remove_tokens, STACK(base, 1));
				COPY(STACK(base, 0), STACK(base, 2));
				maptree(STACK(base, 1));
				COPY(STACK(base, 1), STACK(base, 0));
			}
		}
	}
}
