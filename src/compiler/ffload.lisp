;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Lader-Parser fuer FFI-Beschreibungsdateien.
;;; 
;;; $Revision: 1.18 $
;;; $Log: ffload.lisp,v $
;;; Revision 1.18  1993/11/03  11:45:30  pm
;;; Inkonsistenzen in den Symbolnamen behoben.
;;;
;;; Revision 1.17  1993/07/26  16:43:50  pm
;;; Anlegen der Interface-datei eingefügt
;;;
;;; Revision 1.16  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.15  1993/06/10  09:32:09  pm
;;; Quelltext aufgeraeumt
;;;
;;; Revision 1.14  1993/05/31  17:02:28  pm
;;; Bearbeitung von def-call-in eingefuegt
;;;
;;; Revision 1.13  1993/05/23  17:49:59  pm
;;; Aufruf von def-c-type um das Argument package erweitert
;;;
;;; Revision 1.12  1993/05/13  12:42:54  pm
;;; Gleichen Fehler nochmal korrigiert.
;;;
;;; Revision 1.11  1993/05/13  12:00:51  pm
;;; Fehler behoben
;;;
;;; Revision 1.10  1993/05/12  08:38:05  pm
;;; packages korrigiert
;;;
;;; Revision 1.9  1993/05/06  14:32:58  pm
;;; (use-package FFI) herausgenommen
;;;
;;; Revision 1.8  1993/03/10  12:47:24  pm
;;; Kleinigkeiten geaendert
;;;
;;; Revision 1.7  1993/02/17  11:02:19  hk
;;; *clicc-package* durch *package* ersetzt.
;;;
;;; Revision 1.6  1993/02/16  15:23:33  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.5  1993/01/18  10:56:02  pm
;;; Inkonsistens behoben
;;;
;;; Revision 1.4  1992/12/01  15:10:04  pm
;;; kleine Aenderungen
;;;
;;; Revision 1.3  1992/11/10  10:24:35  pm
;;; Fehler ausgemaerzt
;;;
;;; Revision 1.2  1992/11/05  10:45:44  pm
;;; p1-load-foreign eingebaut
;;;
;;; Revision 1.1  1992/10/19  09:29:46  pm
;;; Initial revision
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Globale Variablen
;;------------------------------------------------------------------------------
(defvar *interface-file-stream* NIL)    ; aktuelle Definitions-Datei
(defvar *interface-file-queue* (empty-queue)) 
                                        ; Liste der geladenen Definitionsdateien

;;------------------------------------------------------------------------------
;; p1-load-foreign
;; Analysiert eine Beschreibungsdatei.
;; Folgende Konstrukte werden erkannt:
;;  - foreign-package-name
;;  - def-call-out
;;  - def-call-in
;;  - def-c-type
;;------------------------------------------------------------------------------
(defun p1-load-foreign (all-args)
  (let* ((name-of-file (first all-args))
         (file-name (pathname-name name-of-file))
         (extension (pathname-type name-of-file))
         (directory (directory-namestring name-of-file))
         (*PACKAGE* (find-package "FFI"))
         definition-file
         interface-file
         form)
    
    ;; Den Filenamen bei Bedarf mit der Endung ".def" versehen
    ;;--------------------------------------------------------
    (unless (or (equal extension "def") (equal extension 'nil))
      (clicc-error "Wrong extension for LOAD-FOREIGN: ~A." extension))
    
    (setq definition-file (concatenate 'string directory file-name ".def"))
    (setq interface-file (concatenate 'string directory file-name "-ffi.h"))
    
    ;; Ist die Beschreibungsdatei nicht vorhanden, Fehler ausgeben
    ;;------------------------------------------------------------
    (unless (probe-file definition-file)
      (clicc-error "Unknown file: ~A." definition-file))
    
    ;; Die Definitionsdatei nur bearbeiten, wenn sie nicht schon einmal
    ;; bearbeitet wurde.
    ;;------------------
    (unless (member interface-file *interface-file-queue* :test #'string=)
      
      ;; File oeffnen und bearbeiten
      ;;----------------------------
      (with-open-file (def-file definition-file :direction :input)
        
        ;; C-Header-Datei oeffnen
        ;;-----------------------
        (with-open-file (*interface-file-stream* 
                         interface-file
                         :direction :output
                         :if-exists :supersede)
          
          (clicc-message "Load definition-file ~A" definition-file)
          
          (loop
           (setq form (clicc-read def-file))
           (if (or (eq '|eof| form)
                   #+allegro-v4.1 (eq 'EOF form) ;little bug in acl
                   )
               (return)
               
               ;; Die Eintraege abarbeiten.
               ;;--------------------------
               (case (first form)
                 (ffi:foreign-package-name
                  (let ((package-name (second form)))
                    (setq *package* (find-package package-name))
                    (unless *package* 
                      (setq *package* (make-package package-name)))
                    (use-package "FFI" *package*)
                    (when (third form)
                      (clicc-warning "skipping extra arguments"))))
                 
                 (ffi:def-call-out
                     (export (p1-def-call-out (rest form)) *package*))
                 
                 (ffi:def-call-in
                     (p1-def-call-in (rest form)))
                 
                 (ffi:def-c-type
                     (p1-def-c-type (rest form) *package*))
                 
                 (t
                  (clicc-error "Not implemented: ~A" (car form))))))
          
          (add-q interface-file *interface-file-queue*))))))

;;------------------------------------------------------------------------------
(provide "ffload")
