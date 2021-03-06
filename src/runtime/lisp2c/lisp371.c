/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

extern CL_INIT XIpattern_matches[];
extern CL_INIT XFpattern_matches[];
CL_INIT Kpattern_matches[] =
{
	MAKE_STRING(40, "ecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 2 */
};

static void Z126_maybe_prepend(CL_FORM *base);
static void Z127_matches(CL_FORM *base, CL_FORM *display[]);

void pattern_matches(CL_FORM *base)
{
	CL_FORM *display[1];
	COPY(ARG(1), ARG(2));
	Flength(ARG(2));
	COPY(ARG(0), ARG(3));
	pattern_pieces(ARG(3));
	LOAD_SMALLFIXNUM(0, ARG(4));
	LOAD_NIL(ARG(5));
	LOAD_NIL(ARG(6));
	LOAD_NIL(ARG(7));
	display[0] = ARG(0);
	Z127_matches(ARG(3), display);
	MV_TO_STACK(mv_count, 4);
	{
		int nargs;
		nargs = mv_count;
		mv_count = 1;
		{
			switch(nargs)
			{
				case 0:
				LOAD_NIL(ARG(3));
				case 1:
				LOAD_NIL(ARG(4));
				nargs = 2;
			}
			COPY(ARG(3), ARG(5));
			if(CL_LISTP(ARG(4)))
			{
				COPY(ARG(4), ARG(6));
				list_reverse(ARG(6));
			}
			else
			{
				COPY(ARG(4), ARG(6));
				vector_reverse(ARG(6));
			}
			COPY(ARG(5), ARG(0));
			COPY(ARG(6), &mv_buf[0]);
			mv_count = 2;
		}
	}
}

static void Z126_maybe_prepend(CL_FORM *base)
{
	if(CL_TRUEP(ARG(1)))
	{
		COPY(ARG(2), ARG(3));
		Flength(ARG(3));
		COPY(ARG(3), ARG(4));
		LOAD_CHAR(ARG(5), ' ', ARG(5));
		make_string1(ARG(4));
		COPY(ARG(3), ARG(5));
		LOAD_NIL(ARG(6));
		COPY(ARG(2), ARG(7));
		M1_1:;
		if(CL_ATOMP(ARG(7)))
		{
			LOAD_NIL(ARG(6));
			goto RETURN1;
		}
		COPY(ARG(7), ARG(8));
		COPY(GET_CAR(ARG(8)), ARG(6));
		COPY(ARG(5), ARG(8));
		F1minus(ARG(8));
		COPY(ARG(8), ARG(5));
		COPY(ARG(6), ARG(9));
		COPY(ARG(4), ARG(10));
		COPY(ARG(8), ARG(11));
		Fset_schar(ARG(9));
		COPY(ARG(7), ARG(8));
		COPY(GET_CDR(ARG(8)), ARG(7));
		goto M1_1;
		RETURN1:;
		ALLOC_CONS(ARG(8), ARG(4), ARG(0), ARG(0));
	}
	else
	{
	}
}

static void Z127_matches(CL_FORM *base, CL_FORM *display[])
{
	M1_1:;
	if(CL_TRUEP(ARG(0)))
	{
		if(CL_CONSP(ARG(0)))
		{
			COPY(GET_CAR(ARG(0)), ARG(5));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[239], ARG(5));	/* ~a is not a list */
			COPY(ARG(0), ARG(6));
			Ferror(ARG(5), 2);
		}
		if(CL_SMSTRP(ARG(5)))
		{
			COPY(ARG(1), ARG(6));
			COPY(ARG(5), ARG(7));
			Flength(ARG(7));
			Fplus(ARG(6), 2);
			COPY(ARG(6), ARG(7));
			COPY(&display[0][2], ARG(8));
			Fle(ARG(7), 2);
			if(CL_TRUEP(ARG(7)))
			{
				COPY(ARG(5), ARG(7));
				COPY(&display[0][1], ARG(8));
				LOAD_SMALLFIXNUM(0, ARG(9));
				LOAD_NIL(ARG(10));
				COPY(ARG(1), ARG(11));
				COPY(ARG(6), ARG(12));
				stringE1(ARG(7));
				if(CL_TRUEP(ARG(7)))
				{
					if(CL_CONSP(ARG(0)))
					{
						COPY(GET_CDR(ARG(0)), ARG(7));
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&KClisp[241], ARG(7));	/* ~a is not a list */
						COPY(ARG(0), ARG(8));
						Ferror(ARG(7), 2);
					}
					COPY(ARG(2), ARG(8));
					COPY(ARG(3), ARG(9));
					COPY(ARG(4), ARG(10));
					Z126_maybe_prepend(ARG(8));
					COPY(ARG(7), ARG(0));
					COPY(ARG(6), ARG(1));
					COPY(ARG(8), ARG(2));
					LOAD_NIL(ARG(3));
					LOAD_NIL(ARG(4));
					goto M1_1;
				}
				else
				{
					LOAD_NIL(ARG(0));
				}
			}
			else
			{
				LOAD_NIL(ARG(0));
			}
		}
		else
		{
			if(CL_LISTP(ARG(5)))
			{
				if(CL_CONSP(ARG(5)))
				{
					COPY(GET_CAR(ARG(5)), ARG(6));
				}
				else
				{
					if(CL_TRUEP(ARG(5)))
					{
						LOAD_SMSTR((CL_FORM *)&KClisp[239], ARG(6));	/* ~a is not a list */
						COPY(ARG(5), ARG(7));
						Ferror(ARG(6), 2);
					}
					else
					{
						COPY(ARG(5), ARG(6));
					}
				}
				if(CL_SYMBOLP(ARG(6)) && GET_SYMBOL(ARG(6)) == SYMBOL(Slisp, 255))	/* CHARACTER-SET */
				{
					COPY(ARG(1), ARG(7));
					COPY(&display[0][2], ARG(8));
					Flt(ARG(7), 2);
					if(CL_TRUEP(ARG(7)))
					{
						COPY(&display[0][1], ARG(7));
						COPY(ARG(1), ARG(8));
						Fschar(ARG(7));
						if(CL_CONSP(ARG(5)))
						{
							COPY(GET_CDR(ARG(5)), ARG(8));
						}
						else
						{
							if(CL_TRUEP(ARG(5)))
							{
								LOAD_SMSTR((CL_FORM *)&KClisp[241], ARG(8));	/* ~a is not a list */
								COPY(ARG(5), ARG(9));
								Ferror(ARG(8), 2);
							}
							else
							{
								COPY(ARG(5), ARG(8));
							}
						}
						LOAD_GLOBFUN(&CFcharE, ARG(9));
						LOAD_NIL(ARG(10));
						LOAD_NIL(ARG(11));
						LOAD_SMALLFIXNUM(0, ARG(12));
						COPY(SYMVAL(Slisp, 0), ARG(13));	/* MOST-POSITIVE-FIXNUM */
						LOAD_NIL(ARG(14));
						if(CL_LISTP(ARG(8)))
						{
							COPY(ARG(7), ARG(15));
							COPY(ARG(8), ARG(16));
							LOAD_NIL(ARG(17));
							LOAD_GLOBFUN(&CFcharE, ARG(18));
							LOAD_NIL(ARG(19));
							COPY(ARG(12), ARG(20));
							COPY(ARG(13), ARG(21));
							LOAD_NIL(ARG(22));
							list_find(ARG(15));
							bool_result = CL_TRUEP(ARG(15));
						}
						else
						{
							COPY(ARG(7), ARG(15));
							COPY(ARG(8), ARG(16));
							LOAD_NIL(ARG(17));
							LOAD_GLOBFUN(&CFcharE, ARG(18));
							LOAD_NIL(ARG(19));
							COPY(ARG(12), ARG(20));
							COPY(ARG(13), ARG(21));
							LOAD_NIL(ARG(22));
							vector_find(ARG(15));
							bool_result = CL_TRUEP(ARG(15));
						}
						if(bool_result)
						{
							if(CL_CONSP(ARG(0)))
							{
								COPY(GET_CDR(ARG(0)), ARG(8));
							}
							else
							{
								LOAD_SMSTR((CL_FORM *)&KClisp[241], ARG(8));	/* ~a is not a list */
								COPY(ARG(0), ARG(9));
								Ferror(ARG(8), 2);
							}
							COPY(ARG(1), ARG(9));
							F1plus(ARG(9));
							ALLOC_CONS(ARG(12), ARG(7), ARG(4), ARG(10));
							COPY(ARG(8), ARG(0));
							COPY(ARG(9), ARG(1));
							LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(3));	/* T */
							COPY(ARG(10), ARG(4));
							goto M1_1;
						}
						else
						{
							LOAD_NIL(ARG(0));
						}
					}
					else
					{
						LOAD_NIL(ARG(0));
					}
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kpattern_matches[0], ARG(0));	/* ecase: the value ~a is not a legal value */
					COPY(ARG(6), ARG(1));
					Ferror(ARG(0), 2);
				}
			}
			else
			{
				if(CL_SYMBOLP(ARG(5)) && GET_SYMBOL(ARG(5)) == SYMBOL(Slisp, 273))	/* SINGLE-CHAR-WILD */
				{
					COPY(ARG(1), ARG(6));
					COPY(&display[0][2], ARG(7));
					Flt(ARG(6), 2);
					if(CL_TRUEP(ARG(6)))
					{
						if(CL_CONSP(ARG(0)))
						{
							COPY(GET_CDR(ARG(0)), ARG(6));
						}
						else
						{
							LOAD_SMSTR((CL_FORM *)&KClisp[241], ARG(6));	/* ~a is not a list */
							COPY(ARG(0), ARG(7));
							Ferror(ARG(6), 2);
						}
						COPY(ARG(1), ARG(7));
						F1plus(ARG(7));
						COPY(&display[0][1], ARG(8));
						COPY(ARG(1), ARG(9));
						Fschar(ARG(8));
						ALLOC_CONS(ARG(10), ARG(8), ARG(4), ARG(8));
						COPY(ARG(6), ARG(0));
						COPY(ARG(7), ARG(1));
						LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(3));	/* T */
						COPY(ARG(8), ARG(4));
						goto M1_1;
					}
					else
					{
						LOAD_NIL(ARG(0));
					}
				}
				else
				{
					if(CL_SYMBOLP(ARG(5)) && GET_SYMBOL(ARG(5)) == SYMBOL(Slisp, 263))	/* MULTI-CHAR-WILD */
					{
						if(CL_CONSP(ARG(0)))
						{
							COPY(GET_CDR(ARG(0)), ARG(6));
						}
						else
						{
							LOAD_SMSTR((CL_FORM *)&KClisp[241], ARG(6));	/* ~a is not a list */
							COPY(ARG(0), ARG(7));
							Ferror(ARG(6), 2);
						}
						COPY(ARG(1), ARG(7));
						COPY(ARG(2), ARG(8));
						LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(9));	/* T */
						COPY(ARG(4), ARG(10));
						Z127_matches(ARG(6), display);
						MV_TO_STACK(mv_count, 7);
						{
							int nargs;
							nargs = mv_count;
							mv_count = 1;
							{
								switch(nargs)
								{
									case 0:
									LOAD_NIL(ARG(6));
									case 1:
									LOAD_NIL(ARG(7));
									nargs = 2;
								}
								if(CL_TRUEP(ARG(6)))
								{
									LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(0));	/* T */
									COPY(ARG(7), ARG(1));
									COPY(ARG(1), &mv_buf[0]);
									mv_count = 2;
								}
								else
								{
									COPY(ARG(1), ARG(8));
									COPY(&display[0][2], ARG(9));
									Flt(ARG(8), 2);
									if(CL_TRUEP(ARG(8)))
									{
										COPY(&display[0][1], ARG(8));
										COPY(ARG(1), ARG(9));
										Fschar(ARG(8));
										ALLOC_CONS(ARG(10), ARG(8), ARG(4), ARG(8));
										COPY(ARG(1), ARG(9));
										F1plus(ARG(9));
										COPY(ARG(9), ARG(1));
										LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(3));	/* T */
										COPY(ARG(8), ARG(4));
										goto M1_1;
									}
									else
									{
										LOAD_NIL(ARG(0));
									}
								}
							}
						}
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&Kpattern_matches[2], ARG(0));	/* etypecase: the value ~a is not a legal value */
						COPY(ARG(5), ARG(1));
						Ferror(ARG(0), 2);
					}
				}
			}
		}
	}
	else
	{
		COPY(ARG(1), ARG(5));
		COPY(&display[0][2], ARG(6));
		Fnumeql(ARG(5), 2);
		if(CL_TRUEP(ARG(5)))
		{
			LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(0));	/* T */
			COPY(ARG(2), ARG(1));
			COPY(ARG(3), ARG(2));
			COPY(ARG(4), ARG(3));
			Z126_maybe_prepend(ARG(1));
			COPY(ARG(1), &mv_buf[0]);
			mv_count = 2;
		}
		else
		{
			LOAD_NIL(ARG(0));
			LOAD_NIL(ARG(1));
			COPY(ARG(1), &mv_buf[0]);
			mv_count = 2;
		}
	}
	goto RETURN1;
	RETURN1:;
}
