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
;;; Funktion : Eine Implementation der Fakultaetsfunktion.
;;;            Verwendet den Y-Kombinator, um Rekursion auszudruecken.
;;;
;;; $Revision: 1.8 $
;;; $Id: y-fac.lisp,v 1.8 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "USER")

;;------------------------------------------------------------------------------
;; Implementation der Fakultaetsfunktions mittels des Y-Kombinators
;;------------------------------------------------------------------------------
(defun fak (n)
  (let* ((Y #'(lambda (f)
                (funcall #'(lambda (g) (funcall g g))
                         #'(lambda (x)
                             (funcall f #'(lambda ()
                                            (funcall x x)))))))
         (fac (funcall Y #'(lambda (fg)
                             #'(lambda (n)
                                 (if (= n 0)
                                     1
                                     (* n (funcall (funcall fg) (- n 1)))))))))
    (funcall fac n)))


;;------------------------------------------------------------------------------
;; Einige Testaufrufe dazu:
;;------------------------------------------------------------------------------
(clicc-test "Y-fac" 
            ";;; Here fac is implemented with the Y Combinator."
            (((fak  0)         1)
             ((fak  1)         1)
             ((fak  3)         6)
             ((fak  4)        24)
             ((fak  5)       120)
             ((fak  6)       720)
             ((fak  7)      5040)
             ((fak  8)     40320)
             ((fak  9)    362880)
             ((fak 10)   3628800)
             ((fak 11)  39916800)
             ((fak 12) 479001600)
             )
            :test #'eql
            )
                  
;;------------------------------------------------------------------------------
;; Implementation der Fakultaetsfunktions mittels des Y-Kombinators
;; wobei die Zahl n als Liste der L"ange n dargestellt wird.
;;------------------------------------------------------------------------------
(defun incl   (l) (cons 1 l))
(defun decl   (l) (cdr l))
(defun addl   (m n) (if (zerolp m) n (addl (decl m) (incl n))))
(defun multl  (m n) (if (onelp m) n (addl n (multl (decl m) n))))
(defun zerolp (l) (null l))
(defun onelp  (l) (zerolp (decl l)))
(defconstant zerol '())

(defun fakl (n)
  (let* ((Y #'(lambda (f)
                (funcall #'(lambda (g) (funcall g g))
                         #'(lambda (x)
                             (funcall f #'(lambda ()
                                            (funcall x x)))))))
         (fac (funcall Y #'(lambda (fg)
                             #'(lambda (&rest n)
                                 (if (zerolp n)
                                     (incl zerol)
                                     (multl n (apply (funcall fg) (decl n)))))))))
    (apply fac n)))

;;------------------------------------------------------------------------------
;; Einige Testaufrufe dazu:
;;------------------------------------------------------------------------------
(clicc-test "Y-fac-list" 
";;; Here fac is implemented with the Y Combinator and numbers are ~%
implemented as lists"
            (((fakl '())      (1))
             ((fakl '(1))     (1))
             ((fakl '(1 1 1)) (1 1 1 1 1 1))
             )
            :test #'equal
            )
                       
;;------------------------------------------------------------------------------

(provide "y-fac")
