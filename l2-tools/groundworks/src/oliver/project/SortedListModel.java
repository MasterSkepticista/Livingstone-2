// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * SortedListModel.java
 *
 * Created on July 17, 2002, 4:30 PM
 */

package oliver.project;

/**
 *
 * @author  stephen
 */
public class SortedListModel extends javax.swing.AbstractListModel {
    
    private java.util.Vector delegate = new java.util.Vector();
    
    /** Creates a new instance of SortedListModel */
    public SortedListModel() {
    }
    
    /** Returns the value at the specified index.
     * @param index the requested index
     * @return the value at <code>index</code>
     */
    public Object getElementAt(int index) {
        return delegate.elementAt(index);
    }
    
    /**
     * Returns the length of the list.
     * @return the length of the list
     */
    public int getSize() {
        return delegate.size();
    }
        
    public int getInsertIndexOf(Object obj) {
    	return -(1 + java.util.Collections.binarySearch(delegate,obj));
    }

    
    public void removeElement(Object obj) {
        int i = delegate.indexOf(obj);
        if(i < 0) return; 
        delegate.remove(obj);
        fireIntervalRemoved(this, i, i);
    }
    
    /**
     * Adds the specified component to the list. 
     *
     * @param   obj   the component to be added
     */
    public void addElement(Object obj) {
	int index = -(1 + java.util.Collections.binarySearch(delegate,obj));
        if(index > -1) {
            delegate.insertElementAt(obj, index);
            fireIntervalAdded(this, index, index);
        }
    }

}
