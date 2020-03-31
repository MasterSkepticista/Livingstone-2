// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * MyComboBoxModel.java
 *
 * Created on November 19, 2002, 2:54 PM
 */

package oliver.tree;

/**
 *
 * @author  stephen
 */
public class MyComboBoxModel extends javax.swing.DefaultComboBoxModel {
    
    /** Holds value of property isDefault. */
    private boolean isDefault = false;
    
    /** Creates a new instance of MyComboBoxModel */
    public MyComboBoxModel() {
        super();
    }
    
    /** Creates a new instance of MyComboBoxModel */
    public MyComboBoxModel(Object[] items) {
        super(items);
    }
    
    /** Creates a new instance of MyComboBoxModel */
    public MyComboBoxModel(java.util.Vector v) {
        super(v);
    }

    
    /** Getter for property isDefault.
     * @return Value of property isDefault.
     *
     */
    public boolean getIsDefault() {
        return this.isDefault;
    }
    
    /** Setter for property isDefault.
     * @param isDefault New value of property isDefault.
     *
     */
    public void setIsDefault(boolean isDefault) {
        this.isDefault = isDefault;
    }
    
}
