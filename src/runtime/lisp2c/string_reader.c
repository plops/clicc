#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cstring_reader = {string_reader, 2};

void string_reader(CL_FORM *base)
{
	LOAD_NIL(STACK(base, 2));
	LOAD_FIXNUM(0, STACK(base, 3));
	COPY(SYMVAL(Slisp, 376), STACK(base, 4));	/* *TOKEN* */
	Fset_fill_pointer(STACK(base, 3));
	M94_1:;
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
	LOAD_NIL(STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
	Fread_char(STACK(base, 2), 4);
	COPY(SYMVAL(Slisp, 382), STACK(base, 3));	/* *READTABLE* */
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 5));	/* READTABLE */
	struct_ref(STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	Fchar_code(STACK(base, 4));
	vref(STACK(base, 3));
	if(CL_SYMBOLP(STACK(base, 3)) && GET_SYMBOL(STACK(base, 3)) == SYMBOL(Slisp, 391))	/* SINGLE-ESCAPE */
	{
		COPY(STACK(base, 0), STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
		LOAD_NIL(STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 5));	/* T */
		Fread_char(STACK(base, 2), 4);
		COPY(STACK(base, 2), STACK(base, 3));
		COPY(SYMVAL(Slisp, 376), STACK(base, 4));	/* *TOKEN* */
		Fvector_push_extend(STACK(base, 3), 2);
	}
	else
	{
		if(EQ(STACK(base, 1), STACK(base, 2)) || CL_FLOATP(STACK(base, 1)) && GET_FLOAT(STACK(base, 1)) == GET_FLOAT(STACK(base, 2)))
		{
			COPY(SYMVAL(Slisp, 376), STACK(base, 0));	/* *TOKEN* */
			LOAD_FIXNUM(0, STACK(base, 1));
			Fsubseq(STACK(base, 0), 2);
			goto RETURN104;
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 3));
			COPY(SYMVAL(Slisp, 376), STACK(base, 4));	/* *TOKEN* */
			Fvector_push_extend(STACK(base, 3), 2);
		}
	}
	goto M94_1;
	RETURN104:;
}
