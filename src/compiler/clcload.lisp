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
;;; Funktion : Startmodul zum Laden des Compilers in ein Common-LISP System
;;;
;;; $Revision: 1.111 $
;;; $Id: clcload.lisp,v 1.111 1995/06/01 16:03:55 hk Exp $
;;;-----------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Damit require dort sucht, wo diese Datei clcload steht
;;------------------------------------------------------------------------------
#+:mcl
(pushnew (directory-namestring ccl:*loading-file-source-file*)
         ccl:*module-search-path* :test #'pathname-match-p)

(in-package "USER")

;;------------------------------------------------------------------------------
;; System-spezifische Dinge
;;------------------------------------------------------------------------------
#+(or allegro-v4.0 allegro-v4.1)
(setq comp:*cltl1-compile-file-toplevel-compatibility-p* t)

#+AKCL
(multiple-value-bind (sym status)
    (find-symbol "TAG" "LISP")
  (when (eq status :EXTERNAL)
    (unexport sym "LISP")))

#+CMU17
(when (equal (lisp-implementation-version) "17f")
  (require "cmu-17f-slow-method-patch"))

;;------------------------------------------------------------------------------
;; Package RUNTIME erzeugen. Ein anderes Package, das nur zuf"allig "RT" hei"st
;; vorher in "original-rt" umbenennen.
;;------------------------------------------------------------------------------
(when (and (find-package "RT")
           (not (member "CLICC-RT" (package-nicknames "RT") :test #'equal)))
              (rename-package "RT" "original-rt"))

(unless (find-package "RT")
        (make-package "RT" :use () :nicknames '("CLICC-RT")))

;;------------------------------------------------------------------------------
;; Package FFI erzeugen. Ein anderes Package, das nur zuf"allig "FFI" hei"st
;; vorher in "original-ffi" umbenennen.
;;------------------------------------------------------------------------------
(when (and (find-package "FFI")
           (not (member "CLICC-FFI" (package-nicknames "FFI") :test #'equal)))
              (rename-package "FFI" "original-ffi"))

(unless (find-package "FFI")
        (make-package "FFI" :use () :nicknames '("CLICC-FFI")))

(in-package "FFI")

(lisp:export 
 '(c-array c-char c-char-p c-double c-double-p c-enum c-float c-float-p c-fun
   c-handle c-int c-int-p c-long c-long-double c-long-double-p c-long-p c-ptr
   c-short c-short-p c-string c-string-p c-struct c-union c-unsigned-char
   c-unsigned-char-p c-unsigned-int c-unsigned-int-p c-unsigned-long
   c-unsigned-long-p c-unsigned-short c-unsigned-short-p c-vararg c-void
   copy-c-string def-c-type def-call-in def-call-out foreign-package-name
   free lisp-character lisp-float lisp-integer load-foreign make-c-string
   make-lisp-string sys-include user-include)
 "FFI")

(lisp:in-package "USER")

;;------------------------------------------------------------------------------
;; Definition der Funktion rt::c-getenv
;;------------------------------------------------------------------------------
#+(or EXCL KCL)
(defun rt::c-getenv (name)
  (system:getenv name))
#+LUCID
(defun rt::c-getenv (name)
  (lcl:environment-variable name))
#+CMU
(defun rt::c-getenv (name)
  (setq name (intern name "KEYWORD"))
  (cdr (assoc name extensions:*ENVIRONMENT-LIST*)))
#+(or :lispworks CLISP)
(defun rt::c-getenv (name)
  (system::getenv name))

;;------------------------------------------------------------------------------
;; Eigene Lisp- und User-Packages generieren
;;------------------------------------------------------------------------------
(unless (find-package "CLICC-LISP")
        (make-package "CLICC-LISP" :use () :nicknames '("L")))

(import '(
   
   ;; Lambda-List-Keywords
   ;;---------------------
   &allow-other-keys &aux &body &environment &key &optional &rest &whole
   
   ;; Miscellaneous
   ;;--------------
   lambda declare otherwise *
   
   ;; Special Operators
   ;;------------------
   block catch eval-when flet function go if labels let let* 
   locally macrolet multiple-value-call multiple-value-prog1 progn progv quote
   return-from setq tagbody the throw unwind-protect
     
   ;; Atomic Type Specifiers
   ;;-----------------------
   array atom bignum bit bit-vector
   character compiled-function complex cons double-float
   fixnum float function hash-table
   integer keyword list long-float nil null number package
   pathname random-state ratio rational readtable sequence
   short-float signed-byte simple-array simple-bit-vector
   simple-string simple-vector single-float standard-char stream string t
   unsigned-byte vector
   
   ;; Compound-Only Type Specifier Names
   ;;-----------------------------------
   and eql member mod not or satisfies values

   ;; Declaration Identifiers
   ;;------------------------
   ignore
   
   ;; Macros, Data and Control Flow
   ;;------------------------------
   psetq return
   and cond or when unless case ccase ecase typecase ctypecase etypecase
   multiple-value-bind multiple-value-list multiple-value-setq 
   prog prog* prog1 prog2 psetf shiftf rotatef

   ;; Other Macros
   ;;-------------
   assert decf do do* do-all-symbols
   do-external-symbols do-symbols dolist dotimes loop remf with-open-stream
   with-input-from-string with-output-to-string with-open-file

   ;; Functions wich are used during macro expansion
   ;;-----------------------------------------------
   >= car cdr close streamp open get-output-stream-string
   make-string-output-stream make-string-input-stream list 
   list* 1+ 1- ash logxor logior logand logandc2
   typep eql error values not

   ;; Functions wich are used during setf expansion
   ;;----------------------------------------------
   cons append list apply aref funcall + - adjoin first rest

   ;; Functions which are used for macro expander function generation
   ;;----------------------------------------------------------------
   getf car cadr caddr cadddr cdr cddr cdddr cddddr endp error nth nthcdr
   null not eq endp member)

   "CLICC-LISP")

  ;;---------------------------------------------------------------------------
  
(in-package "CLICC-LISP")

(lisp:export '(
          &allow-other-keys &aux &body &environment &key &optional
          &rest &whole * ** *** *break-on-signals* *compile-file-pathname*
          *compile-file-truename* *compile-print* *compile-verbose* *debug-io*
          *debugger-hook* *default-pathname-defaults* *error-output* *features*
          *gensym-counter* *load-pathname* *load-print* *load-truename*
          *load-verbose* *macroexpand-hook* *modules* *package* *print-array*
          *print-base* *print-case* *print-circle* *print-escape* *print-gensym*
          *print-length* *print-level* *print-lines* *print-miser-width*
          *print-pprint-dispatch* *print-pretty* *print-radix* *print-readably*
          *print-right-margin* *query-io* *random-state* *read-base*
          *read-default-float-format* *read-eval* *read-suppress* *readtable*
          *standard-input* *standard-output* *terminal-io* *trace-output* + ++
          +++ - / // /// /= 1+ 1- < <= = > >= abort abs acons acos acosh
          add-method adjoin adjust-array adjustable-array-p allocate-instance
          alpha-char-p alphanumericp and append apply apropos apropos-list aref
          arithmetic-error arithmetic-error-operands arithmetic-error-operation
          array array-dimension array-dimension-limit array-dimensions
          array-displacement array-element-type array-has-fill-pointer-p
          array-in-bounds-p array-rank array-rank-limit array-row-major-index
          array-total-size array-total-size-limit arrayp ash asin asinh assert
          assoc assoc-if assoc-if-not atan atanh atom base-char base-string
          bignum bit bit-and bit-andc1 bit-andc2 bit-eqv bit-ior bit-nand
          bit-nor bit-not bit-orc1 bit-orc2 bit-vector bit-vector-p bit-xor
          block boole boole-1 boole-2 boole-and boole-andc1 boole-andc2 boole-c1
          boole-c2 boole-clr boole-eqv boole-ior boole-nand boole-nor boole-orc1
          boole-orc2 boole-set boole-xor both-case-p boundp break
          broadcast-stream broadcast-stream-streams built-in-class butlast byte
          byte-position byte-size caaaar caaadr caaar caadar caaddr caadr caar
          cadaar cadadr cadar caddar cadddr caddr cadr call-arguments-limit
          call-method call-next-method car case catch ccase cdaaar cdaadr cdaar
          cdadar cdaddr cdadr cdar cddaar cddadr cddar cdddar cddddr cdddr cddr
          cdr ceiling cell-error cell-error-name cerror change-class char
          char-code char-code-limit char-downcase char-equal char-greaterp
          char-int char-lessp char-name char-not-equal char-not-greaterp
          char-not-lessp char-upcase char/= char< char<= char= char> char>=
          character characterp check-type cis class class-name class-of
          clear-input clear-output close clrhash code-char coerce
          compilation-speed compile compile-file compile-file-pathname
          compiled-function compiled-function-p compiler-macro
          compiler-macro-function complement complex complexp
          compute-applicable-methods compute-restarts concatenate
          concatenated-stream concatenated-stream-streams cond condition
          conjugate cons consp constantly constantp continue control-error
          copy-alist copy-list copy-pprint-dispatch copy-readtable copy-seq
          copy-structure copy-symbol copy-tree cos cosh count count-if
          count-if-not ctypecase debug decf declaim declaration declare
          decode-float decode-universal-time defclass defconstant defgeneric
          define-compiler-macro define-condition define-method-combination
          define-modify-macro define-setf-expander defmacro defmethod defpackage
          defparameter defsetf defstruct deftype defun defvar delete
          delete-duplicates delete-file delete-if delete-if-not delete-package
          denominator deposit-field describe describe-object destructuring-bind
          digit-char digit-char-p directory directory-namestring disassemble
          division-by-zero do do* do-all-symbols do-external-symbols do-symbols
          documentation dolist dotimes double-float double-float-epsilon
          double-float-negative-epsilon dpb dribble dynamic-extent ecase
          echo-stream echo-stream-input-stream echo-stream-output-stream ed
          eighth elt encode-universal-time end-of-file endp enough-namestring
          ensure-generic-function eq eql equal equalp error etypecase eval
          eval-when evenp every exp export expt extended-char fboundp fceiling
          fdefinition ffloor fifth file-author file-error file-error-pathname
          file-length file-namestring file-position file-stream
          file-string-length file-write-date fill fill-pointer find
          find-all-symbols find-class find-if find-if-not find-method
          find-package find-restart find-symbol finish-output first fixnum flet
          float float-digits float-precision float-radix float-sign
          floating-point-inexact floating-point-invalid-operation
          floating-point-overflow floating-point-underflow floatp floor
          fmakunbound force-output format formatter fourth fresh-line fround
          ftruncate ftype funcall function function-keywords
          function-lambda-expression functionp gcd generic-function gensym
          gentemp get get-decoded-time get-dispatch-macro-character
          get-internal-real-time get-internal-run-time get-macro-character
          get-output-stream-string get-properties get-setf-expansion
          get-universal-time getf gethash go graphic-char-p handler-bind
          handler-case hash-table hash-table-count hash-table-p
          hash-table-rehash-size hash-table-rehash-threshold hash-table-size
          hash-table-test host-namestring identity if ignorable ignore
          ignore-errors imagpart import in-package incf initialize-instance
          inline input-stream-p inspect integer integer-decode-float
          integer-length integerp interactive-stream-p intern
          internal-time-units-per-second intersection invalid-method-error
          invoke-debugger invoke-restart invoke-restart-interactively isqrt
          keyword keywordp labels lambda lambda-list-keywords
          lambda-parameters-limit last lcm ldb ldb-test ldiff
          least-negative-double-float least-negative-long-float
          least-negative-normalized-double-float
          least-negative-normalized-long-float
          least-negative-normalized-short-float
          least-negative-normalized-single-float least-negative-short-float
          least-negative-single-float least-positive-double-float
          least-positive-long-float least-positive-normalized-double-float
          least-positive-normalized-long-float
          least-positive-normalized-short-float
          least-positive-normalized-single-float least-positive-short-float
          least-positive-single-float length let let* lisp-implementation-type
          lisp-implementation-version list list* list-all-packages list-length
          listen listp load load-logical-pathname-translations load-time-value
          locally log logand logandc1 logandc2 logbitp logcount logeqv
          logical-pathname logical-pathname-translations logior lognand lognor
          lognot logorc1 logorc2 logtest logxor long-float long-float-epsilon
          long-float-negative-epsilon long-site-name loop loop-finish
          lower-case-p machine-instance machine-type machine-version
          macro-function macroexpand macroexpand-1 macrolet make-array
          make-broadcast-stream make-concatenated-stream make-condition
          make-dispatch-macro-character make-echo-stream make-hash-table
          make-instance make-instances-obsolete make-list make-load-form
          make-load-form-saving-slots make-method make-package make-pathname
          make-random-state make-sequence make-string make-string-input-stream
          make-string-output-stream make-symbol make-synonym-stream
          make-two-way-stream makunbound map map-into mapc mapcan mapcar mapcon
          maphash mapl maplist mask-field max member member-if member-if-not
          merge merge-pathnames method method-combination
          method-combination-error method-qualifiers min minusp mismatch mod
          most-negative-double-float most-negative-fixnum
          most-negative-long-float most-negative-short-float
          most-negative-single-float most-positive-double-float
          most-positive-fixnum most-positive-long-float
          most-positive-short-float most-positive-single-float muffle-warning
          multiple-value-bind multiple-value-call multiple-value-list
          multiple-value-prog1 multiple-value-setq multiple-values-limit
          name-char namestring nbutlast nconc next-method-p nil nintersection
          ninth no-applicable-method no-next-method not notany notevery
          notinline nreconc nreverse nset-difference nset-exclusive-or
          nstring-capitalize nstring-downcase nstring-upcase nsublis nsubst
          nsubst-if nsubst-if-not nsubstitute nsubstitute-if nsubstitute-if-not
          nth nth-value nthcdr null number numberp numerator nunion oddp open
          open-stream-p optimize or otherwise output-stream-p package
          package-error package-error-package package-name package-nicknames
          package-shadowing-symbols package-use-list package-used-by-list
          packagep pairlis parse-error parse-integer parse-namestring pathname
          pathname-device pathname-directory pathname-host pathname-match-p
          pathname-name pathname-type pathname-version pathnamep peek-char phase
          pi plusp pop position position-if position-if-not pprint
          pprint-dispatch pprint-exit-if-list-exhausted pprint-fill
          pprint-indent pprint-linear pprint-logical-block pprint-newline
          pprint-pop pprint-tab pprint-tabular prin1 prin1-to-string princ
          princ-to-string print print-not-readable print-not-readable-object
          print-object print-unreadable-object probe-file proclaim prog prog*
          prog1 prog2 progn program-error progv provide psetf psetq push pushnew
          quote random random-state random-state-p rassoc rassoc-if
          rassoc-if-not ratio rational rationalize rationalp read read-byte
          read-char read-char-no-hang read-delimited-list read-from-string
          read-line read-preserving-whitespace reader-error readtable
          readtable-case readtablep real realp realpart reduce
          reinitialize-instance rem remf remhash remove remove-duplicates
          remove-if remove-if-not remove-method remprop rename-file
          rename-package replace require rest restart restart-bind restart-case
          restart-name return return-from revappend reverse room rotatef round
          row-major-aref rplaca rplacd safety satisfies sbit scale-float schar
          search second sequence serious-condition set set-difference
          set-dispatch-macro-character set-exclusive-or set-macro-character
          set-pprint-dispatch set-syntax-from-char setf setq seventh shadow
          shadowing-import shared-initialize shiftf short-float
          short-float-epsilon short-float-negative-epsilon short-site-name
          signal signed-byte signum simple-array simple-base-string
          simple-bit-vector simple-bit-vector-p simple-condition
          simple-condition-format-arguments simple-condition-format-control
          simple-error simple-string simple-string-p simple-type-error
          simple-vector simple-vector-p simple-warning sin single-float
          single-float-epsilon single-float-negative-epsilon sinh sixth sleep
          slot-boundp slot-exists-p slot-makunbound slot-missing slot-unbound
          slot-value software-type software-version some sort space special
          special-operator-p speed sqrt stable-sort standard standard-char
          standard-char-p standard-class standard-generic-function
          standard-method standard-object step storage-condition store-value
          stream stream-element-type stream-error stream-error-stream
          stream-external-format streamp string string-capitalize
          string-downcase string-equal string-greaterp string-left-trim
          string-lessp string-not-equal string-not-greaterp string-not-lessp
          string-right-trim string-stream string-trim string-upcase string/=
          string< string<= string= string> string>= stringp structure
          structure-class structure-object style-warning sublis subseq subsetp
          subst subst-if subst-if-not substitute substitute-if substitute-if-not
          subtypep svref sxhash symbol symbol-function symbol-macrolet
          symbol-name symbol-package symbol-plist symbol-value symbolp
          synonym-stream synonym-stream-symbol t tagbody tailp tan tanh tenth
          terpri the third throw time trace translate-logical-pathname
          translate-pathname tree-equal truename truncate two-way-stream
          two-way-stream-input-stream two-way-stream-output-stream type
          type-error type-error-datum type-error-expected-type type-of typecase
          typep unbound-slot unbound-slot-instance unbound-variable
          undefined-function unexport unintern union unless unread-char
          unsigned-byte untrace unuse-package unwind-protect
          update-instance-for-different-class
          update-instance-for-redefined-class upgraded-array-element-type
          upgraded-complex-part-type upper-case-p use-package use-value
          user-homedir-pathname values values-list variable vector vector-pop
          vector-push vector-push-extend vectorp warn warning when
          wild-pathname-p with-accessors with-compilation-unit
          with-condition-restarts with-hash-table-iterator
          with-input-from-string with-open-file with-open-stream
          with-output-to-string with-package-iterator with-simple-restart
          with-slots with-standard-io-syntax write write-byte write-char
          write-line write-string write-to-string y-or-n-p yes-or-no-p zerop)

        "CLICC-LISP"
        )

(lisp:in-package "USER")

;;------------------------------------------------------------------------------
(unless (find-package "CLICC-USER")
        (make-package "CLICC-USER" :use '("CLICC-LISP")))

;;------------------------------------------------------------------------------
;; Den Rest in das Package CLICC
;;------------------------------------------------------------------------------
(unless (find-package "CLICC")
  (make-package "CLICC" :use '(#-mcl "LISP"
                               #+mcl "COMMON-LISP"
                               #+CLOS "CLOS"
                               #+(and PCL (not CLTL2))  "PCL")))

(in-package "CLICC")

#+allegro-v4.0 (import '(cltl1:require cltl1:provide cltl1:*modules*))

#+lispworks (import '(system::require system::provide system::*modules*))

#+mcl (import '(ccl:require ccl:provide ccl:*modules*))

(import
 '(do-lisp-module do-inline-module ask-runtime compile-clicc clicc do-clicc)
 "USER")

;;------------------------------------------------------------------------------
;; Eigenen Backquote Reader installieren, um eine normierte Darstellung
;; fuer eingelesene Backquote Ausdruecke zu erhalten.
;;------------------------------------------------------------------------------
(require "bq-read")

;;------------------------------------------------------------------------------
(require "clcdef")

;;------------------------------------------------------------------------------
;; CLICC soll gespraechig sein
;;------------------------------------------------------------------------------
(setq *SHOW-VERSION* t)
(setq *CLICC-PRINT* t)

;;------------------------------------------------------------------------------
(require "config")

;;------------------------------------------------------------------------------
;; DIE Funktion zum Aufruf von CLICC
;; Resultat zeigt an, ob die Uebersetzung erfolgreich war
;;------------------------------------------------------------------------------
(defun clicc (filename &KEY
                       (out              nil)
                         
                       (module-compiler  nil)
                       (lisp-module      nil)
                       (inline-module    nil)
                         
                       (verbose          *SHOW-VERSION*)
                       (print            *CLICC-PRINT*)
                       (memsizes         *MEMSIZES*)
                       (no-codegen       *NO-CODEGEN*)
                       (ti-level         *TI-Level*)
                       (obrep            *OBREP*)
                       (max-lines        *C-max-line-count*)
                       (split            *SPLIT-FILES*)
                       (flat-ifs         *FLAT-IFS*))

  (setq *FILENAME*         filename)
  (setq *OUT-FILENAME*     out)
  
  (setq *MODULE-COMPILER*  module-compiler)
  (setq *LISP-MODULE*      lisp-module)
  (setq *INLINE-MODULE*    inline-module)

  ;; Durch die Zuweisung werden die aktuellen Einstellungen zum Defaultwert
  ;; für den nächsten Aufruf
  ;;------------------------
  (setq *SHOW-VERSION*     verbose)
  (setq *CLICC-PRINT*      print)
  (setq *MEMSIZES*         memsizes)
  (setq *NO-CODEGEN*       no-codegen)
  (setq *TI-Level*         ti-level)
  (setq *OBREP*            obrep)
  (setq *C-max-line-count* max-lines)
  (setq *SPLIT-FILES*      split)
  (setq *FLAT-IFS*         flat-ifs)
  
  ;; Package automatisch auf CLICC setzen, damit Debugging erleichtert wird
  ;;-----------------------------------------------------------------------
  (in-package "CLICC")
  
  (let* ((lisp-package (find-package "LISP"))
         (lisp-name (package-name lisp-package))
         (lisp-nick (package-nicknames lisp-package))
         (user-package (find-package "USER"))
         (user-name (package-name user-package))
         (user-nick (package-nicknames user-package))
         (*package* *package*)

         ;; "home:" ist der Pfad, wenn man auf einem Mac eine Applikation auf
         ;; das CLiCC Icon zieht.
         ;;----------------------
         #+:mcl (*default-pathname-defaults*
                 (directory-namestring
                  (cl:translate-logical-pathname "home:"))))
    (unwind-protect
         (progn
           (setq *bq-level* 0)
           #+allegro-v4.1 (setq excl:*enable-package-locked-errors* nil)
           (rename-package lisp-package "original-lisp")
           #+allegro-v4.1 (setq excl:*enable-package-locked-errors* t)
           (rename-package user-package "original-user")
           (rename-package "CLICC" "original-clicc")
           (when (find-package "CLICC-CLICC")
             (rename-package "CLICC-CLICC" "CLICC"))
           (rename-package "CLICC-LISP" "LISP")
           (rename-package "CLICC-USER" "USER")
           (do-clicc))
      (rename-package "LISP" "CLICC-LISP" '("L"))
      (rename-package "USER" "CLICC-USER")
      (when (find-package "CLICC")
        #+allegro-v4.1 (rename-package "CLICC" "CLICC")
        (rename-package "CLICC" "CLICC-CLICC"))
      (rename-package "original-clicc" "CLICC")
      #+allegro-v4.1 (setq excl:*enable-package-locked-errors* nil)
      (rename-package lisp-package lisp-name lisp-nick)
      #+allegro-v4.1 (setq excl:*enable-package-locked-errors* t)
      (rename-package user-package user-name user-nick))))

;;------------------------------------------------------------------------------
;; Die restlichen Definitionen dienen zur leichteren Handhabung der Uebersetzung
;; von clicc bzw. des Clicc-Laufzeitsystems
;;------------------------------------------------------------------------------
(defvar *CLICC-LISP-DIR*)
(defvar *CLICC-LISP2C-DIR*)
(defvar *CLICC-COMPILER-DIR*)
(defvar *lisp-extension*)
(defvar *compiled-extension*)

;;------------------------------------------------------------------------------
;; CLiCC Directories werden gesetzt.
;; Dies sind die Verzeichnisse des Laufzeitsystems und den
;; Quellcode des Compilers.
;; Die Wurzel des CLiCC-Filebaums und die Stellen, wo lisp.def/syntax und
;; sys.def/syntax zu finden sind, werden in config.lisp in der Funktion
;; setup-clicc-pathenames gesetzt.
;;------------------------------------------------------------------------------
(defun setup-clcload-pathnames ()

  (setq *CLICC-LISP-DIR* (append *CLICC-DIR* (list "src" "runtime" "lisp")))
  (setq *CLICC-LISP2C-DIR* (append *CLICC-DIR* (list "src" "runtime" "lisp2c")))
  (setq *CLICC-COMPILER-DIR* (append *CLICC-DIR* (list "src" "compiler"))))

(setup-clcload-pathnames)

  
(setq *lisp-extension* "lisp")

#+LUCID
(progn
  (import '(lcl:quit lcl:cd lcl:pwd))
  (setq *compiled-extension* (car lcl:*load-binary-pathname-types*)))
#+EXCL
(progn
  (import '(excl:exit excl:chdir excl:current-directory))
  (setq *compiled-extension* excl:*fasl-default-type*))
#+KCL
(progn 
  (setq *lisp-extension* "lsp")
  (setq *compiled-extension* "o"))
#+CMU
(progn
  (import '(extensions:quit))
  (setq *compiled-extension* (c:backend-fasl-file-type c:*backend*)))
#+CLISP
(progn
  ;; type must be upper case under DOS
  (setq *lisp-extension* (pathname-type "any.lsp"))
  (setq *compiled-extension* (pathname-type "any.fas")))
#+LISPWORKS
(setq *compiled-extension system::*binary-file-type*)

;;------------------------------------------------------------------------------
;; Fragt, welche in LISP geschriebenen CLICC-Laufzeitsystem-Funktionen mittels
;; clicc nach C uebersetzt werden sollen.
;;------------------------------------------------------------------------------
(defun ask-runtime ()
  (let ((*SHOW-VERSION* nil)
        (*CLICC-PRINT* t)
        (*SPLIT-FILES* *SPLIT-FILES*)
        (*ONLY-PASS1* *ONLY-PASS1*))

    (loop
     (format
      t
      "~&A)ll L)isp-module I)nline-module Q)uit~%~
      P)rint messages is ~a  S)plit is ~a  O)nly Pass1 is ~a  ~
       K)ill C-Files ~%~
       choose: "
      (if *CLICC-PRINT* "on" "off")
      (if *SPLIT-FILES* "on" "off")
      (if *ONLY-PASS1* "on" "off")) 

     (case (let ((*package* (find-package "CLICC"))) (read))
           (Q (return))    
           (A (do-inline-module) (do-lisp-module) (return))
           (L (do-lisp-module))
           (I (do-inline-module))
           (O (setq *ONLY-PASS1* (not *ONLY-PASS1*)))
           (P (setq *CLICC-PRINT* (not *CLICC-PRINT*)))
           (S (setq *SPLIT-FILES* (not *SPLIT-FILES*)))
           (K (mapc #'delete-file
                    (directory (make-pathname
                                :directory *CLICC-LISP2C-DIR*
                                :name :wild
                                :type "c"))))))))

;;------------------------------------------------------------------------------
;; Pr"uft, ob Probleme durch Splitting auftreten werden
;;------------------------------------------------------------------------------
(defun check-split (unsplitted splitted)
  (not
   (cond
     (*SPLIT-FILES*
      (when (probe-file (make-pathname :directory *CLICC-LISP2C-DIR*
                                       :name unsplitted :type "c"))
        (format t "There is an unsplitted version, use K)ill first~%")
        t))
     (t (when (probe-file (make-pathname :directory *CLICC-LISP2C-DIR*
                                         :name splitted :type "c"))
          (format t "There is an splitted version, use K)ill first~%")
          t)))))

;;------------------------------------------------------------------------------
;; Compiliert das Lisp Modul des Laufzeitsystems
;;------------------------------------------------------------------------------
(defun do-lisp-module ()
  (when (check-split "lisp" "Fread")
    (let ((*C-max-line-count* nil)
          (*delete-verbosity* 2)
          (*default-pathname-defaults*
           (make-pathname :directory *CLICC-LISP-DIR*)))
      (clicc "lisp" 
             :out (make-pathname :directory *CLICC-LISP2C-DIR* :name "lisp")
             :module-compiler t
             :lisp-module t))))

;;------------------------------------------------------------------------------
;; Compiliert die Inline Funktionen, ueber die der Compiler spezielles Wissen
;; hat
;;------------------------------------------------------------------------------
(defun do-inline-module ()
  (when (check-split "inline" "Fconsp")
    (let ((*delete-verbosity* 2)
          (*default-pathname-defaults*
           (make-pathname :directory *CLICC-LISP-DIR*)))
      (clicc "inline" 
             :out (make-pathname :directory *CLICC-LISP2C-DIR* :name "inline")
             :module-compiler t
             :inline-module t
             :lisp-module t))))

;;------------------------------------------------------------------------------
;; Kompiliert alle in *MODULES* angegebenen Dateien neu, falls noetig.
;;------------------------------------------------------------------------------
(defun compile-clicc (&optional (module-list (reverse *MODULES*)))
  (dolist (module module-list)
    (let* ((lisp-filename 
            (make-pathname
             :directory *CLICC-COMPILER-DIR*
             :name (pathname-name module)
             :type *lisp-extension*))
           (sbin-filename
            (make-pathname
             :type *compiled-extension*
             :defaults lisp-filename)))
      (when (probe-file lisp-filename)
        (when (os-file-newer-as lisp-filename sbin-filename)
          #+CMU
          (compile-file lisp-filename :load t)
          #-CMU
          (progn
            (compile-file lisp-filename)
            (load         sbin-filename)))))))

;;------------------------------------------------------------------------------
(defun os-file-newer-as (file1 file2)
  (let ((file2-write-date (and (probe-file file2) (file-write-date file2))))
    (or (null file2-write-date)
        (> (file-write-date file1) file2-write-date))))

;;------------------------------------------------------------------------------
(require "clcmain")
;;;#-CMU17(require "printzs")              ;bug in pcl:class-direct-superclasses
