;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Definitionen fuer Strukturen, Makros, Konstanten, Variablen  
;;;
;;; $Revision: 1.43 $
;;; $Log: clcdef.lisp,v $
;;; Revision 1.43  1993/12/22  09:22:50  hk
;;; Für CMU17 müssen bei make-instance Symbole statt Klassen verwendet
;;; werden.
;;;
;;; Revision 1.42  1993/12/06  16:44:52  hk
;;; Definition und Initialisierung einiger globaler Variablen nach hier
;;; verlagert. Englische Kommentare für Variablen, die der Benutzer
;;; verändern darf.
;;;
;;; Revision 1.41  1993/11/05  14:20:50  hk
;;; require an den Dateianfang verschoben, um Probleme mit akcl zu vermeiden.
;;;
;;; Revision 1.40  1993/10/25  11:35:20  hk
;;; (defconstant *CLICC-Version* "0.6.2")
;;;
;;; Revision 1.39  1993/09/09  06:14:47  ft
;;; *METHOD-CACHE-LIMIT* ist ab jetzt eine Variable.
;;;
;;; Revision 1.38  1993/08/20  11:47:59  ft
;;; Neue Programmkonstante *METHOD-CACHE-LIMIT* eingefuehrt, die angibt
;;; wieviele effektive Methoden eine generische Funktion besitzen darf,
;;; ohne dass ein Methoden-Cache angelegt wird.
;;;
;;; Revision 1.37  1993/07/23  08:26:29  hk
;;; (defconstant *CLICC-Version* 0.6.1)
;;;
;;; Revision 1.36  1993/07/11  13:34:17  kl
;;; Neue Variable *ITERATIONS* eingefuehrt.
;;;
;;; Revision 1.35  1993/06/19  21:13:21  hk
;;; *clicc-version* auf 0.6
;;;
;;; Revision 1.34  1993/06/17  10:01:58  hk
;;; Die Make Funktion, die von defzws generiert wird, ruft nun make-instance
;;; mit einer Klasse als erstem Argument auf.
;;;
;;; Revision 1.33  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.32  1993/06/04  13:12:40  hk
;;; Neue Variable *C-max-line-count*.
;;;
;;; Revision 1.31  1993/05/19  15:10:55  hk
;;; *zw-sym-fun-hash-table* :
;;; make-hash-table erst in init-zw-sym-fun-hash-table.
;;;
;;; Revision 1.30  1993/05/19  10:24:12  hk
;;; *CLICC-INPUT* gestrichen.
;;;
;;; Revision 1.29  1993/05/19  08:52:24  hk
;;; (defvar *ffi-package*)
;;;
;;; Revision 1.28  1993/04/22  12:45:04  hk
;;; Unbenutzte Variablen Definitionen gestrichen.
;;;
;;; Revision 1.27  1993/04/18  16:57:42  atr
;;; Seiteneffektanalyse angeschaltet.
;;;
;;; Revision 1.26  1993/04/16  08:09:39  hk
;;; Neue Variablen *LISP-MODULE*, *INLINE-MODULE*.
;;;
;;; Revision 1.25  1993/04/02  11:53:31  uho
;;; *MODULE-COMPILER* eingefuehrt um den Modul-Compiler aktivieren
;;; zu koennen.
;;;
;;; Revision 1.24  1993/03/18  15:47:15  hk
;;; *NO-SIDE-EFFECT-ANALYSIS* mit T initialisiert.
;;;
;;; Revision 1.23  1993/03/18  15:19:46  ft
;;; Tippfehler.
;;;
;;; Revision 1.22  1993/03/18  15:18:23  ft
;;; Neuen Schalter *NO-SIDE-EFFECT-ANALYSIS* eingebaut.
;;;
;;; Revision 1.21  1993/03/18  14:48:52  uho
;;; Globale Variable *SYNTAX-EXPORT* eingefuehrt, die die Liste
;;; der Konstrukte enthaelt, die syntaktische exportiert werden.
;;; In defzws werden Accessor-Funktionen als inline deklariert.
;;; In defzws werden Konstruktorfunktionen fuer die jeweilige Klasse in der Form
;;; make-<name> angelegt.
;;;
;;; Revision 1.20  1993/02/16  15:48:04  hk
;;; Revision Keyword eingefuegt, *clicc-package* entfernt.
;;;
;;; Revision 1.19  1993/01/21  09:43:50  uho
;;; *FLAT-IFS* eingefuehrt, Option zum Generieren flacher IFs.
;;;
;;; Revision 1.18  1993/01/20  15:17:49  kl
;;; Ladereihenfolge umgestellt. clcmisc wird jetzt schon hier geladen.
;;;
;;; Revision 1.17  1993/01/14  12:48:10  hk
;;; *user-package* und *runtime-package* neu.
;;;
;;; Revision 1.16  1993/01/02  12:57:06  kl
;;; *NO-CODEGEN* zum Abschalten der Codegenerierung eingebaut.
;;; Deklaration der Variablen *SPLIT* aus clcload hierhin verlegt.
;;;
;;; Revision 1.15  1992/11/20  13:52:52  ft
;;; Variable *zw-sym-fun-hash-table* eingefuehrt.
;;;
;;; Revision 1.14  1992/10/15  11:47:06  uho
;;; *COMPILE-MODULE* eingefuegt, um neues Modulsystem zu implementieren.
;;;
;;; Revision 1.13  1992/10/07  17:37:22  hk
;;; Neue Variable *OPTIMIZE*.
;;;
;;; Revision 1.12  1992/10/05  16:18:22  uho
;;; Kommentar zu *MEMSIZES* korrigiert.
;;;
;;; Revision 1.11  1992/10/05  11:11:41  uho
;;; Initialisierung von *MEMSIZES* entfernt.
;;;
;;; Revision 1.10  1992/10/05  10:48:26  uho
;;; *MEMSIZES* eingefuegt, um Speicherbreichsverwaltung parametrisierbar
;;; zu machen.
;;;
;;; Revision 1.9  1992/09/29  20:38:41  hk
;;; (defvar *TAB*) entfernt, nicht mehr benutzt.
;;;
;;; Revision 1.8  1992/09/25  16:44:16  kl
;;; Umstellung auf die neuen Dateien zsdef.lisp und zsops.lisp.
;;;
;;; Revision 1.7  1992/08/14  08:45:16  kl
;;; Konstante *CLICC-Version* eingefuehrt.
;;;
;;; Revision 1.6  1992/08/05  09:43:02  hk
;;; *SYS-FUN-DEF* definiert.
;;;
;;; Revision 1.5  1992/07/29  14:09:07  hk
;;; Neu Variablen *keyword-package* und *lisp-package*.
;;;
;;; Revision 1.4  1992/07/22  15:41:04  hk
;;; Macro defclass1, um Structuren wie mit defzws definieren zu koennen.
;;;
;;; Revision 1.3  1992/07/08  15:36:03  hk
;;; Neue Variable *sym-adr*, gestrichen: *c-name-prefix*.
;;;
;;; Revision 1.2  1992/06/04  07:11:20  hk
;;; Nach Umstellung auf die Lisp nahe Zwischensprache, Syntax-Fehler
;;; sind schon beseitigt
;;;
;;; Revision 1.1  1992/03/24  16:54:56  hk
;;; Initial revision
;;;
;;; Changes  :
;;; 03.02.92 : Neue Komponente 'constants' in der Struktur fun.
;;; 04.02.92 : Neue Struktur STRUCTURED-CONST.
;;; 10.02.92 : Neue Komponente 'module' in der struktur SYM
;;; 12.02.92 : Neue Komponenten 'ref-fix-array' und 'ref-float-array'
;;;            in der Struktur STRUCTURED-CONST, damit waehrend der
;;;            Codegenerierung die entsprechenden Deklarationen erzeugt werden
;;;            koennen.
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Global constants
;;------------------------------------------------------------------------------
(defconstant *CLICC-Version* "0.6.2")   ; Versionsnumber of the compiler

;;------------------------------------------------------------------------------
;; Global variables. The default values may be adjusted by the user.
;;------------------------------------------------------------------------------
(defparameter *ANSI-C* T)               ; generate ANSI C code
(defparameter *warn-unbound-vars* nil)  ; give warning if variable with name
                                        ; *...* is not bound and has no special
                                        ; declaration
(defparameter *SPLIT-FILES* NIL)        ; a separate file for each global
                                        ; function
(defparameter *C-max-line-count* 50000) ; split file into file1, file2, ..
                                        ; if more lines than indicated
(defparameter *FLAT-IFS* nil)           ; Should if forms be expressed by
                                        ; goto statements or by if statements
(defparameter *ONLY-PASS1* NIL)         ; Do only macroexpansion and
                                        ; transformation into the intermediate
                                        ; language. This may be used to check
                                        ; the restrictions od CL_0.
(defparameter *NO-CODEGEN* NIL)         ; No code generation
(defparameter *NO-SIDE-EFFECT-ANALYSIS* NIL) ; No side effect analysis
(defparameter *OPTIMIZE*   T)           ; Do optimizations
(defparameter *ITERATIONS* 1)           ; Number of analyzation / optimization
                                        ; cycles
(defparameter *METHOD-CACHE-LIMIT* 4)   ; use a method cache if a generic
                                        ; function has more methods than this
                                        ; value
(DEFVAR *MEMSIZES*    
  '((2000 . 5000) (#x10000 . #x60000)  (#x600 . #x6000) (#x200 . #x2000))) 
;;          stack           form-heap           fx-heap          fl-heap
                                        ; a list of pairs (min . max)
                                        ; the sizes of the different memory
                                        ; areas: lisp-stack, form-heap,
                                        ; fixnum/character-heap and float-heap
(defvar *SHOW-VERSION* NIL)             ; show version and copyright message
(defvar *CLICC-PRINT* NIL)              ; show messages

;;------------------------------------------------------------------------------
;; global variables
;;------------------------------------------------------------------------------
(defvar *FILENAME*)                     ; Name der Datei, die uebersetzt
                                        ; werden soll
(defvar *EXTENSION*)                    ; Extension der Hauptdatei, falls
                                        ; angegeben, "" sonst.
(defvar *OUT-FILENAME*)                 ; Der Name (ohne Extension) der Datei,
                                        ; in der der Code generiert wird.
(defvar *MODULE-COMPILER*)              ; Soll ein Modul uebersetzt werden?
                                        ; T: Modul wird uebersetzt und
                                        ;    Headerfiles geschrieben
                                        ; NIL: Komplettkompilation
(defvar *LISP-MODULE*)                  ; Das Lisp Modul soll uebersetzt werden
(defvar *INLINE-MODULE*)                ; Code fuer Lisp Funktionen generieren
                                        ; ueber deren Code der Compiler
                                        ; spezielles Wissen hat.
(defvar *module*)                       ; das gerade bearbeitete Modul
(defvar *NERRORS*)                      ; Zaehler fuer Fehler
(defvar *NWARNINGS*)                    ; Zaehler fuer Warnungen
(defvar *keyword-package*)
(defvar *lisp-package*)
(defvar *user-package*)
(defvar *runtime-package*)
(defvar *ffi-package*)
(defvar *zw-sym-fun-hash-table*)        ; Fuer unsere eigenes SYMBOL-FUNCTION
                                        ; im compiletime interpreter.
(defvar *CLICC-REQUIRED*)               ; Liste der Modulnamen fuer REQUIRE
(defvar *SYNTAX-EXPORT*)                ; Liste, in die fuer den Syntaxteil
                                        ; exportierten Konstrukte aufgesammelt
                                        ; werden.
                                        ; - Macros
                                        ; - Makrofunktionen
                                        ; - DEFSETFs
                                        ; - DEFTYPEs
                                        ; - specials
(defvar *CLICC-PATH-STRING*)            ; Ein String, der einen Pfadnamen
                                        ; enthaelt; die Wurzel des CLICC-
                                        ; Dateibaumes.

;;------------------------------------------------------------------------------
;; special declarations
;;------------------------------------------------------------------------------
(defvar *FUN-NAME*)                     ; Name der momentan uebersetzten
                                        ; Funktion bzw. "toplevel form"
(defvar *CURRENT-FORM*)                 ; Aktuelle Form fuer Fehlermeldungen
(defvar *SDF* NIL)                      ; Handelt es sich um eine vom System
                                        ; generierte Fkt. ?
(defvar *CLICC-FILENAME*)               ; Name der momentan uebersetzten Datei

;;------------------------------------------------------------------------------
;; Typ Bool
;;------------------------------------------------------------------------------
(deftype bool ()
  '(or null (member t)))

;;------------------------------------------------------------------------------
;; defzws stuetzt sich nicht auf defstruct sondern auf defclass, weil
;; - in defclass fuer Slots eine Typspezifikation angegeben werden kann,
;;   ohne dass eine Initform angegeben werden muss,
;; - die Accessor-Funktionen generisch sind, und somit fuer verschiedene
;;   Klassen gleiche Namen haben koennen.
;;------------------------------------------------------------------------------
(defmacro defzws (name supers &rest slots)
  (let ((accessors ()))
    (setq slots
          (mapcar #'(lambda (slot-desc)
                      (when (atom slot-desc) (setq slot-desc (list slot-desc)))
                      (let* ((sym (car slot-desc))
                             (name (symbol-name sym)))
                        (append slot-desc
                                (list :accessor
                                      (let ((acc (intern-prefixed "?" sym)))
                                        (push acc accessors)
                                        acc)
                                      :initarg
                                      (intern name (find-package "KEYWORD"))))))
                  slots))
    `(progn
      (defclass ,name ,supers (,@slots))
      ,@(mapcar #'(lambda (slot-accessor)
                    `(proclaim '(inline ,slot-accessor)))
         accessors)
      (defun ,(intern-prefixed "MAKE-" name) (&rest options)
        (apply #'make-instance
               #-CMU17(find-class ',name)
               #+CMU17',name
               options))
      (defun ,(intern-postfixed name "-P") (x)
        (typep x ',name)))))

;;------------------------------------------------------------------------------
(defmacro defclass1 (&rest args) `(defzws ,@args))

;;------------------------------------------------------------------------------
(provide "clcdef")
