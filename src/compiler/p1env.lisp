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
;;; Funktion : Environments
;;;
;;; $Revision: 1.58 $
;;; $Id: p1env.lisp,v 1.58 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Die globale Umgebung:
;;------------------------------------------------------------------------------

(defvar *GLOBAL-ENVIRONMENT*)
(defvar *DEFAULT-GLOBAL-ENVIRONMENT*)

;;------------------------------------------------------------------------------
;; Dynamische Variable. Gibt an, wenn ein Lisp-Ausdruck nur deshalb in die
;; Zwischensprache transformiert wird, um spaeter interpretiert zu werden.
;; Wenn sie den Wert T hat, aendert sich das Verhalten von p1-make-new-symbol.
;;------------------------------------------------------------------------------
(defvar *COMPILER-EVAL* nil)

;;------------------------------------------------------------------------------
(defclass1 class-entry ()               ;Klassen-Eintrag im types-Slot
                                        ;der Klasse global-env
  class-def                             ;Instanz von class-def
  (forward :type bool)                  ;T=forwarded, NIL=defined
  (finalized :type bool)                ;T=finalized, NIL=not finalized
  (reader :type list)                   ;A-Liste: Slot-Name . Reader-Funs
  (writer :type list))                  ;A-Liste: Slot-Name . Writer-Funs

;;------------------------------------------------------------------------------
(defclass1 global-env ()
  
  ;; A-Liste
  ;; Schluessel: Symbol
  ;; Wert: Cons-Paar X
  ;; (:SPECIAL-FORM . Transformationsfunktion in die Zwischensprache)
  ;; :TOP-LEVEL noch nicht implementiert
  ;; (:SYS-MACRO . Transformationsfunktion in die Quellsprache)
  ;; (:USER-MACRO . defined-fun) Die Funktion wird zur Uebersetzungszeit
  ;;                             interpretiert und transformiert einen
  ;;                             Quellsprachenausdruck in die Quellsprache
  ;; (:IMPORTED-FUN . imported-fun)
  ;; (:GLOBAL-FUN . global-fun)
  ;; (:FORWARD . defined-named-const)
  ;; (:GENERIC-FUN . generic-fun)
  ;; (:FOREIGN-FUN . foreign-fun)
  ;; oder
  ;; (:COMPILER-MACRO . (expander . X))
  ;;-----------------------------
  (operators     :initform () :type list)

  ;; A-Liste
  ;; Schluessel: Symbol
  ;; Wert: dynamic
  ;; Alle definierenden und angewandten Vorkommen von dynamischen Variablen
  ;; mit gleichem Namen werden durch genau ein Zwischensprachkonstrukt
  ;; repraesentiert.
  ;;----------------
  (dyn-vars      :initform () :type list)

  ;; Liste mit Elementen vom typ sym.
  ;; Die Komponente symbol von sym dient als Schluessel
  ;;---------------------------------------------------
  (syms          :initform () :type list)

  ;; A-Liste
  ;; Schluessel: Symbol
  ;; Wert: Cons-Paar
  ;; (:TYPE              . type-expander)
  ;; (:CLASS             . Instanz von class-entry)
  ;;                       An dieser Stelle kann keine Instanz von defined-class
  ;;                       benutzt werden, da zusaetzlich Marker sowie die Namen
  ;;                       der Zugriffsfunktionen hier untergebracht sind.
  ;; (:BUILT-IN          . Instanz von built-in-class-def)
  ;; (:STRUCT            . Instanz von structure-class-def)
  ;;-----------------------------------
  (types         :initform () :type list)

  ;; A-Liste
  ;; Schluessel: Symbol
  ;; Wert: Name des foreign-type
  ;;----------
  (fftypes :initform () :type list)

  ;; A-Liste
  ;; Schlussel: Symbol
  ;; Wert: call-in-fun
  ;;------------------
  (call-in-funs :initform () :type list)

  ;; A-Liste
  ;; Schluessel: Symbol
  ;; Wert: ???
  ;;----------
  (structures    :initform () :type list)

  ;; A-Liste
  ;; Schluessel: Symbol
  ;; Wert: Cons-Paar
  ;; (:COMPLEX-SETF-METHOD .  Transformationsfunktion in die Quellsprache)
  ;; (:SIMPLE-SETF-METHOD . defined-fun)
  ;;                             Die Funktion wird zur Uebersetzungszeit
  ;;                             interpretiert und transformiert einen
  ;;                             Quellsprachenausdruck in die Quellsprache
  ;;----------------------------------------------------------------------
  (setf-methods  :initform () :type list)
  
  ;; A-Liste
  ;; Schluessel: Symbol
  ;; Wert: Cons-Paar
  ;; (:FORWARD . defined-named-const)
  ;; (:IMPORTED-FUN . imported-fun)
  ;; (:GLOBAL-FUN . global-fun)
  ;; (:GENERIC-FUN . generic-fun)
  ;;----------------------------------------------------------------------
  (setf-functions :initform () :type list)

  ;; Liste der Toplevel-Forms des Moduls in Zwischensprachdarstellung
  ;;-----------------------------------------------------------------
  (toplevel-forms :initform () :type list)

  (special-decls :initform () :type list)

  (ignore-decls :initform () :type list)

  ;; Kontextbedingung: Wenn eine globale SPECIAL-Deklaration existiert,
  ;; duerfen keine lokalen Bindungen gleichen Namens vorher existieren.
  ;; In 'lex-vars' werden daher die Namen saemtlicher lexikalischer Variablen
  ;; abgespeichert.
  ;;---------------
  (lex-vars :initform () :type list))

;;------------------------------------------------------------------------------
(defun copy-global-env (old)
  (make-instance 'global-env
    :operators (?operators old)
    :syms (?syms old)
    :special-decls (?special-decls old)
    :types (?types old)
    :fftypes (?fftypes old)
    :structures (?structures old)
    :setf-methods (?setf-methods old)
    :setf-functions (?setf-functions old)
    :toplevel-forms (?toplevel-forms old)
    :ignore-decls (?ignore-decls old)
    :lex-vars (?lex-vars old)
    :dyn-vars (?dyn-vars old)))

;;------------------------------------------------------------------------------
(defvar saved-sym-consts)
  
(defun save-sym-status ()
  (setq saved-sym-consts
        (mapcar #'?constant-value (?syms *GLOBAL-ENVIRONMENT*))))

(defun reset-sym-status ()
  (mapc #'(lambda (sym val)
            (setf (?used sym) nil)
            (setf (?constant-value sym) val))
        (?syms *GLOBAL-ENVIRONMENT*)
        saved-sym-consts))

;;------------------------------------------------------------------------------
(defmacro in-compile-time-env (&body forms)
  `(let ((*compiler-eval* t))
    ,@forms))  
  
;;------------------------------------------------------------------------------
;; Operators: Special Forms, globale Makros, globale Funktionen
;;------------------------------------------------------------------------------
(defmacro set-global-op (name type op)
  `(push (cons ,name (cons ,type ,op)) (?operators *GLOBAL-ENVIRONMENT*)))

(defmacro set-special-form (name expander)
  `(set-global-op ,name :SPECIAL-FORM ,expander))

(defmacro set-top-level (name expander)
  `(set-global-op ,name :TOP-LEVEL ,expander))

(defmacro set-sys-macro (name expander)
  `(set-global-op ,name :SYS-MACRO ,expander))

(defmacro set-macro (name expander)
  `(set-global-op ,name :USER-MACRO ,expander))

(defmacro set-sys-fun (name fun)
  `(set-global-op ,name :IMPORTED-FUN ,fun))

(defmacro set-imported-fun (name fun)
  `(set-global-op ,name :IMPORTED-FUN ,fun))

(defmacro set-global-fun (name fun)
  `(set-global-op ,name :GLOBAL-FUN ,fun))

(defmacro set-forward-fun (name fun)
  `(set-global-op ,name :FORWARD ,fun))

(defmacro set-generic-fun (name fun)
  `(set-global-op ,name :GENERIC-FUN ,fun))

(defmacro set-foreign-fun (name fun)
  `(set-global-op ,name :FOREIGN-FUN ,fun))

(defmacro set-compiler-macro (name fun)
  `(let ((entry (assoc ,name (?operators *GLOBAL-ENVIRONMENT*))))
    (if entry
        
        ;; Bestehenden Eintrag (name . (:typ . op)) zu
        ;; (name . (:COMPILER-MACRO . (op' . (:typ . op)))) erweitern.
        ;;------------------------------------------------------------
        (setf (cdr entry) (cons :COMPILER-MACRO (cons ,fun (cdr entry))))

        ;; Neuen Eintrag (name . (:COMPILER-MACRO . (op' . nil)))
        ;;-------------------------------------------------------
        (set-global-op ,name :COMPILER-MACRO (list ,fun)))))

(defmacro get-global-op (name)
  `(cdr (assoc ,name (?operators *GLOBAL-ENVIRONMENT*) :test #'eq)))

(defmacro get-global-fun (name)
  `(let ((op (get-global-op ,name)))
    (cdr (if (eq :COMPILER-MACRO (car op)) (cddr op)
             op))))

(defun redef-op-error (operator-key name)
  (clicc-error "It is illegal to redefine the ~a ~S"
               (ecase operator-key
                 (:SPECIAL-FORM "special form")
                 ((:SYS-MACRO :USER-MACRO) "macro")
                 ((:GLOBAL-FUN :LOCAL-FUN :IMPORTED-FUN :COMPILER-MACRO)
                  "function")
                 (:GENERIC-FUN "generic function")
                 (:FOREIGN-FUN "foreign function"))
               name))

(defun op-as-fun-error (operator-key name)
  (clicc-error "It is illegal to use the ~a ~s as a functional object"
               (ecase operator-key
                 (:SPECIAL-FORM "special form")
                 ((:SYS-MACRO :USER-MACRO :LOCAL-MACRO) "macro")
                 (:FOREIGN-FUN "foreign function"))
               name))

;;------------------------------------------------------------------------------
;; SETF-Functions
;;------------------------------------------------------------------------------
(defmacro set-global-setf-fun-def (name type function)
  `(push (cons (second ,name) (cons ,type ,function))
         (?setf-functions *GLOBAL-ENVIRONMENT*)))

(defmacro set-forward-setf-fun (name const)
  `(set-global-setf-fun-def ,name :FORWARD ,const))

(defmacro set-imported-setf-fun (name fun)
  `(set-global-setf-fun-def ,name :IMPORTED-FUN ,fun))

(defmacro get-global-setf-fun-def (name)
  `(cdr (assoc (second ,name) (?setf-functions *GLOBAL-ENVIRONMENT*)
         :test #'eq)))

(defmacro get-global-setf-fun (name)
  `(cdr (get-global-setf-fun-def ,name)))

;;------------------------------------------------------------------------------
;; Names of previous lexical variables
;;------------------------------------------------------------------------------
(defmacro save-lex-var-name (symbol)
  `(let ((symbol ,symbol))
    (when (symbol-package symbol)
      (pushnew symbol (?lex-vars *GLOBAL-ENVIRONMENT*)))))

(defmacro lex-var-name-p (symbol)
  `(member ,symbol (?lex-vars *GLOBAL-ENVIRONMENT*)))

;;------------------------------------------------------------------------------
;; Global Symbols
;;------------------------------------------------------------------------------
(defmacro bind-symbol (sym)
  `(push ,sym (?syms *GLOBAL-ENVIRONMENT*)))

;;------------------------------------------------------------------------------
(defun get-symbol-bind (symbol)
  (dolist (sym (?syms *GLOBAL-ENVIRONMENT*))
    (when (eq symbol (?symbol sym)) (return sym))))

;;------------------------------------------------------------------------------
;; Dynamic Variables
;;------------------------------------------------------------------------------
(defun find-global-dynamic (name)
  (assoc name (?dyn-vars *GLOBAL-ENVIRONMENT*) :test #'eq))

(defun get-global-dynamic (name)
  (let ((bind (find-global-dynamic name)))
    (if bind
        (cdr bind)
        (let ((var (make-instance 'dynamic
                                  :sym (p1-make-symbol name))))
          (unless *compiler-eval*
            (push (cons name var)
                  (?dyn-vars *GLOBAL-ENVIRONMENT*)))
          var))))
                
;;------------------------------------------------------------------------------
;; Type Specifiers
;;------------------------------------------------------------------------------
(defmacro set-type-def (type type-expander)
  `(push (cons ,type (cons :TYPE ,type-expander))
    (?types *GLOBAL-ENVIRONMENT*)))

(defmacro get-type-def (type)
  `(cdr (assoc ,type (?types *GLOBAL-ENVIRONMENT*) :test #'eq)))

;;------------------------------------------------------------------------------
;; Classes
;;------------------------------------------------------------------------------
(defmacro set-global-class-entry (name class forward finalized reader writer)
  `(push (cons ,name (cons :CLASS (make-instance 'class-entry 
                                                  :class-def ,class
                                                  :forward ,forward
                                                  :finalized ,finalized
                                                  :reader ,reader
                                                  :writer ,writer)))
    (?types *GLOBAL-ENVIRONMENT*)))

(defmacro set-class-entry (name class reader writer)
  `(set-global-class-entry ,name ,class nil t ,reader ,writer))

(defmacro set-forward-class-entry (name class)
  `(set-global-class-entry ,name ,class t nil nil nil))

(defmacro set-unfinalized-class-entry (name class reader writer)
  `(set-global-class-entry ,name ,class nil nil ,reader ,writer))

(defun get-class-entry (name)
  (cdr (assoc name (?types *GLOBAL-ENVIRONMENT*) :test #'eq)))

(defmacro set-built-in (name class)
  `(push (cons ,name (cons :BUILT-IN ,class)) (?types *GLOBAL-ENVIRONMENT*)))

(defmacro get-built-in (name)
  `(cdr (assoc ,name (?types *GLOBAL-ENVIRONMENT*) :test #'eq)))

;;------------------------------------------------------------------------------
;; Foreign Types
;;------------------------------------------------------------------------------
(defmacro set-fftype (name fftype)
  `(push (cons ,name ,fftype) (?fftypes *GLOBAL-ENVIRONMENT*)))

(defmacro get-fftype (name)
  `(cdr (assoc ,name (?fftypes *GLOBAL-ENVIRONMENT*) :test #'eq)))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defmacro set-call-in-fun (name fun)
  `(push (cons ,name ,fun) (?call-in-funs *GLOBAL-ENVIRONMENT*)))

(defmacro get-call-in-fun (name)
  `(cdr (assoc ,name (?call-in-funs *GLOBAL-ENVIRONMENT*) :test #'eq)))

;;------------------------------------------------------------------------------
;; Generalized Variables
;;------------------------------------------------------------------------------
(defmacro set-setf-method-def (name function setf-method-type)
  `(push (cons ,name (cons ,setf-method-type ,function))
    (?setf-methods *GLOBAL-ENVIRONMENT*)))

(defmacro get-setf-method-def (name)
  `(cdr (assoc ,name (?setf-methods *GLOBAL-ENVIRONMENT*) :test #'eq)))

;;------------------------------------------------------------------------------
;; Structures
;;------------------------------------------------------------------------------
(defmacro set-struct-def (name struct)
  `(push (cons ,name ,struct) (?structures *GLOBAL-ENVIRONMENT*)))
 
(defmacro get-struct-def (name)
  `(cdr (assoc ,name (?structures *GLOBAL-ENVIRONMENT*) :test #'eq)))

;;------------------------------------------------------------------------------
;; Toplevel Forms
;;------------------------------------------------------------------------------
(defmacro save-toplevel-form (form)
  `(push ,form (?toplevel-forms *GLOBAL-ENVIRONMENT*)))

(defmacro get-toplevel-form-list ()
  `(reverse (?toplevel-forms *GLOBAL-ENVIRONMENT*)))

;;------------------------------------------------------------------------------
;; Die lexikalische (oder lokale) Umgebung:
;;------------------------------------------------------------------------------
(defclass1 env ()
  (funs         :initform () :type list)
  (setf-funs    :initform () :type list)
  (vars         :initform () :type list)
  (blocks       :initform () :type list)
  (tags         :initform () :type list)
  (ignore-decls :initform () :type list))

;;------------------------------------------------------------------------------
(defun copy-env (old)
  (make-instance 'env
    :funs (?funs old)
    :setf-funs (?setf-funs old)
    :vars (?vars old)
    :blocks (?blocks old)
    :tags (?tags old)
    :ignore-decls (?ignore-decls old)))

(defvar *LOCAL-ENVIRONMENT*)

;;------------------------------------------------------------------------------
(defun get-operator-def (name)
  (let ((operator-def (cdr (assoc name (?funs *LOCAL-ENVIRONMENT*) :test #'eq))))
    (if operator-def
      operator-def
      (get-global-op name))))

(defun get-setf-fun-def (name)
  (let ((setf-fun-def (cdr (assoc (second name) 
                                  (?setf-funs *LOCAL-ENVIRONMENT*)
                                  :test #'eq))))
    (if setf-fun-def
        setf-fun-def
      (get-global-setf-fun-def name))))

;;------------------------------------------------------------------------------
;; Find the function which is denoted by a extended function designator
;;------------------------------------------------------------------------------
(defun name2fun (name)
  (let* ((operator-def (if (consp name)
                           (get-setf-fun-def name)
                           (get-operator-def name))))

    (when (eq :COMPILER-MACRO (car operator-def))
      (setq operator-def (cddr operator-def)))
    (case (car operator-def)
      ((:IMPORTED-FUN :GLOBAL-FUN :LOCAL-FUN) (cdr operator-def))
      (t nil))))

;;------------------------------------------------------------------------------
;; Funktionsbindungen fuer LABELS/FLET
;;------------------------------------------------------------------------------
(defun bind-local-fun (name fun)
  (if (consp name)
      (push (cons name (cons :LOCAL-SETF-FUN fun)) 
            (?setf-funs *LOCAL-ENVIRONMENT*))
    (push (cons name (cons :LOCAL-FUN fun)) (?funs *LOCAL-ENVIRONMENT*))))

;;------------------------------------------------------------------------------
;; Funktionsbindungen fuer MACROLET
;;------------------------------------------------------------------------------
(defun bind-local-macro (name fun)
  (push (cons name (cons :LOCAL-MACRO fun)) (?funs *LOCAL-ENVIRONMENT*)))

;;------------------------------------------------------------------------------
;; Variablen-Bindungen
;;------------------------------------------------------------------------------
(defmacro bind-var (name var)
  `(push (cons ,name ,var) (?vars *LOCAL-ENVIRONMENT*)))

;;------------------------------------------------------------------------------
(defun get-var-bind (name)
  (labels ((special-symbol-name-p (s)
             (let ((name (symbol-name s)))
               (and (eql (char name 0) #\*)
                    (eql (char name (1- (length name))) #\*)))))
    (let ((var-bind (assoc name (?vars *LOCAL-ENVIRONMENT*) :test #'eq)))
      (cond
        (var-bind (cdr var-bind))
        (t (unless (p1-proclaimed-special-p name)
             (when (or *warn-unbound-vars* (not (special-symbol-name-p name)))
               (clicc-warning "Assuming ~20s is global variable." name)))

           (get-global-dynamic name))))))

;;------------------------------------------------------------------------------
;; BLOCK-Bindungen fuer BLOCK/RETURN-FROM
;;------------------------------------------------------------------------------
(defmacro bind-block (name cont)
  `(push (cons ,name ,cont) (?blocks *LOCAL-ENVIRONMENT*)))

(defun get-block-bind (name)
  (let ((block-bind (assoc name (?blocks *LOCAL-ENVIRONMENT*) :test #'eq)))
    (if block-bind
      (cdr block-bind)
      (clicc-error "No BLOCK named ~S" name))))

;;------------------------------------------------------------------------------
;; TAG-Bindungen fuer TAGBODY/GO
;;------------------------------------------------------------------------------
(defmacro bind-tag (tag tagged-form)
  `(push (cons ,tag ,tagged-form) (?tags *LOCAL-ENVIRONMENT*)))

(defun get-tag-bind (tag)
  (let ((tag-bind (assoc tag (?tags *LOCAL-ENVIRONMENT*) :test #'eql)))
    (if tag-bind
      (cdr tag-bind)
      (clicc-error "There is no TAG for (go ~S)" tag))))

;;------------------------------------------------------------------------------
(provide "p1env")
