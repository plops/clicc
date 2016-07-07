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
;;; Funktion : Initialisierung des Compilers,
;;;            Initialisierung des Uebersetzungsumgebung.
;;;
;;; $Revision: 1.73 $
;;; $Id: p0init.lisp,v 1.73 1995/02/23 15:44:02 wg Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defun p0-init ()

  (setq *LOCAL-ENVIRONMENT* (make-instance 'env))
  (setq *keyword-package* (find-package "KEYWORD"))
  (setq *lisp-package* (find-package "LISP"))
  (setq *user-package* (find-package "USER"))
  (setq *runtime-package* (find-package "RT"))
  (setq *ffi-package* (find-package "FFI"))
  (setq *interface-file-queue* (empty-queue))
  (setq *ffi-include-flag* NIL)
  (setq *ffi-need-savestack* NIL)
  (setq *ffi-include-queue* (empty-queue))
  (setq *ffi-type-queue* (empty-queue))
  (setq *ffi-signatures* '())
  (setq *c-name-prefix* 0)

  (init-pass1)
  (setq *GLOBAL-ENVIRONMENT* (make-instance 'global-env))

  (clicc-message "Initialize Special Forms")
  (p0-init-special-form)

  (clicc-message "Initialize System Macros")
  (p0-init-sys-macro)
  (p0-init-defsetf)

  (clicc-message "Initialize System Functions")
  (opti-mark-special-funs)
  (p3-mark-special-funs)
  (sc-mark-special-funs)
  (ti-mark-special-funs)
  (weight-mark-special-funs)
  (cg-mark-special-funs)
  (p0-mark-special-sys-fun-with-mv)
  (import-read *LIB-SYS-PATH*)
  (unless *lisp-module*
    (clicc-message "Reading lisp.def")
    (import-read *LIB-LISP-PATH*))
  (opti-set-special-funs)
  (p3-set-special-funs)
  (sc-set-special-funs)
  (ti-set-special-funs)
  (weight-set-special-funs)
  (cg-set-special-funs)

  (p0-init-bq-funs)
  (p0-init-fftypes)

  (clicc-message "Initialize Evaluator")
  (init-zw-sym-fun-hash-table)

  (init-macro-error-funs)
  (p0-init-compiler-macro)
  (init-built-in-classes)
     
  (unless *lisp-module*
    (clicc-message "Reading lisp.syntax")
    (syntax-import *LIB-LISP-PATH*)))

;;------------------------------------------------------------------------------
;; Initialisieren der SETF-Methoden
;;------------------------------------------------------------------------------
(defun p0-init-defsetf ()
  (set-setf-method-def 'L::apply
                       #'define-setf-method-apply :COMPLEX-SETF-METHOD)
  (set-setf-method-def 'L::getf
                       #'define-setf-method-getf :COMPLEX-SETF-METHOD))

;;------------------------------------------------------------------------------
;; Initialisieren der System-Funktionen in der Compiler Umgebung
;;------------------------------------------------------------------------------
(defmacro p0-special-funs ((slot prefix) &rest names)
  (labels ((prefix-name (name) 
             (intern (concatenate 'string prefix "-" (string name))))
           (prefix-set-name (name) 
             (intern
              (concatenate 'string prefix "-SET-" (string (second name))))))
    `(progn
      
      (defun ,(prefix-name "MARK-SPECIAL-FUNS") ()
        (dolist (name ',names)
          (when (consp name) (setq name (second name))) ; (setf name)
          (setf (get name 'zws-type)
                #-CMU17 (find-class 'special-sys-fun)
                #+CMU17 'special-sys-fun)))

      (defun ,(prefix-name "SET-SPECIAL-FUNS") ()
        ,@(mapcar #'(lambda (name)
                      (if (consp name)
                          `(let ((fun (cdr (get-global-setf-fun-def ',name))))
                            (when fun
                              (setf (,slot fun)
                                    #',(prefix-set-name name))))
                          `(let ((fun (get-global-fun ',name)))
                            (when fun
                              (setf (,slot fun)
                                    #',(prefix-name name))))))
                  names)))))

(defun p0-mark-special-sys-fun-with-mv ()
  (setf (get 'values 'zws-type)
        #-CMU17 (find-class 'special-sys-fun-with-mv)
        #+CMU17 'special-sys-fun-with-mv))

;;------------------------------------------------------------------------------
;; Ermittelt die Symbole der von Back-Quote benutzten Funktionen
;;------------------------------------------------------------------------------
(defun p0-init-bq-funs ()
  (let ((bq-append (first (read-from-string "`(,@x ,@y)")))
        (bq-cons   (first (read-from-string "`(1 ,@x)")))
        (bq-list   (first (read-from-string "`(,x)")))
        (bq-list*  (first (read-from-string "`(1 2 ,@x)"))))

    (unless (eq bq-append 'L::append)
      (set-sys-fun bq-append (get-global-fun 'L::append)))
    (unless (eq bq-cons 'L::cons)
      (set-sys-fun bq-cons   (get-global-fun 'L::cons)))
    (unless (eq bq-list 'L::list)
      (set-sys-fun bq-list   (get-global-fun 'L::list)))
    (unless (eq bq-list* 'L::list*)
      (set-sys-fun bq-list*  (get-global-fun 'L::list*)))))

;;------------------------------------------------------------------------------
;; Anlegen der Klassen fuer vordefinierte Typen in der Globalen Umgebung
;; (vgl. [SteeleII] S 783)
;;------------------------------------------------------------------------------
(defun init-built-in-classes ()
 (set-class-entry 'L::T
                   (make-instance 'class-def
                                  :super-list '()
                                  :class-precedence-list '(L::T)
                                  :slot-descr-list '()
                                  :symbol 'L::T)
                   NIL NIL)
 (set-class-entry 'L::STANDARD-OBJECT
                   (make-instance 'class-def
                                  :super-list '(L::T)
                                  :class-precedence-list '(L::STANDARD-OBJECT L::T)
                                  :slot-descr-list '()
                                  :symbol 'L::STANDARD-OBJECT)
                   NIL NIL))

;;------------------------------------------------------------------------------
;; Vordefinieren einiger Typen.
;;------------------------------------------------------------------------------
(defun p0-init-fftypes ())
  
;;------------------------------------------------------------------------------
;; Macro zum Initialisieren der globalen Umgebung
;;------------------------------------------------------------------------------
(defmacro def-global-op (op &rest names)
  (let ((set-function (intern-prefixed "SET-" op)))
    `(defun ,(intern-prefixed "P0-INIT-" op) ()
      ,@(mapcar
         #'(lambda (name)
             `(,set-function ',name #',(intern-prefixed "P1-" name)))
         names))))
    
;;------------------------------------------------------------------------------
;; Initialisieren der Special Forms in der Compiler Umgebung
;;------------------------------------------------------------------------------
(def-global-op special-form
  L::BLOCK
  L::CATCH
  L::COMPILER-LET
  L::DECLARE
  L::EVAL-WHEN
  L::FIND-CLASS
  L::FLET
  L::FUNCTION
  L::GO
  L::IF
  L::LABELS
  L::LET
  L::LET*
  L::MACROLET
  L::MULTIPLE-VALUE-CALL
  L::MULTIPLE-VALUE-PROG1
  L::PROGN
  L::PROGV
  L::QUOTE
  L::RETURN-FROM
  L::SETQ
  L::TAGBODY
  L::THE
  L::THROW
  L::UNWIND-PROTECT)

;;------------------------------------------------------------------------------
;; Bekanntgeben der Toplevel-Forms
;;------------------------------------------------------------------------------
(def-global-op top-level
  L::defclass
  L::declaim
  L::defconstant
  L::defgeneric
  ;; L::define-modify-macro
  ;; L::define-setf-method
  L::defmacro
  L::defmethod
  L::defparameter
  L::defsetf
  L::defstruct
  L::deftype
  L::defun
  L::defvar
  L::in-package
  L::use-package
  L::provide
  L::require
  L::load
  ;; L::load-foreign
  L::export
  L::proclaim)

;;------------------------------------------------------------------------------
;; Initialisieren der System Makros in der Compiler Umgebung
;;------------------------------------------------------------------------------
(def-global-op sys-macro
  L::AND
  L::ASSERT
  L::CASE
  ;;  CCASE                  Nicht implementiert
  ;;  CHECK-TYPE             Nicht implementiert
  L::COND
  ;;  CTYPECASE              Nicht implementiert
  L::DECF
  L::DO
  L::DO*
  L::DO-ALL-SYMBOLS
  L::DO-EXTERNAL-SYMBOLS
  L::DO-SYMBOLS
  L::DOLIST
  L::DOTIMES
  L::ECASE
  L::ETYPECASE
  L::INCF
  L::LOCALLY
  L::LOOP
  L::MULTIPLE-VALUE-BIND
  L::MULTIPLE-VALUE-LIST
  L::MULTIPLE-VALUE-SETQ
  L::OR
  L::POP
  L::PROG
  L::PROG*
  L::PROG1
  L::PROG2
  L::PSETF
  L::PSETQ
  L::PUSH
  L::PUSHNEW
  L::REMF
  L::RETURN
  L::ROTATEF
  L::SETF
  L::SHIFTF
  ;;  STEP                   Nicht implementiert
  ;;  TIME                   Nicht implementiert
  ;;  TRACE                  Nicht implementiert
  L::TYPECASE
  L::UNLESS
  ;;  UNTRACE                Nicht implementiert
  L::WHEN
  L::WITH-INPUT-FROM-STRING
  L::WITH-OPEN-FILE
  L::WITH-OPEN-STREAM
  L::WITH-OUTPUT-TO-STRING)

;;------------------------------------------------------------------------------
;; System Compiler Macros bekanntgeben
;;------------------------------------------------------------------------------
(def-global-op compiler-macro
  L::typep
  L::make-instance)

;;------------------------------------------------------------------------------
(provide "p0init")
