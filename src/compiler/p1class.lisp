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
;;; Funktion : Pass 1 der Klassenverarbeitung, sowie der Instantierung
;;;
;;; $Revision: 1.66 $
;;; $Id: p1class.lisp,v 1.66 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Die Expansionsfunktion fuer das Compiler-Macro zu make-instance.
;; (make-instance 'constant &rest args) ->
;; (make-instance (find-class 'constant) &rest args)
;; sonst unveraendert.
;;------------------------------------------------------------------------------
(defun p1-make-instance (form)
  (match-args
   form MAKE-INSTANCE (class-specifier . keys)
   (if (quote-form-p class-specifier)
       `(L::make-instance
         (L::find-class ,class-specifier) . ,keys)
       form)))

(defun bind-forward-class (class-name)
  (let ((class (make-instance 'defined-class :symbol class-name)))
    (set-forward-class-entry class-name class)
    (get-class-entry class-name)))

;;------------------------------------------------------------------------------
;; p1-find-class expandiert den Aufruf zu einer Klasse
;;------------------------------------------------------------------------------
(defun p1-find-class (args)
  (let ((class-specifier (first args)))
    (if (quote-form-p class-specifier)
        (let ((class-entry 
               (or (get-class-entry (second class-specifier))
                   (bind-forward-class (second class-specifier)))))
          (if class-entry
              (?class-def (cdr class-entry))
              (clc-error "~A is not a valid argument for FIND-CLASS." 
                         class-specifier)))
        (clc-error "~A is not a valid argument for FIND-CLASS." 
                   class-specifier))))

;;------------------------------------------------------------------------------
;; p1-defclass legt das entsprechende Zwischensprachkonstrukt an
;; die Vererbung wird erst am Ende von Pass 1 durchgefuehrt
;;------------------------------------------------------------------------------
(defun p1-defclass (all-args)
  (block define-class
  
    ;; erste Parameterpruefung
    ;;------------------------
    (unless (and (listp all-args) (>= (length all-args) 3)
                 (listp (second all-args)) (listp (third all-args))
                 (every #'listp (cdddr all-args)))
      (clicc-error
       ILLEGAL_CALL "DEFCLASS"
       "CLASS-NAME ({SUPERCLASS-NAME}*) ({SLOT-SPECIFIER}*) [CLASS-OPTION]"))

    (multiple-value-bind
        (class-name superclass-names slot-specifier default-initargs)
        (parse-defclass-args all-args)
    
      (let ((class-entry                ; Klasseninfo aus dem global-env
             (get-class-entry class-name)) 
            class                       ; Instanz von class-def
            superclasses                ; Superklassen-Referenzen
            slot-descriptions           ; Liste von slot-desc Instanzen
            (*CURRENT-FORM* class-name))
      
        ;; gebe Meldung aus
        ;;-----------------
        (clicc-message "Analyse DEFCLASS     ~A" class-name)

        (when default-initargs
          (clc-error "class-option :DEFAULT-INITARGS will be ignored"))

        ;; Pruefen ob die Klasse schon definiert/referenziert  wurde
        ;; sonst neue Instanz von defined-class erzeugen
        ;;----------------------------------------------
        (case (car class-entry)
          ((:TYPE :STRUCT)              ; Name schon vergeben
           (clc-error REDEF_CLASS class-name)
           (return-from define-class))
          (:CLASS                       ; Klasse schon referenziert/definiert
           (cond ((?forward (cdr class-entry))
                  (setf (?forward (cdr class-entry)) nil
                        class (?class-def (cdr class-entry))))
                 (T (clc-error REDEF_CLASS class-name)
                    (return-from define-class))))
          (otherwise                    ; kein Eintrag im global-env!
           (setf class (make-instance 'defined-class :symbol class-name))
           (set-unfinalized-class-entry class-name class nil nil)
           (setf class-entry (get-class-entry class-name))))

        ;; Parameteranalyse z.B Ermittlung der Metaklasse
        ;;-----------------------------------------------
        (setf superclass-names (remove 'T superclass-names))
        (let ((red-superclass-names (remove-duplicates superclass-names)))
          (when (< (length red-superclass-names)
                   (length superclass-names))
            (clc-error "Some classes mentioned multiple times in ~
                        superclass-list. They will be ignored.")
            (setf superclass-names red-superclass-names)))
        (setf superclasses              ; suche def. Klassen/erzeuge Vorw.-Ref.
          (mapcar #'(lambda (superclass-name)
                      (let ((superclass-def (get-class-entry superclass-name))
                            superclass)
                        (ecase (car superclass-def)
                          ((:TYPE :STRUCT :BUILT-IN)
                           (clc-error 
                            "~S is not a legal superclass, using T instead" 
                            superclass-name)
                           (setf superclass 
                             (?class-def (cdr (get-class-entry 'T)))))
                          (:CLASS
                           (setf superclass 
                             (?class-def (cdr superclass-def))))
                          ((nil)
                           (setf superclass
                             (make-instance 'defined-class
                               :symbol superclass-name))
                           (set-forward-class-entry superclass-name
                                                    superclass)))
                        superclass))
                  superclass-names))
       
        ;; hier slot-descriptions aus slot-specifier erzeugen
        ;; spielen hier auch die default-initargs rein ???
        ;;------------------------------------------------
        (setf slot-descriptions (generate-slot-descs slot-specifier))
        (maplist #'(lambda (list) 
                     (when (member 
                            (?symbol (first list)) (rest list) :key #'?symbol)
                       (clc-error 
                        "The slot ~S has been declared twice in class ~S."
                        (?symbol (first list)) class-name)))
                (mapcar #'car slot-descriptions))

        ;; trage die Zugriffsmethoden im class-entry ein
        ;;----------------------------------------------
        (setf (?reader (cdr class-entry))
          (mapcar #'(lambda (slot-description)
                      (cons (?symbol (first slot-description)) 
                            (second slot-description)))
                  slot-descriptions))
        (setf (?writer (cdr class-entry))
          (mapcar #'(lambda (slot-description)
                      (cons (?symbol (first slot-description))
                            (third slot-description)))
                  slot-descriptions))

        ;; fuelle Slots der Instanz
        ;;-------------------------
        (setf (?super-list class)      superclasses
              (?slot-descr-list class) (mapcar #'car slot-descriptions))
      
        ;; Rueckgabewert=Instanz...kein einheitlicher Rueckgabewert fuer def... 
        ;;---------------------------------------------------------------------
        class
        ))))

;;------------------------------------------------------------------------------
;; parse-defclass-args: Parser Funktion fuer die Parameter des macros defclass
;;------------------------------------------------------------------------------
(defun parse-defclass-args (all-args)
  (let ((class-name                     ; Name der Klasse als Symbol
         (pop all-args))
        (defined-superclasses           ; Liste definierter Superklassen
            (pop all-args))
        ;; Die folgenden drei koennen auch nil enthalten
        (superclasses nil)              ; Liste aller Superklassen
        (slot-specifier                 ; definierte Slot Spezifizierer
         (pop all-args))                              
        (default-initargs nil))
    (setf superclasses defined-superclasses) ; vorlaeufig !!!
    (dolist (option all-args)
      (case (first option)
        (:default-initargs (setf default-initargs (cdr option)))
        (:documentation nil)            ; will be ignored
        (:metaclass
         (clc-error "The class-option ~A is not a part of the ~
                         implemented language ~%and  will be ignored."
                    (first option)))                          
        (otherwise
         (clc-error "The ~A is not a valid class-option and will be ignored."
                    (first option)))))
    (values class-name superclasses slot-specifier default-initargs)))

;;------------------------------------------------------------------------------
;; Funktionen zum Erzeugen von Instanzen von slot-desc aus einer
;; slot-specifier Liste
;;------------------------------------------------------------------------------
(defun generate-slot-descs (slot-specifier-list)
  (mapcar #'generate-slot-desc slot-specifier-list))

(defun generate-slot-desc (slot-specifier)
  (labels ((s-fun-p (f) (and (consp f) (eq (first f) 'function))))
    (multiple-value-bind 
          (symbol allocation initargs initform type reader writer)
                                        ;other-options!
        (parse-slot-specifier (if (consp slot-specifier) 
                                  slot-specifier 
                                  (list slot-specifier)))
      (multiple-value-bind
            (evaluated-initform evaluated)
          (cond
            ((eq initform :unbound) (values initform T))
            ((s-fun-p initform)                        (values initform nil))
            (T                                         (p1-eval initform)))
        (let* ((*SDF* T)
               (initfunction-name (intern (concatenate 'string 
                                                       (string symbol)
                                                       "-initfunction" 
                                                       (string (gensym)))))
               (processed-initform
                (cond
                  ((eq evaluated-initform :unbound)
                   evaluated-initform)
                  (evaluated 
                   (p1-form
                    `(L::QUOTE ,evaluated-initform)))
                  (T
                   (progn 
                     (p1-defun `(,initfunction-name () ,initform))
                     (p1-form  `#',initfunction-name)))))
               (slot-description (make-instance 'slot-desc
                                                :symbol symbol
                                                :initform processed-initform
                                                :initargs 
                                                (mapcar #'p1-make-symbol
                                                        initargs)
                                                :allocation allocation
                                                :declared-type type)))
          (list slot-description reader writer))))))

;;------------------------------------------------------------------------------
;; parse-slot-specifier: Parser fuer den Slot-Specifier eines DEFCLASS
;;------------------------------------------------------------------------------
(defun parse-slot-specifier (slot-specifier)
  (let ((symbol (car slot-specifier))
        (type T)
        (initform :unbound)
        (allocation ':instance)
        ;; fuer ^^^ koennte man noch einen Fehler bei mehrfacher Angabe ausgeben
        (reader (empty-queue))
        (writer (empty-queue))
        (initargs (empty-queue))
        (other-options (empty-queue))
        (last-key nil))
    (do ((options (cdr slot-specifier) (cddr options)))
        ((null options))
      (case (car options)
        (:reader
         (add-q (cadr options) reader)
         (setf last-key ':reader))
        (:writer
         (add-q (cadr options) writer)
         (setf last-key ':writer)) 
        (:accessor
         (add-q (cadr options) reader)
         (add-q (list 'L::setf (cadr options)) writer)
         (setf last-key ':accessor))
        (:allocation
         (setf allocation (cadr options))
         (setf last-key nil))
        (:initarg
         (add-q (cadr options) initargs)
         (setf last-key ':initarg))
        (:initform
         (setf initform (cadr options))
         (setf last-key nil))
        (:type
         (setf type (cadr options))
         (setf last-key nil))
        (otherwise
         (case last-key
           (:reader
            (add-q (cadr options) reader))
           (:writer
            (add-q (cadr options) writer))
           (:accessor
            (add-q (cadr options) reader)
            (add-q (list 'L::setf (cadr options)) writer))
           (:initarg
            (add-q (cadr options) initargs))
           (otherwise
            (add-q (car options) other-options)
            (add-q (cadr options) other-options))))))
    (when (> (length (queue2list initargs)) 1)
      (clc-error "There is more than one initarg specified for slot ~A, ~%~
                 continue to ignore all but the first." symbol))
    (values
     symbol 
     allocation 
     (queue2list initargs) 
     initform 
     type
     (queue2list reader) 
     (queue2list writer) 
     (queue2list other-options))))

;;------------------------------------------------------------------------------
;; finalize-class(es): Berechnung d. class-precedence-lists, Vererbung
;; Der Aufruf muss am Anfang von p1-end stattfinden, da indirekt p1-defun
;; aufgerufen wird.
;;------------------------------------------------------------------------------

(defun finalize-classes ()
  (let ((all-Rc '())                    ; fuer die lokalen Ordnungen Rc
        (class-message-flag nil)
        (built-in-message-flag nil))
    (labels

        ;;----------------------------------------------------------------------
        ;; finalize-class: Finalisiert eine Klasse 
        ;;----------------------------------------------------------------------
        ((finalize-class (class)

           ;; gebe eine Meldung aus (einmal)
           ;;-------------------------------
           (unless class-message-flag
             (setf class-message-flag T)
             (clicc-message "Finalizing classes ..."))
                     
           (catch 'CLICC-ERROR
             (let* ((class-name (?symbol class))
                    (class-entry (cdr (get-class-entry class-name)))
                    (*CURRENT-FORM* class-name))    
             
               ;; pruefe ob diese Klasse nicht schon finalized ist
               ;;-------------------------------------------------
               (if (?forward class-entry)
                   (progn
                     (clc-error "Class ~S is referenced, but not defined." 
                                class-name)

                     ;; falls obiger Fehler fortgesetzt wird muessen Defaults
                     ;; fuer die Klasse angenommen werden
                     ;;----------------------------------
                     (setf (?forward class-entry) nil)
                     (setf (?finalized class-entry) nil)
                     (setf (?class-precedence-list (?class-def class-entry))
                           (make-instance 'structured-literal
                                          :value
                                          (list (?class-def class-entry))
                                          :needs-fixnum-array nil
                                          :needs-float-array nil))
                     (setf (?slot-descr-list (?class-def class-entry)) nil)
                     (add-q (?class-def class-entry)
                            (?class-def-list *module*)))
                   
                   (unless (?finalized class-entry)

                     ;; finalisiere die Oberklassen
                     ;;----------------------------
                     (dolist (superclass (?super-list class))
                       (finalize-class superclass))
               
                     ;; Berechnung der class-precedence-list
                     ;;-------------------------------------
                     (setf (?class-precedence-list class)
                           (compute-c-p-l class))
                               
                     ;; vererbe die Slots aus den Oberklassen
                     ;;--------------------------------------
                     (dolist (superclass 
                               (butlast (cdr (?class-precedence-list class))))
                       (dolist (slot-desc (reverse
                                           (?slot-descr-list superclass)))
                         (unless (member (?symbol slot-desc) 
                                         (mapcar #'?symbol 
                                                 (?slot-descr-list class)))
                           (let ((superclass-entry 
                                  (cdr (get-class-entry 
                                        (?symbol superclass))))
                                 (new-slot (copy-slot-desc slot-desc)))
                              (setf (?inherited-from new-slot)
                                    (?class-def superclass-entry))
                              (setf (?slot-descr-list class)
                                    (cons new-slot (?slot-descr-list class)))
                              (push 
                               (cons (?symbol slot-desc) 
                                     (copy-list 
                                      (cdr (assoc (?symbol slot-desc) 
                                                  (?reader 
                                                   superclass-entry)
                                                  :test #'eq))))
                               (?reader class-entry))
                              (push 
                               (cons (?symbol slot-desc) 
                                     (copy-list 
                                      (cdr (assoc (?symbol slot-desc) 
                                                  (?writer 
                                                   superclass-entry)
                                                  :test #'eq))))
                               (?writer class-entry))))))
                     
                     ;; Vergabe der offsets fuer die Slots
                     ;;-----------------------------------
                     (let ((offset 0))
                       (dolist (slot-desc (?slot-descr-list class))
                         (setf (?offset slot-desc) offset)
                         (setf offset (1+ offset))))
               
                     ;; ueberfuehre die Komponenten in Zwischensprache
                     ;; dabei kann es nicht geschehen, dass der Aufruf
                     ;; BUTLAST NIL liefert, da das Resultat mind. eine
                     ;; Liste mit der Klasse selbst ist
                     ;;-----------------------------------------------
                     (setf (?class-precedence-list class)
                           (make-instance 'structured-literal
                                          :value 
                                          (butlast 
                                           (?class-precedence-list class))
                                          :needs-fixnum-array nil
                                          :needs-float-array nil))

                     ;; markiere die Klasse als finalized
                     ;;----------------------------------
                     (setf (?finalized class-entry) T)
               
                     ;; Trage die Klasse in class-def-list des akt. Moduls ein
                     ;;-------------------------------------------------------
                     (add-q class (?class-def-list *module*))))

               class)))

         ;;---------------------------------------------------------------------
         ;;finalize-built-in: Finalisiert eine 'built-in'-Klasse 
         ;;---------------------------------------------------------------------
         (finalize-built-in (class)
           (let ((*CURRENT-FORM* (?symbol class)))
             
          
             ;; pruefe ob diese Klasse nicht schon finalized ist
             ;;-------------------------------------------------
             (unless (?class-precedence-list class)

               ;; gebe eine Meldung aus (einmal)
               ;;-------------------------------
               (unless built-in-message-flag
                 (setf built-in-message-flag T)
                 (clicc-message "Finalizing built-in-classes ..."))
               
               ;; finalize die Oberklassen
               ;;-------------------------
               (dolist (superclass (?super-list class))
                 (finalize-built-in superclass))
               
               ;; Berechnung der class-precedence-list
               ;;-------------------------------------
               (setf (?class-precedence-list class)
                     (compute-c-p-l class)))))

         ;;---------------------------------------------------------------------
         ;; optimize-slot-positions: Optimiere die Positionen von Slots, so dass
         ;;                          weniger Zugriffsfunkt. benoetigt werden
         ;;---------------------------------------------------------------------
         (optimize-slot-positions ()
           (let ((slot-counts ()))

             ;; Zaehle die Vorkommen von Slot-Bezeichnern
             ;;------------------------------------------
             (dolist (class (queue2list (?class-def-list *module*)))
               (dolist (slot (?slot-descr-list class))
                 (let ((slot-count (assoc (?symbol slot) slot-counts
                                          :test #'eq)))
                   (if slot-count (rplacd slot-count
                                          (cons (1+ (cadr slot-count))
                                                (cons class (cddr slot-count))))
                       (setf slot-counts (cons (cons (?symbol slot)
                                                     (cons 1 (list class)))
                                               slot-counts))))))
             (setf slot-counts (sort slot-counts #'> :key #'cadr))
             (when slot-counts
               (clicc-message "Optimizing slot positions ... ")

               ;; sortiere die Klassen
               ;;---------------------
               (dolist (slot-count slot-counts)
                 (setf (cddr slot-count) (reverse
                                          (sort (cddr slot-count)
                                                #'preceding-class-p))))

               ;; optimiere Positionierung der Slots
               ;;-----------------------------------
               (dolist (slot-count slot-counts)
                 (let* ((slot-name (first slot-count))
                        (new-pos (?offset
                                  (find slot-name (?slot-descr-list
                                                   (first (cddr slot-count)))
                                        :key #'?symbol))))
                   (dolist (class (cddr slot-count))
                     (let ((orig-slot (find slot-name (?slot-descr-list class)
                                            :key #'?symbol))
                           (dest-slot (find new-pos (?slot-descr-list class)
                                            :key #'?offset)))
                       (if (or (null dest-slot) (?moved dest-slot)
                               (eq (?offset orig-slot) (?offset dest-slot)))
                           (setf new-pos (?offset orig-slot))
                           (progn
                             (setf (?offset dest-slot) (?offset orig-slot))
                             (setf (?offset orig-slot) new-pos)
                             (setf (?moved orig-slot) T)))))))

               ;; sortiere die Slots entsrechend ihrem offset
               ;;--------------------------------------------
               (dolist (class (queue2list (?class-def-list *module*)))
                 (when class
                   (setf (?slot-descr-list class)
                         (sort (?slot-descr-list class) #'<
                               :key #'?offset)))))))

         ;;---------------------------------------------------------------------
         ;; generate-accessor-methods: Erzeuge die Slot-Zugriffs-Methoden
         ;;                            Diese müssen auch Typtests enthalten
         ;;                            (mittels THE) damit die primitiven
         ;;                            Zugriffsf. darauf verzichten können.
         ;;---------------------------------------------------------------------
         (generate-accessor-methods ()
           (when (queue2list (?class-def-list *module*))
             (clicc-message "Generating slot accessors ..."))
           (dolist (class (queue2list (?class-def-list *module*)))

             ;; erzeuge die Slot-Zugriffsmethoden
             ;;----------------------------------
             (let* ((*SDF* T)           ; System-Defined-Function
                    (class-name (?symbol class))
                    (class-entry (cdr (get-class-entry class-name))))
               (dolist (slot-desc (?slot-descr-list class))
                 (let* ((type (?declared-type slot-desc))
                        (offset (?offset slot-desc))
                        (dont-generate-flag (if (?inherited-from slot-desc)
                                                (eq offset
                                                    (?offset
                                                     (find (?symbol slot-desc)
                                                           (?slot-descr-list
                                                            (?inherited-from
                                                             slot-desc))
                                                           :key #'?symbol)))
                                                nil)))
                   (unless dont-generate-flag
                     (dolist (reader-name (cdr (assoc (?symbol slot-desc)
                                                      (?reader class-entry)
                                                      :test #'eq)))
                       (when reader-name
                         (p1-defmethod 
                          `(,reader-name ((instance ,class-name))
                            (L::the ,type (rt::instance-ref instance ,offset)))
                          :dont-generate-local-funs T)))
                     (dolist (writer-name 
                               (cdr (assoc (?symbol slot-desc)
                                           (?writer class-entry)
                                           :test #'eq)))
                       (when writer-name
                         (p1-defmethod 
                          `(,writer-name (new-value (instance ,class-name))
                            (rt::instance-set 
                             (L::the ,type new-value) instance ,offset))
                          :dont-generate-local-funs T)))))))))
           
         ;;---------------------------------------------------------------------
         ;; compute-c-p-l: Berechnung der class-precedence-list 
         ;;---------------------------------------------------------------------
         (compute-c-p-l (class)
           (compute-local-precedence (cons class (?super-list class)))
           (let* ((Sc                   ;erweiterte Superklassenliste
                   (cons class
                         (remove-duplicates (collect-superclasses class))))
                  (R                    ;Ordnung auf class
                   (mapappend #'(lambda (a-class)
                                  (cdr (assoc (?symbol a-class) 
                                              all-Rc
                                              :test #'eq)))
                                         Sc)))
             (append (topological-sort Sc R class)
                     (list (?class-def (rest (get-class-entry `T)))))))

         ;;---------------------------------------------------------------------
         ;; compute-local-precedence: vgl. CLtL p. 782
         ;;---------------------------------------------------------------------
         (compute-local-precedence (cl)
           (let ((lc (mapcar #'list (butlast cl) (rest cl))))
             (push (cons (?symbol (first cl)) lc) all-Rc)
             lc))
         
         ;;---------------------------------------------------------------------
         ;; collect-superclasses: Berechne die Menge aller Superklassen
         ;;---------------------------------------------------------------------
         (collect-superclasses (class)
           (let ((result (?super-list class))
                 (others (mapcar #'collect-superclasses (?super-list class))))
             (mapcar #'(lambda (other)
                         (setf result (append result other)))
                     others)
             result))

         ;;---------------------------------------------------------------------
         ;; topological-sort: vgl. CLtL p. 784
         ;;---------------------------------------------------------------------
         (topological-sort (Sc R class)
           (block topological-sort
             (let ((result '()))
               (loop
                (let ((choices (remove-if
                                #'(lambda (a-class)
                                    (member a-class R :key #'cadr))
                                Sc))
                      choice)
                  (when (null choices) 
                    (if (null Sc)
                        (return-from topological-sort result)
                        (clicc-error 
                         "Inconsistent precedence graph for class ~S.~% ~
                          Unable to find positions for ~S"
                         (?symbol class) (mapcar #'?symbol Sc))))
                  (setf choice (if (null (cdr choices))
                                   (car choices)
                                   (select-other-choice choices result class)))
                  (setf result (append result (list choice)))
                  (setf Sc (remove choice Sc))
                  (setf R (remove choice R :test #'member)))))))
         
         ;;---------------------------------------------------------------------
         ;; select-other-choice: vgl. CLtL p. 784
         ;;---------------------------------------------------------------------
         (select-other-choice (choices result class)
           (block select-other-choice
             (dolist (a-result (reverse result))
               (dolist (possible-choice (?super-list a-result))
                 (when (member possible-choice choices) 
                   (return-from select-other-choice possible-choice))))
             (clicc-error "Inconsistent precedence graph for class ~S.~% ~
                           Unable to choose one of ~S" 
                          (?symbol class) (mapcar #'?symbol choices))))
         
         ;;---------------------------------------------------------------------
         ;; copy-slot-desc: Erzeugen einer Kopie von einer Instanz der Klasse
         ;;                 slot-desc
         ;;---------------------------------------------------------------------
         (copy-slot-desc (old-slot-desc)
           (make-instance 'slot-desc 
                          :symbol (?symbol old-slot-desc)
                          :initform (?initform old-slot-desc)
                          :initargs (?initargs old-slot-desc)
                          :allocation (?allocation old-slot-desc)
                          :offset (?offset old-slot-desc)
                          :declared-type (?declared-type old-slot-desc))))
         
      ;; Rumpf von finalize-classes
      ;;---------------------------
      (dolist (a-type (?types *GLOBAL-ENVIRONMENT*))
        (case (cadr a-type)
          (:CLASS
           (finalize-class (?class-def (cddr a-type))))
          (:BUILT-IN
           (finalize-built-in (cddr a-type)))))
      (when *OPTIMIZE* (optimize-slot-positions))
      (let* ((cgol (queue2list (?class-def-list *module*)))
             (order (+ (length cgol) 1)))
        (setf cgol (class-precedence-sort cgol))
        (dolist (class cgol)
          (decf order)
          (setf (?order class) order))
        (setf (?class-def-list *module*) (list2queue cgol)))
      (generate-accessor-methods))))

;;------------------------------------------------------------------------------
;; symbolize-class-names: wandelt die Klassen- und Slotsymbole in sym's um
;;------------------------------------------------------------------------------
(defun symbolize-class-names ()
  (dolist (a-class (queue2list (?class-def-list *module*)))
    (unless (sym-p (?symbol a-class))
      (setf (?symbol a-class) (p1-constant (?symbol a-class))))
    (dolist (a-slot (?slot-descr-list a-class)) 
      (setf (?symbol a-slot) (p1-constant (?symbol a-slot))))))

;;------------------------------------------------------------------------------
;; export-classes: Setzt das exported-flag der Klassen und veranlasst ggf. das
;;                 schattierte exportieren von Oberklassen
;;------------------------------------------------------------------------------
(defun export-classes ()
  (dolist (class (?class-def-list *module*))
    (unless (?exported class)
      (let ((symbol (?symbol (?symbol class))))
        (when (consp symbol) (setq symbol (second symbol)))
        (when (symbol-package symbol)
          (multiple-value-bind (s status)
              (find-symbol (symbol-name symbol) (symbol-package symbol))
            (declare (ignore s))
            (when (eq :external status)
              (dolist (superclass
                        (rest (?value (?class-precedence-list class))))
                (unless (?exported superclass)
                  (setf (?exported superclass) t)
                  (unless (member :full-specializable
                                  (?export-goals superclass))
                    (push :shadow-specializable
                          (?export-goals superclass)))))
              (setf (?exported class) t)
              (unless (?export-goals class)
                (setf (?export-goals class)
                      (list :full-subclassable
                            :full-instanceable
                            :full-specializable))))))))))

;;------------------------------------------------------------------------------
;; class-precedence-sort: sortiert eine Liste von Klassen gemaess der durch
;;                        die c-p-l's vorgegeben Reihenfolgen
;;------------------------------------------------------------------------------
(defun class-precedence-sort (class-list)
  (let ((todo class-list)
        (work nil)
        (result nil))
    (loop
     (when (null todo) (return-from class-precedence-sort result))
     (setf work todo)
     (setf todo nil)
     (dolist (class work)
       (if (null (set-difference (cdr (?value (?class-precedence-list class)))
                                 result))
           (push class result)
           (push class todo))))))
     
;;------------------------------------------------------------------------------
(provide "p1class")




