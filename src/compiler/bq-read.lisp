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
;;; Funktion : eigener Backquote-Reader + Simplifier
;;;
;;; $Revision: 1.10 $
;;; $Id: bq-read.lisp,v 1.10 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Backquote ...
;;------------------------------------------------------------------------------
;;; angelehnt an "Backquote, Appendix C in CLtL, 2nd Ed."
;;; erweitert um Backquote in Vektoren.
;;------------------------------------------------------------------------------

(defvar *comma*        (make-symbol "COMMA"))
(defvar *comma-atsign* (make-symbol "COMMA-ATSIGN"))
(defvar *comma-dot*    (make-symbol "COMMA-DOT"))
(defvar *bq-list*      (make-symbol "BQ-LIST"))
(defvar *bq-append*    (make-symbol "BQ-APPEND"))
(defvar *bq-list**     (make-symbol "BQ-LIST*"))
(defvar *bq-nconc*     (make-symbol "BQ-NCONC"))
(defvar *bq-clobberable* (make-symbol "BQ-CLOBBERABLE"))
(defvar *bq-quote*     (make-symbol "BQ-QUOTE"))
(defvar *bq-quote-nil* (list *bq-quote* nil))
(defvar *bq-vector*    (make-symbol "BQ-VECTOR"))
(defvar *bq-level*     0)
(defvar *cons-reader*  (get-macro-character #\( ))
(defvar *vector-reader* (get-dispatch-macro-character #\# #\())

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
(defun vector-reader (stream char i)
  (if (> *bq-level* 0)
      (let ((list (funcall *cons-reader* stream char)))
        (when i
          (error "#~s( syntax is not allowed in backquoted expressions" i))
        (cons *bq-vector* list))
      (funcall *vector-reader* stream char i)))

;;------------------------------------------------------------------------------
(defun bq-read (stream)
  (read stream t nil t))


(set-dispatch-macro-character #\# #\( #'vector-reader)

(set-macro-character #\, #'comma-reader NIL)
(set-macro-character #\` #'backquote-reader NIL)

;;------------------------------------------------------------------------------
(provide "bq-read")
