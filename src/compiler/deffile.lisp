;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Bearbeiten der Interface-Beschreibungen fuer Modulkompilation
;;;            (.def Dateien)
;;;            - exportierte Funktionen
;;;            - exportierte Symbole
;;;
;;; $Revision: 1.32 $
;;; $Log: deffile.lisp,v $
;;; Revision 1.32  1993/12/22  09:22:14  hk
;;; F¸r CMU17 m¸ssen bei make-instance Symbole statt Klassen verwendet
;;; werden.
;;;
;;; Revision 1.31  1993/12/19  14:08:46  hk
;;; In import-fun stellt nun sicher, daﬂ wirklich alle :mv-spec :t zu
;;; :mv-spec T werden.
;;;
;;; Revision 1.30  1993/12/16  09:36:53  hk
;;; In das .def File wird nicht das Symbol T sondern das Keyword :T
;;; geschrieben, um Probleme mit packages zu vermeiden. Beim Einlesen wird
;;; wieder zu T ¸bergegangen.
;;;
;;; Revision 1.29  1993/12/09  14:36:42  hk
;;; Beim Lesen eines .def Files mittels import-read wird sichergestellt,
;;; daﬂ das verwendete Package bei Bedarf mit make-package generiert wird.
;;;
;;; Revision 1.28  1993/12/03  09:59:37  ft
;;; Aufrufe von make-instance in import-fun und import-sym optimiert.
;;;
;;; Revision 1.27  1993/08/19  15:22:11  hk
;;; get-symbol-bind statt eines expliziten find in *GLOBAL-ENVIRONMENT*
;;; verwendet.
;;;
;;; Revision 1.26  1993/08/19  10:34:16  hk
;;; Auch Funktionen mit Namen (setf xxx) kˆnne special-sys-fun sein.
;;;
;;; Revision 1.25  1993/07/27  14:12:14  atr
;;; Import-fun leicht geaendert.
;;;
;;; Revision 1.24  1993/07/20  13:26:49  uho
;;; 'import-read' auf 'imported-module'-Zwischensprachkonstrukt
;;; umgestellt.
;;;
;;; Revision 1.23  1993/07/20  12:09:58  atr
;;; In der Beschreibung einer Funktion stehen unter dem Key
;;; :has-funs-as-args nicht defined-syms sondern die Symbole (lisp symbole).
;;; Beim Importieren werden die defined-syms in der *global-environment* gesucht.
;;;
;;; Revision 1.22  1993/07/19  15:04:22  uho
;;; In .def-Files wird jetzt unter dem Schluesselwort :NAME zusaetzlich
;;; zum Namen des Moduls auch der Namen seiner Initialisierungsfunktion
;;; und die Symbol-base (der Name der Tabelle fuer die Symbole)
;;; abgelegt. 'import-read' liefert diese drei Namen nun als multiplen
;;; Wert und der Modul-Slot 'loaded-modules' enthaelt nun eine Liste
;;; dreielementiger Listen der Form: (Modulname, Initfunktionsname, Symbolbase).
;;;
;;; Revision 1.21  1993/07/19  11:28:26  atr
;;; Slot HAS-FUNS-AS-ARGS steht in der fun-descr einer
;;; exportierten Funktion. Der Slot wird bei der Seiteneffektanalyse
;;; gebraucht.
;;;
;;; Revision 1.20  1993/07/14  17:17:26  atr
;;; In der .def Datei werden die Symbole zunaechst ausgeschrieben,
;;; und dann die Funktionen.
;;; Einlesen der Beschreibungen von Funktionen korrigiert.
;;;
;;; Revision 1.19  1993/06/27  12:14:33  atr
;;; READ und WRITE-LIST bei imported-fun sind jetzt
;;; Listen von Variablen und nicht mehr von Symbolen der Variablen.
;;;
;;; Revision 1.18  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.17  1993/06/12  15:57:01  atr
;;; Tippfehler korrigiert data-effect --> data-effects.
;;;
;;; Revision 1.16  1993/06/09  12:16:56  ft
;;; Fehlende find-class bei make-instance eingef¸gt.
;;;
;;; Revision 1.15  1993/06/07  07:24:25  hk
;;; Schreibfehler.
;;;
;;; Revision 1.14  1993/06/05  21:38:54  hk
;;; Fehler in export-fun behoben.
;;;
;;; Revision 1.13  1993/06/04  14:49:53  hk
;;; :unknown von read- und write-list kann unveraendert geschrieben werden.
;;;
;;; Revision 1.12  1993/06/04  11:10:43  hk
;;; In :read-list und :write-list werden nun Symbole und nicht
;;; Zwischensprachausdruecke angegeben.
;;;
;;; Revision 1.11  1993/06/04  08:32:42  hk
;;; In export-fun: '@ -> ,@ und ' -> ,
;;;
;;; Revision 1.10  1993/05/30  13:59:57  atr
;;; Jetzt werden die Slots read-list , write-list und
;;; data-effects auch ausgeschrieben.
;;;
;;; Revision 1.9  1993/05/14  12:15:59  hk
;;; In .def Files wird ein 2-zeiliger Kommentar geschrieben.
;;;
;;; Revision 1.8  1993/05/14  09:11:00  hk
;;; *package* nicht mit NIL initialisieren.
;;;
;;; Revision 1.7  1993/04/22  12:20:40  hk
;;; export-write beruecksichtigt *OUT-FILENAME*.
;;; Verschiedenartige Eintraege in .def-Files werden durch Keywords
;;; eingeleitet. import-fun beruecksichtigt special-sys-funs.
;;; Bei importierten Symbolen wird Slot base gesetzt.
;;; Neuer Eintrag :sys fuer sys.def: keinen Aufruf von
;;; Initialisierungsfunktion generieren. Neuer Eintrag :package, der
;;; den Namen des package des Moduls angibt.
;;;
;;; Revision 1.6  1993/04/15  16:09:56  hk
;;; Fehler behoben.
;;;
;;; Revision 1.5  1993/04/14  12:20:46  hk
;;; Nur Funktionen mit ?exported = T werden geschrieben.
;;;
;;; Revision 1.4  1993/04/14  10:26:12  hk
;;; import-fun: beruecksichtigt (setf f).
;;;
;;; Revision 1.3  1993/04/08  07:37:39  hk
;;; (provide DEFFILE) -> (provide deffile).
;;;
;;; Revision 1.2  1993/04/03  09:53:08  hk
;;; Alten Dateinamen entfernt.
;;;
;;; Revision 1.1  1993/04/03  09:49:12  hk
;;; Initial revision
;;;
;;;-----------------------------------------------------------------------------

(in-package "CLICC")     

;; Exportierte importierte Funktionen sind in waehrend der Codegenerierung gar
;; nicht zugreifbar. Ihre Spezifikation muesste also nach dem Einlesen in
;; Pass1 bereits wieder herausgeschrieben werden. Die Export-Anweisung erfolgt
;; evtl. erst nach dem Einlesen der Spezifikation, also kann das Schreiben der
;; Spezifikation erst am Ende von Pass1 erfolgen. Dann sind die importierten
;; Funktionen noch im global Environment zugreifbar.  Da (load ..) transitiv
;; ist, werden die exportierten Funktionen des Moduls, das vom aktuellen Modul
;; geladen wird, automatisch bekanntgegeben. Es muss nur durch geeignete
;; Packageoperationen sichergestellt werde, dass diese Funktionen mit beiden
;; Packagenamen qualifiziert werden darf.

;; Das gleiche gilt fuer exportierte importierte Symbole.

;; C-Namen von Funktionen aus verschiedenen Modulen: Wenn in zwei Modulen
;; Funktionen mit gleichem Namen definiert werden, dann ist es schwierig, zwei
;; eindeutige C-namen daraus zu generieren. Als Alternativen bieten sich an:
;; 1) den Packagenamen voranstellen, oder den kuerzesten Nickname, fuer Lisp
;; evtl. kein Praefix, 2) einen eindeutigen Praefix voranstellen, das
;; erfordert jedoch Benutzerinteraktion, da verschiedene Praefixe bei
;; getrennter Modulkompilation nicht automatisch bestimmt werden koennen, 3)
;; generieren des C-namens direkt aus dem Lisp-Namen unter der Annahme, dass
;; keine Namenskonflikte auftreten, diese Annahme kann beim Importieren der
;; Module automatisch geprueft werden.

;;------------------------------------------------------------------------------
;; Berechnet den Namen der .def Datei aus dem Namen eines Moduls 
;;------------------------------------------------------------------------------
(defun calc-def-filename (name)
  (concatenate 'string name ".def"))

;;------------------------------------------------------------------------------
;; Kodiert die Liste der Variablen im Slot has-funs-as-args einer exported-fun.
;; Keyword Parameter werden durch das Keyword Symbol ersetzt , required
;; und optional Parameter werden durch ihre Position in der Parameterliste 
;; ersetzt.
;;------------------------------------------------------------------------------
(defun update-and-encode (function)
  (when (?has-funs-as-args function)
    (let ((code-list nil)
          (all-vars (?all-vars (?params function)))
          (key-var-list (mapcar #'?var (?key-list (?params function)))))
      (dolist (one-spec-var (?has-funs-as-args function))
        (when (and (static-p one-spec-var)
                   (member one-spec-var all-vars
                           :test #'eq)
                   (not (eq (?rest (?params function)) one-spec-var)))
          (if (member one-spec-var key-var-list :test #'eq)
              (push (?symbol (?sym (find one-spec-var (?key-list 
                                              (?params function))
                                         :key #'?var))) code-list)
              (push (position one-spec-var all-vars :test #'eq) code-list))))
      (setf (?has-funs-as-args function) code-list))))

;;------------------------------------------------------------------------------
;; Beschreibung einer exportierten Funktion in die .def Datei schreiben
;; Keine Symbole auﬂer Keywords schreiben, um Probleme mit Packages zu vermeiden
;;------------------------------------------------------------------------------
(defun export-fun (fun)
  (when (?exported fun)
    (update-and-encode fun)
    (print `(,(?symbol fun) :par-spec ,(?par-spec fun) :adr ,(?adr fun)
             ,@(unless (eql (?mv-spec fun) 1)
                       `(:mv-spec ,(if (eq (?mv-spec fun) 'T)
                                       :T
                                       (?mv-spec fun))))
             ,@(unless (eq (?read-list fun) ()) 
                 `(:read-list ,(if (atom (?read-list fun))
                                   (?read-list fun)
                                   (mapcar #'(lambda (x) (?symbol (?sym x)))
                                           (?read-list fun)))))
             ,@(unless (eq (?write-list fun) ())
                 `(:write-list ,(if (atom (?write-list fun))
                                    (?write-list fun)
                                    (mapcar #'(lambda (x) (?symbol (?sym x)))
                                            (?write-list fun)))))
             ,@(unless (eq (?data-effects fun) nil)
                 `(:data-effects ,(?data-effects fun)))
             ,@(unless (eq (?has-funs-as-args fun) nil)
                 `(:has-funs-as-args ,(?has-funs-as-args fun)))))))



;;------------------------------------------------------------------------------
;; Beschreibung eines exportierten Symbols in die .def Datei schreiben
;;------------------------------------------------------------------------------
(defun export-sym (sym)
  (when (?exported sym)
    (print `(,(?symbol sym) :adr ,(?adr sym)
             ,@(unless (eq :no-const (?constant-value sym))
                 '(:constant-value :unknown))))))

;;------------------------------------------------------------------------------
;; Schreiben der .def Datei
;; Die Symbole zur Benennung der Beschreibungen werden im aktuellen Package,
;; also ohne Package-Qualifizierer ausgegeben.
;;------------------------------------------------------------------------------
(defun export-write ()
  (with-open-file (*standard-output* (calc-def-filename *OUT-FILENAME*)
                                     :direction :output
                                     :if-exists :supersede)
    (let ((*package* (?package *module*))
          (*print-circle* nil)
          (*print-length* nil)
          (*print-level* nil))
      (format T ";;; This is a CLICC-generated definition header file.~%")
      (format T ";;; It contains exports of module ~A~%"
              (package-name *package*))
      (print :name)
      (print (?name *module*))
      (print (?init-fun-name *module*))
      (print (?symbol-base *module*))
      (print :package)
      (print (package-name (?package *module*)))
      (print :sym)
      (mapc #'export-sym (?sym-list *module*))
      (terpri)
      (print :fun)
      (mapc #'export-fun (?fun-list *module*))
      (terpri)
      (print :eof))))

;;------------------------------------------------------------------------------
;; Die Beschreibung einer Funktion enthaelt, in den Slots READ und WRITE
;; sowohl imported-symbols als auch defined-symbols.
;; Bei imported-symbols liefert "get-global-dynamic" die entsprechende 
;; globale Variable zurueck.
;; Bei defined-symbols liefert "imported-get-global-dynamic" eine
;; neue dynamische Variable zurueck, die nur in der Beschreibung
;; der Funktion referenziert wird.
;;------------------------------------------------------------------------------

(defun imported-get-global-dynamic (symbol)
  (let ((sym (make-instance 'imported-sym
                            :symbol symbol 
                            :constant-value :unknown)))
    (make-instance 'dynamic :sym sym))) 

;;------------------------------------------------------------------------------
;; Liefert die dem Symbol entsprechende dynamische Variable , wenn das 
;; Symbol imported-sym ist, sonst eine neue dynamische Variable 
;; die einen Zeiger auf das Symbol enthaelt.
;;------------------------------------------------------------------------------
(defun get-dyn (sym)
  (let ((bind (find-global-dynamic sym)))
    (if bind
        (cdr bind)
        (imported-get-global-dynamic sym))))

;;------------------------------------------------------------------------------
;; Beschreibung einer importierten Funktion bearbeiten
;;------------------------------------------------------------------------------
(defun import-fun (fun-descr)
  (multiple-value-bind (indicator value tail)
      (get-properties (cdr fun-descr) '(:mv-spec))
    (when (eq value :T)
      (setf (second tail) 'T)))
  (let* ((name (car fun-descr))
         (fun (apply #'make-instance
                     (get (if (consp name) (second name) name)
                          'zws-type
                          #-CMU17 (find-class 'imported-fun)
                          #+CMU17 'imported-fun)
                     :symbol name
                     (cdr fun-descr))))
    (when (not (slot-boundp fun 'adr))
      (cg-set-C-name fun))
    (let ((read-list  (cadr (member :read-list  fun-descr))))
      (if  (atom read-list)
           (setf (?read-list fun) read-list)
           (setf (?read-list fun)
                 (mapcar #'get-dyn read-list))))
    (let ((write-list (cadr (member :write-list fun-descr))))
      (if  (atom write-list)
           (setf (?write-list fun) write-list)
           (setf (?write-list fun)
                 (mapcar #'get-dyn write-list))))
    (setf (?has-funs-as-args fun)
          (mapcar #'(lambda (elem)
                      (if (integerp elem)
                          elem
                          (get-symbol-bind elem)))
                  (cadr (member :has-funs-as-args fun-descr))))
    (if (consp name)
        (set-imported-setf-fun name fun)
        (set-imported-fun name fun))))

;;------------------------------------------------------------------------------
;; Beschreibung eines importierten Symbols bearbeiten
;;------------------------------------------------------------------------------
(defun import-sym (sym-descr sym-base)
  (let ((sym
         (apply #'make-instance
                #-CMU17 (find-class 'imported-sym) #+CMU17 'imported-sym
                :symbol (car sym-descr)
                :base sym-base
                (cdr sym-descr))))
    (bind-symbol sym)))

;;------------------------------------------------------------------------------
;; Lesen der .def Datei
;; Die Symbole zur Benennung der Beschreibungen werden in das Package des Moduls
;; eingelesen.
;;------------------------------------------------------------------------------
(defun import-read (module-name)
  (with-open-file (*standard-input* (calc-def-filename module-name)
                                    :direction :input)
    
    ;; Package wird spaeter durch :package spezifiziert, waehle zunaechst
    ;; *keyword-package* als unschaedlichen Wert
    ;;-------------------------------------------------
    (let ((*package* *keyword-package*)
          (name nil)
          (init-fun-name nil)
          (symbol-base nil)
          (sys nil)
          (type (read))
          (new-module nil))
                      
      (loop
       (case type
         (:sys  (setq sys (read))
                (setq type (read)))
         (:name (setq name (read))
                (setq init-fun-name (read))
                (setq symbol-base (read))
                (setq type (read)))
         (:package (let ((name (read)))
                     (setq *package* (or (find-package name)
                                         (make-package name :use ()))))
                   (setq type (read)))
         (:fun (do ((fun-descr (read) (read)))
                   ((atom fun-descr) (setq type fun-descr))
                 (import-fun fun-descr)))
         (:sym (if (null symbol-base)
                   (internal-error
                    "import-read"
                    ":NAME must preceede :SYM in ~A~%"
                    (calc-def-filename module-name))
                   (do ((sym-descr (read) (read)))
                       ((atom sym-descr) (setq type sym-descr))
                     (import-sym sym-descr symbol-base))))
         (:eof (return))
         (T (internal-error "import-read" "Undefined keyword: ~A" type))))

      ;; Importiertes Modul erzeugen
      ;;----------------------------
      (setq new-module
            (make-imported-module :name name
                                  :init-fun-name init-fun-name
                                  :symbol-base symbol-base))

      ;; Falls kein Name angegeben ist, dann ist es sys.def
      ;;---------------------------------------------------
      (unless sys
        (addnew-q new-module (?loaded-modules *module*)))

      new-module)))

;;------------------------------------------------------------------------------
(provide "deffile")
