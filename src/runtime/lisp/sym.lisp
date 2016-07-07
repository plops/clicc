;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Laufzeitsystem
;;;            - GET, SET-GET
;;;            - REMPROP
;;;            - GETF, SETF
;;;            - GET-PROPERTIES
;;;            - COPY-SYMBOL
;;;            - GENSYM, GENTEMP
;;;            - KEYWORDP
;;;
;;; $Revision: 1.4 $
;;; $Log: sym.lisp,v $
;;; Revision 1.4  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.3  1993/04/22  10:48:21  hk
;;; (in-package "RUNTIME") -> (in-package "LISP"),
;;; Definitionen exportiert, defvar, defconstant, defmacro aus
;;; clicc/lib/lisp.lisp einkopiert. rt::set-xxx in (setf xxx) umgeschrieben.
;;; Definitionen und Anwendungen von/aus Package Runtime mit rt: gekennzeichnet.
;;; declaim fun-spec und declaim top-level-form gestrichen.
;;;
;;; Revision 1.2  1993/02/16  14:34:20  hk
;;; clicc::declaim -> declaim, clicc::fun-spec (etc.) -> lisp::fun-spec (etc.)
;;; $Revision: 1.4 $ eingefuegt
;;;
;;; Revision 1.1  1992/03/24  17:12:55  hk
;;; Initial revision
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(get remprop getf get-properties copy-symbol gensym *gensym-counter*
   gentemp keywordp))

(export '(rt::set-prop rt::remf-internal) "RT")

;;--------------------------------------------------------------------------
; Verhalten, falls die Property-List-Funktionen auf Werte angewandt werden, 
; die keine Property-Listen sind: 
; - Liste mit gerader Anzahl von Elementen, INDICATOR nicht vorhanden,
;   (CDR(LAST pliste)) = Atom != NIL
;     - getf:     default
;     - set-getf: (indicator value ... ... . atom)
;     - remf-internal: (... ... . atom)
; - Liste mit ungerader Anzahl von Elementen, letztes Element = INDICATOR
;     - getf:     NIL
;     - set-getf: Error "RPLACA applied to empty List"
;     - remf-internal: letztes Element wird entfernt
; - Liste mit ungerader Anzahl von Elementen, (CDR (LAST pliste)) = ATOM
;   (CAR (LAST pliste)) = INDICATOR
;     - getf, set-getf, remf-internal: Error "CDR applied to non NIL atom"
;;--------------------------------------------------------------------------

;;--------------------------------------------------------------------------
(defun get (symbol indicator &optional default)
  (getf (symbol-plist symbol) indicator default))

;;--------------------------------------------------------------------------
(defun (setf get) (value symbol indicator &optional default)
  (setf (getf (symbol-plist symbol) indicator) value)
  value)

;;--------------------------------------------------------------------------
(defun remprop (symbol indicator)
  (remf (symbol-plist symbol) indicator))

;;--------------------------------------------------------------------------
;; Hilfsfunktionen fuer getf, set-getf
;;--------------------------------------------------------------------------
(defun get-prop (list indicator)
  (cond
    ((atom list) nil) 
    ((eq (car list) indicator) list)
    (t (get-prop (cddr list) indicator))))

;;--------------------------------------------------------------------------
;; Resultat: veraenderte / erweiterte Pliste
;;--------------------------------------------------------------------------
(defun rt:set-prop (plist indicator value)
  (let ((list (get-prop plist indicator)))
    (cond
      (list
       (rplaca (cdr list) value)
       plist)
      (t (cons indicator (cons value plist))))))

;;--------------------------------------------------------------------------
(defun getf (place indicator &optional default)
  (let ((list (get-prop place indicator)))
    (if list
      (cadr list)
      default)))

;;--------------------------------------------------------------------------
;; (remf place :test) -->
;; (multiple-value-bind (plist found)
;;     (remf-internal x :test)
;;   (if found
;;     (progn (setf place plist) t)
;;     nil))
;; Resultate
;; 1. verkleinerte Liste oder NIL
;; 2. Wert gibt an, ob Indicator gefunden wurde
;;--------------------------------------------------------------------------
(defun rt:remf-internal (list indicator)
  (cond
    ((atom list) (values nil nil))
    ((eq (car list) indicator) (values (cddr list) t))
    (t (do ((list1 list (cddr list1))
            (list2 (cddr list) (cddr list2)))
           ((atom list2) (values nil nil)) ;end test
         (when (eq (car list2) indicator)
           (rplacd (cdr list1) (cddr list2))
           (return-from rt:remf-internal (values list t)))))))
 
;;--------------------------------------------------------------------------
(defun get-properties (list indicator-list)
  (cond
    ((atom list) (values nil nil nil))
    ((member (car list) indicator-list) (values (car list) (cadr list) list))
    (t (get-properties (cddr list) indicator-list)))) 

;;--------------------------------------------------------------------------
;; copy-symbol sym &optional copy-props
;;--------------------------------------------------------------------------
(defun copy-symbol (sym &optional copy-props)
  (let ((new-sym (make-symbol (symbol-name sym))))
    (when copy-props
      (when (boundp sym)
        (set new-sym (symbol-value sym)))
      (setf (symbol-plist new-sym) (copy-list (symbol-plist sym))))
    new-sym))

;;--------------------------------------------------------------------------
;; gensym &optional x
;;--------------------------------------------------------------------------
(defparameter *gensym-prefix* "G")
(defparameter *gensym-counter*  0)

(defun gensym (&optional x)
  (typecase x
    ((null))
    ((integer 0 *) (setq *gensym-counter* x))
    (string (setq *gensym-prefix* x))
    (otherwise (error "positive integer or string expected")))
  (prog1
      (make-symbol
       (concatenate 'string *gensym-prefix*
                    (prin1-to-string *gensym-counter*)))
    (incf *gensym-counter*)))

;;--------------------------------------------------------------------------
;; gentemp &optional (prefix "T") (package *package*)
;;--------------------------------------------------------------------------
(defparameter *gentemp-counter* 0)

(defun gentemp (&optional (prefix "T") (package *package*))
  (unless (stringp prefix)
    (error "string expected"))
  (loop
    (incf *gentemp-counter*)
    
    (let ((name (concatenate 'string
                             prefix
                             (prin1-to-string *gentemp-counter*))))
      (multiple-value-bind (sym exists)
          (intern name package) 
        (unless exists (return sym))))))
                           
;;--------------------------------------------------------------------------
;; keywordp object
;;--------------------------------------------------------------------------
(defun keywordp (object)
  (and (symbolp object) (eq (symbol-package object) *keyword-package*)))
