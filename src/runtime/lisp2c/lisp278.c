/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void string_to_simple_string(CL_FORM *base)
{
	if(CL_SMSTRP(ARG(0)))
	{
	}
	else
	{
		COPY(ARG(0), ARG(1));
		complex_vector_displaced(ARG(1));
		Fminusp(ARG(1));
		if(CL_TRUEP(ARG(1)))
		{
			COPY(ARG(0), ARG(1));
			complex_vector_fillptr(ARG(1));
			Fminusp(ARG(1));
		}
		else
		{
			goto ELSE1;
		}
		if(CL_TRUEP(ARG(1)))
		{
			complex_vector_data(ARG(0));
		}
		else
		{
			ELSE1:;
			LOAD_SMALLFIXNUM(0, ARG(1));
			if(CL_LISTP(ARG(0)))
			{
				COPY(SYMVAL(Slisp, 0), ARG(2));	/* MOST-POSITIVE-FIXNUM */
				list_subseq(ARG(0));
			}
			else
			{
				COPY(SYMVAL(Slisp, 0), ARG(2));	/* MOST-POSITIVE-FIXNUM */
				vector_subseq(ARG(0));
			}
		}
	}
}
