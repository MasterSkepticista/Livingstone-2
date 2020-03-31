//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.table.*;
import javax.swing.tree.*;
import java.awt.event.*;
import java.util.*;
import java.awt.*;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import gov.nasa.arc.l2tools.gui.table.multispancell.*;
import gov.nasa.arc.l2tools.gui.table.multispanheader.*;
import livdll.*;
import gov.nasa.arc.l2tools.gutil.TextFrame;

public class CandidateManager extends JFrame implements PropertyChangeListener, Tool {

  private MyTableModel candidatesTableModel;
  private MultiSpanCellTable candidatesTable;
  private MyTableModel candidateClassesTableModel;
  private MultiSpanCellTable candidateClassesTable;
  private MyToolBar toolbar;
  private DefaultComboBoxModel history = new DefaultComboBoxModel();
  private boolean disableHistoryActionListener = false;
  private DefaultTreeModel assignmentTreeModel =
    new DefaultTreeModel(new StateTreeNode("root",true));
  private JTree tree;
  private JTabbedPane tp;
  private JLabel infoLine;
  private int lastSelectedCandidateIndex = -1;
  private int lastSelectedCandidateClassIndex = -1;
  private Candidate[] candidates = null;
  private Candidate[] candidateClasses = null;
  private Candidate candidatesMatch = null;
  private DataModel model;
  private JScrollPane candidatesScrollPane;
  private TextFrame discrepancyPane;

  private class HistoryActionListener implements ActionListener {
    public void actionPerformed(ActionEvent e) {
      if(disableHistoryActionListener == false) {
	State state = (State)history.getSelectedItem();
	if(state == null) return;
	model.setSelectedTimeStep(state.getTime());
        System.out.println("\nCandidate Mgr selected timestep " + state.getTime());
        // reissue prompt
        System.out.print("======> ");
      }
    }
  }

  // tab 0 - find candidate class's time permutations on the
  // Candidates tab
  // flip to Candidates tab, highlight time permutations, and
  // scroll them to top of pane
  private class ClassListSelectionListener implements ListSelectionListener {
    public void valueChanged( ListSelectionEvent e) {
      if (e.getValueIsAdjusting()) { return; }
      if (model != null) {
	int s = CandidateManager.this.candidateClassesTable.getSelectedRow();
	int index = candidateClassesTableModel.getCandidateFor( s);
	if (index > -1 && index < candidateClasses.length &&
            index != lastSelectedCandidateClassIndex) {
          if (lastSelectedCandidateClassIndex != -1) {
            markCandidateUnSelected
              ( candidateClasses[lastSelectedCandidateClassIndex]);
          }
          lastSelectedCandidateClassIndex = index;
          // set all assignments of this candidate as selected
          markCandidateSelected( candidateClasses[index]);
          // highlight time permutations
          if (highlightPermutations( index) == 1) {
            // if only 1 time permutation, select it
            model.setSelectedCandidate( candidatesMatch);
          }
          tp.setSelectedIndex( 1); // flip to Candidates tab
          // System.err.println( "start run setFocus");
          // setFocus();
        }
      }
    }
  } // end ClassListSelectionListener

  // cannot get this to work: if mouse does not move when switching from
  // tab 0 to tab 1, two clicks are needed to select candidate.
  // if mouse is moved, only one click is needed
  private void setFocus() {
    Runnable doRequestFocus = new Runnable() {
        public void run() {
          JComponent comp= (JComponent) tp.getSelectedComponent();
          boolean isVisible = comp.isVisible();
          System.err.println( "run setFocus selected " + tp.getSelectedIndex() +
                              " visible " + isVisible + " enabled " +
                              comp.isRequestFocusEnabled());
          // comp.grabFocus();
          comp.setNextFocusableComponent( comp);
        }
      };
    SwingUtilities.invokeLater( doRequestFocus);
  } // end requestFocus


  private int highlightPermutations( int index) {
    CandidateAssignment[] candClassAssigns =
      ((Candidate) candidateClasses[index]).getCandidateAssignments();
    int candClassAssignsLength = candClassAssigns.length;
    int candidateMatchCnt = 0, rowCntFirstMatch = 0;
    candidatesMatch = null;
    for (int i = 0, n = candidates.length; i < n; i++) {
      CandidateAssignment[] candAssigns =
        ((Candidate) candidates[i]).getCandidateAssignments();
      int canAssignsLength = candAssigns.length;
      boolean isHighlighted = true;
      if (canAssignsLength != candClassAssignsLength) {
        isHighlighted = false;
      } else {
        for (int k = 0; k < candClassAssignsLength; k++) {
          String candClassAssignText = candClassAssigns[k].getText();
          boolean isMatch = false;
          for (int j = 0; j < canAssignsLength; j++) {
            if (candAssigns[j].getText().equals( candClassAssignText)) {
              isMatch = true;
              break;

            }
          }
          if (! isMatch) {
            isHighlighted = false;
            break;
          }
        }
      }
      if (isHighlighted) {
        if (candidatesMatch == null) {
          candidatesMatch = candidates[i];  // install this, if only match
          rowCntFirstMatch = i;
        }
        candidateMatchCnt += 1;
      }
      // System.err.println( "cand # " + i + "isHighlighted" + isHighlighted);
      for (int j = 0; j < canAssignsLength; j++) {
        if (isHighlighted) {
          candAssigns[j].setBackgroundColor( Color.yellow);
        } else {
          candAssigns[j].setBackgroundColor( Color.white);
        }
      }
    }
    // scroll first matched candidate to top of scroll pane
    JScrollBar verticalBar = candidatesScrollPane.getVerticalScrollBar();
    verticalBar.setValue( candidatesTable.getRowHeight() * rowCntFirstMatch);
    return candidateMatchCnt;
  } // end highlightPermutations

  // tab 1 - select which time permutation of candidate class to install
  private class CandidateListSelectionListener implements ListSelectionListener {
    public void valueChanged( ListSelectionEvent e) {
      if (e.getValueIsAdjusting()) {
        return;
      }
      if (model != null) {
	int s = CandidateManager.this.candidatesTable.getSelectedRow();
	int index = candidatesTableModel.getCandidateFor(s);
	if (index > -1 && index < candidates.length &&
            index != lastSelectedCandidateIndex) {
	  model.setSelectedCandidate( candidates[index]);
          if (lastSelectedCandidateIndex != -1) {
            markCandidateUnSelected( candidates[lastSelectedCandidateIndex]);
          }
          lastSelectedCandidateIndex = index;
          // set all assignments of this candidate as selected
          markCandidateSelected( candidates[index]);
	}
      }
    }
  } // end CandidateListSelectionListener

  private void markCandidateSelected( Candidate candidate) {
    CandidateAssignment[] candAssigns = candidate.getCandidateAssignments();
    for (int i = 0, n = candAssigns.length; i < n; i++) {
      candAssigns[i].setSelected( true);
    }
  } // end markCandidateSelected

  private void markCandidateUnSelected( Candidate candidate) {
    CandidateAssignment[] candAssigns = candidate.getCandidateAssignments();
    for (int i = 0, n = candAssigns.length; i < n; i++) {
      candAssigns[i].setSelected( false);
    }
  } // end markCandidateUnSelected

  public CandidateManager(EngineManager engineManager, DataModel dataModel) {
    super("Candidate Manager");
    dataModel.addPropertyChangeListener(this);
    setModel(dataModel);
    getContentPane().setLayout(new BorderLayout());
    toolbar = new MyToolBar();
    getContentPane().add(toolbar, "North");
    tp = new JTabbedPane();

    tp.setPreferredSize(new Dimension(500,500));  //added by RG
    tp.setMinimumSize(new Dimension(500, 200));   //added by RG

    // create Candidate Classes tab
    candidateClassesTableModel = new MyTableModel( "candidateClasses");
    candidateClassesTable = new MultiSpanCellTable( candidateClassesTableModel);
    setupTableHeader( candidateClassesTable);
    ListSelectionListener candClassSelectionListener =
      new ClassListSelectionListener();
    candidateClassesTable.getSelectionModel().
      addListSelectionListener( candClassSelectionListener);
    candidateClassesTable.getSelectionModel().
      setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    candidateClassesTable.setCellSelectionEnabled(false);
    candidateClassesTable.setRowSelectionAllowed(true);
    JScrollPane candClassScrollPane = new JScrollPane( candidateClassesTable);
    tp.add( candClassScrollPane, "Classes");

    // create All Candidates tab
    candidatesTableModel = new MyTableModel( "candidates");
    candidatesTable = new MultiSpanCellTable(candidatesTableModel);
    // this code causes column identifiers to disappear under JDK1.3
//     candidatesTable = new MultiSpanCellTable(candidatesTableModel) {
//       protected JTableHeader createDefaultTableHeader() {
// 	return new GroupableTableHeader(columnModel);
//       }
//     };
    setupTableHeader( candidatesTable);
    ListSelectionListener candidateSelectionListener =
      new CandidateListSelectionListener();
    candidatesTable.getSelectionModel().
      addListSelectionListener( candidateSelectionListener);
    candidatesTable.getSelectionModel().
      setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    candidatesTable.setCellSelectionEnabled(false);
    candidatesTable.setRowSelectionAllowed(true);
    candidatesScrollPane = new JScrollPane(candidatesTable);
    tp.add(candidatesScrollPane, "Candidates");

    createAssignmentsTab();
    //Added By RG
    discrepancyPane = new TextFrame(500,100,"Discrepant Commands and Observations");

    JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT,tp, discrepancyPane);
    splitPane.setOneTouchExpandable(true);

    JPanel pp = new JPanel(new BorderLayout());
    pp.add(splitPane, "Center");
    infoLine = new JLabel(" ");
    pp.add(infoLine, "South");
    getContentPane().add(pp, "Center");

    //This line causes the Split Pane components to size incorrectly
    //getRootPane().setPreferredSize(new Dimension(500,500));

    setSize(new Dimension(600,600));   //Added By RG
    pack();
  }





  private final void createAssignmentsTab() {
    tree = new JTree(assignmentTreeModel);
    tree.setRootVisible(true);
    DefaultTreeCellRenderer tcr = new DefaultTreeCellRenderer();
    Icon branch = null;
    Icon leaf = null;
    try {
      ClassLoader cl = getClass().getClassLoader();
      if(cl != null) {
	java.net.URL url = cl.getResource("gov.nasa.arc.l2tools.icons/component.jpg");
	if(url != null) {
	  branch = new ImageIcon(url);
	}
	tcr.setOpenIcon(branch);
	tcr.setClosedIcon(branch);
	url = cl.getResource("gov.nasa.arc.l2tools.icons/attribute.jpg");
	if(url != null) {
	  leaf = new ImageIcon(url);
	}
       tcr.setLeafIcon(leaf);
      }
    } catch(SecurityException e) {
      System.out.println(e);
    }
    tree.setCellRenderer(tcr);

    JScrollPane sp = new JScrollPane(tree);
    tp.add(sp, "Assignments");
  } // end createAssignmentsTab


  public java.awt.Container getContainer() { return this; }

  public void setModel(DataModel model) { this.model = model; }

  public DataModel getModel() { return model; }

  private void setupTableHeader( MultiSpanCellTable table) {
    TableColumnModel cm = table.getColumnModel();
    ColumnGroup g_name = new ColumnGroup("Rank");
    g_name.add(cm.getColumn(1));
    g_name.add(cm.getColumn(2));
    // this code causes column identifiers to disappear under JDK1.3
    // source code not avialable
    // this produced |   Rank   |
    //               |     |    |
//     GroupableTableHeader header = (GroupableTableHeader)table.getTableHeader();
//     header.addColumnGroup(g_name);
    cm.getColumn(0).setPreferredWidth(30);
    cm.getColumn(1).setPreferredWidth(30);
    cm.getColumn(2).setPreferredWidth(30);
    cm.getColumn(3).setPreferredWidth(40);
    cm.getColumn(4).setPreferredWidth(370);
    //    table.setAutoResizeMode(JTable.AUTO_RESIZE_LAST_COLUMN);
  }

  private void clear() {
    clearDiscrepancys(); //Added By RG
    clearCandidates();
    clearHistory();
  }



  //Added By RG
  private void clearDiscrepancys(){

    discrepancyPane.clear();
  }



  private void clearCandidates() {
    lastSelectedCandidateIndex = -1;
    lastSelectedCandidateClassIndex = -1;
    candidatesTable.clearSelection();
    candidatesTableModel.clear();
    candidateClassesTable.clearSelection();
    candidateClassesTableModel.clear();
    repaint();
  }

  private void clearHistory() {
    history.removeAllElements();
    StateTreeNode root = (StateTreeNode)assignmentTreeModel.getRoot();
    root.clearValues();
    //    assignmentTreeModel.nodeStructureChanged(root);
  }

  public void propertyChange(PropertyChangeEvent evt) {
    // System.err.println("\nCanMgr.propertyChange: evt " + evt.getPropertyName());
//     try {
//       throw new Exception();
//     } catch (Exception e) { e.printStackTrace(); }

    if (! model.getEnableGUIUpdate())
      return;

    if (evt.getPropertyName().equals("candidates") && model != null) {
      clear();
      tp.setSelectedIndex( 0); // put candidate classes on top
      // tp.getSelectedComponent().requestFocus();
      candidateClasses = model.getCandidateClasses();
      // System.err.println( "CM.propertyChange candidateClasses size " +
      //                     candidateClasses.length);
      candidates = model.getCandidates();
      // System.err.println( "CM.propertyChange candidates size " +
      //                     candidates.length);
      if (candidateClasses.length > 0) {
        if ((candidateClasses.length == 1) &&
            (candidates.length == 1)) {
          // DataModel.replyFoundCandidates does setSelectedCandidate
          // if # of candidates = 1
          markCandidateSelected( candidateClasses[0]);
          tp.setSelectedIndex( 1); // put all candidates on top
          // tp.getSelectedComponent().requestFocus();
        }
        candidateClassesTableModel.setCandidates( candidateClasses);
      }

      if(candidates.length > 0) {
        candidatesTableModel.setCandidates( candidates);
        discrepancyPane.setText(model.getDiscrepancyAssignments()); //Added By RG
      }

    } else if(evt.getPropertyName().equals("searchTermination") && model != null) {
      infoLine.setText(model.getSearchTermination());

    } else if(evt.getPropertyName().equals("selectedCandidate") && model != null) {
      Candidate c = model.getSelectedCandidate();
      if(c != null) {
        if(candidates != null && candidates.length > 0) {
          for(int index = 0; index < candidates.length; index++) {
            if(candidates[index] == c) {
              int[] selectedRows = candidatesTableModel.getSelectionFor(index);
              if(selectedRows != null && selectedRows.length > 0)
                candidatesTable.setRowSelectionInterval(selectedRows[0], selectedRows[0]);
              CandidateManager.this.clearHistory();
              CandidateManager.this.getHistoryForCandidate(index);
              CandidateManager.this.toolbar.checkArrows();
              int i = model.getSelectedTimeStep() - model.getStartTimeStep();
              if(i > -1 && i < history.getSize()) {
                State state = (State)history.getElementAt(i);
                updateAssignments(state);
              }
              if (candidates.length > 1) {
                System.out.println("\nCandidate Mgr selected candidate " + index +
                                   " from Candidates");
                // reissue prompt
                System.out.print("======> ");
              }
              break;
            }
          }
        }
      }

    } else if(evt.getPropertyName().equals("selectedTimeStep") && model != null) {
      Object obj = CandidateManager.this.history.getSelectedItem();
      int index = CandidateManager.this.history.getIndexOf(obj);
      int i = model.getSelectedTimeStep() - model.getStartTimeStep();
      if(i > -1 && i < history.getSize()) {
        State state = (State)history.getElementAt(i);
        updateAssignments(state);
      }

      //    } else if(evt.getPropertyName().equals("state") && model != null) {
      //      if(model.getState().equals("searching"))
      //        clear();

    }
  }


  public int[] getCurrentSelectionInterval() {
    int s = CandidateManager.this.candidatesTable.getSelectedRow();
    if(s == -1) return null;
    else return candidatesTableModel.getSelectionFor(s);
  }

  private void selectLastHistory() {
    int index = history.getSize()-1;
    //    if(lastSelectedHistory != -1 && lastSelectedHistory < index) index = lastSelectedHistory;
    Object last = history.getElementAt(index);
    history.setSelectedItem(last);
    CandidateManager.this.toolbar.checkArrows();
  }

  private void updateAssignments(State state) {
    if(state != null && model.getPipeline() != null) {
      //      if(history.getSelectedItem() != state) history.setSelectedItem(state);
      StateTreeNode root = (StateTreeNode)assignmentTreeModel.getRoot();
      //      root.clearValues();
      state.setRootNode(root);
      state.flushCache(tree, assignmentTreeModel, null);
      toolbar.checkArrows();
    }
  }

  protected void getHistoryForCandidate(int index) {
    if(model != null && candidates != null) {
      try {
	disableHistoryActionListener = true;
	AttributeAssignment[][] atts =
          model.getPipeline().getAttributeAssignments(this, candidates[index]);
	for(int i = 0; i < atts.length; i++) {
	  history.addElement(new State(Integer.toString(i+model.getStartTimeStep()),
                                       i+model.getStartTimeStep(), atts[i]));
	}
	int i = model.getSelectedTimeStep() - model.getStartTimeStep();
	if(i > 0 && i < history.getSize()) {
	  history.setSelectedItem(history.getElementAt(i));
	}
	disableHistoryActionListener = false;
      } catch(IndexOutOfBoundsException e) {
	System.out.println(e);      } catch(L2NativeException e) {
	System.out.println(e);
      }
    }
  }

  private class MyToolBar extends JToolBar {

    private Icon leftIcon = null;
    private Icon rightIcon = null;
    private LeftArrow left;
    private RightArrow right;

    public MyToolBar() {
      super();
      try {
	ClassLoader cl = CandidateManager.this.getClass().getClassLoader();
	  if(cl != null) {
	    java.net.URL url = cl.getResource("gov.nasa.arc.l2tools.icons/left.gif");
	      if(url != null) {
		  leftIcon = new ImageIcon(url);
	      }
	      url = cl.getResource("gov.nasa.arc.l2tools.icons/right.gif");
	      if(url != null) {
		rightIcon = new ImageIcon(url);
	      }
	  }
      } catch(SecurityException e) {
	System.out.println(e);
      }

      left = new LeftArrow(null, leftIcon);
      add(left);

      JComboBox historyList = new JComboBox(CandidateManager.this.history);
      ActionListener selectionListener = new HistoryActionListener();
      historyList.addActionListener(selectionListener);
      add(historyList);
      right = new RightArrow(null, rightIcon);
      add(right);
    }

    public void goLeft() {
      Object obj = CandidateManager.this.history.getSelectedItem();
      int index = CandidateManager.this.history.getIndexOf(obj);
      obj = CandidateManager.this.history.getElementAt(index - 1);
      CandidateManager.this.history.setSelectedItem(obj);
      checkArrows();
    }

    public void goRight() {
      Object obj = CandidateManager.this.history.getSelectedItem();
      int index = CandidateManager.this.history.getIndexOf(obj);
      obj = CandidateManager.this.history.getElementAt(index + 1);
      CandidateManager.this.history.setSelectedItem(obj);
      checkArrows();
    }

    public void checkArrows() {
      Object obj = CandidateManager.this.history.getSelectedItem();
      if(obj == null) {
	left.setEnabled(false);
	right.setEnabled(false);
      } else {
	int index = CandidateManager.this.history.getIndexOf(obj);
	if(index < 1) left.setEnabled(false);
	else left.setEnabled(true);
	if(index > CandidateManager.this.history.getSize() - 2) right.setEnabled(false);
	else right.setEnabled(true);
      }
    }

    private class LeftArrow extends AbstractAction {
      public LeftArrow(String label, Icon icon) { super(label, icon); }
      public void actionPerformed(ActionEvent e) {
	MyToolBar.this.goLeft();
      }
    }
    private class RightArrow extends AbstractAction {
      public RightArrow(String label, Icon icon) { super(label, icon); }
      public void actionPerformed(ActionEvent e) {
	MyToolBar.this.goRight();
      }
    }
  }

}

