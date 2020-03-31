/*
 * (swing1.1beta3)
 *
 */

package gov.nasa.arc.l2tools.gui.table.multispancell;

import java.util.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;


/**
 * @version 1.0 11/22/98
 */

public class AttributiveCellTableModel extends DefaultTableModel {
    
    protected CellAttribute cellAtt;
    
    public AttributiveCellTableModel() {
        this((Vector)null, 0);
    }
    public AttributiveCellTableModel(int numRows, int numColumns) {
        super(numRows, numColumns);
    }
    public AttributiveCellTableModel(Vector columnNames, int numRows) {
        super(columnNames,numRows);
    }
    public AttributiveCellTableModel(Object[] columnNames, int numRows) {
        super(columnNames, numRows);
    }
    public AttributiveCellTableModel(Vector data, Vector columnNames) {
        super(data, columnNames);
    }
    public AttributiveCellTableModel(Object[][] data, Object[] columnNames) {
        super(data, columnNames);
    }
    
    
    public void setDataVector(Vector newData, Vector columnNames) {
        super.setDataVector(newData, columnNames);
        cellAtt = new DefaultCellAttribute(dataVector.size(),
        columnIdentifiers.size());
        newRowsAdded(new TableModelEvent(this, 0, getRowCount()-1,
        TableModelEvent.ALL_COLUMNS, TableModelEvent.INSERT));
    }
    
    public void addColumn(Object columnName, Vector columnData) {
        if (columnName == null)
            throw new IllegalArgumentException("addColumn() - null parameter");
        columnIdentifiers.addElement(columnName);
        int index = 0;
        Enumeration enumeration = dataVector.elements();
        while (enumeration.hasMoreElements()) {
            Object value;
            if ((columnData != null) && (index < columnData.size()))
                value = columnData.elementAt(index);
            else
                value = null;
            ((Vector)enumeration.nextElement()).addElement(value);
            index++;
        }
        
        //
        cellAtt.addColumn();
        
        fireTableStructureChanged();
    }
    
    public void addRow(Vector rowData) {
        if (rowData == null) {
            rowData = new Vector(getColumnCount());
        }
        else {
            rowData.setSize(getColumnCount());
        }
        dataVector.addElement(rowData);
        
        //
        cellAtt.addRow();
        
        newRowsAdded(new TableModelEvent(this, getRowCount()-1, getRowCount()-1,
        TableModelEvent.ALL_COLUMNS, TableModelEvent.INSERT));
    }
    
    public void insertRow(int row, Vector rowData) {
        if (rowData == null) {
            rowData = new Vector(getColumnCount());
        }
        else {
            rowData.setSize(getColumnCount());
        }
        
        dataVector.insertElementAt(rowData, row);
        
        //
        cellAtt.insertRow(row);
        
        newRowsAdded(new TableModelEvent(this, row, row,
        TableModelEvent.ALL_COLUMNS, TableModelEvent.INSERT));
    }
    
    public CellAttribute getCellAttribute() {
        return cellAtt;
    }
    
    public void setCellAttribute(CellAttribute newCellAtt) {
        int numColumns = getColumnCount();
        int numRows    = getRowCount();
        if ((newCellAtt.getSize().width  != numColumns) ||
        (newCellAtt.getSize().height != numRows)) {
            newCellAtt.setSize(new Dimension(numRows, numColumns));
        }
        cellAtt = newCellAtt;
        fireTableDataChanged();
    }
    
  /*
  public void changeCellAttribute(int row, int column, Object command) {
    cellAtt.changeAttribute(row, column, command);
  }
   
  public void changeCellAttribute(int[] rows, int[] columns, Object command) {
    cellAtt.changeAttribute(rows, columns, command);
  }
   */
    
}

