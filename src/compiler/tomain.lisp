;;;-----------------------------------------------------------------------------
;;; Copyright (C) 1993 Christian-Albrechts-Universitaet zu Kiel, Germany
;;;-----------------------------------------------------------------------------
;;; Projekt  : APPLY - A Practicable And Portable Lisp Implementation
;;;            ------------------------------------------------------
;;; Funktion : Funktionen zur Optimierungen, die auf den Typinformationen
;;;            aufbauen.
;;;
;;; $Revision: 1.27 $
;;; $Log: tomain.lisp,v $
;;; Revision 1.27  1993/10/28  07:29:11  kl
;;; Verwendung von has-no-side-effect gestrichen. Soll in seo... erledigt werden.
;;;
;;; Revision 1.26  1993/09/21  15:04:36  jh
;;; dec-used-slot eingebaut.
;;;
;;; Revision 1.25  1993/09/16  17:39:29  jh
;;; Schreibfehler behoben.
;;;
;;; Revision 1.24  1993/08/27  15:51:40  hk
;;; Kleinen Fehler in (to-1form app) behoben.
;;;
;;; Revision 1.23  1993/08/26  14:48:30  jh
;;; Statistik erweitert.
;;;
;;; Revision 1.22  1993/07/23  09:40:08  hk
;;; Neues Macro eliminating-msg fuer Meldung, dass Ausdrucke eliminiert
;;; werden. Benutzt *optimize-verbosity*.
;;;
;;; Revision 1.21  1993/06/30  13:46:41  jh
;;; An optimain.lisp angepasst.
;;;
;;; Revision 1.20  1993/06/17  08:00:09  hk
;;; Copright Notiz eingefuegt
;;;
;;; Revision 1.19  1993/06/10  09:54:18  kl
;;; Bindung der Variable *current-fun* korrigiert.
;;;
;;; Revision 1.18  1993/06/08  13:13:37  jh
;;; Variable *no-to* zum Ausschalten der Optimierungen eingebaut.
;;;
;;; Revision 1.17  1993/05/27  12:49:40  jh
;;; Optimierung fuer bottom-getypte Form einer Applikation eingebaut.
;;;
;;; Revision 1.16  1993/05/25  15:12:52  jh
;;; bottom-Optimierung fuer mv-lambda eingebaut.
;;;
;;; Revision 1.15  1993/05/19  14:40:12  jh
;;; Einige bottom-Optimierungen eingebaut.
;;;
;;; Revision 1.14  1993/05/13  13:19:26  jh
;;; Durch Optimierung entstandene forms werden jetzt ebenfalls optimiert.
;;;
;;; Revision 1.13  1993/05/13  10:15:23  jh
;;; In switch-forms wird jetzt auch otherwise optimiert.
;;;
;;; Revision 1.12  1993/05/12  08:47:43  kl
;;; Schreibfehler korrigiert.
;;;
;;; Revision 1.11  1993/05/09  16:26:42  kl
;;; Ausgabe erweitert.
;;;
;;; Revision 1.10  1993/05/09  13:05:40  kl
;;; Fehler in to-1form (if-form) behoben.
;;;
;;; Revision 1.9  1993/05/07  10:06:26  hk
;;; Schreibfehler in (to-parts switch-form) behoben.
;;;
;;; Revision 1.8  1993/05/06  13:31:06  kl
;;; Verwendung von has-no-side-effect eingebaut und Ausgabe erweitert.
;;;
;;; Revision 1.7  1993/05/05  11:04:22  jh
;;; Spezialbehandlung fuer bottom-t bei if-Optimierung eingebaut.
;;;
;;; Revision 1.6  1993/04/30  14:32:02  hk
;;; do-to deaktiviert, da Fehler in Typinferenz oder Typoptimierungen.
;;;
;;; Revision 1.5  1993/04/30  13:26:32  hk
;;; Hier wurde bisher JEDES (mv-lambda args body) zu args optimiert.
;;;
;;; Revision 1.4  1993/04/21  11:35:30  jh
;;; mv-lambda eingebaut.
;;;
;;; Revision 1.3  1993/02/16  16:09:13  hk
;;; Revision Keyword eingefuegt.
;;;
;;; Revision 1.2  1993/01/07  12:59:31  jh
;;; Statistik fuer die if-Optimierungen eingebaut.
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; to-1form optimiert einen Zwischensprachausdruck, ohne seine Teilausdruecke
;; zu optimieren.
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Ausgabe der Meldung, dass Ausdrucke eliminiert werden
;;------------------------------------------------------------------------------
(defmacro eliminating-msg (&rest args)
  `(when (> *optimize-verbosity* 1)
    (clicc-message ,@args)))

;;------------------------------------------------------------------------------
;; An var-refs, named-consts etc. kann nichts optimiert werden.
;;------------------------------------------------------------------------------

(defmethod to-1form ((a-form form))
  a-form)

;;------------------------------------------------------------------------------

(defun is-bottom-typed (a-form)
  (is-bottom-t (?type a-form)))

(defmethod to-1form ((a-setq-form setq-form))
  (if (is-bottom-typed (?form a-setq-form))
      (progn
        (eliminating-msg "(in ~A) Eliminating bottom-typed setq." *current-fun*)
        (inc-stats 'bt-setq)
        (dec-used-slot (?location a-setq-form) 'write)
        (?form a-setq-form))
      a-setq-form))

;;------------------------------------------------------------------------------

(defmethod to-1form ((a-progn-form progn-form))
  (let* ((form-list (?form-list a-progn-form))
         (bottom-form-pos (position-if #'is-bottom-typed form-list)))
    (setf (?form-list a-progn-form)
          (if (and bottom-form-pos
                   (< (incf bottom-form-pos) (length form-list)))
              (progn
                (eliminating-msg
                 "(in ~A) Eliminating unreachable code in a progn-form"
                 *current-fun*)
                (inc-stats 'bt-progn)
                (mapc #'dec-used-slot (nthcdr bottom-form-pos form-list))
                (let ((new-form-list (subseq form-list 0 bottom-form-pos)))
                  (setf (?type a-progn-form)
                        (?type (first (last new-form-list))))
                  new-form-list))
              form-list)))
  a-progn-form)

;;------------------------------------------------------------------------------

(defmethod to-1form ((an-if-form if-form))
  (let* ((pred (?pred an-if-form))
         (pred-type (?type pred))
         (then (?then an-if-form))
         (else (?else an-if-form))
         eliminated-case
         (result 
          (cond ((is-bottom-t pred-type)
                 (inc-stats 'then-optis)
                 (dec-used-slot then)
                 (inc-stats 'else-optis)
                 (dec-used-slot else)
                 (setq eliminated-case "then- and else")
                 pred)
                ((zs-subtypep pred-type null-t)
                 (inc-stats 'then-optis)
                 (dec-used-slot then)
                 (setq eliminated-case "then")
                 (optimize-1form
                  (make-instance 'progn-form 
                                 :form-list (list pred else)
                                 :type (?type else))))
                ((not (zs-subtypep null-t pred-type))
                 (inc-stats 'else-optis)
                 (dec-used-slot else)
                 (setq eliminated-case "else")
                 (optimize-1form
                  (make-instance 'progn-form
                                 :form-list (list pred then)
                                 :type (?type then))))
                (T an-if-form))))
    (when eliminated-case
      (eliminating-msg "(in ~A) Eliminating unreachable ~A-case."
                       *current-fun*
                       eliminated-case))
    result))

;;------------------------------------------------------------------------------

(defmethod to-1form ((an-app app))
  (if (or (some #'is-bottom-typed (?arg-list an-app))
          (is-bottom-typed (?form an-app)))
      (progn
        (eliminating-msg "(in ~A) Eliminating unreachable app-form"
                         *current-fun*)
        (inc-stats 'bt-app)
        (let ((form-list (cons (?form an-app) (?arg-list an-app))))
          (optimize-1form
           (make-instance 'progn-form
                          :form-list form-list

                          ;; form-list ist mind. 1 elementig, also kann man
                          ;; das letzte Element bestimmen.
                          ;;------------------------------
                          :type (?type (first (last form-list)))))))
      an-app))

;;------------------------------------------------------------------------------

(defmethod to-1form ((a-let*-form let*-form))
  (let* ((init-list (?init-list a-let*-form))
         (bottom-form-pos (position-if #'is-bottom-typed init-list)))
    (when bottom-form-pos
      (eliminating-msg "(in ~A) Eliminating unreachable code in a let*-form"
                       *current-fun*)
      (inc-stats 'bt-let*)
      (mapc #'dec-used-slot (nthcdr (1+ bottom-form-pos)
                                    (?init-list a-let*-form)))
      (dolist (var (nthcdr (1+ bottom-form-pos) (?var-list a-let*-form)))
        (dec-used-slot var 'write))
      (dec-used-slot (?body a-let*-form))
      (setf (?init-list a-let*-form) (subseq init-list 0 bottom-form-pos)
            (?var-list a-let*-form) (subseq (?var-list a-let*-form)
                                            0 bottom-form-pos)
            (?body a-let*-form) (nth bottom-form-pos init-list)
            (?type a-let*-form) (?type (?body a-let*-form)))))
  a-let*-form)

;;------------------------------------------------------------------------------

(defmethod to-1form ((a-mv-lambda mv-lambda))
  (if (is-bottom-typed (?arg a-mv-lambda))
      (progn
        (eliminating-msg "(in ~A) Eliminating unreachable mv-lambda"
                         *current-fun*)
        (inc-stats 'bt-mv-lambda)
        (dec-used-slot (?params a-mv-lambda))
        (dec-used-slot (?body a-mv-lambda))
        (?arg a-mv-lambda))
      a-mv-lambda))

;;------------------------------------------------------------------------------
(provide "tomain")
