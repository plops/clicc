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
;;; Funktion : Ein Typinferenzalgorithmus fuer die Zwischensprache LZS
;;;            Zuerst wird die Zwischensprache traversiert, dabei nach
;;;            Typen analysiert und mit Typinformationen versehen.
;;;            Auf Wunsch werden in einem zweiten Schritt Statistiken 
;;;            erstellt und ausgegeben.
;;;
;;; $Revision: 1.44 $
;;; $Id: timain.lisp,v 1.44 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "tidef")
(require "timisc")
(require "tidecl")
(require "tiimpdec")
(require "tiassert")
(require "tipass1")
(require "tipass2")
(require "tipass3")
(require "statistics")


;;------------------------------------------------------------------------------
;; Hauptfunktion des Typinferenzalgorithmus. 
;;
;; Vorbedingungen:
;;   Das uebergebene Modul muss ein Zwischensprach-Modul sein, und die 
;;   Seiteneffekt- (static-effect) und Kontrollflussinformationen (appfuns) 
;;   zu allen Applikationen muessen vorliegen.
;;
;; Nachbedingungen:
;;   In allen Typannotationen der Zwischensprachkonstrukte des uebergebenen 
;;   Moduls liegen in einem gewissen Sinne `korrekte' Typen.
;;
;; Erl"auterung:
;;   Es wird das in *module* enthaltene Zwischensprachenmodul analysiert. 
;;   In Abhaengigkeit des Intensitaetsgrades *ti-level* wird eine mehr oder
;;   weniger intensive Typinferenz durchgefuehrt.
;;   *ti-verbosity* ist der Gespraechigkeitsgrad der Typinferenz. 
;;------------------------------------------------------------------------------
(defun do-ti (&KEY ((:module    *module*      ) *module*      )
                   ((:level     *ti-level*    ) *ti-level*    )
                   ((:verbosity *ti-verbosity*) *ti-verbosity*))

  (unless *ti-type-declarations-are-initialized*
    (initialize-type-declarations)
    ;; (setf *ti-type-declarations-are-initialized* T)
    )
    
  (when (> *ti-verbosity* 0)
    (clicc-message "Type inference (level ~D)" *ti-level*))
  
  (prepare-type-inference *module*) 

  (when (do-type-inference-on-non-literals)
    (analyse-objects (append (?class-def-list *module*) (?all-funs *module*))))
                                              
  (when (> *ti-verbosity* 2)
    (do-statistics)))
  

    
;;------------------------------------------------------------------------------
;; Funktion zur Ausgabe des Typs einer definierten Funktion.
;;------------------------------------------------------------------------------
(defun write-function-type (a-defined-function)
  (let ((function-name (?symbol a-defined-function)))

    (cond ((zerop (?used a-defined-function))
           (clicc-message "~S is never used." function-name))
          (T
           
           (if (use-call-contexts a-defined-function)
               (clicc-message "The type of ~S is: " function-name)  
               (clicc-message "~S is used as: " function-name))

           (let* ((all-vars 
                   (?all-vars (?params a-defined-function)))
                  (parameter-types 
                   (mapcar #'output-type (mapcar #'?type all-vars)))
                  (result-type     
                   (output-type (?result-type a-defined-function))))

             (unless (endp parameter-types)
               (format t "~S" (first parameter-types))
               
               (dolist (type (rest parameter-types))
                 (format t " x ~S" type)))
             
             (format t " -> ~S~%" result-type))))

    nil))


;;------------------------------------------------------------------------------
;; Gibt die Funktionsbeschreibungen der in `a-function-list' enthaltenen 
;; Funktionen aus. Die Vorbesetzung dieser Liste ist die fun-list des *module*.
;;------------------------------------------------------------------------------
(defun list-function-types (&optional (a-function-list (?fun-list *module*)))
  (dolist (a-function a-function-list)
    (write-function-type a-function))
  (terpri))


;;------------------------------------------------------------------------------
;; Gibt die Funktionsbeschreibungen aller im Modul *module* definierten 
;; Funktionen aus.
;;------------------------------------------------------------------------------
(defun list-all-function-types ()
  (list-function-types (?all-funs *module*)))


;;------------------------------------------------------------------------------
;; Liefert die entsprechende Funktion aus der functions-liste
;;------------------------------------------------------------------------------
(defun get-function (name)
  (find name (?all-funs *module*) :key #'?symbol))

;;------------------------------------------------------------------------------
(provide "timain")
