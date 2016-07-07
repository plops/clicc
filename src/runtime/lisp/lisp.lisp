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
;;; Funktion : Das Lisp Modul mit Loads, Definitionen und Toplevel Forms,
;;;            die der Initialisierung dienen.
;;;
;;; $Revision: 1.21 $
;;; $Id: lisp.lisp,v 1.21 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP" :use ())
(export
 '(MOST-POSITIVE-FIXNUM MOST-NEGATIVE-FIXNUM SHORT-FLOAT-EPSILON
   SHORT-FLOAT-EPSILON SHORT-FLOAT-NEGATIVE-EPSILON PI
   ARRAY-DIMENSION-LIMIT ARRAY-RANK-LIMIT ARRAY-TOTAL-SIZE-LIMIT
   CALL-ARGUMENTS-LIMIT LAMBDA-LIST-KEYWORDS LAMBDA-PARAMETERS-LIMIT
   MULTIPLE-VALUES-LIMIT CHAR-CODE-LIMIT
   ARRAY ATOM BIGNUM BIT BIT BIT-VECTOR CHARACTER COMPILED-FUNCTION COMPLEX
   CONS DOUBLE-FLOAT FIXNUM FLOAT FUNCTION INTEGER KEYWORD LIST LONG-FLOAT MOD
   NULL NUMBER RANDOM-STATE RATIO RATIONAL SEQUENCE SHORT-FLOAT SIGNED-BYTE
   SIMPLE-BIT-VECTOR SIMPLE-STRING SIMPLE-VECTOR SINGLE-FLOAT STANDARD-CHAR
   STRING SYMBOL UNSIGNED-BYTE VECTOR T))

;;------------------------------------------------------------------------------
;; Das Symbol '* wird vom Laufzeitsystem benutzt, um unspezifizierte Teile von
;; Typen auszudruecken.
;;------------------------------------------------------------------------------
(export '*)

;;------------------------------------------------------------------------------
;; Konstanten
;;------------------------------------------------------------------------------
(defconstant T 'T)

(defconstant MOST-POSITIVE-FIXNUM (rt::most-positive-fixnum))
(defconstant MOST-NEGATIVE-FIXNUM (rt::most-negative-fixnum))
(defconstant SHORT-FLOAT-EPSILON     1.1920929e-7) ; from ALLEGRO
(defconstant SHORT-FLOAT-NEGATIVE-EPSILON 1.1920929e-7) ; from ALLEGRO
(defconstant PI                      3.141592653589793)
(defconstant ARRAY-DIMENSION-LIMIT   MOST-POSITIVE-FIXNUM)
(defconstant ARRAY-RANK-LIMIT        MOST-POSITIVE-FIXNUM)
(defconstant ARRAY-TOTAL-SIZE-LIMIT  MOST-POSITIVE-FIXNUM)
(defconstant CALL-ARGUMENTS-LIMIT    MOST-POSITIVE-FIXNUM)
(defconstant LAMBDA-LIST-KEYWORDS
  '(&optional &rest &key &allow-other-keys &aux &body &whole &environment))
(defconstant LAMBDA-PARAMETERS-LIMIT  MOST-POSITIVE-FIXNUM)
(defconstant MULTIPLE-VALUES-LIMIT   20) ; (1+ C:MV_BUFSIZE)
(defconstant CHAR-CODE-LIMIT         256)
(defconstant WRONG_TYPE "~S should be of type ~S")

;;------------------------------------------------------------------------------
;; Type Definitionen
;;------------------------------------------------------------------------------
(def-built-in ARRAY 
    :type-expander ((&optional type size)
                    (cond
                      ((eq '* type) nil)
                      ((symbolp type) nil)
                      (t (error "array type: illegal type ~a" type)))
                    (cond
                      ((eq '* size) nil)
                      ((and (integerp size) (plusp size))
                       (setq size (list size)))
                      ((and (listp size)
                            (every #'(lambda (x)
                                       (or (eq '* x)
                                           (and (integerp x) (plusp x))))
                                   size))
                       nil)
                      (t (error "array type: illegal size ~a" size)))
                    `(lisp::array-internal ,type ,size))
    :superclasses ())
(def-built-in VECTOR
  :type-expander
  ((&optional element-type size) `(array ,element-type (,size)))
  :superclasses (array))
(def-built-in BIT-VECTOR
    :type-expander ((&optional size) `(array bit (,size)))
    :superclasses (vector))
(def-built-in STRING
    :type-expander ((&optional size) `(array standard-char (,size)))
    :superclasses (vector))

(def-built-in CHARACTER
    :type-expander (() '(satisfies characterp))
    :superclasses ())
(def-built-in FUNCTION
    :type-expander (() '(satisfies functionp))
    :superclasses ())

(def-built-in NUMBER
    :type-expander (() '(satisfies numberp))
    :superclasses ())
(def-built-in COMPLEX
    :type-expander (() '(satisfies complexp))
    :superclasses (number))
(def-built-in FLOAT
    :type-expander (() '(satisfies floatp))
    :superclasses (number))
(def-built-in RATIONAL
    :type-expander (() '(satisfies rationalp))
    :superclasses (number))
(def-built-in RATIO
    :type-expander (() '(satisfies rt::ratiop))
    :superclasses (rational))
(def-built-in INTEGER
    :type-expander ((&optional low high)
                    (cond
                      ((eq '* low) nil)
                      ((integerp low) nil)
                      ((and (consp low) (integerp (car low)) (null (cdr low)))
                       (setq low (1+ low)))
                      (t (error "integer type: illegal limit ~a" low)))
                    (cond
                      ((eq '* high) nil)
                      ((integerp high) high)
                      ((and (consp high) (integerp (car high))
                            (null (cdr high)))
                       (setq high (1- high)))
                      (t (error "integer type: illegal limit ~a" high)))
                    `(lisp::integer-internal ,low ,high))
    :superclasses (rational))

(def-built-in SEQUENCE
    :type-expander (() '(or list vector))
    :superclasses ())
(def-built-in LIST
    :type-expander (() '(satisfies listp))
    :superclasses (sequence))
(def-built-in CONS
    :type-expander (() '(satisfies consp))
    :superclasses (list))
(def-built-in SYMBOL
    :type-expander (() '(satisfies symbolp))
    :superclasses ())
(def-built-in NULL
    :type-expander (() '(satisfies null))
    :superclasses (symbol list))

(deftype ATOM () '(satisfies atom))
;;(deftype BIGNUM () '(satisfies rt::bignump))
(deftype BIT () '(integer 0 1))
(deftype COMPILED-FUNCTION () '(satisfies compiled-function-p))
(deftype DOUBLE-FLOAT      () '(satisfies floatp))
;(deftype FIXNUM () `(integer most-negative-fixnum most-positive-fixnum))
(deftype FIXNUM () `(satisfies rt::fixnump)) 
(deftype KEYWORD           () '(satisfies keywordp))
(deftype LONG-FLOAT        () '(satisfies floatp))
(deftype MOD              (n) `(integer 0 (,n)))
;;       NIL                  --> wird gesondert behandelt
(deftype SHORT-FLOAT       () '(satisfies floatp))

(deftype SIGNED-BYTE (&optional s)
 (cond
    ((eq s '*) 'integer)
    ((and (integerp s) (plusp s))
     `(integer ,(- (expt 2 (1- s))) ,(1- (expt 2 (1- s)))))
    (t (error "signed-byte type: illegal bit count ~s" s))))

(deftype SIMPLE-ARRAY (&optional type size)
  (cond
    ((eq '* type) nil)
    ((symbolp type) nil)
    (t (error "array type: illegal type ~a" type)))
  (cond
    ((eq '* size) nil)
    ((and (integerp size) (plusp size)) (setq size (list size)))
    ((and (listp size)
          (every #'(lambda (x)
                     (or (eq '* x) (and (integerp x) (plusp x)))) size))
     nil)
    (t (error "array type: illegal size ~a" size)))
   `(lisp::simple-array-internal ,type ,size))

(deftype SIMPLE-BIT-VECTOR (&optional size) `(simple-array bit (,size)))
(deftype SIMPLE-STRING (&optional size) `(simple-array standard-char (,size)))
(deftype SIMPLE-VECTOR (&optional size) `(simple-array t (,size)))
(deftype SINGLE-FLOAT      () '(satisfies floatp))
(deftype STANDARD-CHAR     () '(satisfies standard-char-p))
;;       T                    --> wird gesondert behandelt
(deftype UNSIGNED-BYTE (&optional s)
  (cond
    ((eq s '*) '(integer 0))
    ((and (integerp s) (plusp s)) `(integer 0 ,(1- (expt 2 s))))
    (t (error "unsigned-byte type: illegal bit count ~s" s))))

;;------------------------------------------------------------------------------
;; Setf Definitionen
;;------------------------------------------------------------------------------
(defsetf SYMBOL-VALUE SET)

(load "stream")                         ; vor allen anderen fuer IO
(load "array")
(load "char")
(load "clos")
(load "coerce")
(load "file")
(load "pathname")
(load "filesys")
(load "map")
(load "misc")
(load "num")
(load "struct")                         ; vor read fuer struct-reader
                                        ; und vor packg
(load "packg")                          ; vor read + sym

(setq *package* (rt:ensure-package "LISP")) ; vor allsyms: (export t)
(import '(nil))

(load "pred")
(load "hash")                           ; nach struct
(load "read")
(load "print")
(load "format")                         ; nach print
(load "error")                          ; nach print
(load "seq")
(load "list")                           ; nach seq
(load "string")
(load "equal")
(load "sym")
(load "typspc")
(load "yesno")
(load "foreign")
(load "environ")

(load "startup")

;; Die folgende Datei braucht nur geladen zu werden, wenn das Laufzeitsystem
;; sicherstellen muss, dass alle 973 externen Symbole im Lisp Package
;; vorhanden sind.
;;----------------
(load "allsyms")
