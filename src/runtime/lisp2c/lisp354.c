#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z133_sort(CL_FORM *base, CL_FORM *display[]);

void list_merge_sort(CL_FORM *base)
{
	CL_FORM *display[1];
	COPY(STACK(base, 0), STACK(base, 3));
	display[0] = STACK(base, 0);
	Z133_sort(STACK(base, 3), display);
	COPY(STACK(base, 3), STACK(base, 0));
}

static void Z133_sort(CL_FORM *base, CL_FORM *display[])
{
	COPY(STACK(base, 0), STACK(base, 1));
	Flength(STACK(base, 1));
	LOAD_NIL(STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	LOAD_NIL(STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	LOAD_FIXNUM(2, STACK(base, 9));
	Flt(STACK(base, 8), 2);
	if(CL_TRUEP(STACK(base, 8)))
	{
	}
	else
	{
		COPY(STACK(base, 1), STACK(base, 8));
		LOAD_FIXNUM(2, STACK(base, 9));
		Fnumeql(STACK(base, 8), 2);
		if(CL_TRUEP(STACK(base, 8)))
		{
			COPY(&display[0][2], STACK(base, 8));
			COPY(STACK(base, 0), STACK(base, 9));
			Fcar(STACK(base, 9));
			Ffuncall(STACK(base, 8), 2);
			mv_count = 1;
			COPY(STACK(base, 8), STACK(base, 6));
			COPY(&display[0][2], STACK(base, 8));
			COPY(STACK(base, 0), STACK(base, 9));
			Fcdr(STACK(base, 9));
			Fcar(STACK(base, 9));
			Ffuncall(STACK(base, 8), 2);
			mv_count = 1;
			COPY(STACK(base, 8), STACK(base, 7));
			COPY(&display[0][1], STACK(base, 8));
			COPY(STACK(base, 6), STACK(base, 9));
			COPY(STACK(base, 7), STACK(base, 10));
			Ffuncall(STACK(base, 8), 3);
			mv_count = 1;
			if(CL_TRUEP(STACK(base, 8)))
			{
				bool_result = CL_TRUEP(STACK(base, 8));
			}
			else
			{
				COPY(&display[0][1], STACK(base, 9));
				COPY(STACK(base, 7), STACK(base, 10));
				COPY(STACK(base, 6), STACK(base, 11));
				Ffuncall(STACK(base, 9), 3);
				mv_count = 1;
				bool_result = NOT(CL_TRUEP(STACK(base, 9)));
			}
			if(bool_result)
			{
			}
			else
			{
				Fnreverse(STACK(base, 0));
			}
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 8));
			COPY(STACK(base, 8), STACK(base, 9));
			LOAD_FIXNUM(2, STACK(base, 10));
			floor_internal(STACK(base, 9));
			COPY(STACK(base, 9), STACK(base, 1));
			LOAD_FIXNUM(1, STACK(base, 8));
			COPY(STACK(base, 0), STACK(base, 9));
			M189_1:;
			COPY(STACK(base, 8), STACK(base, 10));
			COPY(STACK(base, 1), STACK(base, 11));
			Fge(STACK(base, 10), 2);
			if(CL_TRUEP(STACK(base, 10)))
			{
				COPY(STACK(base, 0), STACK(base, 2));
				COPY(STACK(base, 9), STACK(base, 10));
				Fcdr(STACK(base, 10));
				COPY(STACK(base, 10), STACK(base, 3));
				COPY(STACK(base, 9), STACK(base, 10));
				LOAD_NIL(STACK(base, 11));
				Frplacd(STACK(base, 10));
				goto RETURN207;
			}
			COPY(STACK(base, 8), STACK(base, 10));
			F1plus(STACK(base, 10));
			COPY(STACK(base, 9), STACK(base, 11));
			Fcdr(STACK(base, 11));
			COPY(STACK(base, 11), STACK(base, 9));
			COPY(STACK(base, 10), STACK(base, 8));
			goto M189_1;
			RETURN207:;
			COPY(STACK(base, 2), STACK(base, 8));
			Z133_sort(STACK(base, 8), display);
			COPY(STACK(base, 8), STACK(base, 2));
			COPY(STACK(base, 3), STACK(base, 8));
			Z133_sort(STACK(base, 8), display);
			COPY(STACK(base, 8), STACK(base, 3));
			COPY(STACK(base, 2), STACK(base, 8));
			Fendp(STACK(base, 8));
			if(CL_TRUEP(STACK(base, 8)))
			{
				COPY(STACK(base, 3), STACK(base, 0));
			}
			else
			{
				COPY(STACK(base, 3), STACK(base, 8));
				Fendp(STACK(base, 8));
				if(CL_TRUEP(STACK(base, 8)))
				{
					COPY(STACK(base, 2), STACK(base, 0));
				}
				else
				{
					LOAD_NIL(STACK(base, 8));
					LOAD_NIL(STACK(base, 9));
					{
						CL_FORM *lptr;
						lptr = form_alloc(STACK(base, 10), 2);
						COPY(STACK(base, 8), CAR(lptr));
						COPY(STACK(base, 9), CDR(lptr));
						LOAD_CONS(lptr, STACK(base, 4));
					}
					COPY(STACK(base, 4), STACK(base, 5));
					COPY(&display[0][2], STACK(base, 8));
					COPY(STACK(base, 2), STACK(base, 9));
					Fcar(STACK(base, 9));
					Ffuncall(STACK(base, 8), 2);
					mv_count = 1;
					COPY(STACK(base, 8), STACK(base, 6));
					COPY(&display[0][2], STACK(base, 7));
					COPY(STACK(base, 3), STACK(base, 8));
					Fcar(STACK(base, 8));
					Ffuncall(STACK(base, 7), 2);
					mv_count = 1;
					M190_1:;
					COPY(&display[0][1], STACK(base, 8));
					COPY(STACK(base, 6), STACK(base, 9));
					COPY(STACK(base, 7), STACK(base, 10));
					Ffuncall(STACK(base, 8), 3);
					mv_count = 1;
					if(CL_TRUEP(STACK(base, 8)))
					{
						bool_result = CL_TRUEP(STACK(base, 8));
					}
					else
					{
						COPY(&display[0][1], STACK(base, 9));
						COPY(STACK(base, 7), STACK(base, 10));
						COPY(STACK(base, 6), STACK(base, 11));
						Ffuncall(STACK(base, 9), 3);
						mv_count = 1;
						bool_result = NOT(CL_TRUEP(STACK(base, 9)));
					}
					if(bool_result)
					{
						COPY(STACK(base, 5), STACK(base, 8));
						COPY(STACK(base, 2), STACK(base, 9));
						Frplacd(STACK(base, 8));
						COPY(STACK(base, 5), STACK(base, 8));
						Fcdr(STACK(base, 8));
						COPY(STACK(base, 8), STACK(base, 5));
						COPY(STACK(base, 2), STACK(base, 8));
						Fcdr(STACK(base, 8));
						COPY(STACK(base, 8), STACK(base, 2));
						Fendp(STACK(base, 8));
						if(CL_TRUEP(STACK(base, 8)))
						{
							COPY(STACK(base, 5), STACK(base, 8));
							COPY(STACK(base, 3), STACK(base, 9));
							Frplacd(STACK(base, 8));
							COPY(STACK(base, 4), STACK(base, 0));
							Fcdr(STACK(base, 0));
							goto RETURN208;
						}
						COPY(&display[0][2], STACK(base, 8));
						COPY(STACK(base, 2), STACK(base, 9));
						Fcar(STACK(base, 9));
						Ffuncall(STACK(base, 8), 2);
						mv_count = 1;
						COPY(STACK(base, 8), STACK(base, 6));
					}
					else
					{
						COPY(STACK(base, 5), STACK(base, 8));
						COPY(STACK(base, 3), STACK(base, 9));
						Frplacd(STACK(base, 8));
						COPY(STACK(base, 5), STACK(base, 8));
						Fcdr(STACK(base, 8));
						COPY(STACK(base, 8), STACK(base, 5));
						COPY(STACK(base, 3), STACK(base, 8));
						Fcdr(STACK(base, 8));
						COPY(STACK(base, 8), STACK(base, 3));
						Fendp(STACK(base, 8));
						if(CL_TRUEP(STACK(base, 8)))
						{
							COPY(STACK(base, 5), STACK(base, 8));
							COPY(STACK(base, 2), STACK(base, 9));
							Frplacd(STACK(base, 8));
							COPY(STACK(base, 4), STACK(base, 0));
							Fcdr(STACK(base, 0));
							goto RETURN208;
						}
						COPY(&display[0][2], STACK(base, 7));
						COPY(STACK(base, 3), STACK(base, 8));
						Fcar(STACK(base, 8));
						Ffuncall(STACK(base, 7), 2);
						mv_count = 1;
					}
					goto M190_1;
					RETURN208:;
				}
			}
		}
	}
}
