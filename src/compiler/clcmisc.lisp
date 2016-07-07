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
;;; Funktion : Hilsfunktionen, Hilfsmakros, Fehlermeldungen, Par-spec
;;;
;;; $Revision: 1.37 $
;;; $Id: clcmisc.lisp,v 1.37 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")     

;;------------------------------------------------------------------------------
;;                                ALLGEMEINE HILFSFUNKTIONEN
;;------------------------------------------------------------------------------
(defmacro 1++ (place)
  `(PROG1 ,place (INCF ,place)))

(defun collect-if (&rest args)
  (apply #'REMOVE-IF-NOT args))

(defun p1-endp (form)
  (typecase form
    (null t)
    (cons nil)
    (t (clicc-error NO_LIST_END form))))

;;------------------------------------------------------------------------------
;; Apply function f to each element of array a.
;; Return a as result.
;;------------------------------------------------------------------------------
(defun map-array (f a)
  (let* ((total-size (array-total-size a))
         (linear-array (make-array total-size
                                   :displaced-to a
                                   :element-type (array-element-type a))))
    (dotimes (i total-size)
      (funcall f (elt linear-array i)))
    a))

;;------------------------------------------------------------------------------
;; Apply function f to each element of array a and write the result of each
;; application back into the array.
;; Return a as result.
;;------------------------------------------------------------------------------
(defun update-array (f a)
  (let* ((total-size (array-total-size a))
         (linear-array (make-array total-size
                                   :displaced-to a
                                   :element-type (array-element-type a))))
    (dotimes (i total-size)
      (setf (elt linear-array i) (funcall f (elt linear-array i))))
    a))

;;------------------------------------------------------------------------------
;; Create a new array with the same dimensions as array a and apply the
;; function f to the elements of a to compute the values of the new array.
;; Returns the new array.
;;------------------------------------------------------------------------------
(defun copy-array (f a)
  (let* ((new (make-array (array-dimensions a)))
         (total-size (array-total-size a))
         (l (make-array total-size
                        :displaced-to a
                        :element-type (array-element-type a)))
         (l2 (make-array total-size
                         :displaced-to new
                         :element-type (array-element-type new))))
    
    (dotimes (i total-size)
      (setf (elt l2 i) (funcall f (elt l i))))
    new))

;;------------------------------------------------------------------------------
(defun intern-prefixed (prefix symbol-or-string)
  (intern (concatenate 'string prefix (string symbol-or-string))))

(defun intern-postfixed (symbol-or-string postfix)
  (intern (concatenate 'string (string symbol-or-string) postfix)))

(defun intern-postnum (symbol i)
  (intern (format nil "~A~A" symbol i)))

;;------------------------------------------------------------------------------
(defun mapappend (fun &rest args) 
  (if (some #'null args)
      '()
      (append (apply fun (mapcar #'first args))
              (apply #'mapappend fun (mapcar #'rest args)))))

;;------------------------------------------------------------------------------
(defun duplicates-in-list (l)
  (if (endp l)
       l
       (if (member (car l) (cdr l) :test #'equal)
           (cons (car l) (duplicates-in-list (cdr l)))
           (duplicates-in-list (cdr l)))))

;;------------------------------------------------------------------------------
;; Destrukturiert die Argumente eines Aufrufs der Form (name . args)
;;------------------------------------------------------------------------------
(defmacro match-args (form name p . body)
  `(match ,name (cdr ,form) ,p . ,body))

;;------------------------------------------------------------------------------
;; Fuehrt den Rumpf body in einer Umgebung aus, in der die Variablen der
;; strukturierten Lambda-Liste p an Werte aus dem strukturierten Datum in form
;; gebunden sind. name wird fuer Fehlermeldungen verwendet.
;; p := symbol | null | (p . p)
;;------------------------------------------------------------------------------
(defmacro match (name form p . body)
  (typecase p
    (null `(progn (unless (null ,form)
                    (clc-error "ignoring extra ~a in ~a" ,form ',name))
            . ,body))
    (symbol `(let ((,p ,form)) . ,body))
    (atom (error "illegal use of match macro"))
    (t `(if (atom ,form)
         (progn (clc-error "~a does not match ~a in ~a" ,form ',p ',name) nil)
         (match ,name (car ,form) ,(car p)
          (match ,name (cdr ,form) ,(cdr p) . ,body))))))

;;------------------------------------------------------------------------------
;; Abstrakter Datentyp Queue
;; Eine Cons-Zelle enthaelt im Car eine Liste und im Cdr den letzten Cons-Knoten
;; dieser Liste.
;; Eine leere Queue wird durch einen Cons-Knoten, der im Car und Cdr Nil
;; enthaelt, dargestellt.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defun empty-queue ()
  (cons nil nil))

(defun empty-queue-p (q)
  (null (car q)))

(defun list2queue (l)
  (cons l (last l)))                   ; (last ()) = ()

(defun queue2list (q)
  (car q))

(defun add-q (e q)
  (let ((new-cons (cons e nil)))
    (if (null (cdr q))
        (rplaca q new-cons)
        (rplacd (cdr q) new-cons))
    (rplacd q new-cons)))

(defun addnew-q (e q)
  (unless (member e (queue2list q))
    (add-q e q)))

;;------------------------------------------------------------------------------
;; Letztes Element der Queue
;;------------------------------------------------------------------------------
(defun last-q (q)
  (cadr q))

(defun set-last-q (e q)
  (rplaca (cdr q) e))


;;------------------------------------------------------------------------------
;; Stellt die Parameter-Spezifikation eine exakte Parameter-Anzahl dar ?
;;------------------------------------------------------------------------------
(defmacro exact-par-spec-p (par-spec)
  `(>= ,par-spec 0))

;;------------------------------------------------------------------------------
;; Dekodiert eine Parameter-Spezifikation, die eine Mindestanzahl bedeutet.
;;------------------------------------------------------------------------------
(defmacro at-least-params (par-spec)
  `(1- (ABS ,par-spec)))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun positive-par-spec (par-spec)
  (if (exact-par-spec-p par-spec)
      par-spec
      (at-least-params par-spec)))

;;------------------------------------------------------------------------------
;; Prüft, ob die Anzahl der aktuellen Parameter passend ist. Rückgabewerte sind:
;;  'not-enough-argsX, wenn zu wenig aktuelle Parameter vorliegen,
;;  'too-much-args, wenn zu viele Parameter vorliegen,
;;  nil sonst.
;;------------------------------------------------------------------------------
(defun number-of-args-is-not-ok (par-spec number-of-args)
  (if (exact-par-spec-p par-spec)
      (cond ((< number-of-args par-spec) 'not-enough-args)
            ((> number-of-args par-spec) 'too-much-args)
            (T nil))
      (cond ((< number-of-args (at-least-params par-spec)) 'not-enough-args)
            (T nil))))


;;------------------------------------------------------------------------------
;; Ueberprueft, ob die Anzahl der aktuellen Parameter korrekt ist
;;------------------------------------------------------------------------------
(defun clc-check-nparams (par-spec number-of-arguments symbol)
  (case (number-of-args-is-not-ok par-spec number-of-arguments)
    (not-enough-args 
     (clc-error NOT_ENOUGH_ARGS symbol (positive-par-spec par-spec)))
    (too-much-args
     (clc-error TOO_MUCH_ARGS symbol par-spec))
    (otherwise)))

;;------------------------------------------------------------------------------
;; Ausgabe von Meldungen, Error oder Warning
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defun say-where ()
  (when (and (not *CLICC-PRINT*)
             *FUN-NAME*)
    (when (not (symbolp *FUN-NAME*))
      (terpri *error-output*))
    (format *error-output* ";;; While analyzing ")
    (if (symbolp *FUN-NAME*)
      (format *error-output* "function ~s" *FUN-NAME*)
      (format *error-output* "~A" *FUN-NAME*))
    (terpri *error-output*)
    (setq *FUN-NAME* nil)))

;;------------------------------------------------------------------------------
;; Diese Funktion kann in allen Phasen des Compilers aufgerufen werden.
;; Sie gibt eine Fehlermeldung in einheitlicher Weise aus und erhoeht
;; *NERRORS*.
;;------------------------------------------------------------------------------
(defun clc-error (error-message &rest args-for-error-message)
  (incf *NERRORS*)
  (format *error-output* "Error: ")
  (apply #'format *error-output* error-message args-for-error-message)
  (terpri *error-output*)
  (terpri *error-output*)
  (break))

;;------------------------------------------------------------------------------
;; Diese Funktion darf nur in Pass1 aufgerufen werden.
;;------------------------------------------------------------------------------
(defun clicc-error (error-message &rest args-for-error-message)
  (incf *NERRORS*)
  (say-where)
  (if *CURRENT-FORM*
    (format *error-output* "Error in form ~s~% -> " *CURRENT-FORM*)
    (format *error-output* "Error: "))
  (apply #'format *error-output* error-message args-for-error-message)
  (terpri *error-output*)
  (terpri *error-output*)
  (break)
  (throw 'clicc-error nil))

;;------------------------------------------------------------------------------
(defun clicc-warning (warn-string &rest args-for-warn-string)
  (incf *NWARNINGS*)
  (say-where)
  (format *error-output* "; Warning: ")
  (apply #'format *error-output* warn-string args-for-warn-string)
  (terpri *error-output*))

;;------------------------------------------------------------------------------
(defun clicc-message (mess-string &rest args-for-mess-string)
  (when *CLICC-PRINT*
    (format t ";;; ")
    (apply #'format t mess-string args-for-mess-string)
    (terpri)))

;;------------------------------------------------------------------------------
(defun clicc-message-line (&optional (length 75))
  (clicc-message (make-string length :initial-element #\-)))

;;------------------------------------------------------------------------------
;; Funktion zur Behandlung interner Fehler:
;;------------------------------------------------------------------------------
(defun internal-error (function-name err-string &rest args-for-err-string)
  (error (concatenate 'string 
                      (format nil "internal (~S), " function-name)
                      (apply #'format nil err-string args-for-err-string))))


;;------------------------------------------------------------------------------
;; Umwandlung von Strings in gueltige C-Identifikatoren
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Generierung aus einem String einen neuen String, der als C-Identifikator
;; benutzt werden kann. Der String kann Zeichen enthalten, die in
;; C-Identifikatoren nicht verwendet werden duerfen.
;; 
;; Konvertierungsregeln:
;; Grossbuchstaben werden zu Kleinbuchstaben,
;; <,>,=,/,*,? werden zu L,G,E,N,X,Q
;; - wird zu _
;; alle weiteren Zeichen werden  zu _
;;
;; Diese Konvertierung ist nicht injektiv, daher wird dem generierten String
;; im allgemeinen ein eindeutiger Prefix vorangestellt.  
;; Als eindeutiger Prefix werden Strings der Form "Z<n>" verwendet, in denen
;; <n> einen natuerliche Zahl ist, die fuer jeden neuen Identifikator um 1
;; erhoeht wird.
;;
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Umwandlung eines Strings in einen C-Identifkator.  
;;
;; Wenn allow-illegal NIL ist, wird eine Fehlermeldung ausgegeben, falls der
;; String Zeichen enthaelt, die keine direkte Entsprechung in
;; C-Identifikatoren haben.  
;;
;; Ist allow-illegal T, so wird kein Fehler ausgeloest sondern der Verstoss im
;; zweiten Resultatswert gemeldet.
;;
;; lower-case-normal bestimmt, wie mit Buchstaben umgegangen werden soll.  Bei
;; lower-case-normal=T wird die Gross/Kleinschreibung beibehalten, bei
;; lower-case-normal T wird sie umgekehrt.
;;------------------------------------------------------------------------------
(defun C-ify (name &key (allow-illegal nil) (lower-case-normal nil))
  ;; lower-case-normal   T: a->a ; (A->A  illegal:=T)
  ;; lower-case-normal NIL: A->a ; (a->A  illegal:=T)
  (let* ((illegal nil)
         (upper-case-normal (not lower-case-normal))
         (result
          (map 'string
               #'(lambda (char)
                   (cond
                     ((and upper-case-normal (upper-case-p char))
                      (char-downcase char))
                     ((and lower-case-normal (lower-case-p char)) char)
                     ((digit-char-p char) char)
                     ((eql #\- char) #\_)
                     ((eql #\< char) #\L) ; less
                     ((eql #\> char) #\G) ; greater
                     ((eql #\= char) #\E) ; eql
                     ((eql #\/ char) #\N) ; not
                     ((eql #\* char) #\X)
                     ((eql #\% char) #\P)
                     ((eql #\? char) #\Q)
                     ((eql #\_ char) #\U) 
                     (t (setq illegal t)
                        (cond
                          ((and upper-case-normal (lower-case-p char))
                           (char-upcase char))
                          ((and lower-case-normal (upper-case-p char)) char)
                          (T #\_)))))
               name)))
    (when (or (zerop (length result))
              (digit-char-p (char result 0)))
      (setq illegal t))
    (when (and illegal (not allow-illegal))
      (clc-error "~s can't be transformed into C identifier" name))
    (values result illegal)))

;;------------------------------------------------------------------------------
;; Stelle einem String einen eindeutigen Prefix voran. Verwende die Variable
;; *C-NAME-PREFIX* zum Durchnumerieren der Prefixe.
;;------------------------------------------------------------------------------
(defun unique-prefix (string &key (prefix-letter "Z"))
  (incf *C-NAME-PREFIX*)
  (concatenate 'string
               prefix-letter
               (princ-to-string *C-NAME-PREFIX*) "_" string))

;;------------------------------------------------------------------------------
;; Bestimme einen eindeutigen gueltigen C-Identifikator mit angegebenem Prefix
;; zum uebergebenen Namen.
;;------------------------------------------------------------------------------
(defun calc-C-name (prefix name)
  (multiple-value-bind (c-name illegal)
      (C-ify name :allow-illegal T :lower-case-normal T)
    (if illegal
        (unique-prefix c-name :prefix-letter prefix)
        (concatenate 'string prefix c-name))))

;;------------------------------------------------------------------------------
;; Testet, ob eine Datei mit Namen 'filename' geoeffnet werden kann.
;; Pr"uft verschiedene Extensions.
;; Resultat: Truename, falls Datei existiert, NIL sonst.
;;------------------------------------------------------------------------------
(defun clc-probe-file (pathname)
  (let ((type (pathname-type pathname)))
    (if type
      (probe-file pathname)
      (dolist (type *POSSIBLE-LISP-EXTENSIONS* nil)
        (let ((file-exists
               (probe-file (make-pathname :type type :defaults pathname))))
          (when file-exists
            (return file-exists)))))))

;;------------------------------------------------------------------------------
(provide "clcmisc")

