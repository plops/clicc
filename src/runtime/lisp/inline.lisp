;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Definition der Lisp Funktionen, deren Aufrufe vom Compiler
;;;            direkt in entsprechenden C-Code ueberfuehrt werden. Nur wenn
;;;            diese Funktionen als funktionale Objekte verwendet werden, dann
;;;            kommen die Definitionen in dieser Datei zur Anwendung.
;;;
;;; $Revision: 1.4 $
;;; $Log: inline.lisp,v $
;;; Revision 1.4  1993/09/28  15:20:04  pm
;;; C-Typtests werden jetzt inline-compiliert
;;;
;;; Revision 1.3  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.2  1993/04/22  08:53:16  hk
;;; (in-package RUNTIME) gestrichen.
;;;
;;; Revision 1.1  1993/04/07  09:12:26  hk
;;; Initial revision
;;;
;;;-----------------------------------------------------------------------------

(defun arrayp (x) (arrayp x))
(defun atom (x) (atom x))
(defun characterp (x) (characterp x))
(defun cons (x y) (cons x y))
(defun consp (x) (consp x))
(defun eq (x1 x2) (eq x1 x2))
(defun eql (x1 x2) (eql x1 x2))
(defun floatp (x) (floatp x))
(defun functionp (x) (functionp x))
(defun integerp (x) (integerp x))
(defun listp (x) (listp x))
(defun not (x) (not x))
(defun numberp (x) (numberp x))
(defun rt::simple-array-p (x) (rt::simple-array-p x))
(defun simple-string-p (x) (simple-string-p x))
(defun simple-vector-p (x) (simple-vector-p x))
(defun stringp (x) (stringp x))
(defun symbolp (x) (symbolp x))
(defun vectorp (x) (vectorp x))
(defun rt::c-char-p (x) (rt::c-char-p x))
(defun rt::c-short-p (x) (rt::c-short-p x))
(defun rt::c-int-p (x) (rt::c-int-p x))
(defun rt::c-long-p (x) (rt::c-long-p x))
(defun rt::c-unsigned-char-p (x) (rt::c-unsigned-char-p x))
(defun rt::c-unsigned-short-p (x) (rt::c-unsigned-short-p x))
(defun rt::c-unsigned-int-p (x) (rt::c-unsigned-int-p x))
(defun rt::c-unsigned-long-p (x) (rt::c-unsigned-long-p x))
