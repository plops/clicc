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
;;; Function : Generalized Variables
;;;
;;; $Revision: 1.14 $
;;; $Id: p1setf.lisp,v 1.14 1994/11/22 14:49:16 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;;   DEFSETF-Definitionen in lisp.lisp 

;;------------------------------------------------------------------------------
;; DEFSETF access-fn { update-fn [doc-string] ||
;;                     lambda-list (store-variable)
;;                     [[ {declaration}* || doc-string ]] {form}* }
;;------------------------------------------------------------------------------
(defun p1-defsetf (args)
  (let (access-fn update-fn-or-lambda-list)
    (tagbody
       (setq access-fn
             (if (atom args) (go NO-MATCH) (pop args)))
       (setq update-fn-or-lambda-list
             (if (atom args) (go NO-MATCH) (pop args)))
       (go end)
     NO-MATCH
       (clicc-error ILLEGAL_CALL "DEFSETF" "(ACCESS-FN &REST RESTARGS)")
     END)
    (when (not (symbolp access-fn))
      (clicc-error NOT_A_SYMBOL "ACCESS-FN" access-fn))

    (if (listp update-fn-or-lambda-list)

      ;; die komplexe Form von DEFSETF
      ;;-----------------------------
      (let ((lambda-list update-fn-or-lambda-list)
            store-variable-list   store-variable
            body)
        
        (clicc-message "Analyse DEFSETF      ~A" access-fn)
        
        (tagbody
           (setq store-variable-list
                 (if (atom args)
                     (go no-match)
                     (pop args)))
           (setq store-variable
                 (if (atom store-variable-list)
                     (go no-match)
                     (pop store-variable-list)))
           (when (not (null store-variable-list)) (go no-match))
           (setq body args)
           (go end)
         NO-MATCH
           (clicc-error ILLEGAL_CALL "DEFSETF"
                        "(ACCESS-FN LAMBDA-LIST (STORE-VARIABLE) &REST BODY)")
         END)

        (when (get-setf-method-def access-fn)
          (clicc-error "The SETF-method for ~A is declared twice" access-fn))
        (set-setf-method-def access-fn
                             (p1-gen-macro-fun access-fn
                                               (cons store-variable lambda-list)
                                               body)
                             :SIMPLE-SETF-METHOD))

      ;; die einfache Form von DEFSETF
      ;;-----------------------------
      (let ((update-fn  update-fn-or-lambda-list)
            (doc-string (if (atom args) "" (pop args))))
        (when (not (null args))
          (clicc-error ILLEGAL_CALL "DEFSETF"
                       "(ACCESS-FN UPDATE-FN &OPTIONAL DOC-STRING)"))

        (p1-top-level-form
;;;            	`(DEFSETF ,access-fn (&REST args) (newvalue)
;;;                    ,doc-string
;;;                  `(,',update-fn ,@args ,newvalue))
         (list 'L::DEFSETF access-fn '(L::&REST args) '(newvalue)
               doc-string
               `(L::CONS ',update-fn (L::APPEND args (L::LIST newvalue)))))
        (when *MODULE-COMPILER*
          (retract-syntax-export))))         ; don't include long form
    NIL))

;;------------------------------------------------------------------------------
;; Vordefinierte komplexe SETF-Methoden
;;------------------------------------------------------------------------------
(defun define-setf-method-apply (function_rest)
  (when (not (consp function_rest))
    (error ILLEGAL_CALL "SETF of APPLY" "(FUNCTION &REST ARGS)"))
  (let ((function (first function_rest))
        (args     (rest  function_rest))
         fun-name)
    (tagbody
       (when (atom function)                     (go no-match))
       (when (not (eq (pop function) 'L::FUNCTION)) (go no-match))
       (setq fun-name
             (if (atom function)
               (go no-match)
               (pop function)))
       (when (null function) (go end))
     NO-MATCH
       (clicc-error
        "SETF of APPLY is only defined for function args like #'symbol.")
     END)

    ;; (SETF (APPLY #'fn arg1 ... argN) newvalue) kann nur angewendet werden
    ;; auf Funktionen FN, bei denen in der UPDATE-FN newvalue NICHT der
    ;; letzte Parameter ist (siehe S. 127).
    ;; Dies ist in dieser Implementierung nur AREF !
    ;;----------------------------------------------
    (when (not (eq fun-name 'L::AREF))
      (clicc-error "APPLY of ~S not understood as a location for SETF."
                   fun-name))

    (multiple-value-bind (vars vals store-vars store-form access-form)
        (p1-get-setf-method (cons fun-name args))
      (setq store-form `(L::APPLY #',(first store-form) ,@(rest store-form)))
      (values vars vals store-vars store-form access-form))))

;;------------------------------------------------------------------------------
(defun define-setf-method-getf (place_indicator_rest)
  (let (place   indicator   default)
    (tagbody
       (setq place
             (if (atom place_indicator_rest)
               (go NO-MATCH)
               (pop place_indicator_rest)))
       (setq indicator
             (if (atom place_indicator_rest)
               (go NO-MATCH)
               (pop place_indicator_rest)))

       (setq default                    ; Wird nicht benutzt, ist ok.
             (if (atom place_indicator_rest)
               NIL
               (pop place_indicator_rest)))
       (when (null place_indicator_rest) (go END))
     NO-MATCH
       (clicc-error ILLEGAL_CALL "GETF" "(SYMBOL INDICATOR &OPTIONAL DEFAULT)")
     END)
    
    (multiple-value-bind (vars vals store-vars store-form access-form)
        (p1-get-setf-method place)
      (let ((indicator-var (gensym))
            (new-store-var (gensym)))
        (setq vars (append vars (list indicator-var)))
        (setq vals (append vals (list indicator)))
        (setq store-form
              `(L::LET ((,(first store-vars)
                      (RT::SET-PROP ,access-form
                                    ,indicator-var
                                    ,new-store-var)))
                ,store-form))
        (setq store-vars (list new-store-var)))
      (values vars vals store-vars store-form access-form))))

;;------------------------------------------------------------------------------
;; Liefert die SETF-Methode zu einer place-Form
;; Ergebnis    : 5 M-V Werte
;;               1. eine Liste von temporaeren Variablen, fuer jedes Argument
;;                  von 'form' genau eine.
;;               2. die Liste der Argumente von 'form'
;;               3. eine Liste von tempor"aren Variablen, an die die zu
;;                  speichernden Werte gebunden werden.
;;                  (Hier sind keine multiplen Werte zugelassen, daher besteht
;;                  die Liste nur aus einem Element)
;;               4. eine Form, die den Aufruf der Speicherfunktion
;;                  repraesentiert
;;               5. eine Form, die den Aufruf der Zugriffsfunktion
;;                  repraesentiert
;;
;; Bemerkung   : Um mit Aufrufen der Form (POP (THE value-type place)) bzw.
;;               (POP (macro-call arg1 ... argn)) fertig werden zu koennen,
;;               werden diese beiden F"alle am Anfang der Funktion behandelt.
;;------------------------------------------------------------------------------
(defun p1-get-setf-method (place)

  ;; place = (THE value-type form) ->
  ;; value-type wird an die access-form annotiert.
  ;;----------------------------------------------
  (when (and (consp place) (eq (first place) 'L::THE))
    (multiple-value-bind (value-type form)
        (parse-the (rest place))
      (multiple-value-bind (vars vals store-vars store-form access-form)
          (p1-get-setf-method form)
        (return-from p1-get-setf-method
          (values vars
                  vals
                  store-vars
                  store-form
                  `(L::THE ,value-type ,access-form))))))
  
  ;; place = (macro-call arg1 ... argn) -> Expandieren des Makroaufrufes
  ;;--------------------------------------------------------------------
  (multiple-value-bind (expanded-form expanded-p)
      (p1-macroexpand-1 place)
    (when expanded-p
      (return-from p1-get-setf-method (p1-get-setf-method expanded-form))))

  (if (atom place)
    (if (symbolp place)
      (let ((newsym (gensym)))
        (values () () `(,newsym) `(L::SETQ ,place ,newsym) place))
      (clicc-error NO_GET-SETF-METHOD place))
    (let ((name (first place))
          (args (rest  place))
           setf-method-def)
      (cond
        ((not (symbolp name))
         (clicc-error NO_LOC_SPEC name))
        ((setq setf-method-def (get-setf-method-def name))

         ;; Der Funktionsname muss sich auf eine globale Funktion beziehen.
         ;;----------------------------------------------------------------
         (let ((operator-def (get-operator-def name)))
           (when (eq (car operator-def) :LOCAL-FUN)
             (clicc-error NO_LOC_SPEC name)))

         (if (eq (car setf-method-def) :COMPLEX-SETF-METHOD)

           ;; Eine mittels DEFINE-SETF-METHOD definierte SETF-Expansion
           ;;----------------------------------------------------------
           (funcall (cdr setf-method-def) args)

           ;; Eine mittels DEFSETF definierte SETF-Expansion
           ;;-----------------------------------------------
           (do (vars vals store-var store-form access-form arg  new-args)
               ((null args)
                (setq new-args    (reverse new-args))
                (setq vars        (reverse vars))
                (setq vals        (reverse vals))
                (setq store-var   (gensym))
                (setq store-form  (p1-expand-user-macro
                                   (cdr setf-method-def)
                                   (list* name store-var new-args)))
                (setq access-form (cons name new-args))

                (values vars vals (list store-var) store-form access-form))
             (setq arg (pop args))
             (cond
               ((simple-constant-p arg)
                (push arg new-args))
               (t (let ((tmpvar (gensym)))
                    (push tmpvar vars)
                    (push arg    vals)
                    (push tmpvar new-args)))))))
        (t
         
         ;; Eine mittels (DEFUN (SETF ...) ...) definierte SETF-Expansion
         ;;--------------------------------------------------------------
         (do                            ;do wie oben bis auf store-form
             (vars vals store-var store-form access-form arg  new-args)
               ((null args)
                (setq new-args    (reverse new-args))
                (setq vars        (reverse vars))
                (setq vals        (reverse vals))
                (setq store-var   (gensym))
                (setq store-form  `(L::FUNCALL
                                    #'(L::SETF ,name) ,store-var ,@new-args))
                (setq access-form (cons name new-args))

                (values vars vals (list store-var) store-form access-form))
             (setq arg (pop args))
             (cond
               ((simple-constant-p arg)
                (push arg new-args))
               (t (let ((tmpvar (gensym)))
                    (push tmpvar vars)
                    (push arg    vals)
                    (push tmpvar new-args)))))
           )))))

;;------------------------------------------------------------------------------
;; SETF {place newvalue}*
;;------------------------------------------------------------------------------
(defun p1-setf (place_newvalue-list)
  (labels ((single-setf (place newvalue)
             (block single-setf
               (when (and (consp place) (eq (first place) 'L::THE))
                 (multiple-value-bind (value-type form) (parse-the (rest place))
                   (return-from single-setf
                     (single-setf form `(L::THE ,value-type ,newvalue))))))
  
             (multiple-value-bind (expanded-form expanded-p)
                 (p1-macroexpand-1 place)
               (if expanded-p
                   (single-setf expanded-form newvalue)
                   (multiple-value-bind
                         (vars vals store-vars store-form access-form)
                       (p1-get-setf-method place)
                     (declare (ignore access-form))
                     `(L::LET* ,(mapcar #'list (append vars store-vars)
                                 (append vals (list newvalue)))
                       ,store-form))))))
    
    (do (place newvalue (setf-list ()))
      
        ((p1-endp place_newvalue-list)
         `(L::PROGN ,@(reverse setf-list)))

      (setq place (pop place_newvalue-list))
      (setq newvalue (if (atom place_newvalue-list)
                         (clicc-error ODD_NARGS "SETF")
                         (pop place_newvalue-list)))
      (push (single-setf place newvalue) setf-list))))

;;------------------------------------------------------------------------------
;; PSETF {place newvalue}* 
;;------------------------------------------------------------------------------
(defun p1-psetf (place_newvalue-list)
  (do (place newvalue let*-var-list body)
      ((p1-endp place_newvalue-list)
       ;; >> Makroexpansion
       ;;    --------------
       `(L::LET* ,(reverse let*-var-list)
         ,@(reverse body)
         NIL))

    (setq place (pop place_newvalue-list))
    (setq newvalue
          (if (atom place_newvalue-list)
            (clicc-error ODD_NARGS "PSETF")
            (pop place_newvalue-list)))

    (multiple-value-bind (vars vals store-vars store-form access-form)
        (p1-get-setf-method place)
      (declare (ignore access-form))

      (mapc #'(lambda (var val)
                (push (list var val) let*-var-list))
            (append vars store-vars)
            (append vals (list newvalue)))
      (push store-form body))))

;;------------------------------------------------------------------------------
;; SHIFTF {place}+ newvalue 
;;------------------------------------------------------------------------------
(defun p1-shiftf (places_newvalue)
  (when (< (length places_newvalue) 2) (clicc-error TOO_FEW_ARGS "SHIFTF"))

  (do* (let*-var-list
        body
        (shifted-out   (gensym))
        (store-var_i-1  shifted-out)
        place)
       
       ((p1-endp (rest places_newvalue))
        (push (list store-var_i-1 (pop places_newvalue)) let*-var-list)

        ;; >> Makroexpansion
        ;;    --------------
        `(L::LET* ,(reverse let*-var-list)
          ,@(reverse body)
          ,shifted-out))

    (setq place (pop places_newvalue))
    (multiple-value-bind (vars vals store-vars store-form access-form)
        (p1-get-setf-method place)

      (mapc #'(lambda (var val)
                (push (list var val) let*-var-list))
            vars  vals)
      (push (list store-var_i-1 access-form) let*-var-list)
      (setq store-var_i-1 (first store-vars))
      (push store-form body))))

;;------------------------------------------------------------------------------
;; ROTATEF {place}*
;;------------------------------------------------------------------------------
(defun p1-rotatef (places)
  (cond
    ((null places)
     NIL)
    ((null (rest places))
     `(L::PROGN ,(first places) NIL))
    (t (do (let*-var-list
            body
            place
            (i 0 (1+ i))                ; Zaehler fuer den Index von place_i
            store-var_n-access-form_0
            (store-var_i-1 nil))
           
           ((p1-endp places)
            (setq let*-var-list (reverse let*-var-list))
            (setf (car store-var_n-access-form_0) store-var_i-1)

            ;; >> Makroexpansion
            ;;    --------------
            `(L::LET* ,let*-var-list
              ,@(reverse body)
              NIL))

         (setq place (pop places))
         (multiple-value-bind (vars vals store-vars store-form access-form)
             (p1-get-setf-method place)
           (mapc #'(lambda (var val)
                     (push (list var val) let*-var-list))
                 vars  vals)
           (let ((store-var_i-1-access-form_i (list store-var_i-1 access-form)))
             (push store-var_i-1-access-form_i let*-var-list)
             (when (zerop i)
               (setq store-var_n-access-form_0 store-var_i-1-access-form_i)))
           (setq store-var_i-1 (first store-vars))
           (push store-form body))))))


;;------------------------------------------------------------------------------
;; INCF place [delta]
;;------------------------------------------------------------------------------
(defun p1-incf (place_rest)
  (let (place delta)
    (tagbody
       (setq place
             (if (atom place_rest)
               (go no-match)
               (pop place_rest)))
       (setq delta
             (if (atom place_rest)
               1
               (pop place_rest)))
       (when (null place_rest) (go end))

     NO-MATCH
       (clicc-error ILLEGAL_CALL "INCF" "(PLACE &OPTIONAL (DELTA 1))")
     END)
    (multiple-value-bind (vars vals store-vars store-form access-form)
        (p1-get-setf-method place)

      ;; >> Makroexpansion
      ;;    --------------
      `(L::LET* ,(append (mapcar #'list vars vals)
               `((,(first store-vars) (L::+ ,access-form ,delta))))
        ,store-form))))

;;------------------------------------------------------------------------------
;; DECF place [delta] 
;;------------------------------------------------------------------------------
(defun p1-decf (place_rest)
  (let (place delta)
    (tagbody
       (setq place
             (if (atom place_rest)
               (go no-match)
               (pop place_rest)))
       (setq delta
             (if (atom place_rest)
               1
               (pop place_rest)))
       (when (null place_rest) (go end))

     NO-MATCH
       (clicc-error ILLEGAL_CALL "DECF" "(PLACE &OPTIONAL (DELTA 1))")
     END)
    (multiple-value-bind (vars vals store-vars store-form access-form)
        (p1-get-setf-method place)

      ;; >> Makroexpansion
      ;;    --------------
      `(L::LET* ,(append (mapcar #'list vars vals)
               `((,(first store-vars) (L::- ,access-form ,delta))))
        ,store-form))))

;;------------------------------------------------------------------------------
;; PUSH item place 
;;------------------------------------------------------------------------------
(defun p1-push (item_place_rest)
  (let (item place)
    (tagbody
       (setq item
             (if (atom item_place_rest)
               (go no-match)
               (pop item_place_rest)))
       (setq place
             (if (atom item_place_rest)
               (go no-match)
               (pop item_place_rest)))
       (when (null item_place_rest) (go end))
     NO-MATCH
       (clicc-error ILLEGAL_CALL "PUSH" "(ITEM PLACE)")
     END)
    
    (cond
      ((symbolp place)
       
       ;; Wenn PLACE ein Symbol ist, wird direkt eine Optimierung vorgenommen.
       ;;---------------------------------------------------------------------
       `(L::SETQ ,place (L::CONS ,item ,place)))

      (t (multiple-value-bind (vars vals store-vars store-form access-form)
             (p1-get-setf-method place)

           ;; >> Makroexpansion
           ;;------------------
           `(L::LET* ,(append (mapcar #'list vars vals)
                    `((,(first store-vars) (L::CONS ,item ,access-form))))
             ,store-form))))))

;;------------------------------------------------------------------------------
;; PUSHNEW item place &KEY :test :test-not :key
;;------------------------------------------------------------------------------
(defun p1-pushnew (item_place_rest)
  (let (item place rest)
    (tagbody
       (setq item
             (if (atom item_place_rest)
               (go no-match)
               (pop item_place_rest)))
       (setq place
             (if (atom item_place_rest)
               (go no-match)
               (pop item_place_rest)))
       (setq rest item_place_rest)
       (go end)
     NO-MATCH
       (clicc-error ILLEGAL_CALL
                    "PUSHNEW"
                    "(ITEM PLACE &KEY :TEST :TEST-NOT :KEY)")
     END)
    (cond
      ((symbolp place)
       
       ;; Wenn PLACE ein Symbol ist, wird direkt eine Optimierung vorgenommen.
       ;;---------------------------------------------------------------------
       `(L::SETQ ,place (L::ADJOIN ,item ,place ,@rest)))
      
      (t (multiple-value-bind (vars vals store-vars store-form access-form)
             (p1-get-setf-method place)

           ;; >> Makroexpansion
           ;;------------------
           `(L::LET* ,(append (mapcar #'list vars vals)
                    `((,(first store-vars)
                       (L::ADJOIN ,item ,access-form ,@rest))))
             ,store-form))))))

;;------------------------------------------------------------------------------
;; POP place
;;------------------------------------------------------------------------------
(defun p1-pop (place_rest)
  (let (place store-var)
    (when (or (atom place_rest)
              (rest place_rest))
      (clicc-error ILLEGAL_CALL "POP" "(PLACE)"))
    (setq place (car place_rest))
    (cond
      ((symbolp place)
       
       ;; Wenn PLACE ein Symbol ist, wird direkt eine Optimierung vorgenommen.
       ;; Man muesste sonst
       ;;    (LET* ((G768 X))
       ;;       (PROG1 (FIRST G768) (SETQ G768 (REST G768)) (SETQ X G768)))
       ;; vereinfachen koennen.
       ;;----------------------
       `(L::PROG1 (L::FIRST ,place)
         (L::SETQ ,place (L::REST ,place))))
      
      (t (multiple-value-bind (vars vals store-vars store-form access-form)
             (p1-get-setf-method place)
           (setq store-var (first store-vars))

           ;; >> Makroexpansion
           ;;------------------
           `(L::LET* ,(append (mapcar #'list vars vals)
                    `((,store-var ,access-form)))
             (L::PROG1 (L::FIRST ,store-var)
               (L::SETQ ,store-var (L::REST ,store-var))
               ,store-form)))))))

;;------------------------------------------------------------------------------
(provide "p1setf")
