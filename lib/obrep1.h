/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : obrep1.h - datenrepräsentationsspezifisch
 *
 * $Revision: 1.10 $
 * $Log: obrep1.h,v $
 * Revision 1.10  1993/11/12  13:09:05  sma
 * Neue Konstante BITS_PER_FIXNUM.
 *
 * Revision 1.9  1993/10/29  15:02:31  sma
 * Änderungen für korrektes Funktionieren von obrep1 & obrep2.
 *
 * Revision 1.8  1993/10/14  15:54:04  sma
 * Weitere Makros eingeführt, um obrep1 und obrep2 ohne Änderung von
 * weiterem Code austauschen zu können.
 *
 * Revision 1.7  1993/09/28  16:03:29  pm
 * Fehler korrigiert.
 *
 * Revision 1.6  1993/09/28  14:44:39  pm
 * Pointer-Tags eingetragen
 *
 * Revision 1.5  1993/09/13  11:51:37  sma
 * Fehler in Längenangaben von Arrays, Vectoren und Instanzen beseitigt
 * durch Einführen des SET_AR_SIZE-Makros.
 *
 * Revision 1.4  1993/09/09  15:30:13  uho
 * Einige Makrodefinitionen eingerückt, um die Lesbarkeit zu erhöhen.
 *
 * Revision 1.3  1993/09/06  16:43:57  sma
 * Umgeordnet, besser kommentiert, neue Makros für Konstantendefinitionen.
 * cgconst.lisp erzeugt jetzt nicht mir direkt CL_INIT-Strukturen, sondern
 * MAKE_* Makros, die die eigentliche Repräsentation der Lispdaten
 * abstrahieren.
 *
 * Revision 1.2  1993/08/23  09:59:44  pm
 * Aenderungen fuer C-Pointer
 *
 * Revision 1.1  1993/08/22  14:26:37  sma
 * Initial revision
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * Werte für UNBOUND, NIL und T
 *----------------------------------------------------------------------------*/
#define UNBOUND_VALUE  0
#define NIL_VALUE      0
#define T_VALUE        SYMBOL(Ssys, 1)

/*------------------------------------------------------------------------------
 * Datenstruktur der LISP-Objekte
 *----------------------------------------------------------------------------*/
typedef unsigned short TAG;
typedef struct clx_form CL_FORM;

struct clx_form
{
   TAG tag;
   union
   {
      long i;
      double *fl;
      unsigned long ch;         /* Wichtig, kein 'char' nehmen ! (s.u.) */
      long *i_ptr;
      char *ch_ptr;
      CL_FORM *form;
      CL_FORM *symbol;
      FILE *cfile;
      CLOSURE_FUN *fun;
      DOWN_FUNARG *dfarg;
      GLOBAL_FUNARG *gfarg;
   } val;
};


/*------------------------------------------------------------------------------
 * Datenstruktur und Makros zum Initialisieren von LISP-Objekten.
 *
 * Es muß gelten sizeof(long) == sizeof( Komponenten von CL_FORM ),
 * sonst kann fehlerhafter Code erzeugt werden.
 *          CL_FORM          CL_INIT
 *       long: |----|,    long:   |----|
 *       PTR:  |------|   PTR:  |------|
 *       char: |-|        char:      |-|
 *----------------------------------------------------------------------------*/
typedef struct
{
   TAG tag;
   long val;
}
CL_INIT;
#define CL_INIT2(name) CL_INIT name[] =
   

/*------------------------------------------------------------------------------
 * Konstruktoren für Konstantendefinitionen
 *----------------------------------------------------------------------------*/
#define END_SYMDEF  0           /* Endmarkierung einer Symboltabelle */
#define IS_END_SYMDEF(x)        (TYPE_OF(x) == END_SYMDEF)

#define MAKE_NIL                { CL_NIL, (long)NIL_VALUE }
#define MAKE_T                  { CL_SYMBOL, (long)T_VALUE }
#define MAKE_UNBOUND            { CL_UNBOUND, (long)UNBOUND_VALUE }
#define MAKE_FIXNUM(num)        { CL_FIXNUM, num }
#define MAKE_FLOAT(flptr)       { CL_FLOAT, (long)flptr }
#define MAKE_CHAR(chr)          { CL_CHAR,  (long)chr }

#define MAKE_STRING(len, str)\
   { CL_FIXNUM, len },\
   { RT_CHAR_PTR, (long)str }

#define MAKE_SYMBOL(len, name, plst, val, pkg, cf)\
   { CL_FIXNUM, len },\
   { RT_CHAR_PTR, (long)name },\
     plst, val, pkg, cf

#define MAKE_CLASS(name, cpl, slotnum, slotinfo)\
   { CL_FIXNUM, 5 },\
     MAKE_NIL,\
     MAKE_SYMREF(name),\
     cpl,\
     MAKE_FIXNUM(slotnum),\
     slotinfo

#define MAKE_INSTANCE(sz, class)\
   { CL_FIXNUM, sz },\
     class

#define MAKE_CONS(car, cdr)     car, cdr
#define MAKE_VECTOR(sz)         { CL_FIXNUM, sz }
#define MAKE_ARRAY(rank, dims, sz)\
   { rank, (long)dims },\
   { CL_FIXNUM, sz }

#define MAKE_STRREF(str)        { CL_SMSTR, (long)str }
#define MAKE_SYMREF(sym)        { CL_SYMBOL, (long)sym }
#define MAKE_CLASSREF(class)    { CL_INSTANCE, (long)class }
#define MAKE_LIST(list)         { CL_CONS, (long)list }
#define MAKE_VECREF(vec)        { CL_SMVEC_T, (long)vec }
#define MAKE_ARREF(ar)          { CL_SMAR_T, (long)ar }
#define MAKE_STRUCTREF(str)     { CL_STRUCT, (long)str }

#define MAKE_GLOBFUN(fun)       { CL_GLOBFUN, (long)fun }

#define CONST_SYM   MAKE_T      /* Symbol ist konstant */
#define NORMAL_SYM  MAKE_NIL    /* Symbol ist veränderbar */


/*-------------------------------------------------------------------------
 * Definitionen der Datentypen von LISP-Objekten
 *
 * Bei den Array-Typen ist folgendes zu beachten:
 *    - Der erste Typ sollte bei einer Zahl 16^n beginnen
 *    - Es gilt immer die Reihenfolge T, FIXNUM, FLOAT, CHAR
 *-------------------------------------------------------------------------*/
#define CL_UNBOUND       0
#define CL_FIXNUM        1      /* number */
#define CL_FLOAT         2      /* number */
#define CL_CHAR          3

#define RT_FORM_PTR      4      /* Zeiger auf FORM-Array */
#define RT_FIXNUM_PTR    5      /* Zeiger auf FIXNUM-Array */
#define RT_FLOAT_PTR     6      /* Zeiger auf FLOAT-Array */
#define RT_CHAR_PTR      7      /* Zeiger auf CHAR-Array (String) */

#define CL_SYMBOL       13      /* symbol */
#define CL_NIL          14      /* symbol, list,          sequence */
#define CL_CONS         15      /*         list,          sequence */

#define CL_VEC          16
#define CL_VEC_T        16      /*         vector, array, sequence */
#define CL_VEC_FIXNUM   17      /*         vector, array, sequence */
#define CL_VEC_FLOAT    18      /*         vector, array, sequence */
#define CL_STRING       19      /* string, vector, array, sequence */

#define CL_SMVEC        20     
#define CL_SMVEC_T      20      /*         vector, array, sequence */
#define CL_SMVEC_FIXNUM 21      /*         vector, array, sequence */
#define CL_SMVEC_FLOAT  22      /*         vector, array, sequence */
#define CL_SMSTR        23      /* string, vector, array, sequence */

#define CL_SMAR         24
#define CL_SMAR_T       24      /*                 array, sequence */
#define CL_SMAR_FIXNUM  25      /*                 array, sequence */
#define CL_SMAR_FLOAT   26      /*                 array, sequence */
#define CL_SMAR_CHAR    27      /*                 array, sequence */

#define CL_AR           28
#define CL_AR_T         28      /*                 array, sequence */
#define CL_AR_FIXNUM    29      /*                 array, sequence */
#define CL_AR_FLOAT     30      /*                 array, sequence */
#define CL_AR_CHAR      31      /*                 array, sequence */

#define CL_CLOSURE      32      /* function */
#define CL_DOWNFUN      33      /* function */
#define CL_GLOBFUN      34      /* function */

#define CL_CODE         37
#define CL_IND          38
#define CL_STRUCT       39      /* Structure */
#define CL_CFILE        41
#define CL_UNIQUE_TAG   42      /* fuer automatisch generierte CATCH-Tags */

#define CL_INSTANCE     50      /* CLOS Instanz */

/* Die nächsten Konstanten müssen in dieser Reihenfolge stehen!! */
#define CL_C_CHAR           60  /* char, signed-char */
#define CL_C_SHORT          61  /* short, short-int, signed-short,
                                   signed-short-int */ 
#define CL_C_INT            62  /* int, signed, signed-int */
#define CL_C_LONG           63  /* long, long-int, signed-long,
                                   signed-long-int */
#define CL_C_UNSIGNED_CHAR  64 
#define CL_C_UNSIGNED_SHORT 65
#define CL_C_UNSIGNED_INT   66  /* unsigned-int, unsigned */
#define CL_C_UNSIGNED_LONG  67

#define CL_C_FLOAT          68
#define CL_C_DOUBLE         69
#define CL_C_LONG_DOUBLE    70

#define CL_C_STRUCT         71
#define CL_C_UNION          72
#define CL_C_ARRAY          73

#define CL_C_CHAR_PTR            74
#define CL_C_SHORT_PTR           75
#define CL_C_INT_PTR             76
#define CL_C_LONG_PTR            77

#define CL_C_UNSIGNED_CHAR_PTR   78
#define CL_C_UNSIGNED_SHORT_PTR  79
#define CL_C_UNSIGNED_INT_PTR    80
#define CL_C_UNSIGNED_LONG_PTR   81

#define CL_C_STRUCT_PTR          82
/* Bis hier ist die Reihenfolge wichtig */


/*------------------------------------------------------------------------------
 * Bereiche für Sequencen, Vektoren und Arrays.
 *----------------------------------------------------------------------------*/
#define T_SEQ_LO   CL_NIL
#define T_SEQ_HI   CL_AR_CHAR
#define T_VEC_LO   CL_VEC_T
#define T_VEC_HI   CL_SMSTR
#define T_ARR_LO   CL_VEC_T
#define T_ARR_HI   CL_AR_CHAR
#define T_SMAR_LO  CL_SMVEC_T
#define T_SMAR_HI  CL_SMAR_CHAR


/*------------------------------------------------------------------------------
 * Makros für den Zugriff auf die Komponenten eines LISP-Objektes
 *----------------------------------------------------------------------------*/
/* Tags */
#define TYPE_OF(loc)            ((loc)->tag)
#define SET_TAG(loc, value)     (TYPE_OF(loc) = (value))

/* Grundtypen */
#define GET_FORM(loc)           ((loc)->val.form)

#define GET_FIXNUM(loc)         ((loc)->val.i)
#define GET_FLOAT(loc)          (*((loc)->val.fl))
#define GET_CHAR(loc)           (char)((loc)->val.ch)

#define GET_FIXNUM_PTR(loc)     ((loc)->val.i_ptr)
#define GET_FLOAT_PTR(loc)      ((loc)->val.fl)
#define GET_CHAR_PTR(loc)       ((loc)->val.ch_ptr)

/* Listen */
#define CAR(lptr)               (lptr)
#define CDR(lptr)               ((lptr) + 1)
#define GET_CAR(loc)            ((loc)->val.form)
#define GET_CDR(loc)            ((loc)->val.form + 1)

/* Symbol */
#define GET_SYMBOL(loc)         ((loc)->val.symbol)

/* Funktionen */
#define GET_FUN(loc)            ((loc)->val.fun)
#define GET_DFARG(loc)          ((loc)->val.dfarg)
#define GET_GFARG(loc)          ((loc)->val.gfarg)

#define INDIRECT(loc)           ((loc)->val.form)
#define GET_CFILE(loc)          ((loc)->val.cfile)

/* Foreign Fuctions */
#define GET_C_CHAR(loc)               (char)((loc)->val.ch)
#define GET_C_SHORT(loc)              (short)((loc)->val.i)
#define GET_C_INT(loc)                (int)((loc)->val.i)
#define GET_C_LONG(loc)               (long)((loc)->val.i)
#define GET_C_UNSIGNED_CHAR(loc)      (unsigned char)((loc)->val.ch)
#define GET_C_UNSIGNED_SHORT(loc)     (unsigned short)((loc)->val.i)
#define GET_C_UNSIGNED_INT(loc)       (unsigned int)((loc)->val.i)
#define GET_C_UNSIGNED_LONG(loc)      (unsigned long)((loc)->val.i)

#define GET_C_CHAR_PTR(loc)           (char *)((loc)->val.ch_ptr)
#define GET_C_SHORT_PTR(loc)          (short *)((loc)->val.ch_ptr)
#define GET_C_INT_PTR(loc)            (int *)((loc)->val.ch_ptr)
#define GET_C_LONG_PTR(loc)           (long *)((loc)->val.ch_ptr)
#define GET_C_UNSIGNED_CHAR_PTR(loc)  (unsigned char *)((loc)->val.ch_ptr)
#define GET_C_UNSIGNED_SHORT_PTR(loc) (unsigned short *)((loc)->val.ch_ptr)
#define GET_C_UNSIGNED_INT_PTR(loc)   (unsigned int *)((loc)->val.ch_ptr)
#define GET_C_UNSIGNED_LONG_PTR(loc)  (unsigned long *)((loc)->val.ch_ptr)

#define GET_C_STRING(loc)             (char *)((loc)->val.ch_ptr)

#define GET_C_STRUCT(loc)             ((loc)->val.ch_ptr)
#define GET_C_STRUCT_PTR(loc)         ((loc)->val.ch_ptr)

#define GET_C_STRUCT_PTR_VALUE(loc)   GET_CHAR_PTR(OFFSET(GET_FORM(loc), 2))



/*------------------------------------------------------------------------------
 * Prädikate zum Testen des Datentyps
 *----------------------------------------------------------------------------*/
#define CL_NILP(loc)       (TYPE_OF (loc) == CL_NIL)
#define CL_TRUEP(loc)      (TYPE_OF (loc) != CL_NIL)
#define CL_UNBOUNDP(loc)   (TYPE_OF (loc) == CL_UNBOUND)

#define CL_FIXNUMP(loc)    (TYPE_OF (loc) == CL_FIXNUM)
#define CL_FLOATP(loc)     (TYPE_OF (loc) == CL_FLOAT)
#define CL_CHARP(loc)      (TYPE_OF (loc) == CL_CHAR)
#define CL_SYMBOLP(loc)    (TYPE_OF (loc) == CL_SYMBOL)
#define CL_CONSP(loc)      (TYPE_OF (loc) == CL_CONS)
#define CL_ATOMP(loc)      (TYPE_OF (loc) != CL_CONS)
#define CL_SMSTRP(loc)     (TYPE_OF (loc) == CL_SMSTR)
#define CL_STRINGP(loc)    (TYPE_OF (loc) == CL_STRING)
#define CL_SMVEC_T_P(loc)  (TYPE_OF (loc) == CL_SMVEC_T)
#define CL_VEC_T_P(loc)    (TYPE_OF (loc) == CL_VEC_T)
#define CL_CLOSUREP(loc)   (TYPE_OF (loc) == CL_CLOSURE)
#define CL_GLOBFUNP(loc)   (TYPE_OF (loc) == CL_GLOBFUN)
#define CL_DOWNFUNP(loc)   (TYPE_OF (loc) == CL_DOWNFUN)
#define CL_INSTANCEP(loc)  (TYPE_OF (loc) == CL_INSTANCE)
#define CL_STRUCTP(loc)    (TYPE_OF (loc) == CL_STRUCT)

#define CL_C_STRUCT_P(loc) (TYPE_OF (loc) == CL_C_STRUCT)
#define CL_C_CHAR_P(loc)           (TYPE_OF (loc) == CL_C_CHAR)
#define CL_C_SHORT_P(loc)          (TYPE_OF (loc) == CL_C_SHORT ||\
                                    TYPE_OF (loc) == CL_C_CHAR)
#define CL_C_INT_P(loc)            (TYPE_OF (loc) == CL_C_INT ||\
                                    TYPE_OF (loc) == CL_C_SHORT ||\
                                    TYPE_OF (loc) == CL_C_CHAR)
#define CL_C_LONG_P(loc)           (TYPE_OF (loc) == CL_C_LONG ||\
                                    TYPE_OF (loc) == CL_C_INT ||\
                                    TYPE_OF (loc) == CL_C_SHORT ||\
                                    TYPE_OF (loc) == CL_C_CHAR)
#define CL_C_UNSIGNED_CHAR_P(loc)  (TYPE_OF (loc) == CL_C_UNSIGNED_CHAR)
#define CL_C_UNSIGNED_SHORT_P(loc) (TYPE_OF (loc) == CL_C_UNSIGNED_SHORT ||\
                                    TYPE_OF (loc) == CL_C_UNSIGNED_CHAR)
#define CL_C_UNSIGNED_INT_P(loc)   (TYPE_OF (loc) == CL_C_UNSIGNED_INT ||\
                                    TYPE_OF (loc) == CL_C_UNSIGNED_SHORT ||\
                                    TYPE_OF (loc) == CL_C_UNSIGNED_CHAR)
#define CL_C_UNSIGNED_LONG_P(loc)  (TYPE_OF (loc) == CL_C_UNSIGNED_LONG ||\
                                    TYPE_OF (loc) == CL_C_UNSIGNED_INT ||\
                                    TYPE_OF (loc) == CL_C_UNSIGNED_SHORT ||\
                                    TYPE_OF (loc) == CL_C_UNSIGNED_CHAR)
#define CL_C_CHAR_PTR_P(loc)       (TYPE_OF (loc) == CL_C_CHAR_PTR)
#define CL_C_SHORT_PTR_P(loc)      (TYPE_OF (loc) == CL_C_SHORT_PTR)
#define CL_C_INT_PTR_P(loc)        (TYPE_OF (loc) == CL_C_INT_PTR)
#define CL_C_LONG_PTR_P(loc)       (TYPE_OF (loc) == CL_C_LONG_PTR)
#define CL_C_UNSIGNED_CHAR_PTR_P(loc)  (TYPE_OF (loc) == CL_C_UNSIGNED_CHAR_PTR)
#define CL_C_UNSIGNED_SHORT_PTR_P(loc) (TYPE_OF (loc) == CL_C_UNSIGNED_SHORT_PTR)
#define CL_C_UNSIGNED_INT_PTR_P(loc)   (TYPE_OF (loc) == CL_C_UNSIGNED_INT_PTR)
#define CL_C_UNSIGNED_LONG_PTR_P(loc)  (TYPE_OF (loc) == CL_C_UNSIGNED_LONG_PTR)


#define CL_NUMBERP(obj)       (CL_FIXNUMP(obj) || CL_FLOATP(obj))
#define CL_LISTP(loc)         (CL_CONSP(loc) || CL_NILP(loc))
#define CL_STRING_P(loc)      (CL_SMSTRP(loc) || CL_STRINGP(loc))
#define CL_FUNCTION_P(loc)    (CL_CLOSUREP(loc) || CL_DOWNFUNP(loc) ||\
                               CL_GLOBFUNP(loc))
#define CL_VECTOR_P(loc)      VECTORP(TYPE_OF(loc))
#define CL_ARRAY_P(loc)       ARRAYP(TYPE_OF(loc))
#define CL_SMAR_P(loc)        SMARP(TYPE_OF(loc))

/*------------------------------------------------------------------------------
 * Weitere Typtests
 *----------------------------------------------------------------------------*/
#define VECTORP(ar_type)     (CL_VEC_T <= (ar_type) && (ar_type) <= CL_SMSTR)
#define ARRAYP(ar_type)      (CL_VEC_T <= (ar_type) && (ar_type) <= CL_AR_CHAR)
/* VECTOR AND NOT SIMPLE */
#define VECTOR_NS_P(ar_type) (CL_VEC_T <= (ar_type) && (ar_type) <= CL_STRING)
#define SMVECP(ar_type)      (CL_SMVEC_T <= (ar_type) && (ar_type) <= CL_SMSTR)
#define SMARP(ar_type)       (CL_VEC_T <= (ar_type) && (ar_type)<=CL_SMAR_CHAR)


/*------------------------------------------------------------------------------
 * Makros zum Laden der LISP-Objekte
 *----------------------------------------------------------------------------*/
#define LOAD_NIL(loc)          (SET_TAG(loc, CL_NIL),GET_FORM(loc) = NIL_VALUE)
#define LOAD_T(loc)            (SET_TAG(loc, CL_SYMBOL),GET_FORM(loc) = T_VALUE)
#define LOAD_FIXNUM(num, loc)  (SET_TAG(loc, CL_FIXNUM),GET_FIXNUM(loc) = (num))
#define LOAD_FLOAT(flptr, loc) (SET_TAG(loc, CL_FLOAT),(loc)->val.fl=(flptr))
#define LOAD_CHAR(chr, loc)    (SET_TAG(loc, CL_CHAR),((loc)->val.ch=(chr)))

#define LOAD_FORM_PTR(form, loc)   (SET_TAG (loc, RT_FORM_PTR),\
                                    GET_FORM (loc) = (form))
#define LOAD_FIXNUM_PTR(iptr, loc) (SET_TAG (loc, RT_FIXNUM_PTR),\
                                    GET_FIXNUM_PTR (loc) = (iptr))
#define LOAD_FLOAT_PTR(flptr, loc) (SET_TAG (loc, RT_FLOAT_PTR),\
                                    GET_FLOAT_PTR (loc) = (flptr))
#define LOAD_CHAR_PTR(chptr, loc)  (SET_TAG (loc, RT_CHAR_PTR),\
                                    GET_CHAR_PTR (loc) = (chptr))

#define LOAD_CONS(car, loc)      (SET_TAG(loc, CL_CONS),GET_FORM(loc) = (car))
#define LOAD_SYMBOL(sym, loc)    (SET_TAG(loc, CL_SYMBOL),GET_FORM(loc) = (sym))

#define LOAD_CLASS(class, loc) (SET_TAG(loc, CL_INSTANCE),\
                                GET_FORM(loc) = (class))

#define LOAD_SMSTR(strptr, loc)   (SET_TAG(loc, CL_SMSTR),\
                                   GET_FORM(loc) = strptr)
#define LOAD_STRING(strptr, loc)  (SET_TAG(loc, CL_STRING),\
                                   GET_FORM(loc) = strptr)
#define LOAD_SMVEC_T(strptr, loc) (SET_TAG(loc, CL_SMVEC_T),\
                                   GET_FORM(loc) = strptr)
#define LOAD_SMAR_T(strptr, loc)  (SET_TAG(loc, CL_SMAR_T),\
                                   GET_FORM(loc) = strptr)

#define LOAD_CODE(fun, loc)      (SET_TAG(loc, CL_CODE),GET_FUN(loc) = fun)
#define LOAD_IND(ptr, loc)       (SET_TAG(loc, CL_IND),GET_FORM(loc) = ptr)
#define LOAD_GLOBFUN(ptr, loc)   (SET_TAG(loc, CL_GLOBFUN),GET_GFARG(loc) = ptr)
#define LOAD_DOWNFUN(ptr, loc)   (SET_TAG(loc, CL_DOWNFUN),GET_DFARG(loc) = ptr)
#define LOAD_CLOSURE(ptr, loc)   (SET_TAG(loc, CL_CLOSURE),GET_FORM(loc) = ptr)
#define LOAD_UNBOUND(sym_val)    SET_TAG(sym_val, CL_UNBOUND)
#define LOAD_STRUCT(ptr, loc)    (SET_TAG(loc, CL_STRUCT),GET_FORM(loc) = ptr)
#define LOAD_CFILE(ptr, loc)     (SET_TAG(loc, CL_CFILE),GET_CFILE(loc) = ptr)
#define LOAD_UNIQUE_TAG(loc)     (SET_TAG(loc, CL_UNIQUE_TAG),\
                                  GET_FIXNUM(loc) = tag_counter++)

#define LOAD_VECTOR(vec,tag,loc)  (SET_TAG(loc, tag),GET_FORM(loc) = (vec))
#define LOAD_ARRAY(vec,tag,loc)  (SET_TAG(loc, tag),GET_FORM(loc) = (vec))

#define LOAD_C_CHAR(chr, loc) \
   (SET_TAG(loc, CL_C_CHAR), ((loc)->val.ch) = (unsigned long)(chr))
#define LOAD_C_SHORT(num, loc) \
   (SET_TAG(loc, CL_C_SHORT), ((loc)->val.i) = (long)(num))
#define LOAD_C_INT(num, loc) \
   (SET_TAG(loc, CL_C_INT), ((loc)->val.i) = (long)(num))
#define LOAD_C_LONG(num, loc) \
   (SET_TAG(loc, CL_C_LONG), ((loc)->val.i) = (long)(num))
#define LOAD_C_UNSIGNED_CHAR(chr, loc) \
   (SET_TAG(loc, CL_C_UNSIGNED_CHAR), ((loc)->val.ch) = (unsigned long)(chr))
#define LOAD_C_UNSIGNED_SHORT(num, loc) \
   (SET_TAG(loc, CL_C_UNSIGNED_SHORT), ((loc)->val.i) = (long)(num))
#define LOAD_C_UNSIGNED_INT(num, loc) \
   (SET_TAG(loc, CL_C_UNSIGNED_INT), ((loc)->val.i) = (long)(num))
#define LOAD_C_UNSIGNED_LONG(num, loc) \
   (SET_TAG(loc, CL_C_UNSIGNED_LONG), ((loc)->val.i) = (long)(num))

#define LOAD_C_CHAR_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_CHAR_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_SHORT_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_SHORT_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_INT_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_INT_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_LONG_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_LONG_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_UNSIGNED_CHAR_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_UNSIGNED_CHAR_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_UNSIGNED_SHORT_PTR(ptr, loc)  \
   (SET_TAG(loc, CL_C_UNSIGNED_SHORT_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_UNSIGNED_INT_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_UNSIGNED_INT_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_UNSIGNED_LONG_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_UNSIGNED_LONG_PTR), ((loc)->val.ch_ptr) = (char *)(ptr))
#define LOAD_C_STRUCT(ptr, loc) \
   (SET_TAG(loc, CL_C_STRUCT), ((loc)->val.form) = (ptr))
#define LOAD_C_STRUCT_PTR(ptr, loc) \
   (SET_TAG(loc, CL_C_STRUCT_PTR), ((loc)->val.form) = (ptr))


/*------------------------------------------------------------------------------
 * Aufbau von Symbolen
 *----------------------------------------------------------------------------*/
#define OFF_SYM_PNAME           0
#define OFF_SYM_LEN             0
#define OFF_SYM_C_STRING        1
#define OFF_SYM_PLIST           2
#define OFF_SYM_VALUE           3
#define OFF_SYM_PACKAGE         4
#define OFF_SYM_CONSTFLAG       5

#define SYM_SIZE                6

#define SYM_PLIST(sym)          (GET_FORM(sym) + OFF_SYM_PLIST)
#define SYM_VALUE(sym)          (GET_FORM(sym) + OFF_SYM_VALUE)
#define SYM_PACKAGE(sym)        (GET_FORM(sym) + OFF_SYM_PACKAGE)

#define SYM_CONSTFLAG(sym)      (GET_FORM(sym) + OFF_SYM_CONSTFLAG)
#define SYM_IS_CONST(sym)       CL_TRUEP(SYM_CONSTFLAG(sym))
#define SYM_SET_CONST(sym)      LOAD_T(SYM_CONSTFLAG(sym))
#define SYM_SET_NORMAL(sym)     LOAD_NIL(SYM_CONSTFLAG(sym))

#define LOAD_SYM_PNAME(sym, loc)  LOAD_SMSTR(GET_FORM (sym), loc)
#define SYMBOL(base, index)  (CL_FORM *)(&base[SYM_SIZE * index])
#define SYMVAL(base, index)  ((CL_FORM *)&base[SYM_SIZE * index + OFF_SYM_VALUE])

/* Erstellt neues Symbol. Parameter ist CL_FORM *, Name steht in base+0 */
#define SYM_MAKE_SYM(sym) \
   sym = form_alloc(STACK(base, 1), SYM_SIZE);\
   LOAD_FIXNUM(AR_SIZE(GET_FORM(STACK(base, 0))), OFFSET(sym,OFF_SYM_LEN));\
   LOAD_CHAR_PTR(sm_get_c_string(STACK(base, 0)), \
                 OFFSET(sym,OFF_SYM_C_STRING));\
   LOAD_NIL(OFFSET(sym,OFF_SYM_PLIST));\
   LOAD_UNBOUND(OFFSET(sym,OFF_SYM_VALUE));\
   LOAD_NIL(OFFSET(sym,OFF_SYM_PACKAGE));\
   LOAD_NIL(OFFSET(sym,OFF_SYM_CONSTFLAG))


/*------------------------------------------------------------------------------
 * Zugriff auf Klassen
 *----------------------------------------------------------------------------*/
#define CLASS_SIZE 6
#define CLASS(index) ((CL_FORM *)&classes[CLASS_SIZE * index])


/*------------------------------------------------------------------------------
 * Zugriff auf die Komponenten eines Strings / Vektors / Arrays
 *----------------------------------------------------------------------------*/
#define AR_SIZE(ar)                 GET_FIXNUM(ar)
#define SET_AR_SIZE(sz, ar)         LOAD_FIXNUM(sz, ar)
#define AR_BASE(ar)                 OFFSET(ar, 1)
#define AR_DIMS(ar)                 GET_FIXNUM_PTR(OFFSET(ar, -1))
#define AR_DIM(ar, i)               (AR_DIMS(ar)[(i)+1])
#define AR_RANK(ar)                 AR_DIM(ar, -1)
#define MAKE_NOT_DISPLACED(ar)      LOAD_NIL(OFFSET(ar, -2))
#define DISPLACED_P(ar)             CL_TRUEP(OFFSET(ar,-2))
#define DISPLACED_TO(ar)            AR_BASE(ar)
#define DISPLACED_INDEX_OFFSET(ar)  OFFSET(ar, -2)

#define FILL_PTR(vec)               OFFSET(vec, -1)
#define MAKE_FILL_PTR(vec)          LOAD_FIXNUM(0, FILL_PTR(vec))
#define HAS_FILL_PTR(vec)           CL_TRUEP(FILL_PTR(vec))
#define GET_FILL_PTR(vec)           AR_SIZE(vec)
#define SET_FILL_PTR(sz, vec)       SET_AR_SIZE(sz, vec)
#define AR_SIZE_WHEN_FP(vec)        GET_FIXNUM(FILL_PTR(vec))
#define SET_AR_SIZE_WHEN_FP(sz,vec) LOAD_FIXNUM(sz, FILL_PTR(vec))

#define FORM_AR(ar)                 GET_FORM       (AR_BASE(ar))
#define FIXNUM_AR(ar)               GET_FIXNUM_PTR (AR_BASE(ar))
#define FLOAT_AR(ar)                GET_FLOAT_PTR  (AR_BASE(ar))
#define CHAR_AR(ar)                 GET_CHAR_PTR   (AR_BASE(ar))


/*------------------------------------------------------------------------------
 * Makro für EQ-Funktion
 *----------------------------------------------------------------------------*/
#define EQ(x, y) (TYPE_OF(x) == TYPE_OF(y) && GET_FORM(x) == GET_FORM(y))


/*------------------------------------------------------------------------------
 * Kopieren von statischen Variablen in den Heap.
 * Parameter: 1. Zeiger auf die statische Variable im Stack
 *            2. Zeiger auf Stack-Top
 *----------------------------------------------------------------------------*/
#define GEN_HEAPVAR(var, top) {              \
   CL_FORM *_heap_ptr = form_alloc(top, 1);  \
   COPY(var, _heap_ptr);                     \
   LOAD_IND(_heap_ptr, var);                 \
}


/*------------------------------------------------------------------------------
 * Makro für das Kopieren der CL_FORM-Struktur
 * Bei einigen Kompilern muss Komponentenweise kopiert werden.
 *----------------------------------------------------------------------------*/
#define COPY(source, dest) (*(dest) = *(source))

/* Falls komponentenweise kopiert werden muss, dann unbedingt den Typ
 * zuerst kopieren! Denn sonst wurden Aufrufe der Form
 * COPY(SYM_VALUE(STACK(base, 1)), STACK(base, 1))
 * nicht korrekt uebersetzt werden.
 *    #define COPY(source, dest)\
 *       (dest->tag = source->tag, dest->val = source->val)
 */


/*------------------------------------------------------------------------------
 * Closure auf Heap erzeugen und Kopf ausfüllen.
 *----------------------------------------------------------------------------*/
#define GEN_CLOSURE(ar, top, sz, code, ps) \
   CL_FORM *ar = form_alloc(top, sz);      \
   LOAD_FIXNUM((sz) - 1, OFFSET(ar, 0));   \
   LOAD_CODE(code, OFFSET(ar, 1));         \
   LOAD_FIXNUM(ps, OFFSET(ar, 2))


/*------------------------------------------------------------------------------
 * Speicheranforderungsfunktion
 *----------------------------------------------------------------------------*/
#define MALLOC(size)  malloc(size)

/*------------------------------------------------------------------------------
 * Garbage-Collector-Funktion
 *----------------------------------------------------------------------------*/
#define SAVE_FORM(form)  save_form(form)

/*------------------------------------------------------------------------------
 * Größe der von form_alloc alloziierten Blöcke
 *----------------------------------------------------------------------------*/
#define HEAP_ALIGN(num)

/*------------------------------------------------------------------------------
 * Anzahl der Bits, die für Bitvektoren in einer Fixnum genutzt werden
 *----------------------------------------------------------------------------*/
#define BITS_PER_FIXNUM (sizeof(long) * 8)

/* Ende */


#define TYPESWITCH(x)  switch (TYPEOF(x)) {
#define ENDSWITCH      }
#define ENDCASE        break;
#define CASE_NIL       case CL_NIL:
#define CASE_FIXNUM    case CL_FIXNUM:
/* ... */
#define DEFAULT        default:

/*
#define TYPESWITCH(x)  { CL_FORM *__f = (x);
#define ENDSWITCH      }
#define ENDCASE        } else
#define CASE_NIL       if (CL_NILP(__f)) {
#define CASE_FIXNUM    if (CL_FIXNUMP(__f)) {
#define DEFAULT        {
*/
