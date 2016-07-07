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
;;; Funktion : Definitionen der glob. Variablen und Konstanten der Typinferenz
;;;
;;; $Revision: 1.41 $
;;; $Id: tidef.lisp,v 1.41 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Natuerliche Zahl, die angibt, wie intensiv die Typinferenz arbeiten soll. Je 
;; hoeher der angegebenen Wert ist, desto `globaler' arbeitet die Typinferenz.  
;; Mit niedrigerem Grad sinkt die fuer die Typinferenz benoetigte Rechenzeit, 
;; allerdings werden dafuer in der Regel die inferierten Ergebnisse schwacher.
;;
;; Die genaue Einteilung ist wie folgt:
;;
;;  0 : Es werden keine Typen inferiert. Die Typannotationen der Literale 
;;      werden auf die entsprechenden Typen, alle anderen Typannotationen
;;      werden auf das Top-Element des Typverbandes gesetzt.
;;
;;  1 : Es werden keine Aufrufkontexte beachtet. Das angewendete Verfahren 
;;      entspricht in diesem Fall einer intraprozeduralen Typinferenz.
;;      Diese Stufe wird realisiert, indem die Eintrittsumgebungen aller
;;      Funktionen mit Top-Elementen vorbesetzt werden und der Ergebnistyp
;;      von Funktionen auf Top gesetzt wird.
;;
;;  2 : In dieser Stufe werden alle Aufrufkontexte beachtet.
;;
;;  3 : In dieser Stufe werden die Typen globaler Variablen intensiver als in
;;      den anderen Stufen behandelt.
;;      Die Ausfuehrung dieser Analysestufe benoetigt in der Regel deutlich 
;;      mehr Speicher als die anderen Stufen. Sie ermittelt dafuer die im 
;;      Vergleich zu den anderen Stufen praezisesten Typinformationen.
;;------------------------------------------------------------------------------
(defvar *ti-level* 2)


;;------------------------------------------------------------------------------
;; Prädikate für einzelne Typinferenzfeatures:
;;------------------------------------------------------------------------------


;; Soll eine Typinferenz auch auf den Nichtliteralen durchgeführt werden?
;;------------------------------------------------------------------------------
(defun do-type-inference-on-non-literals ()
  (> *ti-level* 0))


;; Soll eine interprozedurale Analyse durchgeführt werden? 
;;------------------------------------------------------------------------------
(defun do-interprocedural-type-inference ()
  (> *ti-level* 1))


;; Sollen für dynamisch gebundene Variablen präzise Typen ermittelt werden?
;;------------------------------------------------------------------------------
(defun use-bindings-of-dynamic-variables ()
  (> *ti-level* 2))


;; Soll die (aufwendigere) Typsemantik für Funktionen verwendet werden?
;;------------------------------------------------------------------------------
(defun use-precise-function-type-semantics ()
  (> *ti-level* 2))


;;------------------------------------------------------------------------------
;; Natuerliche Zahl fuer den Gespraechigkeitsgrad der Typinferenz. Je hoeher der
;; angegebenen Wert ist, desto mehr Ausgaben und Meldungen werden waehrend des
;; Durchlaufs erzeugt. Ist *ti-verbosity*=0, werden keine Ausgaben geschrieben.
;;------------------------------------------------------------------------------
(defvar *ti-verbosity* 3)


;;------------------------------------------------------------------------------
;; Diese Variable wird zum Propagieren von Typbindungen verwendet. Sie enthaelt
;; die Typumgebung fuer locations in Form einer Assoziationsliste. Die Liste
;; besteht aus Paaren (<location> . <zugeordneter Typ>).
;;------------------------------------------------------------------------------
(defvar *type-environment*)        


;;------------------------------------------------------------------------------
;; Menge der noch zu analysierenden Funktionen und Klassen. Diese Variable wird
;; im zweiten Pass der Typinferenz (tipass2) verwendet. 
;;------------------------------------------------------------------------------
(defvar *ti-workset*)  


;;------------------------------------------------------------------------------
;; *successor-workset* enthaelt die Menge der zu analysierenden Nachfolger der 
;; aktuell analysierten Funktion. Sie enthaelt somit einen kleinen Ausschnitt
;; der Menge aller zu bearbeitenden Funktionen. 
;; Die Einfuehrung dieser Menge beschleunigt die Fixpunktiteration, weil nicht 
;; alle Mengenoperation auf der evtl. sehr grossen *ti-workset* geschehen.
;;------------------------------------------------------------------------------
(defvar *successor-workset*)


;;------------------------------------------------------------------------------
;; Sind die Typdeklarationen initialisiert worden? Typdeklarationen sind die
;; Typabstraktionsfunktionen der importierten Funktionen und Funktionen zur 
;; Typzusicherungen fuer Ausdruecke an Praedikatsposition eines Konditionals.
;;------------------------------------------------------------------------------
(defvar *ti-type-declarations-are-initialized*)


;;------------------------------------------------------------------------------
;; Umgebung, in der Funktionen fuer Typzusicherungen in if-Konstrukten in Form
;; einer Assoziationsliste vorliegen. Sie enthaelt Paare der Gestalt:
;; (<Praedikat> . <Typzusicherungsfunktion>)
;;------------------------------------------------------------------------------
(defvar *ti-predicate-assertion-environment* ())


;;------------------------------------------------------------------------------
;; Angabe, wie haeufig die Groesse der *ti-workset* ausgeben werden soll.
;;------------------------------------------------------------------------------
(defconstant *ti-write-size-of-workset-interval* 300)


;;------------------------------------------------------------------------------
(provide "tidef")



