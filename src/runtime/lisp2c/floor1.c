/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void floor1(CL_FORM *base)
{
	LOAD_SMALLFIXNUM(0, ARG(2));
	rt_convert_to_int(ARG(0));
}
