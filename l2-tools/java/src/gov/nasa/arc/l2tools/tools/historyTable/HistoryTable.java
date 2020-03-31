//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
/*  
* Author : Mark Shirley
* Description : HistoryTable.java
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
import gov.nasa.arc.l2tools.gutil.PopupMenu;
import gov.nasa.arc.l2tools.gutil.PopupMenuCallback;
import gov.nasa.arc.l2tools.gutil.PopupMenuItem;
import gov.nasa.arc.l2tools.gutil.PresentationBase;

import forwill.Server;
import gov.nasa.arc.l2tools.util.EventLog;
import livdll.*;
import java.awt.*;
import java.awt.event.*;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.*;
import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.event.*;
import javax.swing.table.*;

/** A frame that listens to engine events and displays a table of
    attribute names and values from the history start time to the end
    time.  The rows can be restricted to attributes containing a
    substring. */
public class HistoryTable extends JFrame implements PropertyChangeListener, Tool {

  static final String FRAME_TITLE = "L2 History Table";

  /* holds the JTable */
  public JTable table = null;

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

  /* Holds the start timestep for the current history */
  int startTimeStep = 0;

  /* Holds the end timestep for the current history */
  int endTimeStep = 0;

  /* Holds the current timestep */
  int currentTimeStep = 0;

  /* Which column to sort on */
  int sortColumn = 0;

  /* True if we sort low-to-high, false if high-to-low */
  boolean sortNormal = true;

  // vector of Justification frames created - wmt
  Vector justificationFrames = new Vector();

  public Vector getJustificationFrames() {
    return justificationFrames;
  }

  /* List of attributeNames, sorted with Steve's numerical sorting hack
     so that xx10xx appears after xx9xx */
  java.util.List stevesSpecialSortedNames = null;

  // Wragg - added some crude thread handling
  private Thread worker;
  private Runnable doAttributeAssignmentsWork = null;
  private Runnable currentTimeStepWork = null;
  private Runnable modelWork = null;
  private Runnable attributeNamesWork = null;
  private boolean runnable = true;

  private Vector work = new Vector();

  private class MyWorkerThread extends Thread {
    public void run() {
      boolean idle = false;
      while (runnable) {
        if (idle) {
          if (engineModel != null) {
            if(doAttributeAssignmentsWork != null) {
              doAttributeAssignmentsWork.run();
              doAttributeAssignmentsWork = null;
            } else if(currentTimeStepWork != null) {
              currentTimeStepWork.run();
              currentTimeStepWork = null;
            } else if(modelWork != null) {
              modelWork.run();
              modelWork = null;
            } else if(attributeNamesWork != null) {
              attributeNamesWork.run();
              attributeNamesWork = null;
            }
          }
        }
        if(doAttributeAssignmentsWork == null &&
           currentTimeStepWork == null &&
           modelWork == null &&
           attributeNamesWork == null)
          try {
            idle = true;
            sleep(1000);
          } catch(InterruptedException e) {}
      }
    }
  }

  /* Debugging */
  public HistoryTableModel getFilteredModel() { return filteredModel; }

  public AttributeAssignment[][] getAssignmentArray() { return assignmentArray; }

  public HistoryTable(DataModel bean) {
    this();
    setModel(bean);
    setVisible(true);
  }

  public HistoryTable() {
    super();
    setTitle(FRAME_TITLE);
    Container contentPane = getContentPane();
    contentPane.setLayout( new BoxLayout( contentPane, BoxLayout.Y_AXIS));
    setSize(300,400);
    setVisible(false);

    // Wragg - added some crude thread handling
    worker = new MyWorkerThread();
    worker.start();

    // Wragg - added closing listener 10/24/00
    addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent e) {
          closeHistoryTable();
        }
      });

    // Set up the table.  filteredModel is set up earlier in the initialization */
    table = new JTable(filteredModel);
    table.getTableHeader().setReorderingAllowed(false);
    JScrollPane scrollpane = new JScrollPane(table);
    contentPane.add( scrollpane, BorderLayout.NORTH);

    table.setDefaultRenderer(AttributeAssignment.class,new HistoryTableRenderer());
    table.addMouseListener( new HistoryTableMouseListener());

    TableColumnModel tcm = table.getColumnModel();
    // System.err.println("HistoryTable.HistroyTable: col0 width 150");
    tcm.getColumn(0).setPreferredWidth(150);
    table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
    // table.setAutoResizeMode(JTable.AUTO_RESIZE_LAST_COLUMN);
    table.setPreferredScrollableViewportSize(table.getPreferredSize());

    constructFilterAndAttentionPanels( contentPane);

    timer.setRepeats(false);
  } // end constructor


  public void closeHistoryTable() {
    engineModel.removePropertyChangeListener( this);
    engineModel = null;
    assignmentArray = null;
    timer.stop();
    timer = null;
    // stop thread processing
    runnable = false;
    // close justification windows
    for (int i = 0, n = justificationFrames.size(); i < n; i++) {
      ((JFrame) justificationFrames.get( i)).dispose();
    }
  } // end closeHistoryTable


  private final void constructFilterAndAttentionPanels( Container contentPane) {
    /* This should work in both 1.2 and 1.3.  But the JTextPane isn't
     working in jdk1.2 under linux.... */
    filter.addKeyListener(new KeyAdapter() {
        public void keyPressed(KeyEvent e) { filterChanged(); }
        public void keyReleased(KeyEvent e) { filterChanged(); }
      });
    boolean addCopyButton = true;

    if (!addCopyButton) {
      contentPane.add( filter,BorderLayout.NORTH);
    } else {
      JPanel filterPanel = new JPanel();
      filterPanel.setLayout( new BoxLayout( filterPanel, BoxLayout.X_AXIS));
      filterPanel.setBorder
        ( BorderFactory.createCompoundBorder
          ( BorderFactory.createEmptyBorder( 0, 0, 3, 0),
            ( BorderFactory.createMatteBorder( 0, 0, 1, 0, Color.black))));
      // add a label for the filter
      JLabel filterLabel = new JLabel(" Disjunctive Filter ");
      filterLabel.setForeground( Color.black);
      filterLabel.setFont(new Font("Dialog",Font.PLAIN,12));
      filterPanel.add( filterLabel, BorderLayout.WEST);
      // the filter
      filter.setMaximumSize( new Dimension( 2000, 20));
      filterPanel.add( filter, BorderLayout.WEST);
      // add a button to copy the frame
      JButton btnNewTable = new JButton("Copy");
      btnNewTable.setToolTipText( "Copy to new History Table");
      btnNewTable.setFont(new Font("Dialog",Font.PLAIN,12));
      btnNewTable.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e) {
            copyFrame();
          }
        });
      filterPanel.add( btnNewTable,BorderLayout.EAST);
      contentPane.add( filterPanel, BorderLayout.NORTH);

      JPanel attentionPanel = new JPanel();
      attentionPanel.setLayout( new BoxLayout( attentionPanel, BoxLayout.Y_AXIS));
       JLabel mouseRLabel =
        new JLabel( "Mouse-R on values => Print Connected Clauses; Show Justification;" +
                    " Set Value BG Color");
      mouseRLabel.setForeground( Color.black);
      mouseRLabel.setFont( new Font( "Dialog", Font.PLAIN, 12));
      mouseRLabel.setHorizontalAlignment( SwingConstants.LEFT);
      attentionPanel.add( mouseRLabel, BorderLayout.WEST);
      JLabel connClausesLabel =
        new JLabel( "Connected Clauses are output to Skunkworks/Livingstone window");
      connClausesLabel.setForeground( Color.black);
      connClausesLabel.setFont( new Font( "Dialog", Font.PLAIN, 12));
      connClausesLabel.setHorizontalAlignment( SwingConstants.LEFT);
      attentionPanel.add( connClausesLabel, BorderLayout.WEST);

      contentPane.add( attentionPanel, BorderLayout.WEST);
    }
  } // end constructFilterAndAttentionPanels


  public HistoryTable(String sTitle) {
    this();
    setTitle(sTitle);
  }

  public HistoryTable(String sTitle, DataModel engineModel) {
    this();
    setTitle(sTitle);
    setModel(engineModel);
  }


  // Tools interface

  public Container getContainer() { return this; }

  public void setModel(DataModel engineModel) {
    this.engineModel = engineModel;
    engineModel.addPropertyChangeListener(this);
    reinitialize();
    repaint(100);
  }

  public DataModel getModel() { return engineModel; }

  void reinitialize() {
    setTitle(FRAME_TITLE+": "+engineModel.getModel());
    assignmentArray = null;
    startTimeStep = engineModel.getStartTimeStep();
    currentTimeStep = engineModel.getCurrentTimeStep(); // must be before getAttributeAssignments()
    endTimeStep = engineModel.getEndTimeStep();
    try {
      doAttributeAssignments(engineModel.getSelectedCandidate());
    } catch (Exception e) { EventLog.report(e); }
  }

  public void propertyChange(PropertyChangeEvent evt) {
    // System.out.println("HistoryTable: PropertyChangeEvent name="+ evt.getPropertyName());
    try {
      if (engineModel.getEnableCandMgrOnlyUpdate())
        return;

      if (evt.getPropertyName().equals("selectedCandidate") &&
          engineModel.getEnableGUIUpdate()) {
        doAttributeAssignmentsWork = new Runnable() {
	  public void run() {
	    try {
	      doAttributeAssignments(engineModel.getSelectedCandidate());
              repaint(100); // new
	    } catch (Exception err) { EventLog.report(err); }
	  }
	};

      } else if (evt.getPropertyName().equals("lastProgressPostTime")) {
        // echoCommand calls setLastProgressPostTime after updating the end time
        // via setEndTimeStep - wmt

        // setting endTimeStep & startTimeStep must be outside
        // new Runnable, in order for method getColumnClass
        // of inner class HistoryTableModel to properly get
        // currrent values - wmt
        startTimeStep = engineModel.getStartTimeStep();
        currentTimeStep = engineModel.getCurrentTimeStep();
        endTimeStep = engineModel.getEndTimeStep();
        // System.err.println("HistoryTable.PropertyChangeEvent: lastProgressPostTime start " + engineModel.getStartTimeStep() + " stop " + engineModel.getEndTimeStep() + " current " + engineModel.getCurrentTimeStep());
        // System.out.println("Just set endTimeStep="+endTimeStep);
        if (engineModel.getEnableGUIUpdate()) {
          doAttributeAssignmentsWork = new Runnable() {
              public void run() {
                try {
                  assignmentArray = null;
                  doAttributeAssignments(engineModel.getSelectedCandidate());
                  repaint(100);
                } catch (Exception err) { EventLog.report(err); }
              }
            };
        }

      } else if (evt.getPropertyName().equals("currentTimeStep") &&
                 engineModel.getEnableGUIUpdate()) {
        // System.err.println("HistoryTable.PropertyChangeEvent: currentTimeStep start " + engineModel.getStartTimeStep() + " stop " + engineModel.getEndTimeStep() + " current " + engineModel.getCurrentTimeStep());
        currentTimeStepWork = new Runnable() {
	  public void run() {
	    currentTimeStep = engineModel.getCurrentTimeStep();
	    repaint(100);
	  }
	};

      } else if (evt.getPropertyName().equals("model")) {
         modelWork = new Runnable() {
	  public void run() {
	    setTitle(FRAME_TITLE+": "+engineModel.getModel());
	    // Initialize
	    startTimeStep = engineModel.getStartTimeStep();
	    currentTimeStep = engineModel.getCurrentTimeStep(); // must be before getAttributeAssignments()
	    endTimeStep = engineModel.getEndTimeStep();
	    assignmentArray = null;
	    // We'll pick this up with the attributeNames change event
            try {
	      stevesSpecialSortedNames = null; // resets number of rows when changing models
              doAttributeAssignments(null);
            } catch (Exception err) { EventLog.report(err);
            }
            // clear the history table - wmt
            repaint(0);
	  }
	};

      } else if (evt.getPropertyName().equals("attributeNames") &&
                 engineModel.getEnableGUIUpdate()) {
        // this is redundant to "model" - wmt 14nov02
//         attributeNamesWork = new Runnable() {
// 	  public void run() {
// 	    try {
// 	      stevesSpecialSortedNames = null;
// 	      doAttributeAssignments(engineModel.getSelectedCandidate());
// 	    } catch (Exception err) { EventLog.report(err); }
// 	  }
// 	};
      }
    } catch (Exception err) {
      EventLog.report(err);
      assignmentArray = null;
      repaint(100);
    }
  }

  void doAttributeAssignments(Candidate c) throws L2NativeException {
    //    System.out.println("doAttributeAssignments c="+c);
    if ((engineModel == null) ||
        ("none".equals(engineModel.getModel()))) {
      return;
    }
    if (stevesSpecialSortedNames==null) doUpdateSortedNames();

    boolean arrayWasNull = assignmentArray==null;

    assignmentArray = engineModel.getPipeline().getAttributeAssignments2
      (this, c, stevesSpecialSortedNames);
    //reportAssignmentArray();

    if (assignmentArray == null) {
      return;
    }


    // If the array is new, then apply the filter.  Else, don't
    if (arrayWasNull) filteredModel.setFilter(filter.getText());

    // Just changing the data with filteredModel.fireTableDataChanged()
    // won't work because it doesn't change the column names.

    Vector v = saveColumnWidths();
    filteredModel.fireTableStructureChanged();
    restoreColumnWidths(v);
  }

  Vector saveColumnWidths() {
    int count = table.getColumnCount();
    // System.err.println("HistoryTable.saveColumnWidths: count " + count);
    Vector widths= new Vector(count);
    TableColumnModel cm = table.getColumnModel();
    int rowCount = table.getRowCount();
    // use font metrics to determine column widths - wmt
    // for initial 0-1 time steps, this does not determine the 1 column,
    // so that is done in restoreColumnWidths
    String valueString;
    Font font = table.getFont();
    FontMetrics fontMetrics = table.getFontMetrics( font);
    for (int i=0; i<count; i++) {
        // make sure <undefined> can be displayed
        int maxWidth = fontMetrics.stringWidth( "<undefined> ");
        int valueFontWidth = 0;
        for (int j = 0; j < rowCount; j++) {
          if (i == 0) {
            // returns string
            try {
              //   System.err.println("HT.saveColumnWidths: i " + i + " j " + j + " value " +
              //                                table.getValueAt(j, i) + " len " +
              //                                ((String) table.getValueAt(j, i)).length());
              valueString = ((String) table.getValueAt(j, i));
            } catch (Exception e) {
              // prevent stack trace for
              // ArrayIndexOutOfBoundsException error
              valueString = new String( "");
            }
          } else {
            try {
              //  System.err.println("HT.saveColumnWidths: i " + i + " j " + j + " value " +
              //  ((AttributeAssignment) table.getValueAt(j, i)).getValue() +
              //  " len " +
              //  ((String) ((AttributeAssignment) table.getValueAt(j, i)).getValue()).length());
              valueString = ((String) ((AttributeAssignment)
                                       table.getValueAt(j, i)).getValue());
            } catch (Exception e) {
              // prevent stack trace for <column bounds> error
              valueString = new String( "");
            }
          }
          valueFontWidth = fontMetrics.stringWidth( valueString.concat( " "));
          if (valueFontWidth > maxWidth)
            maxWidth = valueFontWidth;
        }
	widths.addElement(new Integer(maxWidth));
    }
    // System.err.println(" widths " + widths);
    return widths;
  }

  void restoreColumnWidths(Vector widths) {
    //System.out.println("restoreColumnWidths");
    int count = table.getColumnCount();

    // use font metrics to determine rightmost column width - wmt
    String valueString;
    Font font = table.getFont();
    FontMetrics fontMetrics = table.getFontMetrics( font);
    int col = count - 1;
    int rowCount = table.getRowCount();
    // make sure <undefined> can be displayed
    int maxWidth = fontMetrics.stringWidth( "<undefined> ");
    int valueFontWidth = 0;
    for (int j = 0; j < rowCount; j++) {
      try {
        //  System.err.println("HT.restoreColumnWidths: col " + col + " j " + j + " value " +
        //  ((AttributeAssignment) table.getValueAt(j, col)).getValue() +
        //  " len " +
        //  ((String) ((AttributeAssignment) table.getValueAt(j, col)).getValue()).length());
        valueString = ((String) ((AttributeAssignment)
                                 table.getValueAt(j, col)).getValue());
      } catch (Exception e) {
        // prevent stack trace for <column bounds> error
        valueString = new String( "");
      }
      valueFontWidth = fontMetrics.stringWidth( valueString.concat( " "));
      if (valueFontWidth > maxWidth)
        maxWidth = valueFontWidth;
    }
    widths.addElement(new Integer(maxWidth));
    int len = widths.size();
    // System.err.println("HT.restoreColumnWidths: count " + count + " len " + len);
    // if len < count, make len equal to count
    for (int k = 0; k < count - len; k++) {
      widths.addElement(new Integer(maxWidth));
    }
    len = count;

    // count-1 or count depends on AutoResizeMode
    for (int i=0;i<count;i++) {
      if (i>len-1) break;
      int width = ((Integer)(widths.elementAt(i))).intValue();
      // System.err.println("restoreColumnWidths: i " + i + " width " + width);
      String columnName = table.getColumnName(i);
      table.getColumn(columnName).setPreferredWidth(width);
    }
  }

  void doUpdateSortedNames() {
    stevesSpecialSortedNames = new java.util.ArrayList();
    Collection c = engineModel.getAttributeNames();
    if (c == null) {
      return;
    }
    // System.out.println("doUpdateSortedNames: c="+c);
    stevesSpecialSortedNames.addAll(c);
    Collections.sort(stevesSpecialSortedNames, new MyComparator());
  }

  public void setVisible(boolean b) {
    // set default location, only if not set by CommandLine.dispatchCommand.loadModel
    if (b && getLocation().equals( new Point()))
      setLocation(50, 50);
    super.setVisible(b);
  }

  /* Data structure for holding filtered and sorted table rows. */
  class SortableRow {
    SortableRow(int index, String sortKey) {
      this.index = index;
      this.sortKey = sortKey;
    }
    /* Index of the row this represents in assignmentArray */
    int index = -1;
    /* String that sorting is going to occur on */
    String sortKey = "";
  }

  /** Inner class implementing the data model to map assignments into
    the table API.*/
  HistoryTableModel filteredModel = new HistoryTableModel();

  class HistoryTableModel extends AbstractTableModel {
    /* used to limit the assignments shown */
    DisjunctiveFilter filter = new DisjunctiveFilter();

    /* Holds SortableRows for the subset of assignments that match the filter */
    ArrayList matches = null;

    public Class getColumnClass(int col) {
      return col==0 ? String.class : AttributeAssignment.class;
    }
    public int getColumnCount() {
      // System.out.println("getColumnCount="+(endTimeStep - startTimeStep + 2));
      // System.out.println("endTimeStep=" + endTimeStep);
      // System.out.println("startTimeStep="+startTimeStep);
      return endTimeStep - startTimeStep + 2; } // + 1 for indexing + 1 for names

    public int getRowCount() {
      if (assignmentArray==null) return 0;
      if (matches==null) getMatchesAndSort(sortColumn);
      return matches.size();
    }
    public Object getValueAt(int row, int col) {
      //System.out.println("getValueAt "+row+","+col);
      if (assignmentArray==null) return "<null array>";
      if (matches==null) getMatchesAndSort(sortColumn);
      if (row>matches.size()-1 || row<0) return "<row bounds>";

      // Handle the reverse sort here.  Cute.
      int index = -1;
      if (sortNormal) {
	index = ((SortableRow)matches.get(row)).index;
      } else {
	index = ((SortableRow)matches.get(matches.size()-row)).index;
      }

      // Handle the column

      /* saved.  This returns strings
      if (col==0) {
	return ((AttributeAssignment)assignmentArray[0][index]).getName();
      } else {
	int c = col-1;
	if (c<0 || c>assignmentArray.length-1) return "<column bounds>";
	AttributeAssignment[] a = assignmentArray[c];
	if (a==null) return "<null collumn>";
	return ((AttributeAssignment)a[index]).getValue();
      }
      */

      try {
        if (col==0) {
          // returns a string
          String attributeName =
            ((AttributeAssignment)assignmentArray[0][index]).getName();
          // System.err.println("getValueAt: attributeName " + attributeName);
          // trim off "test."
          String[] pair = LivingstoneEngineAdapter.breakAtSubstring( attributeName, "test.");
          if ( "".equalsIgnoreCase(pair[1]))
            return pair[0];
          else
            return pair[1];
        } else {
          int c = col-1;
          if (c<0 || c>assignmentArray.length-1) return "<column bounds>";
          AttributeAssignment[] a = assignmentArray[c];
          // returns an AttributeAssignment
          return a[index];
        }
      } catch (ArrayIndexOutOfBoundsException e) {
        return "<array bounds>";
      }
    }

    public String getColumnName(int col) {
      if (col==0) {
	return "Attribute";
      } else {
        if(engineModel != null) col = col+engineModel.getStartTimeStep();
	return Integer.toString(col-1);
      }
    }

    void setFilter(String filter) {
      // System.out.println("in setFilter filter="+filter);
      if (filter==null) return;
      this.filter.setPattern(filter.toLowerCase());
      getMatchesAndSort(sortColumn);
      // I'm not sure this is right since it may lose column order,
      // but I don't know what other event to fire
      fireTableChanged(new TableModelEvent(filteredModel));
      //fireTableStructureChanged();
    }

    // compute the matching subset
    void getMatchesAndSort(int column) {
      //System.out.println("getMatchesAndSort()");
      //System.out.println("assignmentArray="+assignmentArray);
      //System.out.println("currentTimeStep="+currentTimeStep);
      if (matches==null) matches = new ArrayList();
      matches.clear();
      if (assignmentArray==null) {
	System.out.println("No matches because the whole history array was null.");
	return;
      }

      //for (int i=0;i<assignmentArray.length;i++)
      //System.out.println("ary["+i+"].length="+(assignmentArray[i]==null ?
      //"null"
      //: Integer.toString(assignmentArray[i].length)));

      int idx = findNonNullArray();
      if (idx<0) {
	System.out.println("No arrays hold data.");
	return;
      }
      AttributeAssignment[] ary = assignmentArray[idx];
      //System.out.println("Filtering...");
      //System.out.println("idx="+idx);
      //System.out.println("ary.length="+ary.length);
      for (int i=0;i<ary.length;i++) {
	AttributeAssignment aa = ary[i];
	String name = aa.getName();
	//System.out.println("  name="+name);
	// Old : if (filter.equals("") || ary[i].getName().toLowerCase().indexOf(filter)>-1)
	if (filter.match(ary[i].getName().toLowerCase())) {
	  matches.add(new SortableRow(i,(column==0) ? name : aa.getValue()));
	}
      }
      // Sort.  I'm reversing when I read data from matches, so I don't
      // have to do it here.
      //System.out.println("column="+column);
      if (column==0) {
	//Collections.sort(matches); // for now
      } else {
	Collections.sort(matches);
      }
    }
  };

  int findNonNullArray() {
    for (int i=0;i<assignmentArray.length;i++)
      if (assignmentArray[i]!=null) return i;
    return -1;
  }

  /* filter handling */
  public void filterChanged() {
    timer.restart();
  }

  public void timerFired() {
    //System.out.println("timer text="+filter.getText());
    filteredModel.setFilter(filter.getText());
    filteredModel.fireTableChanged(new TableModelEvent(filteredModel));
    //table.invalidate();
  }

  public void copyFrame() {
    HistoryTable table = new HistoryTable();
    table.assignmentArray = assignmentArray;
    table.startTimeStep = startTimeStep;
    table.endTimeStep = endTimeStep;
    //    System.out.println("Before copy.  Just set endTimeStep to "+endTimeStep);
    table.currentTimeStep = currentTimeStep;
    table.sortColumn = sortColumn;
    table.sortNormal = sortNormal;
    table.setModel(getModel());
    String filterString = filter.getText();
    table.filter.setText(filterString);
    table.filteredModel.setFilter(filterString);
    // CommandLine keeps list of all History Tables
    Server.server.getCommandLine().addHistoryTable( table);
    table.setVisible(true);
  }

  void reportAssignmentArray() {
    System.out.println("reportAssignmentArray...");
    System.out.println("  assignmentArray="+assignmentArray);
    System.out.println("  currentTimeStep="+currentTimeStep);
    if (assignmentArray==null) {
      System.out.println("No matches because the whole history array was null.");
      return;
    }
    for (int i=0;i<assignmentArray.length;i++) {
      System.out.println("  assignmentArray["+i+"].length="+(assignmentArray[i]==null ?
							     "null"
							     : Integer.toString(assignmentArray[i].length)));
      if (assignmentArray[i]==null) {
	System.out.println("    It's null.");
      } else {
	AttributeAssignment[] ary = assignmentArray[i];
	System.out.println("    It's length is "+ary.length);
	/*
	  for (int j=0;j<ary.length;j++) {
	  AttributeAssignment a = ary[j];
	  if (a instanceof AttributeAssignment) {
	  System.out.println("    ary["+j+"]="+ary[j].getPropString());
	  } else {
	  System.out.println("    ary["+j+"]="+ary[j]);
	  }
	  }
	  */
      }
    }
  }

  // Color coding the values

  class HistoryTableRenderer extends DefaultTableCellRenderer {
    public void setValue(Object value) {
      if (value instanceof AttributeAssignment) {
	String v = ((AttributeAssignment)value).getValue();
	setText(v);
	setBackground(ColorDB.getColor(v));
      } else if (value instanceof String) {
	// Shouldn't occur
	setText((String)value);
      } else {
	setText("<Rendering Error>");
      }
    }
  }

  //
  class HistoryTableMouseListener extends MouseAdapter {
    public void mousePressed(MouseEvent e) {
      if (e.isPopupTrigger()) handlePopup(e);
    }
    public void mouseReleased(MouseEvent e) {
      if (e.isPopupTrigger()) handlePopup(e);
    }
    void handlePopup(MouseEvent e) {
      Component c = e.getComponent();
      if (!(c instanceof JTable)) return;
      JTable table = (JTable)c;
      int row = table.rowAtPoint(e.getPoint());
      int col = table.columnAtPoint(e.getPoint());
      Object value = filteredModel.getValueAt(row,col);

      if (value instanceof AttributeAssignment) {
	AttributeAssignment a = (AttributeAssignment)value;
	PopupMenu.popup(new AttributeAssignmentInHistoryPresentation(a),
			null,
			c,
			e.getX(),
			e.getY());
	return;
      }

      if (value instanceof String) {
	return;
      }

      EventLog.report("Unrecognized object in History Table.  Ignoring popup menu request.");
    }

  }

  class AttributeAssignmentInHistoryPresentation extends PresentationBase {
    AttributeAssignmentInHistoryPresentation(AttributeAssignment assignment) {
      super(assignment);
    }

    public AttributeAssignment getAttributeAssignment() {
      return (AttributeAssignment)object;
    }

    public void addPopupMenuItems(Component c, Vector items) {
      // Print Connected Clauses
      items.addElement(new PopupMenuItem("Print connected clauses",
					 new PopupMenuCallback() {
	public void callback(Component context) {
	  ScenarioInterface pipeline = engineModel.getPipeline();
	  AttributeAssignment a = getAttributeAssignment();
	  String pstring = null;
	  if (a.getTime()==engineModel.getEndTimeStep()) {
	    // Doesn't do it right for the last timestep
	    pstring = a.getName() + "=" + a.getValue();
	  } else {
	    pstring = a.getPropString();
	  }
	  ((LivingstoneEngineAdapter)engineModel.getPipeline()).getEngine().printPropClauses(pstring);
	}
      }));

      // Show Justification
      items.addElement(new PopupMenuItem("Show Justification",
					 new PopupMenuCallback() {
	public void callback(Component context) {
       try{
        JFrame frame = new AttributeAssignmentJustificationFrame(
                                             getAttributeAssignment().getPropString(engineModel.getPipeline()),
                                             new DataModelState(engineModel));

        justificationFrames.addElement( frame );
       }
       catch(L2NativeException e) {
        JOptionPane.showMessageDialog(null,e.getMessage(),new String("Livingstone Error"),JOptionPane.ERROR_MESSAGE);
       }
       catch(DataModelStateError e) {
        JOptionPane.showMessageDialog(null,e.getMessage(),new String("L2 Engine Inconsistency"),JOptionPane.ERROR_MESSAGE);
       }
       catch(Exception e) {
        JOptionPane.showMessageDialog(null,e.getMessage(),new String("Justification Error"),JOptionPane.PLAIN_MESSAGE);
       }
	}
      }));

      // Set value background color
      items.addElement(new PopupMenuItem("Set value background color",
					 new PopupMenuCallback() {
	public void callback(Component context) {
	  if (ColorDB.chooseColorFor(HistoryTable.this.getContentPane(),
				     getAttributeAssignment().getValue()))
	    repaint(100);
	}
      }));

    }
  }

}
