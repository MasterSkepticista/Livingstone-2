// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class AttributeType extends Xmpl {
    
    public AttributeType() {
        super();
    }
    
    public AttributeType(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public int compareTo(Object o) {
        if(o instanceof ComponentType) return -1;
        else return super.compareTo(o);
    }
    
    public java.util.Vector getValues() {
        // The namespace can be "at:" or "ct:" so better check.
        String myprefix = "at:";
        if(getName().startsWith("ct:")) myprefix = "ct:";
        String myname = myprefix+"value";
        java.util.Vector v1 = getChildrenNamed(myname);
        java.util.Vector v2 = new java.util.Vector(v1.size());
        java.util.Enumeration enum = v1.elements();
        while(enum.hasMoreElements()) {
            String name = ((Xmpl)enum.nextElement()).getAttribute("name",null);
            if(name != null) v2.addElement(name);
        }
        return v2;
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        java.util.Vector values = new java.util.Vector();
        java.util.Vector relations = new java.util.Vector();
        java.util.Enumeration enum = getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Value) values.addElement(obj);
            else if(obj instanceof Relation) relations.addElement(obj);
        }
        writeSpaces(writer, indent);
        String m = getAttribute("modifier",null);
        if(m != null) writer.print(m+" ");
        writer.print("enum "+getAttribute("name", "######")+" {");
        enum = values.elements();
        while(enum.hasMoreElements()) {
            writer.print(((Value)enum.nextElement()).getAttribute("name","######"));
            if(enum.hasMoreElements()) writer.print(", ");
        }
        writer.print("}");
        if(relations.size() > 0) {
            writer.print(" {\n");
            indent = indent + getTabSize();
            enum = relations.elements();
            while(enum.hasMoreElements()) {
                Relation r = (Relation)enum.nextElement();
                r.writeJmpl(writer, true, indent);
            }
            indent = indent - getTabSize();
            writeSpaces(writer, indent);
            writer.print("}");
        }
        writer.print(";\n");
    }
    
    
    
    public void setXmpl(Xmpl n) throws Exception {
        super.setXmpl(n);
        
        // The namespace can be "at:" or "ct:" so better check.
        String myprefix = "at:";
        if(getName().startsWith("ct:")) myprefix = "ct:";
        String myname = myprefix+"value";
        
        String nprefix = "at:";
        if(n.getName().startsWith("ct:")) nprefix = "ct:";
        String nname = nprefix+"value";
        
        java.util.Enumeration enum = getChildrenNamed(myname).elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement mychild = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            if(mychild instanceof Xmpl && mychild.getName().equals(myname)) {
                // remove all the values, but nothing else
                ((Xmpl)mychild).removeXmplFromParent(this);
            }
        }
        
        enum = n.getChildrenNamed(nname).elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement nchild = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            if(nchild instanceof Xmpl && nchild.getName().equals(nname)) {
                Value v = new Value();
                v.setName(myname);
                v.setAttribute("name", nchild.getAttribute("name","######"));
                int index = getChildrenCount();
                insertXmplChild(this, v, index);
            }
        }
    }
    
}
