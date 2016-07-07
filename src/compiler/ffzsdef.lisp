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
;;; Funktion : Zwischensprachkonstrukte fuer das Foreign Function Interface
;;;
;;; $Revision: 1.13 $
;;; $Id: ffzsdef.lisp,v 1.13 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------


;;------------------------------------------------------------------------------
;; TODO-Liste
;;------------------------------------------------------------------------------
;; * in def-zws foreign-fun noch die Inforamtionen fuer die Seiteneffektanalyse
;;   eintragen.


;;------------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Fuer Foreign Functions wird ein eigenes Zwischensprachkonstrukt
;; eigerichtet, da keins der Vorhandenen geeignete Slots enthaelt.
;; Foreign-Functions sind naemlich keine Funktionen im Sinne von LISP.  Die
;; Slots im Einzelnen: 
;;
;; - arguments: Enthaelt die Typ-Signatur der Foreign-Function
;; - name: Name der zu generierenden FF
;; - callback: Die FF ruft selbst LISP-Funktionen auf.
;; - return-type: Der von der FF zurueckgelieferte Wert.
;;------------------------------------------------------------------------------
(defzws foreign-fun (form)
  (arguments :type list)
  (name :type string)
  (callback :initform nil :type bool)
  return-type
  ;; Annotation
  ;;-----------
  (par-spec :type integer)
  (symbol :type symbol)                 ; Name im Quelltext
  (other-sym :type symbol)
)


;;------------------------------------------------------------------------------
;; Call-In-Funktions sind LISP-Funktionen, die speziell behandelt werden
;; muessen. Da sie vorwaerts deklariert werden, koennen die Informationen
;; nicht ins Globale Environment unter operators abgelegt werden. Deshalb
;; muessen diese gemerkt werden. Dies geschieht durch folgendes lokales
;; Zwischensprachkonstrukt. Man haette auch eine Struktur dafuer verwenden
;; koennen, doch so geht es einfacher.
;; Fuer dieses Konstrukt muessen keine Methoden geschrieben werden.
;;------------------------------------------------------------------------------
(defzws call-in-fun ()
  (arguments :type list)
  (foreign-name :type string)
  return-type
)

(provide "ffzsdef")
