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
;;; Funktion : Die Voranalyse zur Seiteneffektanalyse.
;;; 
;;; $Revision: 1.58 $
;;; $Id: closure-analysis.lisp,v 1.58 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Die Voranalyse zur Seiteneffektanalyse. Es wird untersucht, welche
;; Parameter einer Funktion die Eigenschaft E haben. Ein Parameter p der
;; Funktion f hat die Eigenschaft E genau dann wenn:
;; p mindestens einmal als Funktion angewendet wird oder einer Funktion als
;; Argument übergeben wird, wo der zugehörige Parameter die Eigenschaft E hat
;; und wenn p nur Werte zugewiesen werden, die Literale oder seiteneffektfreie
;; Funktionen oder Werte anderer Parameter mit der Eigenschaft E sind.
;;
;; Die Eigenschaft E eines Parameters p der Funktion f wird in der
;; nachfolgenden Seiteneffektanalyse verwendet: Aufrufe von p als Funktion
;; werden zunächst als seiteffektfrei klassifiziert und als Ausgleich werden
;; an den Aufrufstellen von f die Seiteneffekte der an p gebundenen Funktionen
;; verwendet. Ohne diese Analyse würde für alle Aufrufe von p der größte
;; Seiteneffekt angenommen werden.
;;
;; W"ahrend der Analyse wird f"ur im Slot has-funs-as-args eine A-Liste
;; aufgebaut, die für die Variablen der Parameterliste der aktuellen angibt,
;; welchen Status sie hat oder mit welcher anderen Variablen sie in
;; Alias-Relation steht.
;; Nach Abschlu"s der Analyse wird die A-Liste mit Hilfe der Funktion
;; encode-fun-arg-alist kodiert.
;; 
;; Status:
;; nil : noch nichts bekannt
;; :FUN-AS-ARG-CALL : Parameter wird appliziert oder ist Argument einer Funktion
;; wo der zugeh"orige Parameter auch den Status :FUN-AS-ARG-CALL hat
;; :UNKNOWN-CALL : dem Parameter wird ein Wert zugewiesen, der nicht Wert eines
;; Parameters mit Status :FUN-AS-ARG-CALL ist und auch keine Seiteneffektfreie
;; Funktion ist und auch kein Literal ist.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Eine Liste der aktuellen und der lexikalisch umfassenden Funktionen
;; 1. Eintrag: aktuelle Funktion
;; letzter Eintrag: globale Funktion
;;------------------------------------------------------------------------------
(defvar *current-fun-list* ())

;;------------------------------------------------------------------------------
(defun get-defining-fun (var)
  (let ((index-of-fun (- *level* (?level var))))
    (nth index-of-fun *current-fun-list*)))

;;------------------------------------------------------------------------------
;; Eine Liste von Listen mit Eintr"agen der Form:
;; (called-fun pos . var-list)
;; F"ur die aktuelle und die lexikalisch umfassenden Funktionen jeweils eine
;; Liste.
;; called-fun wird in der aktuellen bzw. einer umfassenden aufgerufen und
;; die in var-list angegebenen Parameter stehen an der Argument-Position pos.
;;------------------------------------------------------------------------------
(defvar *call-info-list* ())

;;------------------------------------------------------------------------------
;; Die Variable enthält
;; - T, wenn das Resultat des Ausdrucks als Funktion aufgerufen wird
;; - eine defined-fun, wenn der Ausdruck als Argument an diese Funktion
;;   "ubergeben wird,
;; - ein Paar (var . status), wenn der Ausdruck einem Parameter zugewiesen wird,
;; - nil sonst.
;;------------------------------------------------------------------------------
(defvar *app-or-alias*)

;;------------------------------------------------------------------------------
;; Enth"alt eine Position in der Argument-Liste oder ein Keyword, wenn
;; *app-or-alias* mit einer defined-fun belegt ist.
;;------------------------------------------------------------------------------
(defvar *called-pos*)

;;------------------------------------------------------------------------------
;; Funktionen zur Verwaltung der Alias Beziehung von Variablen
;;------------------------------------------------------------------------------
(defun get-alias (var.value)
  (if (consp (cdr var.value))
      (get-alias (cdr var.value))
      var.value))

;;------------------------------------------------------------------------------
(defun get-var-alias (var)
  (get-alias (assoc var (?has-funs-as-args (get-defining-fun var)))))

;;------------------------------------------------------------------------------
(defun get-max-alias (pair)
  (let ((pair-list (?has-funs-as-args (get-defining-fun (car pair)))))
    (loop
     (let ((new-pair (rassoc pair pair-list)))
       (if new-pair
           (setq pair new-pair)
           (return pair))))))

;;------------------------------------------------------------------------------
(defun alias-to-unknown ()
  (when (consp *app-or-alias*)
    (setf (cdr *app-or-alias*) :UNKNOWN-CALL)))

;;------------------------------------------------------------------------------
;; Diese Methode traversiert jede Funktion, und sammelt alle Variablen die
;; nur auf Funktionsposition stehen. Diese Variablen werden in dem Slot
;; HAS-FUNS-AS-ARGS abgespeichert.
;;------------------------------------------------------------------------------
(defun pre-analyse-module ()
  (let ((*level* 0))
    (dolist (fun (?fun-list *module*))
      (setf (?mv-called-by fun) ()))
    (pre-analyse-defun (?toplevel-forms *module*))
    (mapc #'pre-analyse-defun (?fun-list *module*))
    (propagate-fun-vars)))

;;------------------------------------------------------------------------------
;; Analyse globaler Funktionen
;;------------------------------------------------------------------------------
(defun pre-analyse-defun (fun)
  (get-fun-vars-of-fun fun))
    
;;------------------------------------------------------------------------------
;; Analyse von Funktionen. In den Slot has-funs-as-args werden die Parameter
;; eingetragen, die als Funktion aufgerufen werden.
;;------------------------------------------------------------------------------
(defun get-fun-vars-of-fun (fun)
  (setf (?has-funs-as-args fun) ())
  (let ((*current-fun-list* (cons fun *current-fun-list*))
        (*call-info-list* (cons () *call-info-list*))
        (params (?params fun)))

    ;; required Parameter haben potentiell die Eigenschaft fun-var
    ;;------------------------------------------------------------
    (dolist (required (?var-list params))
      (push (cons required nil) (?has-funs-as-args fun)))

    ;; optionale Parameter haben potentiell die Eigenschaft fun-var,
    ;; wenn der Defaultwert jedoch eine unbekannte oder
    ;; seiteneffektverursachende Funktion ist, dann geht die Eigenschaft
    ;; verloren
    ;;---------
    (dolist (opt (?opt-list params))
      (let ((*app-or-alias* (cons (?var opt) nil)))
        (push *app-or-alias* (?has-funs-as-args fun))
        (get-fun-vars (?init opt))))

    ;; Keyword Parameter werden wie optionale behandelt.
    ;;--------------------------------------------------
    (dolist (key (?key-list params))
      (let ((*app-or-alias* (cons (?var key) nil)))
        (push *app-or-alias* (?has-funs-as-args fun))
        (get-fun-vars (?init key))))

    ;; der Rumpf
    ;;----------
    (get-fun-vars-no-app (?body fun))
    
    ;; In den Slot mv-called-by wird eine Liste mit Eintr"agen folgender Form
    ;; eingetragen:
    ;; (pos caller . var-list)
    ;; Dieser Eintrag kodiert einen Aufruf der Funktion in caller, wobei an
    ;; der Position pos ein Parameter var von caller als Argument "ubergeben
    ;; wird. Wenn bei der Bestimmung des Fixpunkts herausstellt, da"s der
    ;; zugeh"orige Parameter der Funktion eine Funktion enthalten mu"s, dann
    ;; mu"s diese Eigenschaft auch an var von caller propagiert werden.
    ;;-----------------------------------------------------------------
    (dolist (info (car *call-info-list*))
      (let ((called-fun (pop info))
            (pos (pop info))
            (var info)
            (vars ()))

        ;; wenn die die Eigenschaft fun-var der Variablen noch nicht bestimmt
        ;; wurde, dann wird sie zusammen mit ihren Aliases eingetragen.
        ;;---------------------------------------------------------------
        (let ((pair (get-var-alias var)))
          (when (and pair (null (cdr pair)))
            (setq pair (get-max-alias pair))
            (loop
             (push (car pair) vars)
             (when (null (cdr pair)) (return)))
            (push (list* pos fun vars) (?mv-called-by called-fun)))))))
  (encode-fun-arg-alist fun))

;;------------------------------------------------------------------------------
(defun get-fun-vars-of-params (params)
  (dolist (opt (?opt-list params))
    (get-fun-vars-no-app (?init opt)))
  (dolist (key (?key-list params))
    (get-fun-vars-no-app (?init key))))

;;------------------------------------------------------------------------------
(defun get-fun-vars-no-app (form)
  (let ((*app-or-alias* nil))
    (get-fun-vars form)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form var-ref))
  (let ((var (?var form)))
    (when (local-static-p var)
      (typecase *app-or-alias*
        (null nil)

        ;; rechte Seite einer Zuweisung
        ;;-----------------------------
        (cons
         (labels ((max-fun-vars-info (v1 v2)
                    (cond
                      ((null v1) v2)
                      ((null v2) v1)
                      ((eq :UNKNOWN-CALL v1) v1)
                      ((eq :UNKNOWN-CALL v2) v2)
                      (T v1))))         ; v1 = v2 = :FUN-AS-ARG-CALL
                     

           ;; Aliasing der Variablen auf der rechten und linken Seite einer
           ;; Zuweisung. Nur zwischen Parametern der gleichen Funktion soll
           ;; Aliasing erlaubt sein.
           ;;-----------------------
           (let (pair)
             (setq *app-or-alias* (get-alias *app-or-alias*))
             (cond
               ((and (= (?level (car *app-or-alias*)) (?level var))
                     (setq pair (get-var-alias var)))
                (unless (or (eq *app-or-alias* pair)
                            (eq :UNKNOWN-CALL (cdr *app-or-alias*)))
                  (let ((value (cdr pair)))
                    (setf (cdr pair) *app-or-alias*)
                    (setf (cdr *app-or-alias*)
                          (max-fun-vars-info value (cdr *app-or-alias*))))))
               
               (t (setf (cdr *app-or-alias*) :UNKNOWN-CALL))))))

        ;; ist Argument einer Funktion
        ;;----------------------------
        (fun
         (let ((pair (get-var-alias var))
               (level (?level var)))
           (when (and pair (null (cdr pair)))
             (push (list* *app-or-alias* *called-pos* var)
                   (nth (- *level* level) *call-info-list*)))))
      
        ;; wird aufgerufen
        ;;----------------
        (t (let ((pair (get-var-alias var)))
             (when (and pair (null (cdr pair)))
               (setf (cdr pair) :FUN-AS-ARG-CALL))))))))

;;------------------------------------------------------------------------------
;; Bei einer Zuweisung an einen Parameter können folgende Situationen auftreten:
;; - Der neue Wert ist ein Literal, eine seiteneffektfreie Funktion oder ein
;;   Aufruf der Funktion ERROR. Dann bleibt der Status des Parameters
;;   unverändert.
;; - Der neue Wert ist der Wert eines anderen Parameters. Dann werden die beiden
;;   Parameter als Aliase betrachtet und jede Änderung im Status des einen
;;   Parameters verändert den Staus des anderen Parameters auf gleiche Weise.
;; - Sonst wird der Status zu :UNKNOWN-CALL, d.h. für den Parameter erfolgt
;;   keine Sonderbehandlung.
;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((a-setq setq-form))
  (let* ((var (?var (?location a-setq)))
         pair)

    (cond
      ((and (local-static-p var)
            (setq pair (get-var-alias var))
            (not (eq :UNKNOWN-CALL (cdr pair))))
       (let ((*app-or-alias* pair))
         (get-fun-vars (?form a-setq)))

       ;; die Variable mit dem "au"seren *app-or-alias* verkn"upfen
       ;;----------------------------------------------------------
       (get-fun-vars (?location a-setq)))
        
      (t (get-fun-vars (?form a-setq))))))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((app app))

  ;; wir kennen das Resultat nicht
  ;;------------------------------
  (unless (eq (?form app) *error-function*)
    (alias-to-unknown))
  
  (let ((form (?form app))
        (arg-list (?arg-list app)))
    (typecase form

      ;; wir wissen bereits, welche Parameter als Funktion aufgerufen werden
      ;;--------------------------------------------------------------------
      (imported-fun
       (let ((*app-or-alias* t)
             (key-pos (get-key-pos form))
             (i 0))
    
         ;; required & optional arguments
         ;;------------------------------
         (loop
          (when (or (null arg-list) (and key-pos (>= i key-pos))) (return))
          (if (pos-fun-arg-p i form)
              (get-fun-vars (pop arg-list))
              (get-fun-vars-no-app (pop arg-list)))
          (incf i))
      
         ;; keyword arguments
         ;;------------------
         (loop
          (when (null arg-list) (return))
          (cond
            ((sym-p (car arg-list))
             (if (key-fun-arg-p (pop arg-list) form)
                 (get-fun-vars (pop arg-list))
                 (get-fun-vars-no-app (pop arg-list))))

            ;; das Keyword wird berechnet
            ;;---------------------------
            (t (get-fun-vars-no-app (pop arg-list))
               (get-fun-vars-no-app (pop arg-list)))))))

      ;; wir merken uns, welche Parameter der aktuellen Funktion an welche
      ;; Argumente der aufgerufenen Funktion gebunden werden, um sp"ater den
      ;; Fixpunkt bestimmen zu k"onnen.
      ;;-------------------------------
      (defined-fun
          (let* ((*app-or-alias* form)
                 *called-pos*
                 (params (?params form))
                 (key-pos (+ (length (?var-list (?params form)))
                             (length (?opt-list (?params form)))))
                 (i 0)
                 k)
          
            ;; required & optional arguments
            ;;------------------------------
            (loop
             (when (or (null arg-list) (>= i key-pos)) (return))
             (setq *called-pos* i)
             (get-fun-vars (pop arg-list))
             (incf i))

            (cond
              ((?rest params)
               (dolist (arg arg-list)
                 (get-fun-vars-no-app arg)))
              
              (t
               ;; keyword arguments
               ;;------------------
               (loop
                (when (null arg-list) (return)) 
                (cond
                  ((sym-p (setq k (car arg-list))) 
                   (pop arg-list)
                   (when arg-list
                     (setq *called-pos* k)
                     (get-fun-vars (pop arg-list))))

                  ;; das Keyword wird berechnet
                  ;;---------------------------
                  (t (get-fun-vars-no-app (pop arg-list))
                     (when arg-list (get-fun-vars-no-app (pop arg-list))))))))))
      (t
       ;; berechnete Funktion
       ;;--------------------
       (let ((*app-or-alias* t))
         (get-fun-vars form)
         (dolist (arg arg-list)
           (get-fun-vars-no-app arg)))))))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((fun fun))
  (unless (eq fun *error-function*)
    (alias-to-unknown)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((fun imported-fun))
  (unless (eq fun *error-function*)
    (when (or (?read-list fun)
              (?write-list fun)
              (?data-effects fun))
      (alias-to-unknown))))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((fun foreign-fun))
  (alias-to-unknown))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form let*-form))
  (dolist (init-form (?init-list form))
    (get-fun-vars-no-app init-form))
  (get-fun-vars (?body form)))

;;-----------------------------------------------------------------------------
(defmethod get-fun-vars ((form labels-form))
  (dolist (fun (?fun-list form))
    (setf (?mv-called-by fun) ()))
  (let ((*level* (1+ *level*)))
    (dolist (fun (?fun-list form))
      (get-fun-vars-of-fun fun)))
  (get-fun-vars (?body form)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form if-form))
  (get-fun-vars-no-app (?pred form))
  (get-fun-vars (?then form))
  (get-fun-vars (?else form)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((switch switch-form))
  (get-fun-vars-no-app (?form switch))
  (dolist (one-case (?case-list switch))
    (get-fun-vars (?form one-case)))
  (get-fun-vars (?otherwise switch)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form progn-form))
  (mapc-progn-form-list (?form-list form)
                        #'get-fun-vars-no-app
                        #'get-fun-vars))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form tagbody-form))
  (get-fun-vars (?first-form form))
  (mapc-tagged-form-list (?tagged-form-list form)
                         #'get-fun-vars-no-app
                         #'get-fun-vars))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form tagged-form)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((let/cc let/cc-form))
  (alias-to-unknown)
  (get-fun-vars (?body let/cc)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((a-cont cont)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((mv-lambda mv-lambda))
  (get-fun-vars-of-params (?params mv-lambda))
  (get-fun-vars-no-app (?arg mv-lambda))
  (get-fun-vars (?body mv-lambda)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form defined-named-const))
  (get-fun-vars (?value form)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form imported-named-const))
  (alias-to-unknown))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form literal)))

;;------------------------------------------------------------------------------
(defmethod get-fun-vars ((form class-def)))

;;------------------------------------------------------------------------------
;; Fixpunkt bestimmen
;; Der Slot mv-called-by enth"alt eine Liste mit Eintr"agen der Form
;; (pos caller . var-list)
;; Wenn zu dem Slot has-funs-as-args einer Funktion ein Parameter hinzugef"ugt
;; wird, der an der Position pos steht, dann
;; werden diejenigen Parameter von caller, die in var-list angegeben sind, in
;; has-funs-as-args von caller eingetragen.
;;------------------------------------------------------------------------------
(defun propagate-fun-vars ()
  (let ((counter 0))
    (labels ((update-fun-vars (info newspec)
               (when (is-fun-arg-pos (pop info) newspec)
                 (let* ((fun (pop info))
                        (params (?params fun))
                        (spec (?has-funs-as-args fun))
                        (changed nil))
                   (unless spec (incf counter))
                   (dolist (var info)
                     (multiple-value-bind (spec1 changed1)
                         (add-fun-var var params spec)
                       (when changed1
                         (setq changed t)
                         (setq spec spec1))))
                   (when changed
                     (setf (?has-funs-as-args fun) spec)
                     (dolist (info (?mv-called-by fun))
                       (update-fun-vars info spec))))))
             (determine-fun-vars (fun-list)
               (dolist (fun fun-list)
                 (let ((spec (?has-funs-as-args fun)))
                   (when spec (incf counter))
                   (dolist (info (?mv-called-by fun))
                     (update-fun-vars info spec)))
                 (determine-fun-vars (?local-funs fun)))))

      ;; Bestimmen des Fixpunktes im Aufrufgraphen der definierten Funktionen
      ;; Die Hauptfunktion wird nicht betrachtet, da sie nicht aufgerufen wird.
      ;;-----------------------------------------------------------------------
      (determine-fun-vars (?fun-list *module*)))
    
    (clicc-message "~s special functions found " counter)))

;;------------------------------------------------------------------------------
(provide "closure-analysis")    
