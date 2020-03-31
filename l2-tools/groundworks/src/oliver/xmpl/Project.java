// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

package oliver.xmpl;

public class Project extends Xmpl {
    
    private oliver.project.ProjectListener model;
    private oliver.xmpl.Xmpl lastSelectedNode = null;
    private static int gridsize = 5;
    
    public Project() {
        super();
    }
    
    public Project(String fullName, String namespace, String systemID, int lineNr) {
        super(fullName, namespace, systemID, lineNr);
    }
    
    public oliver.project.ProjectListener getModel() {
        return model;
    }
    
    public void setModel(oliver.project.ProjectListener model) {
        this.model = model;
    }
    
    public static int getGridSize() {
        return gridsize;
    }
    
    public oliver.xmpl.Xmpl getLastSelectedNode() {
        return lastSelectedNode;
    }
    
    public void setLastSelectedNode(oliver.xmpl.Xmpl lastSelectedNode) {
        this.lastSelectedNode = lastSelectedNode;
    }
    
    public void writeJmpl(java.io.PrintWriter writer, boolean prettyprint, int indent) throws java.io.IOException {
        Class lastChildClass = null;
        java.util.Enumeration enum = getChildren().elements();
        while(enum.hasMoreElements()) {
            Object obj = enum.nextElement();
            if(obj instanceof Xmpl) {
                if(obj.getClass() != lastChildClass) writer.println();
                ((Xmpl)obj).writeJmpl(writer, prettyprint, indent);
                writer.println();
                writer.println();
                lastChildClass = obj.getClass();
            }
        }
    }

    public void fireNodeSelected(Object source, oliver.xmpl.Xmpl node) {
        if(model == null) return;
        model.undoableEditHappened(new javax.swing.event.UndoableEditEvent(source,oliver.project.ProjectModelUndoEvent.createNodeSelectedEvent(source, node, lastSelectedNode)));
    }
    public void fireNodeInstanceVisible(Object source, oliver.xmpl.Xmpl node) {
        if(model == null) return;
        model.undoableEditHappened(new javax.swing.event.UndoableEditEvent(source,oliver.project.ProjectModelUndoEvent.createNodeInstanceVisibleEvent(source, node)));
    }
    
    public void fireNodeInstanceRoot(Object source, oliver.xmpl.Xmpl node) {
        if(model == null) return;
        String name = node.getAttribute("name", null);
        if(name != null) setAttribute("name", name);
        model.undoableEditHappened(new javax.swing.event.UndoableEditEvent(source,oliver.project.ProjectModelUndoEvent.createNodeInstanceRoot(source, node)));
    }
    // Invoked after a node has changed in some way.
    public void fireNodesChanged(Object source, oliver.xmpl.Xmpl node, String name, String newvalue, String oldvalue) {
        if(model == null) return;
        model.undoableEditHappened(new javax.swing.event.UndoableEditEvent(source,oliver.project.ProjectModelUndoEvent.createNodeChangedEvent(source, node, name, newvalue, oldvalue)));
    }
    // Invoked after nodes have been inserted into the tree.
    public void fireNodesInserted(Object source, oliver.xmpl.Xmpl parent, int childIndex, oliver.xmpl.Xmpl child) {
        if(model == null) return;
        model.undoableEditHappened(new javax.swing.event.UndoableEditEvent(source,oliver.project.ProjectModelUndoEvent.createNodeInsertedEvent(source, parent, childIndex, child)));
    }
    // Invoked after nodes have been removed from the tree.
    public void fireNodesRemoved(Object source, oliver.xmpl.Xmpl parent, int childIndex, oliver.xmpl.Xmpl child) {
        if(model == null) return;
        model.undoableEditHappened(new javax.swing.event.UndoableEditEvent(source,oliver.project.ProjectModelUndoEvent.createNodeRemovedEvent(source, parent, childIndex, child)));
    }
    // Invoked after the tree has drastically changed structure from a given node down.
    public void fireEverythingChanged(Object source, oliver.xmpl.Xmpl parent) {
        if(model == null) return;
        model.undoableEditHappened(new javax.swing.event.UndoableEditEvent(source,oliver.project.ProjectModelUndoEvent.createEverythingChangedEvent(source, parent)));
    }
    
    
}
