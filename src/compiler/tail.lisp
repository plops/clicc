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
;;; Funktion : Tail-rekursive Aufrufe werden in geeigneter Form in
;;;            Spruenge umgewandelt.
;;; 
;;; $Revision: 1.21 $
;;; $Id: tail.lisp,v 1.21 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Z"ahler f"ur erfolgreiche Tail-Rekursions-Eliminierungen
;;------------------------------------------------------------------------------
(defvar *app-counter*)

;;--------------------------------------------------------------------------
(defun tail-rec-module ()
  (let ((*app-counter* 0)
        (*level* 0))
    (clicc-message "Tail recursion elimination ...")
    (dolist (fun (?fun-list *module*))
      (tail-rec-fun fun))
    (clicc-message "~s tail recursive calls eliminated"
                   *app-counter*)))

;;--------------------------------------------------------------------------
(defun closure-uses-vars (local-fun-list)
  (dolist (local-fun local-fun-list)
    (when (or (and (eq :closure (?closure local-fun))
                   (some #'(lambda (var)
                             (eql (?level var) *level*))
                         (?free-lex-vars local-fun)))
              (closure-uses-vars (?local-funs local-fun)))
      (return t)))
  nil)

;;------------------------------------------------------------------------------
;; Die Marke, f"ur den sprung an den Anfang der Funktion
;;------------------------------------------------------------------------------
(defvar *body-tag*)

;;--------------------------------------------------------------------------
;; Die Tail-Rekursions-Eliminierung wird nur auf Funktionen angewendet, die
;; keine Closures mit gef"ahrdeten freien Variablen bilden und keine
;; Rest-Parameter haben.
;; Der Rumpf der Funktion wird in ein Let/cc-Konstrukt umgewandelt.
;; Dieses Let/cc-Konstrukt enth"alt einen Tagbody mit nur einem Tag.
;; In diesem Tag wird die Continuation auf den neuen Rumpf der Funktion,
;; indem Tail-rec-Aufrufe durch Sprünge ersetzt werden, aufgerufen.
;; (block end
;;   (tagbody
;;     start
;;     (return-from end <body>)))
;;--------------------------------------------------------------------------
(defun tail-rec-fun (fun)
  (unless (or (?rest (?params fun))
              (closure-uses-vars (?local-funs fun)))

    (let* ((*body-tag* nil)
           (*current-fun* fun)
           (body (transform-tail-calls (?body fun))))
      (when *body-tag*

        (clicc-message "tail recursion in ~s" (?symbol fun))
        (incf *app-counter*)
        (let* ((new-tagbody (make-tagbody-form
                             :first-form empty-list
                             :tagged-form-list (list *body-tag*)
                             :level *level*))
               (cont (make-cont :level *level* :read 1 :mv-spec (?mv-spec fun)
                                :only-local t :unknown-caller nil))

               (new-body (make-let/cc-form :cont cont
                                           :body new-tagbody)))
          (setf (?tagbody *body-tag*) new-tagbody)

          (setf (?form *body-tag*) (make-app :form cont
                                             :arg-list (list body)
                                             :mv-used t
                                             :downfun-list ()
                                             :called-funs (list cont)
                                             :other-funs nil))
          (setf (?body fun) new-body))))))

;;--------------------------------------------------------------------------
;; Ein Tail-rekursiver Aufruf der Form (f arg1 ... argN) wird umgeformt
;; in  (let ((local-arg1 arg1)
;;                ...
;;           (local-argN argN))
;;         (setq P1 local-arg1)
;;                ...
;;         (setq PN local-argN)
;;         (go start))
;;--------------------------------------------------------------------------
(defun optimize-tail-call (fun arg-list)
  (unless *body-tag* (setq *body-tag* (make-tagged-form :used 0)))
  (incf (?used *body-tag*))
  (multiple-value-bind (var-list init-list subst-map)
      (let ((*only-application* nil))
        (bind-arguments (?params fun) arg-list))
    (make-let*-form
     :var-list var-list
     :init-list init-list
     :body (let ((form-list (list *body-tag*)))
             (dolist (subst subst-map)
               (push (update-old-with-new subst) form-list))
             (make-progn-form :form-list form-list)))))

;;------------------------------------------------------------------------
;; Diese Funktion liefert fuer ein Paar (var . local-var) eine
;; setq-form (setq old var-ref.local-var).
;;------------------------------------------------------------------------
(defun update-old-with-new (pair-of-twin-vars)
  (let* ((old (make-var-ref :var (car pair-of-twin-vars)))
         (new (make-var-ref :var (cdr pair-of-twin-vars))))
    (make-setq-form :location old :form new)))

;;------------------------------------------------------------------------
;; dynamic-var-bound-p überprüft ob eine Liste von Variablen eine dynamische
;; Variable enthält.
;;------------------------------------------------------------------------
(defun dynamic-var-bound-p (list-of-var)
  (if (null list-of-var)
      nil
      (if (dynamic-p (car list-of-var))
          T
          (dynamic-var-bound-p (cdr list-of-var)))))

;;------------------------------------------------------------------------
;; transform-tail-calls traversiert eine Ausdruck und wendet auf alle
;; tail-rekursivenden Aufrufe von *current-fun* die Funktion
;; optimize-tail-call an.
;; Dies geschieht nicht f"ur R"umpfe von Let-Ausdr"ucken und
;; MV-lambda-Ausdr"ucken, die dynamische Variablen binden, da die dynamische
;; Bindung verloren geht, wenn man den rekursiven Aufruf durch einen Sprung
;; ersetzt
;;-------------------------------------------------------------------------

(defmethod transform-tail-calls ((an-app app))
  (let ((fun (?form an-app)))
    (if (eq fun *current-fun*)
        (optimize-tail-call fun (?arg-list an-app))
        an-app)))

(defmethod transform-tail-calls ((a-progn-form progn-form))
  (let ((last-form (last (?form-list a-progn-form))))
    (setf (car last-form) (transform-tail-calls (car last-form)))
    a-progn-form))

(defmethod transform-tail-calls ((tagbody tagbody-form))
  (let ((last-tagged-form (car (last (?tagged-form-list tagbody)))))
    (setf (?form last-tagged-form)
          (transform-tail-calls (?form last-tagged-form))))
  tagbody)

;;--------------------------------------------------------------------------
;; In der rechten Seite einer Zuweisung können keine rekursive Aufrufe durch
;; Sprünge ersetzt werden, weil die letzte Berechnung die Zuweisung an die
;; Variable ist.
;;--------------------------------------------------------------------------
(defmethod transform-tail-calls ((a-setq-form setq-form))
  a-setq-form)

(defmethod transform-tail-calls ((a-let-form let*-form))
  (unless (dynamic-var-bound-p (?var-list a-let-form))
    (setf (?body a-let-form) (transform-tail-calls (?body a-let-form))))
  a-let-form)

(defmethod transform-tail-calls ((a-mv-lambda mv-lambda))
  (unless (dynamic-var-bound-p (?all-vars (?params a-mv-lambda)))
    (setf (?body a-mv-lambda) (transform-tail-calls (?body a-mv-lambda))))
  a-mv-lambda)

(defmethod transform-tail-calls ((a-switch-form switch-form))
  (dolist (one-case (?case-list a-switch-form))
    (setf (?form one-case) (transform-tail-calls (?form one-case))))
  (setf (?otherwise a-switch-form)
        (transform-tail-calls (?otherwise a-switch-form)))
  a-switch-form)

(defmethod transform-tail-calls ((a-let/cc-form let/cc-form))
  (setf (?body a-let/cc-form ) (transform-tail-calls (?body a-let/cc-form)))
  a-let/cc-form)

(defmethod transform-tail-calls ((a-if-form if-form))
  (setf (?then a-if-form) (transform-tail-calls (?then a-if-form)))
  (setf (?else a-if-form) (transform-tail-calls (?else a-if-form)))
  a-if-form)

(defmethod transform-tail-calls ((a-labels labels-form))
  (let ((*level* (1+ *level*)))
    (dolist (fun (?fun-list a-labels))
      (tail-rec-fun fun)))
  (setf (?body a-labels) (transform-tail-calls (?body a-labels)))
  a-labels)

;;------------------------------------------------------------------------
;; Die anderen Ausdr"ucke enthalten keine Komponenten, die Tail-rekursive
;; Aufrufe sind.
;;------------------------------------------------------------------------
(defmethod transform-tail-calls ((a-cont cont))
  a-cont)

(defmethod transform-tail-calls ((any-thing-else form))
  any-thing-else)

;;------------------------------------------------------------------------------
(provide "tail")
