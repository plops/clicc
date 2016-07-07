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
;;; Funktion : Behandlung definierender Vorkommen generischer Funktionen und
;;;            Methoden, Finalisierung generischer Funktionen.
;;;
;;; $Revision: 1.95 $
;;; $Id: p1generic.lisp,v 1.95 1995/03/05 11:02:55 wg Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")  

;;------------------------------------------------------------------------------
;; p1-defgeneric: legt das entsprechende Zwischensprachkonstrukt an die
;;                folgenden Passes kuemmern sich dann um die Methodenauswahl
;;------------------------------------------------------------------------------
(defun p1-defgeneric (all-args)

  ;; hier erste Parameterpruefung
  ;;-----------------------------
  
  (multiple-value-bind 
        (gf-name gf-lambda-list method-combination-type gf-meth-descr-list)
      (parse-defgeneric-args all-args)
    
    ;; gebe Meldung aus
    ;;-----------------
    (clicc-message "Analyse DEFGENERIC   ~A" gf-name)
      
    (let* ((SETF-FUN (consp gf-name))
           gf                           ; Instanz von generic-fun
           (operator-def (if SETF-FUN
                             (get-setf-fun-def gf-name)
                             (get-operator-def gf-name)))
           (*CURRENT-FORM* gf-name))
      
      ;; ist gen. Funktion schon vorhanden, dann Fehler, sonst erzeuge neue
      ;;-------------------------------------------------------------------
      (case (car operator-def)
        ((nil)                          ; noch kein Eintrag vorhanden
         (setq gf (make-instance 'defined-generic-fun 
                                 :symbol gf-name
                                 :fun (make-instance
                                       'global-fun
                                       :par-spec (ll-par-spec gf-lambda-list)
                                       :symbol gf-name)))
         (if SETF-FUN
             (set-global-setf-fun-def gf-name :GENERIC-FUN gf)
             (set-generic-fun gf-name gf)))
        (:FORWARD                       ;( gen.) Funktion schon referenziert
         (setq gf (make-instance 'defined-generic-fun 
                                 :symbol gf-name
                                 :fun (make-instance
                                       'global-fun
                                       :par-spec (ll-par-spec gf-lambda-list)
                                       :symbol gf-name)))
         (setf (?value (cdr operator-def)) (?fun gf))
         (setf (car operator-def) :GENERIC-FUN)
         (setf (cdr operator-def) gf))
        (otherwise                      ; alle uebrigen Keys 
         (redef-op-error (car operator-def) gf-name)))
      
      ;; wende p1-defmethod auf die Methodendefinitionen an
      ;;---------------------------------------------------
      (dolist (meth-descr gf-meth-descr-list)
        (p1-defmethod `(,gf-name ,@meth-descr)))
      
      ;; fuelle Slots der Instanz
      ;;-------------------------
      (setf (?argument-precedence-order gf) gf-lambda-list) ;vorlaeufig!!
      (setf (?method-combination gf) method-combination-type)
      
      ;; Rueckgabewert=Instanz ... kein einheitlicher Rueckgabewert fuer def... 
      ;;------------------------------------------------------------------------
      gf)))

;;------------------------------------------------------------------------------
;; p1-defmethod: verarbeitet das Macro (defmethod...)
;;------------------------------------------------------------------------------
(defun p1-defmethod (all-args &key (dont-generate-local-funs nil))
  
  ;; hier erste Parameterpruefung
  ;;-----------------------------
  
  (multiple-value-bind
        (method-name method-qualifiers specialized-lambda-list doc-decl
                     method-body)
      (parse-defmethod-args all-args)
    
    (let* ((SETF-FUN (consp method-name))
           (method                      ; Instanz von method-def
            (make-instance 'method-def))
           gf                           ; Instanz der zugehoerigen gener. Fkt.
           (operator-def 
            (if SETF-FUN
                (get-setf-fun-def method-name)
                (get-operator-def method-name)))
           (lambda-list                 ; nichtspezialisierte Lambda-Liste
            (unspecialize-lambda-list specialized-lambda-list))
           extended-lambda-list         ; um next-method-list erweitert
           (is-complex-ll (member-if #'lambda-list-keyword-p lambda-list))
           (specializer-list            ; Liste der specializer
            (extract-specializers specialized-lambda-list))
           (specializer-pos             ; Position des ersten spez. Param.
            (if specializer-list
                (list
                 (if is-complex-ll
                     (position-if
                      #'consp specialized-lambda-list
                      :end (position-if #'lambda-list-keyword-p
                                        specialized-lambda-list))
                     (position-if #'consp specialized-lambda-list))
                 (if is-complex-ll
                     (position-if
                      #'consp specialized-lambda-list
                      :from-end T
                      :end (position-if #'lambda-list-keyword-p
                                        specialized-lambda-list))
                     (position-if #'consp specialized-lambda-list
                                  :from-end T)))
                nil))
           method-symbol                ; fuer den Aufruf von p1-defun
           method-fun                   ; f. d. von p1-defun erzeugte Funktion
           extended-method-body         ; um labels fuer c-n-m erweitert
           (*CURRENT-FORM* method-name)
           (dont-check-ll nil))         ; falls die gf hier erzeugt wurde

      ;; ermittle einen Namen fuer die Funktion der Methode
      ;;---------------------------------------------------
      (setf method-symbol
            (intern
             (apply #'concatenate
                    `(string
                      ,(string (if SETF-FUN 
                                   (intern-prefixed "SETF-"
                                                    (second method-name))
                                   method-name))
                      "--"
                      ,(if (first method-qualifiers)
                           (concatenate 'string
                                        (string (first method-qualifiers))
                                        "-")
                           "")
                      "METHOD( "
                      ,@(mapcar #'(lambda (string)
                                    (concatenate 'string string " "))
                         (mapcar #'string
                          (mapcar #'?symbol specializer-list)))
                      ")"))))

      ;; suche zugeh. gen. Funktion; falls diese nicht existiert erzeuge eine
      ;;---------------------------------------------------------------------
      (case (car operator-def)
        ((nil)
         (setq gf (make-instance 'defined-generic-fun
                                 :symbol method-name
                                 :fun (make-instance
                                       'global-fun
                                       :par-spec (ll-par-spec lambda-list)
                                       :symbol method-name)
                                 :argument-precedence-order
                                 (method-ll-to-gf-ll lambda-list)
                                 :specializer-pos specializer-pos))
         (if SETF-FUN
             (set-global-setf-fun-def method-name :GENERIC-FUN gf)
             (set-generic-fun method-name gf))
         (setf dont-check-ll t))
        (:GENERIC-FUN                   ;gefunden!
         (setq gf (cdr operator-def))
         (if (first (?specializer-pos gf))
           (when (and (first specializer-pos)
                      (> (first (?specializer-pos gf)) (first specializer-pos)))
             (setf (first (?specializer-pos gf)) (first specializer-pos)))
           (setf (first (?specializer-pos gf)) (first specializer-pos)))
         (if (second (?specializer-pos gf))
           (when (and (second specializer-pos)
                      (< (second (?specializer-pos gf))
                         (second specializer-pos)))
             (setf (second (?specializer-pos gf)) (second specializer-pos)))
           (setf (second (?specializer-pos gf)) (second specializer-pos))))
        (:FORWARD
         (setq gf (make-instance 'defined-generic-fun
                                 :symbol method-name
                                 :fun (make-instance
                                       'global-fun
                                       :par-spec (ll-par-spec lambda-list)
                                       :symbol method-name)
                                 :argument-precedence-order
                                 (method-ll-to-gf-ll lambda-list)
                                 :specializer-pos specializer-pos))
         (setf (?value (cdr operator-def)) (?fun gf))
         (setf (car operator-def) :GENERIC-FUN)
         (setf (cdr operator-def) gf)
         (setf dont-check-ll t))
        (otherwise
         (redef-op-error (car operator-def) method-name)))
      
      ;; gebe Meldung aus
      ;;-----------------
      (unless *SDF* (clicc-message "Analyse DEFMETHOD    ~A ~A" 
                                   method-name 
                                   (mapcar #'?symbol specializer-list)))
      
      ;; Parameteranalyse
      ;;-----------------
      (unless dont-check-ll
        (check-ll-congruence (?argument-precedence-order gf) lambda-list))
      (setf extended-lambda-list (add-required lambda-list 'next-method-list))
      (when is-complex-ll
        (setf extended-lambda-list (replace-keywords extended-lambda-list)))
      (setf specializer-list (append specializer-list
                                     (list (?class-def
                                            (cdr (get-class-entry 'L::T))))))
      
      ;; berechne erweiterten Methoden-Rumpf
      ;;------------------------------------
      (cond
          ((and dont-generate-local-funs is-complex-ll)
           (setf extended-method-body
                 `((labels 
                       ((method-body ,lambda-list ,doc-decl ,@method-body))
                     (apply #'method-body
                            ,@(ll2fp (replace-keywords lambda-list)))))))
          (dont-generate-local-funs
           (setf extended-method-body method-body))
          (is-complex-ll
           (setf extended-method-body
                 `((L::labels 
                       ((L::call-next-method ()
                          (L::if (L::null next-method-list)
                              (L::no-next-method)
                              (L::let* ((next-method
                                         (L::first next-method-list))
                                     (next-method-list
                                      (L::rest next-method-list)))
                                (L::apply next-method
                                         ,@(ll2fp extended-lambda-list)))))
                        (L::next-method-p ()
                          next-method-list)
                        (method-body ,lambda-list ,doc-decl ,@method-body))
                     (L::apply #'method-body
                            ,@(ll2fp (replace-keywords lambda-list)))))))
          (T
           (setf extended-method-body
                 `(,doc-decl
                   (L::labels 
                       ((L::call-next-method ()
                          (if (L::null next-method-list)
                              (L::no-next-method)
                              (L::let* ((next-method
                                         (L::first next-method-list))
                                     (next-method-list
                                      (L::rest next-method-list)))
                                (L::funcall next-method
                                         ,@(ll2fp extended-lambda-list)))))
                        (L::next-method-p ()
                          next-method-list))
                     ,@method-body)))))
      
      ;; erzeuge method-fun durch Aufruf von p1-defun
      ;;---------------------------------------------
      (let ((*SDF* T))                  ; System-Defined-Function
        (p1-defun `(,method-symbol ,extended-lambda-list
                    ,@extended-method-body)
                  (if (consp method-name) (second method-name) method-name)))
      (setq method-fun (cdr (get-operator-def method-symbol)))
      
      ;; fuelle Slots der Instanz von method-def
      ;;----------------------------------------
      (setf (?fun method) method-fun)
      (setf (?specializer-list method) specializer-list)
      (setf (?qualifier-list method) method-qualifiers)
      
      ;;trage Instanz in gf-Instanz ein
      ;;-------------------------------
      (setf (?method-list gf) (append (?method-list gf) (list method)))
      
      ;;Rueckgabewert=Instanz ... kein einheitlicher Rueckgabewert fuer def...
      ;;----------------------------------------------------------------------
      method
      )))

;;------------------------------------------------------------------------------
;; Parser Funktionen fuer die Parameter der macros defgeneric & defmethod
;;------------------------------------------------------------------------------
(defun parse-defgeneric-args (all-args)
  (multiple-value-bind (gf-name rest-args)
      (parse-named-lambda 'DEFGENERIC all-args)
    (let ((gf-lambda-list (first rest-args))  ; Lambda-Liste der gf
          (method-combination 'L:standard)    ; default
          (gf-meth-descr-list (empty-queue))) ; Liste von Methodenbeschreibungen
      (dolist (arg (rest rest-args))
        (case (first arg)
          ((:argument-precedence-order 
            declare 
            :documentation) 
           (clicc-warning "The generic-function-option ~A is not implemented ~
                          yet ~%and will be ignored." (first arg)))
          (:method-combination
           (unless (equalp (second arg) method-combination)
             (format t "~s~%" method-combination)
             (clicc-warning "Only standard-method-combination is implemented ~
                          yet ~%and will be used instead of ~s." (second arg))))
          ((:generic-function-class
            :method-class)
           (clicc-warning "The generic-function-option ~A is not a part of the ~
                         implemented language ~%and will be ignored."
                      (first arg)))
          (:method
           (add-q (rest arg) gf-meth-descr-list))
          (otherwise
           (clicc-warning "The ~A is not a valid generic-function-option ~%and ~
                       will be ignored." (first arg)))))
      (values
       gf-name
       gf-lambda-list
       method-combination
       (queue2list gf-meth-descr-list)
       ))))

(defun parse-defmethod-args (all-args)
  (multiple-value-bind (method-name rest-args)
      (parse-named-lambda 'DEFMETHOD all-args)
    (let (specialized-lambda-list
          doc-decl-forms
          ;;folgendens wird als Queue verwaltet - siehe clcmisc.lisp
          (method-qualifiers (empty-queue)))
      (do ((arg (first rest-args) (first r-args))
           (r-args (rest rest-args) (rest r-args)))
          ((listp arg)
           (setf specialized-lambda-list arg)
           (setf doc-decl-forms r-args))
        (add-q arg method-qualifiers))
      (multiple-value-bind (documentation declarations method-body)
          (p1-get-doc-string/decl/forms doc-decl-forms)
        (declare (ignore documentation))
        (values
         method-name
         (queue2list method-qualifiers)
         specialized-lambda-list
         declarations
         method-body)))))

;;------------------------------------------------------------------------------
;; unspecialize-lambda-list: spec.-lambda-list -> unspec.-lambda-list
;;------------------------------------------------------------------------------
(defun unspecialize-lambda-list (specialized-lambda-list 
                                 &optional (num-o-specs 0 ))
  (let ((parameter (first specialized-lambda-list)))
    (cond
      ((null parameter)
       NIL)
      ((consp parameter)
       (incf num-o-specs)
;;       (when (> num-o-specs 1)
;;         (clc-error
;;          "There is more than one specialized parameter,~%~
;;          all but the first will be ignored. "))
       `(,(first parameter) 
         ,@(unspecialize-lambda-list (rest specialized-lambda-list)
                                     num-o-specs)))
      ((lambda-list-keyword-p parameter)
       specialized-lambda-list)
      (T
       `(,parameter 
         ,@(unspecialize-lambda-list (rest specialized-lambda-list)))))))
  
;;------------------------------------------------------------------------------
;; extract-specializers: spez.-lambda-list -> specializer
;;------------------------------------------------------------------------------
(defun extract-specializers (specialized-lambda-list)
  (if (or (null specialized-lambda-list)
          (lambda-list-keyword-p (first specialized-lambda-list)))

      ;; alle required-parameter betrachtet
      ;;-----------------------------------
      NIL

      ;; betrachte required-parameter
      ;;-----------------------------
      (let ((parameter (first specialized-lambda-list))
            class-entry
            class)
        (if (consp parameter)
            
            ;; spezialisierter Parameter
            ;;--------------------------
            (if (consp (cdr parameter))
                (progn
                  (setf class-entry (get-class-entry (cadr parameter)))
                  (case (car class-entry)
                    ((:TYPE :STRUCT)
                     (clicc-warning 
                      "~S is not a legal specializer, using T instead"
                      (cadr parameter))
                     (setf class (?class-def (cdr (get-class-entry 'L::T)))))
                    (:CLASS
                     (setf class (?class-def (cdr class-entry))))
                    (:BUILT-IN
                     (setf class (cdr class-entry)))
                    (otherwise
                     (setf class (make-instance 'defined-class
                                                :symbol (cadr parameter)))
                     (set-forward-class-entry (cadr parameter) class)))
                  (cons class
                        (extract-specializers (rest specialized-lambda-list))))

                ;; doted-pairs sind keine gültigen Spezialisierer
                ;;-----------------------------------------------
                (clicc-error "~S is not a valid specialized-lambda-list."
                             specialized-lambda-list))
            
            ;; unspezialisierter Parameter
            ;;----------------------------
            (cons (?class-def (cdr (get-class-entry 'L::T)))
                  (extract-specializers (rest specialized-lambda-list)))))))

;;------------------------------------------------------------------------------
;; add-required: Erweiterung einer lambda-list um einen required Parameter
;;------------------------------------------------------------------------------
(defun add-required (lambda-list new-required)
  (let ((param (car lambda-list)))
    (cond
      ((null param)
       `(,new-required))
      ((and (atom param) (lambda-list-keyword-p param))
       `(,new-required ,@lambda-list))
      (t
       `(,param ,@(add-required (cdr lambda-list) new-required))))))

;;------------------------------------------------------------------------------
;; replace-keywords: Sammle alle l-l-keywords in einem Rest-Parameter
;;------------------------------------------------------------------------------
(defun replace-keywords (lambda-list)
  (let ((pos (position-if #'lambda-list-keyword-p lambda-list)))
    (if pos
        (append (subseq lambda-list 0 pos) `(L::&REST rest))
        lambda-list)))

;;------------------------------------------------------------------------------
;; ll2fp: (p1 .. pn [&REST rest]) --> (p1 .. pn [rest])
;;------------------------------------------------------------------------------
(defun ll2fp (lambda-list)
  (let ((param (first lambda-list)))
    (cond 
      ((null param)
       (if (null (rest lambda-list))
           nil
           `(,param ,@(ll2fp (rest lambda-list)))))
      ((lambda-list-keyword-p param)
       (if (eq param 'L::&REST)
           (ll2fp (rest lambda-list))
           (clicc-error "internal: LL2FP cannot handle ~S-parameter." param)))
      (T
       `(,param ,@(ll2fp (rest lambda-list)))))))

;;------------------------------------------------------------------------------
;; remove-aux: entfernt die auxiliary variables aus einer lambda list
;;------------------------------------------------------------------------------
(defun remove-aux (lambda-list)
  (let ((param (first lambda-list))
        (rest-params (rest lambda-list)))
    (cond 
      ((null param)
       (if (null rest-params)
           nil
           `(,param ,@(remove-aux rest-params))))
      ((eq param 'L::&AUX)
       nil)
      (T
       `(,param ,@(remove-aux rest-params))))))

;;------------------------------------------------------------------------------
;; method-ll-to-gf-ll: Konvertiere eine Methoden-Lambda-Liste in die einer
;;                     generischen Funktion
;;------------------------------------------------------------------------------
(defun method-ll-to-gf-ll (method-ll)
  (let ((state 'REQUIRED)
        (gf-ll (empty-queue)))
    (dolist (param method-ll)
      (case param
        (&OPTIONAL (add-q param gf-ll) (setf state 'OPTIONAL))
        (&REST (add-q param gf-ll) (setf state 'REST))
        (&KEY (add-q param gf-ll) (setf state 'KEY))
        (&AUX (return-from method-ll-to-gf-ll (queue2list gf-ll)))
        (otherwise
         (case state
           ((REQUIRED OPTIONAL)
            (add-q (if (atom param) param (first param)) gf-ll))
           (KEY nil)
           (otherwise (add-q param gf-ll))))))
    (queue2list gf-ll)))

;;------------------------------------------------------------------------------
;; check-ll-congruence: Prüfe die Kongruenz von Methoden-Lambda-Listen
;;                      (vgl. CLtL p.791f)
;;------------------------------------------------------------------------------
(defun check-ll-congruence (gf-ll method-ll)
  (multiple-value-bind
        (gf-num-o-req gf-num-o-opt gf-rest-present gf-keys
         gf-allow-other-keys)
      (scan-gf-ll gf-ll)
    (declare (ignore gf-allow-other-keys))
    (multiple-value-bind
          (method-num-o-req method-num-o-opt method-rest-present
           method-keys method-allow-other-keys)
        (scan-method-ll method-ll)
      (unless (= gf-num-o-req method-num-o-req)
        (clicc-error "The method has not the same number of required parameters~
                      as the generic-function."))
      (unless (= gf-num-o-opt method-num-o-opt)
        (clicc-error "The method has not the same number of optional parameters~
                      as the generic-function."))
      (when (or gf-rest-present gf-keys)
        (unless (or method-rest-present
                    (not (null method-keys)))
          (clicc-error
           "The method has to define either rest- or key-parameters.")))
      (when gf-keys
        (unless (or (atom gf-keys)
                    method-allow-other-keys
                    (and method-rest-present
                         (null method-keys))
                    (mapcar
                     #'(lambda (gf-key) (member gf-key method-keys))
                     gf-keys))
          (clicc-error "The method has to accept all keywords that are defined~
                        for the generic function."))))))
         
;;------------------------------------------------------------------------------
;; scan-gf-ll: Prüfung der Lambda-Liste einer generischen Funktion und
;;             Ermittlung der Werte zur Kongruenzprüfung der
;;             Methoden-Lambda-Listen
;;------------------------------------------------------------------------------
(defun scan-gf-ll (gf-ll)
  (let ((state 'REQUIRED)
        (num-o-req 0)
        (num-o-opt 0)
        (rest-present nil)
        (keys-present nil)
        (keys (empty-queue))
        (allow-other-keys nil))
    (dolist (param gf-ll)
      (case param
        (&OPTIONAL (setf state 'OPTIONAL))
        (&REST (setf rest-present T)
               (setf state 'REST))
        (&KEY (setf keys-present T)
              (setf state 'KEY))
        (&ALLOW-OTHER-KEYS (setf allow-other-keys T)
                           (setf state 'ALLOW-OTHER-KEYS))
        (&AUX
         (clicc-warning "The use of &AUX Parameters is not allowed in ~
                         generic function lambda lists and will be ignored.")
         (return))
        (otherwise
         (case state
           (REQUIRED (incf num-o-req))
           (OPTIONAL (incf num-o-opt))
           (KEY (add-q
                 (if (atom param)
                     (intern (symbol-name param) "KEYWORD")
                     (if (atom (first param))
                         (intern (symbol-name (first param)) "KEYWORD")
                         (first (first param))))
                 keys))))))
    (values
     num-o-req
     num-o-opt
     rest-present 
     (if (empty-queue-p keys) keys-present (queue2list keys))
     allow-other-keys)))

;;------------------------------------------------------------------------------
;; scan-method-ll: Prüfung der Lambda-Liste einer Methode und Ermittlung der
;;                 Werte zur Kongruenzprüfung der Methoden-Lambda-Liste
;;------------------------------------------------------------------------------
(defun scan-method-ll (method-ll)
  (let ((state 'REQUIRED)
        (num-o-req 0)
        (num-o-opt 0)
        (rest-present nil)
        (keys-present nil)
        (keys (empty-queue))
        (allow-other-keys nil))
    (dolist (param method-ll)
      (case param
        (&OPTIONAL (setf state 'OPTIONAL))
        (&REST (setf rest-present T)
               (setf state 'REST))
        (&KEY (setf keys-present T)
              (setf state 'KEY))
        (&ALLOW-OTHER-KEYS (setf allow-other-keys T)
                           (setf state 'ALLOW-OTHER-KEYS))
        (&AUX (return))
        (otherwise
         (case state
           (REQUIRED (incf num-o-req))
           (OPTIONAL (incf num-o-opt))
           (KEY (add-q
                 (if (atom param)
                     (intern (symbol-name param) "KEYWORD")
                     (if (atom (first param))
                         (intern (symbol-name (first param)) "KEYWORD")
                         (first (first param))))
                 keys))))))
    (values
     num-o-req
     num-o-opt
     rest-present 
     (if (empty-queue-p keys) keys-present (queue2list keys))
     allow-other-keys)))
                     
;;------------------------------------------------------------------------------
;; finalize-generic-fun(s): Methoden-Dispatch-Funktion erstellen, Parameter-
;;                          listen vergleichen, ...
;;------------------------------------------------------------------------------
(defun finalize-generic-funs ()
  (let ((message-flag nil))
    (dolist (a-fun (append
                    (?operators *GLOBAL-ENVIRONMENT*)
                    (?setf-functions *GLOBAL-ENVIRONMENT*)))
      (when (eq (cadr a-fun) :GENERIC-FUN)
        (unless message-flag 
          (setf message-flag T)
          (clicc-message "Finalizing generic functions ..."))
        (finalize-generic-fun (cddr a-fun))))))

(defun finalize-generic-fun (gf)
  (let* ((specializer '())
         (method-lists '())
         (cnm-lists '())
         (sub-cnm-lists '())
         (around  '())
         (before  '())
         (primary '())
         (after   '())
         (gf-fun '())
         sub-funs
         upper-method-lists
         lower-method-lists
         (method-params (?argument-precedence-order gf))
         (gf-params (replace-keywords method-params))
         (extended-gf-params (add-required gf-params 'next-method-list))
         (is-simple-ll (not (member-if #'lambda-list-keyword-p gf-params)))
         (specializer-pos (first (?specializer-pos gf)))
         (last-specializer-pos (second (?specializer-pos gf)))
         (gf-name (?symbol gf))
         (gf-symbol (if (consp gf-name)
                        (intern-prefixed "SETF-" (second gf-name)) gf-name))
         (*CURRENT-FORM* gf-name)
         (t-flag nil)
         (cache-methods nil)
         (no-multi-methods T)
         (method-cache-class (gensym))
         (method-cache-fun (gensym))
         (method-cache-nml (gensym)))
    (labels
        (;;---------------------------------------------------------------------
         ;; get-associated-m-i-classes: sucht zu einer Klasse diejenigen
         ;;                             direkten Unterklassen mit multipler
         ;;                             Vererbung
         ;;---------------------------------------------------------------------
         (get-associated-m-i-classes (class)
           (remove 2 (remove class (queue2list (?class-def-list *module*))
                             :test-not #'member :key #'?super-list)
                   :test #'> :key #'(lambda (class)
                                      (length (?super-list class)))))
           
         ;;---------------------------------------------------------------------
         ;; construct-cnm-list: Erzeugt die in der Dispatch-Fun benoetigt Liste
         ;;                     fuer den Aufruf von call-next-method
         ;;---------------------------------------------------------------------
         (construct-cnm-list (method-list)
           `(,(first method-list)
             ,@(mapcar #'?fun (rest (second method-list)))
             ,(second (assoc (first method-list) sub-funs :test #'eq))))
         
         ;;---------------------------------------------------------------------
         ;; construct-subfun-method-list: Erzeuge eine Liste der in der Subfun
         ;;                             aufzurufenden Methoden
         ;;                             (Klasse (Before-Methoden) (P...) (A...))
         ;;---------------------------------------------------------------------
         (construct-subfun-method-list (method-list)
           `(,(first method-list)       ;Klasse
             ,(third method-list)       ;Before-Methoden
             ,(first (fourth method-list)) ;erste Primary-Methode
             ,(fifth method-list)))     ;After-Methoden

         ;;---------------------------------------------------------------------
         ;; construct-subfun-cnm-list: Erzeugt die in der Subfun benoetigt Liste
         ;;                         fuer den Aufruf von call-next-method
         ;;---------------------------------------------------------------------
         (construct-subfun-cnm-list (method-list)
           `(,(first method-list)
             ,@(mapcar #'?fun (rest (fourth method-list)))))
         
         ;;---------------------------------------------------------------------
         ;; construct-subfun-fun: Erzeuge eine Subfun der gen. Fkt. welche fuer
         ;;                    den Aufruf der Before-, Primary- und After-
         ;;                    Methoden sorgt
         ;;---------------------------------------------------------------------
         (construct-subfun-fun (method-list)
             (let ((*SDF* T))
               `(,(first method-list)
                 ,(p1-defun
                   `(,(intern (apply #'concatenate
                               `(string
                                 ,(string gf-symbol)
                                 "--"
                                 "SUBFUN( "
                                 ,(string (gensym))
                                 " )")))
                     ,extended-gf-params
                     (progn
                       ,@(mapcar #'construct-subfun-method-app 
                                 (second method-list))
                       ,(if (fourth method-list)
                            `(multiple-value-prog1
                              ,(construct-subfun-method-app (third method-list))
                              ,@(mapcar #'construct-subfun-method-app 
                                 (fourth method-list)))
                            (construct-subfun-method-app
                             (third method-list)))))))))
                            
         ;;---------------------------------------------------------------------
         ;; construct-subfun-method-app: Erzeugt die Applikation einer Methode
         ;;                              in einer SubFun
         ;;---------------------------------------------------------------------
         (construct-subfun-method-app (method)
           (when method
             (construct-method-app
              (?symbol (?fun method))
              (rest (assoc (first (?specializer-list method)) sub-cnm-lists
                           :test #'eq)))))
         
         ;;---------------------------------------------------------------------
         ;; construct-method-app: Erzeugt die Applikation einer Methode
         ;;---------------------------------------------------------------------
         (construct-method-app (method-symbol nml)
           (if is-simple-ll
               `(,method-symbol
                 ,@(add-required gf-params
                    (if (null (first nml))
                        '()
                        `(list ,@(mapcar #'add-hq
                                  (mapcar #'?symbol nml))))))
               `(apply ,(add-hq method-symbol)
                 ,@(ll2fp (add-required gf-params
                           (if (null (first nml))
                               '()
                               `(list ,@(mapcar #'add-hq
                                         (mapcar #'?symbol nml)))))))))
         
         ;;---------------------------------------------------------------------
         ;; construct-method-list: Erzeuge die komplexe method-list einer Klasse
         ;;                        (Klasse (Around-Methoden) ...)
         ;;---------------------------------------------------------------------
         (construct-method-list (class)
           (let ((method-list
                  `(,class
                    ,(remove class around 
                      :test-not #'preceding-class-p :key #'ml-spec-pos)
                    ,(remove class before 
                      :test-not #'preceding-class-p :key #'ml-spec-pos)
                    ,(remove class primary 
                      :test-not #'preceding-class-p :key #'ml-spec-pos)
                    ,(remove class after 
                      :test-not #'preceding-class-p :key #'ml-spec-pos))))
             `(,(first method-list)
               ,(mapcar #'second (second method-list))
               ,(mapcar #'second (third method-list))
               ,(mapcar #'second (fourth method-list))
               ,(mapcar #'second (fifth method-list)))))
         
         ;;---------------------------------------------------------------------
         ;; construct-upper-method-list: Erzeugt ein Tripel
         ;;                         (Klasse Symbol-der-ersten-Around-Methode
         ;;                          Liste-fuer-next-method)
         ;;                         bzw. (Klasse Symbol-der-Subfun
         ;;                               Liste-fuer-next-method)
         ;;---------------------------------------------------------------------
         (construct-upper-method-list (method-list)
           (let ((class (first method-list))
                 (first-around (second method-list)))
             (if (null first-around)
                 `(,class
                   ,(?symbol (second (assoc class sub-funs :test #'eq)))
                   ,(rest (assoc class sub-cnm-lists :test #'eq)))
                 `(,class
                   ,(?symbol (?fun (first first-around)))
                   ,(rest (assoc class cnm-lists :test #'eq))))))
         
         ;;---------------------------------------------------------------------
         ;; construct-a-case: Erzeugt eine Liste von Listen (Klasse Aufruf)
         ;; fuer das 'typecase' der Dispatch-Funktion
         ;;---------------------------------------------------------------------
         (construct-a-case (upper-method-list)
           `(,(cond
                ((eq (?symbol (first upper-method-list)) 'L::T)
                 (setf t-flag t) 
                 'L::T)
                (T
                 (?symbol (first upper-method-list))))
             ,@(when (and cache-methods (not (or (built-in-class-def-p
                                                  (first upper-method-list))
                                                 (eq (?symbol
                                                      (first upper-method-list))
                                                     'L::T))))
                   `((L:setf ,method-cache-class
                      (L:class-of ,(nth specializer-pos method-params)))
                     (L:setf ,method-cache-fun
                      ,(add-hq (second upper-method-list)))
                     (L:setf ,method-cache-nml
                      (list ,@(mapcar #'add-hq
                                      (mapcar #'?symbol
                                              (third upper-method-list)))))))
             ,(construct-method-app (second upper-method-list)
               (third upper-method-list))))
          
         ;;---------------------------------------------------------------------
         ;; ml-spec-pos: liefert den specializer aus einer Methodenliste vom Typ
         ;; around, before, primary oder after 
         ;;---------------------------------------------------------------------
         (ml-spec-pos (list)
           (if specializer-pos
               (nth specializer-pos (car list))
               (?class-def (cdr (get-class-entry 'L::T)))))

         ;;---------------------------------------------------------------------
         ;; add-hq: Symbol -> #'Symbol
         ;;---------------------------------------------------------------------
         (add-hq (symbol)
           `#',symbol)

         ;;---------------------------------------------------------------------
         ;; devide-specializer: trenne eine Liste von Klassen in defined und
         ;;                     built-in
         ;;                     MV: Liste von definierten Klassen
         ;;                         Liste von built-in Klassen
         ;;---------------------------------------------------------------------
         (devide-specializer (classes)
           (let ((defined-classes nil)
                  (built-in-classes nil))
             (dolist (class classes)
               (cond
                 ((typep class 'built-in-class-def)
                  (push class built-in-classes))
                 ((typep class 'defined-class)
                  (push class defined-classes))
                 ((typep class 'class-def) ;vorlaeufig
                  (push class defined-classes))))
             (values defined-classes built-in-classes)))
         
         ;;---------------------------------------------------------------------
         ;; devide-methods: trenne eine Liste von Methoden entsprechend ihren
         ;;                 Spezialisierern in defined und built-in
         ;;                 MV: Liste von Methoden spez. definierten Klassen
         ;;                     Liste von Methoden spez. built-in Klassen
         ;;---------------------------------------------------------------------
         (devide-methods (methods)
           (let ((defined-classes-methods nil)
                  (built-in-classes-methods nil))
             (dolist (method methods)
               (cond
                 ((typep (ml-spec-pos method) 'built-in-class-def)
                  (push method built-in-classes-methods))
                 ((typep (ml-spec-pos method) 'defined-class)
                  (push method defined-classes-methods))
                 ((typep (ml-spec-pos method) 'class-def) ;vorlaeufig
                  (push method defined-classes-methods))))
             (values defined-classes-methods built-in-classes-methods)))
         
         ;;---------------------------------------------------------------------
         ;; construct-typecase : Erzeuge geschachtelte TYPECASE, die den
         ;;                      Dispatch ausfuehren
         ;;---------------------------------------------------------------------
         (construct-typecase (available-methods)

           ;; Berechnung der Methoden-Listen
           ;;-------------------------------
           (when specializer-pos
             (setf specializer (mapcar #'(lambda (method)
                                           (nth specializer-pos 
                                                (?specializer-list method)))
                                       available-methods))
             (setf specializer
                   (remove-duplicates
                    (append
                     (mapappend #'get-associated-m-i-classes specializer)
                     specializer)))
             (multiple-value-bind (ud-specs bi-specs)
                 (devide-specializer specializer)
               (setf bi-specs (sort bi-specs #'> :key #'?order))
               (setf ud-specs (sort ud-specs #'> :key #'?order))
               (setf specializer (append bi-specs ud-specs))))

           (if (or (null specializer-pos)
                   (= specializer-pos last-specializer-pos))
               
               ;; letzte typecase-Ebene erreicht
               ;;-------------------------------
               (progn
                 (setf method-lists '())
                 (setf cnm-lists '())
                 (setf sub-cnm-lists '())
                 (setf around  '())
                 (setf before  '())
                 (setf primary '())
                 (setf after   '())
                 (setf sub-funs '())
                 (setf upper-method-lists '())
                 (setf lower-method-lists '())
                 (dolist (method available-methods)
                   (case (car (?qualifier-list method))
                     (:around
                      (setf around `((,(?specializer-list method) ,method)
                                     ,@around)))
                     (:before
                      (setf before `((,(?specializer-list method) ,method)
                                     ,@before)))
                     (:after
                      (setf after `((,(?specializer-list method) ,method)
                                    ,@after)))
                     (otherwise
                      (setf primary 
                            `((,(?specializer-list method) ,method)
                              ,@primary)))))
                 
                 (labels ((?spec-order (method)
                            (?order (ml-spec-pos method)))
                          (class-precedence-sort (method-list)
                            (multiple-value-bind (ud-specs bi-specs)
                                (devide-methods method-list)
                              (setf bi-specs (sort bi-specs #'>
                                                   :key #'?spec-order))
                              (setf ud-specs (sort ud-specs #'>
                                                   :key #'?spec-order))
                              (append bi-specs ud-specs))))
                   (setf around (class-precedence-sort around))
                   (setf before (class-precedence-sort before))
                   (setf primary (class-precedence-sort primary))
                   (setf after (reverse (class-precedence-sort after))))
                 
                 (setf method-lists 
                       (mapcar #'construct-method-list specializer))
                 (setf lower-method-lists 
                       (mapcar #'construct-subfun-method-list method-lists))
                 (setf sub-cnm-lists
                       (mapcar #'construct-subfun-cnm-list method-lists))
                 
                 ;; Berechnung der Unterfunktionen
                 ;;-------------------------------
                 (setf sub-funs 
                       (mapcar #'construct-subfun-fun lower-method-lists))
                 (setf cnm-lists 
                       (mapcar #'construct-cnm-list method-lists))
                 (setf upper-method-lists
                       (mapcar #'construct-upper-method-list method-lists))
                 
                 
                 ;; Gibt es mehr als eine unspezialisierte Methode ?
                 ;;-------------------------------------------------
                 (when (and (null specializer-pos) (rest available-methods))
                   (clicc-error "Too many unspecialized methods in generic ~
                           function ~A." gf-name))
                 
                 ;; Berechnung der typecase
                 ;;------------------------
                 (if specializer-pos
                     (if cache-methods
                         `(typecase ,(nth specializer-pos method-params)
                           ,@(append 
                              (mapcar #'construct-a-case upper-method-lists)
                              (if t-flag
                                  nil
                                  `((T (CLICC-LISP::no-applicable-method 
                                        (quote ,gf-name)
                                        ,@(ll2fp gf-params)))))))
                         `(typecase ,(nth specializer-pos method-params)
                           ,@(append 
                              (mapcar #'construct-a-case upper-method-lists)
                              (if t-flag
                                  nil
                                  `((T (CLICC-LISP::no-applicable-method 
                                        (quote ,gf-name)
                                        ,@(ll2fp gf-params))))))))
                     `(,gf-params
                       ,(construct-method-app
                         (?symbol (?fun (first available-methods))) nil))))

               ;; eine weitere typecase-Ebene aufmachen
               ;;--------------------------------------
               (let ((reduced-methods available-methods)
                     (previous-specializer nil))

                 (setf t-flag (member 'L::T specializer :key #'?symbol))
                 (incf specializer-pos)
                 `(typecase ,(nth (- specializer-pos 1) method-params)
                   ,@(append
                      (mapcar
                       #'(lambda (a-specializer)
                           (when previous-specializer
                             (setf reduced-methods
                                   (remove
                                    previous-specializer reduced-methods
                                    :test #'restricted-preceding-class-p
                                    :key #'(lambda (method)
                                             (nth
                                              (- specializer-pos 1)
                                              (?specializer-list method))))))
                           (setf previous-specializer a-specializer)
                           `(,(?symbol a-specializer)
                             ,(construct-typecase
                               reduced-methods)))
                       specializer)
                      (if t-flag
                          nil
                          `((T (CLICC-LISP::no-applicable-method 
                                (quote ,gf-name)
                                ,@(ll2fp gf-params)))))))))))
         
      ;;------------------------------------------------------------------------
      ;; Rumpf von FINALIZE-GENERIC-FUN
      ;;------------------------------------------------------------------------
      (catch 'CLICC-ERROR

        ;; Existieren Multi-Methoden ?
        ;;----------------------------
        (setf no-multi-methods (eql specializer-pos last-specializer-pos))

        ;; Soll ein Methodencache angelegt werden ?
        ;;-----------------------------------------
        (when (and *OPTIMIZE*
                   (> (length (?method-list gf)) *METHOD-CACHE-LIMIT*)
                   no-multi-methods)
          (setf cache-methods t)
          (let ((*SDF* t))
            (p1-top-level-form `(L:defvar ,method-cache-class))
            (p1-top-level-form `(L:defvar ,method-cache-fun))
            (p1-top-level-form `(L:defvar ,method-cache-nml))))
        
        ;; Berechne die Dispatch-Funktion
        ;;-------------------------------
        (setf
         gf-fun
         (p1-named-lambda
          (?fun gf) gf-symbol
          (if (consp gf-name) (second gf-name) gf-name)
          (if specializer-pos
              (if cache-methods
                  `(,gf-params
                    (if (and (rt::instancep
                              ,(nth specializer-pos method-params))
                             (eq (L:class-of
                                  ,(nth specializer-pos method-params))
                                 ,method-cache-class))
                        (funcall ,method-cache-fun
                                 ,@(ll2fp
                                    (add-required gf-params
                                                  method-cache-nml)))
                        ,(construct-typecase (?method-list gf))))
                  `(,gf-params
                    ,(construct-typecase (?method-list gf))))
              `(,gf-params
               ,(construct-method-app
                 (?symbol (?fun (first (?method-list gf)))) nil)))))
        
        ;; hier Fuellen weiterer Slots der gen. Fkt
        ;;-----------------------------------------
        (setf (?fun gf) gf-fun)
        
        ;; hier evtl. Eintragen der gen. Fkt in die fun-list des aktuellen 
        ;; Moduls aber bis jetzt als einfache Funktion behandeln
        ;;------------------------------------------------------
        ;;(add-q gf (?fun-list *module*))
        (add-q gf-fun (?fun-list *module*))))))

;;------------------------------------------------------------------------------
;; benutzte Praedikate ueber Beziehungen im Klassenbaum
;;------------------------------------------------------------------------------
(defun direct-subclass-p (subclass superclass)
  (if (or (null subclass) (null superclass)) 
      nil
      (member superclass (?super-list subclass))))

(defun preceding-class-p (preceding-class class)
  (cond 
    ((or (null preceding-class) (null class))
     nil)
    ((eq (?symbol class) 'L::t)
     t)
    (t
     (let ((class-precedence-list (?class-precedence-list preceding-class)))
       (if (consp class-precedence-list)
           (member class class-precedence-list)
           (member class (?value class-precedence-list)))))))

(defun strictly-preceding-class-p (preceding-class class)
  (cond 
    ((or (null preceding-class) (null class))
     nil)
    ((eq (?symbol class) 'L::t)
     t)
    (t
     (let ((class-precedence-list (?class-precedence-list preceding-class)))
       (if (consp class-precedence-list)
           (member class (cdr class-precedence-list))
           (member class (cdr (?value class-precedence-list))))))))

(defun restricted-preceding-class-p (preceding-class class)
  (cond 
    ((or (null preceding-class) (null class))
     nil)
    ((eq (?symbol preceding-class) 'L::t)
     nil)
    (t
     (let ((class-precedence-list (?class-precedence-list preceding-class)))
       (if (consp class-precedence-list)
           (member class class-precedence-list)
           (member class (?value class-precedence-list)))))))

;;------------------------------------------------------------------------------
(provide "p1generic")








