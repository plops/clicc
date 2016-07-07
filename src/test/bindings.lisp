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
;;; Funktion : Tests zu Wertbindungen, z. B. in let/let*-Ausdruecken
;;;
;;; $Revision: 1.5 $
;;; $Id: bindings.lisp,v 1.5 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "USER")

;;------------------------------------------------------------------------------
;; Funktionsdefinitionen zu den Bindungstests:
;;------------------------------------------------------------------------------


;;------------------------------------------------------------------------------
;; Nur statisch gebundene Variablen:
;;------------------------------------------------------------------------------
(defun bindings1a (a b)
  (let ((a b)
        (b a))
    (list a b)))

;;------------------------------------------------------------------------------
;; special-Deklaration vor einem let-Ausdruck:
;;------------------------------------------------------------------------------
(defun bindings2a (a b)
  (declare (special a b))
  (let ((a b)
        (b a))
    (list a b)))


;;------------------------------------------------------------------------------
;; special-Deklaration in einem let-Ausdruck:   [Ste84]
;;------------------------------------------------------------------------------
(defun bindings3a (a b)
  (let ((c a)
        (d b))
    (declare (special c d))
    (let ((c d)
          (d c))
      (list c d))))

;;------------------------------------------------------------------------------
;; special-Deklaration vor und in einem let-Ausdruck:   
;;------------------------------------------------------------------------------
(defun bindings4a (a b)
  (declare (special a b))
  (let ((a b)
        (b a))
    (declare (special a b))
    (list a b)))

;;------------------------------------------------------------------------------
;; Beispiel aus dem draft zu  ANSI Common Lisp:
;;------------------------------------------------------------------------------
(defun bindings5a ()
  (let ((x 1))
    (declare (special x))
    (let ((x 2))
      (let ((old-x x)
            (x 3))
        (declare (special x))
        (list old-x x)))))

;;------------------------------------------------------------------------------
;; globale special-Deklaration und lokale special Deklaration
;;------------------------------------------------------------------------------
(defvar *c* 77)
(defun bindings6a (a b *c* d)
  (declare (special a))
  (let ((a b)
        (b a)
        (*c* d)
        (d *c*))
    (declare (special a))
    (list a b *c* d)))

;;------------------------------------------------------------------------------
;; Nur statisch gebundene Variablen:
;;------------------------------------------------------------------------------
(defun bindings1b (a b)
  (let* ((a b)
         (b a))
    (list a b)))

;;------------------------------------------------------------------------------
;; special-Deklaration vor einem let-Ausdruck:
;;------------------------------------------------------------------------------
(defun bindings2b (a b)
  (declare (special a b))
  (let* ((a b)
         (b a))
    (list a b)))


;;------------------------------------------------------------------------------
;; special-Deklaration in einem let-Ausdruck:   [Ste84]
;;------------------------------------------------------------------------------
(defun bindings3b (a b)
  (let* ((c b)
         (d a))
    (declare (special c d))
    (let* ((c d)
           (d c))
      (list c d))))

;;------------------------------------------------------------------------------
;; special-Deklaration vor und in einem let-Ausdruck:   
;;------------------------------------------------------------------------------
(defun bindings4b (a b)
  (declare (special a b))
  (let* ((a b)
         (b a))
    (declare (special a b))
    (list a b)))

;;------------------------------------------------------------------------------
;; Beispiel aus dem draft zu  ANSI Common Lisp:
;;------------------------------------------------------------------------------
(defun bindings5b ()
  (let* ((x 1))
    (declare (special x))
    (let* ((x 2))
      (let* ((old-x x)
             (x 3))
        (declare (special x))
        (list old-x x)))))

;;------------------------------------------------------------------------------
;; Aufruf der Bindungstests:
;;------------------------------------------------------------------------------
(clicc-test "Binding" 
            ";;; Tests variable bindings introduced by let and let*."
            (((bindings1a 1 2)    (2 1))
             ((bindings2a 3 4)    (4 3))
             ((bindings3a 5 6)    (6 5))
             ((bindings4a 7 8)    (8 7))
             ((bindings5a)        (1 3) "uses [Ste84]'s declaration scope")
             ((bindings5a)        (2 3) "uses ANSI CL's declaration scope")
             ((bindings6a 1 2 3 4) (2 1 4 3))
             ((bindings1b 1 2)    (2 2))
             ((bindings2b 3 4)    (4 4))
             ((bindings3b 5 6)    (5 5))
             ((bindings4b 7 8)    (8 8))
             ((bindings5b)        (1 3) "uses [Ste84]'s declaration scope")
             ((bindings5b)        (2 3) "uses ANSI CL's declaration scope")
             ))
              


;;------------------------------------------------------------------------------

(provide "bindings")
