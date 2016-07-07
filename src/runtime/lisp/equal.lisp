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
;;; Function : equal, equalp
;;;
;;; $Revision: 1.9 $
;;; $Id: equal.lisp,v 1.9 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(equal equalp))

;;------------------------------------------------------------------------------
;; Einschraenkungen:
;; - BITVECTOR: entfaellt
;; - PATHNAME : entfaellt
;;------------------------------------------------------------------------------
(defun equal (x y)
  (declare (:simp-when-some-arg-not-cons/pathn/string/bitv eql))
  (typecase x
    (cons (and (consp y) (equal (car x) (car y)) (equal (cdr x) (cdr y))))
    (string (and (stringp y) (string= x y)))
    (T (eql x y))))

;;------------------------------------------------------------------------------
;; Einschraenkungen:
;; - HASHTABLE: entfaellt
;;------------------------------------------------------------------------------
(defun equalp (x y)
  (typecase x
    (character (and (characterp y) (char-equal x y)))
    (number (and (numberp y) (= x y)))
    (cons (and (consp y) (equalp (car x) (car y)) (equalp (cdr x) (cdr y))))
    (vector (and (vectorp y)
                 
                 ;; LENGTH beachtet Fill-Pointer
                 ;;-----------------------------
                 (eql (length x) (length y))
                 (dotimes (i (length x) t)
                   (unless (equalp (aref x i) (aref y i))
                     (return nil)))))
    (array (and (arrayp y) (eql (array-rank x) (array-rank y))
                (dotimes (i (array-rank x) t)
                  (unless (eql (array-dimension x i) (array-dimension y i))
                    (return nil)))
                (dotimes (i (array-total-size x) t)
                  (unless (equalp (row-major-aref x i) (row-major-aref y i))
                    (return nil)))))
;;  (hashtable ...)
    (rt:struct (and (rt::structp y)
                 (eq (rt::struct-type x) (rt::struct-type y))
                 (let ((type (rt::struct-type x)))
                   (dotimes (i (rt::struct-size x) t)
                     (unless (equalp (rt::struct-ref x i type)
                                     (rt::struct-ref y i type))
                       (return nil))))))

    (T (eq x y))))
