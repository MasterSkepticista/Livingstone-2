// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Case extends Xmpl {
    
    public Case() {
        super();
    }
    
    public Case(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        writeSpaces(writer, indent);
        writer.println("case "+getAttribute("names", "######")+":");
        
        java.util.Enumeration enum = getChildren().elements();
        Object obj = enum.nextElement();
        // assuming one child of type Group
        if(obj instanceof Group) {
            enum = ((Group)obj).getChildren().elements();
            
            indent = indent + getTabSize();
            while(enum.hasMoreElements()) {
                obj = enum.nextElement();
                if(obj instanceof Xmpl) {
                    ((Xmpl)obj).writeJmpl(writer, prettyprint, indent);
                }
            }
            indent = indent - getTabSize();
            
        }
    }
}
