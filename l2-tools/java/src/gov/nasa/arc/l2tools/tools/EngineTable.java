//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class EngineTable extends JScrollPane {

  private int rowClickedOn = -1;
  private MyPopupMenu popup;
  private JTable table ;

  public EngineTable(ToolFactory toolFactory, DefaultListModel engines) {
    super();
    table = new JTable(new EngineTableModel(engines));
    popup = new MyPopupMenu(toolFactory);
    table.setPreferredScrollableViewportSize(table.getPreferredSize());
    setViewportView(table);
    table.addMouseListener(new PopupListener());
  }

  
  class MyPopupMenu extends JPopupMenu {

    public MyPopupMenu(final ToolFactory toolFactory) {
      super();
      /*
      JMenuItem menuItem = new JMenuItem("New Engine");
      menuItem.addActionListener(new ActionListener() {
	  public void actionPerformed(ActionEvent e) {
	    try {
	      toolFactory.createDataModel(rowClickedOn);
	    } catch(ArrayIndexOutOfBoundsException err) {}
	  }
	});
      add(menuItem);
      */
      JMenuItem menuItem = new JMenuItem("new HistoryTable");
      menuItem.addActionListener(new ActionListener() {
	  public void actionPerformed(ActionEvent e) {
	    try {
	      toolFactory.newHistoryTableTool(rowClickedOn);
	    } catch(ArrayIndexOutOfBoundsException err) {}
	  }
	});
      add(menuItem);

    }
  }

  class PopupListener extends MouseAdapter {
    
    public void mousePressed(MouseEvent e) {
      maybeShowPopup(e);
    }
    
    public void mouseReleased(MouseEvent e) {
      maybeShowPopup(e);
    }

    private void maybeShowPopup(MouseEvent e) {
      if (e.isPopupTrigger()) {
	Point p = getViewport().toViewCoordinates(new Point(e.getX(), e.getY()));
	rowClickedOn = table.rowAtPoint(p);
	popup.show(e.getComponent(), e.getX(), e.getY());
      }
    }
  }
}
