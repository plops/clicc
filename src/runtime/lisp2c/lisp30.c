#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z42_g150(CL_FORM *base);
static void Z43_g149(CL_FORM *base);
static void Z44_g148(CL_FORM *base);
static void Z45_g147(CL_FORM *base);
static void Z46_g146(CL_FORM *base);
static void Z47_g145(CL_FORM *base);
static void Z48_g144(CL_FORM *base);

void make_file_io_stream(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 101), STACK(base, 2));	/* FILE-IO */
	{
		GEN_CLOSURE(array, STACK(base, 3), 4, Z48_g144, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 3));
	}
	COPY(STACK(base, 3), STACK(base, 3));
	{
		GEN_CLOSURE(array, STACK(base, 4), 4, Z47_g145, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 4));
	}
	COPY(STACK(base, 4), STACK(base, 4));
	{
		GEN_CLOSURE(array, STACK(base, 5), 4, Z46_g146, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 5));
	}
	COPY(STACK(base, 5), STACK(base, 5));
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z45_g147, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	{
		GEN_CLOSURE(array, STACK(base, 7), 4, Z44_g148, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 7));
	}
	COPY(STACK(base, 7), STACK(base, 7));
	{
		GEN_CLOSURE(array, STACK(base, 8), 4, Z43_g149, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 8));
	}
	COPY(STACK(base, 8), STACK(base, 8));
	{
		GEN_CLOSURE(array, STACK(base, 9), 4, Z42_g150, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 9));
	}
	COPY(STACK(base, 9), STACK(base, 9));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 10));	/* STREAM */
	COPY(STACK(base, 2), STACK(base, 11));
	COPY(STACK(base, 1), STACK(base, 12));
	COPY(STACK(base, 3), STACK(base, 13));
	COPY(STACK(base, 4), STACK(base, 14));
	COPY(STACK(base, 8), STACK(base, 15));
	LOAD_GLOBFUN(&Cnil_fun, STACK(base, 16));
	COPY(STACK(base, 5), STACK(base, 17));
	COPY(STACK(base, 6), STACK(base, 18));
	COPY(STACK(base, 7), STACK(base, 19));
	COPY(STACK(base, 9), STACK(base, 20));
	make_struct(STACK(base, 10), 11);
	COPY(STACK(base, 10), STACK(base, 0));
}

static void Z42_g150(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	c_fclose(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 2));
	close1(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z43_g149(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	c_fputc(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z44_g148(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_file_length(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z45_g147(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	c_fseek(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z46_g146(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_ftell(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z47_g145(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	c_ungetc(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z48_g144(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_fgetc(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}
