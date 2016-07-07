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
;;; Funktion : Ein Interpretierer fuer Teile der Zwischensprache.
;;;
;;; $Revision: 1.71 $
;;; $Id: p1eval.lisp,v 1.71 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defun p1-eval (form)
  (catch 'cant-eval
    (let ((v (zw-eval (in-compile-time-env (p1-form form)) (zw-empty-env))))
      (labels ((chk-no-fun (v)
                 (typecase v
                   (symbol nil)         ; CLtL1 (!) symbol < function
                   (cons (chk-no-fun (car v)) (chk-no-fun (cdr v)))
                   (function (unable-to-eval v))
                   #+(OR CLISP LUCID) (string nil)        
                   ((array standard-char) nil)
                   (array (map-array #'chk-no-fun v))
                   (t nil))))
        (chk-no-fun v)
        (values v t)))))

;;------------------------------------------------------------------------------
(defun unable-to-eval (form)
  (throw 'cant-eval
    (values form NIL)))

;;------------------------------------------------------------------------------
;; Funktion, die ein leeres Environment generiert.
;;------------------------------------------------------------------------------
(defun zw-empty-env () ())

;;------------------------------------------------------------------------------
;; erweitert das aktuelle Environment um eine Bindung fuer var mit dem initialen
;; Wert value.
;;------------------------------------------------------------------------------
(defmacro zw-bind (var value env)
  `(push (cons ,var ,value) ,env))

;;------------------------------------------------------------------------------
;; Setzt den Wert von var in env destruktiv auf value.
;;------------------------------------------------------------------------------
(defmacro zw-setq (var value env)
  `(setf (cdr (assoc ,var ,env)) ,value))

;;------------------------------------------------------------------------------
;; Bestimmt den aktuellen Wert von var in env.
;;------------------------------------------------------------------------------
(defmacro zw-get-bind (var env)
  `(cdr (assoc ,var ,env)))


;;------------------------------------------------------------------------------
;; Fehlerfall
;;------------------------------------------------------------------------------
(defmethod zw-eval ((anything t) env)
  (declare (ignore env))
  #+PCL (when (functionp anything)
          (return-from zw-eval anything))
  (error "can't eval ~A" anything))

;;------------------------------------------------------------------------------
;; Auswertung von Literalen
;;------------------------------------------------------------------------------


;;------------------------------------------------------------------------------
;; Die als strukturierte Literale repraesentierten Listen enthalten mit Ausnahme
;; von Symbolen und Strukturen Literale in ihrer Lisp-Repraesentation.
;; Der Interpretierer ruft in diesem Fall eval-structure-literal-list auf.
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form structured-literal) env)
  (let ((value (?value form)))
    (etypecase value
      ((or list array) (eval-structured-literal-value value env)))))

;;------------------------------------------------------------------------------
;; Die in der Liste enthaltenen Symbole werden ausgewertet, alle anderen
;; Literale bleiben erhalten.
;;------------------------------------------------------------------------------
(defun eval-structured-literal-value (x &optional env)
  (labels
      ((f (x)
         (typecase x
           (cons (cons (f (car x)) (f (cdr x))))
           (string x)
           (array (copy-array #'f x))
           (sym  (zw-eval x env))
           (literal-instance (zw-eval x env))
           (t x))))
    (f x)))

;;------------------------------------------------------------------------------
(defmethod zw-eval ((a-simple-literal simple-literal) env)
  (declare (ignore env))
  (?value a-simple-literal))

(defmethod zw-eval ((a-null-form null-form) env)
  (declare (ignore env))
  '())

;;------------------------------------------------------------------------------
;; literal-instance
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form literal-instance) env)
  (declare (ignore env))
  (unable-to-eval form))

;;------------------------------------------------------------------------------
;; Symbole
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form sym) env)
  (declare (ignore env))
  (?symbol form))

;;------------------------------------------------------------------------------
;; Klassen
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form class-def) env)
  (declare (ignore env))
  form)

;;------------------------------------------------------------------------------
;; Auswertung eines lesenden Zugriffs auf eine Variable
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form var-ref) env)
  (setq form (?var form))
  (cond
    ((local-static-p form)
     (zw-get-bind form env))
    (t (setq form (?sym form))
       (cond
         ((constant-value-p form) (zw-eval (?constant-value form) env))
         (t (unable-to-eval form))))))

;;------------------------------------------------------------------------------
;; Auswertung einer benannten Konstante
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form defined-named-const) env)
  (if (member (?value form) '(:unknown :forward))
      (unable-to-eval form)
      (zw-eval (?value form) env)))

(defmethod zw-eval ((form imported-named-const) env)
  (declare (ignore env))
  (unable-to-eval form))

;;------------------------------------------------------------------------------
;; Zuweisung an eine Variable
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form setq-form) env)
  (let ((var (?var (?location form))))
    (cond
      ((local-static-p var)
        (zw-setq var (zw-eval (?form form) env) env))
      (t (unable-to-eval form)))))

;;------------------------------------------------------------------------------
;; Hintereinanderausfuehrung
;;------------------------------------------------------------------------------
(defmethod zw-eval ((a-progn-form progn-form) env)
  (let ((evaluator #'(lambda (form)
                       (zw-eval form env))))
    (mapc-progn-form-list (?form-list a-progn-form) evaluator evaluator)))

;;------------------------------------------------------------------------------
;; Konditional
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form if-form) env)
  (if (zw-not-null-p (zw-eval (?pred form) env))
      (zw-eval (?then form) env)
      (zw-eval (?else form) env)))

;;------------------------------------------------------------------------------
;; Hinzufuegen neuer Variablenbindungen
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form let*-form) env)
  (mapc #'(lambda (variable init-form)
            (if (local-static-p variable)
                (zw-bind variable (zw-eval init-form env) env)
                (unable-to-eval form)))
        (?var-list form)
        (?init-list form))
  (zw-eval (?body form) env))

;;------------------------------------------------------------------------------
;; Multiple Values
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form mv-lambda) env)
  (multiple-value-call
      (zw-eval (make-defined-fun :symbol 'mv-lambda
                                 :params (?params form)
                                 :body (?body form))
               env)
    (zw-eval (?arg form) env)))
  
;;------------------------------------------------------------------------------
;; Tagbody und Go
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form tagbody-form) env)
  (let ((secret (gensym))
        (next-tagged-forms (?tagged-form-list form))
        target)
    (zw-bind form secret env)
    (setq target (catch secret (zw-eval (?first-form form) env)))
    (loop
     (if (tagged-form-p target)
         (setq next-tagged-forms (member target (?tagged-form-list form)))
         (unless next-tagged-forms (return)))
     (setq target
           (catch secret (zw-eval (?form (pop next-tagged-forms)) env))))))

;;------------------------------------------------------------------------------
(defmethod zw-eval ((form tagged-form) env)
  (throw (zw-get-bind (?tagbody form) env) form))

;------------------------------------------------------------------------------
;; Continuations
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form let/cc-form) env)
  (let ((secret (gensym)))
    (zw-bind (?cont form) #'(lambda (x) (throw secret x)) env)
    (catch secret
      (zw-eval (?body form) env))))

;;------------------------------------------------------------------------------
(defmethod zw-eval ((cont cont) env)
  (zw-get-bind cont env))

;;------------------------------------------------------------------------------
;; Funktionales Objekt
;; Globale und importierte Funktionen greifen nicht auf env zu
;;------------------------------------------------------------------------------
(defmethod zw-eval ((fun fun) env)
  #'(lambda (&rest arg-list)
      (zw-apply fun arg-list env)))

;;------------------------------------------------------------------------------
;; Funktion des Wirts Lispsystems.
;; Kommt nur im Rumpf von Makroexpansionsfunktionen vor und nur fuer Lisp
;; Systemfunktionen.
;;------------------------------------------------------------------------------
#-PCL                        ; siehe (zw-eval T T)
(defmethod zw-eval ((function function) env)
  (declare (ignore env))
  function)

;;------------------------------------------------------------------------------
;; Funktionsanwendung
;;------------------------------------------------------------------------------
(defmethod zw-eval ((app app) env)
  (apply (zw-eval (?form app) env)
         (mapcar #'(lambda (arg) (zw-eval arg env)) (?arg-list app))))

;;------------------------------------------------------------------------------
;; Definition lokaler Funktionen
;; Die Definitionen brauchen nicht betrachtet zu werden, da die angewandten
;; Vorkommen alle relevanten Informationen enthalten.
;;------------------------------------------------------------------------------
(defmethod zw-eval ((form labels-form) env)
  (zw-eval (?body form) env))

;;------------------------------------------------------------------------------
(defmethod zw-apply ((fun defined-fun) arg-list env)
  (let ((params (?params fun)))
    (dolist (var (?var-list params))
      (zw-bind var (pop arg-list) env))
    (dolist (opt (?opt-list params))
      (zw-bind (?var opt)
               (cond
                 (arg-list
                  (when (?suppl opt) (zw-bind (?suppl opt) (zw-t) env))
                  (pop arg-list))
                 (t (when (?suppl opt) (zw-bind (?suppl opt) nil env))
                    (zw-eval (?init opt) env)))
               env))
    (when (?rest params)
      (zw-bind (?rest params) arg-list env))
    (when (?key-list params)
      (unable-to-eval (?symbol fun))))
  (zw-eval (?body fun) env))

;;------------------------------------------------------------------------------
(defmethod zw-apply ((fun imported-fun) arg-list env)
  (let ((app-fun (zw-symbol-fun (?symbol fun))))
    (cond
      (app-fun (apply app-fun arg-list))
      ((slot-boundp fun 'syntax) (zw-apply (?syntax fun) arg-list env))
      (T (unable-to-eval (?symbol fun))))))

;;------------------------------------------------------------------------------
(defun zw-t ()
  (get-symbol-bind t))
  
;;------------------------------------------------------------------------------
(defun zw-not-null-p (x) x)

;;------------------------------------------------------------------------------
(defun zw-error (&rest args) (apply #'clicc-error args))

;;------------------------------------------------------------------------------
;; Unser eigenes symbol-function
;;------------------------------------------------------------------------------
(defun zw-symbol-fun (fun-sym)
  (gethash fun-sym *zw-sym-fun-hash-table*))

;;------------------------------------------------------------------------------
;; Initialisierung der Hash-Table fuer zw-symbol-fun
;;------------------------------------------------------------------------------
(defun init-zw-sym-fun-hash-table ()
  (setq *zw-sym-fun-hash-table* (make-hash-table :test #'equal :size 377))
  (macrolet
    #-CLISP
    ((sym-fun (name fun)
              `(setf (gethash ',name *zw-sym-fun-hash-table*) #',fun)))
    #+CLISP
    ((sym-fun (name fun)
              `(let (dummy-to-avoid-error)
                (setq dummy-to-avoid-error #',fun)
                (setf (gethash ',name *zw-sym-fun-hash-table*)
                 dummy-to-avoid-error))))
    
    ;; Predicates
    ;; typep und subtypep des Wirts-Lispsystems nicht benutzen
    (sym-fun L::null null)
    (sym-fun L::symbolp symbolp)
    (sym-fun L::atom atom)
    (sym-fun L::consp consp)
    (sym-fun L::listp listp)
    (sym-fun L::numberp numberp)
    (sym-fun L::integerp integerp)
    (sym-fun L::rationalp rationalp)
    (sym-fun L::floatp floatp)
    ;; .. real, complex
    (sym-fun L::characterp characterp)
    (sym-fun L::stringp stringp)
    ;; bit-vector-p, vectorp, simple-vector-p
    (sym-fun L::simple-string-p simple-string-p)
    ;; simple-bit-vector-p, arrayp, packagep, functionp, compiled-function-p
    ;; common
    (sym-fun L::eq eq)
    (sym-fun L::eql eql)
    (sym-fun L::equal equal)
    (sym-fun L::not not)

    ;; Control Structure
    ;; kein symbol-value, symbol-function, boundp, fboundp, special-form-p, set
    ;; kein makunbound, fmakunbound
    ;; kein get-setf-method, get-setf-method-multiple-value
    (sym-fun
     L::apply
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply #'apply fn args)
           (error "apply: first argument ~a must be a function" fn))))
    (sym-fun
     L::funcall
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply fn args)
           (error "funcall: first argument ~a must be a function" fn))))
    (sym-fun
     L::mapcar
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply #'mapcar fn args)
           (error "mapcar: first argument ~a must be a function" fn))))
    (sym-fun
     L::maplist
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply #'maplist fn args)
           (error "maplist: first argument ~a must be a function" fn))))
    (sym-fun
     L::mapc
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply #'mapc fn args)
           (error "mapc: first argument ~a must be a function" fn))))
    (sym-fun
     L::mapl
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply #'mapl fn args)
           (error "mapl: first argument ~a must be a function" fn))))
    (sym-fun
     L::mapcan
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply #'mapcan fn args)
           (error "mapcan: first argument ~a must be a function" fn))))
    (sym-fun
     L::mapcon
     (lambda (fn &rest args)
       (if (functionp fn)
           (apply #'mapcon fn args)
           (error "mapcon: first argument ~a must be a function" fn))))
    (sym-fun L::values values)

    ;; Symbols
    ;; keine P-Listen von Symbolen bearbeiten !
    (sym-fun L::getf getf)
    (sym-fun
     rt::set-prop
     (lambda (plist indicator value)
       (labels ((get-prop (list indicator)
                  (cond
                    ((atom list) nil) 
                    ((eq (car list) indicator) list)
                    (t (get-prop (cddr list) indicator)))))
         (let ((list (get-prop plist indicator)))
           (cond
             (list
              (rplaca (cdr list) value)
              plist)
             (t (cons indicator (cons value plist))))))))
    
    (sym-fun
     rt::remf-internal
     (lambda (list indicator)
       (cond
         ((atom list) (values nil nil))
         ((eq (car list) indicator) (values (cddr list) t))
         (t (do ((list1 list (cddr list1))
                 (list2 (cddr list) (cddr list2)))
                ((atom list2) (values nil nil)) ;end test
              (when (eq (car list2) indicator)
                (rplacd (cdr list1) (cddr list2))
                (return (values list t))))))))

    (sym-fun L::get-properties get-properties)
    (sym-fun L::symbol-name symbol-name)
    (sym-fun L::make-symbol make-symbol)
    (sym-fun L::copy-symbol copy-symbol)
    (sym-fun L::gensym gensym)
    (sym-fun L::gentemp gentemp)
    (sym-fun L::symbol-package symbol-package)
    (sym-fun L::keywordp keywordp)

    ;; Packages
    (sym-fun L::intern intern)
    (sym-fun L::find-package find-package)

    ;; Numbers
    (sym-fun L::zerop zerop)
    (sym-fun L::plusp plusp)
    (sym-fun L::minusp minusp)
    (sym-fun L::oddp oddp)
    (sym-fun L::evenp evenp)
    (sym-fun L::= =)
    (sym-fun L::/= /=)
    (sym-fun L::< <)
    (sym-fun L::> >)
    (sym-fun L::<= <=)
    (sym-fun L::>= >=)
    (sym-fun L::max max)
    (sym-fun L::min min)
    (sym-fun L::+ +)
    (sym-fun L::- -)
    (sym-fun L::* *)
    (sym-fun L::/ /)
    (sym-fun L::1+ 1+)
    (sym-fun L::1- 1-)
    (sym-fun L::exp exp)
    (sym-fun L::expt expt)
    (sym-fun L::log log)
    (sym-fun L::sqrt sqrt)
    (sym-fun L::isqrt isqrt)
    (sym-fun L::abs abs)
    (sym-fun L::signum signum)
    (sym-fun L::float float)
    (sym-fun L::floor floor)
    (sym-fun L::ceiling ceiling)
    (sym-fun L::truncate truncate)
    (sym-fun L::round round)
    (sym-fun L::mod mod)
    (sym-fun L::rem rem)
    (sym-fun L::logior logior)
    (sym-fun L::logxor logxor)
    (sym-fun L::logand logand)
    (sym-fun L::logeqv logeqv)
    (sym-fun L::lognand lognand)
    (sym-fun L::lognor lognor)
    (sym-fun L::logandc1 logandc1)
    (sym-fun L::logandc2 logandc2)
    (sym-fun L::logorc1 logorc1)
    (sym-fun L::logorc2 logorc2)
    (sym-fun L::ash ash)

    ;; Characters
    (sym-fun L::standard-char-p standard-char-p)
    (sym-fun L::graphic-char-p graphic-char-p)
    (sym-fun L::alpha-char-p alpha-char-p)
    (sym-fun L::upper-case-p upper-case-p)
    (sym-fun L::lower-case-p lower-case-p)
    (sym-fun L::both-case-p both-case-p)
    (sym-fun L::digit-char-p digit-char-p)
    (sym-fun L::alphanumericp alphanumericp)
    (sym-fun L::char= char=)
    (sym-fun L::char\= char\=)
    (sym-fun L::char< char<)
    (sym-fun L::char> char>)
    (sym-fun L::char<= char<=)
    (sym-fun L::char>= char>=)
    (sym-fun L::char-equal char-equal)
    (sym-fun L::char-not-equal char-not-equal)
    (sym-fun L::char-lessp char-lessp)
    (sym-fun L::char-greaterp char-greaterp)
    (sym-fun L::char-not-greaterp char-not-greaterp)
    (sym-fun L::char-not-lessp char-not-lessp)
    (sym-fun L::char-code char-code)
    (sym-fun L::code-char code-char)
    (sym-fun L::character character)
    (sym-fun L::char-upcase char-upcase)
    (sym-fun L::char-downcase char-downcase)
    (sym-fun L::digit-char digit-char)
    (sym-fun L::char-name char-name)
    (sym-fun L::name-char name-char)
    
    ;; Sequences
    (sym-fun L::elt elt)
    (sym-fun (L::setf L::elt) (lambda (v s i)
                                (setf (elt s i) v)))
    (sym-fun L::subseq subseq)
    (sym-fun L::copy-seq copy-seq)
    (sym-fun L::length length)
    (sym-fun L::reverse reverse)
    (sym-fun L::nreverse nreverse)
    (sym-fun L::make-sequence make-sequence)
    (sym-fun L::concatenate concatenate)
    (sym-fun L::map map)
    (sym-fun L::some some)
    (sym-fun L::every every)
    (sym-fun L::notany notany)
    (sym-fun L::notevery notevery)
    (sym-fun L::reduce reduce)
    (sym-fun L::fill fill)
    (sym-fun L::replace replace)
    (sym-fun L::remove remove)
    (sym-fun L::remove-if remove-if)
    (sym-fun L::remove-if-not remove-if-not)
    (sym-fun L::delete delete)
    (sym-fun L::delete-if delete-if)
    (sym-fun L::delete-if-not delete-if-not)
    (sym-fun L::remove-duplicates remove-duplicates)
    (sym-fun L::delete-duplicates delete-duplicates)
    (sym-fun L::substitute substitute)
    (sym-fun L::substitute-if substitute-if)
    (sym-fun L::substitute-if-not substitute-if-not)
    (sym-fun L::nsubstitute nsubstitute)
    (sym-fun L::nsubstitute-if nsubstitute-if)
    (sym-fun L::nsubstitute-if-not nsubstitute-if-not)
    (sym-fun L::find find)
    (sym-fun L::find-if find-if)
    (sym-fun L::find-if-not find-if-not)
    (sym-fun L::position position)
    (sym-fun L::position-if position-if)
    (sym-fun L::position-if-not position-if-not)
    (sym-fun L::count count)
    (sym-fun L::count-if count-if)
    (sym-fun L::count-if-not count-if-not)
    (sym-fun L::mismatch mismatch)
    (sym-fun L::search search)
    (sym-fun L::sort sort)
    (sym-fun L::stable-sort stable-sort)
    (sym-fun L::merge merge)

    ;; Lists
    (sym-fun L::car car)
    (sym-fun (L::setf L::car) (lambda (v c)
                                (setf (car c) v)))
    (sym-fun L::cdr cdr)
    (sym-fun (L::setf L::cdr) (lambda (v c)
                                (setf (cdr c) v)))
    (sym-fun L::caar caar)
    (sym-fun (L::setf L::caar) (lambda (v c)
                                 (setf (caar c) v)))
    (sym-fun L::cadr cadr)
    (sym-fun (L::setf L::cadr) (lambda (v c)
                                 (setf (cadr c) v)))
    (sym-fun L::cdar cdar)
    (sym-fun (L::setf L::cdar) (lambda (v c)
                                 (setf (cdar c) v)))
    (sym-fun L::cddr cddr)
    (sym-fun (L::setf L::cddr) (lambda (v c)
                                 (setf (cddr c) v)))
    (sym-fun L::caaar caaar)
    (sym-fun (L::setf L::caaar) (lambda (v c)
                                  (setf (caaar c) v)))
    (sym-fun L::caadr caadr)
    (sym-fun (L::setf L::caadr) (lambda (v c)
                                  (setf (caadr c) v)))
    (sym-fun L::cadar cadar)
    (sym-fun (L::setf L::cadar) (lambda (v c)
                                  (setf (cadar c) v)))
    (sym-fun L::caddr caddr)
    (sym-fun (L::setf L::caddr) (lambda (v c)
                                  (setf (caddr c) v)))
    (sym-fun L::cadar cadar)
    (sym-fun (L::setf L::cadar) (lambda (v c)
                                  (setf (cadar c) v)))
    (sym-fun L::caddr caddr)
    (sym-fun (L::setf L::caddr) (lambda (v c)
                                  (setf (caddr c) v)))
    (sym-fun L::cdaar cdaar)
    (sym-fun (L::setf L::cdaar) (lambda (v c)
                                  (setf (cdaar c) v)))
    (sym-fun L::cdadr cdadr)
    (sym-fun (L::setf L::cdadr) (lambda (v c)
                                  (setf (cdadr c) v)))
    (sym-fun L::cddar cddar)
    (sym-fun (L::setf L::cddar) (lambda (v c)
                                  (setf (cddar c) v)))
    (sym-fun L::cdddr cdddr)
    (sym-fun (L::setf L::cdddr) (lambda (v c)
                                  (setf (cdddr c) v)))
    (sym-fun L::caaaar caaaar)
    (sym-fun (L::setf L::caaaar) (lambda (v c)
                                   (setf (caaaar c) v)))
    (sym-fun L::caaadr caaadr)
    (sym-fun (L::setf L::caaadr) (lambda (v c)
                                   (setf (caaadr c) v)))
    (sym-fun L::caadar caadar)
    (sym-fun (L::setf L::caadar) (lambda (v c)
                                   (setf (caadar c) v)))
    (sym-fun L::caaddr caaddr)
    (sym-fun (L::setf L::caaddr) (lambda (v c)
                                   (setf (caaddr c) v)))
    (sym-fun L::cadaar cadaar)
    (sym-fun (L::setf L::cadaar) (lambda (v c)
                                   (setf (cadaar c) v)))
    (sym-fun L::cadadr cadadr)
    (sym-fun (L::setf L::cadadr) (lambda (v c)
                                   (setf (cadadr c) v)))
    (sym-fun L::caddar caddar)
    (sym-fun (L::setf L::caddar) (lambda (v c)
                                   (setf (caddar c) v)))
    (sym-fun L::cadddr cadddr)
    (sym-fun (L::setf L::cadddr) (lambda (v c)
                                   (setf (cadddr c) v)))
    (sym-fun L::cdaaar cdaaar)
    (sym-fun (L::setf L::cdaaar) (lambda (v c)
                                   (setf (cdaaar c) v)))
    (sym-fun L::cdaadr cdaadr)
    (sym-fun (L::setf L::cdaadr) (lambda (v c)
                                   (setf (cdaadr c) v)))
    (sym-fun L::cdadar cdadar)
    (sym-fun (L::setf L::cdadar) (lambda (v c)
                                   (setf (cdadar c) v)))
    (sym-fun L::cdaddr cdaddr)
    (sym-fun (L::setf L::cdaddr) (lambda (v c)
                                   (setf (cdaddr c) v)))
    (sym-fun L::cddaar cddaar)
    (sym-fun (L::setf L::cddaar) (lambda (v c)
                                   (setf (cddaar c) v)))
    (sym-fun L::cddadr cddadr)
    (sym-fun (L::setf L::cddadr) (lambda (v c)
                                   (setf (cddadr c) v)))
    (sym-fun L::cdddar cdddar)
    (sym-fun (L::setf L::cdddar) (lambda (v c)
                                   (setf (cdddar c) v)))
    (sym-fun L::cddddr cddddr)
    (sym-fun (L::setf L::cddddr) (lambda (v c)
                                   (setf (cddddr c) v)))
    (sym-fun L::cons cons)
    (sym-fun L::tree-equal tree-equal)
    (sym-fun L::endp endp)
    (sym-fun L::list-length list-length)
    (sym-fun L::nth nth)
    (sym-fun (L::setf L::nth) (lambda (v l i)
                                (setf (nth l i) v)))
    (sym-fun L::first first)
    (sym-fun (L::setf L::first) (lambda (v c)
                                  (setf (first c) v)))
    (sym-fun L::second second)
    (sym-fun (L::setf L::second) (lambda (v c)
                                   (setf (second c) v)))
    (sym-fun L::third third)
    (sym-fun (L::setf L::third) (lambda (v c)
                                  (setf (third c) v)))
    (sym-fun L::fourth fourth)
    (sym-fun (L::setf L::fourth) (lambda (v c)
                                   (setf (fourth c) v)))
    (sym-fun L::fifth fifth)
    (sym-fun (L::setf L::fifth) (lambda (v c)
                                  (setf (fifth c) v)))
    (sym-fun L::sixth sixth)
    (sym-fun (L::setf L::sixth) (lambda (v c)
                                  (setf (sixth c) v)))
    (sym-fun L::seventh seventh)
    (sym-fun (L::setf L::seventh) (lambda (v c)
                                    (setf (seventh c) v)))
    (sym-fun L::eighth eighth)
    (sym-fun (L::setf L::eighth) (lambda (v c)
                                   (setf (eighth c) v)))
    (sym-fun L::ninth ninth)
    (sym-fun (L::setf L::ninth) (lambda (v c)
                                  (setf (ninth c) v)))
    (sym-fun L::tenth tenth)
    (sym-fun (L::setf L::tenth) (lambda (v c)
                                  (setf (tenth c) v)))
    (sym-fun L::rest rest)
    (sym-fun (L::setf L::rest) (lambda (v c)
                                 (setf (rest c) v)))
    (sym-fun L::nthcdr nthcdr)
    (sym-fun L::last last)
    (sym-fun L::list list)
    (sym-fun L::list* list*)
    (sym-fun L::make-list make-list)
    (sym-fun L::append append)
    (sym-fun L::copy-list copy-list)
    (sym-fun L::copy-alist copy-alist)
    (sym-fun L::copy-tree copy-tree)
    (sym-fun L::revappend revappend)
    (sym-fun L::nconc nconc)
    (sym-fun L::nreconc nreconc)
    (sym-fun L::butlast butlast)
    (sym-fun L::nbutlast nbutlast)
    (sym-fun L::ldiff ldiff)
    (sym-fun L::rplaca rplaca)
    (sym-fun L::rplacd rplacd)
    (sym-fun L::subst subst)
    (sym-fun L::subst-if subst-if)
    (sym-fun L::subst-if-not subst-if-not)
    (sym-fun L::nsubst nsubst)
    (sym-fun L::nsubst-if nsubst-if)
    (sym-fun L::nsubst-if-not nsubst-if-not)
    (sym-fun L::sublis sublis)
    (sym-fun L::nsublis nsublis)
    (sym-fun L::member member)
    (sym-fun L::member-if member-if)
    (sym-fun L::member-if-not member-if-not)
    (sym-fun L::tailp tailp)
    (sym-fun L::adjoin adjoin)
    (sym-fun L::union union)
    (sym-fun L::nunion nunion)
    (sym-fun L::intersection intersection)
    (sym-fun L::set-difference set-difference)
    (sym-fun L::subsetp subsetp)
    (sym-fun L::acons acons)
    (sym-fun L::pairlis pairlis)
    (sym-fun L::assoc assoc)
    (sym-fun L::assoc-if assoc-if)
    (sym-fun L::assoc-if-not assoc-if-not)
    (sym-fun L::rassoc rassoc)
    (sym-fun L::rassoc-if rassoc-if)
    (sym-fun L::rassoc-if-not rassoc-if-not)

    ;; Arrays
    ;; ..
    (sym-fun L::aref aref)
    (sym-fun (L::setf L::aref) (lambda (v &rest l)
                                 (setf (apply #'aref l) v)))
    (sym-fun L::svref svref)

;;; is not required to be supported, in ANSI-CL use #'(setf svref) instead    
;;;    (sym-fun (L::setf L::svref) (lambda (v l)
;;;                                  (setf (apply #'svref l) v)))
    ;; ..
    
    ;; Strings
    (sym-fun L::char char)
;;;    (sym-fun (L::setf L::char) (lambda (v l)
;;;                                 (setf (apply #'char l) v)))
    (sym-fun L::schar schar)
;;;    (sym-fun (L::setf L::schar) (lambda (v l)
;;;                                  (setf (apply #'schar l) v)))
    (sym-fun L::string= string=)
    (sym-fun L::string-equal string-equal)
    (sym-fun L::string< string<)
    (sym-fun L::string> string>)
    (sym-fun L::string<= string<=)
    (sym-fun L::string>= string>=)
    (sym-fun L::string/= string/=)
    (sym-fun L::string-lessp string-lessp)
    (sym-fun L::string-greaterp string-greaterp)
    (sym-fun L::string-not-greaterp string-not-greaterp)
    (sym-fun L::string-not-lessp string-not-lessp)
    (sym-fun L::string-not-equal string-not-equal)
    (sym-fun L::make-string make-string)
    (sym-fun L::string-trim string-trim)
    (sym-fun L::string-left-trim string-left-trim)
    (sym-fun L::string-right-trim string-right-trim)
    (sym-fun L::string-upcase string-upcase)
    (sym-fun L::string-downcase string-downcase)
    (sym-fun L::string-capitalize string-capitalize)
    (sym-fun L::nstring-upcase nstring-upcase)
    (sym-fun L::nstring-downcase nstring-downcase)
    (sym-fun L::nstring-capitalize nstring-capitalize)
    (sym-fun L::string string)

    ;; I/O
    ;; ..
    (sym-fun L::write-to-string write-to-string)
    (sym-fun L::prin1-to-string prin1-to-string)
    (sym-fun L::princ-to-string princ-to-string)
    ;; ..
    (sym-fun L::format format)

    ;; Errors
    (sym-fun L::error error)
    (sym-fun L::warn warn)
    (sym-fun L::break break)
    ;; ..
    ))

;;------------------------------------------------------------------------------
(provide "p1eval")
