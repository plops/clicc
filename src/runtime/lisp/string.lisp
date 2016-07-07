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
;;; Function  : Strings
;;;
;;; $Revision: 1.14 $
;;; $Id: string.lisp,v 1.14 1994/11/22 14:55:56 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(char schar string= string-equal string< string> string<= string>=
   string/= string-lessp string-greaterp string-not-greaterp
   string-not-lessp string-not-equal make-string string-trim
   string-left-trim string-right-trim string-upcase string-downcase
   string-capitalize nstring-upcase nstring-downcase nstring-capitalize
   string))

(defmacro set-seq-end (end length)
  `(when (null ,end) (setq ,end ,length)))

;;-----------------------------------------------------------------------------
;; 18.1. String Access
;;-----------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Für Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant NO_INDEX
   "The index arg ~S for the vector ~S is not a fixnum in range [~S, ~S).")

;;-----------------------------------------------------------------------------
;; CHAR string index
;;-----------------------------------------------------------------------------
(defun char (string index)
  (row-major-aref string index))

;;-----------------------------------------------------------------------------
;; (SETF CHAR) character string index
;;-----------------------------------------------------------------------------
(defun (setf char) (character string index)
  (setf (row-major-aref string index) character))

;;-----------------------------------------------------------------------------
;; SCHAR simple-string index
;;-----------------------------------------------------------------------------
(defun schar (simple-string index)
  (unless (simple-string-p simple-string)
    (error WRONG_TYPE simple-string 'simple-string))
  (unless (check-integer index 0 (1- (rt::plain-vector-length simple-string)))
    (error OUT_OF_RANGE index (rt::plain-vector-length simple-string)))
  (rt::pvref simple-string index))

;;-----------------------------------------------------------------------------
;; (SETF SCHAR) character simple-string index
;;-----------------------------------------------------------------------------
(defun (setf schar) (character simple-string index)
  (unless (simple-string-p simple-string)
    (error WRONG_TYPE simple-string 'simple-string))
  (unless (characterp character)
    (error WRONG_TYPE character 'character))
  (unless (check-integer index 0 (1- (rt::plain-vector-length simple-string)))
    (error OUT_OF_RANGE index (rt::plain-vector-length simple-string)))
  (rt::set-pvref character simple-string index))

;;-----------------------------------------------------------------------------
;; 18.2. String Comparison
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; STRING= string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string= (string1 string2
                        &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))
  
  (cond
    ((= (- end1 start1) (- end2 start2))
     (do ((i start1 (1+ i))
          (j start2 (1+ j)))
         ((= i end1) t)
       (unless (char= (char string1 i) (char string2 j))
         (return-from string= nil))))
    (t nil)))

;;-----------------------------------------------------------------------------
;; STRING-EQUAL string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string-equal (string1 string2
                             &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (cond
    ((= (- end1 start1) (- end2 start2))
     (do ((i start1 (1+ i))
          (j start2 (1+ j)))
         ((= i end1) t)
       (unless (char-equal (char string1 i) (char string2 j))
         (return-from string-equal nil))))
    (t nil)))

;;-----------------------------------------------------------------------------
;; STRING< string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string< (string1 string2
                        &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= j end2) (return nil))
    (when (= i end1) (return   i))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char< char1 char2)
       (return-from string< i))
      ((char= char1 char2))
      (t (return-from string< nil)))))

;;-----------------------------------------------------------------------------
;; STRING> string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string> (string1 string2
                        &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= i end1) (return nil))
    (when (= j end2) (return   i))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char> char1 char2)
       (return-from string> i))
      ((char= char1 char2))
      (t (return-from string> nil)))))

;;-----------------------------------------------------------------------------
;; STRING<= string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string<= (string1 string2
                         &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= i end1) (return   i))
    (when (= j end2) (return nil))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char< char1 char2) (return-from string<= i))
      ((char= char1 char2))
      (t (return-from string<= nil)))))

;;-----------------------------------------------------------------------------
;; STRING>= string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string>= (string1 string2
                         &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= j end2) (return   i))
    (when (= i end1) (return nil))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char> char1 char2)
       (return-from string>= i))
      ((char= char1 char2))
      (t (return-from string>= nil)))))

;;-----------------------------------------------------------------------------
;; STRING/= string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string/= (string1 string2
                         &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= i end1) (return (if (= j end2) nil i)))
    (when (= j end2) (return i))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char= char1 char2))
      (t (return-from string/= i)))))

;;-----------------------------------------------------------------------------
;; STRING-LESSP string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string-lessp (string1 string2
                             &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= j end2) (return nil))
    (when (= i end1) (return   i))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char-lessp char1 char2)
       (return-from string-lessp i))
      ((char-equal char1 char2))
      (t (return-from string-lessp nil)))))

;;-----------------------------------------------------------------------------
;; STRING-GREATERP string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string-greaterp (string1 string2
                                &key
                                (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= i end1) (return nil))
    (when (= j end2) (return   i))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char-greaterp char1 char2)
       (return-from string-greaterp i))
      ((char-equal char1 char2))
      (t (return-from string-greaterp nil)))))

;;-----------------------------------------------------------------------------
;; STRING-NOT-GREATERP string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string-not-greaterp (string1 string2
                                    &key
                                    (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= i end1) (return   i))
    (when (= j end2) (return nil))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char-lessp char1 char2)
       (return-from string-not-greaterp i))
      ((char-equal char1 char2))
      (t (return-from string-not-greaterp nil)))))

;;-----------------------------------------------------------------------------
;; STRING-NOT-LESSP string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string-not-lessp (string1 string2
                                 &key
                                 (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
       (j start2 (1+ j))
       char1
       char2)
      (nil)
    (when (= j end2) (return   i))
    (when (= i end1) (return nil))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char-greaterp char1 char2)
       (return-from string-not-lessp i))
      ((char-equal char1 char2))
      (t (return-from string-not-lessp nil)))))

;;-----------------------------------------------------------------------------
;; STRING-NOT-EQUAL string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string-not-equal (string1 string2
                                 &key
                                 (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (set-seq-end end1 (length string1))
  (set-seq-end end2 (length string2))

  (do ((i start1 (1+ i))
        (j start2 (1+ j))
        char1
        char2)
      (nil)
    (when (= i end1) (return (if (= j end2) nil i)))
    (when (= j end2) (return i))
    (setq char1 (char string1 i))
    (setq char2 (char string2 j))
    (cond
      ((char-equal char1 char2))
      (t (return-from string-not-equal i)))))

;;-----------------------------------------------------------------------------
;; 18.3. String Construction and Manipulation
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; MAKE-STRING size &KEY :initial-element
;;-----------------------------------------------------------------------------
(defun make-string (size &key (initial-element #\Space))
  (unless (check-integer size 0 (1- array-dimension-limit))
    (error "~S is not a legal size for a string." size))
  (unless (typep initial-element 'character)
    (error "The value of INITIAL-ELEMENT, ~S, should be a CHARACTER."
           initial-element))
  (rt::make-vector-char size initial-element))

;;-----------------------------------------------------------------------------
;; STRING-TRIM character-bag string
;;-----------------------------------------------------------------------------
(defun string-trim (character-bag string)
  (setq string (string string))
  (let* ((length (length string))
         (start 0)
         (end length))
    (loop
      (when (= start length) (return-from string-trim ""))
      (unless (find (char string start) character-bag :test #'char=)
        (return))
      (incf start))
    (loop
      (when (= start (1- end)) (return))
      (unless (find (char string (1- end)) character-bag :test #'char=)
        (return))
      (decf end))
    (if (and (zerop start) (= end length))
      string
      (subseq string start end))))

;;-----------------------------------------------------------------------------
;; STRING-LEFT-TRIM character-bag string
;;-----------------------------------------------------------------------------
(defun string-left-trim (character-bag string)
  (setq string (string string))
  (let ((length (length string))
        (start  0))
    (loop
      (when (= start length) (return-from string-left-trim ""))
      (unless (find (char string start) character-bag :test #'char=)
        (return))
      (incf start))
    (if (zerop start)
      string
      (subseq string start))))

;;-----------------------------------------------------------------------------
;; STRING-RIGHT-TRIM character-bag string
;;-----------------------------------------------------------------------------
(defun string-right-trim (character-bag string)
  (setq string (string string))
  (let* ((length (length string))
         (end    length))
    (loop
      (when (zerop end) (return-from string-right-trim ""))
      (unless (find (char string (1- end)) character-bag :test #'char=)
        (return))
      (decf end))
    (if (= end length)
      string
      (subseq string 0 end))))

;;-----------------------------------------------------------------------------
;; STRING-UPCASE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun string-upcase (string &key (start 0) (end nil))
  (setq string (string string))
  (set-seq-end end (length string))

  (setq string (copy-seq string))
  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-upcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; STRING-DOWNCASE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun string-downcase (string &key (start 0) (end nil))
  (setq string (string string))
  (set-seq-end end (length string))

  (setq string (copy-seq string))
  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-downcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; STRING-CAPITALIZE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun string-capitalize (string &key (start 0) (end nil))
  (setq string (string string))
  (set-seq-end end (length string))

  (setq string (copy-seq string))
  (do ((i start (1+ i))
       (word nil)
       char)
      ((= i end) string)
    (setq char (schar string i))
    (setf (schar string i)
          (cond
            ((alphanumericp char)
             (cond
               (word
                (char-downcase char))
               (t (setq word t)
                  (char-upcase   char))))
            (t (setq word nil)
               char)))))

;;-----------------------------------------------------------------------------
;; NSTRING-UPCASE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun nstring-upcase (string &key (start 0) (end nil))
  (set-seq-end end (length string))

  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-upcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; NSTRING-DOWNCASE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun nstring-downcase (string &key (start 0) (end nil))
  (set-seq-end end (length string))

  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-downcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; NSTRING-CAPITALIZE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun nstring-capitalize (string &key (start 0) (end nil))
  (set-seq-end end (length string))

  (do ((i start (1+ i))
       (word nil)
       char)
      ((= i end) string)
    (setq char (schar string i))
    (setf (schar string i)
          (cond
            ((alphanumericp char)
             (cond
               (word
                (char-downcase char))
               (t (setq word t)
                  (char-upcase   char))))
            (t (setq word nil)
               char)))))

;;-----------------------------------------------------------------------------
;; STRING x
;;-----------------------------------------------------------------------------
(defun string (x)
  (cond
    ((stringp x) x)
    ((symbolp x) (symbol-name x))
    ((characterp x) (make-string 1 :initial-element x))
    (t (error "It could not be generated a string from ~S" x))))
