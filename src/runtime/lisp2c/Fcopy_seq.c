/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

void Fcopy_seq(CL_FORM *base)
{
	LOAD_SMALLFIXNUM(0, ARG(1));
	Fsubseq(ARG(0), 2);
}
