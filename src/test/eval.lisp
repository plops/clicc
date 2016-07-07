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
;;; Inhalt   : Run time eval
;;;
;;; $Revision: 1.2 $
;;; $Id: eval.lisp,v 1.2 1995/11/29 15:36:15 uho Exp $
;;;-----------------------------------------------------------------------------

(unless (find-package "EVAL") (make-package "EVAL" :use NIL))
(in-package "EVAL" :use NIL)

(lisp:import '(lisp:defmacro lisp:&rest lisp:progn lisp:defun lisp:quote
               lisp:if lisp:null lisp:setf lisp:getf lisp:symbol-plist
               lisp:car lisp:function lisp:cdr
               lisp:let lisp:unless lisp:error lisp:cond lisp:consp
               lisp:mapcar lisp:nil lisp:T lisp:symbolp 
               lisp:symbol-value))

(defmacro declare-funcallable (&rest syms)
  `(PROGN ,@(make-function-association syms)))

(defun make-function-association (syms) 
  (if (null syms) 
      nil 
      `((SETF 
         (GETF (SYMBOL-PLIST (QUOTE ,(car syms))) 'EVAL-FUNCTION) 
         (FUNCTION ,(car syms)))
        ,@(make-function-association (cdr syms)))))

(defun symbol-function (sym)
  (let ((fun (getf (symbol-plist sym) 'EVAL-FUNCTION)))
    (unless fun (error "Undeclared funcallable function ~a~%" sym))
    fun))

(defun fboundp (sym)
  (getf (symbol-plist sym) 'EVAL-FUNCTION))


(defun eval (sexpr)
  (cond
    ((consp sexpr) (lisp:apply (symbol-function (car sexpr))
                               (mapcar #'eval (cdr sexpr))))
    ((symbolp sexpr) (symbol-value sexpr))
    (T sexpr)))

(defun funcall (f &rest args)
  (if (symbolp f)
      (lisp:apply (symbol-function f) args)
      (lisp:apply f args)))

(defun apply (f &rest args)
  (if (symbolp f)
      (lisp:apply #'lisp:apply (symbol-function f) args)
      (lisp:apply #'lisp:apply f args)))


(lisp:export '(declare-funcallable symbol-function fboundp eval funcall apply))


(lisp:provide "eval")
