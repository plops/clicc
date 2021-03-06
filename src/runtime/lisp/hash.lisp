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
;;; Function : Hash Tables
;;;
;;; $Revision: 1.14 $
;;; $Id: hash.lisp,v 1.14 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(hash-table hash-table-p make-hash-table gethash remhash maphash clrhash
   sxhash))

;;------------------------------------------------------------------------------
(defstruct (hash-table (:constructor new-hash-table)
                       (:copier nil)
                       (:predicate hash-table-p))
  (size               0   :type integer)
  (count              0   :type integer)
  (rehash-size      1.5   :type (or integer float))
  (rehash-threshold 0.7   :type float)
  (test             #'eql :type function)
  (array            nil   :type (or null array)))

;;------------------------------------------------------------------------------
;; MAKE-HASH-TABLE &key :test :size :rehash-size :rehash-threshold
;;------------------------------------------------------------------------------
(defun make-hash-table (&key (test #'eql)
                             (size 53)
                             (rehash-size 1.5)
                             (rehash-threshold .7))
  (typecase test
    (symbol (case test
              (eq (setq test #'eq))
              (eql (setq test #'eql))
              (equal (setq test #'equal))
              (equalp (setq test #'equalp))
              (t (error "test ~a is not a valid test-function name" test))))
    (function nil)
    (t (error "test ~a in make-hash-table should be a function or symbol"
              test)))
  (cond
    ((integerp rehash-size)
     (setq rehash-size (+ 1.0 (/ size rehash-size))))
    ((floatp rehash-size)
     (when (>= 1.0 rehash-size)
       (error "rehash-size ~A is not a floating-point number > 1.0"
              rehash-size)))
    (t (error "rehash-size ~A is not a floating-point number or integer"
              rehash-size)))
  
  (unless (and (floatp rehash-threshold) ;!!! realp
               (>= rehash-threshold 0.0)
               (<= rehash-threshold 1.0))
    (error "rehash-threshold ~A is not a floating-point ~
            number between 0 and 1" rehash-threshold))
  (labels ((prime-ceiling (x)
             (declare (fixnum x))
             (if (= (rem x 2) 0) (setq x (+ 1 x)))
             (if (= (rem x 3) 0) (setq x (+ 2 x))) 
             (if (= (rem x 5) 0) (setq x (+ 4 x)))
             x))
    (let ((real-size (prime-ceiling size)))
      (new-hash-table :size real-size
                      :count 0
                      :rehash-size rehash-size
                      :rehash-threshold rehash-threshold
                      :test test
                      :array (make-array real-size :initial-element ())))))
                     
;;------------------------------------------------------------------------------
;; GETHASH key hash-table &optional default
;;------------------------------------------------------------------------------
(defun gethash (key hash-table &optional default)
  (let* ((hash-value (mod (sxhash key) (hash-table-size hash-table)))
         (entry (find key (aref (hash-table-array hash-table) hash-value)
                      :test (hash-table-test hash-table)
                      :key  #'car)))
    (if entry
        (values (cdr entry)   T)
        (values default     NIL))))

;;------------------------------------------------------------------------------
;; (SETF GETHASH) newvalue key hash-table
;;------------------------------------------------------------------------------
(defun (setf gethash) (newvalue key hash-table &optional default)
  (declare (ignore default))
  (let* ((hash-table-size (hash-table-size hash-table))
         (hash-value (mod (sxhash key) hash-table-size))
         (entry (find key (aref (hash-table-array hash-table) hash-value)
                      :test (hash-table-test  hash-table)
                      :key  #'car)))
    (cond
      
      ;; Eintrag ueberschreiben
      ;;-----------------------
      (entry
       (setf (cdr entry) newvalue))

      ;; Table ist voll, rehash
      ;;-----------------------
      ((> (/ (1+ (hash-table-count hash-table)) hash-table-size)
          (hash-table-rehash-threshold hash-table))
       (let* ((hash-table-rehash-size
               (hash-table-rehash-size hash-table))
              (new-hash-table-size
               (if (integerp hash-table-rehash-size)
                   (+ hash-table-size hash-table-rehash-size)
                   (ceiling (* hash-table-size hash-table-rehash-size))))
              (new-hash-table
               (make-hash-table :size new-hash-table-size
                                :test (hash-table-test hash-table)
                                :rehash-threshold
                                (hash-table-rehash-threshold hash-table))))
         (maphash #'(lambda (key value)
                      (setf (gethash key new-hash-table) value))
                  hash-table)
         (setf (gethash key new-hash-table) newvalue)
         (setf (hash-table-size hash-table)
               (hash-table-size new-hash-table))
         (setf (hash-table-count hash-table) (hash-table-count new-hash-table))
         (setf (hash-table-array hash-table) (hash-table-array new-hash-table))
         newvalue))

      ;; Wert hinzufuegen
      ;;-----------------
      (t (incf (hash-table-count hash-table))
         (push (cons key newvalue)
               (aref (hash-table-array hash-table) hash-value))
         newvalue))))

;;------------------------------------------------------------------------------
;; REMHASH key hash-table
;;------------------------------------------------------------------------------
(defun remhash (key hash-table)
  (let ((hash-value (mod (sxhash key) (hash-table-size hash-table))))
    (setf (aref (hash-table-array hash-table) hash-value)
          (delete key (aref (hash-table-array hash-table) hash-value)
                  :test (hash-table-test hash-table)
                  :key #'car))))

;;------------------------------------------------------------------------------
;; MAPHASH function hash-table
;;------------------------------------------------------------------------------
(defun maphash (function hash-table)
  (map nil
       #'(lambda (bucket)
           (mapc #'(lambda (entry)
                     (funcall function (car entry) (cdr entry)))
                 bucket))
       (hash-table-array hash-table))
  nil)

;;------------------------------------------------------------------------------
;; CLRHASH hash-table
;;------------------------------------------------------------------------------
(defun clrhash (hash-table)
  (let ((array (hash-table-array hash-table)))
    (setf (hash-table-count hash-table) 0)
    (dotimes (i (length array))
      (setf (aref array i) nil))
    hash-table))

;;------------------------------------------------------------------------------
;; HASH-TABLE-COUNT hash-table
;;------------------------------------------------------------------------------
;; Generated by defstruct

;;------------------------------------------------------------------------------
;; SXHASH object
;;------------------------------------------------------------------------------
(defun sxhash (object)
  "Computes a hash code for S-EXPR and returns it as an non negative fixnum."
  (internal-sxhash object 0))

;; The maximum length and depth to which we hash lists
;;----------------------------------------------------
(defconstant sxhash-max-len 7)
(defconstant sxhash-max-depth 3)

(defmacro sxhash-list (sequence depth)
  `(if (= ,depth sxhash-max-depth)
       0
       (do ((sequence ,sequence (cdr sequence))
	    (index 0 (1+ index))
	    (hash 2))
	   ((or (atom sequence) (= index sxhash-max-len)) hash)
	 (combine-hash hash (internal-sxhash (car sequence) (1+ ,depth))))))

(defun combine-hash (a b)
  (logxor a b))

(defun internal-sxhash (object depth)
  (typecase object
    (symbol (rt::sxhash-string (symbol-name object)))
    (string (rt::sxhash-string (string-to-simple-string object)))
    (fixnum object)
    (float (truncate object))
    (character (char-code object))
    (list (sxhash-list object depth))
    (rt::struct (internal-sxhash (rt::struct-type object) depth))
    (array (array-rank object))
    (t 42)))

;;------------------------------------------------------------------------------
;; STRING-HASH string hash-size
;;------------------------------------------------------------------------------
(defun string-hash (string hash-size)
  (mod (rt::sxhash-string (string-to-simple-string string)) hash-size))
