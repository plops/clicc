/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void Fabs(CL_FORM *base)
{
	COPY(ARG(0), ARG(1));
	Fminusp(ARG(1));
	if(CL_TRUEP(ARG(1)))
	{
		Fminus(ARG(0), 1);
	}
	else
	{
	}
}
