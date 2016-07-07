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
;;; Funktion : - Backquote-Reader + Simplifier
;;;            - Readtables, Readtable-Funktionen
;;;            - READ
;;;            - READ-PRESERVING-WHITESPACE
;;;            - READ-DELIMITED-LIST
;;;            - READ-LINE
;;;            (- READ-CHAR, UNREAD-CHAR --> file.lisp)
;;;            - PEEK-CHAR
;;;            - READ-FROM-STRING
;;;            - PARSE-INTEGER
;;;
;;; $Revision: 1.18 $
;;; $Id: read.lisp,v 1.18 1995/03/03 17:35:19 wg Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(*read-base* *read-suppress* *features*
   readtable readtablep copy-readtable set-syntax-from-char set-macro-character
   get-macro-character make-dispatch-macro-character
   set-dispatch-macro-character get-dispatch-macro-character read
   read-preserving-whitespace read-delimited-list read-line peek-char
   read-from-string parse-integer))

;;------------------------------------------------------------------------------
(defparameter *comma*        (make-symbol "COMMA"))
(defparameter *comma-atsign* (make-symbol "COMMA-ATSIGN"))
(defparameter *comma-dot*    (make-symbol "COMMA-DOT"))
(defparameter *bq-list*      (make-symbol "BQ-LIST"))
(defparameter *bq-append*    (make-symbol "BQ-APPEND"))
(defparameter *bq-list**     (make-symbol "BQ-LIST*"))
(defparameter *bq-nconc*     (make-symbol "BQ-NCONC"))
(defparameter *bq-clobberable* (make-symbol "BQ-CLOBBERABLE"))
(defparameter *bq-quote*     (make-symbol "BQ-QUOTE"))
(defparameter *bq-quote-nil* (list *bq-quote* nil))
(defparameter *bq-vector*    (make-symbol "VECTOR"))
(defparameter *bq-level*     0)

(defparameter *read-base* 10)
(defparameter *read-suppress* nil)
(defparameter *features* '(:CLICC))

;;; (defparameter *token* (make-array 80
;;;                                   :element-type 'character
;;;                                   :fill-pointer 0
;;;                                   :adjustable t))

(defparameter *token* (make-string 80))
(defvar *fill-pointer* 0)
(defvar *token-length* 80)
(defconstant *token-extension* 80)

(defparameter *uninterned* nil)
(defparameter *preserve-whitespace* nil)
(defparameter *dot-flag* nil)
(defparameter *parenthesis-open* nil)

(defvar *readtable-unchanged*)

(defparameter *standard-readtable* (make-standard-readtable))
(defparameter *readtable* (copy-readtable nil)) ; erst nach obiger Zeile
(defparameter *readtable-syntax* (readtable-syntax *readtable*))
(defparameter *the-readtable* *readtable*)

(setq *readtable-unchanged* T)


;;------------------------------------------------------------------------------
;; Die 80%-Regel: Die Readtable ist nicht angefasst worden.
;;------------------------------------------------------------------------------
;;; Ist *readtable* noch original, d.h. eq zu *the-readtable* und ist
;;; *readtable-unchanged* noch T (80%-Fall), kann optimiert werden.
;;; Kommentar: Den Test (eq *readtable* *the-readtable*) koennen wir
;;;            uns sparen, da copy-readtable aufgerufen werden muss, um
;;;            *readtable* mit etwas Sinnvollem zu belegen. Da copy-readtable
;;;            sowieso destruktiv auf *readtable* sein kann, gilt dann schon
;;;            *readtable-unchanged* = nil.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defmacro unchanged-rtab () `*readtable-unchanged*)


;;------------------------------------------------------------------------------
;; *token*-Behandlung ...
;;------------------------------------------------------------------------------
;;; Selbstgeschriebenes VECTOR-PUSH-EXTEND auf dem STRING *token*
;;; beachtet *fill-pointer* und aktualisiert *token-length*
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defun token-push-extend (char)
  (setf (schar *token* *fill-pointer*) char)
  (incf *fill-pointer*)
  (when (>= *fill-pointer* *token-length*)
        (setq *token*
              (concatenate 'string *token* (make-string *token-extension*)))
        (incf *token-length* *token-extension*)))

;;------------------------------------------------------------------------------
(defun get-token-string ()
  (let ((substring (make-string *fill-pointer*)))
      (do ((i 0 (1+ i)))
          ((= i *fill-pointer*) substring)
        (setf (schar substring i) (schar *token* i)))))  

;;------------------------------------------------------------------------------
(defun token-subseq (start end)
  (when (not (check-integer start 0 *fill-pointer*))
    (error "The value ~S is not an integer in the interval [0,~S]"
           start *fill-pointer*))
  (when (not (check-integer end 0 *fill-pointer*))
    (error "The value ~S is not an integer in the interval [0,~S]"
           end *fill-pointer*))
  (when (> start end)
    (error "The START value ~S is greater than the END value ~S" start end))
  (let ((substring (make-string (- end start))))
      (do ((i start (1+ i))
           (j 0     (1+ j)))
          ((= i end) substring)
        (setf (schar substring j) (schar *token* i)))))  

;;------------------------------------------------------------------------------
;; Backquote ...
;;------------------------------------------------------------------------------
;;; angelehnt an "Backquote, Appendix C in CLtL, 2nd Ed."
;;; erweitert um Backquote in Vektoren.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defun backquote-reader (stream char)
  (declare (ignore char))
  (incf *bq-level*)
  (prog1
      (bq-completely-process (bq-read stream))
    (decf *bq-level*)))

;;------------------------------------------------------------------------------
(defun comma-reader (stream char)
  (declare (ignore char))
  (when (<= *bq-level* 0)
    (error "A comma appeared outside of a backquote"))
  (decf *bq-level*)
  (prog1
      (cons (case (peek-char nil stream t nil t)
              (#\@ (read-char stream t nil t) *comma-atsign*)
              (#\. (read-char stream t nil t) *comma-dot*)
              (otherwise *comma*))
            (bq-read stream))
    (incf *bq-level*)))

;;------------------------------------------------------------------------------
(defun bq-completely-process (x)
  (bq-remove-tokens (bq-simplify (bq-process x))))

;;------------------------------------------------------------------------------
(defun bq-process (x)
  (cond
    ;; `basic --> (QUOTE basic)
    ;;-------------------------
    ((atom x) (list *bq-quote* x))

    ;; `#(x1 x2 x3 ... xn) --> (apply #'vector `(x1 x2 x3 ... xn))
    ;;------------------------------------------------------------
    ((eq (car x) *bq-vector*)
     (let ((list (bq-completely-process (cdr x))))
       (if (eq 'QUOTE (car list))
         (list *bq-quote* (apply #'vector (cadr list)))
         (list 'APPLY `#'VECTOR list))))

    ;; `,form --> form
    ;;----------------
    ((eq (car x) *comma*) (cdr x))

    ;; `,@form --> ERROR
    ;;------------------
    ((eq (car x) *comma-atsign*) (error ",@~S after `" (cdr x)))

    ;; `,.form --> ERROR
    ;;------------------
    ((eq (car x) *comma-dot*) (error ",.~S after `" (cdr x)))

    ;; `(x1 x2 x3 ... xn . atom) -->
    ;;------------------------------
    (t (do ((p x (cdr p))
            (q '() (cons (bracket (car p)) q)))
           ((atom p)

            ;; --> (append [x1] [x2 [x3] ... [xn] (quote atom))
            ;;-------------------------------------------------
            (cons *bq-append*
                  (nreconc q (list (list *bq-quote* p)))))

         ;; `(x1 ... xn . ,form) --> (append [x1] ... [xn] form)
         ;;-----------------------------------------------------
         (when (eq (car p) *comma*)
           (return (cons *bq-append* (nreconc q (list (cdr p))))))

         ;; `(x1 ... xn . ,@form) --> ERROR
         ;;--------------------------------
         (when (eq (car p) *comma-atsign*) (error "Dotted ,@~s" (cdr p)))

         ;; `(x1 ... xn . ,.form) --> ERROR
         ;;--------------------------------
         (when (eq (car p) *comma-dot*) (error "Dotted ,@~s" (cdr p)))))))

;;------------------------------------------------------------------------------
(defun bracket (x)
  (cond
    ((atom x) (list *bq-list* (bq-process x)))
    ((eq (car x) *comma*) (list *bq-list* (cdr x)))
    ((eq (car x) *comma-atsign*) (cdr x))
    ((eq (car x) *comma-dot*) (list *bq-clobberable* (cdr x)))
    (t (list *bq-list* (bq-process x)))))

;;------------------------------------------------------------------------------
(defun maptree (fn x)
  (if (atom x)
    (funcall fn x)
    (let ((a (funcall fn (car x)))
          (d (maptree fn (cdr x))))
      (if (and (eql a (car x)) (eql d (cdr x)))
        x
        (cons a d)))))

;;------------------------------------------------------------------------------
(defun bq-splicing-frob (x)
  (and (consp x)
       (or (eq (car x) *comma-atsign*)
           (eq (car x) *comma-dot*))))

;;------------------------------------------------------------------------------
(defun bq-frob (x)
  (and (consp x)
       (or (eq (car x) *comma*)
           (eq (car x) *comma-atsign*)
           (eq (car x) *comma-dot*))))

;;------------------------------------------------------------------------------
(defun bq-simplify (x)
  (if (atom x)
    x
    (let ((x (if (eq (car x) *bq-quote*)
               x
               (maptree #'bq-simplify x))))
      (if (not (eq (car x) *bq-append*))
        x
        (bq-simplify-args x)))))

;;------------------------------------------------------------------------------
(defun bq-simplify-args (x)
  (do ((args (reverse (cdr x)) (cdr args))
       (result
        nil
        (cond ((atom (car args))
               (bq-attach-append *bq-append* (car args) result))
              ((and (eq (caar args) *bq-list*)
                    (notany #'bq-splicing-frob (cdar args)))
               (bq-attach-conses (cdar args) result))
              ((and (eq (caar args) *bq-list**)
                    (notany #'bq-splicing-frob (cdar args)))
               (bq-attach-conses
                (reverse (cdr (reverse (cdar args))))
                (bq-attach-append *bq-append*
                                  (car (last (car args)))
                                  result)))
              ((and (eq (caar args) *bq-quote*)
                    (consp (cadar args))
                    (not (bq-frob (cadar args)))
                    (null (cddar args)))
               (bq-attach-conses (list (list *bq-quote*
                                             (caadar args)))
                                 result))
              ((eq (caar args) *bq-clobberable*)
               (bq-attach-append *bq-nconc* (cadar args) result))
              (t (bq-attach-append *bq-append*
                                   (car args)
                                   result)))))
      ((null args) result)))

;;------------------------------------------------------------------------------
(defun null-or-quoted (x)
  (or (null x) (and (consp x) (eq (car x) *bq-quote*))))

;;------------------------------------------------------------------------------
(defun bq-attach-append (op item result)
  (cond ((or (null result) (equal result *bq-quote-nil*))
         (if (bq-splicing-frob item) (list op item) item))
        ((and (null-or-quoted item) (null-or-quoted result))
         (list *bq-quote* (append (cadr item) (cadr result))))
        ((and (consp result) (eq (car result) op))
         (list* (car result) item (cdr result)))
        (t (list op item result))))

;;------------------------------------------------------------------------------
(defun bq-attach-conses (items result)
  (cond
    ((and (every #'null-or-quoted items)
          (null-or-quoted result))
     (list *bq-quote*
           (append (mapcar #'cadr items) (cadr result))))
    ((or (null result) (equal result *bq-quote-nil*))
     (cons *bq-list* items))
    ((and (consp result)
         (or (eq (car result) *bq-list*)
             (eq (car result) *bq-list**)))
     (cons (car result) (append items (cdr result))))
    (t (cons *bq-list** (append items (list result))))))

;;------------------------------------------------------------------------------
(defun bq-remove-tokens (x)
  (cond
    ((atom x) (cond
                ((eq x *bq-list*) 'list)
                ((eq x *bq-append*) 'append)
                ((eq x *bq-nconc*) 'nconc)
                ((eq x *bq-list**) 'list*)
                ((eq x *bq-quote*) 'quote)
                (T x)))
    
    ((eq (car x) *bq-clobberable*) (bq-remove-tokens (cadr x)))
    ((and (eq (car x) *bq-list**) (consp (cddr x)) (null (cdddr x)))
     (cons 'cons (maptree #'bq-remove-tokens (cdr x))))
    (T (maptree #'bq-remove-tokens x))))

;;------------------------------------------------------------------------------
(defstruct (readtable (:copier nil)
                      (:predicate readtablep)
                      (:print-function
                       (lambda (readtable stream depth)
                         (declare (ignore readtable depth))
                         (write-string "#<readtable>" stream))))

    ;; fuer jedes Standard-Character ein Eintrag:
    ;; NIL (= ILLEGAL), WHITESPACE, CONSTITUENT, SINGLE-ESCAPE, MULTI-ESCAPE,
    ;; (function . non-terminating-p)
    ;;-------------------------------
    (syntax (make-array char-code-limit :initial-element nil))

    ;; eine Association-List, die fuer jedes Dispatch-Character
    ;; das Dispatch-Array angiebt.
    ;;----------------------------
    (dispatch nil))


;;------------------------------------------------------------------------------
(defmacro get-fun (syntax) `(car ,syntax))
(defmacro terminating-p (syntax) `(null (cdr ,syntax)))

(defmacro get-syntax (c)
  `(if (unchanged-rtab)
       (rt::svref-internal *readtable-syntax* (char-code ,c))
     (aref (readtable-syntax *readtable*) (char-code ,c)) ))

(defmacro constituent-p (c)
  `(eql (rt::svref-internal *readtable-syntax* (char-code ,c))
        'CONSTITUENT))

;;------------------------------------------------------------------------------
(defun copy-readtable (&optional (from *readtable*) (to nil))
  
  ;; originale Readtable potentiell zerstoert
  ;;-----------------------------------------
  (setq *readtable-unchanged* nil)
  
  (when (null from) (setq from *standard-readtable*))
  (when (null to) (setq to (make-readtable)))
  (let ((syntax-from (readtable-syntax from))
        (syntax-to   (readtable-syntax to)))
    (dotimes (i char-code-limit)
      (setf (aref syntax-to i) (aref syntax-from i)))
    (setf (readtable-dispatch to) nil)
    (dolist (pair (readtable-dispatch from))
      (when (car pair)
        (copy-dispatch-macro-character pair from to))))
  to)

;;------------------------------------------------------------------------------
(defun set-syntax-from-char (to-char from-char
                                     &optional
                                     (to-readtable *readtable*)
                                     (from-readtable *standard-readtable*)
                                     &aux pair)
  
  ;; originale Readtable potentiell zerstoert
  ;;-----------------------------------------
  (setq *readtable-unchanged* nil)
  
  (setf (aref (readtable-syntax to-readtable) (char-code to-char))
        (aref (readtable-syntax from-readtable) (char-code from-char)))
  
  ;; evtl. Eintrag loeschen, wenn dispatch-character ueberschrieben wurde
  ;;---------------------------------------------------------------------
  (setq pair (assoc to-char (readtable-dispatch to-readtable)))
  (when pair (setf (car pair) nil) (setf (cdr pair) nil))
  
  ;; wenn dispatch-character kopiert werden soll, dann eine Kopie der
  ;; Dispatch-Funktion erzeugen, die sich genauso wie die alte verhaelt.
  ;;--------------------------------------------------------------------
  (setq pair (assoc from-char (readtable-dispatch from-readtable)))
  (when pair
    (copy-dispatch-macro-character pair from-readtable to-readtable))
  t)

;;------------------------------------------------------------------------------
(defun copy-dispatch-macro-character (pair from to)
  
  ;; originale Readtable potentiell zerstoert
  ;;-----------------------------------------
  (setq *readtable-unchanged* nil)

  (let ((c (car pair))
        (dispatch-from (cdr pair))
        dispatch-to)
    (make-dispatch-macro-character c
                                   (cdr (aref (readtable-syntax from)
                                              (char-code c)))
                                   to)
    (setq dispatch-to (cdr (assoc c (readtable-dispatch to))))
    (dotimes (i char-code-limit)
      (setf (aref dispatch-to i) (aref dispatch-from i)))))

;;-----------------------------------------------------------------------------
(defun set-macro-character (char function
                                 &optional
                                 non-terminating-p
                                 (readtable  *readtable*))
  
  ;; originale Readtable potentiell zerstoert
  ;;-----------------------------------------
  (setq *readtable-unchanged* nil)
  
  (setf (aref (readtable-syntax readtable) (char-code char))
        (cons function non-terminating-p))
  
  ;; evtl. Eintrag loeschen, wenn dispatch-character ueberschrieben wurde
  ;;---------------------------------------------------------------------
  (let ((pair (assoc char (readtable-dispatch readtable))))
    (when pair (setf (car pair) nil) (setf (cdr pair) nil)))
  
  t)

;;------------------------------------------------------------------------------
(defun get-macro-character (char &optional (readtable *readtable*))
  (let ((entry (aref (readtable-syntax readtable) (char-code char))))
    (if entry
      (values (car entry) (cdr entry))
      nil)))

;;------------------------------------------------------------------------------
(defun make-dispatch-macro-character (char &optional
                                           non-terminating-p
                                           (readtable *readtable*))
  
  ;; originale Readtable potentiell zerstoert
  ;;-----------------------------------------
  (setq *readtable-unchanged* nil)
  
  (let ((dispatch-array (make-array char-code-limit :initial-element nil)))
    (setf (aref (readtable-syntax readtable) (char-code char))
          (cons
           ;; Dispatch-Reader
           ;;----------------
           #'(lambda (stream char)
               (let* ((i nil)
                      (c (read-char stream t nil t))
                      (d (digit-char-p c))
                      fun)
                 (when d
                   (setq i d)
                   (loop
                     (setq c (read-char stream t nil t))
                     (setq d (digit-char-p c))
                     (when (null d)
                       (when *read-suppress* (setq i nil))
                       (return))
                     (setq i (+ d (* 10 i)))))
                 (setq fun (aref dispatch-array (char-code (char-upcase c))))
                 (unless fun
                   (error "no ~S dispatch function defined for subchar ~S ~
                           (with arg ~S)" char c i))
                 (funcall fun stream c i)))
           non-terminating-p))
    
    (let ((pair (assoc char (readtable-dispatch readtable))))
      (cond
        ;; altes dispatch-array ueberschreiben
        ;;------------------------------------
        (pair (setf (cdr pair) dispatch-array))

        ;; Eintrag neu anlegen
        ;;--------------------
        (T (push (cons char dispatch-array)
                 (readtable-dispatch readtable))))))
  t)

;;------------------------------------------------------------------------------
(defun set-dispatch-macro-character (disp-char sub-char function
                                               &optional
                                               (readtable *readtable*))
  
  ;; originale Readtable potentiell zerstoert
  ;;-----------------------------------------
  (setq *readtable-unchanged* nil)
  
  (let ((dispatch-array
         (cdr (assoc disp-char (readtable-dispatch readtable)))))
    (unless dispatch-array
      (error "~S is not a dispatch macro character" disp-char))
    (setf (aref dispatch-array (char-code (char-upcase sub-char))) function))
  t)

;;------------------------------------------------------------------------------
(defun get-dispatch-macro-character (disp-char sub-char
                                               &optional
                                               (readtable *readtable*))
  (let ((dispatch-array
         (cdr (assoc disp-char (readtable-dispatch readtable)))))
    (unless dispatch-array
      (error "~S is not a dispatch macro character" disp-char))
    (aref dispatch-array (char-code (char-upcase sub-char)))))

;;------------------------------------------------------------------------------
(defun read-token (stream c syntax)
  (let ((multiple-escape nil)
        (escape nil)
        (colon nil)
        colon-pos)

    (loop
;;;      (setq syntax (get-syntax c))
      (cond
        ((not multiple-escape)
         (case syntax
                           
           (WHITESPACE

            ;; Changed by O.B.
            ;;            (when *preserve-whitespace* (unread-char c stream))
            (unread-char c stream)

            (return))

           (CONSTITUENT
            (when (eql #\: c)
		  (case colon
			((nil) (setq colon 1)
			 (setq colon-pos *fill-pointer*))
			(1 (setq colon 2)
			   (unless (eql colon-pos
					(1- *fill-pointer*))
				   (setq colon 3)))
			(t (setq colon 3))))
            (token-push-extend (char-upcase c))
	    (setq c (read-char stream nil nil))
	    (when (unchanged-rtab)
	     (loop
	      (unless (and c (constituent-p c)) (return))
	      (when (eql #\: c)
		    (case colon
			  ((nil) (setq colon 1)
			   (setq colon-pos *fill-pointer*))
			  (1 (setq colon 2)
			     (unless (eql colon-pos
					  (1- *fill-pointer*))
				     (setq colon 3)))
			  (t (setq colon 3))))
	      (token-push-extend (char-upcase c))
	      (setq c (read-char stream nil nil)))))

           (SINGLE-ESCAPE
            (setq c (read-char stream nil nil))
            (unless c
              (error
               "unexpected End of File after single escape"))
            (setq escape t)
            (token-push-extend c)
	    (setq c (read-char stream nil nil)))

           (MULTIPLE-ESCAPE (setq escape t multiple-escape t)
			    (setq c (read-char stream nil nil)))

           ((nil) (error "illegal Character"))
                           
           (t
            (when (and (consp syntax) (terminating-p syntax))
              (unread-char c stream)
              (return))
            (when (eql #\: c)
              (case colon
                ((nil) (setq colon 1)
                 (setq colon-pos *fill-pointer*))
                (1 (setq colon 2)
                   (unless (eql colon-pos
                                (1- *fill-pointer*))
                     (setq colon 3)))
                (t (setq colon 3))))
            (token-push-extend (char-upcase c))
	    (setq c (read-char stream nil nil))))  ;; end of case

            (unless c (return))
            (setq syntax (get-syntax c)))

        (T (case syntax
		 (CONSTITUENT 
                  (token-push-extend c)
		  (setq c (read-char stream nil nil))
                  (when (unchanged-rtab)
		   (loop
		    (unless (and c (constituent-p c)) (return))
		    (token-push-extend c)
		    (setq c (read-char stream nil nil)))))
		 (SINGLE-ESCAPE
		  (setq c (read-char stream nil nil))
		  (unless c
			  (error "unexpected End of File after single escape"))
		  (token-push-extend c)
		  (setq c (read-char stream nil nil)))
		 (MULTIPLE-ESCAPE (setq multiple-escape nil)
				  (setq c (read-char stream nil nil)))
		 ((NIL) (error "illegal character"))
		 (t (token-push-extend c)
		    (setq c (read-char stream nil nil))))
	   (unless c (error "unexpected End of File after multiple escape"))
           (setq syntax (get-syntax c)))))                        ;end of loop


    ;; nicht analysieren, wenn *READ-SUPPRESS*
    (when *read-suppress* (return-from read-token nil))

    ;; *token* als  Zahl oder Symbol interpretieren
    ;;---------------------------------------------
    (let ((i 0)
          (len *fill-pointer*)
          (sign 1)
          (num1 0) (num2 0.0)
          (base *read-base*)
          c)
      (flet
          (
           ;; liest eine Folge von Ziffern
           ;; (abhaengig von *read-base*).
           ;; Resultat:
           ;; NIL, wenn 0 Ziffern gelesen wurden
           ;; INTEGER, die durch die Ziffernfolge repraesentiert
           ;; wird, sonst.
           ;;---------------------------------------------------
           (read-digits (&aux x d)
             (cond
               ((eql i len) nil)
               (T (setq x (digit-char-p (pvref *token* i) base))
                  (cond
                    ((null x) nil)
                    (T (incf i)
                       (loop
                         (when (eql i len)
                           (return x))
                         (setq d (digit-char-p (pvref *token* i)
                                               base))
                         (when (null d)
                           (return x))
                         (incf i)
                         (setq x (+ (* base x) d))))))))

           (read-sign ()
             (if (eql i len)
               1
               (case (pvref *token* i)
                 (#\- (incf i) -1)
                 (#\+ (incf i) 1)
                 (t 1))))

           (read-float2 (d &aux x)
             (loop
               (when (eql i len)
                 (return))
               (setq c (pvref *token* i))
               (setq x (digit-char-p c))
               (when (null x) (return))
               (incf i)
               (setq num2 (+ num2 (/ x d)))
               (setq d (* d 10.0)))
             num2))

        ;; Rumpf von FLET
        ;;---------------
        (tagbody
           (when (or escape colon)
             (go SYMBOL))

           ;; Wenn letztes Zeichen = #\., dann Dezimal-Integer
           ;;-------------------------------------------------
           (when (eql #\. (pvref *token* (1- len)))
             (setq base 10))

           (setq sign (read-sign))
           (setq num1 (read-digits))
           (when (null num1) (go FLOAT-DOT))
           (when (eql i len)
             ;; Integer
             ;;--------
             (return-from read-token (* sign num1)))
           (setq c (pvref *token* i)) (incf i)
           (cond
             ((eql #\. c)
              ;; Dezimal Integer
              ;;----------------
              (when (eql i len) (return-from read-token (* sign num1)))
                     
              ;; evtl. Floating-Point
              ;;---------------------
              (unless (eql base 10)
                (setq base 10)
                (setq i 0)
                (setq sign (read-sign))
                (setq num1 (read-digits))
                (incf i))
              (setq num2 (read-float2 10.0))
              (go FLOAT2))

             ((eql #\/ c) (go RATIO2))
             (T (go FLOAT-EXPT)))

         FLOAT-DOT

           (setq num1 0)
           (when (eql i len) (go SYMBOL))
           (setq c (pvref *token* i)) (incf i)
           (unless (eql #\. c) (go SYMBOL))
                  
           ;; nur ein Punkt
           ;;--------------
           (when (eql i len) (go SYMBOL))

           (setq c (pvref *token* i)) (incf i)
           (setq num2 (digit-char-p c))
           (when (null num2) (go SYMBOL))
           (setq num2 (/ num2 10.0))
           (setq num2 (read-float2 100.0))
                  
         FLOAT2

           (when (eql i len)
             (return-from read-token (* sign (+ num1 num2))))
           (setq c (pvref *token* i)) (incf i)

         FLOAT-EXPT

           (unless (member (char-downcase c) '(#\e #\s #\f #\d #\l))
             (go SYMBOL))

           (when (eql i len) (go SYMBOL))
           (setq base 10)
           (let ((e-sign (read-sign))
                 (expt (read-digits)))
             (when (or (null expt) (< i len)) (go Symbol))
             (return-from read-token (* sign
                                        (+ num1 num2)
                                        (expt 10 (* e-sign expt)))))

         RATIO2

           (setq num2 (read-digits))
           (when (or (null num2) (< i len)) (go SYMBOL))
           (let ((result (/ num1 num2)))
             (warn "ratio ~a/~a has been read as ~s" num1 num2 result)
             (return-from read-token result))

         SYMBOL

           (cond
             ;; kein Package angegeben
             ;;-----------------------
             ((null colon)
              (unless escape
                ;; Pruefen ob der 'Dot' einer Dotted-List vorliegt
                ;;------------------------------------------------
                (when (and (eql len 1)
                           (eql (pvref *token* 0) #\.) 
                           *dot-flag*)
                  (setq *dot-flag* nil)
                  (return-from read-token nil))

                ;; Pruefen ob das Symbol vollstaendig aus Dots besteht
                ;;----------------------------------------------------
                (do ((i 0 (1+ i)))
                    ((>= i len) (error "illegal token ~S" (get-token-string)))
                  (unless (eql #\. (pvref *token* i)) (return))))

              (return-from read-token
                (if *uninterned*
                  (make-symbol (get-token-string))
                  (values (intern (get-token-string))))))
            
             (*uninterned* (error "token may not contain colons"))

             ;; KEYWORD
             ;;--------
             ((eql colon-pos 0)
              (unless (eql colon 1)
                (error "illegal token ~S" (get-token-string)))
              (return-from read-token
                (values (intern (token-subseq 1 *fill-pointer*)
                                *keyword-package*))))

             ;; Package ist angegeben
             ;;----------------------
             (T (let* ((package-name (token-subseq 0 colon-pos))
                       (package (find-package package-name))
                       (symbol-name
                        (token-subseq (+ colon-pos colon) *fill-pointer*)))
                  (unless package
                    (error "illegal package-name ~S" package-name))
                  (case colon
                    (2 (return-from read-token (values (intern symbol-name
                                                               package))))
                    (1 (multiple-value-bind (symbol where)
                           (find-symbol symbol-name package)
                         (unless (eql :external where)
                           (error "can't find the external symbol ~S in ~S"
                                  symbol-name package))
                         (return-from read-token symbol)))
                    (T (error "illegal Token ~S" (get-token-string))))))))))))

;;------------------------------------------------------------------------------
;; ignore-token stream
;; liest ein token ohne Syntaxpruefung
;;------------------------------------------------------------------------------
(defun ignore-token (stream)
  (let ((c (read-char stream t nil t)))
    (read-token stream c (get-syntax c))
    nil))

;;------------------------------------------------------------------------------
;; die schliessende Klammer wird mittels (read) und nicht mit (peek-char)
;; eingelesen, weil evtl. hinter dem letzten Element der Liste noch
;; Kommentare stehen
;;------------------------------------------------------------------------------
(defun cons-reader (stream char)
  (declare (ignore char))
  (let ((*dot-flag* t)
        (*parenthesis-open* #\))
        list
        last-cons
        x)

    (setq x (bq-read stream))
    (when (null *parenthesis-open*)
      (return-from cons-reader nil))
    (when (null *dot-flag*)
      (error "Nothing appears before the dot in a list"))
    (setq list (setq last-cons (cons x nil)))
    (loop
      (setq x (bq-read stream))
      (when (null *parenthesis-open*)
        (return list))
      (cond
        ;; kein Dot gelesen
        ;;-----------------
        (*dot-flag* (setq last-cons (setf (cdr last-cons) (cons x nil))))

        ;; Dot gelesen, Sonderbehandlung
        ;;------------------------------
        (T (setf (cdr last-cons) (bq-read stream))
           (when (null *parenthesis-open*)
             (error "Nothing appears after the dot in a list"))
           (bq-read stream)
           (unless (null *parenthesis-open*)
             (error "More than one object found after dot in a list"))
           (return list))))))

;;------------------------------------------------------------------------------
(defun right-parenthesis-reader (stream char)
  (declare (ignore stream))
  (cond
    ((eql char *parenthesis-open*) (setq *parenthesis-open* nil) nil)
    (T (warn "Ignoring an unmatched ~a" char) (values))))

;;------------------------------------------------------------------------------
(defun quote-reader (stream char)
  (declare (ignore char))
  (list 'QUOTE (bq-read stream)))

;;------------------------------------------------------------------------------
(defun semicolon-reader (stream char)
  (declare (ignore char))
  (read-line stream nil nil nil)
  (values))

;;------------------------------------------------------------------------------
(defun string-reader (stream char &aux c)
  (setq *fill-pointer* 0)
  (loop
    (setq c (read-char stream t nil t))
    (cond
      ((eql (get-syntax c) 'SINGLE-ESCAPE)
       (setq c (read-char stream t nil t))
       (token-push-extend c))
      ((eql char c)
       (return (get-token-string)))
      (T (token-push-extend c)))))

;;------------------------------------------------------------------------------
(defun char-reader (stream char font)
  (declare (ignore char))
  (let ((c (read-char stream t nil t))
        (c2 (peek-char nil stream nil nil t)))
    
    ;; Steele (353): In the single-character case,
    ;; the character must be followed by a non-constituent character
    ;;--------------------------------------------------------------
    (when (and c2 (eql 'CONSTITUENT (get-syntax c2)))
      ;; multiple-character case
      ;;------------------------
      (setq *fill-pointer* 0)
      (let ((*read-suppress* t))
        (read-token stream c (get-syntax c)))
      (unless *read-suppress* 
        (setq c (name-char (get-token-string)))
        (when (null c) (error "illegal character name ~s" (get-token-string)))))

    (cond
      (*read-suppress* nil)
      (t (when (and font (> font 0))
           (warn "font ~s of ~s will be ignored" font c))
         c))))
  
;;------------------------------------------------------------------------------
(defun function-reader (stream char i)
  (when i (extra-argument char))
  (list 'function (bq-read stream)))

;;------------------------------------------------------------------------------
(defun vector-reader (stream char len)
  (let ((list (cons-reader stream char)))
    (cond (*read-suppress* nil)
          ((zerop *bq-level*)
           (if len
               (let ((listlen (length list)))
                 (when (> listlen len)
                   (error "Vector longer than specified length: #~S~S"
                          len list))
                 (fill (replace (make-array len) list)
                       (car (last list)) :start listlen))
               (apply #'vector list)))
          (len
           (error "#~s( syntax is not allowed in backquoted expressions" len))
          (t (cons *bq-vector* list)))))

;;------------------------------------------------------------------------------
(defun binary-reader (stream char i)
  (when i (extra-argument char))
  (radix-reader stream char 2))

;;------------------------------------------------------------------------------
(defun octal-reader (stream char i)
  (when i (extra-argument char))
  (radix-reader stream char 8))

;;------------------------------------------------------------------------------
(defun hex-reader (stream char i)
  (when i (extra-argument char))
  (radix-reader stream char 16))

;;------------------------------------------------------------------------------
(defun radix-reader (stream char radix)
  (cond (*read-suppress* (ignore-token stream))
	((not radix)
	 (error "Radix missing in #R."))
	((not (<= 2 radix 36))
	 (error "Illegal radix for #R: ~D." radix))
	(t
	 (let ((res (let ((*read-base* radix))
		      (read stream t nil t))))
	   (unless (typep res 'integer #|rational|#)
	     (error "#~A (base ~D) value is not a rational: ~S."
			   char radix res))
	   res))))

;;------------------------------------------------------------------------------
(defun uninterned-reader (stream char i)
  (when i (extra-argument char))
  (let* ((*uninterned* T)
         (value (read stream t nil t)))
    (cond
      (*read-suppress* nil)
      ((symbolp value) value)
      (T (error "illegal value (~S) followed #:" value)))))

;;------------------------------------------------------------------------------
(defun array-reader (stream char n)
  (cond
    (*read-suppress* (read stream t nil t) nil)
    ((null n) (error "Rank argument must be supplied to #~a" char))
    (t (let ((list (read stream t nil t)))
         (labels
             ((calc-dim (rank list)
                (cond
                  ((eql 0 rank) ())
                  (T (cons (length list) (calc-dim (1- rank) (car list)))))))
           (make-array (calc-dim n list) :initial-contents list))))))

;;------------------------------------------------------------------------------
(defun struct-reader (stream char i)
  (when i (extra-argument char))
  (let ((list (read stream t nil t)))
    (cond
      (*read-suppress* nil)
      ((atom list) (error "illegal value (~s) followed #S" list))
      (T (let ((constructor (rt:struct-constructor (car list)))
               (key-value-list (cdr list)))
           (unless constructor (error "~a is not a structure" (car list)))
           (do ((result ()))
               ((endp key-value-list) (apply constructor (nreverse result)))
             (push (intern (string (car key-value-list)) *keyword-package*)
                   result)
             (pop key-value-list)
             (when (endp key-value-list)
               (error "unexpected end in #S~s" key-value-list))
             (push (car key-value-list) result)
             (pop key-value-list)))))))

;;------------------------------------------------------------------------------
(defun feature-plus-reader (stream char i)
  (declare (ignore char))
  (when i (extra-argument #\+))
  (let ((feature
         (let ((*package* *keyword-package*))
           (read stream t nil t))))
    (if (eval-feature feature)
      (bq-read stream)
      (let ((*read-suppress* t))
        (bq-read stream)
        (values)))))

;;------------------------------------------------------------------------------
(defun feature-minus-reader (stream char i)
  (declare (ignore char))
  (when i (extra-argument #\-))
  (let ((feature
         (let ((*package* *keyword-package*))
           (read stream t nil t))))
    (if (eval-feature feature)
      (let ((*read-suppress* t))
        (bq-read stream)
        (values))
      (bq-read stream))))

;;------------------------------------------------------------------------------
(defun eval-feature (feature)
  (if (atom feature)
    (member feature *features*)
    (case (first feature)
      (:not (not (eval-feature (second feature))))
      (:and (dolist (feature (cdr feature))
              (unless (eval-feature feature)
                (return-from eval-feature nil)))
            T)
      (:or (dolist (feature (cdr feature))
             (when (eval-feature feature)
               (return-from eval-feature t)))
           nil)
      (T (error "illegal feature expression ~s" feature)))))
                    
  
;;------------------------------------------------------------------------------
(defun comment-block-reader (stream char i)
  (declare (ignore char))
  (when i (extra-argument #\|))
  (let ((c (read-char stream t nil t))
        (level 0))
    (loop
      (cond
        ((eql #\| c)
         (setq c (read-char stream t nil t))
         (when (eql #\# c)
           (when (eql 0 level) (return (values)))
           (decf level)
           (setq c (read-char stream t nil t))))
        ((eql #\# c)
         (setq c (read-char stream t nil t))
         (when (eql #\| c)
           (incf level)
           (setq c (read-char stream t nil t))))
        (T (setq c (read-char stream t nil t)))))))
  
;;------------------------------------------------------------------------------
(defun extra-argument (c)
  (error "extra argument for #~S" c))

;;------------------------------------------------------------------------------
(defun bq-read (stream)
  (internal-read stream t nil t))

;;------------------------------------------------------------------------------
(defun internal-read (stream eof-error-p eof-value recursive-p)
  (prog1
      (loop
        (let ((c (read-char stream nil nil))
              syntax)
       
          (when (null c)
            (when (or eof-error-p recursive-p)
              (error "unexpected End of File"))
   
            (return-from internal-read eof-value))
   
          (setq syntax (get-syntax c))
   
          (case syntax
   
            (WHITESPACE)                ;ignorieren
   
            ((CONSTITUENT SINGLE-ESCAPE MULTIPLE-ESCAPE)
             (return (progn (setq *fill-pointer* 0)
                            (read-token stream c syntax))))
   
            ((nil) (error "illegal Character"))
           
            ;; (Non-) Terminating-Macro
            ;;-------------------------
            (T (multiple-value-call
                   #'(lambda (&optional (v nil supplied))
                       (when supplied
                         (return v)))
                 (funcall (get-fun syntax) stream c))))))

    (when (and (not recursive-p) (not *preserve-whitespace*))
      (let ((c (read-char stream nil nil)))
        (cond

          ;; EOF (Annahme: Beim naechsten Lesen von stream kommt wieder EOF)
          ;; ---------------------------------------------------------------
          ((null c))

          ;; Weglesen von WHITESPACE auf Top-Level
          ;; -------------------------------------
          ((eql (get-syntax c) 'WHITESPACE))

          ;; Syntaktisch relevante Zeichen muessen erhalten bleiben
          ;; ------------------------------------------------------
          (t (unread-char c stream)))))))

;;------------------------------------------------------------------------------
(defun read (&optional (stream *standard-input*)
                          (eof-error-p t)
                          (eof-value nil)
                          (recursive-p nil))
  (case stream
    ((nil) (setq stream *standard-input*))
    ((t) (setq stream *terminal-io*)))

  (let ((*bq-level* 0))
    (if recursive-p
      (internal-read stream eof-error-p eof-value recursive-p)
      (let ((*preserve-whitespace* nil))
        (internal-read stream eof-error-p eof-value recursive-p)))))

;;------------------------------------------------------------------------------
(defun read-preserving-whitespace (&optional (stream *standard-input*)
                                             (eof-error-p t)
                                             (eof-value nil)
                                             (recursive-p nil))
  (case stream
    ((nil) (setq stream *standard-input*))
    ((t) (setq stream *terminal-io*)))
  (let ((*bq-level* 0))
    (if recursive-p
      (internal-read stream eof-error-p eof-value recursive-p)
      (let ((*preserve-whitespace* t))
        (internal-read stream eof-error-p eof-value recursive-p)))))

;;------------------------------------------------------------------------------
(defun read-delimited-list (char &optional stream recursive-p)
  (case stream
    ((nil) (setq stream *standard-input*))
    ((T) (setq stream *terminal-io*)))
  
  (labels
      ((read-rest ()
                  (if (eql char (peek-char t stream t nil t))
                    (progn (read-char stream) nil)
                    (cons (read stream t nil t) (read-rest)))))
    (read-rest)))

;;------------------------------------------------------------------------------
;; 2 Werte: 1. string, 2. (member nil t), T == eof
;;------------------------------------------------------------------------------
(defun read-line (&optional stream (eof-error-p t) eof-value recursive-p)
  (case stream
    ((nil) (setq stream *standard-input*))
    ((T) (setq stream *terminal-io*)))
  
  (let ((c (read-char stream nil nil)))
    (cond
      ((null c)
       (when eof-error-p (error "unexpected end of file"))
       (values eof-value t))
      (T (setq *fill-pointer* 0)
         (loop
           (when (eql #\Newline c) (return (values (get-token-string) nil)))
           (token-push-extend c)
           (setq c (read-char stream nil nil))
           (when (null c) (return (values (get-token-string) t))))))))

;;------------------------------------------------------------------------------
(defun peek-char (&optional peek-type stream (eof-error-p t)
                            eof-value recursive-p)
  (case stream
    ((nil) (setq stream *standard-input*))
    ((T) (setq stream *terminal-io*)))
  (let ((c (read-char stream nil nil recursive-p)))
    (cond
      ((eq t peek-type)
       (loop
         (when (null c) (return))
         (unless (eql 'WHITESPACE (get-syntax c))
           (return))
         (setq c (read-char stream nil nil recursive-p))))
      ((characterp peek-type)
       (loop
         (when (null c) (return))
         (unless (eql peek-type c)
           (return))
         (setq c (read-char stream nil nil recursive-p)))))
    
    (when (null c)
      (when eof-error-p (error "unexpected end of file"))
      eof-value)
    (prog1 c
      (unread-char c stream))))

;;------------------------------------------------------------------------------
(defun read-from-string (string &optional (eof-error-p t) eof-value
                                &key (start 0) (end (length string))
                                ((:preserve-whitespace *preserve-whitespace*))
                                &aux index (*bq-level* 0))
  (with-input-from-string (stream string :index index :start start :end end)
    (values (internal-read stream eof-error-p eof-value nil) index)))

;;------------------------------------------------------------------------------
(defun parse-integer (string &key (start 0) (end (length string))
                             (radix 10) (junk-allowed nil))
  (prog (c x d sign)

     ;; pruefen ob fuehrende WHITESPACE
     ;;--------------------------------
     (loop
       (when (>= start end) (go NO-INTEGER))
       (setq c (char string start))
       (unless (eq 'WHITESPACE (get-syntax c))
         (return)))

     ;; pruefen ob Vorzeichen
     ;;----------------------
     (setq sign (case c
                  (#\- (incf start) -1)
                  (#\+ (incf start) 1)
                  (t 1)))

     ;; mindestens 1 Ziffer lesen
     ;;--------------------------
     (when (>= start end) (go NO-INTEGER))
     (setq x (digit-char-p (char string start) radix))
     (cond
       ((null x) (go NO-INTEGER))
       (T (incf start)
          (loop
            (when (eql start end)
              (return))
            (setq d (digit-char-p (char string start) radix))
            (when (null d)
              (return))
            (incf start)
            (setq x (+ (* radix x) d)))))

     ;; Integer gefunden
     ;;-----------------
     (cond
       (junk-allowed (return (values x start)))

       ;; pruefen, ob nur WHITESPACE folgt
       ;;---------------------------------
       (T (loop
            (when (>= start end) (return))
            (setq c (char string start))
            (unless (eq 'WHITESPACE (get-syntax c)) (go ERROR)))
          (return (values x start))))

     NO-INTEGER
     (when junk-allowed
       (return (values nil start)))

     ERROR
     (error "illegal integer ~S" string)))

;;------------------------------------------------------------------------------
(defun make-standard-readtable ()
  (let* ((rtab (make-readtable))
         (syntax (readtable-syntax rtab)))
    
    (dolist (whitespace-char '(#\tab #\newline #\page #\return #\space))
      (setf (aref syntax (char-code whitespace-char)) 'WHITESPACE))
    
    (setf (aref syntax (char-code #\backspace)) 'CONSTITUENT)
    
    (do ((i (char-code #\!) (1+ i)))
        ((>= i (char-code #\rubout)))
      (setf (aref syntax i) 'CONSTITUENT))
    
    (setf (aref syntax (char-code #\\)) 'SINGLE-ESCAPE)
    (setf (aref syntax (char-code #\|)) 'MULTIPLE-ESCAPE)
    (make-dispatch-macro-character #\# T rtab)
    (set-dispatch-macro-character #\# #\\ #'char-reader rtab)
    (set-dispatch-macro-character #\# #\' #'function-reader rtab)
    (set-dispatch-macro-character #\# #\( #'vector-reader rtab)
    (set-dispatch-macro-character #\# #\: #'uninterned-reader rtab)
    (set-dispatch-macro-character #\# #\B #'binary-reader rtab)
    (set-dispatch-macro-character #\# #\O #'octal-reader rtab)
    (set-dispatch-macro-character #\# #\X #'hex-reader rtab)
    (set-dispatch-macro-character #\# #\R #'radix-reader rtab)
    (set-dispatch-macro-character #\# #\A #'array-reader rtab)
    (set-dispatch-macro-character #\# #\S #'struct-reader rtab)
    (set-dispatch-macro-character #\# #\+ #'feature-plus-reader rtab)
    (set-dispatch-macro-character #\# #\- #'feature-minus-reader rtab)
    (set-dispatch-macro-character #\# #\| #'comment-block-reader rtab)
    (set-macro-character #\' #'quote-reader NIL rtab)
    (set-macro-character #\( #'cons-reader NIL rtab)
    (set-macro-character #\) #'right-parenthesis-reader NIL rtab)
    (set-macro-character #\, #'comma-reader NIL rtab)
    (set-macro-character #\; #'semicolon-reader NIL rtab)
    (set-macro-character #\" #'string-reader NIL rtab)
    (set-macro-character #\` #'backquote-reader NIL rtab)

    rtab))

    
           

