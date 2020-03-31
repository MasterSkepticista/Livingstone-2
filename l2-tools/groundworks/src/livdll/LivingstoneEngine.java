// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package livdll;

public class LivingstoneEngine extends LivingstoneEngineJNI {
    
    /* The address of the C++ Livingstone instance */
    protected int pointer = -1;  // params
    protected int debugPointer = -1;  // model
    
    /* Records which model was loaded */
    private String modelFilename = null;
    
    /* This is used by livdll. Weird, but it must be here. */
    private int search_tracker_type;
    public static final int CBFS = 1;
    public static final int COVER = 2;
    
    //
    // Accessors
    //
    
    public int getType() { return search_tracker_type; }
    public void setType(int type) { this.search_tracker_type = type; }
    
    //
    // Main Engine Operations
    //
    
    protected void deallocateEngine() {
        deallocateEngineJNI(pointer, debugPointer);
        pointer = -1;
        debugPointer = -1;
    }
    
    public String getModelFilename() { return modelFilename; }
    
    protected void setModelFilename(String filename) throws java.io.IOException, L2NativeException {
        if(debugPointer > -1) {
            // model is already loaded, lets dump it
            deallocateEngine();
            debugPointer = -1;
        }
        
        Parameters p = new Parameters();
        p.load(new java.io.FileInputStream(filename+".params"));
        
        pointer = setCBFSEngineParamsJNI(p.getMaxCBFSCandidateClasses(),p.getMaxCandidatesReturned(),p.getMaxCandidatesSearched(),p.getMaxCutoffWeight(),p.getMaxHistoryCutoff(),p.getMaxTrajectoriesTracked(),p.getProgressType(),p.getFcType());
        
        loadModelJNI(filename,pointer);        
        pointer = allocateCBFSEngineJNI(pointer);
        debugPointer = allocateCBFSDebuggerJNI(pointer);
        
        modelFilename = filename;
    }
    
    public boolean parseCmd(livdll.LivingstoneCommand command) throws L2NativeException {
        if(command.getType()!=command.COMMENT) {
            boolean success = parseCmdJNI( command.getCommand(), command.getArguments());
            return success;
        } else {
            return true;
        }
    }
    
    // ********************************************************************************
    /* These block and return a value from the engine very quickley. */
    
    public int[] getHistoryStartStop() throws L2NativeException {
        return getHistoryStartStopJNI();
    }
    
    /* This nay no longer be needed but I'm leaving it in anyway. */
    public int getCandidateCount() throws L2NativeException {
        return getCandidateCountJNI();
    }
    
    public int getTimeStep() throws L2NativeException {
        return getTimeStepJNI();
    }
    
    // ********************************************************************************
    /* These block and then produce an event, and should be executed from their own
     * thread to keep the gui alive.  The replies should be cued on the event thread
     * when they arrive at the listeners. */
    
    public FindCandidates getDiagnosis() throws livdll.L2NativeException {
        int n = getCandidateCountJNI();
        FindCandidates fc = new FindCandidates(getTimeStepJNI(), getSearchTerminationJNI());
        // these are used by CandidateHistory to compress the history data
        // I can share them for all Candidates and save more memory
        java.util.HashMap tokenkeys = new java.util.HashMap();
        java.util.HashMap numberkeys = new java.util.HashMap();
        Candidate[] candidates = new Candidate[n];
        if(n > 0) {
            for(int i = 0; i < n; i++) {
                int pointer = getCandidatePointerJNI(i);
                final java.util.Vector ca = new java.util.Vector();
                mapOverCandidateAssignmentsJNI(pointer, new java.util.Observer() {
                    public void update(java.util.Observable o, Object arg) {
                        if(!((CandidateAssignment)arg).getAttribute().equals("")) ca.add(arg);
                    }
                });
                CandidateAssignment[] assns = new CandidateAssignment[ca.size()];
                if(ca.size() > 0) ca.copyInto(assns);
                candidates[i] = new Candidate(pointer, assns);
                
                installCandidateJNI(i);
                final java.util.Hashtable history = new java.util.Hashtable();
                mapOverAttributeAssignmentsJNI(new java.util.Observer() {
                    MyComparator myComp = new MyComparator();
                    public void update(java.util.Observable o, Object arg) {
                        if(arg == null) return;
                        AttributeAssignment aa = (AttributeAssignment)arg;
                        //System.out.println("LivingstoneEngine.getAttributeAssignments: aa time " + aa.getTime() + " name " + aa.getName());
                        Integer key = new Integer(aa.getTime());
                        java.util.Vector state = (java.util.Vector)history.get(key);
                        if(state == null) {
                            state = new java.util.Vector();
                            history.put(key, state);
                        }
                        int index = java.util.Collections.binarySearch(state, aa.getName(), myComp);
                        if(index < 0) index = -1-index;
                        state.add(index, aa);
                    }
                });
                
                int[] ss = getHistoryStartStopJNI();
                // System.out.println("LivEng2.getAttributeAssignments: ss[0] " + ss[0] + " ss[1] " + ss[1] + " history.size() " + history.size());
                // history.size() should be equal to (ss[1] - ss[0] + 1)
                //    AttributeAssignment[][] ret = new AttributeAssignment[history.size()][];
                AttributeAssignment[][] ret = new AttributeAssignment[ss[1] - ss[0]+1][];
                if(ss[1] >= ss[0]) {
                    for(int time = ss[0]; time <= ss[1]; time++) {
                        Integer key = new Integer(time);
                        java.util.Vector state = (java.util.Vector)history.get(key);
                        if (state == null) {
                            // fill in history for idle progress which has no mode or modeTransition
                            // variables with its time value
                            state = new java.util.Vector();
                            history.put(key, state);
                        }
                        ret[time-ss[0]] = new AttributeAssignment[state.size()];
                        state.copyInto(ret[time-ss[0]]);
                    }
                }
                candidates[i].setHistory(new CandidateHistory(tokenkeys, numberkeys, ret));
            }
        }
        fc.setCandidates(candidates);
        return fc;
    }
    
    /* Returns an alphabetically sorted set of strings naming the attributes */
    public java.util.SortedSet getAttributeNames() throws L2NativeException {
        final java.util.SortedSet names = new java.util.TreeSet();
        mapOverAttributeNamesJNI(new java.util.Observer() {
            public void update(java.util.Observable o, Object arg) {
                if (!(arg instanceof String)) return;
                String name = (String)arg;
                names.add(name);
            }
        });
        return names;
    }
    
    // JNI testing only
    /* Returns an alphabetically sorted set of strings naming the commandable attributes */
    public java.util.SortedSet getCommandAttributeNames() throws L2NativeException {
        final java.util.SortedSet names = new java.util.TreeSet();
        mapOverCommandAttributeNamesJNI(new java.util.Observer() {
            public void update(java.util.Observable o, Object arg) {
                if (!(arg instanceof String)) return;
                String name = (String)arg;
                names.add(name);
            }
        });
        return names;
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
    
    public Object createAttributeAssignment(String name, String value) {
        return new AttributeAssignment(name, value);
    }
    
    public Object createAttributeAssignment(String name, String value, int time) {
        return new AttributeAssignment(name, value, time);
    }
    
    public Object createCandidateAssignment(String attribute, String value, int weight) {
        return new CandidateAssignment(attribute, value, weight);
    }
    
    public Object createDiscrepancy(String name, String value, int weight) {
        return null;
    }
    
    public Throwable createNativeException(String msg) {
        return new L2NativeException(msg);
    }
    
    public void mapOverJustificationNegative(String msg) {
    }
    
    public void mapOverJustificationPositive(String msg) {
    }
    
    public void mapOverPropositionStringsCallback(String msg) {
    }
    
}
