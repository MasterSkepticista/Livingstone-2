// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * InstanceTreeTableModel.java
 *
 * Created on July 29, 2002, 4:31 PM
 */

package oliver.tree;

/**
 *
 * @author  stephen
 */
public class InstanceTreeTableModel extends oliver.tree.treetable.AbstractTreeTableModel {
    
    static protected String[] columnNames = { "Name","type","Initial Value","Command/Observable" };
    static protected Class[] cTypes = { oliver.tree.treetable.TreeTableModel.class,String.class,MyComboBoxModel.class,javax.swing.DefaultComboBoxModel.class};
    static protected String[] harnessValues = { "Commandable", "Observable", "undefined", "default" }; // the fourth un-named value is State Variable
    static protected String[] modeHarnessValues = { "undefined", "default" };
    /** Creates a new instance of InstanceTreeTableModel */
    public InstanceTreeTableModel(Instance root) {
        super(root);
    }
    /** Returns the child of <code>parent</code> at index <code>index</code>
     * in the parent's
     * child array.  <code>parent</code> must be a node previously obtained
     * from this data source. This should not return <code>null</code>
     * if <code>index</code>
     * is a valid index for <code>parent</code> (that is <code>index >= 0 &&
     * index < getChildCount(parent</code>)).
     *
     * @param   parent  a node in the tree, obtained from this data source
     * @return  the child of <code>parent</code> at index <code>index</code>
     */
    public Object getChild(Object parent, int index) {
        return ((Instance)parent).getChildAt(index);
    }
    
    /** Returns the number of children of <code>parent</code>.
     * Returns 0 if the node
     * is a leaf or if it has no children.  <code>parent</code> must be a node
     * previously obtained from this data source.
     *
     * @param   parent  a node in the tree, obtained from this data source
     * @return  the number of children of the node <code>parent</code>
     */
    public int getChildCount(Object parent) {
        return ((Instance)parent).getChildCount();
    }
    
    /** Returns the number ofs availible column.
     */
    public int getColumnCount() {
        return 4;
    }
    
    /** Returns the name for column number <code>column</code>.
     */
    public String getColumnName(int column) {
        return columnNames[column];
    }
    
    public Class getColumnClass(int column) {
        return cTypes[column];
    }
    
    public boolean isCellEditable(Object node, int column) {
        if(column==0) return super.isCellEditable(node, column);
        else if(column==2) {
            if(node instanceof AttributeInstance) return true;
            else return false;
        } else if(column==3) {
            if(node instanceof AttributeInstance ) {
                String n = ((AttributeInstance)node).getHarnessValue();
                if(n != null && n.equals("State Variable")) return false;
                else return true;
            }
            else return false;
        } else {
            return column > 1;
        }
    }
    
    public void setValueAt(Object value, Object node, int col) {
        if(node instanceof AttributeInstance) {
            if(col==2) {
                if(value == "default") value = null;
                ((AttributeInstance)node).setInitialValue((String)value);
            } else if(col==3) {
                if(value == "default") value = null;
                ((AttributeInstance)node).setHarnessValue((String)value);
            }
        }
    }
    
    /** Returns the value to be displayed for node <code>node</code>,
     * at column number <code>column</code>.
     */
    public Object getValueAt(Object node, int column) {
        if(node instanceof AttributeInstance) {
            AttributeInstance n = (AttributeInstance)node;
            if(column == 0) {
                return this;
            } else if(column == 1) {
                return n.getTypeName();
            } else if(column == 2) {
                MyComboBoxModel m = new MyComboBoxModel(((oliver.xmpl.AttributeType)n.getType()).getValues());
                String hv = n.getHarnessValue();
                
                if(!n.isInStateVector()) m.addElement("undefined");  // state variable can not be undefined
                m.addElement("default");
                String v = n.getInitialValue();
                if(v != null) {
                    int i = v.indexOf("(default)");
                    if(i > -1) {
                        v = v.substring(0,i);
                        m.setIsDefault(true);
                    } else {
                        m.setIsDefault(false);
                    }
                    m.setSelectedItem(v);
                } else {
                    m.setSelectedItem("undefined");
                    m.setIsDefault(true);
                }
                return m;
            } else if(column == 3) {
                if(n.isInStateVector()) {
                    MyComboBoxModel m = new MyComboBoxModel(modeHarnessValues);
                    m.setIsDefault(true);
                    return m;
                }
                MyComboBoxModel m = new MyComboBoxModel(harnessValues);
                String v = n.getHarnessValue();
                if(v != null) {
                    int i = v.indexOf("(default)");
                    if(i > -1) {
                        v = v.substring(0,i);
                        m.setIsDefault(true);
                    } else {
                        m.setIsDefault(false);
                    }
                    m.setSelectedItem(v);
                } else {
                    m.setIsDefault(true);
                    m.setSelectedItem(harnessValues[2]);
                }
                return m;
            } else {
                return null;
            }
        } else if(node instanceof ComponentInstance) {
            ComponentInstance n = (ComponentInstance)node;
            if(column == 0) {
                return this;
            } else if(column == 1) {
                return n.getTypeName();
            } else if(column == 2) {
                return null;
            } else if(column == 3) {
                return null;
            } else {
                return null;
            }
        } else {
            return null;
        }
    }
}
