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
;;; Inhalt   : Funktionen zum Aufspueren und Beseitigen von unbenutzten
;;;            Funktionen, Symbolen und benannten Konstanten.
;;;
;;; $Revision: 1.36 $
;;; $Id: delete.lisp,v 1.36 1994/11/22 14:49:16 hk Exp $
;;------------------------------------------------------------------------------

(in-package "CLICC")

(require "traverse")

;;------------------------------------------------------------------------------
(defvar *objects-to-delete* '(fun local-fun named-constant class))
(defvar *delete-verbosity* 2)
(defvar *unused-local-funs* nil)
(defvar *delete-path* nil)

;;------------------------------------------------------------------------------
;; Die Funktion search-and-delete-unused-objects ermittelt unbenutzte Objekte
;; im aktuellen Modul, gibt diese aus und entfernt sie.
;;------------------------------------------------------------------------------

(defun search-and-delete-unused-objects ()
  (let ((*unused-local-funs* '()))
    (set-used-slots *module*)
    (write-unused-objects *module*)
    (delete-unused-objects *module*)))

;;------------------------------------------------------------------------------
;; clear-all-used-slots setzt die used slots von Funktionen, Symbolen und
;; benannten Konstanten auf 0.
;;------------------------------------------------------------------------------

(defmethod clear-used-slot ((a-zws-object zws-object))
  (setf (?used a-zws-object) 0))

(defmethod clear-read-slot ((a-defined-named-const defined-named-const))
  (setf (?read a-defined-named-const) 0))

(defmethod clear-read-and-write-slot ((a-var var))
  (setf (?read a-var) 0
        (?write a-var) 0))

(defun clear-all-used-slots (a-module)
  (mapc #'clear-used-slot (?fun-list a-module))
  (when (?toplevel-forms a-module)
    (setf (?used (?toplevel-forms a-module)) 1))
  (mapc #'clear-read-slot (?named-const-list a-module))
  (mapc #'clear-used-slot (?sym-list a-module))
  (mapc #'clear-used-slot (?class-def-list a-module))
  (mapc #'clear-read-and-write-slot (?var-list a-module))
  (mapc #'clear-read-and-write-slot (?dyn-var-list a-module)))

;;------------------------------------------------------------------------------
;; is-used ermittelt, ob ein Zwischensprachobjekt benutzt wird.
;;------------------------------------------------------------------------------

(defmethod is-used ((a-zws-object zws-object))
  (plusp (?used a-zws-object)))

(defmethod is-used ((a-global-fun global-fun))
  (or (call-next-method)
      (?exported a-global-fun)
      (?call-in a-global-fun)))

(defmethod is-used ((a-sym sym))
  (or (call-next-method)
      (?exported a-sym)
      (constant-value-p a-sym)))

(defmethod is-used ((a-defined-named-const defined-named-const))
  (or (plusp (?read a-defined-named-const))
      (?exported a-defined-named-const)))

(defmethod is-used ((a-var var))
  (or (plusp (?write a-var))
      (plusp (?read a-var))))

(defmethod is-used ((a-local-static local-static))
  (or (> (?write a-local-static) 1)
      (plusp (?read a-local-static))))

(defmethod is-used ((a-global-static global-static))
  (or (call-next-method)
      (?exported a-global-static)))

(defmethod is-used ((a-imported-static imported-static))
  (or (call-next-method)
      (?exported a-imported-static)))

(defmethod ?used ((an-imported-fun imported-fun))
  most-positive-fixnum)

;;------------------------------------------------------------------------------
;; inc-used-slot erhoeht den Inhalt der used slots von Funktionen, Symbolen und
;; benannten Konstanten, wenn diese beim traversieren der toplevel-forms und der
;; exportierten Funktionen erreicht werden. Werden local-funs oder continuations
;; als unbenutzt erkannt, werden die entsprechenden labels- und let/cc-forms
;; entfernt. Deshalb muss jede Methode das resultierende Zwischensprachobjekt
;; zurueckliefern.
;;------------------------------------------------------------------------------

(defmethod inc-used-slot ((anything T) &optional write)
  (declare (ignore write)))

(defmethod inc-used-slot ((a-var var) &optional write)
  (unless (analysed-p a-var)
    (mark-as-analysed a-var)
    (clear-read-and-write-slot a-var))
  (if write
      (incf (?write a-var))
      (incf (?read a-var))))

(defmethod inc-used-slot ((a-dynamic dynamic) &optional write)
  (declare (ignore write))
  (call-next-method)
  (inc-used-slot (?sym a-dynamic)))

(defmethod inc-used-slot ((a-var-ref var-ref) &optional write)
  (inc-used-slot (?var a-var-ref) write))

(defmethod inc-used-slot ((a-defined-named-const defined-named-const)
                          &optional write)
  (unless (analysed-p a-defined-named-const)
    (mark-as-analysed a-defined-named-const)
    (inc-used-slot (?value a-defined-named-const)))
  (unless write
    (incf (?read a-defined-named-const))))

(defmethod inc-used-slot ((a-sym sym) &optional write)
  (declare (ignore write))
  (unless (analysed-p a-sym)
    (mark-as-analysed a-sym)
    (clear-used-slot a-sym)
    (when (constant-value-p a-sym)
      (inc-used-slot (?constant-value a-sym))))
  (incf (?used a-sym)))

(defmethod inc-used-slot ((a-structured-literal structured-literal)
                          &optional write)
  (declare (ignore write))
  (inc-used-slot (?value a-structured-literal))) ; Kommt genau einmal vor!

(defmethod inc-used-slot ((a-string string) &optional write)
  (declare (ignore write)))

(defmethod inc-used-slot ((an-array array) &optional write)
  (declare (ignore write))
  (let* ((total-size (array-total-size an-array))
         (flat-array (make-array total-size
                                 :displaced-to an-array
                                 :element-type (array-element-type an-array))))
    (dotimes (index total-size)
      (inc-used-slot (aref flat-array index)))))

(defmethod inc-used-slot ((a-cons cons) &optional write)
  (declare (ignore write))
  (inc-used-slot (car a-cons))
  (inc-used-slot (cdr a-cons)))

(defmethod inc-used-slot ((a-literal-instance literal-instance) &optional write)
  (declare (ignore write))
  (inc-used-slot (?class a-literal-instance))
  (mapc #'inc-used-slot (?value-list a-literal-instance)))

(defmethod inc-used-slot ((a-class-def class-def) &optional write)
  (declare (ignore write))
  (unless (analysed-p a-class-def)
    (mark-as-analysed a-class-def)
    (clear-used-slot a-class-def)
    (mapc #'inc-used-slot (rest (?value (?class-precedence-list a-class-def))))
    (mapc #'inc-used-slot (?slot-descr-list a-class-def))
    (inc-used-slot (?symbol a-class-def)))
  (incf (?used a-class-def)))

(defmethod inc-used-slot ((a-slot-desc slot-desc) &optional write)
  (declare (ignore write))
  (inc-used-slot (?symbol a-slot-desc))
  (inc-used-slot (?initform a-slot-desc))
  (mapc #'inc-used-slot (?initargs a-slot-desc)))

(defmethod inc-used-slot ((parameters params) &optional write)
  (declare (ignore write))
  (dolist (a-var (?var-list parameters))
    (inc-used-slot a-var 'write))
  (mapc #'inc-used-slot (?opt-list parameters))
  (when (?rest parameters)
    (inc-used-slot (?rest parameters) 'write))
  (mapc #'inc-used-slot (?key-list parameters)))

(defmethod inc-used-slot ((an-opt opt) &optional write)
  (declare (ignore write))
  (inc-used-slot (?var an-opt) 'write)
  (inc-used-slot (?init an-opt))
  (inc-used-slot (?suppl an-opt) 'write))

(defmethod inc-used-slot ((a-key key) &optional write)
  (declare (ignore write))
  (call-next-method)
  (inc-used-slot (?sym a-key)))

(defmethod inc-used-slot ((a-fun defined-fun) &optional write)
  (declare (ignore write))
  (unless (analysed-p a-fun)
    (mark-as-analysed a-fun)
    (clear-used-slot a-fun)
    (inc-used-slot (?params a-fun))
    (let ((*delete-path* (cons a-fun *delete-path*)))
      (inc-used-slot (?body a-fun)))
    (when (member 'local-fun *objects-to-delete*)
      (setf (?local-funs a-fun)
            (remove-if-not #'is-used (?local-funs a-fun)))))
  (incf (?used a-fun)))

(defmethod inc-used-slot ((an-app app) &optional write)
  (declare (ignore write))
  (inc-used-slot (?form an-app))
  (mapc #'inc-used-slot (?arg-list an-app)))

(defmethod inc-used-slot ((a-setq-form setq-form) &optional write)
  (declare (ignore write))
  (inc-used-slot (?location a-setq-form) 'write)
  (inc-used-slot (?form a-setq-form)))

(defmethod inc-used-slot ((a-progn-form progn-form) &optional write)
  (declare (ignore write))
  (mapc #'inc-used-slot (?form-list a-progn-form)))

(defmethod inc-used-slot ((an-if-form if-form) &optional write)
  (declare (ignore write))
  (inc-used-slot (?pred an-if-form))
  (inc-used-slot (?then an-if-form))
  (inc-used-slot (?else an-if-form)))

(defmethod inc-used-slot ((a-switch-form switch-form) &optional write)
  (declare (ignore write))
  (inc-used-slot (?form a-switch-form))
  (mapc #'inc-used-slot (?case-list a-switch-form))
  (inc-used-slot (?otherwise a-switch-form)))

(defmethod inc-used-slot ((a-labeled-form labeled-form) &optional write)
  (declare (ignore write))
  (inc-used-slot (?value a-labeled-form))
  (inc-used-slot (?form a-labeled-form)))

(defmethod inc-used-slot ((a-let*-form let*-form) &optional write)
  (declare (ignore write))
  (dolist (a-var (?var-list a-let*-form))
    (inc-used-slot a-var 'write))
  (mapc #'inc-used-slot (?init-list a-let*-form))
  (inc-used-slot (?body a-let*-form)))

(defmethod inc-used-slot ((a-labels-form labels-form) &optional write)
  (declare (ignore write))
  (dolist (a-local-fun (?fun-list a-labels-form))
    (clear-used-slot a-local-fun))
  (inc-used-slot (?body a-labels-form))
  (let ((used-local-funs '()))
    (dolist (a-local-fun (?fun-list a-labels-form))
      (if (is-used a-local-fun)
          (push a-local-fun used-local-funs)
          (push (describe-location a-local-fun) *unused-local-funs*)))
    (when (member 'local-fun *objects-to-delete*)
      (setf (?fun-list a-labels-form) used-local-funs))))

(defmethod inc-used-slot ((a-let/cc-form let/cc-form) &optional write)
  (declare (ignore write))
  (let ((cont (?cont a-let/cc-form)))
    (inc-used-slot cont 'write)
    (inc-used-slot (?body a-let/cc-form))))

(defmethod inc-used-slot ((a-tagbody-form tagbody-form) &optional write)
  (declare (ignore write))
  (dolist (a-tagged-form (?tagged-form-list a-tagbody-form))
    (clear-used-slot a-tagged-form))
  (inc-used-slot (?first-form a-tagbody-form))
  (dolist (a-tagged-form (?tagged-form-list a-tagbody-form))
    (inc-used-slot (?form a-tagged-form))))

(defmethod inc-used-slot ((a-tagged-form tagged-form) &optional write)
  (declare (ignore write))
  (incf (?used a-tagged-form)))

(defmethod inc-used-slot ((a-mv-lambda mv-lambda) &optional write)
  (declare (ignore write))
  (inc-used-slot (?params a-mv-lambda))
  (inc-used-slot (?arg a-mv-lambda))
  (inc-used-slot (?body a-mv-lambda)))

;;------------------------------------------------------------------------------
;; dec-used-slots aktualisiert den Inhalt der used-slots der Bestandteile von
;; Zwischensprachausdruecken, die wegoptimiert werden.
;;------------------------------------------------------------------------------

(defmethod dec-used-slot ((anything T) &optional write)
  (declare (ignore write)))

(defmethod dec-used-slot ((a-var var) &optional write)
  (if write
      (decf (?write a-var))
      (decf (?read a-var))))

(defmethod dec-used-slot ((a-dynamic dynamic) &optional write)
  (declare (ignore write))
  (call-next-method)
  (dec-used-slot (?sym a-dynamic)))

(defmethod dec-used-slot ((a-var-ref var-ref) &optional write)
  (dec-used-slot (?var a-var-ref) write))

(defmethod dec-used-slot ((a-defined-named-const defined-named-const)
                          &optional write)
  (unless write
    (decf (?read a-defined-named-const))))

(defmethod dec-used-slot ((a-sym sym) &optional write)
  (declare (ignore write))
  (decf (?used a-sym)))

(defmethod dec-used-slot ((a-structured-literal structured-literal)
                          &optional write)
  (declare (ignore write))
  (dec-used-slot (?value a-structured-literal)))

(defmethod dec-used-slot ((a-string string) &optional write)
  (declare (ignore write)))

(defmethod dec-used-slot ((an-array array) &optional write)
  (declare (ignore write))
  (let* ((total-size (array-total-size an-array))
         (flat-array (make-array total-size
                                 :displaced-to an-array
                                 :element-type (array-element-type an-array))))
    (dotimes (index total-size)
      (dec-used-slot (aref flat-array index)))))

(defmethod dec-used-slot ((a-cons cons) &optional write)
  (declare (ignore write))
  (dec-used-slot (car a-cons))
  (dec-used-slot (cdr a-cons)))

(defmethod dec-used-slot ((a-literal-instance literal-instance) &optional write)
  (declare (ignore write))
  (dec-used-slot (?class a-literal-instance))
  (mapc #'dec-used-slot (?value-list a-literal-instance)))

(defmethod dec-used-slot ((a-class-def class-def) &optional write)
  (declare (ignore write))
  (decf (?used a-class-def)))

(defmethod dec-used-slot ((a-slot-desc slot-desc) &optional write)
  (declare (ignore write))
  (dec-used-slot (?symbol a-slot-desc))
  (dec-used-slot (?initform a-slot-desc))
  (mapc #'dec-used-slot (?initargs a-slot-desc)))

(defmethod dec-used-slot ((parameters params) &optional write)
  (declare (ignore write))
  (dolist (a-var (?var-list parameters))
    (dec-used-slot a-var 'write))
  (mapc #'dec-used-slot (?opt-list parameters))
  (when (?rest parameters)
    (dec-used-slot (?rest parameters) 'write))
  (mapc #'dec-used-slot (?key-list parameters)))

(defmethod dec-used-slot ((an-opt opt) &optional write)
  (declare (ignore write))
  (dec-used-slot (?var an-opt) 'write)
  (dec-used-slot (?init an-opt))
  (dec-used-slot (?suppl an-opt) 'write))

(defmethod dec-used-slot ((a-key key) &optional write)
  (declare (ignore write))
  (call-next-method)
  (dec-used-slot (?sym a-key)))

(defmethod dec-used-slot ((a-fun defined-fun) &optional write)
  (declare (ignore write))
  (decf (?used a-fun)))

(defmethod dec-used-slot ((an-app app) &optional write)
  (declare (ignore write))
  (dec-used-slot (?form an-app))
  (mapc #'dec-used-slot (?arg-list an-app)))

(defmethod dec-used-slot ((a-setq-form setq-form) &optional write)
  (declare (ignore write))
  (dec-used-slot (?location a-setq-form) 'write)
  (dec-used-slot (?form a-setq-form)))

(defmethod dec-used-slot ((a-progn-form progn-form) &optional write)
  (declare (ignore write))
  (mapc #'dec-used-slot (?form-list a-progn-form)))

(defmethod dec-used-slot ((an-if-form if-form) &optional write)
  (declare (ignore write))
  (dec-used-slot (?pred an-if-form))
  (dec-used-slot (?then an-if-form))
  (dec-used-slot (?else an-if-form)))

(defmethod dec-used-slot ((a-switch-form switch-form) &optional write)
  (declare (ignore write))
  (dec-used-slot (?form a-switch-form))
  (mapc #'dec-used-slot (?case-list a-switch-form))
  (dec-used-slot (?otherwise a-switch-form)))

(defmethod dec-used-slot ((a-labeled-form labeled-form) &optional write)
  (declare (ignore write))
  (dec-used-slot (?value a-labeled-form))
  (dec-used-slot (?form a-labeled-form)))

(defmethod dec-used-slot ((a-let*-form let*-form) &optional write)
  (declare (ignore write))
  (dolist (a-var (?var-list a-let*-form))
    (dec-used-slot a-var 'write))
  (mapc #'dec-used-slot (?init-list a-let*-form))
  (dec-used-slot (?body a-let*-form)))

(defmethod dec-used-slot ((a-labels-form labels-form) &optional write)
  (declare (ignore write))
  (dec-used-slot (?body a-labels-form)))

(defmethod dec-used-slot ((a-let/cc-form let/cc-form) &optional write)
  (declare (ignore write))
  (let ((cont (?cont a-let/cc-form)))
    (dec-used-slot cont 'write)
    (dec-used-slot (?body a-let/cc-form))))

(defmethod dec-used-slot ((a-tagbody-form tagbody-form) &optional write)
  (declare (ignore write))
  (dec-used-slot (?first-form a-tagbody-form))
  (dolist (a-tagged-form (?tagged-form-list a-tagbody-form))
    (dec-used-slot (?form a-tagged-form))))

(defmethod dec-used-slot ((a-tagged-form tagged-form) &optional write)
  (declare (ignore write))
  (decf (?used a-tagged-form)))

(defmethod dec-used-slot ((a-mv-lambda mv-lambda) &optional write)
  (declare (ignore write))
  (dec-used-slot (?params a-mv-lambda))
  (dec-used-slot (?arg a-mv-lambda))
  (dec-used-slot (?body a-mv-lambda)))

;;------------------------------------------------------------------------------
;; Die Funktion set-used-slots zaehlt die angewandten Vorkommen von Funktionen,
;; Symbolen und benannten Konstanten, die von den toplevel-forms und den
;; exportierten Funktionen aus erreichbar sind.
;;------------------------------------------------------------------------------

(defun set-used-slots (a-module)
  (new-analyse-mark)
  (clear-all-used-slots a-module)
  ;; Bei Symbolen, die einen konstanten Wert enthalten, muss dieser ebenfalls
  ;; analysiert werden, auch wenn das Symbol nicht benutzt wird. (Wegen der
  ;; moeglichen Anwendung von 'symbol-value'.)
  (dolist (a-sym (?sym-list a-module))
    (when (constant-value-p a-sym)
      (mark-as-analysed a-sym)
      (inc-used-slot (?constant-value a-sym))))
  ;; Traversierung, ausgehend von den toplevel-forms:
  (inc-used-slot (?toplevel-forms a-module))
  ;; Annahme, dass alle call-in-Funktionen auch verwendet werden:
  (dolist (a-global-fun (?all-global-funs a-module))
    (when (?call-in a-global-fun)
      (inc-used-slot a-global-fun)))
  ;; Traversierung, ausgehend von den exportierten Funktionen:
  (dolist (a-global-fun (?fun-list a-module))
    (when (?exported a-global-fun)
      (inc-used-slot a-global-fun))))

(defun set-used-slots-for-cg ()
  (new-analyse-mark)
  (clear-all-used-slots *module*)
  (dolist (a-sym (?sym-list *module*))
    (when (constant-value-p a-sym)
      (mark-as-analysed a-sym)
      (inc-used-slot (?constant-value a-sym))))
  ;; Alle Funktionen werden analysiert, ob benutzt oder nicht.
  (dolist (a-fun (?all-funs *module*))
    (inc-used-slot a-fun)))

;;------------------------------------------------------------------------------
;; Die Funktion describe-location gibt eine Liste zurueck, die die Namen der
;; Funktion a-local-fun und der Funktionen, in der sie lokal definiert ist, in
;; der Schachtelungsreihenfolge von innen nach aussen enthaelt.
;;------------------------------------------------------------------------------

(defun describe-location (a-local-fun)
  (labels ((describe-location-internal (fun-path)
             (if (local-fun-p (first fun-path))
                 (cons (?symbol (first fun-path))
                       (describe-location-internal (rest fun-path)))
                 (list (?symbol (first fun-path))))))
    (cons (?symbol a-local-fun)
          (describe-location-internal *delete-path*))))
  
;;------------------------------------------------------------------------------
;; Die folgenden Funktionen dienen zum Loeschen der als unbenutzt erkannten
;; Funktionen, Symbolen und benannten Konstanten.
;;------------------------------------------------------------------------------

(defun delete-unused-funs (a-module)
  (when (member 'fun *objects-to-delete*)
    (setf (?fun-list a-module)
          (remove-if-not #'is-used (?fun-list a-module)))))

(defun delete-unused-syms (a-module)
  (when (member 'sym *objects-to-delete*)
    (setf (?sym-list a-module)
          (remove-if-not #'is-used (?sym-list a-module)))))

(defun delete-unused-named-consts (a-module)
  (when (member 'named-constant *objects-to-delete*)
    (setf (?named-const-list a-module)
          (remove-if-not #'is-used (?named-const-list a-module)))))

(defun delete-unused-classes (a-module)
  (when (member 'class *objects-to-delete*)
    (setf (?class-def-list a-module)
          (remove-if-not #'is-used (?class-def-list a-module)))))

(defun delete-unused-objects (a-module)
  (delete-unused-funs a-module)
  (delete-unused-syms a-module)
  (delete-unused-named-consts a-module)
  (delete-unused-classes a-module))

;;------------------------------------------------------------------------------
;; write-unused-objects gibt die Liste der unbenutzten Funktionen, Symbolen und
;; benannten Konstanten aus.
;;------------------------------------------------------------------------------
(defun list-names (objects)
  (mapcar #'get-object-name objects))

(defmethod get-object-name ((a-class class-def))
  (?symbol (?symbol a-class)))

(defmethod get-object-name ((an-object T))
  (?symbol an-object))

(defun list-unused-funs (a-module)
  (remove-if #'is-used (?fun-list a-module)))

(defun list-unused-syms (a-module)
  (remove-if #'is-used (?sym-list a-module)))

(defun list-unused-named-consts (a-module)
  (remove-if #'is-used (?named-const-list a-module)))

(defun list-unused-classes (a-module)
  (remove-if #'is-used (?class-def-list a-module)))

(defun write-unused-objects (a-module)
  (clicc-message-line)
  (let ((unused-funs (list-unused-funs a-module))
        (unused-syms (list-unused-syms a-module))
        (unused-named-consts (list-unused-named-consts a-module))
        (unused-classes (list-unused-classes a-module)))
    
    (when (> *delete-verbosity* 1)
      (when unused-syms
        (clicc-message "The unused symbols are:~%~S"
                       (list-names unused-syms)))
      (when unused-named-consts
        (clicc-message "The unused named constants are:~%~S"
                       (list-names unused-named-consts)))
      (when unused-funs
        (clicc-message "The unused functions are:~%~S"
                       (list-names unused-funs)))
      (when *unused-local-funs*
        (clicc-message "The unused local functions are:~%~S"
                       *unused-local-funs*))
      (when unused-classes
        (clicc-message "The unused classes are:~%~S"
                       (list-names unused-classes))))

    (when (> *delete-verbosity* 0)
      (clicc-message "~D unused symbol~:p found"
                     (length unused-syms))
      (clicc-message "~D unused named constant~:p found"
                     (length unused-named-consts))
      (clicc-message "~D unused function~:p found"
                     (length unused-funs))
      (clicc-message "~D unused local function~:p found"
                     (length *unused-local-funs*))
      (clicc-message "~D unused~:* ~[classes~;class~:;classes~] found"
                     (length unused-classes))))

  (clicc-message-line))

;;------------------------------------------------------------------------------
(provide "delete")
