#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kpattern_matches)
{
	MAKE_STRING(40, "ecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 2 */
};

static void Z63_maybe_prepend(CL_FORM *base);
static void Z64_matches(CL_FORM *base, CL_FORM *display[]);

void pattern_matches(CL_FORM *base)
{
	CL_FORM *display[1];
	COPY(STACK(base, 1), STACK(base, 2));
	Flength(STACK(base, 2));
	COPY(STACK(base, 0), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 5));	/* PATTERN */
	struct_ref(STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	LOAD_NIL(STACK(base, 7));
	display[0] = STACK(base, 0);
	Z64_matches(STACK(base, 3), display);
	MV_TO_STACK(mv_count, 4);
	{
		int nargs;
		nargs = mv_count;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 3));
			case 1:
			LOAD_NIL(STACK(base, 4));
			nargs = 2;
		}
		Flist(STACK(base, 5), nargs - 2);
		COPY(STACK(base, 3), STACK(base, 6));
		COPY(STACK(base, 4), STACK(base, 7));
		Freverse(STACK(base, 7));
		COPY(STACK(base, 6), STACK(base, 0));
		COPY(STACK(base, 7), &mv_buf[0]);
		mv_count = 2;
	}
}

static void Z63_maybe_prepend(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 2), STACK(base, 3));
		Flength(STACK(base, 3));
		COPY(STACK(base, 3), STACK(base, 4));
		Fmake_string(STACK(base, 4), 1);
		COPY(STACK(base, 3), STACK(base, 5));
		LOAD_NIL(STACK(base, 6));
		COPY(STACK(base, 2), STACK(base, 7));
		M22_1:;
		if(CL_ATOMP(STACK(base, 7)))
		{
			LOAD_NIL(STACK(base, 6));
			goto RETURN23;
		}
		COPY(STACK(base, 7), STACK(base, 8));
		Fcar(STACK(base, 8));
		COPY(STACK(base, 8), STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 8));
		F1minus(STACK(base, 8));
		COPY(STACK(base, 8), STACK(base, 5));
		COPY(STACK(base, 6), STACK(base, 9));
		COPY(STACK(base, 4), STACK(base, 10));
		COPY(STACK(base, 8), STACK(base, 11));
		Fset_schar(STACK(base, 9));
		Fcdr(STACK(base, 7));
		goto M22_1;
		RETURN23:;
		{
			CL_FORM *lptr;
			lptr = form_alloc(STACK(base, 6), 2);
			COPY(STACK(base, 4), CAR(lptr));
			COPY(STACK(base, 0), CDR(lptr));
			LOAD_CONS(lptr, STACK(base, 0));
		}
	}
	else
	{
	}
}

static void Z64_matches(CL_FORM *base, CL_FORM *display[])
{
	if(CL_TRUEP(STACK(base, 0)))
	{
		COPY(STACK(base, 0), STACK(base, 5));
		Fcar(STACK(base, 5));
		COPY(STACK(base, 5), STACK(base, 6));
		LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 7));	/* STANDARD-CHAR */
		LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 8));	/* * */
		check_simple_array(STACK(base, 6));
		if(CL_TRUEP(STACK(base, 6)))
		{
			COPY(STACK(base, 1), STACK(base, 6));
			COPY(STACK(base, 5), STACK(base, 7));
			Flength(STACK(base, 7));
			Fplus(STACK(base, 6), 2);
			COPY(STACK(base, 6), STACK(base, 7));
			COPY(&display[0][2], STACK(base, 8));
			Fle(STACK(base, 7), 2);
			if(CL_TRUEP(STACK(base, 7)))
			{
				COPY(STACK(base, 5), STACK(base, 7));
				COPY(&display[0][1], STACK(base, 8));
				LOAD_SYMBOL(SYMBOL(Slisp, 211), STACK(base, 9));	/* START2 */
				COPY(STACK(base, 1), STACK(base, 10));
				LOAD_SYMBOL(SYMBOL(Slisp, 212), STACK(base, 11));	/* END2 */
				COPY(STACK(base, 6), STACK(base, 12));
				FstringE(STACK(base, 7), 6);
				if(CL_TRUEP(STACK(base, 7)))
				{
					COPY(STACK(base, 0), STACK(base, 7));
					Fcdr(STACK(base, 7));
					COPY(STACK(base, 6), STACK(base, 8));
					COPY(STACK(base, 2), STACK(base, 9));
					COPY(STACK(base, 3), STACK(base, 10));
					COPY(STACK(base, 4), STACK(base, 11));
					Z63_maybe_prepend(STACK(base, 9));
					LOAD_NIL(STACK(base, 10));
					LOAD_NIL(STACK(base, 11));
					Z64_matches(STACK(base, 7), display);
					COPY(STACK(base, 7), STACK(base, 0));
				}
				else
				{
					LOAD_NIL(STACK(base, 0));
				}
			}
			else
			{
				LOAD_NIL(STACK(base, 0));
			}
		}
		else
		{
			if(CL_LISTP(STACK(base, 5)))
			{
				COPY(STACK(base, 5), STACK(base, 6));
				Fcar(STACK(base, 6));
				if(CL_SYMBOLP(STACK(base, 6)) && GET_SYMBOL(STACK(base, 6)) == SYMBOL(Slisp, 196))	/* CHARACTER-SET */
				{
					COPY(STACK(base, 1), STACK(base, 7));
					COPY(&display[0][2], STACK(base, 8));
					Flt(STACK(base, 7), 2);
					if(CL_TRUEP(STACK(base, 7)))
					{
						COPY(&display[0][1], STACK(base, 7));
						COPY(STACK(base, 1), STACK(base, 8));
						Fschar(STACK(base, 7));
						COPY(STACK(base, 7), STACK(base, 8));
						COPY(STACK(base, 5), STACK(base, 9));
						Fcdr(STACK(base, 9));
						LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 10));	/* TEST */
						{
							static GLOBAL_FUNARG extern_closure = {FcharE, -2};
							LOAD_GLOBFUN(&extern_closure, STACK(base, 11));
						}
						Ffind(STACK(base, 8), 4);
						if(CL_TRUEP(STACK(base, 8)))
						{
							COPY(STACK(base, 0), STACK(base, 8));
							Fcdr(STACK(base, 8));
							COPY(STACK(base, 1), STACK(base, 9));
							F1plus(STACK(base, 9));
							COPY(STACK(base, 2), STACK(base, 10));
							LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 11));	/* T */
							{
								CL_FORM *lptr;
								lptr = form_alloc(STACK(base, 12), 2);
								COPY(STACK(base, 7), CAR(lptr));
								COPY(STACK(base, 4), CDR(lptr));
								LOAD_CONS(lptr, STACK(base, 12));
							}
							Z64_matches(STACK(base, 8), display);
							COPY(STACK(base, 8), STACK(base, 0));
						}
						else
						{
							LOAD_NIL(STACK(base, 0));
						}
					}
					else
					{
						LOAD_NIL(STACK(base, 0));
					}
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kpattern_matches[0], STACK(base, 0));	/* ecase: the value ~a is not a legal value */
					COPY(STACK(base, 6), STACK(base, 1));
					Ferror(STACK(base, 0), 2);
				}
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 214))	/* SINGLE-CHAR-WILD */
				{
					COPY(STACK(base, 1), STACK(base, 6));
					COPY(&display[0][2], STACK(base, 7));
					Flt(STACK(base, 6), 2);
					if(CL_TRUEP(STACK(base, 6)))
					{
						COPY(STACK(base, 0), STACK(base, 6));
						Fcdr(STACK(base, 6));
						COPY(STACK(base, 1), STACK(base, 7));
						F1plus(STACK(base, 7));
						COPY(STACK(base, 2), STACK(base, 8));
						LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
						COPY(&display[0][1], STACK(base, 10));
						COPY(STACK(base, 1), STACK(base, 11));
						Fschar(STACK(base, 10));
						{
							CL_FORM *lptr;
							lptr = form_alloc(STACK(base, 11), 2);
							COPY(STACK(base, 10), CAR(lptr));
							COPY(STACK(base, 4), CDR(lptr));
							LOAD_CONS(lptr, STACK(base, 10));
						}
						Z64_matches(STACK(base, 6), display);
						COPY(STACK(base, 6), STACK(base, 0));
					}
					else
					{
						LOAD_NIL(STACK(base, 0));
					}
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 5)) && GET_SYMBOL(STACK(base, 5)) == SYMBOL(Slisp, 203))	/* MULTI-CHAR-WILD */
					{
						COPY(STACK(base, 0), STACK(base, 6));
						Fcdr(STACK(base, 6));
						COPY(STACK(base, 1), STACK(base, 7));
						COPY(STACK(base, 2), STACK(base, 8));
						LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
						COPY(STACK(base, 4), STACK(base, 10));
						Z64_matches(STACK(base, 6), display);
						MV_TO_STACK(mv_count, 7);
						{
							int nargs;
							nargs = mv_count;
							mv_count = 1;
							switch(nargs)
							{
								case 0:
								LOAD_NIL(STACK(base, 6));
								case 1:
								LOAD_NIL(STACK(base, 7));
								nargs = 2;
							}
							Flist(STACK(base, 8), nargs - 2);
							if(CL_TRUEP(STACK(base, 6)))
							{
								LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
								COPY(STACK(base, 7), STACK(base, 10));
								COPY(STACK(base, 9), STACK(base, 0));
								COPY(STACK(base, 10), &mv_buf[0]);
								mv_count = 2;
							}
							else
							{
								COPY(STACK(base, 1), STACK(base, 9));
								COPY(&display[0][2], STACK(base, 10));
								Flt(STACK(base, 9), 2);
								if(CL_TRUEP(STACK(base, 9)))
								{
									COPY(STACK(base, 0), STACK(base, 9));
									COPY(STACK(base, 1), STACK(base, 10));
									F1plus(STACK(base, 10));
									COPY(STACK(base, 2), STACK(base, 11));
									LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 12));	/* T */
									COPY(&display[0][1], STACK(base, 13));
									COPY(STACK(base, 1), STACK(base, 14));
									Fschar(STACK(base, 13));
									{
										CL_FORM *lptr;
										lptr = form_alloc(STACK(base, 14), 2);
										COPY(STACK(base, 13), CAR(lptr));
										COPY(STACK(base, 4), CDR(lptr));
										LOAD_CONS(lptr, STACK(base, 13));
									}
									Z64_matches(STACK(base, 9), display);
									COPY(STACK(base, 9), STACK(base, 0));
								}
								else
								{
									LOAD_NIL(STACK(base, 0));
								}
							}
						}
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&Kpattern_matches[2], STACK(base, 0));	/* etypecase: the value ~a is not a legal value */
						COPY(STACK(base, 5), STACK(base, 1));
						Ferror(STACK(base, 0), 2);
					}
				}
			}
		}
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 5));
		COPY(&display[0][2], STACK(base, 6));
		Fnumeql(STACK(base, 5), 2);
		if(CL_TRUEP(STACK(base, 5)))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
			COPY(STACK(base, 2), STACK(base, 6));
			COPY(STACK(base, 3), STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 8));
			Z63_maybe_prepend(STACK(base, 6));
			COPY(STACK(base, 5), STACK(base, 0));
			COPY(STACK(base, 6), &mv_buf[0]);
			mv_count = 2;
		}
		else
		{
			LOAD_NIL(STACK(base, 5));
			LOAD_NIL(STACK(base, 6));
			COPY(STACK(base, 5), STACK(base, 0));
			COPY(STACK(base, 6), &mv_buf[0]);
			mv_count = 2;
		}
	}
}
