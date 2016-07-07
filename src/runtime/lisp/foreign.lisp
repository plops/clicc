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
;;; Funktion : Laufzeitfunktionen des FFI
;;;
;;; $Revision: 1.15 $
;;; $Id: foreign.lisp,v 1.15 1994/12/17 11:58:18 pm Exp $
;;------------------------------------------------------------------------------

(in-package "LISP")


(export
 '(ffi:c-char ffi:c-short ffi:c-int ffi:c-long
   ffi:c-unsigned-char ffi:c-unsigned-short
   ffi:c-unsigned-int ffi:c-unsigned-long
   ffi:c-float ffi:c-double 
   ffi:make-c-string
   
   ffi:lisp-integer ffi:lisp-character ffi:lisp-float
   ffi:make-lisp-string 
   
   ffi:c-char-p ffi:c-short-p ffi:c-int-p ffi:c-long-p 
   ffi:c-unsigned-char-p ffi:c-unsigned-short-p 
   ffi:c-unsigned-int-p ffi:c-unsigned-long-p 
   ffi:c-float-p ffi:c-double-p 
   
   ffi:copy-c-string
   ) "FFI")


;;------------------------------------------------------------------------------
;; Die C-Typen anlegen
;;------------------------------------------------------------------------------
(deftype ffi:c-char ()  `(satisfies ffi:c-char-p))
(deftype ffi:c-short () `(satisfies ffi:c-short-p))
(deftype ffi:c-int ()   `(satisfies ffi:c-int-p))
(deftype ffi:c-long ()  `(satisfies ffi:c-long-p))
(deftype ffi:c-unsigned-char ()  `(satisfies ffi:c-unsigned-char-p))
(deftype ffi:c-unsigned-short () `(satisfies ffi:c-unsigned-short-p))
(deftype ffi:c-unsigned-int ()   `(satisfies ffi:c-unsigned-int-p))
(deftype ffi:c-unsigned-long ()  `(satisfies ffi:c-unsigned-long-p))
(deftype ffi:c-float ()        `(satisfies ffi:c-float-p))
(deftype ffi:c-double ()       `(satisfies ffi:c-double-p))

(deftype ffi:c-string () '(satisfies ffi:c-string-p))

;;------------------------------------------------------------------------------
;; Konstanten
;;------------------------------------------------------------------------------
(defconstant NO-CHARACTER "The evaluated value ~S is not a character.")
(defconstant NO-INTEGER "The evaluated value ~S is not an integer.")
(defconstant NO-FLOAT "The evaluated value ~S is not an float.")
(defconstant NO-STRING "The evaluated value ~S is not a string")

;;------------------------------------------------------------------------------
;; Die Konvertierungsfunktionen-Funktionen zum "Casten" von C-Daten.
;;------------------------------------------------------------------------------
(defun ffi:c-char (value)
  (cond 
    ((characterp value)                 ; Lisp-Character
     (rt::make-c-char value))
    ((integerp value)                   ; Lisp-Integer
     (rt::make-c-char-2 value))         
    ((rt::c-primitive-p value)          ; c-char
     (rt::cast-c-char value))
    (t (error-in "C-CHAR" NO-CHARACTER value))))

(defun ffi:c-short (value)
  (cond 
    ((integerp value)
     (rt::make-c-short value))
    ((rt::c-primitive-p value)
     (rt::cast-c-short value))
    (t (error-in "C-SHORT" NO-INTEGER value))))

(defun ffi:c-int (value)
  (cond 
    ((integerp value)
     (rt::make-c-int value))
    ((rt::c-primitive-p value)
     (rt::cast-c-int value))
    (t (error-in "C-INT" NO-INTEGER value))))

(defun ffi:c-long (value)
  (cond 
    ((integerp value)
     (rt::make-c-long value))
    ((rt::c-primitive-p value)
     (rt::cast-c-long value))
    (t (error-in "C-LONG" NO-INTEGER value))))

(defun ffi:c-unsigned-char (value)
  (cond 
    ((characterp value)                 ; Lisp-Character
     (rt::make-c-unsigned-char value))
    ((integerp value)                   ; Lisp-Integer
     (rt::make-c-unsigned-char-2 value))         
    ((rt::c-primitive-p value)          ; c-char
     (rt::cast-c-unsigned-char value))
    (t (error-in "C-UNSIGNED-CHAR" NO-CHARACTER value))))

(defun ffi:c-unsigned-short (value)
  (cond 
    ((integerp value)
     (rt::make-c-unsigned-short value))
    ((rt::c-primitive-p value)
     (rt::cast-c-unsigned-short value))
    (t (error-in "C-UNSIGNED-SHORT" NO-INTEGER value))))

(defun ffi:c-unsigned-int (value)
  (cond 
    ((integerp value)
     (rt::make-c-unsigned-int value))
    ((rt::c-primitive-p value)
     (rt::cast-c-unsigned-int value))
    (t (error-in "C-UNSIGNED-INT" NO-INTEGER value))))

(defun ffi:c-unsigned-long (value)
  (cond 
    ((integerp value)
     (rt::make-c-unsigned-long value))
    ((rt::c-primitive-p value)
     (rt::cast-c-unsigned-long value))
    (t (error-in "C-UNSIGNED-LONG" NO-INTEGER value))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun ffi:c-float (value)
  (cond 
    ((floatp value)
     (rt::make-c-float value))
    ((rt::c-floating-p value)
     (rt::cast-c-float value))
    (t (error-in "C-FLOAT" NO-FLOAT value))))

(defun ffi:c-double (value)
  (cond 
    ((floatp value)
     (rt::make-c-double value))
    ((rt::c-floating-p value)
     (rt::cast-c-double value))
    (t (error-in "C-DOUBLE" NO-FLOAT value))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun ffi:make-c-string (value)
  (cond
    ((stringp value)
     (rt::make-c-string value))
    ((ffi:c-string-p value)
     value)
    (t (error-in "MAKE-C-STRING" NO-STRING value))))

;;------------------------------------------------------------------------------
;; Kopierfunktion fuer Zeichenketten
;;------------------------------------------------------------------------------
(defun ffi:copy-c-string (value)
  (if (ffi:c-string-p value)
      (rt::copy-c-string value)
      (error-in "COPY-C-STRING" NO-STRING value)))

;;------------------------------------------------------------------------------
;; Konvertierungsfunktionen von C nach Lisp.
;;------------------------------------------------------------------------------
(defun ffi:lisp-character (value)
  (cond
    ((rt::c-primitive-p value)
     (rt::make-lisp-character value))
    ((characterp value)
     value)
    (t (error-in "LISP-CHARACTER" NO-CHARACTER value))))

(defun ffi:lisp-integer (value)
  (cond 
    ((rt::c-primitive-p value)
     (rt::make-lisp-integer value))
    ((integerp value)
     value)
    (t (error-in "LISP-INTEGER" NO-INTEGER value))))

(defun ffi:lisp-float (value)
  (cond
    ((rt::c-floating-p value)
     (rt::make-lisp-float value))
    ((floatp value)
     value)
    (t (error-in "LISP-FLOAT" NO-FLOAT value))))

(defun ffi:make-lisp-string (value)
  (cond
    ((ffi:c-string-p value)
     (rt::make-lisp-string value))
    ((stringp value)
     value)
    (t (error-in "MAKE-LISP-STRING" NO-STRING value))))

;;------------------------------------------------------------------------------
;; Testfunktionen
;;------------------------------------------------------------------------------
(defun ffi:c-char-p (object)
  (or (rt::c-char-p object)
      (rt::check-char object)))

(defun ffi:c-short-p (object)
  (or (rt::c-short-p object)
      (rt::check-short object)))

(defun ffi:c-int-p (object)
  (or (rt::c-int-p object)
      (rt::check-int object)))

(defun ffi:c-long-p (object)
  (or (rt::c-long-p object)
      (rt::check-long object)))

(defun ffi:c-unsigned-char-p (object)
  (or (rt::c-unsigned-char-p object)
      (rt::check-unsigned-char object)))

(defun ffi:c-unsigned-short-p (object)
  (or (rt::c-unsigned-short-p object)
      (rt::check-unsigned-short object)))

(defun ffi:c-unsigned-int-p (object)
  (or (rt::c-unsigned-int-p object)
      (rt::check-unsigned-int object)))

(defun ffi:c-unsigned-long-p (object)
  (or (rt::c-unsigned-long-p object)
      (rt::check-unsigned-long object)))

(defun ffi:c-float-p (object)
  (or (rt::c-float-p object)
      (rt::check-float object)))

(defun ffi:c-double-p (object)
  (or (rt::c-double-p object)
      (rt::check-double object)))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
