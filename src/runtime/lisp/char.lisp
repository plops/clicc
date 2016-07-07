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
;;; Function : Characters
;;;
;;; $Revision: 1.15 $
;;; $Id: char.lisp,v 1.15 1994/11/22 14:55:56 hk Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(standard-char-p graphic-char-p alpha-char-p upper-case-p lower-case-p
   both-case-p digit-char-p alphanumericp char= char/= char< char> char<=
   char>= char-equal char-not-equal char-lessp char-greaterp char-not-greaterp
   char-not-lessp char-code code-char character char-upcase char-downcase
   digit-char char-int char-name name-char))

(export 
 '(rt::char= rt::char/= rt::char< rt::char> rt::char<= rt::char>=
   rt::char-equal rt::char-not-equal rt::char-lessp rt::char-greaterp
   rt::char-not-greaterp rt::char-not-lessp) "RT")


;;------------------------------------------------------------------------------
;; Lokal, Typzusicherung
;;------------------------------------------------------------------------------
(defmacro must-be-character (char)
  `(if (characterp ,char)
    ,char
    (error WRONG_TYPE ,char 'character)))

;;------------------------------------------------------------------------------
;; STANDARD-CHAR-P char
;;------------------------------------------------------------------------------
(defun standard-char-p (char)
  (rt::standard-char-p (must-be-character char)))

;;------------------------------------------------------------------------------
;; GRAPHIC-CHAR-P char
;;------------------------------------------------------------------------------
(defun graphic-char-p (char)
  (rt::graphic-char-p (must-be-character char)))

;;------------------------------------------------------------------------------
;; ALPHA-CHAR-P char
;;------------------------------------------------------------------------------
(defun alpha-char-p (char)
  (rt::alpha-char-p (must-be-character char)))
  
;;------------------------------------------------------------------------------
;; UPPER-CASE-P char
;;------------------------------------------------------------------------------
(defun upper-case-p (char)
  (rt::upper-case-p (must-be-character char)))

;;------------------------------------------------------------------------------
;; LOWER-CASE-P char
;;------------------------------------------------------------------------------
(defun lower-case-p (char)
  (rt::lower-case-p (must-be-character char)))

;;------------------------------------------------------------------------------
;; BOTH-CASE-P char
;;------------------------------------------------------------------------------
(defun both-case-p (char)
  (rt::both-case-p (must-be-character char)))

;;------------------------------------------------------------------------------
;; DIGIT-CHAR-P char &optional (radix 10)
;;------------------------------------------------------------------------------
(defun digit-char-p (char &optional (radix 10))
  (unless (check-integer radix 2 36)
    (error "~A should be suitable radix" radix))
  (when (setq char (rt::digit-char (must-be-character char)))
    (if (< char radix)
        char
        nil)))

;;------------------------------------------------------------------------------
;; ALPHANUMERICP char
;;------------------------------------------------------------------------------
(defun alphanumericp (char)
  (or (alpha-char-p char) (digit-char-p char)))

;;------------------------------------------------------------------------------
;; Schnelle zweistellige Vergleichsfunktionen. Werden exportiert, damit der 
;; Optimierer die normalen Funktionen durch diese Varianten ersetzen kann.
;;------------------------------------------------------------------------------
(defun rt::char= (a b)
  (eql (must-be-character a) (must-be-character b)))
(defun rt::char/= (a b)
  (not (eql (must-be-character a) (must-be-character b))))
(defun rt::char< (a b)
  (< (char-code a) (char-code b)))
(defun rt::char> (a b)
  (> (char-code a) (char-code b)))
(defun rt::char<= (a b)
  (<= (char-code a) (char-code b)))
(defun rt::char>= (a b)
  (>= (char-code a) (char-code b)))

;;------------------------------------------------------------------------------
;; CHAR= char &rest more-chars
;;------------------------------------------------------------------------------
(defun char= (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char=))
  (dolist (c chars t)
    (when (rt::char/= c char)
      (return nil))))

;;------------------------------------------------------------------------------
;; CHAR/= char &rest more-chars
;;------------------------------------------------------------------------------
(defun char/= (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char/=))
  (do ((ch chars (cdr ch)))
      ((null ch) t)
    (when (dolist (c ch nil)
            (when (rt::char= char c)
              (return t)))
      (return nil))
    (setq char (car ch))))

;;------------------------------------------------------------------------------
;; CHAR< char &rest more-chars
;;------------------------------------------------------------------------------
(defun char< (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char<))
  (dolist (c chars t)
    (when (rt::char>= char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; CHAR> char &rest more-chars
;;------------------------------------------------------------------------------
(defun char> (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char>))
  (dolist (c chars t)
    (when (rt::char<= char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; CHAR<= char &rest more-chars
;;------------------------------------------------------------------------------
(defun char<= (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char<=))
  (dolist (c chars t)
    (when (rt::char> char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; CHAR>= char &rest more-chars
;;------------------------------------------------------------------------------
(defun char>= (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char>=))
  (dolist (c chars t)
    (when (rt::char< char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; dito für case-insensitive Vergleiche
;;------------------------------------------------------------------------------
(defun rt::char-equal (a b)
  (= (char-code (char-upcase a)) (char-code (char-upcase b))))
(defun rt::char-not-equal (a b)
  (/= (char-code (char-upcase a)) (char-code (char-upcase b))))
(defun rt::char-lessp (a b)
  (< (char-code (char-upcase a)) (char-code (char-upcase b))))
(defun rt::char-greaterp (a b)
  (> (char-code (char-upcase a)) (char-code (char-upcase b))))
(defun rt::char-not-greaterp (a b)
  (<= (char-code (char-upcase a)) (char-code (char-upcase b))))
(defun rt::char-not-lessp (a b)
  (>= (char-code (char-upcase a)) (char-code (char-upcase b))))

;;------------------------------------------------------------------------------
;; CHAR-EQUAL char &rest more-chars
;;------------------------------------------------------------------------------
(defun char-equal (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char-equal))
  (dolist (c chars t)
    (when (rt::char-equal c char)
      (return nil))))

;;------------------------------------------------------------------------------
;; CHAR-NOT-EQUAL char &rest more-chars
;;------------------------------------------------------------------------------
(defun char-not-equal (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char-not-equal))
  (do ((ch chars (cdr ch)))
      ((null ch) t)
    (when (dolist (c ch nil)
            (when (rt::char-not-equal char c)
              (return t)))
      (return nil))
    (setq char (car ch))))

;;------------------------------------------------------------------------------
;; CHAR-LESSP char &rest more-chars
;;------------------------------------------------------------------------------
(defun char-lessp (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char-lessp))
  (dolist (c chars t)
    (when (rt::char-not-lessp char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; CHAR-GREATERP char &rest more-chars
;;------------------------------------------------------------------------------
(defun char-greaterp (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char-greaterp))
  (dolist (c chars t)
    (when (rt::char-not-greaterp char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; CHAR-NOT-GREATERP char &rest more-chars
;;------------------------------------------------------------------------------
(defun char-not-greaterp (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char-not-greaterp))
  (dolist (c chars t)
    (when (rt::char-greaterp char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; CHAR-NOT-LESSP char &rest more-chars
;;------------------------------------------------------------------------------
(defun char-not-lessp (char &rest chars)
  (declare (:simp-when-n-args 2 rt::char-not-lessp))
  (dolist (c chars t)
    (when (rt::char-lessp char c)
      (return nil))
    (setq char c)))

;;------------------------------------------------------------------------------
;; CHAR-CODE char
;;------------------------------------------------------------------------------
(defun char-code (char)
  (rt::char-code (must-be-character char)))

;;------------------------------------------------------------------------------
;; CODE-CHAR code
;;------------------------------------------------------------------------------
(defun code-char (code)
  (if (check-integer code 0 char-code-limit)
      (rt::code-char code)
      nil))

;;------------------------------------------------------------------------------
;; CHARACTER object
;;------------------------------------------------------------------------------
(defun character (object)
  (cond
    ((characterp object) object)
    (T (when (symbolp object)
         (setq object (symbol-name object)))
       (if (and (stringp object) (= (length object) 1))
           (char object 0)
           (error "cannot coerce ~S to type character" object)))))

;;------------------------------------------------------------------------------
;; CHAR-UPCASE char
;;------------------------------------------------------------------------------
(defun char-upcase (char)
  (rt::char-upcase (must-be-character char)))

;;------------------------------------------------------------------------------
;; CHAR-DOWNCASE char
;;------------------------------------------------------------------------------
(defun char-downcase (char)
  (rt::char-downcase (must-be-character char)))

;;------------------------------------------------------------------------------
;; DIGIT-CHAR weight &optional (radix 10)
;;------------------------------------------------------------------------------
(defun digit-char (weight &optional (radix 10))
  (if (and (check-integer radix 2 36)
           (check-integer weight 0 (1- radix)))
      (char "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" weight)
      nil))

;;------------------------------------------------------------------------------
;; CHAR-INT char
;;------------------------------------------------------------------------------
(defun char-int (char)
  (char-code char))

;;------------------------------------------------------------------------------
;; CHAR-NAME char
;;------------------------------------------------------------------------------
(defun char-name (char)
  (must-be-character char)
  (cond
    ((char= char #\Space) "Space")
    ((char= char #\Newline) "Newline")
    ((char= char #\Backspace) "Backspace")
    ((char= char #\Tab) "Tab")
    ((char= char #\Linefeed) "Linefeed")
    ((char= char #\Page) "Page")
    ((char= char #\Return) "Return")
    ((char= char #\Rubout) "Rubout")
    (T nil)))

;;------------------------------------------------------------------------------
;; NAME-CHAR string
;;------------------------------------------------------------------------------
(defun name-char (string)
  (setq string (string string))
  (cond
    ((string-equal string "Space") #\Space) 
    ((string-equal string "Newline") #\Newline)
    ((string-equal string "Backspace") #\Backspace)
    ((string-equal string "Tab") #\Tab)
    ((string-equal string "Linefeed") #\Linefeed)
    ((string-equal string "Page") #\Page)
    ((string-equal string "Return") #\Return)
    ((string-equal string "Rubout") #\Rubout)
    (T nil)))
