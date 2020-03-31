// 
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES. 
// 

/*
 * LivingstoneViewer.java
 *
 * Created on February 4, 2001, 12:17 PM
 */

package oliver.scenario;

/**
 *
 * @author  Stephen Wragg
 * @version
 */
public class CandidatePanel extends javax.swing.JPanel implements javax.swing.event.ListSelectionListener, HistoryControlInterface {
    
    private int fcIndex = -1;
    private boolean autoUpdate = true;
    private boolean showTrailingCommands = true;
    private java.util.Vector findCanidates = new java.util.Vector();
    private java.util.HashSet listeners = new java.util.HashSet();
    
    /** Creates new form LivingstoneViewer */
    public CandidatePanel() {
        initComponents();
        candidateTableModel.addPropertyChangeListener(failureTableModel);
        failureTableModel.addPropertyChangeListener(candidateTableModel);
        candidateTable.getSelectionModel().addListSelectionListener(this);
    }
    
    public void valueChanged(javax.swing.event.ListSelectionEvent e) {
        livdll.Candidate c = null;
        int index = candidateTable.getSelectedRow();
        if(index > -1)
            c = (livdll.Candidate)candidateTableModel.getValueAt(index, 0);
        setSelectedCandidate(c);
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the FormEditor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        splitPane = new javax.swing.JSplitPane();
        candidateScrollPane = new javax.swing.JScrollPane();
        candidateTable = new javax.swing.JTable();
        failureScrollPane = new javax.swing.JScrollPane();
        failureTable = new javax.swing.JTable();

        setLayout(new java.awt.BorderLayout());

        setBorder(new javax.swing.border.TitledBorder("Livingstone"));
        splitPane.setDividerSize(5);
        splitPane.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        splitPane.setResizeWeight(0.4);
        candidateTable.setModel(getMyCandidateModel());
        candidateScrollPane.setViewportView(candidateTable);

        splitPane.setTopComponent(candidateScrollPane);

        failureTable.setModel(getFailureModel());
        failureScrollPane.setViewportView(failureTable);

        splitPane.setBottomComponent(failureScrollPane);

        add(splitPane, java.awt.BorderLayout.CENTER);

    }//GEN-END:initComponents
    
    private javax.swing.table.TableModel getMyCandidateModel() {
        candidateTableModel = new CandidateTableModel();
        candidateTable.getSelectionModel().addListSelectionListener(candidateTableModel);
        candidateTableModel.setTable(candidateTable);
        return candidateTableModel;
    }
    
    private javax.swing.table.TableModel getFailureModel() {
        failureTableModel = new FailureTableModel();
        failureTable.getSelectionModel().addListSelectionListener(failureTableModel);
        failureTableModel.setTable(failureTable);
        return failureTableModel;
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTable candidateTable;
    private javax.swing.JScrollPane failureScrollPane;
    private javax.swing.JTable failureTable;
    private javax.swing.JSplitPane splitPane;
    private javax.swing.JScrollPane candidateScrollPane;
    // End of variables declaration//GEN-END:variables
    
    public CandidateTableModel candidateTableModel;
    public FailureTableModel failureTableModel;
    
    /** Utility field used by bound properties. */
    private java.beans.PropertyChangeSupport propertyChangeSupport =  new java.beans.PropertyChangeSupport(this);
    
    /** Holds value of property selectedCandidate. */
    private livdll.Candidate selectedCandidate = null;
    
    public void removeAllCandidates() {
        clear();
        fcIndex = -1;
        findCanidates = new java.util.Vector();
        fireLivingstoneControlListenerEvent();
    }
    
    public void clear() {
        if(candidateTable.getModel() instanceof CandidateTableModel)
            ((CandidateTableModel)candidateTable.getModel()).clear();
        failureTableModel.clear();
    }
    
    public void addFindCandidates(livdll.FindCandidates fc ) {
        findCanidates.add(fc);
        if(autoUpdate) {
            try {
                javax.swing.SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        setFindCandidatesIndex(getFindCandidatesSize()-1);
                    }
                });
            } catch(IndexOutOfBoundsException e) {}
        } else {
            fireLivingstoneControlListenerEvent();
        }
    }
    
    private void setFindCandidates(livdll.FindCandidates fc) {
        clear();
        livdll.Candidate[] candidates = fc.getCandidates();
        for(int i = 0; i < candidates.length; i++) {
            addCandidate(candidates[i]);
        }
        candidateTable.getSelectionModel().setSelectionInterval(0,0);
    }
    
    public void addCandidate(livdll.Candidate candidate) {
        if(candidateTable.getModel() instanceof CandidateTableModel)
            ((CandidateTableModel)candidateTable.getModel()).addCandidate(candidate);
        if(failureTable.getModel() instanceof FailureTableModel)
            ((FailureTableModel)failureTable.getModel()).addFailures(candidate.getFailures());
    }
    
    /** Add a PropertyChangeListener to the listener list.
     * @param l The listener to add.
     */
    public void addPropertyChangeListener(java.beans.PropertyChangeListener l) {
        propertyChangeSupport.addPropertyChangeListener(l);
    }
    
    /** Removes a PropertyChangeListener from the listener list.
     * @param l The listener to remove.
     */
    public void removePropertyChangeListener(java.beans.PropertyChangeListener l) {
        propertyChangeSupport.removePropertyChangeListener(l);
    }
    
    /** Getter for property selectedCandidate.
     * @return Value of property selectedCandidate.
     */
    public livdll.Candidate getSelectedCandidate() {
        return selectedCandidate;
    }
    
    /** Setter for property selectedCandidate.
     * @param selectedCandidate New value of property selectedCandidate.
     */
    public void setSelectedCandidate(livdll.Candidate selectedCandidate) {
        livdll.Candidate oldSelectedCandidate = this.selectedCandidate;
        this.selectedCandidate = selectedCandidate;
        fireLivingstoneControlListenerEvent();
    }
    
    public void rew() {
        try {
            fcIndex--;
            setFindCandidatesIndex(fcIndex);
            autoUpdate = false;
        } catch(IndexOutOfBoundsException e) {
            fcIndex++;
        }
    }
    
    public void rrew() {
        try {
            setFindCandidatesIndex(0);
            fcIndex = 0;
            autoUpdate = false;
        } catch(IndexOutOfBoundsException e) {}
    }
    
    public void ffwd() {
        int lastIndex = getFindCandidatesSize()-1;
        try {
            setFindCandidatesIndex(lastIndex);
            fcIndex = lastIndex;
            autoUpdate = true;
        } catch(IndexOutOfBoundsException e) {}
    }
    
    public void fwd() {
        try {
            fcIndex++;
            setFindCandidatesIndex(fcIndex);
            autoUpdate = false;
        } catch(IndexOutOfBoundsException e) {
            fcIndex--;
        }
    }
    
    public int getFindCandidatesSize() {
        return findCanidates.size();
    }
    
    public int getFindCandidatesIndex() {
        return fcIndex;
    }
    
    public void addLivingstoneViewControlListener(LivingstoneViewControlListener listener) {
        if(listener != null) listeners.add(listener);
    }
    
    public void setFindCandidatesIndex(int index) throws IndexOutOfBoundsException {
        if(index < 0 || index >= findCanidates.size()) throw new IndexOutOfBoundsException();
        fcIndex = index;
        livdll.FindCandidates fc = (livdll.FindCandidates)findCanidates.get(index);
        setFindCandidates(fc);
        fireLivingstoneControlListenerEvent();
    }
    
    private void fireLivingstoneControlListenerEvent() {
        HistoryControlInterface.HistoryControlListenerEvent event = new HistoryControlInterface.HistoryControlListenerEvent();
        event.index = fcIndex;
        event.size = findCanidates.size();
        if(fcIndex > -1 && fcIndex < event.size) {
            livdll.FindCandidates fc = (livdll.FindCandidates)findCanidates.elementAt(fcIndex);
            event.time = fc.getTime();
            event.selectedCandidate = getSelectedCandidate();
            event.trailingCommands = fc.getTrailingCommands();
            event.showTrailingCommands = showTrailingCommands;
        }
        java.util.Iterator iter = listeners.iterator();
        while(iter.hasNext()) {
            ((HistoryControlInterface.LivingstoneViewControlListener)iter.next()).livingstoneViewControlListenerEvent(event);
        }
    }
    
    public void addTrailingCommand(livdll.LivingstoneCommand command, boolean fireEvent) {
        if(findCanidates.size() > 0) {
            livdll.FindCandidates fc = (livdll.FindCandidates)findCanidates.lastElement();
            fc.addTrailingCommand(command);
            if(fireEvent) fireLivingstoneControlListenerEvent();
        }
    }
    
    public void updateLastFindCandidate(livdll.FindCandidates fc, boolean fireEvent) {
        if(findCanidates.size() > 0) findCanidates.remove(findCanidates.lastElement());
        else fcIndex = 0;
        findCanidates.addElement(fc);
        if(fireEvent) fireLivingstoneControlListenerEvent();
    }
    
    public void clearTrailingCommands() {
        if(findCanidates.size() > 0) {
            livdll.FindCandidates fc = (livdll.FindCandidates)findCanidates.lastElement();
            fc.clearTrailingCommands();
        }
    }
    
    public void setShowTrailingCommands(boolean b) {
        this.showTrailingCommands = b;
        fireLivingstoneControlListenerEvent();
    }
    
    public boolean getShowTrailingCommands() { return this.showTrailingCommands; }
    
}
