// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Rel extends Xmpl {
    
    public Rel() {
        super();
    }
    
    public Rel(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        writeSpaces(writer, indent);
        writer.print(getAttribute("name","######")+"(");
        java.util.StringTokenizer tok = new java.util.StringTokenizer(getAttribute("arguments","")," ");
        while(tok.hasMoreTokens()) {
            writer.print(tok.nextToken());
            if(tok.hasMoreTokens()) writer.print(", ");
        }
        writer.print(");");
        writer.println();
    }
    
}
