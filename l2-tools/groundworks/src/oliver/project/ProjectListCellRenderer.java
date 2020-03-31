// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * ProjectListCellRenderer.java
 *
 * Created on July 17, 2002, 1:14 PM
 */

package oliver.project;

/**
 *
 * @author  stephen
 */
public class ProjectListCellRenderer extends javax.swing.DefaultListCellRenderer {
    private javax.swing.Icon icon1 = new javax.swing.ImageIcon(getClass().getResource("/oliver/tree/4dot1a.gif"));
    private javax.swing.Icon icon2 = new javax.swing.ImageIcon(getClass().getResource("/oliver/tree/2dot1a.gif"));
    private javax.swing.Icon icon3 = new javax.swing.ImageIcon(getClass().getResource("/oliver/tree/16dot4a.gif"));
    public ProjectListCellRenderer() {
        super();
    }
    public java.awt.Component getListCellRendererComponent(javax.swing.JList list, Object value, int index, boolean isSelected, boolean cellHasFocus) {
        if(value instanceof oliver.xmpl.ComponentType) {
            boolean hasState = ((oliver.xmpl.ComponentType)value).getHasState();
            if(hasState) setIcon(icon1);
            else setIcon(icon2);
            value = ((oliver.xmpl.Xmpl)value).getAttribute("name","######");
        } else if(value instanceof oliver.xmpl.AttributeType) {
            setIcon(icon3);
            value = ((oliver.xmpl.Xmpl)value).getAttribute("name","######");
        } else {
            setIcon(null);
        }
        
        setComponentOrientation(list.getComponentOrientation());
        if (isSelected) {
            setBackground(list.getSelectionBackground());
            setForeground(list.getSelectionForeground());
        }
        else {
            setBackground(list.getBackground());
            setForeground(list.getForeground());
        }
        
        setText((value == null) ? "" : value.toString());
        
        setEnabled(list.isEnabled());
        setFont(list.getFont());
        setBorder((cellHasFocus) ? javax.swing.UIManager.getBorder("List.focusCellHighlightBorder") : noFocusBorder);
        
        return this;
    }
}