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
;;; Function : FORMAT
;;;
;;; $Revision: 1.16 $
;;; $Id: format.lisp,v 1.16 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(format))

;;------------------------------------------------------------------------------
;; Einschraenkungen: Bisher sind nur wenige Format-Direktiven, zum Teil
;; unvollständig, implementiert.
;;------------------------------------------------------------------------------
(defun format (dest ctrl &rest args &aux stream)
  (cond
    ((or (null dest) (stringp dest))
     (setq stream (if dest
                      (make-string-output-stream dest)
                      (make-string-output-stream)))
     (unwind-protect
          (progn (format2 stream ctrl args)
                 (if dest nil (get-output-stream-string stream)))
       (close stream)))
    ((eql T dest) (format2 *standard-output* ctrl args) nil)
    ((output-stream-p dest) (format2 dest ctrl args) nil)
    (T (error "illegal destination ~S for format" dest))))

;;------------------------------------------------------------------------------
;; Terminate hat den Wert:  nil, #\), #\], #\}, #\>
;; Values: 1: die Liste der nicht verbrauchten Argumente
;;         2: Die Position hinter einem Terminator bzw. Separator,
;;         wenn die Bearbeitung hierdurch beendet wurde.
;;         3: Der Terminator bzw. Separator, der zur Beendigung der
;;         Bearbeitung führte.
;;------------------------------------------------------------------------------
(defun format2 (stream ctrl orig-args &optional
                       (ctrl-index 0) (terminator nil) (separator nil))
  (let ((args orig-args)
        (arg-index 0)
        (last-arg nil)
        (ctrl-len (length ctrl))
        c params
        param1
        at-sign colon)

    (labels
        ((get-arg ()
           (when (null args) (error "Not enough arguments for format"))
           (incf arg-index)
           (setq last-arg (pop args)))
         
         (get-ctrl ()
           (cond
             ((>= ctrl-index ctrl-len) nil)
             (T (prog1 (char ctrl ctrl-index)
                  (incf ctrl-index)))))
         
         (get-params (&aux c x d)
           (setq params (empty-queue))
           (loop
            (when (>= ctrl-index ctrl-len) (return))
            (setq c (char ctrl ctrl-index))
            (case c
              (#\#       (add-q (length args) params)
                         (incf ctrl-index))
              ((#\V #\v) (add-q (get-arg) params)
                         (incf ctrl-index))
              (#\,       (add-q nil params))
              (T (setq x (digit-char-p c))
                 (when x
                   (loop
                    (incf ctrl-index)
                    (when (>= ctrl-index ctrl-len)
                      (add-q x params) (return))
                    (setq c (char ctrl ctrl-index))
                    (setq d (digit-char-p c))
                    (when (null d) (add-q x params) (return))
                    (setq x (+ (* 10 x) d))))))
            (if (and (< ctrl-index ctrl-len) (eql #\, (char ctrl ctrl-index)))
                (incf ctrl-index)
                (return)))
           (setq params (queue2list params)))
         
         (get-at-sign-or-colon ()
           (setq at-sign nil
                 colon nil)
           (loop
            (cond
              ((>= ctrl-index ctrl-len)
               (error "Syntax error at end of control-string: ~s" ctrl))
              ((eql (char ctrl ctrl-index) #\@)
               (when at-sign
                 (error "Too many at-signs supplied."))
               (setq at-sign T)
               (incf ctrl-index))
              ((eql (char ctrl ctrl-index) #\:)
               (when colon
                 (error "Too many colons supplied."))
               (setq colon T)
               (incf ctrl-index))
              (t (return)))))
         
         (get-directive ()
           (cond
             ((>= ctrl-index ctrl-len)
              (error "Syntax error at end of control-string: ~s" ctrl))
             (T (prog1 (char ctrl ctrl-index)
                  (incf ctrl-index)))))
         
         (skip (terminator &optional separator-stops)
           (loop
            (case (get-ctrl)
              ((nil) (error "~~~A expected in control-string" terminator))
              (#\~ (get-params)
                   (get-at-sign-or-colon)
                   (let ((c (get-directive)))
                     (if (eql c terminator)
                         (return c)
                         (case c
                           (#\( (skip #\)))
                           (#\[ (skip #\]))
                           (#\{ (skip #\}))
                           (#\< (skip #\>))
                           (#\~ (when separator-stops
                                  (return c)))
                           (T nil)))))
              (T nil))))
         
         (insert-space (param col)
           (when (numberp param)
             (setq col (- (funcall (stream-column stream)) col))
             (do ()
                 ((>= col param))
               (write-char #\Space stream)
               (incf col))))

         (tabbing (at-sign &optional (column 1) (colinc 1))
           (cond
             ((null at-sign)
              (let ((current (funcall (stream-column stream))))
                (cond
                  ;; unknown current: simply output two spaces
                  ((null current)
                   (write-char #\Space stream)
                   (write-char #\Space stream))

                  ;; move to column
                  ((> column current)
                   (dotimes (i (- column current))
                     (write-char #\Space stream)))

                  ;; already at or beyond column
                  ((> colinc 1)
                   (let ((beyound (- current column)))
                     (dotimes (i (- colinc (mod beyound colinc)))
                       (write-char #\Space stream)))))))

             ;; relative tabulation
             (T (dotimes (i column)
                  (write-char #\Space stream))
                (when (> colinc 1)
                  (let ((current (funcall (stream-column stream))))
                    (when current
                      (dotimes (i (- colinc (mod current colinc)))
                        (write-char #\Space stream))))))))
         
         (padding (len &optional 
                       (mincol 0)
                       (colinc 1)
                       (minpad 0)
                       (padchar #\space))
           (dotimes (i minpad)
             (write-char padchar stream))
           (incf len minpad)
           (do ()
               ((>= len mincol))
             (dotimes (i colinc)
               (write-char padchar stream))
             (incf len colinc))))
      
      (loop
        (setq c (get-ctrl))
        (when (null c)
          (when terminator
            (error "~~~A expected" terminator))
          (return))
        (cond
          ((not (eql c #\~)) (write-char c stream))
          (T (get-params)
             (setq param1 (first params))
             (get-at-sign-or-colon)
             (setq c (get-directive))
             (case (char-upcase c)
               
;;;-----------------------------------------------------------------------------
               (#\A
                (if params
                    (let* ((arg (get-arg))
                           (str (if (and colon (null arg))
                                    "()"
                                    (princ-to-string arg)))
                           (len (length str)))
                      (cond
                        (at-sign (apply #'padding len params)
                                 (princ str stream))
                        (t (princ str stream)
                           (apply #'padding len params))))
                    (princ (if colon (or (get-arg) "()") (get-arg)) stream)))

;;;-----------------------------------------------------------------------------
               (#\S
                (cond
                  (params (let* ((arg (get-arg))
                                 (str (if (and colon (null arg))
                                          "()"
                                          (prin1-to-string arg)))
                                 (len (length str)))
                            (cond
                              (at-sign (apply #'padding len params)
                                       (princ str stream))
                              (t (princ str stream)
                                 (apply #'padding len params)))))
                  (colon (let ((arg (get-arg)))
                           (if arg
                               (prin1 arg stream)
                               (princ "()" stream))))
                  (t (prin1 (get-arg) stream))))
               
;;;-----------------------------------------------------------------------------
               (#\D
                  (let ((col (funcall (stream-column stream)))
                        (arg (get-arg))
                        (*PRINT-RADIX*  NIL)
                        (*PRINT-BASE*   10))
                     (when (and (integerp arg) at-sign (plusp arg))
                       (write-char #\+ stream))
                     (princ arg stream)
                     (insert-space param1 col)))
;;;-----------------------------------------------------------------------------
               (#\B
                  (let ((col (funcall (stream-column stream)))
                        (*PRINT-RADIX*  NIL)
                        (*PRINT-BASE*   2))
                     (princ (get-arg) stream)
                     (insert-space param1 col)))
;;;-----------------------------------------------------------------------------
               (#\O
                  (let ((col (funcall (stream-column stream)))
                        (*PRINT-RADIX*  NIL)
                        (*PRINT-BASE*   8))
                     (princ (get-arg) stream)
                     (insert-space param1 col)))
;;;-----------------------------------------------------------------------------
               (#\P
                (let ((arg (if colon last-arg (get-arg))))
                  (if (eql arg 1)
                      (when at-sign (write-char #\y stream))
                      (if at-sign
                          (princ "ies" stream) (write-char #\s stream)))))
;;;-----------------------------------------------------------------------------
               (#\F
                  (let ((col (funcall (stream-column stream))))
                     (princ (get-arg) stream)
                     (insert-space param1 col)))
;;;-----------------------------------------------------------------------------
               (#\% (if (numberp param1)
                      (dotimes (i param1) (terpri stream))
                      (terpri stream)))
;;;-----------------------------------------------------------------------------
               (#\& (if (numberp param1)
                        (unless (zerop param1)
                          (fresh-line stream)
                          (dotimes (i (1- param1)) (terpri stream)))
                        (fresh-line stream)))
;;;-----------------------------------------------------------------------------
               (#\| (if (numberp param1)
                      (dotimes (i param1) (write-char #\Page stream))
                      (write-char #\Page stream)))
;;;-----------------------------------------------------------------------------
               (#\~ (if (numberp param1)
                      (dotimes (i param1) (write-char #\~ stream))
                      (write-char #\~ stream)))
;;;-----------------------------------------------------------------------------
               (#\Newline
                (when at-sign
                  (when colon (error "illegal directive ~~:@p"))
                  (terpri stream))
                (unless colon
                  (loop
                   (setq c (get-ctrl))
                   (when (null c) (return))
                   (unless (eq c #\Space)
                     (decf ctrl-index)
                     (return)))))
;;;-----------------------------------------------------------------------------
               (#\T (apply #'tabbing at-sign params))
;;;-----------------------------------------------------------------------------
               (#\*
                (labels ((goto (n)
                           (cond
                             ((eql n 0)
                              (setq args orig-args)
                              (setq arg-index 0)
                              (setq last-arg nil))
                             (t (setq args (nthcdr orig-args (1- n)))
                                (setq arg-index n)
                                (get-arg))))) 
                  (cond
                    (at-sign
                     (when colon (error "illegal directive ~~:@*"))
                     (unless (numberp param1) (setq param1 0))
                     (goto param1))
                    (colon
                     (when (not (numberp param1))
                       (setq param1 1))
                     (when (> param1 arg-index)
                       (error "~~~a:* will skip to nonexistant argument"
                              param1))
                     (goto (- arg-index param1)))
                    (t (when (not (numberp param1))
                         (setq param1 1))
                       (dotimes (i param1) (get-arg))))))
;;;-----------------------------------------------------------------------------
               (#\? (funcall #'format2 stream (get-arg) (get-arg)))

;;;-----------------------------------------------------------------------------
               (#\; (if (or (eql separator #\;) (eql terminator #\;))
                        (return-from format2 (values args ctrl-index #\;))
                        (error "unexpected ~~; in control-string")))
               
;;;-----------------------------------------------------------------------------
               (#\[ (cond
                      ((and colon at-sign)
                       (error "Illegal directive ~~:@["))
                      
                      (colon
                       (cond
                         ((null (get-arg))
                          
                          ;; first clause if arg is nil
                          ;;---------------------------
                          (multiple-value-setq (args ctrl-index)
                            (funcall #'format2
                                     stream ctrl args ctrl-index #\;))
                          (skip #\]))
                         (t

                          ;; second clause if arg is not nil
                          ;;---------------------------------
                          (skip #\;)
                          (multiple-value-setq (args ctrl-index)
                            (funcall #'format2
                                     stream ctrl args ctrl-index #\])))))

                      (at-sign
                       (let ((old-args args))
                         (when (get-arg)

                           ;; use clause if arg is not nil with arg as argument
                           ;;--------------------------------------------------
                           (multiple-value-setq (args ctrl-index)
                             (funcall #'format2
                                      stream ctrl old-args ctrl-index #\])))))

                      (t
                       (let ((index (if param1 param1 (get-arg))))
                         (loop

                          ;; Use this clause
                          ;;----------------
                          (when (eql 0 index)
                            (let (terminated-by)
                              (multiple-value-setq (args ctrl-index
                                                         terminated-by)
                                (funcall #'format2
                                         stream ctrl args ctrl-index #\] #\;))
                              (when (eql #\; terminated-by)
                                (skip #\])))
                            (return))
                         
                          (if (eql (skip #\] #\;) #\;)
                              (cond

                                ;; Default
                                ;;--------
                                (colon
                                 (multiple-value-setq (args ctrl-index)
                                   (funcall #'format2
                                            stream ctrl args ctrl-index #\]))
                                 (return))

                                ;; Try next
                                ;;---------
                                (t (decf index)))

                              ;; No clause applicable
                              ;;---------------------
                              (return)))))))

;;;-----------------------------------------------------------------------------
               (#\] (if (eql terminator #\])
                        (return-from format2 (values args ctrl-index #\]))
                        (error "unexpected ~~} in control-string")))

;;;-----------------------------------------------------------------------------
               (#\{ (let ((arg (get-arg))
                          (new-ctrl-index nil))
                      (unless param1 (setq param1 -1))
                      
                      (loop
                        (when (null arg) (return))
                        (when (eql param1 0) (return))
                        (multiple-value-setq (arg new-ctrl-index)
                          (funcall #'format2
                                   stream ctrl arg ctrl-index #\}))
                        (decf param1))

                      (cond
                        (new-ctrl-index (setq ctrl-index new-ctrl-index))
                        (t (let ((old-ctrl-index ctrl-index))
                             (skip #\})
                             (when (and colon (not (eql 0 param1)))

                               ;; Mindestens einmal
                               ;;------------------
                               (multiple-value-setq (arg new-ctrl-index)
                                 (funcall #'format2
                                          stream
                                          ctrl arg old-ctrl-index #\}))))))))

;;;-----------------------------------------------------------------------------
               (#\} (if (eql terminator #\})
                        (return-from format2 (values args ctrl-index))
                        (error "unexpected ~~} in control-string")))
;;;-----------------------------------------------------------------------------
               (T (error "illegal / unimplemented directive (~A) in ~
                          control-string" c))))))))

  (when terminator (error "unexpected end of control-string")))
