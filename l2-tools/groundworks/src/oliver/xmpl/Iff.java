// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Iff extends Xmpl {
    
    public Iff() {
        super();
    }
    
    public Iff(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }

    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        writeSpaces(writer, indent);
        writer.print("iff (");
        
        java.util.Enumeration enum = getChildren().elements();
        Object obj = enum.nextElement();
        if(obj instanceof Xmpl) {
            ((Xmpl)obj).writeJmpl(writer, false, indent);
        }
        writer.println(") {");
        
        indent = indent + getTabSize();
        if(enum.hasMoreElements()) {
            obj = enum.nextElement();
            if(obj instanceof Group) {
                // Assuming that there is only two children 
                // and the second is a group
                java.util.Enumeration enum2 = ((Group)obj).getChildren().elements();
                while(enum2.hasMoreElements()) {
                    Object obj2 = enum2.nextElement();
                    if(obj2 instanceof Xmpl) {
                        ((Xmpl)obj2).writeJmpl(writer, prettyprint, indent);
                    }
                }
            } else if(obj instanceof Xmpl) {
                ((Xmpl)obj).writeJmpl(writer, prettyprint, indent);
            }
        }
        indent = indent - getTabSize();
        writeSpaces(writer, indent);
        writer.println("}");
    }
}
