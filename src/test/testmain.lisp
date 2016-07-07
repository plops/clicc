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
;;; Inhalt   : Funktionen fuer die Durchfuehrung der Tests und 
;;;            ein einfaches Menue zur Auswahl der einzelnen Tests.
;;;
;;; $Revision: 1.21 $
;;; $Id: testmain.lisp,v 1.21 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "USER")


;;------------------------------------------------------------------------------
;; Variablen fuer die CLiCC-Tests. 
;;------------------------------------------------------------------------------
(defvar *CLICC-TESTS*        nil)          ;Liste aller Tests
(defvar *CLICC-TEST-VERBOSE* nil)          ;Aufruf und Resultate ausgeben?
(defvar *CLICC-FAILED-TESTS* nil)          ;Liste der fehlgeschlagenen Tests



;;------------------------------------------------------------------------------
;; Makro fuer die CLiCC-Tests:
;;
;; `name' enthaelt den Namen des Tests, 
;; `description-format-string' einen Formattext zur Beschreibung des Tests, 
;; `function-call_expected-result_pairlist' eine Liste von Paaren von 
;; Funktionsaufrufen und zugehoerigen erwarteten Funktionsergebnissen. 
;; Als optionale dritte Komponente dieser Liste kann ein String zur naeheren 
;; Beschreibung des Testaufrufes angegeben werden.
;; Mit dem Schluesselwort `test' wird die Testfunktion zum Vergleich von
;; tatsaechlichem und erwartetem Funktionsergebnis angegeben. Der Standardtest
;; ist #'equal.
;;
;; Beispiele:
;; (clicc-test "Partitionen" 
;;             ";;; Returns the set of all partitions of a nonempty set."
;;             (((partitionen '())    ((())))
;;              ((partitionen '(a b)) (((A B)) ((A) (B))))
;;             )
;;             )
;;
;; (clicc-test "Fakultaet" 
;;             ";;; Liefert die Fakultaet zu einer natuerlichen Zahl."
;;             (((fakultaet 3)    6)
;;              ((fakultaet 5)  120) "optionaler zusaetzlicher Text")
;;             )
;;             :test #'eql)
;;------------------------------------------------------------------------------
(defmacro clicc-test (name 
                      description-format-string                  
                      function-call_expected-result_pairlist
                      &key (test '#'equal))
  (let ((function-name (intern (concatenate 'string "CLICC-TEST-" name))))
    `(progn
      (defun ,function-name ()
        (print-header ,name ,description-format-string)
        ,@(funcall #'make-tests 
                   name 
                   function-call_expected-result_pairlist
                   test))
      (pushnew (cons ,name #',function-name) *CLICC-TESTS*))))



;;------------------------------------------------------------------------------
;; Hilfsfunktion fuer das `clicc-test'-Makro
;;------------------------------------------------------------------------------
(defun make-tests (name function-call_expected-result_pairlist test)
  (let ((testno 0))
    (mapcar #'(lambda (function-call_expected-result)
                (incf testno)
                (let* ((raw-test-name   (format nil "~a ~2d" name testno))
                       (function-call   (first function-call_expected-result))
                       (function-name   (first function-call))
                       (quoted-args     (rest  function-call))
                       (expected-result 
                        (second function-call_expected-result))
                       (optional-test-description
                        (third function-call_expected-result))
                       (test-name
                        (if optional-test-description
                            (concatenate 'string raw-test-name " ("
                                         optional-test-description ")")
                            raw-test-name)))

                  `(progn 
                    (when *CLICC-TEST-VERBOSE*
                      (format T "~&;;; Testing ~a ... " ,test-name))
                    (do-test 
                        ,test-name 
                      ',function-call
                      ',expected-result
                      (funcall #',function-name ,@quoted-args)
                      ,test))))
            function-call_expected-result_pairlist)))



;;------------------------------------------------------------------------------
;; In `do-test' wird der eigentliche Test ausgewertet.
;;------------------------------------------------------------------------------
(defun do-test (name function-call expected-result result test)
  (let ((test-is-ok  (funcall test result expected-result)))

    (when *CLICC-TEST-VERBOSE*
      (format T "~A~%" (if test-is-ok "ok" "failed")))

    (unless test-is-ok
      (pushnew name *CLICC-FAILED-TESTS*)

      (format T "~&~%~
                 ;;; Function Call:   ~S~%~
                 ;;; Expected Result: ~S~%~
                 ;;; Result:          ~S~%~%"
              function-call
              expected-result
              result))))



;;------------------------------------------------------------------------------
;; `print-header' gibt den Kopf eines Tests aus, wenn *CLICC-TEST-PRINT-HEADERS*
;; ungleich nil ist.
;;------------------------------------------------------------------------------
(defun print-header (name description-format-string)
  (when *CLICC-TEST-VERBOSE*
    (format T "~&~%~%;;; ---------------------------------------------~
                         -------------------------------~%~
                     ;;; ~a test:~%~
                     ;;;~%"
            name)
    (format T description-format-string)
    (format T "~&;;; ---------------------------------------------------~
                     -------------------------~%")))
 



;;------------------------------------------------------------------------------
;; `print-test-result' gibt das Ergebnis eines oder mehrerer Tests aus.
;;------------------------------------------------------------------------------
(defun print-test-result ()
  (format t "~&~%;;; ---------------------------------------------~
                     -------------------------------~%~
                 ;;; Failed tests: ")

  (if (null *CLICC-FAILED-TESTS*)
      (format t "none~%")
      (dolist (name (reverse *CLICC-FAILED-TESTS*))
        (format t "~&;;;   ~A~%" name)))

  (format t "~&;;; ---------------------------------------------~
                   -------------------------------~%"))
 

             
;;------------------------------------------------------------------------------
;; `print-help' gibt eine kleine Hilfe fuer das Testmenue aus.
;;------------------------------------------------------------------------------
(defun print-help ()
  (format t "~&~%;;; ---------------------------------------------~
                     -------------------------------~%~
                 ;;; Type:~%~
                 ;;;   a number <n> to start the n-th test,~%~
                 ;;;   <A> to start all tests,~%~
                 ;;;   <V> to toggle the verbose mode,~%~
                 ;;;   <D> to display a list of all tests,~%~
                 ;;;   <H> for this help, or~%~
                 ;;;   <Q> to quit.~%~
                 ;;; ---------------------------------------------~
                     -------------------------------~%"))


   
;;------------------------------------------------------------------------------
;; Ein kleines Menue zum Starten ausgewaehlter oder aller Test.
;;------------------------------------------------------------------------------
(defun run-tests ()
  (let ((tests (reverse *CLICC-TESTS*))
        input)

    (flet ((display-tests ()
             (format t "~&~%~%  Available tests:~%~%")
             (dotimes (i (length tests))
               (format t "~&  Test ~2D:   ~A~%" (1+ i) (first (nth i tests))))))
   
      (display-tests)

      (loop
       (format t "~&~%  <number>  A)ll  V)erbose is ~A  D)isplay tests  H)elp  ~
                  Q)uit  choose: "
               (if *CLICC-TEST-VERBOSE* "on" "off"))

       (setf *CLICC-FAILED-TESTS* nil input (read))

       (case input
         (Q (return))
         (D (display-tests))
         (V (setf *CLICC-TEST-VERBOSE* (not *CLICC-TEST-VERBOSE*)))
         (A (dolist (test tests)
              (funcall (rest test)))
            (print-test-result))
         (H (print-help))
         (T (when (and (integerp input) 
                       (>= input 1) 
                       (<= input (length tests)))
              (funcall (rest (nth (1- input) tests))))))

       ))))


#+Lucid 
(unintern 'user::?)

;;------------------------------------------------------------------------------
;; Hier werden die einzelnen Testdateien geladen.
;;------------------------------------------------------------------------------
(require "lambda")
(require "bindings")
(require "side-effect")
(require "ai-course")
(require "expert")        ;Der expert-Test benoetigt Funktionen aus "ai-course".
(require "y-fac")
(require "clos-test")
(require "search")
(require "arrays")

;;------------------------------------------------------------------------------
;; Aufruf des Menues
;;------------------------------------------------------------------------------
(run-tests)
