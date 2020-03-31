;;;
;;; This model was developed by the Embedded Technology Group
;;; at NASA KSC
;;;
;;; -*- Mode:Common-Lisp; Package:TP; Base:10 -*-
(in-package :tp)

#|      Schematic for Circuit Breaker model    NOV-09-1998
    - Added Cost to On and Off modes so that recovery performs as expected.
      Default cost in Livingstone is 0 which causes recovery to recommend too many switch throws.


                                            ---- 1
                                  ----  9 <
                                /           ---- 2
                       ---- 13 <
                      /         \           ---- 3
                     /            ---- 10 <
                    /                       ---- 4
    source ---- 15 <                                 
                    \                       ---- 5
                     \            ---- 11 <
                      \         /           ---- 6
                       ---- 14 <
                                \           ---- 7
                                  ---- 12 <
                                            ---- 8
|#

(defvalues on-off-state (on off))
(defvalues CB-cmd  (on off no-cmd reset))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defcomponent CB (?name)

  (:inputs
   ((cmd-in ?name) 
    :type CB-cmd
    :documentation "Command to CB")
   ((current-in ?name) 
    :type on-off-state
    :documentation "Current flowing into CB."))
  
  (:outputs
   ((current-out ?name)
    :type on-off-state
    :documentation "Current flowing out of CB."))
  
  (:background   :initial-mode on)
  
  (on :type :ok-mode
      :documentation "Turn on CB "
      :model (= (current-in ?name)
		(current-out ?name))
      :transitions ((turn-off
                     :when (= (cmd-in ?name) off)
                     :next off
                     :cost 1)
                    (:otherwise :persist))
  )
  (off :type :ok-mode
       :documentation "Turn off CB "
       :model  (off (current-out ?name))
                                
       :transitions ((turn-on
                     :when (= (cmd-in ?name) on)
                     :next on
                     :cost 1)
		    (:otherwise :persist))
  )
  (tripped  :type :fault-mode
            :documentation "Resettable failure"
            :probability unlikely
            :model (off (current-out ?name))
            :transitions ((perform-reset
                           :when (= (cmd-in ?name) reset)
                           :next off)   ;; note the change from next blown
                          (:otherwise :persist))
  )
  (blown :type :fault-mode
	 :documentation "Blown CB"
         :model (off (current-out ?name))
	 :probability rare
	 :transitions ((:otherwise :persist))
  )
  (unknown-CB-fault :type :fault-mode
	 :documentation "Any behavior can occur here."
	 :probability rare
	 :transitions ((:otherwise :persist))
  )
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defcomponent LED (?name)
    (:inputs
     ((current-in ?name)
      :type on-off-state
      :documentation ""))
     
     (:outputs
      ((LED-state ?name)
       :type on-off-state
       :documentation "")) 
     
     (:background   :initial-mode nominal)
     
     (nominal :type :ok-mode
         :documentation ""
         :model (= (LED-state ?name)
		   (current-in ?name))
         :transitions ((:otherwise :persist))
     )
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defcomponent current-source (?name)
     (:outputs
      ((current-out ?name)
       :type on-off-state
       :documentation "A source of current for the system.")) 
     
     (:background   :initial-mode nominal)
     
     (nominal :type :ok-mode
              :documentation ""
              :model (on (current-out ?name))
              :transitions ((:otherwise :persist))
     )
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defmodule CB-and-LEDs (?name)

  (:structure
   (LED LED-1)  (LED LED-2)  (LED LED-3)  (LED LED-4)
   (LED LED-5)  (LED LED-6)  (LED LED-7)  (LED LED-8)
   (CB CB-1)  (CB CB-2)  (CB CB-3)  (CB CB-4)
   (CB CB-5)  (CB CB-6)  (CB CB-7)  (CB CB-8)
   (CB CB-9)  (CB CB-10) (CB CB-11) (CB CB-12)
   (CB CB-13) (CB CB-14) (CB CB-15)
   (current-source source)
   )
  (:connections  (and
    (= (current-out CB-1)  (current-in LED-1))
    (= (current-out CB-2)  (current-in LED-2))
    (= (current-out CB-3)  (current-in LED-3))
    (= (current-out CB-4)  (current-in LED-4))
    (= (current-out CB-5)  (current-in LED-5))
    (= (current-out CB-6)  (current-in LED-6))
    (= (current-out CB-7)  (current-in LED-7))
    (= (current-out CB-8)  (current-in LED-8))
    (= (current-out CB-9)  (current-in CB-1))
    (= (current-out CB-9)  (current-in CB-2))
    (= (current-out CB-10) (current-in CB-3))
    (= (current-out CB-10) (current-in CB-4))
    (= (current-out CB-11) (current-in CB-5))
    (= (current-out CB-11) (current-in CB-6))
    (= (current-out CB-12) (current-in CB-7))
    (= (current-out CB-12) (current-in CB-8))
    (= (current-out CB-13) (current-in CB-9))
    (= (current-out CB-13) (current-in CB-10))
    (= (current-out CB-14) (current-in CB-11))
    (= (current-out CB-14) (current-in CB-12))
    (= (current-out CB-15) (current-in CB-13))
    (= (current-out CB-15) (current-in CB-14))
    (= (current-out source) (current-in CB-15)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun create-CB ()

  (format t "--> entering create-CB  ~%")
  (instantiate-module '(CB-and-LEDs someName))
  (new-command-table)
  (new-monitor-table)

  ;; create the command structure
  (dolist (cb '(CB-1 CB-2 CB-3 CB-4 CB-5 CB-6 CB-7 CB-8 CB-9
		     CB-10 CB-11 CB-12 CB-13 CB-14 CB-15))
	  (new-command `(cmd-in ,cb)  '(on off no-cmd reset) 'no-cmd))

   ;; Create monitors which will assert persistent values
   (dolist (led '(LED-1 LED-2 LED-3 LED-4 LED-5 LED-6 LED-7 LED-8))
	  (new-monitor `(LED-state ,led) '(on off unknown) 'unknown))

   ;; force the theory to be updated here, to allow querying
   ;; of the initial state ...
   (propagate)

  (setf (system-proposition-monitors *system*) nil)

  (dolist (led '(LED-1 LED-2 LED-3 LED-4 LED-5 LED-6 LED-7 LED-8))
        (new-proposition-monitor
           `(
             ;;    Proposition        Signal to be printed
             ((on  (LED-state ,led))  (state ,led on))
             ((off (LED-state ,led))  (state ,led off))
             )
             ;; Signal if neither on/off
           `(state ,led unknown)
        )
   )
   (do-monitors '((on  (LED-state LED-1))
                  (on  (LED-state LED-2))
                  (on  (LED-state LED-3))
                  (on  (LED-state LED-4))
                  (on  (LED-state LED-5))
                  (on  (LED-state LED-6))
                  (on  (LED-state LED-7))
                  (on  (LED-state LED-8))
                 )
   )
   (format t "--> exiting  create-CB  ~%")
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun test-CB () 
  (format t "--> entering test-CB  ~%")

(format t "~%************** recover test 1 ********************* ~%")
   (create-CB)
   (do-monitors '((unknown  (LED-state LED-1))
                  (unknown  (LED-state LED-2))
                  (unknown  (LED-state LED-3))
                  (unknown  (LED-state LED-4))
                  (unknown  (LED-state LED-5))
                  (unknown  (LED-state LED-6))
                  (unknown  (LED-state LED-7))
                  (unknown  (LED-state LED-8)) ))
     (do-cmd '(cmd-in CB-15) 'off)
     (do-cmd '(cmd-in CB-1) 'off)
     (do-cmd '(cmd-in CB-2) 'off)
     (do-cmd '(cmd-in CB-3) 'off)
     (do-cmd '(cmd-in CB-4) 'off)
     (do-cmd '(cmd-in CB-5) 'off)
     (do-cmd '(cmd-in CB-6) 'off)
     (do-cmd '(cmd-in CB-7) 'off)
     (do-cmd '(cmd-in CB-8) 'off)
     (do-cmd '(cmd-in CB-9) 'off)
     (do-cmd '(cmd-in CB-10) 'off)
     (do-cmd '(cmd-in CB-11) 'off)
     (do-cmd '(cmd-in CB-12) 'off)
     (do-cmd '(cmd-in CB-13) 'off)
     (do-cmd '(cmd-in CB-14) 'off)
   (format t "RECOVERY ~S~%" 
      (recover '(on (LED-STATE LED-6)))
   )

(format t "~%************** recover test 2 ********************* ~%")
   (create-CB)
   (do-monitors '((unknown  (LED-state LED-1))
                  (unknown  (LED-state LED-2))
                  (unknown  (LED-state LED-3))
                  (unknown  (LED-state LED-4))
                  (unknown  (LED-state LED-5))
                  (unknown  (LED-state LED-6))
                  (unknown  (LED-state LED-7))
                  (unknown  (LED-state LED-8)) ))
     (do-cmd '(cmd-in CB-15) 'off)
     (do-cmd '(cmd-in CB-1) 'off)
     (do-cmd '(cmd-in CB-2) 'off)
     (do-cmd '(cmd-in CB-3) 'off)
     (do-cmd '(cmd-in CB-4) 'off)
     (do-cmd '(cmd-in CB-5) 'off)
     (do-cmd '(cmd-in CB-6) 'off)
     (do-cmd '(cmd-in CB-7) 'off)
     (do-cmd '(cmd-in CB-8) 'off)
     (do-cmd '(cmd-in CB-9) 'off)
     (do-cmd '(cmd-in CB-10) 'off)
     (do-cmd '(cmd-in CB-11) 'off)
     (do-cmd '(cmd-in CB-12) 'off)
     (do-cmd '(cmd-in CB-13) 'off)
     (do-cmd '(cmd-in CB-14) 'off)
   (format t "RECOVERY ~S~%" 
      (recover '(on (LED-STATE LED-4)))
   )


(format t "~%************** recover test 3 ********************* ~%")
   (create-CB)
   (format t "RECOVERY ~S~%" 
      (recover '(off (LED-STATE LED-5)))
   )

(format t "~%************** recover test 4 ********************* ~%")
   (create-CB)
   (format t "RECOVERY ~S~%" 
      (recover '(and
                   (off (LED-STATE LED-1))
                   (off (LED-STATE LED-2))
                   (off (LED-STATE LED-3))
                   (off (LED-STATE LED-4))
                   (off (LED-STATE LED-5))
                   (on  (LED-STATE LED-6))
                   (off (LED-STATE LED-7))
                   (off (LED-STATE LED-8))
                )
      )
   )
   (do-monitors '((unknown  (LED-state LED-1))
                  (unknown  (LED-state LED-2))
                  (unknown  (LED-state LED-3))
                  (unknown  (LED-state LED-4))
                  (unknown  (LED-state LED-5))
                  (unknown  (LED-state LED-6))
                  (unknown  (LED-state LED-7))
                  (unknown  (LED-state LED-8)) ))
   (do-cmd '(cmd-in CB-13) 'off)
   (do-cmd '(cmd-in CB-8) 'off)
   (do-cmd '(cmd-in CB-7) 'off)
   (do-cmd '(cmd-in CB-5) 'off)
   (format t "RECOVERY ~S~%" 
      (recover '(and
                   (off (LED-STATE LED-1))
                   (off (LED-STATE LED-2))
                   (on  (LED-STATE LED-3))
                   (on  (LED-STATE LED-4))
                   (off (LED-STATE LED-5))
                   (on  (LED-STATE LED-6))
                   (off (LED-STATE LED-7))
                   (off (LED-STATE LED-8))
                )
      )
   )


#| *************************************************************



(format t "~%~% ********** TEST RESET 1 ********************* ~%")
 (create-CB)
 (do-monitors '((off (LED-state LED-8))))
 (do-cmd '(cmd-in CB-8) 'reset)
 (do-cmd '(cmd-in CB-8) 'on '((on (LED-state LED-8))))

(format t "~%~% ********** TEST RESET 2 ********************* ~%")

 (create-CB)
 (do-monitors '((off  (LED-state LED-1))))
 (do-cmd '(cmd-in CB-1) 'reset  '((off (LED-state LED-1))))


(format t "~%~%~% ********** DIAGNOSTIC TEST 1 ********************* ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-8))
                  (off (LED-state LED-6))
                  (off (LED-state LED-4))
                  (off (LED-state LED-2))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-8))
                  (on (LED-state LED-6))
                  (on (LED-state LED-4))
                  (on (LED-state LED-2))
                 )
   )
(format t "~%~%~% ********** DIAGNOSTIC TEST 2 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-7))
                  (off (LED-state LED-5))
                  (off (LED-state LED-3))
                  (off (LED-state LED-1))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-7))
                  (on (LED-state LED-5))
                  (on (LED-state LED-3))
                  (on (LED-state LED-1))
                 )
   )
(format t "~%~%~% ********** DIAGNOSTIC TEST 3 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-1))
                  (off (LED-state LED-2))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-1))
                  (on (LED-state LED-2))
                 )
   )
(format t "~%~%~% ********** DIAGNOSTIC TEST 4 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-3))
                  (off (LED-state LED-4))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-3))
                  (on (LED-state LED-4))
                 )
   )
(format t "~%~%~% ********** DIAGNOSTIC TEST 5 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-5))
                  (off (LED-state LED-6))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-5))
                  (on (LED-state LED-6))
                 )
   )
(format t "~%~%~% ********** DIAGNOSTIC TEST 6 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-7))
                  (off (LED-state LED-8))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-7))
                  (on (LED-state LED-8))
                 )
   )
(format t "~%~%~% ********** DIAGNOSTIC TEST 7 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-5))
                  (off (LED-state LED-6))
                  (off (LED-state LED-7))
                  (off (LED-state LED-8))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-5))
                  (on (LED-state LED-6))
                  (on (LED-state LED-7))
                  (on (LED-state LED-8))
                 )
   )
(format t "~%~%~% ********** DIAGNOSTIC TEST 8 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off (LED-state LED-1))
                  (off (LED-state LED-2))
                  (off (LED-state LED-3))
                  (off (LED-state LED-4))
                 )
   )
   (do-monitors '(
                  (on (LED-state LED-1))
                  (on (LED-state LED-2))
                  (on (LED-state LED-3))
                  (on (LED-state LED-4))
                 )
   )
(format t "************ DIAGNOSTIC TEST 9 ********************** ~%")
  (create-CB)
   (do-monitors '(
                  (off  (LED-state LED-1))
                  (off  (LED-state LED-2))
                  (off  (LED-state LED-3))
                  (off  (LED-state LED-4))
                  (off  (LED-state LED-5))
                  (off  (LED-state LED-6))
                  (off  (LED-state LED-7))
                  (off  (LED-state LED-8))
                 )
   )


  (create-CB)
(format t "************ DIAGNOSTIC TEST 10 ********************** ~%")
   (do-monitors '(
                  (off (LED-state LED-3))
                  (off (LED-state LED-4))
                  (off (LED-state LED-7))
                  (off (LED-state LED-8))
                 )
   )
(format t "************ DIAGNOSTIC TEST 11 ********************** ~%")
   (do-monitors '((off  (LED-state LED-1))
                  (off  (LED-state LED-2))
                  (off  (LED-state LED-5))
                  (off  (LED-state LED-6))
                 )
   )
(format t "************ DIAGNOSTIC TEST 12 ********************** ~%")
   (do-monitors '(
                  (on  (LED-state LED-2))
                  (on  (LED-state LED-4))
                  (on  (LED-state LED-6))
                  (on  (LED-state LED-8))
                 )
   )
(format t "************ DIAGNOSTIC TEST 13 ********************** ~%")
   (do-monitors '((on  (LED-state LED-1))
                  (on  (LED-state LED-3))
                  (on  (LED-state LED-5))
                  (on  (LED-state LED-7))
                 )
   )



(format t "************ Commanding test 1 ******************** ~%")
  (create-CB)
   (do-monitors '((unknown  (LED-state LED-1))
                  (unknown  (LED-state LED-2))
                  (unknown  (LED-state LED-3))
                  (unknown  (LED-state LED-4))
                  (unknown  (LED-state LED-5))
                  (unknown  (LED-state LED-6))
                  (unknown  (LED-state LED-7))
                  (unknown  (LED-state LED-8)) ))
     (do-cmd '(cmd-in CB-9) 'off)
     (do-cmd '(cmd-in CB-13) 'off)
     (do-cmd '(cmd-in CB-15) 'off)

(format t "************ Commanding test 2  ******************** ~%")
  (create-CB)
   (do-monitors '((unknown  (LED-state LED-1))
                  (unknown  (LED-state LED-2))
                  (unknown  (LED-state LED-3))
                  (unknown  (LED-state LED-4))
                  (unknown  (LED-state LED-5))
                  (unknown  (LED-state LED-6))
                  (unknown  (LED-state LED-7))
                  (unknown  (LED-state LED-8)) ))
     (do-cmd '(cmd-in CB-15) 'off)
     (do-cmd '(cmd-in CB-1) 'off)
     (do-cmd '(cmd-in CB-2) 'off)
     (do-cmd '(cmd-in CB-3) 'off)
     (do-cmd '(cmd-in CB-4) 'off)
     (do-cmd '(cmd-in CB-5) 'off)
     (do-cmd '(cmd-in CB-6) 'off)
     (do-cmd '(cmd-in CB-7) 'off)
     (do-cmd '(cmd-in CB-8) 'off)
     (do-cmd '(cmd-in CB-9) 'off)
     (do-cmd '(cmd-in CB-10) 'off)
     (do-cmd '(cmd-in CB-11) 'off)
     (do-cmd '(cmd-in CB-12) 'off)
     (do-cmd '(cmd-in CB-13) 'off)
     (do-cmd '(cmd-in CB-14) 'off)

     (do-cmd '(cmd-in CB-15) 'on)
     (do-cmd '(cmd-in CB-13) 'on)
     (do-cmd '(cmd-in CB-10) 'on)
     (do-cmd '(cmd-in CB-4) 'on)
     (do-cmd '(cmd-in CB-3) 'on)
     (do-cmd '(cmd-in CB-1) 'on)
     (do-cmd '(cmd-in CB-2) 'on)
     (do-cmd '(cmd-in CB-5) 'on)
     (do-cmd '(cmd-in CB-6) 'on)
     (do-cmd '(cmd-in CB-7) 'on)
     (do-cmd '(cmd-in CB-8) 'on)

     (do-cmd '(cmd-in CB-9) 'on)

     (do-cmd '(cmd-in CB-11) 'on)
     (do-cmd '(cmd-in CB-12) 'on)
     (do-cmd '(cmd-in CB-14) 'on)

(format t "************ TEST RESET FUNCTIONALITY 1 ****************** ~%")
   (create-CB)
   (do-monitors '((off (LED-state LED-1))))

   (do-cmd '(cmd-in CB-1) 'reset
             '((off (LED-state LED-1))))

   (do-cmd '(cmd-in CB-1) 'reset)


(format t "************ TEST RESET FUNCTIONALITY 2 ****************** ~%")
   (create-CB)
   (do-monitors '((off (LED-state LED-1))))

   (do-cmd '(cmd-in CB-1) 'reset
             '((on (LED-state LED-1))))

   (do-cmd '(cmd-in CB-1) 'reset)




(format t "~%************ Tests Reset Fuse Tripped to On ************* ~%")
   (do-cmd '(cmd-in CB-1) 'on
             '((on (LED-state LED-1))))
   (format t "---------RESTORED-------- ~%")
   (do-monitors '((off (LED-state LED-1))))
   (format t "~&~%==>Giving command to reset CB 1 ~%")  
   (do-cmd '(cmd-in CB-1) 'reset
             '((on (LED-state LED-1))))




(format t "~%************** TEST CONTROL ABILITY ********************** ~%")
(format t "***** Single Fuse Cmd  Causes All LED's to Go Off ******** ~%")
   (do-monitors '((on (LED-state LED-1))
                  (on (LED-state LED-2))
                  (on (LED-state LED-3))
                  (on (LED-state LED-4))
                  (on (LED-state LED-5))
                  (on (LED-state LED-6))
                  (on (LED-state LED-7))
                  (on (LED-state LED-8))
                 )
   )
   (format t "---------RESTORED-------- ~%")
     (format t "~&~%==>Giving command to turn off CB 15 ~%")
     (do-cmd '(cmd-in CB-15) 'off
             '((off (LED-state LED-1))
                  (off (LED-state LED-2))
                  (off (LED-state LED-3))
                  (off (LED-state LED-4))
                  (off (LED-state LED-5))
                  (off (LED-state LED-6))
                  (off (LED-state LED-7))
                  (off (LED-state LED-8)))
     )
    


(format t "~%********** Single Fuse Cmd Causes 2 LED's to Go Off ****** ~%")
     (do-cmd '(cmd-in CB-15) 'on
                '((on (LED-state LED-1))
                  (on (LED-state LED-2))
                  (on (LED-state LED-3))
                  (on (LED-state LED-4))
                  (on (LED-state LED-5))
                  (on (LED-state LED-6))
                  (on (LED-state LED-7))
                  (on (LED-state LED-8)))
     )
   (format t "---------RESTORED-------- ~%")
     (format t "~&~%==>Giving command to turn off CB 9 ~%")
     (do-cmd '(cmd-in CB-9) 'off
             '((off (LED-state LED-1))
               (off (LED-state LED-2))))
;;;   (cprops 'LED-2)
;;;   (cprops 'CB-9)




(format t "~%************** Single Fuse Cmd Causes 4 LED's Off ******** ~%")
     (do-cmd '(cmd-in CB-9) 'on
             '((on (LED-state LED-1))
               (on (LED-state LED-2))))
   (format t "---------RESTORED-------- ~%")
     (format t "~&~%==>Giving command to turn off CB 13 ~%")
     (do-cmd '(cmd-in CB-13) 'off
             '((off (LED-state LED-1))
               (off (LED-state LED-2))
               (off (LED-state LED-3))
               (off (LED-state LED-4))))





(format t "~%******** Turn Off All Fuse Cmds, on Fuses to LED's 1,2 ****~%")
     (do-cmd '(cmd-in CB-13) 'on
             '((on (LED-state LED-1))
               (on (LED-state LED-2))
               (on (LED-state LED-3))
               (on (LED-state LED-4))))
   (format t "---------RESTORED-------- ~%")
     (format t "~&~%==>Giving command to turn off all CB ~%")
     (do-cmd '(cmd-in CB-1) 'off
             '((off (LED-state LED-1))))
     (do-cmd '(cmd-in CB-2) 'off
             '((off (LED-state LED-2))))
     (do-cmd '(cmd-in CB-3) 'off
             '((off (LED-state LED-3))))
     (do-cmd '(cmd-in CB-4) 'off
             '((off (LED-state LED-4))))
     (do-cmd '(cmd-in CB-5) 'off
             '((off (LED-state LED-5))))
     (do-cmd '(cmd-in CB-6) 'off
             '((off (LED-state LED-6))))
     (do-cmd '(cmd-in CB-7) 'off
             '((off (LED-state LED-7))))
     (do-cmd '(cmd-in CB-8) 'off
             '((off (LED-state LED-8))))
     (do-cmd '(cmd-in CB-9) 'off)
     (do-cmd '(cmd-in CB-10) 'off)
     (do-cmd '(cmd-in CB-11) 'off)
     (do-cmd '(cmd-in CB-12) 'off)
     (do-cmd '(cmd-in CB-13) 'off)
     (do-cmd '(cmd-in CB-14) 'off)
     (do-cmd '(cmd-in CB-15) 'off)
 (format t "~&~%==>Giving command to turn on CB 15, 13, 9, 2, 1 ~%")
     (do-cmd '(cmd-in CB-15) 'on)
     (do-cmd '(cmd-in CB-13) 'on)
     (do-cmd '(cmd-in CB-9) 'on)
     (do-cmd '(cmd-in CB-2) 'on
             '((on (LED-state LED-2))))
     (do-cmd '(cmd-in CB-1) 'on
             '((on (LED-state LED-1))))





(format t "~%*************  simple fail case **************** ~%")
;;;;;;;;Simple Fail Case - Turn Off All Fuse Commands then Turn on all Fuses
;;;;;;; Fuses  Downstream of LED's 1, 2, 3, 4.  Monitor indicates LED's 1 
;;;;;;; and 2 are failed Off and LED's 3 and 4 are On (Nominal)
     (do-cmd '(cmd-in CB-1) 'on)
     (do-cmd '(cmd-in CB-2) 'on)
     (do-cmd '(cmd-in CB-3) 'on)
     (do-cmd '(cmd-in CB-4) 'on)
     (do-cmd '(cmd-in CB-5) 'on)
     (do-cmd '(cmd-in CB-6) 'on)
     (do-cmd '(cmd-in CB-7) 'on)
     (do-cmd '(cmd-in CB-8) 'on)
     (do-cmd '(cmd-in CB-9) 'on)
     (do-cmd '(cmd-in CB-10) 'on)
     (do-cmd '(cmd-in CB-11) 'on)
     (do-cmd '(cmd-in CB-12) 'on)
     (do-cmd '(cmd-in CB-13) 'on)
     (do-cmd '(cmd-in CB-14) 'on)
     (do-cmd '(cmd-in CB-15) 'on)
   (do-monitors '((on (LED-state LED-1))
                  (on (LED-state LED-2))
                  (on (LED-state LED-3))
                  (on (LED-state LED-4))
                  (on (LED-state LED-5))
                  (on (LED-state LED-6))
                  (on (LED-state LED-7))
                  (on (LED-state LED-8))
                 )
   )
   (format t "---------RESTORED-------- ~%")
     (format t "~&~%==>Giving command to turn off all CB ~%")
     (do-cmd '(cmd-in CB-1) 'off
             '((off (LED-state LED-1))))
     (do-cmd '(cmd-in CB-2) 'off
             '((off (LED-state LED-2))))
     (do-cmd '(cmd-in CB-3) 'off
             '((off (LED-state LED-3))))
     (do-cmd '(cmd-in CB-4) 'off
             '((off (LED-state LED-4))))
     (do-cmd '(cmd-in CB-5) 'off
             '((off (LED-state LED-5))))
     (do-cmd '(cmd-in CB-6) 'off
             '((off (LED-state LED-6))))
     (do-cmd '(cmd-in CB-7) 'off
             '((off (LED-state LED-7))))
     (do-cmd '(cmd-in CB-8) 'off
             '((off (LED-state LED-8))))
     (do-cmd '(cmd-in CB-9) 'off)
     (do-cmd '(cmd-in CB-10) 'off)
     (do-cmd '(cmd-in CB-11) 'off)
     (do-cmd '(cmd-in CB-12) 'off)
     (do-cmd '(cmd-in CB-13) 'off)
     (do-cmd '(cmd-in CB-14) 'off)
     (do-cmd '(cmd-in CB-15) 'off)
 (format t "~&~%==>Giving command to turn on CB 1, 2, 9, 13, 15 ~%")
     (do-cmd '(cmd-in CB-1) 'on)
     (do-cmd '(cmd-in CB-2) 'on)
     (do-cmd '(cmd-in CB-3) 'on)
     (do-cmd '(cmd-in CB-4) 'on)
     (do-cmd '(cmd-in CB-9) 'on)
     (do-cmd '(cmd-in CB-10) 'on)
     (do-cmd '(cmd-in CB-13) 'on)
     (do-cmd '(cmd-in CB-15) 'on
             '((off (LED-state LED-1))
               (off (LED-state LED-2))
               (on (LED-state LED-3))
               (on (LED-state LED-4))))



(format t "~%***************  A more complex fail case*********** ~%")
;;;;;;; Fail Case - Turn Off All Fuse Commands then Turn on all Fuses
;;;;;;; Downstream of LED's 1, 2, 3, 4.  Monitor indicates LED's 1 and 2
;;;;;;; are Tripped and LED's 3 and 4 are On (Nominal).  Reset Fail on
;;;;;;; Fuse-9.  LED-1 transitions to On while LED-2 remains Off.  Fuse-2
;;;;;;; is declared Tripped.  Reset Fail on Fuse-2.  Monitor indicates
;;;;;;; that LED-2 is Off.  Fuse-2 is declared blown.

     (do-cmd '(cmd-in CB-1) 'on)
     (do-cmd '(cmd-in CB-2) 'on)
     (do-cmd '(cmd-in CB-3) 'on)
     (do-cmd '(cmd-in CB-4) 'on)
     (do-cmd '(cmd-in CB-5) 'on)
     (do-cmd '(cmd-in CB-6) 'on)
     (do-cmd '(cmd-in CB-7) 'on)
     (do-cmd '(cmd-in CB-8) 'on)
     (do-cmd '(cmd-in CB-9) 'on)
     (do-cmd '(cmd-in CB-10) 'on)
     (do-cmd '(cmd-in CB-11) 'on)
     (do-cmd '(cmd-in CB-12) 'on)
     (do-cmd '(cmd-in CB-13) 'on)
     (do-cmd '(cmd-in CB-14) 'on)
     (do-cmd '(cmd-in CB-15) 'on)
   (do-monitors '((on (LED-state LED-1))
                  (on (LED-state LED-2))
                  (on (LED-state LED-3))
                  (on (LED-state LED-4))
                  (on (LED-state LED-5))
                  (on (LED-state LED-6))
                  (on (LED-state LED-7))
                  (on (LED-state LED-8))
                 )
   )
   (format t "---------RESTORED-------- ~%")

     (format t "~&~%==>Giving command to turn off all CB ~%")
     (do-cmd '(cmd-in CB-1) 'off
             '((off (LED-state LED-1))))
     (do-cmd '(cmd-in CB-2) 'off
             '((off (LED-state LED-2))))
     (do-cmd '(cmd-in CB-3) 'off
             '((off (LED-state LED-3))))
     (do-cmd '(cmd-in CB-4) 'off
             '((off (LED-state LED-4))))
     (do-cmd '(cmd-in CB-5) 'off
             '((off (LED-state LED-5))))
     (do-cmd '(cmd-in CB-6) 'off
             '((off (LED-state LED-6))))
     (do-cmd '(cmd-in CB-7) 'off
             '((off (LED-state LED-7))))
     (do-cmd '(cmd-in CB-8) 'off
             '((off (LED-state LED-8))))
     (do-cmd '(cmd-in CB-9) 'off)
     (do-cmd '(cmd-in CB-10) 'off)
     (do-cmd '(cmd-in CB-11) 'off)
     (do-cmd '(cmd-in CB-12) 'off)
     (do-cmd '(cmd-in CB-13) 'off)
     (do-cmd '(cmd-in CB-14) 'off)
     (do-cmd '(cmd-in CB-15) 'off)
 (format t "~&~%==>Giving command to turn on CB 1, 2, 9, 13, 15 ~%")
     (do-cmd '(cmd-in CB-1) 'on)
     (do-cmd '(cmd-in CB-2) 'on)
     (do-cmd '(cmd-in CB-3) 'on)
     (do-cmd '(cmd-in CB-4) 'on)
     (do-cmd '(cmd-in CB-9) 'on)
     (do-cmd '(cmd-in CB-10) 'on)
     (do-cmd '(cmd-in CB-13) 'on)
     (do-cmd '(cmd-in CB-15) 'on
             '((off (LED-state LED-1))
               (off (LED-state LED-2))
               (on (LED-state LED-3))
               (on (LED-state LED-4))))
     (do-cmd '(cmd-in CB-9) 'reset
              '((on (LED-state LED-1))))
      (do-cmd '(cmd-in CB-2) 'reset
              '((off (LED-state LED-2))))



(format t "~%************** Unknown-Fault-Mode Failure*********** ~%")
;;;;;;; Issue a Single Fuse Command that should Cause 2 LED's to Go Off;
;;;;;;; however, One LED transitions to Off while the other remains On.
;;;;;;; Fault Mode should be annunciated as "Unknown-Fault-Mode".

    (do-cmd '(cmd-in CB-1) 'on)
    (do-cmd '(cmd-in CB-2) 'on)
    (do-monitors '((on (LED-state LED-1))
                  (on (LED-state LED-2))
                  (on (LED-state LED-3))
                  (on (LED-state LED-4))
                  (on (LED-state LED-5))
                  (on (LED-state LED-6))
                  (on (LED-state LED-7))
                  (on (LED-state LED-8))
                 )
    )
  (format t "---------RESTORED-------- ~%")
  (format t "~&~%==>Giving command to turn off CB 9 ~%")
  (do-cmd '(cmd-in CB-9) 'off
          '((on (LED-state LED-1))
            (off (LED-state LED-2))))
    
   ************************************************************* |#

   (format t "********** exiting test-CB ***************** ~%")
)


