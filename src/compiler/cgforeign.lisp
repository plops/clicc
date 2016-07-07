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
;;; Function : Foreign Functions - codegeneration
;;;
;;; $Revision: 1.28 $
;;; $Id: cgforeign.lisp,v 1.28 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
(defconstant NOT-IMPLEMENTED-YET 
  "Not implemented yet: ~A")

;;******************************************************************************
;; Call-Out
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Erzeuge die Applikation einer Foreign Function
;;------------------------------------------------------------------------------
(defmethod cg-app ((fun foreign-fun) args app)
  (declare (ignore app))

  (let* ((return-type (?return-type fun))
         arg-list
         typesymbol)
    (multiple-value-bind (key old-name old-type)
        (select-type return-type return-type return-type)
      (declare (ignore old-name old-type))
      (case key
        ((old-struct old-union old-array old-handle ptr)
         (setq arg-list (cg-ffi-args (cdr args) fun))
         (setq typesymbol (first args)))
        (otherwise
         (setq arg-list (cg-ffi-args args fun))
         (setq typesymbol nil)))

      ;; Aktuellen stack-top retten fuer eventuelle Call-Ins
      ;;----------------------------------------------------
      (when *ffi-need-savestack*
        (C-assign "save_stack" (CC-Stack *stack-top*)))
      
      (C-ForeignCall fun *stack-top* arg-list typesymbol)
      (stacktop-to-result-loc))))

;;------------------------------------------------------------------------------
;; Pendant zu cg-args. 
;;------------------------------------------------------------------------------
(defun cg-ffi-args (args fun)
  (let ((*result-spec* (stacktop-result-location))
        (old-stack *stack-top*)
        (arglist-queue (empty-queue)))
    
    (do ((arg args (cdr arg))
         (type (?arguments fun) 
               (if (equalp type '(ffi:c-vararg)) type (cdr type))))
        ((null arg))
      (let ((arg (first arg))
            (type (first type)))
        (multiple-value-bind (key old-name old-type)
            (select-type type type type)
          (declare (ignore old-name))
          (case key
            (string     (cg-ff-string arg arglist-queue))
            (primitive  (cg-ff-primitive old-type arg arglist-queue))
            (old-struct (cg-ff-struct-old type arg arglist-queue))
            (old-union  (cg-ff-union-old type arg arglist-queue))
            (old-array  (cg-ff-array-old type arg arglist-queue))
            (old-handle (cg-ff-handle-old arg arglist-queue))
            (ptr        (cg-ff-ptr old-type arg arglist-queue))
            
            (otherwise  (cg-ff-error 'cg-ffi-args old-type))))))

    (setq *stack-top* old-stack)
    (values (queue2list arglist-queue))))

;;------------------------------------------------------------------------------
;; erzeugt:
;;   LOAD_C_... (... , <base,+0>);
;;------------------------------------------------------------------------------
(defun C-ForeignCall (fun base arg-list typesymbol)
  (let* ((return-type (?return-type fun))
         (stack (CC-Stack base))
         (top (CC-Stack *stack-top*)))
    (multiple-value-bind (key old-name old-type)
        (select-type return-type return-type return-type)
      (declare (ignore old-name))
      (case key
        (void
         (C-Cmd (CC-ForeignCallArgs fun arg-list))
         (C-nil stack))
        
        (string
         (C-blockstart)
         (C-VarDeclInit "char *" "var" (CC-ForeignCallArgs fun arg-list))
         (C-MacroCall "LOAD_C_STRING" top "var" stack)
         (C-blockend))
        
        (primitive
         (C-blockstart)
         (C-VarDeclInit (convert-c-type-to-string old-type)
                        "var" (CC-ForeignCallArgs fun arg-list))
         (C-MacroCall (concatenate 'string "LOAD_" (c-macro-string old-type))
                      top "var" stack)
         (C-blockend))
        
        (old-struct
         (let* ((c-type (convert-c-type-to-string return-type)))
           (C-blockstart)
           (C-VarDeclInit "void *"
                          "ptr"
                          (format nil "(void *)malloc(sizeof(~A))" c-type))
           (C-MacroCall "LOAD_C_STRUCT"
                        top "ptr" stack (CC-Symbol typesymbol))
           (C-Cmd 
            (format nil
                    "*((~A *)ptr) = ~A"
                    c-type
                    (CC-ForeignCallArgs fun arg-list)))
           (C-blockend)))

        (old-union
         (let* ((c-type (convert-c-type-to-string return-type)))
           (C-blockstart)
           (C-VarDeclInit "void *"
                          "ptr"
                          (format nil "(void *)malloc(sizeof(~A))" c-type))
           (C-MacroCall "LOAD_C_UNION"
                        top "ptr" stack (CC-Symbol typesymbol))
           (C-Cmd
            (format nil
                    "*((~A *)ptr) = ~A"
                    c-type
                    (CC-ForeignCallArgs fun arg-list)))
           (C-blockend)))

        (old-array
         (let* ((c-type (convert-c-type-to-string return-type)))
           (C-blockstart)
           (C-VarDeclInit "void *"
                          "ptr"
                          (format nil "(void *)malloc(sizeof(~A))" c-type))
           (C-MacroCall "LOAD_C_ARRAY"
                        top "ptr" stack (CC-Symbol typesymbol))
           (C-Cmd
            (format nil
                    "*((~A *)ptr) = ~A"
                    c-type
                    (CC-ForeignCallArgs fun arg-list)))
           (C-blockend)))

        (old-handle
         (C-blockstart)
         (C-VarDeclInit "void *"
                        "var" (CC-ForeignCallArgs fun arg-list))
         (C-MacroCall "LOAD_C_HANDLE"
                      top "var" stack (CC-Symbol typesymbol))
         (C-blockend))

        (ptr
         (let* ((ptr-type (second old-type)))
           (multiple-value-bind (key old-ptr-name old-ptr-type)
               (select-type ptr-type ptr-type ptr-type)
             (declare (ignore old-ptr-type old-ptr-name))
             (case key
               (old-struct
                (C-blockstart)
                (C-VarDeclInit 
                 "void *" "ptr" 
                 (CC-Cast "void *" (CC-ForeignCallArgs fun arg-list)))
                (C-MacroCall "LOAD_C_STRUCT"
                             top "ptr" stack (CC-Symbol typesymbol))
                (C-Blockend))

               (old-union
                (C-blockstart)
                (C-VarDeclInit 
                 "void *" "ptr" 
                 (CC-Cast "void *" (CC-ForeignCallArgs fun arg-list)))
                (C-MacroCall "LOAD_C_UNION"
                             top "ptr" stack (CC-Symbol typesymbol))
                (C-Blockend))

               (old-array
                (C-blockstart)
                (C-VarDeclInit 
                 "void *" "ptr" 
                 (CC-Cast "void *" (CC-ForeignCallArgs fun arg-list)))
                (C-MacroCall "LOAD_C_ARRAY"
                             top "ptr" stack (CC-Symbol typesymbol))
                (C-Blockend))))))

        (lisp
         (C-blockstart)
         (cond 
           ((eq return-type :integer)
            (C-VarDeclInit 
             "long" "var" 
             (CC-Cast "long" (CC-ForeignCallArgs fun arg-list)))
            (C-MacroCall "LOAD_FIXNUM" top "var" stack))

           ((eq return-type :short)
            (C-VarDeclInit 
             "short" "var" 
             (CC-Cast "short" (CC-ForeignCallArgs fun arg-list)))
            (C-MacroCall "LOAD_FIXNUM" top "var" stack))

           ((eq return-type :int)
            (C-VarDeclInit 
             "int" "var" 
             (CC-Cast "int" (CC-ForeignCallArgs fun arg-list)))
            (C-MacroCall "LOAD_FIXNUM" top "var" stack))

           ((eq return-type :long)
            (C-VarDeclInit 
             "long" "var" 
             (CC-Cast "long" (CC-ForeignCallArgs fun arg-list)))
            (C-MacroCall "LOAD_FIXNUM" top "var" stack))

           ((eq return-type :character)
            (C-VarDeclInit 
             "char" "var" 
             (CC-Cast "char" (CC-ForeignCallArgs fun arg-list)))
            (C-MacroCall "LOAD_CHARACTER" top "var" stack))

           ((eq return-type :float)
            (C-VarDeclInit 
             "double" "var" 
             (CC-Cast "double" (CC-ForeignCallArgs fun arg-list)))
            (C-MacroCall "LOAD_FLOAT" top "var" stack))
           )
         (C-blockend))

        (otherwise
         (internal-error 'C-ForeignCall "FATAL ERROR: unknown Pointer-type: ~A"
                         return-type))))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun CC-ForeignCallArgs (fun arg-list)
  (apply #'CC-Call (?name fun) arg-list))

;;******************************************************************************
;; Hilfsfunktionen
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Fehlermeldung
;;------------------------------------------------------------------------------
(defun cg-ff-error (where type)
  (internal-error 
   where "FATAL ERROR: Unknown Type (~A)" type))
  
;;******************************************************************************
;; Routinen zur Erzeugung von Funktionsargumenten
;;******************************************************************************
;;------------------------------------------------------------------------------
;; Fuer Strings
;;------------------------------------------------------------------------------
(defun cg-ff-string (arg queue)
  (let* ((*result-spec* (stacktop-result-location))
         (stack (CC-Stack *stack-top*)))
    (typecase arg
      (var-ref 
       (add-q (CC-MacroCall "GET_C_STRING" (cc-dest (?var arg))) queue))

      (otherwise
       (cg-form arg)
       (add-q (CC-MacroCall "GET_C_STRING" stack) queue)
       (incf *stack-top*)
       (incf (?offset *result-spec*))))))

;;------------------------------------------------------------------------------
;; Primitive Typen
;;------------------------------------------------------------------------------
(defun cg-ff-primitive (type arg queue)
  (let* ((*result-spec* (stacktop-result-location))
         (stack (CC-Stack *stack-top*))
         (macro (format nil "GET_~A" (c-macro-string type))))
    (typecase arg
      (var-ref 
       (add-q (CC-MacroCall macro (cc-dest (?var arg))) queue))
      
      (otherwise
       (cg-form arg)
       (add-q (CC-MacroCall macro stack) queue)
       (incf *stack-top*)
       (incf (?offset *result-spec*))))))

;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun cg-ff-handle-old (arg queue)
  (let* ((*result-spec* (stacktop-result-location))
         (stack (CC-Stack *stack-top*)))

    (typecase arg
      (var-ref 
       (add-q (CC-MacroCall "GET_C_HANDLE" (cc-dest (?var arg))) queue))
      
      (otherwise
       (cg-form arg)
       (add-q (CC-MacroCall "GET_C_HANDLE" stack) queue)
       (incf *stack-top*)
       (incf (?offset *result-spec*))))))
    
;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun cg-ff-struct-old (type arg queue)
  (let* ((*result-spec* (stacktop-result-location))
         (stack (CC-Stack *stack-top*))
         (c-type (convert-c-type-to-string type)))

    (typecase arg
      (var-ref 
       (add-q 
        (format nil "*((~A *)~A)"
                c-type
                (CC-MacroCall "GET_C_STRUCT_PTR" (cc-dest (?var arg))))
        queue))
      
      (otherwise
       (cg-form arg)
       (add-q 
        (format nil "*((~A *)~A)"
                c-type
                (CC-MacroCall "GET_C_STRUCT_PTR" stack))
        queue)
       (incf *stack-top*)
       (incf (?offset *result-spec*))))))
  
;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun cg-ff-union-old (type arg queue)
  (let* ((*result-spec* (stacktop-result-location))
         (stack (CC-Stack *stack-top*))
         (c-type (convert-c-type-to-string type)))

    (typecase arg
      (var-ref 
       (add-q 
        (format nil "*((~A *)~A)"
                c-type
                (CC-MacroCall "GET_C_UNION_PTR" (cc-dest (?var arg))))
        queue))
      
      (otherwise
       (cg-form arg)
       (add-q 
        (format nil "*((~A *)~A)"
                c-type
                (CC-MacroCall "GET_C_UNION_PTR" stack))
        queue)
       (incf *stack-top*)
       (incf (?offset *result-spec*))))))
  
;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun cg-ff-array-old (type arg queue)
  (let* ((*result-spec* (stacktop-result-location))
         (stack (CC-Stack *stack-top*))
         (c-type (convert-c-type-to-string type)))

    (typecase arg
      (var-ref 
       (add-q 
        (format nil "*((~A *)~A)"
                c-type
                (CC-MacroCall "GET_C_ARRAY_PTR" (cc-dest (?var arg))))
        queue))
      
      (otherwise
       (cg-form arg)
       (add-q 
        (format nil "*((~A *)~A)"
                c-type
                (CC-MacroCall "GET_C_ARRAY_PTR" stack))
        queue)
       (incf *stack-top*)
       (incf (?offset *result-spec*))))))
  
;;------------------------------------------------------------------------------
;; 
;;------------------------------------------------------------------------------
(defun cg-ff-ptr (type arg queue)
  (let* ((*result-spec* (stacktop-result-location))
         (stack (CC-Stack *stack-top*))
         (c-type (convert-c-type-to-string type)))

    (typecase arg
      (var-ref
       (multiple-value-bind (key old-name old-type)
           (select-type (second type) (second type) (second type))
         (declare (ignore old-name old-type))
         (case key
           (old-struct
            (add-q
             (CC-Cast c-type
                      (CC-MacroCall "GET_C_STRUCT_PTR" (cc-dest (?var arg))))
             queue))
           
           (old-union
            (add-q 
             (CC-Cast c-type
                      (CC-MacroCall "GET_C_UNION_PTR" (cc-dest (?var arg))))
             queue))
           
           (old-array
            (add-q 
             (CC-Cast
              c-type
              (CC-MacroCall "GET_C_ARRAY_PTR" (cc-dest (?var arg))))
             queue))
           
           (otherwise
            (internal-error 'cg-ff-ptr
                            "Illegal Pointer Type: ~A" type)))))
      
      (otherwise
       (cg-form arg)
       (multiple-value-bind (key old-name old-type)
           (select-type (second type) (second type) (second type))
         (declare (ignore old-name old-type))
         (case key
           (old-struct
            (add-q 
             (CC-Cast
              c-type
              (CC-MacroCall "GET_C_STRUCT_PTR" stack))
             queue))
           
           (old-union
            (add-q 
             (CC-Cast
              c-type
              (CC-MacroCall "GET_C_UNION_PTR" stack))
             queue))
           
           (old-array
            (add-q
             (CC-Cast
              c-type
              (CC-MacroCall "GET_C_ARRAY_PTR" stack))
             queue))
           
           (otherwise
            (internal-error 'cg-ff-ptr
                            "Illegal Pointer Type: ~A" type))))
       (incf *stack-top*)
       (incf (?offset *result-spec*))))))

;;******************************************************************************
;; Call-In
;;******************************************************************************
;;------------------------------------------------------------------------------
;; cg-call-in-interface 
;;------------------------------------------------------------------------------
(defun cg-call-in-interface (fun)
  #+CMU17(declare (notinline C-blockend)) ; by-pass a bug
  (let* ((fun-name (?foreign-name (?call-in fun)))
         (gen-fun-name (?adr fun))
         (return-type (?return-type (?call-in fun)))
         (arguments (?arguments (?call-in fun))))
    
    (labels
        ((make-type ()
           (C-Out (convert-c-type-to-string return-type) " "))
         
         (make-fun-name ()
           (C-Out fun-name))
         
         (make-ansi-or-kr-params ()
           (let* ((laenge (length arguments)))
             (when (> laenge 0)
               (cond
                 (*ANSI-C*
                  (dotimes (i (- laenge 1))
                    (C-Out (convert-c-type-to-string (nth i arguments))
                           (format nil " arg~A, " i)))
                  (C-Out (convert-c-type-to-string (car (last arguments)))
                         (format nil " arg~A" (- laenge 1))))
                 (t
                  (dotimes (i (- laenge 1))
                    (C-Out (format nil "arg~A, " i)))
                  (C-Out (format nil "arg~A" (- laenge 1))))))))
         
         (make-kr-spec ()
           (unless *ANSI-C*
             (let* ((laenge (length arguments)))
               (when (> laenge 0)
                 (dotimes (i laenge)
                   (C-Ln (convert-c-type-to-string (nth i arguments))
                         (format nil " arg~A;" i)))))))
         
         (make-args ()
           (let* ((count 0))
             (dolist (arg arguments)
               (make-one-arg arg (CC-Stack count) count)
               (incf count))))
         
         (make-one-arg (type stack count)
           (multiple-value-bind (key old-name old-type)
               (select-type type type type)
             (case key
               (string
                (C-MacroCall "LOAD_C_STRING"
                             stack
                             (format nil "arg~A" count)
                             stack))
               
               (primitive
                (C-MacroCall 
                 (concatenate 'string "LOAD_" (c-macro-string type)) 
                 stack
                 (format nil "arg~A" count)
                 stack))
               
               (void
                ;; Do nothing!
                ;;------------
                )
               
               (old-struct
                (let* ((typesymbol old-name))
                  (C-MacroCall "LOAD_C_STRUCT"
                               stack 
                               (format nil "(void *)&arg~A" count)
                               stack 
                               (CC-Symbol typesymbol))))
               
               (old-union
                (let* ((typesymbol old-name))
                  (C-MacroCall "LOAD_C_UNION"
                               stack 
                               (format nil "(void *)&arg~A" count)
                               stack 
                               (CC-Symbol typesymbol))))
               
               (old-array
                (let* ((typesymbol old-name))
                  (C-MacroCall "LOAD_C_ARRAY"
                               stack 
                               (format nil "(void *)&arg~A" count)
                               stack 
                               (CC-Symbol typesymbol))))
               
               (old-handle
                (let* ((typesymbol old-name))
                  (C-MacroCall "LOAD_C_HANDLE"
                               stack 
                               (format nil "(void *)arg~A" count)
                               stack 
                               (CC-Symbol typesymbol))))
               
               (lisp
                (cond
                  ((eq type :integer)
                   (C-MacroCall "LOAD_FIXNUM" 
                                stack 
                                (format nil "arg~A" count)
                                stack))
                  ((eq type :short)
                   (C-MacroCall "LOAD_FIXNUM" 
                                stack 
                                (format nil "arg~A" count)
                                stack))
                  ((eq type :int)
                   (C-MacroCall "LOAD_FIXNUM" 
                                stack 
                                (format nil "arg~A" count)
                                stack))
                  ((eq type :long)
                   (C-MacroCall "LOAD_FIXNUM" 
                                stack 
                                (format nil "arg~A" count)
                                stack))
                  ((eq type :character)
                   (C-MacroCall "LOAD_CHARACTER" 
                                stack 
                                (format nil "arg~A" count)
                                stack))
                  ((eq type :float)
                   (C-MacroCall "LOAD_FLOAT" 
                                stack 
                                (format nil "arg~A" count)
                                stack))))
               
               (ptr
                (let* ((ptr-type (second old-type)))
                  (multiple-value-bind (key old-ptr-name old-ptr-type)
                      (select-type ptr-type ptr-type ptr-type)
                    (declare (ignore old-ptr-type))
                    (let* ((typesymbol old-ptr-name))
                      (case key
                        (old-struct
                         (C-MacroCall "LOAD_C_STRUCT"
                                      stack
                                      (format nil "(void *)arg~A" count)
                                      stack
                                      (CC-Symbol typesymbol)))
                        (old-union
                         (C-MacroCall "LOAD_C_UNION"
                                      stack
                                      (format nil "(void *)arg~A" count)
                                      stack
                                      (CC-Symbol typesymbol)))

                        (old-array
                         (C-MacroCall "LOAD_C_ARRAY"
                                      stack
                                      (format nil "(void *)arg~A" count)
                                      stack
                                      (CC-Symbol typesymbol)))))))))))
         
         (make-call ()
           (C-Lispcall gen-fun-name (CC-Stack 0) nil))
         
         (make-return-value (type)
           (multiple-value-bind (key old-name old-type)
               (select-type type type type)
             (case key
               (primitive
                ;; Wandle Das Ergebnis in ein C-Datum um.
                (let* ((fun (cdr (get-operator-def old-name))))
                  (C-Call (?adr fun) (CC-Stack 0)))

                (C-Call
                 "return"                
                 (CC-MacroCall 
                  (concatenate 'string "GET_" (c-macro-string type))
                  (CC-Stack 0))))
               
               (string
                (C-Call "return" (CC-MacroCall "GET_C_STRING" (CC-Stack 0))))
               
               (old-struct
                (let* ((c-type (convert-c-type-to-string type)))
                  (C-Call 
                   "return" 
                   (format nil "*((~A *)~A)"
                           c-type
                           (CC-MacroCall "GET_C_STRUCT_PTR" (CC-Stack 0))))))
               
               (old-union
                (let* ((c-type (convert-c-type-to-string type)))
                  (C-Call 
                   "return" 
                   (format nil "*((~A *)~A)"
                           c-type
                           (CC-MacroCall "GET_C_UNION_PTR" (CC-Stack 0))))))
               
               (old-array
                (let* ((c-type (convert-c-type-to-string type)))
                  (C-Call 
                   "return" 
                   (format nil "*((~A *)~A)"
                           c-type
                           (CC-MacroCall "GET_C_ARRAY_PTR" (CC-Stack 0))))))
               
               (old-handle
                (C-Call "return" (CC-MacroCall "GET_C_HANDLE" (CC-Stack 0))))
               
               (ptr
                (let* ((ptr-type (second old-type)))
                  (multiple-value-bind (key old-ptr-name old-ptr-type)
                      (select-type ptr-type ptr-type ptr-type)
                    (declare (ignore old-ptr-name old-ptr-type))
                    (case key
                      (old-struct
                       (C-Call "return" 
                               (CC-MacroCall "GET_C_STRUCT_PTR" (CC-Stack 0))))
                      
                      (old-union
                       (C-Call "return"
                               (CC-MacroCall "GET_C_UNION_PTR" (CC-Stack 0))))
                      
                      (old-array
                       (C-Call "return" 
                               (CC-MacroCall "GET_C_ARRAY_PTR"
                                             (CC-Stack 0))))))))
               
               (void
                ;; do nothing!
                ;;------------
                )))))
      
      (C-newline)
      (make-type)
      (make-fun-name)
      (C-Out "(")
      (make-ansi-or-kr-params)
      (C-Out ")")
      (C-newline)
      (make-kr-spec)
      (C-blockstart)
      (C-VarDeclInit "CL_FORM *" "base" "save_stack")
      (C-Newline)
      (make-args)
      (make-call)
      (C-Assign "save_stack" "base")
      (make-return-value return-type)
      (C-blockend))))

;;------------------------------------------------------------------------------
(provide "cgforeign")
