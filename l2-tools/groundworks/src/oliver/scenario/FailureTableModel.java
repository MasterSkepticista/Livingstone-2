// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * FailureTableModel.java
 *
 * Created on February 4, 2001, 2:49 PM
 */

package oliver.scenario;

/**
 *
 * @author  Stephen Wragg
 * @version 
 */
public class FailureTableModel extends javax.swing.table.AbstractTableModel implements javax.swing.event.ListSelectionListener, java.beans.PropertyChangeListener {

    private java.util.List failures = new java.util.Vector();
    
    /** Holds value of property table. */
    private javax.swing.JTable table;
    
    /** Holds value of property selectedFailures. */
    private livdll.CandidateAssignment[] selectedFailures;
    
    /** Utility field used by bound properties. */
    private java.beans.PropertyChangeSupport propertyChangeSupport =  new java.beans.PropertyChangeSupport (this);
    
    public boolean lastSelectionMadeByUser = true;
    
    /** Creates new CandidateTableModel */
    public FailureTableModel() {
    }

    public int getColumnCount() {
        return 1;
    }
    
    public int getRowCount() {
        return failures.size();
    }
    
    public java.lang.Object getValueAt(int row,int index) {
        return failures.get(row);
    }
    
    public void clear() {
        failures.clear();
        fireTableDataChanged();
    }
    
    public void addFailure(livdll.CandidateAssignment failure) {
        int index = java.util.Collections.binarySearch(failures,failure);
        if(index > -1) {
            failures.remove(index);
            failures.add(index, failure);
            fireTableRowsUpdated(index,index);
        } else {
            failures.add(-1-index, failure);
            fireTableRowsInserted(-1-index,-1-index);
        }
    }

    public void valueChanged(javax.swing.event.ListSelectionEvent e) {
        if(!lastSelectionMadeByUser || e.getValueIsAdjusting()) return;
        javax.swing.ListSelectionModel s = (javax.swing.ListSelectionModel)e.getSource();
        java.util.TreeSet v = new java.util.TreeSet();
        for(int i = s.getMinSelectionIndex(); i <= s.getMaxSelectionIndex(); i++) {
            if(s.isSelectedIndex(i)) {
                Object obj = table.getValueAt(i,0);
                if(obj != null && obj instanceof livdll.CandidateAssignment) {
                    v.add(obj);
                }
            }
        }
        livdll.CandidateAssignment[] aa = new livdll.CandidateAssignment[v.size()];
        v.toArray(aa);
        setSelectedFailures(aa);
    }
    
    /** Getter for property table.
     * @return Value of property table.
 */
    public javax.swing.JTable getTable() {
        return table;
    }
    
    /** Setter for property table.
     * @param table New value of property table.
 */
    public void setTable(javax.swing.JTable table) {
        this.table = table;
    }
    
    /** Add a PropertyChangeListener to the listener list.
     * @param l The listener to add.
 */
    public void addPropertyChangeListener(java.beans.PropertyChangeListener l) {
        propertyChangeSupport.addPropertyChangeListener (l);
    }
    
    /** Removes a PropertyChangeListener from the listener list.
     * @param l The listener to remove.
 */
    public void removePropertyChangeListener(java.beans.PropertyChangeListener l) {
        propertyChangeSupport.removePropertyChangeListener (l);
    }
    
    /** Getter for property selectedFailures.
     * @return Value of property selectedFailures.
 */
    public livdll.CandidateAssignment[] getSelectedFailures() {
        return selectedFailures;
    }
    
    /** Setter for property selectedFailures.
     * @param selectedFailures New value of property selectedFailures.
 */
    public void setSelectedFailures(livdll.CandidateAssignment[] selectedFailures) {
        livdll.CandidateAssignment[] oldSelectedFailures = this.selectedFailures;
        this.selectedFailures = selectedFailures;
        propertyChangeSupport.firePropertyChange ("selectedFailures", oldSelectedFailures, selectedFailures);
    }
    
    public void propertyChange(java.beans.PropertyChangeEvent e) {
        if(!lastSelectionMadeByUser || e.getPropertyName().equals("selectedFailures")) {
            lastSelectionMadeByUser = false;
            livdll.CandidateAssignment[] show = (livdll.CandidateAssignment[])e.getNewValue();
            table.clearSelection();
            if(show != null && show.length > 0) {
                for(int i = 0; i < show.length; i++)
                    selectThisFailures(show[i]);
            }
            javax.swing.SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    lastSelectionMadeByUser = true;
                }
            });
        }
    }
    
    private void selectThisFailures(livdll.CandidateAssignment show) {
        java.util.Iterator iter = failures.iterator();
        while(iter.hasNext()) {
            livdll.CandidateAssignment a = (livdll.CandidateAssignment)iter.next();
            if(show.equals(a)) {
                int index = failures.indexOf(a);
                table.addRowSelectionInterval(index,index);
            }
        }
    }
    
    public void addFailures(livdll.CandidateAssignment[] a) {
        if(a != null && a.length > 0) {
            for(int i = 0; i < a.length; i++) {
                livdll.CandidateAssignment f = a[i];
                addFailure(f);
            }
        }
    }

    public String getColumnName(int column) {
        return "Failed Components";
    }
    
}
