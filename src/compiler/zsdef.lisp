;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Definition der Zwischensprachenimplementation
;;;
;;; $Revision: 1.134 $
;;; $Log: zsdef.lisp,v $
;;; Revision 1.134  1993/12/18  06:17:32  hk
;;; Definition von empty-list mit defparameter statt defconstant, um
;;; Probleme mit make-load-form zu vermeiden
;;;
;;; Revision 1.133  1993/12/09  09:52:09  uho
;;; Importierte Funktionen besitzen eine weitere Annotation: 'syntax', in
;;; der bei syntaktischem Import die fuer die Uebersetzungszeit-
;;; Interpretationen benoetigte Zwischensprach-Funktion abgelegt wird.
;;;
;;; Revision 1.132  1993/12/06  15:46:26  hk
;;; In class-def den Typ des Slots slot-descr-list korrigiert:
;;; (or list structured-literal null-form)
;;;
;;; Revision 1.131  1993/11/25  13:37:32  ft
;;; Initform des eben erwaehnten Slots korrigiert.
;;;
;;; Revision 1.130  1993/11/25  13:13:21  ft
;;; Der Slot specializer-pos von defined-generic-funs enthaelt jetzt eine
;;; zweielementige Liste mit der Position des ersten und der des letzten
;;; spezialisierten Parameters.
;;;
;;; Revision 1.129  1993/11/05  14:20:50  hk
;;; require an den Dateianfang verschoben, um Probleme mit akcl zu vermeiden.
;;;
;;; Revision 1.128  1993/10/18  10:26:43  hk
;;; Tippfehler behoben: vor Kommentar 'Annotation' fehlte ';;'.
;;;
;;; Revision 1.127  1993/10/14  14:00:04  hk
;;; labeled-form hat ist Unterklasse von zws-object und nicht von form.
;;;
;;; Revision 1.126  1993/10/12  10:27:48  hk
;;; Typ vom Slot data-effects korrigiert und optimiert.
;;;
;;; Revision 1.125  1993/09/29  13:49:47  hk
;;; empty-list wird mit dem typ null-t und nicht bottom-t getypt.
;;;
;;; Revision 1.124  1993/09/21  09:38:33  jh
;;; Die type-Slots importierter Symbole und Funktionen werden jetzt mit
;;; symbol-t bzw. function-t initialisiert.
;;;
;;; Revision 1.123  1993/09/20  14:10:05  jh
;;; Annotation weight-c-inline bei special-sys-fun eingefuegt.
;;;
;;; Revision 1.122  1993/09/13  14:43:08  hk
;;; Slots slot-descr-list und initargs habe in codegen den Typ
;;; structured-literal.
;;;
;;; Revision 1.121  1993/09/13  13:36:44  hk
;;; Typ des Slots type-env in cont von bool auf list gesetzt.
;;;
;;; Revision 1.120  1993/09/10  15:30:55  hk
;;; Typ von class-precedence-list korrigiert: (or list structured-literal)
;;;
;;; Revision 1.119  1993/09/09  12:34:01  jh
;;; Typ des used-Slots von tagged-form auf integer geaendert und mit 0
;;; initialisiert.
;;;
;;; Revision 1.118  1993/09/07  07:45:34  ft
;;; Neue Annotation order an class-def.
;;;
;;; Revision 1.117  1993/09/01  11:43:11  ft
;;; In class-def den Slot super-list aus dem Struktur- in den Annotationsteil
;;; verschoben und einen Kommentar fuer dessen Existenzberechtigung
;;; eingefuegt.
;;;
;;; Revision 1.116  1993/08/25  14:51:53  jh
;;; In special-sys-fun pass2 in opti-pass umbenannt.
;;;
;;; Revision 1.115  1993/08/17  13:39:57  hk
;;; In slot-desc und class-def den Typ des Slots symbol auf
;;; (or sym symbol) gesetzt, weil in Pass1 zunächst Symbole darin eingetragen
;;; werden.
;;;
;;; Revision 1.114  1993/08/04  17:06:40  hk
;;; Typ von read-list und write-list korrigiert
;;;
;;; Revision 1.113  1993/07/27  14:09:49  atr
;;; Typen der slots read und write-list veraedert.
;;; Die sind jetzt entweder Listen oder integerzahlen, wobei eine Zahl
;;; das groesste statische Niveau kodiert, auf dem unbekannte Seiteneffekte
;;; passieren.
;;;
;;; Revision 1.112  1993/07/21  09:10:18  hk
;;; In simple-fun: Typ von has-funs-as-args von bool nach list
;;;
;;; Revision 1.111  1993/07/20  14:46:58  hk
;;; In imported-module: named-constant-base -> named-const-base
;;;
;;; Revision 1.110  1993/07/20  10:18:06  uho
;;; Zwischensprach-Konstrukt 'imported-module' eingefuegt. Es enthaelt nur
;;; Annotationen, um Informationen von Importierten Modulen zu halten, die
;;; in anderen importierten Programmobjekten nicht repraesentiert sind.
;;; Modul-Anotation 'loaded-modules' enthaelt jetzt eine Liste von
;;; 'imported-module's.
;;;
;;; Revision 1.109  1993/07/19  14:28:33  uho
;;; Slot 'init-fun-name' als Annotation fuer Module aufgenommen. Er haelt
;;; den Namen der Initialisierungsfunktion des Moduls.
;;;
;;; Revision 1.108  1993/07/19  12:02:54  hk
;;; Annotation :adr an float-form
;;;
;;; Revision 1.107  1993/07/16  10:12:23  uho
;;; (require ffzsdef) an das Ende verschoben, da es Probleme mit der
;;; Ladereihenfolge unter PCL gab.
;;;
;;; Revision 1.106  1993/06/28  07:30:54  ft
;;; Neue Annotation inherited-from an slot-desc.
;;;
;;; Revision 1.105  1993/06/20  09:26:53  ft
;;; Neue Annotation moved an slot-desc fuer die Optimierung der
;;; Position des Slots in Instanzen der entsprechenden Klasse.
;;;
;;; Revision 1.104  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.103  1993/06/10  18:05:31  atr
;;; Slot DATA-EFFECTS bei APPLICATION eingefuegt,der enthaelt
;;; die Information ob die App destruktiv Daten veraendert , und/oder
;;; neue Daten auf dem Heap ablegt.
;;;
;;; Revision 1.102  1993/06/09  12:46:39  jh
;;; Annotation id wieder entfernt, da das Problem der Namenskollisionen jetzt
;;; anders geloest ist.
;;;
;;; Revision 1.101  1993/06/09  12:13:47  ft
;;; Änderung des Typs des Symbol-Slots in fun geändert, damit auch
;;; extended function designators abgelegt werden können.
;;;
;;; Revision 1.100  1993/06/08  08:22:16  kl
;;; Slot result-type-env fuer einen weiteren Typinferenzlevel eingefuehrt.
;;;
;;; Revision 1.99  1993/06/03  14:01:07  jh
;;; Eindeutige Identifikatoren fuer einige Zwischensprachobjekte eingefuehrt.
;;; Dies ist notwendig, um beim Uebersetzen der Zwischensprache nach Lisp
;;; Namenskollisionen zu vermeiden.
;;;
;;; Revision 1.98  1993/06/01  19:54:45  hk
;;; In global-fun, Slot call-in: :init-form -> :initform.
;;;
;;; Revision 1.97  1993/05/31  17:07:36  pm
;;; Annotation der global-funs um Eintrag fuer call-ins erweitert
;;;
;;; Revision 1.96  1993/05/30  13:58:26  atr
;;; Slot has-funs-as-args eingefuegt. Er zeigt ob die
;;; Funktion Argument(e) hat die nur an Funktionsposition stehen.
;;;
;;; Revision 1.95  1993/05/27  13:17:13  kl
;;; Typisierung der Continuations auf tomain.lisp abgestimmt.
;;;
;;; Revision 1.94  1993/05/20  12:26:34  atr
;;; Symbole 'alloc 'dest 'alloc-dest heissen jetzt
;;; :alloc :dest :alloc-dest.
;;;
;;; Revision 1.93  1993/05/19  09:45:34  hk
;;; metaclass, generic-fun-class, method-class gestrichen.
;;; method-combination spezifiziert, local-generic-fun gestrichen.
;;;
;;; Revision 1.92  1993/05/13  13:38:34  hk
;;; Anpassung an CMU.
;;;
;;; Revision 1.91  1993/05/09  16:45:29  kl
;;; Neuen Slot argument-types fuer imported-funs eingefuehrt.
;;;
;;; Revision 1.90  1993/04/30  09:20:23  kl
;;; Funktionen ?all-global-funs und ?all-funs eingefuehrt.
;;;
;;; Revision 1.89  1993/04/22  11:22:30  hk
;;; Neue Slots loaded-modules, symbol-base in module.
;;; Neuer Slot Base in imported-sym, Methode ?base fuer defined-sym.
;;;
;;; Revision 1.88  1993/04/20  14:06:50  ft
;;; Initforms fuer die Slots eines class-def definiert.
;;;
;;; Revision 1.87  1993/04/19  12:21:45  kl
;;; Special-sys-funs erweitert.
;;;
;;; Revision 1.86  1993/04/19  08:21:01  hk
;;; Dummy ?result-type und ?type-abstraction-function fuer null
;;; definiert, damit bei der Ueberetzung des Inline-Moduls unbekannte Funktionen
;;; ignoriert werden.
;;;
;;; Revision 1.85  1993/04/16  08:56:15  ft
;;; ein '-def' an die Namen von built-in-class und structure-class
;;; angehaengt, da sonst Probleme mit dem Lisp-Package auftreten.
;;;
;;; Revision 1.84  1993/04/16  08:01:53  ft
;;; Neuer Slot type-expander fuer built-in-class.
;;;
;;; Revision 1.83  1993/04/16  05:51:08  ft
;;; Erweiterung um built-in-class und structure-class als Subklassen von class-def.
;;;
;;; Revision 1.82  1993/04/14  07:56:14  kl
;;; Ignore-Deklaration in der u. a. Dummy-Methode geaendert.
;;;
;;; Revision 1.81  1993/04/07  15:53:38  hk
;;; Dummy Methode ((setf ?type-abstraction-function) T global-fun)
;;; definiert.
;;;
;;; Revision 1.80  1993/04/06  17:27:49  hk
;;; In special-sys-fun pass1 gestrichen, codegen -> c-inline.
;;;
;;; Revision 1.79  1993/03/25  09:36:53  uho
;;; Kommentar bei named-const um :forward erweitert.
;;;
;;; Revision 1.78  1993/03/25  08:53:00  hk
;;; Schreibfehler von jh behoben: boll -> bool.
;;;
;;; Revision 1.77 1993/03/24 14:52:10 jh 
;;; Annotationen unknown-caller in fun und cont sowie called-funs und
;;; other-funs in app eingefuehrt. Annotation analysed aus fun entfernt.
;;;
;;; Revision 1.76  1993/03/24  13:41:53  hk
;;; Symbole 'jump, 'closure, 'downfun in Slots heissen jetzt :jump,
;;; :closure und :downfun.
;;;
;;; Revision 1.75  1993/03/22  17:30:45  hk
;;; Keywords in LZS Slots.
;;;
;;; Revision 1.74  1993/03/22  09:31:17  uho
;;; Annotation  syntactically-exported  fuer Funktionen eingefuehrt.
;;;
;;; Revision 1.73  1993/03/19  11:05:31  jh
;;; Annotation local-fun-list im module wieder entfernt.
;;;
;;; Revision 1.72  1993/03/19  10:59:45  hk
;;; Neue Slots name, package im Modul.
;;;
;;; Revision 1.71  1993/03/18  14:15:15  uho
;;; Slot  source  zum Aufnehmen des Quellcodes von Funktionen
;;; in  fun  eingefuehrt.
;;;
;;; Revision 1.70  1993/03/16  16:53:39  jh
;;; analysed-Annotation fuer zws-object eingefuehrt.
;;;
;;; Revision 1.69  1993/03/16  13:44:04  ft
;;; ffzsdef wird jetzt hier required.
;;;
;;; Revision 1.68  1993/03/05  15:34:00  kl
;;; analysed-Annotation fuer Funktionen eingefuehrt.
;;;
;;; Revision 1.67  1993/03/04  10:44:21  kl
;;; Ueberfluessige Annotationen alter Typinferenzversionen entfernt.
;;;
;;; Revision 1.66  1993/03/02  13:18:03  ft
;;; class-defs mit einem Adress-Slot versehen.
;;;
;;; Revision 1.65  1993/02/23  15:39:23  ft
;;; Typ des Slots rest in params um symbol erweitert.
;;;
;;; Revision 1.64  1993/02/16  16:08:49  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.63  1993/02/10  11:21:05  jh
;;; Annotation local-fun-list in module hinzugefuegt.
;;;
;;; Revision 1.62  1993/02/10  09:42:32  ft
;;; Initialisierung der Annotation specializer-pos.
;;;
;;; Revision 1.61  1993/02/10  08:10:16  ft
;;; Annotation specializer-pos an generic-fun hinzugefuegt.
;;;
;;; Revision 1.60  1993/01/24  16:36:16  kl
;;; Typannotationen umgestellt.
;;;
;;; Revision 1.59  1993/01/20  15:16:58  kl
;;; defzws nach clcdef.lisp verlegt.
;;;
;;; Revision 1.58  1993/01/20  12:43:06  jh
;;; Typen der used-slots auf integer geaendert.
;;;
;;; Revision 1.57  1993/01/16  12:44:59  atr
;;; Slot destructive durch data-effects ersetzt.
;;;
;;; Revision 1.56  1993/01/15  09:52:48  hk
;;; Definition fuer ratio-form zunaechst herausgenommen, da der Typ
;;; ratio in clicc noch nicht implementiert ist.
;;;
;;; Revision 1.55  1993/01/15  09:40:25  hk
;;; Definition fuer complex-form zunaechst herausgenommen, da der Typ
;;; complex in clicc noch nicht implementiert ist.
;;;
;;; Revision 1.54  1993/01/10  17:20:59  kl
;;; Die Vorbesetzung der Typannotation von Variablen ist nun bottom-t.
;;;
;;; Revision 1.53  1993/01/05  15:11:36  kl
;;; Annotation used-as-functional-object fuer definierte Funktionen und
;;; Continuations eingefuehrt.
;;;
;;; Revision 1.52  1992/12/10  14:29:29  ft
;;; Writer-Definition in DEFZWS entfernt.
;;;
;;; Revision 1.51  1992/12/10  10:12:57  kl
;;; Annotationen fuer die Typinferenz bei defined-fun und imported-fun angelegt.
;;;
;;; Revision 1.50  1992/12/08  13:51:20  jh
;;; Annotation inline in fun eingefuegt.
;;;
;;; Revision 1.49  1992/11/29  13:26:44  atr
;;; bei FUN kein effect-slot mehr , bei app effect durch read/write-listen ersetzt.
;;;
;;; Revision 1.48  1992/11/24  15:55:23  kl
;;; initform fuer Typannotationen als bottom-t festgelgt. titypes eingebunden.
;;;
;;; Revision 1.47  1992/11/23  13:30:51  kl
;;; Annotation dyn-var-list in module eingefuegt.
;;;
;;; Revision 1.46  1992/11/17  16:06:50  kl
;;; Slottypen der einfachen Literale verfeinert.
;;;
;;; Revision 1.45  1992/11/05  12:11:45  ft
;;; Annotation body in generic-fun in fun umbenannt.
;;;
;;; Revision 1.44  1992/11/04  13:39:31  kl
;;; initform der Typannotationen entfernt.
;;;
;;; Revision 1.43  1992/10/29  07:23:37  ft
;;; body als Annotation an generic-fun hinzugefuegt.
;;;
;;; Revision 1.42  1992/10/12  14:01:39  atr
;;; Fehler korrigiert
;;;
;;; Revision 1.41  1992/10/12  13:56:46  atr
;;; Tippfehler behoben
;;;
;;; Revision 1.40  1992/10/12  13:47:50  atr
;;; Tippfehler behoben
;;;
;;; Revision 1.39  1992/10/12  13:26:33  atr
;;; Annotationen 'write-list 'read-list 'destructiv an FUN angefuegt
;;;
;;; Revision 1.38  1992/10/12  09:42:39  ft
;;; Neuer Slot class-precedence-list an class-def.
;;; Annotationen forward-ref und finalized an class-def geloescht.
;;;
;;; Revision 1.37  1992/10/01  10:55:59  ft
;;; Annotation finalized an class-def angefuegt.
;;;
;;; Revision 1.36  1992/09/25  16:35:33  kl
;;; Neue Klassen fuer die einfachen Literale eingefuehrt.
;;; Die leere Liste, Zeichen und Zahlen werden jetzt als einfache Literale
;;; repraesentiert. Die Konstante empty-list hat als Wert die leere Liste.
;;;
;;; Revision 1.35  1992/09/23  08:14:01  hk
;;; Definition von simple-literal auskommentiert, da SIMPLE-LITERAL-P in
;;; p2main.lisp definiert ist.
;;;
;;; Revision 1.34  1992/09/22  12:15:08  kl
;;; Neue Klasse simple-literal fuer Zahlen, Zeichen, usw. eingefuegt.
;;;
;;; Revision 1.33  1992/09/21  13:22:36  uho
;;; Kosmetik
;;;
;;; Revision 1.32  1992/09/21  13:17:27  uho
;;; Erklaerung eingefuegt, wie sich die Namen der Sprachbeschreibung zu den
;;; Namen der Implementierung verhalten.
;;;
;;; Revision 1.31  1992/09/15  14:22:03  kl
;;; Neue Klassen form und literal eingefuegt.
;;;
;;; Revision 1.30  1992/09/13  11:19:56  atr
;;; annotation effect bei fun und app hinzugefuegt
;;;
;;; Revision 1.29  1992/08/31  10:03:45  ft
;;; Tippfehler in method-def beseitigt.
;;;
;;; Revision 1.28  1992/08/25  13:45:20  ft
;;; Annotationen eingefuegt (class-def:forward-ref, slot-desc:declared-type)
;;;
;;; Revision 1.27  1992/08/10  16:51:37  hk
;;; Neue Klasse special-sys-fun-with-mv, Mehr slots an app und mv-lambda.
;;;
;;; Revision 1.26  1992/08/04  18:43:18  hk
;;; Unnoetige :initforms entfernt.
;;;
;;; Revision 1.25  1992/08/03  12:46:30  kl
;;; (in-package "CLICC") eingefuegt.
;;;
;;; Revision 1.24  1992/07/31  07:57:10  hk
;;; zws-object ist Superklasse aller Klassen der Zwischensprache.
;;;
;;; Revision 1.23  1992/07/30  13:30:20  hk
;;; Slot max-level von defined-fun bleibt uninitialisiert.
;;;
;;; Revision 1.22  1992/07/22  11:32:57  hk
;;; higher-level-calls --> deeper-level-calls.
;;;
;;; Revision 1.21  1992/07/07  09:47:51  hk
;;; Komponenete used in Sym eingefuegt.
;;;
;;; Revision 1.20  1992/06/04  16:15:30  hk
;;; Defaultwert fuer mv-spec von importierten Fkt. auf 1 gesetzt.
;;;
;;; Revision 1.19  1992/06/04  12:45:17  hk
;;; In sym den Slot ?const in ?constant-value umbenannt.
;;;
;;; Revision 1.18  1992/06/04  12:10:19  hk
;;; Den Slot mv-spec von cont mit nil initialisiert.
;;;
;;; Revision 1.17  1992/06/03  17:18:33  hk
;;; In defined-fun wurden deeper-level-calls in higher-level-calls umbenannt.
;;;
;;; Revision 1.16  1992/06/03  16:42:19  hk
;;; Schreibfehler
;;;
;;; Revision 1.15  1992/06/03  14:20:48  hk
;;; In setq-form Slot var-ref in location umbenannt, wg. named-const.
;;;
;;; Revision 1.14  1992/06/03  13:46:39  hk
;;; Slots in special-sys-fun mit nil vorbelegt.
;;;
;;; Revision 1.13  1992/06/03  13:39:34  hk
;;; In defined-sym im Slot package gibt es den ausgezeichneten Wert :uninterned.
;;;
;;; Revision 1.12  1992/06/03  08:29:55  hk
;;; In named-const Slot const in value umbenannt.
;;;
;;; Revision 1.11.1.1  1992/06/03  08:13:13  hk
;;; In named-const Slot const in value umbenannt.
;;;
;;; Revision 1.11  1992/06/03  07:21:32  hk
;;; Annotation read bei named-const hinzugefuegt.
;;;
;;; Revision 1.10  1992/06/02  14:24:08  hk
;;; In literal-instance slot-value-list durch value-list ersetzt.
;;;
;;; Revision 1.9  1992/05/31  19:54:18  hk
;;; package-list ist nur Annotation von module, toplevel-forms hat Typ fun.
;;;
;;; Revision 1.8  1992/05/27  15:27:56  hk
;;; Package Slot eines Symbols ist nil, falls uninterned, d.h. #:symbol.
;;;
;;; Revision 1.7  1992/05/27  14:25:08  hk
;;; Fuer die Klassen wird automatisch ein Praedikat <name>-p definiert.
;;; Module haben eine Komponente package-list (nur fuer CL).
;;; mv-app und apply-app gestrichen, dafuer mv-lambda neu hinzu.
;;; einge Umbenennungen: method -> method-def,
;;;                      struct-literal -> structured-literal.
;;; In Instanzen von params haben Slots den Defaultwert (); eine nicht weiter
;;; initialisierte Instanz entspricht damit einer leeren Lambda-Liste.
;;;
;;; Revision 1.6  1992/05/07  12:43:38  hk
;;; Slots 'symbol', 'initform' und 'initargs' zu 'class-def'
;;; hinzugefuegt. Nach Bonn geschickt.
;;;
;;; Revision 1.5  1992/04/30  12:38:22  hk
;;; Nach Berlin geschickt, nach Durchsicht der Bezeichner und
;;; Definition mittels 'defzws'
;;;
;;; Revision 1.4  1992/04/29  13:08:18  hk
;;; Zwischenversion vor der Umstellung auf Defclass.
;;;
;;; Revision 1.3  1992/04/24  09:42:57  hk
;;; Slot argument-precedence-order in generic-fun eingefuegt
;;;
;;; Revision 1.1  1992/04/24  07:16:06  hk
;;; Initial revision
;;;
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

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
   :initform nil :type bool))

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
  (exported :initform nil :type bool)
  (value                                ; literal || :unknown || :forward
   :initform :unknown)
  ;; Annotation
  ;;-----------
  (read                                 ; Anzahl der lesenden Zugriffe
   :initform 0 :type integer)
  (symbol :type symbol))                ; Name im Quelltext

;;------------------------------------------------------------------------------
(defzws defined-named-const (named-const))

;;------------------------------------------------------------------------------
(defzws imported-named-const (named-const))


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
(defzws defined-class (class-def))

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
  (params :type params)
  ;; Annotation
  ;;-----------
  (read-list                            ; Liste der gelesenen globalen Variablen
   :initform nil 
   :type (or list integer))            
  (write-list                           ; Liste der veraenderten Variablen    
   :initform nil                        
   :type (or list integer))            
  (data-effects                         ; Welche Operationen auf Daten ?
   :initform nil
   :type symbol)
  (symbol :type (or symbol cons))       ; Name im Quelltext
  (used                                 ; Anzahl der Funktionsaufrufe
   :initform 0 :type integer)
  (closure                              ; Closure gebildet, Art
   :initform nil
   :type (member nil t :closure :downfun))
  (downward-args                        ; Position der 'downward' Param.
   :initform () :type list)
  (adr :type string)                    ; Adresse der Funktion im Zielcode
  (mv-spec                              ; Anzahl Multipler Werte
   :initform nil
   :type (or (member nil t :jump) integer))
  (par-spec :type integer)              ; (mindest) Parameterzahl
  (unknown-caller                       ; Gibt es unbekannte Aufrufstellen?
   :initform nil
   :type bool)
  (result-type :initform bottom-t)      ; Resultattyp der Funktion
  (inline :type bool :initform nil)     ; Funktion inline-compilieren?
  (syntactically-exported               ; Markierung, ob Funktion syntaktisch
   :initform nil                        ; exportiert wird
   :type bool)
  source)                               ; Quellcode der Funktion

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
  (has-funs-as-args                     ; ob Parameter nur an Funktions-
   :initform nil                        ; posotion steht.
   :type list)
  body)

;;------------------------------------------------------------------------------
;; Definierte einfache Funktion
;;------------------------------------------------------------------------------
(defzws defined-fun (simple-fun)
  ;; Annotation
  ;;-----------
  (const-list      :type list)              ; Direkt im Rumpf vork. struct-lit.
  (local-funs      :type list :initform ()) ; Direkt im Rumpf lokal def. Funkt.
  (max-level       :type integer)           ; max. stat. Niveau lok. def. Funkt.
  (mv-calls        :type list :initform ()) ; mv-spec haengt von diesen Fkt. ab
  (mv-called-by    :type list :initform ()) ; Diese Fkt. benutzen mv-spec
  (called-by       :type list :initform ()) ; Liste der Aufrufer dieser Funktion
  (pred-type-env   :type list :initform ()) ; Eintrittstypumgebung
  (result-type-env :type list :initform ()) ; Ergebnistypumgebung
  (deeper-level-calls                       ; Aufrufe lok. Fkt. hoeheren Niveaus
   :initform ()  :type list))

;;------------------------------------------------------------------------------
;; Globale definierte einfache Funktion
;;------------------------------------------------------------------------------
(defzws global-fun (defined-fun)
  (exported :initform nil :type bool)
  ;; Annotation
  ;;----------
  (call-in :type list :initform ()))    ; Informationen ueber den Call-In-
                                        ; Aufruf der Funktion.

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
   :initform () :type list)
  (free-local-funs                      ; im Rumpf aufgerufene lokale Funktionen
   :initform () :type list)
  (free-in                              ; kommt in diesen Funktionen frei vor
   :initform () :type list)
  (free-lex-vars                        ; freie statisch geb. lokale Variablen
   :initform () :type list)
  (num-free-lex-vars :type integer)     ; Laenge von 'free-lex-vars'
  (closure-offset :type integer))       ; Offset des Closure-Objekts im Stack

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
  (mv-spec                              ; Anzahl Multipler Werte
   :initform 1                          ; 1 ist Default, nur Ausnahmen angeben
   :type (or (member nil t :jump) integer))
  (type-abstraction-function            ; Bildet Argumenttypen auf Resultattypen
   :initform nil                        ; ab.
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
(defzws special-sys-fun (imported-fun)
  ;; Annotation
  ;;-----------
  (opti-pass       :initform nil)
  (pass3           :initform nil)
  (tipass          :initform nil)
  (weight-c-inline :initform nil)
  (c-inline        :initform nil)
  (special-caller  :initform nil))

;;------------------------------------------------------------------------------
;; Systemfunktion, die einer speziellen Behandlung bedarf und Multiple Werte
;; generiert. Das ist zur Zeit nur die Common Lisp Funktion values.
;;------------------------------------------------------------------------------
(defzws special-sys-fun-with-mv (special-sys-fun))

;;------------------------------------------------------------------------------
(defzws generic-fun (fun)
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
(defzws defined-generic-fun (generic-fun))

;;------------------------------------------------------------------------------
(defzws global-generic-fun (defined-generic-fun)
  (exported :initform nil :type bool))

;;------------------------------------------------------------------------------
(defzws imported-generic-fun (generic-fun)
  (exported :initform nil :type bool)
  ;; Annotation
  ;;-----------
  (mv-spec                              ; Anzahl Multipler Werte
   :initform 1                          ; 1 ist Default, nur Ausnahmen angeben
   :type (or (member nil t :jump) integer)))

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
  (read-list                            ; Liste der gelesenen globalen Variablen
   :initform nil 
   :type (or list integer))            
  (write-list                           ; Liste der veraenderten Variablen    
   :initform nil                        
   :type (or list integer))
  (data-effects                         ; Welche Operationen auf Daten ?
   :initform nil
   :type symbol)
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
  (fn-on-mv-pos  :type bool)            ; auf Pos., die MV + Fkt. Obj. ben.
  (binding-stack-level :type integer)   ; Relative Hoehe des Binding-Stacks
  (mv-spec                              ; Anzahl Multipler Werte
   :initform nil
   :type (or (member nil t :jump) integer))
  (mv-calls                             ; Fkt. von denen mv-spec abhaengt
   :initform () :type list)
  (only-local :type bool)               ; Nur lokale Anwendungen der Contin. ?
  (unknown-caller                       ; Gibt es unbekannte Aufrufstellen?
   :initform nil
   :type bool)
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
  (binding-stack-level :type integer))  ; relative Hoehe des Binding-Stacks

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
  (adr           :type string))         ; Die Adresse im Zielcode

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
  (mv-spec :initform nil                ; Anzahl Multipler Werte
   :type (or (member nil t :jump) integer))
  (mv-calls :initform () :type list))   ; mv-spec haengt von diesen Fkt. ab

;;------------------------------------------------------------------------------
(provide "zsdef")

