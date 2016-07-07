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
;;; Funktion : Codegenerierung, Special Form IF
;;;
;;; $Revision: 1.12 $
;;; $Id: cgif.lisp,v 1.12 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defvar *then-label*)
(defvar *else-label*)

;;------------------------------------------------------------------------------
(defmacro ensure-label (var prefix)
  `(unless ,var
    (setq ,var
     (CC-NameConc ,prefix (incf *if-counter*)))))

;;------------------------------------------------------------------------------
;; if test then [else]
;; Wenn pred wiederum einen oder mehrere if-Ausdr"ucke enthält, dann wird in
;; den "Asten der inneren if-Ausdr"ucke gleich zu dem entsprechenden then-
;; oder else-Teil gesprungen, wenn bekannt ist, da"s der Wert True oder False
;; ist.
;; Wenn zu beiden Zweigen nur gesprungen wird, dann wird kein C-if mehr
;; generiert, sondern am Ende des then-Teils wird ein Sprung hinter den
;; Else-Teil generiert, falls dieser nicht leer ist.
;;
;; Die Variable *FLAT-IFS* bestimmt, ob das C-else verwendet wird:
;; if <pred>        if !(<pred>) goto ELSE
;; {                {
;;    <then>           <then> goto ENDIF
;; }                }
;; else             ELSE:
;; {                {
;;    <else>          <else>
;; }                }
;;                  ENDIF:
;;------------------------------------------------------------------------------
(defmethod cg-form ((form if-form))
  (let ((then-label nil)
        (else-label nil)
        (endif-label nil)
        (need-if t)
        then-has-bool-result
        else-has-bool-result)

    (let ((*result-spec* 'C-bool)
          (*then-label* nil)
          (*else-label* nil)
          (*C-bool* 'none))
      (cg-form (?pred form))
      (setq then-label *then-label*
            else-label *else-label*)

      ;; Wenn *C-bool* nach der Übersetzung des Prädikats immer noch den
      ;; Wert 'none hat, dann wird kein Wert zuückgegeben und somit kein if
      ;; benötigt
      ;;--------------------------------------------------------------------
      (when (eq *C-bool* 'none)
        (setq need-if nil))

      (when need-if
        (if *FLAT-IFS*
            (C-flat-if *C-bool* (ensure-label else-label "ELSE"))
            (C-if *C-bool*))))

    (labels ((cg-then/else (form copy-to-bool-result)
               (cond
                 ((not (eq *result-spec* 'C-bool))
                  (cg-form form))
                 ((and *optimize* (zs-subtypep (?type form) null-t))
                  (let ((*result-spec* nil)) (cg-form form))
                  (unless *else-label*
                    (setq *else-label*
                          (CC-NameConc "ELSE" (incf *if-counter*))))
                  (C-goto *else-label*)
                  nil)
                 ((and *optimize* (not (zs-subtypep null-t (?type form))))
                  (let ((*result-spec* nil)) (cg-form form))
                  (unless *then-label*
                    (setq *then-label*
                          (CC-NameConc "THEN" (incf *if-counter*))))
                  (C-goto *then-label*)
                  nil)
                 (t (cg-form form)
                    (when copy-to-bool-result
                      (to-bool-result))
                    t))))

      (C-blockstart)
      (when then-label (C-label then-label))
      (setq else-has-bool-result
            (and (eq *result-spec* 'C-bool)
                 (not (and *optimize*
                           (or (zs-subtypep (?type (?else form)) null-t)
                               (not (zs-subtypep null-t
                                                 (?type (?else form)))))))))
      (setq then-has-bool-result
            (cg-then/else (?then form) else-has-bool-result))
      (C-blockend)

      (cond

        ;; Leerer Else-Teil: evtl. Label generieren
        ;;-----------------------------------------
        ((and (null *result-spec*) (null-form-p (?else form)))
         (when else-label (C-label else-label)))

        (T (if (and need-if (not *FLAT-IFS*))
               (C-else)
               (C-goto (ensure-label endif-label "ENDIF")))

           (C-blockstart)
           (when else-label (C-label else-label))
           (cg-then/else (?else form) then-has-bool-result)
           (C-blockend)
           (unless (and need-if (not *FLAT-IFS*)) (C-label endif-label))))

      ;; Wenn mindestens ein Zweig ein Resultat generiert hat und ein
      ;; Boolesches Resultat erwartet wird
      ;;----------------------------------
      (when (and (eq *result-spec* 'C-bool)
                 then-has-bool-result
                 else-has-bool-result)
        (setq *C-bool* C-bool_result)))))

;;------------------------------------------------------------------------------
;; Hilfsfunktion, weist der C-Variablen 'bool_result' den Wert des Ausdrucks,
;; der in *C-bool* angegeben ist zu.
;; Die Zuweisung "bool_result = bool_result;" wird unterdrueckt.
;; Wenn der gerade "ubersetzte Ausdruck kein Resultat generiert hat, dann wird
;; die Zuweisung auch unterdr"uckt.
;;------------------------------------------------------------------------------
(defun to-bool-result ()
  (unless (or (string= C-bool_result *C-bool*) (eq 'none *C-bool*))
    (C-Assign C-bool_result *C-bool*)
    (setq *C-bool* C-bool_result)))

;;------------------------------------------------------------------------------
(provide "cgif")
