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
;;; Funktion : Lader-Parser fuer FFI-Beschreibungsdateien.
;;; 
;;; $Revision: 1.30 $
;;; $Id: ffload.lisp,v 1.30 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Globale Variablen
;;------------------------------------------------------------------------------
(defvar *interface-file-stream*)
(defvar *interface-file-queue* (empty-queue)) 
                                        ; Liste der geladenen Definitionsdateien
(defvar *ffi-signatures* '())
(defvar *ffi-type-queue* (empty-queue))
(defvar *ffi-include-flag* NIL)
(defvar *ffi-need-savestack* NIL)
(defvar *ffi-include-queue* (empty-queue))

;;------------------------------------------------------------------------------
;; p1-load-foreign
;; Analysiert eine Beschreibungsdatei.
;; Folgende Konstrukte werden erkannt:
;;  - def-call-out           Definition einer Call-Out-Funktion,
;;  - def-call-in            Definition einer Call-In-Funktion,
;;  - def-c-type             Definition eines C-Types.
;;------------------------------------------------------------------------------
(defun p1-load-foreign (all-args)
  (let* ((name-of-file (first all-args))
         (pathname-of-file (merge-pathnames name-of-file))
         (extension (pathname-type pathname-of-file))
         definition-file
         form)

    ;;-------------------------------------------
    #+CLISP(let ((x "FFI")) (use-package x)) ; do not execute at compile time
    #-CLISP(use-package "FFI")


    ;; Alle weiteren Prototypen und Typen sind nicht aus einer
    ;; Include-datei extrahiert. Wenn doch, dann wird dieses Flag
    ;; gesetzt.
    ;;---------
    (setq *ffi-include-flag* nil)

    ;; Den Filenamen bei Bedarf mit der Endung ".def" versehen
    ;;--------------------------------------------------------
    (unless (or (equal extension "def") (equal extension 'nil))
      (clicc-error "Wrong extension ``~a'' for LOAD-FOREIGN, expected ``def''"
                   extension))
    
    (setq definition-file (calc-def-file pathname-of-file))
    
    ;; Ist die Beschreibungsdatei nicht vorhanden, Fehler ausgeben
    ;;------------------------------------------------------------
    (unless (probe-file definition-file)
      (clicc-error "Unknown file: ~A." definition-file))

    ;; Die Definitionsdatei nur bearbeiten, wenn sie nicht schon einmal
    ;; bearbeitet wurde.
    ;;------------------
    (unless 
        (member definition-file
                (queue2list *interface-file-queue*) 
                :test #'string=)
      
      ;; File oeffnen und bearbeiten
      ;;----------------------------
      (with-open-file (def-file definition-file :direction :input)
        
        (clicc-message "Loading definition-file ~A" definition-file)
        
        ;; Solange S-Ausdruecke lesen, bis Datei zuende
        ;;---------------------------------------------
        (loop
         (setq form (clicc-read def-file))
         (if (or (eq '|eof| form)
                 ;; Kleiner Fehler in Allegro
                 ;; Manchmal wird kein '|eof| gesendet
                 ;;-----------------------------------
                 #+allegro-v4.1 (eq 'EOF form)
                 )
             (return)
             
             ;; Die Eintraege abarbeiten.
             ;;--------------------------
             (case (first form)

               (ffi:sys-include
                (p1-ffi-include (rest form) 'sys)
                (setq *ffi-include-flag* t))

               (ffi:user-include
                (p1-ffi-include (rest form) 'user)
                (setq *ffi-include-flag* t))
               
               (ffi:def-call-out
                   (p1-def-call-out (rest form)))
               
               (ffi:def-call-in
                   (p1-def-call-in (rest form))
                   (setq *ffi-need-savestack* t))
               
               (ffi:def-c-type
                   (p1-def-c-type (rest form)))
               
               (t
                (clicc-error "Unknown identifier in Signature: ~A" 
                             form)))))
        
        ;; Definitionsdatei in die Queue einhaengen, und damit
        ;; fuer eine weitere Benutzung sperren.
        ;;-------------------------------------
        (add-q definition-file *interface-file-queue*)))

;; Nur unusen, wenn auch geused wurde
;    #+CLISP(let ((x "FFI")) (unuse-package x)) ; do not execute at compile time
;    #-CLISP(unuse-package "FFI")))
))

;;------------------------------------------------------------------------------
(provide "ffload")
