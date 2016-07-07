;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Foreign Functions - codegeneration
;;;
;;; $Revision: 1.16 $
;;; $Log: cgforeign.lisp,v $
;;; Revision 1.16  1993/11/03  11:44:22  pm
;;; Inkonsistenzen in den Symbolnamen behoben.
;;;
;;; Revision 1.15  1993/08/24  11:18:54  pm
;;; Erweiterungen um C-Pointer
;;;
;;; Revision 1.14  1993/07/26  16:49:36  pm
;;; Erweiterungen um C_Strukturen.
;;;
;;; Revision 1.13  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.12  1993/06/04  13:44:10  pm
;;; cg-call-in-interface eingebaut
;;;
;;; Revision 1.11  1993/05/31  17:01:13  pm
;;; Schreibfehler beseitigt
;;;
;;; Revision 1.10  1993/05/23  17:46:06  pm
;;; Codegenerierung fuer die C-Funktionen basierend auf den
;;; primitven C-Typen implementiert
;;;
;;; Revision 1.9  1993/05/21  13:55:49  pm
;;; c-int in int umbenannt
;;;
;;; Revision 1.8  1993/05/12  14:11:29  pm
;;; packages verstanden und korrigiert.
;;;
;;; Revision 1.7  1993/03/10  12:46:56  pm
;;; Kleinigkeiten geaendert
;;;
;;; Revision 1.6  1993/02/16  16:07:31  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.5  1993/01/12  14:09:45  pm
;;; Fehler behoben
;;;
;;; Revision 1.4  1992/12/01  15:11:33  pm
;;; c-char* eingebaut
;;;
;;; Revision 1.3  1992/11/25  12:36:55  pm
;;; Codegenerator fuer das Foreign Function Interface
;;;
;;; Revision 1.2  1992/11/05  12:55:19  pm
;;; initial revision
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defconstant NOT-IMPLEMENTED-YET 
  "Not implemented yet: ~A")

;;------------------------------------------------------------------------------
(defmethod cg-app ((fun foreign-fun) args app)
 ;; Bei der Ueberfuehrung in die Zwischensprache wurden schon die Typ- und
 ;; Parameteranzahl- Ueberpruefungen eingebaut, so dass davon ausgegangen
 ;; werden kann, dass eine Applikation an dieser Stelle korrekt ist.  Deshalb
 ;; muessen die Argumente nur noch evaluiert, auf den Stack geschoben und
 ;; konvertiert werden. Danach kann der Aufruf generiert werden. Dieser
 ;; besteht darin, die Argumente vom Stack an die Parameterposition zu
 ;; konvertieren, die Funktion aufzurufen und das Ergebnis wieder auf dem
 ;; Stack abzulegen.
  (declare (ignore app))

  (let* (nargs
         (old-stack *stack-top*))

    (setq nargs (cg-args args 0)) ;; Hier die varargs einbauen!!

    ;; Aktuellen stack retten fuer eeventuelle Call-Ins
    ;;-------------------------------------------------
    (C-Ln "save_stack = base;")

    (C-ForeignCall fun old-stack (length args))
    (stacktop-to-result-loc)
    
    (setq *stack-top* old-stack)
))

;;------------------------------------------------------------------------------
;; erzeugt:
;;   LOAD_C_... (... , <base,+0>);
;;------------------------------------------------------------------------------
(defun C-ForeignCall (fun base arglist-length)
  (let* ((return-type (?return-type fun)))
    (cond (;; primitive Typen
           ;;----------------
           (atom return-type)
           (C-MacroCall (concatenate 
                         'string "LOAD_" (c-macro-string return-type))
                        (CC-ForeignCall fun base arglist-length)
                        (CC-Stack base)))
          
          ;; Pointer
          ;;--------
          ((eq (first return-type) 'ffi:c-ptr)
           (let* ((ptr-typ (second return-type)))
             (cond (;; Zeiger auf primitiven Typ
                    ;;--------------------------
                    (atom ptr-typ)
                    (C-MacroCall (concatenate 
                                  'string "LOAD_"
                                  (c-macro-string ptr-typ) "_PTR")
                                 (CC-ForeignCall fun base arglist-length)
                                 (CC-Stack base)))
                   
                   ;; Zeiger auf benannte Struktur
                   ;;-----------------------------
                   ((eq (first ptr-typ) :struct)
                    (C-MacroCall 
                     "LOAD_C_STRUCT_PTR"
                     (CC-Cast "char *"
                              (CC-ForeignCall fun base arglist-length))
                     (CC-Stack base)))
                   
                   ;; Zeiger auf Array, Pointer, ...
                   ;;-------------------------------
                   ;; *** to do ***
                   
                   ;; Alle anderen Faelle fuehren zu einem Fehler
                   ;;--------------------------------------------
                   (t
                    (clicc-error "illegal pointer-type: ~S" return-type)))))

          
          ;; Strukturen
          ;;-----------
          ((eq (first return-type) :struct)
           ;; *** to do ***
           )
          
          ;; Arrays, ...
          ;;------------
          ;; *** to do ***
          
          ;; Alles andere fuehrt zu einem Fehler
          ;;------------------------------------
          (t 
           (internal-error 'C-ForeignCall
                           NOT-IMPLEMENTED-YET return-type)))))
  
  
;;------------------------------------------------------------------------------
;; Soll erzeugen: ;; (typ)<name>((typ)GET_...(<base,+0>), ...)
;;------------------------------------------------------------------------------
(defun CC-ForeignCall (fun base arglist-length)
  (let (liste
        (arglist (?arguments fun)))

    (dotimes (count arglist-length)     ; noch ohne varargs!  
      (let ((nth-arg (nth count arglist)))
        (setq liste 
              `(,@liste
                ,(cond (;; primitiver Typ
                        ;;---------------
                        (atom nth-arg)
                        (CC-MacroCall 
                         (concatenate 'string "GET_" (c-macro-string nth-arg))
                         (CC-Stack (+ count base))))
                       
                       ;; Pointer
                       ;;--------
                       ((eq (first nth-arg) 'ffi:c-ptr)
                        (let* ((ptr-typ (second nth-arg)))
                          (cond (;; Zeiger auf primitiven Typ
                                 ;;--------------------------
                                 (atom ptr-typ)
                                 (CC-MacroCall
                                  (concatenate 'string "GET_" 
                                               (c-macro-string ptr-typ) "_PTR")
                                  (CC-Stack (+ count base))))
                                
                                ;; Zeiger auf benannte Struktur
                                ;;-----------------------------
                                ((eq (first ptr-typ) :struct)
                                 (let* ((struktur (?symbol (second ptr-typ))))
                                   (CC-Cast 
                                    (CC-Code (c-name-string struktur) " *")
                                    (CC-MacroCall "GET_C_STRUCT_PTR_VALUE"
                                                  (CC-Stack (+ count base))))))

                                ;; Zeiger auf Array, Pointer, ...
                                ;;-------------------------------
                                ;; *** to do ***
                                
                                ;; Alle anderen Faelle fuehren zu einem Fehler
                                ;;--------------------------------------------
                                (t
                                 (clicc-error "illegal pointer-type: ~S" 
                                              ptr-typ)))))
                       
                       ;; Strukturen
                       ;;-----------
                       ((eq (first nth-arg) :struct)
                        ;; *** to do ***
                        )
                       
                       ;; Arrays, ...
                       ;;------------
                       ;; *** to do ***
                       
                       ;; Alles andere fuehrt zu einem Fehler
                       ;;------------------------------------
                       (t
                        (internal-error
                         'C-ForeignCall
                         "illegal pointer-type: ~S" nth-arg)))))))

                
    (apply #'CC-Call (?name fun) liste)))

;;------------------------------------------------------------------------------
;; furchtbarer HACK
;;------------------------------------------------------------------------------
(defun cg-call-in-interface (fun)
  (let* ((fun-name (?foreign-name (?call-in fun)))
         (gen-fun-name (?adr fun)))
    (C-Empty-Ln)
    (cond 
      (*ANSI-C*
       (C-Ln "int " fun-name "(int arg1)"))
      (T
       (C-Ln "int " fun-name "(arg1) int arg1;")))
    (C-blockstart)

    (C-Ln "CL_FORM *base = save_stack;")

    (C-MacroCall "LOAD_C_INT" 
                 "arg1"
                 (CC-MacroCall "STACK" "base" 0))
    (C-Call gen-fun-name
            (CC-MacroCall "STACK" "base" 0))
    (C-Call "return"
            (CC-MacroCall "GET_C_INT"
                          (CC-MacroCall "STACK" "base" 0)))
    (C-blockend)))

;;------------------------------------------------------------------------------
(provide "cgforeign")


