;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Codegenerierung
;;;            Erzeugen von Inline-Code fuer ausgewaehlte Systempraedikate
;;;
;;; $Revision: 1.26 $
;;; $Log: cginline.lisp,v $
;;; Revision 1.26  1993/11/01  15:25:46  hk
;;; Fehler in get-arg-loc (var-ref) behoben. Im Zusammenspiel mit opt-args
;;; wurden in gewissen Fällen lokale Variablen überschrieben, obwohl sie
;;; später noch benutzt wurden.
;;;
;;; Revision 1.25  1993/10/26  14:58:14  sma
;;; EQ Makro wird benutzt; CAR,CDR Makros eingeführt; mehr Typtest-Makros.
;;;
;;; Revision 1.24  1993/09/28  14:45:55  pm
;;; Die C-Konvertierungsfunktionen werden jetzt inline compileiert
;;;
;;; Revision 1.23  1993/09/10  15:08:28  hk
;;; get-arg-loc spezialisiert über form statt t.
;;;
;;; Revision 1.22  1993/09/10  11:49:54  hk
;;; Fehler in cg-eq behoben, wenn beide Argumente einfache Konstanten
;;; waren.
;;;
;;; Revision 1.21  1993/08/18  15:25:32  ft
;;; Die Funktionen instancep, instance-set, instance-ref und
;;; set-slot-unbound werden jetzt inline compiliert.
;;;
;;; Revision 1.20  1993/07/22  13:00:54  hk
;;; In cg-%car und cg-%cdr GET_FORM durch GET_CAR ersetzt.
;;;
;;; Revision 1.19  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.18  1993/04/06  17:28:30  hk
;;; ?codegen -> ?c-inline.
;;;
;;; Revision 1.17  1993/04/06  17:09:29  hk
;;; shift-right, shift-left -> %shift-right, %shift-left.
;;;
;;; Revision 1.16  1993/02/16  15:50:26  hk
;;; Revision Keyword eingefuegt, Symbole im zu uebersetzenden Programm
;;; werden im clicc-lisp Package angesprochen.
;;;
;;; Revision 1.15  1993/01/13  15:06:31  ft
;;; Erweiterung um Funktionen fuer ash.
;;;
;;; Revision 1.14  1993/01/07  10:00:20  hk
;;; Fehler mit special-sys-fun behoben.
;;;
;;; Revision 1.13  1993/01/07  08:31:04  hk
;;; Fehler in macrolet von cg-special-funs behoben.
;;;
;;; Revision 1.12  1993/01/06  13:03:40  hk
;;; Funktionen {p1,p2,p3,cg}-special-funs vereinheitlicht.
;;;
;;; Revision 1.11  1993/01/06  11:18:53  ft
;;; Erweiterung um logische Operationen auf Zahlen.
;;;
;;; Revision 1.10  1992/12/03  14:53:26  hk
;;; typecase -> etypecase
;;;
;;; Revision 1.9  1992/11/26  16:46:17  hk
;;; Neu cg-%vector-length.
;;;
;;; Revision 1.8  1992/11/26  16:00:13  hk
;;; cg-init von cgmain.lisp -> hier, get-arg geaendert, etc.
;;;
;;; Revision 1.7  1992/11/25  17:51:27  hk
;;; Inline Compilation von %car, %cdr, %rplaca, %rplacd, %cons und
;;; einige Umbenennungen: check-integer-low -> fixnum-low-p ...
;;;
;;; Revision 1.6  1992/10/08  14:04:23  hk
;;; cg-eq: Optimierung nach p2, null-form beachtet, cg-get-arg korrigiert.
;;;
;;; Revision 1.5  1992/09/25  17:24:44  kl
;;; C-eq und C-eql auf die neue Repraesentation der einfachen Literale
;;; umgestellt.
;;;
;;; Revision 1.4  1992/09/21  11:18:52  hk
;;; Die eigentliche C-Codegenerierung uebersichtlicher gestaltet
;;;
;;; Revision 1.3  1992/07/30  10:27:07  hk
;;; .
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
;; Bekanntgeben der Funktionen, die inline kompiliert werden.
;;------------------------------------------------------------------------------
(p0-special-funs
 (?c-inline "CG")
 clicc-lisp::eq
 clicc-lisp::eql
 clicc-lisp::not
 clicc-lisp::integerp
 rt::fixnum-low-p
 rt::fixnum-high-p
 clicc-lisp::consp
 clicc-lisp::characterp
 clicc-lisp::simple-string-p
 clicc-lisp::floatp
 clicc-lisp::atom
 clicc-lisp::symbolp
 clicc-lisp::listp
 clicc-lisp::stringp
 clicc-lisp::numberp
 clicc-lisp::functionp
 clicc-lisp::vectorp
 clicc-lisp::simple-vector-p
 clicc-lisp::arrayp
 rt::simple-array-p
 clicc-lisp::values
 clicc-lisp::cons
 rt::%car
 rt::%cdr
 rt::%rplaca
 rt::%rplacd
 rt::%vector-length
 rt::%logior
 rt::%logxor
 rt::%logand
 rt::%lognot
 rt::%shift-right
 rt::%shift-left

 rt::instancep
 rt::instance-ref
 rt::instance-set
 rt::set-slot-unbound

 rt::c-struct-p
 rt::c-char-p
 rt::c-short-p
 rt::c-int-p
 rt::c-long-p
 rt::c-unsigned-char-p
 rt::c-unsigned-short-p
 rt::c-unsigned-int-p
 rt::c-unsigned-long-p
)

;;------------------------------------------------------------------------------
;; Hilfsfunktion, die Code f"ur den Zugriff auf das Resultat eines Ausdrucks
;; generiert. Bei Variablenzugriffen wird ein Kopieren des Werts vermieden.
;;------------------------------------------------------------------------------
(defun CC-get-arg (form)
  (CC-dest (get-arg-loc form)))

;;------------------------------------------------------------------------------
(defmethod get-arg-loc ((form var-ref))
  (etypecase (?var form)
    (dynamic (?var form))
    (static
     (let ((var (?var form)))

       ;; Der Stackframe des Aufrufers wird evtl. gerade
       ;; wiederverwendet. Sicherstellen, daß bei nachfolgenden Aufrufen von
       ;; get-arg-loc diese Variable nicht überschrieben wird.
       ;;-----------------------------------------------------
       (when (eql *level* (?level var))
         (setq *stack-top* (max *stack-top* (1+ (?offset var)))))
       var))))

(defmethod get-arg-loc ((form form))
  (let ((*result-spec* (stacktop-result-location)))
    (cg-form form)
    (incf *stack-top*)
    *result-spec*))

;;------------------------------------------------------------------------------
;; Hilfsfunktion, die bei Bedarf Boolesche C-Werte in die LISP-Werte
;; T / NIL umwandelt.
;;------------------------------------------------------------------------------
(defun pred-result (pred)
  (case *result-spec*
    ((NIL))
    (C-bool (setq *C-bool* pred))
    (T (C-if pred)
       (C-blockstart)
       (C-t (CC-dest *result-spec*))
       (C-blockend)
       (C-else)
       (C-blockstart)
       (C-nil (CC-dest *result-spec*))
       (C-blockend))))

;;------------------------------------------------------------------------------
;; Hilfsfunktion, die das Resultat einer inline kompilierten Funktion an die
;; in *result-spec* angegebene Position kopiert.
;;------------------------------------------------------------------------------
(defun C-result (source)
  (case *result-spec*
    
    ;; Resultat wird nicht benoetigt
    ;;------------------------------
    ((NIL))
    
    ;; Boolesches Resultat gewuenscht
    ;;-------------------------------
    (C-BOOL (setq *C-bool* (CC-make-bool source)))

    ;; Normales Resultat erzeugen
    ;;---------------------------
    (T (C-copy source (CC-dest *result-spec*)))))

;;------------------------------------------------------------------------------
;; EQ, inline
;; ACHTUNG: es wird davon ausgegangen, dass Zeiger die groesste Komponente
;; in CL_FORM sind.
;; Optimiert, wenn einer der Parameter konstant ist.
;;------------------------------------------------------------------------------
(defun cg-eq (form1 form2)
  (pred-result
   (let ((*stack-top* *stack-top*)
         (const-arg 0))

     ;; pruefen, welche der Argumente konstant sind
     ;;--------------------------------------------
     (if (or (simple-literal-p form1) (sym-p form1))
         (incf const-arg 1)
         (setq form1 (CC-get-arg form1)))
     (if (or (simple-literal-p form2) (sym-p form2))
         (incf const-arg 2)
         (setq form2 (CC-get-arg form2)))

     (case const-arg

       ;; eines der Argumente ist konstant
       ;;---------------------------------
       ((1 2) (when (= const-arg 2)
                (rotatef form1 form2))
        (typecase form1
          (null-form (getCode "CL_NILP(~A)" form2))
          (int
           (getCode "CL_FIXNUMP(~A) && GET_FIXNUM(~A) == ~A"
                    form2 form2 (?value form1)))
          (character-form
           (getCode "CL_CHARP(~A) && GET_CHAR(~A) == ~A"
                    form2 form2 (CC-character (?value form1))))
          (sym (getCode "CL_SYMBOLP(~A) && GET_SYMBOL(~A) == ~A"
                        form2 form2 (CC-symbol form1)))
          (T "FALSE")))
	      
       ;; beide Argumente sind konstant
       ;;------------------------------
       (3 (CC-bool (cond
                     ((and (simple-literal-p form1) (simple-literal-p form2))
                      (eq (?value form1) (?value form2)))
                     ((and (sym-p form1) (sym-p form2))
                      (eq form1 form2))
                     (T nil))))

       (t (getCode "EQ(~A, ~A)" form1 form2))))))

;;------------------------------------------------------------------------------
;; EQL, inline
;; Optimierung:
;; Wenn eines der Argument vom Typ FLOAT ist, dann direkt vergleichen.
;;------------------------------------------------------------------------------
(defun cg-eql (form1 form2)
  (pred-result
   (let ((*stack-top* *stack-top*) 
         (floatconst 0))
     
     ;; Pruefen, ob FLOAT Konstanten vorliegen
     ;;---------------------------------------
     (if (float-form-p form1)
         (incf floatconst 1)
         (setq form1 (CC-get-arg form1)))
     (if (float-form-p form2)
         (incf floatconst 2)
         (setq form2 (CC-get-arg form2)))

     (case floatconst

       ;; 'Normales' EQL
       ;;---------------
       (0 (getCode 
           "EQ(~A, ~A) || ~
                CL_FLOATP(~A) && GET_FLOAT(~A) == GET_FLOAT(~A)"
           form1 form2 form1 form1 form2))
     
       ;; beide Argumente sind vom Typ FLOAT
       ;;-----------------------------------
       (3 (CC-bool (= (?value form1) (?value form2))))
     
       ;; eines der Argumente ist vom Typ FLOAT
       ;;--------------------------------------
       (T (when (= floatconst 2)
            (rotatef form1 form2))
          (getCode "CL_FLOATP(~A) && GET_FLOAT(~A) == ~A"
                   form2 form2 (?value form1)))))))

;;------------------------------------------------------------------------------
(defun cg-not (form)
  (pred-result
   (let ((*result-spec* (when *result-spec* 'C-bool)))
     (cg-form form)
     (CC-MacroCall "NOT" *C-bool*))))

;;------------------------------------------------------------------------------
(defun cg-integerp (x-loc)
  (pred-result (getCode "CL_FIXNUMP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-fixnum-low-p (val low)
  (setq val (CC-get-arg val))
  (setq low (CC-get-arg low))
  (pred-result
   (getCode "CL_FIXNUMP(~A) && GET_FIXNUM(~A) >= GET_FIXNUM(~A)" val val low)))

;;------------------------------------------------------------------------------
(defun cg-fixnum-high-p (val high)
  (setq val (CC-get-arg val))
  (setq high(CC-get-arg high))
  (pred-result
   (getCode "CL_FIXNUMP(~A) && GET_FIXNUM(~A) <= GET_FIXNUM(~A)" val val high)))


;;------------------------------------------------------------------------------
(defun cg-consp (x-loc)
  (pred-result
   (getCode "CL_CONSP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-characterp (x-loc)
  (pred-result
   (getCode "CL_CHARP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-simple-string-p (x-loc) 
  (pred-result
   (getCode "CL_SMSTRP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-floatp (x-loc)
  (pred-result
   (getCode "CL_FLOATP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-atom (x-loc)
  (pred-result
   (getCode "CL_ATOMP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-symbolp (x-loc)
  (setq x-loc (CC-get-arg x-loc))
  (pred-result
   (getCode "CL_SYMBOLP(~A) || CL_NILP(~A)" x-loc x-loc)))

;;------------------------------------------------------------------------------
(defun cg-listp (x-loc)
  (pred-result
   (getCode "CL_LISTP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-stringp (x-loc)
  (pred-result
   (getCode "CL_STRING_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-numberp (x-loc)
  (pred-result
   (getCode "CL_NUMBERP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-functionp (x-loc)
  (pred-result
   (getCode "CL_FUNCTION_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-vectorp (x-loc)
  (pred-result
   (getCode "CL_VECTOR_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-simple-vector-p (x-loc)
  (pred-result
   (getCode "CL_SMVEC_T_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-arrayp (x-loc)
  (pred-result
   (getCode "CL_ARRAY_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-simple-array-p (x-loc)
  (pred-result
   (getCode "CL_SMAR_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
(defun cg-instancep (x-loc)
  (pred-result
   (getCode "CL_INSTANCEP(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
;; FFI
;;------------------------------------------------------------------------------
(defun cg-c-struct-p (x-loc)
  (pred-result
   (getCode "CL_C_STRUCT_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-char-p (x-loc)
  (pred-result
   (getCode "CL_C_CHAR_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-short-p (x-loc)
  (pred-result
   (getCode "CL_C_SHORT_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-int-p (x-loc)
  (pred-result
   (getCode "CL_C_INT_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-long-p (x-loc)
  (pred-result
   (getCode "CL_C_LONG_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-unsigned-char-p (x-loc)
  (pred-result
   (getCode "CL_C_UNSIGNED_CHAR_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-unsigned-short-p (x-loc)
  (pred-result
   (getCode "CL_C_UNSIGNED_SHORT_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-unsigned-int-p (x-loc)
  (pred-result
   (getCode "CL_C_UNSIGNED_INT_P(~A)" (CC-get-arg x-loc))))

(defun cg-c-unsigned-long-p (x-loc)
  (pred-result
   (getCode "CL_C_UNSIGNED_LONG_P(~A)" (CC-get-arg x-loc))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun cg-%car (cell)
  (setq cell (CC-get-arg cell))
  (C-result (CC-MacroCall "GET_CAR" cell)))

;;------------------------------------------------------------------------------
(defun cg-%cdr (cell)
  (setq cell (CC-get-arg cell))
  (C-result (CC-MacroCall "GET_CDR" cell)))

;;------------------------------------------------------------------------------
(defun cg-cons (x y)
  (setq x (CC-get-arg x))
  (setq y (CC-get-arg y))
  (case *result-spec*
    ((NIL))
    (C-BOOL (setq *C-bool* C-TRUE))
    (T (let ((var "lptr"))
         (C-Blockstart)
         (C-PtrDecl "CL_FORM" var)
         (C-assign var (CC-Call "form_alloc" (CC-StackTop) 2))
         (C-copy x (CC-MacroCall "CAR" var))
         (C-copy y (CC-MacroCall "CDR" var))
         (C-MacroCall "LOAD_CONS" var (CC-dest *result-spec*))
         (C-Blockend)))))

;;------------------------------------------------------------------------------
(defun cg-%rplaca (x y)
  (setq x (get-arg-loc x))
  (setq y (get-arg-loc y))
  (C-copy (CC-dest y) (CC-MacroCall "CAR" (CC-MacroCall "GET_FORM" 
                                                        (CC-dest x))))
  (to-result-loc x))

;;------------------------------------------------------------------------------
(defun cg-%rplacd (x y)
  (setq x (get-arg-loc x))
  (setq y (get-arg-loc y))
  (C-copy (CC-dest y) (CC-MacroCall "CDR" (CC-MacroCall "GET_FORM" 
                                                        (CC-dest x))))
  (to-result-loc x))

;;------------------------------------------------------------------------------
(defun cg-%vector-length (x)
  (setq x (CC-get-arg x))
  (C-MacroCall "LOAD_FIXNUM"
               (CC-MacroCall "AR_SIZE" (CC-MacroCall "GET_FORM" x))
               (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%logior (x y)
  (setq x (CC-get-arg x))
  (setq y (CC-get-arg y))
  (C-MacroCall "LOAD_FIXNUM"
               (CC-opIor
                (CC-MacroCall "GET_FIXNUM" x)
                (CC-MacroCall "GET_FIXNUM" y))
               (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%logxor (x y)
  (setq x (CC-get-arg x))
  (setq y (CC-get-arg y))
  (C-MacroCall "LOAD_FIXNUM"
               (CC-op^
                (CC-MacroCall "GET_FIXNUM" x)
                (CC-MacroCall "GET_FIXNUM" y))
               (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%logand (x y)
  (setq x (CC-get-arg x))
  (setq y (CC-get-arg y))
  (C-MacroCall "LOAD_FIXNUM"
               (CC-op&
                (CC-MacroCall "GET_FIXNUM" x)
                (CC-MacroCall "GET_FIXNUM" y))
               (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%lognot (x)
  (setq x (CC-get-arg x))
  (C-MacroCall "LOAD_FIXNUM"
               (CC-op~
                (CC-MacroCall "GET_FIXNUM" x))
               (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%shift-right (i c)
  (setq i (CC-get-arg i))
  (setq c (CC-get-arg c))
  (C-MacroCall "LOAD_FIXNUM"
               (CC-op>>
                (CC-MacroCall "GET_FIXNUM" i)
                (CC-MacroCall "GET_FIXNUM" c))
               (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-%shift-left (i c)
  (setq i (CC-get-arg i))
  (setq c (CC-get-arg c))
  (C-MacroCall "LOAD_FIXNUM"
               (CC-op<<
                (CC-MacroCall "GET_FIXNUM" i)
                (CC-MacroCall "GET_FIXNUM" c))
               (CC-dest *result-spec*)))

;;------------------------------------------------------------------------------
(defun cg-instance-ref (instance offset)
  (setq instance (CC-get-arg instance))
  (setq offset (CC-get-arg offset))
  (C-result (CC-MacroCall "OFFSET"
                          (CC-MacroCall "AR_BASE"
                                        (CC-MacroCall "GET_FORM" instance))
                          (CC-op+ 1 (CC-MacroCall "GET_FIXNUM" offset)))))

;;------------------------------------------------------------------------------
(defun cg-instance-set (new-value instance offset)
  (setq new-value (CC-get-arg new-value))
  (setq instance (CC-get-arg instance))
  (setq offset (CC-get-arg offset))
  (C-copy new-value
          (CC-MacroCall "OFFSET"
                        (CC-MacroCall "AR_BASE"
                                      (CC-MacroCall "GET_FORM" instance))
                        (CC-op+ 1 (CC-MacroCall "GET_FIXNUM" offset)))))

;;------------------------------------------------------------------------------
(defun cg-set-slot-unbound (instance offset)
  (setq instance (CC-get-arg instance))
  (setq offset (CC-get-arg offset))
  (C-MacroCall "LOAD_UNBOUND"
               (CC-MacroCall "OFFSET"
                             (CC-MacroCall "AR_BASE"
                                           (CC-MacroCall "GET_FORM" instance))
                             (CC-op+ 1 (CC-MacroCall "GET_FIXNUM" offset)))))


;;------------------------------------------------------------------------------
(provide "cginline")
