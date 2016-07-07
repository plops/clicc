;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Inhalt   : Laufzeitfunktionen des FFI
;;;
;;; $Revision: 1.9 $
;;; $Log: foreign.lisp,v $
;;; Revision 1.9  1993/11/03  12:32:30  pm
;;; Inkonsistenzen in den Symbolnamen behoben.
;;;
;;; Revision 1.8  1993/09/19  15:12:36  pm
;;; Erweiterungen fuer C-Strings
;;;
;;; Revision 1.7  1993/08/24  11:21:59  pm
;;; Erweiterungen um C-Pointer
;;;
;;; Revision 1.6  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.5  1993/05/23  17:56:08  pm
;;; Alle in Lisp geschriebenen Konstruktor- und Konvertierungs-
;;; Funktionen fuer die primitiven C-Typen implementiert
;;;
;;; Revision 1.4  1993/05/21  13:59:37  pm
;;; c-int in int umbenannt
;;;
;;; Revision 1.3  1993/05/06  14:38:36  pm
;;; erste Versuche fuer int's
;;;
;;; Revision 1.2  1993/04/28  09:10:27  pm
;;; initial revision
;;;
;;------------------------------------------------------------------------------

(in-package "LISP")


(export
 '(ffi:c-int 
   ffi:lisp-integer ffi:lisp-character ffi:lisp-float)
 "FFI")

;;------------------------------------------------------------------------------
;; Die C-Typen anlegen
;;------------------------------------------------------------------------------
(deftype ffi:c-int () `(satisfies rt::c-int-p))

;;------------------------------------------------------------------------------
;; Die Konstruktor-Funktionen zum Anlegen von C-Daten
;;------------------------------------------------------------------------------
(defun ffi:c-int (value)
  (if (typep value 'fixnum)
      (rt::make-c-int value)
      (error-in 'c-int
                "The evaluated value ~S is not of type fixnum." value)))


(defun ffi:lisp-character (c-value)
  (if (or (rt::c-char-p c-value)
          (rt::c-unsigned-char-p c-value))
      (rt::make-lisp-character c-value)
      (error-in 
       'lisp-character
       "The evaluated value ~S is not of type c-<char>." c-value)))

(defun ffi:lisp-integer (c-value)
  (if (or (rt::c-short-p c-value)
          (rt::c-int-p c-value)
          (rt::c-long-p c-value)
          (rt::c-unsigned-short-p c-value)
          (rt::c-unsigned-int-p c-value)
          (rt::c-unsigned-long-p c-value))
      (rt::make-lisp-integer c-value)
      (error-in
       'lisp-integer
       "The evaluated value ~S is not of type c-<integer>." c-value)))

(defun ffi:lisp-float (c-value)
  (if (or (rt::c-float-p c-value)
          (rt::c-double-p c-value)
          (rt::c-long-double-p c-value))
      (rt::make-lisp-float c-value)
      (error-in
       'lisp-float 
       "The evaluated value ~S is not of type c-<float>." c-value)))


#|
(export
 '(ffi::c-char ffi::c-unsigned-char
   ffi::c-short ffi::c-int ffi::c-long 
   ffi::c-unsigned-short ffi::c-unsigned-int ffi::c-unsigned-long
   ffi::c-float ffi::c-double ffi::c-long-double
   ffi::c-char-ptr ffi::c-unsigned-char-ptr 
   ffi::c-short-ptr ffi::c-int-ptr ffi::c-long-ptr
   ffi::c-unsigned-short-ptr ffi::c-unsigned-int-ptr ffi::c-unsigned-long-ptr
   ffi::c-string
   ffi::lisp-character ffi::lisp-integer ffi::lisp-float)
 "FFI")

;;------------------------------------------------------------------------------

(defun ffi:c-string (value)
  (if (typep value 'string)
      (rt::make-c-char-ptr value)
      (error-in 'c-string
                "The evaluated value ~S is not of type string." value)))

(defun ffi:c-char (value)
  (if (typep value 'character)
      (rt::make-c-char value)
      (error-in 'c-char 
                "The evaluated value ~S is not of type character." value)))

(defun ffi:c-unsigned-char (value)
  (if (typep value 'character)
      (rt::make-c-unsigned-char value)
      (error-in 'c-unsigned-char 
                "The evaluated value ~S is not of type character." value)))

(defun ffi:c-short (value)
  (if (typep value 'fixnum)
      (rt::make-c-short value)
      (error-in 'c-short 
                "The evaluated value ~S is not of type fixnum." value)))

(defun ffi:c-int (value)
  (if (typep value 'fixnum)
      (rt::make-c-int value)
      (error-in 'c-int
                "The evaluated value ~S is not of type fixnum." value)))

(defun ffi:c-long (value)
  (if (typep value 'fixnum)
      (rt::make-c-long value)
      (error-in 'c-long 
                "The evaluated value ~S is not of type fixnum." value)))

(defun ffi:c-unsigned-short (value)
  (if (typep value 'fixnum)
      (rt::make-c-unsigned-short value)
      (error-in 'c-unsigned-short 
                "The evaluated value ~S is not of type fixnum." value)))

(defun ffi:c-unsigned-int (value)
  (if (typep value 'fixnum)
      (rt::make-c-unsigned-int value)
      (error-in 'c-unsigned-int
                "The evaluated value ~S is not of type fixnum." value)))

(defun ffi:c-unsigned-long (value)
  (if (typep value 'fixnum)
      (rt::make-c-unsigned-long value)
      (error-in 'c-unsigned-long 
                "The evaluated value ~S is not of type fixnum." value)))

(defun ffi:c-float (value)
  (if (typep value 'float)
      (rt::make-c-float value)
      (error-in 'c-float 
                "The evaluated value ~S is not of type float." value)))

(defun ffi:c-double (value)
  (if (typep value 'float)
      (rt::make-c-double value)
      (error-in 'c-double 
                "The evaluated value ~S is not of type float." value)))

(defun ffi:c-long-double (value)
  (if (typep value 'float)
      (rt::make-c-long-double value)
      (error-in 'c-long-double
                "The evaluated value ~S is not of type float." value)))

;;------------------------------------------------------------------------------
;;------------------------------------------------------------------------------
(defun ffi:c-char-ptr (c-value)
  (if (rt::c-char-p c-value)
      (rt::make-c-char-ptr c-value)
      (error-in
       'c-char-ptr "The evaluated value ~S is not of type c-char." c-value)))

(defun ffi:c-short-ptr (c-value)
  (if (rt::c-short-p c-value)
      (rt::make-c-short-ptr c-value)
      (error-in
       'c-short-ptr "The evaluated value ~S is not of type c-short." c-value)))

(defun ffi:c-int-ptr (c-value)
  (if (rt::c-int-p c-value)
      (rt::make-c-int-ptr c-value)
      (error-in
       'c-short-ptr "The evaluated value ~S is not of type c-int." c-value)))

(defun ffi:c-long-ptr (c-value)
  (if (rt::c-long-p c-value)
      (rt::make-c-long-ptr c-value)
      (error-in
       'c-short-ptr "The evaluated value ~S is not of type c-long." c-value)))

(defun ffi:c-unsigned-char-ptr (c-value)
  (if (rt::c-unsigned-char-p c-value)
      (rt::make-c-unsigned-char-ptr c-value)
      (error-in
       'c-unsigned-char-ptr 
       "The evaluated value ~S is not of type c-unsigned-char." c-value)))

(defun ffi:c-unsigned-short-ptr (c-value)
  (if (rt::c-unsigned-short-p c-value)
      (rt::make-c-unsigned-short-ptr c-value)
      (error-in
       'c-unsigned-short-ptr 
       "The evaluated value ~S is not of type c-unsigned-short." c-value)))

(defun ffi:c-unsigned-int-ptr (c-value)
  (if (rt::c-unsigned-int-p c-value)
      (rt::make-c-unsigned-int-ptr c-value)
      (error-in
       'c-unsigned-short-ptr 
       "The evaluated value ~S is not of type c-unsigned-int." c-value)))

(defun ffi:c-unsigned-long-ptr (c-value)
  (if (rt::c-unsigned-long-p c-value)
      (rt::make-c-unsigned-long-ptr c-value)
      (error-in
       'c-short-ptr 
       "The evaluated value ~S is not of type c-unsigned-long." c-value)))

;;------------------------------------------------------------------------------

|#
