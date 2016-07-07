;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;--------------------------------------------------------------------------
;;; Project    :CLICC ein Commmon LIsp to C Compiler
;;;             ------------------------------------
;;; Dateiname  : CLOSURE-ANALYSIS.LISP
;;; Funktion   : Die vorbereitung fuer die Seiteneffektanalyse , sowie fuer 
;;;              Die verschiedenen Optimierungsverfahren.
;;;              1) Es wird eine Liste der im Module enstandenen Closures 
;;;                 erstellt.
;;;              2) Die  Closures werden analysiert, und attributiert, mit 
;;;                 Informationen die zur Codegenerierung sowie fuer die 
;;;                 Optimierung der Tail-rekursion nuetzlich sind.
;;; Autor      : Anouar Trigui
;;; $Revision: 1.30 $
;;; $Log: closure-analysis.lisp,v $
;;; Revision 1.30  1993/12/17  10:12:46  hk
;;; Für CMU: Lokale Funktion all-vars-or-funs aus der Methode
;;; (get-fun-vars setq-form) herausgezogen.
;;;
;;; Revision 1.29  1993/11/09  16:39:55  atr
;;; Die globalen Variablen stehen jetzt in se-init-lisp.
;;;
;;; Revision 1.28  1993/10/25  10:03:28  atr
;;; Get-result-position-forms einer app ist die app selbst, es wird nicht
;;; weiter auf sonderf"alle geachtet.
;;;
;;; Revision 1.27  1993/10/18  22:23:38  atr
;;; Code etwas verbessert, so da"s die Voranalyse etwas schneller l"auft.
;;;
;;; Revision 1.26  1993/10/14  17:03:32  atr
;;; Die Variable *map-functions* wird hier nicht mehr benutzt, stattdessen
;;; wird der Slot ?special-cller bei special-defined-funs benutzt.
;;;
;;; Revision 1.25  1993/10/13  17:09:41  atr
;;; Eine Methode find-closures f"ur VAR eingef"ugt.
;;;
;;; Revision 1.24  1993/10/13  14:44:37  atr
;;; Dei Spezialisierung uber T durch Spezialisierung "uber Form ersetzt,
;;; und Methoden f"ur CONT eingef"ugt.
;;;
;;; Revision 1.23  1993/10/11  10:41:11  atr
;;; Die Funktion list-the-special-defined-funs und die Variable
;;; *special-defined-funs* entfernt.
;;;
;;; Revision 1.22  1993/10/08  10:11:14  hk
;;; Fehler behoben: 'get-fun-vars' wird nun auch auf die 1. Komponente
;;; einer Applikation angewendet, wenn dieses eine Variable ist.
;;; Aus der Methode für params eine eigene Funktion gemacht.
;;;
;;; Revision 1.21  1993/10/08  09:16:34  hk
;;; Fehler in der letzten Änderung behoben
;;;
;;; Revision 1.20  1993/10/07  15:33:16  hk
;;; Die Komponente ?free-lex-vars von lokalen Funktionen wird während der
;;;  Ausführung von 'get-fun-vars' für die Typinferenz gesetzt.
;;;
;;; Revision 1.19  1993/10/07  13:59:35  hk
;;; special Variablen aufgeräumt.
;;;
;;; Revision 1.18  1993/09/06  15:10:32  atr
;;; Die Deklaration der Variablen *current-function* steht jetzt in
;;; static-analysis.
;;;
;;; Revision 1.17  1993/08/25  14:05:10  atr
;;; Die Voranalyse ist jetzt besser. Die Variablen die auf
;;; Funktionsposition aber durch eine Setq modifiziert werden, werden jetzt
;;; auch in den Slot HAS-FUNS-AS-ARGS eingetragen, falls der Wert bei der Setq
;;; eine andere Variable, die im Slot schon vorhanden ist oder eine bekannte Funktion
;;; ist.
;;;
;;; Revision 1.16  1993/07/27  14:01:30  atr
;;; Neue Funktion generates-closures implmentiert,die abfragt
;;; ob in einer Funktion closures erzeugt werden.
;;;
;;; Revision 1.15  1993/07/21  12:27:37  atr
;;; Funktionen besser kommentiert.
;;;
;;; Revision 1.14  1993/07/13  16:22:06  atr
;;; Level bei dynamischen Variablen wird nicht gesetzt.
;;;
;;; Revision 1.13  1993/07/13  14:40:06  hk
;;; Lokale Aenderung von Annouar, die auch den Slot level der Variablen
;;; eines mv-lambda setzt.
;;;
;;; Revision 1.12  1993/07/13  11:08:48  atr
;;; Nun werden die Slots LEVEL bei lokalen Funktionen
;;; und bei Variablen hier gesetzt.
;;;
;;; Revision 1.11  1993/07/06  13:17:44  atr
;;; Unnoetige Ausgabe entfernt.
;;;
;;; Revision 1.10  1993/06/30  16:49:57  atr
;;; Ausgabe -preparing side effect analysis- entfernt.
;;;
;;; Revision 1.9  1993/06/26  16:01:30  atr
;;; (require static-effect) entfernt .
;;;
;;; Revision 1.8  1993/06/26  13:33:33  atr
;;; Neue Methoden geschrieben zur Attributierung der SPEC-DEFINED-FUNS
;;; als Vorbereitung zur Seiteneffektanalyse.
;;;
;;; Revision 1.7  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.6  1993/06/14  10:54:16  atr
;;; Unnoetige Ausgabe entfernt.
;;;
;;; Revision 1.5  1993/06/14  10:47:15  atr
;;; Funktionsdefinitionen verbessert.
;;;
;;; Revision 1.4  1993/06/09  12:21:30  atr
;;; delete durch remove ersetzt, denn delete ist destruktiv.
;;;
;;; Revision 1.3  1993/05/30  14:04:24  atr
;;; Analyse fuer special-defined-functions (also Funktionen
;;; die immer ein oder mehrere Parameter haben , die nur auf Funktions-
;;; position stehen) Diese Funktionen werden anderes analysiert als normale
;;; Funktionen.
;;;
;;; Revision 1.2  1993/04/30  09:37:12  hk
;;; Revision-Keyword eingetragen.
;;;
;;; Revision 1.1  1993/04/27  10:53:56  atr
;;; Initial revision
;;;
;;;--------------------------------------------------------------------------

(in-package "CLICC")

(defmethod get-result-position-forms ((form if-form))
  (append  (get-result-position-forms  (?then form))
           (get-result-position-forms  (?else form))))

(defmethod get-result-position-forms ((form progn-form))
  (get-result-position-forms (car (last (?form-list form)))))

;;;-------------------------------------------------------------------------
;;Der tagbody-Konstrukt liefert immer nil als Resultat.
;;--------------------------------------------------------------------------
(defmethod get-result-position-forms ((form tagbody-form)))

(defmethod get-result-position-forms ((form tagged-form)))


(defmethod get-result-position-forms ((form let*-form))
  (get-result-position-forms (?body form)))

;;;-------------------------------------------------------------------------
;;; Bei einer Switch-form werden alle Ergebnis-formen jeder labelled Form
;;; zusammen mit der Ergebnis-form der Otherwise-form zurueckgegeben.
;;;------------------------------------------------------------------------- 
(defmethod get-result-position-forms ((form switch-form))
  (let ((app-nodes (get-result-position-forms  (?otherwise form))))
    (dolist (one-case (?case-list form) app-nodes)
      (setq app-nodes (append app-nodes 
                              (get-result-position-forms one-case))))))

(defmethod get-result-position-forms ((form labeled-form))
  (get-result-position-forms (?form form)))

(defmethod get-result-position-forms ((form let/cc-form))
  (get-result-position-forms (?body form)))

;;;-------------------------------------------------------------------------
;;; Bei der Applikation einer Continuation oder der Funktion RETURN auf 
;;; Argumente werden die Argumente sonst wird die ganze Applikation 
;;; zur"uckgegeben.
;;;-------------------------------------------------------------------------
(defmethod get-result-position-forms ((form app))
  (list form))

;;;-------------------------------------------------------------------------
;;; Bei einem Labels Konstrukt wird der Rumpf traversiert.
;;;-------------------------------------------------------------------------
(defmethod get-result-position-forms ((form labels-form))
  (get-result-position-forms (?body form)))

;;;-------------------------------------------------------------------------
;;; Hier geht es nicht um Applikationen von Funktionen, sondern um andere 
;;; Angewandte Vorkommen von Funktionen, aber nicht auf Funktionsposition.
;;;-------------------------------------------------------------------------
(defmethod get-result-position-forms ((form fun))
  (list form))
  
(defmethod get-result-position-forms ((form mv-lambda))
  (get-result-position-forms (?body form)))

(defmethod get-result-position-forms ((form setq-form))
  (get-result-position-forms (?form form))) 

(defmethod get-result-position-forms ((a-cont cont))
  (list a-cont))

(defmethod get-result-position-forms ((any-thing form ))
 (unless (null any-thing )
   (list any-thing)))

;;;----------------------------------------------------------------------
;;; get-fun-on-result-position liefert die Liste der Closures die bei 
;;; der gegebenen Form auf Ergebnisposition stehen, wenn vorhanden, sonst
;;; nil.
;;;----------------------------------------------------------------------
(defun get-fun-on-result-position (form)
  (remove-if-not #'fun-p (get-result-position-forms form)))

;;;----------------------------------------------------------------------
;;; The-applied-function nimmt eine app und liefert die applizierte Form
;;; zurueck.
;;;----------------------------------------------------------------------
(defun the-applied-function (app)
  (let ((functional (?form app))
        (arg-list   (?arg-list app)))
    (case functional 
      (defined-fun-p functional)
      
      ;;; Bei einer Applikation durch FUNCALL oder APPLY oder bei einer 
      ;;; Iteration durch MAP* wird die applizierte Form zurueckgegeben
      ;;; und nicht die Funktion Funcall selbst.
      ;;; sonst wird die applizierte Funktion zurueckgegeben.
      ;;;---------------------------------------------------------------
      (imported-fun-p functional)
      (special-sys-fun-p 
       (if  (?special-caller functional )
            (the-applied-function (make-instance 'app
                                                 :form (first arg-list)
                                                 :arg-list 
                                                 (cdr (?arg-list app))))
            functional))
      
      ;;; Bei einer cont-app wird nil zurueckgegeben.
      ;;;--------------------------------------------
      (cont-p nil)
      (app-p  (the-applied-function (?form functional)))
      
      ;;; anderenfalls werden alle Formen gesammelt, die auf 
      ;;; Resultatsposition stehen .
      (t (get-result-position-forms functional)))))

;;;-----------------------------------------------------------------------
;;; Get-applied-functions traversiert eine Form, und liefert alle 
;;; applizierten Formen bei Applikationen, die auf Ergebnisposition stehen.
;;;-----------------------------------------------------------------------
(defun get-applied-functions (form)
  (let ((applied-functions nil))
    (dolist (one-res-form (get-result-position-forms form))
      (when (app-p one-res-form)
        (setq applied-functions (append applied-functions
                                        (the-applied-function one-res-form)))))
    applied-functions))
 
;;;------------------------------------------------------------------------
;;; Analyse der Closures in einem Module.
;;;------------------------------------------------------------------------
(defun get-closures-of-module ()
  (let ((*se-var-env*        nil)
        (*se-vars-to-funs*   nil))
    (mapcar #'get-closures-of-function (?fun-list *module*))))

(defun get-closures-of-function (function)
  (find-closures (?body function)))

(defun generates-closures (function)
  (let ((*upward-fun-args* nil)
        (*down-fun-args*   nil))
    (dolist (one-opt-par (?opt-list (?params function)))
      (find-closures (?init one-opt-par)))
    (dolist (one-key-par (?key-list (?params function)))
      (find-closures (?init one-key-par)))
    (find-closures (?body function))
    (if *upward-fun-args* 
        T
        nil)))

(defmethod find-closures ((ref var-ref)))
  
(defmethod find-closures ((form if-form))
  (find-closures (?pred form))
  (find-closures (?then form))
  (find-closures (?else form)))

(defmethod find-closures ((form progn-form))
  (dolist (one-form (?form-list form))
    (find-closures one-form)))
                
(defmethod find-closures ((form tagbody-form))
  (let ((tagged-form-list (?tagged-form-list form)))
    (dolist (one-tagged-form tagged-form-list)
      (find-closures (?form one-tagged-form)))
    (find-closures (?first-form form))))


(defmethod find-closures ((form tagged-form)))


(defmethod find-closures ((let*form let*-form))
  (find-closures (?body let*form))
  (dolist (one-init-form (?init-list let*form))
    (find-closures one-init-form)))

(defmethod find-closures ((one-class-def class-def) )
  (dolist (one-slot-descr (?slot-descr-list one-class-def))
    (find-closures (?initform one-slot-descr ))))

(defmethod find-closures ((switch switch-form))
  (find-closures (?otherwise switch))
  (let ( (case-list    (?case-list switch)))
    (dolist (one-case case-list)
      (find-closures (?form one-case)))))

(defmethod find-closures ((let/cc let/cc-form))
  (find-closures (?body let/cc)))

(defmethod find-closures ((labels labels-form))
  (dolist (one-local-fun (?fun-list labels))
    (find-closures (?body one-local-fun)))
  (find-closures (?body labels)))


(defmethod find-closures ((an-app app))
  (let* ((functional      (?form     an-app))
         (arg-list        (?arg-list an-app)))
    
    (when  (and (special-sys-fun-p  functional)
                (?special-caller functional))
      (setq functional (first arg-list))
      (setq arg-list (cdr arg-list)))
    (unless  (or (var-ref-p functional)
                 (fun-p functional))
      (let ((down-fun-list (remove-if-not 
                            #'defined-fun-p
                            (get-result-position-forms functional))))
        (setq *down-fun-args* (remove-duplicates 
                               (append 
                                *down-fun-args* 
                                down-fun-list)))))
    (dolist (one-arg arg-list)
      (find-closures one-arg))))

(defmethod find-closures ((function defined-fun))
  (pushnew function *upward-fun-args*))
  
(defmethod find-closures ((function local-fun))
  (pushnew  function *upward-fun-args*)) 

(defmethod find-closures ((a-mv-lambda mv-lambda))
  (find-closures (?body a-mv-lambda))
  (find-closures (?arg  a-mv-lambda)))

(defmethod find-closures ((a-setq setq-form))
  (find-closures (?form a-setq)))


(defmethod find-closures ((a-var var)))

(defmethod find-closures ((a-cont cont)))

(defmethod find-closures ((any-thing-else form)))

;;;-------------------------------------------------------------------------
;;; DIE VORBEREITUNG ZUR SEITENEFFEKTANALYSE.
;;; ES WIRD UNTERSUCHT OB EINER DER PARAMETER EINER FUNKTION  AN 
;;; FUNKTIONSPOSITION STEHT.UND NUR GELESEN WIRD, ODER SO VERAENDERT 
;;; WIRD DASS DER NEUE WERT EIN "AEHNLICHER" PARAMETER ODER EINE BEKANNTE 
;;; FUNKTION IST. APPLIKATIONEN SAEMTLICHER VARIABLEN 
;;; SIND NICHT WIE APPLIKATIONEN VON GLOBALEN VARIABLEN ODER VON 
;;; LOKALEN VARIABLEN, DIE ABER  AN VERSCHIEDENEN STELLEN STEHEN, 
;;; DENN FUER DIE ERSTEN VARIABELN WIRD AN DER AUFRUFSTELLE DER 
;;; FUNKTION DAS FUNKTIONALE OBJEKT UNTERSUCHT, WAEHREND IM ZWEITEN 
;;; FALL MAN NICHT BESTIMMEN KANN WELCHES FUNKTIONALE OBJEKT 
;;; APPLIZIERT WIRD .(DIES KANN NUR MIT HILFE EINER DATENFLUSS-
;;; ANALYSE BESTIMMT WERDEN).
;;;------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Info für die Typinferenz bereitstellen:
;; Wenn 'var' frei in der momentan uebersetzten Funktion vorkommt, wird 'var'
;; unter 'free-lex-vars' vermerkt.
;;------------------------------------------------------------------------------
(defun se-check-if-free-lex-var (var)
  (when (< (?level var) *static-level*)
    (pushnew var (?free-lex-vars *current-function*))))

;;;------------------------------------------------------------------------
;;; Diese methode traversiert jede Funktion, und sammelt alle Variablen
;;; die nur auf Funktionsposition stehen.
;;; Diese Variablen werden zur Zeit in dem Slot HAS-FUNS-AS-ARGS
;;; abgespeichert.
;;;------------------------------------------------------------------------

(defun pre-analyse-module ()
  (let ((*se-fun-counter* 0)
        (*static-level* 0))
    
    (pre-analyse-function (?toplevel-forms *module*))
    (mapc #'pre-analyse-function (?fun-list *module*))
    (clicc-message "~s special functions found " *se-fun-counter*)))

(defun pre-analyse-function  (function)
  (let* ((*current-function*  function)
         (*se-var-env*          (?all-vars (?params function)))
         (*se-vars-to-funs*     nil))
    (get-fun-vars-of-function  function)))


(defun get-fun-vars-of-function (function)
  
  ;; Attributierung der Slots LEVEL bei Funktionen und bei den Variablen.
  ;;---------------------------------------------------------------------
  (dolist (one-var (?all-vars (?params function)))
    (unless (dynamic-p one-var)
      (setf (?level one-var)  *static-level*)))
  
  (get-fun-vars-of-params (?params function))
  (get-fun-vars (?body function))
  
  (when  *se-vars-to-funs*
    
    ;; Die Variablen werden in dem Slot HAS-FUNS-AS-ARGS abgespeichert.
    ;;-----------------------------------------------------------------
    (let ((own-special-vars (remove-if #'(lambda (var)
                                           (< *static-level* (?level var)))
                                       *se-vars-to-funs* ))) 
      (when own-special-vars 
        (setf (?has-funs-as-args function) own-special-vars)
        (incf *se-fun-counter*)))))

(defun get-fun-vars-of-params (parameter)
  (dolist (one-opt (?opt-list parameter))
    (get-fun-vars (?init one-opt)))
  (dolist (one-key (?key-list parameter))
    (get-fun-vars (?init one-key))))


(defmethod get-fun-vars  ((ref var-ref))
  
  ;; freie Variablen für Typinferenz bestimmen
  ;;------------------------------------------
  (when (local-static-p (?var ref))
    (se-check-if-free-lex-var (?var ref))))


(defmethod get-fun-vars ((form if-form))
  (get-fun-vars (?pred form))
  (get-fun-vars (?then form))
  (get-fun-vars (?else form)))

(defmethod get-fun-vars ((form progn-form))
  (dolist (one-form (?form-list form))
    (get-fun-vars one-form)))

(defmethod get-fun-vars ((form tagbody-form))
  (let ((tagged-form-list (?tagged-form-list form)))
    (dolist (one-tagged-form tagged-form-list)
      (get-fun-vars (?form one-tagged-form)))
    (get-fun-vars (?first-form form))))

(defmethod get-fun-vars ((form tagged-form)))


(defmethod get-fun-vars ((let*form let*-form))
  
  ;; Setzen der Slots LEVEL bei den Variablen
  ;;-----------------------------------------
  (dolist (one-var (?var-list let*form))
    (unless (dynamic-p one-var)
      (setf (?level one-var) *static-level*)))
  
  ;; Analyse der Init-formen...
  ;;---------------------------
  (dolist (one-init-form (?init-list let*form))
    (get-fun-vars one-init-form))
  
  ;; Analyse des Rumpfes
  ;;--------------------
  (get-fun-vars  (?body let*form)))

(defmethod get-fun-vars ((one-class-def class-def) )
  (dolist (one-slot-descr (?slot-descr-list one-class-def))
    (get-fun-vars (?initform one-slot-descr ))))

(defmethod get-fun-vars ((switch switch-form))
  (get-fun-vars (?form switch))
  (let ( (case-list    (?case-list switch)))
    (dolist (one-case case-list)
      (get-fun-vars (?form one-case))))
  (get-fun-vars (?otherwise switch)))

(defmethod get-fun-vars ((let/cc let/cc-form))
  (get-fun-vars (?body let/cc)))

;;;---------------------------------------------------------------------------
;;; Bei einem Labels-Ausdruck werden zunaechst die lokale Funktionen
;;; analysiert und attributiert , und dann wird die gerade analysierte 
;;; Funktion weiter analysiert .
;;;--------------------------------------------------------------------------
(defmethod get-fun-vars ((labels labels-form))
  (let ((*static-level* (1+ *static-level*)))
    (dolist (one-local-fun (?fun-list labels))
      (setf (?level one-local-fun) *static-level*)
      (setf (?free-lex-vars one-local-fun) ())
      (pushnew one-local-fun (?local-funs *current-function*))
      (let*  ((*current-function*  one-local-fun)
              (*se-var-env* (append *se-var-env* 
                                    (?all-vars (?params one-local-fun)))))
        (get-fun-vars-of-function  one-local-fun)
        
        ;; Propagieren der FREIEN lokalen lexikalischen Var. für Typinferenz
        ;;------------------------------------------------------------------
        (dolist (local-fun (?local-funs one-local-fun))
          (mapc #'se-check-if-free-lex-var (?free-lex-vars local-fun))))))
  
  (get-fun-vars (?body labels)))


(defmethod get-fun-vars ((an-app app))
  (let* ((functional      (?form     an-app))
         (arg-list        (?arg-list an-app)))
    (block BEGIN
      (when (and (special-sys-fun-p  functional)
                 (?special-caller functional))
        (let ((new-app (make-instance 'app 
                                      :form (car (?arg-list an-app))
                                      :arg-list (cdr (?arg-list an-app)))))
          (get-fun-vars new-app)
          (return-from BEGIN)))
      (when (and (defined-fun-p functional)
                 (?has-funs-as-args functional))
        (dolist (one-spec-var (?has-funs-as-args functional))
          (let ((arg  (get-arg-from-param one-spec-var an-app)))
            (when (and (var-ref-p arg)
                       (member (?var arg) *se-var-env* :test #'eq))
              (pushnew (?var arg) *se-vars-to-funs*)))))

      (when (and (imported-fun-p functional)
                 (?has-funs-as-args functional))
        (dolist (one-spec-var (?has-funs-as-args functional))
          (let ((arg  (get-arg-from-coded-param one-spec-var an-app)))
            (when (and (var-ref-p arg)
                       (member (?var arg) *se-var-env* :test #'eq))
              (pushnew (?var arg) *se-vars-to-funs*)))))

      (when  (var-ref-p functional)
        (let (( var (?var functional)))
          (when (and 
                 (member var  *se-var-env* :test #'eq)
                 (static-p (?var functional)))
            (pushnew var  *se-vars-to-funs*))))
      (get-fun-vars functional)
      (dolist (one-arg arg-list)
        (get-fun-vars one-arg)))))

(defmethod get-fun-vars ((function defined-fun)))

(defmethod get-fun-vars ((function local-fun)))



(defmethod get-fun-vars ((a-mv-lambda mv-lambda))
  (dolist (one-var (?all-vars (?params a-mv-lambda)))
    (unless (dynamic-p one-var)
      (setf (?level one-var) *static-level*)))
  (get-fun-vars (?body a-mv-lambda))
  (get-fun-vars (?arg  a-mv-lambda)))

#+CMU
(defun all-vars-or-funs (list-of-forms)
  (let (one-form )
    (loop 
     (if (endp list-of-forms)
         (return T)
         (progn 
           (setq one-form (pop list-of-forms))
           (unless  (or (equal empty-list one-form)
                        (and (var-ref-p one-form)
                             (member (?var one-form) 
                                     *se-var-env* :test #'eq))
                        (simple-fun-p one-form)
                                   
                        (and (app-p one-form)
                             (eq  (?form one-form) 
                                  (get-function 
                                   'clicc-lisp::error))))
             (return nil)))))))

;;;------------------------------------------------------------------------
;;; Wenn eine Variable neu definiert wird, wird der zugewiesene Wert 
;;; untersucht, und entsprechend wird entschieden ob  sie nicht in der 
;;; Umgebung bleibt.
;;;------------------------------------------------------------------------
(defmethod get-fun-vars ((a-setq setq-form))
  (labels (#-CMU
           (all-vars-or-funs (list-of-forms)
             (let (one-form )
               (loop 
                (if (endp list-of-forms)
                    (return T)
                    (progn 
                      (setq one-form (pop list-of-forms))
                      (unless  (or (equal empty-list one-form)
                                   (and (var-ref-p one-form)
                                        (member (?var one-form) 
                                                *se-var-env* :test #'eq))
                                   (simple-fun-p one-form)
                                   
                                   (and (app-p one-form)
                                        (eq  (?form one-form) 
                                             (get-function 
                                              'clicc-lisp::error))))
                        (return nil))))))))
    
    ;; freie Variablen für Typinferenz bestimmen
    ;;------------------------------------------
    (let ((var (?var (?location a-setq))))
      (when (local-static-p var)
        (se-check-if-free-lex-var var))
      
      (let* ((form-of-setq (?form a-setq))
             (result-forms (get-result-position-forms form-of-setq)))
        (unless   (all-vars-or-funs  result-forms)
          ;; Wenn alle Formen auf Resultatsposition Variablen aus meiner
          ;; Umgebung oder bekannte Funktionen sind, dann bleibt diese 
          ;; Variable in der Umgebung erhalten, sonst wird sie entfernt.
          ;;-------------------------------------------------------------
          (setq *se-var-env* 
                (remove var *se-var-env* :test #'eq))
          (setq *se-vars-to-funs* 
                (remove var *se-vars-to-funs* :test #'eq)))
        (get-fun-vars form-of-setq)))))

(defmethod get-fun-vars ((a-cont cont)))


(defmethod get-fun-vars ((any-thing-else form )))

;;;-------------------------------------------------------------------------
(provide "closure-analysis")    
        








