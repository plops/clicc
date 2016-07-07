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
;;; Function : Main function, when using CLiCC as stand alone application
;;;
;;; $Revision: 1.19 $
;;; $Id: clicc.lisp,v 1.19 1994/11/25 14:52:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP" :nicknames '("CLICC-LISP" "L") :use ())
(in-package "USER")                     ; Damit es dieses Package wirklich gibt
(lisp:in-package "RT" :use ())
(lisp:in-package "FFI" :use ())
(lisp:export
 '(c-array c-char c-char-p c-double c-double-p c-enum c-float c-float-p c-fun
   c-handle c-int c-int-p c-long c-long-double c-long-double-p c-long-p c-ptr
   c-short c-short-p c-string c-string-p c-struct c-union c-unsigned-char
   c-unsigned-char-p c-unsigned-int c-unsigned-int-p c-unsigned-long
   c-unsigned-long-p c-unsigned-short c-unsigned-short-p c-vararg c-void
   copy-c-cstring def-c-type def-call-in def-call-out foreign-package-name
   free lisp-character lisp-float lisp-integer load-foreign make-c-string
   make-lisp-string sys-include user-include)
 "FFI")

(lisp:in-package "CLICC" :use '("LISP"))

;;------------------------------------------------------------------------------
(require "clcmain")

(defconstant *COMMAND-OPTIONS* '((#\V . *SHOW-VERSION*)
                                 (#\v . *CLICC-PRINT*)
                                 (#\m . *MODULE-COMPILER*)
                                 (#\l . *LISP-MODULE*)
                                 (#\i . *INLINE-MODULE*)
                                 (#\s . *SPLIT-FILES*)
                                 (#\f . *FLAT-IFS*)
                                 (#\t *TI-LEVEL*)
                                 (#\R *OBREP*)
                                 (#\O *ITERATIONS*)
                                 (#\c *C-max-line-count*)
                                 (#\o *OUT-FILENAME*)))



(defun clicc-main ()
  (labels
      ((usage ()
         (format
          t
"Usage: clicc [-lmsvV] [-c count] [-t 0-3] [-O level] [-R 1-3] [-o name] name~%~
                  ~A-c: Maximum line count of C file~%~
                  ~A-f: Flat IFs~%~
                  ~A-i: Inline Module~%~
                  ~A-l: Lisp Module~%~
                  ~A-m: Module-Compiler~%~
                  ~A-O: Number of optimization cycles~%~
                  ~A-s: Split Files~%~
                  ~A-t: Type Inference Level 0, 1, 2 or 3~%~
                  ~A-R: Data Representation schema 1, 2 or 3~%~
                  ~A-v: Verbose~%~
                  ~A-V: Show Version~%~
                  ~A-o: Output file~%~
                  ~Aname: The Lisp File~%"
#\Tab #\Tab #\Tab #\Tab #\Tab #\Tab #\Tab #\Tab #\Tab #\Tab #\Tab #\Tab #\Tab)
         (return-from clicc-main)))
       
    (setq *MODULE-COMPILER* nil
          *LISP-MODULE* nil
          *INLINE-MODULE* nil
          *OUT-FILENAME* nil
          *FILENAME* nil)

    (let ((options (cdr (rt::c-argv))))
      (when (null options)
        (usage))
      (do ((opt (car options) (car options)))
          ((endp options)
           (unless *FILENAME* (usage))
           (when (stringp *ITERATIONS*)
             (setq *ITERATIONS* (read-from-string *ITERATIONS*))
             (unless (integerp *ITERATIONS*) (usage))
             (when (zerop *ITERATIONS*) (setq *optimize* nil)))
           (when (stringp *TI-LEVEL*)
             (let ((l (digit-char-p (character *TI-LEVEL*))))
               (if l (setq *TI-LEVEL* l) (usage))))
           (when (stringp *OBREP*)
             (let ((o (digit-char-p (character *OBREP*))))
               (if (and o (<= 1 o 3)) (setq *OBREP* o) (usage))))
           (when (stringp *C-max-line-count*)
             (setq *C-max-line-count* (read-from-string *C-max-line-count*))
             (unless (integerp *C-max-line-count*) (usage)))
           (do-clicc))
        (pop options)
        (cond
          ((eql #\- (char  opt 0))
           (dotimes (i (1- (length opt)))
             (let ((var (cdr (assoc (char opt (1+ i)) *COMMAND-OPTIONS*)))) 
               (if var
                   (if (atom var)
                       (set var t)
                       (if (consp options)
                           (setf (symbol-value (car var)) (pop options))
                           (usage)))
                   (usage)))))
          ((not *FILENAME*) (setq *FILENAME* opt))
          (T (usage)))))))

(clicc-main)
