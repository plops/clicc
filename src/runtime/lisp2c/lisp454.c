/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void rt_ensure_package(CL_FORM *base)
{
	COPY(ARG(0), ARG(1));
	Ffind_package(ARG(1));
	if(CL_TRUEP(ARG(1)))
	{
		COPY(ARG(1), ARG(0));
	}
	else
	{
		LOAD_SYMBOL(SYMBOL(Slisp, 381), ARG(1));	/* USE */
		LOAD_NIL(ARG(2));
		Fmake_package(ARG(0), 3);
	}
}