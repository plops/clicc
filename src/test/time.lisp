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
;;; Content  : Definition of the time macro, which is not supported by CLiCC
;;;            because it is considered to be part of the development
;;;            environment
;;;
;;; $Revision: 1.2 $
;;; $Id: time.lisp,v 1.2 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------
(in-package "USER" :use '("LISP" "FFI"))

(load-foreign "time.def")

(defun ticks ()
  (let* ((tmsbuf (make-tms))
         (current-time (times tmsbuf))
         (utime (tms-utime tmsbuf))
         (stime (tms-stime tmsbuf)))
    (free tmsbuf)
    (values (lisp-integer utime) (lisp-integer stime))))
  
(defun print-time (u0 u1 s0 s1)
  (labels
      ((rounded (x)
         (/ (round (* 10 x)) 10)))
    (let* ((hz 60)
           (ut (rounded (/ (- u1 u0) hz)))
           (st (rounded (/ (- s1 s0) hz)))
           (tot (+ ut st)))
      (format T "~%user ~A sec, system ~A sec, sum ~A sec~%"
              ut st tot))))

(defmacro time (&rest form)
  (let ((u0 (gensym))
        (s0 (gensym))
        (rslt (gensym))
        (u1 (gensym))
        (s1 (gensym)))
    `(multiple-value-bind (,u0 ,s0)
      (ticks)
      (let ((,rslt (progn ,@form)))
        (multiple-value-bind (,u1 ,s1)
            (ticks)
          (print-time ,u0 ,u1 ,s0 ,s1))
          ,rslt))))
