#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z79_g770(CL_FORM *base);

void Ffind_package(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 1));
	{
		CONTENV new_cont;
		CL_FORM *caller_base;
		new_cont.bind_top = bind_top;
		new_cont.last = last_cont;
		LOAD_UNIQUE_TAG(STACK(base, 1));
		GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
		caller_base = (CL_FORM *)SETJMP(new_cont.jmp_buf);
		if(caller_base == NULL)
		{
			last_cont = &new_cont;
			if(CL_SYMBOLP(INDIRECT(STACK(base, 0))) || CL_NILP(INDIRECT(STACK(base, 0))))
			{
				COPY(INDIRECT(STACK(base, 0)), STACK(base, 2));
				Fsymbol_name(STACK(base, 2));
				COPY(STACK(base, 2), INDIRECT(STACK(base, 0)));
			}
			LOAD_NIL(STACK(base, 2));
			{
				GEN_CLOSURE(array, STACK(base, 3), 5, Z79_g770, 1);
				COPY(STACK(base, 1), &array[3]);
				COPY(STACK(base, 0), &array[4]);
				LOAD_CLOSURE(array, STACK(base, 3));
			}
			COPY(STACK(base, 3), STACK(base, 3));
			COPY(SYMVAL(Slisp, 302), STACK(base, 4));	/* *PACKAGE-ARRAY* */
			Fmap(STACK(base, 2), 3);
			COPY(STACK(base, 2), STACK(base, 0));
			RETURN54:;
			last_cont = new_cont.last;
		}
		else
		{
			last_cont = new_cont.last;
			if(EQ(STACK(caller_base, 0), INDIRECT(STACK(base, 1))))
			{
				COPY(STACK(caller_base, 1), STACK(base, 0));
			}
			else
			{
				call_cont(caller_base);
			}
		}
	}
}

static void Z79_g770(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 5));	/* PACKAGE */
	struct_ref(STACK(base, 3));
	FstringE(STACK(base, 2), 2);
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		call_cont(STACK(base, 2));
	}
	LOAD_NIL(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	LOAD_FIXNUM(1, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 5));	/* PACKAGE */
	struct_ref(STACK(base, 3));
	M48_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 0));
		goto RETURN55;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 5));
	FstringE(STACK(base, 4), 2);
	if(CL_TRUEP(STACK(base, 4)))
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 4));
		COPY(STACK(base, 1), STACK(base, 5));
		call_cont(STACK(base, 4));
	}
	Fcdr(STACK(base, 3));
	goto M48_1;
	RETURN55:;
}
