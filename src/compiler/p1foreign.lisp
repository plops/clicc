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
;;; Function : Foreign Functions
;;;
;;; $Revision: 1.37 $
;;; $Id: p1foreign.lisp,v 1.37 1994/11/22 14:49:16 hk Exp $
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
(defconstant ILLEGAL_ARGUMENT
  "Not enough or to many arguments for ~S in PARSE-C-TYPE")

;;------------------------------------------------------------------------------
;; Meldungen.
;;------------------------------------------------------------------------------
(defconstant ANALYSE-CALL-OUT   "Analyse Call-Out-Function ~S")
(defconstant ANALYSE-CALL-IN    "Analyse Call-In-Function  ~S")
(defconstant ANALYSE-DEF-C-TYPE "Analyse C-Type-Definition ~S")


;;******************************************************************************
;; DEF-CALL-OUT
;;******************************************************************************
;;------------------------------------------------------------------------------
;; DEF-CALL-OUT name {option}*
;;
;; option ::= 
;;     (:name <c-name>)
;;   | (:arguments ({<c-type>}*))
;;   | (:return-type <c-type>)
;;
;;------------------------------------------------------------------------------
(defun p1-def-call-out 
    (name_options &key
                  macro                 ; Keine Signatur herausschreiben
                  intern                ; Keine Meldung ausgeben
                  (typechecking t))     ; Kein Typtest erzeugen
  
  (unless intern (clicc-message ANALYSE-CALL-OUT (first name_options)))
  
  (multiple-value-bind 
        (name c-name arguments return-type)
      (parse-foreign-fun-args name_options)
    (let ((operator-def (get-operator-def name)))
      
      (case (car operator-def)
        ;; Neue Definition eintragen
        ;;--------------------------
        ((nil)
         (let* ((modifier
                 (multiple-value-bind (key old-name old-type)
                     (select-type return-type return-type return-type)
                   (declare (ignore old-name old-type))
                   (case key
                     ((old-struct old-union old-array old-handle ptr) 1)
                     (otherwise 0))))
                (argument-length (+ (length arguments) modifier))
                (gen-sym (gensym))
                (foreign-fun
                 (make-instance 'foreign-fun
                                :arguments arguments
                                :name c-name
                                :return-type return-type
                                :symbol name
                                :par-spec argument-length
                                :other-sym gen-sym)))
           
           (set-foreign-fun gen-sym foreign-fun)
           
           (if (null arguments)
               (ff-make-simple-defun name return-type gen-sym)
               (let* ((argument-liste 
                       (ff-make-argument-liste arguments))
                      (let-liste
                       (ff-make-let-liste argument-liste arguments))
                      (and-liste 
                       (ff-make-and-liste argument-liste arguments)))
                 (ff-make-difficult-defun name typechecking argument-liste
                                          let-liste and-liste 
                                          return-type gen-sym)))
           
           ;; Signatur herausschreiben
           ;;-------------------------
           (unless macro (ff-signatur foreign-fun))))
        
        ;; keine Forwaertsreferenz erlaubt
        ;;--------------------------------
        (:FORWARD
         (internal-error 'p1-def-call-out NO_FORWARD_REF name))
        
        ;; keine Mehrfachdefinition erlaubt
        ;;---------------------------------
        (t (redef-op-error (car operator-def) name)))
      
      (values))))

;;------------------------------------------------------------------------------
;; C-Funktion hat kein Argument
;;------------------------------------------------------------------------------
(defun ff-make-simple-defun (name return-type new-name)
  (p1-defun
   `(,name ()
     ,(ff-make-foreign-call '() return-type new-name))))

;;------------------------------------------------------------------------------
;; C-Funktion hat mindestens ein Argument
;;------------------------------------------------------------------------------
(defun ff-make-difficult-defun 
    (name typechecking argument-liste let-liste and-liste return-type new-name)
  (if typechecking
      (p1-defun
       `(,name ,argument-liste
         (let* ,let-liste
           (if (and ,@and-liste)
               ,(ff-make-foreign-call argument-liste return-type new-name)
               (error "illegal argument-type for foreign-fun")))))
      (p1-defun
       `(,name ,argument-liste
         (let* ,let-liste
           ,(ff-make-foreign-call argument-liste return-type new-name))))))

;;------------------------------------------------------------------------------
;; Erzeuge die Argumentliste, das heist eine Liste mit passender Anzahl an mit
;; gensym erzeugten Symbolen.
;;------------------------------------------------------------------------------
(defun ff-make-argument-liste (arguments)
  (let* (liste)
    (dotimes (count (length arguments))
      (push (gensym) liste))
    liste))

;;------------------------------------------------------------------------------
;; Erzeuge die Liste der Typtests.
;;------------------------------------------------------------------------------
(defun ff-make-let-liste (argument-liste arguments)
  (let* (liste)
    (dotimes (count (length arguments))
      (let* ((argument (nth count arguments))
             (gensym (nth count argument-liste)))
        (multiple-value-bind (key old-name old-type)
            (select-type argument argument argument)
          (declare (ignore old-name))
          (case key
            (primitive
             (push `(,gensym (,old-type ,gensym))
                   liste))))))
    liste))

;;------------------------------------------------------------------------------
;; Erzeuge die Liste der Typtests.
;;------------------------------------------------------------------------------
(defun ff-make-and-liste (argument-liste arguments)
  (let* (liste)
    (dotimes (count (length arguments))
      (let* ((argument (nth count arguments))
             (gensym (nth count argument-liste)))
        (multiple-value-bind (key old-name old-type)
            (select-type argument argument argument)
          (declare (ignore old-name old-type))
          (case key
            (primitive
             ;; do nothing
             )
            (otherwise
             (push (ff-make-type-test gensym argument)
                   liste))))))
    liste))

;;------------------------------------------------------------------------------
;; Erzeuge einen Typtest
;;------------------------------------------------------------------------------
(defun ff-make-type-test (var type)
  (multiple-value-bind (key old-name old-type)
      (select-type type type type)
    (case key
      (ptr
       (let* ((ptr-type (second old-type)))
         (multiple-value-bind (key old-ptr-name old-ptr-type)
             (select-type ptr-type ptr-type ptr-type)
           (declare (ignore key old-ptr-type))
           (let* ((test-name
                   (intern-postfixed old-ptr-name "-P")))
             `(,test-name ,var)))))
      (otherwise
       (let* ((test-name (intern-postfixed old-name "-P")))
         `(,test-name ,var))))))

;;------------------------------------------------------------------------------
;; Erzeuge den Aufruf der foreign-Function.
;;------------------------------------------------------------------------------
(defun ff-make-foreign-call (argument-liste return-type new-name)
  (multiple-value-bind (key old-name old-type)
      (select-type return-type return-type return-type)
    (case key
      ((old-struct old-handle old-union old-array) 
       (let* ((typesym (intern-postfixed (intern-prefixed "--" old-name) "--")))
         `(,new-name ,typesym ,@argument-liste)))

      (ptr
       (let* ((ptr-type (second old-type)))
         (multiple-value-bind (key old-ptr-name old-ptr-type)
             (select-type ptr-type ptr-type ptr-type)
           (declare (ignore old-ptr-type))
           (case key
             ((old-struct old-handle old-union old-array) 
              (let* ((typesym 
                      (intern-postfixed 
                       (intern-prefixed "--" old-ptr-name) "--")))
                `(,new-name ,typesym ,@argument-liste)))
             (otherwise
              `(,new-name ,@argument-liste))))))

      (otherwise
       `(,new-name ,@argument-liste)))))

;;******************************************************************************
;; Hilfsfunktionen von def-call-out
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Holt den Funktionsname und die Optionen-Liste und testet einige
;; Fehlerquellen ab.
;;
;; Resultat:
;; (MV) <name> <c-name> <arguments> <return-type>
;;------------------------------------------------------------------------------
(defun parse-foreign-fun-args (name_options)
  (let (name
        options)
    
    ;; Kein Name angegeben
    ;;--------------------
    (when (null name_options)
      (internal-error 'parse-foreign-fun-args NO_NAME_SPECIFIED))
    
    (if (atom name_options)
        (setq name name_options
              options nil)              ; keine Options
        (setq name (first name_options)
              options (rest name_options)))
    
    ;; Kein Symbol als Name
    ;;---------------------
    (unless (symbolp name)
      (internal-error 'parse-foreign-fun-args NO_NAME name 'DEF-CALL-OUT))
    
    ;; Uebrige Werte generieren
    ;;-------------------------
    (multiple-value-bind (c-name arguments return-type)
        (parse-ff-key-list name options)
      (values name c-name arguments return-type))))

;;------------------------------------------------------------------------------
;; Parsed die Key-Liste einer Call-Out-Definition. Es werden
;; Default-Werte angelegt, wenn keine Keys angegeben sind.
;; Es werden Fehler in der Key-List abgefangen.
;;
;; Die Keyword-Liste wird von links nach rechts durchgegangen, wobei
;; das letzte Auftreten eines Keywords den endgueltigen Wert festlegt.
;; [entgegen den ueblichen LISP-Konventionen.]
;;
;; Resultat: (MV) <c-name> <arguments> <return-type>
;;------------------------------------------------------------------------------
(defun parse-ff-key-list (name options)
  (let ((c-name (c-name-string name))
        (arguments '())
        (return-type 'ffi:c-int))
    
    ;; Key-Word-Liste muss gerade sein
    ;;--------------------------------
    (unless (evenp (length options))
      (internal-error 'parse-ff-key-list ODD_LEN_KEYLIST))
    
    ;; Key-Word-Liste durchgehen
    ;;--------------------------
    (do* ((key-list options (cddr key-list))
          (key (first key-list) (first key-list))
          (arg (second key-list) (second key-list)))
         ((null key-list))
      
      ;; zuerst ein Key-Word
      ;;--------------------
      (unless (keywordp key)
        (internal-error 'parse-ff-key-list NO_KEYWORD key))
      
      (case key
        ;; C-Name
        ;;-------
        (:name 
         (if (stringp arg)
             (setq c-name arg)
             (internal-error 'parse-ff-key-list NO_STRING arg)))
        
        ;; Argument-Liste
        ;;---------------
        (:arguments
         (cond 
           ;; leere Argumentliste
           ;;--------------------
           ((null arg)
            (setq arguments nil))
           
           ;; ein Argument
           ;;-------------
           ((and (listp arg) (= (length arg) 1))
            (setq arguments (list (parse-c-type (first arg)
                                                :struktur nil
                                                :union nil
                                                :feld nil
                                                :handle nil))))
           ;; mehr als ein Argument
           ;;----------------------
           ((listp arg)
            (setq arguments
                  (mapcar #'(lambda (r-l)
                              (parse-c-type r-l 
                                            :struktur nil
                                            :union nil
                                            :feld nil
                                            :handle nil
                                            :void nil))
                          arg)))

           ;; Keine Liste ist ein Fehler
           ;;---------------------------
           (t
            (internal-error 'parse-ff-key-list NO_LIST arg))))
        
        ;; Rueckgabewert
        ;;--------------
        (:return-type
         (setq return-type (parse-c-type arg
                                         :struktur nil
                                         :union nil
                                         :feld nil
                                         :handle nil)))

        ;; Ungueltiger Key
        ;;----------------
        (otherwise
         (internal-error 'parse-ff-key-list
          ILLEGAL_KEY '(:NAME :ARGUMENTS :RETURN-TYPE) key))))
    
    ;; Tests:
    ;;-------
    (when (equal arguments '(ffi:c-void))
      (setq arguments '()))

    (values c-name arguments return-type)))

;;******************************************************************************
;; DEF-CALL-IN
;;******************************************************************************
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
                           :arguments arg-list
                           :foreign-name c-name
                           :return-type return-type))
      
      (values name))))

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


;;******************************************************************************
;; DEF-C-type
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Syntax einer C-Typ-Definition:
;;
;; DEF-C-TYPE name data-type
;;------------------------------------------------------------------------------
(defun p1-def-c-type (name_type)
  (let (name type)
    (clicc-message ANALYSE-DEF-C-TYPE (first name_type))

    ;; Es muessen immer Name und Typ angegeben werden
    ;;-----------------------------------------------
    (unless (= (length name_type) 2)
      (clicc-error "You must specify a name and a type"))

    ;; Der Name muss ein Symbol sein
    ;;------------------------------
    (setq name (first name_type))
    (unless (symbolp name) (clicc-error NO_NAME name 'DEF-C-TYPE))

    ;; Der Name darf nicht redefiniert werden
    ;;---------------------------------------
    (when (or (get-fftype name) (member-of-ffi name))
      (clicc-error "It is illegal to redefine the c-type ~S" name))      

    ;; Die Typdefinition parsen
    ;;-------------------------
    (setq type (parse-c-type (second name_type)
                             :lisp nil
                             :void nil
                             :ptr nil))
 
    ;; Typ ins globale Environment eintragen.
    ;;---------------------------------------
    (set-fftype name type)

    ;; markieren, ob Typ in die Signatur geschrieben werden soll.
    ;;-----------------------------------------------------------
    (unless *ffi-include-flag*
      (add-q name *ffi-type-queue*))

    ;; Die Lisp-Zugriffsfunktionen erzeugen.
    ;;--------------------------------------
    (multiple-value-bind (key old-name old-type)
        (select-type name type type)
      (case key
        (string     (ff-string name old-type))
        (primitive  (ff-prim-type name old-type))
        (new-array  (ff-new-array name type))
        (old-array  (ff-old-array name old-name old-type))
        (new-handle (ff-new-handle name))
        (old-handle (ff-old-handle name old-name))
        (new-struct (ff-new-struct name type))
        (old-struct (ff-old-struct name old-name old-type))
        (new-union  (ff-new-union name type))
        (old-union  (ff-old-union name old-name old-type))))))

;;------------------------------------------------------------------------------
;; Finalisieren der Typen, erzeugen der Funktionen (in fftypes.lisp)
;;------------------------------------------------------------------------------
(defun finalize-fftypes ()
  (let* ((fftypes (reverse (?fftypes *GLOBAL-ENVIRONMENT*))))

    ;; wenn diese beiden Listen leer sind, wird hier eh nix gemacht --sma
    ;;-------------------------------------------------------------------
    (unless (or fftypes *ffi-signatures*) (return-from finalize-fftypes))

    (setq *interface-file-stream* 
          (open (make-pathname :name (ffi-include-name
                                      (pathname-name *OUT-FILENAME*))
                               :type "h"
                               :defaults *OUT-FILENAME*)
                :direction :output
                :if-exists :supersede
                :if-does-not-exist :create))
    
    (dolist (name_type fftypes)
      (let* ((name (first name_type))
             (type (rest name_type)))
        (multiple-value-bind (key old-name old-type)
            (select-type name type type)
          (declare (ignore old-name old-type))
          (case key
            ;; Strukturen
            ;;-----------
            (new-struct 
             (let* ((*print-circle* nil)
                    (c-type (convert-c-type-to-string name))
                    (malloc-name (intern-prefixed "MALLOC-" name))
                    (size-of-name (intern-prefixed "SIZE-OF-" name))
                    (define-name-malloc (c-macro-string malloc-name))
                    (define-name-size (c-macro-string size-of-name))
                    (slots (rest type)))
               
               ;; Nur rausschreiben, wenn nicht aus include-Datei extrahiert
               ;;-----------------------------------------------------------
               (when (member name (queue2list *ffi-type-queue*))
                 ;; Typ definieren
                 ;;---------------
                 (write-to-header-file
                  (format nil
                          "struct ~A {~%~:{~T~A ~A;~%~}};"
                          c-type (mapcar #'make-c-struct-component slots))))
                 
               ;; Typedef auf den Typ
               ;;--------------------
               (write-to-header-file
                (format nil 
                        "typedef struct ~A ~A;" 
                        c-type c-type))
               
               ;; Alloziiere Speicher fuer die Struktur
               ;;--------------------------------------
               (write-to-header-file 
                (format nil
                        "#define ~A() must_malloc(sizeof(~A))" 
                        define-name-malloc c-type))
               
               ;; Groesse der Struktur
               ;;---------------------
               (write-to-header-file
                (format nil "#define ~A() sizeof(~A)"
                        define-name-size c-type))
               
               ;; Zugriffsfunktionen fuer die einzelnen slots
               ;;--------------------------------------------
               (finalise-access name type name)))
            
            ;; Unionen
            ;;--------
            (new-union  
             (let* ((*print-circle* nil)
                    (c-type (convert-c-type-to-string name))
                    (malloc-name (intern-prefixed "MALLOC-" name))
                    (size-of-name (intern-prefixed "SIZE-OF-" name))
                    (define-name-malloc (c-macro-string malloc-name))
                    (define-name-size (c-macro-string size-of-name))
                    (slots (rest type)))

               ;; Nur rausschreiben, wenn nicht aus include-Datei extrahiert
               ;;-----------------------------------------------------------
               (when (member name (queue2list *ffi-type-queue*))
                 ;; Typ definieren
                 ;;---------------
                 (write-to-header-file
                  (format nil
                          "union ~A {~%~:{~T~A ~A;~%~}};"
                          c-type (mapcar #'make-c-union-component slots))))
                 
               ;; Typedef auf den Typ
               ;;--------------------
               (write-to-header-file
                (format nil 
                        "typedef union ~A ~A;" 
                        c-type c-type))
               
               ;; Alloziiere Speicher fuer die Union
               ;;-----------------------------------
               (write-to-header-file 
                (format nil
                        "#define ~A() malloc(sizeof(~A))" 
                        define-name-malloc c-type))
               
               ;; Groesse der Union
               ;;------------------
               (write-to-header-file
                (format nil "#define ~A() sizeof(~A)"
                        define-name-size c-type))
               
               ;; Zugriffsfunktionen fuer die einzelnen slots
               ;;--------------------------------------------
               (finalise-access name type name)))
            
            ;; Arrays
            ;;-------
            (new-array  
             (let* ((*print-circle* nil)
                    (array-type (second type))
                    (dimension-list (third type))
                    (c-type (convert-c-type-to-string name))
                    (c-type-2 (convert-c-type-to-string array-type))
                    (malloc-name (intern-prefixed "MALLOC-" name))
                    (size-of-name (intern-prefixed "SIZE-OF-" name))
                    (define-name-malloc (c-macro-string malloc-name))
                    (define-name-size (c-macro-string size-of-name))
                    (aref-name (intern-prefixed "AREF-" name))
                    (get-name (intern-prefixed "GET-" aref-name))
                    (get-name-string (c-macro-string get-name))
                    (set-name (intern-prefixed "SET-" aref-name))
                    (set-name-string (c-macro-string set-name))
                    dimension-desc
                    )
               
               ;; Nur rausschreiben, wenn nicht aus include-Datei extrahiert
               ;;-----------------------------------------------------------
               (when (member name (queue2list *ffi-type-queue*))
                 ;; Typ definieren
                 ;;---------------
                 (write-to-header-file
                  (format nil "typedef ~A ~A~{[~A]~};" 
                          (convert-c-type-to-string array-type)
                          c-type dimension-list)))
               
               ;; Alloziiere Speicher fuer das Array
               ;;-----------------------------------
               (write-to-header-file
                (format nil "#define ~A() malloc(sizeof(~A))" 
                        define-name-malloc c-type))
               
               ;; Groesse des Arrays
               ;;-------------------
               (write-to-header-file
                (format nil "#define ~A() sizeof(~A)" 
                        define-name-size c-type))
               
               ;; erzeuge die lesenden und schreibenden Zugriffsfunktionen
               ;;---------------------------------------------------------
               (multiple-value-bind (key old-array-name old-array-type)
                   (select-type name array-type array-type)
                 (declare (ignore old-array-type old-array-name))
                 
                 (dotimes (count (length dimension-list))
                   (push (intern (format nil "x~A" count)) dimension-desc))
                 
                 (write-to-header-file 
                  (format nil 
                          "#define ~A(ptr~{,~A~}) ((*((~A *)(ptr)))~{[~A]~})"
                          get-name-string dimension-desc 
                          c-type dimension-desc))
                 
                 (case key
                   ((string primitive old-handle ptr)
                    (write-to-header-file
                     (format
                      nil
                      "#define ~A(ptr~{,~A~},value) (((*((~A *)(ptr)))~{[~A]~}) = ((~A)(value)))"
                      set-name-string dimension-desc 
                      c-type dimension-desc c-type-2)))
                   (otherwise
                    (write-to-header-file
                     (format 
                      nil
                      "#define ~A(ptr~{,~A~},value) memcpy((void *)&((((~A *)(ptr))~{[~A]~})), (void *)(&((~A)(value))), sizeof(~A))"
                      set-name-string dimension-desc 
                      c-type dimension-desc c-type-2 c-type-2)))))))))))
    
    (dolist (sig *ffi-signatures*)
      (write-to-header-file sig))
    
    (close *interface-file-stream*)))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun finalise-access (name type original-name &optional (access ""))
  (dolist (slot (rest type))
    (let* ((*print-circle* nil)
           (c-type (convert-c-type-to-string original-name))
           (slot-name (c-name-string (first slot)))
           (slot-type (second slot))
           (slot-reader 
            (intern (concatenate 'string (string name) "-" (string slot-name))))
           )
      
      (multiple-value-bind (key old-name old-type)
          (select-type name slot-type slot-type)
        (declare (ignore old-type old-name))
        
        (case key
          ((string primitive old-handle old-struct old-union old-array ptr)
           (let* 
               ((get-name (intern-prefixed "GET-" slot-reader))
                (get-name-string (c-macro-string get-name))
                (set-name (intern-prefixed "SET-" slot-reader))
                (set-name-string (c-macro-string set-name))
                (c-type-2 (convert-c-type-to-string slot-type))
                )
             
             (write-to-header-file
              (format nil "#define ~A(ptr) (((~A *)(ptr))->~A~A)"
                      get-name-string c-type access slot-name))
             
             (case key
               ((string primitive old-handle ptr)
                (write-to-header-file
                 (format
                  nil
                  "#define ~A(ptr, value) ((((~A *)(ptr))->~A~A) = ((~A)(value)))"
                  set-name-string c-type access slot-name c-type-2)))
               (otherwise               ; *** to be checked ***
                (write-to-header-file
                 (format 
                  nil
                  "#define ~A(ptr, value) memcpy(&(((~A *)(ptr))->~A~A), &((~A)(value)), sizeof(~A))"
                  set-name-string c-type 
                  access slot-name c-type-2 c-type-2))))))
          
          ((new-struct new-union)
           ;; Keine Zugriffsfunktion, aber
           ;; Zugriffsfunktionen fuer die Komponenten
           ;;----------------------------------------
           (let* ((access-string 
                   (concatenate 'string access (string slot-name) ".")))
             (finalise-access
              slot-reader slot-type original-name access-string))))))))

;;******************************************************************************
;; C-Typen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Parsen eine C-Typspezifikation
;;------------------------------------------------------------------------------
(defun parse-c-type (type-spec &key
                               (primitiv t)
                               (zeichenkette t)
                               (struktur t)
                               (union t)
                               (feld t)
                               (handle t)
                               (lisp t)
                               (void t) (ptr t)
                               (synonym t)
                               (called-by-ptr nil))
  
  (cond 
    ;; String
    ;;-------
    ((eq type-spec 'ffi:c-string)
     (if (and zeichenkette (not called-by-ptr))
         (values type-spec)
         (clicc-error "Misuse of c-string")))
    
    ;; vordefinierter Typ
    ;;-------------------
    ((or (eq type-spec 'ffi:c-char) 
         (eq type-spec 'ffi:c-short)
         (eq type-spec 'ffi:c-int) 
         (eq type-spec 'ffi:c-long)
         (eq type-spec 'ffi:c-unsigned-char) 
         (eq type-spec 'ffi:c-unsigned-short)
         (eq type-spec 'ffi:c-unsigned-int) 
         (eq type-spec 'ffi:c-unsigned-long)
         (eq type-spec 'ffi:c-float)
         (eq type-spec 'ffi:c-double))
     (if primitiv
         (values type-spec)
         (clicc-error "Misuse of c-<primitive>")))

    ;; Handle
    ;;-------
    ((eq type-spec 'ffi:c-handle)
     (if handle
         (values type-spec)
         (clicc-error "Misuse of c-handle")))
    
    ;; Speziell behandelte Typen
    ;;--------------------------
    ((eq type-spec 'ffi:c-void)
     (if void
         (values 'ffi:c-void)
         (clicc-error "Misuse of c-void")))
    
    ((or (eq type-spec :integer)
         (eq type-spec :short)
         (eq type-spec :int)
         (eq type-spec :long)
         (eq type-spec :character)
         (eq type-spec :float))
     (if lisp
         (values type-spec)
         (clicc-error "Misuse of :<lisp-val>")))
    
    ;; Synonymtyp
    ;;-----------
    ((atom type-spec)
     (if synonym
         (if (get-fftype type-spec)
             (values type-spec)
             (if called-by-ptr
                 (values type-spec)
                 (clicc-error "Unknown type-specifier ~S" type-spec)))
         (clicc-error "Misuse of Synonym-Type")))

;;;*** ptr auf nicht-strukturierte sind nicht moeglich***!!!


    ;; Struktur  (ffi:c-struct (<bezeichner> <typ>)+)
    ;;---------
    ((and (listp type-spec) 
          (eq (first type-spec) 'ffi:c-struct))
     (if (and struktur (>= (length type-spec) 2))
         (values 
          `(ffi:c-struct 
            ,@(maplist
               ;; slot-description
               ;;-----------------
               #'(lambda (r-l)
                   (if (member (caar r-l) (cdr r-l) :key #'car)
                       ;;     <bezeichner>
                       (clicc-error "Identifier used twice")
                       (if (= (length (car r-l)) 2)
                           (values `(,(caar r-l)
                                     ,(parse-c-type (cadar r-l)
                                                    :lisp nil
                                                    :void nil)))
                           (clicc-error ILLEGAL_ARGUMENT 'c-struct))))
               (rest type-spec))))
         (clicc-error ILLEGAL_ARGUMENT 'c-struct)))
    
    ;; Union  (ffi:c-union (<bezeichner> <typ>)+)
    ;;------
    ((and (listp type-spec) 
          (eq (first type-spec) 'ffi:c-union))
     (if (and union (>= (length type-spec) 2))
         (values 
          `(ffi:c-union
            ,@(maplist
               ;; slot-description
               ;;-----------------
               #'(lambda (r-l)
                   (if (member (caar r-l) (cdr r-l) :key #'car)
                       ;;     <bezeichner>
                       (clicc-error "Identifier used twice")
                       (if (= (length (car r-l)) 2)
                           (values `(,(caar r-l)
                                     ,(parse-c-type (cadar r-l)
                                                    :lisp nil
                                                    :void nil)))
                           (clicc-error ILLEGAL_ARGUMENT 'c-union))))
               (rest type-spec))))
         (clicc-error ILLEGAL_ARGUMENT 'c-union)))
    
    ;; Array  (ffi:c-array <type> <dimensions>)
    ;;------
    ((and (listp type-spec)
          (eq (first type-spec) 'ffi:c-array))
     (if (and feld (= (length type-spec) 3))
         (flet ((parse-dimensions (number-or-list)
                  (cond ((integerp number-or-list)
                         (values (list number-or-list)))
                        ((listp number-or-list)
                         (mapcar 
                          #'(lambda (a-num) 
                              (if (integerp a-num)
                                  (values a-num)
                                  (clicc-error "~A must be an integer"
                                               a-num)))
                          number-or-list))
                        (t
                         (clicc-error 
                          "~A must be an integer or list of integers"
                          number-or-list)))))
           (values
            `(ffi:c-array
              ,(parse-c-type (second type-spec)
                             :lisp nil
                             :void nil)
              ,(parse-dimensions (third type-spec)))))
         (clicc-error ILLEGAL_ARGUMENT 'c-array)))
    
    ;; Pointer
    ;;--------
    ((and (listp type-spec)
          (eq (first type-spec) 'ffi:c-ptr))
     (if (and ptr (= (length type-spec) 2))
         (values
          `(ffi:c-ptr ,(parse-c-type (second type-spec) 
                                     :called-by-ptr t
                                     :ptr nil
                                     :lisp nil
                                     :primitiv nil
                                     :handle nil)))
         (clicc-error ILLEGAL_ARGUMENT 'c-pointer)))
    
    ;; Keine andere Moeglichkeit
    ;;--------------------------
    (t
     (clicc-error "Unknown type-spec ~S." type-spec))))


;;------------------------------------------------------------------------------
;; Liefert zuereuck:
;; (MV) <key> <name> <type>
;;  - string
;;  - primitive
;;  - old-struct
;;  - old-union
;;  - old-array
;;  - old-handle
;;  - new-struct
;;  - new-union
;;  - new-array
;;  - new-handle
;;  - ptr
;;  - void
;;  - lisp
;;  sonst Fehler
;;------------------------------------------------------------------------------
(defun select-type (name type ref-type &key iterate)
  (cond
    ;; Kein typ
    ;;---------
    ((eq type nil)
     (internal-error 'select-type "Not a valid c-type: ~A" ref-type))

    ;; String
    ;;-------
    ((eq type 'ffi:c-string) 
     (values 'string name type))

    ;; Handle
    ;;-------
    ((eq type 'ffi:c-handle)
     (if iterate
         (values 'old-handle name type)
         (values 'new-handle name type)))

    ;; Elementarer Type
    ;;-----------------
    ((or (eq type 'ffi:c-char) 
         (eq type 'ffi:c-short)
         (eq type 'ffi:c-int) 
         (eq type 'ffi:c-long)
         (eq type 'ffi:c-unsigned-char) 
         (eq type 'ffi:c-unsigned-short)
         (eq type 'ffi:c-unsigned-int) 
         (eq type 'ffi:c-unsigned-long)
         (eq type 'ffi:c-float)
         (eq type 'ffi:c-double)
         (eq type 'ffi:c-long-double))
     (values 'primitive name type))

    ;; Void
    ;;-----
    ((eq type 'ffi:c-void)
     (values 'void name type))

    ;; Lisp-Typen
    ;;-----------
    ((or (eq type :float)
         (eq type :integer)
         (eq type :short)
         (eq type :int)
         (eq type :long)
         (eq type :character))
     (values 'lisp name type))
          
    ;; Synonym
    ;;--------
    ((atom type)
      (select-type type (get-fftype type) ref-type :iterate t))

    ;; Struktur
    ;;---------
    ((eq (first type) 'ffi:c-struct)
     (if iterate
         (values 'old-struct name type)
         (values 'new-struct name type)))
    
    ;; Union
    ;;------
    ((eq (first type) 'ffi:c-union)
     (if iterate
         (values 'old-union name type)
         (values 'new-union name type)))
        
    ;; Array
    ;;------
    ((eq (first type) 'ffi:c-array) 
     (if iterate
         (values 'old-array name type)
         (values 'new-array name type)))

    ;; Pointer
    ;;--------
    ((eq (first type) 'ffi:c-ptr)
     (values 'ptr name type))

    ;; Weitere:
    ;; c-void, c-vararg
    ;;-----------------
     
    ;; Alles andere ist ein Fehler
    ;;----------------------------
    (t (internal-error 'select-type "Not a valid c-type: ~A" ref-type))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun member-of-ffi (type)
  (member type 
          '(ffi:c-char ffi:c-short ffi:c-int ffi:c-long 
            ffi:c-unsigned-char ffi:c-unsigned-short 
            ffi:c-unsigned-int ffi:c-unsigned-long 
            ffi:c-float ffi:c-double 
            ffi:c-handle ffi:c-string ffi:c-void)))


;;------------------------------------------------------------------------------
(defun ffi-include-name (name)
  (concatenate 'string name "-ffi"))


;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------

(defun p1-ffi-include (forms flag)
  (cond ((and (= (length forms) 1)
              (typep (first forms) 'string))
         (if (equal flag 'user)
             (add-q (format nil "\"~A\"" (first forms)) *ffi-include-queue*)
             (add-q (format nil "<~A>" (first forms)) *ffi-include-queue*)))
        ((typep (first forms) 'string)
         (internal-error 
          'p1-ffi-include 
          "Too many/few arguments to FFI:~A-INCLUDE" flag))
        (t
         (internal-error
          'p1-ffi-include
          "Wrong type to FFI:~A-INCLUDE" flag))))

;;------------------------------------------------------------------------------
(provide "p1foreign")
