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
;;; Funktion : Hilfsfunktionen, Makros und Fehlerbehandlung zur Typinferenz
;;;
;;; $Revision: 1.46 $
;;; $Id: timisc.lisp,v 1.46 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------

(require "titypes")
(require "tidef")


;;------------------------------------------------------------------------------
;; Liefert eine leere Typumgebung.
;;------------------------------------------------------------------------------
(defun empty-environment () `())


;;------------------------------------------------------------------------------
;; Liefert die Bindung zu einer location.
;;------------------------------------------------------------------------------
(defun get-type-binding (location &optional (env *type-environment*))
  (assoc location env :test #'eq))


;;------------------------------------------------------------------------------
;; Liefert die Bindung zu einer location.
;;------------------------------------------------------------------------------
(defun get-type (location env)
  (let ((type-binding (get-type-binding location env)))
    (if type-binding 
        (rest type-binding)
        bottom-t)))


;;------------------------------------------------------------------------------
;; Setzt eine Typbindung.
;;------------------------------------------------------------------------------
(defmacro bind-type (location type env)
  `(let ((type-binding (get-type-binding ,location ,env)))
    (if type-binding
        (setf (rest type-binding) ,type)
        (push (cons ,location ,type) ,env))))


;;------------------------------------------------------------------------------
(defun bind-and-set-type (location type)
  (bind-type location type *type-environment*)
  (setf (?type location) type))


;;------------------------------------------------------------------------------
(defun bind-and-update-type (location type)
  (bind-type location type *type-environment*)
  (update-type-f (?type location) type))


;;------------------------------------------------------------------------------
;; assert-type sichert einer location in einer Umgebung einen Typ zu. Als 
;; Resultat wird die aktualisierte Bindung geliefert.
;;------------------------------------------------------------------------------
(defmacro assert-type (location new-type environment)
  `(let* ((old-type (get-type ,location ,environment))
          (type-meet (type-meet old-type ,new-type)))
    (unless (type-eq old-type type-meet)
      (bind-type ,location type-meet ,environment))))


;;------------------------------------------------------------------------------
;; simple-set-parameter-types bindet an alle Parameter einer Funktion den Typ 
;; `argument-type', der mit top-t vorbesetzt ist.
;;------------------------------------------------------------------------------
(defun simple-set-parameter-types (params &optional (argument-type top-t))
  (let* ((old-param-types (mapcar #'?type (?all-vars params))))

    ;; Zuerst werden die benoetigten Parameter gebunden.
    ;;--------------------------------------------------
    (dolist (a-var (?var-list params))
      (update-type-f (?type a-var) argument-type))

      
    ;; Dann werden die optionalen und Schluesselwortparameter gebunden.
    ;;-----------------------------------------------------------------
    (dolist (a-param (append (?opt-list params) (?key-list params)))
      (update-type-f (?type (?var a-param)) argument-type))      


    ;; Der Restparameter ist - wenn vorhanden - eine Liste.
    ;;-----------------------------------------------------
    (when (?rest params)
      (update-type-f (?type (?rest params)) (list-of argument-type)))

    
    ;; Liefere als Ergebnis, ob die Parametertypen gleich geblieben sind.
    ;;-------------------------------------------------------------------
    (every #'type-eq old-param-types (mapcar #'?type (?all-vars params)))))


;;------------------------------------------------------------------------------
;; set-param-types bindet Argumenttypen von getypten Argumenten an Parameter.
;; An supplied-Komponenten der Schluesselwort- und optionalen Parameter wird
;; entweder null-t oder not-null-t gebunden.
;;------------------------------------------------------------------------------
(defun set-parameter-types (params argument-types)
  (let ((old-param-types (mapcar #'?type (?all-vars params))))
  
    ;; Zuerst werden die benoetigten Parameter gebunden.
    ;;--------------------------------------------------
    (dolist (a-var (?var-list params))
      (update-type-f (?type a-var) (pop argument-types)))
      

    ;; Dann werden die optionalen Parameter gebunden.
    ;;-----------------------------------------------
    (dolist (an-opt (?opt-list params))
      (let ((type (pop argument-types)))
        (when type
          (update-type-f (?type (?var an-opt)) type))))
      

    ;; Nun werden die Schluesselwortparameter gebunden.
    ;;-------------------------------------------------
    (let ((union-type (apply #'multiple-type-join argument-types)))
      (dolist (a-key (?key-list params))
        (update-type-f (?type (?var a-key)) union-type)))      
      

    ;; Zuletzt wird - wenn vorhanden - der Restparameter gebunden.
    ;;----------------------------------------------------------------
    (when (?rest params)
      (update-type-f (?type (?rest params))
                     (apply #'list-cons-of argument-types)))


    ;; Liefere als Ergebnis, ob die Parametertypen gleich geblieben sind.
    ;;-------------------------------------------------------------------
    (every #'type-eq old-param-types (mapcar #'?type (?all-vars params)))))
  
  

;;------------------------------------------------------------------------------
;; bind-parameter-types bindet Parameter und beruecksichtigt initiale Werte und
;; supplied-Variablen.
;;------------------------------------------------------------------------------
(defun bind-parameter-types (params)
  (dolist (var (?var-list params))
    (bind-type var (?type var) *type-environment*))

  (dolist (param (append (?opt-list params) (?key-list params)))
    (let* ((var       (?var   param))
           (suppl     (?suppl param))
           (init      (?init  param))
           (init-type (if (is-side-effect-free init)
                          (analyse-types init)
                          (let* ((old-env (copy-type-env *type-environment*))
                                 (result  (analyse-types init)))
                            (setf *type-environment*
                                  (join-type-environments old-env
                                                          *type-environment*))
                            result))))

      (bind-and-update-type var (type-join (?type var) init-type))
      (when suppl
        (bind-and-update-type suppl bool-t))))

  (let ((rest (?rest params)))
    (when rest
      (bind-type rest (?type rest) *type-environment*))))


;;------------------------------------------------------------------------------
;; Setze alle Typbindungen auf das angegebene Element des Typverbandes.
;;------------------------------------------------------------------------------
(defun set-all-type-bindings-to (type)
  (dolist (pair *type-environment*)
    (setf (rest pair) type)))


;;------------------------------------------------------------------------------
;; Setzt Typbindungen nach einem Seiteneffekt geeignet zurueck.
;; Wenn `write-effect' eine Liste ist, dann werden die enthaltenen Variablen auf
;; Top gesetzt. Andernfalls wird `write-effect' als statische Tiefe derjenigen
;; Variablen angesehen, die zurueckzusetzen sind.
;; Zudem werden bei einem aufgetretenen destruktiven Seiteneffekt Listentypen 
;; zurueckgesetzt.
;;------------------------------------------------------------------------------
(defun reset-type-bindings (write-effect not-destructive)
  (cond ((listp write-effect)
         (unless not-destructive
           (reset-type-bindings-after-destructive-effect))
         (dolist (location write-effect)
           (bind-type location top-t *type-environment*)))
        
        (T
         (dolist (pair *type-environment*)
           (let ((location (first pair))
                 (type     (rest  pair)))
             (cond ((or (dynamic-p location)
                        (<= (?level location) write-effect))
                    (setf (rest pair) top-t))
                   
                   ((and (not not-destructive)
                         (zs-subtypep list-cons-of-bottom-t type))
                    (setf (rest pair) (type-join type list-component-top-t))))
             )))))

                  
;;------------------------------------------------------------------------------
;; Setzt Typbindungen nach einem destruktiven Seiteneffekt geeignet zurueck.
;; In dem jetzigen Typverband werden Listentypen auf list-of(Top) gesetzt, alle
;; anderen Typen bleiben erhalten.
;;------------------------------------------------------------------------------
(defun reset-type-bindings-after-destructive-effect ()
  (dolist (pair *type-environment*)
    (let ((type (rest pair)))
      (when (zs-subtypep list-cons-of-bottom-t type)
        (setf (rest pair) (type-join type list-component-top-t))))))
                             

;;------------------------------------------------------------------------------
;; Liefert eine Kopie der uebergebenen Typumgebung.
;;------------------------------------------------------------------------------
(defun copy-type-env (type-environment)
  (copy-alist type-environment))


;;------------------------------------------------------------------------------
;; Liefert die Vereinigungsumgebung zweier Typumgebungen. Als zweiter Wert wird
;; geliefert, ob die Vereinigungsumgebung echt schaerfere Typbindungen enthaelt
;; als Typumgebung1.
;;------------------------------------------------------------------------------
(defun join-type-environments (environment1 environment2)
  (let ((result ())
        (result-is-superenv-of-env1 nil))
    (dolist (assoc1 environment1 (values (append result environment2) 
                                         (or result-is-superenv-of-env1
                                             (not (endp environment2)))))
      (let* ((key1   (first assoc1))
             (data1  (rest  assoc1))
             (assoc2 (get-type-binding key1 environment2)))
        (cond (assoc2
               (let ((new-data (type-join data1 (rest assoc2))))
                 (push (cons key1 new-data) result)
                 (unless (type-eq new-data data1)
                   (setf result-is-superenv-of-env1 T))
                 (setf environment2 (remove key1 environment2 :key #'first))))
              (T 
               (push assoc1 result)))))))


;;------------------------------------------------------------------------------
;; Liefert zur Vorgaengertypumgebung einer Funktion und einer beliebigen 
;; Typumgebung die durch Vereinigung entstandene Vorgaengertypumgebung.
;;------------------------------------------------------------------------------
(defun join-pred-type-env-with-type-env (pred-type-env environment)
  (let ((result-is-superenv-of-pred-type-env nil))
    (values
     (dolist (an-entry pred-type-env pred-type-env)
       (let* ((location (first an-entry))
              (old-type (rest  an-entry))
              (binding  (get-type-binding location environment))
              (new-type (if binding 
                            (type-join (rest binding) old-type)
                            top-t)))
         (unless (type-eq old-type new-type)
           (setf result-is-superenv-of-pred-type-env t)
           (setf (rest an-entry) new-type))))
     result-is-superenv-of-pred-type-env)))


;;------------------------------------------------------------------------------
;; Aktualisiert alle Typbindungen aus `env1' mit den in `env2' vorliegenden 
;; Typbindungen.
;;------------------------------------------------------------------------------
(defun update-type-env (env1 env2)
  (let ((environment-changed nil))
    (values
     (mapcar #'(lambda (an-entry)
                 (let* ((location (first an-entry))
                        (old-type (rest  an-entry))
                        (binding  (get-type-binding location env2)))
                   (if binding
                       (let ((new-type (rest binding)))
                         (unless (type-eq old-type new-type)
                           (setq environment-changed t))
                         (cons location new-type))
                       (cons location old-type))))
             env1)
     environment-changed)))


;;------------------------------------------------------------------------------
;; Aktualisiert alle Typbindungen aus `old-env', deren locations nicht in 
;; `bound-vars-env' liegen, mit den in `new-env' vorliegenden Typbindungen.
;;------------------------------------------------------------------------------
(defun reset-let-type-env (old-env new-env bound-vars-env)
  (mapcar #'(lambda (an-entry)
              (let* ((var   (first an-entry))
                     (bound (get-type-binding var bound-vars-env)))
                (if bound
                    (if (dynamic-p var)
                        (cons var (rest bound))
                        an-entry)
                    (cons var (get-type var new-env)))))
          old-env))


;;------------------------------------------------------------------------------
;; Gibt die Anzahl der in der Menge der zu bearbeitenden Funktionen *work-set*
;; verbliebenen Funktionen aus.
;;------------------------------------------------------------------------------
(defun write-size-of-workset (workset iterations)
  (when (and (> *ti-verbosity* 2) 
             (= 0 (mod iterations *ti-write-size-of-workset-interval*)))

    (clicc-message "~4D elements remain in the analyzation workset." 
                   (length workset))))


;;------------------------------------------------------------------------------
(provide "timisc")



