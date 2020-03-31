// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * FindCandidates.java
 *
 * Created on March 15, 2001, 5:48 PM
 */

package livdll;

/**
 *
 * @author  Administrator
 * @version
 */
public class FindCandidates extends java.lang.Object implements java.io.Serializable {
    
    public java.util.Vector candidates = new java.util.Vector();
    public java.util.Vector trailingCommands = new java.util.Vector();
    
    /** Holds value of property time. */
    private int time;
    
    private String searchTermination = "";
    
    /** Creates new FindCandidates */
    public FindCandidates() {
    }
    
    public void addTrailingCommand(LivingstoneCommand command) {
        trailingCommands.addElement(command);
    }
    
    public void clearTrailingCommands() { trailingCommands.clear(); }
    
    public java.util.Vector getTrailingCommands() { return trailingCommands; }
    
    public String toString() {
        StringBuffer b = new StringBuffer();
        b.append("FindCandidates");
        java.util.Enumeration enum = candidates.elements();
        while(enum.hasMoreElements()) {
            b.append("\n");
            b.append("    "+enum.nextElement().toString());
        }
        return b.toString();
    }
    
    public FindCandidates(int time, String searchTermination) {
        this.time = time;
        this.searchTermination = searchTermination;
    }
    
    public Candidate[] getCandidates() {
        Candidate[] c = new Candidate[candidates.size()];
        candidates.copyInto(c);
        return c;
    }
    
    public void setCandidates(Candidate[] c) {
        candidates = new java.util.Vector(c.length);
        for(int i = 0; i < c.length; i++)
            candidates.addElement(c[i]);
    }
    
    public void addCandidate(Candidate candidate) {
        candidates.add(candidate);
    }
    
    public void addCandidate(int index, Candidate candidate) throws  ArrayIndexOutOfBoundsException {
        candidates.add(index, candidate);
    }
    
    public Candidate getCandiate(int index) throws  ArrayIndexOutOfBoundsException {
        return(Candidate)candidates.get(index);
    }
    
    public boolean removeCandidate(Candidate candidate) {
        return candidates.removeElement(candidate);
    }
    
    public void removeCandidateAt(int index) throws  ArrayIndexOutOfBoundsException {
        candidates.removeElementAt(index);
    }
    
    public String getSearchTermination() { return searchTermination; }
    
    public void setSearchTermination(String searchTermination) { this.searchTermination = searchTermination; }
    
    public int size() { return candidates.size(); }
    
    public int getTime() {
        return time;
    }
    
    public void setTime(int time) {
        this.time = time;
    }
    
}
