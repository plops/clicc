#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFmake_array = {Fmake_array, -2};

CL_INIT2(KFmake_array)
{
	MAKE_STRING(109, "A dimension argument to MAKE-ARRAY, ~A, is unusable ~\n                     since it is not a positive fixnum."),	/* 0 */
	MAKE_STRING(94, "The :initial-element, ~A, is of the wrong type; ~\n                     it should be of type ~A"),	/* 2 */
	MAKE_STRING(57, ":initial-element and :initial-contents are both supplied."),	/* 4 */
	MAKE_STRING(182, "The :initial-contents argument, ~A,\n                                    isn\'t the correct shape or size\n                                    for an array of rank ~A with dimensions ~A"),	/* 6 */
	MAKE_STRING(43, "The :displaced-to argument is not an array."),	/* 8 */
	MAKE_STRING(59, "Cannot displace the array, because it has a different type."),	/* 10 */
	MAKE_STRING(97, "Cannot displace the array, ~\n                because the total size of the to-array is too small."),	/* 12 */
	MAKE_STRING(120, "A :displaced-index-offset argument is given to make-array, ~\n                        but there is no :displaced-to array"),	/* 14 */
	MAKE_STRING(58, "The displaced-to array is smaller than the displaced array"),	/* 16 */
	MAKE_STRING(147, "The :displaced-index-offset argument, ~A, ~\n                  is not in the linearized range [0, ~A) ~\n                  of the :displaced-to array"),	/* 18 */
	MAKE_STRING(59, "The fill-pointer ~A must be an integer in the range [0, ~A]"),	/* 20 */
	MAKE_STRING(48, "Fill-pointers are only for 1-dimensional arrays."),	/* 22 */
};

static void Z54_set_initial_contents(CL_FORM *base, CL_FORM *display[]);
static void Z55_chk_shape_of_init_cont(CL_FORM *base);

void Fmake_array(CL_FORM *base, int nargs)
{
	CL_FORM *display[1];
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 0 + nargs));
	{
		BOOL supl_flags[7];
		static CL_FORM * keylist[] =
		{
			SYMBOL(Slisp, 95),	/* ELEMENT-TYPE */
			SYMBOL(Slisp, 123),	/* INITIAL-ELEMENT */
			SYMBOL(Slisp, 99),	/* INITIAL-CONTENTS */
			SYMBOL(Slisp, 96),	/* ADJUSTABLE */
			SYMBOL(Slisp, 97),	/* FILL-POINTER */
			SYMBOL(Slisp, 124),	/* DISPLACED-TO */
			SYMBOL(Slisp, 125),	/* DISPLACED-INDEX-OFFSET */
		};
		keysort(STACK(base, 1), nargs - 1, 7, keylist, supl_flags, FALSE);
		if(NOT(supl_flags[0]))
		{
			LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 1));	/* T */
		}
		if(NOT(supl_flags[1]))
		{
			LOAD_NIL(STACK(base, 2));
			LOAD_NIL(STACK(base, 8));
		}
		else
		{
			LOAD_T(STACK(base, 8));
		}
		if(NOT(supl_flags[2]))
		{
			LOAD_NIL(STACK(base, 3));
			LOAD_NIL(STACK(base, 9));
		}
		else
		{
			LOAD_T(STACK(base, 9));
		}
		GEN_HEAPVAR(STACK(base, 3), STACK(base, 10));
		if(NOT(supl_flags[3]))
		{
			LOAD_NIL(STACK(base, 4));
		}
		if(NOT(supl_flags[4]))
		{
			LOAD_NIL(STACK(base, 5));
		}
		if(NOT(supl_flags[5]))
		{
			LOAD_NIL(STACK(base, 6));
		}
		if(NOT(supl_flags[6]))
		{
			LOAD_FIXNUM(0, STACK(base, 7));
			LOAD_NIL(STACK(base, 10));
		}
		else
		{
			LOAD_T(STACK(base, 10));
		}
	}
	if(CL_ATOMP(INDIRECT(STACK(base, 0))))
	{
		COPY(INDIRECT(STACK(base, 0)), STACK(base, 11));
		Flist(STACK(base, 11), 1);
		COPY(STACK(base, 11), INDIRECT(STACK(base, 0)));
	}
	COPY(INDIRECT(STACK(base, 0)), STACK(base, 11));
	Flength(STACK(base, 11));
	GEN_HEAPVAR(STACK(base, 11), STACK(base, 12));
	COPY(INDIRECT(STACK(base, 0)), STACK(base, 12));
	LOAD_NIL(STACK(base, 13));
	LOAD_FIXNUM(1, STACK(base, 14));
	M4_1:;
	if(CL_TRUEP(STACK(base, 12)))
	{
	}
	else
	{
		COPY(STACK(base, 14), STACK(base, 12));
		goto RETURN4;
	}
	COPY(STACK(base, 12), STACK(base, 15));
	COPY(STACK(base, 15), STACK(base, 16));
	Fcar(STACK(base, 16));
	COPY(STACK(base, 16), STACK(base, 15));
	COPY(STACK(base, 12), STACK(base, 16));
	COPY(STACK(base, 16), STACK(base, 17));
	Fcdr(STACK(base, 17));
	COPY(STACK(base, 17), STACK(base, 12));
	COPY(STACK(base, 15), STACK(base, 13));
	if(CL_FIXNUMP(STACK(base, 13)))
	{
		LOAD_FIXNUM(0, STACK(base, 15));
		COPY(STACK(base, 13), STACK(base, 16));
		LOAD_FIXNUM(2147483647, STACK(base, 17));
		Fle(STACK(base, 15), 3);
		bool_result = CL_TRUEP(STACK(base, 15));
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
		LOAD_SMSTR((CL_FORM *)&KFmake_array[0], STACK(base, 15));	/* A dimension argument to MAKE-ARRAY, ~A, is unusable ~
                     since it is not a positive fixnum. */
		COPY(STACK(base, 13), STACK(base, 16));
		Ferror(STACK(base, 15), 2);
	}
	COPY(STACK(base, 14), STACK(base, 15));
	COPY(STACK(base, 13), STACK(base, 16));
	Fmult(STACK(base, 15), 2);
	COPY(STACK(base, 15), STACK(base, 14));
	goto M4_1;
	RETURN4:;
	if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 23))	/* FIXNUM */
	{
		LOAD_FIXNUM(1, STACK(base, 13));
	}
	else
	{
		if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 24))	/* FLOAT */
		{
			LOAD_T(STACK(base, 13));
		}
		else
		{
			LOAD_NIL(STACK(base, 13));
		}
		if(CL_TRUEP(STACK(base, 13)))
		{
			bool_result = TRUE;
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 37))	/* SHORT-FLOAT */
			{
				LOAD_T(STACK(base, 14));
			}
			else
			{
				LOAD_NIL(STACK(base, 14));
			}
			if(CL_TRUEP(STACK(base, 14)))
			{
				bool_result = TRUE;
			}
			else
			{
				if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 42))	/* SINGLE-FLOAT */
				{
					LOAD_T(STACK(base, 15));
				}
				else
				{
					LOAD_NIL(STACK(base, 15));
				}
				if(CL_TRUEP(STACK(base, 15)))
				{
					bool_result = TRUE;
				}
				else
				{
					if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 22))	/* DOUBLE-FLOAT */
					{
						LOAD_T(STACK(base, 16));
					}
					else
					{
						LOAD_NIL(STACK(base, 16));
					}
					if(CL_TRUEP(STACK(base, 16)))
					{
						bool_result = TRUE;
					}
					else
					{
						bool_result = CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 29);	/* LONG-FLOAT */
					}
				}
			}
		}
		if(bool_result)
		{
			LOAD_FIXNUM(2, STACK(base, 13));
		}
		else
		{
			if(CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 43))	/* STANDARD-CHAR */
			{
				LOAD_T(STACK(base, 13));
			}
			else
			{
				LOAD_NIL(STACK(base, 13));
			}
			if(CL_TRUEP(STACK(base, 13)))
			{
				bool_result = TRUE;
			}
			else
			{
				bool_result = CL_SYMBOLP(STACK(base, 1)) && GET_SYMBOL(STACK(base, 1)) == SYMBOL(Slisp, 18);	/* CHARACTER */
			}
			if(bool_result)
			{
				LOAD_FIXNUM(3, STACK(base, 13));
			}
			else
			{
				LOAD_FIXNUM(0, STACK(base, 13));
			}
		}
	}
	if(CL_TRUEP(STACK(base, 8)))
	{
		COPY(STACK(base, 2), STACK(base, 14));
		COPY(STACK(base, 13), STACK(base, 15));
		type_code_p(STACK(base, 14));
		bool_result = NOT(CL_TRUEP(STACK(base, 14)));
	}
	else
	{
		bool_result = FALSE;
	}
	if(bool_result)
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_array[2], STACK(base, 14));	/* The :initial-element, ~A, is of the wrong type; ~
                     it should be of type ~A */
		COPY(STACK(base, 2), STACK(base, 15));
		COPY(STACK(base, 13), STACK(base, 16));
		to_element_type(STACK(base, 16));
		Ferror(STACK(base, 14), 3);
	}
	if(CL_TRUEP(STACK(base, 9)))
	{
		if(CL_TRUEP(STACK(base, 8)))
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[4], STACK(base, 14));	/* :initial-element and :initial-contents are both supplied. */
			Ferror(STACK(base, 14), 1);
		}
		{
			GEN_CLOSURE(array, STACK(base, 14), 6, Z55_chk_shape_of_init_cont, 2);
			COPY(STACK(base, 0), &array[3]);
			COPY(STACK(base, 11), &array[4]);
			COPY(STACK(base, 3), &array[5]);
			LOAD_CLOSURE(array, STACK(base, 14));
		}
		COPY(STACK(base, 14), STACK(base, 15));
		COPY(INDIRECT(STACK(base, 0)), STACK(base, 16));
		COPY(INDIRECT(STACK(base, 3)), STACK(base, 17));
		Z55_chk_shape_of_init_cont(STACK(base, 15));
	}
	if(CL_TRUEP(STACK(base, 6)))
	{
		if(CL_TRUEP(STACK(base, 8)))
		{
			COPY(SYMVAL(Slisp, 120), STACK(base, 14));	/* DISPLACED_NOT_WITH */
			LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 15));	/* INITIAL-ELEMENT */
			Ferror(STACK(base, 14), 2);
		}
		if(CL_TRUEP(STACK(base, 9)))
		{
			COPY(SYMVAL(Slisp, 120), STACK(base, 14));	/* DISPLACED_NOT_WITH */
			LOAD_SYMBOL(SYMBOL(Slisp, 99), STACK(base, 15));	/* INITIAL-CONTENTS */
			Ferror(STACK(base, 14), 2);
		}
		if(CL_ARRAY_P(STACK(base, 6)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[8], STACK(base, 14));	/* The :displaced-to argument is not an array. */
			Ferror(STACK(base, 14), 1);
		}
		COPY(STACK(base, 6), STACK(base, 14));
		array_element_type_internal(STACK(base, 14));
		if(CL_FIXNUMP(STACK(base, 14)) && GET_FIXNUM(STACK(base, 14)) == 0)
		{
			LOAD_T(STACK(base, 15));
		}
		else
		{
			LOAD_NIL(STACK(base, 15));
		}
		if(CL_TRUEP(STACK(base, 15)))
		{
			bool_result = TRUE;
		}
		else
		{
			bool_result = EQ(STACK(base, 13), STACK(base, 14)) || CL_FLOATP(STACK(base, 13)) && GET_FLOAT(STACK(base, 13)) == GET_FLOAT(STACK(base, 14));
		}
		if(bool_result)
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[10], STACK(base, 14));	/* Cannot displace the array, because it has a different type. */
			Ferror(STACK(base, 14), 1);
		}
		COPY(STACK(base, 12), STACK(base, 14));
		COPY(STACK(base, 6), STACK(base, 15));
		Farray_total_size(STACK(base, 15));
		Fgt(STACK(base, 14), 2);
		if(CL_TRUEP(STACK(base, 14)))
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[12], STACK(base, 14));	/* Cannot displace the array, ~
                because the total size of the to-array is too small. */
			Ferror(STACK(base, 14), 1);
		}
	}
	if(CL_TRUEP(STACK(base, 10)))
	{
		if(CL_TRUEP(STACK(base, 6)))
		{
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[14], STACK(base, 14));	/* A :displaced-index-offset argument is given to make-array, ~
                        but there is no :displaced-to array */
			Ferror(STACK(base, 14), 1);
		}
		COPY(STACK(base, 6), STACK(base, 14));
		Farray_total_size(STACK(base, 14));
		COPY(STACK(base, 12), STACK(base, 15));
		COPY(STACK(base, 14), STACK(base, 16));
		Fgt(STACK(base, 15), 2);
		if(CL_TRUEP(STACK(base, 15)))
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[16], STACK(base, 15));	/* The displaced-to array is smaller than the displaced array */
			Ferror(STACK(base, 15), 1);
		}
		if(CL_FIXNUMP(STACK(base, 7)))
		{
			LOAD_FIXNUM(0, STACK(base, 15));
			COPY(STACK(base, 7), STACK(base, 16));
			COPY(STACK(base, 14), STACK(base, 17));
			COPY(STACK(base, 12), STACK(base, 18));
			Fminus(STACK(base, 17), 2);
			Fle(STACK(base, 15), 3);
			bool_result = CL_TRUEP(STACK(base, 15));
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
			LOAD_SMSTR((CL_FORM *)&KFmake_array[18], STACK(base, 15));	/* The :displaced-index-offset argument, ~A, ~
                  is not in the linearized range [0, ~A) ~
                  of the :displaced-to array */
			COPY(STACK(base, 7), STACK(base, 16));
			COPY(STACK(base, 6), STACK(base, 17));
			Farray_total_size(STACK(base, 17));
			COPY(STACK(base, 12), STACK(base, 18));
			Fminus(STACK(base, 17), 2);
			Ferror(STACK(base, 15), 3);
		}
	}
	COPY(INDIRECT(STACK(base, 11)), STACK(base, 14));
	LOAD_FIXNUM(1, STACK(base, 15));
	Fnumeql(STACK(base, 14), 2);
	if(CL_TRUEP(STACK(base, 14)))
	{
		LOAD_NIL(STACK(base, 14));
		if(CL_TRUEP(STACK(base, 5)))
		{
			if(CL_FIXNUMP(STACK(base, 5)))
			{
				LOAD_FIXNUM(0, STACK(base, 15));
				COPY(STACK(base, 5), STACK(base, 16));
				COPY(STACK(base, 12), STACK(base, 17));
				Fle(STACK(base, 15), 3);
				bool_result = CL_TRUEP(STACK(base, 15));
			}
			else
			{
				bool_result = FALSE;
			}
			bool_result = NOT(bool_result);
		}
		else
		{
			bool_result = FALSE;
		}
		if(bool_result)
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[20], STACK(base, 15));	/* The fill-pointer ~A must be an integer in the range [0, ~A] */
			COPY(STACK(base, 5), STACK(base, 16));
			COPY(STACK(base, 12), STACK(base, 17));
			Ferror(STACK(base, 15), 3);
		}
		COPY(STACK(base, 13), STACK(base, 15));
		COPY(STACK(base, 12), STACK(base, 16));
		COPY(STACK(base, 4), STACK(base, 17));
		COPY(STACK(base, 5), STACK(base, 18));
		COPY(STACK(base, 6), STACK(base, 19));
		COPY(STACK(base, 7), STACK(base, 20));
		make_vector_internal(STACK(base, 15));
		COPY(STACK(base, 15), STACK(base, 14));
		if(CL_TRUEP(STACK(base, 8)))
		{
			COPY(STACK(base, 14), STACK(base, 15));
			COPY(STACK(base, 2), STACK(base, 16));
			Ffill(STACK(base, 15), 2);
		}
		if(CL_TRUEP(STACK(base, 9)))
		{
			COPY(STACK(base, 14), STACK(base, 15));
			COPY(INDIRECT(STACK(base, 3)), STACK(base, 16));
			Freplace(STACK(base, 15), 2);
		}
		COPY(STACK(base, 14), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 14));
		if(CL_TRUEP(STACK(base, 5)))
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[22], STACK(base, 15));	/* Fill-pointers are only for 1-dimensional arrays. */
			Ferror(STACK(base, 15), 1);
		}
		{
			static GLOBAL_FUNARG extern_closure = {make_array_internal, -7};
			LOAD_GLOBFUN(&extern_closure, STACK(base, 15));
		}
		COPY(STACK(base, 13), STACK(base, 16));
		COPY(STACK(base, 12), STACK(base, 17));
		COPY(STACK(base, 4), STACK(base, 18));
		COPY(STACK(base, 6), STACK(base, 19));
		COPY(STACK(base, 7), STACK(base, 20));
		COPY(INDIRECT(STACK(base, 11)), STACK(base, 21));
		COPY(INDIRECT(STACK(base, 0)), STACK(base, 22));
		Fapply(STACK(base, 15), 8);
		mv_count = 1;
		COPY(STACK(base, 15), STACK(base, 14));
		if(CL_TRUEP(STACK(base, 8)))
		{
			COPY(STACK(base, 14), STACK(base, 15));
			Farray_total_size(STACK(base, 15));
			LOAD_FIXNUM(0, STACK(base, 16));
			M5_1:;
			COPY(STACK(base, 16), STACK(base, 17));
			COPY(STACK(base, 15), STACK(base, 18));
			Fge(STACK(base, 17), 2);
			if(CL_TRUEP(STACK(base, 17)))
			{
				goto RETURN5;
			}
			COPY(STACK(base, 2), STACK(base, 17));
			COPY(STACK(base, 14), STACK(base, 18));
			COPY(STACK(base, 16), STACK(base, 19));
			Fset_row_major_aref(STACK(base, 17));
			COPY(STACK(base, 16), STACK(base, 17));
			F1plus(STACK(base, 17));
			COPY(STACK(base, 17), STACK(base, 16));
			goto M5_1;
			RETURN5:;
		}
		if(CL_TRUEP(STACK(base, 9)))
		{
			LOAD_FIXNUM(0, STACK(base, 15));
			COPY(INDIRECT(STACK(base, 0)), STACK(base, 16));
			COPY(INDIRECT(STACK(base, 3)), STACK(base, 17));
			display[0] = STACK(base, 0);
			Z54_set_initial_contents(STACK(base, 15), display);
		}
		COPY(STACK(base, 14), STACK(base, 0));
	}
}

static void Z54_set_initial_contents(CL_FORM *base, CL_FORM *display[])
{
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(STACK(base, 1), STACK(base, 3));
		Fcar(STACK(base, 3));
		COPY(STACK(base, 1), STACK(base, 4));
		Fcdr(STACK(base, 4));
		LOAD_FIXNUM(0, STACK(base, 5));
		LOAD_FIXNUM(0, STACK(base, 6));
		M6_1:;
		COPY(STACK(base, 5), STACK(base, 7));
		COPY(STACK(base, 3), STACK(base, 8));
		Fge(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			COPY(STACK(base, 6), STACK(base, 0));
			goto RETURN6;
		}
		COPY(STACK(base, 6), STACK(base, 7));
		COPY(STACK(base, 0), STACK(base, 8));
		COPY(STACK(base, 6), STACK(base, 9));
		Fplus(STACK(base, 8), 2);
		COPY(STACK(base, 4), STACK(base, 9));
		COPY(STACK(base, 2), STACK(base, 10));
		COPY(STACK(base, 5), STACK(base, 11));
		Felt(STACK(base, 10));
		Z54_set_initial_contents(STACK(base, 8), display);
		Fplus(STACK(base, 7), 2);
		COPY(STACK(base, 7), STACK(base, 6));
		COPY(STACK(base, 5), STACK(base, 7));
		F1plus(STACK(base, 7));
		COPY(STACK(base, 7), STACK(base, 5));
		goto M6_1;
		RETURN6:;
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 3));
		COPY(&display[0][14], STACK(base, 4));
		COPY(STACK(base, 0), STACK(base, 5));
		Fset_row_major_aref(STACK(base, 3));
		LOAD_FIXNUM(1, STACK(base, 0));
	}
}

static void Z56_g158(CL_FORM *base);

static void Z55_chk_shape_of_init_cont(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 1), STACK(base, 3));
	if(NOT(CL_TRUEP(INDIRECT(STACK(base, 1)))))
	{
		LOAD_T(STACK(base, 3));
	}
	else
	{
		LOAD_NIL(STACK(base, 3));
	}
	if(CL_TRUEP(STACK(base, 3)))
	{
		LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 0));	/* T */
	}
	else
	{
		COPY(INDIRECT(STACK(base, 1)), STACK(base, 4));
		Fcar(STACK(base, 4));
		COPY(STACK(base, 2), STACK(base, 5));
		Flength(STACK(base, 5));
		Fnumneql(STACK(base, 4), 2);
		if(CL_TRUEP(STACK(base, 4)))
		{
			LOAD_SMSTR((CL_FORM *)&KFmake_array[6], STACK(base, 4));	/* The :initial-contents argument, ~A,
                                    isn't the correct shape or size
                                    for an array of rank ~A with dimensions ~A */
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 5));
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 6));
			COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 7));
			Ferror(STACK(base, 4), 4);
		}
		else
		{
			GEN_CLOSURE(array, STACK(base, 4), 5, Z56_g158, 1);
			COPY(STACK(base, 1), &array[3]);
			COPY(STACK(base, 0), &array[4]);
			LOAD_CLOSURE(array, STACK(base, 4));
			COPY(STACK(base, 4), STACK(base, 4));
			COPY(STACK(base, 2), STACK(base, 5));
			Fmapc(STACK(base, 4), 2);
			COPY(STACK(base, 4), STACK(base, 0));
		}
	}
}

static void Z56_g158(CL_FORM *base)
{
	COPY(GET_FORM(STACK(base, 0)) + 4, STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	Fcdr(STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	Z55_chk_shape_of_init_cont(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}
