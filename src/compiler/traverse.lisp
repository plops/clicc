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
;;; Funktion : Traversieren von Zwischensprachkonstrukten
;;;
;;; $Revision: 1.23 $
;;; $Id: traverse.lisp,v 1.23 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Die folgenden Funktionen dienen zur Erzeugung und Benutzung von
;;  Analysemarken.
;;------------------------------------------------------------------------------

(defvar *analyse-mark* 'analyse-mark)

(defun new-analyse-mark ()
  (setq *analyse-mark* (gensym)))

(defun analysed-p (a-zws-object)
  (eq (?analysed a-zws-object) *analyse-mark*))

(defun mark-as-analysed (a-zws-object)
  (setf (?analysed a-zws-object) *analyse-mark*))

;;------------------------------------------------------------------------------
;; Globale Variablen fuer die Traversierung der Zwischensprache:
;;------------------------------------------------------------------------------

;; Analysepfad, um Rekursionen zu entdecken
(defvar *tr-analyzation-path*)


;; Listen der Funktionen, die beim Traversieren der Zwischensprache an jedem 
;; Knoten vor bzw. nach eintritt in die Teilausdruecke appliziert werden sollen.
(defvar *tr-before-funs* '())
(defvar *tr-after-funs* '())

;; Enthaelt das Praedikat, das bestimmt, ob der form-slot einer Applikation
;; traversiert werden soll.
(defvar *tr-app-form-p* #'(lambda (an-app) (declare (ignore an-app)) T))


(defmethod is-defined-fun-without-recursion-p ((a-fun fun))
  nil)

(defmethod is-defined-fun-without-recursion-p ((a-defined-fun defined-fun))
  (not (member a-defined-fun *tr-analyzation-path*)))


;; Enthaelt das Praedikat, das bestimmt, ob der body einer Funktion traversiert
;; werden soll.
(defvar *tr-fun-body-p* #'is-defined-fun-without-recursion-p)

;; Enthaelt das Praedikat, das bestimmt, welche Funktionen bereits an der Stelle
;; ihrer Definition traversiert werden sollen. (Von welchen Funktionen aus
;; startet die Traversierung?)
(defvar *tr-fun-selector* #'(lambda (a-fun) (declare (ignore a-fun)) T))

;;------------------------------------------------------------------------------
;; Traversiere das Modul und appliziere an jedem Knoten die in *tr-before-funs*
;; und *tr-after-funs* angegebenen Funktionen.
;;------------------------------------------------------------------------------
(defun traverse-module (a-module
                        &key
                        ((:before-funs *tr-before-funs*) *tr-before-funs*)
                        ((:after-funs *tr-after-funs*) *tr-after-funs*)
                        ((:fun-selector *tr-fun-selector*) *tr-fun-selector*)
                        ((:tr-app-form-p *tr-app-form-p*) *tr-app-form-p*)
                        ((:tr-fun-body-p *tr-fun-body-p*) *tr-fun-body-p*))

  (setf *tr-analyzation-path* '())

  (dolist (a-fun (?all-global-funs a-module))
    (when (funcall *tr-fun-selector* a-fun)
      (traverse-function a-fun))))

;;------------------------------------------------------------------------------
;; Es folgen die Methoden zu der generischen Funktion `traverse-zs', die
;; zu jedem Zwischensprachkonstrukt die entsprechenden Sprachteile analysiert.
;;------------------------------------------------------------------------------
(defgeneric traverse-zs (analysable-object))


;;------------------------------------------------------------------------------
;; Diese Methode wird fuer die verschiedenen Zwischensprachausdruecke geeignet
;; spezialisiert.
;;------------------------------------------------------------------------------
(defmethod traverse-zs ((anything T))
  #+PCL
  (when (functionp anything)
    (return-from traverse-zs anything))
  (internal-error 'traverse-zs "can't traverse ~S." anything))


;;------------------------------------------------------------------------------
;; In Makroexpansionsfunktionen und auch NUR dort treten auch Funktionen des
;; Wirtssystems auf. Beim Traversieren werden sie ignoriert. 
;;------------------------------------------------------------------------------
#-PCL                        ; siehe (traverse-zs T)
(defmethod traverse-zs ((a-function function)))

(defmethod traverse-zs ((a-zws-object zws-object)))


;;------------------------------------------------------------------------------
;; Wenn *tr-fun-body-p* erfuellt ist, wird der Rumpf der Funktion traversiert.
;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-fun fun))
  (when (funcall *tr-fun-body-p* a-fun)
    (traverse-function a-fun)))


;;------------------------------------------------------------------------------
;; An jedem Zwischensprachknoten werden die Funktionen aus *tr-before-funs*
;; und *tr-after-funs* auf den Knoten appliziert.
;;------------------------------------------------------------------------------
(defmethod traverse-zs :before ((a-zws-object zws-object))
           (dolist (a-function *tr-before-funs*)
             (funcall a-function a-zws-object)))

(defmethod traverse-zs :after ((a-zws-object zws-object))
           (dolist (a-function *tr-after-funs*)
             (funcall a-function a-zws-object)))


;;------------------------------------------------------------------------------
(defmethod traverse-function ((a-form form)))

;;------------------------------------------------------------------------------
;; Die Funktion a-fun wird in den Analysepfad eingetragen und ihr Rumpf
;; traversiert.
;;------------------------------------------------------------------------------
(defmethod traverse-function ((a-fun fun))
  (let ((*tr-analyzation-path*  (cons a-fun *tr-analyzation-path*))
        (params                 (?params a-fun)))
    (dolist (param (append (?opt-list params) (?key-list params)))
      (traverse-zs (?init param)))
    (traverse-zs (?body a-fun))))

;;------------------------------------------------------------------------------
;; Force analysis with args type top_t when call-in
;;------------------------------------------------------------------------------
(defmethod traverse-function ((a-global-fun global-fun))
  (when (?call-in a-global-fun)
    (dolist (var (?var-list (?params a-global-fun)))
      (update-type-f (?type var) top-t)))
  (call-next-method))

;;------------------------------------------------------------------------------
;; Wenn *tr-app-form-p* erfuellt ist, wird die form der Applikation traversiert.
;;------------------------------------------------------------------------------
(defmethod traverse-zs ((an-app app))
  (mapc #'traverse-zs (?arg-list an-app))
  (when (funcall *tr-app-form-p* an-app)
    (traverse-zs (?form an-app))))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-setq-form setq-form))
  (traverse-zs (?location a-setq-form))
  (traverse-zs (?form a-setq-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-progn-form progn-form))
  (mapc #'traverse-zs (?form-list a-progn-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((an-if-form if-form))
  (traverse-zs (?pred an-if-form))
  (traverse-zs (?then an-if-form))
  (traverse-zs (?else an-if-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-switch-form switch-form))
  (traverse-zs (?form a-switch-form))
  (mapc #'traverse-zs (?case-list a-switch-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-labeled-form labeled-form))
  (traverse-zs (?value a-labeled-form))
  (traverse-zs (?form a-labeled-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-labels-form labels-form))
  (dolist (a-fun (?fun-list a-labels-form))
    (when (funcall *tr-fun-selector* a-fun)
      (traverse-function a-fun)))
  (traverse-zs (?body a-labels-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-let*-form let*-form))
  (mapc #'traverse-zs (?init-list a-let*-form))
  (traverse-zs (?body a-let*-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-let/cc-form let/cc-form))
  (traverse-zs (?body a-let/cc-form)))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-tagbody-form tagbody-form))
  (traverse-zs (?first-form a-tagbody-form))
  (mapc #'traverse-zs (mapcar #'?form (?tagged-form-list a-tagbody-form))))

;;------------------------------------------------------------------------------
(defmethod traverse-zs ((a-mv-lambda mv-lambda))
  (traverse-zs (?body a-mv-lambda))
  (traverse-zs (?arg a-mv-lambda)))

;;------------------------------------------------------------------------------

(provide "traverse")
