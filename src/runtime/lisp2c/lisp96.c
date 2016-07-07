#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kmaybe_diddle_case)
{
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 0 */
	MAKE_STRING(44, "etypecase: the value ~a is not a legal value"),	/* 2 */
};

static void Z59_check_for(CL_FORM *base);
static void Z60_diddle_with(CL_FORM *base);

void maybe_diddle_case(CL_FORM *base)
{
	if(CL_TRUEP(STACK(base, 1)))
	{
		static GLOBAL_FUNARG extern_closure = {Fupper_case_p, 1};
		LOAD_GLOBFUN(&extern_closure, STACK(base, 2));
		COPY(STACK(base, 0), STACK(base, 3));
		Z59_check_for(STACK(base, 2));
		{
			static GLOBAL_FUNARG extern_closure = {Flower_case_p, 1};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 3));
		}
		COPY(STACK(base, 0), STACK(base, 4));
		Z59_check_for(STACK(base, 3));
		if(CL_TRUEP(STACK(base, 2)))
		{
			bool_result = CL_TRUEP(STACK(base, 3));
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
			if(CL_TRUEP(STACK(base, 2)))
			{
				LOAD_GLOBFUN(&CFstring_downcase, STACK(base, 4));
				COPY(STACK(base, 0), STACK(base, 5));
				Z60_diddle_with(STACK(base, 4));
				COPY(STACK(base, 4), STACK(base, 0));
			}
			else
			{
				if(CL_TRUEP(STACK(base, 3)))
				{
					LOAD_GLOBFUN(&CFstring_upcase, STACK(base, 4));
					COPY(STACK(base, 0), STACK(base, 5));
					Z60_diddle_with(STACK(base, 4));
					COPY(STACK(base, 4), STACK(base, 0));
				}
				else
				{
				}
			}
		}
	}
	else
	{
	}
}

static void Z59_check_for(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 3));	/* PATTERN */
	struct_typep(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_NIL(STACK(base, 2));
		COPY(STACK(base, 1), STACK(base, 3));
		LOAD_FIXNUM(0, STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 5));	/* PATTERN */
		struct_ref(STACK(base, 3));
		M16_1:;
		if(CL_ATOMP(STACK(base, 3)))
		{
			LOAD_NIL(STACK(base, 2));
			COPY(STACK(base, 2), STACK(base, 0));
			goto RETURN17;
		}
		COPY(STACK(base, 3), STACK(base, 4));
		Fcar(STACK(base, 4));
		COPY(STACK(base, 4), STACK(base, 2));
		LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 5));	/* STANDARD-CHAR */
		LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 6));	/* * */
		check_simple_array(STACK(base, 4));
		if(CL_TRUEP(STACK(base, 4)))
		{
			COPY(STACK(base, 0), STACK(base, 4));
			COPY(STACK(base, 2), STACK(base, 5));
			Z59_check_for(STACK(base, 4));
			bool_result = CL_TRUEP(STACK(base, 4));
		}
		else
		{
			if(CL_CONSP(STACK(base, 2)))
			{
				COPY(STACK(base, 1), STACK(base, 4));
				Fcar(STACK(base, 4));
				if(CL_SYMBOLP(STACK(base, 4)) && GET_SYMBOL(STACK(base, 4)) == SYMBOL(Slisp, 196))	/* CHARACTER-SET */
				{
					COPY(STACK(base, 0), STACK(base, 5));
					COPY(STACK(base, 1), STACK(base, 6));
					Fcdr(STACK(base, 6));
					Z59_check_for(STACK(base, 5));
					bool_result = CL_TRUEP(STACK(base, 5));
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
		}
		if(bool_result)
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
			goto RETURN17;
		}
		Fcdr(STACK(base, 3));
		goto M16_1;
		RETURN17:;
	}
	else
	{
		if(CL_LISTP(STACK(base, 1)))
		{
			LOAD_NIL(STACK(base, 2));
			COPY(STACK(base, 1), STACK(base, 3));
			M17_1:;
			if(CL_ATOMP(STACK(base, 3)))
			{
				LOAD_NIL(STACK(base, 2));
				COPY(STACK(base, 2), STACK(base, 0));
				goto RETURN18;
			}
			COPY(STACK(base, 3), STACK(base, 4));
			Fcar(STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 2));
			COPY(STACK(base, 0), STACK(base, 4));
			COPY(STACK(base, 2), STACK(base, 5));
			Z59_check_for(STACK(base, 4));
			if(CL_TRUEP(STACK(base, 4)))
			{
				LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
				goto RETURN18;
			}
			Fcdr(STACK(base, 3));
			goto M17_1;
			RETURN18:;
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 2));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
			check_simple_array(STACK(base, 2));
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(STACK(base, 1), STACK(base, 2));
				Flength(STACK(base, 2));
				LOAD_FIXNUM(0, STACK(base, 3));
				M18_1:;
				COPY(STACK(base, 3), STACK(base, 4));
				COPY(STACK(base, 2), STACK(base, 5));
				Fge(STACK(base, 4), 2);
				if(CL_TRUEP(STACK(base, 4)))
				{
					LOAD_NIL(STACK(base, 0));
					goto RETURN19;
				}
				COPY(STACK(base, 0), STACK(base, 4));
				COPY(STACK(base, 1), STACK(base, 5));
				COPY(STACK(base, 3), STACK(base, 6));
				Fschar(STACK(base, 5));
				Ffuncall(STACK(base, 4), 2);
				mv_count = 1;
				if(CL_TRUEP(STACK(base, 4)))
				{
					LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
					goto RETURN19;
				}
				F1plus(STACK(base, 3));
				goto M18_1;
				RETURN19:;
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 197))	/* UNSPECIFIC */
				{
					LOAD_NIL(STACK(base, 0));
				}
				else
				{
					LOAD_SMSTR((CL_FORM *)&Kmaybe_diddle_case[0], STACK(base, 0));	/* etypecase: the value ~a is not a legal value */
					Ferror(STACK(base, 0), 2);
				}
			}
		}
	}
}

static void Z61_g331(CL_FORM *base);

static void Z60_diddle_with(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 2));
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(STACK(base, 1)), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 3));	/* PATTERN */
	struct_typep(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		GEN_CLOSURE(array, STACK(base, 2), 5, Z61_g331, 1);
		COPY(STACK(base, 1), &array[3]);
		COPY(STACK(base, 0), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 2));
		COPY(STACK(base, 2), STACK(base, 2));
		COPY(INDIRECT(STACK(base, 1)), STACK(base, 3));
		LOAD_FIXNUM(0, STACK(base, 4));
		LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 5));	/* PATTERN */
		struct_ref(STACK(base, 3));
		Fmapcar(STACK(base, 2), 2);
		make_pattern(STACK(base, 2), 1);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	else
	{
		if(CL_LISTP(INDIRECT(STACK(base, 1))))
		{
			COPY(INDIRECT(STACK(base, 0)), STACK(base, 2));
			COPY(INDIRECT(STACK(base, 1)), STACK(base, 3));
			Fmapcar(STACK(base, 2), 2);
			COPY(STACK(base, 2), STACK(base, 0));
		}
		else
		{
			COPY(INDIRECT(STACK(base, 1)), STACK(base, 2));
			LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
			LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
			check_simple_array(STACK(base, 2));
			if(CL_TRUEP(STACK(base, 2)))
			{
				COPY(INDIRECT(STACK(base, 0)), STACK(base, 2));
				COPY(INDIRECT(STACK(base, 1)), STACK(base, 3));
				Ffuncall(STACK(base, 2), 2);
				COPY(STACK(base, 2), STACK(base, 0));
			}
			else
			{
				LOAD_SMSTR((CL_FORM *)&Kmaybe_diddle_case[2], STACK(base, 2));	/* etypecase: the value ~a is not a legal value */
				COPY(INDIRECT(STACK(base, 1)), STACK(base, 3));
				Ferror(STACK(base, 2), 2);
			}
		}
	}
}

static void Z61_g331(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 43), STACK(base, 3));	/* STANDARD-CHAR */
	LOAD_SYMBOL(SYMBOL(Slisp, 48), STACK(base, 4));	/* * */
	check_simple_array(STACK(base, 2));
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
		Ffuncall(STACK(base, 2), 2);
		COPY(STACK(base, 2), STACK(base, 0));
	}
	else
	{
		if(CL_CONSP(STACK(base, 1)))
		{
			COPY(STACK(base, 1), STACK(base, 2));
			Fcar(STACK(base, 2));
			if(CL_SYMBOLP(STACK(base, 2)) && GET_SYMBOL(STACK(base, 2)) == SYMBOL(Slisp, 196))	/* CHARACTER-SET */
			{
				LOAD_SYMBOL(SYMBOL(Slisp, 196), STACK(base, 3));	/* CHARACTER-SET */
				COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 4));
				COPY(STACK(base, 1), STACK(base, 5));
				Fcdr(STACK(base, 5));
				Ffuncall(STACK(base, 4), 2);
				mv_count = 1;
				{
					CL_FORM *lptr;
					lptr = form_alloc(STACK(base, 5), 2);
					COPY(STACK(base, 3), CAR(lptr));
					COPY(STACK(base, 4), CDR(lptr));
					LOAD_CONS(lptr, STACK(base, 0));
				}
			}
			else
			{
				COPY(STACK(base, 1), STACK(base, 0));
			}
		}
		else
		{
			COPY(STACK(base, 1), STACK(base, 0));
		}
	}
}
