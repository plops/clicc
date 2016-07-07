;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : System-Funktionen (15. Characters)                             
;;;
;;; $Revision: 1.7 $
;;; $Log: char.lisp,v $
;;; Revision 1.7  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.6  1993/05/14  13:32:47  hk
;;; Funktion character definiert.
;;;
;;; Revision 1.5  1993/04/22  10:48:21  hk
;;; (in-package "RUNTIME") -> (in-package "LISP"),
;;; Definitionen exportiert, defvar, defconstant, defmacro aus
;;; clicc/lib/lisp.lisp einkopiert. rt::set-xxx in (setf xxx) umgeschrieben.
;;; Definitionen und Anwendungen von/aus Package Runtime mit rt: gekennzeichnet.
;;; declaim fun-spec und declaim top-level-form gestrichen.
;;;
;;; Revision 1.4  1993/02/16  14:34:20  hk
;;; clicc::declaim -> declaim, clicc::fun-spec (etc.) -> lisp::fun-spec (etc.)
;;; $Revision: 1.7 $ eingefuegt
;;;
;;; Revision 1.3  1992/07/06  09:12:17  hk
;;; Schreibfehler.
;;;
;;; Revision 1.2  1992/07/06  08:28:10  hk
;;; Neue Syntax fuer declaim fun-spec.
;;;
;;; Revision 1.1  1992/03/24  17:12:55  hk
;;; Initial revision
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export '(digit-char-p code-char digit-char character))

;;-----------------------------------------------------------------------------
;; DIGIT-CHAR-P char &OPTIONAL (radix 10)
;;-----------------------------------------------------------------------------
(defun digit-char-p (char &optional (radix 10))
  (when (not (typep radix '(integer 2 36)))
    (error "~A should be suitable radix" radix))
  (rt::digit-char-p-internal char radix))

;;-----------------------------------------------------------------------------
;; CODE-CHAR code
;;-----------------------------------------------------------------------------
(defun code-char (code)
  (if (check-integer code 0 char-code-limit)
    (rt::code-char-internal code)
    nil))

;;-----------------------------------------------------------------------------
;; DIGIT-CHAR weight &OPTIONAL (radix 10)
;;-----------------------------------------------------------------------------
(defun digit-char (weight &optional (radix 10))
  (if (and (typep radix  '(integer 2 36))
           (check-integer weight 0 (1- radix)))
    (rt::digit-char-internal weight radix)
    nil))

;;------------------------------------------------------------------------------
;; CHARACTER object
;;------------------------------------------------------------------------------
(defun character (object)
  (when (symbolp object) (setq object (symbol-name object)))
  (if (and (stringp object) (eql (length object) 1))
      (char object 0)
      (error "cannot coerce ~S to type character" object)))
