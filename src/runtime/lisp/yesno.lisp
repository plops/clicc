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
;;; Funktion : Y-OR-N-P, YES-OR-NO-P
;;;
;;; $Revision: 1.5 $
;;; $Id: yesno.lisp,v 1.5 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(y-or-n-p yes-or-no-p))

;;------------------------------------------------------------------------------
(defun y-or-n-p (&optional format-string &rest args &aux answer)
  (when format-string
    (format *query-io* "~&~?" format-string args) )
  (loop
    (princ "(y or n):" *query-io*)
    (setq answer (read-line *query-io*))
    (cond
      ((string-equal "y" answer) (return t))
      ((string-equal "n" answer) (return nil))
      (T (print "Please answer y or n." *query-io*)) )))

;;------------------------------------------------------------------------------
(defun yes-or-no-p (&optional format-string &rest args &aux answer)
  (when format-string
    (format *query-io* "~&~?" format-string args) )
  (loop
    (princ "(yes or no):" *query-io*)
    (setq answer (read-line *query-io*))
    (cond
      ((string-equal "yes" answer) (return t))
      ((string-equal "no" answer) (return nil))
      (T (print "Please answer yes or no." *query-io*)) )))
