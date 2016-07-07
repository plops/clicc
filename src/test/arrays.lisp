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
;;; Funktion : Einfache Routinen zum Testen von Arrays.
;;;
;;; $Revision: 1.4 $
;;; $Id: arrays.lisp,v 1.4 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "USER")

;;------------------------------------------------------------------------------
;; Anlegen, Displacen, Ajdusten und Zugriff auf Arrays.
;;------------------------------------------------------------------------------

(defun displace-arrays ()
  (let (a1 a2 c)
    (setq a1 (make-array 6 :adjustable t
                         :initial-contents '"Drache"))
    (setq a2 (make-array '(1 1 2) :displaced-to a1
                         :displaced-index-offset 3))
    (setf c (aref a2 0 0 0))
    (setf (aref a2 0 0 0) (aref a2 0 0 1))
    (setf (aref a2 0 0 1) c)
    (coerce (adjust-array a1 5) 'string)))

;;------------------------------------------------------------------------------
;; Fillpointer.
;;------------------------------------------------------------------------------

(defun fptest ()
  (let* ((v (make-array 3 :fill-pointer 1))
         (r (list (not (null (array-has-fill-pointer-p v))) (fill-pointer v))))
    (setf (fill-pointer v) 0)
    (setq r (append r (list (length v) (array-total-size v))))
    (setf (fill-pointer v) 2)
    (append r (list (length v) (fill-pointer v)))))


;;------------------------------------------------------------------------------
;; In Testmain einbinden
;;------------------------------------------------------------------------------
(clicc-test "Array1"
            ";;; Tests creation, displacing, adjusting and accessing of arrays"
            (((displace-arrays) "Drahc")))

(clicc-test "Vector1"
            ";;; Tests the handling of fill-pointers"
            (((fptest) (t 1 0 3 2 2))))

(provide "arrays")
