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
;;; Function : Data structures for storing side effect information
;;; $Revision: 1.5 $
;;; $Id: se-init.lisp,v 1.5 1994/11/22 14:49:16 hk Exp $
;;;-------------------------------------------------------------------------

(in-package "CLICC")

;;--------------------------------------------------------------------------
;; Bei der Analyse der ZWS-Formen werden die gelesenen Variablen bzw
;; die vera"nderten Variablen in den read-list-Slot bzw in den 
;; write-list-Slot eingetragen.
;;--------------------------------------------------------------------------
(defclass1 effect  ()
  (read-list   :initform nil :type (or integer list ))
  (write-list  :initform nil :type (or integer list ))
  (data-effects :initform nil :type (member nil :alloc :dest :jump
                                            :alloc-jump :alloc-dest :dest-jump
                                            :alloc-dest-jump)))

  
;;-------------------------------------------------------------------------
;; Diese Strukture dient zum Abspeichern der textuell sichtbaren 
;; Effekte durch ein SETQ oder Referenzen von Variablen.
;;-------------------------------------------------------------------------
(defclass1 local-effect ()
  (read-list  :initform nil :type list)
  (write-list :initform nil :type list))

;;-------------------------------------------------------------------------
(provide "se-init")    
