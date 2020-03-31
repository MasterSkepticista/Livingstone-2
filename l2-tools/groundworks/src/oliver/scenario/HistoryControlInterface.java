// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * HistoryControlInterface.java
 *
 * Created on December 11, 2002, 5:49 PM
 */

package oliver.scenario;

/**
 *
 * @author  stephen
 */
public interface HistoryControlInterface {

    public void rrew();
    public void rew();
    public void fwd();
    public void ffwd();
    
    public void setShowTrailingCommands(boolean b);
    
    public void removeAllCandidates();
    
    public int getFindCandidatesSize();
    public int getFindCandidatesIndex();
    public void setFindCandidatesIndex(int index) throws IndexOutOfBoundsException;
    
    public void addFindCandidates(livdll.FindCandidates fc );
    
    public void updateLastFindCandidate(livdll.FindCandidates fc, boolean fireEvent);
    public void addTrailingCommand(livdll.LivingstoneCommand command, boolean fireEvent);
    public void clearTrailingCommands();
    
    public void addLivingstoneViewControlListener(LivingstoneViewControlListener listener);
    
    public static interface LivingstoneViewControlListener {
        public void livingstoneViewControlListenerEvent(HistoryControlListenerEvent e);
    }
    
    public static class HistoryControlListenerEvent {
        public int index;
        public int size;
        public int time;
        public livdll.Candidate selectedCandidate;
        public java.util.Vector trailingCommands = new java.util.Vector();
        public boolean showTrailingCommands = true;
        public String toString() { return (index+1)+" of "+size; }
    }
}
