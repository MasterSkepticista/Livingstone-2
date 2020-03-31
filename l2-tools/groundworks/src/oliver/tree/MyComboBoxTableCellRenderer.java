// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * MyComboBoxTableCellRenderer.java
 *
 * Created on November 18, 2002, 2:54 PM
 */

package oliver.tree;

/**
 *
 * @author  stephen
 */
public class MyComboBoxTableCellRenderer extends javax.swing.table.DefaultTableCellRenderer {
    
    /**  Returns the component used for drawing the cell.  This method is
     *  used to configure the renderer appropriately before drawing.
     *
     * @param	table		the <code>JTable</code> that is asking the
     * 				renderer to draw; can be <code>null</code>
     * @param	value		the value of the cell to be rendered.  It is
     * 				up to the specific renderer to interpret
     * 				and draw the value.  For example, if
     * 				<code>value</code>
     * 				is the string "true", it could be rendered as a
     * 				string or it could be rendered as a check
     * 				box that is checked.  <code>null</code> is a
     * 				valid value
     * @param	isSelected	true if the cell is to be rendered with the
     * 				selection highlighted; otherwise false
     * @param	hasFocus	if true, render cell appropriately.  For
     * 				example, put a special border on the cell, if
     * 				the cell can be edited, render in the color used
     * 				to indicate editing
     * @param	row	        the row index of the cell being drawn.  When
     * 				drawing the header, the value of
     * 				<code>row</code> is -1
     * @param	column	        the column index of the cell being drawn
     *
     */
    public java.awt.Component getTableCellRendererComponent(javax.swing.JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        super.getTableCellRendererComponent(table,value,isSelected,hasFocus,row,column);
        if(value instanceof MyComboBoxModel) {
            MyComboBoxModel m = (MyComboBoxModel)value;
            setText(m.getSelectedItem().toString());
            if(m.getIsDefault()) setFont(getFont().deriveFont(java.awt.Font.PLAIN));
            else setFont(getFont().deriveFont(java.awt.Font.BOLD));
        } else if(value instanceof javax.swing.DefaultComboBoxModel) {
            javax.swing.DefaultComboBoxModel m = (javax.swing.DefaultComboBoxModel)value;
            setText(m.getSelectedItem().toString());
            setFont(getFont().deriveFont(java.awt.Font.PLAIN));
        }
        return this;
    }
    
}
