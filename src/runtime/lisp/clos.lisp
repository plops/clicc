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
;;; Function : Objectsystem
;;;
;;; $Revision: 1.37 $
;;; $Id: clos.lisp,v 1.37 1995/03/04 22:07:38 wg Exp $
;;;----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(make-instance slot-value slot-boundp slot-exists-p slot-makunbound no-next-method
   no-applicable-method slot-missing slot-unbound))
(export '(rt::typep-class) "RT")

;;------------------------------------------------------------------------------
(defmacro direct-class-of (instance-or-class)
  `(rt::instance-ref ,instance-or-class -1))

(defmacro set-direct-class-of (instance new-value)
    `(rt::instance-set ,new-value ,instance -1))

(defmacro name-of (class)
    `(rt::instance-ref ,class 0))

(defmacro class-precedence-list (class)
    `(rt::instance-ref ,class 1))

(defmacro num-o-slots (class)
    `(rt::instance-ref ,class 2))

(defmacro slot-infos (class)
    `(rt::instance-ref ,class 3))

(defmacro slot-key (slot-info)
    `(first ,slot-info))

(defmacro slot-init (slot-info)
    `(second ,slot-info))

(defmacro slot-name (slot-info)
    `(third ,slot-info))

;;------------------------------------------------------------------------------
;; AUFBAU EINES KLASSENOBJEKTS:
;; 
;; --------------------------
;;|Typ : CL_INSTANCE         |
;;|--------------------------|
;;|-1 : NIL                  | eine Klasse ist also nicht Instanz einer Klasse
;;|--------------------------|
;;| 0 : name-of              |
;;|--------------------------|
;;| 1 : class-precedence-list|
;;|--------------------------|
;;| 2 : num-o-slots          | Anzahl der Slots in Instanzen dieser Klasse
;;|--------------------------|
;;| 3 : slot-infos           | Liste von Slot-Beschreibungen
;; --------------------------
;;
;; 
;; SLOT-BESCHREIBUNGEN:
;; 
;; (key init-val-or-fun name)
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; MAKE-INSTANCE class &REST keys
;;------------------------------------------------------------------------------
(defun make-instance (class &rest keys)
  (unless (and (rt::instancep class) (not (direct-class-of class)))
    (error-in 'make-instance 
              (if (symbolp class)
                  "The computed symbol ~S is not a valid class-argument, ~
                   these have been restricted to classes."
                  "~S is not a valid class-argument.")
              class))
  (let ((instance (rt::make-instance
                   (+ (num-o-slots class) 1)))
        (slot-pos 0)) 
    (set-direct-class-of instance class)
    (dolist (slot-info (slot-infos class))
      (let ((init-val-or-fun
             (getf keys (slot-key slot-info) (slot-init slot-info))))
        (if (and (functionp init-val-or-fun)
                 (eq init-val-or-fun (slot-init slot-info)))
            (rt::instance-set (funcall init-val-or-fun) instance slot-pos)
            (rt::instance-set init-val-or-fun instance slot-pos)))
      (incf slot-pos))
    instance))

;;------------------------------------------------------------------------------
;; REINITIALIZE-INSTANCE instance slot-names &REST initargs
;;------------------------------------------------------------------------------
(defun reinitialize-instance (instance slot-names &rest initargs)
  (unless (or (not (rt::instancep instance)) (direct-class-of instance))
    (error-in 'reinitialize-instance "~S is not an instance." instance))
  (let ((slot-pos 0))
    (dolist (slot-info (slot-infos instance))
      (let ((init-val-or-fun
             (getf initargs (slot-key slot-info) (slot-init slot-info))))
        (if (and (eq init-val-or-fun (slot-init slot-info)))
            (when (or (eq T slot-names)
                      (member (slot-name slot-info) initargs))
              (if (and (functionp init-val-or-fun)
                       (eq init-val-or-fun (slot-init slot-info)))
                  (rt::instance-set (funcall init-val-or-fun) instance slot-pos)
                  (rt::instance-set init-val-or-fun instance slot-pos)))
            (if (and (functionp init-val-or-fun)
                     (eq init-val-or-fun (slot-init slot-info)))
                (rt::instance-set (funcall init-val-or-fun) instance slot-pos)
                (rt::instance-set init-val-or-fun instance slot-pos))))
      (incf slot-pos))))

;;------------------------------------------------------------------------------
;; TYPEP-CLASS object class
;;------------------------------------------------------------------------------
(defun rt:typep-class (object class)
  (if (rt::instancep object)
      (if (eq class (direct-class-of object))
          object
          (simple-member class
                         (class-precedence-list (direct-class-of object))))
      nil))

(defun simple-member (item list)
  (cond
    ((null list)
     nil)
    ((eq item (car list))
     item)
    (T
     (simple-member item (cdr list)))))

;;------------------------------------------------------------------------------
;; SLOT-VALUE object slot-name
;;------------------------------------------------------------------------------
(defun slot-value (object slot-name)
  (let* 
      ((slot-infos (slot-infos (class-of object)))
       (slot-pos   (position slot-name slot-infos :key #'third)))
    (if slot-pos
        (if (rt::unbound-value-p (rt::instance-ref object slot-pos))
            (slot-unbound (class-of object) object slot-name)
            (rt::instance-ref object slot-pos))
        (slot-missing (class-of object) object slot-name 'SLOT-VALUE))))

;;------------------------------------------------------------------------------
;; (SETF SLOT-VALUE) object slot-name
;;------------------------------------------------------------------------------
(defun (setf slot-value) (new-value object slot-name)
  (let* 
      ((slot-infos (slot-infos (class-of object)))
       (slot-pos   (position slot-name slot-infos :key #'third)))
    (if slot-pos
        (rt::instance-set new-value object slot-pos)
        (slot-missing (class-of object) object slot-name '(SETF SLOT-VALUE)))))

;;------------------------------------------------------------------------------
;; SLOT-BOUNDP instance slot-name
;;------------------------------------------------------------------------------
(defun slot-boundp (instance slot-name)
  (let* 
      ((slot-infos (slot-infos (class-of instance)))
       (slot-pos   (position slot-name slot-infos :key #'third)))
    (if slot-pos
        (not (rt::unbound-value-p (rt::instance-ref instance slot-pos)))
        (slot-missing (class-of instance) instance slot-name 'SLOT-BOUNDP))))

;;------------------------------------------------------------------------------
;; SLOT-EXISTS-P instance slot-name
;;------------------------------------------------------------------------------
(defun slot-exists-p (instance slot-name)
  (let* 
      ((slot-infos (slot-infos (class-of instance)))
       (slot-pos   (position slot-name slot-infos :key #'third)))
    (if slot-pos T NIL)))

;;------------------------------------------------------------------------------
;; SLOT-MAKUNBOUND instance slot-name
;;------------------------------------------------------------------------------
(defun slot-makunbound (instance slot-name)
  (let* 
      ((slot-infos (slot-infos (class-of instance)))
       (slot-pos   (position slot-name slot-infos :key #'third)))
    (if slot-pos
        (rt::instance-set (rt::unbound-value) instance slot-pos)
      (slot-missing (class-of instance) instance 
                    slot-name 'SLOT-MAKUNBOUND)))
  instance)

;;------------------------------------------------------------------------------
;; CLASS-OF instance
;;------------------------------------------------------------------------------
(defun class-of (instance)
  (if (rt::instancep instance)
      (direct-class-of instance)
      (error "~S ist not a valid argument for CLASS-OF, ~
              these have been restricted to instances of user-defined-classes."
             instance)))

;;------------------------------------------------------------------------------
;; Funktionen zur Fehlerbehandlung der von CLOS erzeugten Laufzeitfehler:
;; NO-NEXT-METHOD
;; NO-APPLICABLE-METHOD gf-name &REST gf-args
;; SLOT-MISSING class object slot-name operation
;; SLOT-UNBOUND class instance slot-name
;;------------------------------------------------------------------------------
(defun no-next-method ()
  (error "No next method."))

(defun no-applicable-method (gf-name &rest gf-args)
  (error "No applicable method for generic function ~s with args ~s." 
         gf-name gf-args))

(defun slot-missing (class object slot-name operation)
  (error "~S: The slot ~s is missing from the object ~s of class ~s."
         operation slot-name object class))

(defun slot-unbound (class instance slot-name)
  (error "The slot ~s is unbound in the object ~s of class ~s."
         slot-name instance class))

;;------------------------------------------------------------------------------
