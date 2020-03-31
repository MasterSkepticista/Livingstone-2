// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Failure extends Xmpl {
    
    public Failure() {
        super();
    }
    
    public Failure(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        String name = getAttribute("name", "######");
        String from = getAttribute("from","######");
        String to = getAttribute("to", "######");
        String probability = getAttribute("probability", "######");
        if(prettyprint) writeSpaces(writer, indent);
        writer.println("failure "+name+"("+from+", "+to+", "+probability+") {");
        indent = indent + getTabSize();
        // there should just be one group, so no linereturn
        java.util.Enumeration enum = getChildren().elements();
        Object obj = enum.nextElement();
        if(obj instanceof Group) {
            enum = ((Group)obj).getChildren().elements();
        }
        while(enum.hasMoreElements()) {
            obj = enum.nextElement();
            if(obj instanceof Xmpl) {
                ((Xmpl)obj).writeJmpl(writer, prettyprint, indent);
            }
        }
        indent = indent = getTabSize();
        if(prettyprint) writeSpaces(writer, indent);
        writer.println("}");
    }
    
    
}
