#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

GLOBAL_FUNARG CFset_gethash = {Fset_gethash, -4};

static void Z83_g909(CL_FORM *base);

void Fset_gethash(CL_FORM *base, int nargs)
{
	switch(nargs)
	{
		case 3:
		LOAD_NIL(STACK(base, 3));
		case 4:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(STACK(base, 2), STACK(base, 4));
	LOAD_FIXNUM(0, STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 6));	/* HASH-TABLE */
	struct_ref(STACK(base, 4));
	COPY(STACK(base, 1), STACK(base, 5));
	LOAD_FIXNUM(0, STACK(base, 6));
	internal_sxhash(STACK(base, 5));
	COPY(STACK(base, 4), STACK(base, 6));
	Fmod(STACK(base, 5));
	COPY(STACK(base, 1), STACK(base, 6));
	COPY(STACK(base, 2), STACK(base, 7));
	LOAD_FIXNUM(5, STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 9));	/* HASH-TABLE */
	struct_ref(STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	vref(STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 8));	/* TEST */
	COPY(STACK(base, 2), STACK(base, 9));
	LOAD_FIXNUM(4, STACK(base, 10));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 11));	/* HASH-TABLE */
	struct_ref(STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 139), STACK(base, 10));	/* KEY */
	LOAD_GLOBFUN(&CFcar, STACK(base, 11));
	Ffind(STACK(base, 6), 6);
	if(CL_TRUEP(STACK(base, 6)))
	{
		COPY(STACK(base, 6), STACK(base, 7));
		COPY(STACK(base, 0), STACK(base, 8));
		if(CL_CONSP(STACK(base, 6)))
		{
			COPY(STACK(base, 0), CDR(GET_FORM(STACK(base, 6))));
		}
		else
		{
			LOAD_SMSTR((CL_FORM *)&KClisp[6], STACK(base, 9));	/* ~a is not a cons */
			COPY(STACK(base, 6), STACK(base, 10));
			Ferror(STACK(base, 9), 2);
		}
	}
	else
	{
		COPY(STACK(base, 2), STACK(base, 7));
		LOAD_FIXNUM(1, STACK(base, 8));
		LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 9));	/* HASH-TABLE */
		struct_ref(STACK(base, 7));
		F1plus(STACK(base, 7));
		COPY(STACK(base, 4), STACK(base, 8));
		Fdiv(STACK(base, 7), 2);
		COPY(STACK(base, 2), STACK(base, 8));
		LOAD_FIXNUM(3, STACK(base, 9));
		LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 10));	/* HASH-TABLE */
		struct_ref(STACK(base, 8));
		Fgt(STACK(base, 7), 2);
		if(CL_TRUEP(STACK(base, 7)))
		{
			COPY(STACK(base, 2), STACK(base, 7));
			LOAD_FIXNUM(2, STACK(base, 8));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 9));	/* HASH-TABLE */
			struct_ref(STACK(base, 7));
			if(CL_FIXNUMP(STACK(base, 7)))
			{
				COPY(STACK(base, 4), STACK(base, 8));
				COPY(STACK(base, 7), STACK(base, 9));
				Fplus(STACK(base, 8), 2);
			}
			else
			{
				COPY(STACK(base, 4), STACK(base, 8));
				COPY(STACK(base, 7), STACK(base, 9));
				Fmult(STACK(base, 8), 2);
				LOAD_FIXNUM(1, STACK(base, 9));
				ceiling_internal(STACK(base, 8));
			}
			LOAD_SYMBOL(SYMBOL(Slisp, 335), STACK(base, 9));	/* SIZE */
			COPY(STACK(base, 8), STACK(base, 10));
			LOAD_SYMBOL(SYMBOL(Slisp, 213), STACK(base, 11));	/* TEST */
			COPY(STACK(base, 2), STACK(base, 12));
			LOAD_FIXNUM(4, STACK(base, 13));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 14));	/* HASH-TABLE */
			struct_ref(STACK(base, 12));
			LOAD_SYMBOL(SYMBOL(Slisp, 337), STACK(base, 13));	/* REHASH-THRESHOLD */
			COPY(STACK(base, 2), STACK(base, 14));
			LOAD_FIXNUM(3, STACK(base, 15));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 16));	/* HASH-TABLE */
			struct_ref(STACK(base, 14));
			Fmake_hash_table(STACK(base, 9), 6);
			GEN_HEAPVAR(STACK(base, 9), STACK(base, 10));
			{
				GEN_CLOSURE(array, STACK(base, 10), 4, Z83_g909, 2);
				COPY(STACK(base, 9), &array[3]);
				LOAD_CLOSURE(array, STACK(base, 10));
			}
			COPY(STACK(base, 10), STACK(base, 10));
			COPY(STACK(base, 2), STACK(base, 11));
			Fmaphash(STACK(base, 10));
			COPY(STACK(base, 0), STACK(base, 10));
			COPY(STACK(base, 1), STACK(base, 11));
			COPY(INDIRECT(STACK(base, 9)), STACK(base, 12));
			Fset_gethash(STACK(base, 10), 3);
			COPY(STACK(base, 2), STACK(base, 10));
			COPY(INDIRECT(STACK(base, 9)), STACK(base, 11));
			LOAD_FIXNUM(0, STACK(base, 12));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 13));	/* HASH-TABLE */
			struct_ref(STACK(base, 11));
			COPY(STACK(base, 2), STACK(base, 12));
			LOAD_FIXNUM(0, STACK(base, 13));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 14));	/* HASH-TABLE */
			COPY(STACK(base, 11), STACK(base, 15));
			set_struct_ref(STACK(base, 12));
			COPY(STACK(base, 2), STACK(base, 10));
			COPY(INDIRECT(STACK(base, 9)), STACK(base, 11));
			LOAD_FIXNUM(1, STACK(base, 12));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 13));	/* HASH-TABLE */
			struct_ref(STACK(base, 11));
			COPY(STACK(base, 2), STACK(base, 12));
			LOAD_FIXNUM(1, STACK(base, 13));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 14));	/* HASH-TABLE */
			COPY(STACK(base, 11), STACK(base, 15));
			set_struct_ref(STACK(base, 12));
			COPY(STACK(base, 2), STACK(base, 10));
			COPY(INDIRECT(STACK(base, 9)), STACK(base, 11));
			LOAD_FIXNUM(5, STACK(base, 12));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 13));	/* HASH-TABLE */
			struct_ref(STACK(base, 11));
			COPY(STACK(base, 2), STACK(base, 12));
			LOAD_FIXNUM(5, STACK(base, 13));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 14));	/* HASH-TABLE */
			COPY(STACK(base, 11), STACK(base, 15));
			set_struct_ref(STACK(base, 12));
		}
		else
		{
			COPY(STACK(base, 2), STACK(base, 7));
			COPY(STACK(base, 2), STACK(base, 8));
			LOAD_FIXNUM(1, STACK(base, 9));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 10));	/* HASH-TABLE */
			struct_ref(STACK(base, 8));
			F1plus(STACK(base, 8));
			COPY(STACK(base, 2), STACK(base, 9));
			LOAD_FIXNUM(1, STACK(base, 10));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 11));	/* HASH-TABLE */
			COPY(STACK(base, 8), STACK(base, 12));
			set_struct_ref(STACK(base, 9));
			COPY(STACK(base, 2), STACK(base, 7));
			LOAD_FIXNUM(5, STACK(base, 8));
			LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 9));	/* HASH-TABLE */
			struct_ref(STACK(base, 7));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 8), 2);
				COPY(STACK(base, 1), CAR(lptr));
				COPY(STACK(base, 0), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 8));
			}
			COPY(STACK(base, 7), STACK(base, 9));
			COPY(STACK(base, 5), STACK(base, 10));
			vref(STACK(base, 9));
			{
				CL_FORM *lptr;
				lptr = form_alloc(STACK(base, 10), 2);
				COPY(STACK(base, 8), CAR(lptr));
				COPY(STACK(base, 9), CDR(lptr));
				LOAD_CONS(lptr, STACK(base, 8));
			}
			COPY(STACK(base, 8), STACK(base, 9));
			COPY(STACK(base, 7), STACK(base, 10));
			COPY(STACK(base, 5), STACK(base, 11));
			Fset_aref(STACK(base, 9), 3);
		}
	}
}

static void Z83_g909(CL_FORM *base)
{
	COPY(STACK(base, 2), STACK(base, 3));
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 5));
	Fset_gethash(STACK(base, 3), 3);
	COPY(STACK(base, 3), STACK(base, 0));
}
