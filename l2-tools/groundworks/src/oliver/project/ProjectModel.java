// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * ProjectModel.java
 *
 * Created on September 13, 2002, 10:55 AM
 */

package oliver.project;

import oliver.xmpl.*;

/**
 *
 * @author  stephen
 */
public class ProjectModel implements oliver.project.ProjectListener {
    
    private Project root = null;
    
    /** Utility field used by event firing mechanism. */
    private javax.swing.event.EventListenerList listenerList =  null;
    
    public Project getRoot() { return root; }
    
    public void setRoot(Project root) {
        if(this.root != null) this.root.setModel(null);
        this.root = root;
        if(root != null) root.setModel(this);
        javax.swing.event.UndoableEditEvent undoEvent = new javax.swing.event.UndoableEditEvent(this, ProjectModelUndoEvent
        .createEverythingChangedEvent(this, root));
        undoableEditHappened(undoEvent);
    }
    
    
    /* name should be non null, unique and legal. */
    public void addNewAttributeType(Object source, String name) {
        if(name != null && oliver.xmpl.Xmpl.isLegalName(name)
        && root.findFirstNodeWithNameTag(name, root.getChildren()) == null) {
            
            oliver.xmpl.AttributeType at = new oliver.xmpl.AttributeType();
            at.setName("attributetype");
            at.setAttribute("name", name);
            
            root.insertXmplChild(this, at, 0);
            
        }
    }
    
    /* name should be non null, unique and legal. */
    public void addNewComponentType(Object source, String name) {
        if(name != null && oliver.xmpl.Xmpl.isLegalName(name)
        && root.findFirstNodeWithNameTag(name, root.getChildren()) == null) {
            
            oliver.xmpl.ComponentType ct = new oliver.xmpl.ComponentType();
            ct.setName("componenttype");
            ct.setAttribute("name", name);
            
            root.insertXmplChild(this, ct, 0);
            
        }
    }
    
    public oliver.xmpl.Xmpl getSelectedNode() {
        return root.getLastSelectedNode();
    }
    
    /** Registers UndoableEditListener to receive events.
     * @param listener The listener to register.
     */
    public synchronized void addUndoableEditListener(javax.swing.event.UndoableEditListener listener) {
        if (listenerList == null ) {
            listenerList = new javax.swing.event.EventListenerList();
        }
        listenerList.add(javax.swing.event.UndoableEditListener.class, listener);
    }
    
    /** Removes UndoableEditListener from the list of listeners.
     * @param listener The listener to remove.
     */
    public synchronized void removeUndoableEditListener(javax.swing.event.UndoableEditListener listener) {
        if (listenerList == null ) {
            listenerList.remove(javax.swing.event.UndoableEditListener.class, listener);
            if(listenerList.getListenerCount() == 0) listenerList = null;
        }
    }
    
    public synchronized void removeAllListeners() {
        listenerList = null;
    }
    
    /** An undoable edit happened
     *
     */
    public void undoableEditHappened(javax.swing.event.UndoableEditEvent e) {
        if(e.getEdit() instanceof oliver.project.ProjectModelUndoEvent) {
            oliver.project.ProjectModelUndoEvent edit = (oliver.project.ProjectModelUndoEvent)e.getEdit();
            if(edit.getType() == edit.NODE_SELECTED) {
                root.setLastSelectedNode(edit.getParent());
            }
        }
        
        if (listenerList == null || listenerList.getListenerCount() == 0) return;
        Object[] listeners = listenerList.getListenerList();
        for (int i = listeners.length-2; i>=0; i-=2) {
            if (listeners[i]==javax.swing.event.UndoableEditListener.class) {
                ((javax.swing.event.UndoableEditListener)listeners[i+1]).undoableEditHappened(e);
            }
        }
    }
    
}
