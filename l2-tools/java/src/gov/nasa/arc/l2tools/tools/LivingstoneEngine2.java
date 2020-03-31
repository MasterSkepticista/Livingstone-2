//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

import java.util.*;
import java.io.File;
//import org.xml.sax.*;
import livdll.*;
import javax.swing.DefaultListModel;

/* Java wrapper for the C++ Livingstone engine.  Multiple instances can
   be created. */
public class LivingstoneEngine2 extends LivingstoneEngine implements EngineControlInterface {

  /* Records which model was loaded */
  // no private, since it is set in LivingstoneEngineAdapter.loadModel
  public String modelFilename = null;

  /* This is the last list of candidates. I need this to map from pointers to candidate index.
   * The index of a candidate is needed to load it, but it can change after a fc.  The pointer
   * is the only stable identifier, so a I need a mapping. */
  private Candidate[] candidates = new Candidate[0];
  private Candidate[] candidateClasses = new Candidate[0];

  private Attribute[] attributes = new Attribute[0];

  /* Candidates are hashed by pointer. */
  private Hashtable candidateMap = new Hashtable();

  /* This is set by the constructors of subclasses. */
  protected int search_tracker_type;
  
  /* Don't use this constructor directly.  Use the subclasses static factory methods instead. */
  protected LivingstoneEngine2(int pointer, int debug_pointer) {
    super( pointer, debug_pointer);
  }

  //
  // Accessors
  //

  public int getType() { return search_tracker_type; }

  public String getModelFilename() { return modelFilename; }

  //
  // Main Engine Operations
  //

  // loadModel is no longer an engine object operation, since the engine object
  // is not created until LivingstoneEngineAdapter.newEngine and
  // LivingstoneEngineAdapter.loadModel have been called

//   public synchronized void loadModel(String filename) throws L2NativeException {
//     if (modelFilename!=null) {
//       System.out.println("Tried to load a second file into this engine.  First="+modelFilename+" second="+filename);
//       return;
  //     }
//     loadModelJNI(filename);
//     modelFilename = filename;
//     //    loadHarness(filename+".hrn");
//   }

  /* This moves Livingstone ahead in time via issuing a command.*/
  public synchronized void minProgress(String cmd) throws L2NativeException {
    minProgressJNI(cmd);
  }

  /* This moves Livingstone ahead in time via issuing a command.*/
  public synchronized void fullProgress(String cmd) throws L2NativeException {
    fullProgressJNI(cmd);
  }

  /* provided for consistency with assign() */
//   public synchronized void progress(String cmdname, String value) throws L2NativeException {
//     progressJNI(cmdname+"="+value);
//     assignJNI(cmdname,value);
//   }

  public synchronized boolean propagate() throws L2NativeException {
    boolean consistent = (1==propagateJNI());
    return consistent;
  }

  /* map assign(var,unknown) to unassign(var) */
  /* handle n var=val tokens */
  public synchronized void assign(String monitor) throws L2NativeException {
    // System.out.println("LivingstoneEngine2.assign: monitor " + monitor);
    StringTokenizer varVals = new StringTokenizer( monitor, " ");
    int numVarVals = varVals.countTokens();
    String[] varValArray = new String[numVarVals * 2];
    String[] pair = new String[2];
    for (int i = 0; i < numVarVals; i++) {
      pair = breakAtSubstring(varVals.nextToken(), "=");
      varValArray[i * 2] = pair[0].trim();
      varValArray[(i * 2) + 1] = pair[1].trim();
    }
    String newMonitor = new String( "");
    boolean first = true;
    for (int i = 0; i < numVarVals; i++) {
      // System.out.println("i " + i + " " + varValArray[i * 2] + " " + varValArray[(i * 2) + 1]);
      if ("unknown".equalsIgnoreCase( varValArray[(i * 2) + 1]) ||
          "<Unassigned>".equalsIgnoreCase( varValArray[(i * 2) + 1])) {
        // System.out.println("  unknown " + varValArray[i * 2]);
        unassignJNI(varValArray[i * 2]);
      } else {
        if (first == true) {
          first = false;
        } else {
          newMonitor = newMonitor.concat( " ");
        }
        newMonitor = newMonitor.concat( varValArray[i * 2]);
        newMonitor = newMonitor.concat( "=");
        newMonitor = newMonitor.concat( varValArray[(i * 2) + 1]);

        // test assignJNI (comment out assignMultipleJNI below)
        // assignJNI(varValArray[i * 2], varValArray[(i * 2) + 1]);
        // System.out.println("LivingstoneEngine2.assign: " + varValArray[i * 2] + "=" +
        //                    varValArray[(i * 2) + 1]);
      }
    }
    // System.out.println("LivingstoneEngine2.assign: newMonitor " + newMonitor);
    if (! "".equalsIgnoreCase( newMonitor))
      assignMultipleJNI( newMonitor);
  }
  
  public synchronized void unassign(String variable) throws L2NativeException {
    unassignJNI(variable);
  }
  
  public synchronized boolean parseCmd (String cmd, String args) throws L2NativeException {
    boolean success = parseCmdJNI( cmd, args);
    return success;
  }

  public synchronized void listenerUsage() throws L2NativeException {
    listenerUsageJNI();
  }

  // ********************************************************************************
  /* These block and return a value from the engine very quickley. */

  public synchronized int[] getHistoryStartStop() throws L2NativeException {
    return getHistoryStartStopJNI();
  }

  public synchronized void execWarpCommands(String commands) throws L2NativeException {
    executeWarpCommandsJNI(commands);
  }

  /* This nay no longer be needed but I'm leaving it in anyway. */  
  public synchronized int getCandidateCount() throws L2NativeException {
    return getCandidateCountJNI();
  }

  public synchronized int getTimeStep() throws L2NativeException {
    return getTimeStepJNI();
  }

  public synchronized void buildAttributes() {}
  
  public synchronized Attribute[] getAttributes() { return attributes; }

  public synchronized PropValueResponse getPropValue(String propName) throws L2NativeException {
    int result = getPropValueJNI(propName);
    return PropValueResponseArray[result];
  }

  public static class PropValueResponse {
    public String msg = "";
    private PropValueResponse(String msg) { this.msg = msg; }
  }

  public static PropValueResponse PROP_NOT_FOUND = new PropValueResponse("not_found");
  public static PropValueResponse PROP_TRUE = new PropValueResponse("true");
  public static PropValueResponse PROP_FALSE = new PropValueResponse("false");
  public static PropValueResponse PROP_UNKNOWN = new PropValueResponse("unknown");

  protected static PropValueResponse[] PropValueResponseArray = {PROP_NOT_FOUND,PROP_TRUE,PROP_FALSE,PROP_UNKNOWN};

  // ********************************************************************************
  /* These block and then produce an event, and should be executed from their own 
   * thread to keep the gui alive.  The replies should be cued on the event thread
   * when they arrive at the listeners. */

  public synchronized void findCandidates( String command) throws L2NativeException {
    // System.out.println("==== LivingstoneEngine2.findCandidates() called ===== command " + command);
      
    findDiscrepancys(); //Added by RG
    
    if ("prune-search".equalsIgnoreCase(command))
      // findCandidatesJNI();
      pruneAndSearchJNI();
    else if ("find-fresh".equalsIgnoreCase(command))
      findFreshJNI();
    else if ("extend".equalsIgnoreCase(command))
      extendJNI();
    else if ("fc".equalsIgnoreCase(command)) {
      // fc or find-candidates -- which variant?
      String fcStyle = getFcStyleJNI();
      // the L2 command line cmd is "prune-search" 
      if (fcStyle.equals( "prune-and-search"))
        fcStyle = "prune-search";
      System.out.println( "Skunkworks.command(" + fcStyle + ")");
      if (fcStyle.equals( "prune-search"))
        pruneAndSearchJNI();
      else if (fcStyle.equals( "find-fresh"))
        findFreshJNI();
      else if (fcStyle.equals( "extend"))
        extendJNI();
      else {
        System.err.println("LivingstoneEngine2.findCandidates: fcStyle " +
                                     fcStyle + " not handled");
        System.exit( 0);
      }
    } else if("updateCandidates".equalsIgnoreCase(command)) {
      // used by checkpoint restore 
    } else {
      System.err.println("LivingstoneEngine2.findCandidates: command " +
                         command + " not handled");
      System.exit( 0);
    }
    int n = getCandidateCountJNI();
    candidates = new Candidate[n];
    if(n > 0) {
      for(int i = 0; i < n; i++) {
        int pointer = getCandidatePointerJNI(i);
        getCandidateAssigments( candidates, pointer, i);
      }
    }
    int m = getCandidateClassCountJNI();
    candidateClasses = new Candidate[m];
    if (m > 0) {
      for(int i = 0; i < m; i++) {
        int pointer = getCandidateClassPointerJNI(i);
        getCandidateAssigments( candidateClasses, pointer, i);
      }
    }
  }

  private synchronized void getCandidateAssigments( Candidate[] candidates,
                                                    int pointer, int i)
    throws L2NativeException {
    final Vector ca = new Vector();
    mapOverCandidateAssignmentsJNI( pointer, new Observer() {
        public void update( Observable o, Object arg) {
          ca.add( arg);
        }
      });
    CandidateAssignment[] assns = new CandidateAssignment[ca.size()];
    if(ca.size() > 0) ca.copyInto( assns);
    candidates[i] = new Candidate( pointer, assns);
  }

  public synchronized Candidate[] getCandidates() {
    return candidates;
  }

  public synchronized Candidate[] getCandidateClasses() {
    return candidateClasses;
  }

  public synchronized String getSearchTermination() throws L2NativeException {
    return getSearchTerminationJNI();
  }


  /* I need this to map from pointers to candidate index.
   * The index of a candidate is needed to load it, but it can change after a fc.  The pointer
   * is the only stable identifier, so a I need a mapping. */
  private int getCandidateIndex(Candidate c) throws IndexOutOfBoundsException {
    // System.err.println("getCandidateIndex length " +  candidates.length);
    for(int i = 0; i < candidates.length; i++) {
      // System.err.println("   i " + i + " p1 " + c.getPointer());
      // System.err.println("   i " + i + " p2 " + candidates[i].getPointer());
      if(c.getPointer() == candidates[i].getPointer()) return i;
    }
    // instead of throwing IndexOutOfBoundsException, return -1
    //throw new IndexOutOfBoundsException();
    return -1;
  }


  // used by Candidate Manager
  public synchronized AttributeAssignment[][] getAttributeAssignments(Candidate candidate) throws L2NativeException, IndexOutOfBoundsException {
    if (candidate == null) {
      installCandidateJNI(0);
    } else {
      int candidateIndex = getCandidateIndex(candidate);
      if (candidateIndex != -1) {
        installCandidateJNI( candidateIndex );
      } else {
        // invalid index - return empty array
        return null;
      }
    }
    final Hashtable history = new Hashtable();
        
    mapOverAttributeAssignmentsJNI(new Observer() {
        MyComparator myComp = new MyComparator();
        public void update(Observable o, Object arg) {
          if(arg == null) return;
          AttributeAssignment aa = (AttributeAssignment)arg;
          // System.out.println("LivEng2.getAttributeAssignments: aa time " + aa.getTime() + " name " + aa.getName());
          Integer key = new Integer(aa.getTime());
          Vector state = (Vector)history.get(key);
          if(state == null) {
            state = new Vector();
            history.put(key, state);
          }
          int index = Collections.binarySearch(state, aa.getName(), myComp);
          if(index < 0) index = -1-index;
          state.add(index, aa);
        }
      });

    int[] ss = getHistoryStartStopJNI();
    // System.out.println("LivEng2.getAttributeAssignments: ss[0] " + ss[0] + " ss[1] " + ss[1] + " history.size() " + history.size());
    // history.size() should be equal to (ss[1] - ss[0] + 1) 
    //    AttributeAssignment[][] ret = new AttributeAssignment[history.size()][];
    AttributeAssignment[][] ret = new AttributeAssignment[ss[1] - ss[0] + 1][];
    if(ss[1] > ss[0]) {
      for(int time = ss[0]; time <= ss[1]; time++) {
        Integer key = new Integer(time);
        Vector state = (Vector)history.get(key);
        if (state == null) {
          // fill in history for idle progress which has no mode or modeTransition
          // variables with its time value
          state = new Vector();
          history.put(key, state);
        }
        ret[time-ss[0]] = new AttributeAssignment[state.size()];
        state.copyInto(ret[time-ss[0]]);
      }
    }
    return ret;
  }

  // Second implementation - used by History Table
  public synchronized AttributeAssignment[][] getAttributeAssignments2(Candidate c, Collection names)
       throws IndexOutOfBoundsException, L2NativeException {
	 // System.err.println("LivingstoneEngine2>>getAttributeAssignments2");
         // System.err.println("   length " +  candidates.length + " count " + getCandidateCountJNI());
         // prevent "L2NativeException: Install candidate index exceeds candidate number."
         // when no candidates are found
         if ((candidates.length > 0) && (getCandidateCountJNI() > 0)) {
           if (c == null) {
             installCandidateJNI(0);
             // System.err.println("  c null count " + getCandidateCountJNI());
           } else {
             // System.err.println("  c " +  getCandidateIndex(c) + " count " + getCandidateCountJNI());
             int candidateIndex = getCandidateIndex(c);
             if (candidateIndex != -1) {
               installCandidateJNI( candidateIndex );
             } else {
               return null;
             }
           }
         }

	 //    if (names==null) names = getAttributeNames();
	 if (names==null) throw new IndexOutOfBoundsException("getAttributesAssignments2 called with null list");
	 int ss[] = getHistoryStartStop();

	 //System.out.println("getAttributeAssignments2: ss="+ss);

	 AttributeAssignment result[][] = new AttributeAssignment[ss[1]-ss[0]+1][names.size()];

	 int index = 0;
	 String name = "";
	 String value = "";
         boolean exogenous = false;
	 for (Iterator i=names.iterator();i.hasNext();) {
	   name = (String)i.next();
	   for (int time=ss[0];time<=ss[1];time++) {
	     value = "<undefined>";
             // do not get value of modeTransition varibles at current time
             // since variableValueAtTimeJNI prints an error msg
             // Java_LivingstoneEngine_variableValueAtTimeJNI: No such variable `test.cb11.modeTransition'
             if ((time == ss[1]) && (name.indexOf("modeTransition") >= 0))
               value = "<undefined>";
             else {
               try {
                 value = variableValueAtTime(name,time);
                 exogenous =variableExogenousAtTime(name,time);
                 if (exogenous) {
                   value = value + "(e)";
                 } else {
                   value = value + "(i)";       
                 } 
               } catch (L2NativeException e) {};
             }
	     // System.err.println("getAttributeAssignments2 name="+name+"  time="+time+" value="+value);
	     result[time-ss[0]][index] = new AttributeAssignment(name,value,time);
	   }
	   index++;
	 }
	 return result;
  }

  /* Returns an alphabetically sorted set of strings naming the attributes */
  public synchronized SortedSet getAttributeNames() throws L2NativeException {
    final SortedSet names = new TreeSet();
    mapOverAttributeNamesJNI(new Observer() {
        public void update(Observable o, Object arg) {
          if (!(arg instanceof String)) return;
          String name = (String)arg;
          // System.out.println("LE2:getAttributeNames name " + name);
          if (!name.startsWith("Start."))
            names.add(name);
	  if (name.endsWith(".mode")) 
	    names.add(name+"Transition");
        }
      });
    return names;
  }

  public synchronized boolean restoreCheckpoint( String name) throws L2NativeException {
    return restoreCheckpointJNI( name);
  }

  // JNI testing only
  /* Returns an alphabetically sorted set of strings naming the commandable attributes */
  public synchronized SortedSet getCommandAttributeNames() throws L2NativeException {
    final SortedSet names = new TreeSet();
    mapOverCommandAttributeNamesJNI(new Observer() {
        public void update(Observable o, Object arg) {
          if (!(arg instanceof String)) return;
          String name = (String)arg;
          names.add(name);
        }
      });
    return names;
  }

  // JNI testing only
  /* Returns an alphabetically sorted set of strings naming the observable attributes */
  public synchronized SortedSet getObservableAttributeNames() throws L2NativeException {
    final SortedSet names = new TreeSet();
    mapOverObservableAttributeNamesJNI(new Observer() {
        public void update(Observable o, Object arg) {
          if (!(arg instanceof String)) return;
          String name = (String)arg;
          names.add(name);
        }
      });
    return names;
  }

  // JNI testing only
  public synchronized boolean setAttributeMode( String attributeName, String mode)
    throws L2NativeException {
    return setAttributeModeJNI( attributeName, mode);
  }

  // JNI testing only
  public synchronized void createCheckpoint( String name) throws L2NativeException {
    createCheckpointJNI( name);
  }

  // JNI testing only
  public synchronized void deleteCheckpoint( String name) throws L2NativeException {
    deleteCheckpointJNI( name);
  }

  // JNI testing only
  public synchronized void addAssumption( String args) throws L2NativeException {
    StringTokenizer varVal = new StringTokenizer( args, " ");
    String[] pair = new String[2];
    pair = breakAtSubstring(varVal.nextToken(), "=");
    addAssumptionJNI( pair[0].trim(), pair[1].trim());
  }

  // ********************************************************************************
  // Fault simulation and insertion
  //

  /* implicitly sets the transition variable.  If you want to set the
     mode variable directly, use assign(). */
  public void setMode(String modename, String value) throws L2NativeException {
    setTransitionVariable(modename+"Transition",value);
  }

  /* A synonym for setMode() */
  public void insertFault(String modename, String value) throws L2NativeException {
    setMode(modename, value);
  }
    
  /* Set a transition variable at the latest time */
  public void setTransitionVariable(String name, String value) throws L2NativeException {
    setTransitionVariableAtTime(name,value,getTimeStep()-1);
  }

  /* set a transition variable at any specified time */
  public synchronized void setTransitionVariableAtTime(String name, String value, int time) throws L2NativeException {
    if (time<1) throw new Error("You cannot set a transition variable in time 0.  Jim will fix this.");
    setTransitionVariableAtTimeJNI(name,value,time);    
  }
  
  
  
  //Added by RG
  //****************************************************************************
  // Support for Discrepancies.  Queries Livingstone Engine for all discrepant
  // commands and observations resulting from a conflict.
  //Note, Discrepancies are flushed by the L2 engine, once a Find Candidates
  //is processed
  //****************************************************************************
 
  
  //CallBack used to return L2 results/discrepancies.
  //Function is called from  findDiscrepancysJNI (refer to Livdll.cpp)
  final Vector discrepancys = new Vector(); 
  
  public synchronized void findDiscrepancys() throws L2NativeException{ 
      
      discrepancys.clear();
      findDiscrepancysJNI(new Observer(){
        public void update( Observable o, Object arg){
          discrepancys.add(arg);  
        }
      });
  } 
  
  public Vector getDiscrepancys() {return discrepancys;}
  
  //***************************************************************************
  
  
  
  // ********************************************************************************
  // Justification support
  //

  /* Return a description of the justification for a proposition.
     Blocks and returns quickly. */
  public synchronized JustificationStruct justificationStruct(String propname) throws L2NativeException {
    int result = 0;
    try {
      tempJustificationStruct = new JustificationStruct();            
      result = justifyPropositionJNI(propname);
      // System.out.println("JustificationStruct result="+result);
      if (result==3) return tempJustificationStruct;
    } finally {
      tempJustificationStruct = null;
    }
    return null;
  }

  /* Temporary variable used by justificationStruct().  That method is
     synchronized, so this is only used by one call at a time. */
  protected JustificationStruct tempJustificationStruct = null;

  /* Callback from justifyPropositionJNI() */
  public void mapOverJustificationPositive(String prop) {
    //System.out.println("mapOverJustificationPositive: "+prop);
    if (tempJustificationStruct!=null)
      tempJustificationStruct.positive_antecedents.add(prop);

  }

  /* Callback from justifyPropositionJNI() */
  public void mapOverJustificationNegative(String prop) {
    //System.out.println("mapOverJustificationPositive: "+prop);
    if (tempJustificationStruct!=null)
      tempJustificationStruct.negative_antecedents.add(prop);
  }

  public void mapOverPropositionStringsCallback(String msg) {
    System.out.println("LivingstoneEngine2.mapOverPropositionStringsCallback() not implemented.");
  }

  /* Used in the creation of justifications */
  public class JustificationStruct {
    public Vector positive_antecedents = new Vector();
    public Vector negative_antecedents = new Vector();
  }

  public synchronized String variableValueAtTime(String vname, int time) throws L2NativeException {
    return variableValueAtTimeJNI(vname,time);
  }

  public synchronized boolean variableExogenousAtTime(String vname, int time) throws L2NativeException {
    return variableExogenousAtTimeJNI(vname,time);
  }

  /*
  public synchronized String variableValueAtTime(String vname, int time) throws L2NativeException {
    return variableValueAtTimeJNI(vname,time);
  }
  */

  public synchronized void printPropClauses(String propname) {
    // System.err.println("printPropClauses propname " + propname);
    // remove (e) or (i) appended to values for History Table display
    int index = propname.lastIndexOf("(");
    if (index != -1) {
      propname = propname.substring(0, index);
    }
    String[] pair = new String[2];
    pair = breakAtSubstring( propname, "=");
    if ("<undefined>".equalsIgnoreCase(pair[1] )) {
      System.err.println("\nCannot print connected clauses for `" + propname + "'");
      System.err.print("======> ");
    } else {
      printPropClausesJNI(propname);
    }
  }


  /* returns an array of two strings.  The first is the string before
     the first occurance of pattern.  The second is the string after
     the first occurance.  If pattern does not occur in line, then return
     [<line>,""]. */
  public static String[] breakAtSubstring(String line, String pattern) {
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

  // obsolete
//   public static void main(String[] args) {
//     try {
//       L2_ptr = LivingstoneCBFSEngine.setCBFSEngineParams( 5, 100, 100, 3, 5, "min", "prune-search");
//       engine.helloWorldJNI();
//       engine.loadModel("/home/stephen/skunkworks/stanley-sample-user-files/livingstone/models/modules/cbAndLeds");
//       LivingstoneCBFSEngine.factory( engine);

//       SortedSet list = engine.getAttributeNames();
//       if(list.size() > 0) {
//         for(Iterator i=list.iterator();i.hasNext();) {
//           System.out.println(i.next());
//         }
//       }
//     } catch(L2NativeException e) {
//       System.out.println(e);
//     }
//   }

}
