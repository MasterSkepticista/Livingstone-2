// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * MyComboBoxTableCellEditor.java
 *
 * Created on November 18, 2002, 1:49 PM
 */

package oliver.tree;

/**
 *
 * @author  stephen
 */
public class MyComboBoxTableCellEditor extends javax.swing.DefaultCellEditor {
    
    /** Creates a new instance of MyComboBoxTableCellEditor */
    public MyComboBoxTableCellEditor() {
        super(new javax.swing.JComboBox());
        delegate = new EditorDelegate() {
	    public void setValue(Object value) {
                if(value == null) return;
		((javax.swing.JComboBox)editorComponent).setModel((javax.swing.DefaultComboBoxModel)value);
            }
	    public Object getCellEditorValue() {
		return ((javax.swing.JComboBox)editorComponent).getSelectedItem();
	    }
            public boolean shouldSelectCell(java.util.EventObject anEvent) { 
                if (anEvent instanceof java.awt.event.MouseEvent) { 
                    java.awt.event.MouseEvent e = (java.awt.event.MouseEvent)anEvent;
                    return e.getID() != java.awt.event.MouseEvent.MOUSE_DRAGGED;
                }
                return true;
            }
	    public boolean stopCellEditing() {
		if (((javax.swing.JComboBox)editorComponent).isEditable()) {
		    // Commit edited value.
		    ((javax.swing.JComboBox)editorComponent).actionPerformed(new java.awt.event.ActionEvent(
				     MyComboBoxTableCellEditor.this, 0, ""));
		}
		return super.stopCellEditing();
	    }
        };
	((javax.swing.JComboBox)editorComponent).addActionListener(delegate);
    }
}
