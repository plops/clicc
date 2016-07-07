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
;;; Funktion : Codegenerierung: Funktionen, die C-Code erzeugen
;;;
;;; $Revision: 1.36 $
;;; $Id: cgcode.lisp,v 1.36 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defconstant MAX-INDENT 6)

;;------------------------------------------------------------------------------
;; *CG-COMMENT* Enthaelt Kommentarstrings fuer generierte Zeilen oder NIL,
;; falls kein Kommentar am Ende der Zeile ausgegeben werden soll. Kann
;; waehrend des Codegenerierens gesetzt oder mittels (ADD-COMMENT <string>)
;; verlaengert werden. Wird unmittelbar vor dem naechsten Zeilenwechsel
;; ausgegeben und dann wieder auf NIL zurueckgesetzt.
;;------------------------------------------------------------------------------
(defvar *CG-COMMENT* NIL) 


;;------------------------------------------------------------------------------
(proclaim
 '(inline
   C-Out C-Ln C-Empty-Ln C-Cmd C-Control C-Decl C-init-CL_FORM CC-Code getCode
   C-MacroCmd
   C-MacroCall C-Call C-Abort C-Lispcall C-copy C-assign C-goto C-label
   C-break C-SetMV C-ResetMV C-blockstart C-blockend C-if C-else C-switch
   C-case C-defaultcase C-MemUpMove C-MVToStack C-VarDecl C-VarDeclInit
   C-ExternVarDecl
   C-PtrDecl C-ArrayDecl C-ExternArrayDecl C-PtrArrayDecl C-ArrayInitDecl
   C-StaticArrayInitDecl C-StaticDoubleInit
   C-static-GLOBAL_FUNARG-Init C-initstart C-initend CC-MacroCall CC-Call
   CC-Cast CC-Address CC-NameConc CC-make-bool CC-bool CC-op+ CC-op- CC-op>
   CC-op< CC-opIor CC-op^ CC-op& CC-op~ CC-op<< CC-op>>
   CC-op!= CC-op== CC-op&& CC-opIIor CC-arrayptr CC-arraykomp CC-structkomp 
   CC-Stack CC-mv_buf))

;;------------------------------------------------------------------------------
;; Namen von Identifikatoren
;;------------------------------------------------------------------------------
(defconstant C-bool_result "bool_result")
(defconstant C-display "display")
(defconstant C-new_display "new_display")
(defconstant C-nargs "nargs")
(defconstant C-local "local")
(defconstant C-rest "rest")
(defconstant C-ALL_OPT_SUPPLIED "ALL_OPT_SUPPLIED")
(defconstant C-keylist "keylist")
(defconstant C-supl_flags "supl_flags")
(defconstant C-mv_count "mv_count")
(defconstant C-TRUE "TRUE")
(defconstant C-FALSE "FALSE")


;;------------------------------------------------------------------------------
;; Fuegt den String COMMENT abgetrennt durch TAB und von C-Kommentarzeichen
;; umschlossen an der aktuellen Position im generierten Code ein.
;; Enthaelt COMMENT selbst "*/" so wird dies durch "*\/" dargestellt.
;;------------------------------------------------------------------------------
(defun C-Comment (comment)
  ;; alle */ in comment durch *\/ ersetzen
  ;;--------------------------------------
  (do ( (pos (search "*/" comment) (search "*/" comment)))
      ((null pos) t)
    (setq comment (concatenate 'string
                               (subseq comment 0 pos)
                               "*\\/"
                               (subseq comment (+ 2 pos)))))
  (write-char #\Tab *C-file*)
  (write-string "/* " *C-file*)
  (write-string comment *C-file*)
  (write-string "*/" *C-file*))


;;------------------------------------------------------------------------------
;; Fuegt an den Kommentarstring fuer die aktuelle Zeile den String
;; ADDITIONAL-COMMENT an. Ein trennendes bzw. abschliessendes Leerzeichen wird
;; automatisch angefuegt.
;;------------------------------------------------------------------------------
(defun add-comment (additional-comment)
  (setq *CG-COMMENT* (concatenate 'string *CG-COMMENT* additional-comment " ")))

;;------------------------------------------------------------------------------
;; Fuegt den in der globalen Variablen *CG-COMMENT* aufgesammelten String an
;; der aktuellen Position im generierten Code als C-Kommentar ein. Setzt
;; *CG-COMMENT* auf NIL zurueck. Falls *CG-COMMENT* schon urspruenglich den
;; Wert NIL hat, wird kein Kommentar ausgegeben.
;;------------------------------------------------------------------------------
(defun CG-Comment ()
  (when *CG-COMMENT*
    (C-Comment *CG-COMMENT*)
    (setq *CG-COMMENT* nil)))


;;------------------------------------------------------------------------------
;; Beenden der aktuellen Zeile im C-Code, ggf. Ausgabe eines Zeilenkommentars.
;;------------------------------------------------------------------------------
(defun C-Newline ()
  (CG-Comment)
  (incf *C-line-count*)
  (terpri *C-file*))

;;------------------------------------------------------------------------------
;; Ausgabe einer Leerzeile im C-Code.
;;------------------------------------------------------------------------------
(defun C-Empty-Ln ()
  (incf *C-line-count*)
  (terpri *C-file*))
          
;;------------------------------------------------------------------------------
;; Fuegt die Argumente (Strings, Buchstaben oder Zahlen) zu einem String
;; zusammen, der dann als C-Code ausgeben wird. Es erfolgt kein Zeilenumbruch.
;;------------------------------------------------------------------------------
(defun C-Out (&rest args)
  (let ((*print-circle* nil))
    (dotimes (i *C-indent*)
      (write-char #\Tab *C-file*))
    (dolist (arg args)
      (if (stringp arg)
        (write-string arg *C-file*)
        (princ arg *C-file*)))))

;;------------------------------------------------------------------------------
;; Ausgabe einer Zeile C-Code
;;------------------------------------------------------------------------------
(defun C-Ln (&rest args)
  (apply #'C-Out args)
  (C-Newline))

;;------------------------------------------------------------------------------
;; Ausgabe einer mit ";" terminierten Anweisung im C-Code
;; Optimierung aufeinanderfolgender Kopieranweisungen kann nicht mehr erfolgen.
;; Deklarationen koennen erst nach dem erneuten oeffnen eines Blocks erfolgen.
;; Ist die globale Variable *CG-COMMENT* gesetzt, wird die Zeile mit diesem
;; Kommentar verziert. Jeder Kommentar wird genau einmal ausgegeben.
;;------------------------------------------------------------------------------
(defun C-Cmd (&rest args)
  (apply #'C-Out args)
  (write-char #\; *C-file*)
  (C-Newline)
  (setq *copy-source* nil)
  (setq *block-open* nil))

;;------------------------------------------------------------------------------
;; Ausgabe einer Zeile C-Code, die den Kontrollfluss aendert.
;; Optimierung aufeinanderfolgender Kopieranweisungen kann nicht mehr erfolgen.
;; Deklarationen koennen erst nach dem erneuten oeffnen eines Blocks erfolgen.
;;------------------------------------------------------------------------------
(defun C-Control (&rest args)
  (apply #'C-Ln args)
  (setq *copy-source* nil)
  (setq *block-open* nil))

;;------------------------------------------------------------------------------
;; Ausgabe einer mit ";" terminierten Zeile im C-Code
;;------------------------------------------------------------------------------
(defun C-Decl (&rest args)
  (apply #'C-Out args)
  (write-char #\; *C-file*)
  (C-Newline))

;;------------------------------------------------------------------------------
;; Ausgabe von syntaktisch falschem Code, um sicherzustellen, dass der
;; C-Compiler eine Datei nicht uebersetzt, wenn bei der Uebersetzung nach
;; C Fehler aufgetreten sind.
;;------------------------------------------------------------------------------
(defun C-BadCode ()
  (C-Ln "#define " "ERROR_IN_GENERATED_CODE 0")
  (C-Ln "#define " "ERROR_IN_GENERATED_CODE 1"))

;;------------------------------------------------------------------------------
;; Fuegt die Argumente (Strings, Buchstaben oder Zahlen) zu einem String
;; zusammen, der dann als C-Code verwendet wird.
;;------------------------------------------------------------------------------
(defun CC-Code (&rest args)
  (let ((*print-circle* nil))
    (format nil "~{~A~}" args)))

;;------------------------------------------------------------------------------
(defun getCode (&rest rest)
  (let ((*print-circle* nil))
    (apply #'format nil rest)))

;;------------------------------------------------------------------------------
(defun C-include (include-file)
  (C-Ln "#include \"" include-file "\""))

;;------------------------------------------------------------------------------
(defun C-sysinclude (include-file)
  (C-Ln "#include <" include-file ">"))

;;------------------------------------------------------------------------------
;; C-Anweiungen
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defun C-MacroCmd (name)
  (C-Cmd name))

;;------------------------------------------------------------------------------
;; Ausgabe des C-Codes zum Aufruf eines Macros
;;------------------------------------------------------------------------------
(defun C-MacroCall (name &rest args)
  (C-Cmd (apply #'CC-MacroCall name args)))

;;------------------------------------------------------------------------------
;; Ausgabe des C-Codes zum Aufruf einer Funktion
;;------------------------------------------------------------------------------
(defun C-Call (name &rest args)
  (C-Cmd (apply #'CC-Call name args)))

;;------------------------------------------------------------------------------
;; Ausgabe des C-Codes zum Abbruch der Programmausfuehrung
;;------------------------------------------------------------------------------
(defun C-Abort (message)
  (C-Call "Labort" message))

;;------------------------------------------------------------------------------
;; Ausgabe des C-Codes zum Aufruf einer uebersetzten Lisp-Funktion
;;------------------------------------------------------------------------------
(defun C-Lispcall (name base nargs &optional display)
  (let (extra-args)
    (when nargs (push nargs extra-args))
    (when display (push display extra-args))
    (apply #'C-Call name base extra-args)))

;;------------------------------------------------------------------------------
(defun C-copy (source dest)
  (C-MacroCall "COPY" source dest))

;;------------------------------------------------------------------------------
;; Ausgabe einer C-Zuweisungs-Anweisung
;;------------------------------------------------------------------------------
(defun C-assign (dest src)
  (C-Cmd dest " = " src))

;;------------------------------------------------------------------------------
;; Ausgabe einer C-goto-Anweisung
;;------------------------------------------------------------------------------
(defun C-goto (label)
  (C-Cmd "goto " label))

;;------------------------------------------------------------------------------
;; Ausgabe eines C-labels, gefolgt von einer leeren Anweisung.
;; Die leere Anweisung ist notwendig, weil im aktuellen Block evtl. keine
;; Anweisung mehr folgt.
;;------------------------------------------------------------------------------
(defun C-label (label)
  (C-Cmd label ":"))

;;------------------------------------------------------------------------------
;; Ausgabe einer C-break-Anweisung
;;------------------------------------------------------------------------------
(defun C-break ()
  (C-Cmd "break"))

;;------------------------------------------------------------------------------
;; Setzt die globale Variable, die die Anzahl der gueltigen Multiple-Values
;; enthaelt, auf einen Wert.
;;------------------------------------------------------------------------------
(defun C-SetMV (i)
  (C-Assign C-mv_count i))

;;------------------------------------------------------------------------------
(defun C-ResetMV ()
  (C-SetMV 1))

;;------------------------------------------------------------------------------
;; Strukturierte Anweisungen
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defun C-blockstart ()
  (cond
    (*block-open* (push *C-indent* *not-opened*))
    (T (C-Ln "{")
       (incf *C-indent*)
       (setq *block-open* t))))

;;------------------------------------------------------------------------------
(defun C-blockend ()
  (cond
    ((eq (car *not-opened*) *C-indent*) (pop *not-opened*))
    (T (decf *C-indent*)
       (C-Control "}"))))

;;------------------------------------------------------------------------------
;; Ausgabe des Tests einer C-if-Anweisung
;;------------------------------------------------------------------------------
(defun C-if (pred)
  (C-Control "if(" pred ")"))

;;------------------------------------------------------------------------------
;; Ausgabe des Tests einer flachen C-if-Anweisung: if( !(<pred>) ) goto <label>;
;;------------------------------------------------------------------------------
(defun C-flat-if (pred label)
  (C-Control
   "if(!(" pred
   (concatenate 'string ")) goto " label ";")))

;;------------------------------------------------------------------------------
(defun C-else ()
  (C-Control "else"))

;;------------------------------------------------------------------------------
;; Ausgabe des Tests einer C-switch-Anweisung
;;------------------------------------------------------------------------------
(defun C-switch (pred)
  (C-Control "switch(" pred ")"))

;;------------------------------------------------------------------------------
;; Ausgabe eines C-Case-Labels
;;------------------------------------------------------------------------------
(defun C-case (label)
  (C-Control "case " label ":"))

;;------------------------------------------------------------------------------
;; Ausgabe eines C-Default-Case-Labels
;;------------------------------------------------------------------------------
(defun C-defaultcase ()
  (C-Control "default:"))

;;------------------------------------------------------------------------------
;; Verschiebt in einem Array von CL_FORMs den Bereich [SrcLow .. SrcHigh[ um
;; MoveDiff CL_FORMs in Richtung hoeherer Adressen. Quelle und Ziel koennen
;; sich uebelappen.
;;------------------------------------------------------------------------------
(defun C-MemUpMove (SrcHigh SrcLow MoveDiff)
  (C-MacroCall "MEM_UP_MOVE" SrcHigh SrcLow Movediff))

;;------------------------------------------------------------------------------
;; Kopiert N-1 CL_FORMs beginned bei Offset im aktuellen Activation Record in
;; den Multiple-Value Buffer.
;;------------------------------------------------------------------------------
(defun C-MVToStack (N Offset)
  (C-MacroCall "MV_TO_STACK" N Offset))
;;------------------------------------------------------------------------------
;; Eine Anweisung, die bei der ersten Ausführung keinen Effekt hat und bei
;; allen nachfolgenden Ausführungen die Ausgeführung der aktuellen Funktion
;; abbricht.
;;------------------------------------------------------------------------------
(defun C-only-once ()
  (C-Macrocall "ONLY_ONCE"))

;;------------------------------------------------------------------------------
;; Variablen Deklarationen + Initialisierungen
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
(defun C-VarDecl (typ name)
  (C-Decl typ " " name))

;;------------------------------------------------------------------------------
(defun C-VarDeclInit (typ name value)
  (C-Decl typ " " name " = " value))

;;------------------------------------------------------------------------------
(defun C-ExternVarDecl (typ name)
  (C-Decl "extern " typ " " name))

;;------------------------------------------------------------------------------
(defun C-PtrDecl (typ name)
  (C-Decl typ " *" name))

;;------------------------------------------------------------------------------
(defun C-ArrayDecl (typ name size)
  (C-Decl typ " " name "[" size "]"))

;;------------------------------------------------------------------------------
(defun C-ExternArrayDecl (typ name)
  (C-Decl "extern " typ " " name "[]"))

;;------------------------------------------------------------------------------
(defun C-PtrArrayDecl (typ name size)
  (C-Decl typ " *" name "[" size "]"))

;;------------------------------------------------------------------------------
(defun C-StaticDoubleInit (name value)
  (C-Decl "static double " name " = " value))

;;------------------------------------------------------------------------------
(defun C-GLOBAL_FUNARG-Init (name fun-name par-spec)
  (C-MacroCall "GEN_GLOBAL_FUNARG" name fun-name par-spec))

;;------------------------------------------------------------------------------
(defun C-static-GLOBAL_FUNARG-Init (name fun-name par-spec)
  (C-MacroCall "GEN_STATIC_GLOBAL_FUNARG" name fun-name par-spec))

;;------------------------------------------------------------------------------
(defun C-ArrayInitDecl (typ name)
  (C-Ln typ " " name "[] ="))

;;------------------------------------------------------------------------------
(defun C-StaticArrayInitDecl (typ name)
  (C-Ln "static " typ " " name "[] ="))
  
;;------------------------------------------------------------------------------
(defun C-initstart ()
  (C-Ln "{")
  (incf *C-indent*))

;;------------------------------------------------------------------------------
(defun C-initend ()
  (decf *C-indent*)
  (C-Ln "};"))

;;------------------------------------------------------------------------------
;; Initialisierung einer Komponente eines Arrays des Typs CL_FORM
;;------------------------------------------------------------------------------
(defun C-init-CL_FORM (type init-form)
  (write-string "{ " *C-file*)
  (princ type *C-file*)
  (write-string ", " *C-file*)
  (cond ((eql init-form C-most-negative-fixnum)
                      ; Umgeht die Warnung "integer-overflow" des acc
         (incf init-form)
         (write-char #\( *C-file*)
         (princ init-form *C-file*)
         (write-string ") -1 }," *C-file*))
        ((integerp init-form)
         (princ init-form *C-file*)
         (write-string " }," *C-file*))
        (T
         (write-string "(long)" *C-file*)
         (princ init-form *C-file*)
         (write-string " }," *C-file*)))
  (C-Newline))

;;------------------------------------------------------------------------------
;; Initialisierung einer Komponente eines Arrays eines unstrukturierten Typs
;;------------------------------------------------------------------------------
(defun C-init (value)
  (C-Ln value #\,))

;;------------------------------------------------------------------------------
;; Generierung eines Strings, der einen C-Ausdruck beschreibt
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; C-Code zum Aufruf eines Makros
;;------------------------------------------------------------------------------
(defun CC-MacroCall (name &rest args)
  (if args
      (apply #'CC-Call name args)
      name))

;;------------------------------------------------------------------------------
;; C-Code zum Aufruf einer Funktion
;;------------------------------------------------------------------------------
(defun CC-Call (name &rest args)
  (let ((*print-circle* nil))
    (if args
        (format nil "~A(~A~{, ~A~})" name (first args) (rest args))
        (format nil "~A()" name))))

;;------------------------------------------------------------------------------
(defun CC-Cast (typ expr)
  (format nil "(~A)~A" typ expr))

;;------------------------------------------------------------------------------
(defun CC-Address (expr)
  (format nil "&~A" expr))

;;------------------------------------------------------------------------------
(defun CC-NameConc (&rest komponent)
  (apply #'CC-Code komponent))

;;------------------------------------------------------------------------------
(defun CC-make-bool (loc)
  (CC-MacroCall "CL_TRUEP" loc))

;;------------------------------------------------------------------------------
(defun CC-bool (x)
  (if x C-TRUE C-FALSE))

;;------------------------------------------------------------------------------
(defun CC-op+ (e1 e2)
  (CC-Code e1 " + " e2))

;;------------------------------------------------------------------------------
(defun CC-op- (e1 e2)
  (CC-Code e1 " - " e2))

;;------------------------------------------------------------------------------
(defun CC-op> (e1 e2)
  (CC-Code e1 " > " e2))

;;------------------------------------------------------------------------------
(defun CC-op< (e1 e2)
  (CC-Code e1 " < " e2))

;;------------------------------------------------------------------------------
(defun CC-opIor (e1 e2)
  (CC-Code e1 " | " e2))

;;------------------------------------------------------------------------------
(defun CC-op^ (e1 e2)
  (CC-Code e1 " ^ " e2))

;;------------------------------------------------------------------------------
(defun CC-op& (e1 e2)
  (CC-Code e1 " & " e2))

;;------------------------------------------------------------------------------
(defun CC-op~ (x)
  (CC-Code " ~ " x))

;;------------------------------------------------------------------------------
(defun CC-op>> (e1 e2)
  (CC-Code e1 " >> " e2))

;;------------------------------------------------------------------------------
(defun CC-op<< (e1 e2)
  (CC-Code e1 " << " e2))

;;------------------------------------------------------------------------------
(defun CC-op!= (e1 e2)
  (CC-Code e1 " != " e2))

;;------------------------------------------------------------------------------
(defun CC-op== (e1 e2)
  (CC-Code e1 " == " e2))

;;------------------------------------------------------------------------------
(defun CC-op&& (e1 e2)
  (CC-Code e1 " && " e2))

;;------------------------------------------------------------------------------
(defun CC-opIIor (e1 e2)
  (CC-Code e1 " || " e2))

;;------------------------------------------------------------------------------
(defun CC-arrayptr (array index)
  (CC-Code "&" array "[" index "]"))

;;------------------------------------------------------------------------------
(defun CC-arraykomp (array index)
  (CC-Code array "[" index "]"))

;;------------------------------------------------------------------------------
(defun CC-structkomp (struct komp)
  (CC-Code struct "." komp))

;;------------------------------------------------------------------------------
;; C-Code zum Zugriff auf eine `rest'-Variable
;;------------------------------------------------------------------------------
(defun CC-restvar (offset)
  (getCode "~A_~A" C-rest (1- (- offset))))

;;------------------------------------------------------------------------------
;; C-Code zum Zugriff auf einen Offset im aktuellen Activation Record.
;;------------------------------------------------------------------------------
(defun CC-Stack (offset)
  (if *rest-optimization*
      (if (minusp offset)
          (CC-restvar offset)
          (if (< offset *rest-optimization*)
              (CC-MacroCall "ARG" offset)
              (CC-MacroCall "LOCAL" (- offset *rest-optimization*))))
      (CC-MacroCall "ARG" offset)))

;;------------------------------------------------------------------------------
;; C-Code zum Zugriff auf einen Offset in anderen Activation Records.
;;------------------------------------------------------------------------------
(defun CC-caller-stack (offset C-caller_base)
  (CC-MacroCall "STACK" C-caller_base offset))

;;------------------------------------------------------------------------------
;; C-Code zum Zugriff auf eine Komponente des Multiple-Value Buffers
;;------------------------------------------------------------------------------
(defun CC-mv_buf (i)
  (CC-arrayptr "mv_buf" i))

;;------------------------------------------------------------------------------
;; C-Code zum Definieren von Preprozessor-Symbolen, falls sie nicht schon
;; definiert sind.
;;------------------------------------------------------------------------------
(defun C-DefSymDefault (name value)
  (C-Ln "#ifndef " name)
  (C-Ln "#define " name " " value)
  (C-Ln "#endif")
  (C-Empty-Ln))

;;------------------------------------------------------------------------------
;; C-Code zum Definieren der Groessen der Speicherbereiche
;; Stack, Form-Heap, String/Integer-Heap, Float-Heap
;;------------------------------------------------------------------------------
(defun C-DefMemSizes ()
  (let ((minstacksize     (car (first  *MEMSIZES*)))
        (maxstacksize     (cdr (first  *MEMSIZES*)))
        (minform-heapsize (car (second *MEMSIZES*)))
        (maxform-heapsize (cdr (second *MEMSIZES*)))
        (minfx-heapsize   (car (third  *MEMSIZES*)))
        (maxfx-heapsize   (cdr (third  *MEMSIZES*)))
        (minfl-heapsize   (car (fourth *MEMSIZES*)))
        (maxfl-heapsize   (cdr (fourth *MEMSIZES*))))

    (C-DefSymDefault "MINSTACKSIZE"     minstacksize)
    (C-DefSymDefault "MAXSTACKSIZE"     maxstacksize)
    (C-DefSymDefault "MINFORM_HEAPSIZE" minform-heapsize)
    (C-DefSymDefault "MAXFORM_HEAPSIZE" maxform-heapsize)
    (C-DefSymDefault "MINFX_HEAPSIZE"   minfx-heapsize)
    (C-DefSymDefault "MAXFX_HEAPSIZE"   maxfx-heapsize)
    (C-DefSymDefault "MINFL_HEAPSIZE"   minfl-heapsize)
    (C-DefSymDefault "MAXFL_HEAPSIZE"   maxfl-heapsize)
    
    (C-VarDeclInit "unsigned" "minstacksize"     "MINSTACKSIZE")
    (C-VarDeclInit "unsigned" "maxstacksize"     "MAXSTACKSIZE") 
    (C-VarDeclInit "unsigned" "minform_heapsize" "MINFORM_HEAPSIZE") 
    (C-VarDeclInit "unsigned" "maxform_heapsize" "MAXFORM_HEAPSIZE") 
    (C-VarDeclInit "unsigned" "minfx_heapsize"   "MINFX_HEAPSIZE") 
    (C-VarDeclInit "unsigned" "maxfx_heapsize"   "MAXFX_HEAPSIZE") 
    (C-VarDeclInit "unsigned" "minfl_heapsize"   "MINFL_HEAPSIZE") 
    (C-VarDeclInit "unsigned" "maxfl_heapsize"   "MAXFL_HEAPSIZE")))

;;------------------------------------------------------------------------------
(provide "cgcode")


