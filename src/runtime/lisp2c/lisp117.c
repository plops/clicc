#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cparse_unix_namestring = {parse_unix_namestring, 3};

CL_INIT2(Kparse_unix_namestring)
{
	MAKE_STRING(28, "search lists not implemented"),	/* 0 */
	MAKE_STRING(2, ".."),	/* 2 */
};

void parse_unix_namestring(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 3), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fnumneql(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 0), STACK(base, 4));
		COPY(STACK(base, 3), STACK(base, 5));
		Fschar(STACK(base, 4));
		LOAD_CHAR('/', STACK(base, 5));
		FcharE(STACK(base, 4), 2);
	}
	else
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(STACK(base, 3), STACK(base, 5));
		F1plus(STACK(base, 5));
		COPY(STACK(base, 5), STACK(base, 3));
	}
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 7), 2);
		COPY(STACK(base, 5), CAR(lptr));
		COPY(STACK(base, 6), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 5));
	}
	M26_1:;
	LOAD_CHAR('/', STACK(base, 6));
	COPY(STACK(base, 0), STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 8));	/* START */
	COPY(STACK(base, 3), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 10));	/* END */
	COPY(STACK(base, 2), STACK(base, 11));
	Fposition(STACK(base, 6), 6);
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 6), STACK(base, 7));
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 7));
	}
	{
		CL_FORM *lptr;
		lptr = form_alloc(STACK(base, 8), 2);
		COPY(STACK(base, 3), CAR(lptr));
		COPY(STACK(base, 7), CDR(lptr));
		LOAD_CONS(lptr, STACK(base, 7));
	}
	COPY(STACK(base, 5), STACK(base, 8));
	add_q(STACK(base, 7));
	if(CL_TRUEP(STACK(base, 6)))
	{
	}
	else
	{
		goto RETURN27;
	}
	COPY(STACK(base, 6), STACK(base, 7));
	F1plus(STACK(base, 7));
	COPY(STACK(base, 7), STACK(base, 3));
	goto M26_1;
	RETURN27:;
	COPY(STACK(base, 4), STACK(base, 6));
	COPY(STACK(base, 5), STACK(base, 7));
	Fcar(STACK(base, 7));
	COPY(STACK(base, 6), STACK(base, 3));
	COPY(STACK(base, 7), &mv_buf[0]);
	mv_count = 2;
	COPY(&mv_buf[0], STACK(base, 4));
	{
		int nargs;
		nargs = 2;
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
		if(CL_TRUEP(STACK(base, 3)))
		{
			LOAD_NIL(STACK(base, 6));
		}
		else
		{
			COPY(STACK(base, 4), STACK(base, 6));
			Fcar(STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 7));
			Fcar(STACK(base, 7));
			COPY(STACK(base, 6), STACK(base, 8));
			Fcdr(STACK(base, 8));
			LOAD_NIL(STACK(base, 9));
			COPY(STACK(base, 7), STACK(base, 10));
			M27_1:;
			COPY(STACK(base, 10), STACK(base, 11));
			COPY(STACK(base, 8), STACK(base, 12));
			Fnumeql(STACK(base, 11), 2);
			if(CL_TRUEP(STACK(base, 11)))
			{
				LOAD_NIL(STACK(base, 11));
				COPY(STACK(base, 7), STACK(base, 12));
				COPY(STACK(base, 11), STACK(base, 7));
				COPY(STACK(base, 12), &mv_buf[0]);
				mv_count = 2;
				goto RETURN28;
			}
			if(CL_TRUEP(STACK(base, 9)))
			{
				LOAD_NIL(STACK(base, 9));
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 11));
				COPY(STACK(base, 10), STACK(base, 12));
				Fschar(STACK(base, 11));
				if(CL_CHARP(STACK(base, 11)) && GET_CHAR(STACK(base, 11)) == '\\')
				{
					LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 9));	/* T */
				}
				else
				{
					if(CL_CHARP(STACK(base, 11)) && GET_CHAR(STACK(base, 11)) == ':')
					{
						COPY(STACK(base, 0), STACK(base, 12));
						COPY(STACK(base, 7), STACK(base, 13));
						COPY(STACK(base, 10), STACK(base, 14));
						remove_backslashes(STACK(base, 12));
						COPY(STACK(base, 10), STACK(base, 13));
						F1plus(STACK(base, 13));
						COPY(STACK(base, 12), STACK(base, 7));
						COPY(STACK(base, 13), &mv_buf[0]);
						mv_count = 2;
						goto RETURN28;
					}
				}
			}
			F1plus(STACK(base, 10));
			goto M27_1;
			RETURN28:;
			COPY(&mv_buf[0], STACK(base, 8));
			{
				int nargs;
				nargs = 2;
				mv_count = 1;
				switch(nargs)
				{
					case 0:
					LOAD_NIL(STACK(base, 7));
					case 1:
					LOAD_NIL(STACK(base, 8));
					nargs = 2;
				}
				Flist(STACK(base, 9), nargs - 2);
				if(CL_TRUEP(STACK(base, 7)))
				{
					LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
					COPY(STACK(base, 6), STACK(base, 10));
					COPY(STACK(base, 8), STACK(base, 11));
					if(CL_CONSP(STACK(base, 6)))
					{
						COPY(STACK(base, 8), CAR(GET_FORM(STACK(base, 6))));
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&KClisp[12], STACK(base, 12));	/* ~a is not a cons */
						COPY(STACK(base, 6), STACK(base, 13));
						Ferror(STACK(base, 12), 2);
					}
				}
				COPY(STACK(base, 7), STACK(base, 6));
			}
		}
		COPY(STACK(base, 4), STACK(base, 7));
		Flast(STACK(base, 7), 1);
		Fcar(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 8));
		Fcar(STACK(base, 8));
		COPY(STACK(base, 7), STACK(base, 9));
		Fcdr(STACK(base, 9));
		COPY(STACK(base, 8), STACK(base, 10));
		COPY(STACK(base, 9), STACK(base, 11));
		Fnumeql(STACK(base, 10), 2);
		if(CL_TRUEP(STACK(base, 10)))
		{
			LOAD_NIL(STACK(base, 7));
		}
		else
		{
			COPY(STACK(base, 4), STACK(base, 10));
			Fbutlast(STACK(base, 10), 1);
			COPY(STACK(base, 10), STACK(base, 4));
			LOAD_CHAR('.', STACK(base, 10));
			COPY(STACK(base, 0), STACK(base, 11));
			LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 12));	/* START */
			COPY(STACK(base, 8), STACK(base, 13));
			F1plus(STACK(base, 13));
			LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 14));	/* END */
			COPY(STACK(base, 9), STACK(base, 15));
			LOAD_SYMBOL(SYMBOL(Slisp, 229), STACK(base, 16));	/* FROM-END */
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 17));	/* T */
			Fposition(STACK(base, 10), 8);
			if(CL_TRUEP(STACK(base, 10)))
			{
				LOAD_CHAR('.', STACK(base, 11));
				COPY(STACK(base, 0), STACK(base, 12));
				LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 13));	/* START */
				COPY(STACK(base, 8), STACK(base, 14));
				F1plus(STACK(base, 14));
				LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 15));	/* END */
				COPY(STACK(base, 10), STACK(base, 16));
				LOAD_SYMBOL(SYMBOL(Slisp, 229), STACK(base, 17));	/* FROM-END */
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 18));	/* T */
				Fposition(STACK(base, 11), 8);
			}
			else
			{
				LOAD_NIL(STACK(base, 11));
			}
			LOAD_SYMBOL(SYMBOL(Slisp, 200), STACK(base, 12));	/* NEWEST */
			if(CL_TRUEP(STACK(base, 11)))
			{
				COPY(STACK(base, 10), STACK(base, 13));
				LOAD_FIXNUM(2, STACK(base, 14));
				Fplus(STACK(base, 13), 2);
				COPY(STACK(base, 9), STACK(base, 14));
				Fnumeql(STACK(base, 13), 2);
				if(CL_TRUEP(STACK(base, 13)))
				{
					COPY(STACK(base, 0), STACK(base, 13));
					COPY(STACK(base, 10), STACK(base, 14));
					F1plus(STACK(base, 14));
					Fschar(STACK(base, 13));
					LOAD_CHAR('*', STACK(base, 14));
					FcharE(STACK(base, 13), 2);
					bool_result = CL_TRUEP(STACK(base, 13));
				}
				else
				{
					bool_result = FALSE;
				}
				if(bool_result)
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 202), STACK(base, 12));	/* WILD */
				}
				else
				{
					COPY(STACK(base, 10), STACK(base, 13));
					F1plus(STACK(base, 13));
					COPY(STACK(base, 9), STACK(base, 14));
					Flt(STACK(base, 13), 2);
					if(CL_TRUEP(STACK(base, 13)))
					{
						COPY(STACK(base, 10), STACK(base, 13));
						F1plus(STACK(base, 13));
						M28_1:;
						COPY(STACK(base, 13), STACK(base, 14));
						COPY(STACK(base, 9), STACK(base, 15));
						Fnumeql(STACK(base, 14), 2);
						if(CL_TRUEP(STACK(base, 14)))
						{
							bool_result = TRUE;
							goto RETURN29;
						}
						LOAD_CHAR('0', STACK(base, 14));
						COPY(STACK(base, 0), STACK(base, 15));
						COPY(STACK(base, 13), STACK(base, 16));
						Fschar(STACK(base, 15));
						LOAD_CHAR('9', STACK(base, 16));
						FcharLE(STACK(base, 14), 3);
						if(CL_TRUEP(STACK(base, 14)))
						{
						}
						else
						{
							bool_result = FALSE;
							goto RETURN29;
						}
						F1plus(STACK(base, 13));
						goto M28_1;
						RETURN29:;
					}
					else
					{
						bool_result = FALSE;
					}
					if(bool_result)
					{
						COPY(STACK(base, 0), STACK(base, 12));
						LOAD_SYMBOL(SYMBOL(Slisp, 161), STACK(base, 13));	/* START */
						COPY(STACK(base, 10), STACK(base, 14));
						F1plus(STACK(base, 14));
						LOAD_SYMBOL(SYMBOL(Slisp, 162), STACK(base, 15));	/* END */
						COPY(STACK(base, 9), STACK(base, 16));
						Fparse_integer(STACK(base, 12), 5);
						mv_count = 1;
					}
					else
					{
						LOAD_NIL(STACK(base, 11));
					}
				}
			}
			if(CL_TRUEP(STACK(base, 11)))
			{
				COPY(STACK(base, 0), STACK(base, 13));
				COPY(STACK(base, 8), STACK(base, 14));
				COPY(STACK(base, 11), STACK(base, 15));
				maybe_make_pattern(STACK(base, 13));
				COPY(STACK(base, 0), STACK(base, 14));
				COPY(STACK(base, 11), STACK(base, 15));
				F1plus(STACK(base, 15));
				COPY(STACK(base, 10), STACK(base, 16));
				maybe_make_pattern(STACK(base, 14));
				COPY(STACK(base, 12), STACK(base, 15));
				COPY(STACK(base, 13), STACK(base, 7));
				COPY(STACK(base, 14), &mv_buf[0]);
				COPY(STACK(base, 15), &mv_buf[1]);
				mv_count = 3;
			}
			else
			{
				if(CL_TRUEP(STACK(base, 10)))
				{
					COPY(STACK(base, 0), STACK(base, 13));
					COPY(STACK(base, 8), STACK(base, 14));
					COPY(STACK(base, 10), STACK(base, 15));
					maybe_make_pattern(STACK(base, 13));
					COPY(STACK(base, 0), STACK(base, 14));
					COPY(STACK(base, 10), STACK(base, 15));
					F1plus(STACK(base, 15));
					COPY(STACK(base, 9), STACK(base, 16));
					maybe_make_pattern(STACK(base, 14));
					COPY(STACK(base, 12), STACK(base, 15));
					COPY(STACK(base, 13), STACK(base, 7));
					COPY(STACK(base, 14), &mv_buf[0]);
					COPY(STACK(base, 15), &mv_buf[1]);
					mv_count = 3;
				}
				else
				{
					COPY(STACK(base, 0), STACK(base, 13));
					COPY(STACK(base, 8), STACK(base, 14));
					COPY(STACK(base, 9), STACK(base, 15));
					maybe_make_pattern(STACK(base, 13));
					LOAD_NIL(STACK(base, 14));
					COPY(STACK(base, 12), STACK(base, 15));
					COPY(STACK(base, 13), STACK(base, 7));
					COPY(STACK(base, 14), &mv_buf[0]);
					COPY(STACK(base, 15), &mv_buf[1]);
					mv_count = 3;
				}
			}
		}
		MV_TO_STACK(mv_count, 8);
		{
			int nargs;
			nargs = mv_count;
			mv_count = 1;
			switch(nargs)
			{
				case 0:
				LOAD_NIL(STACK(base, 7));
				case 1:
				LOAD_NIL(STACK(base, 8));
				case 2:
				LOAD_NIL(STACK(base, 9));
				nargs = 3;
			}
			Flist(STACK(base, 10), nargs - 3);
			LOAD_NIL(STACK(base, 11));
			LOAD_NIL(STACK(base, 12));
			LOAD_NIL(STACK(base, 13));
			LOAD_NIL(STACK(base, 14));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 15), 2);
				COPY(STACK(base, 13), CAR(lptr));
				COPY(STACK(base, 14), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 13));
			}
			if(CL_TRUEP(STACK(base, 6)))
			{
				LOAD_SMSTR((CL_FORM *)&Kparse_unix_namestring[0], STACK(base, 14));	/* search lists not implemented */
				Ferror(STACK(base, 14), 1);
			}
			LOAD_NIL(STACK(base, 14));
			COPY(STACK(base, 4), STACK(base, 15));
			M29_1:;
			if(CL_ATOMP(STACK(base, 15)))
			{
				LOAD_NIL(STACK(base, 14));
				goto RETURN30;
			}
			COPY(STACK(base, 15), STACK(base, 16));
			Fcar(STACK(base, 16));
			COPY(STACK(base, 16), STACK(base, 14));
			Fcar(STACK(base, 16));
			COPY(STACK(base, 14), STACK(base, 17));
			Fcdr(STACK(base, 17));
			COPY(STACK(base, 16), STACK(base, 18));
			COPY(STACK(base, 17), STACK(base, 19));
			Fnumeql(STACK(base, 18), 2);
			if(CL_TRUEP(STACK(base, 18)))
			{
			}
			else
			{
				COPY(STACK(base, 0), STACK(base, 18));
				COPY(STACK(base, 16), STACK(base, 19));
				COPY(STACK(base, 17), STACK(base, 20));
				maybe_make_pattern(STACK(base, 18));
				if(CL_SMSTRP(STACK(base, 18)))
				{
					COPY(STACK(base, 18), STACK(base, 19));
					LOAD_SMSTR((CL_FORM *)&Kparse_unix_namestring[2], STACK(base, 20));	/* .. */
					FstringE(STACK(base, 19), 2);
					bool_result = CL_TRUEP(STACK(base, 19));
				}
				else
				{
					bool_result = FALSE;
				}
				if(bool_result)
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 205), STACK(base, 19));	/* UP */
					COPY(STACK(base, 13), STACK(base, 20));
					add_q(STACK(base, 19));
				}
				else
				{
					COPY(STACK(base, 18), STACK(base, 19));
					COPY(STACK(base, 13), STACK(base, 20));
					add_q(STACK(base, 19));
				}
			}
			Fcdr(STACK(base, 15));
			goto M29_1;
			RETURN30:;
			COPY(STACK(base, 13), STACK(base, 14));
			COPY(STACK(base, 14), STACK(base, 13));
			Fcar(STACK(base, 13));
			if(CL_TRUEP(STACK(base, 3)))
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 198), STACK(base, 14));	/* ABSOLUTE */
				{
					CL_FORM *lptr;
					lptr = form_alloc(STACK(base, 15), 2);
					COPY(STACK(base, 14), CAR(lptr));
					COPY(STACK(base, 13), CDR(lptr));
					LOAD_CONS(lptr, STACK(base, 13));
				}
			}
			else
			{
				if(CL_TRUEP(STACK(base, 13)))
				{
					LOAD_SYMBOL(SYMBOL(Slisp, 201), STACK(base, 14));	/* RELATIVE */
					{
						CL_FORM *lptr;
						lptr = form_alloc(STACK(base, 15), 2);
						COPY(STACK(base, 14), CAR(lptr));
						COPY(STACK(base, 13), CDR(lptr));
						LOAD_CONS(lptr, STACK(base, 13));
					}
				}
				else
				{
					LOAD_NIL(STACK(base, 13));
				}
			}
			COPY(STACK(base, 7), STACK(base, 14));
			COPY(STACK(base, 8), STACK(base, 15));
			COPY(STACK(base, 9), STACK(base, 16));
			COPY(STACK(base, 11), STACK(base, 0));
			COPY(STACK(base, 12), &mv_buf[0]);
			COPY(STACK(base, 13), &mv_buf[1]);
			COPY(STACK(base, 14), &mv_buf[2]);
			COPY(STACK(base, 15), &mv_buf[3]);
			COPY(STACK(base, 16), &mv_buf[4]);
			mv_count = 6;
		}
	}
}
