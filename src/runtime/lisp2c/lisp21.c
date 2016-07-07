#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

CL_INIT2(KFmake_string_input_stream)
{
	MAKE_STRING(23, "reached start of stream"),	/* 0 */
	MAKE_STRING(22, "%s should be eql to %s"),	/* 2 */
	MAKE_STRING(16, "illegal position"),	/* 4 */
};

static void Z20_g119(CL_FORM *base);
static void Z21_g118(CL_FORM *base);
static void Z22_g117(CL_FORM *base);
static void Z23_g115(CL_FORM *base);
static void Z24_g112(CL_FORM *base);

void Fmake_string_input_stream(CL_FORM *base, int nargs)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 0 + nargs));
	if(nargs > 3)
	{
		goto ALL_OPT_SUPPLIED;
	}
	switch(nargs)
	{
		case 3:
		ALL_OPT_SUPPLIED:;
		GEN_HEAPVAR(STACK(base, 2), STACK(base, 0 + nargs));
		case 2:
		GEN_HEAPVAR(STACK(base, 1), STACK(base, 0 + nargs));
	}
	switch(nargs)
	{
		case 1:
		LOAD_FIXNUM(0, STACK(base, 1));
		GEN_HEAPVAR(STACK(base, 1), STACK(base, 2));
		case 2:
		COPY(INDIRECT(STACK(base, 0)), STACK(base, 2));
		Flength(STACK(base, 2));
		GEN_HEAPVAR(STACK(base, 2), STACK(base, 3));
		case 3:
		break;
		default:
		Labort(TOO_MANY_ARGS);
	}
	COPY(INDIRECT(STACK(base, 1)), STACK(base, 3));
	GEN_HEAPVAR(STACK(base, 3), STACK(base, 4));
	LOAD_SYMBOL(SYMBOL(Slisp, 79), STACK(base, 4));	/* TYPE */
	LOAD_SYMBOL(SYMBOL(Slisp, 94), STACK(base, 5));	/* STRING-INPUT */
	LOAD_SYMBOL(SYMBOL(Slisp, 81), STACK(base, 6));	/* READC */
	{
		GEN_CLOSURE(array, STACK(base, 7), 6, Z24_g112, 0);
		COPY(STACK(base, 0), &array[3]);
		COPY(STACK(base, 2), &array[4]);
		COPY(STACK(base, 3), &array[5]);
		LOAD_CLOSURE(array, STACK(base, 7));
	}
	COPY(STACK(base, 7), STACK(base, 7));
	LOAD_SYMBOL(SYMBOL(Slisp, 82), STACK(base, 8));	/* UNREADC */
	{
		GEN_CLOSURE(array, STACK(base, 9), 6, Z23_g115, 1);
		COPY(STACK(base, 0), &array[3]);
		COPY(STACK(base, 1), &array[4]);
		COPY(STACK(base, 3), &array[5]);
		LOAD_CLOSURE(array, STACK(base, 9));
	}
	COPY(STACK(base, 9), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 85), STACK(base, 10));	/* TELL */
	{
		GEN_CLOSURE(array, STACK(base, 11), 4, Z22_g117, 0);
		COPY(STACK(base, 3), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 11));
	}
	COPY(STACK(base, 11), STACK(base, 11));
	LOAD_SYMBOL(SYMBOL(Slisp, 86), STACK(base, 12));	/* SEEK */
	{
		GEN_CLOSURE(array, STACK(base, 13), 5, Z21_g118, 1);
		COPY(STACK(base, 3), &array[3]);
		COPY(STACK(base, 2), &array[4]);
		LOAD_CLOSURE(array, STACK(base, 13));
	}
	COPY(STACK(base, 13), STACK(base, 13));
	LOAD_SYMBOL(SYMBOL(Slisp, 87), STACK(base, 14));	/* LENGTH */
	{
		GEN_CLOSURE(array, STACK(base, 15), 4, Z20_g119, 0);
		COPY(STACK(base, 2), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 15));
	}
	COPY(STACK(base, 15), STACK(base, 15));
	LOAD_SYMBOL(SYMBOL(Slisp, 88), STACK(base, 16));	/* CLOSE */
	LOAD_GLOBFUN(&Cclose1, STACK(base, 17));
	make_stream(STACK(base, 4), 14);
	COPY(STACK(base, 4), STACK(base, 0));
}

static void Z20_g119(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
}

static void Z21_g118(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 3));
	Fle(STACK(base, 2), 2);
	if(CL_TRUEP(STACK(base, 2)))
	{
		COPY(STACK(base, 1), INDIRECT(GET_FORM(STACK(base, 0)) + 3));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_string_input_stream[4], STACK(base, 2));	/* illegal position */
		Ferror(STACK(base, 2), 1);
	}
}

static void Z22_g117(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 0));
}

static void Z23_g115(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 3));
	Fle(STACK(base, 2), 2);
	if(CL_TRUEP(STACK(base, 2)))
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_string_input_stream[0], STACK(base, 2));	/* reached start of stream */
		Ferror(STACK(base, 2), 1);
	}
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 2));
	F1minus(STACK(base, 2));
	COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 5));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 3));
	Fchar(STACK(base, 2));
	if(EQ(STACK(base, 1), STACK(base, 2)) || CL_FLOATP(STACK(base, 1)) && GET_FLOAT(STACK(base, 1)) == GET_FLOAT(STACK(base, 2)))
	{
	}
	else
	{
		LOAD_SMSTR((CL_FORM *)&KFmake_string_input_stream[2], STACK(base, 2));	/* %s should be eql to %s */
		COPY(STACK(base, 1), STACK(base, 3));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 4));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 5));
		Fchar(STACK(base, 4));
		Ferror(STACK(base, 2), 3);
	}
	LOAD_NIL(STACK(base, 0));
}

static void Z24_g112(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 1));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 4), STACK(base, 2));
	Flt(STACK(base, 1), 2);
	if(CL_TRUEP(STACK(base, 1)))
	{
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 2));
		Fchar(STACK(base, 1));
		COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 5), STACK(base, 2));
		F1plus(STACK(base, 2));
		COPY(STACK(base, 2), INDIRECT(GET_FORM(STACK(base, 0)) + 5));
		COPY(STACK(base, 1), STACK(base, 0));
	}
	else
	{
		LOAD_NIL(STACK(base, 0));
	}
}
