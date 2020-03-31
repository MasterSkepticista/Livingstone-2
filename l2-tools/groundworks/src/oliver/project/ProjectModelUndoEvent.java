// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * ProjectModelUndoEvent.java
 *
 * Created on June 27, 2002, 1:43 PM
 */

package oliver.project;

import oliver.xmpl.*;

/**
 *
 * @author  stephen
 */
public class ProjectModelUndoEvent extends javax.swing.undo.AbstractUndoableEdit {
    
    public static int NODES_CHANGED = 0;
    public static int NODES_INSERTED = 1;
    public static int NODES_REMOVED = 2;
    public static int EVERYTHING_CHANGED = 3;
    public static int NODE_SELECTED = 4;
    public static int NODE_INSTANCE_ROOT = 5;
    public static int NODE_INSTANCE_VISIBLE = 6;
    public static int NODE_COMPILED = 7;
    public static String[] names = {"node changed","node inserted","node removed","everything changed","node selected","node instance root","node instance visible","node compiled"};
    
    
    private int type;
    private Object source;  // could be an undo edit like this
    private Xmpl parent;
    private String name;
    private String newvalue;
    private String oldvalue;
    private int childIndex;
    private Xmpl child;
    
    public String toString() {
        StringBuffer buf = new StringBuffer();
        buf.append(names[type]+":"+parent.getAttribute("name", "######")+" ");
        if(type == 0) {
            buf.append(name+" "+oldvalue+" -> "+newvalue);
        } else if(type == 1) {
            buf.append(child.getAttribute("name","######")+" at "+childIndex);
        } else if(type == 2) {
            buf.append(child.getAttribute("name","######")+" at "+childIndex);
        } else if(type == 7) {
        } else {
            buf.append(super.toString());
        }
        return buf.toString();
    }
    
    /** Creates a new instance of ProjectModelUndoEvent */
    protected ProjectModelUndoEvent(int type, Object source, Xmpl parent, String name, String newvalue, String oldvalue, int childIndex, Xmpl child) {
        super();
        this.type = type;
        this.source = source;
        this.parent = parent;
        this.name = name;
        this.newvalue = newvalue;
        this.oldvalue = oldvalue;
        this.childIndex = childIndex;
        this.child = child;
    }
    
    public int getType() { return type; }
    public Object getSource() { return source; }
    public Xmpl getParent() { return parent; }
    public String getName() { return name; }
    public String getNewValue() { return newvalue; }
    public String getOldValue() { return oldvalue; }
    public int getChildIndex() { return childIndex; }
    public Xmpl getChild() { return child; }
    
    public boolean isSignificant() {
        if(type == NODE_SELECTED) return false;
        else return super.isSignificant();
    }
    
    public boolean canUndo() {
        if(type == NODES_REMOVED
        || type == NODES_INSERTED
        || type == NODE_SELECTED) return true;
        return false;
    }
    
    // The source is "this" so undos dont' get added to the undo stack
    public void undo() throws javax.swing.undo.CannotUndoException {
        if(type==NODES_REMOVED) {
            if(child != null && parent != null && child instanceof Xmpl &&
            childIndex > -1 && childIndex <= parent.getChildrenCount()) {
                try {
                    parent.insertXmplChild(this,(Xmpl)child, childIndex);
                } catch(Exception e) {
                    throw new javax.swing.undo.CannotUndoException();
                }
            } else {
                throw new javax.swing.undo.CannotUndoException();
            }
            
        } else if(type==NODES_INSERTED) {
            if(child != null && parent != null && child instanceof Xmpl) {
                try {
                    child.removeXmplFromParent(this);
                } catch(Exception e) {
                    throw new javax.swing.undo.CannotUndoException();
                }
            } else {
                throw new javax.swing.undo.CannotUndoException();
            }
            
        } else if(type==NODE_SELECTED) {
            if(child != null) {
                try {
                    child.selectXmplNode(this);
                } catch(Exception e) {
                    throw new javax.swing.undo.CannotUndoException();
                }
            } else {
                throw new javax.swing.undo.CannotUndoException();
            }
        }
        
    }
    
    public static ProjectModelUndoEvent createNodeChangedEvent(Object source, Xmpl parent, String name, String newvalue, String oldvalue) {
        return new ProjectModelUndoEvent(NODES_CHANGED,source,parent,name,newvalue,oldvalue,-1,null);
    }
    public static ProjectModelUndoEvent createNodeInsertedEvent(Object source, Xmpl parent, int childIndex, Xmpl child) {
        return new ProjectModelUndoEvent(NODES_INSERTED,source,parent,null,null,null,childIndex,child);
    }
    public static ProjectModelUndoEvent createNodeRemovedEvent(Object source, Xmpl parent, int childIndex, Xmpl child) {
        return new ProjectModelUndoEvent(NODES_REMOVED,source,parent,null,null,null,childIndex,child);
    }
    public static ProjectModelUndoEvent createEverythingChangedEvent(Object source, Xmpl root) {
        return new ProjectModelUndoEvent(EVERYTHING_CHANGED,source,root,null,null,null,-1,null);
    }
    public static ProjectModelUndoEvent createNodeSelectedEvent(Object source, Xmpl parent, Xmpl child) {
        // child is the old selection for undos
        return new ProjectModelUndoEvent(NODE_SELECTED,source,parent,null,null,null,-1,child);
    }
    public static ProjectModelUndoEvent createNodeInstanceVisibleEvent(Object source, Xmpl node) {
        return new ProjectModelUndoEvent(NODE_INSTANCE_VISIBLE,source,node,null,null,null,-1,null);
    }
    public static ProjectModelUndoEvent createNodeInstanceRoot(Object source, Xmpl node) {
        return new ProjectModelUndoEvent(NODE_INSTANCE_ROOT,source,node,null,null,null,-1,null);
    }
    
}
