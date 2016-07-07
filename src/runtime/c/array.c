/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : System-Funktionen (Arrays)                                
 *
 * $Revision: 1.14 $
 * $Log: array.c,v $
 * Revision 1.14  1993/11/25  15:07:22  hk
 * Fehler in make_pure_array behoben.
 *
 * Revision 1.13  1993/10/29  15:16:57  sma
 * Methode, den Array-Typ (inkorrekterweise) nochmals im TAG-Feld der
 * Arraygröße zu speichern entfernt. Aus diesem Grund
 * (set_)row_major_aref_internal geändert.
 *
 * Revision 1.12  1993/09/13  11:52:05  sma
 * Fehler in Längenangaben von Arrays, Vectoren und Instanzen beseitigt
 * durch Einführen des SET_AR_SIZE-Makros.
 *
 * Revision 1.11  1993/08/26  15:20:57  hk
 * Typ von set_array_header von (CL_FORM *) nach void.
 * Unbenutzte Variable entfernt.
 *
 * Revision 1.10  1993/08/20  10:13:41  hk
 * array_element_type_internal prüft nicht mehr auf array Typ.
 *
 * Revision 1.9  1993/07/08  13:12:05  sma
 * OFFSET-Marko eingeführt.
 *
 * Revision 1.8  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.7  1993/05/08  18:19:20  hk
 * Argumentreihenfolge von set-row-major-aref-internal, set-svref-internal
 * und set-fill-pointer-internal geaendert.
 *
 * Revision 1.6  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.5  1993/02/17  15:25:58  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.4  1993/01/05  10:25:12  hk
 * Neue Funktion shrink-vector
 *
 * Revision 1.3  1992/11/16  11:41:10  hk
 * Typ von set_row_major_aref_internal von (void *) nach void, scheint ein
 * Schreibfehler gewesen zu sein.
 *
 * Revision 1.2  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

#define AET_T           0
#define AET_FIXNUM      1
#define AET_FLOAT       2
#define AET_STRING_CHAR 3

#define GET_AET(ar_type) ((ar_type) % 4)

/* Vorwaertsdeklarationen
   ---------------------- */
void Farray_total_size();

/* Laufzeitfehlermeldungen
   ----------------------- */
char No_array[]    = "~a is not an array";
char No_fill_ptr[] = "~a is not a vector with fill-pointer";
char No_vector[]   = "~a is not a vector";

/*------------------------------------------------------------------------------
 * Array Creation
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Der Zeiger auf den Beginn des Array-Headers wird umgebogen
 * auf den entsprechenden Eintrag vom Array-Header
 *----------------------------------------------------------------------------*/
void set_array_header(array_header, header_size, rank)
CL_FORM **array_header;
int header_size, rank;
{
   switch(header_size)
   {
   case 1:                      /* (SIMPLE-VECTOR T) */
      break;
   case 2:                      /* (OR SIMPLE-VECTOR */
                                /* SIMPLE-STRING */
                                /* (SIMPLE-ARRAY T)) */
      if(rank != 1)
        (*array_header) ++;
      break;
   case 3:                      /* SIMPLE-ARRAY */
      (*array_header) ++;
      break;
   default:                     /* (OR ARRAY VECTOR STRING) */
      (*array_header) += 2;
   }
}

/*------------------------------------------------------------------------------
 * Alloziert einen Array-Header und ein Array.
 * Im Array-Header werden ein Zeiger auf das Array, die Anzahl
 * der Dimensionen und die Dimensionen selber eingetragen.
 * Rueckgabewert: Zeiger auf den Array-Header
 *----------------------------------------------------------------------------*/
CL_FORM *make_pure_array(base, header_size, rank, dimensions, element_type,
                          array_size)
CL_FORM *base, *dimensions;
int header_size, rank;
long element_type, array_size;
{
   CL_FORM *array_header;
   long *dims, i;

   switch(element_type)
   {
   case AET_T:
   {
      CL_FORM *pure_array;

      array_header = form_alloc(STACK(base, 0), header_size + array_size);
      pure_array = array_header + header_size;
      set_array_header(&array_header, header_size, rank);
      for(i = 0; i < array_size; i++)
      {
         LOAD_NIL(OFFSET(pure_array, i));
      }

      /* Wenn es kein (SIMPLE-VECTOR T) oder (SIMPLE-ARRAY T) ist,
       * Zeiger auf das eigentliche Array setzen.
       */
      if(header_size > 2 && array_size > 0)
      {
          LOAD_FORM_PTR(pure_array, AR_BASE(array_header));
      }
      break;
   }
   case AET_FIXNUM:
   {
      long *pure_array;

      array_header = form_alloc(STACK(base, 0), (long)header_size);
      set_array_header(&array_header, header_size, rank);
      pure_array = fixnum_alloc(STACK(base, 0), array_size);
      for(i = 0; i < array_size; i++)
      {
         pure_array[i] = 0;
      }
      LOAD_FIXNUM_PTR(pure_array, AR_BASE(array_header));
      break;
   }
   case AET_FLOAT:
   {
      double *pure_array;

      array_header = form_alloc(STACK(base, 0), (long)header_size);
      set_array_header(&array_header, header_size, rank);
      pure_array = float_alloc(STACK(base, 0), array_size);
      for(i = 0; i < array_size; i++)
      {
         pure_array[i] = 0;
      }
      LOAD_FLOAT_PTR(pure_array, AR_BASE(array_header));
      break;
   }
   case AET_STRING_CHAR:
   {
      char *pure_array;

      array_header = form_alloc(STACK(base, 0), (long)header_size);
      set_array_header(&array_header, header_size, rank);
      pure_array = char_alloc(STACK(base, 0), array_size);
      for(i = 0; i < array_size; i++)
      {
         pure_array[i] = ' ';
      }
      LOAD_CHAR_PTR(pure_array, AR_BASE(array_header));
   }
   } /*switch*/
   if(rank != 1)
   {
      dims = fixnum_alloc(STACK(base, 0), (long)rank + 1);
      AR_DIMS(array_header) = dims;
      AR_RANK(array_header) = rank;
      dims++;                   /* Die Laengenangabe ueberspringen */
      for(i = 0; i <= rank; i++)
      {
         dims[i] = GET_FIXNUM(dimensions + i);
      }
   }
   return(array_header);
}

/*------------------------------------------------------------------------------
 * make-vector-internal element-type size adjustable
 *                      fill-pointer displaced-to displaced-index-offset
 *----------------------------------------------------------------------------*/

#define Element_type           STACK(base, 0) /* codiert:  z.B. T => 0 */
#define Size                   STACK(base, 1)
#define Adjustable             STACK(base, 2)
#define Fill_pointer           STACK(base, 3)
#define Displaced_to           STACK(base, 4)
#define Displaced_index_offset STACK(base, 5)

void make_vector_internal(base)
CL_FORM *base;
{
   /* Ein Vektor, der nicht 'adjustable' ist, keinen 'fill-pointer' besitzt und
      nicht 'displaced' ist, wird als einfacher Vektor implementiert.
      */
   BOOL simple = CL_NILP(Adjustable) && CL_NILP(Fill_pointer)
      && CL_NILP(Displaced_to);
   CL_FORM *vector;
   TAG vector_type;
   long vector_elem_type = GET_FIXNUM(Element_type);
   long vector_size      = GET_FIXNUM(Size);

   if(simple)
   {
      /* Es wird ein Array mit folgender Charakteristik erzeugt:
         Wenn es sich um (SIMPLE-VECTOR T) handelt, hat der Array-Header die
         Groesse 1, sonst 2.
         Die Anzahl der Dimensionen ist 1 (=> keine Liste der Dimensionen).
         Die Elemente des Arrays sind vom Typ 'vector_elem_type'.
         Das Array hat insgesamt 'vector_size' Elemente.
         */
      vector = make_pure_array(STACK(base, 6),
                               (vector_elem_type == AET_T ? 1 : 2),
                                1, (CL_FORM *)NULL,
                                vector_elem_type,
                                vector_size);
      vector_type = CL_SMVEC + vector_elem_type;
      SET_AR_SIZE(vector_size, vector);
   }
   else
   {
      if(! CL_NILP(Displaced_to)) /* Ist der Vektor 'displaced' ? */
      {
         vector = form_alloc(STACK(base, 6), 4L) + 2;
         COPY(Displaced_index_offset, DISPLACED_INDEX_OFFSET(vector));
         COPY(Displaced_to,           DISPLACED_TO(vector));
      }
      else
      {
         vector = make_pure_array(STACK(base, 6), 4, 1, (CL_FORM *)NULL,
                                   vector_elem_type, vector_size);
         MAKE_NOT_DISPLACED(vector);
      }

      if(! CL_NILP(Fill_pointer)) /* Hat der Vektor einen Fill-pointer ? */
      {
         MAKE_FILL_PTR(vector);
         SET_FILL_PTR(GET_FIXNUM(Fill_pointer), vector);
         SET_AR_SIZE_WHEN_FP(vector_size, vector);
      }
      else
      {
         LOAD_NIL(FILL_PTR(vector));
         SET_AR_SIZE(vector_size, vector);
      }
      vector_type = CL_VEC + vector_elem_type;
   }
   LOAD_VECTOR(vector, vector_type, STACK(base, 0));
}

#undef Element_type
#undef Size
#undef Adjustable
#undef Fill_pointer
#undef Displaced_to
#undef Displaced_index_offset

/*------------------------------------------------------------------------------
 * make-array-internal element-type size adjustable
 *                     displaced-to displaced-index-offset rank dimensions
 *----------------------------------------------------------------------------*/

#define Element_type           STACK(base, 0)
#define Size                   STACK(base, 1)
#define Adjustable             STACK(base, 2)
#define Displaced_to           STACK(base, 3)
#define Displaced_index_offset STACK(base, 4)
#define Rank                   STACK(base, 5)
#define Dimensions             STACK(base, 6)

void make_array_internal(base, nargs)
CL_FORM *base;
int nargs;
{
   BOOL simple = CL_NILP(Adjustable) && CL_NILP(Displaced_to);
   CL_FORM *array;
   TAG array_type;
   long array_elem_type = GET_FIXNUM(Element_type);
   long array_size      = GET_FIXNUM(Size);
   long array_rank      = GET_FIXNUM(Rank);

   if(simple)
   {
      array_type = CL_SMAR + array_elem_type;
      array = make_pure_array(STACK(base, nargs),
                              (array_elem_type == AET_T ? 2 : 3),
                              (int)array_rank,
                               Dimensions,
                               array_elem_type,
                               array_size);
   }
   else
   {
      array_type = CL_AR + array_elem_type;
      if(! CL_NILP(Displaced_to))
      {
         array = make_pure_array(STACK(base, nargs), 4,
                                  (int)array_rank, Dimensions,
                                  array_elem_type, 0L);
         COPY(Displaced_index_offset, DISPLACED_INDEX_OFFSET(array));
         COPY(Displaced_to,           DISPLACED_TO(array));
      }
      else
      {
         array = make_pure_array(STACK(base, nargs), 4,
                                 (int)array_rank, Dimensions,
                                  array_elem_type, array_size);
         MAKE_NOT_DISPLACED(array);
      }
   }
   SET_AR_SIZE(array_size, array);
   LOAD_ARRAY(array, array_type, STACK(base, 0));
}

#undef Element_type
#undef Size
#undef Adjustable
#undef Displaced_to
#undef Displaced_index_offset
#undef Rank
#undef Dimensions

/*------------------------------------------------------------------------------
 * 17.2. Array Access
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * row_major_aref_internal  array row-major-index
 *------------------------------------------------------------------------------
 * Referenziert aus einem Array den Wert an der Position
 * <row_major_index> in row-major-order.
 *----------------------------------------------------------------------------*/
void row_major_aref_internal(base)
CL_FORM *base;
{
   CL_FORM *array           = GET_FORM  (STACK(base, 0));
   long     row_major_index = GET_FIXNUM(STACK(base, 1));

   switch(TYPE_OF(STACK(base, 0))) {
   case CL_SMVEC_T:
   case CL_SMAR_T :
      COPY(OFFSET(AR_BASE(array),row_major_index), STACK(base, 0));
      break;
   case CL_SMVEC_FIXNUM:
   case CL_SMAR_FIXNUM:
      LOAD_FIXNUM(*(GET_FIXNUM_PTR(AR_BASE(array)) + row_major_index),
                  STACK(base, 0));
      break;
   case CL_SMVEC_FLOAT:
   case CL_SMAR_FLOAT:
      LOAD_FLOAT(GET_FLOAT_PTR(AR_BASE(array)) + row_major_index,
                 STACK(base, 0));
      break;
   case CL_SMSTR:
   case CL_SMAR_CHAR:
      LOAD_CHAR(GET_CHAR_PTR(AR_BASE(array))[row_major_index],
                STACK(base, 0));
      break;
   default:
      if (DISPLACED_P(array))
      {
         COPY(DISPLACED_TO(array), STACK(base, 0));
         row_major_index += GET_FIXNUM(DISPLACED_INDEX_OFFSET(array));
         LOAD_FIXNUM(row_major_index, STACK(base, 1));
         row_major_aref_internal(STACK(base, 0));
         return;
      }
      switch(TYPE_OF(STACK(base, 0)))
      {
      case CL_VEC_T:
      case CL_AR_T:
         COPY(OFFSET(GET_FORM(AR_BASE(array)),row_major_index), STACK(base, 0));
         break;
      case CL_VEC_FIXNUM:
      case CL_AR_FIXNUM:
         LOAD_FIXNUM(*(GET_FIXNUM_PTR(AR_BASE(array)) + row_major_index),
                     STACK(base, 0));
         break;
      case CL_VEC_FLOAT:
      case CL_AR_FLOAT:
         LOAD_FLOAT(GET_FLOAT_PTR(AR_BASE(array)) + row_major_index,
                    STACK(base, 0));
         break;
      case CL_STRING:
      case CL_AR_CHAR:
         LOAD_CHAR(GET_CHAR_PTR(AR_BASE(array))[row_major_index],
                   STACK(base, 0));
         break;
      default:
         Labort("unknown array type");
      }
   }
}

/*------------------------------------------------------------------------------
 * set-row-major-aref-internal newvalue array index
 *------------------------------------------------------------------------------
 * Schreibt in ein Array einen Wert an die Position
 * 'index' bzgl. Row-Major-Order.
 *----------------------------------------------------------------------------*/
void set_row_major_aref_internal(base)
CL_FORM *base;
{
   CL_FORM *array = GET_FORM(STACK(base, 1));
   long index = GET_FIXNUM(STACK(base, 2));

   switch(TYPE_OF(STACK(base, 1)))
   {
   case CL_SMVEC_T:
   case CL_SMAR_T :
      COPY(STACK(base, 0), OFFSET(AR_BASE(array), index));
      break;
   case CL_SMVEC_FIXNUM:
   case CL_SMAR_FIXNUM:
      *(FIXNUM_AR(array) + index) = GET_FIXNUM(STACK(base, 0));
      break;
   case CL_SMVEC_FLOAT:
   case CL_SMAR_FLOAT:
      *(FLOAT_AR(array)  + index) = GET_FLOAT(STACK(base, 0));
      break;
   case CL_SMSTR:
   case CL_SMAR_CHAR:
      *(CHAR_AR(array)   + index) = GET_CHAR(STACK(base, 0));
      break;
   default:
      if (DISPLACED_P(array))
      {
         COPY(DISPLACED_TO(array), STACK(base, 1));
         index += GET_FIXNUM(DISPLACED_INDEX_OFFSET(array));
         LOAD_FIXNUM(index, STACK(base, 2));
         set_row_major_aref_internal(STACK(base, 0));
         return;
      }
      switch(TYPE_OF(STACK(base, 1)))
      {
      case CL_VEC_T:
      case CL_AR_T:
         COPY(STACK(base, 0), OFFSET(FORM_AR(array), index));
         break;
      case CL_VEC_FIXNUM:
      case CL_AR_FIXNUM:
         *(FIXNUM_AR(array) + index) = GET_FIXNUM(STACK(base, 0));
         break;
      case CL_VEC_FLOAT:
      case CL_AR_FLOAT:
         *(FLOAT_AR(array)  + index) = GET_FLOAT(STACK(base, 0));
         break;
      case CL_STRING:
      case CL_AR_CHAR:
         *(CHAR_AR(array)   + index) = GET_CHAR(STACK(base, 0));
         break;
      default:
         Labort("unknown array type");
      }
   }
}

/*------------------------------------------------------------------------------
 * svref-internal simple-vector index
 *----------------------------------------------------------------------------*/
void svref_internal(base)
CL_FORM *base;
{
   CL_FORM *simple_vector = GET_FORM(STACK(base, 0));
   long      index        = GET_FIXNUM(STACK(base, 1));

   COPY(OFFSET(AR_BASE(simple_vector), index), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * set-svref-internal newvalue simple-vector index
 *----------------------------------------------------------------------------*/
void set_svref_internal(base)
CL_FORM *base;
{
   CL_FORM *simple_vector = GET_FORM (STACK(base, 1));
   long index = GET_FIXNUM(STACK(base, 2));

   COPY(STACK(base, 0), OFFSET(AR_BASE(simple_vector), index));
}

/*------------------------------------------------------------------------------
 * 17.3. Array Information
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * array-element-type-internal array
 *----------------------------------------------------------------------------*/
void array_element_type_internal(base)
CL_FORM *base;
{
   TAG array_type = TYPE_OF(STACK(base, 0));

   LOAD_FIXNUM(GET_AET(array_type), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * array-rank array
 *----------------------------------------------------------------------------*/
void Farray_rank(base)
CL_FORM *base;
{
   TAG array_type = TYPE_OF(STACK(base, 0));

   if(VECTORP(array_type))
   {
      LOAD_FIXNUM(1, STACK(base, 0));
   }
   else if(ARRAYP(array_type))
   {
      CL_FORM *array = GET_FORM(STACK(base, 0));
      LOAD_FIXNUM(AR_RANK(array), STACK(base, 0));
   }
   else
      Lerror(STACK(base, 0), No_array);
}

/*------------------------------------------------------------------------------
 * array-dimension-internal array axis-number
 *----------------------------------------------------------------------------*/
void array_dimension_internal(base)
CL_FORM *base;
{
   TAG array_type = TYPE_OF(STACK(base, 0));

   if(VECTORP(array_type))
   {
      Farray_total_size(STACK(base, 0));
   }
   else if(ARRAYP(array_type))
   {
      CL_FORM *array       = GET_FORM  (STACK(base, 0));
      long     axis_number = GET_FIXNUM(STACK(base, 1));

      LOAD_FIXNUM(AR_DIM(array, axis_number), STACK(base, 0));
   }
   else
      Lerror(STACK(base, 0), No_array);
}

/*------------------------------------------------------------------------------
 * array-total-size array
 *----------------------------------------------------------------------------*/
void Farray_total_size(base)
CL_FORM *base;
{
   TAG      array_type = TYPE_OF (STACK(base, 0));
   CL_FORM *array      = GET_FORM(STACK(base, 0));

   if(VECTOR_NS_P(array_type))
   {
      if(HAS_FILL_PTR(array))
         LOAD_FIXNUM(AR_SIZE_WHEN_FP(array), STACK(base, 0));
      else
         LOAD_FIXNUM(AR_SIZE(array), STACK(base, 0));
   }
   else if(ARRAYP(array_type))
   {
      LOAD_FIXNUM(AR_SIZE(array), STACK(base, 0));
   }
   else
      Lerror(STACK(base, 0), No_array);
}
/*------------------------------------------------------------------------------
 * shrink-vector array new-size
 *----------------------------------------------------------------------------*/
void shrink_vector(base)
CL_FORM *base;
{
   CL_FORM *array = GET_FORM(STACK(base, 0));
   int new_size = GET_FIXNUM(STACK(base, 1));
   
   if(!ARRAYP(TYPE_OF(STACK(base, 0))))
      Lerror(STACK(base, 0), No_array);
   if(!CL_FIXNUMP(STACK(base, 1)) || new_size < 0)
      Lerror(STACK(base, 1), "~a is not a positive fixnum");
   if(new_size > AR_SIZE(array))
      Lerror(STACK(base, 1), "new-size ~a is bigger than old-size");
   
   SET_AR_SIZE(new_size, array);
}

/*------------------------------------------------------------------------------
 * adjustable-array-p array
 *----------------------------------------------------------------------------*/
void Fadjustable_array_p(base)
CL_FORM *base;
{
   TAG array_type = TYPE_OF (STACK(base, 0));

   if(array_type == CL_SMVEC_T || array_type == CL_SMAR_T)
   {
      LOAD_NIL(STACK(base, 0));
   }
   else if(ARRAYP(array_type))
   {
      LOAD_T(STACK(base, 0));
   }
   else
      Lerror(STACK(base, 0), No_array);
}

/*------------------------------------------------------------------------------
 * 17.4. Functions on Arrays of Bits     (Nicht implementiert)
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * 17.5. Fill Pointers
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * array-has-fill-pointer-p array
 *----------------------------------------------------------------------------*/
void Farray_has_fill_pointer_p(base)
CL_FORM *base;
{
   TAG array_type = TYPE_OF(STACK(base, 0));

   if(! ARRAYP(array_type))
      Lerror(STACK(base, 0), No_array);
   if(VECTOR_NS_P(array_type) && HAS_FILL_PTR(GET_FORM(STACK(base, 0))))
      LOAD_T(STACK(base, 0));
   else
      LOAD_NIL(STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * fill-pointer vector
 *----------------------------------------------------------------------------*/
void Ffill_pointer(base)
CL_FORM *base;
{
   TAG array_type = TYPE_OF(STACK(base, 0));

   if(! VECTORP(array_type))
      Lerror(STACK(base, 0), No_array);
   if(! SMVECP(array_type))
   {
      CL_FORM *vector = GET_FORM(STACK(base, 0));
      if(! HAS_FILL_PTR(vector))
         Lerror(STACK(base, 0), No_fill_ptr);
      /* Der FILL-POINTER ist dort eingetragen, wo bei einem SIMPLE-VECTOR die
         Laenge eingetragen ist. Dies erleichtert die Programmierung der
         Funktionen, die den FILL-POINTER beachten muessen.
         */
      LOAD_FIXNUM(AR_SIZE(vector), STACK(base, 0));
   }
   else
      Lerror(STACK(base, 0), No_fill_ptr);
}

/*------------------------------------------------------------------------------
 * set-fill-pointer-internal new-fp vector
 *----------------------------------------------------------------------------*/
void set_fill_pointer_internal(base)
CL_FORM *base;
{
   CL_FORM *vector = GET_FORM(STACK(base, 1));

   SET_FILL_PTR(GET_FIXNUM(STACK(base, 0)), vector);
}

/*------------------------------------------------------------------------------
 * displace-array array new-array
 *----------------------------------------------------------------------------*/
void displace_array(base)
CL_FORM *base;
{
   TAG array_type = TYPE_OF (STACK(base, 0));
   CL_FORM     *array = GET_FORM(STACK(base, 0));
   CL_FORM *new_array = GET_FORM(STACK(base, 1));

   SET_AR_SIZE(AR_SIZE(new_array), array);
   COPY(AR_BASE(new_array), AR_BASE(array));
   if(VECTOR_NS_P(array_type))
   {
      MAKE_NOT_DISPLACED(array);
      COPY(FILL_PTR(new_array), FILL_PTR(array));
   }
   else if(ARRAYP(array_type))
   {
      MAKE_NOT_DISPLACED(array);
      AR_DIMS(array) = AR_DIMS(new_array);
   }
}
