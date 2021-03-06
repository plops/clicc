/* This file was generated by CLiCC 0.6.5 [OBREP 2] */
#include <c_decl.h>
#if OBREP != 2
	#error "Wrong OBREP!"
#endif
#include "sys.h"
#include "lisp.h"

extern CL_INIT XIdigit_char1[];
extern CL_INIT XFdigit_char1[];
CL_INIT Kdigit_char1[] =
{
	MAKE_STRING(36, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"),	/* 0 */
};

void digit_char1(CL_FORM *base)
{
	if(CL_FIXNUMP(ARG(1)))
	{
		LOAD_SMALLFIXNUM(2, ARG(2));
		COPY(ARG(1), ARG(3));
		LOAD_SMALLFIXNUM(36, ARG(4));
		Fle(ARG(2), 3);
	}
	else
	{
		goto ELSE1;
	}
	if(CL_TRUEP(ARG(2)))
	{
		if(CL_FIXNUMP(ARG(0)))
		{
			LOAD_SMALLFIXNUM(0, ARG(2));
			COPY(ARG(0), ARG(3));
			COPY(ARG(1), ARG(4));
			F1minus(ARG(4));
			Fle(ARG(2), 3);
		}
		else
		{
			goto ELSE2;
		}
	}
	else
	{
		ELSE1:;
		goto ELSE2;
	}
	if(CL_TRUEP(ARG(2)))
	{
		LOAD_SMSTR((CL_FORM *)&Kdigit_char1[0], ARG(2));	/* 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ */
		COPY(ARG(0), ARG(3));
		Frow_major_aref(ARG(2));
		COPY(ARG(2), ARG(0));
	}
	else
	{
		ELSE2:;
		LOAD_NIL(ARG(0));
	}
}
