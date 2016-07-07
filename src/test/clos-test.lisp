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
;;; Funktion : Tests fuer das Objektsystem
;;;
;;; $Revision: 1.8 $
;;; $Id: clos-test.lisp,v 1.8 1994/11/22 14:45:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "USER")

;;------------------------------------------------------------------------------
;; Klassendefinitionen
;;------------------------------------------------------------------------------
(defclass c1 () ((s1 :accessor ?s1 :initarg :s1)))
(defclass c2 (c1) ())
(defclass c3 (c1) ())
(defclass c4 (c3 c2) ())
(defclass c5 (c2) ())

(defclass d1 () ((s1 :initform 'd1 :reader ?s1)))
(defclass d2 () ((s1 :initform 'd2 :reader ?s1)))
(defclass d3 (d2 d1) ())

;;------------------------------------------------------------------------------
;; Methodendefinitionen ...
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; ... fuer den Test der Slot-Verwaltung
;;------------------------------------------------------------------------------
(defmethod m ((a1 c1)) (?s1 a1))

;;------------------------------------------------------------------------------
;; ... fuer den Test von CALL-NEXT-METHOD
;;------------------------------------------------------------------------------
(defmethod n ((a c1) &key v1) (list 'c1 v1))
(defmethod n ((a c2) &key v1) (cons 'c2 (call-next-method)))
(defmethod n ((a c3) &key v1) (cons 'c3 (call-next-method)))
(defmethod n ((a c4) &key v1) (cons 'c4 (call-next-method)))

(defmethod n1 ((a d1)) (list 'd1))
(defmethod n1 ((a d2)) (cons 'd2 (call-next-method)))
(defmethod n1 ((a d3)) (cons 'd3 (call-next-method)))

;;------------------------------------------------------------------------------
;; ... fuer die Behandlung der Parameter in spezialisierten Lambda-Listen
;;------------------------------------------------------------------------------
(defmethod o ((p1 c1) &optional p2) 
  (if p2
      (cons p2 (list 'b))
      (list 'b 'a)))

(defmethod o ((p1 c2) &optional p2) 
  (if p2
      (cons p2 (list 'd))
      (list 'd 'c)))

;;------------------------------------------------------------------------------
(defmethod p ((p1 c1) &key key1 key2) (list (n p1 :v1 key2) key1))

;;------------------------------------------------------------------------------
;; Tests
;;------------------------------------------------------------------------------

(clicc-test "clos"
            ";;; Some Tests for the Object-System."
            (((m (make-instance 'c1 :s1 'INITIAL))      INITIAL)
             ((?s1 (make-instance 'd3))                 d2)
             ((n (make-instance 'c4 :s1 3) :v1 'bar)    (C4 C3 C2 C1 bar))
             ((n1 (make-instance 'd3))                  (D3 D2 D1)
              "A known bug which appears when using call-next-method
               in presence of multiple inheritance")
             ((o (make-instance 'c3))                   (b a))
             ((o (make-instance 'c3) 'a)                (a b))
             ((o (make-instance 'c5))                   (d c))
             ((o (make-instance 'c5) 'c)                (c d))
             ((p (make-instance 'c1) :key2 'b :key1 'a) ((c1 b) a))
             ))

;;------------------------------------------------------------------------------

(provide "clos-test")

