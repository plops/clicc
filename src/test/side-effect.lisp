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
;;; Inhalt   : Tests zu Seiteneffekten.
;;;
;;; $Revision: 1.7 $
;;; $Id: side-effect.lisp,v 1.7 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "USER")

;;------------------------------------------------------------------------------
;; Definitionen zu den Bindungstests:
;;------------------------------------------------------------------------------

(defvar *global*)

;;------------------------------------------------------------------------------
;; Seiteneffekt auf globale Variable in globaler Funktion.
;;------------------------------------------------------------------------------
(defun se1a (a b)
  (setf *global* 'wow)
  (+ a b))

(defun se1b ()
  (let ((*global* 1))
    (se1a 2 3)
    (if (numberp *global*)
        'wrong
        'right)))


;;------------------------------------------------------------------------------
;; Seiteneffekt auf globale Variable in lokaler Funktion.
;;------------------------------------------------------------------------------
(defun se2a (a b)
  (labels ((se2aa (l) 
             (dolist (e l)
               (setf *global* 'wow)
               (+ a b))))

    (se2aa '(1 2 3))))

(defun se2b ()
  (setf *global* 1)
  (se2a 1 2)
  (if (numberp *global*)
      'wrong
      'right))

;;------------------------------------------------------------------------------
;; Seiteneffekt auf lokale Variable in lokaler Funktion
;;------------------------------------------------------------------------------
(defun se3 ()
  (let ((local '(1 2))
        (sum   0))

    (labels ((se3a ()
               (dolist (e local)
                 (unless (eq e 'a)
                   (return (setf local 'wow)))
                 (incf sum))))

      (se3a)
      (if (listp local)
          'wrong
          'right))))

             

;;------------------------------------------------------------------------------
;; Seiteneffekt auf globale Variable im Initialisierungsausdruck eines 
;; optionalen Parameters.
;;------------------------------------------------------------------------------
(defun se4a (a &optional (b (progn (setf *global* 'wow) 3)))
  (cons a b))

(defun se4b ()
  (let ((*global* 1))
    (se4a 1)
    (if (numberp *global*)
        'wrong
        'right)))


;;------------------------------------------------------------------------------
;; Destruktiver Seiteneffekt auf eine Liste.
;;------------------------------------------------------------------------------
(defun se5a (l)
  (setf (first l) 'wow))


(defun se5b (a)
  (let ((b (cons '3 a)))
    (se5a a)
    (if (numberp (second b))
        'wrong
        'right)))

;;------------------------------------------------------------------------------
;; Seiteneffekt auf eine globale Variable im Initialisierungsausdruck des
;; Slots einer Klasse.
;;------------------------------------------------------------------------------
(defclass se6-class ()
  ((slot1 :initform (progn (se1a 1 2) 3))))


(defun se6 ()
  (let ((*global* 1))
    (make-instance 'se6-class)
    (if (numberp *global*)
      'wrong
        'right)))

;;------------------------------------------------------------------------------
;; Kontrollfluss-Seiteneffekt
;;------------------------------------------------------------------------------
(defun se7 ()
  (let ((v 'right))
   (tagbody
      (let ((x (go end)))
        (setq v 'wrong)
        (setq v x))
    end)
   v))


;;------------------------------------------------------------------------------
;; Seiteneffekt in einer von maphash applizierten Funktion
;;------------------------------------------------------------------------------
(defun se8 ()
  (let ((ht (make-hash-table))
        (a nil))
    (setf (gethash 's ht) 'linear)
    (maphash #'(lambda (var value)
                 (IF (eq value 'linear)
                     (setq a (cons var a))))
             ht)
    (if a
        'right
        'wrong)))


;;------------------------------------------------------------------------------
;; Aufrufe der Seiteneffekttests:
;;------------------------------------------------------------------------------
(clicc-test 
 "Side effect" 
 ";;; Tests the combination of side effect analysis, type inference~%~
  ;;; and optimizations using the infered informations.~%~
  ;;; The functions contain different forms of side effects and ~%~
  ;;; type tests."
 (((se1b)        right "side effect in global function")
  ((se2b)        right "side effect in local function")
  ((se3)         right "side effect in local function")
  ((se4b)        right "side effect in initform")
  ((se5b '(2 1)) right "destructive side effect")
  ((se6)         right "side effect in slot-initform")
  ((se7)         right "side effect of go")
  ((se8)         right "side effect in maphash")
  ))
              
;;------------------------------------------------------------------------------

(provide "side-effect")
