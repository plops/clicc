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
;;; Funktion : Codegenerierung: Struktur- und Variablen-Deklarationen
;;;
;;; $Revision: 1.19 $
;;; $Id: cgdefs.lisp,v 1.19 1994/12/01 16:53:18 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")     

;;------------------------------------------------------------------------------
;; Klassen und Subklassen der Knoten der Zwischensprache, die nur waehrend der
;; Codegenerierung verwendet werden.  Instanzen dieser Klassen werden in
;; *result-spec* eingetragen, um Spezialfaelle zu beschreiben.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Das Resultat soll auf der Spitze des LISP-Stacks erzeugt werden.
;;------------------------------------------------------------------------------
(defzws stacktop-result (local-static))

;;------------------------------------------------------------------------------
;; Ein Wert im Multiple-Value-Buffer soll erzeugt werden.
;;------------------------------------------------------------------------------
(defzws mv-buf ()
  (offset :type integer))               ; Offset im Multiple-Value-Buffer

;;------------------------------------------------------------------------------
;; Globale Variablen
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; die naechste freie Position im LISP-Stack
;;------------------------------------------------------------------------------
(defvar *stack-top* 0)

;;------------------------------------------------------------------------------
;; Ein LISP-Ausdruck, der beschreibt, 
;; - ob ein Resultat benoetigt wird oder nicht.
;; - ob als Resultat ein LISP-Wert oder ein Boolescher C-Wert benoetigt wird.

;; - wo der LISP-Wert erzeugt werden soll.
;;------------------------------------------------------------------------------
(defvar *result-spec*)
(proclaim '(type (or (member nil C-bool) static dynamic mv-buf) *result-spec* ))

;;------------------------------------------------------------------------------
;; Ein String, der in C-Syntax einen Booleschen Ausdruck beschreibt;
;; das Boolesche Resultat eines LISP-Ausdrucks.
;;------------------------------------------------------------------------------
(defvar *C-bool*)

;;------------------------------------------------------------------------------
;; Anzahl gebundener Special Variablen.
;;------------------------------------------------------------------------------
(defvar *special-count*)

;;------------------------------------------------------------------------------
;; Jeder special Variablen wird der aktuelle Wert von *special-count*
;; zugeordnet. Diese Zuordnung ist zwar nicht eindeutig innerhalb
;; eines Programms, genuegt aber, um waehrend der Uebersetzung die special
;; Variablen eindeutig zu identifizieren.
;; 
;; Eine Liste von Nummern, die die special Variablen bezeichnen,
;; die beim Verlassen des Lambda-Ausdrucks NICHT restauriert werden duerfen,
;; da den special Variablen schon ein neuer Wert zugewiesen wurde.
;;------------------------------------------------------------------------------
(defvar *do-not-restore*)

;;------------------------------------------------------------------------------
;; Das statische Niveau der gerade uebersetzten Funktion
;;------------------------------------------------------------------------------
;;Schon in p3main definiert
;;(defvar *level*)

;;------------------------------------------------------------------------------
;; Die Closure mit dem hoechsten Level, innerhalb der der gerade uebersetze
;; Ausdruck liegt, (Repraesentiert durch die zugehoerige local-fun Struktur).
;; NIL, falls der Ausdruck innerhalb keiner Closure liegt.
;;------------------------------------------------------------------------------
(defvar *closure*)

;;------------------------------------------------------------------------------
;; Das statische Niveau der Closure in *closure* oder
;; das statische Niveau einer globalisierten lokalen Funktion oder
;; 0, wenn gerade keine Closure uebersetzt wird.
;;------------------------------------------------------------------------------
(defvar *cl-level*)

;;------------------------------------------------------------------------------
;; Anzahl der neu angelegten DOWNFUN-Parameter einer Funktion
;;------------------------------------------------------------------------------
(defvar *downfun-count*)

;;------------------------------------------------------------------------------
;; Zaehler zum Generieren eindeutiger marken fuer Tagbody bzw. Continuations
;;------------------------------------------------------------------------------
(defvar *tagbody-counter*)
(defvar *cont-counter*)
(defvar *if-counter*)

;;------------------------------------------------------------------------------
;; Beschreibung von Quelle und Ziel des letzten Kopierbefehls fuer Lisp-Daten.
;; Diese Information wird benutzt, um redundante Kopierbefehle zu vermeiden.
;; Wenn kein Kopierbefehl ausgefuehrt wurde, hat *copy-source* den wert nil.
;;------------------------------------------------------------------------------
(defvar *copy-source*)
(defvar *copy-dest*)

;;------------------------------------------------------------------------------
;; Anzahl der Tabs, die am Anfang der naechsten mit C-Ln erzeugten Zeile 
;; erzeugt werden soll. Wird durch C-blockstart und C-blockend veraendert
;;------------------------------------------------------------------------------
(defvar *C-indent*)

;;------------------------------------------------------------------------------
;; Flag, das anzeigt, ob gerade ein C-Block durch '{' geoeffnet wurde
;;------------------------------------------------------------------------------
(defvar *block-open*)

;;------------------------------------------------------------------------------
;; Liste der Werte von *C-indent* fuer die Aufrufe der Funktion 'C-blockstart',
;; die keinen Code erzeugt haben, weil dann zwei offnende '{' direkt aufeinander
;; gefolgt waeren.
;;------------------------------------------------------------------------------
(defvar *not-opened*)

;;------------------------------------------------------------------------------
;; Zaehler fuer die Anzahl der Zeilen in der gerade generierten C Datei
;;------------------------------------------------------------------------------
(defvar *C-line-count*)

;;------------------------------------------------------------------------------
;; Die Datei, in die der C-Code geschrieben wird
;;------------------------------------------------------------------------------
(defvar *C-file*)

;;------------------------------------------------------------------------------
;; Flag, ob eine Optimierung eines Restlistenparamters durchgeführt werden
;; soll bzw kann. Dieser wird dann direkt als Zeiger auf die im Stack liegende
;; Restliste direkt in C verwaltet und das erzeugen der Restliste mit Flist
;; wird unterdrückt.
;;------------------------------------------------------------------------------
(defvar *rest-optimization* nil)

;;------------------------------------------------------------------------------
;; Zähler für eindeutige Namen von Restvariablen
;;------------------------------------------------------------------------------
(defvar *rest-var-count*)

;;------------------------------------------------------------------------------
;; Für die Statistik
;;------------------------------------------------------------------------------
(defvar *rlo-statistics-rest-funs*)
(defvar *rlo-statistics-rest-opt*)
(defvar *rlo-statistics-rest-usage*)

;;------------------------------------------------------------------------------
(provide "cgdefs")
