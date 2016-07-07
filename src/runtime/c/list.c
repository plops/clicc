/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : System-Funktionen: Lists
 *
 * $Revision: 1.9 $
 * $Log: list.c,v $
 * Revision 1.9  1993/09/19  18:13:22  sma
 * raw-list-length ist jetzt eine C-Funktion, und so deutlich schneller.
 * simple-assoc ist jetzt eine C-Funktion für (assoc item alist :test #'eq)
 * und deutlich schneller als die (allgemeine) Lisp-Variante. Wenn assoc
 * nur auf Symbolen arbeitet und so ein eq-Vergleich ausreicht, bitte
 * diesen explizit angeben. Dann wird diese Funktion aufgerufen.
 *
 * Revision 1.8  1993/09/06  16:39:28  sma
 * Laufzeitverhalten optimiert.
 *
 * Revision 1.7  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.6  1993/04/22  10:29:34  hk
 * fun_decl.h -> sys.h.
 *
 * Revision 1.5  1993/02/17  15:51:57  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.4  1992/11/26  17:05:38  hk
 * Viele Funktionen von hier nach list.lisp.
 *
 * Revision 1.3  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.2  1992/07/21  14:58:07  hk
 * Fset_car --> set_car, Fset_cdr --> set_cdr.
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

/* Laufzeitfehlermeldungen */

char No_list[] = "~a is not a list";


/*------------------------------------------------------------------------------
 * Voraussetzung: <lptr> ist ein Zeiger auf ein Listenelement                   
 * Rueckgabewert: Falls das Listenende erreicht wurde (Rest der Liste ist       
 *                NIL oder ein Atom) wird NULL zurueckgegeben.                  
 *                Sonst ein Zeiger auf das naechste Listenelement.              
 *----------------------------------------------------------------------------*/
CL_FORM *NEXT_CAR(lptr)
CL_FORM *lptr;
{
   lptr = CDR(lptr);            /* Zeiger auf Zelle für CDR */
   if (CL_CONSP(lptr))
      return GET_CAR(lptr);
   else
      return (CL_FORM *)NULL;
}

/*#define NEXT_CAR(lptr) (CL_CONSP(CDR(lptr)) ? GET_CAR(CDR(lptr)) : NULL)*/

/*------------------------------------------------------------------------------
 * Ermittelt die Länge einer Liste.                                            
 * Voraussetzung: <lptr> Zeiger -> CONS-Form                                    
 * Rueckgabewert: Laenge der Liste                                              
 *----------------------------------------------------------------------------*/
long list_length(lptr)
CL_FORM *lptr;
{
   long list_len = 0;

   for (lptr = GET_CAR(lptr); lptr; lptr = NEXT_CAR(lptr))
      list_len++;
   return list_len;
}

/*------------------------------------------------------------------------------
 * list &rest args                                                              
 *----------------------------------------------------------------------------*/
void Flist(base, nargs)
CL_FORM *base;
int nargs;
{
   if(nargs == 0)               /* (list) = () */
   {
      LOAD_NIL(STACK(base, 0));
   }
   else
   {
      CL_FORM *lptr, *lp;
      int i;

      lptr = form_alloc(STACK(base, nargs), nargs * 2);
      nargs--;
      for(lp = lptr, i = 0; i < nargs; lp += 2, i++)
      {
         COPY(STACK(base, i),CAR(lp));
         LOAD_CONS(lp + 2, CDR(lp));
      }
      COPY(STACK(base, i), CAR(lp));
      LOAD_NIL(CDR(lp));
      LOAD_CONS(lptr, STACK(base, 0));
   }
}

/*------------------------------------------------------------------------------
 * list* arg &rest others                                                       
 *----------------------------------------------------------------------------*/
void FlistX(base, nargs)
CL_FORM *base;
int nargs;
{
   CL_FORM *lptr, *lp;
   int i;

   if(nargs == 1)              /* (list* x) = x */
      return;

   nargs--;
   lptr = form_alloc(STACK(base, nargs + 1),  nargs * 2);
   for(lp = lptr, i = 0; i < nargs; lp += 2, i++)
   {
      COPY(STACK(base, i), CAR(lp));
      LOAD_CONS(lp + 2, CDR(lp));
   }
   COPY(STACK(base, i), CDR(lp - 2));
   LOAD_CONS(lptr, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * append &rest lists                                                           
 *----------------------------------------------------------------------------*/
void Fappend(base, nargs)
CL_FORM *base;
int nargs;
{
   int i, list_len = 0;
   CL_FORM *lptr, *lp1, *lp2;

   switch(nargs)
   {
   case 0:                      /* (APPEND) = NIL */
      LOAD_NIL(STACK(base, 0));
      return;
   case 1:                      /* (APPEND arg) = arg */
      return;
   default:
      /* Gesamtlistenlänge der ersten  N - 1 Argumente bestimmen */
      /*---------------------------------------------------------*/
      nargs--;
      for(i = 0; i < nargs; i++) 
      {
         if (CL_CONSP(STACK(base, i)))
            list_len += list_length(STACK(base, i));
         else if (!CL_NILP(STACK(base, i)))
            Lerror(STACK(base, i), No_list);
      }
      
      /* Liste leer, letztes Argument zurückgeben */
      /*------------------------------------------*/
      if(list_len == 0)
      {
         COPY(STACK(base, nargs), STACK(base, 0));
         return;
      }

      /* Kopieren der ersten N - 1 Listen */
      /*----------------------------------*/
      lptr = form_alloc(STACK(base, nargs + 1), list_len * 2);

      for (lp1 = lptr, i = 0; i < nargs; i++)
      {
         if (CL_CONSP(STACK(base, i))) /* nur CONS oder NIL möglich */
         {
            for (lp2 = GET_CAR(STACK(base, i)); lp2; lp2 = NEXT_CAR(lp2))
            {
               COPY(lp2, CAR(lp1));
               LOAD_CONS(lp1 + 2, CDR(lp1));
               lp1 += 2;
            }
         }
      }
      /* Anhängen der Restliste */
      /*------------------------*/
      COPY(STACK(base, i), CDR(lp1 - 2));
      LOAD_CONS(lptr, STACK(base, 0));
   }
}

/*------------------------------------------------------------------------------
 * raw-list-length list
 * Der Parameter "list" muß eine Liste <> der leeren List NIL sein!
 *----------------------------------------------------------------------------*/
void raw_list_length(base)
CL_FORM *base;
{
   CL_FORM *list = STACK(base, 0);
   int i = 0;
   do
   {
      i++;
      list = CDR(GET_CAR(list));
   }
   while (CL_CONSP(list));
   LOAD_FIXNUM(i, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * simple-assoc item a-list
 *  entspricht (assoc item a-list :test #'eq)
 * Der Paramter "a-list" muß eine korrekte a-list sein!
 *----------------------------------------------------------------------------*/
void simple_assoc(base)
CL_FORM *base;
{
   CL_FORM *alist = STACK(base, 1);
   
   while (CL_CONSP(alist))
   {
      CL_FORM *car = GET_CAR(alist);
      if (!CL_CONSP(car))
         Labort("simple-assoc: corrupt a-list");
      
      if (EQ(GET_CAR(car), STACK(base, 0)))
      {
         COPY(car, STACK(base, 0));
         return;
      }
      alist = GET_CDR(alist);
   }
   LOAD_NIL(STACK(base, 0));
}
