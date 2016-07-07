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
;;; Funktion : Definition der Lisp Funktionen, deren Aufrufe vom Compiler
;;;            direkt in entsprechenden C-Code ueberfuehrt werden. Nur wenn
;;;            diese Funktionen als funktionale Objekte verwendet werden, dann
;;;            kommen die Definitionen in dieser Datei zur Anwendung.
;;;
;;; $Revision: 1.10 $
;;; $Id: inline.lisp,v 1.10 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(defun atom (x) (atom x))
(defun characterp (x) (characterp x))
(defun cons (x y) (cons x y))
(defun consp (x) (consp x))
(defun eq (x1 x2) (eq x1 x2))
(defun eql (x1 x2) (eql x1 x2))
(defun floatp (x) (floatp x))
(defun functionp (x) (functionp x))
(defun integerp (x) (integerp x))
(defun rt::fixnump (x) (rt::fixnump x))
(defun listp (x) (listp x))
(defun numberp (x) (numberp x))
(defun simple-string-p (x) (simple-string-p x))
(defun simple-vector-p (x) (simple-vector-p x))
(defun simple-bit-vector-p (x) (simple-bit-vector-p x))
(defun symbolp (x) (symbolp x))

