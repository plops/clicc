/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void stringNE1(CL_FORM *base)
{
	COPY(ARG(0), ARG(6));
	Fstring(ARG(6));
	COPY(ARG(6), ARG(0));
	COPY(ARG(1), ARG(6));
	Fstring(ARG(6));
	COPY(ARG(6), ARG(1));
	if(CL_TRUEP(ARG(3)))
	{
	}
	else
	{
		COPY(ARG(0), ARG(6));
		Flength(ARG(6));
		COPY(ARG(6), ARG(3));
	}
	if(CL_TRUEP(ARG(5)))
	{
	}
	else
	{
		COPY(ARG(1), ARG(5));
		Flength(ARG(5));
	}
	COPY(ARG(2), ARG(6));
	COPY(ARG(4), ARG(7));
	LOAD_NIL(ARG(8));
	LOAD_NIL(ARG(9));
	M1_1:;
	COPY(ARG(6), ARG(10));
	COPY(ARG(3), ARG(11));
	Fnumeql(ARG(10), 2);
	if(CL_TRUEP(ARG(10)))
	{
		COPY(ARG(7), ARG(10));
		COPY(ARG(5), ARG(11));
		Fnumeql(ARG(10), 2);
		if(CL_TRUEP(ARG(10)))
		{
			LOAD_NIL(ARG(0));
		}
		else
		{
			COPY(ARG(6), ARG(0));
		}
		goto RETURN2;
	}
	COPY(ARG(7), ARG(10));
	COPY(ARG(5), ARG(11));
	Fnumeql(ARG(10), 2);
	if(CL_TRUEP(ARG(10)))
	{
		COPY(ARG(6), ARG(0));
		goto RETURN2;
	}
	COPY(ARG(0), ARG(10));
	COPY(ARG(6), ARG(11));
	COPY(ARG(10), ARG(12));
	COPY(ARG(11), ARG(13));
	Frow_major_aref(ARG(12));
	COPY(ARG(12), ARG(8));
	COPY(ARG(1), ARG(10));
	COPY(ARG(7), ARG(11));
	COPY(ARG(10), ARG(9));
	COPY(ARG(11), ARG(10));
	Frow_major_aref(ARG(9));
	COPY(ARG(8), ARG(10));
	COPY(ARG(9), ARG(11));
	rt_charE(ARG(10));
	if(CL_TRUEP(ARG(10)))
	{
	}
	else
	{
		COPY(ARG(6), ARG(0));
		goto RETURN1;
	}
	COPY(ARG(6), ARG(10));
	F1plus(ARG(10));
	COPY(ARG(7), ARG(11));
	F1plus(ARG(11));
	COPY(ARG(11), ARG(7));
	COPY(ARG(10), ARG(6));
	goto M1_1;
	RETURN2:;
	RETURN1:;
}
