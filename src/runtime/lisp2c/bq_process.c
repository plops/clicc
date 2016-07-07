#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kbq_process)
{
	MAKE_SYMREF(SYMBOL(Slisp, 25)),	/* 0 CONS(0) FUNCTION */
	MAKE_LIST(&Kbq_process[2]),
	MAKE_SYMREF(SYMBOL(Slisp, 47)),	/* CONS(2) VECTOR */
	MAKE_NIL,
	MAKE_STRING(12, ",@~S after `"),	/* 2 */
	MAKE_STRING(12, ",.~S after `"),	/* 4 */
	MAKE_STRING(11, "Dotted ,@~s"),	/* 6 */
	MAKE_STRING(11, "Dotted ,@~s"),	/* 8 */
};

void bq_process(CL_FORM *base)
{
	if(CL_ATOMP(STACK(base, 0)))
	{
		COPY(SYMVAL(Slisp, 372), STACK(base, 1));	/* *BQ-QUOTE* */
		COPY(STACK(base, 0), STACK(base, 2));
		Flist(STACK(base, 1), 2);
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 1));
		Fcar(STACK(base, 1));
		if(EQ(STACK(base, 1), SYMVAL(Slisp, 374)))	/* *BQ-VECTOR* */
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcdr(STACK(base, 1));
			bq_process(STACK(base, 1));
			bq_simplify(STACK(base, 1));
			bq_remove_tokens(STACK(base, 1));
			COPY(STACK(base, 1), STACK(base, 2));
			Fcar(STACK(base, 2));
			if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 383))	/* QUOTE */
			{
				COPY(SYMVAL(Slisp, 372), STACK(base, 2));	/* *BQ-QUOTE* */
				LOAD_GLOBFUN(&CFvector, STACK(base, 3));
				COPY(STACK(base, 1), STACK(base, 4));
				Fcdr(STACK(base, 4));
				Fcar(STACK(base, 4));
				Fapply(STACK(base, 3), 2);
				mv_count = 1;
				Flist(STACK(base, 2), 2);
				COPY(STACK(base, 2), STACK(base, 0));
			}
			else
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 384), STACK(base, 2));	/* APPLY */
				LOAD_CONS((CL_FORM *)&Kbq_process[0], STACK(base, 3));
				COPY(STACK(base, 1), STACK(base, 4));
				Flist(STACK(base, 2), 3);
				COPY(STACK(base, 2), STACK(base, 0));
			}
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 1));
			Fcar(STACK(base, 1));
			if(EQ(STACK(base, 1), SYMVAL(Slisp, 364)))	/* *COMMA* */
			{
				Fcdr(STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 1));
				Fcar(STACK(base, 1));
				if(EQ(STACK(base, 1), SYMVAL(Slisp, 365)))	/* *COMMA-ATSIGN* */
				{
					LOAD_SMSTR((CL_FORM *)&Kbq_process[4], STACK(base, 1));	/* ,@~S after ` */
					COPY(STACK(base, 0), STACK(base, 2));
					Fcdr(STACK(base, 2));
					Ferror(STACK(base, 1), 2);
				}
				else
				{
					COPY(STACK(base, 0), STACK(base, 1));
					Fcar(STACK(base, 1));
					if(EQ(STACK(base, 1), SYMVAL(Slisp, 366)))	/* *COMMA-DOT* */
					{
						LOAD_SMSTR((CL_FORM *)&Kbq_process[6], STACK(base, 1));	/* ,.~S after ` */
						COPY(STACK(base, 0), STACK(base, 2));
						Fcdr(STACK(base, 2));
						Ferror(STACK(base, 1), 2);
					}
					else
					{
						COPY(STACK(base, 0), STACK(base, 1));
						LOAD_NIL(STACK(base, 2));
						M82_1:;
						if(CL_ATOMP(STACK(base, 1)))
						{
							COPY(SYMVAL(Slisp, 372), STACK(base, 3));	/* *BQ-APPEND* *BQ-QUOTE* */
							COPY(STACK(base, 1), STACK(base, 4));
							Flist(STACK(base, 3), 2);
							Flist(STACK(base, 3), 1);
							COPY(STACK(base, 2), STACK(base, 4));
							Freverse(STACK(base, 4));
							COPY(STACK(base, 3), STACK(base, 5));
							Fnconc(STACK(base, 4), 2);
							COPY(STACK(base, 4), STACK(base, 3));
							{
								CL_FORM *lptr;
								lptr = form_alloc(STACK(base, 4), 2);
								COPY(SYMVAL(Slisp, 368), CAR(lptr));
								COPY(STACK(base, 3), CDR(lptr));
								LOAD_CONS(lptr, STACK(base, 0));
							}
							goto RETURN91;
						}
						COPY(STACK(base, 1), STACK(base, 3));
						Fcar(STACK(base, 3));
						if(EQ(STACK(base, 3), SYMVAL(Slisp, 364)))	/* *COMMA* */
						{
							COPY(STACK(base, 1), STACK(base, 3));	/* *BQ-APPEND* */
							Fcdr(STACK(base, 3));
							Flist(STACK(base, 3), 1);
							COPY(STACK(base, 2), STACK(base, 4));
							Freverse(STACK(base, 4));
							COPY(STACK(base, 3), STACK(base, 5));
							Fnconc(STACK(base, 4), 2);
							COPY(STACK(base, 4), STACK(base, 3));
							{
								CL_FORM *lptr;
								lptr = form_alloc(STACK(base, 4), 2);
								COPY(SYMVAL(Slisp, 368), CAR(lptr));
								COPY(STACK(base, 3), CDR(lptr));
								LOAD_CONS(lptr, STACK(base, 0));
							}
							goto RETURN91;
						}
						COPY(STACK(base, 1), STACK(base, 3));
						Fcar(STACK(base, 3));
						if(EQ(STACK(base, 3), SYMVAL(Slisp, 365)))	/* *COMMA-ATSIGN* */
						{
							LOAD_SMSTR((CL_FORM *)&Kbq_process[8], STACK(base, 3));	/* Dotted ,@~s */
							COPY(STACK(base, 1), STACK(base, 4));
							Fcdr(STACK(base, 4));
							Ferror(STACK(base, 3), 2);
						}
						COPY(STACK(base, 1), STACK(base, 3));
						Fcar(STACK(base, 3));
						if(EQ(STACK(base, 3), SYMVAL(Slisp, 366)))	/* *COMMA-DOT* */
						{
							LOAD_SMSTR((CL_FORM *)&Kbq_process[10], STACK(base, 3));	/* Dotted ,@~s */
							COPY(STACK(base, 1), STACK(base, 4));
							Fcdr(STACK(base, 4));
							Ferror(STACK(base, 3), 2);
						}
						COPY(STACK(base, 1), STACK(base, 3));
						Fcdr(STACK(base, 3));
						COPY(STACK(base, 1), STACK(base, 4));
						Fcar(STACK(base, 4));
						if(CL_ATOMP(STACK(base, 4)))
						{
							COPY(SYMVAL(Slisp, 367), STACK(base, 5));	/* *BQ-LIST* */
							COPY(STACK(base, 4), STACK(base, 6));
							bq_process(STACK(base, 6));
							Flist(STACK(base, 5), 2);
							COPY(STACK(base, 5), STACK(base, 4));
						}
						else
						{
							COPY(STACK(base, 4), STACK(base, 5));
							Fcar(STACK(base, 5));
							if(EQ(STACK(base, 5), SYMVAL(Slisp, 364)))	/* *COMMA* */
							{
								COPY(SYMVAL(Slisp, 367), STACK(base, 5));	/* *BQ-LIST* */
								COPY(STACK(base, 4), STACK(base, 6));
								Fcdr(STACK(base, 6));
								Flist(STACK(base, 5), 2);
								COPY(STACK(base, 5), STACK(base, 4));
							}
							else
							{
								COPY(STACK(base, 4), STACK(base, 5));
								Fcar(STACK(base, 5));
								if(EQ(STACK(base, 5), SYMVAL(Slisp, 365)))	/* *COMMA-ATSIGN* */
								{
									Fcdr(STACK(base, 4));
								}
								else
								{
									COPY(STACK(base, 4), STACK(base, 5));
									Fcar(STACK(base, 5));
									if(EQ(STACK(base, 5), SYMVAL(Slisp, 366)))	/* *COMMA-DOT* */
									{
										COPY(SYMVAL(Slisp, 371), STACK(base, 5));	/* *BQ-CLOBBERABLE* */
										COPY(STACK(base, 4), STACK(base, 6));
										Fcdr(STACK(base, 6));
										Flist(STACK(base, 5), 2);
										COPY(STACK(base, 5), STACK(base, 4));
									}
									else
									{
										COPY(SYMVAL(Slisp, 367), STACK(base, 5));	/* *BQ-LIST* */
										COPY(STACK(base, 4), STACK(base, 6));
										bq_process(STACK(base, 6));
										Flist(STACK(base, 5), 2);
										COPY(STACK(base, 5), STACK(base, 4));
									}
								}
							}
						}
						{
							CL_FORM *lptr;
							lptr = form_alloc(STACK(base, 5), 2);
							COPY(STACK(base, 4), CAR(lptr));
							COPY(STACK(base, 2), CDR(lptr));
							LOAD_CONS(lptr, STACK(base, 2));
						}
						COPY(STACK(base, 3), STACK(base, 1));
						goto M82_1;
						RETURN91:;
					}
				}
			}
		}
	}
}
