/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *-----------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : System-Funktionen: Structures
 *
 * $Revision: 1.9 $
 * $Log: structure.c,v $
 * Revision 1.9  1993/10/14  12:56:46  sma
 * new-struct-internal gestrichen, dafür new-struct von lisp nach C
 * verschoben und 2x TYPE_OF durch CL_STRUCTP ersetzt.
 *
 * Revision 1.8  1993/07/06  13:21:02  sma
 * OFFSET-Makro eingeführt.
 *
 * Revision 1.7  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.6  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.5  1993/04/05  10:43:37  hk
 * Resultattypen korrigiert.
 *
 * Revision 1.4  1993/02/17  15:47:36  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.3  1993/01/11  14:09:05  hk
 * structure -> struct
 *
 * Revision 1.2  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *----------------------------------------------------------------------------*/

#include "c_decl.h"
#include "sys.h"

/* Fehlermeldungen
   --------------- */
char No_struct[] = "~a is not a structure";

#define STRUCT_HEADER 2
#define STRUCT_OFFSET 1

/*------------------------------------------------------------------------------
 * structp object
 *----------------------------------------------------------------------------*/
void structp(base)
CL_FORM *base;
{
   RET_BOOL(CL_STRUCTP(STACK(base, 0)));
}

/*------------------------------------------------------------------------------
 * struct-type structure
 *----------------------------------------------------------------------------*/
void struct_type(base)
CL_FORM *base;
{
   CL_FORM *header = GET_FORM(STACK(base, 0));

   if (! CL_STRUCTP(STACK(base, 0)))
      Lerror(STACK(base, 0), No_struct);
   COPY(AR_BASE(header), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * new-struct size type
 * Erzeugt neue Structur der Größe size vom Typ type
 *----------------------------------------------------------------------------*/
void new_struct(base)
CL_FORM *base;
{
	long size = GET_FIXNUM(STACK(base, 0));
   CL_FORM *strct;
   
   strct = form_alloc(STACK(base, 2), size + STRUCT_HEADER);
   SET_AR_SIZE(size + STRUCT_OFFSET, strct);
   COPY(STACK(base, 1), AR_BASE(strct));
   LOAD_STRUCT(strct, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * struct-size struct
 *----------------------------------------------------------------------------*/
void struct_size(base)
CL_FORM *base;
{
   CL_FORM *header = GET_FORM(STACK(base, 0));
   LOAD_FIXNUM(AR_SIZE(header) - STRUCT_OFFSET, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * struct-ref-internal structure index
 *----------------------------------------------------------------------------*/
void struct_ref_internal(base)
CL_FORM *base;
{
   CL_FORM *header = GET_FORM  (STACK(base, 0));
   long     index  = GET_FIXNUM(STACK(base, 1));
   COPY(OFFSET(AR_BASE(header), STRUCT_OFFSET + index), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * set-struct-ref-internal structure index newvalue
 *----------------------------------------------------------------------------*/
void set_struct_ref_internal(base)
CL_FORM *base;
{
   CL_FORM *header = GET_FORM  (STACK(base, 0));
   long     index  = GET_FIXNUM(STACK(base, 1));
   COPY(STACK(base, 2), OFFSET(AR_BASE(header), STRUCT_OFFSET + index));
   COPY(STACK(base, 2), STACK(base, 0));
}
