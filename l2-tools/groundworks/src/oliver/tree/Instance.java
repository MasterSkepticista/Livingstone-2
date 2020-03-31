//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//

package oliver.tree;

/*
 * Model.java
 *
 * Created on July 5, 2002, 1:30 PM
 */

import oliver.xmpl.*;

/**
 *
 * @author  stephen
 */
public abstract class Instance extends javax.swing.tree.DefaultMutableTreeNode {
    
    protected oliver.project.ProjectModel model = null;
    protected Attribute att;
    protected Xmpl type;
    
    /** Creates a new instance of Model */
    public Instance(oliver.project.ProjectModel model) {
        super();
        this.model = model;
    }
    
    public String getInitialValue() {
        javax.swing.tree.TreeNode[] path = getPath();
        if(path.length == 0) return null;
        int[] depth = {0};
        String v = ((Instance)path[0]).getInitialValue(path, depth);
        if(v != null && depth[0] != 0) v = v+"(default)";
        return v;
    }
    
    /** This function should only be called on the first node in the path.
     *  It is used recursively from from getInitialValue() in this same class,
     *  and should not be called from anywhere else.
     */
    public static String getInitialValue(javax.swing.tree.TreeNode[] path, int[] depth) {
        if(path.length > depth[0]+1) {
            StringBuffer fullname = new StringBuffer();
            for(int i = depth[0]+1; i < path.length; i++) {
                fullname.append(((Instance)path[i]).getName());
                if(i < path.length-1) fullname.append(".");
            }
            String value = ((Instance)path[depth[0]]).getType().getInitialValue(fullname.toString());
            if(value == null && path.length > depth[0]+2) {
                depth[0]++;
                return ((Instance)path[depth[0]]).getInitialValue(path, depth);
            } else {
                return value;
            }
        } else {
            System.out.println("Instance.getInitialValue() this should never happen.");
            return null;
        }
    }
    
    public void setInitialValue(String value) {
        StringBuffer fullname = new StringBuffer();
        javax.swing.tree.TreeNode[] path = getPath();
        if(path.length > 1) {
            for(int i = 1; i < path.length; i++) {
                fullname.append(((Instance)path[i]).getName());
                if(i < path.length-1) fullname.append(".");
            }
        }
        ((Instance)path[0]).getType().setInitialValue(fullname.toString(), value);
    }
    
    public String getHarnessValue() {
        javax.swing.tree.TreeNode[] path = getPath();
        if(path.length == 0) return null;
        int[] depth = {0};
        String v = ((Instance)path[0]).getHarnessValue(path, depth);
        if(v != null && depth[0] != 0) v = v+"(default)";
        return v;
    }
    
    /** This function should only be called on the first node in the path.
     *  It is used recursively from from getHarnessValue() in this same class,
     *  and should not be called from anywhere else.
     */
    public static String getHarnessValue(javax.swing.tree.TreeNode[] path, int[] depth) {
        if(path.length > depth[0]+1) {
            StringBuffer fullname = new StringBuffer();
            for(int i = depth[0]+1; i < path.length; i++) {
                fullname.append(((Instance)path[i]).getName());
                if(i < path.length-1) fullname.append(".");
            }
            String value = ((Instance)path[depth[0]]).getType().getHarnessValue(fullname.toString());
            if(value == null && path.length > depth[0]+2) {
                depth[0]++;
                return ((Instance)path[depth[0]]).getHarnessValue(path, depth);
            } else {
                return value;
            }
        } else {
            System.out.println("Instance.getHarnessValue() this should never happen.");
            return null;
        }
    }
    
    public void setHarnessValue(String value) {
        StringBuffer fullname = new StringBuffer();
        javax.swing.tree.TreeNode[] path = getPath();
        if(path.length > 1) {
            for(int i = 1; i < path.length; i++) {
                fullname.append(((Instance)path[i]).getName());
                if(i < path.length-1) fullname.append(".");
            }
        }
        ((Instance)path[0]).getType().setHarnessValue(fullname.toString(), value);
    }
    
    public net.n3.nanoxml.XMLElement createHarness() {
        net.n3.nanoxml.XMLElement xml = new net.n3.nanoxml.XMLElement("mplHarness");
        xml.setAttribute("name", getName());
        xml.setAttribute("version","1.00");
        java.util.TreeMap map = new java.util.TreeMap();
        createHarness(map);
        java.util.Iterator iter = map.keySet().iterator();
        while(iter.hasNext()) {
            String name = (String)iter.next();
            net.n3.nanoxml.XMLElement a = (net.n3.nanoxml.XMLElement)map.get(name);
            xml.addChild(a);
        }
        return xml;
    }
    
    /** Recursive call to populate the list for root element
     *  I'm using a map to avoid duplicates. */
    public void createHarness(java.util.SortedMap map) {
        java.util.Enumeration enum1 = children();
        while(enum1.hasMoreElements()) {
            Instance child = (Instance)enum1.nextElement();
            child.createHarness(map);
        }
        java.util.Enumeration enum2 = getType().harnessElements();
        while(enum2.hasMoreElements()) {
            livdll.AttributeAssignment aa = (livdll.AttributeAssignment)enum2.nextElement();
            if(aa.getValue().equals("Commandable")) {
                net.n3.nanoxml.XMLElement child = new net.n3.nanoxml.XMLElement("cmd");
                String name = getFullName()+"."+aa.getName();
                child.setAttribute("name", name);
                map.put(name,child);
            }
            else if(aa.getValue().equals("Observable")) {
                net.n3.nanoxml.XMLElement child = new net.n3.nanoxml.XMLElement("obs");
                String name = getFullName()+"."+aa.getName();
                child.setAttribute("name", name);
                map.put(name,child);
            }
            else if(aa.getValue().equals("undefined")) {
                String name = getFullName()+"."+aa.getName();
                map.remove(name);
                
            }
            else if(aa.getValue().equals("default")) {
                System.out.println("Instance.createHarness() this shouldn't happen.");
            }
        }
    }
    
    public net.n3.nanoxml.XMLElement createInitialState() {
        net.n3.nanoxml.XMLElement xml = new net.n3.nanoxml.XMLElement("mplInit");
        xml.setAttribute("name", getName());
        xml.setAttribute("version","1.00");
        java.util.TreeMap map = new java.util.TreeMap();
        createInitialState(map);
        java.util.Iterator iter = map.keySet().iterator();
        while(iter.hasNext()) {
            String name = (String)iter.next();
            net.n3.nanoxml.XMLElement a = (net.n3.nanoxml.XMLElement)map.get(name);
            xml.addChild(a);
        }
        return xml;
    }
    
    /** Recursive call to populate the list for root element
     *  I'm using a map to avoid duplicates. */
    public void createInitialState(java.util.SortedMap map) {
        java.util.Enumeration enum1 = children();
        while(enum1.hasMoreElements()) {
            Instance child = (Instance)enum1.nextElement();
            child.createInitialState(map);
        }
        java.util.Enumeration enum2 = getType().initialStateElements();
        while(enum2.hasMoreElements()) {
            livdll.AttributeAssignment aa = (livdll.AttributeAssignment)enum2.nextElement();
            net.n3.nanoxml.XMLElement child = new net.n3.nanoxml.XMLElement("assign");
            String name = getFullName()+"."+aa.getName();
            child.setAttribute("eq", name+"="+aa.getValue());
            map.put(name,child);
        }
    }
    
    /** Returns the name starting with test for the top instance.
     *  Used to generate Harness. */
    private String getFullName() {
        StringBuffer fullname = new StringBuffer();
        javax.swing.tree.TreeNode[] path = getPath();
        if(path.length > 0) {
            for(int i = 0; i < path.length; i++) {
                fullname.append(((Instance)path[i]).getName());
                if(i < path.length-1) fullname.append(".");
            }
        }
        return fullname.toString();
    }
    
    public String getName() {
        return att.getAttribute("name","######");
    }
    
    public String getTypeName() {
        return att.getAttribute("type","######");
    }
    
    /** There could be a lot of things in the xmpl that I don't even know about
     *  This is used to find the index of a child that is being inserted. */
    protected static boolean willCreateInstance(Object xmpl) {
        if(xmpl instanceof Attribute) return true;
        else return false;
    }
    
    public void getVisibleNodes(javax.swing.JTree tree, java.util.Vector v) {
        java.util.Enumeration enum = children();
        while(enum.hasMoreElements()) {
            Instance child = ((Instance)enum.nextElement());
            if(child.getAllowsChildren()) child.getVisibleNodes(tree, v);
            javax.swing.tree.TreePath path = new javax.swing.tree.TreePath(child.getPath());
            if(tree.isVisible(path)) v.addElement(path);
        }
    }
    
    public void getNodes(java.util.Vector v) {
        java.util.Enumeration enum = children();
        while(enum.hasMoreElements()) {
            Instance child = ((Instance)enum.nextElement());
            if(child.getAllowsChildren()) child.getNodes(v);
            javax.swing.tree.TreePath path = new javax.swing.tree.TreePath(child.getPath());
            v.addElement(path);
        }
    }
    
    public static oliver.tree.Instance createInstance(oliver.project.ProjectModel model, Xmpl parent, Attribute att) throws Exception {
        String typename = att.getAttribute("type",null);
        if(typename == null) throw new Exception("Instance.createInstance() typename is null");
        Xmpl type = parent.findType(typename);
        if(type != null) {
            if(type instanceof AttributeType) {
                return new oliver.tree.AttributeInstance(model, parent, att);
            } else if(type instanceof ComponentType) {
                return new oliver.tree.ComponentInstance(model, parent, att);
            }
        }
        return null;
    }
    
    /** The changes in the tree are handled by the editorview.  This provides it with
     *  a list of the treenodes that have been effected by changes to an att or type.*/
    public Instance[] findInstancesForType(Xmpl x) {
        java.util.Vector v = new java.util.Vector();
        findInstancesForType(v, x);
        Instance[] r = new Instance[v.size()];
        v.copyInto(r);
        return r;
    }
    
    /** This is used to make inserts and deletes to the tree. */
    protected void findInstancesForType(java.util.Vector v, Xmpl x) {
        if(x == type) v.add(this);
        else {
            java.util.Enumeration enum = children();
            while(enum.hasMoreElements()) {
                Object obj = enum.nextElement();
                if(obj instanceof Instance) {
                    ((Instance)obj).findInstancesForType(v, x);
                }
            }
        }
    }
    
    /** This is used to make inserts and deletes to the tree. */
    public int findRemoveChildIndexForAtt(Xmpl att) {
        int i = -1;
        java.util.Enumeration enum = children();
        while(enum.hasMoreElements()) {
            i++;
            Instance child = (Instance)enum.nextElement();
            if(child.getAtt() == att) return i;
        }
        if(i == children.size() - 1) i = -1;
        return i;
    }
    
    /** This is used to make inserts and deletes to the tree. */
    public int findInsertChildIndexForAtt(Xmpl att) {
        int i = -1;
        int max = type.getChildrenCount();
        java.util.Enumeration enum = type.getChildren().elements();
        while(enum.hasMoreElements()) {
            Object child = enum.nextElement();
            if(willCreateInstance(child)) {
                i++; max--;
            }
            if(child == att) return i;
        }
        if(i == max - 1) i = -1;
        return i;
    }
    
    public void setAtt(Attribute att) {
        this.att = att;
    }
    
    public Attribute getAtt() {
        return att;
    }
    
    public void setType(Xmpl type) {
        this.type = type;
    }
    
    public Xmpl getType() {
        return type;
    }
    
    public Xmpl findType(String name) {
        return model.getRoot().findType(name);
    }
    
    public Instance getInstanceParent() {
        return (Instance)getParent();
    }
    
    public void setInstanceParent(Instance parent) {
        setParent(parent);
    }
    
    public oliver.project.ProjectModel getModel() {
        return model;
    }
    
    protected void removeXmplFromParent() throws Exception {
        att.removeXmplFromParent(this);
    }
    
    public javax.swing.Action[] getActions(java.awt.Component source, java.util.Properties userProperties) {
        //javax.swing.Action[] b = type.getActions(source, userProperties);
        javax.swing.Action[] b = new javax.swing.Action[0];
        javax.swing.Action[] a = new javax.swing.Action[2];
        javax.swing.AbstractAction action = new javax.swing.AbstractAction("Delete") {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                try {
                    removeXmplFromParent();
                } catch(Exception ex) {
                    System.out.println(ex);
                }
            }
        };
        a[0] = action;
        action = new javax.swing.AbstractAction("Properties") {
            public void actionPerformed(java.awt.event.ActionEvent e) {
                System.out.println("properties for an Instance");
            }
        };
        a[1] = action;
        javax.swing.Action[] c = addActions(a, b);
        return c;
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
    
    public abstract void updateChildChanged(oliver.project.ProjectModelUndoEvent event);
    
}
