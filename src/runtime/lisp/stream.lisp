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
;;; Funktion : - STREAM als LISP-Struktur
;;;            - SYNONYM-, BROADCAST-, CONCATENATED-, TWO-WAY-, ECHO-,
;;;              STRING-INPUT-, STRING-OUTPUT-STREAM
;;;            - FILE-...-STREAM
;;;            - CLOSE
;;;
;;; $Revision: 1.14 $
;;; $Id: stream.lisp,v 1.14 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(*terminal-io* *standard-input* *standard-output* *error-output* *query-io*
   stream streamp
   make-synonym-stream make-broadcast-stream make-concatenated-stream
   make-two-way-stream make-echo-stream make-string-input-stream
   make-string-output-stream get-output-stream-string input-stream-p
   output-stream-p stream-element-type close))

(export '(rt::file-name rt::make-stream) "RT")

;;------------------------------------------------------------------------------
(defparameter *terminal-io*     (make-terminal-io))
(defparameter *standard-input*  (make-synonym-stream '*terminal-io*))
(defparameter *standard-output* *standard-input*)
(defparameter *error-output*    *standard-input*)
(defparameter *query-io*        *standard-input*)

;;------------------------------------------------------------------------------
(defstruct (stream (:copier nil)
                   (:predicate streamp)
                   (:constructor rt::make-stream)
                   (:print-function print-stream))
  type

  ;; zusaetzliche Informationen:
  ;; - File-name bei File-Streams
  ;; - Symbol bei Synonym-Streams
  ;; - get-output-stream-string bei string-output-streams
  ;;-----------------------------------------------------
  extra
  (readc #'undef-stream-op)
  (unreadc #'undef-stream-op)
;;; (listen #'undef-stream-op)
  (writec #'undef-stream-op)
  (column #'nil-fun)
  (tell #'nil-fun)
  (seek #'undef-stream-op)
  (length #'undef-stream-op)
  close)

;;------------------------------------------------------------------------------
(defun print-stream (stream to-stream depth)
  (write-string "#<" to-stream)
  (princ (stream-type stream) to-stream)
  (write-string "-STREAM" to-stream)
  (when (member (stream-type stream) '(FILE-INPUT FILE-OUTPUT FILE-IO))
    (write-char #\Space to-stream)
    (write-string (stream-extra stream) to-stream))
  (write-char #\> to-stream))
  
;;------------------------------------------------------------------------------
(defun undef-stream-op (&rest x)
  (declare (ignore x))
  (error "undefined Stream Operation"))

;;------------------------------------------------------------------------------
(defun nil-fun () nil)

;;------------------------------------------------------------------------------
(defun close-err (&rest x)
  (declare (ignore x))
  (error "stream is closed"))

;;------------------------------------------------------------------------------
;; verbietet weitere E/A-Operationen auf dem Stream
;; wiederholtes Close bleibt ohne Wirkung
;;------------------------------------------------------------------------------
;; CLtL,2.Ed: Closing a composite stream has no effect on its contents
;;------------------------------------------------------------------------------
(defun close1 (stream)
  (setf (stream-readc stream) #'close-err)
  (setf (stream-unreadc stream) #'close-err)
;;;  (setf (stream-listen stream) #'close-err)
  (setf (stream-writec stream) #'close-err)
  (setf (stream-tell stream) #'close-err)
  (setf (stream-seek stream) #'close-err)
  (setf (stream-close stream)
        ;; Identitaet
        ;;-----------
        #'(lambda (stream) (declare (ignore stream)) "already closed")))

;;------------------------------------------------------------------------------
;; Resultat von readc ist nil, wenn End-of-File. !!!
;;------------------------------------------------------------------------------
;;------------------------------------------------------------------------------
(defun make-synonym-stream (symbol)
  (rt::make-stream :type 'SYNONYM
                   :extra symbol
                   :readc
                   #'(lambda () (funcall (stream-readc (symbol-value symbol))))
                   :unreadc
                   #'(lambda (c)
                       (funcall (stream-unreadc (symbol-value symbol)) c))
                   :writec
                   #'(lambda (c)
                       (funcall (stream-writec (symbol-value symbol)) c))
;;;                :listen
;;;                #'(lambda () (funcall (stream-listen (symbol-value symbol))))
                   :column #'(lambda ()
                               (funcall (stream-column (symbol-value symbol))))
                   :tell
                   #'(lambda () (funcall (stream-tell (symbol-value symbol))))
                   :seek
                   #'(lambda () (funcall (stream-seek (symbol-value symbol))))
                   :length
                   #'(lambda () (funcall (stream-length (symbol-value symbol))))
                   :close
                   #'close1))

;;------------------------------------------------------------------------------
(defun make-broadcast-stream (&rest streams)
  (rt::make-stream :type 'BROADCAST
                   :writec
                   #'(lambda (c) 
                       (dolist (s streams)
                         (funcall (stream-writec s) c)))
                   :close #'close1))

;;------------------------------------------------------------------------------
(defun make-concatenated-stream (&rest streams)
  (rt::make-stream
   :type 'CONCATENATED
   :readc
   (labels
       ((readc ()
          (if (null streams)
              nil
              (progn
                (let ((c (funcall (stream-readc (car streams)))))
                  (if c
                      c
                      (progn
                        (pop streams)
                        (readc))))))))
     #'readc)
   :unreadc
               
   ;; Error, wenn streams == NIL
   ;;---------------------------
   #'(lambda (c) (funcall (stream-unreadc (car streams)) c))
;;;            :listen
;;;            #'(lambda () (if (null streams)
;;;                           nil
;;;                           (funcall (stream-listen (car streams)))))
   :close #'close1))

;;------------------------------------------------------------------------------
(defun make-two-way-stream (in out)
  (rt::make-stream :type 'TWO-WAY
                   :readc #'(lambda () (funcall (stream-readc in)))
                   :unreadc #'(lambda () (funcall (stream-unreadc in)))
;;;                :listen #'(lambda () (funcall (stream-listen in)))
                   :writec #'(lambda () (funcall (stream-writec out)))
                   :column #'(lambda () (funcall (stream-column out)))
                   :close #'close1))
               
;;------------------------------------------------------------------------------
(defun make-echo-stream (in out)
  (let ((unread-count 0))
    (rt::make-stream
     :type 'ECHO
     :readc
     #'(lambda()
         (let ((c (funcall (stream-readc in))))
           (if c
               ;; echo, wenn nicht EOF und nicht unread-char
               (if (eql unread-count 0)
                   (progn
                     (funcall (stream-writec out) c)
                     c)
                   (decf unread-count))
               ;; EOF, kein Echo
               nil)))
     :unreadc #'(lambda (c)
                  (funcall (stream-unreadc in) c) 
                  (incf unread-count))
;;;              :listen #'(lambda () (funcall (stream-listen in)))
     :writec #'(lambda (c) (funcall (stream-writec out) c))
     :column #'(lambda () (funcall (stream-column out)))
     :close #'close1)))
               
;;------------------------------------------------------------------------------
(defun make-string-input-stream (string &optional
                                        (start 0) (end (length string)))
  (let ((index start))
    (rt::make-stream
     :type 'STRING-INPUT
     :readc
     #'(lambda ()
         (if (< index end)
             (prog1 (char string index)
               (incf index))
                     
             ;; EOF
             ;;----
             nil))
     :unreadc
     #'(lambda (c)
         (when (<= index start) (error "reached start of stream"))
         (decf index)
         (unless (eql c (char string index))
           (error "%s should be eql to %s" c (char string index)))
         nil)
;;;            :listen #'(lambda () (< index end))
     :tell #'(lambda () index)
     :seek #'(lambda (pos) (if (<= pos end)
                               (setq index pos)
                               (error "illegal position")))
     :length #'(lambda () end)
     :close #'close1)))

;;------------------------------------------------------------------------------
;; zusaetzlicher Parameter STRING, damit WITH-OUTPUT-TO-STRING implementiert
;; werden kann.
;;------------------------------------------------------------------------------
(defun make-string-output-stream
  (&optional (string (make-array 10 :element-type 'character
                                 :adjustable t
                                 :fill-pointer 0))
   &aux (column 0))
  (rt::make-stream
   :type 'STRING-OUTPUT
   :writec
   #'(lambda (c)
       (if (eql c #\Newline)
           (setq column 0)
           (incf column))
       (vector-push-extend c string))
   :column #'(lambda () column)
   :tell #'(lambda () (length string))
   :length #'(lambda () (length string))

   ;; fuer GET-OUTPUT-STREAM-STRING
   ;;------------------------------
   :extra
   #'(lambda () (prog1 (make-array (length string)
                                   :element-type 'character
                                   :initial-contents string)
                  (setf (fill-pointer string) 0)))
   :close #'close1))

;;------------------------------------------------------------------------------
(defun get-output-stream-string (stream)
  (unless (eq `STRING-OUTPUT (stream-type stream))
    (error "string-output-stream expected"))
  (funcall (stream-extra stream)))

;;------------------------------------------------------------------------------
(defun input-stream-p (stream)
  (if (eq 'SYNONYM (stream-type stream))
    (input-stream-p (symbol-value (stream-extra stream)))
    (member (stream-type stream)
            `(FILE-INPUT FILE-IO CONCATENATED TWO-WAY ECHO STRING-INPUT))))

;;------------------------------------------------------------------------------
(defun output-stream-p (stream)
  (if (eq 'SYNONYM (stream-type stream))
    (output-stream-p (symbol-value (stream-extra stream)))
    (member (stream-type stream)
            `(FILE-OUTPUT FILE-IO BROADCAST TWO-WAY ECHO STRING-OUTPUT))))

;;------------------------------------------------------------------------------
(defun stream-element-type (stream)
  (if (streamp stream) (error "stream expected") 'character))

;;------------------------------------------------------------------------------
;; rt:file-name stream &optional new-name
;;------------------------------------------------------------------------------
(defun rt:file-name (stream  &optional new-name)
  (unless (member (stream-type stream) '(FILE-INPUT FILE-OUTPUT FILE-IO))
    (error "there is no filename associated with stream ~a" stream))
  (when new-name
    (setf (stream-extra stream) new-name))
  (stream-extra stream))

;;----------------------------------------------------------------------------
(defun make-file-input-stream (C-file name)
  (rt::make-stream :type 'FILE-INPUT
                   :extra name
                   :readc #'(lambda () (rt::C-fgetc C-file))
                   :unreadc #'(lambda (c) (rt::C-ungetc c C-file))
;;;                :listen #'(lambda () (rt::C-listen C-file))
                   :tell #'(lambda () (rt::C-ftell C-file))
                   :seek #'(lambda (pos) (rt::C-fseek C-file pos))
                   :length #'(lambda () (rt::C-file-length C-file))
                   :close #'(lambda (stream)
                              (rt::C-fclose C-file) (close1 stream))))

;;------------------------------------------------------------------------------
(defun make-file-output-stream (C-file name &aux (column nil))
  (rt::make-stream
   :type 'FILE-OUTPUT
   :extra name
   :writec #'(lambda (c)
               (if (eql c #\Newline)
                   (setq column 0)
                   (when column (incf column)))
               (rt::C-fputc c C-file))
   :column #'(lambda () column)
   :tell #'(lambda () (rt::C-ftell C-file))
   :seek #'(lambda (pos) (setq column nil) (rt::C-fseek C-file pos))
   :length #'(lambda () (rt::C-file-length C-file))
   :close #'(lambda (stream)
              (rt::C-fclose C-file) (close1 stream))))

;;----------------------------------------------------------------------------
(defun make-file-io-stream (C-file name)
  (rt::make-stream
   :type 'FILE-IO
   :extra name
   :readc #'(lambda () (rt::C-fgetc C-file))
   :unreadc #'(lambda (c) (rt::C-ungetc c C-file))
;;;            :listen #'(lambda () (rt::C-listen C-file))
   :tell #'(lambda () (rt::C-ftell C-file))
   :seek #'(lambda (pos) (rt::C-fseek C-file pos))
   :length #'(lambda () (rt::C-file-length C-file))
   :writec #'(lambda (c) (rt::C-fputc c C-file))
   :close #'(lambda (stream)
              (rt::C-fclose C-file) (close1 stream))))

;;------------------------------------------------------------------------------
(defun make-terminal-io ()
  "zum Erzeugen von *terminal-io* mittels stdin, stdout"
  (let ((stdin (rt::C-stdin))
        (stdout (rt::C-stdout))
        (column 0))
    (rt::make-stream
     :type 'FILE-IO
     :extra "stdin / stdout"
     :readc #'(lambda () (rt::C-fgetc stdin))
     :unreadc #'(lambda (c) (rt::C-ungetc c stdin))
;;;              :listen #'(lambda () (rt::C-listen stdin))
     :writec #'(lambda (c)
                 (if (eql c #\newline)
                     (setq column 0)
                     (incf column))
                 (rt::C-fputc c stdout))
     :column #'(lambda () column)
     :close #'(lambda (stream)
                (declare (ignore stream))
                "can't close *terminal-io*"))))

;;------------------------------------------------------------------------------
(defun close (stream &key abort)
  (funcall (stream-close stream) stream))
