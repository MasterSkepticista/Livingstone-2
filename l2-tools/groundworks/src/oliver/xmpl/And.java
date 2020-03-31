// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class And extends Xmpl {
    
    public And() {
        super();
    }
    
    public And(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    // boolean prettyprint means top level for expressions
    // no return at end unless top
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        
        if(prettyprint) writeSpaces(writer, indent);  // if top level
        writer.print("(");
        indent = indent + getTabSize();
        
        java.util.Enumeration enum = getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            
            if(obj instanceof Xmpl) {
                ((Xmpl)obj).writeJmpl(writer, false, indent);
                if(enum.hasMoreElements()) {
                    writer.println(" & ");
                    writeSpaces(writer, indent);
                }
            }
            
        }
        
        writer.print(")");
        if(prettyprint) writer.println(";");
    }
    
    
}
