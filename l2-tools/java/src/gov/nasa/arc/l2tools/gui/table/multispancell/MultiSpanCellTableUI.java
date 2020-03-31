/*
 * (swing1.1beta3)
 *
 */

package gov.nasa.arc.l2tools.gui.table.multispancell;

import java.util.*;
import java.awt.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.plaf.basic.*;

import livdll.CandidateAssignment;

/**
 * @version 1.0 11/26/98
 *
 *  specialized for the Candidate Manager by Will Taylor 29oct02
 */

public class MultiSpanCellTableUI extends BasicTableUI {
    
    CandidateCellRenderer candidateCellRenderer;
    
    public void paint(Graphics g, JComponent c) {
        Rectangle oldClipBounds = g.getClipBounds();
        Rectangle clipBounds    = new Rectangle(oldClipBounds);
        int tableWidth   = table.getColumnModel().getTotalColumnWidth();
        clipBounds.width = Math.min(clipBounds.width, tableWidth);
        g.setClip(clipBounds);
        
        candidateCellRenderer = new CandidateCellRenderer( table);
        
        int firstIndex = table.rowAtPoint(new Point(0, clipBounds.y));
        int  lastIndex = table.getRowCount()-1;
        
        Rectangle rowRect = new Rectangle(0,0,
        tableWidth, table.getRowHeight() + table.getRowMargin());
        rowRect.y = firstIndex*rowRect.height;
        
        for (int index = firstIndex; index <= lastIndex; index++) {
            if (rowRect.intersects(clipBounds)) {
                // System.out.println();                  // debug
                // System.err.println("MultiSpanCellTableUI row " + index + ": " +
                //                   table.getValueAt( index, 4).getClass().getName());
                paintRow(g, index);
            }
            rowRect.y += rowRect.height;
        }
        g.setClip(oldClipBounds);
    }
    
    private void paintRow(Graphics g, int row) {
        Rectangle rect = g.getClipBounds();
        boolean drawn  = false;
        
        AttributiveCellTableModel tableModel = (AttributiveCellTableModel)table.getModel();
        CellSpan cellAtt = (CellSpan)tableModel.getCellAttribute();
        int numColumns = table.getColumnCount();
        
        for (int column = 0; column < numColumns; column++) {
            
            TableColumn columnObject = table.getColumnModel().getColumn( column);
            columnObject.setCellRenderer( candidateCellRenderer);
            
            Rectangle cellRect = table.getCellRect(row,column,true);
            int cellRow,cellColumn;
            if (cellAtt.isVisible(row,column)) {
                cellRow    = row;
                cellColumn = column;
                //  System.out.print("   "+column+" ");  // debug
            } else {
                cellRow    = row + cellAtt.getSpan(row,column)[CellSpan.ROW];
                cellColumn = column + cellAtt.getSpan(row,column)[CellSpan.COLUMN];
                //  System.out.print("  ("+column+")");  // debug
            }
            if (cellRect.intersects(rect)) {
                drawn = true;
                paintCell(g, cellRect, cellRow, cellColumn);
            } else {
                if (drawn) break;
            }
        }
        
    }
    
    private void paintCell(Graphics g, Rectangle cellRect, int row, int column) {
        int spacingHeight = table.getRowMargin();
        int spacingWidth  = table.getColumnModel().getColumnMargin();
        
        Color c = g.getColor();
        g.setColor(table.getGridColor());
        // System.err.println( "paintCell drawRect " + g.getColor());
        g.drawRect(cellRect.x,cellRect.y,cellRect.width-1,cellRect.height-1);
        g.setColor(c);
        
        cellRect.setBounds(cellRect.x + spacingWidth/2, cellRect.y + spacingHeight/2,
        cellRect.width - spacingWidth, cellRect.height - spacingHeight);
        
        if (table.isEditing() && table.getEditingRow()==row &&
        table.getEditingColumn()==column) {
            Component component = table.getEditorComponent();
            component.setBounds(cellRect);
            component.validate();
        }
        else {
            DefaultTableCellRenderer renderer =
            (DefaultTableCellRenderer) table.getCellRenderer(row, column);
            Component component = table.prepareRenderer(renderer, row, column);
            
            if (component.getParent() == null) {
                rendererPane.add(component);
            }
            rendererPane.paintComponent(g, component, table, cellRect.x, cellRect.y,
            cellRect.width, cellRect.height, true);
        }
    }
    
    class CandidateCellRenderer extends DefaultTableCellRenderer {
        
        private JTable table;
        
        public CandidateCellRenderer( JTable table) {
            super();
            this.table = table;
            
            //       setOpaque( true);
            //       setHorizontalAlignment( CENTER);
            //       setVerticalAlignment( CENTER);
            setForeground( Color.black);
            setFont( new Font( "Courier New", Font.PLAIN, 12));
        } // end constructor
        
        
        protected void setValue( Object value) {
            if (value instanceof CandidateAssignment) {
                setText( ((CandidateAssignment) value).getText());
            } else {
                setText( (value == null) ? "" : value.toString());
            }
        }
        
        
        /**
         * getTableCellRendererComponent - implements DefaultTableCellRenderer
         *
         * @param table - JTable - attribute timelines table
         * @param value - Object - cell object to display
         * @param isSelected - boolean -
         * @param hasFocus - boolean -
         * @param row - int - table row number
         * @param column - int - table column number
         * @return this - TimelinesTableCellRenderer -
         */
        public Component getTableCellRendererComponent( JTable table, Object value,
        boolean isSelected, boolean hasFocus,
        int row, int column) {
            // System.err.println( "getTableCellRendererComponent row " + row + " col " +
            //                   column + " value " + value);
            if(table.getColumnCount() > 3) {
                Object obj = table.getValueAt( row, 4);
                if(obj instanceof CandidateAssignment) {
                    if (isSelected ||
                    ((CandidateAssignment) table.getValueAt( row, 4)).isSelected()) {
                        setBackground( table.getSelectionBackground());
                    } else {
                        // color all columns the same as col 4
                        setBackground( ((CandidateAssignment) table.getValueAt( row, 4)).getBackgroundColor());
                    }
                }
            }
            setValue( value);
            return this;
        } // end getTableCellRendererComponent
        
    } // end class CandidateCellRenderer
    
    
}




