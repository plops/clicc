/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void c_getenv(CL_FORM *base)
{
	string_to_simple_string(ARG(0));
	c_getenv_internal(ARG(0));
}
