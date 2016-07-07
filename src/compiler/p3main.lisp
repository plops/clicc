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
;;; Funktion : Attributierung des Zwischencodes fuer die Codeerzeugung
;;;
;;; $Revision: 1.77 $
;;; $Id: p3main.lisp,v 1.77 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defvar *LEVEL*)                        ; Aktuelles statisches Niveau
(defvar *LOCAL-FUNS*)                   ; Direkt im Rumpf definierte lokale Fkt.
(defvar *CONST-LIST*)                   ; In einer globalen Fkt. enthaltene
                                        ; strukturierte Konstanten
(defvar *DOWNWARD-FUNARG* NIL)          ; Das funktionale Argument wird an
                                        ; dieser Stelle nur als
                                        ; 'downward function' verwendet.
(defvar *DOWNFUNS*)                     ; Queue der Downward-Funargs
(defvar *MV-SPEC*    NIL)               ; Anzahl der Rueckgabewerte
                                        ; einer Funktion
(defvar *MV-CALLS*   ())                ; Liste der Funktionen, die fuer die
                                        ; Ermittlung von mv-spec benoetigt
                                        ; werden
(defvar *MV-POSITION* NIL)              ; Wird die aktuelle Form fuer die
                                        ; Ermittlung von mv-spec benoetigt ?
(defvar *FN-ON-MV-POS* NIL)             ; Steht die Funktionsanwendung auf
                                        ; Ergebnispos. ?
(defvar *CONTINUATIONS*)                ; Liste aller Continuations des Prog.
(defvar *MV-LAMBDAS*)                   ; Liste aller Vorkommen von mv-lambda

;;------------------------------------------------------------------------------
;; Funktionen, die in Pass3 in besonderer Weise analysiert werden
;;------------------------------------------------------------------------------
(p0-special-funs
 (?pass3 "P3")
 rt::progv
 rt::catch
 rt::unwind-protect
 L::apply
 L::values)

;;------------------------------------------------------------------------------
;; PASS_3
;;------------------------------------------------------------------------------
(defun pass_3 ()
  (dolist (fun (?fun-list *module*))
    (setf (?mv-called-by fun) ()))
  (setq *CONTINUATIONS* ())
  (setq *MV-LAMBDAS* ())
  (mapc #'p3-defun (?fun-list *module*))
  (p3-defun (?toplevel-forms *module*))
  (p3-analyse-mv-spec))

;;------------------------------------------------------------------------------
(defun p3-defun (fun)

  ;; Globale Funktionen haben das statische Niveau 0.
  ;;-------------------------------------------------
  (let ((*LEVEL* 0)
        (*CONST-LIST* (empty-queue)))

    (p3-fun-def fun)
    (setf (?const-list fun) (queue2list *CONST-LIST*)))

  ;; Bestimmen, welche lokalen Funktionen NICHT globalisiert werden
  ;; koennen.  Eine lokale Funktion kann nicht globalisiert werden, wenn
  ;; sie freie lexikalisch gebundene Variablen enthaelt oder wenn eine
  ;; ihrer freien lokalen Funktionen nicht globalisiert werden kann.  Es
  ;; wird zunaechst davon ausgegangen, dass alle lokalen Funktionen
  ;; globalisierbar sind.  Wenn eine Funktion freie lexikalisch gebundene
  ;; Variablen enthaelt, wird sie als nicht globalisierbar gekennzeichnet.
  ;; Wenn eine Funktion f als nicht globalisierbar gekennzeichnet wurde,
  ;; muessen alle lokalen Funktionen, in denen f direkt oder indirekt frei
  ;; vorkommt, auch als nicht globalisierbar gekennzeichnet werden.
  ;;---------------------------------------------------------------------
  (process-local-funs fun
                      #'(lambda (fun)
                          (when (and (?free-lex-vars fun)
                                     (?as-global-fun fun))
                            (labels ((propagate (fun)
                                       (setf (?as-global-fun fun) nil)
                                       (dolist (fun (?free-in fun))
                                         (when (?as-global-fun fun)
                                           (propagate fun)))))
                              (propagate fun)))))

  ;; Lokale Funktionen, die als funktionales Objekt verwendet werden und
  ;; von denen nicht gezeigt werden konnte, dass sie nur 'downward'
  ;; verwendet werden, sind als Closure gekennzeichnet.  Die
  ;; Closure-Eigenschaft muss an alle nicht globalisierten lokalen
  ;; Funktionen, die in Closures frei vorkommen, propagiert werden.  Die
  ;; lokal freien lex. Variablen von Closures kennzeichnen.
  ;;---------------------------------------------------------------------
  (process-local-funs fun
                      #'(lambda (fun)
                          (when (and (eq (?closure fun) :CLOSURE)
                                     (not (?as-global-fun fun)))
                            (labels ((propagate (fun)
                                       (dolist (var (?free-lex-vars fun))
                                         (setf (?closure var) t))
                                       (dolist (fun (?free-local-funs fun))
                                         (unless
                                             (or (eq (?closure fun)
                                                     :CLOSURE)
                                                 (?as-global-fun fun))
                                           (setf (?closure fun) :CLOSURE)
                                           (propagate fun)))))
                              (propagate fun)))))

  ;; Ermitteln der max. statischen Aufruftiefe von Funktionen.  Die
  ;; Berechnung erfolgt 'depth first', d.h. der Wert fuer eine Funktion
  ;; haengt von den Werten der in dieser Funktion lokal definierten
  ;; Funktionen ab. Bei lokalen Funktionen haengt max-level zudem noch von
  ;; den auf gleichem Niveau aufgerufenen Funktionen ab.  Hier wird ein
  ;; 'free-in' benutzt, das eine Funktion f auch dann angibt, wenn in ihr
  ;; eine Funktion g nur indirekt frei vorkommt. Es wuerde reichen, nur
  ;; die direkte Relation zu betrachten, die indirekte Relation schadet
  ;; aber nicht.
  ;;---------------------------------------------------------
  (labels ((setf-deeper-max-level (fun)

             ;; Das Maximum der max-level aus DEEPER-LEVEL-CALLS berechnen.
             ;; Funktionen, die kein 'display' benoetigen, werden dabei
             ;; nicht beruecksichtigt.
             ;;----------------------------------------------------------
             (setf (?max-level fun)
                   (reduce #'(lambda (x deeper-level)
                               (if (needs-display deeper-level)
                                   (max x (1+ (?max-level deeper-level)))
                                   x))
                           (?deeper-level-calls fun)
                           :initial-value 0)))

           (process (funs)
             (dolist (fun funs)
               (process (?local-funs fun)) ; depth first !
               (setf-deeper-max-level fun))
             (let ((sorted (sort (collect-if #'needs-display funs)
                                 #'>
                                 :key #'?max-level)))
               (dolist (fun sorted)
                 (dolist (free-in (?free-in fun))
                   (when (and (= (?level fun) (?level free-in))
                              (> (?max-level fun) (?max-level free-in)))
                     (setf (?max-level free-in) (?max-level fun)))))))

           (needs-display (local-fun)
             (not (or (eq (?closure local-fun) :CLOSURE)
                      (?as-global-fun local-fun)))))

    ;; lokale Funktionen bearbeiten
    ;;-----------------------------
    (process (?local-funs fun))

    ;; globale Funktionen bearbeiten
    ;;------------------------------
    (setf-deeper-max-level fun)))

;;------------------------------------------------------------------------------
;; Die Funktion f wird auf alle lokalen Funktionen von fun angewendet.
;; Es wird zugesichert, dass vor der Bearbeitung einer Funktion g alle in g
;; lokal definierten Funktionen bearbeitet wurden.
;;------------------------------------------------------------------------------
(defun process-local-funs (fun f)
  (labels ((process (funs)
             (dolist (fun funs)
               (process (?local-funs fun))
               (funcall f fun))))
    (process (?local-funs fun))))

;;------------------------------------------------------------------------------
(defun p3-fun-def (fun)
  (let ((*DOWNWARD-FUNARG* nil)
        (*CURRENT-FUN* fun)
        (*LOCAL-FUNS* ()))

    (setf (?deeper-level-calls fun) ())

    (p3-params (?params fun))

    (let ((*MV-POSITION*  t)
          (*FN-ON-MV-POS* nil)
          (*MV-SPEC*      nil)
          (*MV-CALLS*     ()))
      (p3-form (?body fun))

      (setf (?mv-spec fun) *MV-SPEC*)
      (setf (?mv-calls fun) *MV-CALLS*)

      (dolist (fun-which-is-called *MV-CALLS*)
        (pushnew fun (?mv-called-by fun-which-is-called))))

    (setf (?local-funs fun) *LOCAL-FUNS*)

    ;; Propagieren der FREIEN lokalen Funktionen und lexikalischen Variablen.
    ;;-----------------------------------------------------------------------
    (dolist (local-fun *LOCAL-FUNS*)
      (mapc #'p3-check-if-free-local-fun (?free-local-funs local-fun))
      (mapc #'p3-check-if-free-lex-var   (?free-lex-vars   local-fun)))))

;;------------------------------------------------------------------------------
(defun p3-params (params)
  (p3-set-var-level (?all-vars params))
  (dolist (opt (?opt-list params))
    (p3-arg (?init opt)))
  (dolist (key (?key-list params))
    (p3-arg (?init key))))

;;------------------------------------------------------------------------------
;; Funktionsaufruf
;;------------------------------------------------------------------------------
(defmethod p3-form ((app app))
  (setf (?mv-used app) *MV-POSITION*)
  (p3-app (?form app) (?arg-list app) app))

;;------------------------------------------------------------------------------
(defmethod p3-app ((fun defined-fun) args app)
  (declare (ignore args))

  ;; Merken, wenn der Aufruf fuer die Ermittlung von MV-SPEC benoetigt wird.
  ;;------------------------------------------------------------------------
  (when *MV-POSITION*
    (pushnew fun *MV-CALLS*))
  (p3-set-fn-pos-mv-spec t)

  (when (local-fun-p fun)

    ;; Handelt es sich um ein freies Vorkommen der lokalen Funktion ?
    ;;---------------------------------------------------------------
    (p3-check-if-free-local-fun fun)

    (p3-check-level-of-call fun))

  (p3-down-args app))

;;------------------------------------------------------------------------------
(defmethod p3-app ((fun special-sys-fun) args app)
  (if (?pass3 fun)
      (funcall (?pass3 fun) args app)
      (call-next-method)))

;;------------------------------------------------------------------------------
(defmethod p3-app ((fun imported-fun) args app)
  (declare (ignore args))
  (p3-set-mv-spec-if-necessary (?mv-spec fun))
  (p3-set-fn-pos-mv-spec t)
  (p3-down-args app))

;;------------------------------------------------------------------------------
;; Aufruf einer Foreign-Function
;;------------------------------------------------------------------------------
(defmethod p3-app ((fun foreign-fun) args app)
  (p3-set-mv-spec-if-necessary 1) ;oder nil fuer ffi:void
  (p3-set-fn-pos-mv-spec t)
  (p3-args args)
  (setf (?downfun-list app) ()))


;;------------------------------------------------------------------------------
;; Aufruf einer berechneten Funktion
;;------------------------------------------------------------------------------
(defmethod p3-app ((fun form) args app)
  (p3-set-fn-pos-mv-spec t)
  (let ((*MV-POSITION*  NIL)
        (*FN-ON-MV-POS* *MV-POSITION*))
    (p3-form fun))
  (p3-args args)
  (setf (?downfun-list app) ()))

;;------------------------------------------------------------------------------
;; funktionales Argument
;;------------------------------------------------------------------------------
(defmethod p3-form ((fun global-fun))
  (setf (?closure fun) T)

  (p3-set-mv-spec-if-necessary 1)       ; liefert EINEN Wert
  (when *FN-ON-MV-POS* (pushnew fun *MV-CALLS*)))

;;------------------------------------------------------------------------------
(defmethod p3-form ((fun imported-fun))
  (setf (?closure fun) T)

  (p3-set-mv-spec-if-necessary 1)       ; liefert EINEN Wert
  (p3-set-fn-pos-mv-spec (?mv-spec fun)))

;;------------------------------------------------------------------------------
(defmethod p3-form ((fun local-fun))

  (p3-set-mv-spec-if-necessary 1)       ; liefert EINEN Wert
  (when *FN-ON-MV-POS* (pushnew fun *MV-CALLS*))

  (p3-check-if-free-local-fun fun)
  (p3-check-level-of-call fun)

  (cond

    ;; Wird das Ergebnis als Downward-Funarg benutzt ?
    ;;------------------------------------------------
    (*DOWNWARD-FUNARG*

     ;; Falls von der lokalen Funktion noch keine Closure gebildet
     ;; wurde, Funktion als DOWNFUN kennzeichnen.
     ;;------------------------------------------
     (when (null (?closure fun))
       (setf (?closure fun) :DOWNFUN))

     ;; Fun in die Funktionsapplikation eintragen.
     ;;------------------------------------------------
     (add-q fun *DOWNFUNS*))

    ;; Falls es kein Downward-Funarg ist, als CLOSURE kennzeichnen
    ;;------------------------------------------------------------
    (T (setf (?closure fun) :CLOSURE))))

;;------------------------------------------------------------------------------
;; progn
;;------------------------------------------------------------------------------
(defmethod p3-form ((form progn-form))
  (mapc-progn-form-list (?form-list form) #'p3-arg #'p3-form))

;;------------------------------------------------------------------------------
;; Literale, also (), numbers, characters, symbole
;;------------------------------------------------------------------------------
(defmethod p3-form ((an-literal literal))
  (p3-atom an-literal))

;;------------------------------------------------------------------------------
;; Strukturierte Literale
;;------------------------------------------------------------------------------
(defmethod p3-form ((obj structured-literal))
  (call-next-method)
  (add-q obj *CONST-LIST*))

;;------------------------------------------------------------------------------
;; Klasse
;;------------------------------------------------------------------------------
(defmethod p3-form ((form class-def))
  (dolist (slot (?slot-descr-list form)) 
    (when (global-fun-p (?initform slot)) (p3-form (?initform slot))))
  (p3-atom form))

;;------------------------------------------------------------------------------
(defun p3-atom (form)
  (p3-set-mv-spec-if-necessary 1)
  (when *FN-ON-MV-POS*
    (clicc-warning "The constant ~a is used as a functional argument." form)))

;;------------------------------------------------------------------------------
;; let*
;;------------------------------------------------------------------------------
(defmethod p3-form ((form let*-form))
  (p3-set-var-level (?var-list form))
  (p3-args (?init-list form))

  ;; Da der Ausdruck eine neue Umgebung anlegt,
  ;; kann das Ergebnis kein DOWNWARD-FUNARG sein.
  ;;---------------------------------------------
  (let ((*DOWNWARD-FUNARG* NIL))
    (p3-form (?body form))))

;;------------------------------------------------------------------------------
;; mv-lambda
;;------------------------------------------------------------------------------
(defmethod p3-form ((form mv-lambda))
  (let ((*MV-POSITION*     t)
        (*FN-ON-MV-POS*    nil)
        (*MV-SPEC*         nil)
        (*MV-CALLS*        ())
        (*DOWNWARD-FUNARG* NIL))
    (p3-form (?arg form))
    (setf (?mv-spec form) *MV-SPEC*)
    (setf (?mv-calls form) *MV-CALLS*)
    (push form *MV-LAMBDAS*))

  (p3-params (?params form))

  ;; Da der Ausdruck eine neue Umgebung anlegt,
  ;; kann das Ergebnis kein DOWNWARD-FUNARG sein.
  ;;---------------------------------------------
  (let ((*DOWNWARD-FUNARG* NIL))
    (p3-form (?body form))))

;;------------------------------------------------------------------------------
;; Variablen Referenz
;;------------------------------------------------------------------------------
(defmethod p3-form ((form var-ref))
  (p3-set-mv-spec-if-necessary 1)
  (p3-set-fn-pos-mv-spec t)

  (setq form (?var form))
  (if (local-static-p form)
    (p3-check-if-free-lex-var form)))

;;------------------------------------------------------------------------------
;; Referenz einer Named Constant
;;------------------------------------------------------------------------------
(defmethod p3-form ((form named-const))
  (p3-set-mv-spec-if-necessary 1)
  (p3-set-fn-pos-mv-spec t))

;;------------------------------------------------------------------------------
;; setq
;;------------------------------------------------------------------------------
(defmethod p3-form ((form setq-form))
  (p3-set-mv-spec-if-necessary 1)       ; liefert EINEN Wert

  (let ((var (?var (?location form))))
    (when (local-static-p var)
      (p3-check-if-free-lex-var var))

    ;; *FN-ON-MV-POS* wird ggf. von (?form form) bestimmt, also
    ;; *FN-ON-MV-POS* nicht an nil binden !
    ;;------------------------------------------------------------
    (let ((*MV-POSITION* NIL)

          ;; 'form' darf nicht mehr als DOWNWARD-FUNARG
          ;; gekennzeichnet werden, da durch die Zuweisung
          ;; die Closure aus der aktuellen Umgebung
          ;; herausgereicht werden koennte.
          ;;-------------------------------
          (*DOWNWARD-FUNARG* NIL))

      (p3-form (?form form)))))

;;------------------------------------------------------------------------------
;; labels
;;------------------------------------------------------------------------------
(defmethod p3-form ((form labels-form))
  (let ((fun-list (?fun-list form))
         (body (?body form)))
    
    (dolist (fun fun-list)
      (setf (?as-global-fun fun) t)
      (setf (?free-local-funs fun) ())
      (setf (?free-lex-vars   fun) ())
      (setf (?mv-called-by fun) ())
      (setf (?free-in fun) ()))
    
    (setq *LOCAL-FUNS* (append fun-list *LOCAL-FUNS*))
    (let ((*LEVEL* (1+ *LEVEL*)))

      ;; Jeder lokalen Funktion wird ein statisches Niveau von N + 1 zugeordnet.
      ;;------------------------------------------------------------------------
      (mapc #'(lambda (fun)
                (setf (?level fun) *LEVEL*))
            fun-list)

      ;; Analysieren der lokalen Funktionen
      ;;-----------------------------------
      (mapc #'p3-fun-def fun-list))

    ;; Das Ergebnis von labels wird aus dem Bindungsbereich der hierdrin
    ;; definierten Funktionen herausgereicht. Hier waere eine weitergehende
    ;; Analyse notwendig, um festzustellen, dass ein Abschluss, der herausge-
    ;; reicht wird, trotzdem ein DOWNWARD-FUNARG ist.
    ;;-----------------------------------------------
    (let ((*DOWNWARD-FUNARG* NIL))
      (p3-form body))))

;;------------------------------------------------------------------------------
;; if
;;------------------------------------------------------------------------------
(defmethod p3-form ((form if-form))
  (p3-arg  (?pred form))
  (p3-form (?then form))
  (p3-form (?else form)))

;;------------------------------------------------------------------------------
;; let/cc
;;------------------------------------------------------------------------------
(defmethod p3-form ((form let/cc-form))
  (let ((cont (?cont form)))

    (push cont *CONTINUATIONS*)
    (setf (?level cont) *LEVEL*)
    (setf (?only-local cont) t)
    (setf (?mv-position  cont) *MV-POSITION*)
    (setf (?fn-on-mv-pos cont) *FN-ON-MV-POS*)
    (setf (?mv-spec cont) nil)
    (setf (?mv-calls cont) ())

    (let ((*DOWNWARD-FUNARG* NIL)       ; Anmerkung siehe LABELS/FLET
          (*MV-SPEC*         nil)
          (*MV-CALLS*         ()))
      (p3-form (?body form))

      ;; Die Information ueber den Rumpf mit den kombinierten Informationen
      ;; der Anwendungen der Continuation kombinieren.
      ;;-----------------------------------------------
      (setf (?mv-spec cont) (p3-combine-mv-spec (?mv-spec cont) *MV-SPEC*))
      (setf (?mv-calls cont) (union *MV-CALLS* (?mv-calls cont))))

    (p3-set-mv-spec-if-necessary (?mv-spec cont))
    (p3-set-fn-pos-mv-spec (?mv-spec cont))
    (setq *MV-CALLS* (union *MV-CALLS* (?mv-calls cont)))))

;;------------------------------------------------------------------------------
;; Angewandtes Vorkommen einer Continuation
;;------------------------------------------------------------------------------
(defmethod p3-form ((form cont))
  (setf (?mv-spec form) t)
  (p3-atom form))

;;------------------------------------------------------------------------------
;; Aufruf einer Continuation
;;------------------------------------------------------------------------------
(defmethod p3-app ((cont cont) args app)
  (unless (eql (length args) 1)
    (internal-error 'p3-main
                    "continuation should be called with exactly one argument"))

  ;; Sprung aus einer Funktion heraus ?
  ;;-----------------------------------
  (when (> *LEVEL* (?level cont))
    (setf (?only-local cont) nil)

    ;; Bei Closurebildung muss die statisch gebundene Cont. beachtet werden
    ;;---------------------------------------------------------------------
    (p3-check-if-free-lex-var cont))

  ;; Die MV-Spezifikation des Sprungs ist relevant fuer die MV-Spezifikation
  ;; des Let/cc.
  ;;---------------------------------
  (let ((*DOWNWARD-FUNARG* nil)         ; siehe auch LABELS/FLET
        (*MV-POSITION*     (?mv-position  cont))
        (*FN-ON-MV-POS*    (?fn-on-mv-pos cont))
        (*MV-SPEC*         nil)
        (*MV-CALLS*        ()))
    (p3-form (first args))
    (setf (?downfun-list app) ())

    ;; Die Continuation wurde evtl. schon an anderer Stelle angewendet.
    ;; Deshalb die hier gewonnene Information mit der schon vorhandenen
    ;; kombinieren.
    ;;-----------------------------------------------------------------
    (setf (?mv-spec cont) (p3-combine-mv-spec (?mv-spec cont) *MV-SPEC*))
    (setf (?mv-calls cont) (union (?mv-calls cont) *MV-CALLS*)))

  ;; Sprung kann auf Ergebnisposition einer lokalen Funktion stehen.
  ;; In diesem Fall wird als MV-Spezifikation :JUMP eingetragen.
  ;;------------------------------------------------------------
  (p3-set-mv-spec-if-necessary :JUMP))

;;------------------------------------------------------------------------------
;; tagbody
;;------------------------------------------------------------------------------
(defmethod p3-form ((form tagbody-form))
  (setf (?level form) *LEVEL*)
  (p3-arg (?first-form form))
  (mapc-progn-form-list (?tagged-form-list form)
                        #'(lambda (tagged-form) (p3-arg (?form tagged-form)))
                        #'(lambda (tagged-form) (p3-form (?form tagged-form)))))

;;------------------------------------------------------------------------------
;; go (d.h. angewandtes Vorkommen von tagged-form)
;;------------------------------------------------------------------------------
(defmethod p3-form ((form tagged-form))

  ;; GO kann auf Ergebnisposition einer lokalen Funktion stehen.
  ;; In diesem Fall wird als MV-Spezifikation :JUMP eingetragen.
  ;;------------------------------------------------------------
  (p3-set-mv-spec-if-necessary :JUMP)
  (p3-set-fn-pos-mv-spec :JUMP))

;;------------------------------------------------------------------------------
;; Spezielle Optimierungen von Systemfunktionen
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; PROGV-INTERNAL symbols values body
;;------------------------------------------------------------------------------
(defun p3-progv (args app)
  (let ((symbols (pop args))
        (values (pop args))
        (body (pop args)))

    (p3-arg symbols)
    (p3-arg values)
    (p3-set-fn-pos-mv-spec t)
    (let ((*DOWNWARD-FUNARG* T)
          (*DOWNFUNS* (empty-queue))
          (*MV-POSITION*     NIL)
          (*FN-ON-MV-POS* *MV-POSITION*))
      (p3-form body)
      (setf (?downfun-list app) (queue2list *DOWNFUNS*)))))

;;------------------------------------------------------------------------------
;; CATCH-INTERNAL tag body
;;------------------------------------------------------------------------------
(defun p3-catch (args app)

  ;; Da das CATCH durch ein THROW dynamisch verlassen werden kann,
  ;; ist die MV-Spezifikation unbekannt.
  ;;-----------------------------------------
  (p3-set-mv-spec-if-necessary T)
  (p3-set-fn-pos-mv-spec t)

  (let ((tag (pop args))
        (funarg (pop args)))

    (p3-arg tag)
    (let ((*DOWNWARD-FUNARG*  T)
          (*DOWNFUNS* (empty-queue))
          (*MV-POSITION*     NIL)       ; keine MV, sondern 1 Funktion
          (*FN-ON-MV-POS*    NIL))
      (p3-form funarg)
      (setf (?downfun-list app) (queue2list *DOWNFUNS*)))))

;;------------------------------------------------------------------------------
;; UNWIND-PROTECT-INTERNAL protected-form cleanup-form
;;------------------------------------------------------------------------------
(defun p3-unwind-protect (args app)
  (let ((protected-form (pop args))
        (cleanup-form (pop args))
        (*DOWNWARD-FUNARG*  T)
        (*DOWNFUNS* (empty-queue)))     ; Liste der Downward Funargs

    ;; Die MV-Spezifikation fuer UNWIND-PROTECT ergibt sich
    ;; aus der MV-Spezifikation der Protected-Form.
    ;;---------------------------------------------
    (p3-set-fn-pos-mv-spec t)
    (let ((*MV-POSITION*  NIL)
          (*FN-ON-MV-POS* *MV-POSITION*))
      (p3-form protected-form))
    (let ((*MV-POSITION*  NIL)
          (*FN-ON-MV-POS* NIL))
      (p3-form cleanup-form))           ; nicht p3-arg, da evtl. downfun
    (setf (?downfun-list app) (queue2list *DOWNFUNS*))))

;;------------------------------------------------------------------------------
;; APPLY fn args
;;------------------------------------------------------------------------------
(defun p3-apply (args app)
  (p3-set-fn-pos-mv-spec t)
  (let ((*DOWNWARD-FUNARG*  T)
        (*DOWNFUNS* (empty-queue))
        (*MV-POSITION*  NIL)
        (*FN-ON-MV-POS* *MV-POSITION*))
    (p3-form (first args))
    (setf (?downfun-list app) (queue2list *DOWNFUNS*)))
  (p3-args (rest args)))

;;------------------------------------------------------------------------------
;; VALUES args
;;------------------------------------------------------------------------------
(defun p3-values (args app)
  (p3-set-mv-spec-if-necessary (length args))
  (p3-down-args app))


;;------------------------------------------------------------------------------
;; Ermittelt die kleinste obere Schranke zweier Elemente aus dem unten
;; angegbenen Verband.
;;
;;      t     = Anzahl der Werte kann erst zur Laufzeit bestimmt werden
;;   /    \
;;  0 1 2 ... = Anzahl der Werte ist n
;;   \    /
;;    JUMP    = Ausdruck wird durch einen Sprung verlassen oder terminiert nicht
;;      |
;;     nil    = Anzahl der Werte ist noch nicht bestimmt
;;
;;------------------------------------------------------------------------------
(defun p3-combine-mv-spec (old-mv-spec new-mv-spec)
  (cond
    ((eq old-mv-spec nil) new-mv-spec)
    ((eq new-mv-spec nil) old-mv-spec)
    ((eq old-mv-spec :JUMP) new-mv-spec)
    ((eq new-mv-spec :JUMP) old-mv-spec)
    ((eql old-mv-spec new-mv-spec) old-mv-spec)
    (t t)))
    
;;------------------------------------------------------------------------------
;; Setzen die MV-Spezifikation (falls noetig)
;;------------------------------------------------------------------------------
(defun p3-set-fn-pos-mv-spec (mv-spec)
  (when *FN-ON-MV-POS*
    (setq *MV-SPEC* (p3-combine-mv-spec *MV-SPEC* mv-spec))))

;;------------------------------------------------------------------------------
(defun p3-set-mv-spec-if-necessary (mv-spec)
  (when *MV-POSITION*
    (setq *MV-SPEC* (p3-combine-mv-spec *MV-SPEC* mv-spec))))

;;------------------------------------------------------------------------------
;; Bestimmt die multiplen Werte der definierten Funktionen, indem durch ein
;; Fixpunktverfahren die lokal bestimmten Werte von mv-spec entlang der
;; Kanten mv-called-by propagiert werden.
;; Anschlie"send wird diese L"osung in die Continuations und MV-Lambdas
;; "ubertragen. 
;;------------------------------------------------------------------------------
(defun p3-analyse-mv-spec ()
  (labels ((update-mv (fun newspec)
             (let* ((oldspec (?mv-spec fun))
                    (spec oldspec))
               (setq spec (p3-combine-mv-spec spec newspec))
               (unless (eql spec oldspec)
                 (setf (?mv-spec fun) spec)
                 (dolist (called-by (?mv-called-by fun))
                   (update-mv called-by spec)))))
           (determine-mv (fun-list)
             (dolist (fun fun-list)
               (let ((spec (?mv-spec fun)))
                 (dolist (called-by (?mv-called-by fun))
                   (update-mv called-by spec)))
               (determine-mv (?local-funs fun)))))

    ;; Bestimmen des Fixpunktes im Aufrufgraphen der definierten Funktionen
    ;;---------------------------------------------------------------------
    (determine-mv (?all-global-funs *module*)))

  (labels ((combine-mv-calls (fun)
             (let ((mv-spec (?mv-spec fun)))
               (dolist (called (?mv-calls fun))
                 (setq mv-spec
                       (p3-combine-mv-spec mv-spec (?mv-spec called))))
               (setf (?mv-spec fun) mv-spec))))

    ;; Eintragen der MV-Spezifikationen in die Continuations
    ;;------------------------------------------------------
    (dolist (cont *CONTINUATIONS*)
      (combine-mv-calls cont))

    ;; Eintragen der MV-Spezifikationen in die mv-lambda Ausdruecke
    ;;-------------------------------------------------------------
    (dolist (form *MV-LAMBDAS*)
      (combine-mv-calls form))))

;;------------------------------------------------------------------------------
;; Hilfsfunktionen
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Analysiert die Argumente eines Funktionsaufrufes.
;;------------------------------------------------------------------------------
(defun p3-arg (arg)
  (let ((*MV-POSITION*     NIL)         ; Argumente sind NICHT ergebnisrelevant.
        (*FN-ON-MV-POS*    NIL)
        (*DOWNWARD-FUNARG* NIL))        ; Argumente koennen hier nicht als
                                        ; DOWNWARD-FUNARG erkannt werden.
    (p3-form arg)))

;;------------------------------------------------------------------------------
(defun p3-args (args)
  (mapc #'p3-arg args))

;;------------------------------------------------------------------------------
;; Analysiert Argumente einer Applikation und traegt die Downward Funargs in
;; die downfun-list der Applikation ein.
;;------------------------------------------------------------------------------
(defun p3-down-args (app)
  (let* ((fun (?form app))
         (args (?arg-list app))
         (*DOWNFUNS* (empty-queue))
         (downward-args (?downward-args fun))
         (i 0))
    (dolist (arg args)
      (if (eql i (car downward-args))
          (let ((*MV-POSITION*  NIL)
                (*FN-ON-MV-POS* NIL)
                (*DOWNWARD-FUNARG*  T))
            (pop downward-args)
            (p3-form arg))
          (p3-arg arg))
      (incf i))
    (setf (?downfun-list app) (queue2list *DOWNFUNS*))))

;;------------------------------------------------------------------------------
;; Setzt bei statisch gebundenen Variablen das statische Niveau.
;;------------------------------------------------------------------------------
(defun p3-set-var-level (var-list)
  (mapc #'(lambda (var)
            (when (local-static-p var)
              (setf (?level var) *LEVEL*)))
        var-list))

;;------------------------------------------------------------------------------
;; Wenn 'var' frei in der momentan uebersetzten Funktion vorkommt, wird 'var'
;; unter 'free-lex-vars' vermerkt.
;;------------------------------------------------------------------------------
(defun p3-check-if-free-lex-var (var)
  (when (< (?level var) *LEVEL*)
    (pushnew var (?free-lex-vars *CURRENT-FUN*))))

;;------------------------------------------------------------------------------
;; Prueft, ob es sich um einen DEEPER-LEVEL-CALL handelt.
;; (ein Funktionsaufruf einer im Rumpf der aktuellen Funktion
;; lokal definierten Funktion)
;;------------------------------------------------------------------------------
(defun p3-check-level-of-call (fun)
  (when (> (?level fun) *LEVEL*)
      (pushnew fun (?deeper-level-calls *CURRENT-FUN*))))

;;------------------------------------------------------------------------------
;; Wenn 'local-fun' frei in der momentan uebersetzten Funktion vorkommt, wird
;; 'local-fun' unter 'free-local-funs' vermerkt.  In 'local-fun' wird die
;; aktuelle Funktion in der Annotation 'free-in' vermerkt.  Eine (lokale)
;; Funktion gilt als nicht frei in sich selbst.
;;------------------------------------------------------------------------------
(defun p3-check-if-free-local-fun (local-fun)
  (when (and (<= (?level local-fun) *LEVEL*)
             (not (eq local-fun *CURRENT-FUN*)))
    (pushnew local-fun (?free-local-funs *CURRENT-FUN*))
    (pushnew *CURRENT-FUN* (?free-in local-fun))))

;;------------------------------------------------------------------------------
(provide "p3main")
