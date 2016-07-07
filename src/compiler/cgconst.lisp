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
;;;            Generierung von konstanten Daten
;;;            - Arrays
;;;            - Strukturen
;;;            - Listen
;;;            - Characters
;;;            - Floats
;;;            - Fixnums
;;;            - Symbole
;;;            - Global Closures
;;;            - Klassen
;;;
;;; $Revision: 1.66 $
;;; $Id: cgconst.lisp,v 1.66 1994/12/06 15:42:25 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")     

(defconstant min-smallfixnum -1000)     ; Bereich der vordefinierten 
(defconstant max-smallfixnum  1000)     ; Konstanten bei Obrep = 2

;;------------------------------------------------------------------------------
;; Code für den Zugriff auf `Literale' generieren
;;------------------------------------------------------------------------------
(defmethod cg-form ((form sym))
  (case *result-spec*
    ((nil))
    (C-BOOL (setq *C-bool* C-TRUE))
    (t (C-symbol form (CC-dest *result-spec*)))))

;;------------------------------------------------------------------------------
(defmethod cg-form ((form class-def))
  (case *result-spec*
    ((nil))
    (C-BOOL (setq *C-bool* C-TRUE))
    (t (C-class form (CC-dest *result-spec*)))))

;;------------------------------------------------------------------------------
(defmethod cg-form ((form simple-literal))  
  (case *result-spec*
    ((nil))
    (C-BOOL (setq *C-bool* (CC-bool (not (null-form-p form)))))
    (t (let ((dest (CC-dest *result-spec*)))
         (etypecase form
           (null-form      (C-nil dest))
           (int            (C-integer   (?value form) dest))
           (character-form (C-character (?value form) dest))
           (float-form     (C-float form dest)))))))

;;------------------------------------------------------------------------------
(defmethod cg-form ((obj structured-literal))
  (case *result-spec*
    ((nil))
    (C-BOOL (setq *C-bool* C-TRUE))
    (t (C-MacroCall (CC-NameConc "LOAD_" 
                                 (typecase (?value obj)
                                   (cons "CONS")
                                   (string (add-comment (?value obj)) "SMSTR")
                                   (vector "SMVEC_T")
                                   (array "SMAR_T")
                                   (literal-instance "STRUCT")
                                   (T (error "internal (cg-form), ~S~
                                         has unknown type" (?value obj)))))
                    (CC-cast "CL_FORM *" (?adr obj))
                    (CC-dest *result-spec*)))))

;;------------------------------------------------------------------------------
;; Erzeugt CL_INIT-Array mit konstanten Daten
;;------------------------------------------------------------------------------
(defun gen-literals (literals name-postfix)
  (let (array-name
        float-array-name
        fixnum-array-name
        float-table
        fixnum-table)
    
    (labels
        ((gen-const-vector (elt adr)
           (let ((new-adr adr))
             (dotimes (i (length elt))
               (setq new-adr (gen-immed (aref elt i) new-adr))))
           (let ((new-adr adr))
             (dotimes (i (length elt))
               (setq new-adr (gen-const (aref elt i) new-adr)))
             new-adr))
         
         (gen-const (elt adr)
           (typecase elt
             
             (cons
              (add-comment (format nil "CONS(~A)" adr))
              (ecase *OBREP*
                (1 (incf adr 2))
                ((2 3) (C-init (CC-MacroCall "MAKE_CONS"))
                 (incf adr 3)))
              (let ((new-adr (gen-immed (car elt) adr)))
                (gen-immed (cdr elt) new-adr))
              (let ((new-adr (gen-const (car elt) adr)))
                (gen-const (cdr elt) new-adr)))
             
             (string
              (C-const-string elt)
              (+ 2 adr))
             
             (vector
              (add-comment (format nil "VECTOR(~A)" adr))
              (incf adr (1+ (length elt)))
              (C-init (CC-MacroCall "MAKE_VECTOR" (length elt)))
              (gen-const-vector elt adr))
             
             (array
              (let ((vector (make-array (array-total-size elt)
                                        :displaced-to elt
                                        :element-type
                                        (array-element-type elt))))
                ;; Array-Header
                ;;-------------
                (C-init (CC-MacroCall "MAKE_ARRAY" 
                                      (array-rank elt)
                                      (CC-arrayptr fixnum-array-name
                                                   (length fixnum-table))
                                      (length vector)))
                (dolist (dim (array-dimensions elt))
                  (vector-push-extend dim fixnum-table))
                (gen-const-vector vector (1+ adr))))
             
             (literal-instance
              (let* ((list (?value-list elt))
                     (len (length list)))
                (incf adr (+ 2 len))
                (C-init (CC-MacroCall "MAKE_STRUCT" (1+ len)
                                      (CC-const-symbol (?class elt))))
                (gen-const-vector list adr)))
             
             (t adr)))
         
         (gen-immed (elt adr)
           (typecase elt
             
             ;; Die einfachen Literale:
             ;;------------------------
             (null-form      (C-const-nil) adr)
             (character-form (C-const-character (?value elt)) adr)
             (int            (C-const-fixnum (?value elt)) adr)
             (float-form     (C-const-float (float-address (?value elt))) adr)
             
             ;; Die Typen, die innerhalb strukturierter Literale auftreten:
             ;;------------------------------------------------------------
             (null           (C-const-nil) adr)
             (character      (C-const-character elt) adr)
             (sym            (C-const-symbol    elt) adr)
             (integer        (C-const-fixnum    elt) adr)
             (float          (C-const-float (float-address elt)) adr)
             (class-def      (C-const-class elt) adr)
             (global-fun     (C-const-fun elt) adr)
             (T (cond
                  ((eq elt :unbound) (C-const-unbound) adr)
                  (T (C-init (CC-MacroCall 
                              (cg-type-spec elt)
                              (CC-arrayptr array-name
                                           (if (is-array elt) (1+ adr) adr))))
                     (+ adr (sizeof elt)))))))
         
         (sizeof (elt)
           (typecase elt
             (cons (+ (sizeof (car elt)) (sizeof (cdr elt))
                      (ecase *OBREP* (1 2) ((2 3) 3))))
             (string 2)
             (vector
              (let ((size (1+ (length elt))))
                (dotimes (i (length elt))
                  (incf size (sizeof (aref elt i))))
                size))
             (literal-instance
              (1+ (length (?value-list elt))))
             (array
              (let ((vector (make-array (array-total-size elt)
                                        :displaced-to elt
                                        :element-type
                                        (array-element-type elt))))
                (1+ (sizeof vector))))
             (T 0)))
         
         (float-address (float)
           (let ((pos (position float float-table)))
             (CC-arrayptr float-array-name
                          (if pos pos (vector-push-extend float float-table)))))
         
         (cg-floats ()
           (when (and float-table (> (length float-table) 0))
             (C-StaticArrayInitDecl "double" float-array-name)
             (C-initstart)
             (map nil 
                  #'(lambda (float) 
                      (C-init float))
                  float-table)
             (C-initend)))
         
         (cg-fixnums ()
           (when (and fixnum-table (> (length fixnum-table) 0))
             (C-StaticArrayInitDecl "long" fixnum-array-name)
             (C-initstart)
             (map nil 
                  #'(lambda (fixnum) 
                      (C-init (CC-fixnum fixnum)))
                  fixnum-table)
             (C-initend))))
    
      ;; Start der Funktion...
      ;;----------------------
      (when literals
        
        (setq array-name (CC-NameConc "K" name-postfix))

        (C-empty-Ln)
        (when (dolist (const literals nil)
                (when (and (structured-literal-p const)
                           (?needs-fixnum-array const))
                  (return t)))
          (setq fixnum-table (make-array 1 :fill-pointer 0 :adjustable t))
          (setq fixnum-array-name (CC-NameConc "X" name-postfix))
          (C-ExternArrayDecl "long" fixnum-array-name))
        
        (when (dolist (const literals nil)
                (when (or (float-form-p const)
                          (?needs-float-array const))
                  (return t)))
          (setq float-table (make-array 1 :fill-pointer 0 :adjustable t))
          (setq float-array-name (CC-NameConc "O" name-postfix))
          (C-ExternArrayDecl "double" float-array-name))

        (when (= *OBREP* 2) (init-extra-tables name-postfix))
        
        (C-CL_INIT-Decl array-name)
        (C-initstart)
        (let ((adr 0) (i 0))
          (dolist (struct literals)
            (add-comment (format nil "~a" adr))
            (incf i 2)
            (etypecase struct
              (float-form (setf (?adr struct) (float-address (?value struct))))
              (structured-literal
               (let ((elt (?value struct)))
                 (setf (?adr struct)
                       (CC-arrayptr array-name
                                    (if (is-array elt) (1+ adr) adr)))
                 (setq adr (gen-const elt adr)))))))
        (C-initend)
      
        (cg-floats)
        (cg-fixnums)
        (when (= *OBREP* 2) (cg-extra-tables))))))

;;------------------------------------------------------------------------------
(defun C-CL_INIT-Decl (name)
  (C-ArrayInitDecl "CL_INIT" name))

(defun C-Static-CL_INIT-Decl (name)
  (C-StaticArrayInitDecl "CL_INIT" name))

;;------------------------------------------------------------------------------
(defun is-array (elt) (and (arrayp elt) (not (vectorp elt))))

;;------------------------------------------------------------------------------
(defun cg-type-spec (elt)
  (typecase elt
    (cons "MAKE_CONSREF")
    (string "MAKE_STRREF")
    (vector "MAKE_VECREF")
    (array "MAKE_ARREF")
    (literal-instance "MAKE_STRUCTREF")
    (class-def "MAKE_CLASSREF")
    (T (error "internal (cg-type-spec), ~S has unknown type" elt))))
  
;;------------------------------------------------------------------------------
;; initialisiert den naechsten Eintrag in 'const_forms' mit NIL, ...
;;------------------------------------------------------------------------------
(defun C-const-nil ()                  
  (C-init (CC-const-nil)))

;;------------------------------------------------------------------------------
(defun CC-const-nil ()
  "MAKE_NIL")

;;------------------------------------------------------------------------------
(defun C-const-unbound ()
  (C-init (CC-const-unbound)))

;;------------------------------------------------------------------------------
(defun CC-const-unbound ()
  "MAKE_UNBOUND")

;;------------------------------------------------------------------------------
(defun C-const-fixnum (i)
  (C-init (CC-const-fixnum i)))

;;------------------------------------------------------------------------------
(defun CC-const-fixnum (i)
  (case *OBREP*
    ((1 3) (CC-MacroCall "MAKE_FIXNUM" (CC-fixnum i)))
    (2 (CC-MacroCall "MAKE_FIXNUMREF" (extrafixnum i)))))

;;------------------------------------------------------------------------------
;; Umgeht die Warnung "integer-overflow" des acc bei -2^31
;;------------------------------------------------------------------------------
(defun CC-fixnum (i)
  (if (eql i C-most-negative-fixnum)
      (format nil "(~A) - 1" (incf i))
      (format nil "~A" i)))

;;------------------------------------------------------------------------------
(defun C-const-float (f)
  (C-init (CC-const-float f)))

(defun CC-const-float (f)
  (case *OBREP*
    ((1 3) (CC-MacroCall "MAKE_FLOAT" f))
    (2 (CC-MacroCall "MAKE_FLOATREF" (extrafloat f)))))

;;------------------------------------------------------------------------------
(defun C-const-character (c)
  (C-init (CC-const-character c)))

(defun CC-const-character (c)
  (CC-MacroCall "MAKE_CHAR" (CC-character c)))

;;------------------------------------------------------------------------------
(defun C-const-symbol (sym)
  (C-init (CC-const-symbol sym)))

(defun CC-const-symbol (sym)
  (CC-MacroCall "MAKE_SYMREF" (CC-symbol sym)))

;;------------------------------------------------------------------------------
(defun C-const-string (string)
  (C-init 
   (CC-MacroCall "MAKE_STRING"
                 (length string)
                 (CC-const-string string))))

;;------------------------------------------------------------------------------
(defun CC-const-string (string)
  (labels
      ((char-ok-for-C (c)
         (and (graphic-char-p c)
              (not (find c "'\"\\")))))
    
    (let ((new-string "\""))
      (loop
       (when (= 0 (length string))
         (return (concatenate 'string new-string "\"")))
       (let ((p (position-if-not #'char-ok-for-C string)))
         (cond
           (p
            (setq new-string
                  (concatenate 'string
                               new-string
                               (subseq string 0 p)
                               (CCC-character (char string p))))
            (setq string (subseq string (1+ p))))
           (t
            (setq new-string (concatenate 'string
                                          new-string string))
            (setq string ""))))))))

;;------------------------------------------------------------------------------
(defun C-float (float dest)
  (if (slot-boundp float 'adr)
      (C-MacroCall "LOAD_FLOAT" (CC-stack *stack-top*) (?adr float)   dest)
      (C-MacroCall "GEN_FLOAT"  (CC-stack *stack-top*) (?value float) dest)))

;;------------------------------------------------------------------------------
(defun C-string (string dest)
  (C-MacroCall "GEN_SMSTR" 
               (length string) 
               (CC-const-string string)
               dest))

;;------------------------------------------------------------------------------
(defun CC-special (dynamic)
  (let ((sym (?sym dynamic)))
    (add-comment (string (?symbol sym)))
    (CC-MacroCall "SYMVAL" (?base sym) (?adr sym))))

;;------------------------------------------------------------------------------
(defun CC-symbol (sym)
  (add-comment (string (?symbol sym)))
  (CC-MacroCall "SYMBOL" (?base sym) (?adr sym)))

;;------------------------------------------------------------------------------
(defun C-symbol (sym dest)
  (C-MacroCall "LOAD_SYMBOL" (CC-symbol sym) dest))

;;------------------------------------------------------------------------------
(defun C-nil (dest)
  (C-MacroCall "LOAD_NIL" dest))

;;------------------------------------------------------------------------------
(defun C-t (dest)
  (C-MacroCall "LOAD_T" dest))

;;------------------------------------------------------------------------------
(defun C-integer (i dest)
  (if (and (= *OBREP* 2) (integerp i) (< min-smallfixnum i max-smallfixnum))
      (C-MacroCall "LOAD_SMALLFIXNUM" i dest)
      (C-MacroCall "LOAD_FIXNUM" (CC-stack *stack-top*) (CC-fixnum i) dest)))

;;------------------------------------------------------------------------------
(defun CCC-character (c)
  (case c
    (#\newline "\\n")
    (#\backspace "\\b")
    (#\tab "\\t")
    (#\page "\\f")
    (#\return "\\r")
    (#\rubout "\\177")
    (#\\ "\\\\")
    (#\" "\\\"")
    (#\' "\\'")
    (T (cond ((graphic-char-p c) (string c))
             (T (let ((code (char-code c))
                      (*print-base* 8))
                  ;; Ausgabe in oktal, 3 Zeichen breit, mit führenden Nullen
                  ;;--------------------------------------------------------
                  (concatenate 'string "\\" (format nil "~3,'0o" code))))))))

;;------------------------------------------------------------------------------
(defun CC-character (c)
  (concatenate 'string "'" (CCC-character c) "'"))

;;------------------------------------------------------------------------------
(defun C-character (c dest)
  (C-MacroCall "LOAD_CHAR" (CC-stack *stack-top*) (CC-character c) dest))

;;------------------------------------------------------------------------------
(defun C-const-class (class)
  (C-init (CC-MacroCall "MAKE_CLASSREF" (CC-class class))))

;;------------------------------------------------------------------------------
(defun CC-class (class)
  (add-comment (string (?symbol (?symbol class))))
  (CC-MacroCall "CLASS" (?adr class)))

;;------------------------------------------------------------------------------
(defun C-class (class dest)
    (C-MacroCall "LOAD_CLASS" (CC-class class) dest))

;;------------------------------------------------------------------------------
(defun C-const-fun (fun)
  (C-init (CC-MacroCall "MAKE_GLOBFUN" 
                        (CC-Address (CC-NameConc "C" (?adr fun))))))

;;------------------------------------------------------------------------------
;; Berechnet den Namen des Arrays, in dem die Symbole eines Moduls alloziert
;; werden
;;------------------------------------------------------------------------------
(defun calc-symbol-base (module-name)
  (calc-C-name "S" module-name))

;;------------------------------------------------------------------------------
;; Berechnet den Namen des Arrays, in dem die named Constants eines Moduls
;; alloziert werden
;;------------------------------------------------------------------------------
(defun calc-named-const-base (module-name)
  (calc-C-name "C" module-name))

;;------------------------------------------------------------------------------
;; (?package-list *module*) enthaelt eine Liste aller Packages,
;; die das uebersetzte Programm benutzt.
;;------------------------------------------------------------------------------
;; vorlaeufige Package-Beschreibung erzeugen:
;; NIL: uninterned Symbol
;;  0 : Symbol in Keyword-Package
;;  n : internal Symbol
;; -n : external Symbol
;; (wobei n der Index des Packages ist in dem zur Laufzeit erzeugten
;;  Array *package-list*)
;;------------------------------------------------------------------------------
(defun cg-package-cell (sym)
  (let ((packg (?package sym))
        (exported (?exported sym)))
    (if (eq :uninterned packg)
        (CC-const-nil)
        (let ((i (position packg (?package-list *module*) :test #'string=)))
          (if i
              (CC-const-fixnum (if (eq exported T) (- i) i))
              (progn
                (clicc-warning
                 "~a will be uninterned, package ~a is unknown"
                 (?symbol sym) packg)
                (CC-const-nil)))))))
  
;;------------------------------------------------------------------------------
;; Ein Array mit allen Symbolen des Moduls angelegen und intialisieren.
;;------------------------------------------------------------------------------
(defun cg-gen-symbols ()
  (C-empty-Ln)
  
  ;; Fuer alle Symbole, die als Konstanten deklariert sind,
  ;; grosse konstante Werte anlegen.
  ;;--------------------------------------------------
  (gen-literals
   (mapcan #'(lambda (sym)
               (let ((value (?constant-value sym)))
                 (when (constant-value-p sym)
                   (typecase value
                     ((or float-form structured-literal) (list value))
                     (T nil)))))
           (?sym-list *module*))
   (?symbol-base *module*))
  
  (when (= *OBREP* 2) (init-extra-tables (?symbol-base *module*)))

  (C-empty-Ln)
  (C-CL_INIT-Decl (?symbol-base *module*))
  (C-initstart)

  (let ((i 0))
    (dolist (sym (?sym-list *module*))
      ;; Kommentar ist Index in das Array
      ;; --------------------------------
      (add-comment (format nil "SYMBOL(~A)" i))
      (C-Newline)
      
      ;; Symbol
      ;;-------
      (C-init (CC-MacroCall
               (if (eq (?constant-value sym) :no-const)
                   "MAKE_SYMBOL"
                   "MAKE_CONST_SYMBOL")
               ;; Name
               ;;-----
               (length (?name sym))
               (CC-const-string (?name sym))

               ;; Wert Zelle
               ;;-----------
               (let ((value (?constant-value sym)))
                 (if (constant-value-p sym)
                     (typecase value
                       (null-form
                        (CC-const-nil))
                       (int 
                        (CC-const-fixnum (?value value)))
                       (character-form 
                        (CC-const-character (?value value)))
                       (sym 
                        (CC-const-symbol value))
                       (float-form
                        (CC-const-float (?adr value)))
                       (structured-literal
                        (CC-MacroCall (cg-type-spec (?value value)) 
                                      (?adr value)))
                       (t (error "constant ~a not implemented" value)))
                     ;; else
                     ;;-----
                     (CC-const-unbound)))
               ;; Package
               ;;--------
               (cg-package-cell sym)))
      
      (incf i)))
  
  ;; Das letzte Symbol wird durch eine NULL-Form abgeschlossen. Diese
  ;; Eigenschaft wird vom Garbage-Collector und der Funktion
  ;; setup_symbols_iterator ausgenutzt.
  ;;-------------------------------------------------------------------
  (C-Ln "END_SYMDEF")
  (C-initend)
  (when (= *OBREP* 2) (cg-extra-tables)))

;;------------------------------------------------------------------------------
(defun cg-gen-classes()
  (C-empty-Ln)

  ;; Erzeuge Code fuer die Komponenten einer Klasse
  ;;-----------------------------------------------
  (gen-literals
   (mapcan #'(lambda (class)
               (if (null-form-p (?slot-descr-list class))
                   `(,(?class-precedence-list class))
                   `(,(?class-precedence-list class)
                     ,(?slot-descr-list class))))
           (?class-def-list *module*))
   "classes")

  (C-empty-Ln)
  (C-CL_INIT-Decl "classes")
  (C-initstart)
  (let ((i 0))
    (dolist (class (?class-def-list *module*))

      ;; Kommentar ist Index in das Array
      ;; --------------------------------
      (add-comment (format nil "CLASS(~A)" i))
      (incf i)
      (C-Newline)

      (C-init (CC-MacroCall
               "MAKE_CLASS"
               ;; Name
               ;;-----
               (CC-symbol (?symbol class))

               ;; Class Precedence List
               ;;----------------------
               (CC-MacroCall
                (cg-type-spec (?value (?class-precedence-list class))) 
                (?adr (?class-precedence-list class)))
               
               ;; Number of Slots
               ;;----------------
               (if (null-form-p (?slot-descr-list class))
                   0
                   (length (?value (?slot-descr-list class))))

               ;; Slot Info List
               ;;---------------
               (if (null-form-p (?slot-descr-list class))
                   (CC-const-nil)
                   (CC-MacroCall
                    (cg-type-spec (?value (?slot-descr-list class))) 
                    (?adr (?slot-descr-list class))))))))
  
  (C-initend))

;;------------------------------------------------------------------------------
;; Ein Array mit allen Symbolen des Moduls angelegen und intialisieren.
;;------------------------------------------------------------------------------
(defun cg-gen-named-constants ()
  
  ;; Fuer alle Symbole, die als Konstanten deklariert sind,
  ;; grosse konstante Werte anlegen.
  ;;--------------------------------------------------
  (gen-literals
   (mapcan #'(lambda (const)
               (let ((value (?value const)))
                 (unless (eq :unknown value)
                   (typecase value
                     ((or float-form structured-literal) (list value))
                     (T nil)))))
           (?named-const-list *module*))
   (?named-const-base *module*)))

;;------------------------------------------------------------------------------
;; Die folgenden Funktionen werden nur für OBREP = 2 benötigt und bauen
;; Tabellen mit den zusätzlichen Datenstrukturen für Fixnums und Floats auf
;;------------------------------------------------------------------------------
(defvar extrafixnum-table)
(defvar extrafixnum-name)
(defvar extrafloat-table)
(defvar extrafloat-name)

;;------------------------------------------------------------------------------

(defun init-extra-tables (name-postfix)
  (setq extrafixnum-table (make-array 1 :fill-pointer 0 :adjustable t))
  (setq extrafixnum-name (CC-NameConc "XI" name-postfix))
  (C-ExternArrayDecl "CL_INIT" extrafixnum-name)

  (setq extrafloat-table (make-array 1 :fill-pointer 0 :adjustable t))
  (setq extrafloat-name (CC-NameConc "XF" name-postfix))
  (C-ExternArrayDecl "CL_INIT" extrafloat-name))

(defun cg-extra-tables ()
  (when (and extrafixnum-table (> (length extrafixnum-table) 0))
    (C-StaticArrayInitDecl "CL_INIT" extrafixnum-name)
    (C-initstart)
    (map nil #'(lambda (fixnum) 
                 (C-init (CC-MacroCall "MAKE_FIXNUM" fixnum)))
         extrafixnum-table)
    (C-initend))

  (when (and extrafloat-table (> (length extrafloat-table) 0))
    (C-StaticArrayInitDecl "CL_INIT" extrafloat-name)
    (C-initstart)
    (map nil #'(lambda (float) 
                 (C-init (CC-MacroCall "MAKE_FLOAT" float)))
         extrafloat-table)
    (C-initend))

  (setq extrafloat-table nil extrafixnum-table nil))

(defun extrafixnum (i)
  (if (< min-smallfixnum i max-smallfixnum)
      (CC-arrayptr "fixnum_ob" (- i min-smallfixnum))
      (let ((pos (position i extrafixnum-table)))
        (CC-arrayptr extrafixnum-name
                     (if pos pos 
                         (vector-push-extend i extrafixnum-table))))))

(defun extrafloat (f)
  (let ((pos (position f extrafloat-table)))
    (CC-arrayptr extrafloat-name
                 (* (if pos pos 
                        (vector-push-extend f extrafloat-table)) 2))))

;;------------------------------------------------------------------------------
(provide "cgconst")
