/* This file was generated by CLiCC 0.6.4 [obrep 1] */
#define __OBREP 1
#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

void Fbit_vector_p(CL_FORM *base)
{
	LOAD_BOOL(CL_SMVEC_BIT_P(ARG(0)), ARG(1));
	if(CL_TRUEP(ARG(1)))
	{
		COPY(ARG(1), ARG(0));
	}
	else
	{
		COPY(ARG(0), ARG(2));
		LOAD_SYMBOL(SYMBOL(Slisp, 150), ARG(3));	/* COMPLEX-VECTOR */
		rt_struct_typep(ARG(2));
		if(CL_TRUEP(ARG(2)))
		{
			COPY(ARG(0), ARG(2));
			M1_1:;
			complex_base_array_data(ARG(2));
			if(CL_SMVECP(ARG(2)))
			{
			}
			else
			{
				goto M1_1;
			}
			goto RETURN1;
			RETURN1:;
			LOAD_BOOL(CL_SMVEC_BIT_P(ARG(2)), ARG(0));
		}
		else
		{
			LOAD_NIL(ARG(0));
		}
	}
}