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
;;; Funktion : Nach der eigentlichen Typinferenz wird das analysierte 
;;;            Zwischensprachkonstrukt noch einmal traversiert und dabei
;;;            nach Typfehlern untersucht. Die folgenden Fehler werden gemeldet:
;;;             o an Funktionsposition einer Applikation steht ein Objekt, das
;;;               kein Funktionstyp ist,
;;;             o eine Funktion wurde auf Argumente mit Bottom-Typ appliziert.
;;;
;;; $Revision: 1.20 $
;;; $Id: tipass3.lisp,v 1.20 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "titypes")
(require "tidef")
(require "traverse")

;;------------------------------------------------------------------------------
;; Traversiert das uebergebene Modul und durchsucht es nach Typfehlern.
;;------------------------------------------------------------------------------
(defun look-for-type-errors (a-module)
  (when (> *ti-verbosity* 0)
    (clicc-message "Looking for type errors"))
  (mapc #'tw-fun (?fun-list a-module)))

;;------------------------------------------------------------------------------
;; Funktionen, deren Parameter den Typ BOTTOM haben, sollen nicht weiter
;; analysiert werden
;;------------------------------------------------------------------------------
(defun tw-fun (fun)
  (let* ((*current-fun* fun)
         (all-vars (?all-vars (?params fun)))
         (bottom-params (remove-if-not #'is-bottom-typed all-vars)))
    (cond
      (bottom-params
       (ti-warning "Some parameters are BOTTOM typed")
       (ti-warn-line 2 "the bottom typed parameters : ~S"
                     (mapcar #'?symbol bottom-params)))
      (T (tw-params (?params fun))
         (tw-form (?body fun))))))

;;------------------------------------------------------------------------------
(defun tw-params (params)
  (dolist (a-opt (?opt-list params))
    (tw-form (?init a-opt)))
  (dolist (a-key (?key-list params))
    (tw-form (?init a-key))))

;;------------------------------------------------------------------------------
;; Untersucht Ausdr"ucke auf potentielle Typfehler.
;; Wenn ein Ausdruck mit Typfehlern gefunden wurde, so werden seine
;; Teilausdr"ucke nicht weiter analysiert, um redundanten Meldungen vorzubeugen.
;;------------------------------------------------------------------------------
(defgeneric tw-form (object))

(defmethod tw-form ((a-form form)))
(defmethod tw-form ((a-cont cont)))

;;------------------------------------------------------------------------------
(defmethod tw-form ((an-app app))
  (let* ((function       (?form an-app))
         (function-type  (?type function))
         (arguments      (?arg-list an-app))
         (argument-types (mapcar #'?type arguments)))

    (cond
      ((and (not (types-are-conform function-t function-type))
            (not (cont-p function)))
       (ti-warning "A non-function is applied.")
       (ti-warn-line 2 "function      : ~S" function)
       (ti-warn-line 2 "function type : ~S" (output-type function-type))
       (ti-warn-line 2 ""))

      ((some #'is-bottom-t argument-types)
       (ti-warning "BOTTOM typed argument types")
       (ti-warn-line 2 "applied function : ~S" 
                     (if (fun-p function)
                         (?symbol function)
                         `unknown))
       (ti-warn-line 2 "arguments        : ~S" arguments)
       (ti-warn-line 2 "argument types   : ~S" (mapcar #'output-type 
                                                       argument-types)))
      (T (tw-form function)
         (mapc #'tw-form arguments)))))

;;------------------------------------------------------------------------------
(defmethod tw-form ((a-setq-form setq-form))
  (cond
    ((is-bottom-t (?type a-setq-form))
     (ti-warning "A location is set to type BOTTOM.")
     (ti-warn-line 2 "location : ~S"
                   (?symbol (let ((location (?location a-setq-form)))
                              (if (var-ref-p location)
                                  (?var location)
                                  location))))
     (ti-warn-line 2 "form     : ~S" (?form a-setq-form)))
    (T (tw-form (?form a-setq-form)))))

;;------------------------------------------------------------------------------
(defmethod tw-form ((a-let*-form let*-form))
  (let* ((init-list (?init-list a-let*-form))
         (var-list (?var-list a-let*-form))
         (bottom-form-pos (position-if #'is-bottom-typed init-list)))
    (cond
      (bottom-form-pos
       (ti-warning "An initform in let* has type BOTTOM")
       (ti-warn-line 2 "variable : ~S"
                     (?symbol (nth bottom-form-pos var-list)))
       (ti-warn-line 2 "initform : ~S" (nth bottom-form-pos init-list)))
      (T (mapc #'tw-form init-list)
         (tw-form (?body a-let*-form))))))

;;------------------------------------------------------------------------------
(defmethod tw-form ((a-labels-form labels-form))
  (mapc #'tw-fun (?fun-list a-labels-form))
  (tw-form (?body a-labels-form)))

;;------------------------------------------------------------------------------
(defmethod tw-form ((a-progn-form progn-form))
  (do* ((form-list (?form-list a-progn-form) (cdr form-list))
        (form (first form-list)))
       ((null form-list))
    (when (and (cdr form-list) (is-bottom-typed form))
      (ti-warning "A BOTTOM typed form precedes a form") 
      (ti-warn-line 2 "the bottom typed form : ~S" form)
      (ti-warn-line 2 "the next form         : ~S" (second form-list))
      (return))
    (tw-form form)))
        
;;------------------------------------------------------------------------------
(defmethod tw-form ((an-if-form if-form))
  (let ((pred (?pred an-if-form)))
    (cond
      ((is-bottom-typed pred)
       (ti-warning "The predicate of an if-form has type BOTTOM")
       (ti-warn-line 2 "the predicate : ~S" pred))
      (T (tw-form pred)
         (tw-form (?then an-if-form))
         (tw-form (?else an-if-form))))))

;;------------------------------------------------------------------------------
(defmethod tw-form ((a-switch-form switch-form))
  (let ((form (?form a-switch-form)))
    (cond
      ((is-bottom-typed form)
       (ti-warning "The control form of an switch-form has type BOTTOM")
       (ti-warn-line 2 "the form : ~S" form))
      (T (tw-form form)
         (dolist (a-labeled-form (?case-list a-switch-form))
           (tw-form (?form a-labeled-form)))
         (tw-form (?otherwise a-switch-form))))))


;;------------------------------------------------------------------------------
(defmethod tw-form ((a-let/cc-form let/cc-form))
  (tw-form (?body a-let/cc-form)))

;;------------------------------------------------------------------------------
(defmethod tw-form ((a-tagbody-form tagbody-form))
  (tw-form (?first-form a-tagbody-form))
  (mapc-tagged-form-list (?tagged-form-list a-tagbody-form)
                         #'tw-form
                         #'tw-form))

;;------------------------------------------------------------------------------
(defmethod tw-form ((a-mv-lambda mv-lambda))
  (cond
    ((is-bottom-typed (?arg a-mv-lambda))
     (ti-warning "Argument of mv-lambda has type BOTTOM.")
     (ti-warn-line 2 "argument: ~s" (?arg a-mv-lambda)))
    (T (tw-form (?arg a-mv-lambda))
       (tw-params (?params a-mv-lambda))
       (tw-form (?body a-mv-lambda)))))

;;------------------------------------------------------------------------------
(defun is-bottom-typed (a-form)
  (is-bottom-t (?type a-form)))

;;------------------------------------------------------------------------------
;; Gibt eine Warnzeile aus. Beachtet *ti-verbosity*.
;;------------------------------------------------------------------------------
(defun ti-warn-line (priority warn-string &rest args-for-warn-string)
  (when (>= *ti-verbosity* priority)
    (format *error-output* "; ")
    (apply #'format *error-output* warn-string args-for-warn-string)
    (terpri *error-output*)))


;;------------------------------------------------------------------------------
;; ti-warning gibt eine Warnung aus und erhoeht den Zaehler fuer Warnungen.
;;------------------------------------------------------------------------------
(defun ti-warning (warn-string &rest args-for-warn-string)
  (incf *NWARNINGS*)
  (let ((complete-warn-string
         (concatenate 'string 
                      (format nil "Warning (in ~A): " 
                              (?symbol *current-fun*))
                      warn-string)))
    (apply #'ti-warn-line 1 complete-warn-string args-for-warn-string)))
  

;;------------------------------------------------------------------------------
(provide "tipass3") 



