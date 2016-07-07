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
;;; Function  : Sequences
;;;
;;; $Revision: 1.41 $
;;; $Id: seq.lisp,v 1.41 1994/11/22 14:55:56 hk Exp $
;;;-----------------------------------------------------------------------------

(in-package "LISP")

(export
 '(complement elt subseq copy-seq length reverse nreverse make-sequence
   concatenate map some every notany notevery reduce fill replace remove
   remove-if remove-if-not delete delete-if delete-if-not remove-duplicates
   delete-duplicates substitute substitute-if substitute-if-not nsubstitute
   nsubstitute-if nsubstitute-if-not find find-if find-if-not position
   position-if position-if-not count count-if count-if-not mismatch search
   sort))

;;------------------------------------------------------------------------------
;; Fuer Fehlermeldungen
;;------------------------------------------------------------------------------
(defconstant NOT_IN_RANGE
  "The value ~S is not an integer in the interval [0,~S]")

;;------------------------------------------------------------------------------
(defun sequence-type (sequence)
  (cond
    ((listp   sequence) 'list)
    ((vectorp sequence) `(vector ,(array-element-type sequence)))))

;;------------------------------------------------------------------------------
(defmacro check-seq-test (test test-not)
  `(cond
    (,test ,test)
    (,test-not #'(lambda (&rest arguments)
                   (not (apply ,test-not arguments))))
    (t #'eql)))

;;------------------------------------------------------------------------------
(defmacro check-seq-end (end length)
  `(let ((l ,length))
    (when (> ,end l) (setq ,end l))))

;;-----------------------------------------------------------------------------
;; APPLY-KEY saves us a function call sometimes.
;;-----------------------------------------------------------------------------
(defmacro apply-key (key element)
  `(if ,key
       (funcall ,key ,element)
       ,element))

;;-----------------------------------------------------------------------------
;; Seq-Dispatch does an efficient type-dispatch on the given Sequence.
;;-----------------------------------------------------------------------------
(defmacro seq-dispatch (sequence list-form array-form)
  `(if (listp ,sequence)
       ,list-form
       ,array-form))

;;------------------------------------------------------------------------------
;; Abstrakter Datentyp Queue
;; Eine Cons-Zelle enthaelt im Car eine Liste und im Cdr den letzten Cons-Knoten
;; dieser Liste.
;; Eine leere Queue wird durch einen Cons-Knoten, der im Car und Cdr Nil
;; enthaelt, dargestellt. 
;;------------------------------------------------------------------------------

(defun empty-queue ()
  (cons nil nil))

(defun empty-queue-p (q)
  (null (car q)))

(defun list2queue (l)
  (cons l (last l)))                   ; (last ()) = ()

(defun queue2list (q)
  (car q))

(defun add-q (e q)
  (let ((new-cons (cons e nil)))
    (if (null (cdr q))
        (rplaca q new-cons)
        (rplacd (cdr q) new-cons))
    (rplacd q new-cons)))

(defun addnew-q (e q)
  (unless (member e (queue2list q))
    (add-q e q)))

;;------------------------------------------------------------------------------
;; Letztes Element der Queue
;;------------------------------------------------------------------------------
(defun last-q (q)
  (cadr q))

(defun set-last-q (e q)
  (rplaca (cdr q) e))

;;-----------------------------------------------------------------------------
;; COMPLEMENT fn                                              [CL2]
;;-----------------------------------------------------------------------------
(defun complement (fn)
  #'(lambda (&rest arguments)
      (not (apply fn arguments))))

;;-----------------------------------------------------------------------------
;; 14.1. Simple Sequence Functions                                            
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; ELT sequence index
;;-----------------------------------------------------------------------------
(defun elt (sequence index)
  (seq-dispatch
   sequence
   (nth index sequence)
   (row-major-aref sequence index)))

;;-----------------------------------------------------------------------------
;; (setf ELT) newvalue sequence index
;;-----------------------------------------------------------------------------
(defun (setf elt) (newvalue sequence index)
  (seq-dispatch
   sequence
   (setf (nth index sequence) newvalue)
   (setf (row-major-aref sequence index) newvalue)))

;;-----------------------------------------------------------------------------
;; SUBSEQ sequence start &OPTIONAL end
;;-----------------------------------------------------------------------------
(defun subseq (sequence start &optional (end most-positive-fixnum))
  (seq-dispatch
   sequence
   (list-subseq sequence start end)
   (vector-subseq sequence start end)))

;;-----------------------------------------------------------------------------
(defun vector-subseq (vector start end)
  (let ((length (length vector)))
    (check-seq-end end length)
    (let ((subseq (make-array (- end start)
                              :element-type (array-element-type vector))))
      (do ((i start (1+ i))
           (j 0     (1+ j)))
          ((= i end) subseq)
        (setf (elt subseq j) (elt vector i))))))

;;-----------------------------------------------------------------------------
(defun list-subseq (list start end)
  (let ((subseq (empty-queue)))
    (setq list (nthcdr start list))
    (decf end start)
    (dotimes (i end)
      (when (endp list) (return))
      (add-q (pop list) subseq))
    (queue2list subseq)))

;;------------------------------------------------------------------------------
;; (SETF SUBSEQ)
;;------------------------------------------------------------------------------
(defsetf SUBSEQ (sequence start &optional end) (new-sequence)
  `(PROGN (REPLACE ,sequence ,new-sequence :START1 ,start :END1 ,end)
    ,new-sequence))

;;-----------------------------------------------------------------------------
;; COPY-SEQ sequence
;;-----------------------------------------------------------------------------
(defun copy-seq (sequence)
  (subseq sequence 0))

;;------------------------------------------------------------------------------
;; length sequence
;;------------------------------------------------------------------------------
(defun length (sequence)
  (declare (:my-last-arg-may-be-rest-var :length))
  (seq-dispatch
   sequence
   (let ((n 0)) (dolist (s sequence) (incf n)) n)
   (vector-length sequence)))

;;-----------------------------------------------------------------------------
;; REVERSE sequence
;;-----------------------------------------------------------------------------
(defun reverse (sequence)
  (seq-dispatch
   sequence
   (list-reverse sequence)
   (vector-reverse sequence)))

;;-----------------------------------------------------------------------------
(defun vector-reverse (vector)
  (let* ((length (length vector))
         (reverse (make-array length
                              :element-type (array-element-type vector))))
    (do ((i 0 (1+ i))
         (j (1- length) (1- j)))
        ((= i length) reverse)
      (setf (elt reverse j) (elt vector i)))))

;;-----------------------------------------------------------------------------
(defun list-reverse (list)
  (let ((reverse nil))
    (dolist (elem list)
      (setf reverse (cons elem reverse)))
    reverse))

;;-----------------------------------------------------------------------------
;; NREVERSE sequence
;;-----------------------------------------------------------------------------
(defun nreverse (sequence)
  (typecase sequence
    (null nil)
    (cons (list-nreverse sequence))
    (T (vector-nreverse sequence))))

;;-----------------------------------------------------------------------------
(defun vector-nreverse (vector)
  (do ((i 0 (1+ i))
       (j (1- (length vector)) (1- j))
       seq-elem-i)
      ((>= i j) vector)
    (setq seq-elem-i (elt vector i))
    (setf (elt vector i) (elt vector j))
    (setf (elt vector j) seq-elem-i)))

;;-----------------------------------------------------------------------------
(defun list-nreverse (list)
  (labels ((doit (head tail)
	     (let ((rest (cdr tail)))
	       (setf (cdr tail) head)
	       (if (null rest)
		   tail
		   (doit tail rest)))))
    (doit nil list)))

;;-----------------------------------------------------------------------------
;; MAKE-SEQUENCE type size &KEY :initial-element
;;-----------------------------------------------------------------------------
;; type ::= LIST |
;;          (SIMPLE-STRING size)              | (STRING size) |
;;          (SIMPLE-VECTOR element-type size) | (VECTOR element-type size)
;;------------------------------------------------------------------------------
(defun make-sequence (type size &key (initial-element nil init-elem-sp))
  (let ((result-type (normalize-type type))
        size-of-type)
    (tagbody
       (cond
         ((atom result-type)
          (if (eq result-type 'list)
            (return-from make-sequence
              (if init-elem-sp
                (make-list size :initial-element initial-element)
                (make-list size)))
            (go INVALID-SEQTYPE)))
         (t (case (car result-type)
              ((simple-array array)
               (let ((element-type (second result-type))
                     (dimensions   (third  result-type)))
                 (when (rest dimensions) (go INVALID-SEQTYPE))
                 (setq size-of-type (first dimensions))
                 (when (and (not (eq size-of-type '*))
                            (/= size size-of-type))
                   (go INCONSISTENT-SIZE))
                 (return-from make-sequence
                   (if (eq element-type 'character)
;;;                    (eq element-type 'string-char)
                     (if init-elem-sp
                       (make-string size :initial-element initial-element)
                       (make-string size))
                     (apply #'make-array size
                            (append
                             (if init-elem-sp
                               `(:initial-element ,initial-element)
                               ())
                             (if (eq element-type '*)
                               ()
                               `(:element-type ,element-type))))))))
              (t (go INVALID-SEQTYPE)))))
     INVALID-SEQTYPE
       (error "~S is an invalid sequence typespecifier." type)
     INCONSISTENT-SIZE
       (error "The size ~S is inconsistent with the specified size ~S in type."
              size size-of-type))))

;;-----------------------------------------------------------------------------
;; 14.2. Concatenating, Mapping, and Reducing Sequences                       
;;-----------------------------------------------------------------------------

;;-----------------------------------------------------------------------------
;; CONCATENATE result-type &REST sequences
;;-----------------------------------------------------------------------------
(defun concatenate (result-type &rest sequences)
  (if (eq result-type 'list)
    (apply #'concatenate-to-list sequences)
    (apply #'concatenate-to-non-list result-type sequences)))

;;-----------------------------------------------------------------------------
(defun concatenate-to-non-list (result-type &rest sequences)
  (let (new-sequence
        (size  0)
        (index 0))
    (dolist (sequence sequences)
      (incf size (length sequence)))
    (setq new-sequence (make-sequence result-type size))
    (dolist (sequence sequences)
      (seq-dispatch sequence
       (dolist (elem sequence)
         (setf (elt new-sequence index) elem)
         (incf index))
       (dotimes (i (length sequence))
         (setf (elt new-sequence index) (elt sequence i))
         (incf index))))
    new-sequence))

;;-----------------------------------------------------------------------------
(defun concatenate-to-list (&rest sequences)
  (let ((new-sequence (empty-queue)))
    (dolist (sequence sequences)
      (seq-dispatch sequence
       (dolist (elem sequence)
         (add-q elem new-sequence))
       (dotimes (i (length sequence))
         (add-q (elt sequence i) new-sequence))))
    (queue2list new-sequence)))

;;------------------------------------------------------------------------------
;; MAP result-type function sequence &REST more-sequences
;;------------------------------------------------------------------------------
(defun map (result-type function sequence &rest more-sequences)
  (setq more-sequences (cons sequence more-sequences))
  (let ((l (apply #'min (mapcar #'length more-sequences)))
        (i 0))
    (labels ((get-elem (rest-sequences)
               (seq-dispatch (car rest-sequences)
                             (pop (car rest-sequences))
                             (elt (car rest-sequences) i))))
      (cond
        ((null result-type)
         (loop
          (when (>= i l) (return nil))
          (apply function (maplist #'get-elem more-sequences))
          (incf i)))
        ((eq 'list result-type)
         (let ((x (empty-queue)))
           (loop
            (when (>= i l) (return (queue2list x)))
            (add-q (apply function (maplist #'get-elem more-sequences)) x)
            (incf i))))
        (T
         (let ((x (make-sequence result-type l)))
           (loop
            (when (>= i l) (return x))
            (setf (elt x i) (apply function
                                   (maplist #'get-elem more-sequences)))
            (incf i))))))))



;;------------------------------------------------------------------------------
;; SOME predicate sequence &REST more-sequences
;;------------------------------------------------------------------------------
(defun some (predicate sequence &rest more-sequences)
  (setq more-sequences (cons sequence more-sequences))
  (let ((l (apply #'min (mapcar #'length more-sequences)))
        (i 0))
    (labels ((get-elem (rest-sequences)
               (seq-dispatch (car rest-sequences)
                             (pop (car rest-sequences))
                             (elt (car rest-sequences) i))))
      (loop
       (when (>= i l) (return NIL))
       (let ((that-value
              (apply predicate (maplist #'get-elem more-sequences))))
         (when that-value (return that-value)))
       (incf i)))))

;;------------------------------------------------------------------------------
;; EVERY predicate sequence &REST more-sequences
;;------------------------------------------------------------------------------
(defun every (predicate sequence &rest more-sequences)
  (setq more-sequences (cons sequence more-sequences))
  (let ((l (apply #'min (mapcar #'length more-sequences)))
        (i 0))
    (labels ((get-elem (rest-sequences)
               (seq-dispatch (car rest-sequences)
                             (pop (car rest-sequences))
                             (elt (car rest-sequences) i))))
      (loop
       (when (>= i l) (return T))
       (unless (apply predicate (maplist #'get-elem more-sequences))
         (return NIL))
       (incf i)))))

;;------------------------------------------------------------------------------
;; NOTANY predicate sequence &REST more-sequences
;;------------------------------------------------------------------------------
(defun notany (predicate sequence &rest more-sequences)
  (not (apply #'some predicate sequence more-sequences)))

;;------------------------------------------------------------------------------
;; NOTEVERY predicate sequence &REST more-sequences
;;------------------------------------------------------------------------------
(defun notevery (predicate sequence &rest more-sequences)
  (not (apply #'every predicate sequence more-sequences)))

;;-----------------------------------------------------------------------------
;; REDUCE function sequence &KEY :from-end :start :end :initial-value
;;-----------------------------------------------------------------------------
(defun reduce (function sequence &key
                        from-end (start 0) (end most-positive-fixnum)
                        (initial-value nil init-val-sp))
  (let ((length (length sequence)))
    (check-seq-end end length)
    (cond
      ((zerop length) (if init-val-sp
                          initial-value
                          (funcall function)))
      ((and (= length 1) (not init-val-sp)) (elt sequence 0))
      ((not from-end)
       (when (not init-val-sp)
         (setq initial-value (elt sequence start))
         (incf start))
       (do ((result initial-value))
           ((>= start end) result)
         (setq result (funcall function result (elt sequence start)))
         (incf start)))
      (t (decf end)
         (when (not init-val-sp)
           (setq initial-value (elt sequence end))
           (decf end))
         (do ((result initial-value))
             ((>= start end) result)
           (setq result (funcall function (elt sequence end) result))
           (decf end))))))

;;-----------------------------------------------------------------------------
;; FILL sequence item &KEY :start :end
;;-----------------------------------------------------------------------------
(defun fill (sequence item &key (start 0) (end most-positive-fixnum))
  (let ((length (length sequence)))
    (check-seq-end end length)
    (do ((i start (1+ i)))
        ((>= i end) sequence)
      (setf (elt sequence i) item))))

;;-----------------------------------------------------------------------------
;; REPLACE sequence1 sequence2 &KEY :start1 :end1 :start2 :end2
;;-----------------------------------------------------------------------------
(defun replace (sequence1 sequence2 &key
                          (start1 0) (end1 most-positive-fixnum)
                          (start2 0) (end2 most-positive-fixnum))
  (check-seq-end end1 (length sequence1))
  (check-seq-end end2 (length sequence2))
  (let ((length (min (- end1 start1) (- end2 start2))))
    (if (and (eq sequence1 sequence2)
             (> start1 start2))
      (do ((i 0 (1+ i))
           (i1 (+ start1 (1- length)) (1- i1))
           (i2 (+ start2 (1- length)) (1- i2)))
          ((>= i length))
        (setf (elt sequence1 i1) (elt sequence2 i2)))
      (do ((i 0 (1+ i))
           (i1 start1 (1+ i1))
           (i2 start2 (1+ i2)))
          ((>= i length))
        (setf (elt sequence1 i1) (elt sequence2 i2)))))
  sequence1)

;;-----------------------------------------------------------------------------
;; REMOVE item sequence &KEY :from-end :test :test-not :start :end :count :key
;;-----------------------------------------------------------------------------
(defun remove (item sequence &key
                    from-end test test-not (start 0)
                    (end most-positive-fixnum) (count most-positive-fixnum)
                    key)
  (setq test (check-seq-test test test-not))
  (seq-dispatch
   sequence
   (list-remove item sequence from-end test start end count key)
   (vector-remove item sequence from-end test start end count key)))

;;-----------------------------------------------------------------------------
(defun list-remove (item sequence from-end test start end count key)
  (let ((matched-items (empty-queue))
        (num-matched 0)
        (new-sequence (empty-queue)))
    
    (let ((i 0))
      (dolist (elem sequence)
        (when (>= i end) (return))
        (when (and (>= i start) (funcall test item (apply-key key elem)))
          (add-q i matched-items)
          (incf num-matched))
        (incf i)))
    (setq matched-items (queue2list matched-items))
    (if (not from-end)
        (do ((i 0 (1+ i)))
            ((or (endp sequence) (>= i end)))
          (cond
            ((and (plusp count) (eql i (car matched-items)))
             (pop matched-items)
             (decf count)
             (pop sequence))
            (t (add-q (pop sequence) new-sequence))))
        (do ((i 0 (1+ i)))
            ((or (endp sequence) (>= i end)))
          (cond
            ((and (<= num-matched count) (eql i (car matched-items)))
             (pop matched-items)
             (decf num-matched)
             (pop sequence))
            (t (add-q (pop sequence) new-sequence)
               (when (eql i (car matched-items))
                 (pop matched-items)
                 (decf num-matched))))))
    (queue2list new-sequence)))

;;-----------------------------------------------------------------------------
(defun vector-remove (item sequence from-end test start end count key)
  (let ((length (length sequence))
        (matched-items (empty-queue))
        (num-matched 0)
        new-sequence)
    (check-seq-end end length)
    (do ((i start (1+ i)))
        ((>= i end))
      (when (funcall test item (apply-key key (elt sequence i)))
        (add-q i matched-items)
        (incf num-matched)))
    (setq matched-items (queue2list matched-items))
    (setq new-sequence  (make-sequence (sequence-type sequence)
                                       (- length (min num-matched count))))
    (if (not from-end)
        (do ((i 0 (1+ i))
             (j 0))
            ((>= i length))
          (cond
            ((and (plusp count) (eql i (car matched-items)))
             (pop matched-items)
             (decf count))
            (t (setf (elt new-sequence j) (elt sequence i))
               (incf j))))
        (do ((i 0 (1+ i))
             (j 0))
            ((>= i length))
          (cond
            ((and (<= num-matched count) (eql i (car matched-items)))
             (pop matched-items)
             (decf num-matched))
            (t (setf (elt new-sequence j) (elt sequence i))
               (when (eql i (car matched-items))
                 (pop matched-items)
                 (decf num-matched))
               (incf j)))))
    new-sequence))

;;-----------------------------------------------------------------------------
;; REMOVE-IF predicate sequence &KEY :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun remove-if (predicate sequence &key
                            from-end (start 0)
                            (end most-positive-fixnum)
                            (count most-positive-fixnum) key)
  (remove nil sequence
          :test #'(lambda (item seq-elem)
                    (declare (ignore item))
                    (funcall predicate seq-elem))
          :from-end from-end :start start :end end :count count :key key))

;;-----------------------------------------------------------------------------
;; REMOVE-IF-NOT predicate sequence &KEY :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun remove-if-not (predicate sequence &key
                                from-end (start 0)
                                (end most-positive-fixnum)
                                (count most-positive-fixnum) key)
  (remove nil sequence
          :test #'(lambda (item seq-elem)
                    (declare (ignore item))
                    (not (funcall predicate seq-elem)))
          :from-end from-end :start start :end end :count count :key key))
 
;;-----------------------------------------------------------------------------
;; DELETE item sequence &KEY :from-end :test :test-not :start :end :count :key
;;-----------------------------------------------------------------------------
(defun delete (item sequence &key
                    from-end test test-not (start 0) (end most-positive-fixnum)
                    (count most-positive-fixnum) key)
  (typecase sequence
    (null nil)
    (cons
     (list-delete item sequence from-end test test-not start end count key))    
    (T
     (setq test (check-seq-test test test-not))
     (vector-remove item sequence from-end test start end count key))))

;;-----------------------------------------------------------------------------
(defun list-delete (item list from-end test test-not start end count key)
  (cond
    ((or from-end test test-not start end key)
     (setq test (check-seq-test test test-not))
     (list-remove item list from-end test start end count key))
    (t (loop
        (when (or (null list) (zerop count))
          (return-from list-delete list))
        (cond
          ((eql item (car list))
           (setf list (cdr list))
           (decf count))
          (t (return))))
       (let ((l1 list) (l2 (cdr list)))
         (loop
          (when (or (null l2) (zerop count))
            (return-from list-delete list))
          (cond
            ((eql item (car l2))
             (setq l2 (cdr l2))
             (setf (cdr l1) l2)
             (decf count))
            (t (setq l1 l2 l2 (cdr l2)))))))))
      
;;-----------------------------------------------------------------------------
;; DELETE-IF predicate sequence &KEY :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun delete-if (predicate sequence &key
                            from-end (start 0)
                            (end most-positive-fixnum)
                            (count most-positive-fixnum) key)
  (delete nil sequence
          :test #'(lambda (item seq-elem)
                    (declare (ignore item))
                    (funcall predicate seq-elem))
          :from-end from-end :start start :end end :count count :key key))

;;-----------------------------------------------------------------------------
;; DELETE-IF-NOT predicate sequence &KEY :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun delete-if-not (predicate sequence &key
                                from-end (start 0)
                                (end most-positive-fixnum)
                                (count most-positive-fixnum) key)
  (delete nil sequence
          :test #'(lambda (item seq-elem)
                    (declare (ignore item))
                    (not (funcall predicate seq-elem)))
          :from-end from-end :start start :end end :count count :key key))

;;-----------------------------------------------------------------------------
;; REMOVE-DUPLICATES sequence &KEY :from-end :test :test-not :start :end :key
;;-----------------------------------------------------------------------------
(defun remove-duplicates (sequence &key
                                   from-end test test-not (start 0)
                                   (end most-positive-fixnum)
                                   key)
  (setq test (check-seq-test test test-not))
  (seq-dispatch
   sequence
   (list-remove-duplicates sequence from-end test start end key)
   (vector-remove-duplicates sequence from-end test start end key)))

;;-----------------------------------------------------------------------------
;; CMU-CL Code
;; Remove duplicates from a list. If from-end, remove the later duplicates,
;; not the earlier ones. Thus if we check from-end we don't copy an item
;; if we look into the already copied structure (from after :start) and see
;; the item. If we check from beginning we check into the rest of the 
;; original list up to the :end marker (this we have to do by running a
;; do loop down the list that far and using our test.
;;-----------------------------------------------------------------------------
(defun list-remove-duplicates (list from-end test start end key)
  (let* ((result (list ())) ; Put a marker on the beginning to splice with.
	 (splice result)
	 (current list))
    (do ((index 0 (1+ index)))
	((= index start))
      (setq splice (cdr (rplacd splice (list (car current)))))
      (setq current (cdr current)))
    (do ((index 0 (1+ index)))
	((or (endp current) (>= index end)))
      (when (or (and from-end 
                     (not (member (apply-key key (car current))
                                  (nthcdr (1+ start) result)
                                  :test test
                                  :key key)))
                (and (not from-end)
                     (not (do ((it (apply-key key (car current)))
                               (l (cdr current) (cdr l))
                               (i (1+ index) (1+ i)))
                              ((or (endp l) (>= i end))
                               ())
                            (if (funcall test (apply-key key (car l)) it)
                                (return t))))))
	  (setq splice (cdr (rplacd splice (list (car current))))))
      (setq current (cdr current)))
    (do ()
	((atom current))
      (setq splice (cdr (rplacd splice (list (car current)))))
      (setq current (cdr current)))
    (cdr result)))

;;-----------------------------------------------------------------------------
;; CMU-CL Code
;;-----------------------------------------------------------------------------
(defun vector-remove-duplicates (vector from-end test start end key
                                        &aux (length (length vector)))
  (check-seq-end end length)
  (let ((result (make-array length :element-type (array-element-type vector)))
	(index 0)
	(jndex start))
    (do ()
	((= index start))
      (setf (aref result index) (aref vector index))
      (setq index (1+ index)))
    (do ((elt))
	((= index end))
      (setq elt (aref vector index))
      (unless (or (and from-end
		        (position (apply-key key elt) result :start start
			   :end jndex :test test :key key))
		  (and (not from-end)
		        (position (apply-key key elt) vector :start (1+ index)
			   :end end :test test :key key)))
	(setf (aref result jndex) elt)
	(setq jndex (1+ jndex)))
      (setq index (1+ index)))
    (do ()
	((= index length))
      (setf (aref result jndex) (aref vector index))
      (setq index (1+ index))
      (setq jndex (1+ jndex)))
    (rt::shrink-pvector result jndex)))

;;-----------------------------------------------------------------------------
;; DELETE-DUPLICATES sequence &KEY :from-end :test :test-not :start :end :key
;;-----------------------------------------------------------------------------
(defun delete-duplicates (sequence &key
                                   from-end test test-not (start 0)
                                   (end most-positive-fixnum)
                                   key)
  (remove-duplicates sequence
                     :from-end from-end :test test :test-not test-not
                     :start start :end end :key key))

;;------------------------------------------------------------------------------
;; SUBSTITUTE newitem olditem sequence &KEY :from-end :test :test-not :start
;;   :end :count :key
;;------------------------------------------------------------------------------
(defun substitute (new old sequence &key
                       from-end test test-not
                       (start 0) (end most-positive-fixnum)
                       (count most-positive-fixnum) key)
  (setq test (check-seq-test test test-not))
  (let ((length (length sequence)))
    (check-seq-end end length)
    
    (seq-dispatch
     sequence
     (if from-end
         (nreverse (list-substitute new old (reverse sequence) test
                                    (- length end) (- length start)
                                    count key))
         (list-substitute new old sequence test start end count key))
     (if from-end
         (vector-substitute new old sequence  test -1 (1- length)
                            -1 length (1- end) (1- start) count key)
         (vector-substitute new old sequence test 1 0 length length
                            start end count key)))))

;;------------------------------------------------------------------------------
(defun list-substitute (new old list test start end count key)
  (let* ((result (list nil))
	 elt
	 (splice result)
	 (list list))                   ; Get a local list for a stepper.
    (do ((index 0 (1+ index)))
	((= index start))
      (setq splice (cdr (rplacd splice (list (car list)))))
      (setq list (cdr list)))
    (do ((index start (1+ index)))
	((or (= index end) (null list) (= count 0)))
      (setq elt (car list))
      (setq splice
	    (cdr (rplacd splice
                         (list (cond
                                 ((funcall test old (apply-key key elt))
                                  (setq count (1- count))
                                  new)
                                 (t elt))))))
      (setq list (cdr list)))
    (do ()
	((null list))
      (setq splice (cdr (rplacd splice (list (car list)))))
      (setq list (cdr list)))
    (cdr result)))

;;------------------------------------------------------------------------------
(defun vector-substitute (new old sequence  test incrementer left right length
                              start end count key)
  (let ((result (make-array length :element-type (array-element-type sequence)))
	(index left))
    (do ()
	((= index start))
      (setf (aref result index) (aref sequence index))
      (setq index (+ index incrementer)))
    (do ((elt))
	((or (= index end) (= count 0)))
      (setq elt (aref sequence index))
      (setf (aref result index) 
	    (cond ((funcall test old (apply-key key elt))
		   (setq count (1- count))
		   new)
		  (t elt)))
      (setq index (+ index incrementer)))
    (do ()
	((= index right))
      (setf (aref result index) (aref sequence index))
      (setq index (+ index incrementer)))
    result))

;;-----------------------------------------------------------------------------
;; SUBSTITUTE-IF newitem test sequence &KEY :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun substitute-if (new predicate sequence &key
                          from-end (start 0) (end most-positive-fixnum)
                          (count most-positive-fixnum) key)
  (substitute new nil sequence
              :test #'(lambda (item seq-elem)
                        (declare (ignore item))
                        (funcall predicate seq-elem))
              :from-end from-end :start start :end end :count count :key key))

;;-----------------------------------------------------------------------------
;; SUBSTITUTE-IF-NOT newitem test sequence &KEY
;;    :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun substitute-if-not (new predicate sequence &key
                              from-end (start 0) (end most-positive-fixnum)
                              (count most-positive-fixnum) key)
  (substitute new nil sequence
              :test #'(lambda (item seq-elem)
                        (declare (ignore item))
                        (not (funcall predicate seq-elem)))
              :from-end from-end :start start :end end :count count :key key))

;;------------------------------------------------------------------------------
;; NSUBSTITUTE newitem olditem sequence &KEY :from-end :test :test-not :start
;;   :end :count :key
;;------------------------------------------------------------------------------
(defun nsubstitute (new old sequence &key
                        from-end test test-not (start 0)
                        (end most-positive-fixnum) (count most-positive-fixnum)
                        key)
  (setq test (check-seq-test test test-not))
  (seq-dispatch
   sequence
   (if from-end
       (nreverse
        (nlist-substitute
         new old (nreverse sequence) test start end count key))
       (nlist-substitute new old sequence test start end count key))
   (progn
     (check-seq-end end (length sequence))
     (if from-end
         (nvector-substitute new old sequence -1
                             test (1- end) (1- start) count key)
         (nvector-substitute new old sequence 1 test start end count key)))))

;;------------------------------------------------------------------------------
(defun nlist-substitute (new old sequence test start end count key)
  (do ((list (nthcdr start sequence) (cdr list))
       (index start (1+ index)))
      ((or (endp list) (= index end) (= count 0)) sequence)
    (when (funcall test old (apply-key key (car list)))
      (rplaca list new)
      (setq count (1- count)))))

;;------------------------------------------------------------------------------
(defun nvector-substitute (new old sequence incrementer
                               test start end count key)
  (do ((index start (+ index incrementer)))
      ((or (= index end) (= count 0)) sequence)
    (when (funcall test old (apply-key key (aref sequence index)))
      (setf (aref sequence index) new)
      (setq count (1- count)))))

;;-----------------------------------------------------------------------------
;; NSUBSTITUTE-IF newitem test sequence &KEY :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun nsubstitute-if (new predicate sequence &key
                           from-end (start 0) (end most-positive-fixnum)
                           (count most-positive-fixnum) key)
  (nsubstitute new nil sequence
               :test #'(lambda (item seq-elem)
                         (declare (ignore item))
                         (funcall predicate seq-elem))
               :from-end from-end :start start :end end :count count :key key))

;;-----------------------------------------------------------------------------
;; NSUBSTITUTE-IF-NOT newitem test sequence &KEY
;;    :from-end :start :end :count :key
;;-----------------------------------------------------------------------------
(defun nsubstitute-if-not (new predicate sequence &key
                                   from-end (start 0) (end most-positive-fixnum)
                                   (count most-positive-fixnum)
                                   key)
  (nsubstitute new nil sequence
               :test #'(lambda (item seq-elem)
                         (declare (ignore item))
                         (not (funcall predicate seq-elem)))
               :from-end from-end :start start :end end :count count :key key))

;;-----------------------------------------------------------------------------
;; FIND item sequence &KEY :from-end :test :test-not :start :end :key
;;-----------------------------------------------------------------------------
(defun find (item sequence &key
                  from-end test test-not (start 0)
                  (end most-positive-fixnum) key)
  (seq-dispatch
   sequence
   (list-find item sequence from-end test test-not start end key)
   (vector-find  item sequence from-end test test-not start end key)))

;;-----------------------------------------------------------------------------
(defun vector-find (item vector from-end test test-not start end key)
  (setq test (check-seq-test test test-not))
  (check-seq-end end (length vector))
  (if (not from-end)
      (do ((i start (1+ i)))
          ((>= i end) nil)
        (let ((elem (elt vector i)))
          (when (funcall test item (apply-key key elem)) (return elem))))
      (do ((i (1- end) (1- i)))
          ((< i start) nil)
        (let ((elem (elt vector i)))
          (when (funcall test item (apply-key key elem)) (return elem))))))

;;-----------------------------------------------------------------------------
(defun list-find (item list from-end test test-not start end key)
  (setq test (check-seq-test test test-not))
  (let ((i start)
        (result nil))
    (dolist (elem (nthcdr start list))
      (when (>= i end) (return))
      (when (funcall test item (apply-key key elem))
        (setf result elem)
        (unless from-end (return)))
      (incf i))
    result))

;;-----------------------------------------------------------------------------
;; FIND-IF predicate sequence &KEY :from-end :start :end :key
;;-----------------------------------------------------------------------------
(defun find-if (predicate sequence &key
                          from-end (start 0)
                          (end most-positive-fixnum) key)
  (find nil sequence
        :test #'(lambda (item seq-elem)
                  (declare (ignore item))
                  (funcall predicate seq-elem))
        :from-end from-end :start start :end end :key key))

;;-----------------------------------------------------------------------------
;; FIND-IF-NOT predicate sequence &KEY :from-end :start :end :key
;;-----------------------------------------------------------------------------
(defun find-if-not (predicate sequence &rest rest-args)
  (apply #'find nil sequence
         :test #'(lambda (item seq-elem)
                   (declare (ignore item))
                   (not (funcall predicate seq-elem)))
         rest-args))

;;-----------------------------------------------------------------------------
;; POSITION item sequence &KEY :from-end :test :test-not :start :end :key
;;-----------------------------------------------------------------------------
(defun position (item sequence &key
                      from-end test test-not (start 0)
                      (end most-positive-fixnum) key)
  (seq-dispatch
   sequence
   (list-position item sequence from-end test test-not start end key)
   (vector-position item sequence from-end test test-not start end key)))

;;-----------------------------------------------------------------------------
(defun vector-position (item sequence from-end test test-not start end key)
  (setq test (check-seq-test test test-not))
  (check-seq-end end (length sequence))

  (if (not from-end)
    (do ((i start (1+ i)))
        ((>= i end) nil)
      (when (funcall test item (apply-key key (elt sequence i)))
        (return i)))
    (do ((i (1- end) (1- i)))
        ((< i start) nil)
      (when (funcall test item (apply-key key (elt sequence i)))
        (return i)))))

;;-----------------------------------------------------------------------------
(defun list-position (item list from-end test test-not start end key)
  (let ((i start)
        (result nil))
    (setq test (check-seq-test test test-not))
    (dolist (elem (nthcdr start list))
      (when (>= i end) (return))
      (when (funcall test item (apply-key key elem))
        (setf result i)
        (unless from-end (return)))
      (incf i))
    result))

;;-----------------------------------------------------------------------------
;; POSITION-IF predicate sequence &KEY :from-end :start :end :key
;;-----------------------------------------------------------------------------
(defun position-if (predicate sequence &key
                              from-end (start 0)
                              (end most-positive-fixnum) key)
  (position nil sequence
            :test #'(lambda (item seq-elem)
                      (declare (ignore item))
                      (funcall predicate seq-elem))
            :from-end from-end :start start :end end :key key))

;;-----------------------------------------------------------------------------
;; POSITION-IF-NOT predicate sequence &KEY :from-end :start :end :key
;;-----------------------------------------------------------------------------
(defun position-if-not (predicate sequence &key
                                  from-end (start 0)
                                  (end most-positive-fixnum) key)
  (position nil sequence
            :test #'(lambda (item seq-elem)
                      (declare (ignore item))
                      (not (funcall predicate seq-elem)))
            :from-end from-end :start start :end end :key key))

;;-----------------------------------------------------------------------------
;; COUNT item sequence &KEY :from-end :test :test-not :start :end :key
;;-----------------------------------------------------------------------------
(defun count (item sequence &key
                   from-end test test-not (start 0)
                   (end most-positive-fixnum) key)
  (declare (ignore from-end))
  (setq test (check-seq-test test test-not))
  (seq-dispatch
   sequence
   (list-count item sequence test start end key)
   (vector-count item sequence test start end key)))

;;-----------------------------------------------------------------------------
(defun list-count (item list test start end key)
  (decf end start)
  (let ((count 0))
    (dolist (elem (nthcdr start list))
      (when (>= 0 end) (return))
      (when (funcall test item (apply-key key elem))
        (incf count))
      (decf end))
    count))

;;-----------------------------------------------------------------------------
(defun vector-count (item vector test start end key)
  (check-seq-end end (length vector))
  (let ((count 0))
    (do ((i start (1+ i)))
        ((>= i end))
      (when (funcall test item (apply-key key (elt vector i)))
        (incf count)))
    count))

;;-----------------------------------------------------------------------------
;; COUNT-IF predicate sequence &KEY :from-end :start :end :key
;;-----------------------------------------------------------------------------
(defun count-if (predicate sequence &key
                           from-end (start 0)
                           (end most-positive-fixnum) key)
  (count nil sequence
         :test #'(lambda (item seq-elem)
                   (declare (ignore item))
                   (funcall predicate seq-elem))
         :from-end from-end :start start :end end :key key))

;;-----------------------------------------------------------------------------
;; COUNT-IF-NOT predicate sequence &KEY :from-end :start :end :key
;;-----------------------------------------------------------------------------
(defun count-if-not (predicate sequence &key
                               from-end (start 0)
                               (end most-positive-fixnum) key)
  (count nil sequence
         :test #'(lambda (item seq-elem)
                   (declare (ignore item))
                   (not (funcall predicate seq-elem)))
         :from-end from-end :start start :end end :key key))

;;------------------------------------------------------------------------------
;; MISMATCH utilities, CMU-CL code
;;------------------------------------------------------------------------------
(defmacro matchify-list (sequence start length end)
  `(setq ,sequence
    (if from-end
        (nthcdr (- ,length ,start 1) (reverse ,sequence))
        (nthcdr ,start ,sequence))))

;;------------------------------------------------------------------------------
(defmacro if-mismatch (elt1 elt2)
  `(cond ((= index1 end1)
	  (return (if (= index2 end2)
		      nil
		      (if from-end (1+ index1) index1))))
	 ((= index2 end2)
	  (return (if from-end (1+ index1) index1)))
	 (t (if (not (funcall test (apply-key key ,elt1) (apply-key key ,elt2)))
		(return (if from-end (1+ index1) index1))))))

;;------------------------------------------------------------------------------
(defmacro mumble-mumble-mismatch ()
  `(do ((index1 start1 (+ index1 inc))
	(index2 start2 (+ index2 inc)))
       (())
     (if-mismatch (aref sequence1 index1) (aref sequence2 index2))))

;;------------------------------------------------------------------------------
(defmacro mumble-list-mismatch ()
  `(do ((index1 start1 (+ index1 inc))
	(index2 start2 (+ index2 inc)))
       (())
     (if-mismatch (aref sequence1 index1) (pop sequence2))))

;;------------------------------------------------------------------------------
(defmacro list-mumble-mismatch ()
  `(do ((index1 start1 (+ index1 inc))
	(index2 start2 (+ index2 inc)))
       (())
     (if-mismatch (pop sequence1) (aref sequence2 index2))))

;;------------------------------------------------------------------------------
(defmacro list-list-mismatch ()
  `(do ((index1 start1 (+ index1 inc))
	(index2 start2 (+ index2 inc)))
    (())
    (if-mismatch (pop sequence1) (pop sequence2))))

;;------------------------------------------------------------------------------
;; MISMATCH sequence1 sequence2 &KEY :from-end :test :test-not
;;          :key :start1 :start2 :end1 :end2
;;------------------------------------------------------------------------------
(defun mismatch (sequence1 sequence2 &key from-end test test-not 
			   (start1 0) end1 (start2 0) end2 key)
  (let ((length1 (length sequence1))
	(length2 (length sequence2)))
    (when (null end1) (setf end1 length1))
    (when (null end2) (setf end2 length2))
    (setq test (check-seq-test test test-not))
    (let ((inc (if from-end -1 1))
          (start1 (if from-end (1- end1) start1))
          (start2 (if from-end (1- end2) start2))
          (end1 (if from-end (1- start1) end1))
          (end2 (if from-end (1- start2) end2)))
      (cond
        ((listp sequence1)
         (matchify-list sequence1 start1 length1 end1)
         (cond
           ((listp sequence2)
            (matchify-list sequence2 start2 length2 end2)
            (list-list-mismatch))
           (T (list-mumble-mismatch))))
        ((listp sequence2)
         (matchify-list sequence2 start2 length2 end2)
         (mumble-list-mismatch))
        (T (mumble-mumble-mismatch))))))

;;------------------------------------------------------------------------------
;; SEARCH utilities, CMU-CL code
;;------------------------------------------------------------------------------
(defmacro compare-elements (elt1 elt2)
  `(if test-not
       (if (funcall test-not (apply-key key ,elt1) (apply-key key ,elt2))
	   (return nil)
	   t)
       (if (not (funcall test (apply-key key ,elt1) (apply-key key ,elt2)))
	   (return nil)
	   t)))

;;------------------------------------------------------------------------------
(defmacro search-compare-list-list (main sub)
  `(do ((main ,main (cdr main))
	(jndex start1 (1+ jndex))
	(sub (nthcdr start1 ,sub) (cdr sub)))
       ((or (null main) (null sub) (= end1 jndex)) t)
     (compare-elements (car main) (car sub))))

;;------------------------------------------------------------------------------
(defmacro search-compare-list-vector (main sub)
  `(do ((main ,main (cdr main))
	(index start1 (1+ index)))
       ((or (null main) (= index end1)) t)
     (compare-elements (car main) (aref ,sub index))))

;;------------------------------------------------------------------------------
(defmacro search-compare-vector-list (main sub index)
  `(do ((sub (nthcdr start1 ,sub) (cdr sub))
	(jndex start1 (1+ jndex))
	(index ,index (1+ index)))
       ((or (= end1 jndex) (null sub)) t)
     (compare-elements (aref ,main index) (car sub))))

;;------------------------------------------------------------------------------
(defmacro search-compare-vector-vector (main sub index)
  `(do ((index ,index (1+ index))
	(sub-index start1 (1+ sub-index)))
       ((= sub-index end1) t)
     (compare-elements (aref ,main index) (aref ,sub sub-index))))

;;------------------------------------------------------------------------------
(defmacro search-compare (main-type main sub index)
  (if (eq main-type 'list)
      `(seq-dispatch ,sub
        (search-compare-list-list ,main ,sub)
        (search-compare-list-vector ,main ,sub))
      `(seq-dispatch ,sub
        (search-compare-vector-list ,main ,sub ,index)
        (search-compare-vector-vector ,main ,sub ,index))))
 
;;------------------------------------------------------------------------------
(defmacro list-search (main sub)
  `(do ((main (nthcdr start2 ,main) (cdr main))
	(index2 start2 (1+ index2))
	(terminus (- (the fixnum end2)
		     (the fixnum (- (the fixnum end1)
				    (the fixnum start1)))))
	(last-match ()))
       ((> index2 terminus) last-match)
     (declare (fixnum index2 terminus))
     (if (search-compare list main ,sub index2)
	 (if from-end
	     (setq last-match index2)
	     (return index2)))))

;;------------------------------------------------------------------------------
(defmacro vector-search (main sub)
  `(do ((index2 start2 (1+ index2))
	(terminus (- end2 (- end1 start1)))
	(last-match ()))
    ((> index2 terminus) last-match)
    (if (search-compare vector ,main ,sub index2)
        (if from-end
            (setq last-match index2)
            (return index2)))))

;;------------------------------------------------------------------------------
;; SEARCH sequence1 sequence2 &KEY :from-end :test :test-not
;;        :key :start1 :start2 :end1 :end2
;;------------------------------------------------------------------------------
(defun search (sequence1 sequence2 &key from-end (test #'eql) test-not 
		(start1 0) end1 (start2 0) end2 key)
  (when (null end1) (setf end1 (length sequence1)))
  (when (null end2) (setf end2 (length sequence2)))
  (seq-dispatch sequence2
    (list-search sequence2 sequence1)
    (vector-search sequence2 sequence1)))

;;-----------------------------------------------------------------------------
;; SORT sequence predicate &KEY :key
;;-----------------------------------------------------------------------------
(defun sort (sequence predicate &key key)
  (seq-dispatch
   sequence
   (list-merge-sort sequence predicate key)
   (quick-sort sequence 0 (length sequence) predicate key)))

;;------------------------------------------------------------------------------
(defun list-merge-sort (l predicate key)
  (labels
      ((sort (l)
         (let ((i (length l))
               left right l0 l1 key-left key-right)
           (cond ((< i 2) l)
                 ((= i 2)
                  (setq key-left (apply-key key (car l)))
                  (setq key-right (apply-key key (cadr l)))
                  (if (or (funcall predicate key-left key-right)
                          (not (funcall predicate key-right key-left)))
                      l
                      (nreverse l)))
                 (t (setq i (floor i 2))
                    (do ((j 1 (1+ j)) (l1 l (cdr l1)))
                        ((>= j i)
                         (setq left l)
                         (setq right (cdr l1))
                         (rplacd l1 nil)))
                    (setq left (sort left))
                    (setq right (sort right))
                    (cond ((endp left) right)
                          ((endp right) left)
                          (t (setq l0 (cons nil nil))
                             (setq l1 l0)
                             (setq key-left (apply-key key (car left)))
                             (setq key-right (apply-key key (car right)))

                             (loop
                               (cond
                                 ((or (funcall predicate key-left key-right)
                                      (not
                                       (funcall predicate key-right key-left)))
                                
                                  (rplacd l1 left)
                                  (setq l1 (cdr l1))
                                  (setq left (cdr left))
                                  (when (endp left)
                                    (rplacd l1 right)
                                    (return (cdr l0)))
                                  (setq key-left (apply-key key (car left))))
                                 (t (rplacd l1 right)
                                    (setq l1 (cdr l1))
                                    (setq right (cdr right))
                                    (when (endp right)
                                      (rplacd l1 left)
                                      (return (cdr l0)))
                                    (setq key-right
                                          (apply-key key (car right)))))))))))))
    (sort l)))

;;------------------------------------------------------------------------------
(defun quick-sort (sequence start end predicate key)
  (let ((j 0) (k 0))
    (when (<= end (1+ start))
      (return-from quick-sort sequence))
    (setq j start)
    (setq k (1- end))
    (do ((d (elt sequence start)))
        ((> j k))
      (do ()
          ((or (> j k)
               (funcall predicate
                        (apply-key key (elt sequence k))
                        (apply-key key d))))
        (decf k))
      (when (< k start)
        (quick-sort sequence (1+ start) end predicate key)
        (return-from quick-sort sequence))
      (do ()
          ((or (> j k)
               (not (funcall predicate
                             (apply-key key (elt sequence j))
                             (apply-key key d)))))
        (incf j))
      (when (> j k) (return))
      (rotatef (elt sequence j) (elt sequence k))
      (incf j)
      (decf k))
    (quick-sort sequence start j predicate key)
    (quick-sort sequence j end predicate key)
    sequence))
