;;;-----------------------------------------------------------------------------
;;; CLiCC: The Common Lisp to C Compiler
;;; Copyright (C) 1994 Wolfgang Goerigk, Ulrich Hoffmann, Heinz Knutzen 
;;; Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; CLiCC has been developed as part of the APPLY research project,
;;; funded by the German Ministry of Research and Technology.
;;; 
;;; CLiCC is free software; you can redistribute it and/or modify
;;; it under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 2 of the License, or
;;; (at your option) any later version.
;;;
;;; CLiCC is distributed in the hope that it will be useful,
;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License in file COPYING for more details.
;;;
;;; You should have received a copy of the GNU General Public License
;;; along with this program; if not, write to the Free Software
;;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;;;-----------------------------------------------------------------------------
;;; Funktion : Definitionen fuer Strukturen, Makros, Konstanten, Variablen  
;;;
;;; $Revision: 1.55 $
;;; $Id: clcdef.lisp,v 1.55 1994/12/01 16:52:21 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Global constants
;;------------------------------------------------------------------------------
(defconstant *CLICC-Version* "0.6.5")   ; version number of the compiler
(defconstant *RCS-Date* "$Date: 1994/12/01 16:52:21 $")

(defun CLiCC-Copyright-Year () (subseq *RCS-Date* 7 11))

;;------------------------------------------------------------------------------
;; Global variables. The default values may be adjusted by the user.
;;------------------------------------------------------------------------------
(defparameter *ANSI-C* T)               ; generate ANSI C code
(defparameter *warn-unbound-vars* nil)  ; give warning if variable with name
                                        ; *...* is not bound and has no special
                                        ; declaration
(defparameter *IGNORE-THE* T)           ; (THE type form) will be ignored
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
(defvar *no-pre-analysis* nil)          ; pre analysis for side effect analysis
(defvar *no-let-optimizing* nil)        ; substitution of local variables
(defvar *no-tail-recursion* nil)        ; no tail recursion elimination

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
(defvar *Side-effect-verbosity* 1)

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
(defvar *CLICC-DIR*)                    ; The :directory which is the root
                                        ; of the CLiCC installation
(defvar *error-function*)               ; Calling ERROR has no side effects
(defvar *C-NAME-PREFIX*)                ; Counterfor generating prefix of
                                        ; C function names

;;------------------------------------------------------------------------------
;; special declarations
;;------------------------------------------------------------------------------
(defvar *FUN-NAME*)                     ; Name der momentan uebersetzten
                                        ; Funktion bzw. "toplevel form"
(defvar *CURRENT-FORM*)                 ; Aktuelle Form fuer Fehlermeldungen
(defvar *CURRENT-FUN*)                  ; Momentan bearbeitete Funktion 
(defvar *SDF* NIL)                      ; Handelt es sich um eine vom System
                                        ; generierte Fkt. ?
(defvar *CLICC-FILENAME*)               ; Name der momentan uebersetzten Datei

;;------------------------------------------------------------------------------
;; Typ Bool
;;------------------------------------------------------------------------------
(deftype bool ()
  '(or null (member t)))

;;------------------------------------------------------------------------------
;; Hilfsfunktionen fuer defzws
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Argument: Property-Liste bestehend aus Keyword- / Ausdruck-Paaren
;;           k1 e1 k2 e2 ... kn en
;; Resultat: Values
;;           1. Property-Liste von Keyword- / Variablen-Paaren
;;              k1 v1 k2 v2 ... kn vn
;;           2. A-Liste von Variablen- / Ausdruck-Listen
;;              (v1 e1) (v2 e2) ... (vn en)
;;------------------------------------------------------------------------------
(defun args2kv/ve-list (args)
  (let ((kv (empty-queue))
        (ve (empty-queue)))
    (loop
     (when (endp args) (return))
     (let ((v (gensym)))
       (add-q (pop args) kv) (add-q v kv)
       (add-q (list v (pop args)) ve)))
    (values (queue2list kv) (queue2list ve))))

;;------------------------------------------------------------------------------
;; Argumente: 1. Property-Liste von Keyword- / Variablen-Paaren
;;               k1 v1 k2 v2 ... kn vn
;;            2. A-Liste von Keyword- / Wert-Paaren
;;               (l1 . i1) (l2 . i2) ... (lm . im)
;; Resultate: Values
;;            1. A-Liste von Keyword- / Variablen- / Wert-Listen
;;               der Keywords aus dem 1. Argument, die in dem 2. Argument
;;               vorhanden sind
;;            2. Teilliste des 1. Arguments, mit den Keywords,
;;               die nicht in dem 2. Argument gefunden wurden.
;;------------------------------------------------------------------------------
(defun split-raw-add-init (kv-list raw-ki-list)
  (let ((raw-kvi (empty-queue))
        (not-raw-kv (empty-queue)))
    (loop
     (when (endp kv-list) (return))
     (let* ((k (pop kv-list))
            (v (pop kv-list))
            (ki (assoc k raw-ki-list)))
       (if ki
           (add-q (list k v (cdr ki)) raw-kvi)
           (progn (add-q k not-raw-kv) (add-q v not-raw-kv)))))
    (values (queue2list raw-kvi) (queue2list not-raw-kv))))

;;------------------------------------------------------------------------------
;; Generiert einen Aufruf der Gestalt
;; (make-instance '<class> :raw `((:k <value> ...)) :k <value> ...)
;;------------------------------------------------------------------------------
(defun gen-raw-constructor-call (class args raw-ki-list)
  `(make-instance #-CMU17 (find-class ',class) #+CMU17 ',class ,@args)
#|
  (let ((raw-v (gensym)))
    (multiple-value-bind (kv ve)
        (args2kv/ve-list args)
      (when raw-ki-list (push (cons raw-v ()) ve))
      (multiple-value-bind (raw-kvi not-raw-kv)
          (split-raw-add-init kv raw-ki-list)
        (when raw-ki-list (push raw-v not-raw-kv) (push :raw not-raw-kv))
        `(let (,@ve)
          ,@(mapcar #'(lambda (kvi)
                        `(unless (eql ,@(cdr kvi))
                          (push (cons ,(first kvi) ,(second kvi)) ,raw-v)))
                    raw-kvi)
          (make-instance #-CMU17 (find-class ',class) #+CMU17 ',class
           ,@not-raw-kv)))))
|#)

;;------------------------------------------------------------------------------
;; defzws stuetzt sich nicht auf defstruct sondern auf defclass, weil
;; - in defclass fuer Slots eine Typspezifikation angegeben werden kann,
;;   ohne dass eine Initform angegeben werden muss,
;; - die Accessor-Funktionen generisch sind, und somit fuer verschiedene
;;   Klassen gleiche Namen haben koennen.
;;------------------------------------------------------------------------------
(defmacro defzws (name supers &rest slots)
  (let ((accessors (empty-queue))
        (real-slots (empty-queue))
        (raw-key-init-pairs (empty-queue))
        (raw-slot-accessors (empty-queue))
        raw-key-init-name)
    (dolist (slot-desc slots)
      (when (atom slot-desc) (setq slot-desc (list slot-desc)))
      (let* ((sym (car slot-desc))
             (acc-sym (intern-prefixed "?" sym))
             (initarg (intern (symbol-name sym) "KEYWORD")))
        (add-q acc-sym accessors)
        (cond
#|          ((getf (cdr slot-desc) :raw nil)
           (let ((init (getf (cdr slot-desc) :initform nil)))
             (unless (or (null init) (eq t init) (keywordp init) (numberp init)
                         (and (consp init) (eq 'quote (car init))))
               (internal-error 'defzws
                               "~a is not allowed as an initial value" init))
             (add-q (cons initarg init) raw-key-init-pairs)
             (add-q `(defmethod ,acc-sym ((i ,name))
                      (let ((a (assoc ,initarg (?raw i))))
                        (if a
                            (cdr a)
                            ,init)))
                    raw-slot-accessors)
             (add-q `(defmethod (setf ,acc-sym) (v (i ,name))
                      (let ((a (assoc ,initarg (?raw i))))
                        (if a
                            (setf (cdr a) v)
                            (unless (eql v ,init)
                              (push (cons ,initarg v) (?raw i))))
                        v))
                    raw-slot-accessors)))
|#          (T
           (remf (cdr slot-desc) :raw)
           (add-q (list* (car slot-desc)
                         :accessor acc-sym
                         :initarg initarg
                         (cdr slot-desc))
                  real-slots)))))

    (setq raw-key-init-pairs (queue2list raw-key-init-pairs))
#|
    (dolist (class supers)
      (let ((raw-key-init-name (intern-postfixed class "-RAW-KEY-INIT")))
        (when (boundp raw-key-init-name)
          (setq raw-key-init-pairs
                (append (symbol-value raw-key-init-name) raw-key-init-pairs)))))
|#

    (when raw-key-init-pairs
      (add-q `(raw :accessor ?raw :initarg :raw :initform nil) real-slots))
    
    `(progn
      (defclass ,name ,supers (,@(queue2list real-slots)))
      ,(when raw-key-init-pairs
              (setq raw-key-init-name (intern-postfixed name "-RAW-KEY-INIT"))
              `(defconstant ,raw-key-init-name
                ',raw-key-init-pairs))
      (proclaim '(inline ,@(queue2list accessors)))
      ,@(queue2list raw-slot-accessors)
      (defmacro ,(intern-prefixed "MAKE-" name) (&rest options)
        (gen-raw-constructor-call ',name options ,raw-key-init-name))
      (defun ,(intern-postfixed name "-P") (x)
        (typep x ',name)))))

;;------------------------------------------------------------------------------
(defmacro defclass1 (&rest args) `(defzws ,@args))

;;------------------------------------------------------------------------------
(provide "clcdef")
