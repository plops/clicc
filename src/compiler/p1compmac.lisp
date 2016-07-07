;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Compiler Macros zur Optimierung von Funktionsaufrufen
;;;
;;; $Revision: 1.4 $
;;; $Log: p1compmac.lisp,v $
;;; Revision 1.4  1993/08/19  10:34:56  hk
;;; Optimierung von (setf aref) nach simplifier.lisp, da es hier
;;; wirkungslos war.
;;;
;;; Revision 1.3  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.2  1993/04/14  10:44:47  hk
;;; provide eingefuegt.
;;;
;;; Revision 1.1  1993/04/08  07:39:38  hk
;;; Initial revision
;;;
;;;-----------------------------------------------------------------------------

(in-package "CLICC")     

;;------------------------------------------------------------------------------
;; (aref array index) -> (rt::vref array index)
;;------------------------------------------------------------------------------
(defun p1-aref (form)
  (match-args
   form AREF (array . indices)
   (if (and (listp indices) (eql 1 (length indices)))
       `(RT::VREF ,array ,(first indices))
       form)))

;;------------------------------------------------------------------------------
;; Optimierung von (setf aref) erfolgt in simplifier.lisp
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(provide "p1compmac")
