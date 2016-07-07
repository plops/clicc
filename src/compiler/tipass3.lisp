;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Nach der eigentlichen Typinferenz wird das analysierte 
;;;            Zwischensprachkonstrukt noch einmal traversiert und dabei
;;;            nach Typfehlern untersucht. Die folgenden Fehler werden gemeldet:
;;;             o an Funktionsposition einer Applikation steht ein Objekt, das
;;;               kein Funktionstyp ist,
;;;             o eine Funktion wurde auf Argumente mit Bottom-Typ appliziert.
;;;
;;; $Revision: 1.17 $
;;; $Log: tipass3.lisp,v $
;;; Revision 1.17  1993/12/09  10:35:48  hk
;;; provide wieder an das Dateiende
;;;
;;; Revision 1.16  1993/11/21  22:09:39  kl
;;; provide umpositioniert.
;;;
;;; Revision 1.15  1993/11/21  19:58:49  kl
;;; Referenz auf *ti-level* entfernt.
;;;
;;; Revision 1.14  1993/10/12  19:52:15  kl
;;; Weitere Methoden fuer Typwarnungen erstellt.
;;;
;;; Revision 1.13  1993/09/12  12:10:30  kl
;;; Spezialisierung ueber T ersetzt.
;;;
;;; Revision 1.12  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.11  1993/05/13  09:13:46  kl
;;; Typfehler werden jetzt gemeldet, wenn einer der Argumenttypen bottom-t ist.
;;;
;;; Revision 1.10  1993/05/06  06:48:23  kl
;;; Ausgabe umgestellt.
;;;
;;; Revision 1.9  1993/04/30  09:21:24  kl
;;; Aufruf von traverse-module vereinfacht.
;;;
;;; Revision 1.8  1993/03/26  07:05:26  kl
;;; Ausgabe bei Typwarnungen geaendert.
;;;
;;; Revision 1.7  1993/03/05  14:28:22  kl
;;; Auf die used-Annotation wird nicht mehr zugegriffen.
;;;
;;; Revision 1.6  1993/03/04  10:45:46  kl
;;; Anpassung an die eingefuehrten Typinferenzlevel.
;;;
;;; Revision 1.5  1993/02/26  11:10:26  jh
;;; traverse-keyword function-selector in fun-selector geaendert.
;;;
;;; Revision 1.4  1993/02/16  16:10:00  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.3  1993/02/15  14:47:03  kl
;;; Die Fehlermeldungen werden nur noch fuer benutzte Funktionen ausgegeben.
;;;
;;; Revision 1.2  1993/02/02  10:12:50  kl
;;; Ausgaben verschoenert.
;;;
;;; Revision 1.1  1993/01/31  13:44:49  kl
;;; Initial revision
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "titypes")
(require "tidef")
(require "traverse")

;;------------------------------------------------------------------------------
;; Gibt eine Warnzeile aus.
;;------------------------------------------------------------------------------
(defun ti-warn-line (priority warn-string &rest args-for-warn-string)
  (when (>= *ti-verbosity* priority)
    (format *error-output* "; ")
    (apply #'format *error-output* warn-string args-for-warn-string)
    (terpri *error-output*)))


;;------------------------------------------------------------------------------
;; ti-warning gibt eine Warnung aus und erhoeht den Zaehler fuer Warnungen.
;;------------------------------------------------------------------------------
(defun ti-warning (warn-string &rest args-for-warn-string)
  (incf *NWARNINGS*)
  (let ((complete-warn-string
         (concatenate 'string 
                      (format nil "Warning (in ~A): " 
                              (?symbol (first *tr-analyzation-path*)))
                      warn-string)))
    (apply #'ti-warn-line 1 complete-warn-string args-for-warn-string)))
  

;;------------------------------------------------------------------------------
;; Untersucht Applikationen auf potentielle Typfehler.
;;------------------------------------------------------------------------------
(defgeneric write-type-warnings (object))

(defmethod write-type-warnings ((a-form form)))
(defmethod write-type-warnings ((a-cont cont)))


(defmethod write-type-warnings ((an-app app))
  (let* ((function       (?form an-app))
         (function-type  (?type function))
         (arguments      (?arg-list an-app))
         (argument-types (mapcar #'?type arguments)))

    (when (and (not (types-are-conform function-t function-type))
               (not (cont-p function)))
      (ti-warning "A non-function is applied.")
      (ti-warn-line 2 "function      : ~S" function)
      (ti-warn-line 2 "function type : ~S" (output-type function-type))
      (ti-warn-line 2 ""))

    (when (some #'is-bottom-t argument-types)
      (ti-warning "Bad argument types")
      (ti-warn-line 2 "applied function : ~S" 
                    (if (fun-p function)
                        (?symbol function)
                        `unknown))
      (ti-warn-line 2 "arguments        : ~S" arguments)
      (ti-warn-line 2 "argument types   : ~S" (mapcar #'output-type 
                                                      argument-types))
      (ti-warn-line 2 ""))))


(defmethod write-type-warnings ((a-var-ref var-ref))
  (when (is-bottom-t (?type a-var-ref))
    (ti-warning "A variable reference is typed BOTTOM.")
    (ti-warn-line 2 "variable-ref : ~S" a-var-ref)))


(defmethod write-type-warn-line ((a-setq-form setq-form))
  (when (is-bottom-t (?type a-setq-form))
    (ti-warning "A location is set to type BOTTOM.")
    (ti-warn-line 2 "location : ~S" (?location a-setq-form))))


;;------------------------------------------------------------------------------
;; Traversiert das uebergebene Modul und durchsucht es nach Typfehlern.
;;------------------------------------------------------------------------------
(defun look-for-type-errors (a-module)
  (when (> *ti-verbosity* 0)
    (clicc-message "Looking for type errors"))
  
  (traverse-module a-module 
                   :after-funs (list #'write-type-warnings)
                   :tr-fun-body-p #'(lambda (a-fun)
                                      (declare (ignore a-fun)) nil)))


;;------------------------------------------------------------------------------
(provide "tipass3") 



