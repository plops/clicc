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
;;; Funktion : Funktionen zur Optimierungen, die auf den Typinformationen
;;;            aufbauen.
;;;
;;; $Revision: 1.31 $
;;; $Id: tomain.lisp,v 1.31 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; to-1form optimiert if-Ausdr"ucke, bei denen der Wahrheitswert des
;; Pr"adikats bereits bekannt ist.
;; F"ur Ausdr"ucke, die den Typ bottom haben, wird eine Warnung ausgegeben.
;; Diese Optimierung wird erst f"ur "au"sere und danach f"ur innere Ausdr"ucke
;; vorgeommen, so da"s nicht ereichbare Zweige von If-Ausdr"ucken eliminiert
;; werden, bevor eine Warnung "uber den Bottom Typ ausgegeben werden kann.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defmethod to-1form ((form form)) form)

;;------------------------------------------------------------------------------
(defmethod to-1form ((an-if-form if-form))
  (let* ((pred (?pred an-if-form))
         (pred-type (?type pred))
         (then (?then an-if-form))
         (else (?else an-if-form))
         eliminated-case
         (result 
          (cond
            ;; Typfehler gefunden, Warnung erfolgt sp"ater
            ;;--------------------------------------------
            ((is-bottom-t pred-type) an-if-form)
            
            ((zs-subtypep pred-type null-t)
             (inc-stats 'then-optis)
             (dec-used-slot then)
             (setq eliminated-case "then")
             (make-instance 'progn-form 
                            :form-list (list pred else)
                            :type (?type else)))
            ((not (zs-subtypep null-t pred-type))
             (inc-stats 'else-optis)
             (dec-used-slot else)
             (setq eliminated-case "else")
             (make-instance 'progn-form
                            :form-list (list pred then)
                            :type (?type then)))
            ((if-form-p pred) (split an-if-form))
            (T an-if-form))))
    (when (and eliminated-case (> *optimize-verbosity* 1))
      (clicc-message "(in ~A) Eliminating unreachable ~A-case."
                     (?symbol *current-fun*)
                     eliminated-case))
    result))

(defun split (an-if-form)
  (let* ((inner-if (?pred an-if-form))
         (then (?then an-if-form))
         (else (?else an-if-form))
         (inner-then (?then inner-if))
         (inner-else (?else inner-if))
         (inner-then-type (?type inner-then))
         (inner-else-type (?type inner-else)))
    (cond
      ((and (not (zs-subtypep null-t inner-then-type))
            (zs-subtypep inner-else-type null-t))
       (inc-stats 'if-if-split)
       (setf (?then inner-if)
             (optimize-1form (make-progn-form :form-list (list inner-then then)
                                              :type (?type then))))
       (setf (?else inner-if)
             (optimize-1form (make-progn-form :form-list (list inner-else else)
                                              :type (?type else))))
       (optimize-1form inner-if))
      ((and (zs-subtypep inner-then-type null-t)
            (not (zs-subtypep null-t inner-else-type)))
       (inc-stats 'if-if-split)
       (setf (?then inner-if)
             (optimize-1form (make-progn-form :form-list (list inner-then else)
                                              :type (?type else))))
       (setf (?else inner-if)
             (optimize-1form (make-progn-form :form-list (list inner-else then)
                                         :type (?type then))))
       (optimize-1form inner-if))
      (T an-if-form))))

;;------------------------------------------------------------------------------
(provide "tomain")
