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
;;; Function : Top-Level-Forms
;;;
;;; $Revision: 1.110 $
;;; $Id: p1tlf.lisp,v 1.110 1994/12/13 16:01:27 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defun p1-top-level-form (form)
  (catch 'CLICC-ERROR
    (let ((*CURRENT-FORM* form))        ; Aktuelle Form fuer Fehlermeldungen
      (if (atom form)
          (save-toplevel-form (p1-form form))
          (let* ((name (first form))
                 (args (rest form)) 
                 (operator-def (get-operator-def name)))
            (cond
              ((eq (car operator-def) :USER-MACRO)
               (p1-top-level-form
                (p1-expand-user-macro (cdr operator-def) form)))

              (t (case name
                   (L::defun       (p1-defun      args))
                   (L::progn       (mapc #'p1-top-level-form args))
                   (L::load        (p1-load       args))
                   (ffi:load-foreign (p1-load-foreign args))
                   (L::provide     (p1-provide    args))
                   (L::require     (p1-require    args))
                   (L::declaim     (p1-declaim    args))
                   (L::proclaim    (p1-proclaim   args))
                   (L::export      (p1-export     args))
                   (L::defgeneric  (p1-defgeneric args))
                   (L::defmethod   (p1-defmethod  args))
                   (L::defclass    (p1-defclass   args))
                   (L::defstruct   (p1-defstruct    args))
                   (t
                    (when *MODULE-COMPILER* (extend-syntax-export form))
                    (case name
                      (L::in-package   (p1-in-package   args))
                      (L::use-package  (p1-use-package  args))
                      (L::import       (p1-import       args))
                      (L::defmacro     (p1-defmacro     args))
                      (L::deftype      (p1-deftype      args))
                      (L::def-built-in (p1-def-built-in args))
                      (L::defconstant  (p1-defconstant  args))
                      (L::defparameter (p1-defparameter args))
                      (L::defvar       (p1-defvar       args))
                      (L::defsetf      (p1-defsetf      args))
                      (t (when *MODULE-COMPILER* (retract-syntax-export))
                         (save-toplevel-form (p1-form form)))))))))))))

;;------------------------------------------------------------------------------
;; Analysiert ein Atom oder einen Aufruf einer Special-Form, eines Makros
;; oder einer Funktion.
;;------------------------------------------------------------------------------
(defun p1-form (form)
  (let ((*CURRENT-FORM* form))
    (if (atom form)
      (p1-atom form)
      (p1-call form))))

;;------------------------------------------------------------------------------
;; Analysiert einen atomaren Ausdruck.
;;------------------------------------------------------------------------------
(defun p1-atom (object)
  (if (and (symbolp object) (not (null object)) (not (eq t object)))
    (p1-read-access-of-var object)
    (p1-constant object)))

;;------------------------------------------------------------------------------
;; Analysiert einen Aufruf
;;------------------------------------------------------------------------------
(defun p1-call (form)
  (let ((name (first form))
        (args (rest form)))
    (cond

      ;; Aufrufe von FUNCALL k"onnen in der
      ;; Zwischensprache direkt dargestellt werden
      ;;------------------------------------------
      ((eq 'L:FUNCALL name)
       (make-app :form (p1-form (pop args)) :arg-list (p1-args args)))
      
      ((symbolp name)
       (let* ((operator-def (get-operator-def name))
              (operator     (cdr operator-def))
              cm-operator)

         ;; Compiler-Macros vorher expandieren und mit p1-call fortfahren,
         ;; wenn keine Expansion stattfand.
         ;; Eintrag im global Environment:
         ;; (:COMPILER-MACRO . (expander . (:???-FUN . fun)))
         ;;--------------------------------------------------
         (when (eq :COMPILER-MACRO (car operator-def))
           (let ((new-form (funcall (car operator) form)))
             (unless (eq form new-form)
               (return-from p1-call (p1-form new-form)))
             (setq cm-operator operator)
             (setq operator-def (cdr cm-operator))
             (setq operator (cdr operator-def))))             
               
         (case (car operator-def)
           (:SPECIAL-FORM
            (funcall operator args))
           (:SYS-MACRO
            (p1-form (p1-expand-system-macro operator form)))
           ((:USER-MACRO :LOCAL-MACRO)
            (p1-form (p1-expand-user-macro operator form)))
           ((:LOCAL-FUN :FOREIGN-FUN)
            (clc-check-nparams
             (?par-spec operator) (length args) (?symbol operator))
            (make-instance 'app :form operator :arg-list (p1-args args)))
           (T
            (make-instance
             'app
             :form
             (or (and *compiler-eval*
                      (eq name (find-symbol (symbol-name name) *lisp-package*))
                      (or (zw-symbol-fun name)
                          (progn
                            (clicc-warning
                             "Function ~s may not be used in macro function"
                             name)
                            nil)))
                                  
                 (ecase (car operator-def)
                   ((:GLOBAL-FUN :IMPORTED-FUN)
                    (clc-check-nparams
                     (?par-spec operator) (length args) (?symbol operator))
                    operator)
                   (:FORWARD operator)
                   (:GENERIC-FUN
                    (let ((fun (?fun operator)))
                      (clc-check-nparams (?par-spec fun)
                                         (length args)
                                         (?symbol fun))
                      fun))
                   ((nil) (bind-forward-ref-fun name cm-operator))))
             :arg-list (p1-args args))))))
      
      ;; Lambda-Applikation
      ;;-------------------
      ((and (consp name) (eq (first name) 'L::LAMBDA))
       (p1-lambda-app (rest name) args))
      ((and (consp name) (eq (first name) 'MV-LAMBDA))
       (p1-mv-lambda (rest name) args))
      (t (clicc-error NO_OPERATOR name)))))

;;------------------------------------------------------------------------------
;; defun name lambda-list {declaration || doc-string}* {form}*
;;------------------------------------------------------------------------------
(defun p1-defun (name_lambda-expr &optional fun-block-name)
  (multiple-value-bind (name lambda-list_body)
      (parse-named-lambda 'L::DEFUN name_lambda-expr)
    
    ;; Bei automatisch generierten Funktionen keine Meldung ausgeben.
    ;;---------------------------------------------------------------
    (unless *SDF* (clicc-message "Analyse DEFUN        ~A" name))
    
    (let* ((SETF-FUN (consp name))
           (operator-def (if SETF-FUN
                            (get-setf-fun-def name)
                            (get-operator-def name)))
           cm-operator
          fun)

      (when (eq :COMPILER-MACRO (car operator-def))
        (setq cm-operator (cdr operator-def))
        (setq operator-def (cdr cm-operator)))
      
      (case (car operator-def)
        ((nil)
         (setq fun (make-instance 'global-fun :symbol name))
         (if cm-operator
             (setf (cdr cm-operator) (cons :GLOBAL-FUN fun))
             (if SETF-FUN
                 (set-global-setf-fun-def name :GLOBAL-FUN fun)
                 (set-global-fun name fun))))
        (:FORWARD
         (setq fun (make-instance 'global-fun :symbol name))
         (setf (?value (cdr operator-def)) fun)
         (setf (car operator-def) :GLOBAL-FUN)
         (setf (cdr operator-def) fun))
        (:IMPORTED-FUN
         (if (and *inline-module* (?c-inline (cdr operator-def)))
             (setq fun (make-instance 'global-fun :symbol name :exported T))
             (clicc-error REDEF_FUN name)))
        (t (redef-op-error (car operator-def) name)))

      (unless fun-block-name
        (setf fun-block-name (if (consp name) (second name) name)))

      (setq fun (p1-named-lambda fun name fun-block-name
                                 lambda-list_body))
      (add-q fun (?fun-list *module*))
      (when *module-compiler* (setf (?source fun) lambda-list_body))

      ;; Liefert das generierte Zwischensprachkonstrukt als Wert
      ;;--------------------------------------------------------
      fun)))

;;------------------------------------------------------------------------------
;; Forwaerts Referenzen auf Funktionen werden durch eine Indirektion mittels
;; einer named-constant ausgedrueckt. Die Named-Constant bekommt als Wert
;; :forward und entspricht damit der der Spezifikation der Zwischensprache. Am
;; Ende von Pass1 werden alle Named-Constants ueberprueft, ob sie einen
;; gueltigen Wert haben.
;;------------------------------------------------------------------------------
(defun bind-forward-ref-fun (name comp-macro-operator)
  (let ((const (make-instance 'defined-named-const
                              :symbol name
                              :value :forward)))
    (if comp-macro-operator
        (setf (cdr comp-macro-operator) (cons :FORWARD const))
        (if (consp name)
            (set-forward-setf-fun name const)
            (set-forward-fun name const)))
    (add-q const (?named-const-list *module*))
    const))

;;------------------------------------------------------------------------------
;; Analysiert die Argumente eines Funktionsaufrufes.
;;------------------------------------------------------------------------------
(defun p1-args (args)
  (mapcar #'p1-form args))

;;------------------------------------------------------------------------------
;; Analysiert einen lesenden Variablenzugriff
;; Resultat : Darstellung der Variablen in der abstrakten Syntax
;;------------------------------------------------------------------------------
(defun p1-read-access-of-var (var)
  (if (keywordp var)
     (p1-form `(L::QUOTE ,var))
     (let ((var-bind (get-var-bind var)))
          (make-instance 'var-ref :var var-bind))))

;;------------------------------------------------------------------------------
;; Ermittelt die abstrakte Syntax des Symbols. Erzeugt, falls noetig,
;; einen neuen Eintrag in der Symboltabelle.
;;------------------------------------------------------------------------------
(defun p1-make-symbol (symbol)
  (cond ((null symbol)
         (clicc-warning "NIL is used as symbol.")
         empty-list)
        (T (let ((sym (get-symbol-bind symbol)))
             (if sym
                 sym
                 (p1-make-new-symbol symbol))))))

;;------------------------------------------------------------------------------
;; Ermittelt die abstrakte Syntax des Symbols.
;; Erzeugt einen neuen Eintrag in der Symboltabelle.
;;------------------------------------------------------------------------------
(defun p1-make-new-symbol (symbol)
  (cond
    (*compiler-eval* (make-instance 'sym :symbol symbol))
    (t  
     (let ((sym
            (make-instance
             'defined-sym
             :name (symbol-name symbol)
             :package
             (if (not (symbol-package symbol))
                 :uninterned
                 (if (or (eq (symbol-package symbol) *lisp-package*)
                      
                         ;; Auch Symbole, die aus dem Wirst-Lisp-Package in das
                         ;; CLICC-Lisp Package importiert wurden, werden so
                         ;; behandelt, als waere ihr Home Package das
                         ;; CLICC-LISP-Package
                         ;;-------------------                      
                         (eq symbol (find-symbol (symbol-name symbol)
                                                 *lisp-package*)))
                     "LISP"
                     (package-name (symbol-package symbol))))
             :symbol symbol)))
       (when (eq (symbol-package symbol) *keyword-package*)
         (setf (?exported sym) t)
         (setf (?constant-value sym) sym))
       (bind-symbol sym)
       (add-q sym (?sym-list *module*))
       sym))))

;;------------------------------------------------------------------------------
;; Analysiert einen konstanten Ausdruck und stellt strukturierte
;; konstante Ausdruecke durch eine Struktur vom Typ structured-literal dar.
;;------------------------------------------------------------------------------
(defun p1-constant (object)
  (let ((array-in-const nil)
        (float-in-const nil))
    (labels
        ((in-constant (object)
           (typecase object
    
             (cons (cons (in-constant (car object))
                         (in-constant (cdr object))))
    
             (null   nil)

             (symbol (p1-make-symbol object))
    
             (number (typecase object
                       (integer object)
                       (float   (setq float-in-const t) object)
                       (t       (clicc-error TYPE_NOT_IMPL object))))

             (character object)

             (string object)

             (vector (dotimes (i (length object))
                       (setf (elt object i)
                             (in-constant (elt object i))))
                     object)
             (array (update-array #'in-constant object)
                    (setq array-in-const object)
                    object)
             
             (literal-instance (setf (?class object)
                                     (p1-make-symbol (?class object)))
                               (setf (?value-list object)
                                     (mapcar #'in-constant
                                             (?value-list object)))
                               object)

             (function (clicc-error "Function object not allowed as constant"))
             (t (error "Unknown atom ~A" object)))))
    
      (typecase object
        (null   empty-list)
        (number (typecase object
                  (integer (make-instance 'int :value object))
                  (float   (make-instance 'float-form :value object))
                  (t       (clicc-error TYPE_NOT_IMPL object))))
        (character (make-instance 'character-form :value object))
          
        ((or cons array literal-instance)
         (make-instance 'structured-literal
                        :value (in-constant object)
                        :needs-fixnum-array array-in-const
                        :needs-float-array float-in-const))
        (t (in-constant object))))))


;;------------------------------------------------------------------------------
;; DEFCONSTANT name initial-value [documentation]
;;------------------------------------------------------------------------------
(defun p1-defconstant (name_initial-value_documentation)
  (let (name initial-value documentation)
    (tagbody
       (setq name
             (if (atom name_initial-value_documentation)
               (go no-match)
               (pop name_initial-value_documentation)))
       (clicc-message "Analyse DEFCONSTANT  ~A" name)
       (setq initial-value
             (if (atom name_initial-value_documentation)
               (go no-match)
               (pop name_initial-value_documentation)))
       (setq documentation
             (if (atom name_initial-value_documentation)
               ""
               (pop name_initial-value_documentation)))
       (when (null name_initial-value_documentation) (go end))
       
     NO-MATCH
       (clicc-error ILLEGAL_CALL
                    "(NAME INITIAL-VALUE &OPTIONAL (DOC-STRING NIL DOCP))"
                    "DEFCONSTANT")
     END)
    (unless (symbolp name)          (clicc-error NO_SYMBOL name))
    (unless (stringp documentation) (clicc-error NO_STRING documentation))
    (when (lex-var-name-p name)
      (clicc-error
       "defconstant: ~a has been used as lexical variable before" name))
    (when (let ((dyn-bind (find-global-dynamic name)))
            (and dyn-bind (plusp (?write (cdr dyn-bind)))))
      (clicc-error
       "defconstant: ~a has been bound or assigned a value to before" name))
    (p1-make-constant name initial-value)))

;;------------------------------------------------------------------------------
;; Deklariert eine Konstante 'name' mit Wert 'initial-value'.
;; Der Wert kann evtl. schon zur Uebersetzungszeit bestimmt werden und wird dann
;; dem Symbol zugeordnet. Ansonsten wird das Symbol im Attribut ?constant-value
;; mit dem ausgezeichneten Wert :unknown gekennzeichnet, der ausdrueckt,
;; dass der Wert des Symbols nach seiner Initialisierung zur Laufzeit nicht
;; veraendert werden darf.
;;------------------------------------------------------------------------------
(defun p1-make-constant (name initial-value)
  (let ((sym (p1-make-symbol name)))
    
    ;; Wurde das Symbol bereits mit DEFCONSTANT deklariert ?
    ;;-----------------------------------------------------
    (unless (eq :no-const (?constant-value sym))
      (clicc-warning "~S, being defined as constant ~S, has global value ~S"
                     name initial-value (?constant-value sym)))
    
    ;; Konstanten werden als SPECIAL proklamiert.  Warum ist das notwendig,
    ;; wenn sowieso keine Bindung vorgenommen werden darf ?
    ;;-----------------------------------------------------
    (p1-top-level-form
     `(L::PROCLAIM (L::QUOTE (L::SPECIAL ,name))))
    
    ;; Es wird versucht, 'initial-value' auszuwerten.
    ;;----------------------------------------------
    (multiple-value-bind (evaluated-initial-value evaluated-p)
        (p1-eval initial-value)
      (cond
        (evaluated-p (setf (?constant-value sym)
                           (p1-constant evaluated-initial-value)))
        
        ;; Hier muss zuerst die Zuweisung analysiert werden,
        ;; bevor das Symbol als Konstante markiert wird.
        ;; Sonst handelt es sich um eine nicht erlaubte
        ;; Zuweisung an eine Konstante.
        ;;--------------------------------------------------
        (T (p1-top-level-form `(L::SETQ ,name ,initial-value))
           (setf (?constant-value sym) :unknown))))))

;;------------------------------------------------------------------------------
(defun p1-defconstant-p (symbol)
  (let ((sym (get-symbol-bind symbol)))
    (and sym (not (eq :no-const (?constant-value sym))))))

;;------------------------------------------------------------------------------
;; DEFPARAMETER name initial-value [documentation]
;;------------------------------------------------------------------------------
(defun p1-defparameter (name_initial-value_documentation)
  (let (name initial-value documentation)
    (tagbody
       (setq name
             (if (atom name_initial-value_documentation)
               (go no-match)
               (pop name_initial-value_documentation)))
       (clicc-message "Analyse DEFPARAMETER ~A" name)
       (setq initial-value
             (if (atom name_initial-value_documentation)
               (go no-match)
               (pop name_initial-value_documentation)))
       (setq documentation
             (if (atom name_initial-value_documentation)
               ""
               (pop name_initial-value_documentation)))
       (when (null name_initial-value_documentation) (go end))
       
     NO-MATCH
       (clicc-error ILLEGAL_CALL
                    "(NAME INITIAL-VALUE &OPTIONAL (DOC-STRING NIL DOCP))"
                    "DEFPARAMETER")
     END)
    (unless (symbolp name)          (clicc-error NO_SYMBOL name))
    (unless (stringp documentation) (clicc-error NO_STRING documentation))
    
    (p1-top-level-form
     `(L::PROGN
       (L::PROCLAIM (L::QUOTE (L::SPECIAL ,name)))

       ;; SET setzt auch dann die globale Variable, wenn das DEFPARAMETER
       ;; in einer nicht leeren lexikalischen Umgebung aufgerufen wird.
       ;;--------------------------------------------------------------
       (L::SET (L::QUOTE ,name) ,initial-value)))))

;;------------------------------------------------------------------------------
;; DEFVAR name [initial-value [documentation]]
;;------------------------------------------------------------------------------
(defun p1-defvar (name_initial-value_documentation)
  (let (name initial-value documentation init-val-sp)
    (tagbody
       (setq name
             (if (atom name_initial-value_documentation)
               (go NO-MATCH)
               (pop name_initial-value_documentation)))
       (unless *SDF*
         (clicc-message "Analyse DEFVAR       ~A" name))
       (setq initial-value
             (if (atom name_initial-value_documentation)
               NIL
               (progn
                 (setq init-val-sp t)
                 (pop name_initial-value_documentation))))
       (setq documentation
             (if (atom name_initial-value_documentation)
               ""
               (pop name_initial-value_documentation)))
       (when (null name_initial-value_documentation) (go END))
       
     NO-MATCH
       (clicc-error ILLEGAL_CALL
                    "(NAME &OPTIONAL INITIAL-VALUE (DOC-STRING NIL DOCP))"
                    "DEFVAR")
     END)
    (unless (symbolp name)          (clicc-error NO_SYMBOL name))
    (unless (stringp documentation) (clicc-error NO_STRING documentation))
    
    (p1-top-level-form
     `(L::PROGN
       (L::PROCLAIM (L::QUOTE (L::SPECIAL ,name)))
       ,@(when init-val-sp
           `((L::UNLESS (L::BOUNDP (L::QUOTE ,name))

               ;; SET setzt auch dann die globale Variable, wenn das DEFVAR
               ;; in einer nicht leeren lexikalischen Umgebung aufgerufen wird.
               ;;--------------------------------------------------------------
               (L::SET (L::QUOTE ,name) ,initial-value))))))))

;;------------------------------------------------------------------------------
(defvar *secret-package-name* "1")
(defvar *counter* 1)

(defun find-secret-package-name ()
  (loop
   (if (find-package *secret-package-name*)
       (setq *secret-package-name* (format nil "$%~a%$" (incf *counter*)))
       (return))))

;;------------------------------------------------------------------------------
;; Fuegt Nicknames zu einem Package hinzu.
;;------------------------------------------------------------------------------
(defun add-nicknames (package new-nicknames)
  (let ((name (package-name package))
        (nicknames (union (package-nicknames package) new-nicknames)))
    (rename-package package (find-secret-package-name))
    (rename-package package name nicknames)))

;;------------------------------------------------------------------------------
;; IN-PACKAGE package-name &KEY :nicknames :use
;;------------------------------------------------------------------------------
(defun p1-in-package (name_rest)
  (apply #'(lambda (name &key (nicknames ()) (use '() supplied))               
             (let* ((package (or (find-package name)
                                (make-package name :use '())))
                   new-nicknames
                   new-use)

               ;; 1. IN-PACKAGE: wenn kein :use, dann Default = :use '("LISP")
               ;;-------------------------------------------------------------
               (unless (member package (queue2list (?package-list *module*)))
                 (add-q *PACKAGE* (?package-list *module*))
                 (unless supplied
                   (setq use '("LISP"))))
               
               (setq new-nicknames
                     (set-difference nicknames
                                     (package-nicknames package)
                                     :test #'string=))
               (setq new-use
                     (set-difference use 
                                     (mapcar #'package-name
                                             (package-use-list package))
                                     :test #'string=))
               (when new-nicknames (add-nicknames package new-nicknames))
               (when new-use (use-package use package))
    
               ;; Die IN-PACKAGE Anweisung muss vor der ersten Anweisung
               ;; erfolgen. Innerhalb eines Moduls darf wiederholt IN-PACKAGE
               ;; auf das gleiche Modul angewendet werden.
               ;;-----------------------------------------
               (cond
                 ((not (slot-boundp *module* 'package))
                  (setf (?package *module*) package))
                 ((eq (?package *module*) package) nil)
                 (t
                  (when (and (?toplevel-forms *GLOBAL-ENVIRONMENT*)
                             *MODULE-COMPILER*)
                    (clc-error "changing package to ~a is not allowed while ~
                                compiling Module ~a"
                               (package-name package)
                               (package-name (?package *module*))))
                  (setf (?package *module*) package)))

               ;; Der Aufruf von IN-PACKAGE muss evtl. auch im 
               ;; uebersetzten Programm vorkommen.
               ;; Nur Code erzeugen, wenn sich gegenueber dem letzten
               ;; IN-PACKAGE etwas geaendert hat.
               ;;--------------------------------
               (when (or (not (eq package *PACKAGE*)) new-nicknames new-use)
                 (save-toplevel-form
                  (p1-form `(L::IN-PACKAGE . ,name_rest))))

               ;; Die folgenden READ Operationen des Compilers erfolgen in dem
               ;; angegebenen Package.
               ;;---------------------
               (setq *PACKAGE* package)))
               
         (mapcar #'p1-eval name_rest)))

;;------------------------------------------------------------------------------
;; USE-PACKAGE packages-to-use &OPTIONAL package 
;;------------------------------------------------------------------------------
(defun p1-use-package (args)
  (apply #'(lambda (packages-to-use &OPTIONAL package)
             (let ((current-package
                    (if (null package) *PACKAGE* (find-package package))))

               (when (null current-package)
                 (clicc-error "use-package: package ~s is undefined" package))
               (when (atom packages-to-use)
                 (setq packages-to-use (list packages-to-use)))

               (when (set-difference packages-to-use
                                     (mapcar #'package-name
                                             (package-use-list current-package))
                                     :test #'string=)
                 (use-package packages-to-use current-package))

               (save-toplevel-form (p1-form `(L::USE-PACKAGE . ,args)))))

         (mapcar #'p1-eval args)))

;;------------------------------------------------------------------------------
;; IMPORT symbols &OPTIONAL package
;;------------------------------------------------------------------------------
(defun p1-import (args)
  (apply #'(lambda (symbols &OPTIONAL package)
             (let ((current-package
                    (if (null package) *PACKAGE* (find-package package))))

               (when (null current-package)
                 (clicc-error "import: package ~s is undefined" package))
               (import symbols current-package)
               (save-toplevel-form (p1-form `(L::IMPORT . ,args)))))

         (mapcar #'p1-eval args)))

;;------------------------------------------------------------------------------
;; EXPORT {symbol || symbol-list} &OPTIONAL package
;; Bearbeitet Toplevel Aufrufe von export.
;; Traegt in die zugehoerigen Zwischensprachkonstrukte im Slot exported T ein.
;; Exportiert das Symbol im Lisp-Wirtssystem, wenn es noch nicht exportiert war.
;; Das verhindert, dass auf Symbole des Lisp-Packages export angewendet wird,
;; da das in neueren LISP-Systemen nicht erlaubt sein kann.
;;------------------------------------------------------------------------------
(defun p1-export (args)
  (let ((evaluated-args (mapcar #'p1-eval args)))
    (when (atom evaluated-args)
      (clicc-error ILLEGAL_CALL
                   "export"
                   "(symbols &optional package)"))
    (let ((symbols (pop evaluated-args))
          (package (if (p1-endp evaluated-args)
                       *PACKAGE*
                       (prog1 (pop evaluated-args)
                         (unless (p1-endp evaluated-args)
                           (clicc-error ILLEGAL_CALL
                                        "export"
                                        "(symbols &optional package)"))))))
      (unless (packagep package) (setq package (find-package package)))
      (unless (eq package *PACKAGE*)
        (unless *lisp-module*
          (clicc-error "Only Symbols in current package may be exported")))
      
      (unless (listp symbols)
        (setq symbols (list symbols)))
      (let ((symstoexport (empty-queue)))
        (dolist (symbol symbols)
          (if (symbolp symbol)
              (multiple-value-bind (symbol status)
                  (find-symbol (symbol-name symbol) package)
                (unless (eq :external status)
                  (export (if (null symbol) (list symbol) symbol) package))
                (if (not (null symbol))
                    (let ((sym (p1-make-symbol symbol)))
                      (setf (?exported sym) t)
                      (when (imported-sym-p sym)
                        (add-q symbol symstoexport)))
                    (add-q symbol symstoexport)))
              (clc-error "~s can not be exported" symbol)))
        (unless (empty-queue-p symstoexport)
          (save-toplevel-form
           (p1-form `(L::EXPORT  ',(queue2list symstoexport)))))))))

;;------------------------------------------------------------------------------
;; LOAD filename &KEY :verbose :print :if-does-not-exist
;;------------------------------------------------------------------------------
(defun p1-load (args)
  (labels ((gen-init-call (imported-module)
             (save-toplevel-form
              (make-app 
               :form (make-imported-fun
                      :symbol (make-symbol (?name imported-module))
                      :adr (?init-fun-name imported-module)
                      :par-spec 0
                      :write-list -1)   ; has side effect
               :arg-list ())))

           (gen-reset-package ()
             (save-toplevel-form
              (p1-form
               `(L::SETQ L::*PACKAGE*
                 (L::FIND-PACKAGE ,(package-name *PACKAGE*)))))))

    (let ((evaluated-args (mapcar #'p1-eval args)))
      (when (atom evaluated-args)
        (clicc-error ILLEGAL_CALL
                     "LOAD"
                     "(FILENAME &KEY :VERBOSE :PRINT :IF-DOES-NOT-EXIST)"))
      (let* ((pathname (merge-pathnames (pop evaluated-args))))
                                        ; Schluesselworte ignorieren
        (cond
          ((is-module-import pathname)  ; import
           (clicc-message "Importing definitions from module ~A" pathname)
           (let ((imported-module (import-read pathname)))
             (syntax-import pathname) 
             (gen-init-call imported-module)
           (gen-reset-package)))
          (T                            ; include
           (pass_1-of-file pathname))))))) 

;;------------------------------------------------------------------------------
;; PROVIDE module-name
;;------------------------------------------------------------------------------
(defun p1-provide (args)
  (let ((evaluated-args (mapcar #'p1-eval args)))
    (when (/= (length evaluated-args) 1)
      (clicc-error ILLEGAL_CALL "PROVIDE" "(MODULE-NAME)"))
    (let ((module-name (string (pop evaluated-args))))
      (pushnew module-name *CLICC-REQUIRED* :test #'string=)))) 

;;------------------------------------------------------------------------------
;; REQUIRE module-name &OPTIONAL pathname
;;------------------------------------------------------------------------------
(defun p1-require (args)
  (let ((evaluated-args (mapcar #'p1-eval args)))
    (when (atom evaluated-args)
      (clicc-error ILLEGAL_CALL
                   "REQUIRE" "(MODULE-NAME &OPTIONAL PATHNAME)"))
    (let ((module-name (string (pop evaluated-args))))
      (unless (member module-name *CLICC-REQUIRED* :test #'string=)
        (p1-top-level-form `(L::LOAD ,module-name))))))

;;------------------------------------------------------------------------------
(provide "p1tlf")
