#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(Kmake_standard_readtable)
{
	MAKE_CHAR('\t'),	/* 0 CONS(0) */
	MAKE_LIST(&Kmake_standard_readtable[2]),
	MAKE_CHAR('\n'),	/* CONS(2) */
	MAKE_LIST(&Kmake_standard_readtable[4]),
	MAKE_CHAR('\f'),	/* CONS(4) */
	MAKE_LIST(&Kmake_standard_readtable[6]),
	MAKE_CHAR('\r'),	/* CONS(6) */
	MAKE_LIST(&Kmake_standard_readtable[8]),
	MAKE_CHAR(' '),	/* CONS(8) */
	MAKE_NIL,
};

void make_standard_readtable(CL_FORM *base)
{
	LOAD_FIXNUM(256, STACK(base, 0));
	LOAD_SYMBOL(SYMBOL(Slisp, 123), STACK(base, 1));	/* INITIAL-ELEMENT */
	LOAD_NIL(STACK(base, 2));
	Fmake_array(STACK(base, 0), 3);
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 1));	/* READTABLE */
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	make_struct(STACK(base, 1), 3);
	COPY(STACK(base, 1), STACK(base, 0));
	COPY(STACK(base, 0), STACK(base, 1));
	LOAD_FIXNUM(0, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 3));	/* READTABLE */
	struct_ref(STACK(base, 1));
	LOAD_NIL(STACK(base, 2));
	LOAD_CONS((CL_FORM *)&Kmake_standard_readtable[0], STACK(base, 3));
	M107_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN119;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	Fchar_code(STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 390), STACK(base, 5));	/* WHITESPACE */
	COPY(STACK(base, 1), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fset_aref(STACK(base, 5), 3);
	Fcdr(STACK(base, 3));
	goto M107_1;
	RETURN119:;
	LOAD_CHAR('\b', STACK(base, 2));
	Fchar_code(STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 393), STACK(base, 3));	/* CONSTITUENT */
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fset_aref(STACK(base, 3), 3);
	LOAD_CHAR('!', STACK(base, 2));
	Fchar_code(STACK(base, 2));
	M108_1:;
	COPY(STACK(base, 2), STACK(base, 3));
	LOAD_CHAR('\177', STACK(base, 4));
	Fchar_code(STACK(base, 4));
	Fge(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		goto RETURN120;
	}
	LOAD_SYMBOL(SYMBOL(Slisp, 393), STACK(base, 3));	/* CONSTITUENT */
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fset_aref(STACK(base, 3), 3);
	F1plus(STACK(base, 2));
	goto M108_1;
	RETURN120:;
	LOAD_CHAR('\\', STACK(base, 2));
	Fchar_code(STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 391), STACK(base, 3));	/* SINGLE-ESCAPE */
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fset_aref(STACK(base, 3), 3);
	LOAD_CHAR('|', STACK(base, 2));
	Fchar_code(STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 392), STACK(base, 3));	/* MULTIPLE-ESCAPE */
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fset_aref(STACK(base, 3), 3);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 3));	/* T */
	COPY(STACK(base, 0), STACK(base, 4));
	Fmake_dispatch_macro_character(STACK(base, 2), 3);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('\\', STACK(base, 3));
	LOAD_GLOBFUN(&Cchar_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('\'', STACK(base, 3));
	LOAD_GLOBFUN(&Cfunction_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('(', STACK(base, 3));
	LOAD_GLOBFUN(&Cvector_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR(':', STACK(base, 3));
	LOAD_GLOBFUN(&Cuninterned_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('B', STACK(base, 3));
	LOAD_GLOBFUN(&Cbinary_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('O', STACK(base, 3));
	LOAD_GLOBFUN(&Coctal_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('X', STACK(base, 3));
	LOAD_GLOBFUN(&Chex_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('R', STACK(base, 3));
	LOAD_GLOBFUN(&Cradix_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('A', STACK(base, 3));
	LOAD_GLOBFUN(&Carray_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('S', STACK(base, 3));
	LOAD_GLOBFUN(&Cstruct_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('+', STACK(base, 3));
	LOAD_GLOBFUN(&Cfeature_plus_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('-', STACK(base, 3));
	LOAD_GLOBFUN(&Cfeature_minus_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('#', STACK(base, 2));
	LOAD_CHAR('|', STACK(base, 3));
	LOAD_GLOBFUN(&Ccomment_block_reader, STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_dispatch_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('\'', STACK(base, 2));
	LOAD_GLOBFUN(&Cquote_reader, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('(', STACK(base, 2));
	LOAD_GLOBFUN(&Ccons_reader, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_macro_character(STACK(base, 2), 4);
	LOAD_CHAR(')', STACK(base, 2));
	LOAD_GLOBFUN(&Cright_parenthesis_reader, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_macro_character(STACK(base, 2), 4);
	LOAD_CHAR(',', STACK(base, 2));
	LOAD_GLOBFUN(&Ccomma_reader, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_macro_character(STACK(base, 2), 4);
	LOAD_CHAR(';', STACK(base, 2));
	LOAD_GLOBFUN(&Csemicolon_reader, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('\"', STACK(base, 2));
	LOAD_GLOBFUN(&Cstring_reader, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_macro_character(STACK(base, 2), 4);
	LOAD_CHAR('`', STACK(base, 2));
	LOAD_GLOBFUN(&Cbackquote_reader, STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	COPY(STACK(base, 0), STACK(base, 5));
	Fset_macro_character(STACK(base, 2), 4);
}
