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
;;; Funktion : Codegenerierung: Continuations, Spruenge
;;;
;;; $Revision: 1.19 $
;;; $Id: cgblock.lisp,v 1.19 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; let/cc
;;------------------------------------------------------------------------------
(defmethod cg-form ((form let/cc-form))
  (let ((cont (?cont form))
        (C-new_cont "new_cont")
        (C-caller_base "caller_base")
        (C-last_cont "last_cont"))
    (unless (?only-local cont)
      (C-blockstart)
      (C-VarDecl "CONTENV" C-new_cont)
      (C-PtrDecl "CL_FORM" C-caller_base)

      (C-Assign (CC-StructKomp C-new_cont "bind_top") "bind_top")
      (C-Assign (CC-StructKomp C-new_cont "last") C-last_cont)
      (C-MacroCall "LOAD_UNIQUE_TAG" (CC-StackTop))
      (cg-var-bind cont *stack-top*)
      (incf *stack-top*)

      (C-Assign C-caller_base
                (CC-Cast "CL_FORM *"
                         (CC-MacroCall "SETJMP"
                                       (CC-StructKomp C-new_cont "jmp_buf"))))
      (C-if (CC-op== C-caller_base "NULL"))
      (C-Blockstart)

      ;; Nicht von call_cont
      ;;--------------------
      (C-Assign C-last_cont (CC-Address C-new_cont)))

    ;; Die aktuelle Hoehe des Binding-Stacks merken, damit spaeter bei einem
    ;; lokalen Sprung bekannt ist, ob bzw. wieweit er abgearbeitet werden muss.
    ;; Merken, an welcher Stelle der lokale Sprung sein Resultat erzeugen soll.
    ;;-------------------------------------------------------------------------
    (setf (?binding-stack-level cont) *special-count*)
    (setf (?result-spec cont) *result-spec*)

    ;; Namen der Marke fuer lokale Spruenge vor
    ;; der Bearbeitung des Rumpfes bekanntgeben.
    ;;------------------------------------------
    (setf (?adr cont) (CC-NameConc "RETURN" (incf *cont-counter*)))

    ;; den Rumpf des Blocks uebersetzen
    ;;---------------------------------
    (cg-form (?body form))
    (when (eq *result-spec* 'C-bool)
      (to-bool-result))

    ;; Marke fuer lokale Spruenge generieren.
    ;;---------------------------------------
    (C-Label (?adr cont))

    (unless (?only-local cont)
      (C-Assign C-last_cont (CC-StructKomp C-new_cont "last"))
      (C-blockend)
      (C-else)
      (C-blockstart)
      (C-Assign C-last_cont (CC-StructKomp C-new_cont "last"))

      ;; Nicht lokaler Kontroll Transfer ?
      ;;---------------------------------
      (C-if (CC-MacroCall "EQ" (CC-caller-stack 0 C-caller_base) 
                          (CC-static cont)))
      (C-blockstart)

      ;; Passende Continuation gefunden,
      ;; Resultat an die gewuenschte Position kopieren.
      ;;-----------------------------------------------
      (case *result-spec*
        ((nil) nil)
        (C-BOOL (C-Assign C-bool_result (CC-make-bool 
                                         (CC-caller-stack 1 C-caller_base)))
                (setq *C-bool* C-bool_result))
        (T (C-copy (CC-caller-stack 1 C-caller_base) 
                   (CC-dest *result-spec*))))
      (C-blockend)
      (C-else)
      (C-blockstart)
      (C-Call "call_cont" C-caller_base)
      (C-blockend)
      (C-blockend)

      (decf *stack-top*)
      (C-blockend))))

;;------------------------------------------------------------------------------
;; Aufruf einer Continuation
;;------------------------------------------------------------------------------
(defmethod cg-app ((cont cont) args app)
  (when (?downfun-list app)
    (error "downfuns in continuation calls are not implemented"))

  (cond    
    ((eql *level* (?level cont))

     (let ((*result-spec* (?result-spec cont)))

       ;; Das Resultat gleich an der Resultatsposition der Cont. erzeugen.
       ;;-----------------------------------------------------------------
       (when (not (eql (length args) 1))
         (cg-error "continuation has been called with ~a arguments"
                   (length args))
         (when (null args)
           (push empty-list args)))
       (cg-form (first args))
       (when (eq *result-spec* 'C-bool)
         (to-bool-result)))
     (C-restore-special2 (?binding-stack-level cont))

     ;; An das Ende des Rumpfes von let/cc springen.
     ;;---------------------------------------------
     (C-goto (?adr cont)))

    (t

     ;; Kennung fuer nicht lokalen Sprung erzeugen
     ;;---------------------------------------
     (C-Copy (CC-static cont) (CC-StackTop))
     (incf *stack-top*)

     ;; Resultat erzeugen.
     ;;-------------------
     (let ((*result-spec* (stacktop-result-location)))
       (cg-form (first args)))
     (decf *stack-top*)

     (C-Call "call_cont" (CC-StackTop)))))

;;------------------------------------------------------------------------------
;; Verwendung einer Continuation als funktionales Argument
;;------------------------------------------------------------------------------
(defmethod cg-form ((form cont))
  (to-result-loc form))

;;------------------------------------------------------------------------------
;; tagbody (nur lokale Spruenge)
;;------------------------------------------------------------------------------
#+CMU
(defun gen-tag (tagged-form)
  (C-Label (?adr tagged-form)))

;;------------------------------------------------------------------------------
(defmethod cg-form ((form tagbody-form))

  ;; *special-count* : aktuelle Anzahl der special Variablen muss in GO
  ;; bekannt sein, um die neu gebundenen special Variablen restaurieren
  ;; zu koennen.
  ;;-------------------------------------------------------------------
  (setf (?binding-stack-level form) *special-count*)

  ;; Die Namen der Marken vor der Uebersetzung des Rumpfes bekanntgeben
  ;;-------------------------------------------------------------------
  (incf *tagbody-counter*)
  (let ((tag-counter 0))
    (dolist (tagged-form (?tagged-form-list form))
      (setf (?adr tagged-form)
            (CC-NameConc "M" *tagbody-counter* "_" (incf tag-counter)))))
    
  (let ((*result-spec* nil))
    (cg-form (?first-form form)))

  (labels (#-CMU
           (gen-tag (tagged-form)
             (C-Label (?adr tagged-form))))
              
    (mapc-progn-form-list (?tagged-form-list form)
                          #'(lambda (tagged-form)
                              (gen-tag tagged-form)
                              (let ((*result-spec* nil))
                                (cg-form (?form tagged-form))))

                          ;; Die letzte Tagged-Form liefert das Resultat,
                          ;; im Frontend gewoehnlich auf nil gesetzt.
                          ;;-----------------------------------------
                          #'(lambda (tagged-form)
                              (gen-tag tagged-form)
                              (cg-form (?form tagged-form))))))

;;------------------------------------------------------------------------------
;; go (nur lokale Spruenge)
;;------------------------------------------------------------------------------
(defmethod cg-form ((form tagged-form))
  (cond
    ((eql *level* (?level (?tagbody form)))

     ;; lokales GO
     ;; Vor dem Sprung die Eintraege auf dem Binding-Stack abarbeiten, die
     ;; seit der Aktivierung des Tagbody angelegt wurden.
     ;;-------------------------------------------------------------------
     (C-restore-special2 (?binding-stack-level (?tagbody form)))
     (C-goto (?adr form)))

    (t (error "go out of function should have been transformed ~
                  into call of continuation"))))

;;------------------------------------------------------------------------------
(provide "cgblock")
     
