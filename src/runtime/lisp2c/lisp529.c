/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void Fmake_dispatch_macro_character(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		LOAD_NIL(ARG(1));
		case 2:
		COPY(SYMVAL(Slisp, 449), ARG(2));	/* *READTABLE* */
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	make_dispatch_macro_character1(ARG(0));
}
