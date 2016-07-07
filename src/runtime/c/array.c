/*------------------------------------------------------------------------------
 * CLiCC: The Common Lisp to C Compiler
 * Copyright (C) 1994 Wolfgang Goerigk, Ulrich Hoffmann, Heinz Knutzen 
 * Christian-Albrechts-Universitaet zu Kiel, Germany
 *------------------------------------------------------------------------------
 * CLiCC has been developed as part of the APPLY research project,
 * funded by the German Ministry of Research and Technology.
 * 
 * CLiCC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CLiCC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License in file COPYING for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *------------------------------------------------------------------------------
 * Funktion : Arrays
 *
 * $Revision: 1.21 $
 * $Id: array.c,v 1.21 1994/11/22 14:54:01 hk Exp $
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"


/* Zusicherung: alle Parameter sind vom korrekten Typ! (ausser bei *_p) */
/*----------------------------------------------------------------------*/

/* lokales define */
#define HEADER_SIZE  1


/*------------------------------------------------------------------------------
 * RT::PLAIN-VECTOR-ELEMENT-CODE vector
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_plain_vector_element_code)
{
   /* 2 Zeilen notwendig wegen Seiteneffekte in den LOAD-Makros */
   int code = GET_VECTOR_CODE(ARG(0)); 
   LOAD_FIXNUM(ARG(0), code, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::MAKE-VECTOR-T size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_make_vector_t)
{
   long i, size = GET_FIXNUM(ARG(0));
   CL_FORM *vector;
   
   vector = form_alloc(ARG(1), size + HEADER_SIZE);
   for (i = 1; i <= size; i++)
      LOAD_NIL(OFFSET(vector, i));
   
   INIT_VEC_T(vector, size);
   LOAD_VEC_T(vector, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::MAKE-VECTOR-FIXNUM size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_make_vector_fixnum)
{
   long size = GET_FIXNUM(ARG(0));
   long *data = fixnum_alloc(ARG(0), size);
   CL_FORM *vector;

   vector = form_alloc(ARG(0), 1 + HEADER_SIZE);
   LOAD_FIXNUM_PTR(data, AR_BASE(vector));
   INIT_VEC_FIXNUM(vector, size);
   LOAD_VEC_FIXNUM(vector, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::MAKE-VECTOR-FLOAT size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_make_vector_float)
{
   long size = GET_FIXNUM(ARG(0));
   double *data = float_alloc(ARG(0), size);
   CL_FORM *vector;

   vector = form_alloc(ARG(0), 1 + HEADER_SIZE);
   LOAD_FLOAT_PTR(data, AR_BASE(vector));
   INIT_VEC_FLOAT(vector, size);
   LOAD_VEC_FLOAT(vector, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::MAKE-VECTOR-CHAR size initvalue
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_make_vector_char)
{
   long size = GET_FIXNUM(ARG(0));
   char initvalue = GET_CHAR(ARG(1));
   char *data = char_alloc(ARG(0), size);
   CL_FORM *vector;

   vector = form_alloc(ARG(0), 1 + HEADER_SIZE);
   memset(data, initvalue, size);
   LOAD_CHAR_PTR(data, AR_BASE(vector));
   INIT_VEC_CHAR(vector, size);
   LOAD_VEC_CHAR(vector, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::MAKE_VECTOR_BIT size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_make_vector_bit)
{
   long size = GET_FIXNUM(ARG(0));
   CL_FORM *vector;

   vector = form_alloc(ARG(0), 1 + HEADER_SIZE);
   LOAD_BITS_PTR(bits_alloc(ARG(0), size), AR_BASE(vector));
   INIT_VEC_BIT(vector, size);
   LOAD_VEC_BIT(vector, ARG(0));
}


/*------------------------------------------------------------------------------
 * RT::PVREF vector index
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_pvref)
{
   CL_FORM *vector = GET_FORM(ARG(0));
   long index = GET_FIXNUM(ARG(1));
   
   switch (GET_VECTOR_CODE(ARG(0)))
   {
   case VT_T:
      COPY(OFFSET(AR_BASE(vector), index), ARG(0));
      break;
   case VT_FIXNUM:
      LOAD_FIXNUM(ARG(0), FIXNUM_AR(vector)[index], ARG(0));
      break;
   case VT_FLOAT:
      LOAD_FLOAT(ARG(0), &FLOAT_AR(vector)[index], ARG(0));
      break;
   case VT_CHARACTER:
      LOAD_CHAR(ARG(0), CHAR_AR(vector)[index], ARG(0));
      break;
   case VT_BIT:
      rt_sbvref(base);
      break;
   default:
      Labort("unknown array type");
   }
}
   
/*------------------------------------------------------------------------------
 * RT::SET-PVREF value vector index
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_set_pvref)
{
   CL_FORM *vector = GET_FORM(ARG(1));
   long index = GET_FIXNUM(ARG(2));
   
   switch (GET_VECTOR_CODE(ARG(1)))
   {
   case VT_T:
      COPY(ARG(0), OFFSET(AR_BASE(vector), index));
      break;
   case VT_FIXNUM:
      FIXNUM_AR(vector)[index] = GET_FIXNUM(ARG(0));
      break;
   case VT_FLOAT:
      FLOAT_AR(vector)[index] = GET_FLOAT(ARG(0));
      break;
   case VT_CHARACTER:
      CHAR_AR(vector)[index] = GET_CHAR(ARG(0));
      break;
   case VT_BIT:
      rt_set_sbvref(base);
      break;
   default:
      Labort("unknown array type");
   }
}

/*------------------------------------------------------------------------------
 * RT::SBVREF bit-vector index
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_sbvref)
{
   long *bit_vector = BIT_AR(GET_FORM(ARG(0)));
   long index = GET_FIXNUM(ARG(1));
   
   if (bit_vector[index / BITS_PER_FIXNUM] & (1L << (index % BITS_PER_FIXNUM)))
      LOAD_FIXNUM(ARG(0), 1, ARG(0));
   else
      LOAD_FIXNUM(ARG(0), 0, ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::SET-SBVREF value bit-vector index
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_set_sbvref)
{
   long *bit_vector = BIT_AR(GET_FORM(ARG(1)));
   long index = GET_FIXNUM(ARG(2));
   
   if (GET_FIXNUM(ARG(0)))
      bit_vector[index / BITS_PER_FIXNUM] |= (1L << (index % BITS_PER_FIXNUM));
   else
      bit_vector[index / BITS_PER_FIXNUM] &=~(1L << (index % BITS_PER_FIXNUM));
}

/*------------------------------------------------------------------------------
 * RT::BITOP opcode bit-array1 bit-array2 result-bit-array
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_bitop)
{
   long *a1 = BIT_AR(GET_FORM(ARG(1)));
   long *a2 = BIT_AR(GET_FORM(ARG(2)));
   long *a3 = BIT_AR(GET_FORM(ARG(3)));
   long i, size = AR_SIZE(GET_FORM(ARG(1)));
   
   size = (size + BITS_PER_FIXNUM - 1) / BITS_PER_FIXNUM;
   
   switch (GET_FIXNUM(ARG(0)))
   {
   case 0:                      /* and */
      for (i = 0; i < size; i++)
         a3[i] = a1[i] & a2[i];
      break;
   case 1:                      /* ior */
      for (i = 0; i < size; i++)
         a3[i] = a1[i] | a2[i];
      break;
   case 2:                      /* xor */
      for (i = 0; i < size; i++)
         a3[i] = a1[i] ^ a2[i];
      break;
   case 3:                      /* eqv */  
      for (i = 0; i < size; i++)
         a3[i] = ~(a1[i] ^ a2[i]);
      break;
   case 4:                      /* nand */
      for (i = 0; i < size; i++)
         a3[i] = ~(a1[i] & a2[i]);
      break;
   case 5:                      /* nor */
      for (i = 0; i < size; i++)
         a3[i] = ~(a1[i] | a2[i]);
      break;
   case 6:                      /* andc1 */
      for (i = 0; i < size; i++)
         a3[i] = ~a1[i] & a2[i];
      break;
   case 7:                      /* andc2 */
      for (i = 0; i < size; i++)
         a3[i] = a1[i] & ~a2[i];
      break;
   case 8:                      /* orc1 */
      for (i = 0; i < size; i++)
         a3[i] = ~a1[i] | a2[i];
      break;
   case 9:                      /* orc2 */
      for (i = 0; i < size; i++)
         a3[i] = a1[i] | ~a2[i];
      break;
   case 10:                     /* not */
      for (i = 0; i < size; i++)
         a3[i] = ~a1[i];
      break;
   default:
      Labort("unknown opcode for bit-op");
   }
   COPY(ARG(3), ARG(0));
}

/*------------------------------------------------------------------------------
 * RT::SHRINK-SMSTR simple-string new-size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_shrink_smstr)
{
   long new_size = GET_FIXNUM(ARG(1));
   /* Neue Endnull, falls String später in C-String verwandelt werden soll */
   CHAR_AR(AR_BASE(GET_FORM(ARG(0))))[new_size] = '\0';
   SET_AR_SIZE(new_size, GET_FORM(ARG(0)));
}

/*------------------------------------------------------------------------------
 * RT::SHRINK-PVECTOR vector new-size
 *----------------------------------------------------------------------------*/
LISP_FUN(rt_shrink_pvector)
{
   long new_size = GET_FIXNUM(ARG(1));
   SET_AR_SIZE(new_size, GET_FORM(ARG(0)));
}

