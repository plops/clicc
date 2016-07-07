/* This file was generated by CLiCC 0.6.4 [obrep 1] */
#define __OBREP 1
#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT Kpairlis1[] =
{
	MAKE_STRING(49, "The lists of keys and data are of unequal length."),	/* 0 */
};

void pairlis1(CL_FORM *base)
{
	M1_1:;
	if(CL_ATOMP(ARG(0)))
	{
		if(CL_ATOMP(ARG(1)))
		{
			COPY(ARG(2), ARG(0));
			goto RETURN1;
		}
		else
		{
			goto M1_2;
		}
	}
	else
	{
		if(CL_ATOMP(ARG(1)))
		{
			goto M1_2;
		}
		else
		{
			COPY(ARG(0), ARG(3));
			COPY(ARG(3), ARG(4));
			COPY(GET_CAR(ARG(4)), ARG(3));
			COPY(ARG(1), ARG(4));
			COPY(ARG(4), ARG(5));
			COPY(GET_CAR(ARG(5)), ARG(4));
			COPY(ARG(2), ARG(5));
			ALLOC_CONS(ARG(8), ARG(3), ARG(4), ARG(6));
			ALLOC_CONS(ARG(8), ARG(6), ARG(5), ARG(2));
			COPY(GET_CAR(ARG(0)), ARG(3));
			COPY(ARG(0), ARG(4));
			COPY(ARG(4), ARG(5));
			COPY(GET_CDR(ARG(5)), ARG(0));
			COPY(GET_CAR(ARG(1)), ARG(3));
			COPY(ARG(1), ARG(4));
			COPY(ARG(4), ARG(5));
			COPY(GET_CDR(ARG(5)), ARG(1));
		}
	}
	goto M1_1;
	M1_2:;
	LOAD_SMSTR((CL_FORM *)&Kpairlis1[0], ARG(3));	/* The lists of keys and data are of unequal length. */
	Ferror(ARG(3), 1);
	LOAD_NIL(ARG(0));
	RETURN1:;
}