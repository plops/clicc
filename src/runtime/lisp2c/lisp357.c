/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void Fmerge_pathnames(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 231), ARG(1));	/* *DEFAULT-PATHNAME-DEFAULTS* */
		case 2:
		LOAD_SYMBOL(SYMBOL(Slisp, 259), ARG(2));	/* NEWEST */
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	merge_pathnames1(ARG(0));
}
