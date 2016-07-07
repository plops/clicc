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
;;; Funktion : Optimierungen der Let*-Ausdruecke.
;;;
;;; $Revision: 1.66 $
;;; $Id: optimize.lisp,v 1.66 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(defvar *let-effect*)
(defvar *local-effect*)

;;------------------------------------------------------------------------------
;; Enth"alt eine Variable, wenn die lokale Analyse nur bis zu dem ersten
;; Vorkommen der Variablen erfolgen soll.
;;------------------------------------------------------------------------------
(defvar *until-var* nil)

;;------------------------------------------------------------------------------
;; Z"ahler
;;------------------------------------------------------------------------------
(defvar *eliminated-vars*)
(defvar *eliminated-lets*)
(defvar *vars-bound-but-not-used*)
(defvar *subst-number*)

(defun let-optimizing ()
  (let ((*eliminated-vars* 0)
        (*eliminated-lets*  0)
        (*vars-bound-but-not-used* 0)
        (*subst-number* 0))
    (clicc-message "Optimizing the let-forms ...")
    (clicc-message-line 28)
    (setf (?fun-list *module*)
          (mapcar #'optimize-a-fun (?fun-list *module*)))
    (setf (?toplevel-forms *module*)
          (optimize-a-fun (?toplevel-forms *module*)))
    (clicc-message "~s variables are bound by let but not referenced"
                   *vars-bound-but-not-used*)
    (clicc-message "~s let-forms are eliminated" *eliminated-lets*)
    (clicc-message "~s let-variables are eliminated " *eliminated-vars*)
    (clicc-message "~s substitutions are done " *subst-number*)))


(defun optimize-a-fun (fun)
  (let* ((*current-fun* fun)
         (*level* (?level fun)))
    (setf (?body fun) (optimize-let (?body fun)))
    fun))

;;---------------------------------------------------------------------
;; Optimize-let traversiert die Zwischensprache und optimiert die
;; Let-ausdruecke.
;;---------------------------------------------------------------------
(defmethod optimize-let ((a-if-form if-form))
  (setf (?pred a-if-form) (optimize-let (?pred a-if-form)))
  (setf (?then a-if-form) (optimize-let (?then a-if-form)))
  (setf (?else a-if-form) (optimize-let (?else a-if-form)))
  a-if-form)

(defmethod optimize-let  ((a-progn progn-form))
  (setf (?form-list a-progn)
        (mapcar #'optimize-let (?form-list a-progn)))
  a-progn)

(defmethod optimize-let  ((a-let/cc let/cc-form))
  (setf (?body a-let/cc) (optimize-let (?body a-let/cc)))
  a-let/cc)

(defmethod optimize-let ((a-switch-form switch-form))
  (setf (?form a-switch-form) (optimize-let (?form a-switch-form)))
  (setf (?case-list  a-switch-form)
        (mapcar #'optimize-let (?case-list a-switch-form)))
  (setf (?otherwise a-switch-form)
        (optimize-let (?otherwise a-switch-form)))
  a-switch-form)

(defmethod optimize-let ((a-labeled-form labeled-form))
  (setf (?value a-labeled-form) (optimize-let (?value a-labeled-form)))
  (setf (?form  a-labeled-form) (optimize-let (?form  a-labeled-form)))
  a-labeled-form)

(defmethod optimize-let ((a-tagbody tagbody-form))
  (setf (?first-form a-tagbody) (optimize-let (?first-form a-tagbody)))
  (dolist (one-tagged-form (?tagged-form-list a-tagbody))
    (setf (?form one-tagged-form)  (optimize-let (?form one-tagged-form))))
  a-tagbody)

(defmethod optimize-let ((a-class-def class-def))
  (dolist (one-slot-desc (?slot-descr-list a-class-def))
    (unless (or (null (?initform one-slot-desc))
                (eq (?initform one-slot-desc) :unbound))
      (setf (?initform one-slot-desc)
            (optimize-let (?initform one-slot-desc)))))
  a-class-def)

(defmethod optimize-let ((a-app app))
  (setf (?arg-list a-app)
        (mapcar #'optimize-let (?arg-list a-app)))
  a-app)

(defun let-optimize-opt/key (opt/key)
  (setf (?init opt/key) (optimize-let (?init opt/key)))
  opt/key)

(defun let-optimize-params(parameter)
  (setf (?opt-list parameter) (mapcar #'let-optimize-opt/key
                                      (?opt-list parameter)))
  (setf (?key-list parameter) (mapcar #'let-optimize-opt/key
                                      (?key-list parameter)))
  parameter)

(defmethod optimize-let ((a-mv-lambda mv-lambda))
  (setf (?params a-mv-lambda) (let-optimize-params (?params a-mv-lambda)))
  (setf (?body   a-mv-lambda) (optimize-let (?body   a-mv-lambda)))
  (setf (?arg    a-mv-lambda) (optimize-let (?arg    a-mv-lambda)))
  a-mv-lambda)

(defmethod optimize-let ((a-labels labels-form))
  (setf (?fun-list a-labels) (mapcar #'optimize-a-fun (?fun-list a-labels)))
  (setf (?body a-labels) (optimize-let (?body a-labels)))
  a-labels)

(defmethod optimize-let ((a-let-form let*-form))

  (setf (?init-list a-let-form)
        (mapcar #'optimize-let (?init-list a-let-form)))
  (setf (?body a-let-form) (optimize-let (?body a-let-form)))

  ;; Hier passiert die Optimierung dieses Let-Konstrukts.
  ;;-----------------------------------------------------
  (if (null (?var-list a-let-form))

      ;; (Let*  ()() (form1 ... formN)) --> (progn form1... formN)
      ;;----------------------------------------------------------
      (optimize-0 a-let-form)

      ;; Eventuell wird VAR durch INIT-FORM substituiert.
      ;;-------------------------------------------------
      (let ((init-list (?init-list a-let-form))
            (*let-effect* (empty-effect))
            (*local-effect* (make-instance 'local-effect)))
        (effect-of-form a-let-form *let-effect* *local-effect*)
        (dolist (var (?var-list a-let-form))
          (if (dynamic-p var)
              (pop init-list)
              (setq a-let-form (optimize-1 var (pop init-list) a-let-form))))

        ;; Wenn eine Variable nicht mehr referenziert ist,
        ;; wird sie und die dazugehoerige Init-form aus
        ;; var-list bzw init-form-list entfernt.
        ;;------------------------------------------------
        (delete-unref-var a-let-form)
        (if (null (?var-list a-let-form))
            (optimize-0 a-let-form)
            a-let-form))))

(defmethod optimize-let ((any-thing-else form))
  any-thing-else)

;;-----------------------------------------------------------------------
;; (let () . body) --> body.
;;-----------------------------------------------------------------------
(defun optimize-0 (let-form)
  (setq *eliminated-lets* (1+ *eliminated-lets*))
  (?body let-form))

;;-----------------------------------------------------------------------
;; Optimize-1 untersucht die Init-form einer Variablen, und entscheidet
;; in Abhaengigkeit der moeglichen Effekte des Rumpfes des Let-
;; konstrukts, ob eine Substitution aller Vorkommen der Variablen
;; durch die Init-form moeglich ist.
;; Falls nicht alle Vorkommen der Variablen substituiert werden koennen,
;; wird keine Substitution gemacht.
;;-----------------------------------------------------------------------

(defun optimize-1 (var init-form let-form)
  (when (and (eql 0 (?read var)) (eql 1 (?write var)))
    (incf *vars-bound-but-not-used*))

  (cond
    ((is-a-single-assignment let-form)

     ;; Hier wird versucht folgende Substitution durchzuf"uhren:
     ;; (let ((loc init-form)) (setq var loc)) --> (setq var init-form)
     ;;----------------------------------------------------------------
     (if (let-to-assign-possible let-form)
         (update-let var init-form let-form)
         let-form))

    ((and (var-ref-p init-form)
          (>  (?read var) 1))

     ;; Subs-with-var-is-possible "uberpr"uft die Bedingungen,
     ;; um die Substitution einer Variablen durch eine andere Variable
     ;; durchf"uhren zu k"onnen.
     ;;---------------------------------------------------------
     (if (subst-with-var-is-possible var init-form let-form)
         (update-let var init-form let-form)
         let-form))

    ((and (eql 1  (?write var))
          (eql 1  (?read var)))

     ;; Die lokale Variable ist nur einmal im Rumpf referenziert
     ;; In diesem fall interessieren wir uns nur mit dem Seiteneffekt bis
     ;; zu dieser Referenz, was danach kommt ist uninteressant.
     ;;------------------------------------------------------------------
     (let ((let-effect-until-var   (empty-effect))
           (local-effect-until-var (make-instance 'local-effect)))
       (let ((*until-var* var))
         (catch 'var-reached
           (effect-of-form let-form
                           let-effect-until-var local-effect-until-var)))

       ;; Wenn die Init-form eine Variable ist:
       ;;--------------------------------------
       (if (var-ref-p init-form)
           (let ((glo-w-vars (?write-list let-effect-until-var))
                 (loc-w-vars (?write-list local-effect-until-var)))
             (if (or
                  (member-of-region var glo-w-vars loc-w-vars)
                  (member-of-region (?var init-form) glo-w-vars loc-w-vars)
                  (member (?var init-form) (?var-list let-form) :test #'eq))
                  let-form
                  (update-let var init-form let-form)))

           ;; Sonst wird der Seiteneffekt der Init-form berechnet, und dann
           ;; werden viele Bedingungen überprüft um die Substitution
           ;; vornehmen zu können.
           ;;--------------------------------------------------------------
           (let ((init-form-effect (empty-effect))
                 (init-form-local-effect (make-instance 'local-effect)))
             (effect-of-form init-form init-form-effect init-form-local-effect)
             (if (and
                  ;; Die Seiteneffekte der Init-form sind bekannt.
                  ;;----------------------------------------------
                  (known-side-effect init-form-effect
                                     init-form-local-effect)

                  ;; Weder die Init-form noch der Rumpf der Let-Konstruktes
                  ;; verursachen Sprünge oder destruktive Seiteneffekte.
                  ;;-------------------------------------------------------
                  (member (?data-effects init-form-effect) '(nil :alloc))
                  (member (?data-effects let-effect-until-var)
                          '(nil :alloc))

                  ;; Die Seiteneffekte der Init-form und die des Rumpfes
                  ;; haben keine Read-Write-Konflikte, also die Read-
                  ;; bzw die Write-Regionen haben keinen Schnitt.
                  ;;----------------------------------------------------
                  (no-interference init-form-effect init-form-local-effect
                                   let-effect-until-var
                                   local-effect-until-var)

                  ;; Die lokale Variable wird im Rumpf
                  ;; nicht mehr verändert.
                  ;;----------------------------------
                  (not (member-of-region var
                                         (?write-list let-effect-until-var)
                                         (?write-list local-effect-until-var))))

                 (update-let var init-form let-form)
                 let-form)))))
    (t let-form)))

(defun known-side-effect (effect local-effect)
  (and (listp (?read-list effect))
       (listp (?write-list effect))
       (listp (?read-list local-effect))
       (listp (?write-list local-effect))))

;;----------------------------------------------------------------------------
;; Diese Funktion überprüft ob der Seiteneffekt einer Init-form
;; (effect local-effect) Read-Write Konflikte mit dem Seiteneffekt des
;; Let-Konstrktes hat.
;; Also ob die Read-region und die Write-region einen Schnitt haben.
;;---------------------------------------------------------------------------
(defun no-interference (effect1 local-effect1 effect2 local-effect2)
  (let ((write-region (append  (?write-list effect1)
                               (?write-list local-effect1)))
        (read-region  (append  (?read-list  effect1)
                               (?read-list local-effect1))))
    (and (r/w-interference-free read-region
                                (?write-list  effect2))
         (r/w-interference-free read-region
                                (?write-list  local-effect2))
         (r/w-interference-free write-region
                                (?read-list   effect2))
         (r/w-interference-free write-region
                                (?read-list   local-effect2)))))

;;-------------------------------------------------------------------------
;; Der Rumpf einer Let-Form besteht nur aus einer Zuweisung. Außerdem
;; wird nur eine loale Variable gebunden. Solche Let-Froms können in eine
;; Zuweisung umgewandelt werden, wenn einige Bedingungen gelten.
;;-------------------------------------------------------------------------
(defun is-a-single-assignment (let-form)
  (and (endp (rest (?var-list let-form)))
       (setq-form-p (?body let-form))))

;;-----------------------------------------------------------------------
;; Let-to-assign-possible überprüft ob eine a-single-assignment zu einer
;; Zuweisung umgewandelt werden kann.
;;-----------------------------------------------------------------------
(defun let-to-assign-possible (let-form)
  (let ((body-form (?form (?body let-form)))
        (init-form (car (?init-list let-form)))
        (body-effect (empty-effect))
        (init-effect (empty-effect))
        (body-local-effect (make-instance 'local-effect))
        (init-local-effect (make-instance 'local-effect)))
    (effect-of-form body-form body-effect body-local-effect)
    (effect-of-form init-form init-effect init-local-effect)
    (and (known-side-effect init-effect init-local-effect)
         (member (?data-effects init-effect)  '(nil :alloc))
         (no-interference init-effect init-local-effect
                          body-effect body-local-effect)
         (not (member-of-region  (car (?var-list let-form))
                                 (?write-list body-effect)
                                 (?write-list body-local-effect))))))




;;------------------------------------------------------------------------
;; Eine Substitution der Variablen VAR durch die Variable INIT-VAR
;; ist nur erlaubt wenn :
;; 1 : VAR wird nicht im Rumpf oder in den init-formen veraendert.
;; 2 : INIT-VAR wird nicht im Rumpf oder in den init-formen  veraendert.
;; 3 : INIT-VAR wird nicht (im Falle einer dynamischen Variablen)
;;     nochmal gebunden nach dem Sie referenziert ist.
;;------------------------------------------------------------------------
(defun subst-with-var-is-possible (var init-var let-form)
  (let ((global-written-vars (?write-list *let-effect*))
        (local-written-vars (?write-list *local-effect*)))
    (and
     (not  (member-of-region var global-written-vars local-written-vars))

     (not  (member-of-region (?var init-var) global-written-vars
                             local-written-vars))
     (not (member (?var init-var) (?var-list let-form) :test #'eq)))))


;;--------------------------------------------------------------------------
;; Die READ und WRITE-LISTS sind entweder Listen von Variablen oder
;; integerzahlen, die Mengen von Varaiblen kodieren.
;; member-of-region fragt ab of eine Variable direkt in der Liste
;; ist, oder, im Falle einer integerzahl, ob die Variable in der
;; durch diese integerzahl kodierte Menge ist.
;;--------------------------------------------------------------------------
(defun member-of-region (var global-region local-region)
  (or (and (listp global-region)
           (member var global-region :test #'eq))
      (member var local-region :test #'eq)
      (and (integerp global-region)
           (dynamic-p var))
      (and (integerp global-region)
           (static-p var)
           (>= global-region (?level var)))))

;;-------------------------------------------------------------------------
;; r/w-interference-free ist true , wenn keine Variable von region1
;; enthalten ist in region2, wenn region2 eine Liste ist,
;; und wenn region2 eine Integer ist dann soll sie echt kleiner
;; sein als das statische Niveau jeder Variablen in region1 und
;; ungekehrt. Wenn beide integerzahlen sind, sind sie nicht disjunkt.
;;-------------------------------------------------------------------------

(defun r/w-interference-free (region1 region2)
  (if (and (listp region1)
           (listp region2))
      (null (intersection region1 region2))
      (if (and (integerp region1)
               (integerp region2))
          nil
          (if (integerp region2)
              (if (null region1)
                  T
                  (< region2 (apply #'max
                                    (mapcar #'code-level-of-var region1))))
              (if (null region2)
                  T
                  (< region1 (apply #'max
                                    (mapcar #'code-level-of-var region2))))))))


(defun code-level-of-var (var)
  (if (dynamic-p var)
      -1
      (?level var)))

;;-------------------------------------------------------------------------
;; entscheidet ob eine Form keine Seiteneffekte hat, also auch entfernt
;; werden kann, wenn ihr Resultat nicht mehr benötigt wird.
;;-------------------------------------------------------------------------
(defun is-side-effect-free (form)
  (let ((global-effect (empty-effect))
        (local-effect  (make-instance 'local-effect)))
    (effect-of-form form global-effect local-effect)

    (and (null (?write-list global-effect))
         (null (?write-list local-effect))
         (member (?data-effects global-effect) '(nil :alloc)))))

;;-------------------------------------------------------------------------
;; Diese Funktion fragt ab, ob eine Form dessen Seiteneffekt 'Effect' ist,
;;  Daten destruktiv veraendert.
;;-------------------------------------------------------------------------
(defun not-destructive (effect)
  (not (member (?data-effects effect) '(:dest :alloc-dest :alloc-dest-jump
                                        :dest-jump))))

;;------------------------------------------------------------------------
;; update-let wird aufgerufen um eine  Substitution vorzunehmen,
;; nachdem "optimize-1" diese erlaubt hat.
;;------------------------------------------------------------------------
(defun update-let (var init-form let-form)

  ;; Update der read-slots bei  Variablen.
  ;; Wenn die Init-form eine Variablenreferenz ist, dann wird
  ;; der Slot READ bei der entsprechenden Variablen um die Anzahl
  ;; der Referenzen von VAR erhoeht,
  ;; ansonsten ist die init-form entweder eine Seiteneffektfreie Form
  ;; wo es keine Variablen referenziert sind, oder eine Form die nur
  ;; einmal referenziert ist.
  ;;----------------------------------------------
  (when (var-ref-p init-form)
    (setf (?read (?var init-form)) (+ (?read (?var init-form)) (?read var))))
  (setf (?read var) 0)


  ;; Die Substitution wird zunaechst bei den Initformen der let-form
  ;; vorgenommen.
  ;;---------------
  (setf (?init-list let-form)
        (mapcar #'(lambda (init) (subst-var var init-form init))
                (?init-list let-form)))

  ;; Nun wird in dem Rumf substituiert.
  ;;-----------------------------------
  (setf (?body let-form) (subst-var var init-form (?body let-form)))
  let-form)


;;-------------------------------------------------------------------------
;; Nun werden die VAR-LIST und die INIT-LIST einer LET-FORM nach der
;; Substitutionsphase updated.
;;-------------------------------------------------------------------------
(defun delete-unref-var (let-form)
  (let* ((var-list (?var-list let-form))
         (init-list (?init-list let-form))
         (var-list+1 (cons nil var-list))
         (init-list+1 (cons nil init-list))
         (var-cons var-list+1)
         (init-cons init-list+1)
         var)
    (loop
     (when (null var-list) (return))
     (setq var (car var-list))
     (cond
       ((or (dynamic-p var) (< 0 (?read var)) (< 1 (?write var)))

        ;; Die Variable wurde nicht durch Init-form substituiert.
        ;;-------------------------------------------------------
        (setq var-cons var-list)
        (setq init-cons init-list)
        (setq var-list (cdr var-list))
        (setq init-list (cdr init-list)))

       (T
        ;; Die Variable wurde Substituiert.
        ;;---------------------------------
        (incf *eliminated-vars*)
        (setq var-list (cdr var-list))
        (setq init-list (cdr init-list))
        (setf (cdr var-cons) var-list)
        (setf (cdr init-cons) init-list))))
    (setf (?var-list let-form) (cdr var-list+1))
    (setf (?init-list let-form) (cdr init-list+1))))

;;-------------------------------------------------------------------------
;; Hier wird jedes  Vorkommen von "var" in "body" durch "form" ersetzt.
;;-------------------------------------------------------------------------
(defun subst-var (var form body)
  (labels ((map-subst-var (one-form) (subst-var var form one-form)))

    (etypecase body

      (var-ref
       (if (eq (?var body) var)
            (progn (incf *subst-number*) form)
            body))

      (if-form
       (setf (?pred body) (subst-var var form (?pred body)))
       (setf (?then body) (subst-var var form (?then body)))
       (setf (?else body) (subst-var var form (?else body)))
       body)

      (setq-form
       (setf (?form body) (subst-var var form (?form body)))
       body)

      (progn-form
       (setf (?form-list body) (mapcar #'map-subst-var (?form-list body)))
       body)

      (tagbody-form
       (setf (?first-form body) (subst-var var form (?first-form body)))
       (dolist (one-tagged-form (?tagged-form-list body))
         (setf (?form one-tagged-form)
               (subst-var var form (?form one-tagged-form))))
       body)

      (switch-form
       (setf (?form body) (subst-var var form (?form body)))
       (dolist (a-labeled-form (?case-list body))
         (setf (?form a-labeled-form)
               (subst-var var form (?form a-labeled-form))))
       (setf (?otherwise body) (subst-var var form (?otherwise body)))
       body)

      (let/cc-form
       (setf (?body body) (subst-var var form (?body body)))
       body)

      (let*-form
       (setf (?init-list body) (mapcar #'map-subst-var (?init-list body)))
       (setf (?body body) (subst-var var form (?body body)))
       body)

      (labels-form
       (dolist (one-fun (?fun-list body))
         (subst-var-in-param var form (?params one-fun))
         (setf (?body one-fun) (subst-var var form (?body one-fun))))
       (setf (?body body) (subst-var var form (?body body)))
       body)

      (app
       (setf (?form body) (subst-var var form (?form body)))
       (setf (?arg-list body) (mapcar #'map-subst-var (?arg-list body)))
       body)

      (mv-lambda
       (subst-var-in-param var form (?params body))
       (setf (?body body) (subst-var var form (?body body)))
       (setf (?arg body) (subst-var var form (?arg body)))
       body)

      (cont body)
      (form body))))

;;-------------------------------------------------------------------------
(defun subst-var-in-param (var form params)
  (dolist (opt (?opt-list params))
    (setf (?init opt) (subst-var var form (?init opt))))
  (dolist (key (?key-list params))
    (setf (?init key) (subst-var var form (?init key)))))

;;------------------------------------------------------------------------------
;; Gibt an, ob ein Argument einer Funktion analysiert wird, das einem
;; Parameter zugeordnet ist, der eine Funktion als Wert haben mu"s.
;;------------------------------------------------------------------------------
(defvar *fun-arg-pos*)

;;------------------------------------------------------------------------------
;; Hat Wert True, wenn gerade ein tagbody analysiert wird. In diesem Fall
;; darf die Analyse aufgrund von *until-var* nicht einfach abgebrochen werden,
;; sondern mu"s fortgesetzt werden, bis das tagbody vollst"andig analysiert
;; worden ist.
;; Wenn innerhalb eines tagbody die Variable, die durch *until-var* benannt
;; wird, gefunden wurde, dann erh"alt *in-tagbody* diese Variable als Wert,
;; womit angezeigt wird, da"s nach der Analyse des "au"sersten tagbody die
;; Analyse abgebrochen werden darf.
;;------------------------------------------------------------------------------
(defvar *in-tagbody* nil)

;;-------------------------------------------------------------------------
;; EFFECT-OF-FORM berechnet die Effekte einer Form.
;; Die Effekte, die durch Funktionsaufrufe entstehen werden in
;; "effect" gesammelt, die textuell sichtbaren "also Referenzieren
;; von Variablen oder Veraenderung durch SETQ" werden in local-effect
;; abgespeichert. Wenn der Parameter *until-var* gesetzt wird, wird die
;; die Analyse nach dieser Variablenreferenz beendet. Dies wird ben"otigt
;; um die Seiteneffekte nach dieser Variablenreferenz zu ignorieren.
;;-------------------------------------------------------------------------
(defun effect-of-form (form effect local-effect)
  (setq *in-tagbody* nil)
  (effect-no-fun-arg form effect local-effect))

(defun effect-no-fun-arg (form effect local-effect)
  (let ((*fun-arg-pos* nil))
    (effect-preserve-fun-arg form effect local-effect)))

;;------------------------------------------------------------------------------
;; Der Zustand von *fun-arg-pos* bleibt erhalten
;;------------------------------------------------------------------------------
(defun effect-preserve-fun-arg (form effect local-effect)
  (etypecase form

    (var-ref
     (when *fun-arg-pos* (to-top-effect effect))
       
     (if (and *until-var* (eq *until-var* (?var form)))
         (if *in-tagbody*
             (setq *in-tagbody* *until-var*)
             (throw 'var-reached nil))
         (pushnew (?var form) (?read-list local-effect))))

    (setq-form
     (effect-preserve-fun-arg (?form form) effect local-effect)
     (let ((loc (?location form)))
       (when (var-ref-p loc)
         (pushnew (?var loc) (?write-list local-effect)))))

    (fun (when (and *fun-arg-pos* (not (eq form *error-function*)))
           (union-all-effects effect form)))

    (foreign-fun (when *fun-arg-pos* (to-unknown-call effect)))

    (app
     ;; Erst die Argumente und danach die Funktion analysisren,
     ;; damit *until-var* wirksam werden kann.
     ;; 
     ;; wir m"ussen die Variablen finden, die links in einer
     ;; Applikation oder auf funs-as-args Position
     ;; stehen und deren Effekt nach TOP-EFFECT korrigieren.
     ;;-----------------------------------------------------
     (let* ((functional (?form form))
            (arg-list (?arg-list form)))
       (typecase functional
         (fun
          (cond
            ((not (?has-funs-as-args functional))
                
             ;; In dem Fall, wo die Funktion keine spec-vars hat,
             ;; m"ussen wir nur in die Argumente hineingehen.
             ;;-----------------------------------------------------------
             (dolist (one-arg arg-list)
               (effect-no-fun-arg one-arg effect local-effect)))

            (t

             ;; Einige der Argumente werden an spec-vars gebunden.
             ;; Wenn der Wert eines solchen Arguments aus einer spec-var
             ;; gelesen wird, so mu"s der Effekt nach TOP-EFFECT korrigiert
             ;; werden.
             ;;-----------------------------------------------------------
             (let ((key-pos (get-key-pos functional))
                   (i 0))

               ;; required & optional arguments
               ;;------------------------------
               (loop
                (when (or (null arg-list) (and key-pos (>= i key-pos)))
                  (return))
                (let ((*fun-arg-pos* (pos-fun-arg-p i functional)))
                  (effect-preserve-fun-arg (pop arg-list) effect local-effect))
                (incf i))

               ;; keyword arguments
               ;;------------------
               (loop
                (when (null arg-list) (return))
                (cond
                  ((sym-p (car arg-list))
                   (let ((*fun-arg-pos*
                          (key-fun-arg-p (pop arg-list) functional)))
                     (when arg-list
                       (effect-preserve-fun-arg (pop arg-list)
                                                effect local-effect))))

                  ;; das Keyword wird berechnet
                  ;;---------------------------
                  (t (effect-no-fun-arg (pop arg-list) effect local-effect)
                     (when arg-list
                       (effect-no-fun-arg (pop arg-list)
                                          effect local-effect))))))))

          (if (?calls-free-param functional)

            ;; Der Aufruf einer lokalen Funktion, die einen Parameter einer
            ;; globaleren Funktion aufruft, verursacht lokal betrachtet den
            ;; Top-Effekt
            ;;-----------
            (to-top-effect effect)
            (union-all-effects effect functional)))
         
         (t 
          ;; Funktion ist nicht bekannt, also wurde f"ur kein Argument
          ;; die Eigenschaft has-funs-as-args ausgenutzt.
          ;;---------------------------------------------
          (dolist (one-arg arg-list)
            (effect-no-fun-arg one-arg effect local-effect))
          (let ((*fun-arg-pos* t))
            (effect-preserve-fun-arg functional effect local-effect))))))
         
    
    (let*-form

     ;; ACHTUNG: Hier muss zun"aechst die Var-list untersucht werden,
     ;;          dann wird die Init-list und abschliessend der Rumpf
     ;;          analysiert werden. Diese Reihenfolge ist sehr wichtig,
     ;;          und darf nicht geaendert werden, weil die Berechnung
     ;;          evtl bei einer gesuchten Variablenreferenz abgebrochen
     ;;          wird. In diesem Fall garantiert diese Reihenfolge dass
     ;;          an der richtigen Stelle abgebrochen wird.
     ;;          
     ;; Die Var-list wird als erste untersucht damit die dynamisch gebundenen
     ;; Variablen, die durch einen Let neu gebunden werden, in die WRITE-LIST
     ;; von EFFECT eingetragen werden, bevor die Init-list untersucht wird.
     ;; Denn bei der Untersuchung von der Init-list kann die Analyse
     ;; bei der gesuchten Variablenreferenz abgebrochen werden.
     ;;----------------------------------------------------------------------
     (dolist (one-var (?var-list form))
       (when (dynamic-p one-var)
         (when (listp (?write-list effect))
           (pushnew one-var (?write-list effect)))))

     (dolist (one-init-form (?init-list form))
       (effect-no-fun-arg one-init-form effect local-effect))
     (effect-preserve-fun-arg (?body form) effect local-effect))

    (labels-form

     ;; Die Effekte der lokalen Funktionen werden mitgerechnet, um zu
     ;; "uberpr"ufen ob in den R"umpfen dieser Funktionen substituiert
     ;; werden kann. Dies ist ganz wichtig damit auch die Slots "read"
     ;; und "write" bei Variablen richtig gesetzt werden, und keine
     ;; falschen Optimierungen vorgenommen werden.
     ;;---------------------------------------------------------------
     (dolist (one-local-fun (?fun-list form))
       (union-all-effects effect one-local-fun))
     (effect-preserve-fun-arg (?body form) effect local-effect))

    (if-form
     (effect-no-fun-arg (?pred form) effect local-effect)
     (effect-preserve-fun-arg (?then form) effect local-effect)
     (effect-preserve-fun-arg (?else form) effect local-effect))

    (switch-form
     (effect-no-fun-arg (?form form) effect local-effect)
     (dolist (case (?case-list form))
       (effect-preserve-fun-arg (?form case) effect local-effect))
     (effect-preserve-fun-arg (?otherwise form) effect local-effect))
    
    (progn-form
     (mapc-progn-form-list
      (?form-list form)
      #'(lambda (form) (effect-no-fun-arg form effect local-effect))
      #'(lambda (form) (effect-preserve-fun-arg form effect local-effect))))

    (tagbody-form
     (let ((outmost-tagbody (not *in-tagbody*)))
       (when outmost-tagbody (setq *in-tagbody* t))
       (effect-no-fun-arg (?first-form  form) effect local-effect)
       (mapc-tagged-form-list
        (?tagged-form-list form)
        #'(lambda (form) (effect-no-fun-arg form effect local-effect))
        #'(lambda (form) (effect-preserve-fun-arg form effect local-effect)))
       (when (and outmost-tagbody (eq *in-tagbody* *until-var*))
         (throw 'var-reached nil))))
    
    (tagged-form (setf (?data-effects effect)
                       (max-data-effects :jump (?data-effects effect))))

    (let/cc-form
     (effect-preserve-fun-arg (?body form) effect local-effect))

    (cont (setf (?data-effects effect)
                       (max-data-effects :jump (?data-effects effect))))

    (mv-lambda
     (effect-of-params (?params form) effect local-effect)
     (effect-no-fun-arg (?arg form) effect local-effect)
     (effect-preserve-fun-arg (?body form) effect local-effect))

    (defined-named-const
        (if (eq (?value form) :unknown)
            (when *fun-arg-pos* (to-top-effect effect))
            (effect-preserve-fun-arg (?value form) effect local-effect)))

    (imported-named-const (when *fun-arg-pos* (to-top-effect effect)))

    (class-def nil)
    
    (literal nil)))

;;------------------------------------------------------------------------------
(defun effect-of-params (param effect local-effect)
  (dolist (opt (?opt-list param))
    (effect-no-fun-arg (?init opt) effect local-effect))
  (dolist (key (?key-list param))
    (effect-no-fun-arg (?init key) effect local-effect)))

;;------------------------------------------------------------------------------
(defun to-top-effect (effect)
  (setf (?read-list effect) -1)
  (setf (?write-list effect) -1)
  (setf (?data-effects effect) :alloc-dest-jump))

;;--------------------------------------------------------------------------
(provide "optimize")

