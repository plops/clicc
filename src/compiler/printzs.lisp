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
;;; Funktion : Methoden fuer Print-Object, spezialisiert fuer die Lisp nahe
;;;            Zwischensprache
;;;
;;; $Revision: 1.27 $
;;; $Id: printzs.lisp,v 1.27 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC") 

;;------------------------------------------------------------------------------
;; Symbol fuer ungebundene Slots:
;;------------------------------------------------------------------------------
(defconstant unbound-slot-value '---)


;;------------------------------------------------------------------------------
;; *po-slot-list* enth"alt eine Tabelle f"ur die spezialisierte Print-Object
;; Methode, in der zu jedem Zwischensprachkonstrukt eine Liste von Komponenten
;; abgelegt ist, die gedruckt werden sollen.
;;------------------------------------------------------------------------------
(defvar *po-slot-list* '())


;;------------------------------------------------------------------------------
;; Hilfsfunktion fuer den Umgang mit Klassen.
;;------------------------------------------------------------------------------
(defun direct-superclasses (class)
  #+PCL           (pcl::class-direct-superclasses class)
  #+CLOS          (clos::class-direct-superclasses class)
  #-(or PCL CLOS) (class-direct-superclasses class))

;;------------------------------------------------------------------------------
;; Hilfsfunktion fuer die print-object Methode
;;------------------------------------------------------------------------------
(defun slots-to-print (class-name)
  (if (eq class-name 'zws-object)
      nil
      (append (slots-to-print   
               (class-name (first (direct-superclasses 
                                   (find-class class-name)))))
              (rest (assoc class-name *po-slot-list*)))))


;;------------------------------------------------------------------------------
;; print-object-Methoden fuer die Zwischensprachkonstrukte.
;;------------------------------------------------------------------------------
(defmethod print-object ((an-empty-list null-form) a-stream)
  (princ "[emtpy-list]" a-stream))

;;------------------------------------------------------------------------------
(defmethod print-object ((an-object zws-object) a-stream)
  (let* ((name-of-class 
          (class-name (class-of an-object)))
         (prefix "[")
         (suffix "]")
         (slot-values
          (mapcar #'(lambda (slot) 
                      (if (slot-boundp an-object slot)
                          (slot-value an-object slot)
                          unbound-slot-value))
                  (slots-to-print name-of-class))))
    #+allegro
    (pprint-logical-block (a-stream slot-values :prefix prefix :suffix suffix)
                          (write name-of-class :stream a-stream)
                          (pprint-indent :block 1 a-stream)
                          (loop
                           (pprint-exit-if-list-exhausted)
                           (write-char #\space a-stream)
                           (pprint-newline :fill a-stream)
                           (pprint-fill a-stream (pprint-pop))))


    #-allegro
    (progn
      (format a-stream "~S~S" prefix name-of-class)
      (dolist (slot-value slot-values) 
        (format a-stream " ~s" slot-value))
      (write suffix :stream a-stream))
    
    an-object))


;;------------------------------------------------------------------------------
;; PO-STANDARD Default-Belegung fuer die Variable *po-slot-list*
;;------------------------------------------------------------------------------
(defconstant PO-STANDARD '((module 
                            fun-list
                            class-def-list
                            named-const-list
                            var-list
                            sym-list
                            toplevel-forms)
                           (var)
                           (static symbol)
                           (defined-static)
                           (local-static)
                           (global-static exported)
                           (imported-static exported)
                           (dynamic . (sym))
                           (form)
                           (var-ref . (var))
                           (named-const . (symbol))
                           (defined-named-const . (exported value))
                           (imported-named-const . (value-zs-type))
                           (literal)
                           (sym . (symbol exported))
                           (defined-sym . (name package))
                           (imported-sym)
                           (simple-literal)
                           (character-form .(value))
                           (num . (value))
                           (structured-literal . (value))
                           (literal-instance . (class value-list))
                           (class-def . (symbol 
                                         super-list 
                                         slot-descr-list))
                           (defined-class . (exported))
                           (imported-class)
                           (built-in-class-def . (symbol 
                                                  super-list 
                                                  slot-descr-list
                                                  type-expander))
                           (slot-desc . (symbol initform initargs allocation))
                           (params . (var-list 
                                      opt-list 
                                      rest 
                                      key-list 
                                      allow-other-keys))
                           (opt . (var init suppl))
                           (key . (sym))
                           (fun . (symbol params))
                           (simple-fun . (body))
                           (defined-fun . ())
                           (global-fun . (exported))
                           (local-fun . ())
                           (imported-fun . (exported))
                           (special-sys-fun . ())
                           (generic-fun . (method-list
                                           argument-precedence-order
                                           method-combination))
                           (defined-generic-fun . (exported))
                           (imported-generic-fun . ())
                           (method-def . (fun specializer-list qualifier-list))
                           (app . (form arg-list))
                           (setq-form . (location form))
                           (progn-form . (form-list))
                           (if-form . (pred then else))
                           (switch-form . (form case-list otherwise))
                           (labeled-form . (value form))
                           (let*-form . (var-list init-list body))
                           (labels-form . (fun-list body))
                           (let/cc-form . (cont body))
                           (cont)
                           (tagbody-form . (first-form tagged-form-list))
                           (tagged-form . (form))
                           (mv-lambda . (params body arg))))


;;------------------------------------------------------------------------------
;; Weitere vorgegebene m"ogliche Belegungen f"ur *po-slot-list*
;;------------------------------------------------------------------------------
(setf *po-slot-list* PO-STANDARD)


;;------------------------------------------------------------------------------
;; Funktionen zum Ein- und Ausschalten der Anzeige des Typslots der ZS-Elemente.
;;------------------------------------------------------------------------------
(defmacro find-po-slot-list-element (a-zs-class-name)
  `(rest (assoc ',a-zs-class-name *po-slot-list*)))

(defun types-off ()
  (setf (find-po-slot-list-element form) 
        (remove 'type (find-po-slot-list-element form)))
  (setf (find-po-slot-list-element var) 
        (remove 'type (find-po-slot-list-element var))))

(defun types-on ()
  (push 'type (find-po-slot-list-element form))
  (push 'type (find-po-slot-list-element var)))


;;------------------------------------------------------------------------------
(provide "printzs")
