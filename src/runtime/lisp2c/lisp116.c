#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kmaybe_make_pattern)
{
	MAKE_STRING(33, "``[\'\' with no corresponding ``]\'\'"),	/* 0 */
	MAKE_STRING(0, ""),	/* 2 */
};

static void Z65_flush_pending_regulars(CL_FORM *base, CL_FORM *display[]);

void maybe_make_pattern(CL_FORM *base)
{
	CL_FORM *display[1];
	if(CL_TRUEP(SYMVAL(Slisp, 218)))	/* *IGNORE-WILDCARDS* */
	{
		COPY(STACK(base, 0), STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fsubseq(STACK(base, 3), 3);
		COPY(STACK(base, 3), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 3));
		LOAD_NIL(STACK(base, 4));
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 5), 2);
			COPY(STACK(base, 3), CAR(lptr));
			COPY(STACK(base, 4), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 3));
		}
		LOAD_NIL(STACK(base, 4));
		LOAD_NIL(STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
		COPY(STACK(base, 1), STACK(base, 7));
		M25_1:;
		COPY(STACK(base, 7), STACK(base, 8));
		COPY(STACK(base, 2), STACK(base, 9));
		Fge(STACK(base, 8), 2);
		if(CL_TRUEP(STACK(base, 8)))
		{
			goto RETURN26;
		}
		COPY(STACK(base, 0), STACK(base, 8));
		COPY(STACK(base, 7), STACK(base, 9));
		Fschar(STACK(base, 8));
		if(CL_TRUEP(STACK(base, 4)))
		{
			COPY(STACK(base, 7), STACK(base, 9));
			F1plus(STACK(base, 9));
			COPY(STACK(base, 9), STACK(base, 7));
			LOAD_NIL(STACK(base, 4));
		}
		else
		{
			COPY(STACK(base, 8), STACK(base, 9));
			LOAD_CHAR('\\', STACK(base, 10));
			FcharE(STACK(base, 9), 2);
			if(CL_TRUEP(STACK(base, 9)))
			{
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 4));	/* T */
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
				if(CL_TRUEP(STACK(base, 6)))
				{
				}
				else
				{
					COPY(STACK(base, 7), STACK(base, 6));
				}
				COPY(STACK(base, 7), STACK(base, 9));
				F1plus(STACK(base, 9));
				COPY(STACK(base, 9), STACK(base, 7));
			}
			else
			{
				COPY(STACK(base, 8), STACK(base, 9));
				LOAD_CHAR('?', STACK(base, 10));
				FcharE(STACK(base, 9), 2);
				if(CL_TRUEP(STACK(base, 9)))
				{
					display[0] = STACK(base, 0);
					Z65_flush_pending_regulars(STACK(base, 9), display);
					LOAD_SYMBOL(SYMBOL(Slisp, 214), STACK(base, 9));	/* SINGLE-CHAR-WILD */
					COPY(STACK(base, 3), STACK(base, 10));
					add_q(STACK(base, 9));
					COPY(STACK(base, 7), STACK(base, 9));
					F1plus(STACK(base, 9));
					COPY(STACK(base, 9), STACK(base, 7));
				}
				else
				{
					COPY(STACK(base, 8), STACK(base, 9));
					LOAD_CHAR('*', STACK(base, 10));
					FcharE(STACK(base, 9), 2);
					if(CL_TRUEP(STACK(base, 9)))
					{
						display[0] = STACK(base, 0);
						Z65_flush_pending_regulars(STACK(base, 9), display);
						LOAD_SYMBOL(SYMBOL(Slisp, 203), STACK(base, 9));	/* MULTI-CHAR-WILD */
						COPY(STACK(base, 3), STACK(base, 10));
						add_q(STACK(base, 9));
						COPY(STACK(base, 7), STACK(base, 9));
						F1plus(STACK(base, 9));
						COPY(STACK(base, 9), STACK(base, 7));
					}
					else
					{
						COPY(STACK(base, 8), STACK(base, 9));
						LOAD_CHAR('[', STACK(base, 10));
						FcharE(STACK(base, 9), 2);
						if(CL_TRUEP(STACK(base, 9)))
						{
							display[0] = STACK(base, 0);
							Z65_flush_pending_regulars(STACK(base, 9), display);
							LOAD_CHAR(']', STACK(base, 9));
							COPY(STACK(base, 0), STACK(base, 10));
							LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 11));	/* START */
							COPY(STACK(base, 7), STACK(base, 12));
							LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 13));	/* END */
							COPY(STACK(base, 2), STACK(base, 14));
							Fposition(STACK(base, 9), 6);
							if(CL_TRUEP(STACK(base, 9)))
							{
							}
							else
							{
								LOAD_SYMBOL(SYMBOL(Slisp, 225), STACK(base, 10));	/* NAMESTRING-PARSE-ERROR */
								LOAD_SYMBOL(SYMBOL(Slisp, 226), STACK(base, 11));	/* COMPLAINT */
								LOAD_SMSTR((CL_FORM *)&Kmaybe_make_pattern[0], STACK(base, 12));	/* ``['' with no corresponding ``]'' */
								LOAD_SYMBOL(SYMBOL(Slisp, 227), STACK(base, 13));	/* NAMESTRING */
								COPY(STACK(base, 0), STACK(base, 14));
								LOAD_SYMBOL(SYMBOL(Slisp, 228), STACK(base, 15));	/* OFFSET */
								COPY(STACK(base, 7), STACK(base, 16));
								Ferror(STACK(base, 10), 7);
							}
							LOAD_SYMBOL(SYMBOL(Slisp, 196), STACK(base, 10));	/* CHARACTER-SET */
							COPY(STACK(base, 0), STACK(base, 11));
							COPY(STACK(base, 7), STACK(base, 12));
							F1plus(STACK(base, 12));
							COPY(STACK(base, 9), STACK(base, 13));
							Fsubseq(STACK(base, 11), 3);
							Flist(STACK(base, 10), 2);
							COPY(STACK(base, 3), STACK(base, 11));
							add_q(STACK(base, 10));
							COPY(STACK(base, 9), STACK(base, 10));
							F1plus(STACK(base, 10));
							COPY(STACK(base, 10), STACK(base, 7));
						}
						else
						{
							if(CL_TRUEP(STACK(base, 6)))
							{
							}
							else
							{
								COPY(STACK(base, 7), STACK(base, 6));
							}
							COPY(STACK(base, 7), STACK(base, 9));
							F1plus(STACK(base, 9));
							COPY(STACK(base, 9), STACK(base, 7));
						}
					}
				}
			}
		}
		goto M25_1;
		RETURN26:;
		display[0] = STACK(base, 0);
		Z65_flush_pending_regulars(STACK(base, 8), display);
		COPY(STACK(base, 3), STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 5));
		Fcar(STACK(base, 5));
		COPY(STACK(base, 5), STACK(base, 3));
		if(CL_TRUEP(STACK(base, 3)))
		{
			COPY(STACK(base, 3), STACK(base, 4));
			Fcdr(STACK(base, 4));
			if(CL_TRUEP(STACK(base, 4)))
			{
				bool_result = FALSE;
			}
			else
			{
				COPY(STACK(base, 3), STACK(base, 4));
				Fcar(STACK(base, 4));
				bool_result = CL_SMSTRP(STACK(base, 4));
			}
			if(bool_result)
			{
				COPY(STACK(base, 3), STACK(base, 4));
				Fcar(STACK(base, 4));
				COPY(STACK(base, 4), STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 3), STACK(base, 4));
				make_pattern(STACK(base, 4), 1);
				COPY(STACK(base, 4), STACK(base, 0));
			}
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kmaybe_make_pattern[2], STACK(base, 0));	/*  */
		}
	}
}

static void Z65_flush_pending_regulars(CL_FORM *base, CL_FORM *display[])
{
	if(CL_TRUEP(&display[0][6]))
	{
		if(CL_TRUEP(&display[0][5]))
		{
			COPY(&display[0][0], STACK(base, 0));
			COPY(&display[0][6], STACK(base, 1));
			COPY(&display[0][7], STACK(base, 2));
			remove_backslashes(STACK(base, 0));
		}
		else
		{
			COPY(&display[0][0], STACK(base, 0));
			COPY(&display[0][6], STACK(base, 1));
			COPY(&display[0][7], STACK(base, 2));
			Fsubseq(STACK(base, 0), 3);
		}
		COPY(&display[0][3], STACK(base, 1));
		add_q(STACK(base, 0));
		LOAD_NIL(&display[0][5]);
		LOAD_NIL(&display[0][6]);
		COPY(&display[0][6], STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
