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
;;; Function : Structures
;;;
;;; $Revision: 1.15 $
;;; $Id: struct.lisp,v 1.15 1994/11/22 14:55:56 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(rt::struct rt::struct-typep rt::struct-type rt::make-struct rt::copy-struct
   rt::struct-ref rt::struct-constructor rt::struct-printer
   rt::included-struct) "RT")

;;------------------------------------------------------------------------------
;; Für Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant NO_STRUCT "~S is not a structure of type ~S")

;;------------------------------------------------------------------------------
;; RT::STRUCT Datentyp
;;------------------------------------------------------------------------------
(deftype rt:struct () '(satisfies rt::structp))

;;-----------------------------------------------------------------------------
;; RT::STRUCT-TYPEP object type
;;-----------------------------------------------------------------------------
(defun rt:struct-typep (object type)
  (if (rt::structp object)
      (let ((struct-type (rt::struct-type object)))
        (loop
         (when (eq type struct-type)
           (return object))
         (unless (setq struct-type (get struct-type 'rt::included-struct))
           (return nil))))))

;;------------------------------------------------------------------------------
;; RT::STRUCT-TYPE structure
;;------------------------------------------------------------------------------
(defun rt:struct-type (structure)
  (rt::structure-ref structure -1))

;;-----------------------------------------------------------------------------
;; RT::MAKE-STRUCT type &rest slot-values
;;-----------------------------------------------------------------------------
(defun rt:make-struct (type &rest slot-values)
  (let ((new-structure (rt::new-struct (length slot-values))))
    (setf (rt::structure-ref new-structure -1) type)
    (let ((i 0))
      (dolist (slot slot-values new-structure)
        (setf (rt::structure-ref new-structure i) slot)
        (incf i)))))

;;-----------------------------------------------------------------------------
;; RT::COPY-STRUCT structure type
;;-----------------------------------------------------------------------------
(defun rt:copy-struct (structure type)
  (unless (rt:struct-typep structure type)
    (error NO_STRUCT structure type))
  (let* ((structure-size (rt::struct-size structure))
         (copy-of-structure (rt::new-struct structure-size)))
    (setf (rt::structure-ref copy-of-structure -1) type)
    (dotimes (index structure-size copy-of-structure)
      (setf (rt::structure-ref copy-of-structure index)
            (rt::structure-ref structure index)))))

;;-----------------------------------------------------------------------------
;; RT::STRUCT-REF structure index type
;;-----------------------------------------------------------------------------
(defun rt:struct-ref (structure index type)
  (if (rt:struct-typep structure type)
      (rt::structure-ref structure index)
      (error NO_STRUCT structure type)))

;;-----------------------------------------------------------------------------
;; (SETF RT::STRUCT-REF) newvalue structure index type
;;-----------------------------------------------------------------------------
(defun (setf rt:struct-ref) (newvalue structure index type)
  (if (rt:struct-typep structure type)
      (setf (rt::structure-ref structure index) newvalue)
      (error NO_STRUCT structure type)))

;;-----------------------------------------------------------------------------
;; RT::STRUCT-CONSTRUCTOR symbol
;;-----------------------------------------------------------------------------
(defun rt:struct-constructor (symbol)
  (get symbol 'rt::struct-constructor))

;;-----------------------------------------------------------------------------
;; (SETF RT::STRUCT-CONSTRUCTOR) constructor symbol
;;-----------------------------------------------------------------------------
(defun (setf rt:struct-constructor) (constructor symbol)
  (setf (get symbol 'rt::struct-constructor) constructor))

;;-----------------------------------------------------------------------------
;; RT::STRUCT-PRINTER symbol
;;-----------------------------------------------------------------------------
(defun rt:struct-printer (symbol)
  (get symbol 'rt::struct-printer))

;;-----------------------------------------------------------------------------
;; (SETF RT::STRUCT-PRINTER) print-function symbol
;;-----------------------------------------------------------------------------
(defun (setf rt:struct-printer) (print-function symbol)
  (setf (get symbol 'rt::struct-printer) print-function))

