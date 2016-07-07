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
;;; Funktion : Typdeklarationen der Funktionstypen importierter Funktionen.
;;;
;;; $Revision: 1.50 $
;;; $Id: tiimpdec.lisp,v 1.50 1994/12/17 11:55:26 pm Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "titypes")  
(require "tidef")
(require "timisc")

;;------------------------------------------------------------------------------
;; Beim jedem Laden dieser Datei wird dieser Schalter zurückgesetzt, weil sich
;; Funktionsbeschreibungen geändert haben könnten.
;;------------------------------------------------------------------------------
(setf *ti-type-declarations-are-initialized* nil) 


;;------------------------------------------------------------------------------
;; Einige häufig verwendete Typabstraktionsfunktionen:
;;------------------------------------------------------------------------------
(defun the-first-type (type1 &rest other-types)
  (declare (ignore other-types))
  type1)

;;------------------------------------------------------------------------------
(defun the-second-type (type1 type2 &rest other-types)
  (declare (ignore type1 other-types))
  type2)


;;------------------------------------------------------------------------------
(defun car-type (type)
  (zs-typecase type
               (bottom-t                  bottom-t)
               ((conform non-list-cons-t) top-t)
               ((conform null-t)          (type-join null-t 
                                                     (list-component type)))
               (otherwise                 (list-component type))))


;;------------------------------------------------------------------------------
(defun %car-type (type)
  (zs-typecase type
               (bottom-t                  bottom-t)
               ((conform non-list-cons-t) top-t)
               (otherwise                 (list-component type))))


;;------------------------------------------------------------------------------
(defun cdr-type (type)
  (zs-typecase type
               (bottom-t                  bottom-t)
               (list-t                    (type-join type null-t))
               (non-list-cons-t           not-list-t)
               ((conform non-list-cons-t) top-t)
               (otherwise                 (list-of (list-component type)))))


;;------------------------------------------------------------------------------
(defun caar-type (type)
  (car-type (car-type type)))

(defun cadr-type (type)
  (car-type (cdr-type type)))

(defun cdar-type (type)
  (cdr-type (car-type type)))

(defun cddr-type (type)
  (cdr-type (cdr-type type)))

(defun caaar-type (type)
  (car-type (car-type (car-type type))))

(defun caadr-type (type)
  (car-type (car-type (cdr-type type))))

(defun cadar-type (type)
  (car-type (cdr-type (car-type type))))

(defun caddr-type (type)
  (car-type (cdr-type (cdr-type type))))

(defun cdaar-type (type)
  (cdr-type (car-type (car-type type))))

(defun cdadr-type (type)
  (cdr-type (car-type (cdr-type type))))

(defun cddar-type (type)
  (cdr-type (cdr-type (car-type type))))

(defun cdddr-type (type)
  (cdr-type (cdr-type (cdr-type type))))

(defun cadddr-type (type)
  (car-type (cdr-type (cdr-type (cdr-type type)))))


;;------------------------------------------------------------------------------
(defun cons-type (car-type cdr-type)
  (if (zs-subtypep cdr-type list-t)
      (type-meet list-cons-t
                 (type-join (list-cons-of car-type) 
                            cdr-type))
      cons-t))


;;------------------------------------------------------------------------------
(defun replace-type (cons-type element-type)
  (declare (ignore element-type))
  (type-meet cons-t cons-type))


;;------------------------------------------------------------------------------
(defun member-type (arg1-type list-type &rest other-types)
  (declare (ignore arg1-type other-types))
  (type-meet all-list-t
             (type-join null-t
                        list-type)))

;;------------------------------------------------------------------------------
(defun setf-type  (type1 &rest other-types)
  (declare (ignore other-types))
  type1)


;;------------------------------------------------------------------------------
(defun add-multiplication-op (&rest types)
  (cond ((endp types) 
         fixnum-t)
        ((endp (rest types))
         (type-meet (first types) number-t))
        (T
          (zs-typecase (apply #'multiple-type-join types)
             (bottom-t           bottom-t)
             (byte-t             word-t)
             (word-t             fixnum-t)
             (integer-t          integer-t)
             (float-t            float-t)
             ((conform number-t) number-t)
             (otherwise          bottom-t)))))
       

;;------------------------------------------------------------------------------
(defun inc-dec-op (type)
  (zs-typecase type
    (bottom-t           bottom-t)
    (byte-t             word-t)
    (word-t             fixnum-t)
    (integer-t          integer-t)
    (float-t            float-t)
    ((conform number-t) number-t)
    (otherwise          bottom-t)))
       

;;------------------------------------------------------------------------------
(defun division-op (&rest types)
  (zs-typecase (apply #'multiple-type-join types)
     (bottom-t            bottom-t)
     (float-t             float-t)
     ((conform number-t)  number-t)
     (otherwise           bottom-t)))


;;------------------------------------------------------------------------------
(defun round-fun (number &optional divisor)
  (declare (ignore divisor))
  (type-meet integer-t number))


;;------------------------------------------------------------------------------
(defun numbers-to-number (&rest argument-types)
  (let ((join (apply #'multiple-type-join argument-types)))
    (if (types-are-conform join number-t)
        (type-meet number-t join)
        bottom-t)))

;;------------------------------------------------------------------------------
(defun assert-sequence-t (type)
  (type-meet sequence-t type))


;;------------------------------------------------------------------------------
(defun remove-fun (item-type sequence-type &rest types)
  (declare (ignore item-type types))
  (zs-typecase sequence-type
      (bottom-t             bottom-t)
      (list-t               (type-join null-t sequence-type))
      ((conform sequence-t) (type-join null-t (type-meet sequence-t   
                                                         sequence-type)))
      (otherwise            bottom-t)))


;;------------------------------------------------------------------------------
(defun find-fun (item-type sequence-type &rest types)
  ;; Falls das Element nicht gefunden wird, wird nil geliefert.
  (type-join null-t 
     (if (null types)
         ;; Wenn keine Schluesselworte angegeben werden, dann ist
         ;; das Ergebnis das gesuchte item.
         item-type
         
         ;; Ansonsten wird ein Element der sequence geliefert.
         (zs-typecase sequence-type
                      (bottom-t             bottom-t)
                      (list-t               (list-component sequence-type))
                      ((conform sequence-t) top-t)
                      (otherwise            bottom-t)))))
                                                                  
        
;;------------------------------------------------------------------------------
(defun joined-list-type (type1 type2 &rest types)
  (declare (ignore types))
  (type-meet list-t (type-join type1 type2)))


;;------------------------------------------------------------------------------
(defun abort-function (&rest types)
  (declare (ignore types))
  (set-all-type-bindings-to bottom-t)
  bottom-t)


;;------------------------------------------------------------------------------
;; Initialisiere die Typbeschreibungen der importierten Funktionen. Diese
;; Funktion wird ueberfluessig, wenn die Typbeschreibungen in sys-fun.def
;; oder in den importierten Funktionen zugeordneten Definitionsdateien 
;; festgehalten werden.
;;------------------------------------------------------------------------------
(defun initialize-function-descriptions-part1 ()
  ;; 6.2.2. Specific Data Type Predicates
  ;; ------------------------------------
  (declare-type-predicates ((L::not                null-t)
                            (L::endp               null-t)
                            (L::symbolp            symbol-t)
                            (rt::symp              non-null-sym-t)
                            (L::atom               atom-t)
                            (L::consp              cons-t)
                            (L::listp              all-list-t) 

                            (rt::fixnump           fixnum-t)
                            (L::integerp           integer-t)
                            (L::floatp             float-t)
                            (L::numberp            number-t)

                            (L::characterp         character-t)
                            (L::stringp            string-t)
                            (L::vectorp            vector-t)
                            (L::simple-vector-p    vector-t)
                            (L::simple-string-p    string-t)
                            (L::arrayp             array-t)
                            (L::simple-array-p     array-t)
                            (rt::plain-vector-p    array-t)

                            (L::functionp          function-t)
                            (rt::structp           structure-t)
                            (rt::instancep         class-t)
                            (L::packagep           package-t)
                            (L::streamp            stream-t)
                            (L::hash-table-p       hash-table-t)))
)


;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun initialize-function-descriptions-part2 ()

  ;; 6.3. Equality Predicates
  ;; ------------------------
  (dec-type L::eq     (top-t top-t) -> bool-t)
  (dec-type L::eql    (top-t top-t) -> bool-t)
  (dec-type L::equal  (top-t top-t) -> bool-t)
  (dec-type L::equalp (top-t top-t) -> bool-t)
  

  ;; 7.1.1. Reference
  ;; ----------------
  (dec-type L::symbol-value     (symbol-t) -> top-t)
  (dec-type rt::symbol-value    (symbol-t) -> top-t)
  (dec-type L::boundp           (symbol-t) -> bool-t)
  (dec-type rt::unbound-value-p (top-t)    -> bool-t)

  
  ;; 7.1.2. Assignment
  ;; -----------------
  (dec-type L::set                  (symbol-t top-t) -> top-t #'the-second-type)
  (dec-type (setf rt::symbol-value) (top-t symbol-t) -> top-t #'the-first-type)
  (dec-type L::makunbound           (symbol-t)       -> symbol-t)
  (dec-type rt::makunbound          (symbol-t)       -> symbol-t)
  
  ;;  7.9. Multiple Values
  ;; ---------------------
  (dec-type L::values        () -> top-t   #'(lambda (&rest types) 
                                            (if types (first types) null-t)))

  ;; 10.1. The Property List
  ;; -----------------------
  (dec-type L::get          (symbol-t top-t) -> top-t)
  (dec-type L::remprop      (symbol-t top-t) -> top-t)
  (dec-type L::symbol-plist (symbol-t)       -> top-t)
  (dec-type rt::symbol-plist (symbol-t)      -> list-t)
  (dec-type (setf rt::symbol-plist) (list-t symbol-t) -> list-t 
                                     #'the-first-type)
  
  ;; 10.2. The Print Adr
  ;; --------------------
  (dec-type L::symbol-name  (symbol-t) -> string-t)
  (dec-type rt::symbol-name (symbol-t) -> string-t)
  (dec-type (setf rt::symbol-name) (string-t symbol-t) -> string-t)
  
  ;; 10.3. Creating Symbols
  ;; ----------------------
  (dec-type L::make-symbol    (string-t) -> symbol-t)
  (dec-type rt::make-symbol   ()         -> symbol-t)
  (dec-type L::copy-symbol    (symbol-t) -> symbol-t)
  (dec-type L::gensym         ()         -> symbol-t)
  (dec-type L::gentemp        ()         -> symbol-t)
  (dec-type L::symbol-package (symbol-t) -> (type-join package-t null-t))
  (dec-type L::keywordp       (top-t)    -> bool-t)

  (dec-type rt::symbol-package-index (symbol-t) -> (type-join null-t integer-t))
  (dec-type rt::constant-flag-p (symbol-t) -> bool-t)
  (dec-type rt::set-constant-flag (symbol-t) -> symbol-t)
  
  ;; 11.7. Package System Functions and Variables
  ;; --------------------------------------------
  (dec-type rt::ensure-package      (package-name-t)    -> package-t)
  (dec-type L::make-package         (package-name-t)    -> package-t)
  (dec-type L::in-package           (package-name-t)    -> package-t)
  (dec-type L::find-package         (package-or-name-t) -> 
                                    (type-join null-t package-t))
  (dec-type L::package-name         (package-or-name-t) -> string-t)
  (dec-type L::package-nicknames    (package-or-name-t) -> (list-of string-t))
  (dec-type L::rename-package       (package-or-name-t package-name-t) 
                                    -> package-t)
  (dec-type L::package-use-list     (package-or-name-t)  -> (list-of package-t))
  (dec-type L::package-used-by-list (package-or-name-t)  -> (list-of package-t))
  (dec-type L::list-all-packages    ()                   -> (list-of package-t))
  (dec-type L::package-shadowing-symbols (package-or-name-t) 
                                         -> (list-of symbol-t))

  (dec-type L::intern           (string-t package-or-name-t) -> symbol-t)
  (dec-type L::find-symbol      (string-t package-or-name-t) -> symbol-t)
  (dec-type L::unintern         (symbol-t package-or-name-t) -> bool-t)

  (dec-type L::export           ((list-of symbol-t) package-or-name-t) 
                                -> t-symbol-t)
  (dec-type L::unexport         ((list-of symbol-t) package-or-name-t) 
                                -> t-symbol-t)
  (dec-type L::import           ((list-of symbol-t) package-or-name-t) 
                                -> t-symbol-t)
  (dec-type L::shadowing-import ((list-of symbol-t) package-or-name-t) 
                                -> t-symbol-t)
  (dec-type L::shadow           ((list-of symbol-t) package-or-name-t) 
                                -> t-symbol-t)
  (dec-type L::use-package      ((type-join (list-of package-or-name-t) 
                                            package-or-name-t)
                                 package-or-name-t)
                                -> t-symbol-t)
  (dec-type L::unuse-package    ((type-join (list-of package-or-name-t) 
                                            package-or-name-t)
                                 package-or-name-t)
                                -> t-symbol-t)
  (dec-type L::find-all-symbols () -> (list-of symbol-t))


  ;; 12.2. Predicates on Numbers
  ;; ---------------------------
  (dec-type L::zerop  (number-t)  -> bool-t)
  (dec-type L::plusp  (number-t)  -> bool-t)
  (dec-type L::minusp (number-t)  -> bool-t)
  (dec-type L::oddp   (integer-t) -> bool-t)
  (dec-type L::evenp  (integer-t) -> bool-t)
  
  
  ;; 12.3. Comparisons on Numbers
  ;; ----------------------------
  (dec-type L::=    (number-t number-t) -> bool-t)
  (dec-type L::/=   (number-t number-t) -> bool-t)
  (dec-type L::<    (number-t number-t) -> bool-t)
  (dec-type rt::<fix (fixnum-t fixnum-t) -> top-t)
  (dec-type L::>    (number-t number-t) -> bool-t)
  (dec-type L::<=   (number-t number-t) -> bool-t)
  (dec-type L::>=   (number-t number-t) -> bool-t)
  (dec-type L::min  (number-t number-t) -> number-t  #'numbers-to-number)
  (dec-type L::max  (number-t number-t) -> number-t  #'numbers-to-number)

  ;; 12.4. Arithmetic Operations
  ;; ---------------------------
  (dec-type L::+      (number-t number-t) -> number-t  #'add-multiplication-op)
  (dec-type L::-      (number-t number-t) -> number-t  #'add-multiplication-op)
  (dec-type L::*      (number-t number-t) -> number-t  #'add-multiplication-op)
  (dec-type L::1+     (number-t)          -> number-t  #'inc-dec-op)
  (dec-type L::1-     (number-t)          -> number-t  #'inc-dec-op)
  (dec-type rt::1-fix (fixnum-t)          -> fixnum-t)
  (dec-type L::/      (number-t number-t) -> number-t  #'division-op)
  (dec-type L::abs    (number-t)          -> number-t)

  (dec-type rt::log               (number-t number-t)   -> number-t)
  (dec-type rt::float             (number-t)            -> float-t)
  (dec-type rt::floor             (number-t number-t)   -> number-t)
  (dec-type rt::ceiling           (number-t number-t)   -> number-t)
  (dec-type rt::truncate          (number-t number-t)   -> number-t)
  (dec-type rt::round             (number-t number-t)   -> number-t)
  
  (dec-type rt::%logior           (fixnum-t fixnum-t)   -> fixnum-t)
  (dec-type rt::%logxor           (fixnum-t fixnum-t)   -> fixnum-t)
  (dec-type rt::%logand           (fixnum-t fixnum-t)   -> fixnum-t)
  (dec-type rt::%lognot           (fixnum-t)            -> fixnum-t)
  (dec-type rt::%shift-right      (integer-t fixnum-t)  -> integer-t)

  (dec-type L::ash                (integer-t fixnum-t)  -> integer-t)
  (dec-type L::logxor             (integer-t integer-t) -> integer-t)
  (dec-type L::logior             (integer-t integer-t) -> integer-t)
  (dec-type L::logand             (integer-t integer-t) -> integer-t)
  (dec-type L::logandc1           (integer-t integer-t) -> integer-t)
  (dec-type L::logandc2           (integer-t integer-t) -> integer-t)
  (dec-type L::lognand            (integer-t integer-t) -> integer-t)
  (dec-type L::logorc1            (integer-t integer-t) -> integer-t)
  (dec-type L::logorc2            (integer-t integer-t) -> integer-t)
  (dec-type L::lognot             (integer-t)           -> integer-t)


  ;; 12.6. Type Conversions and Component Extractions on Numbers
  ;; -----------------------------------------------------------
  (dec-type L::float    (number-t float-t)  -> float-t)
  (dec-type L::floor    (number-t number-t) -> number-t  #'round-fun)
  (dec-type L::ceiling  (number-t number-t) -> number-t  #'round-fun)
  (dec-type L::truncate (number-t number-t) -> number-t  #'round-fun)
  (dec-type L::round    (number-t number-t) -> number-t  #'round-fun)


  ;; 13.2. Predicates on Characters
  ;; ------------------------------
  (dec-type L::standard-char-p    (character-t) -> bool-t)
  (dec-type L::graphic-char-p     (character-t) -> bool-t)
  (dec-type L::alpha-char-p       (character-t) -> bool-t)
  (dec-type L::upper-case-p       (character-t) -> bool-t)
  (dec-type L::lower-case-p       (character-t) -> bool-t)
  (dec-type L::both-case-p        (character-t) -> bool-t)
  (dec-type L::digit-char-p       (character-t) -> (type-join null-t fixnum-t))
  (dec-type L::alphanumericp      (character-t) -> bool-t)
                 
  (dec-type L::char=              (character-t character-t) -> bool-t)
  (dec-type L::char/=             (character-t character-t) -> bool-t)
  (dec-type L::char<              (character-t character-t) -> bool-t)
  (dec-type L::char>              (character-t character-t) -> bool-t)
  (dec-type L::char<=             (character-t character-t) -> bool-t)
  (dec-type L::char>=             (character-t character-t) -> bool-t)
  (dec-type L::char-equal         (character-t character-t) -> bool-t)
  (dec-type L::char-not-equal     (character-t character-t) -> bool-t)
  (dec-type L::char-lessp         (character-t character-t) -> bool-t)
  (dec-type L::char-greaterp      (character-t character-t) -> bool-t)
  (dec-type L::char-not-greaterp  (character-t character-t) -> bool-t)
  (dec-type L::char-not-lessp     (character-t character-t) -> bool-t)

  (dec-type rt::char=             (character-t character-t) -> bool-t)
  (dec-type rt::char/=            (character-t character-t) -> bool-t)
  (dec-type rt::char<             (character-t character-t) -> bool-t)
  (dec-type rt::char>             (character-t character-t) -> bool-t)
  (dec-type rt::char<=            (character-t character-t) -> bool-t)
  (dec-type rt::char>=            (character-t character-t) -> bool-t)
  (dec-type rt::char-equal        (character-t character-t) -> bool-t)
  (dec-type rt::char-not-equal    (character-t character-t) -> bool-t)
  (dec-type rt::char-lessp        (character-t character-t) -> bool-t)
  (dec-type rt::char-greaterp     (character-t character-t) -> bool-t)
  (dec-type rt::char-not-greaterp (character-t character-t) -> bool-t)
  (dec-type rt::char-not-lessp    (character-t character-t) -> bool-t)

  ;; 13.3. Character Construction and Selection
  ;; ------------------------------------------
  (dec-type L::char-code          (character-t) -> fixnum-t)
  (dec-type L::code-char          (fixnum-t)    -> or-null-character-t)
; (dec-type L::character          (top-t)       -> character-t)
  (dec-type L::char-upcase        (character-t) -> character-t)
  (dec-type L::char-downcase      (character-t) -> character-t)
  (dec-type L::digit-char         (integer-t)   -> or-null-character-t)
  (dec-type L::char-name          (character-t) -> (type-join null-t string-t))
  (dec-type L::name-char          (top-t) -> top-t
                                  #'(lambda (type) (type-join null-t type)))

  (dec-type rt::char-code         (character-t) -> fixnum-t)
  (dec-type rt::code-char         (fixnum-t)    -> or-null-character-t)
  (dec-type rt::char-upcase       (character-t) -> character-t)
  (dec-type rt::char-downcase     (character-t) -> character-t)

  
  ;; 14. Sequences
  ;; -------------



  ;; 14.1. Simple Sequence Functions
  ;; -------------------------------
  (dec-type L::elt        (sequence-t integer-t) -> top-t)
  (dec-type L::subseq     (sequence-t integer-t integer-t) -> top-t
                          #'(lambda (sequence-type &rest other-types)
                              (declare (ignore other-types))
                              (if (zs-subtypep sequence-type all-list-t) 
                                  (type-meet sequence-type list-t)
                                  (type-meet sequence-type sequence-t))))
  (dec-type L::copy-seq   (sequence-t) -> sequence-t  #'assert-sequence-t)
  (dec-type L::reverse    (sequence-t) -> sequence-t  #'assert-sequence-t)
  (dec-type L::nreverse   (sequence-t) -> sequence-t  #'assert-sequence-t)
  (dec-type L::length     (sequence-t) -> integer-t)


  ;; 14.2. Concatenating, Mapping, and Reducing Sequences
  ;; ----------------------------------------------------
  (dec-type L::concatenate (top-t sequence-t sequence-t) -> sequence-t)
  (dec-type L::map         (top-t function-t sequence-t) -> sequence-t)
  (dec-type L::some        (function-t sequence-t) -> top-t)
  (dec-type L::every       (function-t sequence-t) -> top-t)
  (dec-type L::notany      (function-t sequence-t) -> top-t)
  (dec-type L::notevery    (function-t sequence-t) -> top-t)
  (dec-type L::reduce      (function-t sequence-t) -> top-t)

  ;; Die Funktionen mapcar, maplist, mapc und mapl sind speziell getypt. 
  ;; Die zugehörigen Typabstraktionsfunktionen liegen tipass2.lisp.



  ;; 14.3. Modifying Sequences
  ;; -------------------------
  ;;(fill        )
  ;;(replace     )

  (dec-type L::remove        (top-t      sequence-t) -> sequence-t #'remove-fun)
  (dec-type L::remove-if     (function-t sequence-t) -> sequence-t #'remove-fun)
  (dec-type L::remove-if-not (function-t sequence-t) -> sequence-t #'remove-fun)

  (dec-type L::delete        (top-t      sequence-t) -> sequence-t #'remove-fun)
  (dec-type L::delete-if     (function-t sequence-t) -> sequence-t #'remove-fun)
  (dec-type L::delete-if-not (function-t sequence-t) -> sequence-t #'remove-fun)


  (dec-type L::remove-duplicates (sequence-t) -> sequence-t)
  (dec-type L::delete-duplicates (sequence-t) -> sequence-t)
  (dec-type L::substitute        (top-t top-t sequence-t) -> sequence-t
                                 #'(lambda (type1 type2 sequence &rest types)
                                  (declare (ignore type1 type2 types))
                                  (type-meet sequence sequence-t)))
  ;; .....

  ;; 14.4. Searching Sequences for Items
  ;; -----------------------------------
  (dec-type L::find        (top-t      sequence-t) -> top-t  #'find-fun)
  (dec-type L::find-if     (function-t sequence-t) -> top-t  #'find-fun)
  (dec-type L::find-if-not (function-t sequence-t) -> top-t  #'find-fun)

  (dec-type L::position        (top-t      sequence-t) -> 
                               (type-join null-t integer-t))
  (dec-type L::position-if     (function-t sequence-t) -> 
                               (type-join null-t integer-t))
  (dec-type L::position-if-not (function-t sequence-t) -> 
                               (type-join null-t integer-t))
  
  (dec-type L::count           (top-t      sequence-t) -> integer-t)
  (dec-type L::count-if        (function-t sequence-t) -> integer-t)
  (dec-type L::count-if-not    (function-t sequence-t) -> integer-t)


  ;; 14.5. Sorting and Merging
  ;; -------------------------
  (dec-type L::sort            (sequence-t function-t) -> sequence-t
                               #'(lambda (sequence-type &rest other-types)
                                   (declare (ignore other-types))
                                   (type-meet sequence-t sequence-type)))



) ; initialize-function-descriptions-part2


;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun initialize-function-descriptions-part3 ()
  ;;----------------------------------------------------------------------------
  
  ;; 15.1. Conses
  ;; ------------
  (dec-type rt::%car    (cons-t)       -> top-t   #'%car-type)
  (dec-type rt::%cdr    (cons-t)       -> top-t   #'cdr-type)
  (dec-type rt::%rplaca (cons-t top-t) -> cons-t  #'replace-type)
  (dec-type rt::%rplacd (cons-t top-t) -> cons-t  #'replace-type)
  (dec-type L::rplaca   (cons-t top-t) -> cons-t  #'replace-type)
  (dec-type L::rplacd   (cons-t top-t) -> cons-t  #'replace-type)
  
  (dec-type L::car      (all-list-t) -> top-t   #'car-type)
  (dec-type L::cdr      (all-list-t) -> top-t   #'cdr-type)
  (dec-type L::caar     (all-list-t) -> top-t   #'caar-type)
  (dec-type L::cadr     (all-list-t) -> top-t   #'cadr-type)
  (dec-type L::cdar     (all-list-t) -> top-t   #'cdar-type)
  (dec-type L::cddr     (all-list-t) -> top-t   #'cddr-type)
  (dec-type L::caaar    (all-list-t) -> top-t   #'caaar-type)
  (dec-type L::caadr    (all-list-t) -> top-t   #'caadr-type)
  (dec-type L::cadar    (all-list-t) -> top-t   #'cadar-type)
  (dec-type L::caddr    (all-list-t) -> top-t   #'caddr-type)
  (dec-type L::cdaar    (all-list-t) -> top-t   #'cdaar-type)
  (dec-type L::cdadr    (all-list-t) -> top-t   #'cdadr-type)
  (dec-type L::cddar    (all-list-t) -> top-t   #'cddar-type)
  (dec-type L::cdddr    (all-list-t) -> top-t   #'cdddr-type)
  (dec-type L::cadddr   (all-list-t) -> top-t   #'cadddr-type)
  (dec-type L::first    (all-list-t) -> top-t   #'car-type)
  (dec-type L::second   (all-list-t) -> top-t   #'cadr-type)
  (dec-type L::third    (all-list-t) -> top-t   #'caddr-type)
  (dec-type L::fourth   (all-list-t) -> top-t   #'cadddr-type)
  (dec-type L::rest     (all-list-t) -> top-t   #'cdr-type)

  (dec-type L::cons        (top-t top-t)   -> cons-t #'cons-type)
  (dec-type L::tree-equal  (list-t list-t) -> bool-t)

  (dec-type (setf L::car)    (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::cdr)    (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::caar)   (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::cadr)   (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::cdar)   (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::cddr)   (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::caddr)  (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::first)  (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::second) (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::third)  (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::fourth) (top-t cons-t) -> cons-t  #'setf-type)
  (dec-type (setf L::rest)   (top-t cons-t) -> cons-t  #'setf-type)


  ;; 15.2. Lists
  ;; -----------
  (dec-type L::list-length (list-t)               -> integer-t)
  (dec-type L::nth         (integer-t all-list-t) -> top-t
                           #'(lambda (integer-type cons-type)
                               (declare (ignore integer-type))
                               (car-type cons-type)))
  (dec-type L::nthcdr      (integer-t all-list-t) -> all-list-t
                           #'(lambda (integer-type cons-type)
                               (declare (ignore integer-type))
                               (cdr-type cons-type)))
  (dec-type L::last      (all-list-t integer-t) -> all-list-t  #'the-first-type)
  (dec-type L::list      () -> list-t  #'list-cons-of)
  (dec-type L::list*     () -> list-t  #'list-cons-of)
  (dec-type L::append    (all-list-t all-list-t) -> all-list-t
                         #'(lambda (&rest types)
                             (type-meet list-t 
                                        (apply #'multiple-type-join types))))
  (dec-type L::copy-list   (all-list-t) -> all-list-t  #'the-first-type)
  (dec-type L::copy-alist  (all-list-t) -> all-list-t  #'the-first-type)
  (dec-type L::assoc       (top-t all-list-t) -> all-list-t)
  (dec-type L::butlast     (all-list-t integer-t) -> all-list-t
                           #'(lambda (list-type &optional integer-type) 
                               (declare (ignore integer-type))
                               (type-meet list-t (type-join null-t 
                                                            list-type))))
  (dec-type L::ldiff       (all-list-t all-list-t) -> all-list-t
                           #'(lambda (type1 type2)
                               (type-meet list-t (type-meet type1 type2))))

  ;; 15.5. Using Lists as Sets
  ;; -------------------------
  (dec-type L::member         (top-t      all-list-t) -> top-t
                              #'member-type)
  (dec-type L::member-if      (function-t all-list-t) -> top-t
                              #'member-type)
  (dec-type L::member-if-not  (function-t all-list-t) -> top-t
                              #'member-type)
  (dec-type L::tailp          (all-list-t all-list-t) -> bool-t)
  (dec-type L::adjoin         (top-t all-list-t) -> cons-t
                              #'(lambda (new list &rest other-types)
                                  (declare (ignore other-types))
                                  (type-meet list-cons-t
                                             (type-join list 
                                                        (list-cons-of new)))))
  (dec-type L::union          (all-list-t all-list-t) -> list-t
                              #'joined-list-type)
  (dec-type L::intersection   (all-list-t all-list-t) -> list-t
                              #'(lambda (t1 t2 &rest other-types)
                                  (declare (ignore other-types))
                                  (type-meet list-t (type-meet t1 t2))))
  (dec-type L::set-difference (all-list-t all-list-t) -> list-t
                              #'(lambda (t1 t2 &rest other-types)
                                  (declare (ignore t2 other-types))
                                  (type-meet list-t t1)))
  (dec-type L::subsetp        (all-list-t all-list-t) -> bool-t)

) ; initialize-function-descriptions-part3



;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun initialize-function-descriptions-part4 ()

  ;; 18.2. String Comparison
  ;; -----------------------
  (dec-type L::string=             (or-sym-str-char-t or-sym-str-char-t) 
                                   -> bool-t)
  (dec-type L::string-equal        (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string<             (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string>             (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string<=            (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string>=            (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string/=            (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string-lessp        (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string-greaterp     (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string-not-greaterp (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string-not-lessp    (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)
  (dec-type L::string-not-equal    (or-sym-str-char-t or-sym-str-char-t)
                                   -> bool-t)

  ;; 18.3. String Construction and Manipulation
  ;; ------------------------------------------
  (dec-type L::make-string        (integer-t)                    -> string-t)
  (dec-type L::string-trim        (sequence-t or-sym-str-char-t) -> string-t)
  (dec-type L::string-left-trim   (sequence-t or-sym-str-char-t) -> string-t)
  (dec-type L::string-right-trim  (sequence-t or-sym-str-char-t) -> string-t)
  (dec-type L::string-upcase      (or-sym-str-char-t)            -> string-t) 
  (dec-type L::string-downcase    (or-sym-str-char-t)            -> string-t)
  (dec-type L::string-capitalize  (or-sym-str-char-t)            -> string-t)
  (dec-type L::nstring-upcase     (or-symbol-string-t)           -> string-t)
  (dec-type L::nstring-downcase   (or-symbol-string-t)           -> string-t)
  (dec-type L::nstring-capitalize (or-symbol-string-t)           -> string-t)
  (dec-type L::string             (or-sym-str-char-t)            -> string-t)

  ;; 22.3.1 Output to Character Streams
  ;;-----------------------------------
  (dec-type L::write  (top-t)             -> top-t    #'the-first-type)
  (dec-type L::prin1  (top-t my-stream-t) -> top-t    #'the-first-type)  
  (dec-type L::print  (top-t my-stream-t) -> top-t    #'the-first-type)  
  (dec-type L::pprint (top-t my-stream-t) -> top-t    #'the-first-type)  
  (dec-type L::princ  (top-t my-stream-t) -> top-t    #'the-first-type)  
  
  (dec-type L::write-to-string (top-t) -> string-t)
  (dec-type L::prin1-to-string (top-t) -> string-t)
  (dec-type L::princ-to-string (top-t) -> string-t)

  (dec-type L::write-char   (character-t my-stream-t) -> character-t)
  (dec-type L::write-string (string-t my-stream-t)    -> string-t)
  (dec-type L::terpri       (my-stream-t)             -> null-t)
  (dec-type L::fresh-line   (my-stream-t)             -> bool-t)

  ;;----------------------------------------------------------------------------
  ;; Other
  ;;----------------------------------------------------------------------------
  (dec-type L::read       (my-stream-t) -> top-t)
  (dec-type L::read-line  (my-stream-t) -> top-t)
  (dec-type L::read-char  (my-stream-t) -> top-t)

  (dec-type L::format     (my-stream-t string-t) -> or-symbol-string-t 
            #'(lambda (stream-type &rest types)
                (declare (ignore types)) 
                (zs-typecase stream-type
                   (bottom-t   bottom-t)
                   (null-t     string-t)
                   (t-symbol-t null-t)
                   (stream-t   null-t)
                   ((conform my-stream-t) (type-join string-t null-t))
                   (otherwise  bottom-t))))

  (dec-type L::error           (string-t) -> bottom-t #'abort-function)
  (dec-type rt::the-type-error ()         -> bottom-t #'abort-function)


  (dec-type rt::make-stream    () -> stream-t)


  
  ;;----------------------------------------------------------------------------
  ;; Strukturen
  ;;----------------------------------------------------------------------------
  (dec-type rt::structp          (top-t)             -> bool-t)
  (dec-type rt::struct-typep     (top-t    symbol-t) -> bool-t)
  (dec-type rt::new-struct       (fixnum-t)          -> structure-t)
  (dec-type rt::make-struct      (symbol-t)          -> structure-t)
  (dec-type rt::struct-type      (structure-t)       -> symbol-t)
  (dec-type rt::struct-size      (structure-t)       -> fixnum-t)
  (dec-type rt::struct-ref       (structure-t fixnum-t symbol-t) -> top-t)
  (dec-type (setf rt::struct-ref) (top-t structure-t fixnum-t symbol-t) -> top-t
                                   #'the-first-type)
  (dec-type rt::structure-ref    (structure-t fixnum-t) -> top-t)
  (dec-type (setf rt::structure-ref) (top-t structure-t fixnum-t) -> top-t
                                      #'the-first-type)

  ;;----------------------------------------------------------------------------
  ;; Arrays 
  ;;----------------------------------------------------------------------------
  (dec-type rt::make-vector-t      (integer-t) -> vector-t)
  (dec-type rt::make-vector-fixnum (integer-t) -> vector-t)
  (dec-type rt::make-vector-float  (integer-t) -> vector-t)
  (dec-type rt::make-vector-char   (integer-t character-t) -> vector-t)
  (dec-type rt::make-vector-bit    (integer-t) -> vector-t)

  (dec-type rt::row-major-aref-internal     (array-t integer-t)       -> top-t)
  (dec-type rt::set-row-major-aref-internal (top-t array-t integer-t) -> top-t
                                            #'the-first-type)
  (dec-type rt::svref-internal              (vector-t integer-t)       -> top-t)
  (dec-type rt::set-svref-internal          (top-t vector-t integer-t) -> top-t
                                            #'the-first-type)
  (dec-type rt::pvref                       (vector-t integer-t)       -> top-t)
  (dec-type rt::set-pvref                   (top-t vector-t integer-t) -> top-t
                                            #'the-first-type)
  (dec-type rt::plain-vector-length         (vector-t)           -> integer-t)
  (dec-type rt::plain-vector-element-code   (vector-t)           -> fixnum-t)
  (dec-type rt::check-array                 ()                   -> bool-t)
  (dec-type L::make-array       ((type-join integer-t (list-of integer-t))) -> 
                                array-t)
  (dec-type L::array-dimensions (array-t) -> (list-of integer-t))
  (dec-type L::array-rank       (array-t) -> integer-t)
  (dec-type L::array-total-size (array-t) -> integer-t)
  


  (dec-type L::char             (string-t integer-t) -> character-t)
  (dec-type L::schar            (string-t integer-t) -> character-t)

  (dec-type L::make-hash-table  () -> hash-table-t)
  (dec-type L::maphash          (function-t hash-table-t) -> null-t)
  (dec-type L::string-hash      (string-t integer-t) -> integer-t)
  (dec-type L::sxhash           (top-t) -> integer-t)
  (dec-type rt::sxhash-string   (string-t) -> integer-t)
  (dec-type rt::combine-hash    (integer-t integer-t) -> integer-t)

  (dec-type L::y-or-n-p         (string-t) -> bool-t)
  (dec-type L::yes-or-no-p      (string-t) -> bool-t)

  ;;----------------------------------------------------------------------------
  ;; 23.3 Renaming, deleting, and other file operations
  ;;----------------------------------------------------------------------------
  (dec-type L::delete-file      (file-t)   -> top-t)
  (dec-type L::probe-file       (file-t)   -> (type-join null-t pathname-t))
  (dec-type L::open             (file-t)   -> (type-join null-t stream-t))
  (dec-type L::close            (stream-t) -> top-t)


  ;; 28. Common Lisp Object System (clos.c)
  ;;--------------------------------------
  (dec-type rt::make-instance      (fixnum-t) -> class-t)
  (dec-type rt::instance-ref       (class-t fixnum-t)       -> top-t)
  (dec-type rt::instance-set       (top-t class-t fixnum-t) -> top-t
                                    #'the-first-type)
  (dec-type rt::set-slot-unbound   (fixnum-t class-t) -> fixnum-t)

  ;; (clos.lisp)
  ;;------------
  (dec-type L::make-instance        (class-t)                      -> class-t)
  (dec-type rt::typep-class         (top-t class-t)                -> bool-t)
  (dec-type L::slot-value           (class-t other-symbol-t)       -> top-t)
  (dec-type (setf L::slot-value)    (top-t class-t other-symbol-t) -> top-t
                                    #'the-first-type)
  (dec-type L::slot-boundp          (class-t other-symbol-t)       -> bool-t)
  (dec-type L::slot-makunbound      (class-t other-symbol-t)       -> top-t)
  (dec-type L::class-of             (class-t)                      -> class-t)
  (dec-type L::no-next-method       () -> bottom-t  #'abort-function)
  (dec-type L::no-applicable-method () -> bottom-t  #'abort-function)
  (dec-type L::slot-missing         (class-t class-t other-symbol-t) -> bottom-t
                                    #'abort-function)
  (dec-type L::slot-unbound         (class-t class-t top-t)          -> bottom-t
                                    #'abort-function)


  ;;----------------------------------------------------------------------------
  ;; Funktionen zum Erzeugen von Lisp-Werten aus C-Werten
  ;;----------------------------------------------------------------------------
  ;; (dec-type <fun> (<arg-types>) -> <ret-type>)
  




) ; initialize-function-descriptions-part4


;;------------------------------------------------------------------------------
(provide "tiimpdec")

