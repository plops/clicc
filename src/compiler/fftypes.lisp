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
;;; Inhalt   : Funktionen, die auf den C-Typen basieren
;;;
;;; $Revision: 1.22 $
;;; $Id: fftypes.lisp,v 1.22 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;******************************************************************************
;; generieren der Funktionen zur Manipulation von C-Typen.
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Fuer Strukturen, die noch nicht bekannt sind.
;;------------------------------------------------------------------------------
(defun ff-new-struct (name type)
  (ff-constant-new name)
  (ff-typ name)
  (ff-test-fun-structured name name 'struct)
  (ff-construct-fun name name)
  (ff-copy-fun-new name 'struct)
  (ff-access-fun-new name type name)
  )

;;------------------------------------------------------------------------------
;; Fuer Unionen, die noch nicht bekannt sind.
;;------------------------------------------------------------------------------
(defun ff-new-union (name type)
  (ff-constant-new name)
  (ff-typ name)
  (ff-test-fun-structured name name 'union)
  (ff-construct-fun name name)
  (ff-copy-fun-new name 'union)
  (ff-access-fun-new name type name)
  )

;;------------------------------------------------------------------------------
;; Fuer Handles
;;------------------------------------------------------------------------------
(defun ff-new-handle (name)
  (ff-constant-new name)
  (ff-typ name)
  (ff-test-fun-structured name name 'handle)
  )

;;------------------------------------------------------------------------------
;; Fuer Arrays
;;------------------------------------------------------------------------------
(defun ff-new-array (name type)
  (ff-constant-new name)
  (ff-typ name)
  (ff-test-fun-structured name name 'array)
  (ff-construct-fun name name)
  (ff-copy-fun-new name 'array)
  (ff-access-fun-array-new name type)
  )

;;------------------------------------------------------------------------------
;; Fuer Strings (Synonym)
;;------------------------------------------------------------------------------
(defun ff-string (name old-type)
  (ff-typ name)
  (ff-test-fun name old-type)
  (ff-convert-fun-string name)
  (ff-copy-fun-string name)
  )

;;------------------------------------------------------------------------------
;; Fuer primitive Typen. (Syonym)
;;------------------------------------------------------------------------------
(defun ff-prim-type (name old-type)
  (ff-typ name)
  (ff-test-fun name old-type)
  (ff-convert-fun name old-type)
  )

;;------------------------------------------------------------------------------
;; Fuer Synonyme von Strukturen.
;; 
;; Eine benannte Struktur stuetzt sich weitgehend auf die schon vorher
;; eingebundenen Funktionen.
;;------------------------------------------------------------------------------
(defun ff-old-struct (name old-name old-type)
  (ff-constant-old name old-name)
  (ff-typ name)
  (ff-test-fun-structured name old-name 'struct)
  (ff-construct-fun name old-name)
  (ff-copy-fun-old name old-name 'struct)
  (ff-access-fun-old name old-type name old-name)
  )

;;------------------------------------------------------------------------------
;; Fuer bekannte Unionen.
;; 
;; Eine benannte Union stuetzt sich weitgehend auf die schon vorher
;; eingebundenen Funktionen.
;;------------------------------------------------------------------------------
(defun ff-old-union (name old-name old-type)
  (ff-constant-old name old-name)
  (ff-typ name)
  (ff-test-fun-structured name old-name 'union)
  (ff-construct-fun name old-name)
  (ff-copy-fun-old name old-name 'union)
  (ff-access-fun-old name old-type name old-name)
  )

;;------------------------------------------------------------------------------
;; Fuer Synonyme von handles
;;------------------------------------------------------------------------------
(defun ff-old-handle (name old-name)
  (ff-constant-old name old-name)
  (ff-typ name)
  (ff-test-fun-structured name old-name 'handle)
  )

;;------------------------------------------------------------------------------
;; Fuer Arrays (Synonym)
;;------------------------------------------------------------------------------
(defun ff-old-array (name old-name old-type)
  (ff-typ name)
  (ff-test-fun-structured name old-name 'array)
  (ff-construct-fun name old-name)
  (ff-copy-fun-old name old-name 'array)
  (ff-access-fun-array-old name old-name old-type)
  (ff-constant-old name old-name)
  )

;;******************************************************************************
;; Typ erzeugen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Typ erzeugen.
;;------------------------------------------------------------------------------
(defun ff-typ (name)
  (let* ((test-name (intern-postfixed name "-P")))
    
    (p1-deftype `(,name () (quote (satisfies ,test-name))))))

;;******************************************************************************
;; Testfunktionen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Erzeugt die Testfunktion fuer elementare Typen und fuer Strings:
;;   (<name>-p <value>)
;;------------------------------------------------------------------------------
(defun ff-test-fun (name type)
  (let* ((test-name (intern-postfixed name "-P"))
         (test-type (intern-postfixed type "-P")))
    
    (p1-defun `(,test-name (arg) (,test-type arg)))))

;;------------------------------------------------------------------------------
;; Erzeuge die Testfunktion fuer Strukturen und Unionen, Arrays und Handles:
;;   (<name>-p <value>)
;;------------------------------------------------------------------------------
(defun ff-test-fun-structured (name old-name verweis)
  (let* ((typesymbol (intern-postfixed (intern-prefixed "--" old-name) "--"))
         (test-name (intern-postfixed name "-P"))
         (internal-fun (cond ((eq verweis 'struct) 'RT::struct-p)
                             ((eq verweis 'union) 'RT::union-p)
                             ((eq verweis 'array) 'RT::array-p)
                             ((eq verweis 'handle) 'RT::handle-p))))
    
    (p1-defun `(,test-name (arg) (,internal-fun arg ,typesymbol)))))

;;******************************************************************************
;; Konvertierungsfunktionen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Konvertierungsfunktion fuer primitive Werte:
;;   (<name> <value>)
;;------------------------------------------------------------------------------
(defun ff-convert-fun (name old-type)
  (p1-defun `(,name (arg) (,old-type arg))))

;;------------------------------------------------------------------------------
;; Konvertierungsfunktion fuer Strings:
;;   (make-<name> <value>)
;;------------------------------------------------------------------------------
(defun ff-convert-fun-string (name)
  (let* ((make-name (intern-prefixed "MAKE-" name)))
    
    (p1-defun `(,make-name (arg) (ffi:make-c-string arg)))))

;;******************************************************************************
;; Konstruktorfunktionen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Konstruktor-Funktion fuer neue Strukturen, Unionen und Arrays
;;   (make-<name>)
;;------------------------------------------------------------------------------
(defun ff-construct-fun (name old-name)
  (let* ((make-name (intern-prefixed "MAKE-" name))
         (malloc-name (intern-prefixed "MALLOC-" old-name))
         (define-name (c-macro-string malloc-name))) 
    
    (p1-def-call-out
     `(,make-name
       :arguments (ffi:c-void)
       :return-type (ffi:c-ptr ,old-name)
       :name ,define-name)
     :intern t :macro t)))

;;******************************************************************************
;; Kopierfunktionen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Kopierfunktion fuer Strings
;;   (copy-<name> <arg>)
;;------------------------------------------------------------------------------
(defun ff-copy-fun-string (name)
  (let* ((copy-name (intern-prefixed "COPY-" name)))
    
    (p1-defun `(,copy-name (arg) (ffi:copy-c-string arg)))))

;;------------------------------------------------------------------------------
;; Kopier-Funktion fuer neue Strukturen, Unionen und Arrays
;;   (copy-<name> <arg>)
;;------------------------------------------------------------------------------
(defun ff-copy-fun-new (name verweis)
  (let* ((internal-fun (cond ((eq verweis 'struct) 'RT::copy-c-struct)
                             ((eq verweis 'union) 'RT::copy-c-union)
                             ((eq verweis 'array) 'RT::copy-c-array)))
         
         (copy-name (intern-prefixed "COPY-" name))
         (test-name (intern-postfixed name "-P"))
         (make-name (intern-prefixed "MAKE-" name))
         (size-of-name (intern-prefixed "SIZE-OF-" name))
         (define-name (c-macro-string size-of-name))) 
    
    (p1-def-call-out
     `(,size-of-name 
       :arguments (ffi:c-void) 
       :return-type ffi:c-unsigned-long
       :name ,define-name)
     :intern t :macro t) 
    
    (p1-defun `(,copy-name (arg)
                (if (,test-name arg)
                    (,internal-fun
                     (,make-name) arg (,size-of-name))
                    (error "Runtime Error: not of right type: ~A" arg))))))

;;------------------------------------------------------------------------------
;; Kopier-Funktion fuer Strukturen und Unionen und Arrays (Synonym)
;;   (copy-<name> <arg>)
;;------------------------------------------------------------------------------
(defun ff-copy-fun-old (name old-name verweis)
  (let* ((internal-fun (cond ((eq verweis 'struct) 'RT::copy-c-struct)
                             ((eq verweis 'union) 'RT::copy-c-union)
                             ((eq verweis 'array) 'RT::copy-c-array)))
         
         (copy-name (intern-prefixed "COPY-" name))
         (test-name (intern-postfixed old-name "-P"))
         (make-name (intern-prefixed "MAKE-" old-name))
         (size-of-name (intern-prefixed "SIZE-OF-" old-name))) 
    
    (p1-defun `(,copy-name (arg)
                (if (,test-name arg)
                    (,internal-fun
                     (,make-name) arg (,size-of-name))
                    (error "Runtime Error: not of right type: ~A" arg))))))

;;******************************************************************************
;; Zugriffsfunktionen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Fuer Neue Strukturen und Unionen
;;------------------------------------------------------------------------------
(defun ff-access-fun-new (name type original-name &optional (access ""))
  (dolist (slot (rest type))
    (let* ((slot-name (first slot))
           (slot-type (second slot))
           (slot-reader 
            (intern 
             (concatenate 'string (string name) "-" (string slot-name))))) 
      
      ;; Zugriffsfunktionen abhaengig vom Typ des Slots erzeugen
      ;;--------------------------------------------------------
      (multiple-value-bind (key old-name old-type)
          (select-type name slot-type slot-type)
        (declare (ignore old-type old-name))
        
        (case key
          ;; Strings, Primitive, Synonyme
          ;;-----------------------------
          ((string primitive old-handle old-struct old-union old-array ptr)
           (let* 
               ((get-name (intern-prefixed "GET-" slot-reader))
                (get-name-string (c-macro-string get-name))
                (set-name (intern-prefixed "SET-" slot-reader))
                (set-name-string (c-macro-string set-name))) 
             
             ;; erzeuge die lesende Zugriffsfunktion
             ;; (<name>-<slot> objekt)
             ;;-----------------------
             (p1-def-call-out
              `(,slot-reader
                :arguments ((ffi:c-ptr ,original-name))
                :return-type ,slot-type 
                :name ,get-name-string)
              :macro t :intern t)
             
             ;; erzeuge die schreibenden Zugriffsfunktionen
             ;; (setf (<name>-<slot> objekt) value)
             ;;------------------------------------
             (p1-def-call-out
              `(,set-name 
                :arguments ((ffi:c-ptr ,original-name) ,slot-type)
                :return-type ffi:c-void
                :name ,set-name-string)
              :macro t :intern t)
             
             (p1-defun 
              `((L::setf ,slot-reader) (value object)
                (,set-name object value)
                value))))
          
          ;; Unbenannte Handles
          ;;-------------------
          (new-handle
           ;; keine Zugriffsfunktion!
           ;;------------------------
           )
          
          ;; Unbenannte Strukturen
          ;;----------------------
          ((new-struct new-union)
           ;; Keine Zugriffsfunktion, aber
           ;; Zugriffsfunktionen fuer die Komponenten
           ;;----------------------------------------
           (let* ((access-string 
                   (concatenate 'string access (string slot-name) ".")))
             
             (ff-access-fun-new
              slot-reader slot-type original-name access-string)))
          
          ;; Arrays
          ;;-------
          (new-array
           ;; keine Zugriffsfunktion,
           ;; aber Zugriff fuer Komponenten
           ;;------------------------------
           ;; *** to do *** aref-zugriff.
           )
          
          )))))

;;------------------------------------------------------------------------------
;; Zugriffsfunktionen fuer neue Arrays
;;------------------------------------------------------------------------------
(defun ff-access-fun-array-new (name type)
  (let* ((test-name (intern-postfixed name "-P"))
         (aref-name (intern-prefixed "AREF-" name))
         (get-name (intern-prefixed "GET-" aref-name))
         (get-name-string (c-macro-string get-name))
         (set-name (intern-prefixed "SET-" aref-name))
         (set-name-string (c-macro-string set-name))
         (array-type (second type))
         (dimension-list (third type))
         dimension-desc
         dimension-type-desc
         and-list)

    (multiple-value-bind (key old-name old-type)
        (select-type name array-type array-type)
      (declare (ignore old-type old-name)) 
      
      (case key
        ((string primitive old-handle old-struct old-union old-array ptr)
         ;; Listen fuer die Zugriffe erzeugen
         ;;----------------------------------
         (dotimes (count (length dimension-list))
           (push (intern (format nil "X~A" count)) dimension-desc)
           (push 'ffi:c-unsigned-long dimension-type-desc)
           (push `(typep (ffi:lisp-integer ,(intern (format nil "X~A" count))) 
                   '(integer 0 ,(nth count dimension-list))) and-list))
         (setq dimension-desc (reverse dimension-desc))
         
         (p1-def-call-out
          `(,get-name
            :arguments ((ffi:c-ptr ,name) ,@dimension-type-desc)
            :return-type ,array-type
            :name ,get-name-string)
          :macro t :intern t :typechecking nil)
         
         (p1-defun
          `(,aref-name (objekt ,@dimension-desc)
            (if (and (,test-name objekt)
                     ,@and-list)
                (,get-name objekt ,@dimension-desc)
                (error "not a structured type: ~A" objekt))))
         
         ;; erzeuge die schreibende Zugriffsfunktion
         ;;-----------------------------------------
         (p1-def-call-out
          `(,set-name 
            :arguments ((ffi:c-ptr ,name) ,@dimension-type-desc ,array-type)
            :return-type ffi:c-void
            :name ,set-name-string)
          :macro t :intern t :typechecking nil)
         
         (p1-defun 
          `((L::setf ,aref-name) (value objekt ,@dimension-desc)
            (if (and (,test-name objekt)
                     ,@and-list)
                (progn (,set-name objekt ,@dimension-desc value)
                       value)
                (error "not a structured type: ~A" objekt)))))))))

;;------------------------------------------------------------------------------
;; Zugriffsfunktionen fuer Synonyme von Strukturen und Unionen
;;------------------------------------------------------------------------------
(defun ff-access-fun-old (name type original-name old-name)
  (dolist (slot (rest type))
    (let* ((slot-name (first slot))
           (slot-type (second slot))
           (slot-reader 
            (intern 
             (concatenate 'string (string name) "-" (string slot-name))))
           (old-slot-reader 
            (intern 
             (concatenate 'string (string old-name) "-" (string slot-name)))))
      
      ;; Zugriffsfunktionen abhaengig vom Typ des Slots erzeugen
      ;;--------------------------------------------------------
      (multiple-value-bind (key old-name old-type)
          (select-type name slot-type slot-type)
        (declare (ignore old-type old-name))
        
        (case key
          ;; Strings, Primitive, benannte Strukturen
          ;;----------------------------------------
          ((string primitive old-handle old-struct old-union old-array ptr)
           (let* 
               ((get-name (intern-prefixed "GET-" old-slot-reader))
                (get-name-string (c-macro-string get-name))
                (set-name (intern-prefixed "SET-" old-slot-reader)))
             
             ;; erzeuge die lesende Zugriffsfunktion
             ;; (<name>-<slot> objekt)
             ;;-----------------------
             (p1-def-call-out
              `(,slot-reader
                :arguments ((ffi:c-ptr ,original-name))
                :return-type ,slot-type 
                :name ,get-name-string)
              :macro t :intern t)
             
             ;; erzeuge die schreibenden Zugriffsfunktionen
             ;; (setf (<name>-<slot> objekt) value)
             ;;------------------------------------
             (p1-defun 
              `((L::setf ,slot-reader) (value object)
                (,set-name object value)))))
          
          ;; Unbenannte Handles
          ;;-------------------
          (new-handle
           ;; keine Zugriffsfunktion
           ;;-----------------------
           )
          
          ;; Unbenannte Strukturen
          ;;----------------------
          ((new-struct new-union)
           ;; Keine Zugriffsfunktion, aber
           ;; Zugriffsfunktionen fuer die Komponenten
           ;;----------------------------------------
           (ff-access-fun-old 
            slot-reader slot-type original-name old-slot-reader))
          
          ;; Arrays
          ;;-------
          (new-array
           ;; keine Zugriffsfunktion
           ;;-----------------------
           ;; *** to do *** aref-zugriff.
           ))))))

;;------------------------------------------------------------------------------
;; Zugriffsfunktionen fuer Arrays (Synonyme)
;;------------------------------------------------------------------------------
(defun ff-access-fun-array-old (name old-name old-type)
  (let* ((test-name (intern-postfixed old-name "-P"))
         (aref-name (intern-prefixed "AREF-" name))
         (old-aref-name (intern-prefixed "AREF-" old-name))
         (get-name (intern-prefixed "GET-" old-aref-name))
         (set-name (intern-prefixed "SET-" old-aref-name))
         (array-type (second old-type))
         (dimension-list (third old-type))
         dimension-desc
         and-list)
    
    (multiple-value-bind (key old-name old-type)
        (select-type name array-type array-type)
      (declare (ignore old-name old-type))
      
      (case key
        ((string primitive old-handle old-struct old-union old-array ptr)
         ;; Listen fuer die Zugriffe erzeugen
         ;;----------------------------------
         (dotimes (count (length dimension-list))
           (push (intern (format nil "X~A" count)) dimension-desc)
           (push `(typep (ffi:lisp-integer ,(intern (format nil "X~A" count))) 
                   '(integer 0 ,(nth count dimension-list))) and-list))
         (setq dimension-desc (reverse dimension-desc))
         
         (p1-defun
          `(,aref-name (objekt ,@dimension-desc)
            (if (and (,test-name objekt)
                     ,@and-list)
                (,get-name objekt ,@dimension-desc)
                (error "not a structured type: ~A" objekt))))
         
         ;; erzeuge die schreibende Zugriffsfunktion
         ;;-----------------------------------------
         (p1-defun 
          `((L::setf ,aref-name) (value objekt ,@dimension-desc)
            (if (and (,test-name objekt)
                     ,@and-list)
                (,set-name objekt ,@dimension-desc value)
                (error "not a structured type: ~A" objekt)))))))))

;;******************************************************************************
;; Konstanten fuer die Typ-Symbole definieren
;;******************************************************************************
(defun ff-constant-new (name)
  (let* ((constant (intern-postfixed (intern-prefixed "--" name) "--")))
    (p1-defconstant `(,constant ',name))))

(defun ff-constant-old (name old-name)
  (let* ((constant-new (intern-postfixed (intern-prefixed "--" name) "--"))
         (constant-old (intern-postfixed (intern-prefixed "--" old-name) "--")))
    (p1-defconstant `(,constant-new ,constant-old))))

;;******************************************************************************
;; Hilfsfunktionen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Funktionssignatur herausschreiben.
;;------------------------------------------------------------------------------
(defun ff-signatur (foreign-fun)
  (unless *ffi-include-flag*
    (let* ((argument-liste (?arguments foreign-fun))
           (c-name (?name foreign-fun))
           (return-type (?return-type foreign-fun))
           (*print-circle* nil))
      
      (push 
       (if (null argument-liste)
           (format nil "~A ~A(void);"
                   (convert-c-type-to-string return-type)
                   c-name)
           (format nil "~A ~A(~A~{, ~A~});"
                   (convert-c-type-to-string return-type)
                   c-name
                   (convert-c-type-to-string (first argument-liste))
                   (mapcar #'convert-c-type-to-string (rest argument-liste))
                   ))
       *ffi-signatures*))))

;;------------------------------------------------------------------------------
;; Bekommt einen C-Typ und macht daraus die C-Repraesentation dieses Types als
;; String.
;;------------------------------------------------------------------------------
(defun convert-c-type-to-string (type)
  (let* ((*print-circle* nil))
    
    (multiple-value-bind (key old-name old-type)
        (select-type nil type type)
      (case key
        (string "char *")
        (primitive (minus-to-space old-type))
        (void "void")
        (ptr (format nil "~A *" (convert-c-type-to-string (second type))))
        (new-struct 
         (format nil "struct {~%~:{~T~A ~A;~%~}}" 
                 (mapcar #'make-c-struct-component (rest old-type))))
        (new-union
         (format nil "union {~%~:{~T~A ~A;~%~}}"
                 (mapcar #'make-c-union-component (rest old-type))))
        (new-array
         (format nil "~A~{[~A]~}"
                 (convert-c-type-to-string (second old-type))
                 (third old-type)))
        (new-handle "void *")             ; oder "char *"
        (old-struct (c-name-string old-name))
        (old-union (c-name-string old-name))
        (old-array (c-name-string old-name))
        (old-handle "void *")             ; oder "char *"
        (lisp 
         (case old-type
           (:integer "long")
           (:short "short")
           (:short "short")
           (:int "int")
           (:long "long")
           (:character "char")))))))


;;------------------------------------------------------------------------------
;; Erzeugt eine Liste aus zwei Strings, die eine Komponente einer C-Struktur
;; enthaelt.
;;------------------------------------------------------------------------------
(defun make-c-struct-component (komponente)
  (let* ((k-slot (first komponente))
         (k-slot-name (c-name-string k-slot))
         (k-typ (second komponente)))
    
    (multiple-value-bind (key old-name old-type)
        (select-type nil k-typ k-typ)
      (declare (ignore old-name old-type))
      (case key
        (old-struct
         (list 
          (concatenate 'string "struct " (convert-c-type-to-string k-typ))
          k-slot-name))
        (old-union
         (list 
          (concatenate 'string "union " (convert-c-type-to-string k-typ))
          k-slot-name))
        (ptr
         (multiple-value-bind (key old-name old-type)
             (select-type nil (second k-typ) (second k-typ))
           (declare (ignore old-name old-type))
           (case key
             (old-struct
              (list 
               (concatenate 'string "struct " (convert-c-type-to-string k-typ))
               k-slot-name))
             (old-union
              (list 
               (concatenate 'string "union " (convert-c-type-to-string k-typ))
               k-slot-name))
             (otherwise
              (list
               (convert-c-type-to-string k-typ)
               k-slot-name)))))
        (otherwise
         (list 
          (convert-c-type-to-string k-typ)
          k-slot-name))))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun make-c-union-component (komponente)
  (let* ((k-slot (first komponente))
         (k-slot-name (c-name-string k-slot))
         (k-typ (second komponente)))

    (multiple-value-bind (key old-name old-type)
        (select-type nil k-typ k-typ)
      (declare (ignore old-name old-type))
      (case key
        (old-struct
         (list 
          (concatenate 'string "struct " (convert-c-type-to-string k-typ))
          k-slot-name))
        (old-union
         (list 
          (concatenate 'string "union " (convert-c-type-to-string k-typ))
          k-slot-name))
        (ptr
         (multiple-value-bind (key old-name old-type)
             (select-type nil (second k-typ) (second k-typ))
           (declare (ignore old-name old-type))
           (case key
             (old-struct
              (list 
               (concatenate 'string "struct " (convert-c-type-to-string k-typ))
               k-slot-name))
             (old-union
              (list 
               (concatenate 'string "union " (convert-c-type-to-string k-typ))
               k-slot-name))
             (otherwise
              (list
               (convert-c-type-to-string k-typ)
               k-slot-name)))))
        (otherwise
         (list 
          (convert-c-type-to-string k-typ)
          k-slot-name))))))

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
;; Nur fuer primitive Typen zu verwenden !!!
;; aus `c-unsigned-int' wird der typ `unsigned int'
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
  (let* ((*print-circle* nil))

    (format *interface-file-stream* "~A~%" arg)))

;;------------------------------------------------------------------------------
(provide "fftypes")
