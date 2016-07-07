#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFunintern)
{
	MAKE_STRING(36, "~S and ~S will cause a name conflict"),	/* 0 */
};

void Funintern(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 1:
		COPY(SYMVAL(Slisp, 272), STACK(base, 1));	/* *PACKAGE* */
		case 2:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	LOAD_NIL(STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	COPY(STACK(base, 0), STACK(base, 4));
	Fsymbol_name(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	coerce_to_package(STACK(base, 6));
	COPY(STACK(base, 6), STACK(base, 1));
	COPY(STACK(base, 4), STACK(base, 6));
	COPY(STACK(base, 1), STACK(base, 7));
	Ffind_symbol(STACK(base, 6), 2);
	COPY(&mv_buf[0], STACK(base, 7));
	{
		int nargs;
		nargs = 2;
		mv_count = 1;
		switch(nargs)
		{
			case 0:
			LOAD_NIL(STACK(base, 6));
			case 1:
			LOAD_NIL(STACK(base, 7));
			nargs = 2;
		}
		Flist(STACK(base, 8), nargs - 2);
		COPY(STACK(base, 6), STACK(base, 2));
		COPY(STACK(base, 7), STACK(base, 3));
	}
	if(EQ(STACK(base, 2), STACK(base, 0)))
	{
		if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 305))	/* INTERNAL */
		{
			LOAD_T(STACK(base, 6));
		}
		else
		{
			LOAD_NIL(STACK(base, 6));
		}
		if(CL_TRUEP(STACK(base, 6)))
		{
			bool_result = TRUE;
		}
		else
		{
			bool_result = CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 306);	/* EXTERNAL */
		}
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 306))	/* EXTERNAL */
		{
			COPY(STACK(base, 1), STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 5));
			LOAD_FIXNUM(3, STACK(base, 6));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
			struct_ref(STACK(base, 5));
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 6));
			COPY(STACK(base, 6), STACK(base, 5));
			LOAD_FIXNUM(2, STACK(base, 6));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 7));	/* PACKAGE */
			struct_ref(STACK(base, 5));
		}
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 1), STACK(base, 7));
		LOAD_FIXNUM(4, STACK(base, 8));
		LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 9));	/* PACKAGE */
		struct_ref(STACK(base, 7));
		Fmember(STACK(base, 6), 2);
		if(CL_TRUEP(STACK(base, 6)))
		{
			LOAD_FIXNUM(0, STACK(base, 6));
			LOAD_NIL(STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			LOAD_FIXNUM(5, STACK(base, 9));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 10));	/* PACKAGE */
			struct_ref(STACK(base, 8));
			M53_1:;
			if(CL_ATOMP(STACK(base, 8)))
			{
				LOAD_NIL(STACK(base, 7));
				goto RETURN61;
			}
			COPY(STACK(base, 8), STACK(base, 9));
			Fcar(STACK(base, 9));
			COPY(STACK(base, 9), STACK(base, 7));
			COPY(STACK(base, 4), STACK(base, 9));
			COPY(STACK(base, 7), STACK(base, 10));
			Ffind_symbol(STACK(base, 9), 2);
			COPY(&mv_buf[0], STACK(base, 10));
			{
				int nargs;
				nargs = 2;
				mv_count = 1;
				switch(nargs)
				{
					case 0:
					LOAD_NIL(STACK(base, 9));
					case 1:
					LOAD_NIL(STACK(base, 10));
					nargs = 2;
				}
				Flist(STACK(base, 11), nargs - 2);
				COPY(STACK(base, 9), STACK(base, 2));
				COPY(STACK(base, 10), STACK(base, 3));
			}
			if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 306))	/* EXTERNAL */
			{
				if(CL_FIXNUMP(STACK(base, 6)) && GET_FIXNUM(STACK(base, 6)) == 0)
				{
					COPY(STACK(base, 2), STACK(base, 6));
				}
				else
				{
					if(EQ(STACK(base, 2), STACK(base, 6)))
					{
					}
					else
					{
						LOAD_SMSTR((CL_FORM *)&KFunintern[0], STACK(base, 9));	/* ~S and ~S will cause a name conflict */
						COPY(STACK(base, 6), STACK(base, 10));
						COPY(STACK(base, 2), STACK(base, 11));
						Ferror(STACK(base, 9), 3);
					}
				}
			}
			Fcdr(STACK(base, 8));
			goto M53_1;
			RETURN61:;
			COPY(STACK(base, 1), STACK(base, 6));
			COPY(STACK(base, 0), STACK(base, 7));
			COPY(STACK(base, 1), STACK(base, 8));
			LOAD_FIXNUM(4, STACK(base, 9));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 10));	/* PACKAGE */
			struct_ref(STACK(base, 8));
			LOAD_SYMBOL(SYMBOL(Slisp, 310), STACK(base, 9));	/* COUNT */
			LOAD_FIXNUM(1, STACK(base, 10));
			Fdelete(STACK(base, 7), 4);
			COPY(STACK(base, 1), STACK(base, 8));
			LOAD_FIXNUM(4, STACK(base, 9));
			LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 10));	/* PACKAGE */
			COPY(STACK(base, 7), STACK(base, 11));
			set_struct_ref(STACK(base, 8));
		}
		COPY(STACK(base, 0), STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 8));
		LOAD_FIXNUM(101, STACK(base, 9));
		string_hash(STACK(base, 8));
		del_pack_sym(STACK(base, 6));
		COPY(STACK(base, 0), STACK(base, 6));
		Fsymbol_package(STACK(base, 6));
		if(EQ(STACK(base, 6), STACK(base, 1)))
		{
			LOAD_NIL(STACK(base, 6));
			COPY(STACK(base, 0), STACK(base, 7));
			set_symbol_package(STACK(base, 6));
		}
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
