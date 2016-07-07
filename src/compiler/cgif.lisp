;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Codegenerierung, Special Form IF
;;;
;;; $Revision: 1.9 $
;;; $Log: cgif.lisp,v $
;;; Revision 1.9  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.8  1993/02/16  15:50:38  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.7  1993/01/21  13:49:12  uho
;;; Optionale Generierung flacher ifs eingefuehrt (Funktion cd-if-flat)
;;;
;;; Revision 1.6  1992/09/25  17:20:12  kl
;;; Auf die neue Repraesentation der einfachen Literale umgestellt.
;;;
;;; Revision 1.5  1992/09/21  11:18:52  hk
;;; Die eigentliche C-Codegenerierung uebersichtlicher gestaltet
;;;
;;; Revision 1.4  1992/08/10  11:58:10  hk
;;; *mv-spec* und *mv-produced* gestrichen, da Analyse komplett in Pass3.
;;;
;;; Revision 1.3  1992/08/07  11:55:20  hk
;;; Dateikopf verschoenert.
;;;
;;; Revision 1.2  1992/06/04  07:11:20  hk
;;; Nach Umstellung auf die Lisp nahe Zwischensprache, Syntax-Fehler
;;; sind schon beseitigt
;;;
;;; Revision 1.1  1992/03/24  16:54:56  hk
;;; Initial revision
;;;-----------------------------------------------------------------------------

(in-package "CLICC")     

;;------------------------------------------------------------------------------
;; if test then [else]
;;------------------------------------------------------------------------------
(defmethod cg-form ((form if-form))
  (if *FLAT-IFS*
      (cg-if-flat form)
      (cg-if      form)))


(defun unique-label (prefix)
  (format nil "~A" (gensym prefix)))


(defun cg-if-flat (form)
  (let ((*result-spec* 'C-bool))
    (cg-form (?pred form)))

  (let ((else-label  (unique-label "else"))
        (endif-label (unique-label "endif"))
        (else-absent (and (null *result-spec*) (null-form-p (?else form)))))
    
    (if else-absent
        (C-flat-if *C-bool* endif-label)
        (C-flat-if *C-bool*  else-label))
    
    (incf *C-indent*)

    (cg-form (?then form))
    (when (eq *result-spec* 'C-bool)
      (to-bool-result))
    
    (cond
      (else-absent 
       (decf *C-indent*)
       (C-label endif-label))
      (T
       (C-goto endif-label)
       (decf *C-indent*)
       (C-label else-label)
       (incf *C-indent*)
       (cg-form (?else form))
       (when (eq *result-spec* 'C-bool)
         (to-bool-result))
       (decf *C-indent*)
       (C-label endif-label))))
  (setq *C-bool* C-bool_result))
            

(defun cg-if (form)
    (let ((*result-spec* 'C-bool))
      (cg-form (?pred form)))

    (C-if *C-bool*)
    (C-blockstart)
    (cg-form (?then form))
    (when (eq *result-spec* 'C-bool)
      (to-bool-result))
    (C-blockend) 

    ;; else ist evtl. nil
    ;;-------------------
    (unless (and (null *result-spec*) (null-form-p (?else form)))
      (C-else)
      (C-blockstart)
      (cg-form (?else form))
      (when (eq *result-spec* 'C-bool)
        (to-bool-result))
      (C-blockend))
  (setq *C-bool* C-bool_result))


;;------------------------------------------------------------------------------
;; Hilfsfunktion, weist der C-Variablen 'bool_result' den Wert des Ausdrucks,
;; der in *C-bool* angegeben ist zu.
;; Die Zuweisung "bool_result = bool_result;" wird unterdrueckt.
;;------------------------------------------------------------------------------
(defun to-bool-result ()
  (unless (string= C-bool_result *C-bool*)
    (C-Assign C-bool_result *C-bool*)
    (setq *C-bool* C-bool_result)))

;;------------------------------------------------------------------------------
(provide "cgif")
                                                    
