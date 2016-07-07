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
;;; Funktion : Methoden zum Traversieren der Zwischensprache, die die 
;;;            Zwischensprachausdruecke analysieren, Typen inferieren 
;;;            und die entsprechenden Ausdruecke mit Typinformationen 
;;;            versehen.
;;;
;;; $Revision: 1.50 $
;;; $Id: tipass2.lisp,v 1.50 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "titypes")
(require "tidef")
(require "tidecl")
(require "timisc")


;;------------------------------------------------------------------------------
;; Es folgen die Methoden zu der generischen Funktion `analyse-types', die zu
;; jedem Zwischensprachkonstrukt die entsprechenden Sprachteile analysiert.
;; Waehrend des Traversierens werden die entsprechenden Typannotationen gesetzt.
;;------------------------------------------------------------------------------
(defgeneric analyse-types (analysable-object))

;;------------------------------------------------------------------------------
;; analyse-objects analysiert die Liste der angegebenen Funktionen und Klassen
;; bis zum Erreichen des Typfixpunktes.
;;------------------------------------------------------------------------------
(defun analyse-objects (list)

  ;; Bei einer Fixpunktiteration werden zunaechst alle Elemente einmal
  ;; analysiert. Dadurch wird bei grossen Programmen der Fixpunkt in 
  ;; der Regel zuegiger erreicht.
  ;;------------------------------------------------------------------
  (when (do-interprocedural-type-inference)
    (dolist (an-object list)
      (analyse-object an-object T)))

  (do ((*ti-workset*  list)
       (iterations 0 (1+ iterations)))
      ((endp *ti-workset*))
      
    (write-size-of-workset *ti-workset* iterations)
    (analyse-object (pop *ti-workset*))))


;;------------------------------------------------------------------------------
;; Die Typanalyse einer im Modul definierten Funktion bindet die Argumenttypen
;; an die Parameter und analysiert den Rumpf.
;;------------------------------------------------------------------------------
(defmethod analyse-object ((a-defined-fun defined-fun) &optional no-iteration)
  (let* ((*type-environment* (copy-type-env (?pred-type-env a-defined-fun)))
         (*successor-workset* nil))
    
    ;; Binden der Parameter:
    ;;----------------------
    (bind-parameter-types (?params a-defined-fun))

    ;; Analyse des Funktionsrumpfes
    ;;-----------------------------
    (let* ((old-result-type     (?result-type a-defined-fun))
           (old-result-type-env (?result-type-env a-defined-fun))
           (new-result-type     (analyse-types (?body a-defined-fun)))
           (types-are-unchanged (zs-subtypep new-result-type old-result-type)))
      
      (multiple-value-bind (new-result-type-env env-changed)
          (join-pred-type-env-with-type-env old-result-type-env 
                                            *type-environment*)
        (when env-changed
          (setf (?result-type-env a-defined-fun) new-result-type-env
                types-are-unchanged nil)))

      (cond (no-iteration
             (update-type-f (?result-type a-defined-fun) new-result-type))

            (types-are-unchanged
             (setf *ti-workset* (union *successor-workset*
                                        *ti-workset*)))
            (T
             (setf (?result-type a-defined-fun) new-result-type)
             (setf *ti-workset* (union (union *successor-workset*
                                              (union (?called-by a-defined-fun)
                                                     (list a-defined-fun)))
                                       *ti-workset* )))))))


;;------------------------------------------------------------------------------
(defmethod analyse-object ((a-class-def class-def) &optional no-iteration)
  (declare (ignore no-iteration))
  (dolist (a-slot-desc (?slot-descr-list a-class-def))
    (unless (or (null (?initform a-slot-desc))
                (eq (?initform a-slot-desc) :unbound))
      (analyse-types (?initform a-slot-desc)))))


;;------------------------------------------------------------------------------
;; Die Typanalyse von bennanten Konstanten liefert den Wert der bei der 
;; einmaligen Zuweisung gesetzten Typkomponente.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-named-const named-const))
  (?type a-named-const))


;;------------------------------------------------------------------------------
;; Bei Variablenreferenzen wird die zugehoerige Variable analysiert.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-var-ref var-ref))
  (analyse-types (?var a-var-ref)))


;;------------------------------------------------------------------------------
;; Bei statischen Variablen wird der Wert der Typbindung geliefert.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-static static))
  (get-type a-static *type-environment*))


;;------------------------------------------------------------------------------
;; Die Analyse dynamischer Variablen liefert entweder die Typbindung oder, falls
;; sie eine Common Lisp-Konstante mit bekanntem Wert repraesentiert, den Typ der
;; Konstanten.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-dynamic dynamic))
  (let ((constant-value (?constant-value (?sym a-dynamic))))
    (case constant-value
      (:no-const
       (if (use-bindings-of-dynamic-variables) 
           (get-type a-dynamic *type-environment*)
           top-t))
      (:unknown
       (?type a-dynamic))
      (otherwise
       (analyse-types constant-value)))))


;;------------------------------------------------------------------------------
;; Continuations als funktionales Objekt liefern den Funktionstyp.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-cont cont))
  function-t)


;;------------------------------------------------------------------------------
;; Typanalyse der Literale liefert den im tiPass1 ermittelten (konstanten) Typ.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-literal literal))
  (?type a-literal))

(defmethod analyse-types ((a-fun fun))
  function-t)


;;------------------------------------------------------------------------------
;; Wird eine Continuation appliziert, dann wird das Typ-Bottomelement geliefert.
;; Ausserdem wird in die Annotation `type' der Argumenttyp der Applikation mit
;; aufgenommen und die aktuelle Typumgebung wird auf die leere Umgebung gesetzt.
;;------------------------------------------------------------------------------
(defmethod analyse-function-app ((a-cont cont) argument-types)
  (update-type-f (?result-type a-cont) (first argument-types))
  (setf (?type-env a-cont) 
        (join-type-environments *type-environment* (?type-env a-cont))
        *type-environment*
        (empty-environment))
  bottom-t)


;;------------------------------------------------------------------------------
;; Die Applikation einer benutzerdefinierten Funktion liefert den Wert der
;; Komponente ?result-type.
;;------------------------------------------------------------------------------
(defun analyse-defined-function-app (a-fun argument-types &optional union-type)
  (let* ((old-type-env    
          *type-environment*)
         (old-pred-type-env   
          (?pred-type-env a-fun))
         (param-types-changed 
          (not (if union-type
                   (simple-set-parameter-types (?params a-fun) union-type)
                   (set-parameter-types (?params a-fun) argument-types)))))

    (multiple-value-bind (new-pred-type-env env-changed)
      (join-pred-type-env-with-type-env old-pred-type-env *type-environment*)

      (cond (env-changed 
             (setf (?pred-type-env a-fun) new-pred-type-env)
             (pushnew a-fun *successor-workset*))
            (param-types-changed
             (pushnew a-fun *successor-workset*))
            (T nil)))
      
    (setf *type-environment* old-type-env)
    (?result-type a-fun)))


;;------------------------------------------------------------------------------
(defmethod analyse-function-app ((a-fun fun) argument-types)
  (analyse-defined-function-app a-fun argument-types))


;;------------------------------------------------------------------------------
;; Die Typanalyse einer importierten Funktion liefert wenn die Funktion bekannt
;; ist den entsprechenden Resultattyp, das Top-Element des Typverbandes sonst.
;;------------------------------------------------------------------------------
(defmethod analyse-function-app ((an-imp-fun imported-fun) argument-types)
  (if (number-of-args-is-not-ok (?par-spec an-imp-fun) (length argument-types))
      bottom-t
      (let ((type-abstraction-function (?type-abstraction-function an-imp-fun)))
        
        (if (null type-abstraction-function)
            (?result-type an-imp-fun)
            (apply type-abstraction-function argument-types)))))


;;------------------------------------------------------------------------------
;; Der Typ einer Applikation ist der Ergebnistyp des applizierten Ausdrucks bei
;; Anwendung auf die Argumente.
;; Applikationen koennen Seiteneffekte haben. Deshalb ist die Typumgebung nach 
;; der Applikationen so zu aendern, dass die Bindungen der von Seiteneffekten
;; der applizierten Funktion abhaengenden Variablen geloescht werden.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((an-app app))
  (let ((form     (?form     an-app))
        (arg-list (?arg-list an-app)))
    (analyse-types form)
    (let* ((arg-types 
            (mapcar #'analyse-types arg-list))
           (called-funs
            (?called-funs an-app))
           (result-types 
            (mapcar (if (and (special-sys-fun-p form) (?special-caller form))
                        #'(lambda (a-fun)
                            (funcall (?special-caller form) a-fun arg-list))
                         
                        #'(lambda (a-fun)
                            (if (and (special-sys-fun-p a-fun) 
                                     (?tipass a-fun))
                                (apply (?tipass a-fun) arg-list)
                                (analyse-function-app a-fun arg-types))))
                                   
                    called-funs))
           (result
            (if (some #'is-bottom-t arg-types)
                bottom-t
                (apply #'multiple-type-join result-types))))

      ;; Wenn eine importierte Funktion appliziert wird und Typisierungen
      ;; fuer Parameter vorliegen, verwende diese, um:
      ;;  1. die Typbindung der an Argumentposition stehenden Variablen und
      ;;  2. den Ergebnistyp der Applikation
      ;; zu verbessern.
      ;;-------------------------------------------------------------------
      (when (and (imported-fun-p form)
                 (?argument-types form))
        (mapc #'(lambda (argument assertion)
                  (when (var-ref-p argument)
                    (assert-type (?var argument) assertion *type-environment*))
                  (when (is-bottom-t (type-meet assertion (?type argument)))
                    (setf result bottom-t)))
              arg-list (?argument-types form)))

      (if (?other-funs an-app)
          top-t
          result))))


;;------------------------------------------------------------------------------
;; Aendere die Typbindungen entsprechend der moeglichen Seiteneffekte der
;; Applikation.
;;------------------------------------------------------------------------------
(defmethod analyse-types :after ((an-app app))

  ;; Wenn eine definierte Funktion appliziert worden ist, dann aktualisiere 
  ;; die Typumgebung um die in der applizierten Funktion erfolgten 
  ;; Typaenderungen.
  ;; Die geaenderten Typbindungen liegen in der Komponente `result-type-env' 
  ;; der applizierten Funktion.
  ;;
  ;; Ansonsten setze die Typbindungen der von der Seiteneffektanalyse als
  ;; schreibend veraendert angezeigten Variablen auf das Topelement.
  ;; Wenn die Menge der schreibend veraenderten Variablen unbekannt ist, 
  ;; dann setze alle Typbindungen auf das Topelement. 
  ;; Wenn durch die vorliegende Funktionsanwendung destruktive Seiteneffekte 
  ;; auf Variablen ausgeloest worden sind, setze die Typbindungen geeignet
  ;; zurueck.
  ;;------------------------------------------------------------------------
  (let ((form (?form an-app)))
    (cond
      ((defined-fun-p form)
       (setf *type-environment* 
             (update-type-env *type-environment* 
                              (?result-type-env form)))
       (reset-type-bindings '() (not-destructive form)))
    
      ((and (imported-fun-p form) (?has-funs-as-args form))
       (let ((effect (empty-effect))
             (local-effect (make-instance 'local-effect)))
         (effect-of-form an-app effect local-effect)
         (reset-type-bindings (unify-lists (?write-list effect)
                                           (?write-list local-effect))
                              (not-destructive effect))))
      ((?other-funs an-app)
       (reset-type-bindings -1 nil))
    
      (T (dolist (called (?called-funs an-app))
           (when (fun-p called)
             (reset-type-bindings (?write-list called)
                                  (not-destructive called))))))))


;;------------------------------------------------------------------------------
;; Die Applikationen der folgenden Funktionen werden in `appfuns',
;; `static-effect' und in der Typinferenz gesondert behandelt.
;;------------------------------------------------------------------------------
(p0-special-funs
 (?special-caller "SC")
 clicc-lisp::funcall
 clicc-lisp::apply
 clicc-lisp::mapcar
 clicc-lisp::maplist
 clicc-lisp::mapcon
 clicc-lisp::mapcan
 clicc-lisp::mapc
 clicc-lisp::mapl)
 

;;------------------------------------------------------------------------------
(defun SC-funcall (a-fun argument-list)
  (analyse-function-app a-fun (mapcar #'?type (rest argument-list))))

;;------------------------------------------------------------------------------
(defun SC-apply (a-fun argument-list)
  (if (imported-fun-p a-fun)
      (?result-type a-fun)
      (let* ((raw-args      (rest argument-list))
             (raw-arg-types (mapcar #'?type raw-args))
             (applied-types (if raw-arg-types
                                (cons (list-component 
                                       (first (last raw-arg-types)))
                                      (butlast raw-arg-types))
                                top-t))
             (union-type    (apply #'multiple-type-join applied-types)))
        (analyse-defined-function-app a-fun nil union-type))))
      

;;------------------------------------------------------------------------------
;; Hilfsfunktion f"ur mapcar, mapc und mapcan
;;------------------------------------------------------------------------------
(defun mapcxx (a-fun argument-list)
  (let* ((list-types (mapcar #'?type (rest argument-list)))
         (arg-types  (mapcar #'list-component list-types))
         (result     (analyse-function-app a-fun arg-types)))
    result))

;;------------------------------------------------------------------------------
;; Hilfsfunktion f"ur maplist, mapl und mapcon
;;------------------------------------------------------------------------------
(defun maplxx (a-fun argument-list)
  (let* ((list-types (mapcar #'?type (rest argument-list)))
         (arg-types  (mapcar #'(lambda (lt) (type-join null-t lt)) list-types))
         (result     (analyse-function-app a-fun arg-types)))
    result))

;;------------------------------------------------------------------------------
(defun SC-mapcar (a-fun argument-list)
  (list-of (mapcxx a-fun argument-list)))

;;------------------------------------------------------------------------------
(defun SC-maplist (a-fun argument-list)
  (list-of (maplxx a-fun argument-list)))


;;------------------------------------------------------------------------------
(defun SC-mapcan (a-fun argument-list)
  (type-join null-t (mapcxx a-fun argument-list)))

;;------------------------------------------------------------------------------
(defun SC-mapcon (a-fun argument-list)
  (type-join null-t (maplxx a-fun argument-list)))

;;------------------------------------------------------------------------------
(defun SC-mapc (a-fun argument-list)
  (mapcxx a-fun argument-list)
  (?type (second argument-list)))

;;------------------------------------------------------------------------------
(defun SC-mapl (a-fun argument-list)
  (maplxx a-fun argument-list)
  (?type (second argument-list)))

                 
;;------------------------------------------------------------------------------
;; Die Applikationen der folgenden Funktionen sehr speziell behandelt.
;;------------------------------------------------------------------------------
(p0-special-funs
 (?tipass "TI")
 clicc-lisp::coerce
 clicc-lisp::concatenate)

;;------------------------------------------------------------------------------
(defun ti-coerce (&rest arguments)
  (if (< (length arguments) 2)
      bottom-t
      (get-intern-type (second arguments))))

(defun ti-concatenate (&rest arguments)
  (if (rest arguments)
      (type-meet sequence-t (get-intern-type (first arguments)))
      bottom-t))
 

;;------------------------------------------------------------------------------
;; Der Typ eines setq ist der Typ der zugehoerigen Form. Als Seiteneffekt 
;; wird bei Zuweisungen an Variablen die entsprechende Typbindung aktualisiert.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-setq-form setq-form))
  (let ((location (?location a-setq-form))
        (result   (analyse-types (?form a-setq-form))))
    
    ;; Bei Variablenreferenzen wird noch deren Typbindung aktualisiert.
    ;;-----------------------------------------------------------------
    (unless (named-const-p location)
      (let ((var (?var location)))
        (setf (?type location) result)
        (when (or (static-p var)
                  (and (dynamic-p var) 
                       (eq (?constant-value (?sym var)) :no-const))))
        (bind-and-update-type var result)))
      
    result))


;;------------------------------------------------------------------------------
;; Alle Elemente der form-list werden analysiert. Analysierte Typbindungen 
;; werden von Ausdruck zu Nachfolgeausdruck propagiert. Geliefert wird der 
;; Typ des letzten Ausdrucks.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-progn-form progn-form))
  (mapc-progn-form-list (?form-list a-progn-form)
                        #'analyse-types
                        #'analyse-types))


;;------------------------------------------------------------------------------
;; Der Typ eines if-Konstruktes ist der Vereinigungstyp der Typen der then- und
;; der else-Ausdruecke.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((an-if-form if-form))

  (analyse-types (?pred an-if-form))
  
  ;; Merke die Typumgebung vor der Analyse der then- und else-Teile.
  ;;----------------------------------------------------------------
  (multiple-value-bind (then-entry-env else-entry-env)
      (get-type-assertions-from-predicate-position (?pred an-if-form) 
                                                   *type-environment*)

    (let (then-exit-type 
          then-exit-env 
          else-exit-type 
          else-exit-env)
      
      ;; Analyse des then-Teils:
      ;;------------------------
      (setf *type-environment* then-entry-env
            then-exit-type     (analyse-types (?then an-if-form))
            then-exit-env      *type-environment*)
      
      ;; Analyse des else-Teils:
      ;;------------------------
      (setf *type-environment* else-entry-env
            else-exit-type     (analyse-types (?else an-if-form))
            else-exit-env      *type-environment*)
      
      ;; Setzen der Ergebnisumgebung und Rueckgabe des Resultates:
      ;;----------------------------------------------------------
      (setf *type-environment* (join-type-environments then-exit-env
                                                       else-exit-env))
      (type-join then-exit-type else-exit-type))))


;;------------------------------------------------------------------------------
;; Die Typanalyse einer switch-Form analysiert die Form zur Bestimmung des
;; Schluessels und liefert dann den Vereinigungstypen der Analysen der
;; case-Liste.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-switch-form switch-form))
  (analyse-types (?form a-switch-form))
  
  ;; Merke die Typumgebung vor der Analyse des switch-Konstruktes.
  ;;-------------------------------------------------------------- 
  (let* ((old-environment    *type-environment*)
         (result-type        (analyse-types (?otherwise a-switch-form)))
         (result-environment *type-environment*))
    
    (dolist (a-case (?case-list a-switch-form))
      (setf *type-environment* (copy-type-env old-environment)
            result-type        (type-join (analyse-types a-case) result-type)
            result-environment (join-type-environments *type-environment*
                                                       result-environment)))
    
    (setf *type-environment* result-environment)
    result-type))


;;------------------------------------------------------------------------------
;; Analysiere den Wert und den Ausdruck der labeled-form.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-labeled-form labeled-form))
  (analyse-types (?value a-labeled-form))
  (analyse-types (?form a-labeled-form)))


;;------------------------------------------------------------------------------
;; Der Typ einer labels-form ist der Typ des Rumpfes.
;; Die Funktionsbeschreibungen zu den hier lokal definierten Funktionen sind
;; zu Beginn der Analyse bereits angelegt worden.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-labels-form labels-form))
  (analyse-types (?body a-labels-form)))


;;------------------------------------------------------------------------------
;; Die Typanalyse einer let*-Form bindet die Typen der Initialisierungsformen an
;; die zu bindenden Variablen. Als Ergebnis wird das Analyseergebnis des Rumpfes
;; geliefert.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-let*-form let*-form))
  (let ((old-type-env (copy-type-env *type-environment*))
        (bound-vars-env '()))
    
    (mapc #'(lambda (variable init-form)
              (push (cons variable (get-type variable *type-environment*))
                    bound-vars-env)
              (bind-and-update-type variable (analyse-types init-form)))
          (?var-list a-let*-form)
          (?init-list a-let*-form))
    
    (let ((result (analyse-types (?body a-let*-form))))
      
      ;; Ruecksetzen der Typumgebung
      ;;----------------------------
      (setf *type-environment* 
            (reset-let-type-env old-type-env 
                                *type-environment* 
                                bound-vars-env))
      
      result)))


;;------------------------------------------------------------------------------
;; Bei let/cc-Ausdruecken wird der Rumpf analysiert. Waehrend der Analyse des
;; Rumpfes werden jeweils die Typen und die aktuellen Typumgebungen der 
;; Ruecksprungstellen in Annotationen der Continuation abgelegt.
;; Das Analyseergebnis ist davon abhaengig, ob die Continuation (auch) als
;; funktionales Objekt verwendet wird oder nicht.
;; Wenn die Continuation als funktionales Objekt verwendet wird, dann kann mit
;; irgendeiner Typumgebung und einem unbekannten Typ zurueckgesprungen werden.
;; Deshalb wird die Typumgebung zurueckgesetzt und als Ergebnistyp das Top-
;; element geliefert.
;; Wenn die Continuation nicht als funktionales Objekt verwendet wird, dann
;; werden die Ergebnistypumgebung des Rumpfes und die an den Ruecksprungstellen
;; gesammelten Typumgebungen vereinigt. Der Ergebnistyp ist die Typvereinigung
;; aus dem Ergebnistyp des Rumpfes und den an den Ruecksprungstellen gesammelten
;; Typen.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-let/cc-form let/cc-form))
  (let ((cont (?cont a-let/cc-form)))

    (let* ((body-result-type (analyse-types (?body a-let/cc-form)))
           (body-result-env  *type-environment*))
      
      (cond ((?unknown-caller cont)
             (set-all-type-bindings-to top-t)
             top-t)
            (T 
             (setf *type-environment* 
                   (join-type-environments body-result-env (?type-env cont)))
             (type-join body-result-type (?result-type cont)))))))


;;------------------------------------------------------------------------------
;; Bei der Analyse der tagbody-Konstrukte wird das folgende Fixpunktverfahren
;; angewendet. Fuer alle tagged-forms werden die Typumgebungen saemtlicher 
;; Vorgaenger im Datenfluss gesammelt. In einem tagbody-Konstrukt mit den 
;; tagged-forms {T0, T1, ..., Tn} sind die Vorgaenger der tagged-form Ti
;; alle Ausdruecke der Form (GO Ti) und Ti-1.
;;------------------------------------------------------------------------------
(defvar *type-env-changed*)

(defmethod analyse-types ((a-tagbody-form tagbody-form))
  (let ((result (analyse-types (?first-form a-tagbody-form)))
        (tagged-form-list (?tagged-form-list a-tagbody-form)))

    (let ((entry-env *type-environment*))
           
      (loop
       (setq *type-environment* entry-env)
       (let* ((*type-env-changed* nil))
              
         (dolist (tagged-form tagged-form-list)

           ;; die letzte tagged-form bzw. die first-form ist Vorgänger
           ;; der nächsten tagged-form.
           ;;--------------------------
           (add-predecessor-type-env tagged-form *type-environment*)
           (setq *type-environment* 
                 (copy-type-env (?pred-type-env tagged-form)))
           (setq result (analyse-types (?form tagged-form))))
              
         (unless *type-env-changed*
           (return))))

      ;; Resultat des und Typumgebung nach dem letzten Element der
      ;; tagged-form-list bzw. der first-form, falls tagged-form-list leer ist.
      ;;-----------------------------------------------------------------------
      result)))


;;------------------------------------------------------------------------------
;; Wird eine tagged-form analysiert, dann handelt es sich um ein angewandtes
;; Vorkommen. Alle definierenden Vorkommen werden schon im zugehoerigen 
;; tagbody-Konstrukt analysiert.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-tagged-form tagged-form))
  (add-predecessor-type-env a-tagged-form *type-environment*)
  (setf *type-environment* (empty-environment))
  bottom-t)


;;------------------------------------------------------------------------------
;; add-predecessor-type-env vereinigt eine Typumgebung mit der bisherigen 
;; Vereinigungsumgebung aller Vorgaenger.
;;------------------------------------------------------------------------------
(defun add-predecessor-type-env (a-tagged-form type-env)
  (multiple-value-bind (result-env type-env-changed)
      (join-type-environments (?pred-type-env a-tagged-form) type-env)
    (setf (?pred-type-env a-tagged-form) result-env)
    (when type-env-changed
      (setf *type-env-changed* T))))


;;------------------------------------------------------------------------------
;; Bei mv-lambda wird zur Zeit noch an alle Parameter das Top-Element des 
;; Typverbandes gebunden.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((an-mv-lambda mv-lambda))
  ;; Analysiere zuerst den Ausdruck, der eventuell multiple Werte liefert.
  (analyse-types (?arg an-mv-lambda))
  
  (let* ((old-type-env (copy-type-env *type-environment*))
         (params       (?params an-mv-lambda)))
    
    ;; Setze alle Parametertypen auf das Topelement.
    ;;----------------------------------------------
    (simple-set-parameter-types params)
    
    ;; Binden der Parameter
    ;;---------------------
    (bind-parameter-types params)
    
    ;; Analyse des Rumpfs
    ;;-------------------
    (let ((result (analyse-types (?body an-mv-lambda))))
      
      ;; Ruecksetzen der Typumgebung
      ;;----------------------------
      (setf *type-environment* 
            (update-type-env old-type-env *type-environment*))
      
      result)))


;;------------------------------------------------------------------------------
;; Alle Klassen haben zunaechst den Typ class-t
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-class class-def))
  class-t)


;;------------------------------------------------------------------------------
;; Analyse der foreign-fun-Konstrukte liefert zur Zeit noch das Topelement des
;; Typverbands.
;;------------------------------------------------------------------------------
(defmethod analyse-types ((a-foreign-fun foreign-fun))
  top-t)


;;------------------------------------------------------------------------------
;; In der around-Methode zu analyse-types wird die Typkomponente der analysier-
;; ten Form gesetzt.
;;------------------------------------------------------------------------------
(defmethod analyse-types :around ((a-form form))
  (update-type-f (?type a-form) (call-next-method)))

(defmethod analyse-types :around ((a-cont cont))
  (update-type-f (?type a-cont) (call-next-method)))


;;------------------------------------------------------------------------------
(provide "tipass2") 
