// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Group extends Xmpl {
    
    public Group() {
        super();
    }
    
    public Group(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        writeSpaces(writer, indent);
        writer.println("{");
        indent = indent + getTabSize();
        //Class lastChildClass = null;
        java.util.Enumeration enum = getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Xmpl) {
                //if(obj.getClass() != lastChildClass) writer.println();
                ((Xmpl)obj).writeJmpl(writer, prettyprint, indent);
                //lastChildClass = obj.getClass();
            }
        }
        indent = indent - getTabSize();
        
        writeSpaces(writer, indent);
        writer.println("}");
    }

}
