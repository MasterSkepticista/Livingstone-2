;;; Copyright (c) 1997 National Aeronautics and Space Administration.
;;; See mba/Copyright.txt for important copyright and disclaimer information
;;;
;;; -*- Mode:Common-Lisp; Package:TP; Base:10 -*-

(in-package :tp)

#|

 If you are new to Livingstone, you may want to look at the
 simpler switch example in models/switch.lisp before 
 attempting this example.
 
 This example models a valve control system, as shown in several
 Livingstone-related papers.  We have a controller (Valve Drive
 Electronics Control Unit, or VDECU) which commands two drivers.
 In the physical system the controller corresponds to interface
 hardware which controls the drivers, electrical components which
 actually send driving power to the valves.  Each valve consists
 of an actuator, an electromechanical component which opens and
 closes the valve when driven by the driver and a sensor which
 reports the open/closed position of the actuator.
 
 As shown below, the VDECU has commands to turn on and off, and
 commands which it uses to control driver 1 and driver 2.  Each
 driver can open all valves it drives close all valves it drives.
 On command  a driver can also turn on, turn off, and reset itself,
 clearing some failures.

 A driver may fail in a manner which is resettable, in which case
 power cycling will return the driver to nominal, or 
 the driver may fail permanently.  A valve actuator may stick
 open or closed, for which there is no recovery action.  For
 this simple example neither the VDECU or sensors can fail.
                            
                                           #------------#
                                           # VALVE 1    # 
 vdecu on/off---\                          #            # 
 driver 1 cmd----VDECU-+-- DRIVER 1----+---#---ACTUATOR #
 driver 2 cmd___/      |               |   #     ^      # 
                       |               |   #   SENSOR---#--> observation 
                       |               |   #------------#
                       |               |
                       |               |   #------------#
                       |               +---# VALVE 2    #--> observation 
                       |                   #------------#
                       |       
                       |                   #------------#
                       +-- DRIVER 2----+---# VALVE 3    #--> observation  
                                       |   #------------#
                                       |
                                       |   #------------#
                                       +---# VALVE 4    #--> observation  
                                       |   #------------#
                                       |
                                       |   #------------#
                                       +---# VALVE 5    #--> observation  
                                       |   #------------#
                                       |
                                       |   #------------#
                                       +---# VALVE 6    #--> observation  
                                           #------------#

 In our model we define components for the vdecu, driver, actuator
 and sensor.  We then define a module for the valve which consists
 of an actuator and a sensor and the connections between the two.
 A top level module instantiates one vdecu, two drivers, and six valves
 and makes the necessary connections.

 Here we introduce the use of the MRP portion of Livingstone, or
 the Model-based Reactive Planner.  It will find multi-step
 sequences that reconfigure the system into the state we want.

 An example that generates a multi-step nominal reconfiguration
 and an example with a multi-step recovery are shown.

|#
          
(defvalues vdecu-cmd  (on off no-command))
(defvalues driver-cmd  (on off open close no-command))
(defvalues valve-cmd (open close no-command))
(defvalues valve-position (open closed))
(defvalues checksum (ok corrupted))

(defcomponent valve-sensor (?name)
  (:inputs
   ((sensed-position ?name) :type valve-position
    :documentation "Position of sensor"))
  (:outputs
   ((reported-position ?name) :type valve-position
    :documentation "Reported position of sensor"))

  (:background
   :initial-mode nominal)

 (nominal  :type :ok-mode
	   :documentation ""
 	   :model (= (sensed-position ?name)
		     (reported-position ?name))
	   :transitions ((:otherwise :persist)))
)

(defcomponent valve-actuator (?name)
  (:inputs
   ((cmd-in ?name) :type valve-cmd :documentation "Command to actuator"))
  (:outputs
   ((position ?name) :type valve-position
    :documentation "Position of valve"))

 (:background
  :initial-mode closed)
    
 (open  :type :ok-mode
	:documentation ""
	:model (= (position ?name) open )
	:transitions ((close
		       :when (= (cmd-in ?name) close)
		       :next closed)
		      (:otherwise :persist)))

 (closed  :type :ok-mode
	  :documentation ""
	  :model (= (position ?name) closed)
	  :transitions ((open
			 :when (= (cmd-in ?name) open)
			 :next open)
			 (:otherwise :persist)))

 (stuck-closed :type :fault-mode
	       :probability unlikely
	       :documentation ""
	       :model (= (position ?name) closed)
	       :transitions ((:otherwise :persist)))

 (stuck-open :type :fault-mode
	     :probability unlikely
	     :documentation ""
	     :model (= (position ?name) open)
	     :transitions ((:otherwise :persist)))
 )

(defcomponent driver (?name)
 (:inputs
  ((cmd-in ?name) :type driver-cmd :documentation "Command to driver"))

 (:outputs
  ((cmd-out ?name) :type valve-cmd :documentation "Output to driven device")
  ((checksum ?name)
   :type checksum :documentation "A status signal output by the driver"))
 
 (:background
  :initial-mode off)
    
 (off  :type :ok-mode
       :documentation ""
       :model (= (cmd-out ?name) no-command )
       :transitions ((turn-on 
		      :when (= (cmd-in ?name) on)
		      :next on)
		     (:otherwise :persist)))

 (on  :type :ok-mode
      :documentation ""
      :model  (and
	       ;; If the checksum doesn't check, there we don't 
	       ;; trust the driver.  This will force it to be recoverable
	       ;; if we have powered it on and get a bogus checksum.
	       ;; Usually you would have a more complex model of a 
	       ;; device's operation than this.
	       (= (checksum ?name) ok)

	       (if (= (cmd-in ?name) open)
		   (= (cmd-out ?name) open)
		 (if (= (cmd-in ?name) close)
		     (= (cmd-out ?name) close)
		   (= (cmd-out ?name) no-command))))
      :transitions ((turn-off 
		     :when (= (cmd-in ?name) off)
		     :next off)
		    (:otherwise :persist)))

 (recoverable  :type :fault-mode
	       :probability unlikely
	       :model (= (cmd-out ?name) no-command )
	       :documentation ""
	       :transitions ((cut-power
			      :when (= (cmd-in ?name) off)
			      :next off)
			     (:otherwise :persist))))
  
(defcomponent valve-drive-electronics-control-unit (?name)
 (:inputs
  ((cmd-in ?name) :type vdecu-cmd :documentation "Command to VDECU")
  ((driver1-cmd-in ?name) :type driver-cmd  :documentation "Cmd to driver1")
  ((driver2-cmd-in ?name) :type driver-cmd  :documentation "Cmd to driver2"))

 (:outputs
  ((driver1-cmd-out ?name) :type driver-cmd :documentation "Out to driver1")
  ((driver2-cmd-out ?name) :type driver-cmd :documentation "Out to driver2"))

 (:background
  :initial-mode off)

 (off  :type :ok-mode
       :model (and
	       (= (driver1-cmd-out ?name) no-command)
	       (= (driver2-cmd-out ?name) no-command))
       :transitions ((turn-on 
		      :when (= (cmd-in ?name) on)
		      :next on)
		     (:otherwise :persist)))

 (on  :type :ok-mode
      :model (and (= (driver1-cmd-in ?name)
		     (driver1-cmd-out ?name))
		  (= (driver2-cmd-in ?name)
		     (driver2-cmd-out ?name)))
      :transitions ((turn-off 
		      :when (= (cmd-in ?name) off)
		      :next off)
		    (:otherwise :persist))))


(defmodule valve-and-sensor (?name)
  (:inputs 
   ((cmd-in ?name)  :type valve-cmd)) 

  (:outputs
   ((reported-position ?name)  :type valve-position)) 

  (:structure
   (valve-actuator (actuator  ?name))
   (valve-sensor (sensor ?name)))

  (:connections
   (and
    (= (sensed-position (sensor ?name)) (position (actuator ?name)))
    (= (cmd-in ?name) (cmd-in (actuator ?name)))
    (= (reported-position ?name) (reported-position (sensor ?name))))))


(defmodule vdecu-drivers-and-valves ()
  (:structure
   (valve-drive-electronics-control-unit vdecu)
   (driver driver-1)  
   (driver driver-2)  
   (valve-and-sensor valve-1)  
   (valve-and-sensor valve-2)  
   (valve-and-sensor valve-3)  
   (valve-and-sensor valve-4)  
   (valve-and-sensor valve-5)  
   (valve-and-sensor valve-6)  
   )
  (:connections
   (and
    (= (driver1-cmd-out vdecu) (cmd-in driver-1))
    (= (driver2-cmd-out vdecu) (cmd-in driver-2))
    (=  (cmd-out driver-1) (cmd-in valve-1))
    (=  (cmd-out driver-1) (cmd-in valve-2))
    (forall ?valve in (valve-1 valve-2)
	    (=  (cmd-out driver-1) (cmd-in ?valve)))

    (forall ?valve in (valve-3 valve-4 valve-5 valve-6)  
	    (=  (cmd-out driver-2) (cmd-in ?valve))))))


; A function to instantiate the module

(defun create-valves()
  ;; Instantiate the model, monitors and commands within WITH-MRP,
  ;; so that the reactive planner will know about the model. We
  ;; can later ask the reactive planner for recovery plans.

  (with-mrp
   ;; create a vdecu-driver-and-valves module, which will
   ;; instantiate all of its subcomponents
   (instantiate-module '(vdecu-drivers-and-valves)) 

   (new-command-table)
   (new-monitor-table)

   ;; Create a command which will assert a value 
   ;; for (cmd-in vdecu), allow the vdecu
   ;; transition to be inferred, then reset to
   ;; (cmd-in vdecu) to the default.  By convention
   ;; new-command uses no-command as the default idle command. 

   (new-command '(cmd-in vdecu))
   (new-command '(driver1-cmd-in vdecu))
   (new-command '(driver2-cmd-in vdecu))

   ;; Create monitors for our driver checksums
   (new-monitor '(checksum driver-1))
   (new-monitor '(checksum driver-2))

   ;; Create monitors which will assert persistent values for valve-n
   (dolist (valve '(valve-1 valve-2 valve-3 valve-4 valve-5 valve-6))
	   (new-monitor `(reported-position ,valve)))

  ;; Have Livingstone report its estimate of the position of each
  ;; valve actuator
   (dolist (valve '(valve-1 valve-2 valve-3 valve-4 valve-5 valve-6))
	   (export-attribute  `(position (actuator ,valve))))
   )
  )


;; Testing the module
(defun test-valves () 
  (format t "~&~%-->Creating valve module...~%")
  (create-valves)

  (format t "~&~%-->Receive first monitor values for valve positions..~%")
  (do-monitors '((closed (reported-position valve-1))
		 (closed (reported-position valve-2))
		 (closed (reported-position valve-3))
		 (closed (reported-position valve-4))
		 (closed (reported-position valve-5))
		 (closed (reported-position valve-6))))


  (format t "~&~%-->Asking how to get valve-1 open~%")
  (print-reconfig-sequence '(= (actuator valve-1) open))

  (format t "~&~%-->Giving vdecu command on~%")
  (do-cmd  '(cmd-in vdecu) 'on )

  (format t "~&~%-->Giving vdecu command to turn on driver 1 ~%")
  (do-cmd  '(driver1-cmd-in vdecu) 'on '( (ok (checksum driver-1)))) 

  (format t "~&~%-->Giving vdecu command to open valves on driver 1~%")
  (do-cmd  '(driver1-cmd-in vdecu) 'open 
	   '((open (reported-position valve-1))
	     (open (reported-position valve-2))))

  (format t "~&~%-->Asking how to get valve-3 open~%")
  (print-reconfig-sequence '(= (actuator valve-3) open))

  (format t "~&~%-->Giving command to turn on driver 2, but it goes wrong ~%")
  (do-cmd  '(driver2-cmd-in vdecu) 'on '( (corrupted (checksum driver-2))))

  (format t "~&~%--> Again asking how to get valve-3 open~%")
  (print-reconfig-sequence '(=  open (actuator valve-3)))

  (format t "~&~%-->Livingstone recommends we powercycle the driver...~%")
  (do-cmd  '(driver2-cmd-in vdecu) 'off)
  (do-cmd  '(driver2-cmd-in vdecu) 'on '( (ok (checksum driver-2))))

  (format t "~&~%-->Giving vdecu command to open all valves on driver 2~%")
  (do-cmd  '(driver2-cmd-in vdecu) 'open
	   '((open (reported-position valve-3))
	     (open (reported-position valve-4))
	     (open (reported-position valve-4))
	     (open (reported-position valve-5))
	     (open (reported-position valve-6))))
  )

