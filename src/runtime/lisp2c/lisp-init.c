#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KIlisp)
{
	MAKE_STRING(4, "LISP"),	/* 0 */
	MAKE_SYMREF(SYMBOL(Ssys, 1)),	/* 2 CONS(2) T */
	MAKE_LIST(&KIlisp[4]),
	MAKE_NIL,	/* CONS(4) */
	MAKE_NIL,
	MAKE_STRING(5, "COMMA"),	/* 4 */
	MAKE_STRING(12, "COMMA-ATSIGN"),	/* 6 */
	MAKE_STRING(9, "COMMA-DOT"),	/* 8 */
	MAKE_STRING(7, "BQ-LIST"),	/* 10 */
	MAKE_STRING(9, "BQ-APPEND"),	/* 12 */
	MAKE_STRING(8, "BQ-LIST*"),	/* 14 */
	MAKE_STRING(8, "BQ-NCONC"),	/* 16 */
	MAKE_STRING(14, "BQ-CLOBBERABLE"),	/* 18 */
	MAKE_STRING(8, "BQ-QUOTE"),	/* 20 */
	MAKE_STRING(6, "VECTOR"),	/* 22 */
	MAKE_CHAR('\t'),	/* 24 CONS(26) */
	MAKE_LIST(&KIlisp[28]),
	MAKE_CHAR('\n'),	/* CONS(28) */
	MAKE_LIST(&KIlisp[30]),
	MAKE_CHAR('\f'),	/* CONS(30) */
	MAKE_LIST(&KIlisp[32]),
	MAKE_CHAR('\r'),	/* CONS(32) */
	MAKE_LIST(&KIlisp[34]),
	MAKE_CHAR(' '),	/* CONS(34) */
	MAKE_NIL,
	MAKE_STRING(1, "G"),	/* 26 */
	MAKE_NIL,	/* 28 CONS(38) */
	MAKE_LIST(&KIlisp[40]),
	MAKE_SYMREF(SYMBOL(Ssys, 1)),	/* CONS(40) T */
	MAKE_NIL,
	MAKE_STRING(7, "KEYWORD"),	/* 30 */
	MAKE_STRING(2, "RT"),	/* 32 */
	MAKE_STRING(3, "FFI"),	/* 34 */
	MAKE_STRING(4, "LISP"),	/* 36 */
	MAKE_STRING(2, "RT"),	/* 38 */
};

void Ilisp(CL_FORM *base)
{
	ONLY_ONCE;
	make_terminal_io(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 58));	/* *TERMINAL-IO* */
	LOAD_SYMBOL(SYMBOL(Slisp, 58), STACK(base, 0));	/* *TERMINAL-IO* */
	Fmake_synonym_stream(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 59));	/* *STANDARD-INPUT* */
	COPY(SYMVAL(Slisp, 59), SYMVAL(Slisp, 60));	/* *STANDARD-INPUT* *STANDARD-OUTPUT* */
	COPY(SYMVAL(Slisp, 59), SYMVAL(Slisp, 61));	/* *STANDARD-INPUT* *ERROR-OUTPUT* */
	COPY(SYMVAL(Slisp, 59), SYMVAL(Slisp, 62));	/* *STANDARD-INPUT* *QUERY-IO* */
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 0));	/* STREAM */
	LOAD_GLOBFUN(&Cmake_stream, STACK(base, 1));
	LOAD_GLOBFUN(&Cmake_stream, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 3));	/* STREAM */
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 0));	/* PATTERN */
	LOAD_GLOBFUN(&Cmake_pattern, STACK(base, 1));
	LOAD_GLOBFUN(&Cmake_pattern, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 181), STACK(base, 3));	/* PATTERN */
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 0));	/* PATHNAME */
	LOAD_GLOBFUN(&CPmake_pathname, STACK(base, 1));
	LOAD_GLOBFUN(&CPmake_pathname, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 3));	/* PATHNAME */
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 0));	/* HOST */
	LOAD_GLOBFUN(&Cmake_host, STACK(base, 1));
	LOAD_GLOBFUN(&Cmake_host, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 3));	/* HOST */
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	LOAD_NIL(SYMVAL(Slisp, 218));	/* *IGNORE-WILDCARDS* */
	LOAD_SYMBOL(SYMBOL(Slisp, 194), STACK(base, 0));	/* HOST */
	LOAD_GLOBFUN(&Cparse_unix_namestring, STACK(base, 1));
	LOAD_GLOBFUN(&Cunparse_unix_namestring, STACK(base, 2));
	LOAD_GLOBFUN(&Cunparse_unix_host, STACK(base, 3));
	LOAD_GLOBFUN(&Cunparse_unix_directory, STACK(base, 4));
	LOAD_GLOBFUN(&Cunparse_unix_file, STACK(base, 5));
	LOAD_GLOBFUN(&Cunparse_unix_enough, STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 210), STACK(base, 7));	/* LOWER */
	make_struct(STACK(base, 0), 8);
	COPY(STACK(base, 0), SYMVAL(Slisp, 219));	/* *UNIX-HOST* */
	LOAD_SYMBOL(SYMBOL(Slisp, 164), STACK(base, 1));	/* PATHNAME */
	COPY(STACK(base, 0), STACK(base, 2));
	LOAD_NIL(STACK(base, 3));
	LOAD_NIL(STACK(base, 4));
	LOAD_NIL(STACK(base, 5));
	LOAD_NIL(STACK(base, 6));
	LOAD_SYMBOL(SYMBOL(Slisp, 200), STACK(base, 7));	/* NEWEST */
	make_struct(STACK(base, 1), 7);
	COPY(STACK(base, 1), SYMVAL(Slisp, 163));	/* *DEFAULT-PATHNAME-DEFAULTS* */
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 0));	/* PACKAGE */
	LOAD_GLOBFUN(&Craw_make_package, STACK(base, 1));
	LOAD_GLOBFUN(&Craw_make_package, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 273), STACK(base, 3));	/* PACKAGE */
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	LOAD_SMSTR((CL_FORM *)&KIlisp[0], STACK(base, 0));	/* LISP */
	ensure_package(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 272));	/* *PACKAGE* */
	LOAD_CONS((CL_FORM *)&KIlisp[2], STACK(base, 0));
	Fimport(STACK(base, 0), 1);
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 0));	/* HASH-TABLE */
	LOAD_GLOBFUN(&Cnew_hash_table, STACK(base, 1));
	LOAD_GLOBFUN(&Cnew_hash_table, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 327), STACK(base, 3));	/* HASH-TABLE */
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	LOAD_SMSTR((CL_FORM *)&KIlisp[6], STACK(base, 0));	/* COMMA */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 364));	/* *COMMA* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[8], STACK(base, 0));	/* COMMA-ATSIGN */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 365));	/* *COMMA-ATSIGN* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[10], STACK(base, 0));	/* COMMA-DOT */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 366));	/* *COMMA-DOT* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[12], STACK(base, 0));	/* BQ-LIST */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 367));	/* *BQ-LIST* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[14], STACK(base, 0));	/* BQ-APPEND */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 368));	/* *BQ-APPEND* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[16], STACK(base, 0));	/* BQ-LIST* */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 369));	/* *BQ-LIST** */
	LOAD_SMSTR((CL_FORM *)&KIlisp[18], STACK(base, 0));	/* BQ-NCONC */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 370));	/* *BQ-NCONC* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[20], STACK(base, 0));	/* BQ-CLOBBERABLE */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 371));	/* *BQ-CLOBBERABLE* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[22], STACK(base, 0));	/* BQ-QUOTE */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 372));	/* *BQ-QUOTE* */
	LOAD_NIL(STACK(base, 1));
	Flist(STACK(base, 0), 2);
	COPY(STACK(base, 0), SYMVAL(Slisp, 373));	/* *BQ-QUOTE-NIL* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[24], STACK(base, 0));	/* VECTOR */
	Fmake_symbol(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 374));	/* *BQ-VECTOR* */
	LOAD_FIXNUM(0, SYMVAL(Slisp, 375));	/* *BQ-LEVEL* */
	LOAD_FIXNUM(10, SYMVAL(Slisp, 345));	/* *READ-BASE* */
	LOAD_NIL(SYMVAL(Slisp, 346));	/* *READ-SUPPRESS* */
	LOAD_NIL(SYMVAL(Slisp, 347));	/* *FEATURES* */
	LOAD_FIXNUM(80, STACK(base, 0));
	LOAD_SYMBOL(SYMBOL(Slisp, 95), STACK(base, 1));	/* ELEMENT-TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 18), STACK(base, 2));	/* CHARACTER */
	LOAD_SYMBOL(SYMBOL(Slisp, 97), STACK(base, 3));	/* FILL-POINTER */
	LOAD_FIXNUM(0, STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 96), STACK(base, 5));	/* ADJUSTABLE */
	LOAD_SYMBOL(SYMBOL(Ssys, 1), STACK(base, 6));	/* T */
	Fmake_array(STACK(base, 0), 7);
	COPY(STACK(base, 0), SYMVAL(Slisp, 376));	/* *TOKEN* */
	LOAD_NIL(SYMVAL(Slisp, 377));	/* *UNINTERNED* */
	LOAD_NIL(SYMVAL(Slisp, 378));	/* *PRESERVE-WHITESPACE* */
	LOAD_NIL(SYMVAL(Slisp, 379));	/* *DOT-FLAG* */
	LOAD_NIL(SYMVAL(Slisp, 380));	/* *PARENTHESIS-OPEN* */
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
	LOAD_CONS((CL_FORM *)&KIlisp[26], STACK(base, 3));
	M245_1:;
	if(CL_ATOMP(STACK(base, 3)))
	{
		LOAD_NIL(STACK(base, 2));
		goto RETURN286;
	}
	COPY(STACK(base, 3), STACK(base, 4));
	Fcar(STACK(base, 4));
	COPY(STACK(base, 4), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 4));
	COPY(STACK(base, 2), STACK(base, 5));
	Fchar_code(STACK(base, 5));
	LOAD_SYMBOL(SYMBOL(Slisp, 390), STACK(base, 6));	/* WHITESPACE */
	COPY(STACK(base, 4), STACK(base, 7));
	COPY(STACK(base, 5), STACK(base, 8));
	Fset_aref(STACK(base, 6), 3);
	Fcdr(STACK(base, 3));
	goto M245_1;
	RETURN286:;
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_CHAR('\b', STACK(base, 3));
	Fchar_code(STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 393), STACK(base, 4));	/* CONSTITUENT */
	COPY(STACK(base, 2), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fset_aref(STACK(base, 4), 3);
	LOAD_CHAR('!', STACK(base, 2));
	Fchar_code(STACK(base, 2));
	M246_1:;
	COPY(STACK(base, 2), STACK(base, 3));
	LOAD_CHAR('\177', STACK(base, 4));
	Fchar_code(STACK(base, 4));
	Fge(STACK(base, 3), 2);
	if(CL_TRUEP(STACK(base, 3)))
	{
		goto RETURN287;
	}
	COPY(STACK(base, 1), STACK(base, 3));
	COPY(STACK(base, 2), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 393), STACK(base, 5));	/* CONSTITUENT */
	COPY(STACK(base, 3), STACK(base, 6));
	COPY(STACK(base, 4), STACK(base, 7));
	Fset_aref(STACK(base, 5), 3);
	F1plus(STACK(base, 2));
	goto M246_1;
	RETURN287:;
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_CHAR('\\', STACK(base, 3));
	Fchar_code(STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 391), STACK(base, 4));	/* SINGLE-ESCAPE */
	COPY(STACK(base, 2), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fset_aref(STACK(base, 4), 3);
	COPY(STACK(base, 1), STACK(base, 2));
	LOAD_CHAR('|', STACK(base, 3));
	Fchar_code(STACK(base, 3));
	LOAD_SYMBOL(SYMBOL(Slisp, 392), STACK(base, 4));	/* MULTIPLE-ESCAPE */
	COPY(STACK(base, 2), STACK(base, 5));
	COPY(STACK(base, 3), STACK(base, 6));
	Fset_aref(STACK(base, 4), 3);
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
	COPY(STACK(base, 0), SYMVAL(Slisp, 381));	/* *STANDARD-READTABLE* */
	LOAD_NIL(STACK(base, 0));
	Fcopy_readtable(STACK(base, 0), 1);
	COPY(STACK(base, 0), SYMVAL(Slisp, 382));	/* *READTABLE* */
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 0));	/* READTABLE */
	LOAD_GLOBFUN(&Cmake_readtable, STACK(base, 1));
	LOAD_GLOBFUN(&Cmake_readtable, STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 348), STACK(base, 3));	/* READTABLE */
	LOAD_SYMBOL(SYMBOL(Slisp, 323), STACK(base, 4));	/* STRUCT-CONSTRUCTOR */
	Fset_get(STACK(base, 2), 3);
	LOAD_SYMBOL(SYMBOL(Ssys, 1), SYMVAL(Slisp, 399));	/* *PRINT-ARRAY* T */
	LOAD_FIXNUM(10, SYMVAL(Slisp, 400));	/* *PRINT-BASE* */
	LOAD_SYMBOL(SYMBOL(Slisp, 421), SYMVAL(Slisp, 401));	/* *PRINT-CASE* UPCASE */
	LOAD_NIL(SYMVAL(Slisp, 402));	/* *PRINT-CIRCLE* */
	LOAD_SYMBOL(SYMBOL(Ssys, 1), SYMVAL(Slisp, 403));	/* *PRINT-ESCAPE* T */
	LOAD_SYMBOL(SYMBOL(Ssys, 1), SYMVAL(Slisp, 404));	/* *PRINT-GENSYM* T */
	LOAD_NIL(SYMVAL(Slisp, 405));	/* *PRINT-LENGTH* */
	LOAD_NIL(SYMVAL(Slisp, 406));	/* *PRINT-LEVEL* */
	LOAD_NIL(SYMVAL(Slisp, 407));	/* *PRINT-PRETTY* */
	LOAD_NIL(SYMVAL(Slisp, 408));	/* *PRINT-RADIX* */
	LOAD_FIXNUM(6, SYMVAL(Slisp, 435));	/* *ERROR-PRINT-LENGTH* */
	LOAD_FIXNUM(6, SYMVAL(Slisp, 436));	/* *ERROR-PRINT-LEVEL* */
	LOAD_SMSTR((CL_FORM *)&KIlisp[36], SYMVAL(Slisp, 595));	/* G *GENSYM-PREFIX* */
	LOAD_FIXNUM(0, SYMVAL(Slisp, 590));	/* *GENSYM-COUNTER* */
	LOAD_FIXNUM(0, SYMVAL(Slisp, 596));	/* *GENTEMP-COUNTER* */
	LOAD_CONS((CL_FORM *)&KIlisp[38], STACK(base, 0));
	Fexport(STACK(base, 0), 1);
	LOAD_SYMBOL(SYMBOL(Slisp, 0), STACK(base, 0));	/* MOST-POSITIVE-FIXNUM */
	LOAD_SMSTR((CL_FORM *)&KIlisp[42], STACK(base, 1));	/* KEYWORD */
	ensure_package(STACK(base, 1));
	LOAD_SMSTR((CL_FORM *)&KIlisp[44], STACK(base, 2));	/* RT */
	ensure_package(STACK(base, 2));
	LOAD_SMSTR((CL_FORM *)&KIlisp[46], STACK(base, 3));	/* FFI */
	ensure_package(STACK(base, 3));
	LOAD_SMSTR((CL_FORM *)&KIlisp[48], STACK(base, 4));	/* LISP */
	ensure_package(STACK(base, 4));
	Fvector(STACK(base, 1), 4);
	setup_symbols_iterator(STACK(base, 0));
	LOAD_SMSTR((CL_FORM *)&KIlisp[50], STACK(base, 0));	/* RT */
	Ffind_package(STACK(base, 0));
	COPY(STACK(base, 0), SYMVAL(Slisp, 272));	/* *PACKAGE* */
}
