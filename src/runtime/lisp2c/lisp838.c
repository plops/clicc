/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void FFI_make_c_string(CL_FORM *base)
{
	COPY(ARG(0), ARG(1));
	Fstringp(ARG(1));
	if(CL_TRUEP(ARG(1)))
	{
		rt_make_c_string(ARG(0));
	}
	else
	{
		if(CL_C_STRING_P(ARG(0)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[218], ARG(1));	/* ~&Error in ~A: ~?~% */
			LOAD_SMSTR((CL_FORM *)&KClisp[14], ARG(2));	/* MAKE-C-STRING */
			COPY(SYMVAL(Slisp, 715), ARG(3));	/* NO-STRING */
			COPY(ARG(0), ARG(4));
			Flist(ARG(4), 1);
			Ferror(ARG(1), 4);
		}
	}
}
