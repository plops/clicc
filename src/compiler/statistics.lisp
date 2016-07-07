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
;;; Funktion : Methoden zum Traversieren der Zwischensprache, die die 
;;;            Zwischensprachausdruecke zaehlen und Statistiken ueber 
;;;            in den Annotationen liegenden Informationen erstellen.
;;;
;;; $Revision: 1.37 $
;;; $Id: statistics.lisp,v 1.37 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(require "titypes")
(require "traverse")

;;------------------------------------------------------------------------------
;; Globale Variablen fuer den Statistikanalyse:
;;------------------------------------------------------------------------------
(defvar *number-of-forms*)
(defvar *number-of-literals*)
(defvar *number-of-simple-typed-forms*)


;; Tabelle zur Typstatistik mit Eintraegen der Art (Typ . Anzahl Annotierungen) 
(defvar *type-statistic*)


;; Assoziationsliste fuer applizierte Funktionen. Schluessel ist die Funktion
;; und Datum ist die Anzahl der Applikationen dieser Funktion.
(defvar *application-statistic*)


;; Schwellwert fuer die Ausgabe von applizierten Funktionen in Prozent.
(defconstant *st-write-application-threshold* 0.3)


;; Schwellwert fuer die Ausgabe von Typvorkommen.
(defconstant *st-write-type-threshold* 0.2)


;;------------------------------------------------------------------------------
;; Sammle Statistiken zu dem Modul und gib sie aus. Es werden die Ausdruecke 
;; gezaehlt und Statistiken mit folgenden Informationen erstellt:
;; 
;; - Anzahl der literalen und nicht-literalen Ausdruecke,
;; - Haeufigkeitsstatistik der in Applikationen applizierten Funktionen,
;; - Haeufigkeitsstatistik zu den Typannotationen der nicht-literalen Ausdruecke
;;------------------------------------------------------------------------------
(defun do-statistics (&key ((:module    *module*      ) *module*))
  (setf *number-of-forms*              0
        *number-of-literals*           0
        *number-of-simple-typed-forms* 0
        *type-statistic*               '()
        *application-statistic*        '())
    
  (clicc-message "Collecting type statistics")
  (traverse-module *module* 
                   :after-funs
                   (if (> *ti-verbosity* 3)
                       (list #'count-applications
                             #'collect-type-statistics)
                       (list #'collect-type-statistics))
                   :tr-fun-body-p #'(lambda (a-fun)
                                      (declare (ignore a-fun)) nil))
  
  (unless (zerop *number-of-forms*)
    (write-statistics)))


;;------------------------------------------------------------------------------
;; Die generische Funktion `count-applications' erstellt eine Statistik ueber
;; die Applikationen im analysierten Modul. 
;; Zu jeder mittels eines Applikationsausdrucks applizierten Funktion wird die
;; Anzahl der Applikationen aufgefuehrt.
;;------------------------------------------------------------------------------
(defgeneric count-applications (object))

(defmethod count-applications ((a-zws-object zws-object)))

(defmethod count-applications ((an-app app))
  (let ((form (?form an-app)))
    (when (fun-p form)
      (let ((assoc (assoc form *application-statistic* :test #'eq)))
        (if assoc 
            (incf (rest assoc))
            (push (cons form 1) *application-statistic*))))))


;;------------------------------------------------------------------------------
;; Es folgen die Methoden der generischen Funktion zum Erstellen der Statstik
;; ueber die Typannotationen.
;;------------------------------------------------------------------------------
(defun collect-types (a-form)
  (incf *number-of-forms*)
  (when (simple-type-p (?type a-form))
    (incf *number-of-simple-typed-forms*))
  (add-type-statistics a-form))


(defgeneric collect-type-statistics (object))

(defmethod collect-type-statistics ((a-zws-object zws-object)))

(defmethod collect-type-statistics ((a-form form)) 
  (collect-types a-form))

(defmethod collect-type-statistics ((a-cont cont)) 
  (collect-types a-cont))

(defmethod collect-type-statistics ((an-app app))
  (unless (cont-p (?form an-app))
    (collect-types an-app)))

(defmethod collect-type-statistics ((a-tagged-form tagged-form)))

(defmethod collect-type-statistics ((a-literal literal))
  (incf *number-of-forms*) 
  (incf *number-of-literals*)
  (add-type-statistics a-literal))


;;------------------------------------------------------------------------------
;; Fuege einen neuen Eintrag in eine der Statistiktabellen.
;;------------------------------------------------------------------------------
(defmacro add-to-table (entry table test)
  `(let ((assoc (assoc ,entry ,table :test ,test)))
    (if assoc
        (incf (rest assoc))
        (push (cons ,entry 1) ,table))))


;;------------------------------------------------------------------------------
(defun add-type-statistics  (a-form)
  (add-to-table (?type a-form) *type-statistic* #'type-eq))


;;------------------------------------------------------------------------------
;; Ausgabe der gewonnenen Statistikinformation ueber den Zwischensprachencode.
;;------------------------------------------------------------------------------
(defun write-statistics ()

  (when (> *ti-verbosity* 2)
    (write-type-statistics)
    (write-count-statistic))

  (when (> *ti-verbosity* 3)
    (write-application-statistic)))



;;------------------------------------------------------------------------------
;; Ausgabe der Typstatistiken.
;;------------------------------------------------------------------------------
(defun write-type-statistics ()
  (clicc-message-line)
  (clicc-message "The module contains ~D forms, typed as follows:"
                 *number-of-forms*)
  
  ;; Sortiere die Eintraege der Typtabelle nach Haeufigkeit der Vorkommen.
  ;;----------------------------------------------------------------------
  (setf *type-statistic* (sort *type-statistic* #'(lambda (e1 e2)
                                                    (> (rest e1) (rest e2)))))

  (let ((*print-length* 4))

    (dolist (entry *type-statistic*)
      (let* ((type    (first entry))
             (number  (rest entry))
             (percent (* 100 (/ number *number-of-forms*))))
        
        (when (> percent *st-write-type-threshold*)
          (clicc-message
           "~5d form~:p (~4,1F%) ~2:*~[are~; is~:;are~] typed ~*~A."
           number
           percent
           (output-type type))))))

  (clicc-message-line))


;;------------------------------------------------------------------------------
;; Ausgabe der Anzahl der literalen und nichtliteralen Ausdruecke.
;;------------------------------------------------------------------------------
(defun write-count-statistic ()
  (let ((optimally-typed-forms (+ *number-of-literals* 
                                  *number-of-simple-typed-forms*))
        (cons-typed-forms      0))

    (dolist (entry *type-statistic*)
      (let ((type   (first entry))
            (number (rest  entry)))
        (when (and (not (is-bottom-t type))
                   (zs-subtypep type cons-t))
          (incf cons-typed-forms number))))

    (clicc-message 
     "~5D forms (~4,1F%) are proven to be typed optimally."
     optimally-typed-forms
     (* 100 (/ optimally-typed-forms *number-of-forms*))) 

    (clicc-message 
     "~5D forms (~4,1F%) are literals."
     *number-of-literals*
     (* 100 (/ *number-of-literals* *number-of-forms*)))

    ;; (clicc-message 
    ;;  "~5D forms (~4,1F%) are typed ~A."
    ;;  cons-typed-forms
    ;;  (* 100 (/ cons-typed-forms *number-of-forms*))
    ;;  (output-type cons-t))

    )

  (clicc-message-line))


;;------------------------------------------------------------------------------
;; Ausgabe der Haeufigkeitsstatistik zu Funktionsapplikationen.
;;------------------------------------------------------------------------------
(defun write-application-statistic ()
  (let ((number-of-applications 
         (let ((sum 0))
           (dolist (entry *application-statistic* sum)
             (incf sum (rest entry))))))
  
    (clicc-message "There are ~D application~:p as follows:"
                   number-of-applications)

    ;; Sortiere die Funktionen nach der Anzahl ihrer Vorkommen.
    (setf *application-statistic* 
          (sort *application-statistic*
                #'(lambda (entry1 entry2)
                    (> (rest entry1) (rest entry2)))))

    (dolist (entry *application-statistic*)
      (let* ((number  (rest entry))
             (percent (* 100 (/ number number-of-applications)))
             (symbol  (?symbol (first entry))))
        
        (when (> percent *st-write-application-threshold*)
          
          (clicc-message "~5D application~:p (~4,1F%) to ~S"
                         number
                         percent
                         symbol)))))

  (clicc-message-line))


;;------------------------------------------------------------------------------
(provide "statistics")



