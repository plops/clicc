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
;;; Funktion : Ersetzung von Konstanten und Variablen durch ihre Werte.
;;;
;;; $Revision: 1.9 $
;;; $Id: subst.lisp,v 1.9 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "delete")

;;------------------------------------------------------------------------------
;; A-Liste, die Variablen einen Wert zuordnet, durch den Vorkommen der
;; Variablen im Quelltext unter geeigneten Umstaenden ersetzt werden duerfen
;;------------------------------------------------------------------------------
(defvar *SUBSTITUTION* ())

;;------------------------------------------------------------------------------
;; m1, m2 sind partielle Abbildungen, dargestellt als A-Listen,
;; m1 wird auf den Definitionsbereich von m2 eingeschraenkt
;;------------------------------------------------------------------------------
(defun restrict-map-to (m1 m2)
  (collect-if #'(lambda (pair) (member (car pair) m2 :key #'car)) m1))

;;------------------------------------------------------------------------------
;; Die akt. Substitution wird auf die total undef. Fkt. gesetzt.
;;------------------------------------------------------------------------------
(defmacro clear-substitution ()
  `(setq *SUBSTITUTION* ()))

;;------------------------------------------------------------------------------
;; Diese Formen koennen mittels EQ verglichen werden
;;------------------------------------------------------------------------------
(defun copy-is-eq-p (x)
  (or (null-form-p x)
      (character-form-p x)
      (sym-p x)
      (fun-p x)
      (cont-p x)
      (class-def-p x)))


;;------------------------------------------------------------------------------
;; inc-uses erhoeht die used- bzw. read-Slots der eingesetzten Werte
;;------------------------------------------------------------------------------
(defun inc-uses (x)
  (if (cont-p x)
      (incf (?read x))
      (when (or (defined-sym-p x) (defined-fun-p x) (defined-class-p x))
        (incf (?used x)))))

;;------------------------------------------------------------------------------
;; subst-1form substituiert unter geeignten Umstaenden die Vorkommen von
;; Konstanten und Variablen mittels *SUBSTITUTION* in einem Zwischensprach-
;; knoten.
;;------------------------------------------------------------------------------

(defmethod subst-1form ((a-form form))
  a-form)

;;------------------------------------------------------------------------------
;; Variablen Referenz
;;------------------------------------------------------------------------------
(defmethod subst-1form ((a-var-ref var-ref))
  (let ((var-subst (subst-1form (?var a-var-ref))))
    (if (var-p var-subst)
        (progn
          (setf (?var a-var-ref) var-subst)
          a-var-ref)
        var-subst)))

(defmethod subst-1form ((a-dynamic dynamic))
  (let* ((value (?constant-value (?sym a-dynamic))))
    (if (copy-is-eq-p value)
        (progn
          (decf (?read a-dynamic))
          (inc-uses value)
          value)
        a-dynamic)))

(defmethod subst-1form ((a-static static))
  (let ((subst-bind (assoc a-static *SUBSTITUTION* :test #'eq)))
    (if subst-bind
        (progn
          (decf (?read a-static))
          (let ((value (cdr subst-bind)))
            (inc-uses value)
            value))
        a-static)))

;;------------------------------------------------------------------------------
;; angewandtes Vorkommen von named-const
;;------------------------------------------------------------------------------
(defmethod subst-1form ((a-defined-named-const defined-named-const))
  (let ((value (?value a-defined-named-const)))
    (if (copy-is-eq-p value)
        (progn
          (decf (?read a-defined-named-const))
          (inc-uses value)
          value)
        a-defined-named-const)))

;;------------------------------------------------------------------------------
;; setq
;;------------------------------------------------------------------------------
(defmethod subst-1form ((a-setq-form setq-form))
  (let ((var (?var (?location a-setq-form))))
    (when (static-p var)
      ;; Die Variable darf nach Ausfuehrung dieser Zuweisung nicht mehr
      ;; durch ihren initialen Wert ersetzt werden.
      ;;------------------------------------------------------------------
      (setq *SUBSTITUTION* (remove var *SUBSTITUTION* :key #'car))))
  a-setq-form)

;;------------------------------------------------------------------------------
(provide "subst")
