//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package forwill;

import java.util.*;
import java.io.*;
import livdll.L2NativeException;
import java.awt.Component;

import gov.nasa.arc.l2tools.tools.*;
import gov.nasa.arc.l2tools.tools.historyTable.HistoryTable;
import gov.nasa.arc.l2tools.util.OrbServices;

public class Server extends LivingstoneCorba._LivingstoneEngineManagerImplBase {

  public static Server server;

  //  private GUIFrame gf = new GUIFrame();
  private EngineManager engineManager = new EngineManager();
  private CommandLine commandLine;
  private CandidateManager candidateManager;
  private HistoryTable historyTable;
  private StanleyAdapter stanleyAdapter;

  public Server() {
    super();
    OrbServices.getOrb().connect(this);
    engineManager.createDataModel(0);
    commandLine = (CommandLine) engineManager.newCommandLineTool(0);
    candidateManager = (CandidateManager) engineManager.newCandidateManagerTool(0);
    candidateManager.setVisible(true);
    historyTable = (HistoryTable) engineManager.newHistoryTableTool(0);
    historyTable.setVisible(true);
    commandLine.addHistoryTable( historyTable);
    stanleyAdapter = (StanleyAdapter) engineManager.newStanleyTool(0, commandLine);
    // make commandLine avaiable to HistoryTable via getCommandLine
    server = this;
    try {
      // Thread.sleep(3000);
      // slower machines crash with this value (release 1.4), try
      Thread.sleep(6000);
    } catch(InterruptedException e) {
      System.out.println(e);
    }
    run();
  } // end constructor

  public CommandLine getCommandLine() {
    return commandLine;
  }

  public void run() {
    String ior = OrbServices.getOrb().object_to_string(this);
    try {
      OutputStreamWriter o = new OutputStreamWriter(new FileOutputStream("stanleyengine.ior"));
      o.write(ior);
      o.flush();
      o.close();
    } catch (Exception e) { e.printStackTrace(); }
    System.out.println("ior="+ior);
    System.out.println("waiting ....");

    // L2Tools Command Line
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    try {
      System.out.println("Type 'exit' to quit L2Tools.");
      String line = new String("");
      while (true) {
        System.err.print("======> ");
        line = in.readLine();
        if (! ("".equalsIgnoreCase(line))) {
	  // Handle the echo
	  // System.out.println(line);
	  if ("exit".equalsIgnoreCase(line)) {
            // destroy the engine so that L2 will release its allocated storage
            // System.err.println("forwill.Server: exit from L2Tools command line");
            DataModel model = commandLine.getModel();
            ScenarioInterface pipeline = model.getPipeline();
            try {
              pipeline.destroyEngine(commandLine);
            } catch (livdll.L2NativeException e) {
              System.err.println("forwill.Server.run: " + e);
            }
            System.exit(0);
          }
          try {
            line = line.trim();
          } catch (Exception e) {
            // Control-D will get here
            System.exit(1);
          }
              
	  // Is this a comment?
	  if (line.length()>1 && line.substring(0,2).equals("//")) 
            return;
	  // separate the line's name from the arguments
          // special processing for getEngine at L2Tools cmd line
	  String[] pair = LivingstoneEngine2.breakAtSubstring(line," ");
	  if ("getEngine".equals(pair[0])) {
	      String args = pair[1];
	      if(args != null && args.length() > 3) {
		  Vector argsv = new Vector();
		  StringTokenizer st = new StringTokenizer(args);
		  while(st.hasMoreElements()) {
		      argsv.add((String)st.nextElement());
		  }
		  if(argsv.size() > 2) {
		      stanleyAdapter.getEngine((String)argsv.elementAt(0),
				   Integer.parseInt((String)argsv.elementAt(1)),
				   Integer.parseInt((String)argsv.elementAt(2)),
				   Integer.parseInt((String)argsv.elementAt(3)),
				   Integer.parseInt((String)argsv.elementAt(4)),
				   Integer.parseInt((String)argsv.elementAt(5)),
				   Integer.parseInt((String)argsv.elementAt(6)),
				   (String)argsv.elementAt(7),
				   (String)argsv.elementAt(8));
		  }
	      }
	  }
          // System.err.println("forwill/Server:run " + line);
          // pass other cmd line entries to Stanley Adapter
	  stanleyAdapter.command(line);
	}
      }
    } catch (java.io.IOException e) { e.printStackTrace(); }
    System.exit(0);
  }

  // Handling of Stanley CORBA msgs exit and getCommandLine 
  public void exit() { 
    // System.err.println("forwill.Server exit() called from Stanley");
    // destroy the engine so that L2 will release its allocated storage
    DataModel model = commandLine.getModel();
    ScenarioInterface pipeline = model.getPipeline();
    System.err.println("exit");
    try {
      pipeline.destroyEngine(commandLine);
    } catch (L2NativeException e) {
      System.err.println("forwill.Server.exit: " + e);
    }
    System.exit(0);
  }

  public String describe() { return "Server.java"; }

  public LivingstoneCorba.LivingstoneCommandLine getCommandLine
    (String search_method, int max_candidate_classes_returned,
     int max_candidates_returned, int max_candidates_searched,
     int max_cutoff_weight, int max_history_cutoff, int max_trajectories_tracked,
     String progress_cmd_type, String fc_cmd_type) {
    try {
      System.err.println("L2Tools.getCommandLine(" + search_method + " " +
                         max_candidate_classes_returned + " " +
                         max_candidates_returned + " " +
                         max_candidates_searched + " " + max_cutoff_weight + " " +
                         max_history_cutoff + " " +
                         max_trajectories_tracked + " " + progress_cmd_type + " " +
                         fc_cmd_type + ")");
      stanleyAdapter.clearLivingstoneEventListeners();
      stanleyAdapter.getEngine(search_method, max_candidate_classes_returned,
                               max_candidates_returned,
                               max_candidates_searched, max_cutoff_weight,
                               max_history_cutoff, max_trajectories_tracked,
                               progress_cmd_type, fc_cmd_type);
    } catch(Exception e) {
      System.out.println(e);
    }
    // System.err.println("Server - getCommandLine exit");
    // need prompt here
    System.err.print("======> ");
    return stanleyAdapter;
  }

  public LivingstoneCorba.LivingstoneCommandLine getRunningCommandLine() {
    stanleyAdapter.clearLivingstoneEventListeners();
    return stanleyAdapter; 
  }


  public static void main(String[] args) {
    new Server();
  }

}
