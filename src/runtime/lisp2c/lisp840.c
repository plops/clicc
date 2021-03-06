/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void FFI_lisp_character(CL_FORM *base)
{
	COPY(ARG(0), ARG(1));
	c_primitive_p(ARG(1));
	if(CL_TRUEP(ARG(1)))
	{
		rt_make_lisp_character(ARG(0));
	}
	else
	{
		if(CL_CHARP(ARG(0)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[10], ARG(1));	/* LISP-CHARACTER */
			COPY(SYMVAL(Slisp, 712), ARG(2));	/* NO-CHARACTER */
			COPY(ARG(0), ARG(3));
			error_in(ARG(1), 3);
		}
	}
}
