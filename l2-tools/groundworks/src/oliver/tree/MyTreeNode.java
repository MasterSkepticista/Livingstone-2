// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * MyTreeNode.java
 *
 * Created on July 9, 2002, 10:12 AM
 */

package oliver.tree;

/**
 *
 * @author  stephen
 */
public abstract class MyTreeNode extends javax.swing.tree.DefaultMutableTreeNode {

    protected static javax.swing.tree.DefaultTreeCellRenderer renderer = new javax.swing.tree.DefaultTreeCellRenderer();
    
    public abstract javax.swing.Action[] getActions(java.awt.Component source, java.util.Properties userProperties);
    public static javax.swing.tree.TreeCellRenderer getRenderer() {
        return renderer;
    }
}
