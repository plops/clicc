;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Codegenerierung
;;;            - Lambda Listen
;;;            - Variablenzugriffe
;;;            - Binden von Variablen
;;;            - Restaurieren von Special Variablen
;;;            - SETQ
;;;
;;; $Revision: 1.24 $
;;; $Log: cgvars.lisp,v $
;;; Revision 1.24  1993/09/10  15:33:24  hk
;;; Fehler in cg-form (var-ref) behoben
;;;
;;; Revision 1.23  1993/09/10  12:16:14  hk
;;; Vorhergehende Korrektur korrigiert.
;;;
;;; Revision 1.22  1993/09/10  10:05:41  hk
;;; Fehler behoben: Bearbeitung optionaler Parameter hat vorausgesetzt,
;;; daß die Argumentliste immer mit Offset 0 im Activation-Record liegt.
;;; Das ist bei der Bearbeitung von mv-lambda nicht unbedingt gegeben.
;;;
;;; Revision 1.21  1993/09/06  14:33:00  hk
;;; Fehler in (cg-form var-ref) behoben: bei Zugriff auf dynamisch
;;; gebundene Variablen auf Prädikatsposition eines if.
;;;
;;; Revision 1.20  1993/07/20  15:46:27  hk
;;; (cg-form named-const) ist nun definiert, falls der Wert ein Literal
;;; oder ein Symbol ist.
;;;
;;; Revision 1.19  1993/06/28  15:29:18  hk
;;; Fehler in CC-heapenv behoben und die Funktion vereinfacht.
;;;
;;; Revision 1.18  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.17  1993/05/12  15:01:33  hk
;;; Sonderbehandlung fuer Vorwaertsreferenzen auf Funktionen entfernt.
;;;
;;; Revision 1.16  1993/03/22  17:36:22  hk
;;; Keywords in LZS Slots.
;;;
;;; Revision 1.15  1993/02/16  15:48:45  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.14  1992/12/21  11:29:41  hk
;;; Schreibfehler in der letzten Aenderung.
;;;
;;; Revision 1.13  1992/12/21  11:25:48  hk
;;; ?read von nicht aufgeloesten Referenzen auf undefinierte Funktionen
;;; wird auf -1 gesetzt, um anzuzeigen, dass keine erneute Fehlermeldung
;;; ausgegeben zu werden braucht.
;;;
;;; Revision 1.12  1992/10/02  14:07:22  hk
;;; Fehlerbehandlung jetzt lokal
;;;
;;; Revision 1.11  1992/09/24  15:27:46  hk
;;; Fehler bei der Bearbeitung von Special-Parametern behoben.
;;;
;;; Revision 1.10  1992/09/23  12:31:59  hk
;;; Fehler bei der Bearbeitung von Key-supplied-p Variablen behoben.
;;;
;;; Revision 1.9  1992/09/21  11:18:52  hk
;;; Die eigentliche C-Codegenerierung uebersichtlicher gestaltet
;;;
;;; Revision 1.8  1992/08/11  12:47:53  hk
;;; C-Ln --> C-Decl, fuer Deklarationen.
;;;
;;; Revision 1.7  1992/08/10  12:04:25  hk
;;; *mv-spec* und *mv-produced* gestrichen, da Analyse komplett in Pass3.
;;;
;;; Revision 1.6  1992/07/09  16:48:34  hk
;;; Vernuenftige Fehlermeldung bei nicht aufgeloesten Forwarts-Referenzen.
;;;
;;; Revision 1.5  1992/07/06  15:00:47  hk
;;; In cg-var-bind (dynamic) wird die variable nur noch dann mit
;;; *result-spec* verglichen, wenn es ungleich nil ist.
;;;
;;; Revision 1.4  1992/06/04  14:44:34  hk
;;; Schreibfehler in Bearbeitung von setq-form.
;;;
;;; Revision 1.3  1992/06/04  12:59:17  hk
;;; Auf Rumpf von Let* wird mittels ?body und nicht mehr ?form zugegriffen.
;;;
;;; Revision 1.2  1992/06/04  07:11:20  hk
;;; Nach Umstellung auf die Lisp nahe Zwischensprache, Syntax-Fehler
;;; sind schon beseitigt
;;;
;;; Revision 1.1  1992/03/24  16:54:56  hk
;;; Initial revision
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Codegenerierung fuer Lambda-Listen von Funktionen und von mv-lambda
;;------------------------------------------------------------------------------
;; 
;; Lage der Parameter im Activation-Record:
;; 1. normale Parameter, 
;; 2. opt. supplied Parameter, 
;; 3. optionale Parameter, 
;; 4. Key-Parameter, 
;; 5. Rest-Parameter,
;; 6. Key supplied Parameter
;;------------------------------------------------------------------------------
;; EINSCHRAENKUNG: kein Identifikator darf in einer Lambda-Liste mehrfach
;; gebunden werden.
;; ACHTUNG: Der 1. Parameter liegt nicht unbedingt an der Position 0 im
;; Aktivation Record
;;------------------------------------------------------------------------------
(defun cg-params (params known-number-of-args)
  (let* ((nsimple       (length (?var-list params)))
         (nopt          (length (?opt-list params)))
         (nkey          (length (?key-list params)))
         (nsimpopt      (+ nsimple nopt))
         (rest          (?rest params))
         nopt-supplied
         suppl-index
         first-key 
         C-stacktop)

    (setq C-stacktop
          (CC-Stack (if (not known-number-of-args)

                        ;; Stack-Top wird erst zur Laufzeit bekannt, 
                        ;;------------------------------------------
                        (CC-op+ *stack-top* C-nargs)

                        ;; Stack-Top ist zur Uebersetzungszeit bekannt.
                        ;;---------------------------------------------
                        (+ nsimple *stack-top*))))

    ;; normale Parameter bearbeiten
    ;;-----------------------------
    (dolist (simple (?var-list params))
      (cg-var-bind simple *stack-top* C-stacktop)
      (incf *stack-top*))     

    ;; optionale und opt-suppliedp Parameter bearbeiten
    ;;-------------------------------------------------
    (when (> nopt 0)
        
      ;; Anzahl der opt-suppliedp Parameter bestimmen
      ;;---------------------------------------------
      (setq nopt-supplied
            (count-if-not #'null (?opt-list params)
                          :key #'?suppl))
        
      ;; Die optionalen und Rest-Parameter soweit im Stack verschieben, dass
      ;; zwischen den normalen und den optionalen Parametern Platz fuer die
      ;; opt-suppliedp Parameter entsteht.
      ;;--------------------------------------------------------------------
      (when (> nopt-supplied 0)

        ;; (C-MemUpMove SrcHigh SrcLow MoveDiff)
        ;;--------------------------------------
        (C-MemUpMove C-stacktop (CC-StackTop) nopt-supplied)

        ;; opt-supplied-Parameter mit NIL initialisieren
        ;;----------------------------------------------
        (dotimes (i nopt-supplied)
          (C-nil (CC-StackTop))
          (incf *stack-top*))

        ;; opt-supplied-Parameter wie simple-Parameter behandeln
        ;;------------------------------------------------------
        (C-Assign C-nargs (CC-op+ C-nargs nopt-supplied))
        (incf nsimple nopt-supplied)
        (incf nsimpopt nopt-supplied))

      ;; Hier werden die angegebenen optionalen Parameter gebunden
      ;; und die zugehoerigen supplied-p Variablen an T gebunden.
      ;; ACHTUNG: Die Variablen werden hier von RECHTS nach LINKS gebunden.
      ;; Dies fuehrt nur deshalb nicht zu Fehlern, weil jeder Identifikator
      ;; hoechstens einmal gebunden wird und weil waehrend des Bindens
      ;; keine Ausdruecke ausgewertet werden,
      ;; die diese Identifikatoren enthalten.
      ;; Es wird nur dann Code erzeugt, wenn opt-supplied-p Parameter
      ;; vorhanden sind oder wenn mindestens ein optionaler Parameter
      ;; special gebunden wird oder frei in einer Closure vorkommt.
      ;;-------------------------------------------------------------
      (when (or (> nopt-supplied 0)
                (find-if #'(lambda (x) (or (dynamic-p x)
                                           (and (local-static-p x)
                                                (?closure x))))
                         (?opt-list params)
                         :key #'?var))

        (C-if (CC-op> C-nargs nsimpopt))
        (C-blockstart)
        (C-goto C-ALL_OPT_SUPPLIED)
        (C-blockend)

        ;; initialisierte optionale Parameter bearbeiten
        ;;----------------------------------------------
        (C-switch C-nargs)
        (C-blockstart)
        (setq suppl-index *stack-top*)
        (let ((first-CASE t)
              (i nopt)

              ;; Weil cg-var-bind 2x f"ur special und closure Variablen
              ;; aufgerufen wird, soll der erste Aufruf keinen Effekt auf
              ;; folgende special Variablen haben.
              ;;----------------------------------
              (*special-count* *special-count*)
              (*do-not-restore* *do-not-restore*))
          (dolist (opt-spec (reverse (?opt-list params)))

            ;; erzeugt :
            ;;----------
            ;; switch (nargs)
            ;; {
            ;; case <nsimple+nopt>:
            ;; ALL_OPT_SUPPLIED   : <hdl-opt(nopt-1)>
            ;;   ....
            ;; case <nsimple+1   >: <hdl-opt(0)>
            ;; }

            ;; CASE nur einfuegen, wenn ueberhaupt etwas mit
            ;; dem i. Parameter geschieht.
            ;;----------------------------------------------
            (when (or (?suppl opt-spec)
                      (dynamic-p (?var opt-spec))
                      (and (local-static-p (?var opt-spec))
                           (?closure (?var opt-spec))))
              (C-Case (+ nsimple i))
              (when first-CASE
                (setq first-CASE nil)
                (C-label C-ALL_OPT_SUPPLIED))
              (cg-var-bind
               (?var opt-spec) (+ *stack-top* i -1) C-stacktop)
              (when (?suppl opt-spec)
                (decf suppl-index)
                (C-T (CC-Stack suppl-index))
                (cg-var-bind (?suppl opt-spec) suppl-index C-stacktop)))
            (decf i)))
        
        (C-blockend))

      ;; Hier werden die nicht belegten optionalen Parameter
      ;; mit ihrem Defaultwert initialisiert.
      ;; Wenn kein optionaler Parameter angegeben ist, dann liegt
      ;; Stack-Top direkt hinter dem ersten normalen Parameter.
      ;;---------------------------------------------------------
      (C-switch C-nargs)
      (C-blockstart)
      (let ((*result-spec* (stacktop-result-location))
            (i 0))
        (dolist (opt-spec (?opt-list params))
          (C-case (+ nsimple i))
          (cg-form (?init opt-spec))
          (cg-var-bind (?var opt-spec) *stack-top*)
          (incf *stack-top*)
          (incf (?offset *result-spec*))
          (when (?suppl opt-spec)

            ;; supplied-p Parameter wurden schon mit NIL initialisiert
            ;;--------------------------------------------------------
            (cg-var-bind (?suppl opt-spec) suppl-index (CC-StackTop))
            (incf suppl-index))
          (incf i)))

      (cond

        ;; Waehrend der Bearbeitung von &rest und &key sieht es so aus,
        ;; als ob alle optionalen Parameter angegeben worden seien. 
        ;;---------------------------------------------------------
        ((or (> nkey 0) rest)
         (C-Assign C-nargs nsimpopt))
                
        ;; auf ueberschuessige Argumente pruefen
        ;;--------------------------------------
        (T
         (C-case (+ nsimple nopt))
         (C-break)
         (C-defaultcase)
         (C-Abort "TOO_MANY_ARGS")))

      (C-blockend))

    (when (> nkey 0)

      ;; Achtung, erzeugt nicht geschlossenen C-Block !
      ;; Wird erst nach der Bearbeitung der Key-Parameter geschlossen.
      ;;--------------------------------------------------------------
      (C-blockstart)
      
      ;; wird von 'keysort' initialisiert
      ;;---------------------------------
      (C-ArrayDecl "BOOL" C-supl_flags nkey) 

      ;; ein Array mit Zeigern auf die moeglichen Keywords anlegen.
      ;;-----------------------------------------------------------
      (C-StaticArrayInitDecl "CL_FORM *" C-keylist)
      (C-initstart)
      (dolist (key-spec (?key-list params))
        (C-init (CC-symbol (?sym key-spec))))
      (C-initend)

      ;; verschiebt Rest-Liste im Stack, so dass genuegend Platz fuer die
      ;; Key-Werte entsteht und sortiert die Key-Werte in die entstandene
      ;; Luecke. Nicht angegebene Key-Werte werden mit NIL belegt.
      ;;-----------------------------------------------------------------
      (C-Call "keysort" (CC-StackTop) (CC-op- C-nargs nsimpopt)
              nkey C-keylist C-supl_flags (CC-bool (?allow-other-keys params)))
      (setq first-key *stack-top*)
      (incf *stack-top* nkey))
      
    ;; Pruefen, ob die Rest-Variable nur angegeben wurde, damit beliebig viele
    ;; Argumente angegeben werden koennen, ohne dass die Rest-Variable selbst
    ;; verwendet wird.
    ;;----------------
    (when (and rest (or (plusp (?write rest)) (plusp (?read rest))))

      ;; Erzeugt die Rest-Liste im Heap, Zeiger auf diese Liste wird hinter
      ;; den Key-Parametern abgelegt.
      ;;----------------------------------------------
      (C-Lispcall "Flist" (CC-StackTop) (CC-op- C-nargs nsimpopt))
      (cg-var-bind rest *stack-top*)
      (incf *stack-top*))

    (when (> nkey 0)
      (let ((*result-spec* (stack-location first-key *level*))
            (i 0))
        (dolist (key-spec (?key-list params))

          ;; nur etwas zu tun, wenn Supplied-Variable gesetzt
          ;; oder Keyword-Parameter mit Wert != NIL initialisiert wird.
          ;;--------------------------------------------------------------
          (when (or (?init key-spec) (?suppl key-spec))

            (C-if (CC-MacroCall "NOT" (CC-arraykomp C-supl_flags i)))
            (C-blockstart)

            ;; wenn nicht angegeben und der Defaultwert ungleich
            ;; NIL, dann mit Defaultwert initialisieren.
            ;;--------------------------------------------------
            (when (?init key-spec)
              (cg-form (?init key-spec)))
            
            (when (?suppl key-spec)

              ;; supplied Parameter vorhanden, auf nil setzen
              ;;---------------------------------------------
              (C-nil (CC-StackTop))
              (C-blockend)
              (C-else)
              (C-blockstart)

              ;; auf t setzen
              ;;-------------
              (C-t (CC-StackTop)))
            
            (C-blockend)
            
            (when (?suppl key-spec)
              
              ;; ACHTUNG: hier wird nicht streng von links nach rechts gebunden.
              ;; Dies fuehrt nur deshalb nicht zu Fehlern, weil jeder
              ;; Identifikator hoechstens einmal gebunden wird und
              ;; weil waehrend des Bindens keine Ausdruecke ausgewertet werden,
              ;; die diese Identifikatoren enthalten.
              ;;---------------------------------------------------------------
              (cg-var-bind (?suppl key-spec) *stack-top*)
              (incf *stack-top*)))
          
          (cg-var-bind (?var key-spec) (?offset *result-spec*) (CC-StackTop))
          (incf (?offset *result-spec*))
          (incf i)))

      ;; den C-Block schliessen, in dem supl_flags definiert wurde.
      ;;-----------------------------------------------------------
      (C-blockend))))

;;------------------------------------------------------------------------------
;; let*
;;------------------------------------------------------------------------------
(defmethod cg-form ((form let*-form))
  (let ((old-stack *stack-top*)
        (old-special *special-count*)
        (vars (?var-list form))
        (forms (?init-list form)))

    (let ((*result-spec* (stacktop-result-location)))
      (loop (unless vars (return))
       (cg-form (pop forms))
       (cg-var-bind (pop vars) *stack-top*)
       (incf (?offset *result-spec*))
       (incf *stack-top*)))

    (cg-form (?body form))
    (setq *stack-top* old-stack)
    (C-restore-special old-special)))

;;------------------------------------------------------------------------------
;; Variablenzugriff
;;------------------------------------------------------------------------------
(defmethod cg-form ((form var-ref))
  (let ((var (?var form)))
    (cond
      ((and (eq *result-spec* 'C-bool) (dynamic-p var) (> *special-count* 0))
       (C-Assign C-bool_result (CC-make-bool (CC-special var)))
       (setq *C-bool* C-bool_result))
      (t (to-result-loc var)))))

;;------------------------------------------------------------------------------
;; setq
;;------------------------------------------------------------------------------
(defmethod cg-form ((form setq-form))
  (let ((var (?var (?location form))))
    (let ((*result-spec*
           (if (and (local-static-p var)
                    (eql (?offset var) (1- *stack-top*))
                    (eql (?level var) *level*)
                    (not (?closure var)))

             ;; Zuweisung an eine lokale Variable, die direkt unterhalb von
             ;; *stacktop* liegt; (nicht im Heap !)
             ;;------------------------------------
             (stacktop-result-location (?offset var))
             var)))
        
      (cg-form (?form form)))
    (to-result-loc var)))

;;------------------------------------------------------------------------------
;; Named Constant
;;------------------------------------------------------------------------------
(defmethod cg-form ((form named-const))
  (let ((value (?value form)))
    (if (eq :unknown value)
        (internal-error
         'cg-form
         "Named constants with unknown values are nor implemented: ~s"
         (?symbol form))
        (etypecase value
          ((or simple-literal sym structured-literal) (cg-form value))))))

;;------------------------------------------------------------------------------
;; Bindet die Variable 'var' neu. 'var' wird im Stack oder im Heap oder in
;; der Value-Cell eines Symbols angelegt. 'var' wird initialisiert mit dem
;; Wert im aktuellen Activation-Record an der Position 'offset'.
;; C-stacktop enthaelt C-Code, der die naechste freie Position im Stack angibt.
;;------------------------------------------------------------------------------
(defmethod cg-var-bind ((var local-static) offset &optional C-stacktop) 

  ;; Offset eintragen fuer spaetere Zugriffe
  ;;----------------------------------------
  (setf (?offset var) offset)
  (when (?closure var)

    ;; Wenn nichts anderes angeben ist, dann liegt der Wert der zu bindenden
    ;; Variablen auf der Spitze des Stacks. Die naechste freie Position liegt
    ;; also um 1 hoeher.
    ;;------------------
    (unless C-stacktop (setq C-stacktop (CC-Stack (1+ offset))))

    ;; neue Zelle im Heap anlegen, Wert dorthin kopieren und
    ;; auf dem LISP-Stack einen Zeiger hinterlassen
    ;;---------------------------------------------
    (C-MacroCall "GEN_HEAPVAR" (CC-Stack offset) C-stacktop)))

;;------------------------------------------------------------------------------
(defmethod cg-var-bind ((var dynamic) offset &optional C-stacktop)
  (declare (ignore C-stacktop))
  (C-MacroCall "BIND_SPECIAL" (CC-symbol (?sym var)) (CC-Stack offset))

  ;; SONDERFALL feststellen:
  ;; Eine special Variable wird gebunden und das Resultat des aktuellen
  ;; Ausdrucks (Funktion, Let*) wird genau dieser Variablen zugewiesen.
  ;; Dann darf diese Variable nicht restauriert werden.
  ;;---------------------------------------------------
  (when (and *result-spec* (equal-loc *result-spec* var))
    (push *special-count* *do-not-restore*))
  (incf *special-count*))

;;------------------------------------------------------------------------------
;; Eintraege vom Binding-Stack entfernen, und die Special Variablen
;; restaurieren, die nicht in *do-not-restore* eingetragen sind.
;; Setzt *special-count* und *do-not-resore* zurueck.
;;------------------------------------------------------------------------------
(defun C-restore-special (old-special)
  (do ()
      ((= *special-count* old-special))
    (decf *special-count*)
    (cond
      ((eql (car *do-not-restore*) *special-count*)
       (pop *do-not-restore*)
       (C-MacroCmd "POP_SPECIAL"))
      (t (C-MacroCmd "RESTORE_SPECIAL")))))

;;------------------------------------------------------------------------------
;; Wie oben, aber veraendert *special-count* und *do-not-restore* nicht.
;;------------------------------------------------------------------------------
(defun C-restore-special2 (old-special)
  (do ((i *special-count*)
       (l *do-not-restore*))
      ((= i old-special))
    (decf i)
    (cond
      ((eql (car l) i)
       (pop l)
       (C-MacroCmd "POP_SPECIAL"))
      (t (C-MacroCmd "RESTORE_SPECIAL")))))

;;------------------------------------------------------------------------------
;; Resultat: C-Code (als String) fuer den Zugriff auf eine statisch gebundene
;; Variable.
;; Es koennen 3 Faelle eintreten:
;; - Die Variable liegt direkt im Stack.
;; - Die Variable liegt im Heap, da sie aus einer Closure referenziert
;;   (werden) wird. Es existiert aber auch (noch) eine Referenz auf diese 
;;   Variable im Activation Record der Funktion, aus der der Zugriff erfolgt.
;; - Die Variable liegt im Heap. Der Zugriff erfolgt innerhalb einer Closure. 
;;   Die Adresse der Variablen wird der CLOSURE-Datenstruktur entnommen,
;;   die an Position 0 im Activation Record der Closure liegt.
;;------------------------------------------------------------------------------
(defun CC-static (static)
  (let* ((level (?level static))
         (offset (?offset static)))
    
    (cond
      ;; liegt direkt im Stack
      ;;----------------------
      ((not (?closure static)) (CC-frame-access offset level))

      ;; Zugriff innerhalb einer Closure auf eine freie Variable der Closure
      ;;--------------------------------------------------------------------
      ((< level *cl-level*) (CC-clos-heap (closure-var-offset static)))

      ;; Zeiger liegt im Stack, Variable im Heap
      ;;----------------------------------------
      (t (CC-stack-heap offset level)))))

;;------------------------------------------------------------------------------
;; Resultat: C-Code (als String), fuer den Zugriff auf einen Offset in einem
;; Activation Record.
;;------------------------------------------------------------------------------
(defun CC-frame-access (offset level)
  (if (= *level* level)

    ;; liegt im aktuellen Activation Record.
    ;;--------------------------------------
    (CC-Stack offset)

    (CC-arrayptr (CC-arraykomp C-display (- level *cl-level*)) offset)))

;;------------------------------------------------------------------------------
(defun CC-StackTop ()
  (CC-Stack *stack-top*))

;;------------------------------------------------------------------------------
;; Resultat: C-Code (als String) fuer den Zugriff auf eine lokale Variable im
;; Heap ueber eine Indirection-Zelle im Stack.
;;------------------------------------------------------------------------------
(defun CC-stack-heap (offset level)
  (CC-MacroCall "INDIRECT" (CC-frame-access offset level)))

;;------------------------------------------------------------------------------
;; Resultat: C-Code (als String) fuer den Zugriff auf eine lokale Variable im
;; Heap, auf die ein Zeiger in einer Closure-Datenstruktur zeigt.
;;------------------------------------------------------------------------------
(defun  CC-clos-heap (index)
  (CC-MacroCall "INDIRECT" (CC-heapenv index)))

;;------------------------------------------------------------------------------
;; Resultat: C-Code (als String) fuer den Zugriff auf eine Komponenete einer
;; Closure-Datenstruktur
;;------------------------------------------------------------------------------
(defun CC-closure-access (closure index)
  (CC-op+ (CC-MacroCall "GET_FORM" closure) index))

;;------------------------------------------------------------------------------
;; Resultat: C-Code (als String) fuer den Zugriff auf den n-ten Eintrag der
;; CLOSURE-Datenstruktur der aktuellen Closure.  Das 1. Argument einer Closure
;; ist seine CLOSURE-Datenstruktur und wird an der Position 0 im aktuellen
;; Activation-Record uebergeben.
;;------------------------------------------------------------------------------
(defun CC-heapenv (n)
  (CC-closure-access (CC-frame-access 0 *cl-level*) n))

;;------------------------------------------------------------------------------
;; Location fuer *result-spec* erzeugen.
;;------------------------------------------------------------------------------
(defun stacktop-location ()
  (stack-location *stack-top* *level*))

;;------------------------------------------------------------------------------
(defun stack-location (offset level)
  (make-instance 'local-static :offset offset :level level))

;;------------------------------------------------------------------------------
;; Eine Location von diesem Typ wird in *result-spec* eingetragen, wenn das
;; Resultat auf der Spitze des Stack generiert werden soll. Diese Information
;; wird bei der Parameteruebergabe an eine Funktion, die das Resultat
;; berechnet, verwendet, um das aktuelle Activation-Record fuer die
;; aufzurufende Funktion teilweise wiederzuverwenden.
;;------------------------------------------------------------------------------
(defun stacktop-result-location (&optional (offset *stack-top*))
  (make-instance 'stacktop-result :offset offset :level *level*))

;;------------------------------------------------------------------------------
(defun stacktop-to-result-loc ()
  (to-result-loc (stacktop-location)))
  
;;------------------------------------------------------------------------------
(defun to-result-loc (loc)
  (case *result-spec*
    
    ;; Resultat wird nicht benoetigt
    ;;------------------------------
    ((NIL))
    
    ;; Boolesches Resultat gewuenscht
    ;;-------------------------------
    (C-BOOL (setq *C-bool* (CC-make-bool (CC-dest loc))))

    ;; Normales Resultat erzeugen
    ;;---------------------------
    (T (cg-copy loc *result-spec*))))

;;------------------------------------------------------------------------------
(defun CC-dest (loc)
  (typecase loc
    (static (CC-static loc))
    (dynamic (CC-special loc))
    (mv-buf (CC-mv_buf (?offset loc)))
    (T (error "unexpected location: ~S" loc))))

;;------------------------------------------------------------------------------
(defun equal-loc (loc1 loc2)
  (typecase loc1
    (local-static
     (and (local-static-p loc2)
          (eql (?offset loc1) (?offset loc2))
          (eql (?level loc1) (?level loc2))
          (eql (?closure loc1) (?closure loc2))))
    ((or global-static imported-static dynamic) (eq loc1 loc2))
    (mv-buf (and (mv-buf-p loc2)
                 (eql (?offset loc1) (?offset loc2))))
    (T (error "unexpected location: ~S" loc1))))

;;------------------------------------------------------------------------------
(defun cg-copy (source dest)
  (unless (or (equal-loc source dest)
              (and *copy-source*
                   (equal-loc source *copy-dest*)
                   (equal-loc dest *copy-source*)))
    (C-copy (CC-dest source) (CC-dest dest))
    (setq *copy-source* source *copy-dest* dest)))

;;------------------------------------------------------------------------------
(provide "cgvars")
