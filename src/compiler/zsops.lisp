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
;;; Funktion : Allgemeine Operatoren fuer die Lisp nahe Zwischensprache
;;;
;;; $Revision: 1.7 $
;;; $Id: zsops.lisp,v 1.7 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; self-evaluating-p
;;------------------------------------------------------------------------------
(defun self-evaluating-p (v)
  (or (simple-literal-p v)
      (structured-literal-p v)
      (sym-p v)
      (fun-p v)
      (class-def-p v)
      (cont-p v)))

;;------------------------------------------------------------------------------
;; Stellt fest, ob ein Ausdruck kopiert werden kann.
;;------------------------------------------------------------------------------
(defun may-be-copied (form)
  (or (self-evaluating-p form) (var-ref-p form) (named-const-p form)))

;;------------------------------------------------------------------------------
;; Ueberprueft, ob es sich um eine einfache Konstante handelt, d.h. ob die
;; Konstante dupliziert werden darf und trotzdem im Sinne von EQ (EQL)
;; identisch bleibt.
;;------------------------------------------------------------------------------
(defun simple-constant-p (object)
  (or (and (atom object)
           (or (integerp object) (characterp object)))
      (and (consp object)
           (eq (first object) 'QUOTE)
           (symbolp (second object)))))

;;------------------------------------------------------------------------------
;; Iteriert ueber die form-list einer Progn-Form, und appliziert fun bzw. 
;; fun-for-last auf die Elemente.
;; Resultat: Result der Applikation von fun-for-last auf das letzte Element.
;;------------------------------------------------------------------------------
(defun mapc-progn-form-list (form-list fun fun-for-last-element)
  (cond
    ((atom (cdr form-list)) 
     (funcall fun-for-last-element (car form-list)))
    (t 
     (funcall fun (car form-list))
     (mapc-progn-form-list (cdr form-list) fun fun-for-last-element))))

;;------------------------------------------------------------------------------
;; Iteriert ueber die tagged-form-list einer Tagbody-Form, und appliziert fun
;; bzw. fun-for-last auf den Slot form der Elemente.  Resultat: Result der
;; Applikation von fun-for-last auf die form des letzten Elements.
;;------------------------------------------------------------------------------
(defun mapc-tagged-form-list (tagged-form-list fun fun-for-last-element)
  (cond
    ((atom (cdr tagged-form-list)) 
     (funcall fun-for-last-element (?form (car tagged-form-list))))
    (t 
     (funcall fun (?form (car tagged-form-list)))
     (mapc-tagged-form-list (cdr tagged-form-list) fun fun-for-last-element))))

;;------------------------------------------------------------------------------
(provide "zsops")

