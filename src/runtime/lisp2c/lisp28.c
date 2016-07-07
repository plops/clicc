#include <c_decl.h>
#include "sys.h"
#include "lisp.h"

static void Z30_g136(CL_FORM *base);
static void Z31_g135(CL_FORM *base);
static void Z32_g134(CL_FORM *base);
static void Z33_g133(CL_FORM *base);
static void Z34_g132(CL_FORM *base);
static void Z35_g131(CL_FORM *base);

void make_file_input_stream(CL_FORM *base)
{
	GEN_HEAPVAR(STACK(base, 0), STACK(base, 2));
	LOAD_SYMBOL(SYMBOL(Slisp, 100), STACK(base, 2));	/* FILE-INPUT */
	{
		GEN_CLOSURE(array, STACK(base, 3), 4, Z35_g131, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 3));
	}
	COPY(STACK(base, 3), STACK(base, 3));
	{
		GEN_CLOSURE(array, STACK(base, 4), 4, Z34_g132, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 4));
	}
	COPY(STACK(base, 4), STACK(base, 4));
	{
		GEN_CLOSURE(array, STACK(base, 5), 4, Z33_g133, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 5));
	}
	COPY(STACK(base, 5), STACK(base, 5));
	{
		GEN_CLOSURE(array, STACK(base, 6), 4, Z32_g134, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 6));
	}
	COPY(STACK(base, 6), STACK(base, 6));
	{
		GEN_CLOSURE(array, STACK(base, 7), 4, Z31_g135, 0);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 7));
	}
	COPY(STACK(base, 7), STACK(base, 7));
	{
		GEN_CLOSURE(array, STACK(base, 8), 4, Z30_g136, 1);
		COPY(STACK(base, 0), &array[3]);
		LOAD_CLOSURE(array, STACK(base, 8));
	}
	COPY(STACK(base, 8), STACK(base, 8));
	LOAD_SYMBOL(SYMBOL(Slisp, 63), STACK(base, 9));	/* STREAM */
	COPY(STACK(base, 2), STACK(base, 10));
	COPY(STACK(base, 1), STACK(base, 11));
	COPY(STACK(base, 3), STACK(base, 12));
	COPY(STACK(base, 4), STACK(base, 13));
	LOAD_GLOBFUN(&Cundef_stream_op, STACK(base, 14));
	LOAD_GLOBFUN(&Cnil_fun, STACK(base, 15));
	COPY(STACK(base, 5), STACK(base, 16));
	COPY(STACK(base, 6), STACK(base, 17));
	COPY(STACK(base, 7), STACK(base, 18));
	COPY(STACK(base, 8), STACK(base, 19));
	make_struct(STACK(base, 9), 11);
	COPY(STACK(base, 9), STACK(base, 0));
}

static void Z30_g136(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	c_fclose(STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 2));
	close1(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z31_g135(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_file_length(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z32_g134(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 2));
	COPY(STACK(base, 1), STACK(base, 3));
	c_fseek(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z33_g133(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_ftell(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}

static void Z34_g132(CL_FORM *base)
{
	COPY(STACK(base, 1), STACK(base, 2));
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 3));
	c_ungetc(STACK(base, 2));
	COPY(STACK(base, 2), STACK(base, 0));
}

static void Z35_g131(CL_FORM *base)
{
	COPY(INDIRECT(GET_FORM(STACK(base, 0)) + 3), STACK(base, 1));
	c_fgetc(STACK(base, 1));
	COPY(STACK(base, 1), STACK(base, 0));
}
