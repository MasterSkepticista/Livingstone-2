/*
* Author : Mark Shirley
* Description : SimpleCommandLine.java
*
* CHANGELOG:
* 03/16/00      Mhs     Created
* 03/22/00      Mhs     Added support for environment vars and multiple engines
*/

//package gov.nasa.arc.skunkworks.commandline;

import java.io.*;
import java.util.*;
import java.awt.event.*;

import javax.swing.*;

import LivingstoneEngine;

/* Implements a simple CLI for the C++ livingstone engine.   */
public class SimpleCommandLine {

    /** name -> engine map (this supports multiple engines) */
    Hashtable engineMap = new Hashtable();

    /** holds the engine to which commands are being issued */
    protected LivingstoneEngine engine = null;

    /** "Shell" variables */
    Hashtable env = new Hashtable();

    // Local cache of a few shell variables
    String prompt = "prompt> ";
    boolean echo = false;
    boolean ignoreErrors = true;

    /* Input stream */
    BufferedReader in = null;

    // Related to running scenarios from a multi-scenario file
    String scenario = null;
    boolean disabled = false;

    Hashtable actions = new Hashtable();

    public static void main(String[] args) {
        (new SimpleCommandLine()).readEvalPrint();
    }

    public void readEvalPrint() {
        try {
            engine = LivingstoneEngine.cbfsFactory("engine",1);
            readEvalPrint("prompt> ", null, false, false, new BufferedReader(new InputStreamReader(System.in)));
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            LivingstoneEngine.deallocate(engine);
            engine = null;
        }
    }

    /* a version of readEvalPrint that throws its exceptions */
    public void readEvalPrint(String thePrompt,
                              String theScenario, 
                              boolean theEcho, 
                              boolean theIgnoreErrors, 
                              BufferedReader theInputStream) 
         throws Exception 
    {
        prompt = thePrompt;
        scenario = theScenario;
        echo = theEcho;
        ignoreErrors = theIgnoreErrors;
        in = theInputStream;

        env.put("Prompt",prompt);
        env.put("Echo",theEcho ? Boolean.TRUE : Boolean.FALSE);
        env.put("IgnoreErrors",ignoreErrors ? Boolean.TRUE : Boolean.FALSE);

        if (scenario!=null) disabled = true;

        String command;

        while (true) {
            try {
                // Read the command
                System.out.print(prompt);
                command = in.readLine();
                if (command==null) return;

                // Handle the echo
                if (echo) System.out.println(command);

                processCommandLine(command);

            } catch (Exception e) {
                e.printStackTrace();
                if (!ignoreErrors) return;
            }
        }
    }

    public void processCommandLine(String command) {
        // Clean up command
        command = command.trim();

        // Is this a comment?
        if (command.length()>1 && command.substring(0,2).equals("//")) 
            return;
        if (command.length()>4 && command.substring(0,5).equalsIgnoreCase("echo ") && !echo) {
            System.out.println(command);
            return;
        }

        // separate the command's name from the arguments
        String[] pair = breakAtSubstring(command," ");
        dispatchCommand(pair[0].trim(),pair[1].trim());
    }

    public void dispatchCommand(String cname, String args) {
        Action a = null;
        //System.out.println("  cname ->"+cname+"<-");
        //System.out.println("   args=->"+args+"<-");

        if ("exit".equalsIgnoreCase(cname)) System.exit(0);

        if ("".equals(cname)) {
        } else if ((a = (Action)actions.get(cname.toLowerCase()))!=null) {
            // Dispatch to an action
            a.actionPerformed(new ActionEvent(this,0,args));
        } else if ("loadmodel".equalsIgnoreCase(cname)) {
            engine.loadModel(args);
        } else if ("findCandidates".equalsIgnoreCase(cname) || "fc".equalsIgnoreCase(cname)) {
            engine.findCandidates();
        } else if ("progress".equalsIgnoreCase(cname)) {
            engine.progress(args);
        } else if ("conflict".equalsIgnoreCase(cname)) {
            engine.conflict();
        } else if ("assign".equalsIgnoreCase(cname)) {
            String[] pair = breakAtSubstring(args,"=");
            engine.assign(pair[0].trim(),pair[1].trim());
        } else if ("printpropositions".equalsIgnoreCase(cname) || "pps".equalsIgnoreCase(cname)) {
            engine.printPropositions();
        } else if ("cprops".equalsIgnoreCase(cname)) {
            engine.cprops(args);
        } else if ("propagate".equalsIgnoreCase(cname)) {
            engine.propagate();
        } else if ("candidates".equalsIgnoreCase(cname)) {
            System.out.println("there are "+engine.getCandidateCount()+" candidates.");
        } else if ("install".equalsIgnoreCase(cname)) {
            cmdInstall(args);
        } else if ("set".equalsIgnoreCase(cname)) {
            cmdSet(args);
        } else if ("run".equalsIgnoreCase(cname)) {
            cmdRun(args);
        } else if ("scenario".equalsIgnoreCase(cname)) {
            cmdScenario(args);
        } else if ("echo".equalsIgnoreCase(cname)) {
            // Do nothing.  Should have been taken care of.
        } else if ("printenv".equalsIgnoreCase(cname)) {
            for (Enumeration e=env.keys();e.hasMoreElements();) {
                Object key = e.nextElement();
                Object val = env.get(key);
                System.out.println("  "+key+"="+val);
            }
        } else if ("propval".equalsIgnoreCase(cname)) {
            LivingstoneEngine.PropValueResponse response = engine.getPropValue(args);
            System.out.println("response="+response.msg);
        } else {
            System.out.println("Unrecognized command: ->"+cname+"<-");
            System.out.println("Commands are:");
            System.out.println("    assign    <variable>=<value> - assign a variable in the current time");
            System.out.println("    conflict                     - describe the current conflicts");
            System.out.println("    cprops    <pattern>          - print all propositions containing a substring");
            System.out.println("    findCandidates    or fc      - find candidates");
            System.out.println("    loadmodel <model-name>       - load a model file");
            System.out.println("    printPropositions or pps     - print all propositions");
            System.out.println("    progress  <cmd-assignment>   - issue a command");
            System.out.println("    propagate                    - (not used) run the TMS' unit propagation algorithm");
            System.out.println("    propval   <property>         - print the truth value of a single proposition");
            System.out.println("    set <environmentVar>=<value> - Set a control variable within this interpreter");
            System.out.println("---------------");
            System.out.println("    scenario <name>              - mark the start of a scenario");
            System.out.println("    run <file> [<name>]          - run the scenario <name> in file <file>");
            System.out.println("                                   If <name> is not specified, then run them all.");
            System.out.println("---------------");
            System.out.println("    printenv                     - display the CLI environment variables");
            System.out.println("---------------");
            for (Enumeration e=actions.keys();e.hasMoreElements();) {
                String key = (String)e.nextElement();
                Action act = (Action)actions.get(key);
                String name = (String)act.getValue(Action.NAME);
                String desc = (String)act.getValue(Action.SHORT_DESCRIPTION);
                System.out.print("    "+name);
                int count = 29-name.length();
                for (int c=0;c<count;c++) System.out.print(" ");
                System.out.println("- "+((desc==null) ? "<not documented>" : desc));
            }
            System.out.println("---------------");
            System.out.println("    exit");
        }
    }

    public void cmdSet(String args) {
        String[] pair = breakAtSubstring(args,"=");
        String var = pair[0].trim();
        String val = pair[1].trim();
        if ("prompt".equalsIgnoreCase(var)) {
            prompt = pair[1];   // don't use trimmed version
            env.put("prompt",prompt);
        } else if ("echo".equalsIgnoreCase(var)) {
            if ("true".equalsIgnoreCase(val)) {
                echo = true; 
            } else {
                echo = false;
            }
            env.put("Echo",echo ? Boolean.TRUE : Boolean.FALSE);
        } else if ("IgnoreErrors".equalsIgnoreCase(var)) {
            if ("true".equalsIgnoreCase(val)) {
                ignoreErrors = true; 
            } else {
                ignoreErrors = false;
            }
            env.put("IgnoreErrors", ignoreErrors? Boolean.TRUE : Boolean.FALSE);
        } else {
            env.put(var,val);
        }
    }

    public void cmdInstall(String args) {
        try {
            int num = Integer.parseInt(args);
            engine.installCandidate(num);
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }
    }

    public void cmdRun(String args) {
        String[] pair = breakAtSubstring(args," ");
        String file = pair[0].trim();
        String scenario = pair[1].trim();

        if (file.equals("")) {
            System.out.println("run <file> [<name>] - run the scenario <name> in file <file>");
            return;
        }
        if (scenario.equals("")) scenario = null; // conform to my internal convention

        System.out.println("Running the script ....");
        System.out.println("-------------------------");

        // Run the script

        System.out.println("pushing CLI of type "+this.getClass());
        SimpleCommandLine cli = null;
        try {
            cli = (SimpleCommandLine)this.getClass().newInstance();
        } catch (Exception e) {
            System.out.println("Exception while pushing a command line interpreter.");
            e.printStackTrace();
            System.out.println("Aborting....");
            return;
        }
        
        cli.engine = LivingstoneEngine.cbfsFactory(scenario,1);
        setChildCliState(cli);

        BufferedReader in = null;
        try {
            in = new BufferedReader(new FileReader(file));
            cli.readEvalPrint("",scenario,true,true,in);
        } catch (Exception e) { 
            e.printStackTrace();
        } finally {
            if (in!=null) try { in.close(); } catch (java.io.IOException e) { e.printStackTrace(); };
            LivingstoneEngine.deallocate(cli.engine);
        }
        System.out.println("-------------------------");
    }

    /** set up a child CLI.  The default implementation does nothing. */
    public void setChildCliState(SimpleCommandLine cli) { }

    public void cmdScenario(String name) {
        if (disabled) {
            if (name.equalsIgnoreCase(scenario)) {
                System.out.println("disabled = false");
                disabled = false;
            }
        } else {
            if (scenario!=null && !(name.equalsIgnoreCase(scenario))) {
                System.out.println("disabled = true");
                disabled = true;
            }
        }
    }

    //
    // Extended Actions
    //

    public void addAction(Action a) {
        String name = (String)a.getValue(Action.NAME);
        actions.put(name.toLowerCase(),a);
    }

    public void removeAction(Action a) {
        String name = (String)a.getValue(Action.NAME);
        actions.remove(name.toLowerCase());
    }

    //
    // Utilities
    //

    /* returns an array of two strings.  The first is the string before
       the first occurance of pattern.  The second is the string after
       the first occurance.  If pattern does not occur in line, then return
       [<line>,""]. */

    public String[] breakAtSubstring(String line, String pattern) {
        String[] result = new String[2];

        int i = line.indexOf(pattern);
        if (i<0) {
            result[0] = line;
            result[1] = "";
        } else {
            result[0] = line.substring(0,i);
            result[1] = line.substring(i+pattern.length());
        }
        return result;
    }

    public LivingstoneEngine getEngine() { return engine; }

}
