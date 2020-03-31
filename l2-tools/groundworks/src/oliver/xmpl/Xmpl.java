//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//

/*
 * Xmpl.java
 *
 * Created on June 26, 2002, 1:29 PM
 */

package oliver.xmpl;

/**
 *
 * @author  stephen
 */
public class Xmpl extends net.n3.nanoxml.XMLElement implements Comparable {
    
    public static int tabSize = 4;
    public static int EDITOR_CONTEXT = 1;
    public static int DEBUGGER_CONTEXT = 2;
    private java.util.HashMap initialValues = null;
    private java.util.SortedMap harnessValues = null;
    
    public Xmpl() {
        super();
    }
    
    public Xmpl(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public java.util.Vector getAllChildrenNamed(String name) {
        java.util.Vector v = new java.util.Vector();
        getAllChildrenNamed(name, v);
        return v;
    }
    
    public void getAllChildrenNamed(String name, java.util.Vector v) {
        String supername = getAttribute("extends", null);
        if(supername!=null) {
            Xmpl sup = findType(supername);
            if(sup!=null) sup.getAllChildrenNamed(name,v);
        }
        java.util.Enumeration enum = getChildrenNamed(name).elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement c = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            v.addElement(c);
        }
    }
    
    private void cacheInitialValues() {
        initialValues = new java.util.HashMap();
        cacheInitialValues(initialValues);
    }
    
    // took this out of to support inheritance
    public void cacheInitialValues(java.util.HashMap initialValues) {
        String supername = getAttribute("extends", null);
        if(supername!=null) {
            Xmpl sup = findType(supername);
            if(sup!=null) sup.cacheInitialValues(initialValues);
        }
        net.n3.nanoxml.IXMLElement n = getFirstChildNamed("gr:initialValues");
        if(n != null) {
            java.util.Enumeration enum = n.getChildrenNamed("gr:initialValue").elements();
            while(enum.hasMoreElements()) {
                net.n3.nanoxml.IXMLElement c = (net.n3.nanoxml.IXMLElement)enum.nextElement();
                String name = c.getAttribute("name", null);
                String value = c.getAttribute("value", null);
                if(name != null && value != null) initialValues.put(name, value);
            }
        }
    }
    
    public String getInitialValue(String name) {
        if(initialValues == null) cacheInitialValues();
        return (String)initialValues.get(name);
    }
    
    public void setInitialValue(String name, String value) {
        if(initialValues == null) cacheInitialValues();
        if(name == null) return;
        net.n3.nanoxml.IXMLElement iv = getFirstChildNamed("gr:initialValues");
        if(iv == null) {
            iv = new net.n3.nanoxml.XMLElement("gr:initialValues");
            addChild(iv);
        }
        net.n3.nanoxml.IXMLElement assignment = null;
        java.util.Enumeration enum = iv.getChildrenNamed("gr:initialValue").elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement n = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            String nname = n.getAttribute("name", null);
            if(nname != null && nname.equals(name)) {
                assignment = n;
                break;
            }
        }
        if(value == null) {
            initialValues.remove(name);
            if(assignment != null) iv.removeChild(assignment);
        } else {
            initialValues.put(name, value);
            if(assignment != null) assignment.setAttribute("value", value);
            else {
                net.n3.nanoxml.XMLElement child = new net.n3.nanoxml.XMLElement("gr:initialValue");
                child.setAttribute("name", name);
                child.setAttribute("value", value);
                iv.addChild(child);
            }
        }
    }
    
    private void cacheHarnessValues() {
        harnessValues = new java.util.TreeMap();
        cacheHarnessValues(harnessValues);
    }
    
    // took this out of to support inheritance
    public void cacheHarnessValues(java.util.SortedMap harnessValues) {
        String supername = getAttribute("extends", null);
        if(supername!=null) {
            Xmpl sup = findType(supername);
            if(sup!=null) sup.cacheHarnessValues(harnessValues);
        }
        net.n3.nanoxml.IXMLElement n = getFirstChildNamed("gr:harnessValues");
        if(n != null) {
            java.util.Enumeration enum = n.getChildrenNamed("gr:harnessValue").elements();
            while(enum.hasMoreElements()) {
                net.n3.nanoxml.IXMLElement c = (net.n3.nanoxml.IXMLElement)enum.nextElement();
                String name = c.getAttribute("name", null);
                String value = c.getAttribute("value", null);
                if(name != null && value != null) harnessValues.put(name, value);
            }
        }
    }
    public String getHarnessValue(String name) {
        if(harnessValues == null) cacheHarnessValues();
        return (String)harnessValues.get(name);
    }
    
    public void setHarnessValue(String name, String value) {
        if(harnessValues == null) cacheHarnessValues();
        if(name == null) return;
        net.n3.nanoxml.IXMLElement iv = getFirstChildNamed("gr:harnessValues");
        if(iv == null) {
            iv = new net.n3.nanoxml.XMLElement("gr:harnessValues");
            addChild(iv);
        }
        net.n3.nanoxml.IXMLElement assignment = null;
        java.util.Enumeration enum = iv.getChildrenNamed("gr:harnessValue").elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement n = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            String nname = n.getAttribute("name", null);
            if(nname != null && nname.equals(name)) {
                assignment = n;
                break;
            }
        }
        if(value == null) {
            harnessValues.remove(name);
            if(assignment != null) iv.removeChild(assignment);
        } else {
            harnessValues.put(name, value);
            if(assignment != null) assignment.setAttribute("value", value);
            else {
                net.n3.nanoxml.XMLElement child = new net.n3.nanoxml.XMLElement("gr:harnessValue");
                child.setAttribute("name", name);
                child.setAttribute("value", value);
                iv.addChild(child);
            }
        }
    }
    
    public java.util.Enumeration harnessElements() {
        if(harnessValues == null) cacheHarnessValues();
        java.util.Vector v = new java.util.Vector(harnessValues.size());
        java.util.Iterator itr = harnessValues.keySet().iterator();
        while(itr.hasNext()) {
            String name = (String)itr.next();
            String value = (String)harnessValues.get(name);
            livdll.AttributeAssignment aa = new livdll.AttributeAssignment(name, value, 0);
            v.addElement(aa);
        }
        return v.elements();
    }
    
    public java.util.Enumeration initialStateElements() {
        if(initialValues == null) cacheInitialValues();
        java.util.Vector v = new java.util.Vector(initialValues.size());
        java.util.Iterator itr = initialValues.keySet().iterator();
        while(itr.hasNext()) {
            String name = (String)itr.next();
            String value = (String)initialValues.get(name);
            livdll.AttributeAssignment aa = new livdll.AttributeAssignment(name, value, 0);
            v.addElement(aa);
        }
        return v.elements();
    }
    
    public Xmpl getRoot() {
        Object obj = getParent();
        if(obj == null || !(obj instanceof Xmpl)) return this;
        else return ((Xmpl)obj).getRoot();
    }
    
    public Xmpl findType(String name) {
        Xmpl heir = this;
        Xmpl root = getRoot();
        while(heir!=null) {
            Xmpl found = findType(heir,  name);
            if(found!=null) return found;
            String supername = heir.getAttribute("extends", null);
            if(supername!=null) {
                heir = findType(root, supername);
            } else {
                heir = null;
            }
        }
        return findType(root, name);
    }
    
    public static Xmpl findType(Xmpl xmpl, String name) {
        java.util.Enumeration enum = xmpl.getChildren().elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement e = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            String n = e.getAttribute("name",null);
            if(n != null && n.equals(name)) {
                if(e instanceof ComponentType) return (ComponentType)e;
                else if(e instanceof AttributeType) return (AttributeType)e;
            }
        }
        return null;
    }
    
    /*
    public String toString() {
        java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
        net.n3.nanoxml.XMLWriter writer = new net.n3.nanoxml.XMLWriter(out);
        try {
            writer.write(this, true);
        } catch(java.io.IOException e) {
            System.out.println(e);
        }
        return out.toString();
    }*/
    
    public int compareTo(Object o) {
        if(o instanceof Xmpl) {
            String name1 = getAttribute("name", "");
            String name2 = ((Xmpl)o).getAttribute("name","");
            return name1.compareTo(name2);
        } else {
            return -1;
        }
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
    }
    
    protected void writeSpaces(java.io.PrintWriter writer, int indent) throws java.io.IOException {
        for (int i = 0; i < indent; i++) {
            writer.print(' ');
        }
    }
    
    public int getTabSize() {
        return tabSize;
    }
    
    public void setTabSize(int tabSize) {
        this.tabSize = tabSize;
    }
    
    public net.n3.nanoxml.IXMLElement createElement(String fullName, String namespace, String systemID, int lineNr) {
        if(fullName.equals("ex:equals")) return new Equals(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:inequals")) return new Inequals(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:rel")) return new Rel(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:or")) return new Or(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:and")) return new And(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:not")) return new Not(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:group")) return new Group(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:if")) return new If(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:iff")) return new Iff(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:switch")) return new Switch(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ex:case")) return new Case(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("attributetype")) return new AttributeType(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("at:value")) return new Value(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("componenttype")) return new ComponentType(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ct:attribute")) return new Attribute(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ct:attributetype")) return new AttributeType(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ct:value")) return new Value(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ct:statevector")) return new StateVector(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("relation")) return new Relation(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ct:transition")) return new Transition(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("ct:failure")) return new Failure(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("project")) return new Project(fullName, namespace, systemID, lineNr);
        else if(fullName.equals("gr:promotion")) return new Promotion(fullName, namespace, systemID, lineNr);
        else return super.createElement(fullName, namespace, systemID, lineNr);
    }
    
    public static void main(String[] args) {
        Xmpl a = new Xmpl();
        a.setName("car");
        a.setAttribute("make", "ford");
        
        Xmpl b = new Xmpl();
        b.setName("car");
        b.setAttribute("make", "ford");
        
        Xmpl b2 = new Xmpl();
        b2.setName("wheels");
        b2.setAttribute("style", "mag");
        
        b.addChild(b2);
        
        boolean isSub = a.isSubsetOf(b);
        
        System.out.println(isSub);
    }
    
    public void insertXmplChild(Object source, Xmpl child, int index) {
        if(child == null) return;
        // this might have caused a bug so I took it out
        //if(index < 0 || index >= getChildrenCount()) {
        //index = getChildrenCount();
        addChild(child);
        //} else {
        //    insertChild(child, index);
        //}
        Xmpl root = getRoot();
        if(root instanceof Project && root != null) {
            ((Project)root).fireNodesInserted(source, this, index, child);
        }
    }
    
    // basically self delete
    public void removeXmplFromParent(Object source) throws Exception {
        Object obj = getParent();
        if(obj != null && obj instanceof Xmpl) {
            Xmpl xmpl = (Xmpl)obj;
            int index = xmpl.getChildren().indexOf(this);
            if(index < 0) throw new Exception("Xmpl.removeFromParent() child index less than zero");
            xmpl.removeChild(this);
            Xmpl root = getRoot();
            if(root instanceof Project && root != null) {
                ((Project)root).fireNodesRemoved(source, xmpl, index, this);
            }
        } else {
            throw new Exception("Xmpl.removeFromParent() parent null or wrong class");
        }
    }
    
    public void selectXmplNode(Object source) throws Exception {
        Xmpl root = getRoot();
        if(root instanceof Project && root != null) {
            ((Project)root).fireNodeSelected(source, this);
        }
    }
    
    public int findIndexOfChild(net.n3.nanoxml.IXMLElement child) {
        java.util.Vector children = getChildren();
        if(children.size() > 0) {
            for(int i = 0; i < children.size(); i++) {
                Object c = children.elementAt(i);
                if(child.equals(c)) return i;
            }
        }
        return -1;
    }
    
    public javax.swing.Action[] getActions(final int context, final oliver.application.UserProperties userProperties) {
        javax.swing.Action[] a = new javax.swing.Action[1];
        a[0] = new javax.swing.AbstractAction("Delete") {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                Xmpl root = getRoot();
                if(root instanceof Project) {
                    net.n3.nanoxml.IXMLElement p = getParent();
                    if(p != null && p instanceof Xmpl) {
                        int i = ((Xmpl)p).findIndexOfChild(Xmpl.this);
                        p.removeChild(Xmpl.this);
                        if(i > -1) ((Project)root).fireNodesRemoved(Xmpl.this, (Xmpl)p, i, Xmpl.this);
                    }
                }
            }
        };
        /*
        a[1] = new javax.swing.AbstractAction("Show all Instances") {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                System.out.println("Xmpl.getActions() Show all Instances not implmented");
                //fireNodeInstanceVisible(source, Xmpl.this);
            }
        };
         */
        return a;
    }
    
    public static javax.swing.Action[] addActions(javax.swing.Action[] a, javax.swing.Action[] b) {
        if(a == null) a = new javax.swing.Action[0];
        if(b == null) b = new javax.swing.Action[0];
        javax.swing.Action[] c = new javax.swing.Action[a.length + b.length];
        if(a.length > 0)
            for(int i = 0; i < a.length; i++) c[i] = a[i];
        if(b.length > 0)
            for(int i = 0; i < b.length; i++) c[i+a.length] = b[i];
        return c;
    }
    
    
    // Check out super.getFirstChildNamed()
    public static net.n3.nanoxml.IXMLElement findFirstNodeWithNameTag(String name, java.util.Vector list) {
        java.util.StringTokenizer tok = new java.util.StringTokenizer(name, ".");
        while(tok.hasMoreElements()) {
            String n = tok.nextToken();
            java.util.Enumeration enum = list.elements();
            while(enum.hasMoreElements()) {
                net.n3.nanoxml.IXMLElement child = (net.n3.nanoxml.IXMLElement)enum.nextElement();
                String childname = child.getAttribute("name", null);
                if(childname == null) return null;
                if(childname.equals(n)) return child;
            }
        }
        return null;
    }
    
    public Xmpl depthFirstSeachForElementNamed(String name) {
        if(getName().equals(name)) return this;
        java.util.Enumeration enum = getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Xmpl)
                return ((Xmpl)obj).depthFirstSeachForElementNamed(name);
        }
        return null;
    }
    
    public static boolean isLegalName(String name) {
        if(name == null || name.length() < 1) return false;
        for(int i = 0; i < name.length(); i++) {
            if(!(
            Character.isLetterOrDigit(name.charAt(i))
            || name.charAt(i)=='_'
            )) return false;
        }
        return true;
    }
    
    /** Name has several meanings here.  The underlying xml code has support for
     *  a name property for each element.  But our elements have a tag called "name".
     *  I need to generate the fullname (eg "mainPropulsionSystem.aftLO2Tank.lO2EngineInlet")
     *  using the name tag information.
     * public String getFullname() { // "getFullname not getFullName to distinguish
     * return null;
     * } */
    
    
    /** returns true if all the attributes are present and equal, including a
     *  recursive check of children. */
    public boolean isSubsetOf(Xmpl xmpl) {
        java.util.Enumeration enum = enumerateAttributeNames();
        while(enum.hasMoreElements()) {
            String name = (String)enum.nextElement();
            if(name.equals("filename") || name.equals("column") || name.equals("line")) {
                xmpl.removeAttribute(name);
            } else {
                String v1 = getAttribute(name, null);
                String v2 = xmpl.getAttribute(name, null);
                if(!v1.equals(v2)) return false;
            }
        }
        java.util.Vector v = new java.util.Vector(xmpl.getChildren());
        enum = getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Xmpl) {
                Xmpl child = (Xmpl)obj;
                java.util.Enumeration enum2 = v.elements();
                boolean myChildFoundInXmplChildren = false;
                while(enum2.hasMoreElements()) {
                    Object obj2 = enum2.nextElement();
                    if(obj2 instanceof Xmpl) {
                        Xmpl vChild = (Xmpl)obj2;
                        boolean r = child.isSubsetOf(vChild);
                        if(r) {
                            v.remove(vChild); // can't use it twice.
                            myChildFoundInXmplChildren = true;
                            break;
                        }
                    } else {
                        System.out.println("Xmpl.isSubset() not an Xmpl");
                        return false;
                    }
                }
                if(!myChildFoundInXmplChildren) return false;
            } else {
                System.out.println("Xmpl.isSubset() not an Xmpl");
                return false;
            }
        }
        // must have found all children or would have returned false
        return true;
    }
    
    /** When users make changes to the text it will be recompiled and read in to
     *  a new xml tree.  The structure of the new tree has to be incorperated into
     *  the existing tree, while firing updates to views.  This method checks for
     *  changes to the attributes and makes appropriate changes.  It should probably
     *  be made abstract and moved down.
     */
    public void setXmpl(Xmpl n) throws Exception {
        
        if(!getName().equals(n.getName())) {
            throw new Exception("Xmpl.setXmpl() names are different: " +getName()+" "+n.getName());
        }
        // before we get into the children lets look at the properties
        java.util.Enumeration enum = n.enumerateAttributeNames();
        Xmpl root = getRoot();
        while(enum.hasMoreElements()) {
            String name = (String)enum.nextElement();
            String value = n.getAttribute(name, null);
            String myValue = getAttribute(name, null);
            if(value != null && !value.equals(myValue)) {
                setAttribute(name,value);
                if(root instanceof Project) {
                    ((Project)root).fireNodesChanged(this, this, name, value, myValue);
                }
            }
        }
    }
    
    public String getMouseOverText() {
        return getAttribute("name", "######");
    }
    
}