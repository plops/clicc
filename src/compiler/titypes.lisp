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
;;; Funktion : Repraesentation des Typverbandes, Implementation der 
;;;            Typverbandsoperatoren und Deklaration der Elemente
;;;            des Typverbandes
;;;
;;; $Revision: 1.54 $
;;; $Id: titypes.lisp,v 1.54 1994/11/25 13:36:15 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Repraesentation des Typverbandes:
;;
;; Der in der APPLY-Typinferenz verwendete Typverband ist wie folgt aufgebaut:
;; Es gibt eine Menge P von primitiven Typen {p1, ..., pn}. Die Elemente der
;; Potenzmenge von P, das sind Mengen von primitiven Typen, heissen atomare
;; Typen.
;; Durch Anwendung von Typkonstruktoren auf die atomare Typen entsteht die
;; Menge Z der zusammengesetzten Typen. Die Menge der APPLY-Typen ist die 
;; Potenzmenge der Vereinigung der Mengen von primitiven und zusammengesetzten
;; Typen, also die Potenzmenge von (P U Z).
;; Die APPLY-Typen sind also Mengen von primitiven und zusammengesetzten Typen.
;; Eine solche Menge t1 = {p1, ..., pk, z1, ..., zl} hat als Bedeutung, dass 
;; ein Datum mit Typ t1 entweder Typ p1, ..., pk, z1, ..., oder Typ zl hat.
;;
;; Der einzige zur Zeit verwendete Typkonstruktor ist `list-cons-of'.
;;
;; Zur einfachen Handhabung sind alle primitiven, sowie einige atomare und
;; zusammengesetzte Typen benannt. Benannt sind in der Regel Typentsprechungen
;; zu Common Lisp-Typen.
;; Die in der APPLY-Typinferenz verwendeten Typen mit Typbenennung haben zur
;; Unterscheidung von Common Lisp-Typen alle einen Typnamen mit der Endung "-t".
;; So hat z. B. der alle Zahlen umfassende APPLY-Typ den Namen "number-t".
;;
;; Die APPLY-Typen sind Mengen von primitiven und zusammengesetzten Typen.
;; Diese Mengen werden durch kurze Bit-Vektoren (integers) repraesentiert.
;; 
;; Das Bottom-Element des Typverbandes ist als 0 repraesentiert. In der
;; Repraesentationen des Top-Elementes sind alle Bits gesetzt.
;;
;; Die benannten Typen werden beim Laden dieser Datei in der unten angegebenen
;; Art und Weise und Reihenfolge `deklariert'. Zu jedem deklarierten Typ gibt
;; es eine Konstante, die die Typrepraesentation des zugehoerigen Typs enthaelt.
;;
;; Beispiel:
;; Die Konstante mit Namen "null-t" repraesentiert den Typ `null' und
;; hat den Wert 1. Die Konstante mit Namen "t-t" repraesentiert den Typ zum 
;; Element `T' und hat den Wert 2. Die Konstante mit Namen "other-symbol-t" 
;; repraesentiert den Typ aller von `NIL' und `T' verschiedener Symbole und 
;; hat den Wert 4.
;; Der Vereinigungstyp (or null-t t-t other-symbol-t) ist mit "symbol-t"
;; benannt. Er ist die Typentsprechung zum Common Lisp-Typ `symbol'. 
;; Die Konstante mit Namen "symbol-t" hat den Wert 7.
;;------------------------------------------------------------------------------


(defstruct (ti-type (:print-function (lambda (type stream depth)
                                       (declare (ignore depth))
                                       (format stream "~S" 
                                               (output-type type)))))
  (bt 0 :type fixnum)
  (lt 0 :type fixnum))

;;------------------------------------------------------------------------------
;; Inline-Deklarationen zu den Typverbandsoperationen:
;;------------------------------------------------------------------------------
(proclaim
 '(inline 
   type-join multiple-type-join type-meet zs-subtypep 
   type-eq is-bottom-t types-are-conform
   list-cons-of list-of list-component
   simple-type-p))


;;------------------------------------------------------------------------------
;; Deklaration der Elemente des Typverbands:
;;
;; `Typdeklaration' des Typs t1 mit Namen n1 bedeutet hier, dass eine Konstante
;; mit Namen n1 definiert wird, deren Wert die Typrepraesentation von t1 ist.
;;
;; Zuerst werden der Bottom-Typ und die primitiven Typen deklariert. Dann 
;; werden einige atomare Typen benannt. Zuletzt wird zu einigen primitiven und 
;; atomaren Typen der entsprechende NOT-Typ deklariert.
;;------------------------------------------------------------------------------


;;------------------------------------------------------------------------------
;; Makro zum Deklarieren eines im Typverband der Typinferenz verwendeten Typs.
;; Hier könnte defconstant stehen, dann müßte man jedoch eine Methode für
;; make-load-form schreiben und das ist in vielen Lisps noch nicht (richtig)
;; definiert
;;------------------------------------------------------------------------------
(defmacro declare-zs-type (type-symbol type)
  `(defparameter ,type-symbol ,type))


;;------------------------------------------------------------------------------
;; Deklaration der Typen:
;;------------------------------------------------------------------------------
(declare-zs-type bottom-t              (make-ti-type :bt 0))

(declare-zs-type null-t                (make-ti-type :bt (ash 2 -1)))
(declare-zs-type t-symbol-t            (make-ti-type :bt (ash 2 0)))
(declare-zs-type other-symbol-t        (make-ti-type :bt (ash 2 1)))

(declare-zs-type list-cons-of-bottom-t (make-ti-type :bt (ash 2 2)))
(declare-zs-type non-list-cons-t       (make-ti-type :bt (ash 2 3)))

(declare-zs-type byte-t                (make-ti-type :bt (ash 2 4)))
(declare-zs-type non-byte-word-t       (make-ti-type :bt (ash 2 5))) 
(declare-zs-type non-word-fixnum-t     (make-ti-type :bt (ash 2 6))) 
(declare-zs-type bignum-t              (make-ti-type :bt (ash 2 7))) 
(declare-zs-type float-t               (make-ti-type :bt (ash 2 8)))

(declare-zs-type character-t           (make-ti-type :bt (ash 2 9)))

(declare-zs-type string-t              (make-ti-type :bt (ash 2 10)))
(declare-zs-type non-string-vector-t   (make-ti-type :bt (ash 2 11)))
(declare-zs-type non-vector-array-t    (make-ti-type :bt (ash 2 12)))

(declare-zs-type function-t            (make-ti-type :bt (ash 2 13)))
(declare-zs-type structure-t           (make-ti-type :bt (ash 2 14)))
(declare-zs-type class-t               (make-ti-type :bt (ash 2 15)))

(declare-zs-type package-t             (make-ti-type :bt (ash 2 16)))
(declare-zs-type stream-t              (make-ti-type :bt (ash 2 17)))
(declare-zs-type hash-table-t          (make-ti-type :bt (ash 2 18)))
(declare-zs-type readtable-t           (make-ti-type :bt (ash 2 19)))
(declare-zs-type pathname-t            (make-ti-type :bt (ash 2 20)))
(declare-zs-type random-state-t        (make-ti-type :bt (ash 2 21)))

;;------------------------------------------------------------------------------
(defconstant     bits-used             23)
(defconstant     top-number            (1- (ash 2 (1- bits-used))))

;;------------------------------------------------------------------------------
(declare-zs-type  list-component-top-t  (make-ti-type :lt top-number))
(declare-zs-type  list-cons-t           (make-ti-type 
                                         :bt (ti-type-bt list-cons-of-bottom-t)
                                         :lt top-number))



;;------------------------------------------------------------------------------
;; Implementation der Typverbandsoperationen:
;;
;; Die Verbandsoperationen Vereinigung, Schnitt, die Untertyprelation und die
;; Typgleichheit sind durch die entsprechenden Bit-Operation implementiert.
;;------------------------------------------------------------------------------


;;------------------------------------------------------------------------------
;; Liefert die kleinste obere Schranke der uebergebenen Typen.
;;------------------------------------------------------------------------------
(defun type-join (type1 type2)
  (make-ti-type :bt (logior (ti-type-bt type1)
                            (ti-type-bt type2))
                :lt (logior (ti-type-lt type1)
                            (ti-type-lt type2))))


(defun multiple-type-join (&rest types)
  (let ((result bottom-t))
    (dolist (type types result)
      (setf result (type-join result type)))))


;;------------------------------------------------------------------------------
;; Liefert die groesste untere Schranke der uebergebenen Typen.
;;------------------------------------------------------------------------------
(defun type-meet (type1 type2)
  (make-ti-type :bt (logand (ti-type-bt type1)
                            (ti-type-bt type2))
                :lt (logand (ti-type-lt type1)
                            (ti-type-lt type2))))

;;------------------------------------------------------------------------------
;; Untertyprelation im Typverband:
;;------------------------------------------------------------------------------
(defun zs-subtypep (type1 type2)
  (and (zerop (logandc2 (ti-type-bt type1)
                        (ti-type-bt type2)))
       (zerop (logandc2 (ti-type-lt type1)
                        (ti-type-lt type2)))))


;;------------------------------------------------------------------------------
;; Test auf Typgleichheit zweier Typen:
;;------------------------------------------------------------------------------
(defun type-eq (type1 type2)
  (and (eql (ti-type-bt type1)
            (ti-type-bt type2))
       (eql (ti-type-lt type1)
            (ti-type-lt type2))))


;;------------------------------------------------------------------------------
;; Testet, ob `type' das Bottom-Element des Typverbands ist.
;;------------------------------------------------------------------------------
(defun is-bottom-t (type)
  (and (zerop (ti-type-bt type))
       (zerop (ti-type-lt type))))


;;------------------------------------------------------------------------------
;; Prueft, ob zwei Typen disjunkt sind.
;;------------------------------------------------------------------------------
(defun types-are-disjoined (type1 type2)
  (is-bottom-t (type-meet type1 type2)))


;;------------------------------------------------------------------------------
;; Liefert, ob zwei Typen konform sind, d. h. ob der eine Typ Untertyp des 
;; anderen sein kann.
;;------------------------------------------------------------------------------
(defun types-are-conform (type1 type2)
  (not (types-are-disjoined type1 type2)))


;;------------------------------------------------------------------------------
;; Makros zur Deklaration einiger benannter Typen:
;;------------------------------------------------------------------------------
(defmacro declare-joined-type (join-symbol &rest types)
  `(declare-zs-type ,join-symbol (multiple-type-join ,@types)))


;;------------------------------------------------------------------------------
;; Makros zur Deklaration der NOT-Typen:
;;------------------------------------------------------------------------------
(defmacro declare-not-type (type)
  `(declare-zs-type ,(intern-prefixed "NOT-" type) (not-type ,type)))


(defmacro declare-not-types (types)
  `(progn . ,(mapcar #'(lambda (type) `(declare-not-type ,type)) types)))



;;------------------------------------------------------------------------------
;; Deklaration einiger benannter Vereinigungstypen:
;;------------------------------------------------------------------------------
(declare-joined-type word-t         byte-t       non-byte-word-t)
(declare-joined-type fixnum-t       word-t       non-word-fixnum-t)
(declare-joined-type integer-t      fixnum-t     bignum-t)
(declare-joined-type number-t       integer-t    float-t)

(declare-joined-type bool-t         null-t       t-symbol-t)
(declare-joined-type symbol-t       bool-t       other-symbol-t)
(declare-joined-type non-null-sym-t t-symbol-t   other-symbol-t)
(declare-joined-type list-t         null-t       list-cons-t)
(declare-joined-type cons-t         list-cons-t  non-list-cons-t)
(declare-joined-type all-list-t     null-t       cons-t)

(declare-joined-type vector-t       string-t     non-string-vector-t)
(declare-joined-type array-t        vector-t     non-vector-array-t)

(declare-joined-type sequence-t     all-list-t   array-t)


;;------------------------------------------------------------------------------
;; Das Top-Element des Typverbands ist die Typvereinigung aller Typen.
;;------------------------------------------------------------------------------
(declare-zs-type top-t (multiple-type-join symbol-t
                                           number-t
                                           sequence-t 
                                           character-t
                                           function-t
                                           structure-t 
                                           class-t
                                           package-t
                                           stream-t
                                           hash-table-t
                                           readtable-t
                                           pathname-t
                                           random-state-t))


;;------------------------------------------------------------------------------
;; Abbildung von Typen auf den zugeordneten NOT-Typ.
;;------------------------------------------------------------------------------
(defmacro not-type (type)
  `(make-ti-type :bt (logxor top-number (ti-type-bt ,type))
                 :lt (logxor top-number (ti-type-lt ,type))))


;;------------------------------------------------------------------------------
;; Deklaration der NOT-Typen:
;;------------------------------------------------------------------------------
(declare-not-types (null-t list-t all-list-t
                    byte-t word-t fixnum-t bignum-t integer-t float-t number-t
                    symbol-t non-null-sym-t
                    character-t string-t vector-t array-t
                    function-t structure-t class-t
                    package-t stream-t hash-table-t readtable-t pathname-t
                    random-state-t))


;;------------------------------------------------------------------------------
;; Zusaetzliche Benennungen:
;;------------------------------------------------------------------------------
(declare-zs-type     atom-t               (not-type cons-t))
(declare-joined-type list-of-bottom-t     null-t list-cons-of-bottom-t)
(declare-joined-type or-symbol-string-t   symbol-t string-t)
(declare-joined-type or-sym-str-char-t    symbol-t string-t character-t)
(declare-joined-type or-null-character-t  null-t   character-t)
(declare-joined-type package-name-t       or-symbol-string-t)
(declare-joined-type package-or-name-t    package-t package-name-t)
(declare-joined-type my-stream-t          null-t symbol-t stream-t)
(declare-joined-type file-t               string-t pathname-t stream-t)



;;------------------------------------------------------------------------------
;; Weitere Funktion und Makros zum Typverband:
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Liefert den Listencons-Typ, der durch Anwendung des Typkonstruktors 
;; `list-cons-of' auf die uebergebenen Typen entsteht. 
;;------------------------------------------------------------------------------
(defun list-cons-of (&rest types)
  (if types
      (make-ti-type :bt (ti-type-bt list-cons-of-bottom-t)
                    :lt (ti-type-bt (apply #'multiple-type-join types)))
      null-t))
  

;;------------------------------------------------------------------------------
;; Liefert den Listentyp, der durch Anwendung des Typkonstruktors `list-of' auf
;; die uebergebenen Typen entsteht. 
;;------------------------------------------------------------------------------
(defun list-of (&rest types)
  (type-join null-t (apply #'list-cons-of types)))


;;------------------------------------------------------------------------------
;; Diese Abbildung liefert zu einem Typ list-of(A) den Typ A.
;;------------------------------------------------------------------------------
(defun list-component (type)
  (let ((result (make-ti-type :bt (ti-type-lt type))))
    (if (zs-subtypep list-cons-of-bottom-t result)
        (type-join result list-component-top-t)
        result)))


;;------------------------------------------------------------------------------
;; Menge der primitiven Typen.
;;------------------------------------------------------------------------------
(defparameter primitive-types
  (list bottom-t 
        byte-t non-byte-word-t non-word-fixnum-t
        fixnum-t bignum-t
        float-t
        null-t
        t-symbol-t
        other-symbol-t 
        list-cons-t 
        non-list-cons-t
        character-t
        string-t 
        non-string-vector-t 
        non-vector-array-t
        function-t
        structure-t
        class-t
        package-t
        stream-t
        hash-table-t
        readtable-t
        pathname-t
        random-state-t))


;;------------------------------------------------------------------------------
;; Einfache Typen sind die primitiven Typen und die Listentypen von prim. Typen.
;;------------------------------------------------------------------------------
(defparameter simple-types
  (append primitive-types (mapcar #'list-cons-of primitive-types)))


;;------------------------------------------------------------------------------
;; Liefert, ob `type' ein einfacher Typ ist.
;;------------------------------------------------------------------------------
(defun simple-type-p (type)
  (member type simple-types :test #'type-eq))


;;------------------------------------------------------------------------------
;; Makrodefinition fuer ein typecase ueber die zs-Typen. Als Tag des
;; zs-typecase koennen <zs-Typ1>, <(conform <zs-Typ2>)>, <T> und <otherwise> 
;; verwendet werden, die erfuellt werden, wenn sas Typdatum Untertyp von 
;; <zs-Typ1> ist, zu <zs-Typ2> konform ist, bzw. immer erfuellt ist.
;;------------------------------------------------------------------------------
(defmacro zs-typecase (form &body body)
  `(let ((comp ,form))
    (cond ,@(mapcar #'(lambda (tag-forms-pair)
                        (let ((tag   (first tag-forms-pair))
                              (forms (rest  tag-forms-pair)))
                          (cond ((member tag '(T otherwise)) 
                                 `(T ,@forms))
                                ((and (consp tag) (eq 'conform (first tag)))
                                 `((types-are-conform comp ,(second tag))
                                   ,@forms))
                                (T
                                 `((zs-subtypep comp ,tag) ,@forms)))))
                    body))))


;;------------------------------------------------------------------------------
;; Aktualisiert den Typ des Feldes `field' durch Typvereinigung mit `type'.
;;------------------------------------------------------------------------------
(defmacro update-type-f (field type)
  `(setf ,field (type-join ,field ,type)))


;;------------------------------------------------------------------------------
;; Abbildung von Lisp-Typbezeichnern auf die in der Typinferenz verwendeten
;; Typen. Diese Funktion wird z. B. bei der Ermittlung des Resulttatstyps
;; der Funktion `concatenate' verwendet.
;;------------------------------------------------------------------------------
(defun get-intern-type (type-specifier)
  (cond ((sym-p type-specifier)
         (case (?symbol type-specifier)
           (string    string-t)
           (vector    vector-t)
           (array     array-t)
           (list      all-list-t)
           (sequence  sequence-t)
           (fixnum    fixnum-t)
           (integer   integer-t)
           (float     float-t)
           (number    number-t)
           (cons      cons-t)
           (symbol    symbol-t)
           (character character-t)
           (otherwise top-t)))
        (T top-t)))
  
                
;;------------------------------------------------------------------------------
;; Abbildung von Typrepraesentationen auf Typdarstellungen.
;; Fuer die in der Liste `pairs' angegebenen Typen findet diese Funktion eine
;; abkuerzende Schreibweise bei Typausgaben. Bei zusammengesetzten Typen wird
;; der Typkonstruktor explizit dargestellt.
;;------------------------------------------------------------------------------
(defun output-type (type &optional (with-list-decomposition T))
  (let ((pairs `((,top-t               top)
                 
                 ;; NOT-Typen:
                 (,not-null-t          not-null)
                 (,atom-t              atom-t)
                 (,not-all-list-t      not-list)
                 (,not-list-t          not-proper-list)
                 (,not-byte-t          not-byte)
                 (,not-word-t          not-word)
                 (,not-fixnum-t        not-fixnum)
                 (,not-bignum-t        not-bignum)
                 (,not-float-t         not-float)
                 (,not-integer-t       not-integer)
                 (,not-number-t        not-number)
                 (,not-symbol-t        not-symbol)
                 (,not-character-t     not-character)
                 (,not-array-t         not-array)
                 (,not-vector-t        not-vector)
                 (,not-function-t      not-function)
                 (,not-structure-t     not-structure)
                 (,not-string-t        not-string)
                 (,not-class-t         not-class)
                 (,not-package-t       not-package)
                 (,not-stream-t        not-stream)
                 
                 ;; Zusammengesetzte Typen:
                 (,sequence-t          sequence)
                 (,all-list-t          list)
                 (,list-of-bottom-t    list-of-bottom)
                 (,array-t             array)
                 (,vector-t            vector)
                 (,cons-t              cons)
                 (,symbol-t            symbol)
                 (,bool-t              bool)
                 (,number-t            number)
                 (,integer-t           integer)
                 (,fixnum-t            fixnum)
                 (,word-t              word)

                 ;; Primitive Typen:
                 (,byte-t              byte)
                 (,non-byte-word-t     non-byte-word)
                 (,bignum-t            bignum)
                 (,float-t             float)
                 (,null-t              null)
                 (,t-symbol-t          t-symbol)
                 (,other-symbol-t      other-symbol)
                 (,list-cons-of-bottom-t list-cons)
                 (,non-list-cons-t     non-list-cons)
                 (,string-t            string)
                 (,non-string-vector-t non-string-vector)
                 (,non-vector-array-t  non-vector-array)
                 (,character-t         character)
                 (,function-t          function)
                 (,structure-t         structure)
                 (,class-t             class)
                 (,package-t           package)
                 (,stream-t            stream)
                 (,hash-table-t        hash-table)
                 (,readtable-t         readtable)
                 (,pathname-t          pathname)
                 (,random-state-t      random-state)
                 )))

    (if (type-eq bottom-t type)
        'bottom
        (do (or-type-list
             (rest-pairs pairs (rest rest-pairs)))
            ((endp rest-pairs) (if (endp (rest or-type-list))
                                   (first or-type-list)
                                   (cons 'or or-type-list)))
          (let ((subtype        (first  (first rest-pairs)))
                (subtype-symbol (second (first rest-pairs))))
            
            (case subtype-symbol
              (list-cons 
               (setf subtype-symbol 
                     (if with-list-decomposition
                         `(list-cons-of ,(output-type (list-component type) 
                                                      nil))
                         'list-cons)))
              (list-of-bottom
               (setf subtype-symbol 
                     (if with-list-decomposition
                         `(list-of ,(output-type (list-component type) nil))
                         'list)))
              (otherwise nil))
          
            (when (zs-subtypep subtype type)
              (push subtype-symbol or-type-list)
              (setf type (type-meet type (not-type subtype)))))))))

;;------------------------------------------------------------------------------
(provide "titypes")


