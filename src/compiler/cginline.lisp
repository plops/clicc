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
;;; Funktion : Codegenerierung
;;;            Erzeugen von Inline-Code fuer ausgewaehlte Systempraedikate
;;;
;;; $Revision: 1.44 $
;;; $Id: cginline.lisp,v 1.44 1994/12/17 11:54:49 pm Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")     

;;------------------------------------------------------------------------------
;; Bekanntgeben der Funktionen, die inline kompiliert werden.
;;------------------------------------------------------------------------------
(p0-special-funs
 (?c-inline "CG")
 clicc-lisp::eq
 clicc-lisp::eql
 clicc-lisp::integerp
 rt::fixnump
 rt::fixnum-low-p
 rt::fixnum-high-p
 clicc-lisp::consp
 clicc-lisp::characterp
 rt::plain-vector-p
 clicc-lisp::simple-vector-p
 clicc-lisp::simple-string-p
 clicc-lisp::simple-bit-vector-p
 clicc-lisp::floatp
 clicc-lisp::atom
 clicc-lisp::symbolp
 rt::symp
 clicc-lisp::listp
 clicc-lisp::numberp
 clicc-lisp::functionp
 clicc-lisp::values
 clicc-lisp::cons
 rt::%car
 rt::%cdr
 rt::%rplaca
 rt::%rplacd
 rt::%logior
 rt::%logxor
 rt::%logand
 rt::%lognot
 rt::%shift-right
 rt::%shift-left
 rt::instancep
 rt::instance-ref
 rt::instance-set
 rt::plain-vector-length
 rt::svref-internal
 rt::set-svref-internal
 rt::symbol-value
 rt::symbol-plist
 rt::symbol-package
 (L::setf rt::symbol-value)
 (L::setf rt::symbol-plist)
 (L::setf rt::symbol-package)
 rt::symbol-name
 rt::constant-flag-p
 rt::set-constant-flag
 rt::unbound-value
 rt::unbound-value-p
 rt::structp
 rt::struct-size
 rt::structure-ref
 (L::setf rt::structure-ref)
 rt::<fix
 rt::1-fix

 ffi:c-string-p
)

;;------------------------------------------------------------------------------
;; Hilfsfunktion, die Code f"ur den Zugriff auf das Resultat eines Ausdrucks
;; generiert. Bei Variablenzugriffen wird ein Kopieren des Werts vermieden.
;;------------------------------------------------------------------------------
(defun CC-get-arg (form)
  (CC-dest (get-arg-loc form)))

;;------------------------------------------------------------------------------
(defmethod get-arg-loc ((form var-ref))
  (etypecase (?var form)
    (dynamic (?var form))
    (static
     (let ((var (?var form)))

       ;; Der Stackframe des Aufrufers wird evtl. gerade wiederverwendet. Um
       ;; sicherzustellen, daß bei nachfolgenden Aufrufen von get-arg-loc
       ;; diese Variable nicht überschrieben wird, mu"s sie ggf. nach unten
       ;; kopiert werden.  Wenn sie bereits an der richtigen Position liegt,
       ;; dann mu"s *stack-top* erh"oht werden, damit sie nicht von
       ;; nachfolgenden Argumenten "uberschrieben wird.
       ;;-----------------------------------------------------
       (cond
        ((eql *level* (?level var))
         (if (> (?offset var) *stack-top*)
             (call-next-method)
             (progn (incf *stack-top*) var)))

        ;; In diesem Fall wird *stack-top* nicht erh"oht, damit keine L"ucken
        ;; im Stack entstehen. Ist das ein Problem f"ur opt-args ???
        ;;----------------------------------------------------------
        (T var))))))
                 
(defmethod get-arg-loc ((form form))
  (let ((*result-spec* (stacktop-result-location)))
    (cg-form form)
    (incf *stack-top*)
    *result-spec*))

;;------------------------------------------------------------------------------
;; Hilfsfunktion, die bei Konstanten vom Typ INT sofort die Konstante
;; zurückliefert und andernfalls einen GET_FIXNUM-Aufruf und den 'üblichen'
;; Code generiert.
;;------------------------------------------------------------------------------
(defun CC-get-int (form)
  (if (int-p form)
      (?value form)
      (CC-MacroCall "GET_FIXNUM" (CC-get-arg form))))

;;------------------------------------------------------------------------------
;; Hilfsfunktion, die bei Bedarf Boolesche C-Werte in die LISP-Werte
;; true / false umwandelt.
;;------------------------------------------------------------------------------
(defun pred-result (pred)
  (case *result-spec*
    ((NIL))
    (C-bool (setq *C-bool* pred))
    (T (C-MacroCall "LOAD_BOOL" pred (CC-dest *result-spec*)))))

;;------------------------------------------------------------------------------
;; Hilfsfunktion, die das Resultat einer inline kompilierten Funktion an die
;; in *result-spec* angegebene Position kopiert.
;;------------------------------------------------------------------------------
(defun C-result (source)
  (case *result-spec*
    
    ;; Resultat wird nicht benoetigt
    ;;------------------------------
    ((NIL))
    
    ;; Boolesches Resultat gewuenscht
    ;;-------------------------------
    (C-bool (setq *C-bool* (CC-make-bool source)))

    ;; Normales Resultat erzeugen
    ;;---------------------------
    (T (C-copy source (CC-dest *result-spec*)))))

;;------------------------------------------------------------------------------
;; EQ, inline
;; ACHTUNG: es wird davon ausgegangen, dass Zeiger die groesste Komponente
;; in CL_FORM sind.
;; Optimiert, wenn einer der Parameter konstant ist.
;;------------------------------------------------------------------------------
(defun cg-eq (form1 form2)
  (pred-result
   (let ((*stack-top* *stack-top*)
         (const-arg 0))

     ;; pruefen, welche der Argumente konstant sind
     ;;--------------------------------------------
     (if (or (simple-literal-p form1) (sym-p form1))
         (incf const-arg 1)
         (setq form1 (CC-get-arg form1)))
     (if (or (simple-literal-p form2) (sym-p form2))
         (incf const-arg 2)
         (setq form2 (CC-get-arg form2)))

     (case const-arg

       ;; eines der Argumente ist konstant
       ;;---------------------------------
       ((1 2) (when (= const-arg 2)
                (rotatef form1 form2))
        (typecase form1
          (null-form (getCode "CL_NILP(~A)" form2))
          (int
           (getCode "CL_FIXNUMP(~A) && GET_FIXNUM(~A) == ~A"
                    form2 form2 (?value form1)))
          (character-form
           (getCode "CL_CHARP(~A) && GET_CHAR(~A) == ~A"
                    form2 form2 (CC-character (?value form1))))
          (sym (getCode "CL_SYMBOLP(~A) && GET_SYMBOL(~A) == ~A"
                        form2 form2 (CC-symbol form1)))
          (T "FALSE")))
	      
       ;; beide Argumente sind konstant
       ;;------------------------------
       (3 (CC-bool (cond
                     ((and (simple-literal-p form1) (simple-literal-p form2))
                      (eq (?value form1) (?value form2)))
                     ((and (sym-p form1) (sym-p form2))
                      (eq form1 form2))
                     (T nil))))

       (t (getCode "EQ(~A, ~A)" form1 form2))))))

;;------------------------------------------------------------------------------
;; EQL, inline
;; Optimierung:
;; Wenn eines der Argument vom Typ FLOAT ist, dann direkt vergleichen.
;;------------------------------------------------------------------------------
(defun cg-eql (form1 form2)
  (pred-result
   (let ((*stack-top* *stack-top*) 
         (floatconst 0))
     
     ;; Pruefen, ob FLOAT Konstanten vorliegen
     ;;---------------------------------------
     (if (float-form-p form1)
         (incf floatconst 1)
         (setq form1 (CC-get-arg form1)))
     (if (float-form-p form2)
         (incf floatconst 2)
         (setq form2 (CC-get-arg form2)))

     (case floatconst

       ;; 'Normales' EQL
       ;;---------------
       (0 (getCode "EQL(~A, ~A)" form1 form2))
     
       ;; beide Argumente sind vom Typ FLOAT
       ;;-----------------------------------
       (3 (CC-bool (= (?value form1) (?value form2))))
     
       ;; eines der Argumente ist vom Typ FLOAT
       ;;--------------------------------------
       (T (when (= floatconst 2)
            (rotatef form1 form2))
          (getCode "CL_FLOATP(~A) && GET_FLOAT(~A) == ~A"
                   form2 form2 (?value form1)))))))

;;------------------------------------------------------------------------------
;; Typtests...
;;------------------------------------------------------------------------------
(defun cg-integerp (x-loc)
  (pred-result (getCode "CL_FIXNUMP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-fixnump (x-loc)
  (cg-integerp x-loc))

;;------------------------------------------------------------------------------
(defun cg-fixnum-low-p (val low)
  (setq val (CC-get-arg val))
  (setq low (CC-get-arg low))
  (pred-result
   (getCode "CL_FIXNUMP(~A) && GET_FIXNUM(~A) >= GET_FIXNUM(~A)" val val low)))

;;------------------------------------------------------------------------------
(defun cg-fixnum-high-p (val high)
  (setq val (CC-get-arg val))
  (setq high(CC-get-arg high))
  (pred-result
   (getCode "CL_FIXNUMP(~A) && GET_FIXNUM(~A) <= GET_FIXNUM(~A)" val val high)))


;;------------------------------------------------------------------------------
(defun cg-consp (x-loc)
  (pred-result
   (getCode "CL_CONSP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-characterp (x-loc)
  (pred-result
   (getCode "CL_CHARP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-plain-vector-p (x-loc)
  (pred-result
   (getCode "CL_SMVECP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-simple-vector-p (x-loc)
  (pred-result
   (getCode "CL_SMVEC_T_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-simple-string-p (x-loc) 
  (pred-result
   (getCode "CL_SMSTRP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-simple-bit-vector-p (x-loc) 
  (pred-result
   (getCode "CL_SMVEC_BIT_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-floatp (x-loc)
  (pred-result
   (getCode "CL_FLOATP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-atom (x-loc)
  (pred-result
   (getCode "CL_ATOMP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-symbolp (x-loc)
  (setq x-loc (CC-get-arg x-loc))
  (pred-result
   (getCode "CL_SYMBOLP(~A) || CL_NILP(~A)" x-loc x-loc)))

;;------------------------------------------------------------------------------
(defun cg-symp (x-loc)
  (pred-result
   (getCode "CL_SYMBOLP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-listp (x-loc)
  (pred-result
   (getCode "CL_LISTP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-numberp (x-loc)
  (pred-result
   (getCode "CL_NUMBERP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-functionp (x-loc)
  (pred-result
   (getCode "CL_FUNCTIONP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-instancep (x-loc)
  (pred-result
   (getCode "CL_INSTANCEP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
;; FFI
;;------------------------------------------------------------------------------
(defun cg-c-string-p (x-loc)
  (pred-result
   (getCode "CL_C_STRING_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
;; CAR, CDR, etc.
;;------------------------------------------------------------------------------
(defun cg-%car (cell)
  (C-result (CC-MacroCall "GET_CAR" (CC-get-arg cell))))

;;------------------------------------------------------------------------------
(defun cg-%cdr (cell)
  (C-result (CC-MacroCall "GET_CDR" (CC-get-arg cell))))

;;------------------------------------------------------------------------------
(defun cg-cons (x y)
  (setq x (CC-get-arg x))
  (setq y (CC-get-arg y))
  (case *result-spec*
    ((NIL))
    (C-BOOL (setq *C-bool* C-TRUE))
    (T (C-MacroCall "ALLOC_CONS" (CC-stack *stack-top*) x y
                    (CC-dest *result-spec*)))))

;;------------------------------------------------------------------------------
(defun cg-%rplaca (x y)
  (setq x (get-arg-loc x))
  (setq y (get-arg-loc y))
  (C-copy (CC-dest y) (CC-MacroCall "GET_CAR" (CC-dest x)))
  (to-result-loc x))

;;------------------------------------------------------------------------------
(defun cg-%rplacd (x y)
  (setq x (get-arg-loc x))
  (setq y (get-arg-loc y))
  (C-copy (CC-dest y) (CC-MacroCall "GET_CDR" (CC-dest x)))
  (to-result-loc x))

;;------------------------------------------------------------------------------
;; Operationen auf FIXNUMs
;;------------------------------------------------------------------------------
(defun cg-%logior (x y)
  (setq x (CC-get-int x))
  (setq y (CC-get-int y))
  (C-integer (CC-opIor x y) (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%logxor (x y)
  (setq x (CC-get-int x))
  (setq y (CC-get-int y))
  (C-integer (CC-op^ x y) (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%logand (x y)
  (setq x (CC-get-int x))
  (setq y (CC-get-int y))
  (C-integer (CC-op& x y) (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%lognot (x)
  (setq x (CC-get-int x))
  (C-integer (CC-op~ x) (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%shift-right (i c)
  (setq i (CC-get-int i))
  (setq c (CC-get-int c))
  (C-integer (CC-op>> i c) (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%shift-left (i c)
  (setq i (CC-get-int i))
  (setq c (CC-get-int c))
  (C-integer (CC-op<< i c) (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
;; Instanzen...
;;------------------------------------------------------------------------------
(defun cg-instance-ref (instance offset)
  (setq instance (CC-get-arg instance))
  (setq offset (CC-get-int offset))
  (C-result (CC-MacroCall "OFFSET"
                          (getCode "AR_BASE(GET_FORM(~A))" instance)
                          (CC-op+ offset 1))))

;;------------------------------------------------------------------------------
(defun cg-instance-set (newvalue instance offset)
  (setq newvalue (get-arg-loc newvalue))
  (setq instance (CC-get-arg instance))
  (setq offset (CC-get-int offset))
  (C-copy (CC-dest newvalue)
          (CC-MacroCall "OFFSET"
                        (getCode "AR_BASE(GET_FORM(~A))" instance)
                        (CC-op+ offset 1)))
  (to-result-loc newvalue))

;;------------------------------------------------------------------------------
;; Vektoren...
;;------------------------------------------------------------------------------
(defun cg-plain-vector-length (x)
  (setq x (CC-get-arg x))
  (C-integer (CC-MacroCall "AR_SIZE" (CC-MacroCall "GET_FORM" x))
             (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-svref-internal (vector index)
  (setq vector (CC-get-arg vector))
  (setq index (CC-get-int index))
  (C-result (CC-MacroCall "OFFSET"
                          (getCode "AR_BASE(GET_FORM(~A))" vector) index)))

;;------------------------------------------------------------------------------
(defun cg-set-svref-internal (newvalue vector index)
  (setq newvalue (get-arg-loc newvalue))
  (setq vector (CC-get-arg vector))
  (setq index (CC-get-int index))
  (C-Copy (CC-dest newvalue)
          (CC-MacroCall "OFFSET"
                        (getCode "AR_BASE(GET_FORM(~A))" vector) index))
  (to-result-loc newvalue))

;;------------------------------------------------------------------------------
;; Symbole
;;------------------------------------------------------------------------------
(defun cg-symbol-value (sym)
  (C-result (CC-MacroCall "SYM_VALUE" (CC-get-arg sym))))

(defun cg-set-symbol-value (value sym)
  (setq value (get-arg-loc value))
  (C-Copy (CC-dest value) (CC-MacroCall "SYM_VALUE" (CC-get-arg sym)))
  (to-result-loc value))

(defun cg-symbol-plist (sym)
  (C-result (CC-MacroCall "SYM_PLIST" (CC-get-arg sym))))

(defun cg-set-symbol-plist (value sym)
  (setq value (get-arg-loc value))
  (C-Copy (CC-dest value) (CC-MacroCall "SYM_PLIST" (CC-get-arg sym)))
  (to-result-loc value))

(defun cg-symbol-package (sym)
  (C-result (CC-MacroCall "SYM_PACKAGE" (CC-get-arg sym))))

(defun cg-set-symbol-package (value sym)
  (setq value (get-arg-loc value))
  (C-Copy (CC-dest value) (CC-MacroCall "SYM_PACKAGE" (CC-get-arg sym)))
  (to-result-loc value))

(defun cg-symbol-name (sym)
  (setq sym (CC-get-arg sym))
  (case *result-spec*
    ((NIL))
    (C-BOOL (setq *C-bool* C-TRUE))
    (T (C-MacroCall "GET_SYM_NAME" (CC-stack *stack-top*) sym
                    (CC-dest *result-spec*)))))

(defun cg-constant-flag-p (sym)
  (pred-result
   (getCode "SYM_IS_CONST(~A)" (CC-get-arg sym))))

(defun cg-set-constant-flag (sym)
  (C-MacroCall "SYM_SET_CONST" (CC-get-arg sym)))

(defun cg-unbound-value ()
  (C-MacroCall "LOAD_UNBOUND" (CC-dest *result-spec*)))

(defun cg-unbound-value-p (object)
  (pred-result
   (getCode "CL_UNBOUNDP(~A)" (CC-get-arg object))))

;;------------------------------------------------------------------------------
;; Strukturen
;;------------------------------------------------------------------------------
(defun cg-structp (object)
  (pred-result
   (getCode "CL_STRUCTP(~A)" (CC-get-arg object))))

;;------------------------------------------------------------------------------
(defun cg-struct-size (struct)
  (C-integer (getCode "AR_SIZE(GET_FORM(~A))" (CC-get-arg struct))
             (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-structure-ref (struct index)
  (C-result (CC-MacroCall "OFFSET" 
                          (getCode "AR_BASE(GET_FORM(~A))" (CC-get-arg struct))
                          (CC-op+ (CC-get-int index) 1))))

;;------------------------------------------------------------------------------
(defun cg-set-structure-ref (newvalue struct index)
  (setq newvalue (get-arg-loc newvalue))
  (C-Copy (CC-dest newvalue)
          (CC-MacroCall "OFFSET"
                        (getCode "AR_BASE(GET_FORM(~A))" (CC-get-arg struct))
                        (CC-op+ (CC-get-int index) 1)))
  (to-result-loc newvalue))

;;------------------------------------------------------------------------------
;; Arithmetische Funktionen
;;------------------------------------------------------------------------------
(defun cg-<fix (a b)
  (setq a (CC-get-int a))
  (setq b (CC-get-int b))
  (pred-result
   (CC-op< a b)))

;;------------------------------------------------------------------------------

(defun cg-1-fix (a)
  (C-integer (CC-op- (CC-get-int a) 1) (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------


(provide "cginline")
