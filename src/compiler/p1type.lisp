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
;;; Function : Type Specifiers
;;;
;;; $Revision: 1.31 $
;;; $Id: p1type.lisp,v 1.31 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; DEFTYPE name lambda-list { declaration || doc-string }* { form }*  [Macro]
;;------------------------------------------------------------------------------
(defun p1-deftype (name_lambda-list_body)
  (when (atom name_lambda-list_body)
    (clicc-error ILLEGAL_CALL "DEFTYPE" "(NAME (&REST ARGLIST) &BODY BODY)"))
  (let ((name             (first name_lambda-list_body))
        (lambda-list_body (rest  name_lambda-list_body))
        type-def)
    (unless (symbolp name) (clicc-error NO_NAME name "DEFTYPE"))
    (clicc-message "Analyse DEFTYPE      ~A" name)
    (setq type-def (get-type-def name))
    (ecase (car type-def)
      (:TYPE
       (clc-error "Type Specifier ~A declared twice" name))
      (:CLASS
       (clc-error "Type Specifier ~A already declared as a class" name))
      ((nil)

       ;; Bekanntgeben der Makrodefinition
       ;; --------------------------------
       (multiple-value-bind (lambda-list body) 
           (parse-type-expander lambda-list_body)
         (set-type-def name (p1-gen-macro-fun name lambda-list body)))))))

;;------------------------------------------------------------------------------
;; DEF-BUILT-IN name
;;       :TYPE-EXPANDER ( lambda-list { declaration || doc-string }* { form }* )
;;       :SUPERCLASSES ( { built-in-class-name }* )
;;------------------------------------------------------------------------------
(defun p1-def-built-in (args)
  (when (atom args)
    (clicc-error
     ILLEGAL_CALL "DEF-BUILT-IN"
     "NAME :TYPE-EXPANDER TYPE-EXPANDER :SUPERCLASSES SUPERCLASSES"))
  (apply #'do-def-built-in args))

(defun do-def-built-in (name &key type-expander superclasses order)
  (let ((built-in-def (get-built-in name)))
    (clicc-message "Analyse DEF-BUILT-IN ~A" name)
    (unless (symbolp name) (clicc-error NO_NAME name "DEF-BUILT-IN"))
    (ecase (car built-in-def)
      (:BUILT-IN
       (clicc-error "built-in class ~A declared twice" name))
      (:CLASS
       (clicc-error "Type Specifier ~A already declared as a class" name))
      (:TYPE
       (clicc-error "Type Specifier ~A already declared as a type" name))
      (:struct
       (clicc-error "Type Specifier ~A already declared as a structure" name))
      ((nil)
       (multiple-value-bind (lambda-list body) 
           (parse-type-expander type-expander)
         (setf type-expander (p1-gen-macro-fun name lambda-list body))
         (setf superclasses
          (mapcar #'(lambda (superclass-name)
                      (let ((superclass-def (get-class-entry superclass-name))
                            superclass)
                        (ecase (car superclass-def)
                          ((:TYPE :STRUCT)
                           (clicc-error 
                            "~S is not a legal superclass" 
                            superclass-name))
                          (:BUILT-IN
                           (setf superclass 
                             (cdr superclass-def)))
                          (:CLASS
                           (clicc-error
                            "There is already a user-defined class named ~A"
                            superclass-name))
                          ((nil)
                           (clicc-error
                            "The superclass ~A is not defined" 
                            superclass-name)))
                        superclass))
                  superclasses))

         ;; Bekanntgeben der Makrodefinition
         ;; --------------------------------
         (set-built-in name (make-instance 'built-in-class-def
                                           :symbol name
                                           :type-expander type-expander
                                           :super-list superclasses
                                           :slot-descr-list '()
                                           :order order)))))))

;;------------------------------------------------------------------------------
;; Parser fuer die Parameter eines DEFTYPE bzw. den expander eines DEF-BUILT-IN
;;------------------------------------------------------------------------------
(defun parse-type-expander (lambda-list_body)
  (when (atom lambda-list_body) (clicc-error ILLEGAL_L_EXPR))
  (let ((lambda-list (first lambda-list_body))
        (body (rest  lambda-list_body)))
    
    ;; Replace undefaultized optional parameter X by (X '*).
    ;;------------------------------------------------------
    (do ((l lambda-list (cdr l))
         (m nil (cons (car l) m)))
        ((null l))
      (when (member (car l) lambda-list-keywords)
        (unless (eq (car l) '&optional) (return nil))
        (setq m (cons '&optional m))
        (setq l (cdr l))
        (do ()
            ((or (null l) (member (car l) lambda-list-keywords)))
          (if (symbolp (car l))
              (setq m (cons (list (car l) ''*) m))
              (setq m (cons (car l) m)))
          (setq l (cdr l)))
        (setq lambda-list (nreconc m l))
        (return nil)))
    (values lambda-list body)))
    
;;------------------------------------------------------------------------------
;; Expandiert einen spezifizierten Typ in die kanonische Darstellung
;;------------------------------------------------------------------------------
(defun p1-type-expand (type &aux atomic-type)
  (loop
    (setq atomic-type nil)
    (when (atom type)
      (setq type (list type))
      (setq atomic-type t))
    (let* ((type-cons (get-type-def (first type)))
           (type-key  (first type-cons))
           (type-expander (rest type-cons)))
      (ecase type-key
        (:TYPE (setq type (p1-expand-user-macro type-expander type)))
        (:BUILT-IN (setq type 
                         (p1-expand-user-macro (?type-expander type-expander) 
                                               type)))
        (:CLASS (unless (eql 1 (length type))
                  (clc-error "illegal type specifier ~a" type))
                (return `(L::CLASS-INTERNAL ,(first type))))
        ((nil) (return (if atomic-type (first type) type)))))))

;;------------------------------------------------------------------------------
;; Prueft, ob ein Ausdruck ein Aufruf ist
;;------------------------------------------------------------------------------
(defun callp (form)
  (and (consp form) (not (eq (first form) 'QUOTE))))

;;------------------------------------------------------------------------------
(defun quote-form-p (form)
  (and (consp form) (eq (first form) 'QUOTE)))

;;------------------------------------------------------------------------------
;; TYPEP object type
;;
;; Die Expansionsfunktion fuer das Compiler-Macro zu typep.
;; Falls der Type-Specifier als Konstante angegeben ist, wird expandiert,
;; sonst wird keine Expansion vorgenommen.
;;
;; Z.B. (typep x 'keyword)    --> (keywordp x)
;; (typep x '(and t1 t2))     --> (and (typep x 't1) (typep x 't2))
;; (typep x '(or  t1 t2))     --> (or  (typep x 't1) (typep x 't2))
;; (typep x '(not t))         --> (not (typep x 't))
;; (typep x '(member f1 f2))  --> (or (eql x 'f1) (eql x 'f2))
;; (typep x '(satisfies fun)) --> (fun x)
;;------------------------------------------------------------------------------
(defun  p1-typep (form)
  (match-args
   form TYPEP (object type)
   (when (not (quote-form-p type))
     (return-from p1-typep form))
   (when (callp object)
     (let ((newsym (gensym)))
       (return-from p1-typep `(LET ((,newsym ,object)) (TYPEP ,newsym ,type)))))
   (let ((expanded-type (p1-type-expand (parse-quote (rest type)))))
     (cond
       ((eq expanded-type 'L::NIL) 'L::NIL)   ; Type NIL: immer FALSE
       ((eq expanded-type 'L::T)   'L::T)     ; Type T  : immer TRUE
       ((atom expanded-type)
        (clc-error "Illegal type specifier ~A" expanded-type)
        'L::NIL)
       (t (let ((type-specifier (first expanded-type))
                (args           (rest  expanded-type)))
            (case type-specifier
              (L::MEMBER
               `(L::OR ,@(mapcar #'(lambda (object-of-set)
                                  `(L::EQL ,object (L::QUOTE ,object-of-set)))
                       args)))
              (L::EQL
               (unless (eql 1 (length args))
                 (clc-error "Illegal type specifier ~A" expanded-type))
               `(L::EQL ,object (L::QUOTE ,(first args))))
              (L::NOT
               (unless (eql 1 (length args))
                 (clc-error "Illegal type specifier ~A" expanded-type))
               `(L::NOT (L::TYPEP ,object (L::QUOTE ,(first args)))))
              (L::AND
               `(L::AND ,@(mapcar 
                        #'(lambda (specified-type)
                            `(L::TYPEP ,object (L::QUOTE ,specified-type)))
                        args)))
              (L::OR
               `(L::OR ,@(mapcar 
                       #'(lambda (specified-type)
                           `(L::TYPEP ,object (L::QUOTE ,specified-type)))
                       args)))
              
              (L::SATISFIES
               (unless (eql 1 (length args))
                 (clc-error "Illegal type specifier ~A" expanded-type))
               `(,(first args) ,object))
              
              (L::ARRAY-INTERNAL
               (let ((type (first  args))
                     (dims (second args)))
                 (cond
                   ((and (listp dims) (eql 1 (length dims)))
                    (cond
                      ((or (eq type 'L::character)
                           (eq type 'L::standard-char))
                       (if (eq 'L::* (first dims))
                           `(L::stringp ,object)
                           `(and (L::stringp ,object)
                             (L::eql (L::length ,object)
                              ,(first dims)))))
                      ((eq type 'L::*)
                       (if (eq 'L::* (first dims))
                           `(L::vectorp ,object)
                           `(L::and (L::vectorp ,object)
                             (L::eql (L::length ,object) ,(first dims)))))
                      (t `(RT::CHECK-ARRAY
                           ,object (L::QUOTE ,type) (L::QUOTE ,dims)))))
                   ((and (eq type 'L::*) (eq dims 'L::*))
                    `(L::arrayp ,object))
                   (t `(RT::CHECK-ARRAY
                        ,object (L::QUOTE ,type) (L::QUOTE ,dims))))))

              (L::SIMPLE-ARRAY-INTERNAL
               (let ((type (first  args))
                     (dims (second args)))
                 (cond
                   ((and (listp dims) (eql 1 (length dims)))
                    (cond
                      ((or (eq type 'L::character)
                           (eq type 'L::standard-char))
                       (if (eq 'L::* (first dims))
                           `(L::simple-string-p ,object)
                           `(L::and (L::simple-string-p ,object)
                             (L::eql (L::length ,object) ,(first dims)))))
                      ((eq type 'L::T)
                       (if (eq 'L::* (first dims))
                           `(L::simple-vector-p ,object)
                           `(L::and (L::simple-vector-p ,object)
                             (L::eql (L::length ,object) ,(first dims)))))
                      ((eq type 'L::*)
                       (if (eq 'L::* (first dims))
                           `(RT::plain-vector-p ,object)
                           `(L::and (RT::plain-vector-p ,object)
                             (L::eql (L::length ,object) ,(first dims)))))
                      (t
                       `(RT::check-simple-array
                         ,object (L::QUOTE ,type) (L::QUOTE ,dims)))))
                   ((and (eq type 'L::*) (eq dims 'L::*))
                    `(rt::simple-array-p ,object))
                   (t `(RT::check-simple-array
                        ,object (L::QUOTE ,type) (L::QUOTE ,dims))))))
              
              (L::INTEGER-INTERNAL
               (let ((low  (first  args))
                     (high (second args)))
                 (cond
                   ((and (integerp low) (integerp high))
                    `(L::AND (L::INTEGERP ,object) (L::<= ,low ,object ,high)))
                   ((integerp low) `(RT::FIXNUM-LOW-P ,object ,low))
                   ((integerp high) `(RT::FIXNUM-LOW-P ,object ,high))
                   (T `(L::INTEGERP ,object)))))
              
              (L::CLASS-INTERNAL
               `(RT::TYPEP-CLASS ,object (L::find-class ',(first  args))))
              (t (clc-error "Illegal type specifier ~S" expanded-type)
                 'L::NIL))))))))

;;------------------------------------------------------------------------------
(provide "p1type")
