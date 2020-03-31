// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Relation extends Xmpl {
    
    public Relation() {
        super();
    }
    
    public Relation(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        String name = getAttribute("name", "######");
        java.util.Vector v = new java.util.Vector();
        java.util.Enumeration enum = getChildren().elements();
        Object obj = null;
        while(enum.hasMoreElements()) {
            obj = enum.nextElement();
            if(obj instanceof Attribute) v.addElement(obj);
            else if(obj instanceof Group) break;
        }
        
        if(prettyprint) writeSpaces(writer, indent);
        writer.print("relation "+name+"(");
        enum = v.elements();
        while(enum.hasMoreElements()) {
            Attribute att = (Attribute)enum.nextElement();
            writer.print(att.getAttribute("type","######")+" "+att.getAttribute("name","######"));
            if(enum.hasMoreElements()) writer.print(", ");
        }
        writer.println(") {");
        indent = indent + getTabSize();
        // there should just be one group
        enum = ((Group)obj).getChildren().elements();
        while(enum.hasMoreElements()) {
            obj = enum.nextElement();
            if(obj instanceof Xmpl) {
                ((Xmpl)obj).writeJmpl(writer, prettyprint, indent);
            }
        }
        indent = indent - getTabSize();
        if(prettyprint) writeSpaces(writer, indent);
        writer.println("}");
    }
    
}
