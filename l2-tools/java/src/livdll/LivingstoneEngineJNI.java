package livdll;

public abstract class LivingstoneEngineJNI {
    
    static {
        try {
            System.loadLibrary("livdll");
            //            LivingstoneEngine.helloWorldJNI();
        } catch (Error e) {
            System.out.println(e);
            System.exit(0);
        }
    }
    
    public void hello() {
        System.out.println("hello from java");
    }
        
    //
    // Engine allocation / deallocation
    //
    
    public native void helloWorldJNI();
    
    //
    // JNI Engine operations
    //
    
    protected static synchronized native
    int setCBFSEngineParamsJNI(int max_candidate_classes_returned,
    int max_candidates_returned,
    int max_candidates_searched,
    int max_cutoff_weight,
    int max_history_cutoff,
    int max_trajectories_tracked,
    String progress_cmd_type,
    String fc_cmd_type);
    
    /* Returns the address of a new C++ Livingstone engine instance */
    protected static synchronized native int allocateCBFSEngineJNI(int pointer);
    
    /* Returns the address of a new C++ Livingstone debugger instance */
    protected static synchronized native int allocateCBFSDebuggerJNI(int pointer);
    
    protected static synchronized native
    int setCoverEngineParamsJNI(int max_rank,
    int max_history_cutoff,
    int max_trajectories_tracked,
    String progress_cmd_type,
    String fc_cmd_type);
    
    /* Returns the address of a new C++ Livingstone engine instance */
    protected static synchronized native int allocateCoverEngineJNI(int pointer);
    
    /* Returns the address of a new C++ Livingstone debugger instance */
    protected static synchronized native int allocateCoverDebuggerJNI(int pointer);
    
    /* Returns a status code: 0=ok, 1=error.  */
    // synchronized is done in JNI code, so that jobject is available
    // `private static synchronized' replaced by `protected'
    protected native int deallocateEngineJNI(int tracker_address,
    int debugger_address);
    
    // returns
    //         "cbfs": allocate new cbfs engine, using L2 search params
    //         "cover": allocate new cover engine, using L2 search params
    // do not type loadModel as protected, thus allowing it to be called
    // from LivingstoneEngineAdapter -- it is no longer an engine method
    public static native synchronized String loadModelJNI(String filename,
    int pointer)
    throws L2NativeException;
    
    /* returns -1 for consistent, 0 for inconsistent */
    protected native int propagateJNI()
    throws L2NativeException;
    
    protected native void minProgressJNI(String cmd)
    throws L2NativeException;
    
    protected native void fullProgressJNI(String cmd)
    throws L2NativeException;
    
    // assignMultipleJNI(String monitor)
    // is used instead of
    // assignJNI(String monitor)
    // since javah generates in LivingstoneEngine.h --
    // JNIEXPORT void JNICALL
    //   Java_LivingstoneEngine_assignJNI__Ljava_lang_String_2Ljava_lang_String_2
    // (JNIEnv *, jobject, jstring, jstring);
    //
    // JNIEXPORT void JNICALL
    //   Java_LivingstoneEngine_assignJNI__Ljava_lang_String_2
    // (JNIEnv *, jobject, jstring);
    
    protected native void assignMultipleJNI(String monitor)
    throws L2NativeException;
    
    protected native void unassignJNI(String variable)
    throws L2NativeException;
    
    /* return 0=not_found, 1=true, 2=false, 3=unknown */
    protected native int getPropValueJNI(String propName)
    throws L2NativeException;
    
    protected native void installCandidateJNI(int candidate_index)
    throws L2NativeException;
    
    protected native String getFcStyleJNI()
    throws L2NativeException;
    
    protected native void pruneAndSearchJNI()
    throws L2NativeException;
    
    protected native void findFreshJNI()
    throws L2NativeException;
    
    protected native void extendJNI()
    throws L2NativeException;
    
    protected native String getSearchTerminationJNI()
    throws L2NativeException;
    
    protected native int getCandidateCountJNI()
    throws L2NativeException;
    
    protected native synchronized int getCandidatePointerJNI(int candidateNum)
    throws L2NativeException;
    
    protected native void mapOverCandidateAssignmentsJNI(int candidate,
    java.util.Observer o)
    throws L2NativeException;
    
    protected native int getCandidateClassCountJNI()
    throws L2NativeException;
    
    protected native synchronized int getCandidateClassPointerJNI(int candidateNum)
    throws L2NativeException;
    
    protected native void mapOverAttributeAssignmentsJNI(java.util.Observer o)
    throws L2NativeException;
    
    protected native void mapOverAttributeNamesJNI(java.util.Observer o)
    throws L2NativeException;
    
    protected native void mapOverCommandAttributeNamesJNI(java.util.Observer o)
    throws L2NativeException;
    
    protected native void mapOverObservableAttributeNamesJNI(java.util.Observer o)
    throws L2NativeException;
    
    protected native boolean setAttributeModeJNI(String variable,
    String mode)
    throws L2NativeException;
    
    // create checkpoint 'name', throws exception if not created
    protected native void createCheckpointJNI(String name)
    throws L2NativeException;
    
    // restore checkpoint 'name', throws exception if non-existent
    // returns false if no checkpoints exist, or name is invalid
    protected native boolean restoreCheckpointJNI(String name)
    throws L2NativeException;
    
    // delete checkpoint 'name', throws exception if non-existent
    protected native void deleteCheckpointJNI(String name)
    throws L2NativeException;
    
    protected native int getTimeStepJNI()
    throws L2NativeException;
    
    protected native void setTransitionVariableAtTimeJNI(String name,
    String value,
    int time)
    throws L2NativeException;
    
    protected native int[] getHistoryStartStopJNI()
    throws L2NativeException;
    
    protected native void listenerUsageJNI()
    throws L2NativeException;
    
    protected native boolean parseCmdJNI(String cmd,
    String args)
    throws L2NativeException;
    
    /* return 0=not_found, 1=unjustified, 2=exception, 3=ok */
    protected native int justifyPropositionJNI(String propName)
    throws L2NativeException;
    
    protected native String variableValueAtTimeJNI(String var_name,
    int time)
    throws L2NativeException;
    
    protected native boolean variableExogenousAtTimeJNI(String var_name,
    int time)
    throws L2NativeException;
    
    protected native void printPropClausesJNI(String propname);
    
    // execute L2 cmds without Skunkworks/Stanley updates or interaction
    protected native void executeWarpCommandsJNI(String commands)
    throws L2NativeException;
    
    
    //
    // gov.nasa.arc.l2tools.livingstone/data/L2Publisher.java
    // Livingstone PathFinder
    //
    protected native void findCandidatesJNI()
    throws L2NativeException;
    
    //
    // gov.nasa.arc.l2tools.livingstone/data/L2Publisher.java
    // Livingstone PathFinder
    //
    protected native void assignJNI(String variable,
    String value)
    throws L2NativeException;
    
    //
    // Livingstone PathFinder
    //
    protected native void addAssumptionJNI(
    String variable,
    String value)
    throws L2NativeException;
    
    //
    // Livingstone PathFinder
    //
    protected native void mapOverPropositionStringsJNI()
    throws L2NativeException;
    
    //
    // Livingstone Discrepancy support
    // Added by RG
    protected native void findDiscrepancysJNI(java.util.Observer o)
    throws L2NativeException;
    
    //
    // These were added to remove java code dependencies from the livdll.cpp file.
    // Over write these to return your own java objects for the livingstone native datatypes.
    //
    public abstract Throwable createNativeException(String msg);
    
    public abstract Object createCandidateAssignment(String attribute, String value, int weight);
    
    public abstract Object createAttributeAssignment(String name, String value);
    
    public abstract Object createAttributeAssignment(String name, String value, int time);
    
    public abstract Object createDiscrepancy(String name, String value, int weight);
 
    //
    // Some native fuctions make callbacks to this class.  
    //
    // Used with justifyPropositionJNI
    public abstract void mapOverJustificationPositive(String msg);
    public abstract void mapOverJustificationNegative(String msg);
    // Used with mapOverPropositionStringsJNI
    public abstract void mapOverPropositionStringsCallback(String msg);


    
}



