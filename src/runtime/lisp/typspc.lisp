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
;;; Function : Type Specifiers
;;;
;;; $Revision: 1.8 $
;;; $Id: typspc.lisp,v 1.8 1994/11/22 14:55:56 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export '(rt::the-type-error) "RT")

;;------------------------------------------------------------------------------
(defun normalize-type (type)
  (if (atom type)
    (case type
      ((array simple-array vector simple-vector string simple-string)
       (normalize-type (list type)) )
      (t type) )
    (let (type-specifier
          element-type
          dimensions
          (args (rest type)) )
      (tagbody
         (case (car type)
           ((string simple-string)
            (setq type-specifier
                  (if (eq (car type) 'string)
                    'array
                    'simple-array ))
;;; !!! gibts nicht mehr in CLTL2            
;;;-------------------------------------------
;;;            (setq element-type 'string-char)
            (setq element-type 'character)
            (setq dimensions
                  (list (if (atom args)
                          '*
                          (pop args) )))
            (when (not (null args)) (go INVALID-SEQTYPE)) )

           (simple-vector
            (setq type-specifier 'simple-array)
            (setq element-type t)
            (setq dimensions
                  (list (if (atom args)
                          '*
                          (pop args) )))
            (when (not (null args)) (go INVALID-SEQTYPE)) )

           (vector
            (setq type-specifier 'array)
            (setq element-type
                  (if (atom args)
                    '*
                    (pop args) ))
            (setq dimensions
                  (list (if (atom args)
                          '*
                          (pop args) )))
            (when (not (null args)) (go INVALID-SEQTYPE)))

           ((array simple-array)
            (setq type-specifier (car type))
            (setq element-type
                  (if (atom args)
                    '*
                    (pop args) ))
            (setq dimensions
                  (if (atom args)
                    '*
                    (pop args) ))
            (when (not (null args)) (go INVALID-SEQTYPE)) )

           (t (go INVALID-SEQTYPE)) )

         (return-from normalize-type
           (list type-specifier element-type dimensions) )
       INVALID-SEQTYPE
         (error "~S is an invalid sequence typespecifier." type) ))))

;;------------------------------------------------------------------------------
;; THE-TYPE-ERROR value type
;; Die Funktion wird vom dem aus THE expandierten Code im Fehlerfall aufgerufen
;;------------------------------------------------------------------------------
(defun rt:the-type-error (value type)
  (error "type error: ~S is not of type ~S" value type))

;;------------------------------------------------------------------------------


