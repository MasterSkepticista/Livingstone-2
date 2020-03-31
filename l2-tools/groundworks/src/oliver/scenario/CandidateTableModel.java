// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * CandidateTableModel.java
 *
 * Created on February 4, 2001, 2:49 PM
 */

package oliver.scenario;

/**
 *
 * @author  Stephen Wragg
 * @version 
 */
public class CandidateTableModel extends javax.swing.table.AbstractTableModel implements javax.swing.event.ListSelectionListener, java.beans.PropertyChangeListener {

    private java.util.List candidates = new java.util.Vector();
    
    /** Holds value of property table. */
    private javax.swing.JTable table;
    
    /** Holds value of property selectedFailures. */
    private livdll.CandidateAssignment[] selectedFailures;
    
    /** Utility field used by bound properties. */
    private java.beans.PropertyChangeSupport propertyChangeSupport =  new java.beans.PropertyChangeSupport (this);
        
    public boolean lastSelectionMadeByUser = true;
    
    /** Creates new CandidateTableModel */
    public CandidateTableModel() {
    }

    public int getColumnCount() {
        return 1;
    }
    
    public int getRowCount() {
        return candidates.size();
    }
    
    public java.lang.Object getValueAt(int row,int index) {
        Object obj = null;
        try {
            obj = candidates.get(row);
        } catch(ArrayIndexOutOfBoundsException e) {}
        return obj;
    }
    
    public void clear() {
        candidates.clear();
        fireTableDataChanged();
    }
    
    public void addCandidate(livdll.Candidate candidate) {
        int index = java.util.Collections.binarySearch(candidates,candidate);
        if(index > -1) {
            candidates.remove(index);
            candidates.add(index, candidate);
            fireTableRowsUpdated(index,index);
        } else {
            candidates.add(-1-index, candidate);
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
                if(obj != null && obj instanceof livdll.Candidate) {
                    livdll.CandidateAssignment[] a = ((livdll.Candidate)obj).getFailures();
                    if(a != null && a.length > 0) {
                        for(int ii = 0; ii < a.length; ii++)
                            v.add(a[ii]);
                    }
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
        if(e.getPropertyName().equals("selectedFailures")) {
            lastSelectionMadeByUser = false;
            livdll.CandidateAssignment[] show = (livdll.CandidateAssignment[])e.getNewValue();
            table.clearSelection();
            if(show != null && show.length > 0) {
                for(int i = 0; i < show.length; i++)
                    selectCandidatesWithThisFailure(show[i]);
            }
            javax.swing.SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    lastSelectionMadeByUser = true;
                }
            });
        }
    }
    
    private void selectCandidatesWithThisFailure(livdll.CandidateAssignment show) {
        java.util.Iterator iter = candidates.iterator();
        while(iter.hasNext()) {
            livdll.Candidate c = (livdll.Candidate)iter.next();
            livdll.CandidateAssignment[] f = c.getFailures();
            if(f != null && f.length > 0) {
                for(int i = 0; i < f.length; i++) {
                    if(show.equals(f[i])) {
                        int index = candidates.indexOf(c);
                        table.addRowSelectionInterval(index,index);
                    }
                }
            }
        }
    }
    
    public String getColumnName(int column) {
        return "Candidates";
    }
    
}
