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
;;; Funktion : Konstante benannte Strings
;;;
;;; $Revision: 1.8 $
;;; $Id: strconst.lisp,v 1.8 1994/11/22 14:49:16 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "CLICC")

;;------------------------------------------------------------------------------
;; Konstanten des Programms
;;------------------------------------------------------------------------------

;;------------------------------------------------------------------------------
;; Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant NOT_IMPLEMENTED    "~S not implemented.")
(defconstant TYPE_NOT_IMPL      "Data type for ~S not implemented.")
(defconstant NOT_ALLOWED        "Expression not allowed at this position")

(defconstant NO_OPERATOR        "Unknown operator ~S")
(defconstant ILLEGAL_CALL       "The argument list of a call to ~A does not ~
                                 match the lambda list ~A")
(defconstant ILL_FORMED         "The ~S ~S is not of the form ~S")
(defconstant IFD_BODY           "~S is an ill-formed body.")
(defconstant NOT_ENOUGH_ARGS    "A call to ~A needs at least ~D arguments.")
(defconstant TOO_MUCH_ARGS      "A call to ~A should have at most ~D ~
                                 arguments.")

;;------------------------------------------------------------------------------
;; Fehlermeldungen: Top-Level-Forms
;;------------------------------------------------------------------------------
(defconstant TLF_IGNORED        "Top-level-form ~S ignored.")
(defconstant NO_GLOB_VAL        "~S has no global value")
(defconstant NO_GLOB_FUNDEF     "Function ~30S is undefined.")

;;------------------------------------------------------------------------------
;; Fehlermeldungen: Definition einer Funktion/ eines Makros /einer Klasse
;;------------------------------------------------------------------------------
(defconstant ILLEGAL_L_EXPR     "The argument list of a LAMBDA-EXPRESSION does ~
                                 not match ~
                                 the lambda list ((&REST ARGLIST) &BODY BODY)")
(defconstant NO_NAME            "The name ~S in ~S must be a symbol.")
(defconstant REDEF_SPF          "It is illegal to redefine the special form ~S")
(defconstant REDEF_MACRO        "Redefining function ~S which used ~
                                 to be a macro")
(defconstant REDEF_FUN          "Redefining build-in function ~S")
(defconstant REDEF              "Redefining ~S which was defined before.")
(defconstant REDEF_GEN-FUN      "It is illegal to redefine the generic ~
                                 function ~S")
(defconstant REDEF_CLASS        "It is illegal to redefine the class ~S")
                                 
;;------------------------------------------------------------------------------
;; Fehlermeldungen: Analyse der LAMBDA-Liste
;;------------------------------------------------------------------------------
(defconstant IFD_LAMBDA_LIST    "~S is an ill-formed lambda-list~%")
(defconstant IFD_LL_ELEM        "The lambda list element ~S is ill-formed.~%~
                                 It must be of the form: ~S")
(defconstant ILLEGAL_LLKW       "~S is an illegal lambda-list keyword.")

(defconstant DECLARED_TWICE     "The variable ~S occurs more than once in it.")
(defconstant DECLARED_CONSTANT  "~S was declared with DEFCONSTANT.")
(defconstant IS_KEYWORD         "~S is keyword, no variable.")
(defconstant NO_VAR             "Expected to see a variable instead of ~S")

(defconstant NO_OPTIONALS       "No optionals specified after &OPTIONAL")
(defconstant POS_ERR_OPTIONAL   "&OPTIONAL seen after optionals already ~
                                 processed.")

(defconstant NO_REST_VAR        "No variable follows &REST keyword.")
(defconstant SEC_REST_VAR       "Second variable seen after &REST.")
(defconstant POS_ERR_REST       "&REST seen after rest argument already ~
                                 processed.")

(defconstant NO_KEYS            "No keys specified after &KEY")
(defconstant POS_ERR_KEY        "&KEY seen after key arguments already ~
                                 processed.")

(defconstant POS_ERR_AOK        "&ALLOW-OTHER-KEYS has to immediately follow ~
                                 key arguments.")
(defconstant ILLEGAL_AOKP       "After &ALLOW-OTHER-KEYS no parameter allowed.
                                 Expected to see a keyword instead of ~S")

(defconstant NO_AUX             "No aux-parameter specified after &AUX")

(defconstant SPECIAL_DECL_TWICE "Variable ~S is declared SPECIAL twice")

;;------------------------------------------------------------------------------
;; Fehlermeldungen: Special-Forms
;;------------------------------------------------------------------------------
(defconstant NO_MATCH_SF        "The argument list does not match lambda list ~
                                 ~A of ~A")

(defconstant CONST_VAL          "~S has constant value and cannot be changed ~
                                 to ~A")
(defconstant CLOS_OF_MACRO      "The macro ~S is not a valid argument to ~
                                 FUNCTION.")
(defconstant IFD_BINDING        "The binding ~S is ill-formed.~%~
                                 It must be of the form: (VAR | (VAR VALUE))")
(defconstant NO_LEGAL_BLOCKNAME "~S should be a legal block name")
(defconstant TAG_NOT_USED       "Tag ~S in TAGBODY not used")

;;------------------------------------------------------------------------------
;; Fehlermeldungen: Makro-Expansion
;;------------------------------------------------------------------------------
(defconstant NO_LIST_END        "~S is not a legal list end")
(defconstant NO_LEGAL_CLAUSE    "~S is not a legal clause for a ~S form")
(defconstant ODD_NARGS          "Odd number of args to ~S")
(defconstant NO_GET-SETF-METHOD "~S illegal atomic form for GET-SETF-METHOD")
(defconstant NO_LOC_SPEC        "~S is not a known location specifier for SETF")
(defconstant TOO_FEW_ARGS       "Too few argument forms to ~S")

(defconstant ILLEGAL_KEY        "Expected one of the keywords ~S instead of ~S ~
                                 in the keyword portion of a call")
(defconstant ODD_LEN_KEYLIST    "Unpaired item in keyword portion of an ~
                                 argument list")

;;------------------------------------------------------------------------------
;; Fehlermeldungen: System-Funktionen
;;------------------------------------------------------------------------------
(defconstant NARGS_NC           "~S cannot be called with ~D arguments")
(defconstant NO_SYMBOL          "~S should be of type SYMBOL")
(defconstant NO_KEYWORD         "~S should be of type KEYWORD")
(defconstant NO_LIST            "~S should be of type LIST")
(defconstant NO_STRING          "~S should be of type STRING")

(defconstant NOT_A_SYMBOL       "The value of ~A, ~S, should be a symbol.")
(defconstant IFD_DECL_SPEC      "~S is an ill-formed declaration specification")

;;------------------------------------------------------------------------------
(provide "strconst")
