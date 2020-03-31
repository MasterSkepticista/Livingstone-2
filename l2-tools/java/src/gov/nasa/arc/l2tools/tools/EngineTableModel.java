//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

import java.util.Vector;
import java.util.Enumeration;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import javax.swing.DefaultListModel;
import javax.swing.table.TableModel;
import javax.swing.event.TableModelEvent;
import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;
import javax.swing.event.TableModelListener;

public class EngineTableModel implements TableModel, ListDataListener {

  private Vector listeners = new Vector();
  private DefaultListModel engines;
  private DefaultListModel beforeEvent = new DefaultListModel();
  private String[] columnNames = { "Type", "State", "Current Time", "Model"};
  private String[] propertyNames = { "type", "state", "currentTimeStep", "model"};
  private MyPropertyListener mpl = new MyPropertyListener();

  public EngineTableModel(DefaultListModel engines) {
    this.engines = engines;
    addMyListener(0, engines.size());
    engines.addListDataListener(this);
  }

  private void copyEngines() {
    beforeEvent.clear();
    Enumeration enum = listeners.elements();
    while(enum.hasMoreElements()) {
      beforeEvent.addElement(enum.nextElement());
    }
  }

  private void addMyListener(int i0, int i1) {
    for(int i = i0; i < i1; i++) {
      DataModel engine = (DataModel)engines.elementAt(i);
      engine.addPropertyChangeListener(mpl);
    }
    copyEngines();
  }

  private void removeMyListener(int i0, int i1) {
    for(int i = i0; i < i1; i++) {
      DataModel engine = (DataModel)beforeEvent.elementAt(i);
      engine.removePropertyChangeListener(mpl);
    }
    copyEngines();
  }

  class MyPropertyListener implements PropertyChangeListener {
    public void propertyChange(PropertyChangeEvent e) {
      DataModel model = (DataModel)e.getSource();
      int row = engines.indexOf(model);
      String name = e.getPropertyName();
      int i = 0;
      for(; i < propertyNames.length; i++)
	if(name.equals(propertyNames[i])) break;
      fireTableModelEvent(new TableModelEvent(EngineTableModel.this, row, row, i));
    }
  }

  // ListDataListener interface

  // Sent when the contents of the list has changed in a way that's too complex to characterize with the previous methods.
  public void contentsChanged(ListDataEvent e) {
    removeMyListener(e.getIndex0(), e.getIndex1());
    fireTableModelEvent(new TableModelEvent(this));
  }

  // Sent after the indices in the index0,index1 interval have been inserted in the data model.
  public void intervalAdded(ListDataEvent e) {
    addMyListener(e.getIndex0(), e.getIndex1()+1);
    fireTableModelEvent(new TableModelEvent(this,e.getIndex0(),e.getIndex1(),TableModelEvent.ALL_COLUMNS, TableModelEvent.INSERT));
  }

  // Sent after the indices in the index0,index1 interval have been removed from the data model.
  public void intervalRemoved(ListDataEvent e) {
    removeMyListener(e.getIndex0(), e.getIndex1()+1);
    fireTableModelEvent(new TableModelEvent(this,e.getIndex0(),e.getIndex1(),TableModelEvent.ALL_COLUMNS, TableModelEvent.DELETE));
  }

  private void fireTableModelEvent(TableModelEvent e) {
    Enumeration enum = listeners.elements();
    while(enum.hasMoreElements()) {
      ((TableModelListener)enum.nextElement()).tableChanged(e);
    }
  }
 

  // TableModelInterface

  // Add a listener to the list that's notified each time a change to the data model occurs.
  public void addTableModelListener(TableModelListener l) {
    listeners.add(l);
  }

  // Returns the lowest common denominator Class in the column.
  public Class getColumnClass(int columnIndex) {
    return String.class;
  }

  // Returns the number of columns managed by the data source object.
  public int getColumnCount() {
    return columnNames.length;
  }

  // Returns the name of the column at columnIndex.
  public String getColumnName(int columnIndex) {
    return columnNames[columnIndex];
  }

  // Returns the number of records managed by the data source object.
  public  int getRowCount() {
    return engines.size();
  }

  // Returns an attribute value for the cell at columnIndex and rowIndex.
  public Object getValueAt(int rowIndex, int columnIndex) {
    DataModel engine = (DataModel)engines.getElementAt(rowIndex);
    switch(columnIndex) {
    case 0:
      return engine.getType();
    case 1:
      return engine.getState();
    case 2:
      return new Integer(engine.getCurrentTimeStep());
    case 3:
      return engine.getModel();
    default:
      return "error";
    }
  }

  // Returns true if the cell at rowIndex and columnIndex is editable.
  public boolean isCellEditable(int rowIndex, int columnIndex) { 
    return false; 
  }

  // Remove a listener from the list that's notified each time a change to the data model occurs.
  public void removeTableModelListener(TableModelListener l) {
    listeners.remove(l);
  }

  // Sets an attribute value for the record in the cell at columnIndex and rowIndex.
  public  void setValueAt(Object aValue, int rowIndex, int columnIndex) {}

}
