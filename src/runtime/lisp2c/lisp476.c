/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

static void Z141_lambda(CL_FORM *base);

void rt_do_all_symbols_iterator(CL_FORM *base)
{
	GEN_HEAPVAR(ARG(0), ARG(1));
	LOAD_NIL(ARG(1));
	{
		GEN_CLOSURE(array, ARG(2), 4, Z141_lambda, 1);
		COPY(ARG(0), &array[3]);
		LOAD_CLOSURE(array, ARG(2));
	}
	COPY(ARG(2), ARG(2));
	COPY(SYMVAL(Slisp, 379), ARG(3));	/* *PACKAGE-ARRAY* */
	Fmap(ARG(1), 3);
	COPY(ARG(1), ARG(0));
}

static void Z141_lambda(CL_FORM *base)
{
	COPY(ARG(1), ARG(2));
	Ppackage_external(ARG(2));
	COPY(SYMVAL(Slisp, 372), ARG(3));	/* PACKAGE-HASHTAB-SIZE */
	LOAD_SMALLFIXNUM(0, ARG(4));
	M1_1:;
	COPY(ARG(4), ARG(5));
	COPY(ARG(3), ARG(6));
	Fge(ARG(5), 2);
	if(CL_TRUEP(ARG(5)))
	{
		goto RETURN1;
	}
	LOAD_NIL(ARG(5));
	COPY(ARG(2), ARG(6));
	COPY(ARG(4), ARG(7));
	Fsvref(ARG(6));
	M2_1:;
	if(CL_ATOMP(ARG(6)))
	{
		LOAD_NIL(ARG(5));
		goto RETURN2;
	}
	COPY(ARG(6), ARG(7));
	COPY(GET_CAR(ARG(7)), ARG(5));
	COPY(INDIRECT(GET_FORM(ARG(0)) + 3), ARG(7));
	COPY(ARG(5), ARG(8));
	Ffuncall(ARG(7), 2);
	mv_count = 1;
	COPY(ARG(6), ARG(7));
	COPY(GET_CDR(ARG(7)), ARG(6));
	goto M2_1;
	RETURN2:;
	F1plus(ARG(4));
	goto M1_1;
	RETURN1:;
	COPY(ARG(1), ARG(2));
	Ppackage_internal(ARG(2));
	COPY(SYMVAL(Slisp, 372), ARG(3));	/* PACKAGE-HASHTAB-SIZE */
	LOAD_SMALLFIXNUM(0, ARG(4));
	M3_1:;
	COPY(ARG(4), ARG(5));
	COPY(ARG(3), ARG(6));
	Fge(ARG(5), 2);
	if(CL_TRUEP(ARG(5)))
	{
		LOAD_NIL(ARG(0));
		goto RETURN3;
	}
	LOAD_NIL(ARG(5));
	COPY(ARG(2), ARG(6));
	COPY(ARG(4), ARG(7));
	Fsvref(ARG(6));
	M4_1:;
	if(CL_ATOMP(ARG(6)))
	{
		LOAD_NIL(ARG(5));
		goto RETURN4;
	}
	COPY(ARG(6), ARG(7));
	COPY(GET_CAR(ARG(7)), ARG(5));
	COPY(INDIRECT(GET_FORM(ARG(0)) + 3), ARG(7));
	COPY(ARG(5), ARG(8));
	Ffuncall(ARG(7), 2);
	mv_count = 1;
	COPY(ARG(6), ARG(7));
	COPY(GET_CDR(ARG(7)), ARG(6));
	goto M4_1;
	RETURN4:;
	F1plus(ARG(4));
	goto M3_1;
	RETURN3:;
}
