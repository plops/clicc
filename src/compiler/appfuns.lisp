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
;;; Funktion : search-fun-calls ermittelt, zu welchen Funktionen die form einer
;;;            app-form ausgewertet werden kann. Ausserdem wird zu jeder
;;;            Funktion bestimmt, in welchen Funktionen sie aufgerufen werden
;;;            und ob alle Aufrufstellen bekannt sind.
;;;
;;; $Revision: 1.23 $
;;; $Id: appfuns.lisp,v 1.23 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC") 

;;------------------------------------------------------------------------------
(defun search-fun-calls ()
  (sfc-fun-list (cons (?toplevel-forms *module*) (?fun-list *module*)))
  (mapc #'sfc-class (?class-def-list *module*)))

;;------------------------------------------------------------------------------
;; Die Variable enthält
;; - die Applikation, deren Slot form gerade analysiert wird, oder
;; - das Symbol 'no-caller, wenn das Resultat des Ausdrucks nicht als
;;   Funktion aufgerufen wird oder
;; - das Symbol 'unknown-caller, wenn nicht bekannt ist, ob das Resultat 
;;   als Funktion aufgerufen wird.
;;------------------------------------------------------------------------------
(defvar *current-app*)

;;------------------------------------------------------------------------------
(defun sfc-fun-list (fun-list)
  (mapc #'reset-caller-slots fun-list)
  (mapc #'sfc-fun fun-list))
            
;;------------------------------------------------------------------------------
(defun sfc-fun (a-fun)
  (let ((*current-fun* a-fun))
    (sfc-params (?params a-fun))
    (sfc-unknown-caller (?body a-fun))))

;;------------------------------------------------------------------------------
(defun sfc-class (a-class-def)
  (dolist (a-slot-desc (?slot-descr-list a-class-def))
    (unless (or (null (?initform a-slot-desc))
                (eq (?initform a-slot-desc) :unbound))
      (sfc-unknown-caller (?initform a-slot-desc)))))

;;------------------------------------------------------------------------------
(defun reset-caller-slots (a-fun)
  (setf (?called-by a-fun) nil)
  (setf (?unknown-caller a-fun)
        (if (global-fun-p a-fun)
            (?exported a-fun)
            nil)))

;;------------------------------------------------------------------------------
(defun sfc-params (params)
  (dolist (a-opt (?opt-list params))
    (sfc-unknown-caller (?init a-opt)))
  (dolist (a-key (?key-list params))
    (sfc-unknown-caller (?init a-key))))

;;------------------------------------------------------------------------------
;; Analyse eines Ausdrucks in einem Kontext, in dem nicht bekannt ist, wie
;; oder ob das Resultat verwendet wird. Es ist insbesondere nicht bekannt, ob
;; das Resultat aufgerufen wird.
;;------------------------------------------------------------------------------
(defun sfc-unknown-caller (arg)
  (let ((*current-app* 'unknown-caller))
    (sfc-form arg)))

;;------------------------------------------------------------------------------
;; Analyse eines Ausdrucks in einem Kontext, in dem das Resultat nicht
;; aufgerufen wird.
;;------------------------------------------------------------------------------
(defun sfc-no-caller (a-form)
  (let ((*current-app* 'no-caller))
    (sfc-form a-form)))

;;---------------------------------------------------------------------
;; Setzt die Slots called-funs und other-funs von Applikationen,
;; die Slots called-by und unknown-caller von Funktionen und
;; den Slot unknown-caller von Continuations.
;;---------------------------------------------------------------------
(defgeneric sfc-form (form))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((an-app app))
  (let ((form (?form an-app))
        (arg-list (?arg-list an-app))
        (*current-app* an-app))

    (setf (?called-funs an-app) ())
    (setf (?other-funs an-app) nil)
    (sfc-form
     (if (and (special-sys-fun-p form) (?special-caller form))
              
         ;; Bei Iterationsfunktionen sowie apply und funcall interessiert
         ;; nicht die form, sondern der erste Parameter, d.h. die von dieser
         ;; Funktion applizierten Funktionen. Der called-funs-Slot aendert in
         ;; diesem Fall seine Bedeutung entsprechend.
         ;;------------------------------------------
         (pop arg-list)
         form))

    (mapc #'sfc-unknown-caller arg-list))

  ;; Wir wissen nicht, ob oder welche Funktion das Resultat der
  ;; Applikation ist
  ;;-----------------------------------------------------------
  (when (app-p *current-app*)
    (setf (?other-funs *current-app*) T)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-defined-fun defined-fun))
  (cond ((app-p *current-app*)
         (pushnew *current-fun* (?called-by a-defined-fun))
         (pushnew a-defined-fun (?called-funs *current-app*)))
        ((eq 'unknown-caller *current-app*)
         (setf (?unknown-caller a-defined-fun) T))))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-foreign-fun foreign-fun))
  (when (app-p *current-app*)
    (setf (?other-funs *current-app*) t)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-fun fun))
  (when (app-p *current-app*)
    (pushnew a-fun (?called-funs *current-app*))))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-cont cont))
  (cond ((app-p *current-app*)
         (pushnew a-cont (?called-funs *current-app*)))
        ((eq 'unknown-caller *current-app*)
         (setf (?unknown-caller a-cont) T))))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-var-ref var-ref))
  
  ;; Wir wissen nicht, ob oder welche Funktion das Resultat der
  ;; Variablen ist
  ;;-----------------------------------------------------------
  (when (app-p *current-app*)
    (setf (?other-funs *current-app*) T)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-setq-form setq-form))
  (sfc-unknown-caller (?form a-setq-form)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-let*-form let*-form))
  (mapc #'sfc-unknown-caller (?init-list a-let*-form))
  (sfc-form (?body a-let*-form)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-labels-form labels-form))
  (sfc-fun-list (?fun-list a-labels-form))
  (sfc-form (?body a-labels-form)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-progn-form progn-form))
  (mapc-progn-form-list (?form-list a-progn-form)
                        #'sfc-no-caller
                        #'sfc-form))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((an-if-form if-form))
  (sfc-no-caller (?pred an-if-form))
  (sfc-form (?then an-if-form))
  (sfc-form (?else an-if-form)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-switch-form switch-form))
  (sfc-no-caller (?form a-switch-form))
  (mapc #'(lambda (a-labeled-form) (sfc-form (?form a-labeled-form)))
        (?case-list a-switch-form))
  (sfc-form (?otherwise a-switch-form)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-let/cc-form let/cc-form))
  (setf (?unknown-caller (?cont a-let/cc-form)) nil)
  
  ;; Wir wissen nicht, ob die Continuation weitere Funktionen als Resultat hat.
  ;;---------------------------------------------------------------------------
  (when (app-p *current-app*)
    (setf (?other-funs *current-app*) T))
  (sfc-form (?body a-let/cc-form)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-tagbody-form tagbody-form))
  (sfc-no-caller (?first-form a-tagbody-form))
  (mapc-tagged-form-list (?tagged-form-list a-tagbody-form)
                         #'sfc-no-caller
                         #'sfc-form))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-tagged-form tagged-form)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-mv-lambda mv-lambda))
  (sfc-unknown-caller (?arg a-mv-lambda))
  (sfc-params (?params a-mv-lambda))
  (sfc-form (?body a-mv-lambda)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-defined-named-const defined-named-const))
  (sfc-form (?value a-defined-named-const)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((an-imported-named-const imported-named-const))
  (when (app-p *current-app*)
    (setf (?other-funs *current-app*) T)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-class class-def)))

;;------------------------------------------------------------------------------
(defmethod sfc-form ((a-literal literal)))

;;------------------------------------------------------------------------------
(provide "appfuns")
