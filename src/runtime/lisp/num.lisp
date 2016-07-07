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
;;; Function : Numbers
;;;
;;; $Revision: 1.14 $
;;; $Id: num.lisp,v 1.14 1994/11/22 14:55:56 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(max min expt log isqrt abs float floor ceiling truncate round mod rem
   scale-float float-radix float-sign float-precision integer-decode-float
   logior logxor logand logeqv lognand lognor logandc1 logandc2 logorc1 logorc2
   lognot logtest logbitp ash))

(defparameter FLOAT-RADIX              (rt::calc-radix))
(defparameter FLOAT-SIGNIFICAND-LENGTH (rt::calc-mant-dig))
(defparameter I-D-F-FAKTOR (* FLOAT-RADIX FLOAT-SIGNIFICAND-LENGTH))

;;-----------------------------------------------------------------------------
;; Comparisons on Numbers                                               
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; MAX number &REST more-numbers
;;-----------------------------------------------------------------------------
(defun max (number &rest more-numbers)
  (let ((max number))
    (dolist (next-number more-numbers)
      (when (> next-number max)
        (setq max next-number)))
    max))

;;-----------------------------------------------------------------------------
;; MIN number &REST more-numbers
;;-----------------------------------------------------------------------------
(defun min (number &rest more-numbers)
  (let ((min number))
    (dolist (next-number more-numbers)
      (when (< next-number min)
        (setq min next-number)))
    min))

;;-----------------------------------------------------------------------------
;; 12.5.1. Exponential and Logarithmic Functions                              
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; EXPT base-number power-number
;;-----------------------------------------------------------------------------
(defun expt (base-number power-number)
  (cond
    ((minusp power-number) (/ 1 (expt base-number (- power-number))))
;		((eql power-number 0)
;			(coerce 1 (type-of base-number)))
;		((= power-number 0)
;			(if (= base-number 0)
;				(error "Illegal base-number 0 with non-integer power-number 0")
; -------------------------------------------------------------------
; CONTAGION muesste das erste Argument in den Typ umwandeln, der nach
; Floating-point contagion der restlichen Argumente entstehen wuerde.
; -------------------------------------------------------------------
;				(contagion 1 base-number power-number)
;			))
    (t (rt::expt base-number power-number))))

;;-----------------------------------------------------------------------------
;; LOG number &OPTIONAL base
;;-----------------------------------------------------------------------------
(defun log (number &optional (base 2.7182818284590455))
  (rt::log number base))

;;------------------------------------------------------------------------------
;;-ISQRT: Integer square root - isqrt(n)**2 <= n
;; Upper and lower bounds on the result are estimated using integer-length.
;; On each iteration, one of the bounds is replaced by their mean.  The lower
;; bound is returned when the bounds meet or differ by only 1.  Initial bounds
;; guarantee that lg(sqrt(n)) = lg(n)/2 iterations suffice.
;;------------------------------------------------------------------------------
(defun isqrt (n)
  "Returns the root of the nearest integer less than
   n which is a perfect square."
  (if (and (integerp n) (not (minusp n)))
      (do* ((lg (integer-length n))
	    (lo (ash 1 (ash (1- lg) -1)))
	    (hi (+ lo (ash lo (if (oddp lg) -1 0))))) ;tighten by 3/4 if possible.
	   ((<= (1- hi) lo) lo)
	(let ((mid (ash (+ lo hi) -1)))
	  (if (<= (* mid mid) n) (setq lo mid) (setq hi mid))))
      (error "Isqrt: ~S argument must be a nonnegative integer" n)))

;;-----------------------------------------------------------------------------
;; Trigonometric and Related Functions                                
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; ABS number
;;-----------------------------------------------------------------------------
(defun abs (number)
  (if (minusp number) (- number) number))

;;-----------------------------------------------------------------------------
;; Type Conversions and Component Extractions on Numbers                
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; FLOAT number &OPTIONAL other
;;
;; Bei uns ist nur ein FLOAT Typ implementiert.
;;
;;-----------------------------------------------------------------------------
(defun float (number &optional other)
  (when (and other (not (floatp other)))
    (error "The value of OTHER, ~S, should be a FLOAT" other))
  (rt::float number))

;;-----------------------------------------------------------------------------
;; FLOOR number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun floor (number &optional (divisor 1))
  (rt::convert-to-int number divisor 0))

;;-----------------------------------------------------------------------------
;; CEILING number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun ceiling (number &optional (divisor 1))
  (rt::convert-to-int number divisor 1))

;;-----------------------------------------------------------------------------
;; TRUNCATE number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun truncate (number &optional (divisor 1))
  (rt::convert-to-int number divisor 2))

;;-----------------------------------------------------------------------------
;; ROUND number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun round (number &optional (divisor 1))
  (rt::convert-to-int number divisor 3))

;;-----------------------------------------------------------------------------
;; MOD number divisor
;;-----------------------------------------------------------------------------
(defun mod (number divisor)
  (multiple-value-call
      #'(lambda (div rem)
          (declare (ignore div))
          rem)
    (floor number divisor)))

;;-----------------------------------------------------------------------------
;; REM number divisor
;;-----------------------------------------------------------------------------
(defun rem (number divisor)
  (multiple-value-call
      #'(lambda (div rem)
          (declare (ignore div))
          rem)
    (truncate number divisor)))

;;------------------------------------------------------------------------------
;; Die folgenden Funktionen basieren auf der Implementation von floats !!!
;; Augenblicklich erfolgt diese durch C-Daten vom Typ double.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; scale-float float integer
;;------------------------------------------------------------------------------
(defun scale-float (f k)
  (* (the float f) (expt (float FLOAT-RADIX f) (the integer k))))

;;------------------------------------------------------------------------------
;; float-radix float
;;------------------------------------------------------------------------------
(defun float-radix (f)
  (the float f)
  FLOAT-RADIX)

;;------------------------------------------------------------------------------
;; float-sign float1 &OPTIONAL float2
;;------------------------------------------------------------------------------
(defun float-sign (float1 &optional (float2 1.0 supplied))
  (when supplied
    (setf float2 (abs (the float float2))))
  (if (minusp (the float float1)) (- float2) float2))

;;------------------------------------------------------------------------------
;; float-digits float
;;------------------------------------------------------------------------------
;; Kann nicht implementiert werden, da man nur raten könnte, wieviele
;; hidden-bits ein C-Compiler für float-Mantissen benutzt.

;;------------------------------------------------------------------------------
;; float-precision float
;;------------------------------------------------------------------------------
(defun float-precision (f)
  (the float f)
  FLOAT-SIGNIFICAND-LENGTH)

;;------------------------------------------------------------------------------
;; integer-decode-float float
;; Multiple Werte: 1. Mantisse als integer
;;                 2. Exponent 
;;                 3. Vorzeichen ( -1.0 oder 1.0 )
;; float = (* Vorzeichen (expt Mantisse (expt (float-radix float) Exponent)))
;;------------------------------------------------------------------------------
(defun integer-decode-float (f)
  (multiple-value-bind
        (float-sig float-exp float-sign)
      (decode-float f)                  ;hier erfolgt auch der Typtest
    (values
     (floor (* I-D-F-FAKTOR float-sig))
     (- float-exp FLOAT-SIGNIFICAND-LENGTH)
     (floor float-sign))))

;;------------------------------------------------------------------------------
;; Logical Operations on Numbers
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; LOGIOR &REST integers
;;------------------------------------------------------------------------------
(defun logior (&rest integers)
  (do ((result 0 (rt::%logior result (the fixnum (pop integers)))))
      ((null integers) result)))

;;------------------------------------------------------------------------------
;; LOGXOR &REST integers
;;------------------------------------------------------------------------------
(defun logxor (&rest integers)
  (do ((result 0 (rt::%logxor result (the fixnum (pop integers)))))
      ((null integers) result)))

;;------------------------------------------------------------------------------
;; LOGAND &REST integers
;;------------------------------------------------------------------------------
(defun logand (&rest integers)
  (do ((result -1 (rt::%logand result (the fixnum (pop integers)))))
      ((null integers) result)))

;;------------------------------------------------------------------------------
;; LOGEQV &REST integers
;;------------------------------------------------------------------------------
(defun logeqv (&rest integers)
  (do ((result -1 (rt::%lognot (rt::%logxor result (the fixnum (pop integers))))))
      ((null integers) result)))

;;------------------------------------------------------------------------------
;; LOGNAND integer1 integer2
;;------------------------------------------------------------------------------
(defun lognand (integer1 integer2)
  (lognot (logand integer1 integer2)))

;;------------------------------------------------------------------------------
;; LOGNOR integer1 integer2
;;------------------------------------------------------------------------------
(defun lognor (integer1 integer2)
  (lognot (logior integer1 integer2)))

;;------------------------------------------------------------------------------
;; LOGANDC1 integer1 integer2
;;------------------------------------------------------------------------------
(defun logandc1 (integer1 integer2)
  (logand (lognot integer1) integer2))

;;------------------------------------------------------------------------------
;; LOGANDC2 integer1 integer2
;;------------------------------------------------------------------------------
(defun logandc2 (integer1 integer2)
  (logand integer1 (lognot integer2)))

;;------------------------------------------------------------------------------
;; LOGORC1 integer1 integer2
;;------------------------------------------------------------------------------
(defun logorc1 (integer1 integer2)
  (logior (lognot integer1) integer2))

;;------------------------------------------------------------------------------
;; LOGORC2 integer1 integer2
;;------------------------------------------------------------------------------
(defun logorc2 (integer1 integer2)
  (logior integer1 (lognot integer2)))

;;------------------------------------------------------------------------------
;; LOGNOT integer
;;------------------------------------------------------------------------------
(defun lognot (integer)
  (rt::%lognot (the fixnum integer)))

;;------------------------------------------------------------------------------
;; LOGTEST integer1 integer2
;;------------------------------------------------------------------------------
(defun logtest (integer1 integer2)
  (not (zerop (logand integer1 integer2))))

;;------------------------------------------------------------------------------
;; LOGBITP index integer
;;------------------------------------------------------------------------------
(defun logbitp (index integer)
  (not (zerop (logand integer (ash 1 index)))))

;;------------------------------------------------------------------------------
;; ASH integer count
;;------------------------------------------------------------------------------
(defun ash (integer count)
  (if (minusp count)
      (rt::%shift-right integer (- count))
      (rt::%shift-left integer count)))
