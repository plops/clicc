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
;;; Funktion : Seiteneffektanalyse
;;;            Implementierung der folgenden Funktion:
;;;            effect-form : ZWS * EFFECT  -----> EFFEKT
;;;            wobei
;;;            ZWS    : die Menge der Zwischensprachobjekte
;;;            EFFEKT : {(read-list,write-list)/
;;;                       read-list , write-list Mengen von Variablen }
;;;
;;; $Revision: 1.88 $
;;; $Id: static-effect.lisp,v 1.88 1994/11/22 14:49:16 hk Exp $
;;;--------------------------------------------------------------------------

(in-package "CLICC")
(require "se-init")
(require "optimize")
(require "tail")
(require "closure-analysis")

;;------------------------------------------------------------------------
;; Analyse eines Moduls, Let-optimierug und Tail-Rekursion.
;;------------------------------------------------------------------------
(defun analyse-module ()

  (effect-module)

  (unless *no-let-optimizing*
    (let-optimizing))

  (clicc-message-line))

;;-------------------------------------------------------------------------
;; Analyse eines Modules durch eine Fixpunktiteration.
;;-------------------------------------------------------------------------
(defun effect-module ()

  ;; Hier werden die Funktionen weiter attributiert um bessere
  ;; Ergebnisse bei der Seiteneffektanalyse zu erzielen.
  ;;----------------------------------------------------------
  (unless *no-pre-analysis*
    (clicc-message "Preparing side effect analysis...")
    (clicc-message "---------------------------------")
    (pre-analyse-module))

  (clicc-message "Side-effect analysis...")
  (clicc-message "-----------------------")
  (dolist (fun (?fun-list *module*))
    (setf (?mv-called-by fun) ()))

  (dolist (fun (?fun-list *module*))
    (effect-fun fun))
  (effect-fun (?toplevel-forms *module*))

  (clicc-message "Propagating side effects")
  (clicc-message "------------------------")
  (propagate-effects)

  (clicc-message "Preparing statistics")
  (unless (zerop *Side-effect-verbosity*)
    (statistics)))

;;------------------------------------------------------------------------------
;; Die Variable enthält
;; - T, wenn das Resultat des Ausdrucks als Funktion aufgerufen wird oder als
;;   Argument an eine Funktion mit funktionalen Argumenten "ubergeben wird und
;; - nil sonst.
;;------------------------------------------------------------------------------
(defvar *app-pos*)

;;------------------------------------------------------------------------------
;; Eine Liste der Parameter, die an Funktionen gebunden werden m"ussen.
;; Wird aus dem Slot ?has-funs-as-args rekonstruiert.
;; Bei lokalen Funktionen sind in dieser Liste auch die entsprechenden
;; Parameter der umfassenden Funktionen enthalten.
;;------------------------------------------------------------------------------
(defvar *fun-arg-vars*)

;;------------------------------------------------------------------------------
(defun effect-form-no-app (form)
  (let ((*app-pos* nil))
    (effect-form form)))

;;-------------------------------------------------------------------------
;; Hier wird eine Funktionsdefinition analysiert.
;; Aus dem erhaltenen Effekt werden alle lokale Seiteneffekte
;; entfernt, und der globale Effekt wird in den Slots read-list
;; write-list und data-effects abgespeichert.
;;-------------------------------------------------------------------------
(defun effect-fun (fun)
  (let* ((*fun-arg-vars* (if (local-fun-p fun)
                             (append *fun-arg-vars* (calc-fun-arg-vars fun))
                             (calc-fun-arg-vars fun)))
         (*current-fun* fun)
         (*level* (?level fun)))

    (when (local-fun-p fun) (setf (?calls-free-param fun) nil))
    (effect-params (?params fun))
    (effect-form-no-app (?body fun))))

;;--------------------------------------------------------------------------
(defun effect-params (parameter)
  (dolist (opt-param (?opt-list parameter))
    (effect-form-no-app (?init opt-param)))
  (dolist (key-param (?key-list parameter))
    (effect-form-no-app (?init key-param))))

;;--------------------------------------------------------------------------
;; Lesender Zugriff auf eine Variable:
;; lesende Zugriffe auf Konstanten werden ignoriert
;;--------------------------------------------------------------------------
(defmethod effect-form ((ref var-ref))
  (let ((var (?var ref)))
    (cond
      ;; wenn die applizierte Variable im Slot has-funs-as-args
      ;; enthalten ist, dann ist sie aus der Argument Liste dieser
      ;; Funktion oder der naechst globaleren Funktionen und
      ;; steht auf Funktionsposition und wird ausserdem nur
      ;; gelesen oder so veraendert dass der neue Wert
      ;; nur eine aehnliche Variable oder eine bekannte Funktion
      ;; ist.
      ;; In diesem Fall besteht der Effekt dieser Applikation aus
      ;; dem Lesen dieser Variablen.
      ;;----------------------------------------------------------
      ((or (not *app-pos*) (member var *fun-arg-vars*))
       (when (or (not (dynamic-p var))
                 (eql (?constant-value (?sym var)) :no-const))
         (setf (?read-list *current-fun*)
               (add-to-var-set var (?read-list *current-fun*)))

         ;; Aufruf eines freien Parameters, der potentiell eine Funktion
         ;; enth"alt. Das verursacht einen lokalen Top-Effekt, was in
         ;; ?calls-free-param vermerkt wird.
         ;;---------------------------------         
         (when (and *app-pos* (< (?level var) *level*))
           (setf (?calls-free-param *current-fun*)
                 (min-level (?calls-free-param *current-fun*) (?level var))))))

      ;; sonst ist keine Information ueber die Applizierte Variable
      ;; vorhanden, also wird der Effekt auf TOP gesetzt.
      ;;--------------------------------------------------
      (t (to-unknown-call *current-fun*)))))

;;--------------------------------------------------------------------------
;; Schreibender Zugriff auf eine Variable
;;--------------------------------------------------------------------------
(defmethod effect-form ((form setq-form))
  (setf (?write-list *current-fun*)
        (add-to-var-set (?var (?location form)) (?write-list *current-fun*)))
  (effect-form (?form form)))

;;------------------------------------------------------------------------------
;; freie Variablen werden eingetragen
;;------------------------------------------------------------------------------
(defun add-to-var-set (var list-or-level)
  (if (listp list-or-level)
      (progn
        (when (or (dynamic-p var) (< (?level var) *level*))
          (pushnew var list-or-level))
        list-or-level)
      (if (and (static-p var) (< (?level var) *level*))
          (max (?write-list *current-fun*) (?level var))
          list-or-level)))

;;------------------------------------------------------------------------------
;; Eine Funktion hat einen Effekt, wenn sie auf der linken Seite einer
;; Applikation vorkommt.
;;------------------------------------------------------------------------------
(defmethod effect-form ((fun fun))
  (when (and *app-pos*
             (not (eq fun *error-function*))
             (not (eq fun *current-fun*)))
    (pushnew *current-fun* (?mv-called-by fun))))

;;------------------------------------------------------------------------------
(defmethod effect-form ((fun imported-fun))
  (when (and *app-pos* (not (eq fun *error-function*)))
    (union-all-effects *current-fun* fun)))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form foreign-fun))
  (when *app-pos* (to-unknown-call *current-fun*)))

;;--------------------------------------------------------------------------
(defmethod effect-form ((app app))
  (if *app-pos*
      (to-unknown-call *current-fun*)
      (let ((form (?form app))
            (arg-list (?arg-list app))
            (*app-pos* t))

        (effect-form form)

        (typecase form
          (fun
           (let ((key-pos (get-key-pos form))
                 (i 0))

             ;; required & optional arguments
             ;;------------------------------
             (loop
              (when (or (null arg-list) (and key-pos (>= i key-pos))) (return))
              (if (pos-fun-arg-p i form)
                  (effect-form (pop arg-list))
                  (effect-form-no-app (pop arg-list)))
              (incf i))

             ;; keyword arguments
             ;;------------------
             (loop
              (when (null arg-list) (return))
              (cond
                ((sym-p (car arg-list))
                 (if (key-fun-arg-p (pop arg-list) form)
                     (when arg-list (effect-form (pop arg-list)))
                     (when arg-list (effect-form-no-app (pop arg-list)))))

                ;; das Keyword wird berechnet
                ;;---------------------------
                (t (to-unknown-call *current-fun*)
                   (effect-form-no-app (pop arg-list))
                   (when arg-list (effect-form-no-app (pop arg-list))))))))

          (t
           ;; Funktion ist nicht bekannt, also auch nicht, welche Parameter
           ;; die Eigenschaft has-funs-as-args haben.
           ;;----------------------------------------
           (dolist (arg arg-list)
             (effect-form-no-app arg)))))))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form let*-form))
  (dolist (one-form (?init-list form))
    (effect-form-no-app one-form))
  (effect-form (?body form)))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form labels-form))
  (dolist (fun (?fun-list form))
    (setf (?mv-called-by fun) ()))
  (dolist (fun (?fun-list form))
    (effect-fun fun))
  (effect-form (?body form)))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form if-form))
  (effect-form-no-app (?pred form))
  (effect-form (?then form))
  (effect-form (?else form)))

;;-----------------------------------------------------------------------------
(defmethod effect-form ((form switch-form))
  (effect-form-no-app (?form form))
  (dolist (case (?case-list form))
    (effect-form (?form case)))
  (effect-form (?otherwise form)))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form progn-form))
  (mapc-progn-form-list (?form-list form)
                        #'effect-form-no-app
                        #'effect-form))

;;-----------------------------------------------------------------------------
(defmethod effect-form ((form tagbody-form))
  (effect-form-no-app (?first-form  form))
  (mapc-tagged-form-list (?tagged-form-list form)
                         #'effect-form-no-app
                         #'effect-form))

;;--------------------------------------------------------------------------
;; Eine GO-FORM zu einem Tag, der in der *current-fun* definiert ist
;; hat keinen Seiteneffekt, sonst verursacht er einen Sprung au"serhalb
;; der Funktion also :JUMP
;;--------------------------------------------------------------------------
(defmethod effect-form ((form tagged-form))
  (when (< (?level (?tagbody form)) *level*)
    (setf (?data-effects *current-fun*)
          (max-data-effects (?data-effects *current-fun*) :jump))))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form let/cc-form))

  ;; Wir wissen nicht, welche Funktion berechnet wird.
  ;;--------------------------------------------------
  (when *app-pos* (to-unknown-call *current-fun*))
  (effect-form (?body form)))

;;-------------------------------------------------------------------------
(defmethod effect-form ((form cont))
  (when *app-pos*
    (when (< (?level form) *level*)
      (setf (?data-effects *current-fun*)
            (max-data-effects (?data-effects *current-fun*) :jump)))))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form mv-lambda))
  (effect-params (?params form))
  (effect-form-no-app (?arg form))
  (effect-form (?body form)))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form defined-named-const))
  (if (eq (?value form) :unknown)
      (when *app-pos* (to-unknown-call *current-fun*))
      (effect-form (?value form))))

;;--------------------------------------------------------------------------
(defmethod effect-form ((form imported-named-const))
  (when *app-pos* (to-unknown-call *current-fun*)))

;;------------------------------------------------------------------------------
(defmethod effect-form ((form literal)))

;;------------------------------------------------------------------------------
(defmethod effect-form ((form class-def)))


;;------------------------------------------------------------------------------
;; Bestimmt die Seiteneffekte der definierten Funktionen, indem durch ein
;; Fixpunktverfahren die lokal bestimmten Werte von effect entlang der
;; Kanten mv-called-by propagiert werden.
;;------------------------------------------------------------------------------
(defun propagate-effects ()
  (labels ((update-effect (fun newread newwrite newdata newcfp callee-level)
             (let* ((oldread (?read-list fun))
                    (oldwrite (?write-list fun))
                    (olddata (?data-effects fun))
                    (oldcfp (?calls-free-param fun))
                    (read oldread)
                    (write oldwrite)
                    (data olddata)
                    (cfp oldcfp)
                    (caller-level (?level fun)))

               (cond

                 ;; globale Funktion ruft globale oder
                 ;; lokale ruft globale
                 ;;--------------------
                 ((>= caller-level callee-level)
                  (setq read (unify-lists read newread))
                  (setq write (unify-lists write newwrite))
                  (setq data (max-data-effects data newdata))
                  (setq cfp (min-level cfp newcfp)))

                 ;; globale Funktion ruft lokale Funktion,
                 ;; lokale Effekte vorher entfernen.
                 ;;---------------------------------
                 (t                     ; (< caller-level callee-level)
                  (setq read (unify-lists read (remove-local-effect
                                                newread caller-level)))
                  (setq write (unify-lists write (remove-local-effect
                                                  newwrite caller-level)))
                  (setq data (max-data-effects data newdata))
                  (setq cfp (min-level cfp (remove-local-cfp
                                            newcfp caller-level)))))

               (unless (and (eql read oldread)
                            (eql write oldwrite)
                            (eql data olddata)
                            (eql cfp oldcfp))
                 (setf (?read-list fun) read)
                 (setf (?write-list fun) write)
                 (setf (?data-effects fun) data)
                 (setf (?calls-free-param fun) cfp)
                 (dolist (called-by (?mv-called-by fun))
                   (update-effect
                    called-by read write data cfp caller-level)))))

           (determine-effect (fun-list)
             (dolist (fun fun-list)
               (let ((read (?read-list fun))
                     (write (?write-list fun))
                     (data (?data-effects fun))
                     (cfp (?calls-free-param fun))
                     (level (?level fun)))
                 (dolist (called-by (?mv-called-by fun))
                   (update-effect called-by read write data cfp level)))
               (determine-effect (?local-funs fun)))))

    ;; Bestimmen des Fixpunktes im Aufrufgraphen der definierten Funktionen
    ;;---------------------------------------------------------------------
    (determine-effect (?all-global-funs *module*))))

;;------------------------------------------------------------------------------
;; Entfernt alle Eintr"age, die nicht frei relativ zu level sind
;;------------------------------------------------------------------------------
(defun remove-local-effect (list-or-level level)
  (if (listp list-or-level)
      (remove-if #'(lambda (v)
                     (and (local-static-p v)
                          (>= (?level v) level)))
                 list-or-level)
      (min list-or-level (1- level))))

;;------------------------------------------------------------------------------
(defun remove-local-cfp (level caller-level)
  (if level
      (if (>= level caller-level) nil level)
      nil))

;;------------------------------------------------------------------------------
;; Funktionen zur Kodierung und Dekodierung der (A)-Listen, die diejenigen
;; Parameter beschreiben, die nur an Funktionen gebunden werden.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Rekonstruiert aus dem Inhalt des Slots has-funs-as-args
;; eine Liste der Variablen, die an Funktionen gebunden werden m"ussen.
;; Aufbau von has-funs-as-args:
;; pos              |
;; (pos1 pos2 ...)  |
;; ((pos1 pos2 ...) . (nsimpopt key1 key2 ...))
;;------------------------------------------------------------------------------
(defun calc-fun-arg-vars (fun)
  (let ((params (?params fun))
        (arg-spec (?has-funs-as-args fun)))
    (labels ((find-by-pos (i)
               (let* ((required (?var-list params))
                      (len (length required)))
                 (if (< i len)
                     (nth i required)
                     (?var (nth (- i len) (?opt-list params))))))
             (find-by-key (key)
               (?var (find key (?key-list params) :key #'?sym))))
      (cond
        ((null arg-spec) nil)
        ((atom arg-spec) (list (find-by-pos arg-spec)))
        ((integerp (car arg-spec)) (mapcar #'find-by-pos arg-spec))
        (t (append (mapcar #'find-by-pos (car arg-spec))
                   (mapcar #'find-by-key (cdr (cdr arg-spec)))))))))

;;------------------------------------------------------------------------------
;; Konstruiert aus einer A-Liste von Paaren der Form (var . Status) den
;; Inhalt des Slots has-funs-as-args.
;; Jede Variable mit dem Status :FUN-AS-ARG-CALL wird ber"ucksichtigt.
;; Required und optionale Parameter werden durch ihre Position in der
;; Parameterliste kodiert, Keyword Parameter werden durch das Keyword kodiert.
;; In die Liste der Keywords wird als erstes Element die Position des ersten
;; Keyword-Arguments eingetragen.
;;------------------------------------------------------------------------------
(defun encode-fun-arg-alist (fun)
  (let* ((alist (?has-funs-as-args fun))
         (params (?params fun))
         (i-list ())
         (k-list ())
         (nsimple (length (?var-list params)))
         pos key)
    (dolist (arg-info alist)
      (when (eq :FUN-AS-ARG-CALL (cdr (get-alias arg-info)))
        (let ((var (car arg-info)))
          (cond
            ((setq pos (position var (?var-list params)))
             (push pos i-list))
            ((setq pos (position var (?opt-list params) :key #'?var))
             (push (+ pos nsimple) i-list))
            ((setq key (find var (?key-list params) :key #'?var))
             (push (?sym key) k-list))

            ;; freie Variablen bleiben unber"ucksichtigt
            ;;------------------------------------------
            (t nil)))))
    (setf (?has-funs-as-args fun)
          (cond
            (k-list
             (cons i-list (cons (+ nsimple (length (?opt-list params)))
                                k-list)))
            ((null i-list) nil)
            ((consp (cdr i-list)) i-list)
            (t (car i-list))))))

;;------------------------------------------------------------------------------
;; F"ugt die Kodierung der Variablen var entsprechend der Position in params
;; zu arg-spec hinzu.
;;------------------------------------------------------------------------------
(defun add-fun-var (var params arg-spec)
  (let (i k)
    (cond
      ((setq i (position var (?var-list params))))
      ((setq i (position var (?opt-list params) :key #'?var))
       (incf i (length (?var-list params))))
      ((setq k (find var (?key-list params) :key #'?var))
       (setq k (?sym k))))
    (cond
      (i (cond
           ((null arg-spec) (values i t))
           ((atom arg-spec) (if (eql i arg-spec)
                                (values arg-spec nil)
                                (values (list i arg-spec) t)))
           ((integerp (car arg-spec))
            (if (find i arg-spec)
                (values arg-spec nil)
                (values (cons i arg-spec) t)))
           (t (if (find i (car arg-spec))
                  (values arg-spec nil)
                  (values
                   (progn
                     (setf (car arg-spec) (list i (car arg-spec)))
                     arg-spec)
                   t)))))
      (k
       (let ((first-key (+ (length (?var-list params))
                           (length (?opt-list params)))))
         (cond
           ((null arg-spec) (values (cons () (list first-key k)) t))
           ((atom arg-spec)
            (values (cons (list arg-spec) (list first-key k)) t))
           ((integerp (car arg-spec))
            (values (cons arg-spec (list first-key k)) t))
           (t (if (find k (cdr (cdr arg-spec)))
                  (values arg-spec nil)
                  (values
                   (progn
                     (setf (cdr (cdr arg-spec)) (cons k (cdr (cdr arg-spec))))
                     arg-spec)
                   t))))))

      (t (break)))))

;;------------------------------------------------------------------------------
;; Bestimmt aus einer Argument-Position und einer Funktion
;; ob das Argument eine Funktion sein mu"s, indem es die Slots
;; has-funs-as-args analysiert.
;;------------------------------------------------------------------------------
(defun pos-fun-arg-p (i fun)
  (pos-fun-arg-p1 i (?has-funs-as-args fun)))

(defun pos-fun-arg-p1 (i arg-spec)
  (cond
    ((null arg-spec) nil)
    ((atom arg-spec) (eql i arg-spec))
    ((integerp (car arg-spec)) (find i arg-spec))
    (t (find i (car arg-spec)))))

;;------------------------------------------------------------------------------
;; Bestimmt aus einem Keyword einer Funktion
;; ob das zugeh"orige Argument eine Funktion sein mu"s, indem es die Slots
;; has-funs-as-args analysiert.
;;------------------------------------------------------------------------------
(defun key-fun-arg-p (k fun)
  (key-fun-arg-p1 k (?has-funs-as-args fun)))

(defun key-fun-arg-p1 (k arg-spec)
  (and (consp arg-spec)
       (listp (car arg-spec))
       (find k (cdr (cdr arg-spec)))))

;;------------------------------------------------------------------------------
(defun is-fun-arg-pos (pos arg-spec)
  (if (integerp pos)
      (pos-fun-arg-p1 pos arg-spec)
      (key-fun-arg-p1 pos arg-spec)))

;;------------------------------------------------------------------------------
;; Positionen des ersten Keyword Arguments oder nil
;; (= Anzahl der required + optionalen Parameter)
;------------------------------------------------------------------------------
(defun get-key-pos (fun)
  (let ((arg-spec (?has-funs-as-args fun)))
    (if (and (consp arg-spec)
             (listp (car arg-spec)))
        (car (cdr arg-spec))
        nil)))

;;--------------------------------------------------------------------------
;; Hilfsfunktionen ...
;;--------------------------------------------------------------------------

;;-----------------------------------------------------------------------
;; Has-top-effect "uberpr"uft ob der Seiteneffekt einer Funktion schon
;; der Top-effect ist.
;;-----------------------------------------------------------------------
(defun has-top-effect (function)
  (let ((r (?read-list function))
        (w (?write-list function))
        (level (?level function)))
    (and (integerp r)
         (eql r level)
         (integerp w)
         (eql w level)
         (eql :alloc-dest-jump (?data-effects function)))))

;;---------------------------------------------------------------------------
(defun empty-effect ()
  (make-instance 'effect))

;;------------------------------------------------------------------------------
;; Diese Funktion liefert den Effekt f"ur den Aufruf einer unbekannten
;; Funktion. Im allgemeinen k"onnen dadurch alle globalen und lokalen
;; Variablen modifiziert werden, so da"s das statische Niveau der aktuellen
;; Funktion eingetragen wird. Wenn die Funktion allerdings keine lokalen
;; Funktionen hat oder nur solche, die nicht auf die lokalen Variablen
;; zugreifen, dann kann ein optimistischerer Wert angenommen werden.
;; (Man k"onnte sogar noch optimistischer sein, wenn man das gr"o"ste Niveau
;; bestimmen w"urde, auf das die lokalen Funktionen noch zugreifen.)
;;------------------------------------------------------------------------------
(defun to-unknown-call (effect)
  (let ((level *level*))
    (unless (closure-uses-vars (?local-funs *current-fun*))
      (decf level))
    (setf (?read-list effect) level)
    (setf (?write-list effect) level)
    (setf (?data-effects effect) :alloc-dest-jump)))

;;------------------------------------------------------------------------------
(defun unify-lists (list1 list2)
  (if (and (listp list1) (listp list2))
      (dolist (l list2 list1)
        (pushnew l list1))
      (if (and (integerp list1) (integerp list2))
          (max list1 list2)
          (if (integerp list1)
              (combine-level-list list1 list2)
              (combine-level-list list2 list1)))))

;;--------------------------------------------------------------------------
;; Die kleinste obere Schranke zweier Elemente aus dem Verband
;; {nil, :alloc, :dest, :jump, :alloc-dest, :alloc-jump, :dest-jump,
;; :alloc-dest-jump}
;;--------------------------------------------------------------------------
(defun max-data-effects (d1 d2)
  (cond
    ((eq d1 d2) d1)
    ((null d1) d2)
    ((null d2) d1)
    ((or (eq d1 :alloc-dest-jump)
         (eq d2 :alloc-dest-jump)) :alloc-dest-jump)
    ((and (member d1 '(:alloc :dest :alloc-dest))
          (member d2 '(:alloc :dest :alloc-dest))) :alloc-dest)
    ((and (member d1 '(:alloc :jump :alloc-jump))
          (member d2 '(:alloc :jump :alloc-jump))) :alloc-jump)
    ((and (member d1 '(:jump :dest :dest-jump))
          (member d2 '(:jump :dest :dest-jump))) :dest-jump)
    (t :alloc-dest-jump)))

;;----------------------------------------------------------------------------
;; Die Vereinigung einer Menge von Variablen "LIST" und einer Menge,
;; die durch ein statische Niveau "LEVEL"kodiert wird, ist das Maximum
;; aller statischen Levels aller statischen Variablen aus "LIST"
;; und "LEVEL".
;;----------------------------------------------------------------------------
(defun combine-level-list (level list)
  (reduce #'(lambda (x var) (if (static-p var) (max x (?level var)) x))
          list
          :initial-value level))

;;--------------------------------------------------------------------------
;; union-all-effects wird benutzt, um die Effekte einer Applikation
;; zu berechnen. Sie vereinigt die read- und die write-listen der
;; applizierten Form mit den der Argumenten ohne den *level*
;; abzufragen.
;; Denn bei einer Applikation einer lokalen Funktion muessen auch die
;; lokalen Variablen stehen die durch den Aufruf gelesen oder veraendert
;; werden.
;;--------------------------------------------------------------------------
(defun union-all-effects (target-effect effect)
  (setf (?read-list target-effect)
        (unify-lists (?read-list  target-effect) (?read-list  effect)))
  (setf (?write-list target-effect)
        (unify-lists (?write-list target-effect) (?write-list effect)))
  (setf (?data-effects target-effect)
        (max-data-effects (?data-effects target-effect)
                          (?data-effects effect))))

;;------------------------------------------------------------------------------
(defun min-level (l1 l2)
  (cond
    ((null l1) l2)
    ((null l2) l1)
    (t (min l1 l2))))

;;------------------------------------------------------------------------
;; Zur Zeit nur diese einfache Ausgabe der Ergebnisse der Analyse.
;;------------------------------------------------------------------------
(defun statistics ()
  (let ((se-free 0)
        (se-top  0)
        (read    0)
        (write   0)
        (alloc   0)
        (dest    0)
        (jump    0)
        (number-of-funs 0))
    (labels
        ((stat-fun (fun)
           (incf number-of-funs)
           (let ((read-list    (?read-list fun))
                 (write-list   (?write-list fun))
                 (data-effects (?data-effects fun)))
             (when (and (integerp read-list)
                        (integerp write-list))
               (setq se-top (1+ se-top)))
             (when (and (null read-list)
                        (null write-list))
               (setq se-free (1+ se-free)))
             (when (integerp read-list)
               (setq read (1+ read)))
             (when (integerp write-list)
               (setq write (1+ write)))
             (when (or (eql data-effects   :alloc)
                       (eql data-effects   :alloc-dest)
                       (eql data-effects   :alloc-jump)
                       (eql data-effects   :alloc-dest-jump))
               (setq alloc (1+ alloc)))
             (when (or (eql data-effects   :dest)
                       (eql data-effects   :alloc-dest)
                       (eql data-effects   :dest-jump)
                       (eql data-effects   :alloc-dest-jump))
               (setq dest (1+ dest)))
             (when (member data-effects '(:jump :alloc-jump :dest-jump
                                          :alloc-dest-jump))
               (incf jump)))

           (dolist (fun (?local-funs fun))
             (stat-fun fun))))


      (mapc #'stat-fun (?fun-list *module*))
      (stat-fun (?toplevel-forms *module*)))


    (clicc-message "~s functions are analysed" number-of-funs)
    (clicc-message "~s functions are side effect free"
                   se-free)
    (clicc-message "~s functions have an unknown side effect"
                   se-top)
    (clicc-message "~s functions have an unknown read-effect"
                   read)
    (clicc-message "~s functions have an unknown write-effect"
                   write)
    (clicc-message "~s functions may use the heap" alloc)
    (clicc-message "~s functions may be destructive" dest)
    (clicc-message "~s functions may make non local jumps" jump)))

;;--------------------------------------------------------------------------
(provide "static-effect")



