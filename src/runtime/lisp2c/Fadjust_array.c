#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFadjust_array)
{
	MAKE_STRING(95, "The \'array\' argument given to ADJUST-ARRAY, ~A,\n                     isn\'t an adjustable array."),	/* 0 */
	MAKE_STRING(116, "The \'new-dimensions\' argument given to ADJUST-ARRAY, ~A, ~\n              is not the same rank as the argument array."),	/* 2 */
	MAKE_STRING(127, "The \'element-type\' argument to ADJUST-ARRAY, ~A ~\n                  is inconsistent with that of the array to be adjusted [~A]."),	/* 4 */
	MAKE_STRING(107, "A :fill-pointer argument was given to ADJUST-ARRAY, ~\n   \t\t but the array being adjusted, doesn\'t have one."),	/* 6 */
};

void Fadjust_array(CL_FORM *base, int nargs)
{
	BOOL supl_flags[6];
	static CL_FORM * keylist[] =
	{
		SYMBOL(Slisp, 95),	/* ELEMENT-TYPE */
		SYMBOL(Slisp, 123),	/* INITIAL-ELEMENT */
		SYMBOL(Slisp, 99),	/* INITIAL-CONTENTS */
		SYMBOL(Slisp, 97),	/* FILL-POINTER */
		SYMBOL(Slisp, 124),	/* DISPLACED-TO */
		SYMBOL(Slisp, 125),	/* DISPLACED-INDEX-OFFSET */
	};
	keysort(STACK(base, 2), nargs - 2, 6, keylist, supl_flags, FALSE);
	Flist(STACK(base, 8), nargs - 2);
	if(NOT(supl_flags[0]))
	{
		LOAD_NIL(STACK(base, 2));
		LOAD_NIL(STACK(base, 9));
	}
	else
	{
		LOAD_T(STACK(base, 9));
	}
	if(NOT(supl_flags[1]))
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(NOT(supl_flags[2]))
	{
		LOAD_NIL(STACK(base, 4));
	}
	if(NOT(supl_flags[3]))
	{
		LOAD_NIL(STACK(base, 5));
		LOAD_NIL(STACK(base, 10));
	}
	else
	{
		LOAD_T(STACK(base, 10));
	}
	if(NOT(supl_flags[4]))
	{
		LOAD_NIL(STACK(base, 6));
	}
	if(NOT(supl_flags[5]))
	{
		LOAD_NIL(STACK(base, 7));
	}
	COPY(STACK(base, 0), STACK(base, 11));
	Farray_element_type(STACK(base, 11));
	COPY(STACK(base, 0), STACK(base, 12));
	Farray_dimensions(STACK(base, 12));
	LOAD_NIL(STACK(base, 13));
	LOAD_NIL(STACK(base, 14));
	COPY(STACK(base, 0), STACK(base, 15));
	Fadjustable_array_p(STACK(base, 15));
	if(CL_TRUEP(STACK(base, 15)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFadjust_array[0], STACK(base, 15));	/* The 'array' argument given to ADJUST-ARRAY, ~A,
                     isn't an adjustable array. */
		COPY(STACK(base, 0), STACK(base, 16));
		Ferror(STACK(base, 15), 2);
	}
	if(CL_ATOMP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 15));
		Flist(STACK(base, 15), 1);
		COPY(STACK(base, 15), STACK(base, 1));
	}
	COPY(STACK(base, 1), STACK(base, 15));
	Flength(STACK(base, 15));
	COPY(STACK(base, 0), STACK(base, 16));
	Farray_rank(STACK(base, 16));
	Fnumneql(STACK(base, 15), 2);
	if(CL_TRUEP(STACK(base, 15)))
	{
		LOAD_SMSTR((CL_FORM *)&KFadjust_array[2], STACK(base, 15));	/* The 'new-dimensions' argument given to ADJUST-ARRAY, ~A, ~
              is not the same rank as the argument array. */
		COPY(STACK(base, 1), STACK(base, 16));
		Ferror(STACK(base, 15), 2);
	}
	if(CL_TRUEP(STACK(base, 9)))
	{
		COPY(STACK(base, 2), STACK(base, 15));
		Fupgraded_array_element_type(STACK(base, 15));
		COPY(STACK(base, 15), STACK(base, 2));
		if(EQ(STACK(base, 2), STACK(base, 11)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFadjust_array[4], STACK(base, 15));	/* The 'element-type' argument to ADJUST-ARRAY, ~A ~
                  is inconsistent with that of the array to be adjusted [~A]. */
			COPY(STACK(base, 2), STACK(base, 16));
			COPY(STACK(base, 11), STACK(base, 17));
			Ferror(STACK(base, 15), 3);
		}
	}
	if(CL_TRUEP(STACK(base, 10)))
	{
		COPY(STACK(base, 0), STACK(base, 15));
		Farray_has_fill_pointer_p(STACK(base, 15));
		if(CL_TRUEP(STACK(base, 15)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFadjust_array[6], STACK(base, 15));	/* A :fill-pointer argument was given to ADJUST-ARRAY, ~
   		 but the array being adjusted, doesn't have one. */
			Ferror(STACK(base, 15), 1);
		}
		if(CL_TRUEP(STACK(base, 5)))
		{
		}
		else
		{
			COPY(STACK(base, 0), STACK(base, 15));
			Ffill_pointer(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 5));
		}
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 15));
		Farray_has_fill_pointer_p(STACK(base, 15));
		if(CL_TRUEP(STACK(base, 15)))
		{
			COPY(STACK(base, 0), STACK(base, 15));
			Ffill_pointer(STACK(base, 15));
			COPY(STACK(base, 15), STACK(base, 5));
		}
	}
	LOAD_GLOBFUN(&CFmake_array, STACK(base, 15));
	COPY(STACK(base, 1), STACK(base, 16));
	LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 17));	/* ELEMENT-TYPE */
	COPY(STACK(base, 11), STACK(base, 18));
	LOAD_SYMBOL(SYMBOL(Slisp, 96), STACK(base, 19));	/* ADJUSTABLE */
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 20));	/* T */
	LOAD_SYMBOL(SYMBOL(Slisp, 97), STACK(base, 21));	/* FILL-POINTER */
	COPY(STACK(base, 5), STACK(base, 22));
	COPY(STACK(base, 8), STACK(base, 23));
	Fapply(STACK(base, 15), 9);
	mv_count = 1;
	COPY(STACK(base, 15), STACK(base, 13));
	if(CL_TRUEP(STACK(base, 4)))
	{
	}
	else
	{
		COPY(STACK(base, 0), STACK(base, 15));
		Farray_total_size(STACK(base, 15));
		LOAD_FIXNUM(0, STACK(base, 16));
		M10_1:;
		COPY(STACK(base, 16), STACK(base, 17));
		COPY(STACK(base, 15), STACK(base, 18));
		Fge(STACK(base, 17), 2);
		if(CL_TRUEP(STACK(base, 17)))
		{
			goto RETURN10;
		}
		COPY(STACK(base, 12), STACK(base, 17));
		COPY(STACK(base, 16), STACK(base, 18));
		rmi_to_subscripts(STACK(base, 17));
		COPY(STACK(base, 17), STACK(base, 14));
		LOAD_GLOBFUN(&CFarray_in_bounds_p, STACK(base, 17));
		COPY(STACK(base, 13), STACK(base, 18));
		COPY(STACK(base, 14), STACK(base, 19));
		Fapply(STACK(base, 17), 3);
		mv_count = 1;
		if(CL_TRUEP(STACK(base, 17)))
		{
			static GLOBAL_FUNARG extern_closure = {Ffuncall, -2};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 17));
			LOAD_GLOBFUN(&CFset_aref, STACK(base, 18));
			COPY(STACK(base, 0), STACK(base, 19));
			COPY(STACK(base, 16), STACK(base, 20));
			Frow_major_aref(STACK(base, 19));
			COPY(STACK(base, 13), STACK(base, 20));
			COPY(STACK(base, 14), STACK(base, 21));
			Fapply(STACK(base, 17), 5);
			mv_count = 1;
		}
		F1plus(STACK(base, 16));
		goto M10_1;
		RETURN10:;
	}
	COPY(STACK(base, 13), STACK(base, 1));
	displace_array(STACK(base, 0));
}
