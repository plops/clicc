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
;;; Funktion : Auf die Seiteneffektanalyse beruhende Optimierungen.
;;;
;;; $Revision: 1.18 $
;;; $Id: seomain.lisp,v 1.18 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")
(require "titypes")

;;------------------------------------------------------------------------------
;; Nur wenn die seiteneffektfreie Form kein Literal ist, wird die Beseitigung
;; gezaehlt.
;;------------------------------------------------------------------------------

(defun inc-seo-stats (form id)
  (unless (literal-p form)
    (inc-stats id)))

(defun inc-eval-stats (form)
  (if (and (app-p form) (fun-p (?form form)))
      (case (?symbol (?form form))
        (L::eq (inc-stats 'eq-elis))
        (L::not (inc-stats 'not-elis))
        (otherwise (inc-seo-stats form 'fun-evals)))
      (inc-seo-stats form 'fun-evals)))

;;------------------------------------------------------------------------------
;; eliminate-it-p ueberprueft, ob eine Form eliminiert werden darf. Das ist der
;; Fall, wenn die Form keine Seiteneffekte hat und den Kontrollfluss nicht
;; aendert (kein Sprung oder Programmabbruch).
;;------------------------------------------------------------------------------

(defun eliminate-it-p (form)
  (and (is-side-effect-free form)
       (not (is-bottom-t (?type form)))))

;;------------------------------------------------------------------------------
;; An var-refs, named-const etc. kann nichts optimiert werden.
;;------------------------------------------------------------------------------

(defmethod seo-1form ((a-form form))
  a-form)

;;------------------------------------------------------------------------------
;; Seiteneffektfreie Ausdruecke werden zu NIL vereinfacht, wenn das Resultat
;; nicht benoetigt wird. Seiteneffektfreie Ausdruecke, deren Resultat benoetigt
;; wird, werden zu NIL oder T vereinfacht, wenn sie mit NULL bzw. T-SYMBOL
;; getypt sind.
;;------------------------------------------------------------------------------

(defmethod seo-1form :around ((a-form form))
           (if *result-used*
               (if (eliminate-it-p a-form)
                   (let ((type (?type a-form)))
                     (cond
                       ((type-eq null-t type)
                        (inc-eval-stats a-form)
                        (dec-used-slot a-form)
                        empty-list)
                       ((type-eq t-symbol-t type)
                        (inc-eval-stats a-form)
                        (dec-used-slot a-form)
                        (get-symbol-bind 'L::T))
                       (T (call-next-method))))
                   (call-next-method))
               (if (eliminate-it-p a-form)
                   (progn
                     (inc-seo-stats a-form 'seo-optis)
                     (dec-used-slot a-form)
                     empty-list)
                   (call-next-method))))
                        
;;------------------------------------------------------------------------------
(defmethod seo-1form ((a-progn-form progn-form))
  (let ((form-queue (empty-queue))
        (forms (?form-list a-progn-form)))
    (dolist (a-form (butlast forms))
      (unless (eliminate-it-p a-form)
        (add-q a-form form-queue)))
    (let ((a-form (first (last forms))))
      (unless (and (not *result-used*) (eliminate-it-p a-form))
        (add-q a-form form-queue)))
    
    (if (empty-queue-p form-queue)
        empty-list
        (let* ((new-forms (queue2list form-queue))
               (form-n-1 (first (last (butlast new-forms))))
               (form-n (last-q form-queue)))
          (when (and form-n-1           ; Dieser Fall entsteht aus 'dolist'.
                     (null-form-p form-n)
                     (type-eq null-t (?type form-n-1)))
            (setq new-forms (butlast new-forms)))
          (setf (?form-list a-progn-form) new-forms)
          a-progn-form))))

;;------------------------------------------------------------------------------

(provide "seomain")
