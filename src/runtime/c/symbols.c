/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *-----------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : Laufzeitsystem
 *            - SYMBOL-VALUE
 *            - BOUNDP
 *            - SET
 *            - MAKUNBOUND
 *            - SYMBOL-PLIST
 *            - set-symbol-plist
 *            - SYMBOL-NAME
 *            - MAKE-SYMBOL
 *            - SYMBOL-PACKAGE
 *            - set-symbol-package
 *            - set-constant-flag
 *            - setup-symbols-iterator
 *
 * $Revision: 1.12 $
 * $Log: symbols.c,v $
 * Revision 1.12  1993/09/07  17:19:04  sma
 * MAKE_-Makros eingeführt und setup_symbols_iterator verändert.
 *
 * Revision 1.11  1993/07/06  12:32:50  sma
 * OFFSET-Makro eingeführt.
 *
 * Revision 1.10  1993/06/16  14:43:22  hk
 * Copyright Notiz eingefuegt.
 *
 * Revision 1.9  1993/05/12  11:36:38  hk
 * symbol_package_index definiert, wie Fsymbol_package, aber mit anderem
 * Resultattyp.
 *
 * Revision 1.8  1993/05/08  18:16:23  hk
 * set_symbol_plist -> Fset_symbol_plist, Argumentreihenfolge geaendert.
 *
 * Revision 1.7  1993/04/22  10:23:04  hk
 * fun_decl.h -> sys.h, Symbole NIL + T in Ssys definiert,
 * Funktionen fuer den Zugriff auf Komponenten von Symbolen umgestellt,
 * so dass sie die Komponenten von NIL kennen, auch wenn der Wert einer
 * CL_FORM, die NIL darstellt, keinen Zeiger auf das Symbol NIL enthaelt.
 *
 * Revision 1.6  1993/02/17  15:48:19  hk
 * CLICC -> APPLY, Revison Keyword.
 *
 * Revision 1.5  1993/01/08  09:44:13  hk
 * Namen C_ nach c_.
 *
 * Revision 1.4  1992/09/30  17:25:21  hk
 * unbound_value_p neu.
 *
 * Revision 1.3  1992/09/28  17:20:28  hk
 * Lerror -> Labort, neues Lerror mit Lisp-Parameter
 *
 * Revision 1.2  1992/07/21  14:57:11  hk
 * Fset_symbol_plist --> set_symbol_plist.
 *
 * Revision 1.1  1992/03/24  17:03:37  hk
 * Initial revision
 *----------------------------------------------------------------------------*/

#include <c_decl.h>
#include "sys.h"

static char SYM_EXPECTED[] = "~a is not a symbol";
static char TRY_CHANGE_CONST[] = 
   "can not change value of ~a, which is a constant";
static char STRING_EXPECTED[] = "~a is not a string";

/*------------------------------------------------------------------------------
 * Die Symbole NIL Und T
 *----------------------------------------------------------------------------*/
CL_INIT Ssys[] =
{
   MAKE_SYMBOL(3, "NIL",
               MAKE_NIL,        /* Property Liste */ 
               MAKE_NIL,        /* Wert: Wird nicht benutzt */
               MAKE_NIL,        /* Das Package wird vom Lisp Modul gesetzt */
               CONST_SYM),      /* Constant-Flag: Wird nicht benutzt */
   MAKE_SYMBOL(1, "T",
               MAKE_NIL,
               MAKE_T,
               MAKE_NIL,
               CONST_SYM),
   END_SYMDEF                   /* Endmarkierung der Symboltabelle */
};

/*------------------------------------------------------------------------------
 * Konstanten für den Zugriff PLIST, VALUE und PACKAGE von NIL
 *----------------------------------------------------------------------------*/
#define NIL_PLIST            ((CL_FORM *)&Ssys[OFF_SYM_PLIST])
#define LOAD_NIL_PNAME(loc)  LOAD_SMSTR((CL_FORM *)&Ssys[OFF_SYM_PNAME], loc)
#define NIL_PACKAGE          ((CL_FORM *)&Ssys[OFF_SYM_PACKAGE])

/*------------------------------------------------------------------------------
 * symbol-value symbol
 *----------------------------------------------------------------------------*/
void Fsymbol_value(base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      return;
   }
   if (CL_SYMBOLP(STACK(base, 0)))
   {
      COPY(SYM_VALUE(STACK(base, 0)), STACK(base, 0));
   }
   else
   {
      Lerror(STACK(base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * boundp symbol
 * Wenn UNBOUND -> NIL, sonst -> T
 *----------------------------------------------------------------------------*/
void Fboundp(base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      LOAD_T(STACK(base, 0));
   }
   else if (CL_SYMBOLP(STACK(base, 0)))
   {
      RET_BOOL(! CL_UNBOUNDP(SYM_VALUE(STACK(base, 0))));
   }
   else
   {
      Lerror(STACK (base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * Prueft, ob das Argument der spezielle Wert UNBOUND ist.
 * Der kann nur entstehen, wenn auf ein ungebundenes Symbol zugeriffen wird,
 * ohne das Programm abzubrechen.
 * Wird in print benutzt.
 *----------------------------------------------------------------------------*/
void unbound_value_p(base)
CL_FORM *base;
{
   RET_BOOL(CL_UNBOUNDP(STACK(base, 0)));
}

/*------------------------------------------------------------------------------
 * set symbol value
 *----------------------------------------------------------------------------*/
void Fset(base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      Lerror(STACK(base, 0), TRY_CHANGE_CONST);
   }
   else if (CL_SYMBOLP(STACK(base, 0)))
   {
      if(SYM_IS_CONST(STACK(base, 0)))
         Lerror(STACK(base, 0), TRY_CHANGE_CONST);
      COPY(STACK(base, 1), SYM_VALUE(STACK(base, 0)));
   }
   else
   {
      Lerror(STACK(base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * makunbound symbol
 *----------------------------------------------------------------------------*/
void Fmakunbound(base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      Lerror(STACK (base, 0), TRY_CHANGE_CONST);
   }
   else if (CL_SYMBOLP(STACK(base, 0)))
   {
      if(SYM_IS_CONST(STACK(base, 0)))
         Lerror(STACK(base, 0), TRY_CHANGE_CONST);
      LOAD_UNBOUND(SYM_VALUE(STACK(base, 0)));
   }
   else
   {
      Lerror(STACK (base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * symbol-plist symbol
 *----------------------------------------------------------------------------*/
void Fsymbol_plist (base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      COPY(NIL_PLIST, STACK(base, 0));
   }
   else if (CL_SYMBOLP(STACK(base, 0)))
   {
      COPY(SYM_PLIST(STACK(base, 0)), STACK(base, 0));
   }
   else
   {
      Lerror(STACK (base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * (setf symbol-plist) value symbol
 * Resultat: value
 *----------------------------------------------------------------------------*/
void Fset_symbol_plist(base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 1)))
   {
      COPY(STACK(base, 0), NIL_PLIST);
   }
   else if (CL_SYMBOLP(STACK(base, 1)))
   {
      COPY(STACK(base, 0), SYM_PLIST(STACK(base, 1)));
   }
   else
   {
      Lerror(STACK (base, 1), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * symbol-name symbol
 *----------------------------------------------------------------------------*/
void Fsymbol_name(base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      LOAD_NIL_PNAME(STACK(base, 0));
   }
   else if (CL_SYMBOLP(STACK(base, 0)))
   {
      LOAD_SYM_PNAME(STACK(base, 0), STACK(base, 0));
   }
   else
   {
      Lerror(STACK (base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * make-symbol print-name
 *----------------------------------------------------------------------------*/
void Fmake_symbol(base)
CL_FORM *base;
{
   CL_FORM *sym;

   if (CL_STRINGP(STACK(base, 0)))
   {
      /* mittels copy-seq eine Kopie vom Typ CL_SMSTR erzeugen */
      /* ----------------------------------------------------- */
      Fcopy_seq(STACK(base, 0));
   }
   else if (! CL_SMSTRP(STACK(base, 0)))
   {
      Lerror(STACK (base, 0), STRING_EXPECTED);
   }
   /* Ausgelagert in obrepX.h - Erstellt neues Symbol */
   SYM_MAKE_SYM(sym);
   LOAD_SYMBOL(sym, STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * symbol-package sym
 *----------------------------------------------------------------------------*/
void Fsymbol_package (base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      COPY(NIL_PACKAGE, STACK(base, 0));
   }
   else if (CL_SYMBOLP(STACK(base, 0)))
   {
      COPY(SYM_PACKAGE(STACK(base, 0)), STACK(base, 0));
   }
   else
   {
      Lerror(STACK (base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * symbol-package-index symbol
 *----------------------------------------------------------------------------*/
void symbol_package_index (base)
CL_FORM *base;
{
   COPY(SYM_PACKAGE(STACK(base, 0)), STACK(base, 0));
}

/*------------------------------------------------------------------------------
 * set-symbol-package value sym
 * wird nur intern verwendet, keine Typueberpruefung notwendig
 *----------------------------------------------------------------------------*/
void set_symbol_package (base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 1)))
   {
      COPY(STACK(base, 0), NIL_PACKAGE);
   }
   else if (CL_SYMBOLP(STACK(base, 1)))
   {
      COPY(STACK(base, 0), SYM_PACKAGE(STACK(base, 1)));
   }
   else
   {
      Lerror(STACK (base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * set-constant-flag sym                                                  
 * wird nur intern verwendet, keine Typueberpruefung notwendig
 *----------------------------------------------------------------------------*/
void set_constant_flag (base)
CL_FORM *base;
{
   if (CL_NILP(STACK(base, 0)))
   {
      return;
   }
   if (CL_SYMBOLP(STACK(base, 0)))
   {
      SYM_SET_CONST(STACK(base, 0));
   }
   else
   {
      Lerror(STACK (base, 0), SYM_EXPECTED);
   }
}

/*------------------------------------------------------------------------------
 * setup-symbols-iterator first-sym package-vector
 * Wendet setup-symbol auf alle zur Uebersetzungszeit definierten
 * Symbole eines Moduls an.
 *----------------------------------------------------------------------------*/
void setup_symbols_iterator (base)
CL_FORM *base;
{
   CL_FORM *sym = GET_SYMBOL(STACK(base, 0));
   
   /* das Array ist mit einem END_SYMDEF-Eintrag abgeschlossen */
   /* -------------------------------------------------------- */
   while(!IS_END_SYMDEF(sym))
   {
      LOAD_SYMBOL(sym, STACK(base, 2));
      COPY(STACK(base, 1), STACK(base, 3));
      setup_symbol(STACK(base, 2));
      sym += SYM_SIZE; 
   }
}
