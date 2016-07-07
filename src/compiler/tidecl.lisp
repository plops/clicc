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
;;; Funktion : Makros und Funktionen zur Handhabung der Typdeklarationen
;;;
;;; $Revision: 1.38 $
;;; $Id: tidecl.lisp,v 1.38 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "titypes")
(require "tidef")
(require "timisc")
 
;;------------------------------------------------------------------------------
;; Beim jedem Laden dieser Datei wird dieser Schalter zurueckgesetzt, weil sich
;; Funktionsbeschreibungen geaendert haben koennen.
;;------------------------------------------------------------------------------
(setf *ti-type-declarations-are-initialized* nil)


;;------------------------------------------------------------------------------
;; Liefert zu einem Funktionsnamen die entsprechende Funktion.
;;------------------------------------------------------------------------------
(defun get-the-function (function-name)
  (if (atom function-name) 
    (get-global-fun function-name)
    (cdr (get-global-setf-fun-def function-name))))


;;------------------------------------------------------------------------------
;; Makro zur Deklaration der Funktionstypen von importierten Funktionen.
;; Auf den Funktionsnamen folgen die Argumenttypen, ein Pfeil, der Ergebnistyp
;; und eine optionale Typabstraktionsfunktion.
;;------------------------------------------------------------------------------
(defmacro dec-type (function-name argument-types arrow result-type 
                                  &optional type-abstraction-function)
  (declare (ignore arrow))
  `(progn
    (let ((function  (get-the-function ',function-name)))
      (setf (?argument-types function) (list ,@argument-types))
      (setf (?result-type    function) ,result-type)
      ,@(when type-abstraction-function
             `((setf (?type-abstraction-function function) 
               ,type-abstraction-function))))))


;;------------------------------------------------------------------------------
;; Makro fuer Typabstraktionsfunktion der Typtests:
;;------------------------------------------------------------------------------
(defmacro type-test-type-abstraction-function (type)
  `#'(lambda (argument-type)
       (cond ((is-bottom-t argument-type)                    bottom-t)
             ((zs-subtypep argument-type ,type)             t-symbol-t)
             ((not (types-are-conform ,type argument-type)) null-t)
             (T bool-t))))


;;------------------------------------------------------------------------------
;; Makros zum Deklarieren der Typbeschreibungen der Typpraedikate:
;;------------------------------------------------------------------------------
(defmacro declare-type-predicate (type-predicate type)
  `(dec-type ,type-predicate () -> bool-t
             (type-test-type-abstraction-function ,type)))


;;------------------------------------------------------------------------------
(defmacro declare-type-predicates (type-predicate-type-pairs)
  `(progn . 
    ,(mapcar #'(lambda (type-predicate-type-pair) 
                 (let ((type-predicate (first  type-predicate-type-pair))
                       (type           (second type-predicate-type-pair)))  
                   `(declare-type-predicate ,type-predicate ,type)))
              type-predicate-type-pairs)))


;;------------------------------------------------------------------------------
;; Initialisiere die Typabstraktionsfunktionen der importierten Funktionen und
;; die Typzusicherungen.
;;------------------------------------------------------------------------------
(defun initialize-type-declarations ()
  (when (> *ti-verbosity* 0)
    (clicc-message "Initialize type declarations"))

  ;; Deklarationen aus tiimpdec.lisp
  (initialize-function-descriptions-part1)
  (initialize-function-descriptions-part2)
  (initialize-function-descriptions-part3)
  (initialize-function-descriptions-part4)

  ;; Deklarationen aus tiassert.lisp
  (initialize-type-assertion-functions)       
)

;;------------------------------------------------------------------------------
(provide "tidecl")


