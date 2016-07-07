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
;;; Funktion : Funktionen und Makros zum Gewinnen von Typzusicherungen aus
;;;            Ausdruecken, die an Praedikatsposition von Konditionalen stehen.
;;;
;;; $Revision: 1.20 $
;;; $Id: tiassert.lisp,v 1.20 1994/11/22 14:49:16 hk Exp $
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
(defmacro dec-type-assertion (function-name an-optimization-function)
  `(push (cons (get-global-fun ',function-name) ,an-optimization-function) 
         *ti-predicate-assertion-environment*))


;;------------------------------------------------------------------------------
;; Makro zum Deklarieren einer Typzusicherung durch ein Typpraedikat.
;;------------------------------------------------------------------------------
(defmacro dec-typepred-assertion (function-name then-assert else-assert)
  `(dec-type-assertion ,function-name
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
  (dec-type-assertion L::eq      #'eq-type-assertion)
  (dec-type-assertion L::eql     #'eq-type-assertion)
  (dec-type-assertion L::equal   #'eq-type-assertion)
  
  (dec-type-assertion L::=       #'eq-type-assertion)
  (dec-type-assertion L::string= #'eq-type-assertion)
  
  ;;----------------------------------------------------------------------------
  ;; Typdeklaration zu den Typtests
  ;;----------------------------------------------------------------------------
  (dec-typepred-assertion L::endp             null-t         cons-t)
  (dec-typepred-assertion L::symbolp          symbol-t       not-symbol-t)
  (dec-typepred-assertion rt::symp            non-null-sym-t not-non-null-sym-t)
  (dec-typepred-assertion L::atom             atom-t         cons-t)
  (dec-typepred-assertion L::consp            cons-t         atom-t)
  (dec-typepred-assertion L::listp            all-list-t     not-all-list-t)

  (dec-typepred-assertion L::zerop            byte-t         number-t)
  (dec-typepred-assertion rt::fixnump         fixnum-t       not-fixnum-t)
  (dec-typepred-assertion L::integerp         integer-t      not-integer-t)
  (dec-typepred-assertion L::floatp           float-t        not-float-t)
  (dec-typepred-assertion L::numberp          number-t       not-number-t)
 
  (dec-typepred-assertion L::characterp       character-t    not-character-t)

  (dec-typepred-assertion L::stringp          string-t       not-string-t)
  (dec-typepred-assertion L::vectorp          vector-t       not-vector-t)
  (dec-typepred-assertion L::simple-vector-p  vector-t       Top-t)
  (dec-typepred-assertion L::simple-stringr-p string-t       Top-t)
  (dec-typepred-assertion L::arrayp           array-t        not-array-t)
  (dec-typepred-assertion L::simple-array-p   array-t        Top-t)
  (dec-typepred-assertion L::plain-vector-p   array-t        Top-t)

  (dec-typepred-assertion L::functionp        function-t     not-function-t)
  (dec-typepred-assertion rt::structp         structure-t    not-structure-t)
  (dec-typepred-assertion rt::struct-typp     structure-t    Top-t)
  (dec-typepred-assertion rt::instancep       class-t        not-class-t)

  (dec-typepred-assertion L::packagep         package-t      not-package-t)
  (dec-typepred-assertion L::streamp          stream-t       not-stream-t)
  (dec-typepred-assertion L::hash-table-p     hash-table-t   not-hash-table-t)
  (dec-typepred-assertion L::readtable-p      readtable-t    not-readtable-t)
  (dec-typepred-assertion L::pathname-p       pathname-t     not-pathname-t)

  ;;----------------------------------------------------------------------------
  ;; Typdeklaration für car- und cdr-Zugriffe auf Listen
  ;;----------------------------------------------------------------------------
  (dec-type-assertion L::car    #'car-type-assertion)
  (dec-type-assertion L::caar   #'car-type-assertion)
  (dec-type-assertion L::cadr   #'car-type-assertion)
  (dec-type-assertion L::caaar  #'car-type-assertion)
  (dec-type-assertion L::caadr  #'car-type-assertion)
  (dec-type-assertion L::cadar  #'car-type-assertion)
  (dec-type-assertion L::caddr  #'car-type-assertion)
  (dec-type-assertion L::first  #'car-type-assertion)
  (dec-type-assertion L::second #'car-type-assertion)
  (dec-type-assertion L::third  #'car-type-assertion)
  (dec-type-assertion L::fourth #'car-type-assertion)
  (dec-type-assertion L::fifth  #'car-type-assertion)

  (dec-type-assertion L::cdr    #'cdr-type-assertion)
  (dec-type-assertion L::cdar   #'cdr-type-assertion)
  (dec-type-assertion L::cddr   #'cdr-type-assertion)
  (dec-type-assertion L::cdaar  #'cdr-type-assertion)
  (dec-type-assertion L::cdadr  #'cdr-type-assertion)
  (dec-type-assertion L::cddar  #'cdr-type-assertion)
  (dec-type-assertion L::cdddr  #'cdr-type-assertion)
  (dec-type-assertion L::rest   #'cdr-type-assertion)


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
