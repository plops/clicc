;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : System-Funktionen (12. Numbers)                                
;;;
;;; $Revision: 1.10 $
;;; $Log: num.lisp,v $
;;; Revision 1.10  1993/09/02  15:56:37  uho
;;; LOGTEST und LOGBITP definiert. Einige andere logische Funktionen
;;; verschoenert.
;;;
;;; Revision 1.9  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.8  1993/04/22  10:48:21  hk
;;; (in-package "RUNTIME") -> (in-package "LISP"),
;;; Definitionen exportiert, defvar, defconstant, defmacro aus
;;; clicc/lib/lisp.lisp einkopiert. rt::set-xxx in (setf xxx) umgeschrieben.
;;; Definitionen und Anwendungen von/aus Package Runtime mit rt: gekennzeichnet.
;;; declaim fun-spec und declaim top-level-form gestrichen.
;;;
;;; Revision 1.7  1993/04/06  17:07:09  hk
;;; shift-right, shift-left -> %shift-right, %shift-left.
;;;
;;; Revision 1.6  1993/03/02  15:27:48  hk
;;; isqrt definiert.
;;;
;;; Revision 1.5  1993/02/16  14:34:20  hk
;;; clicc::declaim -> declaim, clicc::fun-spec (etc.) -> lisp::fun-spec (etc.)
;;; $Revision: 1.10 $ eingefuegt
;;;
;;; Revision 1.4  1993/01/13  15:07:13  ft
;;; Erweiterung um (ash ...)
;;;
;;; Revision 1.3  1993/01/06  11:18:18  ft
;;; Erweiterung um logische Operationen auf Zahlen.
;;;
;;; Revision 1.2  1992/07/06  09:14:15  hk
;;; Neue Syntax fuer declaim fun-spec.
;;;
;;; Revision 1.1  1992/03/24  17:12:55  hk
;;; Initial revision
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(max min expt log isqrt abs float floor ceiling truncate round mod rem
   logior logxor logand logeqv lognand lognor logandc1 logandc2 logorc1 logorc2
   lognot logtest logbitp ash))

;;-----------------------------------------------------------------------------
;; 12.3. Comparisons on Numbers                                               
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
    (t (rt::expt-internal base-number power-number))))

;;-----------------------------------------------------------------------------
;; LOG number &OPTIONAL base
;;-----------------------------------------------------------------------------
(defun log (number &optional (base 2.7182818284590455))
  (rt::log-internal number base))

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
;; 12.5.2. Trigonometric and Related Functions                                
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; ABS number
;;-----------------------------------------------------------------------------
(defun abs (number)
  (if (minusp number) (- number) number))

;;-----------------------------------------------------------------------------
;; 12.6. Type Conversions and Component Extractions on Numbers                
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
  (rt::float-internal number))

;;-----------------------------------------------------------------------------
;; FLOOR number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun floor (number &optional (divisor 1))
  (rt::floor-internal number divisor))

;;-----------------------------------------------------------------------------
;; CEILING number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun ceiling (number &optional (divisor 1))
  (rt::ceiling-internal number divisor))

;;-----------------------------------------------------------------------------
;; TRUNCATE number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun truncate (number &optional (divisor 1))
  (rt::truncate-internal number divisor))

;;-----------------------------------------------------------------------------
;; ROUND number &OPTIONAL divisor
;;-----------------------------------------------------------------------------
(defun round (number &optional (divisor 1))
  (rt::round-internal number divisor))

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
;; 12.7. Logical Operations on Numbers
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
