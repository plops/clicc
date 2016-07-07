;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Funktionen und Makros zum Gewinnen von Typzusicherungen aus
;;;            Ausdruecken, die an Praedikatsposition von Konditionalen stehen.
;;;
;;; $Revision: 1.15 $
;;; $Log: tiassert.lisp,v $
;;; Revision 1.15  1993/12/09  10:34:05  hk
;;; provide wieder an das Dateiende
;;;
;;; Revision 1.14  1993/11/21  22:05:00  kl
;;; provide anders plaziert.
;;;
;;; Revision 1.13  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.12  1993/06/10  10:23:12  kl
;;; Typsierung der Strukturen verbessert.
;;;
;;; Revision 1.11  1993/06/08  12:00:59  kl
;;; Typisierung der Funktion instancep eingefuegt.
;;;
;;; Revision 1.10  1993/05/23  15:58:24  kl
;;; Anpassung an den neuen Typverband.
;;;
;;; Revision 1.9  1993/05/14  15:43:19  kl
;;; Typzusicherungen erweitert.
;;;
;;; Revision 1.8  1993/04/22  11:25:32  hk
;;; declare-typepred-assertion rt::check-integer gestrichen.
;;;
;;; Revision 1.7  1993/04/18  15:59:38  kl
;;; Debugschalter entfernt.
;;;
;;; Revision 1.6  1993/03/04  09:10:19  kl
;;; Typzusicherungen fuer car- und cdr-Zugriffe verbessert.
;;;
;;; Revision 1.5  1993/02/16  16:11:30  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.4  1993/02/02  09:41:27  kl
;;; Zu starke Typzusicherungen bei Zahlvergleichen entfernt.
;;;
;;; Revision 1.3  1993/01/25  13:19:06  kl
;;; Typzusicherungen der Typtests an den neuen Typverband angepasst.
;;;
;;; Revision 1.2  1993/01/19  11:30:17  kl
;;; get-type-assertions-from-predicate-position aus tipass1 hierhin verlegt.
;;;
;;; Revision 1.1  1993/01/19  09:44:13  kl
;;; Initial revision
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "titypes")
(require "tidef")
(require "timisc")


;;------------------------------------------------------------------------------
;; Beim jedem Laden dieser Datei wird dieser Schalter zurueckgesetzt, weil sich
;; Typzusicherungen geaendert haben koennen.
;;------------------------------------------------------------------------------
(setf *ti-type-declarations-are-initialized* nil)


;;------------------------------------------------------------------------------
;; Makro fuer die Deklaration einer Typzusicherung fuer Ausdrucke an Praedikats-
;; position eines Konditionals.
;;------------------------------------------------------------------------------
(defmacro declare-type-assertion (function-name an-optimization-function)
  `(push (cons (get-global-fun ',function-name) ,an-optimization-function) 
         *ti-predicate-assertion-environment*))


;;------------------------------------------------------------------------------
;; Makro zum Deklarieren einer Typzusicherung durch ein Typpraedikat.
;;------------------------------------------------------------------------------
(defmacro declare-typepred-assertion (function-name then-assert else-assert)
  `(declare-type-assertion ,function-name
    #'(lambda (&rest args)
        (let ((var-ref (first args)))
          (if (var-ref-p var-ref)
              (values (list (?var var-ref)) (list ,then-assert) 
                      (list (?var var-ref)) (list ,else-assert))
              (values nil nil nil nil))))))


;;------------------------------------------------------------------------------
;; Funktion zum Gewinnen von Typzusicherungen aus Vergleichspraedikaten.
;;------------------------------------------------------------------------------
(defun eq-type-assertion (&rest args)
  (let ((type-meet (type-meet (?type (first args)) (?type (second args)))))
    
    (values (mapcar #'?var (remove-if-not #'var-ref-p args))
            (list type-meet type-meet)
            nil nil)))


;;------------------------------------------------------------------------------
;; Funktion zum Gewinnen von Typzusicherungen aus car-aehnlichen Listenzugriffen
;;------------------------------------------------------------------------------
(defun car-type-assertion (arg)
  (if (var-ref-p arg)
      (let ((var (?var arg)))
        (values (list var) (list cons-t) nil nil))
      (values nil nil nil nil)))


;;------------------------------------------------------------------------------
;; Funktion zum Gewinnen von Typzusicherungen aus cdr-aehnlichen Listenzugriffen
;;------------------------------------------------------------------------------
(defun cdr-type-assertion (arg)
  (if (var-ref-p arg)
      (let ((var (?var arg)))
        (values (list var) (list cons-t) nil nil))
      (values nil nil nil nil)))


;;------------------------------------------------------------------------------
;; Funktion zum Initialisieren der Funktionen zur Typzusicherung.
;;------------------------------------------------------------------------------
(defun initialize-type-assertion-functions ()

 
  ;;----------------------------------------------------------------------------
  ;; Besondere Typdeklarationen fuer die Praedikate.
  ;;----------------------------------------------------------------------------
  (declare-type-assertion L::eq      #'eq-type-assertion)
  (declare-type-assertion L::eql     #'eq-type-assertion)
  (declare-type-assertion L::equal   #'eq-type-assertion)
  
  (declare-type-assertion L::=       #'eq-type-assertion)
  (declare-type-assertion L::string= #'eq-type-assertion)
  
  ;;----------------------------------------------------------------------------
  
  (declare-typepred-assertion L::endp         null-t       cons-t)
  (declare-typepred-assertion L::symbolp      symbol-t     not-symbol-t)
  (declare-typepred-assertion L::atom         atom-t       cons-t)
  (declare-typepred-assertion L::consp        cons-t       atom-t)
  (declare-typepred-assertion L::listp        list-t       not-list-t)
  (declare-typepred-assertion L::numberp      number-t     not-number-t)
; (declare-typepred-assertion L::fixnump      fixnum-t     not-fixnum-t)
  (declare-typepred-assertion L::integerp     integer-t    not-integer-t)
  (declare-typepred-assertion L::floatp       float-t      not-float-t)
  (declare-typepred-assertion L::characterp   character-t  not-character-t)
  (declare-typepred-assertion L::stringp      string-t     not-string-t)
  (declare-typepred-assertion L::vectorp      vector-t     not-vector-t)
  (declare-typepred-assertion L::arrayp       array-t      not-array-t)
  (declare-typepred-assertion L::functionp    function-t   not-function-t)
  (declare-typepred-assertion L::zerop        fixnum-t     number-t)
  (declare-typepred-assertion rt::structp     structure-t  not-structure-t)
  (declare-typepred-assertion rt::instancep   class-t      not-class-t)

  ;;----------------------------------------------------------------------------
  ;; Typdeklaration fuer car- und cdr-Zugriffe auf Listen
  ;;----------------------------------------------------------------------------
  (declare-type-assertion L::car    #'car-type-assertion)
  (declare-type-assertion L::caar   #'car-type-assertion)
  (declare-type-assertion L::cadr   #'car-type-assertion)
  (declare-type-assertion L::caaar  #'car-type-assertion)
  (declare-type-assertion L::caadr  #'car-type-assertion)
  (declare-type-assertion L::cadar  #'car-type-assertion)
  (declare-type-assertion L::caddr  #'car-type-assertion)
  (declare-type-assertion L::first  #'car-type-assertion)
  (declare-type-assertion L::second #'car-type-assertion)
  (declare-type-assertion L::third  #'car-type-assertion)
  (declare-type-assertion L::fourth #'car-type-assertion)
  (declare-type-assertion L::fifth  #'car-type-assertion)

  (declare-type-assertion L::cdr    #'cdr-type-assertion)
  (declare-type-assertion L::cdar   #'cdr-type-assertion)
  (declare-type-assertion L::cddr   #'cdr-type-assertion)
  (declare-type-assertion L::cdaar  #'cdr-type-assertion)
  (declare-type-assertion L::cdadr  #'cdr-type-assertion)
  (declare-type-assertion L::cddar  #'cdr-type-assertion)
  (declare-type-assertion L::cdddr  #'cdr-type-assertion)
  (declare-type-assertion L::rest   #'cdr-type-assertion)


) ; initialize-type-assertion-functions



;;------------------------------------------------------------------------------
;; Liefere zu einem Ausdruck A und einer Typumgebung TE die beiden folgenden
;; Typumgebungen:
;; Die erste enthaelt die Typbindungen aus TE erweitert um diejenigen 
;; Typzusicherungen, die sich ergeben, wenn A ungleich nil ist.
;; Die zweite Ergebnistypbindung enthaelt die Typbindungen aus TE erweitert um
;; diejenigen Typzusicherungen, die sich ergeben, wenn A gleich nil ist.
;;------------------------------------------------------------------------------
(defun get-type-assertions-from-predicate-position (predicate type-environment)
  (let ((then-env (copy-type-env type-environment))
        (else-env (copy-type-env type-environment)))

    ;; Wenn an Praedikatsposition eine Variable steht, dann aendere die 
    ;; Typumgebungen des then- und else-Falls entsprechend ab.
    ;;-----------------------------------------------------------------
    (when (var-ref-p predicate)
      (let ((var (?var predicate)))
        (assert-type var not-null-t then-env)
        (assert-type var null-t     else-env)))
        
    
    ;; Wenn das Praedikat ein Typtest auf eine Variable ist, dann aendere 
    ;; die Typumgebungen des then- und else-Falls entsprechend ab.
    ;;-------------------------------------------------------------------
    (when (app-p predicate)
      (let ((assoc (assoc (?form predicate) 
                          *ti-predicate-assertion-environment*)))
        (when assoc
          (multiple-value-bind (then-assertion-vars 
                                then-assertion-types
                                else-assertion-vars 
                                else-assertion-types)
              (apply (rest assoc) (?arg-list predicate))

            (mapc #'(lambda (var type) (assert-type var type then-env))
                  then-assertion-vars then-assertion-types)

            (mapc #'(lambda (var type) (assert-type var type else-env))
                  else-assertion-vars else-assertion-types)))))

    (values then-env else-env)))
    

;;------------------------------------------------------------------------------
(provide "tiassert")
