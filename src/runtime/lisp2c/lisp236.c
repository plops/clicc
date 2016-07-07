#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_hash_table)
{
	MAKE_STRING(57, "test ~a in make-hash-table should be a function or symbol"),	/* 0 */
	MAKE_STRING(51, "rehash-size ~A is not a floating-point number > 1.0"),	/* 2 */
	MAKE_STRING(56, "rehash-size ~A is not a floating-point number or integer"),	/* 4 */
	MAKE_STRING(80, "rehash-threshold ~A is not a floating-point ~\n            number between 0 and 1"),	/* 6 */
};

void Fmake_hash_table(CL_FORM *base, int nargs)
{
	BOOL supl_flags[4];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 213),	/* TEST */
		SYMBOL(Slisp, 335),	/* SIZE */
		SYMBOL(Slisp, 336),	/* REHASH-SIZE */
		SYMBOL(Slisp, 337),	/* REHASH-THRESHOLD */
	};
	keysort(STACK(base, 0), nargs - 0, 4, keylist, supl_flags, FALSE);
	if(NOT(supl_flags[0]))
	{
		static GLOBAL_FUNARG extern_closure = {Feql, 2};
		LOAD_GLOBFUN(&extern_closure, STACK(base, 0));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_FIXNUM(50, STACK(base, 1));
	}
	if(NOT(supl_flags[2]))
	{
		static double local_float = 1.5;
		LOAD_FLOAT(&local_float, STACK(base, 2));
	}
	if(NOT(supl_flags[3]))
	{
		static double local_float = 0.7;
		LOAD_FLOAT(&local_float, STACK(base, 3));
	}
	if(CL_SYMBOLP(STACK(base, 0)) || CL_NILP(STACK(base, 0)))
	{
		if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 339))	/* EQ */
		{
			static GLOBAL_FUNARG extern_closure = {Feq, 2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 0));
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 340))	/* EQL */
			{
				static GLOBAL_FUNARG extern_closure = {Feql, 2};
				LOAD_GLOBFUN(&extern_closure, STACK(base, 0));
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 341))	/* EQUAL */
				{
					LOAD_GLOBFUN(&CFequal, STACK(base, 0));
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 0)) && GET_SYMBOL(STACK(base, 0)) == SYMBOL(Slisp, 342))	/* EQUALP */
					{
						LOAD_GLOBFUN(&CFequalp, STACK(base, 0));
					}
				}
			}
		}
	}
	else
	{
		if(CL_FUNCTION_P(STACK(base, 0)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_hash_table[0], STACK(base, 4));	/* test ~a in make-hash-table should be a function or symbol */
			COPY(STACK(base, 0), STACK(base, 5));
			Ferror(STACK(base, 4), 2);
		}
	}
	if(CL_FIXNUMP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Fdiv(STACK(base, 4), 2);
		{
			static double local_float = 1.0;
			LOAD_FLOAT(&local_float, STACK(base, 5));
		}
		Fplus(STACK(base, 4), 2);
		COPY(STACK(base, 4), STACK(base, 2));
	}
	else
	{
		if(CL_FLOATP(STACK(base, 2)))
		{
			static double local_float = 1.0;
			LOAD_FLOAT(&local_float, STACK(base, 4));
			COPY(STACK(base, 2), STACK(base, 5));
			Fge(STACK(base, 4), 2);
			if(CL_TRUEP(STACK(base, 4)))
			{
				LOAD_SMSTR((CL_FORM *)&KFmake_hash_table[2], STACK(base, 4));	/* rehash-size ~A is not a floating-point number > 1.0 */
				COPY(STACK(base, 2), STACK(base, 5));
				Ferror(STACK(base, 4), 2);
			}
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_hash_table[4], STACK(base, 4));	/* rehash-size ~A is not a floating-point number or integer */
			COPY(STACK(base, 2), STACK(base, 5));
			Ferror(STACK(base, 4), 2);
		}
	}
	if(CL_FLOATP(STACK(base, 3)))
	{
		COPY(STACK(base, 3), STACK(base, 4));
		{
			static double local_float = 0.0;
			LOAD_FLOAT(&local_float, STACK(base, 5));
		}
		Fge(STACK(base, 4), 2);
		if(CL_TRUEP(STACK(base, 4)))
		{
			COPY(STACK(base, 3), STACK(base, 4));
			{
				static double local_float = 1.0;
				LOAD_FLOAT(&local_float, STACK(base, 5));
			}
			Fle(STACK(base, 4), 2);
			bool_result = CL_TRUEP(STACK(base, 4));
		}
		else
		{
			bool_result = FALSE;
		}
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_hash_table[6], STACK(base, 4));	/* rehash-threshold ~A is not a floating-point ~
            number between 0 and 1 */
		COPY(STACK(base, 3), STACK(base, 5));
		Ferror(STACK(base, 4), 2);
	}
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 5));
	LOAD_FIXNUM(2, STACK(base, 6));
	Frem(STACK(base, 5));
	Fzerop(STACK(base, 5));
	if(CL_TRUEP(STACK(base, 5)))
	{
		F1plus(STACK(base, 4));
	}
	COPY(STACK(base, 4), STACK(base, 5));
	LOAD_FIXNUM(3, STACK(base, 6));
	Frem(STACK(base, 5));
	Fzerop(STACK(base, 5));
	if(CL_TRUEP(STACK(base, 5)))
	{
		LOAD_FIXNUM(2, STACK(base, 5));
		Fplus(STACK(base, 4), 2);
	}
	COPY(STACK(base, 4), STACK(base, 5));
	LOAD_FIXNUM(5, STACK(base, 6));
	Frem(STACK(base, 5));
	Fzerop(STACK(base, 5));
	if(CL_TRUEP(STACK(base, 5)))
	{
		LOAD_FIXNUM(4, STACK(base, 5));
		Fplus(STACK(base, 4), 2);
	}
	COPY(STACK(base, 4), STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 6));	/* INITIAL-ELEMENT */
	LOAD_NIL(STACK(base, 7));
	Fmake_array(STACK(base, 5), 3);
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 6));	/* HASH-TABLE */
	COPY(STACK(base, 4), STACK(base, 7));
	LOAD_FIXNUM(0, STACK(base, 8));
	COPY(STACK(base, 2), STACK(base, 9));
	COPY(STACK(base, 3), STACK(base, 10));
	COPY(STACK(base, 0), STACK(base, 11));
	COPY(STACK(base, 5), STACK(base, 12));
	make_struct(STACK(base, 6), 7);
	COPY(STACK(base, 6), STACK(base, 0));
}
