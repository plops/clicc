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
;;; Funktion : Predicates
;;;            - TYPEP, SUBTYPEP in sehr eingeschraenkter Form.
;;;            - Praedikate zum Testen, ob ein Objekt eines bestimmten Typs
;;;              vorliegt. Die Definition dieser Funktionen scheinen zirkulaer
;;;              zu sein. Der Rumpf wird allerdings von CLICC inline
;;;              uebersetzt, so dass der richtige C-Code erzeugt wird.
;;;            - Funktionen zum Testen gewisser Eigenschaften von Fixnums und
;;;              Arrays.
;;;
;;; $Revision: 1.11 $
;;; $Id: pred.lisp,v 1.11 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(typep subtypep not null))
(export '(rt::check-array rt::check-simple-array) "RT")

;;-----------------------------------------------------------------------------
;; TYPEP object type
;;-----------------------------------------------------------------------------
(defun typep (object type)
  (case type
    ((T) T)
    ((NIL) NIL)
    (fixnum
     (check-integer object most-negative-fixnum most-positive-fixnum))
    ((float short-float single-float double-float long-float) (floatp object))
    ((character standard-char) (characterp object))
    ((list) (listp object))
    ((null) (null object))
    ((cons) (consp object))
    (otherwise (error "(TYPEP ~S ~S) is not implemented" object type))))

;;-----------------------------------------------------------------------------
;; SUBTYPEP type1 type2
;;-----------------------------------------------------------------------------
(defun subtypep (type1 type2)
  (case type2
    ((T) t)
    (otherwise (error "(SUBTYPEP ~S ~S) is not implemented" type1 type2))))

;;------------------------------------------------------------------------------
;; Fuer weitere Praedikate siehe inline.lisp
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; NOT object / NULL object
;;------------------------------------------------------------------------------
(defun not (object)
  (if object nil t))

(defun null (object)
  (if object nil t))

;;-----------------------------------------------------------------------------
(defun check-integer (object low high)
  (and (integerp object)
       (<= low object high)))

;;-----------------------------------------------------------------------------
;; CHECK-ARRAY object element-type dimensions
;;-----------------------------------------------------------------------------
(defun rt:check-array (object element-type dimensions)
  (and (arrayp object)
       (check-array-internal object element-type dimensions)))

;;-----------------------------------------------------------------------------
;; CHECK-SIMPLE-ARRAY object element-type dimensions
;;-----------------------------------------------------------------------------
(defun rt:check-simple-array (object element-type dimensions)
  (and (simple-array-p object)
       (check-array-internal object element-type dimensions)))

;;------------------------------------------------------------------------------
(defun check-array-internal (object element-type dimensions)
  (and (or (eq element-type '*)
           (equal (array-element-type object)
                  (upgraded-array-element-type element-type)))
       (or (eq dimensions '*)
           (if (atom dimensions)
             (= (array-rank object) dimensions)
             (and (= (array-rank object) (length dimensions))
                  (let (dimension)
                    (dotimes (i (length dimensions) t)
                      (setq dimension (pop dimensions))
                      (unless (or (eq dimension '*)
                                  (= (array-dimension object i) dimension))
                        (return nil)))))))))
