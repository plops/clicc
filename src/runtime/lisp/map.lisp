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
;;; Funktion : MAPCAR, MAPLIST, MAPC, MAPL, MAPCAN, MAPCON
;;;
;;; $Revision: 1.8 $
;;; $Id: map.lisp,v 1.8 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export '(mapcar maplist mapc mapl mapcan mapcon))

;;------------------------------------------------------------------------------
(defun mapc (f list &rest lists)
  (dolist (x list)
    (labels
        ((get-rest-args (lists)
           (cond
             ((atom lists) nil)
             (T
              ;; eine der Rest-Listen ist vollstaendig abgearbeitet
              ;;---------------------------------------------------
              (when (atom (car lists))
                (return))
              (cons (pop (car lists)) 
                    (get-rest-args (cdr lists)))))))

      (apply f x (get-rest-args lists))))
  list)

;;------------------------------------------------------------------------------
(defun mapl (f list &rest lists)
  (loop
    (labels
        ((get-rest-args (lists)
           (cond
             ((atom lists) nil)
             (T
              ;; eine der Rest-Listen ist vollstaendig abgearbeitet
              ;;---------------------------------------------------
              (when (atom (car lists))
                (return))
              (cons (prog1 (car lists) (pop (car lists)))
                    (get-rest-args (cdr lists)))))))
      (if (atom list)
          (return)
          (apply f list (get-rest-args lists)))
      (pop list)))
  list)

;;------------------------------------------------------------------------------
(defun mapcar (f list &rest lists)
  (declare (:simp-when-no-result mapc))
  (labels
      ((mapcar-internal ()
         (labels
             ((get-rest-args (lists)
                (cond
                  ((atom lists) nil)
                  (T
                   ;; eine der Rest-Listen ist vollstaendig abgearbeitet
                   ;;---------------------------------------------------
                   (when (atom (car lists))
                     (return-from mapcar-internal nil))
                   (cons (pop (car lists)) 
                         (get-rest-args (cdr lists)))))))

           (if (atom list)

               ;; die erste Liste ist vollstaendig abgearbeitet
               ;;----------------------------------------------
               nil

               (cons (apply f (pop list) (get-rest-args lists))
                     (mapcar-internal))))))

    (mapcar-internal)))

;;------------------------------------------------------------------------------
(defun maplist (f list &rest lists)
  (declare (:simp-when-no-result mapl))
  (labels
      ((maplist-internal ()
         (labels
             ((get-rest-args (lists)
                (cond
                  ((atom lists) nil)
                  (T
                   ;; eine der Rest-Listen ist vollstaendig abgearbeitet
                   ;;---------------------------------------------------
                   (when (atom (car lists))
                     (return-from maplist-internal nil))
                   (cons (prog1 list (pop (car lists)))
                         (get-rest-args (cdr lists)))))))

           (if (atom list)
               ;; die erste Liste ist vollstaendig abgearbeitet
               ;;----------------------------------------------
               nil

               (cons (apply f (prog1 list (pop list)) (get-rest-args lists))
                     (maplist-internal))))))

    (maplist-internal)))

;;------------------------------------------------------------------------------
(defun mapcan (f list &rest lists)
  (declare (:simp-when-no-result mapc))
  (labels
      ((mapcan-internal ()
         (labels
             ((get-rest-args (lists)
                (cond
                  ((atom lists) nil)
                  (T
                   ;; eine der Rest-Listen ist vollstaendig abgearbeitet
                   ;;---------------------------------------------------
                   (when (atom (car lists))
                     (return-from mapcan-internal nil))
                   (cons (pop (car lists)) 
                         (get-rest-args (cdr lists)))))))

           (if (atom list)

               ;; die erste Liste ist vollstaendig abgearbeitet
               ;;----------------------------------------------
               nil

               (nconc (apply f (pop list) (get-rest-args lists))
                      (mapcan-internal))))))

    (mapcan-internal)))

;;------------------------------------------------------------------------------
(defun mapcon (f list &rest lists)
  (declare (:simp-when-no-result mapl))
  (labels
      ((mapcon-internal ()
         (labels
             ((get-rest-args (lists)
                (cond
                  ((atom lists) nil)
                  (T
                   ;; eine der Rest-Listen ist vollstaendig abgearbeitet
                   ;;---------------------------------------------------
                   (when (atom (car lists))
                     (return-from mapcon-internal nil))
                   (cons (prog1 list (pop (car lists)))
                         (get-rest-args (cdr lists)))))))

           (if (atom list)

               ;; die erste Liste ist vollstaendig abgearbeitet
               ;;----------------------------------------------
               nil

               (nconc (apply f (prog1 list (pop list)) (get-rest-args lists))
                      (mapcon-internal))))))

    (mapcon-internal)))
