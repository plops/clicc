;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Optimierungen der Let*-Ausdruecke.
;;;
;;; $Revision: 1.43 $
;;; $Log: optimize.lisp,v $
;;; Revision 1.43  1993/11/25  17:10:42  atr
;;; Bei delete-unref-vars werden dynamische Variablen nicht eliminiert.
;;; (let ((loc init-form)) (setq var loc)) --> (setq var init-form).
;;;
;;; Revision 1.42  1993/11/25  16:31:16  atr
;;; Dynamische Variablen werden nicht substituiert.
;;;
;;; Revision 1.41  1993/11/09  16:40:58  atr
;;; Die globalen Variablen stehen jetzt in se-init.lisp.
;;;
;;; Revision 1.40  1993/10/28  08:24:30  kl
;;; has-no-side-effect ersetzt.
;;;
;;; Revision 1.39  1993/10/25  10:09:05  atr
;;; Die Behandlung der Applikationen mit spec-vars korrigiert. Bei
;;; Effect-of-form dieser Applikationen werden die Argumente genau
;;; analysiert.
;;;
;;; Optimize-1 verbessert, komplizierte Ausdr"ucke werden nicht innerhalf
;;; von Schleifen substituiert.
;;;
;;; Revision 1.38  1993/10/22  11:02:29  atr
;;; Fehler in effect-of-form (app) korrigiert.
;;;
;;; Revision 1.37  1993/10/21  10:41:12  atr
;;; Die "Anderungen der letzten Version r"uckg"angig gemacht, und noch
;;; Kommentare hinzugef"ugt.
;;;
;;; Revision 1.36  1993/10/19  16:21:24  atr
;;; Bei effect-of-form von dem LABELS-Konstrukt wird nur der Seiteneffekt
;;; des Rumpfes berechnet. Die Seiteneffekte der lokalen
;;; Funktionensdefinitionen werden selbstverst"andlich nicht mitgerechnet.
;;;
;;; Revision 1.35  1993/10/19  14:29:08  atr
;;; Bei effect-of-form werden jetzt die PARAMS richtig analysiert.
;;;
;;; Revision 1.34  1993/10/19  13:57:31  hk
;;; Fehler in is-side-effect-free behoben.
;;;
;;; Revision 1.33  1993/10/18  20:28:57  atr
;;; Einen Fehler bei effect-of-form f"ue App korrigiert. Dieser Fehler
;;; f"uhrte dazu da"s, bei Testmain einen Ausdruck mit Seiteneffekten auch
;;; entfernt wurde.
;;;
;;; Revision 1.32  1993/10/14  08:56:11  hk
;;; Fehler in let-optimize-params behoben.
;;;
;;; Revision 1.31  1993/10/13  17:05:16  atr
;;; Die Optimierung der Parameterliste einer Funktion oder einer mv-lambda
;;; etwas verbessert.
;;;
;;; Revision 1.30  1993/10/13  14:57:30  atr
;;; Eine unn"otige Meldung, die ich zum Testen eingef"ugt habe, wieder
;;; entfernt.
;;;
;;; Revision 1.29  1993/10/13  14:41:04  atr
;;; Methoden f"ur CONT eingef"ugt, weil CONT keine Unterklasse von FORM
;;; ist.
;;;
;;; Revision 1.28  1993/10/13  11:33:46  atr
;;; Zwei Tippfehler (gerade bei einer kleinen "anderung) korrigiert.
;;;
;;; Revision 1.27  1993/10/13  11:26:22  atr
;;; Die Funktion subst-var ge"andert, die uneffizienten (append ...) durch
;;; Iteration (mapcar #'....) ersetzt.
;;; (any-thing-else T) --> (any-thing-else form)
;;;
;;; Revision 1.26  1993/10/12  18:00:18  atr
;;; Ein Tippfehler korrigiert.
;;;
;;; Revision 1.25  1993/10/11  17:36:14  hk
;;; (clicc-message "~s wegoptimiert" *neu*) gestrichen.
;;;
;;; Revision 1.24  1993/10/11  16:32:14  atr
;;; Meldungen Let-optimizing function ... wieder abgeschaltet.
;;;
;;; Revision 1.23  1993/10/11  15:33:19  atr
;;; Die Variable *neu*, die nur f"ur mich lokal zur Statistik eingef"ugt
;;; wurde, ist jetzt entfernt.
;;;
;;; Revision 1.22  1993/10/11  15:25:23  atr
;;; In Funktion optimize-1 in dem letzten Fall noch die Bedingung
;;; (not-destructive *let-effect*) eingef"ugt, die "uberpr"uft da"s, im
;;; Rumpf der Let-form keine destruktiven Operationen stattfinden.
;;;
;;; Revision 1.21  1993/10/09  18:36:21  atr
;;; Die Let-Optimierungen nehmen jetzt r"uchsicht auf die Spr"unge, die
;;; bei der Seiteneffektanalyse mit :JUMP bezeichnet werden.
;;; Jetzt werden auch die Let-Ausdr"ucke , deren R"umpfe aus einer
;;; Zuweisung bestehen auch optimiert.
;;;
;;; Revision 1.20  1993/09/14  13:21:49  atr
;;; Die Funktion optimize-0 liefert nicht mehr eine ueberfluessige
;;; Progn-form, sondern den Rumpf der Let-form.
;;;
;;; Revision 1.19  1993/09/06  15:12:08  atr
;;; Die Deklaration der Variablen *current-function* steht jetzt in
;;; static-analysis
;;;
;;; Revision 1.18  1993/08/25  14:31:56  atr
;;; Fehler bei is-side-effect-free behoben.
;;;
;;; Revision 1.17  1993/08/25  14:07:38  atr
;;; Eine neue Funktion is-side-effect-free implementiert,
;;; die bei der Verbesserung der Progn-forms benutzt wird.
;;;
;;; Revision 1.16  1993/08/11  17:32:11  atr
;;; Der richtige Stand wieder gerettet.
;;;
;;; Revision 1.15  1993/07/27  13:54:37  atr
;;; Optimierungen an die neue Seiteneffektanalyse angepasst.
;;;
;;; Revision 1.14  1993/07/14  10:47:16  atr
;;; Keliner Fehler bei update-let korrigiert.
;;;
;;; Revision 1.13  1993/07/13  16:28:24  atr
;;; Fehler bei optimize-let von Switch-form korrigiert.
;;;
;;; Revision 1.12  1993/07/06  14:26:46  atr
;;; Supplied Parameter entfernt aus der Parameterliste
;;; von Effect-of-form.
;;; *vars-bound-but-not-referenced* ist jetzt mit defvar anstatt mit
;;; declare (special ...) deklariert.
;;;
;;; Revision 1.11  1993/07/06  13:20:23  atr
;;; Tippfehler subst var --> subst-var korrigiert und
;;; Fehler bei subst-var in defined-fun korrigiert.
;;;
;;; Revision 1.10  1993/07/02  10:22:48  atr
;;; Fehler bei subst-with-app-permissible korrigiert.
;;;
;;; Revision 1.9  1993/07/01  16:03:18  atr
;;; Fehler bei effect-of-form von der switch-form korrigiert.
;;;
;;; Revision 1.8  1993/06/30  16:26:19  atr
;;; Unnoetige Ausgabe entfernt.
;;;
;;; Revision 1.7  1993/06/30  16:00:06  atr
;;; (defvar *let-effect* (empty-effect)) --> (defvar *let-effect*).
;;;
;;; Revision 1.6  1993/06/30  15:45:30  atr
;;; (require static-effect) entfernt.
;;;
;;; Revision 1.5  1993/06/29  23:25:23  atr
;;; Fehler bei der Substitution in den INIT-FORMEN der analysierten
;;; let-form korrigiert.
;;;
;;; Revision 1.4  1993/06/27  15:28:31  atr
;;; Erste ablauffaehige Version der Optimierungen
;;; der Let-ausdruecke.
;;;
;;; Revision 1.3  1993/06/17  08:18:10  hk
;;; Revision und Log eingefuegt.
;;;
;;; revision 1.2 1993/06/17 08:00:09 hk
;;; Copright Notiz eingefuegt
;;; 
;;; revision 1.1 1993/05/10 15:18:50 atr
;;; Initial revision
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;(defvar *eliminated-vars* 0)
;;(defvar *eliminated-lets* 0)
;;(defvar *subst-number*    0)
;;(defvar *let-effect* )
;;(defvar *local-effect*)
;;(defvar *vars-bound-but-not-used* 0)


(defun let-optimizing ()
  (let ((*eliminated-vars* 0)
        (*eliminated-lets*  0)
        (*vars-bound-but-not-used* 0)
        (*subst-number*  0  ))
    (clicc-message "Optimizing the let-forms ...")
    (clicc-message "----------------------------")
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
  (let* ((*current-function* fun)
         (*static-level* (if (global-fun-p fun)
                             0
                             (?level fun))))
    (setf (?body fun) (optimize-let (?body fun)))
    fun))

;;;---------------------------------------------------------------------
;;; Optimize-let Methoden traversiert die Zwischensprache, und 
;;; optimiert die Let-ausdruecke.
;;;---------------------------------------------------------------------

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
    (setf (?initform one-slot-desc)
          (optimize-let (?initform one-slot-desc))))
  a-class-def)


(defmethod optimize-let ((a-slot-descr slot-desc))
  (setf (?initform a-slot-descr) (optimize-let (?initform a-slot-descr)))
  a-slot-descr)

(defmethod optimize-let ((a-app app))
  (setf (?arg-list a-app) 
        (mapcar #'optimize-let (?arg-list a-app)))
  a-app)

(defun let-optimize-opt/key (opt/key)
  (setf (?init opt/key) (optimize-let (?init opt/key)))
  opt/key)

(defun  let-optimize-params(parameter)
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
  (setf (?body     a-labels) (optimize-let (?body     a-labels)))
  a-labels)

(defmethod optimize-let ((a-defined-fun defined-fun))
  
  a-defined-fun)

(defmethod optimize-let ((a-let-form let*-form))
  (let ((init-list      (?init-list a-let-form))
        (var-list       (?var-list  a-let-form))
        (*let-effect*   (empty-effect))
        (*local-effect* (make-instance 'local-effect)))
    
    ;; Jetzt werden zunaechst die Init-formen untersucht,
    ;; und eventuell let-formen optimiert.
    ;;---------------------------------------------------
    (setf (?init-list a-let-form) 
          (mapcar #'optimize-let (?init-list a-let-form)))
    
    ;;Nun werden die Let-formen in dem  Rumpf der Let-form optimiert.
    ;;---------------------------------------------------------------
    (setf (?body a-let-form) (optimize-let (?body a-let-form)))
    
    ;; Hier passiert die Optimierung dieses Let-konstrukts.
    ;;-----------------------------------------------------
    (effect-of-form a-let-form *let-effect* *local-effect*)
    (if (and (null var-list)
             (null init-list))
        
        ;; (Let*  ()() (form1 ... formN)) --> (progn form1... formN)
        ;;----------------------------------------------------------
        (progn (setq *eliminated-lets* (1+ *eliminated-lets*))
               (setq a-let-form (optimize-0 a-let-form)))
        
        ;; Eventuell wird VAR durch INIT-FORM substituiert.
        ;;-------------------------------------------------
        (let (var init-form)
          (dotimes (i  (length (?var-list a-let-form)))
            (setq var        (nth  i (?var-list a-let-form)))
            (setq init-form  (nth  i (?init-list a-let-form)))
            (unless (dynamic-p var)
              (setq a-let-form 
                    (optimize-1 var init-form  a-let-form))))
          
          ;; Wenn eine Variable nicht mehr referenziert ist,
          ;; wird sie und die dazugehoerige Init-form aus 
          ;; var-list bzw aus init-form-list entfernt.
          ;;------------------------------------------------
          (setq a-let-form (delete-unref-var a-let-form))
          (if  (and (null (?var-list  a-let-form))
                    (null (?init-list a-let-form)))
               
               ;; (Let*  ()() (form1 ... formN)) --> (progn form1... formN)
               ;;----------------------------------------------------------
               (progn 
                 (setq *eliminated-lets* (1+ *eliminated-lets*))
                 (setq a-let-form (optimize-0 a-let-form)))
               a-let-form)))
    a-let-form))
(defmethod optimize-let ((a-cont cont))
  a-cont)

(defmethod optimize-let ((any-thing-else form))
  any-thing-else)


(defun optimize-0 (let-form)
  (?body let-form))

;;;-----------------------------------------------------------------------
;;; Optimize-1 untersucht die Init-form einer Variablen, und entscheidet
;;; in Abhaengigkeit der moeglichen Effekte des Rumpfes des Let-
;;; konstrukts, ob eine Substitution aller Vorkommen der Variablen 
;;; durch die Init-form moeglich ist.
;;; Falls nicht alle Vorkommen der Variablen substituiert werden koennen,
;;; wird keine Substitution gemacht.
;;;-----------------------------------------------------------------------

(defun optimize-1 (var init-form let-form)
  
  (when (and (eql 0 (?read var))
             (eql 1 (?write var)))
    (incf *vars-bound-but-not-used*))
  (let ((init-form-effect (empty-effect))
        (init-form-local-effect (make-instance 'local-effect)))
    (cond 
      ;; Simple-contant-p "uberpr"uft ob der Ausdruck dupliziert werden darf
      ;; ohne die EQ-Semantik zu beeiflussen. Ausdr"ucke solcher Art
      ;; k"onnen substituiert werden, auch mehr als einmal, wenn die entspr
      ;; lokale Variable nicht ver"andert wird.
      ;;--------------------------------------------------------------------
      ((simple-constant-p init-form) 
       (if (eql 1 (?write (?var var)))
           (update-let var init-form let-form)
           let-form))
      
      ((var-ref-p init-form)
       
       ;; Subs-with-var-is-permissible "uberpr"uft die Bedingungen, 
       ;; um eine Substitution durchf"uhren zu k"onnen.
       ;;---------------------------------------------------------
       (if (subst-with-var-is-pemissilble var init-form let-form)
           (update-let var init-form let-form)
           let-form))
      ((is-a-single-assignment let-form) 

       ;; Hier wird versucht folgende Substitution durchzuf"uhren:  
       ;; (let ((loc init-form)) (setq var loc)) --> (setq var init-form)
       ;;----------------------------------------------------------------
       
       (update-let var init-form let-form))

      ((and (eql 1  (?write var))
            (eql 1 (?read var)))

       ;; Die lokale Variable ist nur einmal im Rumpf referenziert
       ;;---------------------------------------------------------
       (effect-of-form init-form init-form-effect init-form-local-effect)
       (if (simple-app init-form)
           (if (and (subst-with-app-permissible var init-form let-form)
                    (does-not-jump *let-effect*))
               
               ;; Wenn die Init-form eine Applikation ist, deren Argumente 
               ;; variablen, oder Konstanten sind, dann wird die Substitution
               ;; nur gemacht wenn subst-with-app-permissible wahr ist.
               ;; Die Bedingung (does-not-jump *let-effect*) verhindert eine 
               ;; Substitution in einer LOOP, denn mit einer solchen  
               ;; Substitution wird eine Applikation mehrmals ausgewertet.
               ;;------------------------------------------------------------
               (update-let var init-form let-form)
               let-form)
           
           ;; Nun wenn die Init-form bekannte Seiteneffekte hat,
           ;; dann wird untersucht ob die Substitution machbar ist.
           ;;------------------------------------------------------
           (if (and (not-destructive init-form-effect)
                    (not (fun-p init-form))
                    (does-not-jump init-form-effect)
                    (static-p var)
                    (listp (?read-list init-form-local-effect))
                    (listp (?write-list init-form-local-effect))
                    (listp (?read-list  init-form-effect))
                    (listp (?write-list init-form-effect))
                    (not-destructive *let-effect*)
                    (does-not-jump *let-effect*))
               
               ;; Hier auch wird mit der Bedingung (does-not-jump *let-effect*)
               ;; verhindert, dass ein komplizieter Ausdruck in einer 
               ;; Schleife substituiert wird.
               ;;-------------------------------------------------------------
               (let ((write-region (append 
                                    (?write-list init-form-effect)
                                    (?write-list init-form-local-effect)))
                     (read-region  (append 
                                    (?read-list  init-form-effect)
                                    (?read-list init-form-local-effect))))
                 (if (and 
                      (not (member var write-region :test #'eq))
                      (r/w-interference-free read-region 
                                             (?write-list *let-effect*))
                      (r/w-interference-free read-region
                                             (?write-list *local-effect*))
                      (r/w-interference-free write-region 
                                             (?read-list *let-effect*))
                      (r/w-interference-free write-region 
                                             (?read-list *local-effect*))
                      (not (member-of-region var 
                                             (?write-list *let-effect*)
                                             (?write-list *local-effect*))))
                     
                     
                     (update-let var init-form let-form)
                     let-form))
               let-form)))
      (t  let-form))))

(defun is-a-single-assignment (let-form)
  (and (eql 1 (length (?var-list let-form)))
       (eql 1 (length (?init-list let-form)))
       (setq-form-p (?body let-form))))

;;;------------------------------------------------------------------------
;;; Eine Substitution der Variablen VAR durch die Variable INIT-VAR
;;; ist nur erlaubt wenn :
;;; 1 : VAR wird nicht im Rumpf oder in den init-formen veraedert.
;;; 2 : INIT-VAR wird nichtim Rumpf oder in den init-formen  veraendert.
;;; 3 : init-var wird nicht (im Falle einer dynamischen Variable 
;;;     nochmal gebunden nach dem Sie referenziert ist.
;;;------------------------------------------------------------------------
(defun subst-with-var-is-pemissilble (var init-var let-form)
  (let ((global-written-vars (?write-list *let-effect*))
        (local-written-vars (?write-list *local-effect*)))
    (and 
     (not  (member-of-region var global-written-vars local-written-vars))
     
     (not  (member-of-region (?var init-var) global-written-vars 
                             local-written-vars))
     (not (member (?var init-var) (?var-list let-form) :test #'eq)))))


;;;--------------------------------------------------------------------------
;;; Die READ und WRITE-LISTS sind entweder Listen von Variablen oder
;;; integerzahlen ,die Mengen von Varaiblen kodieren.
;;; member-of-region fragt ab of eine Variable direkt in der Liste 
;;; ist, oder , im Falle einer integerzahl, ob die Variable in der 
;;; durch diese integerzahl kodierte Menge ist.
;;;--------------------------------------------------------------------------
(defun member-of-region (var global-region local-region)
  (or (and (listp global-region)
           (member var global-region :test #'eq))
      (member var local-region :test #'eq)
      (and (integerp global-region)
           (dynamic-p var))
      (and (integerp global-region)
           (static-p var)
           (>= global-region (?level var)))))

;;;------------------------------------------------------------------------
;;; Eine Substitution einer Variablen VAR durch eine Applikation APP
;;; ist erlaubt, wenn die Menge der in APP gelesenen Variablen keinen
;;; Schnitt hat mit der Menge der im Let-konstrukt veraenderten Variablen,
;;; und wenn VAR im Let-konstrukt nicht veraendert wird.
;;;------------------------------------------------------------------------
(defun subst-with-app-permissible (var init-app let-form)
  (let* ((functional (?form    init-app))
         (arg-list   (?arg-list init-app))
         (fun-effect (get-effect functional))
         (read-vars  (mapcar #'?var (remove-if-not #'var-ref-p arg-list))))
    (and (not (member-of-region var (?write-list *let-effect*)
                                (?write-list *local-effect*)))
         (fun-p (?form init-app))
         (only-alloc fun-effect)
         (not-destructive *let-effect*)
         (null (intersection read-vars (?var-list let-form)))
         (let ((global-write-vars (?write-list *let-effect*))
               (local-write-vars  (?write-list *local-effect*)))
           (r/w-interference-free read-vars global-write-vars)
           (r/w-interference-free read-vars local-write-vars)))))


;;;-------------------------------------------------------------------------
;;; r/w-interference-free ist true , wenn keine Variable von region1
;;; enthalten ist in region2, wenn region2 eine Liste ist,
;;; und wenn region2 eine Integer ist dann soll sie echt kleiner
;;; sein als das statische Niveau jeder Variablen in region1 und 
;;; ungekehrt.Wenn beide integerzahlen sind dann sind sie nicht disjunkt.
;;;-------------------------------------------------------------------------

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

;;;----------------------------------------------------------------------------
;;; HILFSFUNKTIONEN ...
;;;----------------------------------------------------------------------------
(defun code-level-of-var (var)
  (if (dynamic-p var)
      -1 
      (?level var)))



(defun get-max-level (var-ref-list)
  (let ((level-list (mapcar #'(lambda (var-ref)
                                (if (dynamic-p (?var var-ref))
                                    -1
                                    (?level (?var var-ref))))
                            var-ref-list)))
    (apply #'max level-list)))



(defun simple-app (form)
  (if (app-p form)
      (and 
       (all-are-side-effect-free (?arg-list form))
       (if (fun-p (?form form))
           (and (not (eq *error-function* (?form form)))
                (only-alloc (get-effect (?form form))))
           nil))))



(defun all-are-side-effect-free (liste)
  (if (endp liste)
      T
      (if (not (may-be-copied (car liste)))
          nil
          (all-are-side-effect-free (cdr liste)))))


(defun only-alloc (effect)
  (and (null (?read-list  effect))
       (null (?write-list effect))
       (not-destructive   effect)
       (does-not-jump       effect)))
  
(defun does-not-jump (effect)
  (not (member (?data-effects effect) '(:jump :alloc-jump :dest-jump 
                                        :alloc-dest-jump))))

;;;-------------------------------------------------------------------------
;;; entscheidet ob eine Form keine Seiteneffekte hat, also auch entfernt
;;; werden kann, wenn sie z.B in einer Progn-form, nicht auf Ergebnisposition
;;; vorkommt.
;;;-------------------------------------------------------------------------
(defun is-side-effect-free (form)
  (let ((global-effect (empty-effect))
        (local-effect  (make-instance 'local-effect)))
    (effect-of-form form global-effect local-effect)

    ;; Eine Form kann entfernt werden wenn sie entweder 
    ;; 1: keine Variablen und keine Daten destruktive veraendert 
    ;;    (lesen von Variablen ist erlaubt)
    ;; 2: Wenn sie Variablen weder liest noch veraedert.
    ;;    (destruktive Effekte sind erlaubt, denn keine Variablen
    ;;     werden gelesen.)
    ;;-----------------------------------------------------------------
    (or (and (only-alloc global-effect)
             (null (?write-list local-effect)))
        (and
         (does-not-jump     global-effect)
         (null (?read-list  global-effect))
         (null (?write-list global-effect))
         (null (?read-list  local-effect))
         (null (?write-list local-effect))))))

;;;-------------------------------------------------------------------------
;;; Diese  Funktion fragen ab, ob eine Form auf dem Heap etwas alloziiert,
;;; also eventuell die "EQ-SEMANTIK" beeiflusst .
;;;-------------------------------------------------------------------------
(defun no-alloc-effect (effect)
  (not (member (?data-effects effect) '(:alloc :allco-dest :alloc-jump 
                                       :alloc-dest-jump) ))) 
;;;------------------------------------------------------------------------
;;;
;;;------------------------------------------------------------------------
;;;-------------------------------------------------------------------------
;;; Diese Funktion fragt ab, ob eine Form dessen Seiteneffekt 'Effect' ist
;;; destruktiv Daten veraendert.
;;;-------------------------------------------------------------------------
(defun not-destructive (effect)
  (not (member (?data-effects effect) '(:dest :alloc-dest :alloc-dest-jump
                                        :dest-jump))))

;;;------------------------------------------------------------------------
;;; update-let wird aufgerufen um eine  Substitution vorzunehmen, 
;;; nachdem "optimize-1" diese erlaubt hat.
;;;------------------------------------------------------------------------
(defun update-let (var init-form let-form)
  
  ;; Update der read-slots bei  Variablen .
  ;; Wenn die Init-form eine Variablenreferenz  ist , dann wird 
  ;; der Slot READ bei der entsprechenden Variablen um die Anzahl 
  ;; der Referenzen von VAR erhoeht.
  ;; ansonsten ist die init-form entweder eine Seiteneffektfreie Form
  ;; wo es keine Variablen referenziert sind, oder eine Form die nur 
  ;; einmal referenziert ist.
  ;;----------------------------------------------
  (when (var-ref-p init-form)
    (setf (?read (?var init-form))
          (+ (?read (?var init-form)) (?read var))))
      
  
  ;; Die Substitution wird zunaechst bei den 
  ;; Initformen der restlichen Bindungen der
  ;; let-form vorgenommen.
  ;;-------------------------------------------
  (dotimes (i    (length (?init-list let-form)))
    (setf (nth i (?init-list let-form))
          (subst-var var  init-form (nth i (?init-list let-form)))))
  
  ;; Nun wird in dem Rumf substituiert.
  ;;-----------------------------------
  (setf (?body let-form) (subst-var  var init-form  (?body let-form)))
  let-form)


;;;-------------------------------------------------------------------------
;;; Nun werden die VAR-LIST und die INIT-LIST einer LET-FORM nach der 
;;; Substitutionsphase  updated.
;;;-------------------------------------------------------------------------
(defun delete-unref-var (let-form)
  (let ((var-list nil)
        (init-list nil)
        var init-form)
    (dotimes (i (length (?var-list let-form)))
      (setq var (nth i (?var-list let-form)))
      (setq init-form (nth i (?init-list let-form)))
      (if  (or  (dynamic-p var) (< 0 (?read var)) (< 1 (?write var)))
           
           ;; Die Variable wurde nicht durch Init-form substituiert.
           ;;-------------------------------------------------------
           (progn 
             (setq var-list  (append var-list (list var)))
             (setq init-list (append init-list (list init-form))))
           
           ;; Die Variable wurde Substituiert.
           ;;---------------------------------
           (incf *eliminated-vars*)))
    
    (setf (?var-list let-form) var-list)
    (setf (?init-list let-form) init-list))
  let-form)


;;;-------------------------------------------------------------------------
;;; Hier wird jedes  Vorkommen von "var" in "body" durch "form" ersetzt.
;;;-------------------------------------------------------------------------

(defun subst-var (var form body)
  (labels ( (map-subst-var (one-form)
                         (subst-var var form one-form)))
    
    (typecase body
      (var-ref 
       (if  (equal  (?var body) var)
            (progn 
              (decf (?read (?var body )))
              (incf *subst-number*)
              form)
            body))
      
      (if-form
       (setf (?pred body ) (subst-var var form (?pred body)))
       (setf (?then body ) (subst-var var form (?then body)))
       (setf (?else body ) (subst-var var form (?else body)))
       body)
    
      (setq-form
       (setf (?form body) (subst-var var form (?form body)))
       body)
      
      (progn-form
       (setf (?form-list body) (mapcar #'map-subst-var (?form-list body)))
       body)
      
      (tagbody-form 
       (setf (?first-form  body ) (subst-var var form (?first-form body)))
       (dolist (one-tagged-form (?tagged-form-list body))
         (setf (?form one-tagged-form) 
               (subst-var var form (?form one-tagged-form))))
       body)
      
      (switch-form 
       (setf (?form body) (subst-var var form (?form body)))
       (setf (?case-list body)
             (mapcar #'map-subst-var (?case-list body)))
       (setf (?otherwise body) (subst-var var form (?otherwise body)))
       body)
      
      (labeled-form 
       (setf (?form body) (subst-var var form  (?form body)))
       body)
      
      (let/cc-form
       (setf (?body body) (subst-var var form (?body body)))
       body)
      
      (let*-form
       (setf (?init-list body)
             (mapcar #'map-subst-var (?init-list body)))
       (setf (?body body) (subst-var var form (?body body)))
       body)
      
      
      (labels-form 
       (dolist (one-fun (?fun-list body))
         (setf (?params one-fun)  (subst-var var form (?params one-fun)))
         (setf (?body one-fun)    (subst-var var form (?body one-fun))))
       (setf (?body body) (subst-var var form (?body body)))
       body)
      
      (defined-fun 
          body)
      
      (params 
       (when (?opt-list body)
         (setf (?opt-list body)
               (mapcar #'map-subst-var (?opt-list body))))
       (when (?key-list body)
         (setf (?key-list body)
               (mapcar #'map-subst-var (?key-list body))))
       body)
      
      (opt
       (setf (?init body) (subst-var var form (?init body)))
       body)
      
      (app 
       (unless (fun-p (?form body))
         (setf (?form body) (subst-var var form (?form body))))
       (setf (?arg-list body)
             (mapcar #'map-subst-var (?arg-list body)))
       body)
      
      (mv-lambda 
       (setf (?params body) (subst-var var form (?params body)))
       (setf (?body   body) (subst-var var form (?body   body)))
       (setf (?arg    body) (subst-var var form (?arg    body)))
       body)
      
      (class-def 
       (dolist (one-slot-descr (?slot-descr-list body))
         (setf (?initform one-slot-descr)
               (subst-var var form (?initform one-slot-descr))))
       body)
      
      (cont body)
      (slot-desc
       (setf (?initform body) (subst-var var form (?initform body)))
       body)
      (form body))))

;;;-------------------------------------------------------------------------
;;; EFFECT-OF-FORM berechnet die Effekte einer Form.
;;; Die Effekte, die durch Funktionsaufrufe entstehen werden in 
;;; "effect" gesammelt, die textuell sichtbaren "also Referenzieren
;;; von Variablen oder Veraenderung durch SETQ" werden in local-effect
;;; abgespeichert.
;;;-------------------------------------------------------------------------

(defun effect-of-form (form effect local-effect &optional (but-form nil))
  (etypecase form 
    (if-form
     (unless (eq but-form form)
       (effect-of-form (?pred form) effect local-effect but-form)
       (effect-of-form (?then form) effect local-effect but-form)
       (effect-of-form (?else form) effect local-effect but-form)))
    (progn-form
     (unless (eq but-form form)
       (dolist (one-form (?form-list form))
         (effect-of-form one-form effect local-effect but-form))))
    
    (let*-form
     (unless (eq but-form form)
       (effect-of-form (?body form) effect local-effect but-form)
       (dolist (one-init-form (?init-list form))
         (effect-of-form one-init-form effect local-effect but-form))
       (dolist (one-var (?var-list form))
         (when (dynamic-p one-var)
           (when (listp (?write-list effect))
             (pushnew one-var (?write-list effect)))))))
    
    (var-ref 
     (unless (eq but-form form)
       (pushnew (?var form) (?read-list local-effect))))
    
    
    (tagbody-form 
     (unless (eq but-form form)
       (effect-of-form (?first-form  form) effect local-effect but-form)
       (dolist (tagged-form (?tagged-form-list form))
         (effect-of-form (?form tagged-form) effect local-effect but-form))))
    
    (switch-form 
     (unless (eq but-form form)
       (effect-of-form (?form form) effect local-effect but-form)
       (dolist (one-labeled-form (?case-list form))
         (effect-of-form one-labeled-form effect local-effect but-form))
       (effect-of-form (?otherwise form) effect local-effect but-form)))
    
    (labeled-form
     (unless (eq but-form form)
       (effect-of-form (?value form) effect local-effect but-form)
       (effect-of-form (?form form) effect  local-effect but-form)))
    
    (let/cc-form 
     (unless (eq but-form form)
       (effect-of-form (?body form) effect local-effect but-form)))
    
    (app
     
     (unless (eq but-form form)
       (let ((functional (?form form))
             (arg-list   (?arg-list form)))
         (if (fun-p functional)
             (if (null (?has-funs-as-args functional))
                 
                 ;; In dem Fall, wo die applizierte Form eine bekannte 
                 ;; Funktion ist, die keine Parameter hat, die an Funktionale
                 ;; Objekte gebunden  werden m"ussen, wird der Effekt der 
                 ;; Funktion in "EFFECT" kopiert, und die Argumente in einem 
                 ;; lokalen Effekt analysiert, um eine detailliertere 
                 ;; Seiteneffektinformation zu bekommen.
                 ;;-----------------------------------------------------------
                 (progn 
                   (union-all-effects effect effect (get-effect functional))
                   (dolist (one-arg arg-list)
                     (effect-of-form one-arg effect local-effect  but-form)))
                 
                 ;; Wenn der Slot HAS-FUNS-AS-ARGS der applizierten Funktion 
                 ;; nicht leer ist, werden die Bindungen an die Variablen im 
                 ;; Slot HAS-UFNS-AS-ARGS "uberpr"uft : Falls eine solcher 
                 ;; Variablen an einer special-variable der *current-function* 
                 ;; gebunden ist, wird "EFFECT" updated und auf TOP-EFFECT 
                 ;; gesetzt,  denn solche Applikationen haben keinen TOP-EFFECT 
                 ;; bei der Seiteneffektanalyse, denn sie werden gesondert 
                 ;; behandelt. Anschlie"send werden die Argumente auch 
                 ;; analysiert um evtl s"amtliche Applikationen bei denen
                 ;; richtig zu behandeln.
                 ;;-----------------------------------------------------------
                 (check-args-for-spec-vars form effect local-effect))
             
             
             ;; Bei einer Applikation einer Variablen wird explizit den 
             ;; "EFFECT" auf TOP gesetzt, denn die Applikationen von Parametern,
             ;; die an funktionalen Objekten gebunden werden m"ussen, keinen 
             ;; TOP-EFFECT bei der Seiteneffektanalyse haben, weil sie 
             ;; gesondert behandelt werden. Bei den Optimierungen soll 
             ;; aber angenommen werden da"s sie den TOP-EFFECT haben. 
             ;; Hier werden die Argumente nicht weiter analysiert, weil der 
             ;; EFFECT schon TOP-EFFECT ist.
             ;;----------------------------------------------------------------
             (if  (var-ref-p functional)
                  (union-all-effects effect effect (top-effect 
                                                    (get-top-effect-level 
                                                     *current-function*)))
                  
                  ;; In den anderen F"allen wird der Effekt der Applikation 
                  ;; in EFFECT einfach kopiert, ohne weitere Verfeinerung 
                  ;; des Seiteneffektes.
                  ;;-------------------------------------------------------
                  
                  (union-all-effects effect effect (get-effect form)))))))
    
    (labels-form 
     (unless (eq but-form form)

       ;; Die Effekte der lokalen Funktionen werden mitgerechnet, um zu 
       ;; "uberpr"ufen ob in den R"umpfen dieser Funktionen substituiert 
       ;; werden kann. Dies ist ganz wichtig damit auch die Slots "read" 
       ;; und "write" bei Variablen richtig gesetzt werden, und keine 
       ;; falschen Optimierungen vorgenommen werden.
       ;;---------------------------------------------------------------
       (dolist (one-local-fun (?fun-list form))
         (unless (equal one-local-fun but-form)
           (union-all-effects effect effect (get-effect one-local-fun))))
       (effect-of-form (?body form) effect local-effect but-form)))
    
    (mv-lambda 
     (unless (eq but-form form)
       (effect-of-form (?params form) effect local-effect but-form)
       (effect-of-form (?body form)   effect local-effect but-form)
       (effect-of-form (?arg form)    effect local-effect but-form)))

    (setq-form
     (unless (eq but-form form)
       (effect-of-form (?form form) effect local-effect but-form)
       (let ((loc  (?location form)))
         (when (var-ref-p loc)
           (pushnew (?var loc) (?write-list local-effect))))))
    
    (class-def
     (dolist (one-slot-descr (?slot-descr-list form))
       (effect-of-form (?initform one-slot-descr)  effect local-effect
                       but-form)))
    (tagged-form (setf (?data-effects effect) 
                       (max-data-effects :jump (?data-effects effect))))

    (params (let ((key-list  (?key-list form ))
                  (opt-list  (?opt-list form )))
              (when key-list 
                (dolist (one-key-param key-list)
                  (effect-of-form (?init one-key-param)
                                  effect local-effect but-form)))
              (when opt-list
                (dolist (one-opt-param opt-list)
                  (effect-of-form (?init one-opt-param) 
                                  effect local-effect but-form)))))
    
              
    (cont (empty-effect))
    (form (empty-effect))))

;;;--------------------------------------------------------------------------
;;; check-args-for-spec-vars bildet die Argument liste auf die Parameterliste
;;; bei einem Aufruf einer Funktion mit special-vars also HAS-FUNS-AS-ARGS 
;;; nicht nil. Wenn eine solche Variable an einer Variablen gebunden ist
;;; wird EFFECT auf TOP gesetzt, sonst wird der Effekt der Appikation 
;;; in EFFECT kopiert und die weiteren Argumente der Applikation analysiert
;;; um evtl Applikationen solcher Art richtig zu behandeln.
;;;--------------------------------------------------------------------------
(defun check-args-for-spec-vars (app effect local-effect)
  (let* ((function      (?form app))
         (spec-vars     (if (defined-fun-p function)
                            (remove-if-not 
                             #'(lambda (var)
                                 (member var 
                                         (?all-vars (?params function)) 
                                         :test #'eq))
                             (?has-funs-as-args function))
                            (?has-funs-as-args function)))
         (args              (if (defined-fun-p function)
                                        (mapcar #'(lambda (one-spec-var)
                                                    (get-arg-from-param 
                                                     one-spec-var app))
                                                spec-vars)
                                        (mapcar #'(lambda (one-spec-var)
                                                    (get-arg-from-coded-param 
                                                     one-spec-var app))
                                                spec-vars)))
         (possible-forms (apply #'append 
                                (mapcar #'get-result-position-forms args ))))
    
    
    
    (if (null (remove-if-not #'var-ref-p possible-forms))
        
        ;; Die special-vars werden nicht an Variablen gebunden:
        ;;---------------------------------------------------------------
        (progn 
          (union-all-effects effect effect (get-effect app))

          ;; nun werden die Argumente auch analysiert, um solche Applikationen
          ;; wie die gerade analysierte Applikation, bei den Argumenten 
          ;; richtig zu behandeln.
          ;;------------------------------------------------------------------
          (dolist (one-arg (?arg-list app))
            (effect-of-form one-arg effect local-effect)))
        
        ;; Hier ist der kritische Fall, wenn eine special-var der applizierten 
        ;; Funktion an einer special-var der gerade analysierten Funktion 
        ;;gebunden ist. Bei der Seiteneffektanalyse hat die ganze Applikation 
        ;; keinen TOP-EFFECT, denn das Argument ist auch eine special-var,
        ;; und diese werden gesondert behandelt.
        ;; Die Optimierungen sollen hier aber den TOP-EFFECT annehmen. 
        ;; In diesem Fall werden die Argumente nicht mehr analysiert, 
        ;; weil der EFFECT schon TOP-EFFECT ist.
        ;;--------------------------------------------------------------------
        (union-all-effects 
         effect effect (top-effect (get-top-effect-level 
                                    *current-function*))))))



(defun unify-effect (effect1 effect2 effect3)
  (setf (?read-list effect1)
        (unify-lists (?read-list effect2 ) (?read-list effect3)))
  (setf (?write-list effect1)
        (unify-lists (?write-list effect2) (?write-list effect3))))

;;;--------------------------------------------------------------------------
(provide "optimize")

