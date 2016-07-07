;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Laufzeitsystem (19. Structures)
;;;
;;; $Revision: 1.9 $
;;; $Log: struct.lisp,v $
;;; Revision 1.9  1993/10/14  13:09:36  sma
;;; rt:new-struct nach structure.c verschoben
;;;
;;; Revision 1.8  1993/06/25  12:38:23  wg
;;; Symbol rt::included-struct aus dem Package RT exportiert.
;;;
;;; Revision 1.7  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.6  1993/04/22  10:48:21  hk
;;; (in-package "RUNTIME") -> (in-package "LISP"),
;;; Definitionen exportiert, defvar, defconstant, defmacro aus
;;; clicc/lib/lisp.lisp einkopiert. rt::set-xxx in (setf xxx) umgeschrieben.
;;; Definitionen und Anwendungen von/aus Package Runtime mit rt: gekennzeichnet.
;;; declaim fun-spec und declaim top-level-form gestrichen.
;;;
;;; Revision 1.5  1993/02/16  14:34:20  hk
;;; clicc::declaim -> declaim, clicc::fun-spec (etc.) -> lisp::fun-spec (etc.)
;;; $Revision: 1.9 $ eingefuegt
;;;
;;; Revision 1.4  1993/01/11  14:38:35  hk
;;; structure -> struct
;;;
;;; Revision 1.3  1992/07/06  09:22:42  hk
;;; Schreibfehler.
;;;
;;; Revision 1.2  1992/07/06  08:18:10  hk
;;; Neue Sysntax fuer declaim fun-spec.
;;;
;;; Revision 1.1  1992/03/24  17:12:55  hk
;;; Initial revision
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(rt::struct rt::struct-typep rt::make-struct rt::copy-struct
   rt::struct-ref rt::set-struct-ref rt::struct-constructor
   rt::set-struct-constructor rt::included-struct) "RT")

;;------------------------------------------------------------------------------
;; Fuer Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant NO_STRUCT "~S is not a structure of type ~S")

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(deftype rt:struct () '(satisfies rt::structp))

;;-----------------------------------------------------------------------------
;; STRUCT-TYPEP object type
;;-----------------------------------------------------------------------------
(defun rt:struct-typep (object type)
  (cond
    ((not (rt::structp object)) NIL)
    (t (let ((struct-type (rt::struct-type object)))
         (loop
           (cond
             ((eq struct-type type)
              (return T))
             (t (setq struct-type (get struct-type 'rt::INCLUDED-STRUCT))
                (when (not struct-type)
                  (return NIL)))))))))

;;-----------------------------------------------------------------------------
;; MAKE-STRUCT type &REST slot-values
;;-----------------------------------------------------------------------------
(defun rt:make-struct (type &rest slot-values)
  (let ((new-structure (rt::new-struct (length slot-values) type)))
    (do ((i 0 (1+ i)))
        ((null slot-values) new-structure)
      (rt::set-struct-ref new-structure i type (pop slot-values)))))

;;-----------------------------------------------------------------------------
;; COPY-STRUCT structure type
;;-----------------------------------------------------------------------------
(defun rt:copy-struct (structure type)
  (when (not (rt:struct-typep structure type))
    (error NO_STRUCT structure type))
  (let* ((structure-size (rt::struct-size structure))
         (copy-of-structure (rt::new-struct structure-size type)))
    (dotimes (index structure-size)

      ;; Optimierung per Hand.
      ;; Kann sonst mit Typanalyse und INLINE-Uebersetzung erreicht werden.
      ;;-------------------------------------------------------------------
      (rt::set-struct-ref-internal copy-of-structure index
                                   (rt::struct-ref-internal structure index)))
    copy-of-structure))

;;-----------------------------------------------------------------------------
;; STRUCT-REF structure index type
;;-----------------------------------------------------------------------------
(defun rt:struct-ref (structure index type)
  (when (not (rt:struct-typep structure type))
    (error NO_STRUCT structure type))
  (rt::struct-ref-internal structure index))

;;-----------------------------------------------------------------------------
;; SET-STRUCT-REF structure index type newvalue
;;-----------------------------------------------------------------------------
(defun rt:set-struct-ref (structure index type newvalue)
  (when (not (rt:struct-typep structure type))
    (error NO_STRUCT structure type))
  (rt::set-struct-ref-internal structure index newvalue))

;;-----------------------------------------------------------------------------
;; STRUCT-CONSTRUCTOR symbol
;;-----------------------------------------------------------------------------
(defun rt:struct-constructor (symbol)
  (get symbol 'rt::STRUCT-CONSTRUCTOR))

;;-----------------------------------------------------------------------------
;; SET-STRUCT-CONSTRUCTOR symbol constructor
;;-----------------------------------------------------------------------------
(defun rt:set-struct-constructor (symbol constructor)
  (setf (get symbol 'rt::STRUCT-CONSTRUCTOR) constructor))
