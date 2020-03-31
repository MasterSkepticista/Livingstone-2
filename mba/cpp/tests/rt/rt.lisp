;;; Copyright (c) 1997 National Aeronautics and Space Administration.
;;; See mba/Copyright.txt for important copyright and disclaimer information
;;;
;;; -*- Mode:Common-Lisp; Package:TP; Base:10 -*-

(in-package :tp)

(defvalues assert-values (asserted not-asserted no-command))
(defvalues comm-status-values (comm no-comm))


(defcomponent remote-terminal (?name)
  (:inputs
    ((reset-cmd-in ?name)
     :type assert-values ))

  (:outputs
   ((comm-status-out ?name)
    :type comm-status-values ))

  (:background  :initial-mode nominal  )

  (nominal
   :type :ok-mode
   :model (= (comm-status-out ?name) comm)
   :transitions ((:otherwise :persist)))

  (resettable-failure
   :type :fault-mode
   :probability 0.01
   :model (=(comm-status-out ?name) no-comm)
   :transitions ((reset-rt 
			   :when (= (reset-cmd-in ?name) asserted)
			   :next nominal)
		 (:otherwise :persist)))

  (permanent-failure
   :type :fault-mode
   :probability 0.001
   :model (=  (comm-status-out ?name) no-comm)))



(defmodule rt-module (?name)
 (:structure
  (remote-terminal ?name)))


(defun create-rt()
  (instantiate-module '(rt-module rt1))
  (new-command '(reset-cmd-in rt1))
  (new-monitor '(comm-status-out rt1)))

(defun rt-test ()
  (create-rt)
  (do-monitors '((no-comm (comm-status-out rt1))))
  (do-cmd '(reset-cmd-in rt1) 'asserted  '((comm (comm-status-out rt1)))))


;;; Note that we repeatedly get the resettable failure diagnosis.
(defun rt-test-2 ()
  (create-rt)
  (do-monitors '((no-comm (comm-status-out rt1))))
  (do-cmd '(reset-cmd-in rt1) 'asserted)
  (do-cmd '(reset-cmd-in rt1) 'asserted)
  (do-cmd '(reset-cmd-in rt1) 'asserted)
  (do-cmd '(reset-cmd-in rt1) 'asserted))


