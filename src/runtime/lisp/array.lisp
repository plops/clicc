;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : System-Funktionen (17. Arrays)                                
;;;
;;; $Revision: 1.14 $
;;; $Log: array.lisp,v $
;;; Revision 1.14  1993/09/03  14:15:12  hk
;;; Fehler in array-in-bounds-p behoben.
;;;
;;; Revision 1.13  1993/09/03  14:07:28  hk
;;; Fehler in array-in-bounds-p behoben.
;;;
;;; Revision 1.12  1993/08/20  10:13:21  hk
;;; array-element-type prüft auf array Typ
;;;
;;; Revision 1.11  1993/08/19  13:38:19  hk
;;; array-in-bounds-p optimiert, array-rmi-internal optimiert und einige
;;; Fehlerabfragen entfernt.
;;;
;;; Revision 1.10  1993/07/15  10:20:21  hk
;;; Neue Funktionen sub-type-code-p und type-code-p eingefuegt, die auf
;;; internen Typkodierungen arbeiten. typep und subtypep werden nicht mehr
;;; verwendet.
;;;
;;; Revision 1.9  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.8  1993/05/08  18:14:56  hk
;;; Aufrufe von set-row-major-aref-internal, set-svref-internal und
;;;  set-fill-pointer-internal geaendert.
;;;
;;; Revision 1.7  1993/04/22  10:48:21  hk
;;; (in-package "RUNTIME") -> (in-package "LISP"),
;;; Definitionen exportiert, defvar, defconstant, defmacro aus
;;; clicc/lib/lisp.lisp einkopiert. rt::set-xxx in (setf xxx) umgeschrieben.
;;; Definitionen und Anwendungen von/aus Package Runtime mit rt: gekennzeichnet.
;;; declaim fun-spec und declaim top-level-form gestrichen.
;;;
;;; Revision 1.6  1993/02/16  14:34:20  hk
;;; clicc::declaim -> declaim, clicc::fun-spec (etc.) -> lisp::fun-spec (etc.)
;;; $Revision: 1.14 $ eingefuegt
;;;
;;; Revision 1.5  1993/01/19  12:11:03  hk
;;; string-char gestrichen in upgraded-array-element-type und
;;; to-internal-element-type.
;;;
;;; Revision 1.4  1992/07/29  13:12:24  hk
;;; Character ist als Element-Typ von Strings zulaessig.
;;;
;;; Revision 1.3  1992/07/06  09:11:43  hk
;;; Schreibfehler.
;;;
;;; Revision 1.2  1992/07/06  08:25:24  hk
;;; Neue Syntax fuer declaim fun-spec.
;;;
;;; Revision 1.1  1992/03/24  17:12:55  hk
;;; Initial revision
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(upgraded-array-element-type
   make-array vector aref row-major-aref svref array-element-type
   array-dimension array-dimensions array-in-bounds-p array-row-major-index
   fill-pointer vector-push vector-push-extend vector-pop adjust-array))

(export '(rt::vref) "RT")

;;------------------------------------------------------------------------------
;; Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant DISPLACED_NOT_WITH ":displaced-to option may not be used with ~A.")
(defconstant NO_ARRAY "~A should be of type ARRAY")
(defconstant OUT_OF_RANGE "The index ~S is not in the range [0, ~S)")

;;-----------------------------------------------------------------------------
;;  MAKE-ARRAY dimensions
;;     &KEY :element-type :initial-element :initial-contents                   
;;          :adjustable   :fill-pointer                                        
;;          :displaced-to :displaced-index-offset                              
;;-----------------------------------------------------------------------------
(defun make-array (dimensions
                   &key
                   (element-type t)
                   (initial-element  nil init-elem-sp)
                   (initial-contents nil init-cont-sp)
                   adjustable
                   fill-pointer
                   displaced-to
                   (displaced-index-offset 0 displ-io-sp))

  (when (atom dimensions) (setq dimensions (list dimensions)))
  (let ((rank (length dimensions))
        (size (check-dimensions dimensions))
        (type-code (to-type-code element-type)))
    (when (and init-elem-sp
               (not (type-code-p initial-element type-code)))
      (error "The :initial-element, ~A, is of the wrong type; ~
                     it should be of type ~A"
             initial-element (to-element-type type-code)))
    (when init-cont-sp
      (when init-elem-sp
        (error ":initial-element and :initial-contents are both supplied."))
      (labels ((chk-shape-of-init-cont (dims init-cont)
                 (cond
                   ((null dims))
                   ((/= (first dims) (length init-cont))
                    (error "The :initial-contents argument, ~A,
                                    isn't the correct shape or size
                                    for an array of rank ~A with dimensions ~A"
                           initial-contents rank dimensions))
                   (t (mapc #'(lambda (reduced-init-cont)
                                (chk-shape-of-init-cont
                                 (rest dims)
                                 reduced-init-cont))
                            init-cont)))))
        (chk-shape-of-init-cont dimensions initial-contents)))
    (when displaced-to
      (when init-elem-sp
        (error DISPLACED_NOT_WITH :initial-element))
      (when init-cont-sp
        (error DISPLACED_NOT_WITH :initial-contents))
      (when (not (arrayp displaced-to))
        (error "The :displaced-to argument is not an array."))
      (when (not (sub-type-code-p
                  type-code
                  (rt::array-element-type-internal displaced-to)))
        (error "Cannot displace the array, because it has a different type."))
      (when (> size (array-total-size displaced-to))
        (error "Cannot displace the array, ~
                because the total size of the to-array is too small.")))
    
    ;; Die Groesse des neuen Array's + DISPLACED-INDEX-OFFSET darf 
    ;;  nicht groesser als die Groesse des Displaced To Array's sein 
    ;;---------------------------------------------------------------
    (when displ-io-sp
      (when (not displaced-to)
        (error "A :displaced-index-offset argument is given to make-array, ~
                        but there is no :displaced-to array"))
      (let ((size-of-displaced-to (array-total-size displaced-to)))
        (when (> size size-of-displaced-to)
          (error "The displaced-to array is smaller than the displaced array"))
        (when (not (check-integer displaced-index-offset
                                  0 (- size-of-displaced-to size)))
          (error "The :displaced-index-offset argument, ~A, ~
                  is not in the linearized range [0, ~A) ~
                  of the :displaced-to array"
                 displaced-index-offset
                 (- (array-total-size displaced-to) size)))))

    (if (= rank 1)
      (let (vector)
        (when (and fill-pointer
                   (not (check-integer fill-pointer 0 size)))
          (error "The fill-pointer ~A must be an integer in the range [0, ~A]"
                 fill-pointer size))
        (setq vector
              (rt::make-vector-internal type-code size
                                    adjustable fill-pointer
                                    displaced-to displaced-index-offset))
        (when init-elem-sp
          (fill vector initial-element))
        (when init-cont-sp
          (replace vector initial-contents))
        vector)
      (let (array)
        (when fill-pointer
          (error "Fill-pointers are only for 1-dimensional arrays."))
        (setq array
              (apply #'rt::make-array-internal
                     type-code size
                     adjustable
                     displaced-to displaced-index-offset
                     rank dimensions))
        (when init-elem-sp
          (dotimes (i (array-total-size array))
            (setf (row-major-aref array i) initial-element)))
        (when init-cont-sp
          (labels ((set-initial-contents (rm-index dims init-cont)
                     (if (null dims)
                       (progn
                         (setf (row-major-aref array rm-index) init-cont)
                         1)
                       (do ((first-dim (first dims))
                            (rest-dims (rest  dims))
                            (i 0 (1+ i))
                            (offset 0))
                           ((>= i first-dim) offset)
                         (incf offset
                               (set-initial-contents
                                (+ rm-index offset)
                                rest-dims
                                (elt init-cont i)))))))
            (set-initial-contents 0 dimensions initial-contents)))
        array))))

;;----------------------------------------------------------------------------
;; Analysiert die Dimensionen beim Aufruf von MAKE-ARRAY und ADJUST-ARRAY.
;; Rueckgabewert: Gesamtgroesse des Arrays
;;----------------------------------------------------------------------------
(defun check-dimensions (dimensions)
  (do (dimension
       (size 1 (* size dimension)))
      ((null dimensions) size)
    (setq dimension (pop dimensions))
    (when (not (check-integer dimension 0 most-positive-fixnum))
      (error "A dimension argument to MAKE-ARRAY, ~A, is unusable ~
                     since it is not a positive fixnum." dimension))))

;;-----------------------------------------------------------------------------
;;  ARRAY-RANK-LIMIT       =
;;  ARRAY-DIMENSION-LIMIT  =
;;  ARRAY-TOTAL-SIZE-LIMIT =
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; VECTOR &REST objects
;;-----------------------------------------------------------------------------
(defun vector (&rest objects)
  (make-array (length objects)
              :element-type t
              :initial-contents objects))

;;-----------------------------------------------------------------------------
;; 17.2. Array Access                                                         
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; AREF array &REST subscripts
;;-----------------------------------------------------------------------------
(defun aref (array &rest subscripts)
   (row-major-aref array (array-rmi-internal array subscripts)))

;;-----------------------------------------------------------------------------
;; (SETF AREF) newvalue array &REST subscripts
;;-----------------------------------------------------------------------------
(defun (setf aref) (newvalue array &rest subscripts)
  (setf (row-major-aref array (array-rmi-internal array subscripts)) newvalue))

;;-----------------------------------------------------------------------------
;; ROW-MAJOR-AREF array index
;;-----------------------------------------------------------------------------
(defun row-major-aref (array index)
  (when (not (arrayp array))
    (error NO_ARRAY array))
  (when (not (check-integer index 0 (1- (array-total-size array))))
    (error OUT_OF_RANGE index (array-total-size array)))
  (rt::row-major-aref-internal array index))

;;-----------------------------------------------------------------------------
;; (SETF ROW-MAJOR-AREF) newvalue array index
;;-----------------------------------------------------------------------------
(defun (setf row-major-aref) (newvalue array index)
  (when (not (arrayp array))
    (error NO_ARRAY array))
  (when (not (check-integer index 0 (1- (array-total-size array))))
    (error OUT_OF_RANGE index (array-total-size array)))
  (when (not (type-code-p newvalue (rt::array-element-type-internal array)))
    (error "The value ~A, is the wrong type for storing into a ~A array."
           newvalue (array-element-type array)))
  (rt::set-row-major-aref-internal newvalue array index))

;;-----------------------------------------------------------------------------
;; VREF vector index                                                          
;; Source-Level Optimierung: (aref vector index) -> (vref vector index)       
;;    Hat den Vorteil, dass kein &REST-Parameter benoetigt wird.              
;;-----------------------------------------------------------------------------
(defun rt:vref (vector index)
  (when (not (typep vector 'VECTOR))
    (error "~A should be of type VECTOR" vector))
  (row-major-aref vector index))

;;-----------------------------------------------------------------------------
;; (SETF VREF) newvalue array index
;;-----------------------------------------------------------------------------
(defun (setf rt:vref) (newvalue vector index)
  (when (not (typep vector 'VECTOR))
    (error "~A should be of type VECTOR" vector))
  (setf (row-major-aref vector index) newvalue))

;;-----------------------------------------------------------------------------
;; SVREF simple-vector index
;;-----------------------------------------------------------------------------
(defun svref (simple-vector index)
  (when (not (typep simple-vector 'SIMPLE-VECTOR))
    (error "~A should be of type SIMPLE-VECTOR" simple-vector))
  (when (not (check-integer index 0 (1- (length simple-vector))))
    (error OUT_OF_RANGE index (length simple-vector)))
  (rt::svref-internal simple-vector index))

;;-----------------------------------------------------------------------------
;; (SETF SVREF) newvalue simple-vector index
;;-----------------------------------------------------------------------------
(defun (setf svref) (newvalue simple-vector index)
  (when (not (simple-vector-p simple-vector))
    (error WRONG_TYPE simple-vector 'SIMPLE-VECTOR))
  (when (not (check-integer index 0 (1- (array-total-size simple-vector))))
    (error OUT_OF_RANGE index (array-total-size simple-vector)))
  (rt::set-svref-internal newvalue simple-vector index))

;;-----------------------------------------------------------------------------
;; 17.3. Array Information                                                    
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; ARRAY-ELEMENT-TYPE array
;;-----------------------------------------------------------------------------
(defun array-element-type (array)
  (when (not (arrayp array))
    (error NO_ARRAY array))
  (to-element-type (rt::array-element-type-internal array)))

;;------------------------------------------------------------------------------
;; Konvertiert die interne Codierung des Array-Element-Type in einen Typ
;; (ein Symbol)
;;------------------------------------------------------------------------------
(defun to-element-type (code)
  (case code
    (0 t)
    (1 'fixnum)
    (2 'float)
    (3 'character)))

;;----------------------------------------------------------------------------
;; Konvertiert den Array-Element-Type (ein Symbol) in eine interne Codierung.
;;----------------------------------------------------------------------------
(defun to-type-code (element-type)
   (case element-type
      (fixnum 1)
      ((float short-float single-float double-float long-float) 2)
      ((standard-char character) 3)
      (otherwise 0)))

;;-----------------------------------------------------------------------------
;;  UPGRADED-ARRAY-ELEMENT-TYPE type
;;-----------------------------------------------------------------------------
(defun upgraded-array-element-type (type)
   (case type
      (fixnum 'fixnum)
      ((standard-char character) 'character)
      ((float short-float single-float double-float long-float) 'float)
      (otherwise t)))

;;------------------------------------------------------------------------------
;; sub-type-code-p code1 code2
;;------------------------------------------------------------------------------
(defun sub-type-code-p (t1 t2)
  (or (eql 0 t2) (eql t1 t2)))

;;------------------------------------------------------------------------------
;; type-code-p x code
;;------------------------------------------------------------------------------
(defun type-code-p (x code)
  (case code
    (0 t)
    (1 (typep x 'fixnum))
    (2 (typep x 'float))
    (3 (typep x 'character))))

;;-----------------------------------------------------------------------------
;; ARRAY-DIMENSION array axis-number
;;-----------------------------------------------------------------------------
(defun array-dimension (array axis-number)
  (when (not (arrayp array))
    (error NO_ARRAY array))

  (when (not (check-integer axis-number 0 (1- (array-rank array))))
    (error "The value ~A is not suitable as an axis-number ~
              for an array with rank ~A" axis-number (array-rank array)))
  (rt::array-dimension-internal array axis-number))

;;-----------------------------------------------------------------------------
;; ARRAY-DIMENSIONS array
;;-----------------------------------------------------------------------------
(defun array-dimensions (array)
  (do ((i (array-rank array))
       (dimensions () (cons (array-dimension array i) dimensions)))
      ((= i 0) dimensions)
    (decf i)))

;;-----------------------------------------------------------------------------
;; ARRAY-IN-BOUNDS-P array &REST subscripts
;;-----------------------------------------------------------------------------
(defun array-in-bounds-p (array &rest subscripts)
  (when (/= (array-rank array) (length subscripts))
    (error "Wrong number of subscripts for array ~a" array))
  (let ((i 0))
    (dolist (index subscripts t)
      (unless (check-integer index 0 (1- (array-dimension array i)))
        (return nil))
      (incf i))))

;;-----------------------------------------------------------------------------
;; ARRAY-ROW-MAJOR-INDEX array &REST subscripts
;;-----------------------------------------------------------------------------
(defun array-row-major-index (array &rest subscripts)
  (array-rmi-internal array subscripts))

;; Das Splitten in eine Funktion mit und ohne &REST-Parameter hat den Vorteil,
;; dass wenn die 'subscripts' bereits als Liste vorliegen, 'array-rmi-internal'
;; aufgerufen werden kann und dadurch ein ueberfluessiges nochmaliges Erzeugen
;; der Liste vermieden wird.

(defun array-rmi-internal (array subscripts)
  (when (not (arrayp array))
    (error NO_ARRAY array))
  (let ((i 0)
        (row-major-index 0))
    (dolist (index subscripts)
      (let ((dim (array-dimension array i)))
        (incf i)
        (setq row-major-index (+ (* row-major-index dim) index))))
    row-major-index))

;;-----------------------------------------------------------------------------
;; 17.5. Fill Pointers                                                        
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; (SETF FILL-POINTER) new-fp vector
;;-----------------------------------------------------------------------------
(defun (setf fill-pointer) (new-fp vector)
  (when (not (array-has-fill-pointer-p vector))
    (error "The argument ~A is not a vector with a fill-pointer." vector))
  (when (not (check-integer new-fp 0 (array-total-size vector)))
    (error "The arg ~S given to SETF of FILL-POINTER is not in ~
              range for an array of total size ~S."
           new-fp (array-total-size vector)))
  (rt::set-fill-pointer-internal new-fp vector))

;;-----------------------------------------------------------------------------
;; VECTOR-PUSH new-element vector
;;-----------------------------------------------------------------------------
(defun vector-push (new-element vector)
  (let ((fp (fill-pointer vector)))
    (cond
      ((< fp (array-dimension vector 0))
       (setf (aref vector fp) new-element)
       (setf (fill-pointer vector) (1+ fp))
       fp)
      (t nil))))

;;-----------------------------------------------------------------------------
;; VECTOR-PUSH-EXTEND new-element vector &OPTIONAL extension
;;-----------------------------------------------------------------------------
(defun vector-push-extend (new-element
                           vector
                           &optional
                           (extension (1+ (array-total-size vector))))
  (let ((fp (fill-pointer vector)))
    (when (>= fp (array-total-size vector))
      (adjust-array vector (+ (array-total-size vector) extension)
                    :fill-pointer fp))
    (setf (aref vector fp) new-element)
    (setf (fill-pointer vector) (1+ fp))
    fp))

;;-----------------------------------------------------------------------------
;; VECTOR-POP vector
;;-----------------------------------------------------------------------------
(defun vector-pop (vector)
  (let ((fp (fill-pointer vector)))
    (when (zerop fp)
      (error "The fill pointer of the vector ~A is zero." vector))
    (decf fp)
    (setf (fill-pointer vector) fp)
    (aref vector fp)))

;;-----------------------------------------------------------------------------
;; 17.6. Changing the Dimensions of an Array                                  
;;-----------------------------------------------------------------------------
(defun rmi-to-subscripts (dimensions row-major-index)
  (cond
    ((null dimensions) nil)
    (t (let ((n-1_dimensions (rest dimensions)))
         (multiple-value-bind (subscript remainder)
             (floor row-major-index (apply #'* n-1_dimensions))
           (cons subscript (rmi-to-subscripts n-1_dimensions remainder)))))))

;;-----------------------------------------------------------------------------
;; ADJUST-ARRAY array new-dimensions
;;    &KEY :element-type :initial-element :initial-contents                   
;;         :adjustable   :fill-pointer                                        
;;         :displaced-to :displaced-index-offset                              
;;-----------------------------------------------------------------------------
(defun adjust-array (array new-dimensions
                           &rest rest-args
                           &key (element-type nil elem-type-sp)
                           initial-element
                           initial-contents
                           (fill-pointer nil fill-pointer-sp)
                           displaced-to
                           displaced-index-offset)
  (declare (ignore initial-element displaced-to displaced-index-offset))
  (let ((original-element-type (array-element-type array))
        (original-dimensions   (array-dimensions   array))
        new-array
        subscripts)
    (unless (adjustable-array-p array)
      (error "The 'array' argument given to ADJUST-ARRAY, ~A,
                     isn't an adjustable array." array))
    (when (atom new-dimensions) (setq new-dimensions (list new-dimensions)))
    (when (/= (length new-dimensions) (array-rank array))
      (error "The 'new-dimensions' argument given to ADJUST-ARRAY, ~A, ~
              is not the same rank as the argument array." new-dimensions))
    (when elem-type-sp
      (setq element-type (upgraded-array-element-type element-type))
      (unless (eq element-type original-element-type)
        (error "The 'element-type' argument to ADJUST-ARRAY, ~A ~
                  is inconsistent with that of the array to be adjusted [~A]."
               element-type original-element-type)))
    (cond
      (fill-pointer-sp
       (when (not (array-has-fill-pointer-p array))
         (error "A :fill-pointer argument was given to ADJUST-ARRAY, ~
   		 but the array being adjusted, doesn't have one."))
       (when (null fill-pointer)
         (setq fill-pointer (fill-pointer array))))
      (t (when (array-has-fill-pointer-p array)
           (setq fill-pointer (fill-pointer array)))))
					
    (setq new-array
          (apply #'make-array new-dimensions
                 :element-type original-element-type
                 :adjustable   T
                 :fill-pointer fill-pointer
                 rest-args))
    (unless initial-contents
      (dotimes (i (array-total-size array))
        (setq subscripts (rmi-to-subscripts original-dimensions i))
        (when (apply #'array-in-bounds-p new-array subscripts)
          (setf (apply #'aref new-array subscripts)
                (row-major-aref array i)))))
    (rt::displace-array array new-array)))
