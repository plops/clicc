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
;;; Function : ERROR, WARN, BREAK
;;;
;;; $Revision: 1.9 $
;;; $Id: error.lisp,v 1.9 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(error warn break))

(defparameter *error-print-length* 6)
(defparameter *error-print-level* 6)

;;------------------------------------------------------------------------------
(defun error (format-string &rest args)
  (let ((*print-length* *error-print-length*)
        (*print-level* *error-print-level*))
  (format *error-output* "~&Error: ~?~%" format-string args))
  (throw 'ERROR-CATCHER -1))

;;------------------------------------------------------------------------------
(defun error-in (caller format-string &rest args)
  (error "~&Error in ~A: ~?~%" caller format-string args))

;;------------------------------------------------------------------------------
(defun warn (format-string &rest args)
  (let ((*print-length* *error-print-length*)
        (*print-level* *error-print-level*))
    (format *error-output* "~&Warning: ~?~%" format-string args)))

;;------------------------------------------------------------------------------
(defun break (&optional format-string &rest args)
  (format *error-output* "~&Break: call to the `break' function.~%~?~%" 
          format-string args)
  (if (y-or-n-p "Restart actions:~% y: continue~% n: abort~%")
      nil
    (throw 'ERROR-CATCHER -1)))
