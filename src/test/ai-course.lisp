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
;;; Funktion : Test einfacher Lisp-Funktionen
;;;
;;; $Revision: 1.15 $
;;; $Id: ai-course.lisp,v 1.15 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "USER")

;;------------------------------------------------------------------------------
;; Die Funktion `Partitionen' liefert zu einer nichtleeren Menge die Menge aller
;; Partitionen in Listendarstellung.
;;------------------------------------------------------------------------------
(DEFUN Partitionen (Menge)
  (if (null (rest Menge))
      (list (list Menge))
      (let ((e        (first Menge))
            (Ergebnis '()))

        (dolist (Partition (Partitionen (rest Menge)) Ergebnis)
          (push (cons (list e) Partition) Ergebnis)
          (dolist (Teilmenge Partition)
            (push (substitute (cons e Teilmenge) Teilmenge Partition) 
                  Ergebnis))))))


(clicc-test "Partitionen" 
            ";;; Returns the set of all partitions of a nonempty set."
            (((partitionen '())    ((())))
             ((partitionen '(a))   (((a))))
             ((partitionen '(a b)) (((A B)) ((A) (B))))
             ))

;;------------------------------------------------------------------------------
;; `Potenzmenge' liefert die Potenzmenge einer Menge.
;;------------------------------------------------------------------------------
(defun Potenzmenge (Menge)
  (if Menge
    ;; Fall 1: Die Menge ist nicht leer.
    (let ((Potenzrestmenge (Potenzmenge (rest Menge))))
      (append Potenzrestmenge
              (mapcar #'(lambda (Element)
                          (cons (first Menge) Element))
                      Potenzrestmenge)))

    ;; Fall 2: Die Menge ist leer.
    '(())
    ))


(clicc-test "Potenzmenge" 
            ";;; Returns a set's powerset."
            (((potenzmenge '())    (nil))
             ((potenzmenge '(a))   (nil (a)))
             ((potenzmenge '(a b)) (nil (b) (a) (a b)))
             ((potenzmenge '(a b c d))
              (NIL (D) (C) (C D) (B) (B D) (B C) (B C D) (A) (A D) (A C) 
                   (A C D) (A B) (A B D) (A B C) (A B C D)))))

;;------------------------------------------------------------------------------
;; Die Funktion 'Diff' wirkt wie in der Aufgabenstellung beschrieben.
;;------------------------------------------------------------------------------
(defun diff (Ausdruck Variable)
  (cond ((eq Ausdruck Variable) 1)
        ((atom Ausdruck) 0)

        (T (let ((Operator  (first Ausdruck))
                 (Argument1 (second Ausdruck))
                 (Argument2 (third Ausdruck)))

             (case Operator 
               ((+ -) 
                `(,Operator ,(diff Argument1 Variable)
                            ,(diff Argument2 Variable)))

               (*
                `(+ (* ,(diff Argument1 Variable) ,Argument2)
                    (* ,Argument1 ,(diff Argument2 Variable))))

               (/ `(/ (- (* ,(diff Argument1 Variable)
                            ,Argument2)
                         (* ,Argument1
                            ,(diff Argument2 Variable)))
                      (* ,Argument2 ,Argument2)))

               (expt 
                (if (and (atom Argument2)
                         (not (eq Argument2 Variable)))
                    `(* (* ,Argument2 ,(diff Argument1 Variable))
                        (expt ,Argument1 (- ,Argument2 1)))
                    (error "~&Der Ausdruck ~S kann nicht differenziert werden."
                           Ausdruck)))

               (otherwise
                (error "~&Der Operator ~S ist unbekannt." Operator)))))))




(clicc-test "Symbolischer Differenzierer" 
            ";;; differentiates a symbolic expression."
            (((diff '3 'x)      
              0)
             ((diff 'x 'x)       
              1)
             ((diff '(- x 0) 'x) 
              (- 1 0))
             ((diff '(+ (* 3 x) (* 2 x)) 'x)
              (+ (+ (* 0 X) (* 3 1)) (+ (* 0 X) (* 2 1))))
             ((diff '(/ x 5) 'x)
              (/ (- (* 1 5) (* X 0)) (* 5 5)))
             ((diff '(expt x 3) 'x)
              (* (* 3 1) (EXPT X (- 3 1))))
             ((diff '(+ (* 3 (expt x 4)) (* 2 (expt x 3))) 'x)
              (+ (+ (* 0 (EXPT X 4)) (* 3 (* (* 4 1) (EXPT X (- 4 1)))))
                 (+ (* 0 (EXPT X 3)) (* 2 (* (* 3 1) (EXPT X (- 3 1)))))))
             ))

;;------------------------------------------------------------------------------
;; `words-in' return a list of the words of a sentence.
;;------------------------------------------------------------------------------
(defconstant *Trennzeichenliste*
  '(#\Space #\Tab #\Newline #\, #\! #\? #\: #\.))

(defun woerter-in (Satz)
  (let* ((getrimmter-Satz (string-trim *Trennzeichenliste* Satz))
         (Trennposition   (position #\Space getrimmter-Satz)))
    (if Trennposition
      (let ((neues-Wort    (subseq getrimmter-Satz 0 Trennposition))
            (Rest-Satz     (subseq getrimmter-Satz (+ Trennposition 1))))
        (cons (string-trim *Trennzeichenliste* neues-Wort)
              (woerter-in Rest-Satz)))
      (list getrimmter-Satz))))
         

(clicc-test "Worttrenner" 
            ";;; Returns a list of the words in a sentence."
            (((woerter-in "Oops, it's already 18:17 o'clock!")
              ("Oops" "it's" "already" "18:17" "o'clock"))
             ((woerter-in "A man, a plan, a canal: panama!")
              ("A" "man" "a" "plan" "a" "canal" "panama"))
             ((woerter-in "Bird lives!")
              ("Bird" "lives"))
             ))


;;------------------------------------------------------------------------------
;; Die Funktion MATCH ist die Hauptfunktion fuer einen Pattern-Matcher. 
;; Bei erfolgreichem Match wird eine Assoziationsliste der Variablenbindungen 
;; geliefert, FAIL sonst. 
;;
;; Die beiden verschiedenen Versionen zeigen eventuelle Unterschiede in der
;; Analyse und Uebersetzung verschiedener Programmierstile auf.
;;------------------------------------------------------------------------------
(defun match1 (pattern datum &optional bindings)
  (let ((pattern-head (first pattern)))

    (cond ((both-empty-p1 pattern datum)
           bindings)

          ((one-empty-p1 pattern datum)
           'fail)

          ((variable-p pattern-head)
           (match-variable1 pattern datum bindings))

          ((plus-variable-p pattern-head)
           (match-plus1 pattern datum bindings))

          (T (match-head-and-rest1 pattern datum bindings)))))

;;------------------------------------------------------------------------------

(defun match2 (pattern datum &optional bindings)
  (cond ((both-empty-p2 pattern datum) 
         bindings)

        ((one-empty-p2  pattern datum) 
         'fail)

        (T (let ((pattern-head (first pattern)))
               
             (cond ((variable-p pattern-head)
                    (match-variable2 pattern datum bindings))
                     
                   ((plus-variable-p pattern-head)
                    (match-plus2 pattern datum bindings))
                     
                   (T (match-head-and-rest2 pattern datum bindings)))))))

;;------------------------------------------------------------------------------

(defconstant *anonymous-symbol* '_)

(defconstant *variable-symbol* '?)

(defconstant *plus-variable-symbol* '+)


(defun both-empty-p1 (pattern datum)
  (and (null pattern) (null datum)))

(defun one-empty-p1 (pattern datum)
  (not (and pattern datum)))

(defun both-empty-p2 (pattern datum)
  (and (endp pattern) (endp datum)))

(defun one-empty-p2 (pattern datum)
  (or (endp pattern) (endp datum)))

(defun variable-p (pattern)
  (and (listp pattern) (eq (first pattern) *variable-symbol*)))

(defun plus-variable-p (pattern)
  (and (listp pattern) (eq (first pattern) *plus-variable-symbol*)))

(defun anonymous-variable-p (variable)
  (eq variable *anonymous-symbol*))

;;------------------------------------------------------------------------------

(defun match-head-and-rest1 (pattern datum bindings)
  ;;Pruefe, ob PATTERN-Kopf und DATUM-Kopf gleich sind.
  (if (equal (first pattern) (first datum))
    ;;Wenn das der Fall ist, dann versuche die Reste zu matchen.
    (match1 (rest pattern) (rest datum) bindings)
    ;;Ansonsten liefere FAIL.
    'fail))

;;------------------------------------------------------------------------------

(defun match-variable1 (pattern datum bindings)
  (let ((binding (find-binding (first pattern) bindings)))
    ;;Pruefe, ob die Patternvariable gebunden ist.
    (if binding
      ;;Wenn das der Fall ist, versuche mit der Substitution zu matchen.
      (if (equal (first (extract-value binding)) (first datum))
        (match1 (rest pattern) (rest datum) bindings)
        'fail)
      ;;Ansonsten fuege die neue Bindung zu den Bindungen hinzu und matche
      ;; die Reste.
      (match1 (rest pattern) 
              (rest datum) 
              (add-binding (first pattern) (list (first datum)) bindings)))))

;;------------------------------------------------------------------------------

(defun match-plus1 (pattern datum bindings)
  (let ((binding (find-binding (first pattern) bindings)))
    ;;Pruefe, ob die Patternvariable gebunden ist.
    (if binding
      ;;Wenn das der Fall ist, und der Wert der Bindung mit der entsprechenden 
      ;; Teilliste aus dem Datum uebereinstimmt, dann versuche die Reste zu
      ;; matchen.
      (let* ((value        (extract-value binding))
             (value-length (list-length value)))
        (if (equal value (nthfirst value-length datum))
          (match1 (rest pattern) (nthcdr value-length datum) bindings)
          'fail))

      ;;Ansonsten versuche die Anfangslisten von Datum an die Variable zu binden
      ;; und mit dieser Bindung die Reste zu matchen.
      (dotimes (n (list-length datum) 'fail)
        (let ((result (match1 (rest pattern) 
                              (nthcdr (+ n 1) datum) 
                              (add-binding (first pattern) 
                                           (nthfirst (+ n 1) datum) 
                                           bindings))))
          (unless (eq 'fail result)
            (return result)))))))

;;------------------------------------------------------------------------------

(defun match-head-and-rest2 (pattern datum bindings)
  ;;Pruefe, ob PATTERN-Kopf und DATUM-Kopf gleich sind.
  (if (equal (first pattern) (first datum))
    ;;Wenn das der Fall ist, dann versuche die Reste zu matchen.
    (match2 (rest pattern) (rest datum) bindings)
    ;;Ansonsten liefere FAIL.
    'fail))

;;------------------------------------------------------------------------------

(defun match-variable2 (pattern datum bindings)
  (let ((binding (find-binding (first pattern) bindings)))
    ;;Pruefe, ob die Patternvariable gebunden ist.
    (if binding
      ;;Wenn das der Fall ist, versuche mit der Substitution zu matchen.
      (if (equal (first (extract-value binding)) (first datum))
        (match2 (rest pattern) (rest datum) bindings)
        'fail)
      ;;Ansonsten fuege die neue Bindung zu den Bindungen hinzu und matche
      ;; die Reste.
      (match2 (rest pattern) 
              (rest datum) 
              (add-binding (first pattern) (list (first datum)) bindings)))))

;;------------------------------------------------------------------------------

(defun match-plus2 (pattern datum bindings)
  (let ((binding (find-binding (first pattern) bindings)))
    ;;Pruefe, ob die Patternvariable gebunden ist.
    (if binding
      ;;Wenn das der Fall ist, und der Wert der Bindung mit der entsprechenden 
      ;; Teilliste aus dem Datum uebereinstimmt, dann versuche die Reste zu
      ;; matchen.
      (let* ((value        (extract-value binding))
             (value-length (list-length value)))
        (if (equal value (nthfirst value-length datum))
          (match2 (rest pattern) (nthcdr value-length datum) bindings)
          'fail))

      ;;Ansonsten versuche die Anfangslisten von Datum an die Variable zu binden
      ;; und mit dieser Bindung die Reste zu matchen.
      (dotimes (n (list-length datum) 'fail)
        (let ((result (match2 (rest pattern) 
                              (nthcdr (+ n 1) datum) 
                              (add-binding (first pattern) 
                                           (nthfirst (+ n 1) datum) 
                                           bindings))))
          (unless (eq 'fail result)
            (return result)))))))

;;------------------------------------------------------------------------------

(defun add-binding (pattern-variable-expression datum bindings)
  (if (anonymous-variable-p (extract-variable pattern-variable-expression))
    bindings
    (cons (make-binding (extract-variable pattern-variable-expression)
                        datum)
          bindings)))

;;------------------------------------------------------------------------------

(defun make-binding (variable datum)
  (cons variable datum))

;;------------------------------------------------------------------------------

(defun find-binding (pattern-variable-expression binding)
  (unless (anonymous-variable-p (extract-variable pattern-variable-expression))
    (assoc (extract-variable pattern-variable-expression) binding)))

;;------------------------------------------------------------------------------

(defun extract-variable (pattern-variable-expression)
  (second pattern-variable-expression))

;;------------------------------------------------------------------------------

(defun extract-value (binding)
  (rest binding))

;;------------------------------------------------------------------------------

(defun nthfirst (n list)
  (if (> (list-length list) n)
    (butlast list (- (list-length list) n))
    list))
 
;;------------------------------------------------------------------------------
(clicc-test "match"
            ";;; Tries to match a pattern with a datum. It returns an~%~
             ;;; a-list of bound pattern symbols, FAIL if the datum doesn't~%~
             ;;; match the pattern."
             (((match1 '(I love (? person1) and (? person2)) 
                       '(I love Mary and Daisy))
               ((PERSON2 DAISY) (PERSON1 MARY)))
              ((match1 '((? THIS) IS THE SAME AS (? THIS)) 
                       '(ABC IS THE SAME AS ABC))
               ((this abc)))
              ((MATCH1 '((+ _)) '(I MATCH ALL))
               nil)
              ((MATCH1 '(BOZO IS-A DOG) '(BOZO IS-A DOG))
               nil)
              ((MATCH1 '((? X) IS-A COW) '(BOZO IS-A DOG))
               fail)
              ((MATCH1 '((? X) IS-A COW) '(BOZO IS-A COW))
               ((X BOZO)))
              ((match2 '(I love (? person1) and (? person2)) 
                       '(I love Mary and Daisy))
               ((PERSON2 DAISY) (PERSON1 MARY)))
              ((match2 '((? THIS) IS THE SAME AS (? THIS)) 
                       '(ABC IS THE SAME AS ABC))
               ((this abc)))
              ((MATCH2 '((+ _)) '(I MATCH ALL))
               nil)
              ((MATCH2 '(BOZO IS-A DOG) '(BOZO IS-A DOG))
               nil)
              ((MATCH2 '((? X) IS-A COW) '(BOZO IS-A DOG))
               fail)
              ((MATCH2 '((? X) IS-A COW) '(BOZO IS-A COW))
               ((X BOZO)))
              ))

;;------------------------------------------------------------------------------

(provide "ai-course")

