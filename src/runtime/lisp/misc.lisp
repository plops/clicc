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
;;; Function : Miscellaneous Features
;;;
;;; $Revision: 1.6 $
;;; $Id: misc.lisp,v 1.6 1994/11/22 14:55:56 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export '(identity documentation))

;;-----------------------------------------------------------------------------
;; IDENTITY object
;;-----------------------------------------------------------------------------
(defun identity (object) object)

;;------------------------------------------------------------------------------
;; DOCUMENTATION symbol doc-type
;; a dummy function, nil means that no documentation exists.
;;------------------------------------------------------------------------------
(defun documentation (symbol doc-type)
  (declare (ignore symbol doc-type))
  nil)

;;------------------------------------------------------------------------------
;; (setf DOCUMENTATION) new-doc symbol doc-type
;; a dummy function, the new documentation is not used to update anything
;;------------------------------------------------------------------------------
(defun (setf documentation) (new-doc symbol doc-type)
  (declare (ignore symbol doc-type))
  new-doc)

