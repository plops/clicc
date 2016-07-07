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
;;; Inhalt   : Test for Run time eval
;;;
;;; $Revision: 1.1 $
;;; $Id: evaltest.lisp,v 1.1 1995/11/28 15:14:03 uho Exp $
;;;-----------------------------------------------------------------------------

(require "eval")


(defvar one 1)
(defvar two 2)

(defun fac (n)
  (if (= 0 n)
      1
      (* n (fac (- n 1)))))

(eval:declare-funcallable + - * / fac)

(loop
 (format t "Input Prefix Arithmetic Expression: ")
 (format t "~% Result = ~A~%" (eval:eval (read))))

