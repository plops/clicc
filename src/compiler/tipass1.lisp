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
;;; Funktion : Vorbereitungen zur Typinferenz. Dazu gehoeren:
;;;             o Typisierung der Literale,
;;;             o Feststellen, ob eine Funktion als funktionales 
;;;               Objekt verwendet wird,
;;;             o Setzen der `called-by'-Komponenten.
;;;
;;; $Revision: 1.70 $
;;; $Id: tipass1.lisp,v 1.70 1994/12/06 13:03:47 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "titypes")
(require "tidef")
(require "traverse")

;;------------------------------------------------------------------------------
;; zs-type-of liefert den Typ eines Literals. Diese Funktion ist nicht 
;; generisch implementiert, weil die zu unterscheidenden Typen im allgemeinen
;; nicht als Klassen zur Verfuegung stehen.
;;------------------------------------------------------------------------------
(defun zs-type-of (anything) 
  (labels ((proper-list-p (a-list)
             (null (rest (last a-list)))))
  
    (etypecase anything
      (null-form        null-t)
      (null             null-t)
      (sym              (if (eq (?symbol anything) T)
                            t-symbol-t
                            other-symbol-t))
      (cons             (if (proper-list-p anything)
                            (apply #'list-cons-of 
                                   (mapcar #'zs-type-of anything))
                            non-list-cons-t))
      (character        character-t)
      (string           string-t)
      (vector           non-string-vector-t)
      (array            non-vector-array-t)
      (fun              function-t)
      (literal-instance class-t)
      ((signed-byte 8)  byte-t)
      ((signed-byte 16) non-byte-word-t)
      ((satisfies target-fixnump) non-word-fixnum-t)
      (integer bignum-t)
      (float float-t)
      (number number-t)
      (package          package-t)
      (stream           stream-t))))

;;------------------------------------------------------------------------------
;; Setze die Typkomponenten der Literale. Wenn die Typen der Nichtliterale 
;; nicht inferiert werden sollen, dann werden die Typannotationen der 
;; Nichtliterale auf das Typ-Topelement gesetzt.
;;------------------------------------------------------------------------------
(defgeneric preset-types (object))

(defmethod preset-types ((a-form form))
  (unless (do-type-inference-on-non-literals)
    (setf (?type a-form) top-t)))

(defmethod preset-types ((a-var var))
  (unless (do-type-inference-on-non-literals)
    (setf (?type a-var) top-t)))

(defmethod preset-types ((a-let* let*-form))
  (unless (do-type-inference-on-non-literals)
    (setf (?type a-let*) top-t)
    (dolist (var (?var-list a-let*))
      (setf (?type var) top-t))))
  
(defmethod preset-types ((the-empty-list null-form))
  (setf (?type the-empty-list) 
        null-t))

(defmethod preset-types ((an-instance literal-instance)) 
  (setf (?type an-instance) 
        class-t))

(defmethod preset-types ((a-simple-literal simple-literal))
  (setf (?type a-simple-literal) 
        (zs-type-of (?value a-simple-literal))))

(defmethod preset-types ((a-structured-literal structured-literal))
  (setf (?type a-structured-literal) 
        (zs-type-of (?value a-structured-literal))))

(defmethod preset-types ((a-sym sym))
  (setf (?type a-sym) 
        (zs-type-of a-sym)))

(defmethod preset-types ((a-fun fun))
  (setf (?type a-fun) 
        (zs-type-of a-fun)))

(defmethod preset-types :after ((a-var-ref var-ref))
  (preset-types (?var a-var-ref)))

(defmethod preset-types :after ((a-dynamic dynamic))
  (let* ((sym (?sym a-dynamic))
         (constant-value (?constant-value sym)))
    (when (constant-value-p sym)
      (preset-types constant-value))))

(defmethod preset-types :after ((a-defined-named-const defined-named-const))
  (let ((value (?value a-defined-named-const)))
    (case value
      (:forward (internal-error 'preset-types
                                ":forward in defined-named-const ~S."
                                a-defined-named-const))
      (:unknown)
      (otherwise (preset-types value)
                 (setf (?type a-defined-named-const) (?type value))))))

(defmethod preset-types :after ((an-imported-named-const imported-named-const))
   (setf (?type an-imported-named-const)
         (case (?value-zs-type an-imported-named-const)
           (:cons cons-t)
           (:string string-t)
           (:vector vector-t)
           (:array array-t)
           (:literal-instance class-t))))

;;------------------------------------------------------------------------------
;; Setze die Typkomponenten der Nicht-Literale auf das Bottom-Element des 
;; Typverbands.
;;------------------------------------------------------------------------------
(defgeneric reset-type-annotation (object))

(defmethod reset-type-annotation ((a-form form))
  (setf (?type a-form) bottom-t))

(defmethod reset-type-annotation ((a-var var))
  (setf (?type a-var) bottom-t))

(defmethod reset-type-annotation ((a-let* let*-form))
  (setf (?type a-let*) bottom-t)
  (dolist (var (?var-list a-let*))
    (setf (?type var) bottom-t)))

(defmethod reset-type-annotation ((a-defined-named-const defined-named-const))
  (let ((value (?value a-defined-named-const)))
    (when (or (eq value :unknown) (eq value :forward))
      (setf (?type a-defined-named-const) bottom-t))))

(defmethod reset-type-annotation ((a-literal literal)))

(defmethod reset-type-annotation ((a-fun fun)))

(defmethod reset-type-annotation ((a-defined-fun defined-fun))
  (setf (?result-type a-defined-fun) bottom-t))

(defmethod reset-type-annotation ((a-sym sym)))

(defmethod reset-type-annotation :after ((a-var-ref var-ref))
  (reset-type-annotation (?var a-var-ref)))


(defun reset-type-annotations ()
  (traverse-module *module*
                   :before-funs (list #'reset-type-annotation)
                   :tr-fun-body-p #'(lambda (a-fun)
                                      (declare (ignore a-fun)) nil)))

  
;;------------------------------------------------------------------------------
;; Sollen/koennen die Aufrufkontexte der Funktion beachtet werden?
;;------------------------------------------------------------------------------
(defun use-call-contexts (fun)
  (and (not (?unknown-caller fun))
       (do-interprocedural-type-inference)))


;;------------------------------------------------------------------------------
;; Funktion zum Ermitteln der potentiell gelesenen/geschriebenen Variablen
;;------------------------------------------------------------------------------
(defun get-read/write-list (read/write-list fun dynamic-variables)
  (let ((result (if (listp read/write-list)
                    read/write-list
                    (append (if (local-fun-p fun)
                                (?free-lex-vars fun)
                                nil)
                            dynamic-variables))))
    (if (use-bindings-of-dynamic-variables)
        result
        (remove-if #'dynamic-p result))))


;;------------------------------------------------------------------------------
;; Setzt die Eintritts- und Ergebnistypumgebungen aller Funktionen auf ihren 
;; jeweiligen Startwert.
;;------------------------------------------------------------------------------
(defun set-all-predecessor-and-result-type-envs (a-module)
  (let* ((dynamic-variables 
          (remove-if-not #'(lambda (var) 
                             (eq (?constant-value (?sym var)) :no-const))
                         (?dyn-var-list a-module)))
         (imported-constants 
          (remove-if-not #'(lambda (var)
                             (and (imported-sym-p (?sym var))
                                  (eq (?constant-value (?sym var)) :unknown)))
                         (?dyn-var-list a-module)))
         (used-dynamic-variables
          (if (use-bindings-of-dynamic-variables)
              dynamic-variables
              nil)))

    (when (and (> *ti-verbosity* 1)
               (use-bindings-of-dynamic-variables)
               (> (length dynamic-variables) 0))
      (clicc-message "The module contains ~D dynamic variables."
                     (length dynamic-variables)))

    (setf *type-environment* (mapcar #'(lambda (var) (cons var top-t))
                                     used-dynamic-variables))

    (labels ((set-pred-and-result-type-env (fun)
               (let ((read-list  (get-read/write-list (?read-list fun) fun
                                                      used-dynamic-variables))
                     (write-list (get-read/write-list (?write-list fun) fun
                                                      used-dynamic-variables))
                     (init-type  (if (use-call-contexts fun) bottom-t top-t)))

                 ;; Setze die Eintrittstypumgebung.                 
                 ;;--------------------------------
                 (setf (?pred-type-env fun)
                       (mapcar #'(lambda (var)
                                   (cons var init-type))
                               (union read-list write-list)))

                 
                 ;; Setze zur intraprozeduralen Inferenz Ergebnistypen auf Top.
                 ;;------------------------------------------------------------
                 (unless (do-interprocedural-type-inference)
                   (setf (?result-type fun) top-t))
                 

                 ;; Setze die Ergebnistypumgebung.
                 ;;-------------------------------
                 (setf (?result-type-env fun)
                       (mapcar #'(lambda (var) (cons var bottom-t)) 
                               write-list)))
               
               
               ;; Binde bei allen Funktionen, deren Aufrufkontexte nicht
               ;; verwendet werden die Parametertypen an das Typ-Topelement.
               ;;-----------------------------------------------------------
               (unless (use-call-contexts fun)
                 (simple-set-parameter-types (?params fun)))))
       
      (mapc #'set-pred-and-result-type-env (?all-funs a-module)))


    (let ((dynamic-variable-type (if (use-bindings-of-dynamic-variables)
                                     bottom-t
                                     top-t)))
          (dolist (a-dynamic dynamic-variables)
            (setf (?type a-dynamic) dynamic-variable-type)))

    (dolist (an-imported-constant imported-constants)
      (setf (?type an-imported-constant) top-t))))


;;------------------------------------------------------------------------------
;; Traversiert das uebergebene Modul und markiert dabei diejenigen Funktionen
;; und Continuations, die als funktionales Objekt verwendet werden.
;;------------------------------------------------------------------------------
(defun prepare-type-inference (a-module)
  (traverse-module a-module
                   :after-funs (list #'preset-types)
                   :tr-fun-body-p #'(lambda (a-fun)
                                      (declare (ignore a-fun)) nil))

  (set-all-predecessor-and-result-type-envs a-module))


;;------------------------------------------------------------------------------
(provide "tipass1") 



