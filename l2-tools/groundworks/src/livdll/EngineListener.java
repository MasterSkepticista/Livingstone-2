// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * EngineListener.java
 *
 * Created on December 2, 2002, 1:01 PM
 */

package livdll;

/**
 *
 * @author  stephen
 */
public interface EngineListener extends java.util.EventListener {
    
    /**
     * A new C++ engine has been allocated for this object, possibly deallocating the previous engine. 
     */
    public void echoNewEngine(Object source, String search_method);

    /**
     * The C++ engine associated with this object has been deallocated. 
     */
    public void echoDestroyEngine(Object source);

    /**
     * A model has been loaded into the engine. 
     */
    public void echoLoadModel(Object source, String fileName, java.util.SortedSet attributes);

    /**
     * A progress command has been issued to the engine. 
     */
    public void echoCommand(Object source, livdll.LivingstoneCommand command);

    /**
     * An assign command has been issued to the engine.  The most recent assignment will be the *first* element of the recentAssignments property on the engine bean.  (The assignments go onto the list in reverse order.) 
     */
    public void echoMonitor(Object source, livdll.LivingstoneCommand command);

    /**
     * A propagate command has been issued to the engine. 
     */
    public void echoPropagate(Object source, boolean consistent, livdll.FindCandidates fc);

    /**
     * A findCandidates command has been issued to the engine. 
     */
    public void echoFindCandidates(Object source);

    /**
     * A findCandidates command has finished. 
     */
    public void replyFoundCandidates(Object source, livdll.FindCandidates fc);

}
