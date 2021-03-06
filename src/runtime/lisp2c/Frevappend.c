/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void Frevappend(CL_FORM *base)
{
	COPY(ARG(0), ARG(2));
	COPY(ARG(1), ARG(3));
	M1_1:;
	if(CL_TRUEP(ARG(2)))
	{
		if(CL_CONSP(ARG(2)))
		{
			goto ELSE1;
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[233], ARG(4));	/* ~a is not a list */
			COPY(ARG(2), ARG(5));
			Ferror(ARG(4), 2);
		}
	}
	else
	{
		goto THEN2;
	}
	{
		THEN2:;
		COPY(ARG(3), ARG(0));
		goto RETURN1;
	}
	ELSE1:;
	COPY(ARG(2), ARG(4));
	if(CL_CONSP(ARG(4)))
	{
		COPY(GET_CDR(ARG(4)), ARG(4));
	}
	else
	{
		if(CL_TRUEP(ARG(4)))
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[241], ARG(5));	/* ~a is not a list */
			COPY(ARG(4), ARG(6));
			Ferror(ARG(5), 2);
		}
		else
		{
			LOAD_NIL(ARG(4));
		}
	}
	COPY(ARG(2), ARG(5));
	if(CL_CONSP(ARG(5)))
	{
		COPY(GET_CAR(ARG(5)), ARG(5));
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
			LOAD_NIL(ARG(5));
		}
	}
	ALLOC_CONS(ARG(7), ARG(5), ARG(3), ARG(3));
	COPY(ARG(4), ARG(2));
	goto M1_1;
	RETURN1:;
}
