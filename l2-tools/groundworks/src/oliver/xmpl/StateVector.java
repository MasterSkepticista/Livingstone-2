// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class StateVector extends Xmpl {
    
    public StateVector() {
        super();
    }
    
    public StateVector(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }

    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        if(prettyprint) writeSpaces(writer, indent);
        writer.println("stateVector ["+getAttribute("vars", "######")+"];");
    }
    
}
