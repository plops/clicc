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
;;; Funktion : Konfigurierung von CLICC
;;;
;;; $Revision: 1.25 $
;;; $Id: config.lisp,v 1.25 1995/11/30 11:07:48 uho Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Eine Liste der LISP-Dateien, die Definitionen enthalten, die vor einer
;; Uebersetzung geladen werden muessen. Sie werden in setup-clicc-pathnames
;; gesetzt.
;;------------------------------------------------------------------------------
(defvar *LIB-LISP-PATH*)
(defvar *LIB-SYS-PATH*)
(defvar *LIB-OBREP-PATH*)

;;------------------------------------------------------------------------------
;; Pfadnamen fuer CLiCC-Directories werden gesetzt.
;; Dies sind die Wurzel des CLiCC-Filebaums und die Stellen wo lisp.def/syntax
;; und sys.def/syntax zu finden sind. Die Pfadnamen fuer die Verzeichnisse des
;; Laufzeitsystems und den Quellcode des Compilers werden in clcload.lisp in
;; der Funktion setup-clcload-pathnames gesetzt.
;;------------------------------------------------------------------------------
(defun setup-clicc-pathnames () 
  ;; Das Verzeichnis bestimmen, an dem CLICC installiert ist. Zur Zeit wird das
  ;; Verzeichnis aus der Environment-Variablen CLICCROOT ausgelesen oder
  ;; anhand des Pfads der Datei .../clicc/src/compiler/clcload.lisp bestimmt.
  ;; Man koennte an dieser Stelle auch direkt einen Pfadnamen angeben.
  ;; ---------------------------------------------------------------------------
  (setq *CLICC-DIR*
        (let* ((pathname
                #-mcl
                 (merge-pathnames
                  (cond ((rt::c-getenv "CLICCROOT"))
                        (T (error
                            "Environment variable CLICCROOT is not set")))
                  "")
                 #+:mcl
                 (make-pathname
                  :directory
                  (butlast (pathname-directory ccl:*loading-file-source-file*)
                   2)))
               (dir (pathname-directory pathname))
               (type (pathname-type pathname))
               (name (pathname-name pathname)))
          (unless (listp dir)
            (error "Expected an structured directory where CLiCC is installed"))
          (when type
            (setq name (namestring (make-pathname :name name :type type))))
          (when name
            (setq dir (append dir (list name))))
          dir))
  (let ((lib-dir (append *CLICC-DIR* (list "lib"))))
    (setq *LIB-LISP-PATH* (make-pathname :name "lisp" :directory lib-dir))
    (setq *LIB-SYS-PATH* (make-pathname :name "sys" :directory lib-dir))
    (setq *LIB-OBREP-PATH*
          (make-pathname :name "obrep" :type "h" :directory lib-dir))))

(setup-clicc-pathnames)

;;------------------------------------------------------------------------------
;; Funktion zum Berechnen des Dateinamens, in die der Code fuer einzelne
;; Funktionen geschrieben wird.
;;------------------------------------------------------------------------------
(defconstant *MAX-ARCHIVE-NAME* 13)
(defun calc-fun-filename (fun-name base-name dir count)
  (make-pathname
   :directory dir
   :name
   (if (<= (length fun-name) *MAX-ARCHIVE-NAME*)
       fun-name
       (let* ((str (princ-to-string count))
              (max-len (- *MAX-ARCHIVE-NAME* (length str))))
         (when (> (length base-name) max-len)
           (setq base-name (subseq base-name 0 max-len)))
         (concatenate 'string base-name str)))
   :type
   "c"))

;;------------------------------------------------------------------------------
;; Funktion zum Berechnen des Dateinamens, wenn der Code auf mehrere Dateien
;; verteilt wird
;;------------------------------------------------------------------------------
(defconstant *MAX-FILE-NAME* 8)
(defun calc-next-C-filename (pathname count)
  (make-pathname
   :name
   (let ((base-name (pathname-name pathname)))
     (if (zerop count)
         base-name
         (let* ((str (princ-to-string count))
                (len (length str)))
           (when (> (length base-name) (- *MAX-FILE-NAME* len))
             (setq base-name (subseq base-name 0 (- *MAX-FILE-NAME* len))))
           (concatenate 'string base-name str))))
   :type "c"
   :defaults pathname))

;;------------------------------------------------------------------------------
;; Datenrepräsentationsverfahren (Objekt-Repräsentation)
;;------------------------------------------------------------------------------
(defvar *OBREP*)                        ; 1, 2 oder 3 möglich

;;------------------------------------------------------------------------------
;; Funktion zum Einlesen der obrep-nummer für die Objektrepräsentation
;;------------------------------------------------------------------------------
(defun define-obrep ()
  (with-open-file (f *LIB-OBREP-PATH* :direction :input)
    (setq *OBREP* (and (eql (read-char f) #\#)
                       (eq (read f) 'define)
                       (eq (read f) 'OBREP)
                       (read f))))
  (if (not (integerp *OBREP*)) (setq *OBREP* 1)))

(define-obrep)

;;------------------------------------------------------------------------------
(defvar *POSSIBLE-LISP-EXTENSIONS* '("lisp" "lsp" "cl"))

;;------------------------------------------------------------------------------
;; Konstanten des C-Zielsystems (Minimalwerte aus <limits.h>)
;;------------------------------------------------------------------------------
(defconstant C-most-negative-fixnum -2147483648)
(defconstant C-most-positive-fixnum  2147483647)


;;------------------------------------------------------------------------------
;; Fixnum-Test des Zielsystems
;; Dieser Test wird in der Funktion zs-type-of (tipass1.lisp) von der 
;; Typinferenz verwendet.
;;------------------------------------------------------------------------------
(defun target-fixnump (anything)
  (and (integerp anything)
       (>= anything C-most-negative-fixnum)
       (<= anything C-most-positive-fixnum)))


;;------------------------------------------------------------------------------
(provide "config")
