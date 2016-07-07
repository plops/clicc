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
;;; Funktion : Einfache Verbesserungen an Zwischensprachknoten.
;;;            Sonderbehandlung spezieller funktionsaufrufe
;;;
;;; $Revision: 1.22 $
;;; $Id: simplifier.lisp,v 1.22 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "delete")
(require "titypes")

;;------------------------------------------------------------------------------
;; Funktionen, die in diesem Pass optimiert werden.
;;------------------------------------------------------------------------------
(p0-special-funs
 (?opti-pass "OPTI")
 L::eq
 L::eql
 L::null
 L::not
 L::=
 L::>
 L::<
 L::+
 L::-
 L::apply
 L::funcall
 L::set
 L::symbol-value)

;;------------------------------------------------------------------------------

(defmethod simplify-1form ((a-form form))
  a-form)

(defmethod simplify-1form ((a-setq-form setq-form))
  (let ((loc (?location a-setq-form)))
    (when (var-ref-p loc)
      (setq loc (?var loc))
      (if (and (local-static-p loc)
               (zerop (?read loc)))
          (progn
            (inc-stats 'setq-elis)
            (decf (?write loc))
            (?form a-setq-form))
          a-setq-form))))
             
(defmethod simplify-1form ((a-progn-form progn-form))
  (let ((form-list (mapcan #'(lambda (form)
                               ;; (g1 .. gi (PROGN f1 .. fn) gi+2 .. gm) -->
                               ;; (g1 .. gi f1 .. fn gi+2 .. gm)
                               (if (progn-form-p form)
                                   (?form-list form)
                                   (list form)))
                           (?form-list a-progn-form))))
    (cond
      ((null form-list)                 ; (PROGN) -> ()
       (inc-stats 'progn-elis) empty-list)
      ((null (rest form-list))          ; (PROGN form) -> form
       (inc-stats 'progn-elis) (first form-list))
      (t (setf (?form-list a-progn-form) form-list) a-progn-form))))

(defmethod simplify-1form ((an-if-form if-form))
  (let ((pred (?pred an-if-form))
        (then (?then an-if-form))
        (else (?else an-if-form)))
    (loop
     (if (and (app-p pred)
              (let ((form (?form pred)))
                (and (fun-p form)
                     (let ((name (?symbol form)))
                       (or (eq name 'L::not)
                           (eq name 'L::null))))))
         (progn
           (inc-stats 'if-not-elis)
           (psetf pred (first (?arg-list pred))
                  then else
                  else then))
         (return)))
    (setf (?pred an-if-form) pred
          (?then an-if-form) then
          (?else an-if-form) else)
    an-if-form))
    
(defmethod simplify-1form ((a-switch-form switch-form))
  (let ((key (?form a-switch-form)))
    (if (copy-is-eq-p key)
        (dolist (a-labeled-form (?case-list a-switch-form)
                 (?otherwise a-switch-form))
          (when (eq key (?value a-labeled-form))
            (return (?form a-labeled-form))))
        a-switch-form)))

(defmethod simplify-1form ((a-let*-form let*-form))
  (let ((body (?body a-let*-form))
        (var-list (?var-list a-let*-form))
        (init-list (?init-list a-let*-form)))
    (if var-list
        (progn
          (let ((var-queue (empty-queue))
                (init-queue (empty-queue)))
            (do (var)
                ((null var-list))
              (setq var (first var-list))
              (if (or (is-used var) (dynamic-p var))
                  (progn
                    (add-q (pop var-list) var-queue)
                    (add-q (pop init-list) init-queue))
                  (return)))
            (when var-list
              (inc-stats 'let-var-elis)
              (pop var-list)
              (setq body
                    (optimize-1form
                     (make-instance
                      'progn-form
                      :form-list (list (pop init-list)
                                       (if var-list
                                           (optimize-1form
                                            (make-instance
                                             'let*-form
                                             :var-list var-list
                                             :init-list init-list
                                             :body body
                                             :type (?type body)))
                                           body))
                      :type (?type body)))))
            (setq var-list (queue2list var-queue)
                  init-list (queue2list init-queue)))
          ;; Geschachtelte let*-forms werden zu einer zusammengefasst.
          ;;----------------------------------------------------------
          (when (let*-form-p body)
            (setq var-list (append var-list (?var-list body))
                  init-list (append init-list (?init-list body))
                  body (?body body)))
          (if var-list
              (progn
                (setf (?var-list a-let*-form) var-list
                      (?init-list a-let*-form) init-list
                      (?body a-let*-form) body
                      (?type a-let*-form) (?type body))
                a-let*-form)
              body))
        ;; let*-forms mit leerer var-list werden entfernt.
        ;;------------------------------------------------
        (progn
          (inc-stats 'let*-elis)
          body))))

(defmethod simplify-1form ((a-labels-form labels-form))
  (if (?fun-list a-labels-form)
      a-labels-form
      (progn
        (inc-stats 'labels-elis)
        (?body a-labels-form))))

(defmethod simplify-1form ((a-let/cc-form let/cc-form))
  (if (zerop (?read (?cont a-let/cc-form)))
      (progn
        (inc-stats 'let/cc-elis)
        (?body a-let/cc-form))
      a-let/cc-form))

(defmethod simplify-1form ((a-tagbody-form tagbody-form))
  ;; Entfernen von nicht benoetigten Sprungmarken und ggf. Zusammenfassen
  ;; von aufeinanderfolgenden tagged-forms.
  ;;---------------------------------------------------------------------
  (let ((tagged-form-queue (empty-queue)))
    (dolist (a-tagged-form (?tagged-form-list a-tagbody-form))
      (cond
        ((plusp (?used a-tagged-form))
         (add-q a-tagged-form tagged-form-queue))
        ;; Die erste tagged-form wird in first-form integriert.
        ;;-----------------------------------------------------
        ((empty-queue-p tagged-form-queue)
         (inc-stats 'tag-elis)
         (setf (?first-form a-tagbody-form)
               (optimize-1form
                (make-instance 'progn-form
                               :form-list (list (?first-form a-tagbody-form)
                                                (?form a-tagged-form))
                               :type (?type (?form a-tagged-form))))))
        ;; Die tagged-form wird in die zuletzt bearbeitete tagged-form
        ;; integriert.
        ;;------------------------------------------------------------
        (t
         (inc-stats 'tag-elis)
         (let ((last-tagged-form (last-q tagged-form-queue)))
           (setf (?form last-tagged-form)
                 (optimize-1form
                  (make-instance 'progn-form
                                 :form-list (list (?form last-tagged-form)
                                                  (?form a-tagged-form))
                                 :type (?type (?form a-tagged-form)))))))))
    (if (empty-queue-p tagged-form-queue)
        (progn
          (inc-stats 'tagbody-elis)
          (?first-form a-tagbody-form))
        (progn
          (setf (?tagged-form-list a-tagbody-form)
                (queue2list tagged-form-queue))
          a-tagbody-form))))
            
(defmethod simplify-1form ((a-mv-lambda mv-lambda))
  (if (eq (?body a-mv-lambda) empty-list)
      (progn
        (inc-stats 'mv-lambda-elis)
        (make-instance 'progn-form
                       :form-list (list (?arg a-mv-lambda)
                                        empty-list)
                       :type null-t))
      a-mv-lambda))

;;------------------------------------------------------------------------------
;; Sucht aus dem Teil einer Argumentliste, der an Keyword Parameter gebunden
;; wird, den Wert heraus heraus, der einem bestimmten Keyword Parameter
;; zugeordnet wird. Wenn das Keyword nicht vorkommt, wird der Defaultwert
;; geliefert. Wenn der Wert nicht bestimmt werden kann, da unevaluierte
;; Keywords vorkommen, dann wird :unknown geliefert.
;;------------------------------------------------------------------------------
(defun zs-get-key-value (key-args keyword default)
  (cond
    ((endp key-args) default)
    ((eq (first key-args) keyword) (second key-args))
    ((not (sym-p (first key-args))) :unknown)
    (T (zs-get-key-value (cddr key-args) keyword default))))

;;------------------------------------------------------------------------------
;; If one of the arguments of a call to EQL satisfies this test then it may
;; be replaced by a call to EQ.
;;------------------------------------------------------------------------------
(defun eql=eq (arg)
  (let ((type (?type arg)))
    (and (types-are-disjoined number-t type)
         (types-are-disjoined character-t type))))

;;------------------------------------------------------------------------------
;; (f arg1 ... argn) --> (f-opt arg1 ... argn)
;; z.B. (aref a i) --> (rt:vref a i)
;;------------------------------------------------------------------------------
(defun simp-when-n-args (app n f-opt)
  (cond
    ((= (length (?arg-list app)) n)
     (inc-stats 'to-n-arg-fun)
     (setf (?form app) f-opt)
     (simplify-1form app))
    (t app)))

;;------------------------------------------------------------------------------
;; (f ...) --> (f-opt ...),
;; wenn das Resultat des Anwendung nicht benötigt wird.
;; z.B. (progn (mapcar ..) ..) --> (progn (mapc ..) ..)
;;------------------------------------------------------------------------------
(defun simp-when-no-result (app f-opt)
  (cond
    ((not *result-used*)
     (inc-stats 'to-noresult-fun)
     (setf (?form app) f-opt)
     (simplify-1form app))
    (t app)))

;;------------------------------------------------------------------------------
;;  (f arg1 ... argn) --> (f-opt arg1 ... argn),
;;  wenn das n-te Argument den Typ cons hat.
;;  z.B. (car x) --> (%car x)
;;------------------------------------------------------------------------------
(defun simp-when-arg-n=cons (app n f-opt)
  (cond
    ((zs-subtypep (?type (nth n (?arg-list app))) cons-t)
     (inc-stats 'fun-to-%fun)
     (setf (?form app) f-opt)
     (simplify-1form app))
    (t app)))

;;------------------------------------------------------------------------------
;; (f arg1 ...) --> (f-opt arg1 ...),
;; wenn nicht alle Argumente vom Typ (or cons pathname string bitvector) sind.
;; z.B. (equal x 's) --> (eql x 's)
;;------------------------------------------------------------------------------
(defun simp-when-some-arg-not-cons/pathn/string/bitv (app f-opt)
  (labels ((equal=eql (arg)
             (let ((type (?type arg)))
               (and (types-are-disjoined cons-t type)
                    (types-are-disjoined non-string-vector-t type)
                    (types-are-disjoined string-t type)
                    (types-are-disjoined pathname-t type)))))
    (cond
      ((some #'equal=eql (?arg-list app))
       (inc-stats 'equal-to-eql)
       (setf (?form app) f-opt)
       (simplify-1form app))
      (t app))))

;;------------------------------------------------------------------------------
;; (f arg1 ...) --> (f-opt arg1 ...),
;; wenn nicht alle Argumente vom Typ (or number character) sind.
;; z.B. (eql x 's) --> (eq x 's)
;;------------------------------------------------------------------------------
(defun simp-when-some-arg-not-num/char (app f-opt)
  (cond
    ((some #'eql=eq (?arg-list app))
     (inc-stats 'eql-to-eq)
     (setf (?form app) f-opt)
     (simplify-1form app))
    (t app)))

;;------------------------------------------------------------------------------
;; (f arg1 .. argn :testkey g) --> (f-opt arg1 .. argn),
;; wenn nur das Test-Keyword mit dem geforderten Wert angegeben ist.
;; z.B. (assoc x a :test #'eq) --> (simple-assoc 'x a)
;;------------------------------------------------------------------------------
(defun simp-when-only-test=value (app
                                  key-args-position test-keyword value f-opt)
  (let ((key-args (nthcdr key-args-position (?arg-list app))))
    (cond
      ((and (eq test-keyword (first key-args))
            (eq value (second key-args))
            (null (cddr key-args)))
       (inc-stats 'only-test-optis)
       (setf (?form app) f-opt)
       (setf (?arg-list app) (subseq (?arg-list app) 0 key-args-position))
       (simplify-1form app))
      (t app))))

;;------------------------------------------------------------------------------
;; (f arg1 .. argn .. :testkey g ..) --> (f arg1 .. argn .. :testkey g-opt ..)
;; wenn die Testfunktion feststeht und angewandt auf das i-te Argument
;; optimiert werden kann.
;; z.B. (assoc 'x a :test #'eql) --> (assoc 'x a :test #'eq)
;;------------------------------------------------------------------------------
(defun simp-test-fun-when-not-testnot (app
                                       test-on-pos
                                       key-args-position
                                       test-keyword
                                       default
                                       test-not-keyword)
  (let* ((arg-list (?arg-list app))
         (key-args (nthcdr key-args-position arg-list))
         (test (zs-get-key-value key-args test-keyword default))
         (test-not (zs-get-key-value key-args test-not-keyword nil)))
    (cond
      ((and (null test-not)
            (fun-p test)
            (?simp-when-some-arg-not-num/char test)
            (eql=eq (nth test-on-pos arg-list)))
       (inc-stats 'test-fun-optis)
       (unless (eq key-args
                   (progn
                     (setf (getf key-args test-keyword)
                           (?simp-when-some-arg-not-num/char test))
                     key-args))
         (setf (?arg-list app)
               (append (subseq arg-list 0 key-args-position) key-args))))
      (t app))))

;;------------------------------------------------------------------------------
;; Spezielle Annotationen von Funktionen beruecksichtigen
;;------------------------------------------------------------------------------
(defmethod simplify-1form ((app app))
  (let ((fun (?form app)))
    (cond
      ((fun-p fun)
       (when (?simp-test-fun-when-not-testnot fun)
         (setq app (apply #'simp-test-fun-when-not-testnot
                          app (?simp-test-fun-when-not-testnot fun))))
       (cond
         ((?simp-when-n-args fun)
          (apply #'simp-when-n-args app (?simp-when-n-args fun)))

         ((?simp-when-no-result fun)
          (simp-when-no-result app (?simp-when-no-result fun)))

         ((?simp-when-arg-n=cons fun)
          (apply #'simp-when-arg-n=cons app (?simp-when-arg-n=cons fun)))

         ((?simp-when-some-arg-not-cons/pathn/string/bitv fun)
          (simp-when-some-arg-not-cons/pathn/string/bitv
           app (?simp-when-some-arg-not-cons/pathn/string/bitv fun)))

         ((?simp-when-some-arg-not-num/char fun)
          (simp-when-some-arg-not-num/char
           app (?simp-when-some-arg-not-num/char fun)))

         ((?simp-when-only-test=value fun)
          (apply #'simp-when-only-test=value
                 app (?simp-when-only-test=value fun)))
       
         ((and (special-sys-fun-p fun) (?opti-pass fun))
          (funcall (?opti-pass fun) app))

         (T app)))
      (T app))))

;;------------------------------------------------------------------------------
;; Funcall kann in der Zwischensprache direkt ausgedrueckt werden.
;;------------------------------------------------------------------------------
(defun opti-funcall (app)
  (let ((arg-list (?arg-list app)))
    (inc-stats 'funcall-elis)
    (setf (?form app) (first arg-list))
    (setf (?arg-list app) (rest arg-list)))
  (simplify-1form app))

;;------------------------------------------------------------------------------
;; Wenn das letzte Argument bekannt ist, kann apply direkt in der Zwischen-
;; sprache ausgedrueckt werden.
;;------------------------------------------------------------------------------
(defun opti-apply (app)
  (let* ((args (?arg-list app))
         (fun (pop args))
         (args-1 (butlast args))
         (last-arg (first (last args))))
    (cond
      ((or (null last-arg)
           (null-form-p last-arg))
       (inc-stats 'apply-elis)
       (setf (?form app) fun)
       (setf (?arg-list app) args-1)
       (simplify-1form app))
      ((and (structured-literal-p last-arg)
            (listp (?value last-arg)))
       (inc-stats 'apply-elis)
       (setf (?form app) fun)
       (setf (?arg-list app) (append args-1 (?value last-arg)))
       (simplify-1form app))
      ((and (app-p last-arg)
            (eq (?form last-arg) (get-global-fun 'L::list)))
       (inc-stats 'apply-elis)
       (setf (?form app) fun)
       (setf (?arg-list app) (append args-1 (?arg-list last-arg)))
       (simplify-1form app))
      (t app))))
    

;;------------------------------------------------------------------------------
(defun opti-eq (app)
  (let* ((arg-list (?arg-list app))
         (arg1 (first arg-list))
         (arg2 (second arg-list)))
    (cond

      ;; (eq <self-evaluating> <self-evaluating>)
      ;; wird ersetzt duch NIL oder T.
      ;;-----------------------------------------
      ((and (self-evaluating-p arg1)
            (self-evaluating-p arg2))
       (inc-stats 'eq-elis)
       (dec-used-slot arg1)
       (dec-used-slot arg2)
       (if (typecase arg1
             (null-form (null-form-p arg2))
             (character-form (and (character-form-p arg2)
                                  (eql (?value arg1) (?value arg2))))
             (int (and (int-p arg2)
                       (eql (?value arg1) (?value arg2))))
             (t (eq arg1 arg2)))
           
           (get-symbol-bind 'L::T)
           empty-list))

      ;; (eq nil x) -> (not x)
      ;;-----------------------
      ((null-form-p arg1)
       (inc-stats 'eq-to-not)
       (setf (?form app) (get-global-fun 'L::not))
       (setf (?arg-list app) (list arg2))
       (opti-not app))

      ;; (eq x nil) -> (not x)
      ;;-----------------------
      ((null-form-p arg2)
       (inc-stats 'eq-to-not)
       (setf (?form app) (get-global-fun 'L::not))
       (setf (?arg-list app) (list arg1))
       (opti-not app))
      
      (t app))))

;;------------------------------------------------------------------------------
(defun opti-eql (app)
  (let ((arg-list (?arg-list app)))
    (labels ((eql=eq (arg)
               (or (copy-is-eq-p arg)
                   (structured-literal-p arg))))

      ;; weiter mit EQ, wenn eines der Argumente eine Konstante
      ;; aber kein Float ist.
      ;;---------------------------------------------------------------
      (when (or (eql=eq (first arg-list)) (eql=eq (second arg-list)))
        (inc-stats 'eql-to-eq)
        (setf (?form app) (get-global-fun 'L::eq))
        (setq app (opti-eq app)))))
  app)

;;------------------------------------------------------------------------------
;; (null x) -> (not x)
;;
;; Das ist nur notwendig, da fur not weitreichendere Optimierungen vorgenommen
;; werden als fuer null.
;;------------------------------------------------------------------------------
(defun opti-null (app)
  (setf (?form app) (get-global-fun 'L::not))
  (opti-not app))

;;------------------------------------------------------------------------------
;; (not x) wird fuer konstante Werte ausgewertet.
;;------------------------------------------------------------------------------
(defun is-constant (x)
  (typecase x
    (defined-named-const (literal-p (?value x)))
    ((or imported-named-const sym literal class-def fun ) t)
    (t nil)))

(defun opti-not (app)
  (let ((arg (first (?arg-list app))))
    (if (is-constant arg)
        (progn
          (inc-stats 'not-elis)
          (dec-used-slot arg)
          (if (null-form-p arg)
              (get-symbol-bind 'L::T)
              empty-list))
        (let (var
              sym)
          (if (and (var-ref-p arg)
                   (dynamic-p (setq var (?var arg)))
                   (constant-value-p (setq sym (?sym var))))
              (progn
                (inc-stats 'not-elis)
                (dec-used-slot arg)
                (if (null-form-p (?constant-value sym))
                    (get-symbol-bind 'L::T)
                    empty-list))
              app)))))

;;------------------------------------------------------------------------------
;; = number &REST more-numbers 
;;------------------------------------------------------------------------------
(defun opti-= (app)
  (let ((arg-list (?arg-list app)))
    (case (length arg-list)
      (0 (get-symbol-bind 'L::T))
      (1 (let ((arg (first arg-list)))
           (if (zs-subtypep (?type arg) number-t)
               arg
               app)))
      (2 (setf (?form app) (get-global-fun 'L::=)) ; Hier wird spaeter eine noch
                                        ; zu schreibende Funktion =2 eingetragen
         (opti-=2 app))
      (otherwise app))))

(defun opti-=2 (app)
  ;; (= form {0 | 0.0}), (= {0 | 0.0} form) -> (ZEROP form)
  ;;-------------------------------------------------------
  (let* ((arg-list (?arg-list app))
         (arg1 (first  arg-list))
         (arg2 (second arg-list)))
    (cond
      ((and (num-p arg1)
            (zerop (?value arg1)))
       (inc-stats '=-to-zerop)
       (setf (?form app) (get-global-fun 'L::ZEROP))
       (setf (?arg-list app) (list arg2))
       app)
      ((and (num-p arg2)
            (zerop (?value arg2)))
       (inc-stats '=-to-zerop)
       (setf (?form app) (get-global-fun 'L::ZEROP))
       (setf (?arg-list app) (list arg1))
       app)
      (t app))))

;;------------------------------------------------------------------------------
;; < number &REST more-numbers 
;;------------------------------------------------------------------------------
(defun opti-< (app)
  (let ((arg-list (?arg-list app)))
    (cond
      ((= (length arg-list) 2)

       ;; (< {0 | 0.0} form) -> (PLUSP  form),
       ;; (< form {0 | 0.0}) -> (MINUSP form)
       ;;-------------------------------------
       (let ((arg1 (first  arg-list))
             (arg2 (second arg-list)))
         (cond
           ((and (num-p arg1)
                 (zerop (?value arg1)))
            (inc-stats '<-to-plusp)
            (setf (?form app) (get-global-fun 'L::PLUSP))
            (setf (?arg-list app) (list arg2))
            app)
           ((and (num-p arg2)
                 (zerop (?value arg2)))
            (inc-stats '>-to-minusp)
            (setf (?form app) (get-global-fun 'L::MINUSP))
            (setf (?arg-list app) (list arg1))
            app)
           (t app))))
      (t app))))

;;------------------------------------------------------------------------------
;; > number &REST more-numbers  
;;------------------------------------------------------------------------------
(defun opti-> (app)
  (let ((arg-list (?arg-list app)))
    (cond
      ((= (length arg-list) 2)

       ;; (> {0 | 0.0} form) -> (MINUSP form),
       ;; (> form {0 | 0.0}) -> (PLUSP  form)
       ;;-------------------------------------
       (let ((arg1 (first  arg-list))
             (arg2 (second arg-list)))
         (cond
           ((and (num-p arg1)
                 (zerop (?value arg1)))
            (inc-stats '>-to-minusp)
            (setf (?form app) (get-global-fun 'L::MINUSP))
            (setf (?arg-list app) (list arg2))
            app)
           ((and (num-p arg2)
                 (zerop (?value arg2)))
            (inc-stats '<-to-plusp)
            (setf (?form app) (get-global-fun 'L::PLUSP))
            (setf (?arg-list app) (list arg1))
            app)
           (t app))))
      (t app))))

;;------------------------------------------------------------------------------
;; + &REST numbers
;;------------------------------------------------------------------------------
(defun opti-+ (app)
  (let ((arg-list (?arg-list app))
        (arg-queue (empty-queue))
        (sum 0)
        (nums 0))
    (dolist (arg arg-list)
      (if (num-p arg)
          (progn
            (incf nums)
            (setq sum (+ sum (?value arg))))
          (add-q arg arg-queue)))
    (unless (zerop sum)
      (add-q (if (integerp sum)
                 (make-instance 'int :value sum :type fixnum-t)
                 (make-instance 'float-form :value sum :type float-t))
             arg-queue))
    (setq arg-list (queue2list arg-queue))
    (setf (?arg-list app) arg-list)
    (case (length arg-list)
      (0 (inc-stats '+-evals) (make-instance 'int :value 0 :type fixnum-t))
      (1 (let ((arg (first arg-list)))
           (if (zs-subtypep (?type arg) number-t)
               (progn
                 (inc-stats '+-evals) 
                 arg)
               (progn
                 (when (> nums 1) (inc-stats '+-evals))
                 (setf (?arg-list app) (list arg))
                 app))))
      (2 (when (> nums 1) (inc-stats '+-evals))
         ;; (+ form 1), (+ 1 form) -> (1+ form)
         ;;------------------------------------
         (let ((arg1 (first  arg-list))
               (arg2 (second arg-list)))
           (cond
             ((and (int-p arg1) (eql (?value arg1) 1))
              (inc-stats '+-to-1+)
              (setf (?form app) (get-global-fun 'L::1+))
              (setf (?arg-list app) (list arg2))
              app)
             ((and (int-p arg2) (eql (?value arg2) 1))
              (inc-stats '+-to-1+)
              (setf (?form app) (get-global-fun 'L::1+))
              (setf (?arg-list app) (list arg1))
              app)
             (t app))))
      (otherwise (when (> nums 1) (inc-stats '+-evals))
                 app))))

;;------------------------------------------------------------------------------
;; - number &REST numbers   
;;------------------------------------------------------------------------------
(defun opti-- (app)
  (let ((arg-list (?arg-list app)))
    (cond
      ((= (length arg-list) 2)

       ;; (- form 1) -> (1- form)
       ;;------------------------
       (let ((arg1 (first  arg-list))
             (arg2 (second arg-list)))
         (cond
           ((and (int-p arg2) (eql (?value arg2) 1))
            (inc-stats '--to-1-)
            (setf (?form app) (get-global-fun 'L::1-))
            (setf (?arg-list app) (list arg1))
            app)
           (t app))))
      (t app))))


;;------------------------------------------------------------------------------
;; Ueberfuehre (SET symbol form) in (SETQ zugeh. dynamic form).
;;------------------------------------------------------------------------------
(defun opti-set (app)
  (let* ((arg-list (?arg-list app))
         (first-arg (first arg-list))
         (second-arg (second arg-list)))
    (if (sym-p first-arg)
        (progn
          (inc-stats 'set-elis)
          (make-instance 'setq-form 
                         :location (make-instance 'var-ref 
                                                  :var (get-global-dynamic 
                                                        (?symbol first-arg))
                                                  :type top-t)
                         :form second-arg
                         :type (?type second-arg)))
        app)))

;;------------------------------------------------------------------------------
;; Ueberfuehre (SYMBOL-VALUE symbol) in eine Referenz auf die zugehoergige
;; dynamische Variable.
;;------------------------------------------------------------------------------
(defun opti-symbol-value (app)
  (let ((first-arg (first (?arg-list app))))
    (if (sym-p first-arg)
        (progn
          (inc-stats 'sym-val-elis)
          (make-instance 'var-ref
                         :var (get-global-dynamic (?symbol first-arg))
                         :type top-t))
        app)))

;;------------------------------------------------------------------------------
(provide "simplifier")
