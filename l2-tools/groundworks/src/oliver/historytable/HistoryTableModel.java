/*
 * HistoryTableModel.java
 *
 * Created on June 16, 2003, 2:10 PM
 */

package oliver.historytable;

/**
 *
 * @author  stephen
 */
public class HistoryTableModel extends javax.swing.table.AbstractTableModel {
    
    private int minTime = 0;
    private int maxTime = 0;
    private boolean hasCurrentTime = false;
    private java.util.TreeMap data = new java.util.TreeMap();
    private java.util.Vector sort = new java.util.Vector();
    private livdll.AttributeAssignment[][] aa;
    private String filter = "";
    
    private StateThread stateThread = new StateThread();
    private FilterThread filterThread = new FilterThread();
    
    /** Creates a new instance of HistoryTableModel */
    public HistoryTableModel() {
        super();
    }
    
    public void setState(livdll.AttributeAssignment[][] state) {
        this.aa = state;
        filterThread.run = false;
        stateThread.run = false;
        while(filterThread.isAlive() || stateThread.isAlive()) {
            Thread.currentThread().yield();
        }
        stateThread = new StateThread();
        stateThread.start();
    }
    
    private abstract class MyThread extends Thread {
        public boolean run = true;
        public void start() {
            run = true;
            super.start();
        }
    }
    
    private class StateThread extends MyThread {
        public void run() {
            boolean hct = false;
            int min = Integer.MAX_VALUE;
            int max = Integer.MIN_VALUE;
            java.util.TreeMap d = new java.util.TreeMap();
            if(aa != null && aa.length > 0 && aa[aa.length-1].length > 0) {
                for(int i = 0; i < aa.length; i++) {
                    for(int j = 0; j < aa[i].length; j++) {
                        livdll.AttributeAssignment a = aa[i][j];
                        java.util.TreeMap row = (java.util.TreeMap)d.get(a.getName());
                        if(row==null) {
                            row = new java.util.TreeMap();
                            d.put(a.getName(), row);
                        }
                        row.put(new Integer(a.getTime()), a);
                        if(a.getTime()==-1) {
                            hct = true;
                        } else {
                            if(a.getTime()<min) min = a.getTime();
                            if(a.getTime()>max) max = a.getTime();
                        }
                        if(!run) break;
                    }
                    if(!run) break;
                }
                minTime = min;
                maxTime = max;
                hasCurrentTime = hct;
                data = d;
            } else {
                minTime = 0;
                maxTime = 0;
                hasCurrentTime = false;
                data = new java.util.TreeMap();
            }
            checkFilter();
        }
    }
    
    private class FilterThread extends MyThread {
        public void run() {
            java.util.Vector s = new java.util.Vector();
            java.util.Iterator enum = data.values().iterator();
            while(enum.hasNext()) {
                java.util.TreeMap row = (java.util.TreeMap)enum.next();
                Object key = row.firstKey();
                livdll.AttributeAssignment a = (livdll.AttributeAssignment)row.get(key);
                String name = a.getName().trim().toLowerCase();
                if(name.equals("") || name.indexOf(filter.toLowerCase())>-1) {
                    s.addElement(row);
                }
                if(!run) break;
            }
            sort = s;
            if(run) fireTableStructureChanged();
        }
    }
    
    public void setFilter(String filter) {
        this.filter = filter;
        checkFilter();
    }
    
    private void checkFilter() {
        filterThread.run = false;
        while(filterThread.isAlive()) {
            Thread.currentThread().yield();
        }
        filterThread = new FilterThread();
        filterThread.start();
    }
    
    /** Returns the number of columns in the model. A
     * <code>JTable</code> uses this method to determine how many columns it
     * should create and display by default.
     *
     * @return the number of columns in the model
     * @see #getRowCount
     *
     */
    public int getColumnCount() {
        int c = maxTime - minTime + 1;
        if(hasCurrentTime) c++;
        return c;
    }
    
    /** Returns the number of rows in the model. A
     * <code>JTable</code> uses this method to determine how many rows it
     * should display.  This method should be quick, as it
     * is called frequently during rendering.
     *
     * @return the number of rows in the model
     * @see #getColumnCount
     *
     */
    public int getRowCount() {
        return sort.size();
    }
    
    /** Returns the value for the cell at <code>columnIndex</code> and
     * <code>rowIndex</code>.
     *
     * @param	rowIndex	the row whose value is to be queried
     * @param	columnIndex 	the column whose value is to be queried
     * @return	the value Object at the specified cell
     *
     */
    public Object getValueAt(int rowIndex, int columnIndex) {
        if(columnIndex==0) {
            java.util.TreeMap row = (java.util.TreeMap)sort.get(rowIndex);
            Object key = row.firstKey();
            livdll.AttributeAssignment a = (livdll.AttributeAssignment)row.get(key);
            return a.getName();
        } else if(columnIndex==(maxTime-minTime+1)) {
            java.util.TreeMap row = (java.util.TreeMap)sort.get(rowIndex);
            Object key = new Integer(-1);
            livdll.AttributeAssignment a = (livdll.AttributeAssignment)row.get(key);
            if(a==null) return null;
            return a.getValue();
        } else {
            java.util.TreeMap row = (java.util.TreeMap)sort.get(rowIndex);
            Object key = new Integer(minTime+columnIndex);
            livdll.AttributeAssignment a = (livdll.AttributeAssignment)row.get(key);
            if(a==null) return null;
            return a.getValue();
        }
    }
    
    public String getColumnName(int columnIndex) {
        if(columnIndex==0) return "Name";
        else {
            try {
                return Integer.toString(aa[columnIndex-1][0].getTime());
            } catch(ArrayIndexOutOfBoundsException e) {
                return null;
            }
        }
    }
    
}
