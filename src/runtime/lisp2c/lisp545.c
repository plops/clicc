/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

GEN_GLOBAL_FUNARG(Cuninterned_reader, uninterned_reader, 3);

extern CL_INIT XIuninterned_reader[];
extern CL_INIT XFuninterned_reader[];
CL_INIT Kuninterned_reader[] =
{
	MAKE_STRING(30, "illegal value (~S) followed #:"),	/* 0 */
};

void uninterned_reader(CL_FORM *base)
{
	if(CL_TRUEP(ARG(2)))
	{
		LOAD_SMSTR((CL_FORM *)&KClisp[208], ARG(3));	/* extra argument for #~S */
		COPY(ARG(1), ARG(4));
		Ferror(ARG(3), 2);
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(3));	/* T */
	BIND_SPECIAL(SYMBOL(Slisp, 443), ARG(3));	/* *UNINTERNED* */
	COPY(ARG(0), ARG(4));
	LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(5));	/* T */
	LOAD_NIL(ARG(6));
	LOAD_SYMBOL(SYMBOL(Slisp, 48), ARG(7));	/* T */
	read1(ARG(4));
	bool_result = CL_TRUEP(SYMVAL(Slisp, 408));	/* *READ-SUPPRESS* */
	if(bool_result)
	{
		LOAD_NIL(ARG(0));
	}
	else
	{
		if(CL_SYMBOLP(ARG(4)) || CL_NILP(ARG(4)))
		{
			COPY(ARG(4), ARG(0));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&Kuninterned_reader[0], ARG(0));	/* illegal value (~S) followed #: */
			COPY(ARG(4), ARG(1));
			Ferror(ARG(0), 2);
		}
	}
	RESTORE_SPECIAL;
}
