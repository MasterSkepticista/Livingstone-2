//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*
* Author : Mark Shirley
* Description : AttributeTable.java
*
* CHANGELOG:
* 03/22/00      Mhs     Created.
*
* Notes:
* - Should consider moving AbstractTableModel outside of
    this so it can be reused within a panel widget.  Or making the widget
    and using it in this frame.
*/

package gov.nasa.arc.l2tools.tools.historyTable;

import gov.nasa.arc.l2tools.tools.*;
import livdll.AttributeAssignment;
import gov.nasa.arc.l2tools.util.EventLog;
import livdll.L2NativeException;
import java.awt.*;
import java.awt.event.*;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import java.util.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;

/** A frame that listens to engine events and displays a table of
    attribute names and values for the current time.  The rows can be
    restricted to attributes containing a substring. */
public class AttributeTable extends JFrame implements PropertyChangeListener, Tool {

  static final String[] columnNames = {"Attribute","Value"};
  static final String FRAME_TITLE = "Attribute Table";

  /* holds the JTable */
  JTable table = null;

  /* only those attributes whose names contain the substring shown in
     this text field will be displayed */
  JTextField filter = new JTextField();

  /* timer used to update the table based on filter changes only at a
     given maximum frequency */
  javax.swing.Timer timer = new javax.swing.Timer(300,new ActionListener() {
    public void actionPerformed(ActionEvent e) { timerFired(); }
  });

  /* Holds pointer to the livingstone data */
  DataModel engineModel = null;

  /* Holds a pointer to the data array */
  AttributeAssignment[][] assignmentArray = null;

  /* Holds the current timestep */
  int currentTimeStep = 0;

  public AttributeTable() {
    setTitle(FRAME_TITLE);
    getContentPane().setLayout(new BorderLayout());
    setSize(300,400);
    setVisible(false);

    // Wragg - added closing listener 10/24/00
    addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent e) {
          engineModel.removePropertyChangeListener(AttributeTable.this);
          engineModel = null;
          assignmentArray = null;
          timer.stop();
          timer = null;
        }
      });

    // Set up the table.  filteredModel is set up earlier in the initialization */
    table = new JTable(filteredModel);
    JScrollPane scrollpane = new JScrollPane(table);
    getContentPane().add(scrollpane,BorderLayout.CENTER);

    // Set up the filter and register a change event handler */
    filter.getDocument().addDocumentListener(new DocumentListener() {
      public void insertUpdate(DocumentEvent e) { filterChanged(); }
      public void removeUpdate(DocumentEvent e) { filterChanged(); }
      public void changedUpdate(DocumentEvent e) { filterChanged(); }
    });

    boolean addCopyButton = true;

    if (!addCopyButton) {
      getContentPane().add(filter,BorderLayout.SOUTH);
    } else {
      // add a button to copy the frame
      JPanel filterPanel = new JPanel();
      filterPanel.setLayout(new BorderLayout());
      filterPanel.add(filter,BorderLayout.CENTER);
      JButton btnNewTable = new JButton("Copy");
      btnNewTable.setFont(new Font("Dialog",Font.PLAIN,8));
      btnNewTable.addActionListener(new ActionListener() {
	public void actionPerformed(ActionEvent e) {
	  copyFrame();
	}
      });
      filterPanel.add(btnNewTable,BorderLayout.EAST);
      getContentPane().add(filterPanel,BorderLayout.SOUTH);
    }

    timer.setRepeats(false);
  }

  public AttributeTable(String sTitle) {
    this();
    setTitle(sTitle);
  }

  public AttributeTable(String sTitle, DataModel engineModel) {
    this();
    setTitle(sTitle);
    setModel(engineModel);
    setTitle(FRAME_TITLE+": "+engineModel.getModel());
  }

  // Tools interface

  public Container getContainer() { return this; }

  public void setModel(DataModel engineModel) {
    this.engineModel = engineModel;
    engineModel.addPropertyChangeListener(this);
  }

  public DataModel getModel() { return engineModel; }

  // "selectedCandidate"
  // "currentTimeStep"
  // "model"

  public void propertyChange(PropertyChangeEvent evt) {
    //    System.out.println("AttributeTable: PropertyChangeEvent name="+evt.getPropertyName());
    try {
      if (evt.getPropertyName().equals("selectedCandidate")) {
	assignmentArray = engineModel.getPipeline().getAttributeAssignments(this, engineModel.getSelectedCandidate());
	repaint(100);
      } else if (evt.getPropertyName().equals("lastProgress")) {
	currentTimeStep = engineModel.getCurrentTimeStep(); // must be before getAttributeAssignments()
	assignmentArray = engineModel.getPipeline().getAttributeAssignments(this, engineModel.getSelectedCandidate());
	repaint(100);
      } else if (evt.getPropertyName().equals("currentTimeStep")) {
	currentTimeStep = engineModel.getCurrentTimeStep();
	repaint(100);
      } else if (evt.getPropertyName().equals("currentTimeStep")) {
	assignmentArray = null;
	repaint(100);
      } else if (evt.getPropertyName().equals("model")) {
	setTitle(FRAME_TITLE+": "+engineModel.getModel());
      }
    } catch (L2NativeException err) {
      EventLog.report(err);
      assignmentArray = null;
      repaint(100);
    }
  }

  public void setVisible(boolean b) {
    if (b)
      setLocation(50, 50);
    super.setVisible(b);
  }
      
  /** Inner class implementing the data model to map assignments into
    the table API */
  AttributeTableModel filteredModel = new AttributeTableModel();

  class AttributeTableModel extends AbstractTableModel {
    /* used to limit the assignments shown */
    String filter = "";

    /* holds the indices of the subset of assignments that match the filter */
    Vector matches = null;

    public int getColumnCount() { return 2; }
    public int getRowCount() { 
      if (assignmentArray==null) return 0;
      if (matches==null) getMatches();
      return matches.size();
    }
    public Object getValueAt(int row, int col) { 
      if (matches==null) getMatches();
      if (matches.size()>row+1) return "";
      if (col==0) {
	return ((AttributeAssignment)matches.elementAt(row+1)).getName();
      } else if (col==1) {
	return ((AttributeAssignment)matches.elementAt(row+1)).getValue();
      } else {
	return "";
      }
    }
    public String getColumnName(int col) {
      return columnNames[col];
    }
    void setFilter(String filter) {
      if (filter==null) return;
      this.filter = filter.toLowerCase();
      getMatches();
      // I'm not sure this is right since it may lose column order,
      // but I don't know what other event to fire
      fireTableStructureChanged();
    }

    // compute the matching subset
    void getMatches() {
      //      System.out.println("getMatches()");
      //      System.out.println("assignmentArray="+assignmentArray);
      //      System.out.println("currentTimeStep="+currentTimeStep);
      matches = new Vector();
      if (assignmentArray==null) return;
      //      for (int i=0;i<assignmentArray.length;i++)
      //	System.out.println("ary["+i+"]="+assignmentArray[i]);
      AttributeAssignment[] ary = assignmentArray[currentTimeStep];
      for (int i=0;i<ary.length;i++) {
	String name = ary[i].getName();
        //	System.out.println("  name="+name);
	if (filter.equals("")) {
	  matches.addElement(ary[i]);
	} else if (ary[i].getName().toLowerCase().indexOf(filter)>-1)
	  matches.addElement(ary[i]);
      }
    }
  };

  /* filter handling */
  public void filterChanged() {
    timer.restart();
  }

  public void timerFired() {
    //System.out.println("timer text="+filter.getText());        
    filteredModel.setFilter(filter.getText());
    table.invalidate();
  }

  public void copyFrame() {
    AttributeTable table = new AttributeTable();
    table.setModel(getModel());
    table.filter.setText(filter.getText());
    table.setVisible(true);
  }

}
