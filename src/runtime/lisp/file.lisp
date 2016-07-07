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
;;; Function : OPEN, FILE-POSITION, FILE-LENGTH,
;;;            READ-CHAR, UNREAD-CHAR WRITE-CHAR
;;;
;;; $Revision: 1.15 $
;;; $Id: file.lisp,v 1.15 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(open file-position file-length read-char unread-char write-char))

;;------------------------------------------------------------------------------
;; Einschraenkungen:
;; - :ELEMENT-TYPE wird nicht unterstuetzt
;; - :RENAME, :RENAME-AND-DELETE werden nicht unterstuetzt
;; - :SUPERSEDE, :NEW-VERSION ueberschreiben eine evtl. bestehende Datei
;;------------------------------------------------------------------------------
(defun open (name &key (direction :input)
                  (if-exists :error)
                  (if-does-not-exist
                   (case direction
                     (:input :error)
                     ((:output :io)
                      (case if-exists
                        ((:overwrite :append) :error)
                        (T :create)))
                     (:probe nil)
                     (T (error "illegal option ~s" direction)))))
  (let (in-file out-file)

    (setq name (namestring name))

    ;; IF-DOES-NOT-EXIST bearbeiten
    ;;-----------------------------
    (setq in-file (rt::C-fopen name "r"))
    (unless in-file
      (case if-does-not-exist
        (:error (error "~s does not exist" name))
        ((nil) (return-from open nil))
        (:create
         (setq out-file (rt::C-fopen name "w+"))
         (when (null out-file) (error "can not create ~S" name)))
        (t (error "illegal option ~s" if-does-not-exist))))

    (labels
        ((get-io-file ()
           (cond
             ;; die Datei wurde neu angelegt
             ;;-----------------------------
             ((null in-file) out-file)

             ;; die Datei existiert schon
             ;;--------------------------
             (t (rt::C-fclose in-file)
                (case if-exists
                  (:error (error "file already exists ~s" name))
                  (:overwrite (rt::C-fopen name "r+"))
                  (:append (rt::C-fopen name "a+"))
                  ((:supersede :new-version) (rt::C-fopen name "w+"))
                  ((nil) (return-from open nil))
                  (T (error "unimplemented option ~S" if-exists)))))))
      
      (case direction
        ((:input :probe)
         (when out-file
           (close out-file)
           (setq in-file (rt::C-fopen name "r+")))
         (when (null in-file) (error "can not open ~S" name))
         (let ((stream (make-file-input-stream in-file name)))
           (when (eq direction :probe) (close stream))
           stream))

        (:output
         (make-file-output-stream (get-io-file) name))

        (:io
         (make-file-io-stream (get-io-file) name))))))

;;------------------------------------------------------------------------------
(defun file-position (stream &optional (position nil pos-supplied))
  (cond
    (pos-supplied (setq position
                        (typecase position
                          (integer
                           (unless (<= 0 position (file-length stream))
                             (error "illegal file-position")))
                          ((member :start) 0)
                          ((member :end) (file-length stream))
                          (T (error "illegal file-position: ~S" position))))
                   (funcall (stream-seek stream) position))
    (T (funcall (stream-tell stream)))))

;;------------------------------------------------------------------------------
(defun file-length (stream)
  (funcall (stream-length stream)))

;;------------------------------------------------------------------------------
(defun read-char (&optional stream (eof-error-p t) eof-value recursive-p)
  (case stream
    ((nil) (setq stream *standard-input*))
    ((t) (setq stream *terminal-io*))
    (T nil))
  (let ((c (funcall (stream-readc stream))))
    (cond
      (c c)
      (T (when (or eof-error-p recursive-p)
           (error "unexpected end-of-stream"))
         eof-value))))

;;------------------------------------------------------------------------------
(defun unread-char (c &optional stream)
  (case stream
    ((nil) (setq stream *standard-input*))
    ((t) (setq stream *terminal-io*))
    (T nil))
  
  (funcall (stream-unreadc stream) c))

;;------------------------------------------------------------------------------
(defun write-char (c &optional stream)
  (case stream
    ((nil) (setq stream *standard-output*))
    ((t) (setq stream *terminal-io*))
    (T nil))
  
  (funcall (stream-writec stream) c))
