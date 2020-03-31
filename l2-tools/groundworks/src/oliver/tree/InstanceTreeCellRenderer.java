// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * InstanceTreeCellRenderer.java
 *
 * Created on July 19, 2002, 1:17 PM
 */

package oliver.tree;

/**
 *
 * @author  stephen
 */
public class InstanceTreeCellRenderer extends javax.swing.tree.DefaultTreeCellRenderer {
    
    private javax.swing.Icon icon1 = new javax.swing.ImageIcon(getClass().getResource("/oliver/tree/4dot1a.gif"));
    private javax.swing.Icon icon2 = new javax.swing.ImageIcon(getClass().getResource("/oliver/tree/2dot1a.gif"));
    private javax.swing.Icon icon3 = new javax.swing.ImageIcon(getClass().getResource("/oliver/tree/16dot4a.gif"));
    
    /** Creates a new instance of InstanceTreeCellRenderer */
    public InstanceTreeCellRenderer() {
        super();
        setLeafIcon(icon3);
    }
        
    public java.awt.Component getTreeCellRendererComponent(javax.swing.JTree tree, Object value,
						  boolean sel,
						  boolean expanded,
						  boolean leaf, int row,
						  boolean hasFocus) {
        if(value instanceof oliver.tree.ComponentInstance) {
            boolean hasState = ((oliver.xmpl.ComponentType)((oliver.tree.ComponentInstance)value).getType()).getHasState();
            if(hasState) {
                setClosedIcon(icon1);
                setOpenIcon(icon1);
            } else {
                setClosedIcon(icon2);
                setOpenIcon(icon2);
            }
        } else {
            setClosedIcon(getDefaultClosedIcon());
            setOpenIcon(getDefaultOpenIcon());
        }
        return super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);
    }

}
