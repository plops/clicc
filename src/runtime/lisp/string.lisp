;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funkion  : System-Funktionen (18. Strings)
;;;
;;; $Revision: 1.7 $
;;; $Log: string.lisp,v $
;;; Revision 1.7  1993/06/16  15:20:38  hk
;;;  Copyright Notiz eingefuegt.
;;;
;;; Revision 1.6  1993/05/08  18:13:50  hk
;;; Aufrufe von set-char-internal und set-schar-internal geaendert.
;;;
;;; Revision 1.5  1993/04/22  10:48:21  hk
;;; (in-package "RUNTIME") -> (in-package "LISP"),
;;; Definitionen exportiert, defvar, defconstant, defmacro aus
;;; clicc/lib/lisp.lisp einkopiert. rt::set-xxx in (setf xxx) umgeschrieben.
;;; Definitionen und Anwendungen von/aus Package Runtime mit rt: gekennzeichnet.
;;; declaim fun-spec und declaim top-level-form gestrichen.
;;;
;;; Revision 1.4  1993/02/16  14:34:20  hk
;;; clicc::declaim -> declaim, clicc::fun-spec (etc.) -> lisp::fun-spec (etc.)
;;; $Revision: 1.7 $ eingefuegt
;;;
;;; Revision 1.3  1992/07/06  10:15:30  hk
;;; STRING-CHAR vorlaeufig durch CHARACTER, (wegen CLTL2).
;;;
;;; Revision 1.2  1992/07/06  09:22:14  hk
;;; Neue Syntax fuer declaim fun-spec.
;;;
;;; Revision 1.1  1992/03/24  17:12:55  hk
;;; Initial revision
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(char schar string= string-equal string< string> string<= string>=
   string/= string-lessp string-greaterp string-not-greaterp
   string-not-lessp string-not-equal make-string string-trim
   string-left-trim string-right-trim string-upcase string-downcase
   string-capitalize nstring-upcase nstring-downcase nstring-capitalize
   string))

;;-----------------------------------------------------------------------------
;; 18.1. String Access
;;-----------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Fuer Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant NO_INDEX
   "The index arg ~S for the vector ~S is not a fixnum in range [~S, ~S).")

;;-----------------------------------------------------------------------------
;; CHAR string index
;;-----------------------------------------------------------------------------
(defun char (string index)
  (when (not (stringp string))
    (error WRONG_TYPE 'STRING string))
  (when (not (check-integer index 0 (1- (length string))))
    (error NO_INDEX index string 0 (length string)))
  (rt::char-internal string index))

;;-----------------------------------------------------------------------------
;; (SETF CHAR) character string index
;;-----------------------------------------------------------------------------
(defun (setf char) (character string index)
  (when (not (stringp string))
    (error WRONG_TYPE 'STRING string))
  (when (not (characterp character))
    (error WRONG_TYPE 'CHARACTER character))
  (when (not (check-integer index 0 (1- (length string))))
    (error NO_INDEX index string 0 (length string)))
  (rt::set-char-internal character string index))

;;-----------------------------------------------------------------------------
;; SCHAR simple-string index
;;-----------------------------------------------------------------------------
(defun schar (simple-string index)
  (declare (simple-string simple-string))
  (when (not (simple-string-p simple-string))
    (error WRONG_TYPE 'SIMPLE-STRING simple-string))
  (when (not (check-integer index 0 (1- (length simple-string))))
    (error NO_INDEX index simple-string 0 (length simple-string)))
  (rt::schar-internal simple-string index))

;;-----------------------------------------------------------------------------
;; (SETF SCHAR) character string index
;;-----------------------------------------------------------------------------
(defun (setf schar) (character string index)
  (when (not (stringp string))
    (error WRONG_TYPE 'STRING string))
  (when (not (characterp character))
    (error WRONG_TYPE 'CHARACTER character))
  (when (not (check-integer index 0 (1- (length string))))
    (error NO_INDEX index string 0 (length string)))
  (rt::set-schar-internal character string index))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

  (cond
    ((= (- end1 start1) (- end2 start2))
     (do ((i start1 (1+ i))
          (j start2 (1+ j)))
         ((= i end1) t)
       (when (not (char= (char string1 i) (char string2 j)))
         (return-from string= nil))))
    (t nil)))

;;-----------------------------------------------------------------------------
;; STRING-EQUAL string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string-equal (string1 string2
                             &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

  (cond
    ((= (- end1 start1) (- end2 start2))
     (do ((i start1 (1+ i))
          (j start2 (1+ j)))
         ((= i end1) t)
       (when (not (char-equal (char string1 i) (char string2 j)))
         (return-from string-equal nil))))
    (t nil)))

;;-----------------------------------------------------------------------------
;; STRING< string1 string2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun string< (string1 string2
                        &key (start1 0) (end1 nil) (start2 0) (end2 nil))
  (setq string1 (string string1))
  (setq string2 (string string2))
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (setq end1    (check-seq-start-end start1 end1 (length string1)))
  (setq end2    (check-seq-start-end start2 end2 (length string2)))

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
  (when (not (check-integer size 0 (1- array-dimension-limit)))
    (error "~S is not a legal size for a string." size))
  (when (not (typep initial-element 'CHARACTER))
    (error "The value of INITIAL-ELEMENT, ~S, should be a CHARACTER."
           initial-element))
  (rt::make-string-internal size initial-element))

;;-----------------------------------------------------------------------------
;; STRING-TRIM character-bag string
;;-----------------------------------------------------------------------------
(defun string-trim (character-bag string)
  (setq string (string string))
  (let* ((length (length string))
         (start       0)
         (end    length))
    (loop
      (when (= start length) (return-from string-trim ""))
      (when (not (find (char string start) character-bag :test #'char=))
        (return))
      (incf start))
    (loop
      (when (= start (1- end)) (return))
      (when (not (find (char string (1- end)) character-bag :test #'char=))
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
      (when (not (find (char string start) character-bag :test #'char=))
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
      (when (not (find (char string (1- end)) character-bag :test #'char=))
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
  (setq end    (check-seq-start-end start end (length string)))

  (setq string (copy-seq string))
  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-upcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; STRING-DOWNCASE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun string-downcase (string &key (start 0) (end nil))
  (setq string (string string))
  (setq end    (check-seq-start-end start end (length string)))

  (setq string (copy-seq string))
  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-downcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; STRING-CAPITALIZE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun string-capitalize (string &key (start 0) (end nil))
  (setq string (string string))
  (setq end    (check-seq-start-end start end (length string)))

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
  (setq end (check-seq-start-end start end (length string)))

  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-upcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; NSTRING-DOWNCASE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun nstring-downcase (string &key (start 0) (end nil))
  (setq end (check-seq-start-end start end (length string)))

  (do ((i start (1+ i)))
      ((= i end) string)
    (setf (schar string i) (char-downcase (schar string i)))))

;;-----------------------------------------------------------------------------
;; NSTRING-CAPITALIZE string &KEY :start :end
;;-----------------------------------------------------------------------------
(defun nstring-capitalize (string &key (start 0) (end nil))
  (setq end (check-seq-start-end start end (length string)))

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
