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
;;; Funktion : Start und Abschluss von Pass 1
;;;
;;; $Revision: 1.88 $
;;; $Id: p1main.lisp,v 1.88 1994/12/13 16:02:29 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "p1macro")   ; muss vor "p1tlf" stehen !!
(require "p1foreign")
(require "ffload")
(require "p1tlf")
(require "p1lambda")
(require "p1spform")
(require "p1type")
(require "p1decls")
(require "p1struct")
(require "p1eval")
(require "p1generic")
(require "p1class")
(require "sexport")

;;------------------------------------------------------------------------------
;; Initialisierung fuer das Frontend
;;------------------------------------------------------------------------------
(defun init-pass1 ()  
  (setq *CLICC-REQUIRED*  ())           ; Liste der uebersetzten Module
  (setq *PACKAGE* (find-package (if *lisp-module* "LISP" "USER")))
  (let ((name (if *module-compiler*
                  (pathname-name *FILENAME*)
                  "main")))
    (setq *module* (make-module
                    :fun-list (empty-queue)
                    :class-def-list (empty-queue)
                    :named-const-list (empty-queue)
                    :var-list (empty-queue)
                    :sym-list (empty-queue)
                    :toplevel-forms nil
                    :package-list (empty-queue)
                    :loaded-modules (empty-queue)
                    :name name)))
  
  (addnew-q (find-package "KEYWORD") (?package-list *module*))

  (setq *SYNTAX-EXPORT* ()))            ; Liste der syntakt. exp. Konstrukte

;;------------------------------------------------------------------------------
;; Pass 1 des COMMON-LISP nach C Compilers.
;;------------------------------------------------------------------------------
(defun pass_1 ()
  
  ;; Alten Dispatch Makro fuer Strukturen retten.
  ;;---------------------------------------------
  (let ((original-s-reader (get-dispatch-macro-character #\# #\S))
        (*features* '(:CLiCC)))
    
    ;; Neuen Dispatch Makro fuer Strukturen setzen.  Durch die Angabe eines
    ;; eigenen #S-Readers kann der Compiler konstante Strukturen einlesen und
    ;; verarbeiten.
    ;;-----------------------------------------------------------------------
    (set-dispatch-macro-character #\# #\S #'p1-s-reader)
    
    (unwind-protect
         (pass_1-of-file *FILENAME*)

      ;; Protected: Ruecksetzen des Dispatch Makros
      ;;-------------------------------------------
      (set-dispatch-macro-character #\# #\S original-s-reader)))

  (p1-end))

;;------------------------------------------------------------------------------
;; Liest einen Lisp Ausdruck aus 'lisp-stream'.
;;------------------------------------------------------------------------------
(defun clicc-read (lisp-stream)
  (read lisp-stream nil '|eof|))

;;------------------------------------------------------------------------------
;; Wendet Pass 1 auf die Daten an, die CLICC-READ liefert.
;;------------------------------------------------------------------------------
(defun pass_1-of-data (&optional lisp-stream)
  (let (form
        (old-package *PACKAGE*))

    (setq *FUN-NAME* "top-level-forms")
    
    (loop
     (setq form (clicc-read lisp-stream))
     (if (eq '|eof| form)
         (return)
         (p1-top-level-form form)))
    
    (unless (or (eq *PACKAGE* old-package) *MODULE-COMPILER*)
      (save-toplevel-form
       (p1-form
        `(L::SETQ L::*PACKAGE* (L::FIND-PACKAGE ,(package-name old-package)))))
      (setq *PACKAGE* old-package))))

;;------------------------------------------------------------------------------
;; Wendet Pass 1 auf die Daten aus der spezifizierten Datei an.
;;------------------------------------------------------------------------------
(defun pass_1-of-file (pathname)
  (let ((*CLICC-FILENAME* (clc-probe-file pathname)))
    (if (null *CLICC-FILENAME*)

        (clc-error "File ~A does not exist.~%" pathname)

        (with-open-file (lisp-stream *CLICC-FILENAME* :direction :input)
          (clicc-message "File ~S" *CLICC-FILENAME*)      
          (pass_1-of-data lisp-stream)))))

;;------------------------------------------------------------------------------
;; Beendet Pass 1 des Compilers
;;------------------------------------------------------------------------------
(defun p1-end ()
  (unless (slot-boundp *module* 'package)
    (setf (?package *module*) (find-package "USER"))
    (add-q (find-package "USER") (?package-list *module*)))
  (when *lisp-module*
    (addnew-q (find-package "RT") (?package-list *module*))
    (addnew-q (find-package "FFI") (?package-list *module*)))
  (finalize-fftypes)
  
  (finalize-classes)
  (finalize-generic-funs)
  (symbolize-class-names)
  (check-undef-funs)
  (dequeue-module)
  (generate-init-fun)
  (collect-packages)
  (export-classes)
  (export-funs)
  (finalize-call-in-funs)

  ;; Symbole aus Package RT unexporten, damit write-syntax-export
  ;; rt:: schreibt.
  ;;---------------
  (when *lisp-module*
    (mapc #'(lambda (sym)
              (when (and (?exported sym)
                         (eq *runtime-package* (symbol-package (?symbol sym))))
                (unexport (?symbol sym) (symbol-package (?symbol sym)))))
          (?sym-list *module*)))
  (when (and *MODULE-COMPILER* (not *inline-module*)) (write-syntax-export)))

;;------------------------------------------------------------------------------
;; Die Top-Level Forms zu der Initialisierungsfunktion des Moduls
;; zusammenfassen
;;------------------------------------------------------------------------------
(defun generate-init-fun ()
  (labels
      ((init-lisp ()
         (list (make-app 
                :form (make-imported-fun
                       :symbol (make-symbol "INIT-LISP")
                       :adr "Ilisp"
                       :par-spec 0
                       :write-list -1)   ; has side effect
                :arg-list ())))
       
       (init-sym/packg ()
         (p1-form
          (if (null (?sym-list *module*))

              ;; Nur Packages generieren, keine Symbole vorhanden
              ;;-------------------------------------------------
              `(L::PROGN
                ,@ (mapcar #'(lambda (p)
                               `(rt::ensure-package ,(package-name p)))
                    (?package-list *module*)))

              ;; Packages generieren und in die Package-Cell der Symbole
              ;; eintragen
              ;;----------
              `(rt::setup-symbols-iterator
                (L::QUOTE ,(?symbol (first (?sym-list *module*))))
                (L::vector
                 ,@ (mapcar #'(lambda (p)
                                `(rt::ensure-package ,(package-name p)))
                     (?package-list *module*)))))))
       
       (calc-setup-sym/packg ()
         (if (or *inline-module*

                 ;; kleines Laufzeitsystem ohne Package-Operationen ?
                 ;;--------------------------------------------------
                 (null (get-global-fun 'L::find-package)))
             ()
             (let ((set-*package*
                    (p1-form
                     `(L::setq L::*package*
                       (L::find-package
                        ,(package-name (second (?package-list *module*))))))))

               (if (member (find-package "USER") (?package-list *module*))
                   (list (init-sym/packg)
                         (p1-form `(L:use-package "LISP" "USER"))
                         set-*package*)
                   (list (init-sym/packg)
                         set-*package*))))))
    
    (setf (?toplevel-forms *module*)
          (make-global-fun
           :symbol 'toplevel-forms
           :params (make-params :var-list nil
                                :opt-list nil
                                :rest nil
                                :key-list nil
                                :allow-other-keys
                                nil
                                :all-vars nil)
           :par-spec 0
           :mv-spec 1
           :body (let ((forms (if *lisp-module*
                                  (append
                                   (get-toplevel-form-list)
                                   (calc-setup-sym/packg))
                                  (append
                                   (unless *module-compiler* (init-lisp))
                                   (calc-setup-sym/packg)
                                   (get-toplevel-form-list)))))
                   (if forms
                       (make-progn-form :form-list forms)
                       empty-list))))))

;;------------------------------------------------------------------------------
;; Ablegen aller Package-Informationen
;;------------------------------------------------------------------------------
(defun collect-packages ()
  
  ;; Im Zwischencode treten nur noch Package-Names auf.
  ;;---------------------------------------------------
  (setf (?package-list *module*)
        (mapcar #'package-name (?package-list *module*))))

;;------------------------------------------------------------------------------
;; Umwandeln der Slots von *module* von einer Queue in eine Liste
;;------------------------------------------------------------------------------
(defun dequeue-module ()
  (setf (?package-list *module*) (queue2list (?package-list *module*)))
  (setf (?fun-list *module*) (queue2list (?fun-list *module*)))
  (setf (?class-def-list *module*) (queue2list (?class-def-list *module*)))
  (setf (?named-const-list *module*) (queue2list (?named-const-list *module*)))
  (setf (?var-list *module*) (queue2list (?var-list *module*)))
  (setf (?sym-list *module*) (queue2list (?sym-list *module*)))
  (setf (?loaded-modules *module*) (queue2list (?loaded-modules *module*)))
  (setf (?dyn-var-list *module*) 
        (mapcar #'cdr (?dyn-vars *GLOBAL-ENVIRONMENT*))))

;;------------------------------------------------------------------------------
;; Setzen des exported Flags von Funktionen
;;------------------------------------------------------------------------------
(defun export-funs ()
  (dolist (fun (?fun-list *module*))
    (let ((symbol (?symbol fun)))
      (when (consp symbol) (setq symbol (second symbol)))
      (when (symbol-package symbol)
        (multiple-value-bind (s status)
            (find-symbol (symbol-name symbol) (symbol-package symbol))
          (declare (ignore s))
          (when (eq :external status)
            (setf (?exported fun) t)))))))

;;------------------------------------------------------------------------------
;; Pruefen auf referenzierte aber nicht definierte Funktionen.
;; Fehlende Funktionen werden automatisch um eine Definition ergaenzt,
;; die einen Laufzeitfehler verursacht.
;;------------------------------------------------------------------------------
(defun check-undef-funs () 
  (dolist (nc (queue2list (?named-const-list *module*)))
    (when (eq (?value nc) :forward)
      (clicc-warning "Function ~s is referenced but not defined." (?symbol nc))
      (let ((*SDF* T))
        (p1-top-level-form
         `(L::defun ,(?symbol nc) (L::&rest args)
           (L::error
            "Function ~s is not defined. It has been called with arguments ~%~s"
            ,(format nil "~a" (?symbol nc))
            args)))))))

;;------------------------------------------------------------------------------
(provide "p1main")
