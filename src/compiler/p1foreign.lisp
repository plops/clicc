;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Foreign Functions
;;;
;;; $Revision: 1.24 $
;;; $Log: p1foreign.lisp,v $
;;; Revision 1.24  1993/11/03  11:40:19  pm
;;; Inkonsistenzen in den Symbolnamen behoben.
;;;
;;; Revision 1.23  1993/08/24  11:20:29  pm
;;; Erweiterungen um C-Pointer
;;;
;;; Revision 1.22  1993/07/26  16:45:56  pm
;;; Erweiterungen um C-Strukturen
;;;
;;; Revision 1.21  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.20  1993/06/10  11:02:38  pm
;;; Quelltext bereinigt
;;;
;;; Revision 1.19  1993/05/31  17:06:02  pm
;;; Abarbeiten von call-ins eingebaut
;;;
;;; Revision 1.18  1993/05/23  17:52:44  pm
;;; p1-def-c-type um das Argument package erweitert
;;;
;;; Revision 1.17  1993/05/21  13:58:02  pm
;;; c-int in int umbenannt
;;;
;;; Revision 1.16  1993/05/12  08:36:57  pm
;;; packages verstanden und ueberarbeitet
;;;
;;; Revision 1.15  1993/04/23  09:42:35  pm
;;; Aufruf von p1-foreign-fun-call optimiert
;;;
;;; Revision 1.14  1993/04/08  12:52:32  pm
;;; Tippfehler beseitigt
;;;
;;; Revision 1.13  1993/04/08  09:16:52  pm
;;; Angefangen aufs C-Typ-System umzustellen
;;; p1-call-foreign-fun in p1-foreign-fun-call umbenannt
;;;
;;; Revision 1.12  1993/03/18  07:40:57  ft
;;; Tippfehler beseitigt.
;;;
;;; Revision 1.11  1993/03/17  14:32:11  pm
;;; struct eingebaut
;;;
;;; Revision 1.10  1993/02/18  10:22:01  kl
;;; Fehler mit den im Package FFI unbekannten Funktionen export und
;;; in-package behoben.
;;;
;;; Revision 1.9  1993/02/17  16:41:37  hk
;;; Package FFI soll KEINE anderen Packages usen, auch nicht Lisp.
;;;
;;; Revision 1.8  1993/02/16  16:58:24  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.7  1992/12/01  15:11:11  pm
;;; c-char* eingebaut
;;;
;;; Revision 1.6  1992/11/10  10:24:00  pm
;;; Fluechtigkeitsfehler behoben
;;;
;;; Revision 1.5  1992/11/05  10:52:02  pm
;;; Ueberarbeitet
;;;
;;; Revision 1.4  1992/11/04  12:41:28  pm
;;; p1-call-foreign-fun
;;;
;;; Revision 1.3  1992/10/19  14:17:44  pm
;;; kleinere Aenderungen
;;;
;;; Revision 1.2  1992/10/19  12:00:50  pm
;;; parser fuer foreign-funs
;;;
;;; Revision 1.1  1992/10/13  14:28:39  pm
;;; Initial revision
;;;-----------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(in-package "CLICC")

(require "fftypes")

;;------------------------------------------------------------------------------
;; Fehlermeldungen.
;;------------------------------------------------------------------------------
(defconstant NO_NAME_SPECIFIED 
  "You must specify a name in DEF-CALL-OUT")
(defconstant NO_FORWARD_REF 
  "You cannot use the call-out-function ~S before its deklaration.")
(defconstant NOT_OF_RIGHT_LENGTH
  "The length (~S) of the argument-list doesn't match its definition (~S)")

;;------------------------------------------------------------------------------
;; Meldungen.
;;------------------------------------------------------------------------------
(defconstant ANALYSE-CALL-OUT   "Analyse Call-Out-Function ~S")
(defconstant ANALYSE-CALL-IN    "Analyse Call-In-Function  ~S")
(defconstant ANALYSE-DEF-C-TYPE "Analyse C-Type-Definition ~S")


;;------------------------------------------------------------------------------
;; DEF-CALL-OUT name {option}*
;;
;; option ::= 
;;     (:name <c-name>)
;;   | (:arguments ({<c-type>}*))
;;   | (:pass-types ({:by-value | :by-reference}*))
;;   | (:return-type <c-type>)
;;   | (:callback <boolean>)
;;
;;------------------------------------------------------------------------------
(defun p1-def-call-out (name_options &key 
                                     macro   ; Keine Signatur herausschreiben
                                     intern) ; Keine Meldung ausgeben
  
  (unless intern (clicc-message ANALYSE-CALL-OUT (first name_options)))
  
  (multiple-value-bind 
        (name c-name arguments return-type pass-types callback)
      (parse-foreign-fun-args name_options)
    
    (let ((operator-def (get-operator-def name)))
      
      (case (car operator-def)
        ;; Neue Definition eintragen
        ;;--------------------------
        ((nil)
         (let* ((argument-length (length-of-arguments arguments))
                (foreign-fun
                 (make-instance 'foreign-fun
                                :arguments arguments
                                :pass-types pass-types
                                :name c-name
                                :return-type return-type
                                :callback callback
                                :symbol name
                                :par-spec argument-length)))
           (set-foreign-fun name foreign-fun)

           ;; Signatur herausschreiben
           ;;-------------------------
           (unless macro (ff-p1-def-call-out-signatur foreign-fun))))
        
        ;; keine Forwaertsreferenz erlaubt
        ;;--------------------------------
        (:FORWARD
         (clicc-error NO_FORWARD_REF name))
        
        ;; keine Mehrfachdefinition erlaubt
        ;;---------------------------------
        (t (redef-op-error (car operator-def) name)))
      
      (values name))))

;;------------------------------------------------------------------------------
;; Syntax einer C-Typ-Definition:
;;
;; DEF-C-TYPE name data-type {option}*
;;------------------------------------------------------------------------------
(defun p1-def-c-type (name_type_options package)
  (multiple-value-bind (name type options)
      (parse-def-c-type-args name_type_options)
    (declare (ignore options))
    
    (clicc-message ANALYSE-DEF-C-TYPE (first name_type_options))

    (let* ((*CLICC-PRINT* nil))
    
      (cond 
        ;; Primitiver Typ
        ;;---------------
        ((atom type)
         (ff-p1-def-c-type-prim-type type name package)
         (ff-p1-def-c-type-ptr type name package))

        ;; unbenannte Struktur
        ;;--------------------
        ((eq (first type) 'ffi:c-struct)
         (ff-p1-def-c-type-new-struct type name package)
         (ff-p1-def-c-type-ptr (get-fftype name) name package))
        
        ;; benannte Struktur
        ;;------------------
        ((eq (first type) :struct)
         (ff-p1-def-c-type-old-struct type name package)
         (ff-p1-def-c-type-ptr type name package))
        
        ;; Pointer
        ;;--------
        ((eq (first type) 'ffi:c-ptr)
         (ff-p1-def-c-type-ptr type name package)

         ;; trage den neuen Typ in das globale Environment ein.
         ;;----------------------------------------------------
         (set-fftype name type))
        

        ;; Vektoren, ...
        ;;------------
        ;; *** to do ***
        

        ;; Alles andere ist ein Fehler
        ;;----------------------------
        (t (clicc-error "Not a valid c-type: ~A" type))))))


;;------------------------------------------------------------------------------
;; Hilfsfunktionen
;;------------------------------------------------------------------------------
;;------------------------------------------------------------------------------
;; Holt den Funktionsname und die Optionen-Liste und testet einige
;; Fehlerquellen ab.
;;
;; Resultat:
;; (MV) <name> <c-name> <arguments> <return-type> <pass-types> <callback>
;;------------------------------------------------------------------------------
(defun parse-foreign-fun-args (name_options)
  (let (name
        options)
    
    ;; Kein Name angegeben
    ;;--------------------
    (when (null name_options)
          (clicc-error NO_NAME_SPECIFIED))

    (if (atom name_options)
        (setq name name_options
              options nil)              ; keine Options
        (setq name (first name_options)
              options (rest name_options)))

    ;; Kein Symbol als Name
    ;;---------------------
    (unless (symbolp name)
      (clicc-error NO_NAME name 'DEF-CALL-OUT))

    ;; Uebrige Werte generieren
    ;;-------------------------
    (multiple-value-bind (c-name arguments return-type pass-types callback)
        (parse-ff-key-list name options)
      (values name c-name arguments return-type pass-types callback))))

;;------------------------------------------------------------------------------
;; Parsed die Key-Liste einer Call-Out-Definition. Es werden
;; Default-Werte angelegt, wenn keine Keys angegeben sind.
;; Es werden Fehler in der Key-List abgefangen.
;;
;; Die Keyword-Liste wird von links nach rechts durchgegangen, wobei
;; das letzte Auftreten eines Keywords den endgueltigen Wert festlegt.
;; [entgegen den ueblichen LISP-Konventionen.]
;;
;; Resultat: (MV) <c-name> <arguments> <return-type> <pass-types> <callback>
;;------------------------------------------------------------------------------
(defun parse-ff-key-list (name options)
  (let ((c-name (c-name-string name))
        (arguments '())
        (pass-types '())
        (callback nil)
        (return-type 'ffi:c-int))

    ;; Key-Word-Liste muss gerade sein
    ;;--------------------------------
    (unless (evenp (length options))
      (clicc-error ODD_LEN_KEYLIST))
    
    ;; Key-Word-Liste durchgehen
    ;;--------------------------
    (do* ((key-list options (cddr key-list))
          (key (first key-list) (first key-list))
          (arg (second key-list) (second key-list)))
         ((null key-list))

      ;; zuerst ein Key-Word
      ;;--------------------
      (unless (keywordp key)
        (clicc-error NO_KEYWORD key))

      (case key
        ;; C-Name
        ;;-------
        (:name 
         (if (stringp arg)
             (setq c-name arg)
             (clicc-error NO_STRING arg)))

        ;; Argument-Liste
        ;;---------------
        (:arguments
         (cond (;; leere Argumentliste
                ;;--------------------
                (null arg)
                (setq arguments nil))

               ;; ein Argument
               ;;-------------
               ((and (listp arg) (= (length arg) 1))
                (setq arguments (list (parse-c-type (first arg)
                                                    :could-be-void t
                                                    :could-be-vararg t))))

               ;; mehr als ein Argument
               ;;----------------------
               ((listp arg)
                (setq arguments
                      (append (mapcar #'parse-c-type (butlast arg))
                              (list (parse-c-type (car (last arg))
                                                  :could-be-vararg t)))))
               
               ;; Keine Liste ist ein Fehler
               ;;---------------------------
               (t
                (clicc-error NO_LIST arg))))
        
        ;; Rueckgabewert
        ;;--------------
        (:return-type
         (setq return-type (parse-c-type arg)))

        ;; Pass-types-Liste
        ;;-----------------
        (:pass-types
         (cond (;; leere Liste
                ;;------------
                (null arg)
                (setq pass-types nil))

               ;; nichtleere Liste
               ;;-----------------
               ((listp arg)
                (dolist (testat arg)
                  (when (and (not (eq testat :by-value))
                             (not (eq testat :by-reference)))
                    (clicc-error ILLEGAL_KEY
                                 '(:BY-REFERENCE :BY-VALUE)
                                 testat)))
                (setq pass-types arg))

               ;; Alles andere ist ein Fehler.
               ;;-----------------------------
               (t
                (clicc-error NO_LIST arg))))

        ;; Callback
        ;;---------
        (:callback
         (setq callback arg))
        
        ;; Ungueltiger Key
        ;;----------------
        (otherwise
         (clicc-error 
          ILLEGAL_KEY 
          '(:NAME :ARGUMENTS :RETURN-TYPE :PASS-TYPES :CALLBACK) key))))
    
    ;; Tests:
    ;;-------
    (when (equal arguments '(ffi:c-void))
      (setq arguments '()))

    (when (not (eq (length arguments) (length pass-types)))
      (clicc-error ":arguments and :pass-types must be of same length"))

    (values c-name arguments return-type pass-types callback)))

;;------------------------------------------------------------------------------
;; Testet auf Korrektheit der Argumente einer C-Typ-Definition
;;
;; RESULTAT: (MV) <name> <type> <options>
;;------------------------------------------------------------------------------
(defun parse-def-c-type-args (name_type_options)
  (let (name
        type
        options)

    ;; Es muessen immer Name und Typ angegeben werden
    ;;-----------------------------------------------
    (unless (>= (length name_type_options) 2)
      (clicc-error "You must specify a name and a type"))

    ;; Der Name muss ein Symbol sein
    ;;------------------------------
    (setq name (first name_type_options))
    (unless (symbolp name)
      (clicc-error NO_NAME name 'DEF-C-TYPE))

    ;; Der Typ darf nicht redefiniert werden
    ;;--------------------------------------
    (when (get-fftype name)
      (clicc-error "It is illegal to redefine the c-type ~S" name))

    ;; Die Typdefinition parsen
    ;;-------------------------
    (setq type (parse-c-type (second name_type_options)))

    ;; Options werden zur Zeit noch nicht unterstuetzt.
    ;;-------------------------------------------------
    (setq options (cddr name_type_options))

    (values name type options)))


;;------------------------------------------------------------------------------
(defconstant ILLEGAL_ARGUMENT
  "Not enough or to many arguments for ~S in PARSE-C-TYPE")

;;------------------------------------------------------------------------------
;; Parsen eine C-Typspezifikation
;;------------------------------------------------------------------------------
(defun parse-c-type (type-spec &key could-be-void called-by-ptr could-be-vararg)
  (cond (;; atomarer Typ
         ;;-------------
         (and (symbolp type-spec) type-spec) 
         (cond ((and (eq type-spec 'void) (not could-be-void)) 
                (clicc-error "VOID cannot be used here.")) 
               ((and (eq type-spec 'vararg) (not could-be-vararg)) 
                (clicc-error "VARARG cannot be used here.")) 
               (t 
                (let ((type (get-fftype type-spec))) 
                  (if called-by-ptr 
                      (unless type 
                        (set-fftype type-spec :not-defined)) 
                      (when (or (not type) (eq type :not-defined)) 
                        (clicc-error "Unknown type-specifier ~S" type-spec))))))
         (values (get-fftype type-spec)))

        ;; Konstruierter Typ
        ;;------------------
        ((listp type-spec) 
         (let ((konstruktor (first type-spec)) 
               (laenge (length type-spec))) 
           (case konstruktor 

             ;; (ptr <typ>||void)
             ;;------------------
             (ffi:c-ptr
              (if (= laenge 2) 
                  (values 
                   `(FFI:C-PTR ,(parse-c-type (second type-spec) 
                           :could-be-void t 
                           :called-by-ptr t))) 
                  (clicc-error ILLEGAL_ARGUMENT 'c-ptr)))

             ;;  (fun <typ> || void (<typ>* [vararg]) || (void))
             ;;-------------------------------------------------
             (ffi:c-fun
              (if (= laenge 3) 
                  (if (listp (third type-spec)) 
                      ;; Es muss ein Ptr vorangestellt sein.
                      ;;------------------------------------
                      (if called-by-ptr 
                          (flet ((parse-fun-arg-list (type-list) 
                                        ; Wenn ein Argument, dann kann
                                        ; es void sein.
                                   (if (= (length type-list) 1) 
                                       (values 
                                        `(,(parse-c-type (first type-list)
                                            :could-be-void t
                                            :could-be-vararg t)))
                                        ; Das letzte Argument der Liste kann
                                        ; vararg sein
                                       (values 
                                        `(,@(mapcar 
                                             #'parse-c-type (butlast type-list))
                                          ,(parse-c-type (car (last type-list))
                                            :could-be-vararg t))))))
                            (values 
                             `(FFI:C-FUN ,(parse-c-type (second type-spec)
                                     :could-be-void t)
                               ,(parse-fun-arg-list (third type-spec)))))
                          (clicc-error 
                           "Type `c-fun' must be used with `c-ptr'."))
                      (clicc-error "Second argument of C-FUN must be a list."))
                  (clicc-error ILLEGAL_ARGUMENT 'c-fun)))

             ;; (stuct (<typ> <bezeichner>)+)
             ;;------------------------------
             (ffi:c-struct
              (if (>= laenge 2) 
                  (values 
                   `(FFI:C-STRUCT 
                     ,@(maplist
                        #'(lambda (r-l)
                            (if (member (cadar r-l) (cdr r-l) :key #'cadr)
                                (clicc-error "Identifier used twice")
                                (if (= (length (car r-l)) 2)
                                    (values `(,(parse-c-type (caar r-l))
                                              ,(cadar r-l)))
                                    (clicc-error ILLEGAL_ARGUMENT 'c-struct))))
                        (rest type-spec))))
                  (clicc-error ILLEGAL_ARGUMENT 'c-struct)))

             ;; (union (<bezeichner> <typ>)+)
             ;;------------------------------
             (ffi:c-union
              )
             
             ;; (enum (bezeichner)+)
             ;;---------------------
             (ffi:c-enum
              )
             
             ;; (vector <c-typ> {<integer> || *}+)
             ;;-----------------------------------
             (ffi:c-array
              (if (>= laenge 3)
                  (flet ((parse-array-arg-list (a-number)
                           (if (or (typep a-number 'fixnum)
                                   (eq a-number '*))
                               (values a-number)
                               (clicc-error "~A must be a fixnum or '*"
                                            a-number))))
                    (values
                     `(FFI:C-ARRAY
                       ,(parse-c-type (second type-spec))
                       ,@(mapcar #'parse-array-arg-list (cddr type-spec))
                       )))
                  (clicc-error ILLEGAL_ARGUMENT 'c-array)))
             
             ;; Jeder andere Konstruktor ist ein Fehler
             ;;----------------------------------------
             (otherwise
              (clicc-error "Unknown constructor ~A" konstruktor)))))

        ;; Keine andere Moeglichkeit
        ;;--------------------------
        (t
         (clicc-error "Unknown type-spec ~S." type-spec))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun length-of-arguments (arguments)
  (cond ((eq (first arguments) 'ffi:c-void) 
         (values 0))
        ((eq (last arguments) 'ffi:c-vararg) 
         (- 1 (length arguments)))
        (t 
         (length arguments))))

;;------------------------------------------------------------------------------
;; finalize-call-in-funs
;;------------------------------------------------------------------------------
(defun finalize-call-in-funs ()
  (let ((call-in-funs (?call-in-funs *GLOBAL-ENVIRONMENT*)))
    (flet ((check-call-in (name_call-in-fun)
             (let* ((name (car name_call-in-fun))
                    (call-in-fun (cdr name_call-in-fun))
                    (operator (get-operator-def name))
                    (global-fun (cdr operator)))
               (unless (eq (car operator) :GLOBAL-FUN)
                 (clc-error "No global-fun declared as call-in-fun: ~A"
                              name))
               (setf (?call-in global-fun) call-in-fun))))

      (mapcar #'check-call-in call-in-funs))))

;;------------------------------------------------------------------------------
;; DEF-CALL-IN name {option}*
;;
;; option ::=
;;     (:name c-name)
;;   | (:arguments ({c-type}*))
;;   | (:return-type c-type)
;;------------------------------------------------------------------------------
(defun p1-def-call-in (name_options)
  
  (clicc-message ANALYSE-CALL-IN (first name_options))
  
  (multiple-value-bind (name c-name arg-list return-type)
      (parse-foreign-fun-args name_options)
    
    (let ((call-in-fun (get-call-in-fun name)))
      ;; keine Mehrfachdefinition erlaubt
      ;;---------------------------------
      (when call-in-fun
        (clicc-error "Call-In-Fun (~S) declared twice" call-in-fun))
      
      ;; Neue Definition eintragen
      ;;--------------------------
      (set-call-in-fun 
       name (make-instance 'call-in-fun
                           :arg-type-list arg-list
                           :foreign-name c-name
                           :return-type return-type))
      
      (values name))))

;;------------------------------------------------------------------------------

(provide "p1foreign")
