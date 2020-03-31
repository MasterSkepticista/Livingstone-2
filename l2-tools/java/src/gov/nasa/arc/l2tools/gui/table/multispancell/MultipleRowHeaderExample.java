/* (swing1.1) (swing#1356,#1454) */
//package jp.gr.java_conf.tame.swing.examples;
package gov.nasa.arc.l2tools.gui.table.multispancell;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;


  
/*   ----------------------------------------------
 *  |         SNo.        |
 *   ----------------------------------------------
 *  |          |     1    |
 *  |   Name   |-----------------------------------
 *  |          |     2    |
 *   ----------------------------------------------
 *  |          |     1    |
 *  |          |-----------------------------------
 *  | Language |     2    |
 *  |          |-----------------------------------
 *  |          |     3    |
 *   ----------------------------------------------
 */

/**
 * @version 1.0 03/06/99
 */
public class MultipleRowHeaderExample extends JFrame {
  Object[][] data;
  Object[] column;
  JTable table;
  MultiSpanCellTable fixedTable;

  public MultipleRowHeaderExample() {
    super( "Multiple Row Header Example" );
    setSize( 400, 150 );
    
    data =  new Object[][]{
        {"SNo."    ,"" },
        {"Name"    ,"1"},
        {""        ,"2"},
        {"Language","1"},
        {""        ,"2"},
        {""        ,"3"}};
    column = new Object[]{"",""};
    
    AttributiveCellTableModel fixedModel = new AttributiveCellTableModel(data, column) {
      public boolean CellEditable(int row, int col) { 
        return false; 
      }
    };
        
    CellSpan cellAtt =(CellSpan)fixedModel.getCellAttribute();
    cellAtt.combine(new int[] {0}    ,new int[] {0,1});
    cellAtt.combine(new int[] {1,2}  ,new int[] {0});
    cellAtt.combine(new int[] {3,4,5},new int[] {0});
    
    //    DefaultTableModel    model = new DefaultTableModel(data.length, 3);
        
    fixedTable = new MultiSpanCellTable( fixedModel );
    //    table = new JTable( model );
    //    fixedTable.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
    fixedTable.setDefaultRenderer(Object.class, new RowHeaderRenderer(fixedTable));
    //    fixedTable.setGridColor(table.getTableHeader().getBackground());
    
    JScrollPane scroll = new JScrollPane( fixedTable );
    //    JViewport viewport = new JViewport();
    //    viewport.setView(fixedTable);
    //    viewport.setPreferredSize(fixedTable.getPreferredSize());
    //    scroll.setRowHeaderView(viewport);
    
    getContentPane().add(scroll, BorderLayout.CENTER);
  }

  public static void main(String[] args) {
    MultipleRowHeaderExample frame = new MultipleRowHeaderExample();
    frame.addWindowListener( new WindowAdapter() {
      public void windowClosing( WindowEvent e ) {
	System.exit(0);
      }
    });
    frame.setVisible(true);
  }

  
  class RowHeaderRenderer extends JLabel implements TableCellRenderer {  
    RowHeaderRenderer(JTable table) {
      JTableHeader header = table.getTableHeader();
      setOpaque(true);
      setBorder(UIManager.getBorder("TableHeader.cellBorder"));
      setHorizontalAlignment(CENTER);
      setForeground(header.getForeground());
      setBackground(header.getBackground());
      setFont(header.getFont());
    }
  
    public Component getTableCellRendererComponent(JTable table, Object value,
                          boolean isSelected, boolean hasFocus, int row, int column) {
      setText((value == null) ? "" : value.toString());
      return this;
    }
  }
  
}

