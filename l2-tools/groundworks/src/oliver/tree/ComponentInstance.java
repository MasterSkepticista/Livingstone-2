//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//

/*
 * ComponentInstance.java
 *
 * Created on July 8, 2002, 10:25 AM
 */

package oliver.tree;

import oliver.xmpl.*;

/**
 *
 * @author  stephen
 */
public class ComponentInstance extends Instance {
    
    private java.util.Vector componentInstances = new java.util.Vector();
    private java.util.Vector attributeInstances = new java.util.Vector();
    
    /*
     * Something weird here... why is there an Attribute created in this class?
     * That should be passed in so changes are recorded.  Fix this later.
     *
     * Must be something I got from the old code.
     *
     */
    
    /** Creates a new instance of ComponentInstance */
    public ComponentInstance(oliver.project.ProjectModel model, ComponentType type) {
        super(model);
        this.type = type;
        this.att = new Attribute("test","",null, Xmpl.NO_LINE);
        this.att.setAttribute("name", "test");
        initChildren(type);
    }
    
    /** Creates a new instance of ComponentInstance.  The reason for passing in the parent
     *  and then dropping it is that the treeNode parent isn't set at construction. */
    public ComponentInstance(oliver.project.ProjectModel model, Xmpl parent, Attribute att) throws Exception {
        super(model);
        this.att = att;
        String name = att.getAttribute("type", null);
        Object obj = parent.findType(name);
        if(obj != null && obj instanceof ComponentType) {
            this.type = (ComponentType)obj;
            initChildren((ComponentType)type);
        } else {
            throw new Exception("ComponentInstance.constructor type not ComponentType");
        }
    }
    
    /** Get rid of this after testing */
    public ComponentInstance(oliver.project.ProjectModel model) throws Exception {
        super(model);
        this.att = new Attribute("test","",null, Xmpl.NO_LINE);
        this.att.setAttribute("name", "test");
        String name = model.getRoot().getAttribute("name", null);
        if(name.endsWith(".jmpl")) name = name.substring(0,name.length()-5);
        java.util.Enumeration enum = model.getRoot().getChildren().elements();
        while(enum.hasMoreElements()) {
            net.n3.nanoxml.IXMLElement xml = (net.n3.nanoxml.IXMLElement)enum.nextElement();
            if(xml instanceof ComponentType) {
                String n = xml.getAttribute("name",null);
                if(n != null && n.equals(name)) {
                    this.type = (ComponentType)xml;
                }
            }
        }
        if(this.type == null) throw new Exception("ComponentInstance.constructor Xmpl is null");
        initChildren((ComponentType)type);
    }
    
    private void initChildren(ComponentType xmpl) {
        if(xmpl == null) return;
        setUserObject(att.getAttribute("name","######"));
        
        // added for inheritance
        String supername = xmpl.getAttribute("extends", null);
        if(supername!=null) {
            oliver.xmpl.Xmpl sup = findType(supername);
            if(sup!=null && sup instanceof ComponentType) initChildren((ComponentType)sup);
        }
        
        java.util.Enumeration enum = xmpl.getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Attribute) {
                Attribute att = (Attribute)obj;
                String debug = att.getInstanceName();
                Xmpl type = findType(att.getAttribute("type",null));
                if(type!=null) {
                    try {
                        if(type instanceof ComponentType) {
                            ComponentInstance child = new ComponentInstance(model, xmpl, (Attribute)obj);
                            componentInstances.add(child);
                            add(child);
                        } else if(type instanceof AttributeType) {
                            AttributeInstance child = new AttributeInstance(model, xmpl, (Attribute)obj);
                            attributeInstances.add(child);
                            add(child);
                        }
                    } catch(Exception e) {
                        System.out.println(e);
                    }
                } else {
                    System.out.println("ComponentType.initChildren() type not found "+att.getAttribute("type",null)+" : "+type.getClass().toString());
                }
            }
        }
    }
    
    public Xmpl findType(String name) {
        return type.findType(name);
    }
    
    public Instance findNodeNamed(String fullname) throws Exception {
        java.util.StringTokenizer tok = new java.util.StringTokenizer(fullname,".");
        String name = tok.nextToken();
        if(getName().equals(name)) return findNodeNamed(tok);
        throw new Exception("ComponentType.findNodeNamed() root name wrong: "+fullname);
    }
    
    public Instance findNodeNamed(java.util.StringTokenizer tok) throws Exception {
        String name = tok.nextToken();
        java.util.Enumeration enum = children();
        while(enum.hasMoreElements()) {
            Instance child = (Instance)enum.nextElement();
            if(child.getName().equals(name)) {
                if(tok.hasMoreTokens() && child instanceof ComponentInstance)
                    return ((ComponentInstance)child).findNodeNamed(tok);
                else return child;
            }
        }
        throw new Exception("ComponentType.findNodeNamed() couldn't find node named");
    }
    
    // This is called from big tree when it learns about a new attribute in the
    // componenttype.  I didn't want to make this class a listener, too messy.
    public void updateChildInserted() {
        
        System.out.println("ComponentInstance.updateChildInserted() not impemented.");
        
    }
    
    // This is called from big tree when it learns about a new attribute in the
    // componenttype.  I didn't want to make this class a listener, too messy.
    public void updateChildDeleted() {
        
        System.out.println("ComponentInstance.updateChildDeleted() not impemented.");
        
    }
    
    // This is called from big tree when it learns about a new attribute in the
    // componenttype.  I didn't want to make this class a listener, too messy.
    public void updateChildChanged(oliver.project.ProjectModelUndoEvent event) {
        if(type == event.getChild()) {
            // this component is an instance of a type that has changed
            removeAllChildren();
            initChildren((oliver.xmpl.ComponentType)type);
            // that should take care of all subcomponents
        } else {
            // but if we didn't find it it may be a child.
            java.util.Enumeration enum = children();
            while(enum.hasMoreElements()) {
                Instance child = (Instance)enum.nextElement();
                child.updateChildChanged(event);
            }
        }
    }
    
    
    
}
