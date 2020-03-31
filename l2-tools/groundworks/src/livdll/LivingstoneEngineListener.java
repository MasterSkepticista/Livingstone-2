// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * LivingstoneEngineListener.java
 *
 * Created on September 12, 2002, 1:42 PM
 */

package livdll;

/**
 *
 * @author  stephen
 */
public interface LivingstoneEngineListener extends java.util.EventListener {
    
    /**
     * A new C++ engine has been allocated for this object, possibly deallocating the previous engine. 
     */
    public void echoNewEngine(Object source, Parameters params);

    /**
     * The C++ engine associated with this object has been deallocated. 
     */
    public void echoDestroyEngine(Object source);

    /**
     * A model has been loaded into the engine. 
     */
    public void echoLoadModel(Object source, String fileName, Object model);

    /**
     * A progress command has been issued to the engine. 
     */
    public void echoCommand(Object source, String cmd, String args);

    /**
     * An assign command has been issued to the engine.  The most recent assignment will be the *first* element of the recentAssignments property on the engine bean.  (The assignments go onto the list in reverse order.) 
     */
    public void echoAssignments(Object source, String assignments);

    /**
     * A propagate command has been issued to the engine. 
     */
    public void echoPropagate(Object source, boolean consistent);

    /**
     * A findCandidates command has been issued to the engine. 
     */
    public void echoFindCandidates(Object source);

    /**
     * A findCandidates command has finished. 
     */
    public void replyFoundCandidates(Object source, FindCandidates fc);

    
}
