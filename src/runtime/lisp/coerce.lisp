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
;;; Function : coerce
;;;
;;; $Revision: 1.8 $
;;; $Id: coerce.lisp,v 1.8 1994/11/29 16:01:13 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(coerce))

;;------------------------------------------------------------------------------
;; Einschraenkungen:
;; - SEQUENCE : siehe CONCATENATE
;; - CHARACTER: keine Umwandlung INTEGER --> CHARACTER (CLtL, 2. Ed., S. 64)
;; - COMPLEX  : entfaellt
;; - FUNCTION : SYMBOL --> FUNCTION nicht implementiert
;;              (LAMBDA ...) --> FUNCTION entfaellt
;;------------------------------------------------------------------------------
(defun coerce (object type)
  (case type
    (list (if (listp object)
              object
              (concatenate 'list object)))
    (simple-string (if (simple-string-p object)
                       object
                       (concatenate 'simple-string object)))
    (string (if (stringp object)
                object
                (concatenate 'string object)))
    (simple-vector (if (simple-vector-p object)
                       object
                       (concatenate 'simple-vector object)))
    (vector (if (vectorp object)
                object
                (concatenate 'vector object)))
    (character (character object))
    ((float short-float single-float double-float long-float) (float object))
    ((t) object)
    (t (error "cannot coerce to ~S" type))))



