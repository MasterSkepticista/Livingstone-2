// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Attribute extends Xmpl {
    
    private int type = 0;
    
    public Attribute() {
        super();
        setName("ct:attribute");
    }
    
    public Attribute(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        writeSpaces(writer, indent);
        String m = getAttribute("modifier",null);
        if(m != null) writer.print(m+" ");
        writer.print(getAttribute("type", "######")+" "+getAttribute("name", "######")+";");
        writer.println();
    }
    
    // This is used in the ComponentFigure to create a list of names
    public void getAllNames(java.util.Vector path, oliver.project.SortedListModel names) {
        Xmpl type = null;
        try {
            type = getType();
        } catch(Exception e) {}
        if(type != null) {
            String instanceName = getInstanceName();
            StringBuffer buf = new StringBuffer();
            java.util.Enumeration enum = path.elements();
            while(enum.hasMoreElements()) {
                buf.append((String)enum.nextElement());
                buf.append(".");
            }
            names.addElement(buf.toString()+instanceName);
            if(type instanceof ComponentType) {
                path.addElement(instanceName);
                java.util.Enumeration atts = ((ComponentType)type).getChildrenNamed("ct:attribute").elements();
                while(atts.hasMoreElements()) {
                    Object obj = atts.nextElement();
                    if(obj instanceof Attribute) {
                        ((Attribute)obj).getAllNames(path, names);
                    }
                }
                path.remove(path.size()-1);
            }
        }
    }
    
    public String getTypeName() {
        return getAttribute("type", null);
    }
    
    public Xmpl getType() {
        String typename = getTypeName();
        if(typename == null) return null;
        else return findType(typename);
    }
    
    
    public java.awt.Point getPoint() {
        String xs = getAttribute("x", null);
        if(xs == null) {
            xs = "100";
            setAttribute("x", xs);
        }
        String ys = getAttribute("y", null);
        if(ys == null) {
            ys = "100";
            setAttribute("y", ys);
        }
        int x = 100;
        int y = 100;
        try {
            x = Integer.valueOf(xs).intValue();
        } catch(NumberFormatException e) {
            setAttribute("x", "100");
        }
        try {
            y = Integer.valueOf(ys).intValue();
        } catch(NumberFormatException e) {
            setAttribute("y", "100");
        }
        return new java.awt.Point(x, y);
    }
    
    public void setTypeName(String typename) {
        setAttribute("type", typename);
    }
    
    public void setInstanceName(String instancename) {
        setAttribute("name", instancename);
    }
    
    public String getInstanceName() {
        return getAttribute("name", null);
    }
    
    public void setModifiers(String modifiers) {
        setAttribute("name", modifiers);
    }
    
    public String getModifiers() {
        return getAttribute("modifiers", null);
    }
    
    public void setPoint(java.awt.Point location) {
        System.out.println("Attribute.setPoint() "+location);
        setAttribute("x", String.valueOf((int)location.getX()));
        setAttribute("y", String.valueOf((int)location.getY()));
    }
    
}