// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Inequals extends Xmpl {
    
    public Inequals() {
        super();
    }
    
    public Inequals(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }

    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        if(prettyprint) writeSpaces(writer, indent);
        java.util.StringTokenizer tok = new java.util.StringTokenizer(getAttribute("arguments","")," ");
        while(tok.hasMoreTokens()) {
            writer.print(tok.nextToken());
            if(tok.hasMoreTokens()) writer.print(" != ");
            else if(prettyprint) writer.print(";");
        }
        if(prettyprint) writer.println();
    }
    
}
