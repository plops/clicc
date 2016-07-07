;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Inhalt   : Die generische Funktion simplify-1form nimmt einfache
;;;            Verbesserungen an einem Zwischensprachknoten vor. Dabei werden
;;;            insbesondere Optimierungen fuer einige Funktionen vorgenommen.
;;;
;;; $Revision: 1.15 $
;;; $Log: simplifier.lisp,v $
;;; Revision 1.15  1993/12/03  12:48:20  jh
;;; Fehler in opti-equal behoben.
;;;
;;; Revision 1.14  1993/11/26  12:24:05  jh
;;; equal wird jetzt moeglichst durch eql ersetzt.
;;;
;;; Revision 1.13  1993/11/15  12:17:31  jh
;;; opti-assoc korrigiert.
;;;
;;; Revision 1.12  1993/10/22  13:48:53  jh
;;; opti-assoc weiter verbessert und opti-+ korrigiert.
;;;
;;; Revision 1.11  1993/10/20  15:43:02  jh
;;; Aufrufe der Funktion assoc werden, wenn moeglich, in Aufrufe von simple-
;;; assoc umgewandelt. Aufrufe der Funktion + werden bei konstanten Argumenten
;;; ausgewertet.
;;;
;;; Revision 1.10  1993/10/05  16:26:52  jh
;;; Fehler in opti-apply und opti-not behoben.
;;;
;;; Revision 1.9  1993/09/21  15:03:28  jh
;;; dec-used-slot eingebaut.
;;;
;;; Revision 1.8  1993/09/20  14:20:24  jh
;;; opti-aref, opti-apply und simplify-1form(setq-form) eingebaut.
;;; simplify-1form(let*-form) erweitert.
;;;
;;; Revision 1.7  1993/09/12  16:59:05  kl
;;; Fehler behoben. (empty-list) -> empty-list
;;;
;;; Revision 1.6  1993/08/31  16:45:47  jh
;;; Fehler in der Statistik behoben.
;;;
;;; Revision 1.5  1993/08/30  14:04:50  jh
;;; opti-not eingebaut und Fehler in opti-set beseitigt.
;;;
;;; Revision 1.4  1993/08/26  14:48:00  jh
;;; Statistik erweitert.
;;;
;;; Revision 1.3  1993/08/25  14:49:54  jh
;;; pass2 und P2 in opti-pass bzw. OPTI umbenannt und Optimierungen fuer
;;; mapcar, maplist, mapcan und mapcon eingebaut.
;;;
;;; Revision 1.2  1993/08/19  10:37:32  hk
;;; Optimierung von (setf aref) eingebaut.
;;;
;;; Revision 1.1  1993/06/30  15:23:15  jh
;;; Initial revision
;;;
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
 L::equal
 L::null
 L::not
 L::=
 L::>
 L::<
 L::+
 L::-
 L::aref
 (L::setf L::aref)
 L::apply
 L::funcall
 L::set
 L::symbol-value
 L::assoc
 L::mapcar
 L::maplist
 L::mapcan
 L::mapcon)

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
                (and (special-sys-fun-p form)
                     (eq (?symbol form) 'L::not))))
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

(defmethod simplify-1form ((an-app app))
  (let ((fun (?form an-app)))
    (if (and (special-sys-fun-p fun) (?opti-pass fun))
        (funcall (?opti-pass fun) an-app)
        an-app)))

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
;; Wenn der Typ eines der beiden Argumente eines equal-Aufrufs die Typen CONS,
;; PATHNAME und VECTOR (hier wuerde eigentlich STRING und BIT-VECTOR reichen,
;; der verwendete Typverband enthaelt den Typ BIT-VECTOR nicht) oder deren
;; Untertypen nicht enthaelt, kann equal durch eql ersetzt werden.
;;------------------------------------------------------------------------------
(defun opti-equal (app)
  (let ((arg-list (?arg-list app)))
    (labels ((equal=eql (arg)
               (let ((type (?type arg)))
                 (and (types-are-disjoined cons-t type)
                      (types-are-disjoined vector-t type)
                      (types-are-disjoined pathname-t type)))))
      (when (or (equal=eql (first arg-list)) (equal=eql (second arg-list)))
        (inc-stats 'equal-to-eql)
        (setf (?form app) (get-global-fun 'L::eql))
        (setq app (opti-eql app)))))
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
    (named-const (literal-p (?value x)))
    ((or sym literal class-def fun ) t)
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
;; (aref array index) --> (vref array index)
;;------------------------------------------------------------------------------
(defun opti-aref (app)
  (cond
    ((= (length (?arg-list app)) 2)
     (inc-stats 'aref-to-vref)
     (setf (?form app) (get-global-fun 'RT::vref))
     app)
    (t app)))

;;------------------------------------------------------------------------------
;; (setf (aref array index) v) --> (setf (rt:vref array index) v)
;;------------------------------------------------------------------------------
(defun opti-set-aref (app)
  (cond
    ((= (length (?arg-list app)) 3)
     (inc-stats 'aref-to-vref)
     (setf (?form app) (cdr (get-global-setf-fun-def '(setf RT::vref))))
     app)
    (t app)))

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
;; Wird bei einem Aufruf von assoc als Testfunktion eq und als Key-Funktion car
;; verwendet kann statt assoc simple-assoc verwendet werden.
;; Ist der Typ FLOAT nicht im Typen des ersten Arguments enthalten, wird eql
;; durch eq ersetzt.
;;------------------------------------------------------------------------------

(defun opti-assoc (app)
  (let* ((arg-list (?arg-list app))
         (item (first arg-list))
         (a-list (second arg-list))
         (key-args (rest (rest arg-list)))
         (eql-fun (get-global-fun 'L::eql))
         (eq-fun (get-global-fun 'L::eq))
         (key-key (get-symbol-bind 'keyword::key))
         (test-key (get-symbol-bind 'keyword::test))
         (test-not-key (get-symbol-bind 'keyword::test-not))
         (key nil)
         (test nil)
         (test-not nil)
         (unknown-or-multiple-keys nil))

    (do* ((keyword (pop key-args) (pop key-args))
          (key-arg (car key-args) (car key-args)))
         ((null key-args))
      (cond ((eq keyword key-key)
             (if key
                 (setq unknown-or-multiple-keys T)
                 (setq key key-arg)))
            ((eq keyword test-key)
             (if test
                 (setq unknown-or-multiple-keys T)
                 (setq test key-arg)))
            ((eq keyword test-not-key)
             (if test-not
                 (setq unknown-or-multiple-keys T)
                 (setq test-not key-arg)))
            (T (setq unknown-or-multiple-keys T)))
      (setq key-args (cdr key-args)))

    ;; Wenn kein Test angegeben ist, wird eql als default eingesetzt.
    ;;---------------------------------------------------------------
    (unless (or test test-not)
      (setq test eql-fun))

    ;; Wenn FLOAT nicht im Typen des ersten Arguments enthalten ist, wird eql
    ;; durch eq ersetzt.
    ;;-----------------------------------------------------------------------
    (when (and (eq test eql-fun) (not (zs-subtypep float-t (?type item))))
      (inc-stats 'eql-to-eq)
      (setq test eq-fun))

    (when (eq test eq-fun)
      (if (and (null key)
               (null unknown-or-multiple-keys))
          (progn
            (inc-stats 'assoc-optis)
            (setf (?form app) (get-global-fun 'rt::simple-assoc)
                  (?arg-list app) (list item a-list)))
          (let ((test-pos (position test-key arg-list)))
            (if test-pos
                (setf (nth (1+ test-pos) arg-list) eq-fun)
                (setf (?arg-list app) (append arg-list
                                              (list test-key eq-fun))))))))
  app)

;;------------------------------------------------------------------------------
;; Wenn das Ergebnis der Applikation nicht benoetigt wird, werden statt
;; mapcar, maplist, mapcan und mapcon mapc und mapl verwendet.
;;------------------------------------------------------------------------------

(defun opti-mapcar (app)
  (unless *result-used*
    (inc-stats 'mapcar-mapc)
    (setf (?form app) (get-global-fun 'L::mapc)))
  app)

(defun opti-maplist (app)
  (unless *result-used*
    (inc-stats 'maplist-mapl)
    (setf (?form app) (get-global-fun 'L::mapl)))
  app)

(defun opti-mapcan (app)
  (unless *result-used*
    (inc-stats 'mapcan-mapc)
    (setf (?form app) (get-global-fun 'L::mapc)))
  app)

(defun opti-mapcon (app)
  (unless *result-used*
    (inc-stats 'mapcon-mapl)
    (setf (?form app) (get-global-fun 'L::mapl)))
  app)

;;------------------------------------------------------------------------------
(provide "simplifier")
