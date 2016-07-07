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
;;; Funktion : Codegenerierung,
;;;            - mv-lambda
;;;            - VALUES (inline)
;;;
;;; $Revision: 1.21 $
;;; $Id: cgvalues.lisp,v 1.21 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC") 

;;------------------------------------------------------------------------------
(defconstant MV-LIMIT 20)

;;------------------------------------------------------------------------------
;; mv-lambda
;;------------------------------------------------------------------------------
(defmethod cg-form ((form mv-lambda))
  (let* ((old-stack *stack-top*)
         (params (?params form))
         (par-spec (calc-par-spec params))
         (mv-spec (?mv-spec form)))
     
    ;; MV generieren
    ;;--------------
    (let ((*result-spec* (stacktop-result-location)))
      (cg-form (?arg form)))

    (cond
        
      ;; bekannte Anzahl von MV
      ;;-----------------------
      ((numberp mv-spec)
       (unless (= 0 mv-spec)
         (incf *stack-top*)
         (dotimes (i (1- mv-spec))
           (C-Copy (CC-mv_buf i) (CC-StackTop))
           (incf *stack-top*)))
       (cond
         ((>= par-spec 0)
          (unless (eql mv-spec par-spec)
            (cg-error "MV-LAMBDA: the number of arguments ~a ~
                       should be ~a" mv-spec par-spec)))
         (t (unless (>= mv-spec (1- (- par-spec)))
              (cg-error "MV-LAMBDA: the number of arguments ~a ~
                         should be >= ~a" mv-spec (1- (- par-spec))))
            (C-Blockstart)
            (C-VarDecl "int" C-nargs)
            (C-Assign C-nargs mv-spec)))
         
       (unless (= 1 mv-spec)
         (C-resetMV)))

      ;; unbekannte Anzahl von MV 
      ;;--------------------------
      (t (incf *stack-top*)
         (C-MVToStack C-mv_count *stack-top*)
         (cond
           ((>= par-spec 0)
            (C-if (CC-op!= C-mv_count par-spec))
            (C-Blockstart)
            (C-Abort "ILLEGAL_ARGS")
            (C-Blockend))
           (t (C-Blockstart)
              (C-VarDecl "int" C-nargs)
              (C-Assign C-nargs C-mv_count)
              (let ((min (1- (- par-spec))))
                (when (> min 0)
                  (C-if (CC-op< C-mv_count min))
                  (C-Blockstart)
                  (C-Abort "TOO_FEW_ARGS")
                  (C-Blockend)))))
         (C-resetMV)))
    (setq *stack-top* old-stack)

    (C-blockstart)
    (let ((*rest-optimization* *rest-optimization*))
      (cg-params params (>= (calc-par-spec params) 0)
                 (rest-optimization-p (?params form) (?body form) nil))
      (cg-form (?body form)))
    (C-blockend)
    (setq *stack-top* old-stack)

    (unless (>= par-spec 0)
      (C-Blockend))))
    

;;------------------------------------------------------------------------------
;; Erzeugt Inline-code fuer die System-Fkt. VALUES.
;;------------------------------------------------------------------------------
(defun cg-values (arg-list app)
  (let ((mv-count (length arg-list))
        (*stack-top* *stack-top*))
    
    (if (or (not (?mv-used app)) (eql mv-count 1))
    
        ;; keine Multiple Values erzeugen
        ;;-------------------------------
        (case mv-count
          (0 (cg-form empty-list))
          (1 (cg-form (first arg-list)))
          (T (opt-args arg-list)        ; veraendert *stack-top* !
             (cg-args arg-list -1)
             (stacktop-to-result-loc)))
    
        ;; Multiple Values erzeugen
        ;;-------------------------
        (case mv-count
          (0 (cg-form empty-list)
             (C-SetMV 0))
      
          ;; Alle MV zunaechst auf dem Stack erzeugen, weil beim Evaluieren der
          ;; Argumente evtl. mv_buf uebertschrieben wird.
          ;;-Beachte:
          ;; Resultat nicht auf fun-result Position erzeugen, weil dadurch
          ;; evtl.  eine lokale Variable ueberschrieben wird, die fuer MV noch
          ;; benoetigt wird.
          ;;--------------------------------------------------------------------
          (t (when (> mv-count MV-LIMIT)
               (cg-error "VALUES is called with ~a arguments, ~
                        but only ~a are allowed"
                         mv-count MV-LIMIT)
               (setq mv-count MV-LIMIT))
             (opt-args arg-list)        ; veraendert *stack-top* !
             (cg-args arg-list -1)
             (stacktop-to-result-loc)
             (dotimes (i (1- mv-count))
               (C-copy (CC-Stack (+ *stack-top* i 1)) (CC-mv_buf i)))
             (C-SetMV mv-count))))))

;;------------------------------------------------------------------------------
(provide "cgvalues")

