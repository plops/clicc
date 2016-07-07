;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Inhalt   : Funktionen, die auf den C-Typen basieren
;;;
;;; $Revision: 1.13 $
;;; $Log: fftypes.lisp,v $
;;; Revision 1.13  1993/11/03  11:43:29  pm
;;; Inkonsistenzen in den Symbolnamen behoben.
;;;
;;; Revision 1.12  1993/08/25  11:38:00  pm
;;; Alle Funktionen, die auf der Bearbeitung der C-Typen beruhen,
;;; wurden in diese Datei transferiert.
;;;
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;==============================================================================
;; PASS-1
;;==============================================================================

;;------------------------------------------------------------------------------
;; Funktionssignatur herausschreiben.
;;------------------------------------------------------------------------------
(defun ff-p1-def-call-out-signatur (foreign-fun)
  (let* ((argument-liste (?arguments foreign-fun))
         (c-name (?name foreign-fun))
         (return-type (?return-type foreign-fun)))
    
    (write-to-header-file
     (format nil "~A ~A(~{~A~^,~});"
             (convert-c-type-to-string return-type)
             c-name
             (mapcar #'convert-c-type-to-string argument-liste)))))

;;==============================================================================
;; generieren der Funktionen zur Manipulation von C-Typen.
;;==============================================================================
;;------------------------------------------------------------------------------
;; Fuer primitive Typen.
;;------------------------------------------------------------------------------
(defun ff-p1-def-c-type-prim-type (type name package)
  (let* ((type-function (intern-postfixed type "-P"))
         (name-function (intern-postfixed name "-P")))
    
    ;; erzeuge die Test-Funktion 
    ;; (<name>-p <c-value>)
    ;;---------------------
    (p1-defmacro `(,name-function (arg) (list ',type-function arg)))
    (export name-function package)
    
    ;; erzeuge die Konstruktorfunktion 
    ;; (<name> <value>)
    ;;-----------------
    (p1-defmacro `(,name (arg) (list ',type arg)))
    
    ;; erzeuge den Typ <name>
    ;;-----------------------
    (p1-deftype `(,name () (quote (satisfies ,type-function))))
    (export name package)

    ;; trage den neuen Typ in das globale Environment ein.
    ;;----------------------------------------------------
    (set-fftype name type)))

;;------------------------------------------------------------------------------
;; Fuer Strukturen, die noch nicht bekannt sind.
;;------------------------------------------------------------------------------
(defun ff-p1-def-c-type-new-struct (type name package)
  
  (let* ((typsymbol name)
         (make-name (intern-prefixed "MAKE-" name))
         (malloc-name (intern-prefixed "MALLOC-" name))
         (c-type (c-name-string name))
         (define-name (string-upcase (minus-to-underscore malloc-name)))
         (test-name (intern-postfixed name "-P")))
    
    ;; Typdefinition in der Headerdatei bekanntmachen
    ;;-----------------------------------------------
    (write-to-header-file
     (format nil "typedef struct {~%~:{~T~A ~A;~%~}} ~A;"
             (mapcar #'make-c-struct-component (rest type))
             c-type))
    
    ;; erzeuge die Konstruktor-Funktion (make-<name>)
    ;;-----------------------------------------------
    (write-to-header-file 
     (format nil "#define ~A() malloc(sizeof(~A))" define-name c-type))
    (p1-def-call-out
     `(,malloc-name 
       :arguments () :return-type (ffi:c-ptr ffi:c-char) :name ,define-name)
     :intern t :macro t) 
    (p1-defmacro 
     `(,make-name () '(RT::internal-make-c-struct ',typsymbol (,malloc-name))))
    (export make-name package)
    
    ;; erzeuge die Test-Funktion (<name>-p objekt)
    ;;--------------------------------------------
    (p1-defmacro
     (list test-name '(arg)
           `(list 'RT::internal-c-struct-p arg (quote ',typsymbol))))
    (export test-name package)
    
    ;; erzeuge die Zugriffsfunktionen
    ;;-------------------------------
    (labels 
        ((gen-slot-reader-and-writer (slot)
           (let* ((slot-type (first slot))
                  (slot-name (second slot))
                  (slot-reader 
                   (intern 
                    (concatenate 'string (string name) "-" (string slot-name))))
                  (retreive-name 
                   (intern 
                    (concatenate 'string "RETREIVE-" (string slot-reader))))
                  (retreive-name-string
                   (format nil "~A"
                           (string-upcase (minus-to-underscore retreive-name))))
                  (give-name
                   (intern (concatenate 'string "GIVE-" (string slot-reader))))
                  (give-name-string
                   (format nil "~A" 
                           (string-upcase (minus-to-underscore give-name)))))
             
             ;; erzeuge die lesende Zugriffsfunktion
             ;; (<name>-<slot> objekt)
             ;;-----------------------
             (write-to-header-file
              (format nil
                      "#define ~A(pointer) ((~A *)(pointer))->~A"
                      retreive-name-string c-type slot-name))
             (p1-def-call-out
              `(,retreive-name
                :arguments ((ffi:c-ptr ffi:c-char))
                :return-type ,slot-type :name ,retreive-name-string)
              :macro t :intern t)
             (p1-defun
              `(,slot-reader (objekt)
                (if (,test-name objekt)
                    (,retreive-name
                     (rt::internal-get-struct-pointer objekt))
                    (error "not a c-struct: ~A" objekt))))
             (export slot-reader package)
             
             ;; erzeuge die schreibenden Zugriffsfunktionen
             ;; (setf (<name>-<slot> objekt) value)
             ;;------------------------------------
             (write-to-header-file
              (format nil
               "#define ~A(pointer, value) (((~A *)(pointer))->~A = (value))"
               give-name-string c-type slot-name))
             (p1-def-call-out
              `(,give-name :arguments ((ffi:c-ptr ffi:c-char) ,slot-type)
                :return-type ,slot-type :name ,give-name-string)
              :macro t :intern t)
             (p1-defun 
              `((setf ,slot-reader) (value objekt)
                (if (,test-name objekt)
                    (,give-name
                     (rt::internal-get-struct-pointer objekt) value)
                    (error "not a c-struct: ~A" objekt)))))))
      
      (mapcar #'gen-slot-reader-and-writer (rest type)))
    
    ;; Zeilenumbruch am Ende des Definitionsteils
    ;;-------------------------------------------
    (write-to-header-file "")
         
    ;; trage den neuen Typ in das globale Environment ein.
    ;;----------------------------------------------------
    (set-fftype name 
                `(:struct ,(make-instance 'c-struct
                            :typsymbol typsymbol
                            :type type
                            :symbol name)))))

;;------------------------------------------------------------------------------
;; Fuer bekannte Strukturen.
;; 
;; Eine benannte Struktur stuetzt sich weitgehend auf die schon vorher
;; eingebundenen Funktionen.
;;------------------------------------------------------------------------------
(defun ff-p1-def-c-type-old-struct (type name package)
  (let* ((struktur (second type))
         (typsymbol (?typsymbol struktur))
         (old-name (?symbol struktur))
         (make-name (intern-prefixed "MAKE-" name))
         (malloc-old-name (intern-prefixed "MALLOC-" old-name))
         (test-name (intern-postfixed name "-P")))
    
    ;; erzeuge die Konstruktor-Funktion (make-<name>)
    ;;-----------------------------------------------
    (p1-defmacro 
     `(,make-name () 
       '(RT::internal-make-c-struct ',typsymbol (,malloc-old-name))))
    (export make-name package)
    
    ;; erzeuge die Test-Funktion (<name>-p objekt)
    ;;--------------------------------------------
    (p1-defmacro 
     (list test-name '(arg)
           `(list 'RT::internal-c-struct-p arg (quote ',typsymbol))))
    (export test-name package)
    
    ;; erzeuge die Zugriffsfunktionen
    ;;-------------------------------
    (labels
        ((gen-slot-reader-and-writer (slot)
           (let* ((slot-name (second slot))
                  (slot-reader
                   (intern 
                    (concatenate 'string (string name) "-" (string slot-name))))
                  (retreive-old-name 
                   (intern 
                    (concatenate 'string 
                                 "RETREIVE-" (string old-name) 
                                 "-" (string slot-name))))
                  (give-old-name
                   (intern
                    (concatenate 'string 
                                 "GIVE-" (string old-name) 
                                 "-" (string slot-name)))))
             
             ;; erzeuge die lesende Zugriffsfunktion
             ;; (<name>-<slot> objekt)
             ;;-----------------------
             (p1-defun
              `(,slot-reader (objekt)
                (if (,test-name objekt)
                    (,retreive-old-name
                     (RT::internal-get-struct-pointer objekt))
                    (error "not a c-struct: ~A" objekt))))
             (export slot-reader package)
             
             ;; erzeuge die schreibende Zugriffsfunktion
             ;; (setf (<name>-slot objekt) value)
             ;;----------------------------------
             (p1-defun 
              `((setf ,slot-reader) (value objekt)
                (if (,test-name objekt)
                    (,give-old-name
                     (rt::internal-get-struct-pointer objekt) value)
                    (error "not a c-struct: ~A" objekt)))))))
      
      (mapcar #'gen-slot-reader-and-writer (rest (?type struktur))))
    
    
    ;; trage den neuen Typ in das globale Environment ein.
    ;;----------------------------------------------------
    (set-fftype name type)))

;;------------------------------------------------------------------------------
;; Fuer Pointer
;;------------------------------------------------------------------------------
(defun ff-p1-def-c-type-ptr (type name package)
  (cond
    ;; Zeiger auf primitiven Typ
    ;;--------------------------
    ((atom type)
     (let* ((k-name (intern-postfixed name "-PTR"))
            (k-type (intern-postfixed type "-PTR"))
            (k-name-test (intern-postfixed name "-PTR-P"))
            (k-type-test (intern-postfixed type "-PTR-P"))
            (k-name-deref (intern-prefixed "DEREF-" k-name))
            (k-type-deref (intern-prefixed "DEREF-" k-type)))

       ;; Testfunktion
       ;; (<name>-ptr-p objekt)
       ;;----------------------
       (p1-defmacro `(,k-name-test (arg) (list ',k-type-test arg)))
       (export k-name-test package)
       
       ;; Konstruktorfunktion
       ;; (<name>-ptr objekt)
       ;;--------------------
       (p1-defmacro `(,k-name (arg) (list ',k-type arg)))
       (export k-name package)
       
       ;; Dereferenzierungsfunktion
       ;; (deref-<name>-ptr objekt)
       ;;--------------------------
       ;; Wenn primitiv, dann Lisp-Objekt anlegen
       (p1-defmacro `(,k-name-deref (arg) (list ',k-type-deref arg)))
       (export k-name-deref package)))

    ;; Zeiger auf Pointer
    ;;-------------------
    ((eq (first type) 'ffi:c-ptr)
     ;; Konstruktorfunktion
     ;; (<name>-ptr objekt)
     ;;--------------------
     ;; *** to do ***

     ;; Testfunktion
     ;; (<name>-ptr-p objekt)
     ;;----------------------
     ;; *** to do ***

     ;; Dereferenzierungsfunktion
     ;; (deref-<name>-ptr objekt)
     ;;--------------------------
     ;; *** to do ***

     )

    ;; Zeiger auf neue Struktur
    ;;-------------------------
    ((eq (first type) 'ffi:c-struct)
     ;; mit meiner Implemetation nicht moeglich!
     (clicc-error "not implemented: (c-ptr (c-struct ...))")
     )

    ;; Zeiger auf benannte Struktur
    ;;-----------------------------
    ((eq (first type) :struct)
     (let* ((k-name (intern-postfixed name "-PTR"))
            (k-name-test (intern-postfixed name "-PTR-P"))
            (k-name-deref (intern-prefixed "DEREF-" k-name))
            (name-test (intern-postfixed name "-P"))
            (struktur (second type))
            (typsymbol (?typsymbol struktur)))

       ;; Konstruktorfunktion
       ;; (<name>-ptr objekt)
       ;;--------------------
       (p1-defmacro 
        (list k-name '(arg)
              `(list 'if 
                (list ',name-test arg)
                (list 'RT::internal-make-c-struct-ptr arg)
                (list 'error "not a c-struct: ~A" arg))))
       (export k-name package)
       
       ;; Testfunktion
       ;; (<name>-ptr-p objekt)
       ;;----------------------
       (p1-defmacro
        (list k-name-test '(arg)
              `(list 'RT::internal-c-struct-ptr-p arg (quote ',typsymbol))))
       
       ;; Dereferenzierungsfunktion
       ;; (deref-<name>-ptr objekt)
       ;;--------------------------
       (p1-defmacro
        (list k-name-deref '(arg)
              `(list 'if
                (list ',k-name-test arg)
                (list 'RT::internal-deref-c-struct-ptr arg)
                (list 'error "not a c-struct-ptr: ~A" arg))))))
     
    ;; Zeiger auf Vektor, ...
    ;;-----------------------
    ;; Konstruktorfunktion
    ;; (<name>-ptr objekt)
    ;;-------------------

    ;; Testfunktion
    ;; (<name>-ptr objekt)
    ;;-------------------
    
    ;; Dereferenzierungsfunktion
    ;; (deref-<name>-ptr objekt)
    ;;-------------------------
    
    
    ;; Alles andere ist ein Fehler
    ;;----------------------------
    (t (clicc-error "not a valid c-pointer-type: ~A" type))))


;; Pointerarithmetik
;;------------------
;; *** to do ***

;; Pointervergleich
;;-----------------
;; *** to do ***


;;==============================================================================
;; CODEGENERIERUNG
;;==============================================================================

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------

;;==============================================================================
;; Hilfsfunktionen
;;==============================================================================

;;------------------------------------------------------------------------------
;; Bekommt einen C-Typ und macht daraus die C-Repraesentation dieses Types als
;; String.
;;------------------------------------------------------------------------------
(defun convert-c-type-to-string (typ)
  (cond 
    ;; primitiver Typ
    ;;---------------
    ((atom typ)
     (minus-to-space typ))
    
    ;; Pointer
    ;;--------
    ((eq (first typ) 'ffi:c-ptr)
     (let* ((ptr-typ (second typ)))
       (format nil "~A *" (convert-c-type-to-string ptr-typ))))
    
    ;; benannte Struktur
    ;;------------------
    ((eq (first typ) :struct)
     (let* ((name (?symbol (second typ))))
       (c-name-string name)))
    
    ;; Vektoren, ...
    ;;--------------
    
    ;; Alles andere ist ein Fehler
    ;;----------------------------
    (t (clicc-error "Not a valid c-type: ~A" typ))))

;;------------------------------------------------------------------------------
;; Erzeugt eine Liste aus zwei Strings, die eine Komponente einer C-Struktur
;; enthalten.
;;------------------------------------------------------------------------------
(defun make-c-struct-component (komponente)
  (let* ((k-typ (first komponente))
         (k-slot (second komponente)))
    (list 
     (convert-c-type-to-string k-typ)
     (string k-slot))))

;;------------------------------------------------------------------------------
;; Erzeugt einen String, in dem alle Minuszeichen in Underscores gewandelt
;; wurden.
;;------------------------------------------------------------------------------
(defun minus-to-underscore (arg)
  (substitute #\_ #\- (string arg)))

;;------------------------------------------------------------------------------
;; Erzeugt einen String, in dem alle Minuszeichen in Spaces gewandelt wurden.
;; Desweiteren werden die ersten beiden Zeichen eliminiert. Wird verwendet, um
;; aus dem LISP-Namen eines C-types den C-Namen desselben Types zu generieren.
;;------------------------------------------------------------------------------
(defun minus-to-space (arg)
  (string-downcase (subseq (substitute #\space #\- (string arg)) 2)))

;;------------------------------------------------------------------------------
;; Erzeugt einen String, in dem alle Minuszeichen durch Underscores ersetzt
;; wurden, und der dann in Kleinbuchstaben konvertiert wurde.
;;------------------------------------------------------------------------------
(defun c-name-string (name)
  (string-downcase (minus-to-underscore name)))

(defun c-macro-string (name)
  (string-upcase (minus-to-underscore name)))

;;------------------------------------------------------------------------------
;; Schreibt Daten in die Interface-Header-Datei.
;;------------------------------------------------------------------------------
(defun write-to-header-file (arg)
  (format *interface-file-stream*
          "~A~%" arg))

;;------------------------------------------------------------------------------
;; Alle auf C-Typen basierenden Analysen bauen auf dem folgenden Conditional
;; auf:
;;------------------------------------------------------------------------------
#|
(cond
  ;; primitiver Typ
  ;;---------------
  ((atom typ)
   )

  ;; Pointer
  ;;--------
  ((eq (first typ) 'ffi:c-ptr)
   (let* ((ptr-typ (second typ)))
     (cond 
       ;; Zeiger auf primitiven Typ
       ;;--------------------------
       ((atom ptr-typ)
        )

       ;; Zeiger auf Pointer
       ;;-------------------
       ((eq (first ptr-typ) 'ffi:c-ptr)
        )

       ;; Zeiger auf Struktur
       ;;--------------------
       ((eq (first ptr-typ) :struct)
        )

       ;; Zeiger auf Vektoren, ...
       ;;-------------------------

       ;; Alles andere ist ein Fehler
       ;;----------------------------
       (t (clicc-error "Not a valid c-pointer-type: ~A" ptr-typ)))))

  ;; Struktur
  ;;---------
  ((eq (first typ) :struct)
   )

  ;; Vektoren, ...
  ;;--------------

  ;; Alles andere ist ein Fehler
  ;;----------------------------
  (t (clicc-error "Not a valid c-type: ~A" typ)))

|#

;;------------------------------------------------------------------------------
(provide "fftypes")
