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
;;; Function : Arrays
;;;
;;; $Revision: 1.25 $
;;; $Id: array.lisp,v 1.25 1994/11/22 14:55:56 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(arrayp simple-array-p stringp vectorp bit-array-p bit-vector-p
   make-array vector aref row-major-aref svref array-element-type
   upgraded-array-element-type array-rank array-dimension array-dimensions
   array-total-size array-in-bounds-p array-row-major-index
   adjustable-array-p bit sbit bit-and bit-ior bit-xor bit-eqv bit-nand
   bit-nor bit-andc1 bit-andc2 bit-orc1 bit-orc2 bit-not
   array-has-fill-pointer-p fill-pointer vector-push vector-push-extend
   vector-pop adjust-array))

(export
 '(rt::vref) "RT")

;;------------------------------------------------------------------------------
;; Datenstrukturen
;;------------------------------------------------------------------------------

(defstruct (complex-base-array (:copier nil))
  (data       nil  :type array)
  (displaced  -1   :type fixnum))

(defstruct (complex-array (:include complex-base-array) (:copier nil))
  (dims       nil  :type list))

(defstruct (complex-vector (:include complex-base-array) (:copier nil))
  (length      0   :type fixnum)
  (fillptr    -1   :type fixnum))


;; C-Funktinen aus array.c
;; -----------------------
;; simple-vector-p (inline)
;; rt::plain-vector-p (inline)
;; rt::plain-vector-element-code    -> 0=T,1=fixnum,2=float,3=character,4=bit
;; rt::plain-vector-length (inline)
;; rt::make-vector-t
;; rt::make-vector-fixnum
;; rt::make-vector-float
;; rt::make-vector-char
;; rt::make-vector-bit
;; rt::svref-internal (inline)
;; rt::set-svref-internal (inline)
;; rt::pvref
;; rt::set-pvref
;; simple-bit-vector-p (inline)
;; rt::sbvref
;; rt::set-sbvref
;; rt::shrink-smstr

;;------------------------------------------------------------------------------
;; Typtests
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; ARRAYP object
;;------------------------------------------------------------------------------
(defun arrayp (object)
  (or (rt::plain-vector-p object) (complex-base-array-p object)))

;;------------------------------------------------------------------------------
;; SIMPLE-ARRAY-P object
;;------------------------------------------------------------------------------
(defun simple-array-p (object)
  ;; jedes complex-array ist adjustable, was diese arrays als simple-arrays
  ;; ausschliesst. Nur ein plain-vector ist nicht adjustable, displaced
  ;; und hat keinen fill-pointer.
  ;;-----------------------------
  (rt::plain-vector-p object))

;;------------------------------------------------------------------------------
;; STRINGP object
;;------------------------------------------------------------------------------
(defun stringp (object)
  (or (simple-string-p object)
      (and (complex-vector-p object)
           (simple-string-p (displaced-data object)))))

;;------------------------------------------------------------------------------
;; VECTORP object
;;------------------------------------------------------------------------------
(defun vectorp (object)
  (or (rt::plain-vector-p object) (complex-vector-p object)))

;;------------------------------------------------------------------------------
;; BIT-ARRAY-P object
;;------------------------------------------------------------------------------
(defun bit-array-p (object)
  (or (simple-bit-vector-p object)
      (simple-bit-vector-p (displaced-data object))))

;;------------------------------------------------------------------------------
;; BIT-VECTOR-P object
;;------------------------------------------------------------------------------
(defun bit-vector-p (object)
  (or (simple-bit-vector-p object)
      (simple-bit-vector-p (displaced-data object))))

;;------------------------------------------------------------------------------
;; lokal: DISPLACED-DATA array
;;------------------------------------------------------------------------------
(defun displaced-data (array)
  (setq array (complex-base-array-data array))
  (if (rt::plain-vector-p array)
      array
      (displaced-data array)))

;;------------------------------------------------------------------------------
;; Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant OUT_OF_RANGE "The index ~S is not in the range [0, ~S)")

;;------------------------------------------------------------------------------
;; MAKE-ARRAY dimensions
;;     &KEY :element-type :initial-element :initial-contents
;;          :adjustable   :fill-pointer
;;          :displaced-to :displaced-index-offset
;;------------------------------------------------------------------------------
(defun make-array (dimensions
                   &key
                   (element-type t)
                   (initial-element  nil init-elem-sp)
                   (initial-contents nil init-cont-sp)
                   adjustable
                   fill-pointer
                   displaced-to
                   (displaced-index-offset 0))

  (cond
    ((and (not (null dimensions))
          (or (atom dimensions)
              (cond
                ((atom (cdr dimensions))
                 (setq dimensions (car dimensions))
                 t)
                (t nil))))

     (unless (check-integer dimensions 0 array-dimension-limit)
       (error "Invalid dimension argument ~A." dimensions))

     (cond
       ((null fill-pointer) (setq fill-pointer -1))
       ((check-integer fill-pointer 0 dimensions) nil)
       (T (setq fill-pointer dimensions)))

     (cond

       ;; ignore initial-element and initial-contents
       ;;--------------------------------------------
       (displaced-to
        (unless (check-integer displaced-index-offset 0 array-dimension-limit)
          (error "Invalid displaced-index-offset argument ~A."
                 displaced-index-offset))
        (make-complex-vector :length dimensions
                             :displaced displaced-index-offset
                             :data displaced-to
                             :fillptr fill-pointer))

       (T (let* ((type-code (to-type-code element-type))
                 (vector (make-plain-vector dimensions type-code)))

            ;; ignore initial-contents if initial-element is supplied
            ;;-------------------------------------------------------
            (cond
              (init-elem-sp (fill vector initial-element))
              (init-cont-sp (replace vector initial-contents)))

            (if (or adjustable (not (eql -1 fill-pointer)))
                (make-complex-vector :length dimensions
                                     :data vector
                                     :fillptr fill-pointer)
                vector)))))

    (T
     ;; ignore fill-pointer
     ;;--------------------
     (cond
       (displaced-to
        (unless (check-integer displaced-index-offset 0 array-dimension-limit)
          (error "Invalid displaced-index-offset argument ~A."
                 displaced-index-offset))
        (make-complex-array :dims dimensions
                            :displaced displaced-index-offset
                            :data displaced-to))
       (T
        (let* ((size (check-dimensions dimensions))
               (type-code (to-type-code element-type))
               (data (make-plain-vector size type-code)))
          (cond
            (init-elem-sp (fill data initial-element))
            (init-cont-sp
             (fill-data-vector data dimensions initial-contents)))
          (make-complex-array :dims dimensions :data data)))))))

;;------------------------------------------------------------------------------
;; lokal: MAKE-PLAIN-VECTOR
;;------------------------------------------------------------------------------
(defun make-plain-vector (size type-code)
  (ecase type-code
    (0 (rt::make-vector-t size))
    (1 (rt::make-vector-fixnum size))
    (2 (rt::make-vector-float size))
    (3 (rt::make-vector-char size #\Space))
    (4 (rt::make-vector-bit size))))

;;------------------------------------------------------------------------------
;; lokal: TO-TYPE-CODE element-type
;;------------------------------------------------------------------------------
(defun to-type-code (element-type)
  (case element-type
    (fixnum 1)
    ((float short-float single-float double-float long-float) 2)
    ((standard-char character) 3)
    (bit 4)
    (otherwise 0)))

;;------------------------------------------------------------------------------
;; lokal: TYPE-CODE-P x code
;;------------------------------------------------------------------------------
(defun type-code-p (x code)
  (ecase code
    (0 t)
    (1 (typep x 'fixnum))
    (2 (typep x 'float))
    (3 (typep x 'character))
    (4 (typep x 'bit))))

;;------------------------------------------------------------------------------
(defun to-element-type (element-code)
  (ecase element-code
    (0 T)
    (1 'fixnum)
    (2 'float)
    (3 'character)
    (4 'bit)))

;;------------------------------------------------------------------------------
;; lokal: CHECK-DIMENSIONS dimensions
;;------------------------------------------------------------------------------
(defun check-dimensions (dimensions)
  (do (dimension
       (size 1 (* size dimension)))
      ((null dimensions) size)
    (setq dimension (pop dimensions))
    (unless (check-integer dimension 0 array-dimension-limit)
      (error "Invalid dimension argument ~A" dimension))))

;;------------------------------------------------------------------------------
;; lokal
;;------------------------------------------------------------------------------
(defun fill-data-vector (vector dimensions initial-contents)
  (let ((index 0))
    (labels ((frob (axis dims contents)
	       (cond ((null dims)
		      (setf (aref vector index) contents)
		      (incf index))
		     (t
		      (unless (= (length contents) (car dims))
			(error "Malformed :initial-contents.  Dimension of ~
			        axis ~D is ~D, but ~S is ~D long."
			       axis (car dims) contents (length contents)))
		      (if (listp contents)
			  (dolist (content contents)
			    (frob (1+ axis) (cdr dims) content))
			  (dotimes (i (length contents))
			    (frob (1+ axis) (cdr dims) (aref contents i))))))))
      (frob 0 dimensions initial-contents))))

;;------------------------------------------------------------------------------
;; VECTOR &REST objects
;;------------------------------------------------------------------------------
(defun vector (&rest objects)
  (make-array (length objects) :element-type t :initial-contents objects))

;;------------------------------------------------------------------------------
;; ADJUST-ARRAY array new-dimensions
;;    &KEY :element-type :initial-element :initial-contents
;;         :adjustable   :fill-pointer
;;         :displaced-to :displaced-index-offset
;;------------------------------------------------------------------------------
(defun adjust-array (array new-dimensions
                           &rest rest-args
                           &key
                           element-type
                           initial-element
                           initial-contents
                           (fill-pointer nil fill-pointer-sp)
                           displaced-to
                           displaced-index-offset)
  (declare
   (ignore element-type initial-element displaced-to displaced-index-offset))
  (let ((original-element-type (array-element-type array))
        (original-dimensions   (array-dimensions   array))
        new-array
        subscripts)

    (labels ((dim-to-skips (dim)          ; wandelt array-dimensions in
               (if (null (cdr dim))       ; skips fuer r.-m.-aref um
                   dim
                   (let ((result (dim-to-skips (cdr dim))))
                     (cons (* (car dim) (car result)) result))))

           (rmi-copy (min-dim new-skip old-skip new-offset old-offset)
             (if (null (cdr min-dim))

                 (dotimes (i (car min-dim))
                   (setf (row-major-aref new-array new-offset)
                         (row-major-aref array old-offset))
                   (incf new-offset)
                   (incf old-offset))

                 (dotimes (i (car min-dim))
                   (rmi-copy (cdr min-dim) (cdr new-skip) (cdr old-skip)
                             (+ new-offset (* i (car new-skip)))
                             (+ old-offset (* i (car old-skip))))))))

      (when (atom new-dimensions) (setq new-dimensions (list new-dimensions)))
      (when (/= (length new-dimensions) (array-rank array))
        (error "The 'new-dimensions' argument given to ADJUST-ARRAY, ~A, ~
              is not the same rank as the argument array." new-dimensions))

      (when (and (array-has-fill-pointer-p array) (null fill-pointer))
        (setq fill-pointer (fill-pointer array)))

      (setq new-array
            (apply #'make-array new-dimensions
                   :element-type original-element-type
                   :adjustable   T
                   :fill-pointer fill-pointer
                   rest-args))
      (unless initial-contents
        (rmi-copy (mapcar #'min new-dimensions original-dimensions)
                  (cdr (dim-to-skips new-dimensions))
                  (cdr (dim-to-skips original-dimensions)) 0 0))
      (displace-array array new-array))))

;;------------------------------------------------------------------------------
;; lokal: DISPLACE-ARRAY array new-array
;;------------------------------------------------------------------------------
(defun displace-array (array new-array)
  (cond
    ((complex-array-p array)
     (setf (complex-array-data array) (complex-array-data new-array))
     (setf (complex-array-displaced array) (complex-array-displaced new-array))
     (setf (complex-array-dims array) (complex-array-dims new-array)))
    (t
     (setf (complex-vector-data array) (complex-vector-data new-array))
     (setf (complex-vector-displaced array)(complex-vector-displaced new-array))
     (setf (complex-vector-length array) (complex-vector-length new-array))
     (setf (complex-vector-fillptr array) (complex-vector-fillptr new-array))))
  array)

;;-----------------------------------------------------------------------------
;; VREF vector index
;; Source-Level Optimierung: (aref vector index) -> (vref vector index)
;; Hat den Vorteil, dass kein &REST-Parameter benoetigt wird.
;;-----------------------------------------------------------------------------
(defun rt:vref (vector index)
  (unless (vectorp vector)
    (error WRONG_TYPE vector 'vector))
  (row-major-aref vector index))

;;-----------------------------------------------------------------------------
;; (SETF VREF) newvalue array index
;;-----------------------------------------------------------------------------
(defun (setf rt:vref) (newvalue vector index)
  (unless (vectorp vector)
    (error WRONG_TYPE vector 'vector))
  (setf (row-major-aref vector index) newvalue))

;;------------------------------------------------------------------------------
;; AREF array &REST subscripts
;;------------------------------------------------------------------------------
(defun aref (array &rest subscripts)
  (declare (:simp-when-n-args 2 rt:vref))
  (row-major-aref array (apply #'array-row-major-index array subscripts)))

;;------------------------------------------------------------------------------
;; (SETF AREF) newvalue array &REST subscripts
;;------------------------------------------------------------------------------
(defun (setf aref) (newvalue array &rest subscripts)
  (declare (:simp-when-n-args 3 (setf rt:vref)))
  (setf (row-major-aref array (apply #'array-row-major-index array subscripts))
        newvalue))

;;------------------------------------------------------------------------------
;; SVREF simple-vector index
;;------------------------------------------------------------------------------
(defun svref (simple-vector index)
  (unless (simple-vector-p simple-vector)
    (error WRONG_TYPE simple-vector 'simple-vector))
  (unless (check-integer index 0 (1- (rt::plain-vector-length simple-vector)))
    (error OUT_OF_RANGE index (rt::plain-vector-length simple-vector)))
  (rt::svref-internal simple-vector index))

;;------------------------------------------------------------------------------
;; lokal: PVREF simple-vector index
;; spart den Typtest auf plain-vector-p
;;------------------------------------------------------------------------------
(defun pvref (simple-vector index)
  (unless (check-integer index 0 (1- (rt::plain-vector-length simple-vector)))
    (error OUT_OF_RANGE index (rt::plain-vector-length simple-vector)))
  (rt::pvref simple-vector index))

;;------------------------------------------------------------------------------
;; (SETF SVREF) newvalue simple-vector index
;;------------------------------------------------------------------------------
(defun (setf svref) (newvalue simple-vector index)
  (unless (simple-vector-p simple-vector)
    (error WRONG_TYPE simple-vector 'simple-vector))
  (unless (check-integer index 0 (1- (rt::plain-vector-length simple-vector)))
    (error OUT_OF_RANGE index (rt::plain-vector-length simple-vector)))
  (rt::set-svref-internal newvalue simple-vector index))

;;------------------------------------------------------------------------------
;; lokal: (SETF PVREF) newvalue simple-vector index
;; spart den Typtest auf plain-vector-p
;;------------------------------------------------------------------------------
(defun (setf pvref) (newvalue simple-vector index)
  (unless (check-integer index 0 (1- (rt::plain-vector-length simple-vector)))
    (error OUT_OF_RANGE index (rt::plain-vector-length simple-vector)))
  (unless (type-code-p newvalue (rt::plain-vector-element-code simple-vector))
    (error "Can't store ~A in a vector of type ~A"
           newvalue (plain-vector-element-type simple-vector)))
  (rt::set-pvref newvalue simple-vector index))

;;------------------------------------------------------------------------------
;; ARRAY-ELEMENT-TYPE array
;;------------------------------------------------------------------------------
(defun array-element-type (array)
  (to-element-type (array-element-type-internal array)))

(defun array-element-type-internal (array)
  (cond
    ((rt::plain-vector-p array) (rt::plain-vector-element-code array))
    (t (rt::plain-vector-element-code (displaced-data array)))))

(defun plain-vector-element-type (vector)
  (rt::plain-vector-element-code vector))

;;------------------------------------------------------------------------------
;; UPGRADED-ARRAY-ELEMENT-TYPE type
;;------------------------------------------------------------------------------
(defun upgraded-array-element-type (type)
  (case type
    (bit 'bit)
    (fixnum 'fixnum)
    ((standard-char character) 'character)
    ((float short-float single-float double-float long-float) 'float)
    (otherwise t)))

;;------------------------------------------------------------------------------
;; ARRAY-RANK array
;;------------------------------------------------------------------------------
(defun array-rank (array)
  (cond
    ((or (rt::plain-vector-p array) (complex-vector-p array)) 1)
    (t (length (complex-array-dims array)))))

;;------------------------------------------------------------------------------
;; ARRAY-DIMENSION array axis-number
;;------------------------------------------------------------------------------
(defun array-dimension (array axis-number)
  (unless (check-integer axis-number 0 (1- (array-rank array)))
    (error "The value ~A is not suitable as an axis-number ~
            for an array with rank ~A" axis-number (array-rank array)))
  (cond
    ((rt::plain-vector-p array) (rt::plain-vector-length array))
    ((complex-vector-p array) (complex-vector-length array))
    (t (elt (complex-array-dims array) axis-number))))

;;------------------------------------------------------------------------------
;; ARRAY-DIMENSIONS array
;;------------------------------------------------------------------------------
(defun array-dimensions (array)
  (cond
    ((rt::plain-vector-p array) (list (rt::plain-vector-length array)))
    ((complex-vector-p array) (list (complex-vector-length array)))
    (t (complex-array-dims array))))

;;------------------------------------------------------------------------------
;; ARRAY-TOTAL-SIZE array
;;------------------------------------------------------------------------------
(defun array-total-size (array)
  (cond
    ((rt::plain-vector-p array) (rt::plain-vector-length array))
    ((complex-vector-p array) (complex-vector-length array))
    ((< (complex-array-displaced array) 0)
     (rt::plain-vector-length (complex-array-data array)))
    (T (apply #'* (complex-array-dims array)))))

;;------------------------------------------------------------------------------
;; ARRAY-IN-BOUNDS-P array &REST subscripts
;;------------------------------------------------------------------------------
(defun array-in-bounds-p (array &rest subscripts)
  (unless (= (array-rank array) (length subscripts))
    (error "Wrong number of subscripts for array ~a" array))
  (let ((i 0))
    (dolist (index subscripts t)
      (unless (check-integer index 0 (1- (array-dimension array i)))
        (return nil))
      (incf i))))

;;------------------------------------------------------------------------------
;; ARRAY-ROW-MAJOR-INDEX array &REST subscripts
;;------------------------------------------------------------------------------
(defun array-row-major-index (array &rest subscripts)
  (let ((i 0)
        (row-major-index 0))
    (dolist (index subscripts)
      (let ((dim (array-dimension array i)))
        (incf i)
        (setq row-major-index (+ (* row-major-index dim) index))))
    row-major-index))

;;------------------------------------------------------------------------------
;; ROW-MAJOR-AREF array index
;;------------------------------------------------------------------------------
(defun row-major-aref (array index)
  (cond
    ((simple-vector-p array) (svref array index))
    ((rt::plain-vector-p array) (pvref array index))
    ((< (complex-base-array-displaced array) 0)
     (pvref (complex-base-array-data array) index))
    (T (row-major-aref (complex-base-array-data array)
                       (+ index (complex-base-array-displaced array))))))

;;------------------------------------------------------------------------------
;; (SETF ROW-MAJOR-AREF) newvalue array index
;;------------------------------------------------------------------------------
(defun (setf row-major-aref) (newvalue array index)
  (cond
    ((simple-vector-p array) (setf (svref array index) newvalue))
    ((rt::plain-vector-p array) (setf (pvref array index) newvalue))
    ((< (complex-base-array-displaced array) 0)
     (setf (pvref (complex-base-array-data array) index) newvalue))
    (T (setf (row-major-aref (complex-base-array-data array)
                             (+ index (complex-base-array-displaced array)))
             newvalue))))

;;------------------------------------------------------------------------------
;; ADJUSTABLE-ARRAY-P array
;;------------------------------------------------------------------------------
(defun adjustable-array-p (array) (complex-base-array-p array))

;;------------------------------------------------------------------------------
;; BIT bit-array &REST subscripts
;;------------------------------------------------------------------------------
(defun bit (bit-array &rest subscripts)
  (unless (bit-array-p bit-array)
    (error WRONG_TYPE bit-array 'bit-array))
  (row-major-aref bit-array
                  (apply #'array-row-major-index bit-array subscripts)))

;;------------------------------------------------------------------------------
;; (SETF BIT) newbit bit-array &REST subscripts
;;------------------------------------------------------------------------------
(defun (setf bit) (newbit bit-array &rest subscripts)
  (unless (bit-array-p bit-array)
    (error WRONG_TYPE bit-array 'bit-array))
  (setf (row-major-aref bit-array
                        (apply #'array-row-major-index bit-array subscripts))
        newbit))

;;------------------------------------------------------------------------------
;; SBIT bit-array &REST subscripts
;;------------------------------------------------------------------------------
(defun sbit (bit-array &rest subscripts)
  (unless (bit-array-p bit-array)
    (error WRONG_TYPE bit-array 'bit-array))
  (row-major-aref bit-array
                  (apply #'array-row-major-index bit-array subscripts)))

;;------------------------------------------------------------------------------
;; (SETF SBIT) newbit bit-array &REST subscripts
;;------------------------------------------------------------------------------
(defun (setf sbit) (newbit bit-array &rest subscripts)
  (unless (bit-array-p bit-array)
    (error WRONG_TYPE bit-array 'bit-array))
  (setf (row-major-aref bit-array
                        (apply #'array-row-major-index bit-array subscripts))
        newbit))

;;------------------------------------------------------------------------------
;; BIT-... bit-array1 bit-array2 &OPTIONAL result-bit-array
;;------------------------------------------------------------------------------
(defun bit-and (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 0 bit-array1 bit-array2 result-bit-array))

(defun bit-ior (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 1 bit-array1 bit-array2 result-bit-array))

(defun bit-xor (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 2 bit-array1 bit-array2 result-bit-array))

(defun bit-eqv (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 3 bit-array1 bit-array2 result-bit-array))

(defun bit-nand (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 4 bit-array1 bit-array2 result-bit-array))

(defun bit-nor (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 5 bit-array1 bit-array2 result-bit-array))

(defun bit-andc1 (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 6 bit-array1 bit-array2 result-bit-array))

(defun bit-andc2 (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 7 bit-array1 bit-array2 result-bit-array))

(defun bit-orc1 (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 8 bit-array1 bit-array2 result-bit-array))

(defun bit-orc2 (bit-array1 bit-array2 &optional result-bit-array)
  (bit-op 9 bit-array1 bit-array2 result-bit-array))

;;------------------------------------------------------------------------------
;; lokal: BIT-OP opcode bit-array1 bit-array2 result-bit-array
;;------------------------------------------------------------------------------
(defun bit-op (opcode bit-array1 bit-array2 result-bit-array)
  (unless (bit-array-p bit-array1)
    (error WRONG_TYPE bit-array1 'bit-array))
  (unless (bit-array-p bit-array2)
    (error WRONG_TYPE bit-array2 'bit-array))
  (unless (equal (array-dimensions bit-array1) (array-dimensions bit-array2))
    (error "~A and ~A must have the same dimensions" bit-array1 bit-array2))

  (rt::bitop opcode bit-array1 bit-array2
             ;;ObdA wird bit-array1 als Referenzarray benutzt
             (cond
               ((null result-bit-array)
                (make-array (array-dimensions bit-array1) :element-type 'bit))
               ((eq result-bit-array t)
                bit-array1)
               (T (unless (bit-array-p result-bit-array)
                    (error WRONG_TYPE result-bit-array 'bit-array))
                  (unless (equal (array-dimensions bit-array1)
                                 (array-dimensions result-bit-array))
                    (error "~A must have the same dimensions as the other ~
                              bit-array parameters" result-bit-array))
                  result-bit-array))))

;;------------------------------------------------------------------------------
;; BIT-NOT bit-array &OPTIONAL result-bit-array
;;------------------------------------------------------------------------------
(defun bit-not (bit-array &optional result-bit-array)
  (bit-op 10 bit-array bit-array result-bit-array))

;;------------------------------------------------------------------------------
;; ARRAY-HAS-FILL-POINTER-P array
;;------------------------------------------------------------------------------
(defun array-has-fill-pointer-p (array)
  (and (complex-vector-p array)
       (not (< (complex-vector-fillptr array) 0))))

;;------------------------------------------------------------------------------
;; FILL-POINTER vector
;;------------------------------------------------------------------------------
(defun fill-pointer (vector)
  (complex-vector-fillptr vector))

;;------------------------------------------------------------------------------
;; (SETF FILL-POINTER) new-fp vector
;;------------------------------------------------------------------------------
(defun (setf fill-pointer) (new-fp vector)
  (setf (complex-vector-fillptr vector) new-fp))

;;------------------------------------------------------------------------------
;; VECTOR-PUSH new-element vector
;;------------------------------------------------------------------------------
(defun vector-push (new-element vector)
  (let ((fp (fill-pointer vector)))
    (cond
      ((< fp (array-dimension vector 0))
       (setf (aref vector fp) new-element)
       (setf (fill-pointer vector) (1+ fp))
       fp)
      (t nil))))

;;------------------------------------------------------------------------------
;; VECTOR-PUSH-EXTEND new-element vector &OPTIONAL extension
;;------------------------------------------------------------------------------
(defun vector-push-extend (new-element vector &optional extension)
  (let ((fp (fill-pointer vector)))
    (when (>= fp (array-total-size vector))
      (adjust-array vector (if extension
                               (+ (array-total-size vector) extension)
                               (* 2 (array-total-size vector)))
                    :fill-pointer fp))
    (setf (aref vector fp) new-element)
    (setf (fill-pointer vector) (1+ fp))
    fp))

;;------------------------------------------------------------------------------
;; VECTOR-POP vector
;;------------------------------------------------------------------------------
(defun vector-pop (vector)
  (let ((fp (fill-pointer vector)))
    (when (zerop fp)
      (error "The fill pointer of the vector ~A is zero." vector))
    (decf fp)
    (setf (fill-pointer vector) fp)
    (aref vector fp)))

;;------------------------------------------------------------------------------
;; SHRINK-SIMPLE-STRING vector newsize
;;------------------------------------------------------------------------------
(defun shrink-simple-string (simple-string new-size)
  (unless (simple-string-p simple-string)
    (error WRONG_TYPE simple-string 'simple-string))
  (unless (check-integer new-size 0 (length simple-string))
    (error "~A is not a positive fixnum or is too big." new-size))
  (rt::shrink-smstr simple-string new-size))

;;------------------------------------------------------------------------------
;; STRING-TO-SIMPLE-STRING string
;;------------------------------------------------------------------------------
(defun string-to-simple-string (string)
  (cond
    ((simple-string-p string) string)
    ((and (< (complex-vector-displaced string) 0)
              (< (complex-vector-fillptr string) 0))
     (complex-vector-data string))
    (t (copy-seq string))))

;;------------------------------------------------------------------------------
;; VECTOR-LENGTH vector (für length aus seq.lisp)
;;------------------------------------------------------------------------------
(defun vector-length (vector)
  (cond
    ((rt::plain-vector-p vector) (rt::plain-vector-length vector))
    ((< (complex-vector-fillptr vector) 0) (complex-vector-length vector))
    (t (complex-vector-fillptr vector))))
