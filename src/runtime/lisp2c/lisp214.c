#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG Cnew_hash_table = {new_hash_table, -1};

void new_hash_table(CL_FORM *base, int nargs)
{
	BOOL supl_flags[6];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 335),	/* SIZE */
		SYMBOL(Slisp, 310),	/* COUNT */
		SYMBOL(Slisp, 336),	/* REHASH-SIZE */
		SYMBOL(Slisp, 337),	/* REHASH-THRESHOLD */
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 338),	/* ARRAY */
	};
	keysort(STACK(base, 0), nargs - 0, 6, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		LOAD_FIXNUM(0, STACK(base, 0));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_FIXNUM(0, STACK(base, 1));
	}
	if(NOT(supl_flags[2]))
	{
		static double local_float = 1.5;
		LOAD_FLOAT(&local_float, STACK(base, 2));
	}
	if(NOT(supl_flags[3]))
	{
		static double local_float = 0.69999999999999996;
		LOAD_FLOAT(&local_float, STACK(base, 3));
	}
	if(NOT(supl_flags[4]))
	{
		static GLOBAL_FUNARG extern_closure = {Feql, 2};
		LOAD_GLOBFUN(&extern_closure, STACK(base, 4));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 5));
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 6));	/* HASH-TABLE */
	COPY(STACK(base, 0), STACK(base, 7));
	COPY(STACK(base, 1), STACK(base, 8));
	COPY(STACK(base, 2), STACK(base, 9));
	COPY(STACK(base, 3), STACK(base, 10));
	COPY(STACK(base, 4), STACK(base, 11));
	COPY(STACK(base, 5), STACK(base, 12));
	make_struct(STACK(base, 6), 7);
	COPY(STACK(base, 6), STACK(base, 0));
}
