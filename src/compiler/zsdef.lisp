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
;;; Funktion : Definition der Zwischensprachenimplementation
;;;
;;; $Revision: 1.148 $
;;; $Id: zsdef.lisp,v 1.148 1994/12/12 14:44:44 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "clcmisc")
(require "clcdef")
(require "titypes")

;;------------------------------------------------------------------------------
;; Die in dieser Implementierung der Zwischensprache benutzten Namen sind an
;; die Beduerfnisse von CommonLisp angelehnt, und daher entweder intuitive
;; Abkuerzungen der in der LZS-Sprachberschreiung vorkommenden Benennungen,
;; oder aber Erweiterungen um den Suffix "-form".  Folgende Tabelle gibt eine
;; Aufstellung der dabei verwendeten Abkuerzungsregeln.
;;
;; LZS-Sprachbeschreibung                       hier verwendete Benennung
;; ----------------------------------------------------------------------
;; function                                     fun
;; constant                                     const
;; variable                                     var
;; symbol                                       sym
;; reference                                    ref
;; descriptor                                   desc
;; parameters                                   params
;; optional                                     opt
;; keyword                                      key
;; application                                  app
;; predicate                                    pred
;; initial-value-list                           init-list
;; continuation                                 cont
;;
;; *   z. B. setq, if, progn                    *-form
;; 
;;------------------------------------------------------------------------------



;;------------------------------------------------------------------------------
;; Definition der Klassen, die die Knoten der Zwischensprache repraesentieren.
;; Zuerst werden die Slots genannt, die den Strukturteil des Knotens beschreiben
;; und dann, gefolgt von dem Kommentar ``;; Annotation'' der Annotationsteil.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Gemeinsame Oberklasse fuer alle Klassen der Zwischensprache.
;;------------------------------------------------------------------------------
(defzws zws-object ()
  ;; Annotation
  ;;-----------
  (analysed                             ; Fuer Marken von Analysedurchlaeufen
   :initform :analyse-mark :type symbol))

;;------------------------------------------------------------------------------
(defzws imported-module (zws-object)
  ;; Annotation
  ;; ----------
  (name :type string)                   ; Name des Moduls (Dateiname ohne Ext.)
  (init-fun-name :type string)          ; Name der Initialisierungsfunktion
  (symbol-base :type string)            ; Name der Tabelle der Symbole
  (named-const-base :type string))   ; Name der Tabelle der named-constants

;;------------------------------------------------------------------------------
(defzws module (imported-module)
  (fun-list :type list)
  (class-def-list :type list)           ; |class|
  (named-const-list :type list)
  (var-list :type list)
  (sym-list :type list)
  (toplevel-forms :type fun)            ; |init-function|
  ;; Annotation
  ;;-----------
  (dyn-var-list :type list)             ; Liste der dynamischen Variablen
  (package-list :type list)             ; nur fuer Common Lisp
  (loaded-modules                       ; Liste der importierten Module
   :initform () :type list)             ; :type (list imported-module)
  (package :type package))              ; Das Package des Moduls

;;------------------------------------------------------------------------------
(defmethod ?all-global-funs ((a-module module))
  (cons (?toplevel-forms a-module) (?fun-list a-module)))

#+CMU
(defun all-funs (fun)
       (cons fun (mapappend #'all-funs (?local-funs fun))))

;;------------------------------------------------------------------------------
(defmethod ?all-funs ((a-module module))
  (labels (#-CMU(all-funs (fun)
                  (cons fun (mapappend #'all-funs (?local-funs fun)))))

    (mapappend #'all-funs (?all-global-funs a-module))))

;;------------------------------------------------------------------------------
;; Oberklasse aller Formen.
;;------------------------------------------------------------------------------
(defzws form (zws-object)
  ;; Annotation
  ;;-----------
  (type :initform bottom-t))                               

;;------------------------------------------------------------------------------
;; Oberklasse aller Variablen.
;;------------------------------------------------------------------------------
(defzws var (zws-object)
  ;; Annotation
  ;;-----------
  (read                                 ; Anzahl der lesenden Zugriffe
   :initform 0 :type integer)
  (write                                ; Anzahl der schreibenden Zugriffe
   :initform 0 :type integer)
  (type                                 ; Typ fuer effiziente Repraesentation
   :initform bottom-t))                    

;;------------------------------------------------------------------------------
;; Statische Variable
;;------------------------------------------------------------------------------
(defzws static (var)
  ;; Annotation
  ;;-----------
  (symbol :type symbol))                ; Name im Quelltext

;;------------------------------------------------------------------------------
;; Definierte statische Variable
;;------------------------------------------------------------------------------
(defzws defined-static (static))

;;------------------------------------------------------------------------------
;; Lokal definierte statische Variable
;;------------------------------------------------------------------------------
(defzws local-static (defined-static)
  ;; Annotation
  ;;-----------
  (offset :type integer)                ; Offset im Stack Frame
  (level :type integer)                 ; Statische Tiefe
  (closure                              ; Kommt frei in einer Closure vor
   :initform nil :type bool)
  (export-id :type integer :raw t))

;;------------------------------------------------------------------------------
;; Global definierte statische Variable
;;------------------------------------------------------------------------------
(defzws global-static (defined-static)
  (exported :initform nil :type bool)
  ;; Annotation
  ;;-----------
  (adr :type string))                   ; Adresse im Zielcode

;;------------------------------------------------------------------------------
;; Importierte statische Variable
;;------------------------------------------------------------------------------
(defzws imported-static (static)
  (exported :initform nil :type bool)
  ;; Annotation
  ;;-----------
  (adr :type string))                   ; Adresse im Zielcode

;;------------------------------------------------------------------------------
;; Dynamische Variable.
;; Die Information, ob es sich um eine importierte und/oder exportierte Variable
;; handelt, kann aus dem Symbol ersehen werden.
;;------------------------------------------------------------------------------
(defzws dynamic (var)
  (sym :type sym))                      ; das zugehoerige Symbol

;;------------------------------------------------------------------------------
;; Variablen Referenz
;;------------------------------------------------------------------------------
(defzws var-ref (form)
  (var :type var))


;;------------------------------------------------------------------------------
;; Benannte Konstanten von Eulisp
;;------------------------------------------------------------------------------
(defzws named-const (form)
  ;; Annotation
  ;;-----------
  (symbol :type symbol))                ; Name im Quelltext

;;------------------------------------------------------------------------------
(defzws defined-named-const (named-const)
  (exported :initform nil :type bool)
  (value                                ; literal || :unknown || :forward
   :initform :unknown)
  ;; Annotation
  ;;-----------
  (read                                 ; Anzahl der lesenden Zugriffe
   :initform 0 :type integer))

;;------------------------------------------------------------------------------
(defzws imported-named-const (named-const)
  (value-zs-type :type symbol)
  (adr :type string))

;;------------------------------------------------------------------------------
;; Oberklasse der Literale
;;------------------------------------------------------------------------------
(defzws literal (form))
  
;;------------------------------------------------------------------------------
;; Symbol
;; der Typ 'symbol' ist in CL reserviert.
;;------------------------------------------------------------------------------
(defzws sym (literal)
  (exported :initform nil :type bool)
  (constant-value                       ; Fuer 'defconstant' in Common Lisp
   :initform :no-const)                 ; literal | :unknown | :no-const
  ;; Annotation
  ;;-----------
  (symbol :type symbol)                 ; Das Symbol im Quellcode
  (used :initform 0 :type integer)
  (adr :type integer))                  ; Adresse im Zielcode

;;------------------------------------------------------------------------------
(defun constant-value-p (sym)
  (not (member (?constant-value sym) '(:unknown :no-const))))


;;------------------------------------------------------------------------------
;; Im Modul definiertes Symbol.
;; Der Slot 'package' ist notwendig, wenn mehrere Packages als ein Modul
;; uebersetzt werden.
;;------------------------------------------------------------------------------
(defzws defined-sym (sym)
  (name :type string)
  package)                              ; Package-Name oder :uninterned

;;------------------------------------------------------------------------------
;; Basisadresse der Symbole des aktuellen Moduls ist fuer alle definierten
;; Symbole des Moduls gleich.
;;------------------------------------------------------------------------------
(defmethod ?base ((sym defined-sym))
  (?symbol-base *module*))

;;------------------------------------------------------------------------------
;; Importiertes Symbol
;; Der type-Slot wird hier mit symbol-t vorbesetzt, da sonst importierte
;; Symbole, die nach der Typanalyse erstmals ins Programm eingefuegt werden,
;; mit bottom-t getypt sind.
;;------------------------------------------------------------------------------
(defzws imported-sym (sym)
  (base :type string)                   ; der Name des Arrays, in dem das Symbol
                                        ; alloziert ist.
  (type :initform symbol-t))

;;------------------------------------------------------------------------------
;; Einfache Literale sind die leere Liste, Zeichen und Zahlen.
;;------------------------------------------------------------------------------
(defzws simple-literal (literal))


;;------------------------------------------------------------------------------
;; empty-list ist die einzige Instanz der Klasse null-form.
;;------------------------------------------------------------------------------
(defzws null-form (simple-literal))

;; Hier könnte defconstant stehen, dann müßte man jedoch eine Methode für
;; make-load-form schreiben und das ist in vielen Lisps noch nicht (richtig)
;; definiert
;;----------
(defparameter empty-list (make-instance 'null-form :type null-t))

;;------------------------------------------------------------------------------
(defzws character-form (simple-literal)
  (value :type character))

;;------------------------------------------------------------------------------
;; Klassen der Zahlenliterale.
;;------------------------------------------------------------------------------
(defzws num (simple-literal)
  (value :type number))

(defzws int (num)
  (value :type integer))

;;(defzws ratio-form (num)
;;  (value :type ratio))

(defzws float-form (num)
  (value :type float)
  ;; Annotation
  ;;-----------
  (adr :type string))                   ; Adresse im Zielcode

;;(defzws complex-form (num)
;;  (value :type complex))

;;------------------------------------------------------------------------------
;; Strukturierte Literale sind Felder, Strukturen (literal-instance) und Paare.
;;------------------------------------------------------------------------------
(defzws structured-literal (literal)
  (value
   :type (or array cons string literal-instance))
  ;; Annotation
  ;;-----------
  (adr :type string)                    ; Adresse im Zielcode
  (needs-fixnum-array :type bool)       ; Konstante enthaelt Array v. Fixnums
  (needs-float-array :type bool))       ; Konstante enthaelt Floats

;;------------------------------------------------------------------------------
;; Eine zur Ubersetzungszeit bekannte Instanz einer Klasse.
;; Wird benoetigt zur Darstellung von #s(struct-name ..) in Common Lisp.
;;------------------------------------------------------------------------------
(defzws literal-instance (literal)
  (class :type class-def)
  (value-list :type list))              ; Werte der Slots als Liste von Literals

;;------------------------------------------------------------------------------
;; Knoten zur Darstellung von definierenden und angewandten Vorkommen von
;; Klassen.
;; Die Klasse 'class' ist in CLOS schon vergeben, deshalb 'class-def'.
;; Die Annotation super-list ist neben dem Slot class-precedence-list noetig,
;; da die Information ob eine Klasse DIREKTE Unterklasse einer anderen ist,
;; fuer Analysen/Optimierungen benoetigt wird, und sich nur auf nicht triviale
;; Weise aus den class-precedence-lists gewinnen laesst.
;; Die Eintraege in der Annotation order definieren eine totale Ordnung auf
;; Klassen. Diese wird benoetigt, um eine Reihenfolge der Tests im Dispatch von
;; generischen Funktionen festzulegen.
;;------------------------------------------------------------------------------
(defzws class-def (form)
  (class-precedence-list                ; List of class-def
   :initform () :type (or list structured-literal))    
  (slot-descr-list                      ; List of slot-desc
   :initform () :type (or list structured-literal null-form))
  ;; Annotation
  ;;-----------
  (super-list :initform '() :type list) ; List of class-def
  (symbol :type (or sym symbol))        ; Name im Quelltext
  (adr    :type integer)                ; relative Adresse im Zielcode
  (used   :initform 0 :type integer)
  (order  :initform 0 :type integer))   ; def. totale Ordnung auf Klassen

;;------------------------------------------------------------------------------
;; Die Annotation export-goals enthält eine Liste von keywords, welche die
;; Verwendungsmöglichkeiten von exportierten Klassen einschränken. Gültige
;; Einträge in diese Liste sind:
;; :full-subclassable :full-instanceable :full-specializable
;; :shadow-specializable
;;------------------------------------------------------------------------------
(defzws defined-class (class-def)
  (exported :initform nil :type bool)
  ;; Annotation
  (export-goals :initform nil :type list)) 

;;------------------------------------------------------------------------------
(defzws imported-class (class-def))

;;------------------------------------------------------------------------------
(defzws built-in-class-def (class-def)
  (type-expander :type defined-fun))

;;------------------------------------------------------------------------------
(defzws structure-class-def (class-def))

;;------------------------------------------------------------------------------
(defzws slot-desc (zws-object)
  (symbol :type (or sym symbol))        ; Name des Slots
  initform                              ; form
  (initargs                             ; List of sym
   :initform () :type (or list structured-literal))
  allocation                            ; instance oder class
  ;; Annotation
  ;;------------
  (moved  :initform nil)                ; offset geaendert ?
  (inherited-from :initform nil)        ; Klasse aus der geerbt wurde
  (offset :type integer)                ; relative Adresse in der Instanz
  declared-type)                        ; aus den slot-options von defclass

;;------------------------------------------------------------------------------
;; Die formale Parameterliste einer Funktionsdefinition.
;; Dieser Knoten taucht nur als Slot in einer Funktionsdefinition auf, seine
;; Slots koennten also auch direkt in dem Knoten 'fun' angegeben werden.
;;------------------------------------------------------------------------------
(defzws params (zws-object)
  (var-list :type list)                 ; required Parameters
  (opt-list :type list)                 ; Liste von Instanzen von 'opt'
  (rest :type (or null symbol var))     ; &Rest Variable oder nil
  (key-list :type list)                 ; Liste von Instanzen von 'key'
  (allow-other-keys :type bool)
  ;; Annotation
  ;;-----------
  (all-vars :type list))                ; Alle Var. zusammen in einer Liste

;;------------------------------------------------------------------------------
(defzws opt (zws-object)
  (var :type var)
  init                                  ; Init Value
  (suppl :type (or null var)))          ; Supplied-p Variable oder nil

;;------------------------------------------------------------------------------
(defzws key (opt)
  (sym :type sym))                      ; Keyword Symbol

;;------------------------------------------------------------------------------
;; Funktion
;; Der Typ 'function' ist in CL reserviert.
;;------------------------------------------------------------------------------
(defzws fun (form)
  ;; Annotation
  ;;-----------
  (symbol :type (or symbol cons)))      ; Name im Quelltext

;;------------------------------------------------------------------------------
;; Dummy ?result-type fuer null definieren, damit bei der
;; Uebersetzung des Inline-Moduls unbekannte Funktionen ignoriert werden.
;;------------------------------------------------------------------------------
(defmethod (setf ?result-type) (value (fun null))
  (declare (ignore value)))

;;------------------------------------------------------------------------------
;; Einfache Funktion (nicht generisch)
;;------------------------------------------------------------------------------
(defzws simple-fun (fun)
  ;; Annotation
  ;;-----------
  (adr :type string)                    ; Adresse der Funktion im Zielcode
  (par-spec :type integer)              ; (mindest) Parameterzahl
  (mv-spec                              ; Anzahl Multipler Werte
   :initform 1 :raw t
   :type (or (member nil t :jump) integer))
  (read-list                            ; Liste der gelesenen globalen Variablen
   :initform nil 
   :type (or list integer))            
  (write-list                           ; Liste der veraenderten Variablen    
   :initform nil                        
   :type (or list integer))            
  (data-effects                         ; Welche Operationen auf Daten ?
   :initform nil
   :type symbol)
  (closure                              ; Closure gebildet, Art
   :initform nil :raw t
   :type (member nil t :closure :downfun))
  (downward-args                        ; Position der 'downward' Param.
   :initform () :type list :raw t)
  (has-funs-as-args                     ; ob Parameter nur an Funktions-
   :initform nil :type list :raw t)     ; position steht.
  (result-type :initform bottom-t)      ; Resultattyp der Funktion
  (simp-when-n-args                     ; Wert: (<n> <simp-fun>)
   :initform nil :raw t)                ; (fun a1 .. an) -> (simp-fun a1 .. an)
  (simp-when-no-result                  ; Wert: <simp-fun>
   :initform nil :raw t)                ; (fun ...) -> (simp-fun ...)
                                        ; wenn Resultat nicht verwendet wird
  (simp-when-arg-n=cons                 ; Wert: (<n> <fun>)
   :initform nil :raw t)                ; (fun .. an ..) -> (simp-fun .. an ..)
                                        ; wenn an vom Typ cons
  (simp-when-some-arg-not-cons/pathn/string/bitv ; Wert: <simp-fun>
    :initform nil :raw t)               ; (fun ...) -> (simp-fun ...)
                                        ; wenn mindestens ein Arg. nicht vom Typ
                                        ; cons, pathname, string, bitvector ist.
  (simp-when-some-arg-not-num/char      ; Wert: <simp-fun>
    :initform nil :raw t)               ; (fun ...) -> (simp-fun ...)
                                        ; wenn mindestens ein Arg. nicht vom Typ
                                        ; number, char ist.
  (simp-test-fun-when-not-testnot       ; Wert: (test-on-pos #non-key-args
   :initform nil :raw t)                ; test-keyword default test-not-keyword)
                                        ; (fun non-key-args .. :test fun ..)
                                        ; (fun non-key-args .. :test opt-fun ..)
  (simp-when-only-test=value            ; Wert: (#non-key-args keyword value
                                        ; simp-fun)
   :initform nil :raw t)                ; (fun non-key-args :test value) ->
                                        ; (simp-fun non-key-args)
  (my-last-arg-may-be-rest-var          ; letztes Argument kann rest-liste sein
   :initform nil :raw t))               ; dann Name der Fnk im keyw-pck.

;;------------------------------------------------------------------------------
;; Definierte einfache Funktion
;;------------------------------------------------------------------------------
(defzws defined-fun (simple-fun)
  (params :type params)
  body
  ;; Annotation
  ;;-----------
  (const-list                           ; Direkt im Rumpf vork. struct-lit.
   :initform () :type list :raw t) 
  (local-funs                           ; Direkt im Rumpf lokal def. Funkt.
   :initform () :type list)             ; kein raw-slot, da initarg :local-funs
                                        ; benoetigt wird.
  (used                                 ; Anzahl der Funktionsaufrufe
   :initform 0 :type integer)
  (max-level                            ; max. stat. Niveau lok. def. Funkt.
   :initform 0 :type integer :raw t) 
  (deeper-level-calls                   ; Aufrufe lok. Fkt. hoeheren Niveaus
   :initform () :type list :raw t)
  (mv-calls        :type list :initform ()) ; mv-spec haengt von diesen Fkt. ab
  (mv-called-by    :type list :initform ()) ; Diese Fkt. benutzen mv-spec
  (called-by       :type list :initform ()) ; Liste der Aufrufer dieser Funktion
  (unknown-caller                       ; Gibt es unbekannte Aufrufstellen?
   :initform nil :type bool :raw t)
  (pred-type-env   :type list :initform ()) ; Eintrittstypumgebung
  (result-type-env :type list :initform ()) ; Ergebnistypumgebung
  (inline                               ; Funktion inline-compilieren?
   :type symbol :initform nil :raw t)   ; NIL, T, COPYABLE
  (syntactically-exported               ; Markierung, ob Funktion syntaktisch
   :initform nil :type bool :raw t)     ; exportiert wird
  source)                               ; Quellcode der Funktion

;;------------------------------------------------------------------------------
;; Globale definierte einfache Funktion
;;------------------------------------------------------------------------------
(defzws global-fun (defined-fun)
  (exported :initform nil :type bool)
  ;; Annotation
  ;;----------
  (call-in :initform nil               ; Informationen ueber den Call-In-
   :raw t))                             ; Aufruf der Funktion.

;;------------------------------------------------------------------------------
;; Zur Erleichterung
;;------------------------------------------------------------------------------
(defmethod ?level ((fun defined-fun)) 0)

;;------------------------------------------------------------------------------
;; Dummy-Funktionen fuer global-fun definieren, damit bei der
;; Uebersetzung des Lisp-Moduls diese Funktionen ignoriert werden.
;;------------------------------------------------------------------------------
(defmethod (setf ?type-abstraction-function) (value (fun global-fun))
  (declare (ignore value)))

(defmethod (setf ?argument-types) (value (fun global-fun))
  (declare (ignore value)))

;;------------------------------------------------------------------------------
;; Dummy-Funktionen fuer null definieren, damit bei der
;; Uebersetzung des Inline-Moduls unbekannte Funktionen ignoriert werden.
;;------------------------------------------------------------------------------
(defmethod (setf ?type-abstraction-function) (value (fun null))
  (declare (ignore value)))

(defmethod (setf ?argument-types) (value (fun null))
  (declare (ignore value)))

;;------------------------------------------------------------------------------
;; lokale einfache Funktion
;; (Kann nicht importiert sein, das ist syntaktisch nicht moeglich.)
;;------------------------------------------------------------------------------
(defzws local-fun (defined-fun)
  ;; Annotation
  ;;-----------
  (level :type integer)                 ; Stat. Niveau der Funktionsdefinition
  (as-global-fun                        ; Kann globalisiert werden
   :initform t :type bool)
  (same-level-calls                     ; Aufrufe lok. Fkt. mit gleichem Niveau
   :initform () :type list :raw t)
  (free-local-funs                      ; im Rumpf aufgerufene lokale Funktionen
   :initform () :type list :raw t)
  (free-in                              ; kommt in diesen Funktionen frei vor
   :initform () :type list :raw t)
  (free-lex-vars                        ; freie statisch geb. lokale Variablen
   :initform () :type list)
  (num-free-lex-vars :type integer)     ; Laenge von 'free-lex-vars'
  (closure-offset :type integer)        ; Offset des Closure-Objekts im Stack
  (calls-free-param                     ; Niveau des aufgerufenen freien
   :type (or null integer))             ; Parameters
  (export-id :type integer :raw t))

(defmethod ?calls-free-param ((fun fun)) nil)
(defmethod (setf ?calls-free-param) (new (fun fun))
  (when new (internal-error 'calls-free-param
                            "global fun shouldn't have free parameters")))

;;------------------------------------------------------------------------------
;; importierte einfache Funktion
;; Der type-Slot wird hier mit function-t vorbesetzt, da sonst importierte
;; Funktionen, die nach der Typanalyse erstmals ins Programm eingefuegt werden,
;; mit bottom-t getypt sind.
;;------------------------------------------------------------------------------
(defzws imported-fun (simple-fun)
  (exported :initform nil :type bool)
  ;; Annotation
  ;;-----------
  (params :initform nil :raw t)         ; Parameterliste fuer Inlining
  (body :initform nil :raw t)           ; Rumpf fuer Inlining
  (local-funs :initform nil :raw t)     ; wird vom Inlining benoetigt
  (opti-pass       :initform nil :raw t)
  (pass3           :initform nil :raw t)
  (tipass          :initform nil :raw t)
  (weight-c-inline :initform nil :raw t)
  (c-inline        :initform nil :raw t)
  (special-caller  :initform nil :raw t)
  (need-no-stack :raw t                 ; Codegeneration: Fkt verändert höchstns
   :initform nil :type bool)            ; eigene Paramter auf Lisp-Stack
  (type-abstraction-function            ; Bildet Argumenttypen auf Resultattypen
   :initform nil :raw t                 ; ab.
   :type (or null function))
  (result-type                          ; Ergebnistyp der Funktion
   :initform top-t)
  (argument-types                       ; Liste der Argumenttypen
   :initform '())
  (type :initform function-t)
  (syntax :type global-fun))            ; fuer syntaktische importierte Fnktnen

;;------------------------------------------------------------------------------
;; Systemfunktion, die in gewissen Phasen des Compilers einer speziellen
;; Behandlung beduerfen.
;;------------------------------------------------------------------------------
(defzws special-sys-fun (imported-fun))

;;------------------------------------------------------------------------------
;; Systemfunktion, die einer speziellen Behandlung bedarf und Multiple Werte
;; generiert. Das ist zur Zeit nur die Common Lisp Funktion values.
;;------------------------------------------------------------------------------
(defzws special-sys-fun-with-mv (special-sys-fun))

;;------------------------------------------------------------------------------
(defzws generic-fun (fun)
  (params :type params)
  (method-list
   :initform () :type list)             ; Instanzen von method-def
  (argument-precedence-order
   :initform () :type list)             ; Liste von Symbolen, die req. Parameter
                                        ; bezeichnen
  (method-combination                   ; Built-in Method-Combination Bezeichner
   :initform 'L::standard :type symbol)
  ;; Annotation
  ;;-----------
  fun                                   ; fuehrt den gen. Dispatch aus
  (specializer-pos                      ; Position des ersten und des letzten
   :initform '(nil nil) :type list))    ; spezialisierten Parameters

;;------------------------------------------------------------------------------
(defzws defined-generic-fun (generic-fun)
  (exported :initform nil :type bool))

;;------------------------------------------------------------------------------
(defzws imported-generic-fun (generic-fun))

;;------------------------------------------------------------------------------
(defzws method-def (zws-object)
  (fun :type fun)
  (specializer-list :type list)         ; List of 'class-def' oder literal
  (qualifier-list :type list))          ; Liste von Symbolen

;;------------------------------------------------------------------------------
;; Eine Funktions-Anwendung;
;; 'form' ist ein 'fun' oder evaluiert zur Laufzeit zu einer Funktion.
;;------------------------------------------------------------------------------
(defzws app (form)
  form
  (arg-list :type list)
  ;; Annotation
  ;;------------
  (mv-used :initform nil :type bool)    ; Darf dieser Aufruf MV generieren ?
  (downfun-list :type list)             ; Liste der Downfuns unter den Arg.
  (called-funs                          ; Enthaelt die Funktionen, zu der die
   :initform ()                         ; form evaluieren kann.
   :type list)
  (other-funs                           ; Gibt es weitere solche Funktionen?
   :initform t
   :type bool))
;;------------------------------------------------------------------------------
(defzws setq-form (form)
  (location :type (or var-ref named-const))
  form)

;;------------------------------------------------------------------------------
(defzws progn-form (form)
  (form-list :type list))

;;------------------------------------------------------------------------------
(defzws if-form (form)
  pred
  then
  else)

;;------------------------------------------------------------------------------
(defzws switch-form (form)
  form
  (case-list :type list)                ; Liste von labeled-form
  otherwise)                            ; Eine 'form'

;;------------------------------------------------------------------------------
(defzws labeled-form (zws-object)
  value                                 ; sym oder simple-literal
  form)

;;------------------------------------------------------------------------------
(defzws let*-form (form)
  (var-list :type list)                 ; Liste der Variablen
  (init-list :type list)                ; Liste der Vorbesetzungen
  body)

;;------------------------------------------------------------------------------
(defzws labels-form (form)
  (fun-list :type list)                 ; List of fun
  body)

;;------------------------------------------------------------------------------
(defzws let/cc-form (form)
  (cont :type cont)
  body)

;;------------------------------------------------------------------------------
(defzws cont (local-static)
  ;; Annotation
  ;;------------
  (mv-position :type bool)              ; auf Pos., die MV benoetigt ?
  (fn-on-mv-pos                         ; auf Pos., die MV + Fkt. Obj. ben.
   :initform nil :type bool :raw t)
  (binding-stack-level :type integer)   ; Relative Hoehe des Binding-Stacks
  (mv-spec                              ; Anzahl Multipler Werte
   :initform 1 :raw t
   :type (or (member nil t :jump) integer))
  (mv-calls                             ; Fkt. von denen mv-spec abhaengt
   :initform () :type list)
  (only-local                           ; Nur lokale Anwendungen der Contin. ?
   :type bool :initform t :raw t)
  (unknown-caller                       ; Gibt es unbekannte Aufrufstellen?
   :initform nil :type bool :raw t)
  (result-type :initform bottom-t)      ; Ergebnistyp der Continuation
  (type-env                             ; Vereinigung der Typumgebungen bei
   :initform nil                        ; Applikationen der Continuation.
   :type list)
  (adr :type string)                    ; Die Adresse im Zielcode
  result-spec)                          ; Resultatspos. des let/cc

;;------------------------------------------------------------------------------
(defzws tagbody-form (form)
  first-form                            ; Ausdruck vor dem 1. Tag, evtl. nil
  (tagged-form-list :type list)
  ;; Annotation
  ;;-----------
  (level :type integer)                 ; statisches Niveau
  (binding-stack-level :type integer)   ; relative Hoehe des Binding-Stacks
  (export-id :type integer :raw t))

;;------------------------------------------------------------------------------
;; tagged-form repraesentiert zugleich das Tag und
;; alle zugehoerigen Go Ausdruecke
;;------------------------------------------------------------------------------
(defzws tagged-form (form)
  form
  (tagbody :type tagbody-form)
  ;; Annotation
  ;;-----------
  (used                                 ; gibt es angewandte Vorkommen ?
   :initform 0 :type integer)
  (pred-type-env                        ; Vereinigung der Typumgebungen aller
   :type list                           ; Sprungstellen zu dieser tagged-form.
   :initform ())
  (adr           :type string)          ; Die Adresse im Zielcode
  (export-id :type integer :raw t))

;;------------------------------------------------------------------------------
;; Konstrukt zum Ausdruecken der Special Forms multiple-value-call und
;; multiple-value-prog1 in der Zwischensprache.
;; Dieses Konstrukt ist sehr aehnlich zu dem Makro multiple-value-bind mit
;; der Ausnahme, dass eine beliebige Lambda-Liste angegeben werden kann.
;;------------------------------------------------------------------------------
(defzws mv-lambda (form)
  (params :type params)
  body
  arg                                   ; generiert evtl. multiple Werte
  ;; Annotation
  ;;-----------
  (mv-spec                              ; Anzahl Multipler Werte
   :type (or (member nil t :jump) integer))
  (mv-calls :initform () :type list))   ; mv-spec haengt von diesen Fkt. ab

;;------------------------------------------------------------------------------
(provide "zsdef")

