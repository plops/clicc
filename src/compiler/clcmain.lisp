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
;;; Funktion : Definition der Function do-clicc zum Aufruf von CLiCC
;;;            Laden der uebrigen Module
;;;
;;; $Revision: 1.58 $
;;; $Id: clcmain.lisp,v 1.58 1994/12/06 15:46:10 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

(require "strconst")
(require "clcdef")
(require "clcmisc")
(require "zsdef")
(require "ffzsdef")
(require "zsops")
(require "clcmisc")
(require "config")
(require "appfuns")
(require "p1env")
(require "p0init")
(require "p1main")
(require "traverse")
(require "inline")
(require "delete")
(require "static-effect")
(require "p3main")
(require "timain")
(require "titypes")
(require "optimain")
(require "cgmain")
(require "deffile")


;;------------------------------------------------------------------------------
;; Der Aufruf des Compilers.
;; Resultat: T, wenn Fehler auftraten, NIL sonst.
;;------------------------------------------------------------------------------
(defun do-clicc ()
  
  ;; Damit die Zahlen klein bleiben.
  ;;--------------------------------
  (gensym 0)                            ; für CltL1
  (setq *gensym-counter* 1)             ; für CLtL2

  (setq *FILENAME* (merge-pathnames *filename*))
  (setq *OUT-FILENAME*
        (if *OUT-FILENAME*
            (merge-pathnames *OUT-FILENAME*)
            *FILENAME*))

  (when *SHOW-VERSION*
    (format t "~&~
;;; CLiCC, the Common Lisp to C Compiler --- Version ~a~%~
;;; Copyright (C) ~a Christian-Albrechts-Universitaet zu Kiel~%~%"
            *CLICC-VERSION* (CLiCC-Copyright-Year)))

  (setq *NERRORS* 0 *NWARNINGS* 0)
  (p0-init)
    
  (when (= *NERRORS* 0)
    (clicc-message "Syntactic and semantic analysis")
    (pass_1))

  (unless *ONLY-PASS1*

    ;; Das Symbol T hat zunaechst den Typ SYMBOL. Wenn es von irgendwelchen
    ;; Optimierungen ins Programm eingebaut wird, sollte es den genaueren Typ
    ;; T-SYMBOL haben, um gleich weitere Optimierungen zu ermoeglichen.
    ;;-----------------------------------------------------------------
    (let ((t-sym (get-symbol-bind 'L::T)))
      (when t-sym (setf (?type t-sym) T-SYMBOL-T)))

    ;; Die Funktion L::error wird gesondert behandelt; es wird angenommen, da"s
    ;; sie keine Seiteneffekte verursacht.
    ;;------------------------------------
    (setq *error-function* (get-global-fun 'L::error))

    (if *OPTIMIZE*
        (progn
          (when (= *NERRORS* 0)
            (search-and-delete-unused-objects))

          (when (and (= *NERRORS* 0)
                     (= *ITERATIONS* 1)
                     (not *no-inlining*))
            (simplify-params #'used-dynamic-vars-simple))
          
          ;; Aufruf einer sehr einfachen Typinferenz
          (when (= *NERRORS* 0)
            (let ((*ti-level* 0)
                  (*ti-verbosity* (min *ti-verbosity* 1)))
              (do-ti)))
          
          (when (= *NERRORS* 0)
            (pass_3))                   ; level-, mv-Slots etc. setzen.

          (when (= *NERRORS* 0)
            (unless *no-tail-recursion*
              (tail-rec-module)))

          (when (= *NERRORS* 0)
            (let ((*no-seo* t)

                  ;; Wenn so fr"uh bereits wesentliches optimiert werden
                  ;; kann, dann ist es verd"achtig
                  ;;------------------------------
                  (*optimize-verbosity* (max *optimize-verbosity* 2)))
              (do-optimization)))

          (when (= *NERRORS* 0)
            (search-and-delete-unused-objects))

          ;; Nach dem Inlining ist man an gel"oschten Funktionen nicht mehr so
          ;; sehr interessiert.
          (let ((*delete-verbosity* (1- *delete-verbosity*)))

            (dotimes (iteration *ITERATIONS*)
              (when (> *ITERATIONS* 1)
                (clicc-message-line 28)
                (clicc-message "~D. iteration step" (1+ iteration))
                (clicc-message-line 28)
                (clicc-message " "))
        
              (when (= *NERRORS* 0)
                (do-inline))
        
              (when (= *NERRORS* 0)
                (search-and-delete-unused-objects))
        
              (when (= *NERRORS* 0)
                (search-fun-calls))
        
              (when (= *NERRORS* 0)
                (pass_3))
            
              ;; Aufruf der Seiteneffektanalyse
              (unless *NO-SIDE-EFFECT-ANALYSIS*
                (when (= *NERRORS* 0)
                  (analyse-module)))
        
              ;; Aufruf der Typinferenz
              (when (= *NERRORS* 0)
                (reset-type-annotations)
                (do-ti))
            
              (when (= *NERRORS* 0)
                (do-optimization))
            
              (when (= *NERRORS* 0)
                (search-and-delete-unused-objects))

              (when (and (= *NERRORS* 0)
                         (> *ITERATIONS* 1)
                         (= iteration 0)
                         (not *no-inlining*))
                (simplify-params #'used-dynamic-vars-with-side-effects)
                (pass_3))))
        
          ;; Suche nach Typfehlern
          (when (= *NERRORS* 0)
            (look-for-type-errors *module*))

          (when (and *module-compiler*
                     (not *inline-module*)
                     (not *no-inlining*)
                     (= 0 *NERRORS*))
            (clicc-message "Preparing Module Interface Specification")
            (prepare-export-write)))

        ;; Wenn nicht optimiert wird, muessen zumindest die named-consts,
        ;; die als Referenzen auf Funktionen gedient haben, ersetzt werden.
        ;; Ausserdem muessen noch die angewandten Vorkommen gezaehlt werden.
        (let ((*no-to* t)
              (*no-seo* t)
              (*no-simp* t))
          (set-used-slots-for-cg)
          (pass_3)
          (do-optimization)))
    
    (when (= *NERRORS* 0)
      (clicc-message "Annotation for code generation")
      (pass_3))

    (unless *NO-CODEGEN*
      (when (= *NERRORS* 0)
        (clicc-message "Code Generation")
        (codegen)))

    (when (and *module-compiler* (not *inline-module*))
      (when (= *NERRORS* 0)
        (clicc-message "Writing Module Interface Specification")
        (export-write))))

  (when (or (> *NERRORS* 0) (> *NWARNINGS* 0))
    (format t ";;; ~D Errors, ~D Warnings~%" *NERRORS*  *NWARNINGS*))
  (terpri)

  (> *NERRORS* 0))

;;------------------------------------------------------------------------------
(provide "clcmain")
