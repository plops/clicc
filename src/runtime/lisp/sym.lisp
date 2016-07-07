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
;;; Funktion : - GET, SET-GET
;;;            - REMPROP
;;;            - GETF, SETF
;;;            - GET-PROPERTIES
;;;            - COPY-SYMBOL
;;;            - GENSYM, GENTEMP
;;;            - KEYWORDP
;;;
;;; $Revision: 1.9 $
;;; $Id: sym.lisp,v 1.9 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(get remprop getf get-properties copy-symbol gensym *gensym-counter*
   gentemp keywordp
   symbol-value set symbol-plist symbol-name symbol-package boundp makunbound))

(export '(rt::set-prop rt::remf-internal) "RT")

;;--------------------------------------------------------------------------
; Verhalten, falls die Property-List-Funktionen auf Werte angewandt werden, 
; die keine Property-Listen sind: 
; - Liste mit gerader Anzahl von Elementen, INDICATOR nicht vorhanden,
;   (CDR(LAST pliste)) = Atom != NIL
;     - getf:     default
;     - set-getf: (indicator value ... ... . atom)
;     - remf-internal: (... ... . atom)
; - Liste mit ungerader Anzahl von Elementen, letztes Element = INDICATOR
;     - getf:     NIL
;     - set-getf: Error "RPLACA applied to empty List"
;     - remf-internal: letztes Element wird entfernt
; - Liste mit ungerader Anzahl von Elementen, (CDR (LAST pliste)) = ATOM
;   (CAR (LAST pliste)) = INDICATOR
;     - getf, set-getf, remf-internal: Error "CDR applied to non NIL atom"
;;--------------------------------------------------------------------------

;;--------------------------------------------------------------------------
(defun get (symbol indicator &optional default)
  (getf (symbol-plist symbol) indicator default))

;;--------------------------------------------------------------------------
(defun (setf get) (value symbol indicator &optional default)
  (setf (getf (symbol-plist symbol) indicator) value)
  value)

;;--------------------------------------------------------------------------
(defun remprop (symbol indicator)
  (remf (symbol-plist symbol) indicator))

;;--------------------------------------------------------------------------
;; Hilfsfunktionen fuer getf, set-getf
;;--------------------------------------------------------------------------
(defun get-prop (list indicator)
  (cond
    ((atom list) nil) 
    ((eq (car list) indicator) list)
    (t (get-prop (cddr list) indicator))))

;;--------------------------------------------------------------------------
;; Resultat: veraenderte / erweiterte Pliste
;;--------------------------------------------------------------------------
(defun rt:set-prop (plist indicator value)
  (let ((list (get-prop plist indicator)))
    (cond
      (list
       (rplaca (cdr list) value)
       plist)
      (t (cons indicator (cons value plist))))))

;;--------------------------------------------------------------------------
(defun getf (place indicator &optional default)
  (let ((list (get-prop place indicator)))
    (if list
      (cadr list)
      default)))

;;--------------------------------------------------------------------------
;; (remf place :test) -->
;; (multiple-value-bind (plist found)
;;     (remf-internal x :test)
;;   (if found
;;     (progn (setf place plist) t)
;;     nil))
;; Resultate
;; 1. verkleinerte Liste oder NIL
;; 2. Wert gibt an, ob Indicator gefunden wurde
;;--------------------------------------------------------------------------
(defun rt:remf-internal (list indicator)
  (cond
    ((atom list) (values nil nil))
    ((eq (car list) indicator) (values (cddr list) t))
    (t (do ((list1 list (cddr list1))
            (list2 (cddr list) (cddr list2)))
           ((atom list2) (values nil nil)) ;end test
         (when (eq (car list2) indicator)
           (rplacd (cdr list1) (cddr list2))
           (return-from rt:remf-internal (values list t)))))))
 
;;--------------------------------------------------------------------------
(defun get-properties (list indicator-list)
  (cond
    ((atom list) (values nil nil nil))
    ((member (car list) indicator-list) (values (car list) (cadr list) list))
    (t (get-properties (cddr list) indicator-list)))) 

;;------------------------------------------------------------------------------
;; make-symbol print-name
;;------------------------------------------------------------------------------
(defun make-symbol (print-name)
  (let ((sym (rt::make-symbol (string-to-simple-string print-name))))
    (setf (rt::symbol-plist sym) nil)
    (setf (rt::symbol-package sym) nil)
    (makunbound sym)))              ;liefert sym zurück

;;--------------------------------------------------------------------------
;; copy-symbol sym &optional copy-props
;;--------------------------------------------------------------------------
(defun copy-symbol (sym &optional copy-props)
  (let ((new-sym (make-symbol (symbol-name sym))))
    (when copy-props
      (when (boundp sym)
        (set new-sym (symbol-value sym)))
      (setf (symbol-plist new-sym) (copy-list (symbol-plist sym))))
    new-sym))

;;--------------------------------------------------------------------------
;; gensym &optional x
;;--------------------------------------------------------------------------
(defparameter *gensym-prefix* "G")
(defparameter *gensym-counter*  0)

(defun gensym (&optional x)
  (typecase x
    ((null))
    ((integer 0 *) (setq *gensym-counter* x))
    (string (setq *gensym-prefix* x))
    (otherwise (error "positive integer or string expected")))
  (prog1
      (make-symbol
       (concatenate 'string *gensym-prefix*
                    (prin1-to-string *gensym-counter*)))
    (incf *gensym-counter*)))

;;--------------------------------------------------------------------------
;; gentemp &optional (prefix "T") (package *package*)
;;--------------------------------------------------------------------------
(defparameter *gentemp-counter* 0)

(defun gentemp (&optional (prefix "T") (package *package*))
  (unless (stringp prefix)
    (error "string expected"))
  (loop
    (incf *gentemp-counter*)
    
    (let ((name (concatenate 'string
                             prefix
                             (prin1-to-string *gentemp-counter*))))
      (multiple-value-bind (sym exists)
          (intern name package) 
        (unless exists (return sym))))))
                           
;;--------------------------------------------------------------------------
;; keywordp object
;;--------------------------------------------------------------------------
(defun keywordp (object)
  (and (symbolp object) (eq (symbol-package object) *keyword-package*)))

;;------------------------------------------------------------------------------
;;------------------------------------------------------------------------------
(defconstant SYM_EXPECTED "~a is not a symbol")
(defconstant TRY_CHANGE_CONST "cannot change value of ~a, which is a constant")

(deftype rt::sym () '(satisfies rt::symp))
(defvar *nil-plist*)
(defvar *nil-package*)

;;------------------------------------------------------------------------------
;; SYMBOL-VALUE symbol
;;------------------------------------------------------------------------------
(defun symbol-value (sym)
  (typecase sym
    (rt::sym (rt::symbol-value sym))
    (null nil)
    (T (error SYM_EXPECTED sym))))

;;------------------------------------------------------------------------------
;; SET sym value
;;------------------------------------------------------------------------------
(defun set (sym value)
  (typecase sym
    (rt::sym (if (rt::constant-flag-p sym)
                 (error TRY_CHANGE_CONST sym)
                 (setf (rt::symbol-value sym) value)))
    (null (error TRY_CHANGE_CONST sym))
    (T (error SYM_EXPECTED sym))))

;;------------------------------------------------------------------------------
;; SYMBOL-PLIST symbol
;;------------------------------------------------------------------------------
(defun symbol-plist (sym)
  (typecase sym
    (rt::sym (rt::symbol-plist sym))
    (null *nil-plist*)
    (T (error SYM_EXPECTED sym))))

;;------------------------------------------------------------------------------
;; (SETF SYMBOL-PLIST) value sym
;;------------------------------------------------------------------------------
(defun (setf symbol-plist) (value sym)
  (typecase sym
    (rt::sym (setf (rt::symbol-plist sym) value))
    (null (setf *nil-plist* value))
    (T (error SYM_EXPECTED sym))))

;;------------------------------------------------------------------------------
;; SYMBOL-NAME symbol
;;------------------------------------------------------------------------------
(defun symbol-name (sym)
  (typecase sym
    (rt::sym (rt::symbol-name sym))
    (null "NIL")
    (T (error SYM_EXPECTED sym))))
  
;;------------------------------------------------------------------------------
;; SYMBOL-PACKAGE symbol
;;------------------------------------------------------------------------------
(defun symbol-package (sym)
  (typecase sym
    (rt::sym (rt::symbol-package sym))
    (null *nil-package*)
    (T (error SYM_EXPECTED sym))))

;;------------------------------------------------------------------------------
;; SYMBOL-PACKAGE-INDEX symbol
;;------------------------------------------------------------------------------
(defun symbol-package-index (sym)
  (typecase sym
    (rt::sym (rt::symbol-package sym))
    (null *nil-package*)
    (T nil)))

;;------------------------------------------------------------------------------
;; SET-SYMBOL-PACKAGE symbol package
;;------------------------------------------------------------------------------
(defun set-symbol-package (sym package)
  (typecase sym
    (rt::sym (setf (rt::symbol-package sym) package))
    (null (setf *nil-package* package))))

;;------------------------------------------------------------------------------
;; BOUNDP object
;;------------------------------------------------------------------------------
(defun boundp (object)
  (typecase object
    (rt::sym (not (rt::unbound-value-p (rt::symbol-value object))))
    (null T)
    (T (error SYM_EXPECTED object))))

;;------------------------------------------------------------------------------
;; MAKUNBOUND sym
;;------------------------------------------------------------------------------
(defun makunbound (sym)
  (typecase sym
    (rt::sym (if (rt::constant-flag-p sym)
                 (error TRY_CHANGE_CONST sym)
                 (setf (rt::symbol-value sym) (rt::unbound-value))))
    (null (error TRY_CHANGE_CONST sym))
    (T (error SYM_EXPECTED sym)))
  sym)
