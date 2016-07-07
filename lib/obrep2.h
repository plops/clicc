/*------------------------------------------------------------------------------
 * Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel
 *------------------------------------------------------------------------------
 * Projekt  : APPLY - A Practicable And Portable Lisp Implementation
 *            ------------------------------------------------------
 * Funktion : obrep2.h - datenrepräsentationsspezifisch
 *
 * $Revision: 1.4 $
 * $Log: obrep2.h,v $
 * Revision 1.4  1993/11/12  13:09:42  sma
 * Neue Konstante BITS_PER_FIXNUM.
 *
 * Revision 1.3  1993/11/04  14:10:26  sma
 * Neues internes Makro: LOAD_MASK
 *
 * Revision 1.2  1993/10/29  15:01:53  sma
 * Alle notwendigen Änderungen für korrektes Funktionieren.
 *
 * Revision 1.1  1993/10/14  15:55:12  sma
 * Initial revision
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 * Werte für UNBOUND, NIL und T
 *----------------------------------------------------------------------------*/
#define UNBOUND_VALUE  ((long)0)
#define NIL_VALUE      ((long)SYMBOL(Ssys, 0))
#define T_VALUE        ((long)SYMBOL(Ssys, 1))

/*------------------------------------------------------------------------------
 * Datenstruktur der LISP-Objekte
 *----------------------------------------------------------------------------*/
typedef unsigned short TAG;
typedef long CL_FORM;


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
typedef long CL_INIT;
#define CL_INIT2(name) CL_INIT name[] __attribute__ ((aligned (8))) =
   

/*------------------------------------------------------------------------------
 * 
 *----------------------------------------------------------------------------*/
#define T_IND 1
#define T_SYMBOL 3
#define T_STRUCT 5
#define T_INSTANCE 7
#define T_CLOSURE 11
#define T_DOWNFUN 13
#define T_GLOBFUN 15
#define T_CFILE 51
#define T_C_FOREIGN 53
#define T_VEC_T (17+0)
#define T_VEC_FIXNUM (17+2)
#define T_VEC_FLOAT (17+4)
#define T_STRING (17+6)
#define T_SMVEC_T (81+0)
#define T_SMVEC_FIXNUM (81 + 2)
#define T_SMVEC_FLOAT (81 + 4)
#define T_SMSTR (81 + 6)
#define T_SMAR_T (97+0)
#define T_SMAR_FIXNUM (97+2)
#define T_SMAR_FLOAT (97+4)
#define T_SMAR_CHAR (97+6)
#define T_AR_T (33+0)
#define T_AR_FIXNUM (33+2)
#define T_AR_FLOAT (33+4)
#define T_AR_CHAR (33+6)

#define INUM(x)  (((long)(x) << 3) + 2)
#define IFLOAT(x)  ((long)(x) + 4)
#define ICHR(x)  (((long)(unsigned char)(x) << 16) + 6)


/*------------------------------------------------------------------------------
 * Konstruktoren für Konstantendefinitionen
 *----------------------------------------------------------------------------*/
#define END_SYMDEF  0           /* Endmarkierung einer Symboltabelle */
#define IS_END_SYMDEF(x)        (*(x) == END_SYMDEF)

#define MAKE_NIL                MAKE_SYMREF(NIL_VALUE)
#define MAKE_T                  MAKE_SYMREF(T_VALUE)
#define MAKE_UNBOUND            UNBOUND_VALUE
#define MAKE_FIXNUM(num)        INUM(num)
#define MAKE_FLOAT(flptr)       IFLOAT(flptr)
#define MAKE_CHAR(chr)          ICHR(chr)

#define MAKE_STRING(len, str)\
   ((long)(len) << 8) + T_SMSTR,\
   (long)(str)

#define MAKE_SYMBOL(len, name, plst, val, pkg, cf)\
   ((long)(len) << 8) + T_SYMBOL,\
   (long)name,\
   plst, val, pkg, cf

#define MAKE_CLASS(name, cpl, slotnum, slotinfo)\
   (5L << 8) + T_INSTANCE,\
   MAKE_NIL,\
   MAKE_SYMREF(name),\
   cpl,\
   MAKE_FIXNUM(slotnum),\
   slotinfo

#define MAKE_INSTANCE(sz, class)\
   ((long)(sz) << 8 + T_INSTANCE,\
   class

#define MAKE_CONS(car, cdr)     car, cdr
#define MAKE_VECTOR(sz)         (((long)(sz) << 8) + T_SMVEC_T)

#define MAKE_ARRAY(rank, dims, sz) sma_error
   
#define MAKE_STRREF(str)        (long)(str)
#define MAKE_SYMREF(sym)        (long)(sym)
#define MAKE_CLASSREF(class)    (long)(class)
#define MAKE_LIST(list)         (long)(list)
#define MAKE_VECREF(vec)        (long)(vec)
#define MAKE_ARREF(ar)          (long)(ar)
#define MAKE_STRUCTREF(str)     (long)(str)

#define MAKE_GLOBFUN(fun)       (1 << 8) + T_GLOBFUN, (long)(fun) 

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
#define TYPE_OF(loc)            type_of(loc)
#define SET_TAG(loc, value)     sma_error

/* Grundtypen */
#define GET_FORM(loc)           ((long *)(*(loc)))

#define GET_FIXNUM(loc)         (*(loc) >> 3)
#define GET_FLOAT(loc)          (*(double *)(*(loc) - 4))
#define GET_CHAR(loc)           (*(loc) >> 16)

#define GET_FIXNUM_PTR(loc)     ((long *)*(loc))
#define GET_FLOAT_PTR(loc)      ((double *)*(loc))
#define GET_CHAR_PTR(loc)       ((char *)*(loc))

/* Listen */
#define CAR(lptr)               (lptr)
#define CDR(lptr)               ((lptr) + 1)
#define GET_CAR(loc)            GET_FORM(loc)
#define GET_CDR(loc)            (GET_FORM(loc) + 1)

/* Symbol */
#define GET_SYMBOL(loc)         GET_FORM(loc)

/* Funktionen */
#define GET_FUN(loc)            ((CLOSURE_FUN *)*(loc))
#define GET_DFARG(loc)          ((DOWN_FUNARG *)GET_FORM(GET_CDR(loc)))
#define GET_GFARG(loc)          ((GLOBAL_FUNARG *)GET_FORM(GET_CDR(loc)))

#define INDIRECT(loc)           GET_FORM(loc)
#define GET_CFILE(loc)          ((FILE *)GET_FORM(GET_CDR(loc)))

/* Foreign Fuctions */
#define GET_C_CHAR(loc)               0
#define GET_C_SHORT(loc)              0
#define GET_C_INT(loc)                0
#define GET_C_LONG(loc)               0
#define GET_C_UNSIGNED_CHAR(loc)      0
#define GET_C_UNSIGNED_SHORT(loc)     0
#define GET_C_UNSIGNED_INT(loc)       0
#define GET_C_UNSIGNED_LONG(loc)      0

#define GET_C_CHAR_PTR(loc)           0
#define GET_C_SHORT_PTR(loc)          0
#define GET_C_INT_PTR(loc)            0
#define GET_C_LONG_PTR(loc)           0
#define GET_C_UNSIGNED_CHAR_PTR(loc)  0
#define GET_C_UNSIGNED_SHORT_PTR(loc) 0
#define GET_C_UNSIGNED_INT_PTR(loc)   0
#define GET_C_UNSIGNED_LONG_PTR(loc)  0

#define GET_C_STRUCT(loc)             0
#define GET_C_STRUCT_PTR(loc)         0

#define GET_C_STRUCT_PTR_VALUE(loc)   0

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

#define CL_NUMBERP(obj)       (CL_FIXNUMP(obj) || CL_FLOATP(obj))
#define CL_LISTP(loc)         (CL_CONSP(loc) || CL_NILP(loc))
#define CL_STRING_P(loc)      (CL_SMSTRP(loc) || CL_STRINGP(loc))
#define CL_FUNCTION_P(loc)    (CL_CLOSUREP(loc) || CL_DOWNFUNP(loc) ||\
                               CL_GLOBFUNP(loc))
#define CL_VECTOR_P(loc)      VECTORP(TYPE_OF(loc))
#define CL_ARRAY_P(loc)       ARRAYP(TYPE_OF(loc))
#define CL_SMAR_P(loc)        SMARP(TYPE_OF(loc))

#define CL_C_STRUCT_P(loc) 0
#define CL_C_CHAR_P(loc)   0
#define CL_C_SHORT_P(loc)  0
#define CL_C_INT_P(loc) 0
#define CL_C_LONG_P(loc) 0
#define CL_C_UNSIGNED_CHAR_P(loc) 0
#define CL_C_UNSIGNED_SHORT_P(loc) 0
#define CL_C_UNSIGNED_INT_P(loc) 0
#define CL_C_UNSIGNED_LONG_P(loc) 0
#define CL_C_CHAR_PTR_P(loc) 0
#define CL_C_SHORT_PTR_P(loc) 0
#define CL_C_INT_PTR_P(loc)    0
#define CL_C_LONG_PTR_P(loc)   0
#define CL_C_UNSIGNED_CHAR_PTR_P(loc) 0
#define CL_C_UNSIGNED_SHORT_PTR_P(loc) 0
#define CL_C_UNSIGNED_INT_PTR_P(loc)   0
#define CL_C_UNSIGNED_LONG_PTR_P(loc) 0


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
#define LOAD_NIL(loc)          (*(loc) = NIL_VALUE)
#define LOAD_T(loc)            (*(loc) = T_VALUE)
#define LOAD_FIXNUM(num, loc)  (*(loc) = INUM(num))
#define LOAD_FLOAT(flptr, loc) (*(loc) = IFLOAT(flptr))
#define LOAD_CHAR(chr, loc)    (*(loc) = ICHR(chr))

#define LOAD_FORM_PTR(form,   loc) (*(loc) = (long)(form))
#define LOAD_FIXNUM_PTR(iptr, loc) LOAD_FORM_PTR(iptr,  loc)
#define LOAD_FLOAT_PTR(flptr, loc) LOAD_FORM_PTR(flptr, loc)
#define LOAD_CHAR_PTR(chptr,  loc) LOAD_FORM_PTR(chptr, loc)

#define LOAD_CONS(car, loc)      LOAD_FORM_PTR(car, loc)
#define LOAD_SYMBOL(sym, loc)    LOAD_FORM_PTR(sym, loc)

#define LOAD_MASK(loc, tag)  (*(loc) = (*(loc) & ~255 | (tag)))

#define LOAD_CLASS(class, loc) (LOAD_FORM_PTR(class, loc),\
                                LOAD_MASK(class, T_INSTANCE))
#define LOAD_STRUCT(ptr, loc) (LOAD_FORM_PTR(ptr, loc),\
                               LOAD_MASK(ptr, T_STRUCT))

#define LOAD_SMSTR(strptr, loc) (LOAD_FORM_PTR(strptr, loc),\
                                 LOAD_MASK(strptr, T_SMSTR))
#define LOAD_STRING(strptr, loc) (LOAD_FORM_PTR(strptr, loc),\
                                  LOAD_MASK(strptr, T_STRING))
#define LOAD_SMVEC_T(strptr, loc) (LOAD_FORM_PTR(strptr, loc),\
                                   LOAD_MASK(strptr, T_SMVEC_T))
#define LOAD_SMAR_T(strptr, loc) (LOAD_FORM_PTR(strptr, loc),\
                                  LOAD_MASK(strptr, T_SMAR_T))

#define LOAD_FUN(fun, tag, loc) {       \
   CL_FORM *_lptr = form_alloc(loc, 2); \
   _lptr[0] = (1 << 8) + tag;           \
   _lptr[1] = (long)fun;                \
   LOAD_FORM_PTR(_lptr, loc);           \
}
#define LOAD_GLOBFUN(ptr, loc) LOAD_FUN(ptr, T_GLOBFUN, loc)
#define LOAD_DOWNFUN(ptr, loc) LOAD_FUN(ptr, T_DOWNFUN, loc)
#define LOAD_CLOSURE(ptr, loc) LOAD_FORM_PTR(ptr, loc)

#define LOAD_CFILE(ptr, loc) LOAD_FUN(ptr, T_CFILE, loc)
      
#define LOAD_UNBOUND(loc)        (*(loc) = UNBOUND_VALUE)
#define LOAD_UNIQUE_TAG(loc)     LOAD_UNBOUND(loc)

#define LOAD_VECTOR(vec,tag,loc) \
    (LOAD_MASK(vec, 17+(tag>=CL_SMVEC)*64+(tag&3)*2),\
     LOAD_FORM_PTR(vec, loc))
#define LOAD_ARRAY(vec,tag,loc) \
    (LOAD_MASK(vec, 33+(tag>=CL_AR)*64+(tag&3)*2),\
     LOAD_FORM_PTR(vec, loc))

#if 0
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
#endif


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

#define LOAD_SYM_PNAME(sym, loc) {               \
   CL_FORM *str = form_alloc(STACK(base, 1), 2); \
   COPY(GET_FORM(sym), str);                     \
   COPY(GET_FORM(sym) + 1, str + 1);             \
   LOAD_SMSTR(str, loc);                         \
}

#define SYMBOL(base, index)  (CL_FORM *)(&base[SYM_SIZE * index])
#define SYMVAL(base, index)  ((CL_FORM *)&base[SYM_SIZE * index + OFF_SYM_VALUE])

/* Erstellt neues Symbol. Parameter ist CL_FORM *, Name steht in base+0 */
#define SYM_MAKE_SYM(sym)                          \
   sym = form_alloc(STACK(base, 1), SYM_SIZE);     \
   sym[0] = (OFF_SYM_LEN << 8) + T_SYMBOL;         \
   sym[1] = (long)sm_get_c_string(STACK(base, 0)); \
   sym[2] = sym[4] = sym[5] = NIL_VALUE;           \
   sym[3] = UNBOUND_VALUE


/*------------------------------------------------------------------------------
 * Zugriff auf Klassen
 *----------------------------------------------------------------------------*/
#define CLASS_SIZE 6
#define CLASS(index) ((CL_FORM *)&classes[CLASS_SIZE * index])


/*------------------------------------------------------------------------------
 * Zugriff auf die Komponenten eines Strings / Vektors / Arrays
 *----------------------------------------------------------------------------*/
#define AR_SIZE(ar)                 (*(ar) >> 8)
#define SET_AR_SIZE(sz, ar)         (*(ar) = ((sz) << 8) + ((*(ar)) & 255))
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
#define EQ(x, y) (*(x) == *(y))


/*------------------------------------------------------------------------------
 * Kopieren von statischen Variablen in den Heap.
 * Parameter: 1. Zeiger auf die statische Variable im Stack
 *            2. Zeiger auf Stack-Top
 *----------------------------------------------------------------------------*/
#define GEN_HEAPVAR(var, top) {        \
   CL_FORM *_ptr = form_alloc(top, 2); \
   _ptr[0] = *var;                     \
   _ptr[1] = (long)0;                  \
   LOAD_FORM_PTR(_ptr, var);           \
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
   ar[0] = (((sz) - 1) << 8) + T_CLOSURE;  \
   ar[1] = (long)(code);                   \
   ar[2] = INUM(ps)

/*------------------------------------------------------------------------------
 * Speicheranforderungsfunktion
 *----------------------------------------------------------------------------*/
#define MALLOC(size)  (PTR)malloc8(size)

/*------------------------------------------------------------------------------
 * Garbage-Collector-Funktion
 *----------------------------------------------------------------------------*/
#define SAVE_FORM(form)  save_form2(form)

/*------------------------------------------------------------------------------
 * Größe der von form_alloc alloziierten Blöcke
 *----------------------------------------------------------------------------*/
#define HEAP_ALIGN(num)  num = (num + 1) & ~1

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

