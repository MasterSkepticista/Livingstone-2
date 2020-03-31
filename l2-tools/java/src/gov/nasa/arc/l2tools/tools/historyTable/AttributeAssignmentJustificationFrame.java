//
// * See the file "l2-tools/disclaimers-and-notices.txt" for
// * information on usage and redistribution of this file,
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools.historyTable;

import livdll.L2NativeException;
import gov.nasa.arc.l2tools.tools.ScenarioInterface;
import gov.nasa.arc.l2tools.gutil.TextFrame;
import livdll.AttributeAssignment;
import javax.swing.plaf.metal.MetalTreeUI;
import gov.nasa.arc.l2tools.tools.LivingstoneEngine2.JustificationStruct;
import gov.nasa.arc.l2tools.util.EventLog;
import java.awt.event.MouseListener;
import java.awt.event.MouseAdapter;
import java.util.List;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.*;
import java.util.*;
import javax.swing.event.TreeSelectionListener;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeEvent;
import javax.swing.ButtonGroup;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.JTextArea;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import java.awt.event.ActionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeSelectionModel;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.tree.TreePath;
import java.awt.Insets;
import java.awt.CardLayout;
import javax.swing.JPanel;
import java.util.HashMap;
import javax.swing.event.ListSelectionListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.plaf.basic.BasicTreeUI;
import javax.swing.plaf.TreeUI;
import javax.swing.tree.TreeCellRenderer;
import java.awt.Component;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.ImageIcon;
import javax.swing.ListModel;
import javax.swing.JLabel;
import javax.swing.ListCellRenderer;
import javax.swing.UIManager;
import java.awt.BorderLayout;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JToolBar;
import javax.swing.ListSelectionModel;
import java.util.List;
import java.beans.PropertyChangeSupport;
import java.beans.PropertyChangeListener;
import javax.swing.tree.DefaultTreeSelectionModel;
import javax.swing.BorderFactory;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;
import java.awt.Toolkit;

import com.graph.*;
import com.graph.algorithm.*;
import com.graph.visual.*;
import com.graph.visual.layout.*;
import com.graph.visual.drawing.*;
import java.awt.*;
import javax.swing.*;

public class AttributeAssignmentJustificationFrame extends JFrame {
  //Type of line-rendering between nodes in the JTree.
  private String lineStyle = "Angled";

  private Proposition propositionToExplain = null;
  //State of the Livingstone Engine.
  private DataModelState dataModelState = null;
  //Displays immediate supporting clause for a proposition
  private ClauseSupportPane  clauseSupportPane = new ClauseSupportPane(0,0,"Immediate Supporting Clause");;
  //Livingstone's chain of reasoning data model
  private InferenceModel model = null;
  //Manages the different Tree Views for a Inference Tree
  private CardLayout treeManager = null;
  //Main Panel to Display the inference tree and related tree views
  private JPanel treePanel = null;
  //Current propositions being displayed by the tree
  private DefaultListModel propList = new DefaultListModel();
  //Graphical view of the propList
  private MyJList jlist = new MyJList(propList);
  //Collection of Tree Views
  private HashMap treeMap = new HashMap();

  //Describes the detail level that should be displayed for inference tree.
  //Proposition Level (default) displays Livingstone whole chain of reasoning
  private static String AttributeLevel = "Attribute";
  private static String PropositionLevel = "Proposition";
  private static String AssumptionLevel = "Assumption";

  private String ModelDisplayDetailLevel = PropositionLevel;

  //Describes the type of tree
  private String TreeMode="TREE";
  private String GraphMode = "GRAPH";
  private String ModelDisplayMode = TreeMode;

  static private ImageIcon leafIcon =null;
  static private ImageIcon propIcon =null;
  static private ImageIcon rootIcon =null;
  static private ImageIcon clausePropIcon =null;

  static Dimension screenSize =null;

  //Custom Look and Feel for Justification Frame
  static {
    try{

    UIManager.put("MyJTree", "gov.nasa.arc.l2tools.tools/historyTable/SkinTreeUI");
    UIManager.put("MyJTree.expandedIcon", SkinTreeUI.ExpandedIcon.createExpandedIcon());
    UIManager.put("MyJTree.collapsedIcon", SkinTreeUI.CollapsedIcon.createCollapsedIcon());
    }
    catch(Exception e) {System.out.println(e.getMessage());}
  }


  /* Class: AttributeAssignmentJustificationFrame
   *
   *    Provides a graphical view of Livingstone's chain of reasoning
   *    for inferred propositions at specific time steps. The chain of reasoning
   *    is composed of propositions and their supporting clauses. Each inferred
   *    propostion is explained via a disjunctive clause (set of propositions).
   *    Each member proposition of a clause is in turn explained by a clause. This
   *    recursive chain of reasoning continues until an assumption (observation/command) is
   *    reached.
   *                        proposition-to-explain
   *                            /     |       \
   *                      prop1      prop2    prop3  (1 supporting clause)
   *                     /   \         |       /   \
   *                  prop3  prop4   prop5  prop6  prop7  (3 supporting clauses)
   *                    :      :       :      :      :
   *
   *
   *
   *    The view is derived from three separate window panes.
   *    - clause view pane  lists the immediate supporting clause for a selected
   *      proposition.
   *
   *    - attribute list pane enumerates all the propositions involved in the
   *      explanation.
   *
   *
   *    - chain of reasoning pane displays the proof tree for the proposition
   *      of interest.
   *
   **/
  public AttributeAssignmentJustificationFrame(String propString, DataModelState dataModelState) throws DataModelStateError, L2NativeException{


    this.dataModelState = dataModelState;                //Set DataModelState before creating any Propositions
                                                         //Class is dependent on the dataModelState var being defined
    this.propositionToExplain = new Proposition(propString);

    this.screenSize = this.getToolkit().getScreenSize();
    int width = (int)(screenSize.getWidth()*.4);
    int height = (int)(screenSize.getHeight()*.4);
    this.screenSize.setSize(new Dimension(width,height));

    loadIcons();
    //Define the Window Pane for Immediate Support Clauses
    initClauseSupportPane();

    //Define the Tree Views describing L2's chain of reasoning
    initTreeViews();

    //Defines the Attribute List pane.
    JScrollPane attributeListView = initList();

    this.setTitle("Justification for "+propositionToExplain);

    JPanel panel = new JPanel(new BorderLayout());

    JSplitPane vSplitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT,treePanel,clauseSupportPane);
    vSplitPane.setOneTouchExpandable(true);


    JPanel attributePanel = new JPanel(new BorderLayout());
    JToolBar toolBar = createReferenceBar();
    attributePanel.add(toolBar,BorderLayout.NORTH);
    attributePanel.add(attributeListView,BorderLayout.CENTER);

    JSplitPane hSplitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,attributePanel,vSplitPane);
    hSplitPane.setDividerSize(3);
    
    panel.add(hSplitPane,BorderLayout.CENTER);


    setJMenuBar(initializeMenu());

    //Displays the default tree view.
    treeManager.show(treePanel,ModelDisplayMode+ModelDisplayDetailLevel);

    getContentPane().add(panel);

    setSize(screenSize);

    pack();
    show();
  }



  /*    Method: loadIcons
   *        Loads .gif icons used by the Justification Tool
   *
   */

  private void loadIcons(){
    leafIcon = loadImage("gov.nasa.arc.l2tools.icons/assumption.gif");
    propIcon = loadImage("gov.nasa.arc.l2tools.icons/supportProp.gif");
    rootIcon = loadImage("gov.nasa.arc.l2tools.icons/explainProp.gif");
    clausePropIcon = loadImage("gov.nasa.arc.l2tools.icons/clauseProp.gif");

  }


  private JToolBar createReferenceBar(){

     JToolBar toolBar = new JToolBar();

     JButton btn = null;

     toolBar.add(btn =new JButton(null,rootIcon));
     btn.setToolTipText("Attribute Assignment to Explain");

     toolBar.add(btn=new JButton(null,propIcon));
     btn.setToolTipText("Attribute Assignment");

     toolBar.add(btn=new JButton(null,leafIcon));
     btn.setToolTipText("Assumption/Observation/Command");

     toolBar.add(btn=new JButton(null,clausePropIcon));
     btn.setToolTipText("Member of Supporting Clause");

     toolBar.setBackground(getBackground());

     Border raisedbevel = BorderFactory.createRaisedBevelBorder();

     Border loweredbevel = BorderFactory.createLoweredBevelBorder();

     Border compound = BorderFactory.createCompoundBorder(raisedbevel, loweredbevel);

     Border titled = BorderFactory.createTitledBorder(compound,"Attribute Assignments",
                                                      TitledBorder.CENTER,TitledBorder.TOP);

     toolBar.setBorder(titled);

     return toolBar;
  }






  /*    Method: loadImage
   *        loads icons from gif/jpeg files.
   *
   */
  private ImageIcon loadImage(String pathFileName){
         ImageIcon image = null;
     try{

         ClassLoader cl = this.getClass().getClassLoader();

	 if(cl == null)  throw new RuntimeException("Class loader failed");

         java.net.URL url = cl.getResource(pathFileName);

	 if(url != null){image = new ImageIcon(url);}
         else throw new RuntimeException("Unable to Locate file");


     }catch(Exception e){
         System.out.println("Couldn't Load Image: " +e);
     }
         return image;
     }


  /*
   *    Method: initTreeViews
   *        Creates the data model used by all the tree views.
   *        6 different tree views are created in memory.
   *        3 tree views are in OUTLINE style display
   *        3 tree views are in GRAPH style displayL2NativeException
   *
   */
  private JScrollPane initTreeViews() throws DataModelStateError, L2NativeException{

    //Create the data model for the propositions.
    //Relationship between JAVA tree nodes is the same as for the actual L2
    //propositions. That is a heirarichal relationships exists between the
    // propositions


    DefaultMutableTreeNode root = new DefaultMutableTreeNode(propositionToExplain);
    model = (InferenceModel)new InferenceModel(root);

    //Create the 3 OUTLINE style trees.  Each tree displays a different degree
    //of detail for Livingstone's Chain of Reasoning

    addTree(model.getAttributeModel(),TreeMode+AttributeLevel);
    addTree(model,TreeMode+PropositionLevel);
    addTree(model.getAssumptionModel(),TreeMode+AssumptionLevel);

    //Create the 3 GRAPH style trees.  Each tree displays a different degree
    //of detail for Livingstone's Chain of Reasoning

    addGraph(model,GraphMode+PropositionLevel);
    addGraph(model.getAttributeModel(), GraphMode+AttributeLevel);
    addGraph(model.getAssumptionModel(), GraphMode+AssumptionLevel);

    JScrollPane treeView = new JScrollPane(treePanel);
    treeView.setMinimumSize(new Dimension(0,0));
    int width = (int) (.55 *screenSize.getWidth());
    int height = (int) (.9 *screenSize.getHeight());
    treeView.setPreferredSize(new Dimension(width,height));
    treePanel.setPreferredSize(new Dimension(width,height));
    return treeView;
  }




  /*    Method: initClauseSupportPane
   *        Defines the window pane to display immediate supporting clauses
   *
   *
   **/
  private void initClauseSupportPane(){

    if(clauseSupportPane == null)
        clauseSupportPane = new ClauseSupportPane(0,0, "Supporting Clause");

    clauseSupportPane.setMinimumSize(new Dimension(0, 0));
    int width = (int) (.45 *screenSize.getWidth());
    int height = (int) (.1 *screenSize.getHeight());
    clauseSupportPane.setPreferredSize(new Dimension(width,height));


  }




  /*
   *    Method: initList
   *        Creates the default view for the attribute list pane
   *        Attribute list contains all the propositions involved in
   *        the chain of reasoning. In List Selection Listeners are
   *        initialized so that the currenly displayed tree can update
   *        its view.
   **/
  private JScrollPane initList(){

    jlist.setCellRenderer(new MyCellRenderer());
    jlist.setBackground(getBackground());
    JScrollPane listView = new JScrollPane();
    int width = (int) (.55 *screenSize.getWidth());
    int height = (int) (1.0 *screenSize.getHeight());
    listView.setPreferredSize(new Dimension(width,height));
    listView.getViewport().setView(jlist);

    jlist.addListSelectionListener(clauseSupportPane);

     //Fills List with DATA from the currently displayed tree.
     createDataList();

     return listView;
  }




  /*    method: createDataList
   *        Creates the viewable attribute list based on the current
   *        tree view being displayed. Each node in the list is
   *        enumerated so that they cross-match with particular tree nodes.
   *
   */
  private void createDataList(){

    jlist.clear();
    int selectedIndex = -1;
    Object values[] = null;
    PropositionListMap map = null;

    map = (PropositionListMap)treeMap.get(ModelDisplayMode+ModelDisplayDetailLevel);

    jlist.setModelFromOrderedList(map.getOrderedValues());

    selectedIndex = map.getSelectedIndex();

    if(selectedIndex != -1)
        jlist.setSelectionIndex(selectedIndex);
  }



  /*
   *    Method: addGraph
   *        Build a visual graph based on the data model
   *
   *        @param model data model for the tree view
   *        @key   unique identifying this tree view
   */

  private void addGraph(TreeModel model, String key) {

    //CardLayout Manager takes care of toggling between the six available
    //tree views
    if(treePanel==null){ treePanel= new JPanel(treeManager = new CardLayout());}

    Tree graph = new TreeImpl();

    DefaultMutableTreeNode modelRoot = (DefaultMutableTreeNode)model.getRoot();

    Vertex graphRoot = new VertexImpl(modelRoot);

    try{
      graph.addNode(null,graphRoot );
    }catch(Exception e){e.printStackTrace();}

    //Taverses the data model to build the graph
    buildGraph(modelRoot, graphRoot,graph);

    MyVisualGraph vgraph = new MyVisualGraph(graph);

    //The attribute list and clause view update their views based on
    //what is the currently selected node in the tree view

   // vgraph.addVisualGraphListener(clauseSupportPane);
    vgraph.addVisualGraphListener(jlist);
    jlist.addPropertyListener(vgraph);

    vgraph.setListenKey(key);

    List visualVertices = vgraph.getVisualVertices();

    GraphScrollPane graphView = new MyGraphScrollPane(new MyGraphPanelState(),vgraph);

    LayeredTreeLayout layoutManager = new LayeredTreeLayout( vgraph );

    graphView.setGraphLayoutManager( layoutManager );
    layoutManager.setXGridDistance(50);
    layoutManager.setYGridDistance(50);

    //Calculates the specific locations for each vertex on the Panel
    vgraph.layout();

    //TreePanel contains the tree views to toggle between
    treePanel.add(graphView,key);

    //Collection also contains references to the tree views.
    //Used to setup listeners, current selection values, and attribute lists
    //The Tree views are not accessible from treePanel. Therefore, they
    //are saved separately (a reference).
    treeMap.put(key,vgraph);
  }



  /*
   *    Method: buildGraph
   *        build a lattice/graph view based on the data model
   *
   */

  private void buildGraph(DefaultMutableTreeNode modelParent, Vertex graphParent, Tree graph){


    for(Enumeration children = modelParent.children(); children.hasMoreElements();){

        DefaultMutableTreeNode modelChild = (DefaultMutableTreeNode)children.nextElement();

        Vertex graphChild = new VertexImpl(modelChild);

        try{ graph.addNode(graphParent, graphChild );}
        catch(Exception e){e.printStackTrace();}
        buildGraph(modelChild,graphChild,graph);
    }
  }




  /*
   *    Method: addTree
   *        Builds a JTREE based on the data model specified
   *
   *        @param model data model
   *        @key   unique indentifier for this tree view
   */
  private void addTree(TreeModel model, String key){

    if(treePanel==null){ treePanel= new JPanel(treeManager = new CardLayout());}

    MyJTree tree = new MyJTree(model);
    tree.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
    tree.setListenKey(key);
    tree.setScrollsOnExpand(true);
    tree.putClientProperty("JTree.lineStyle", lineStyle);

    tree.addPropertyListener(jlist);
    jlist.addPropertyListener(tree);


    JScrollPane treeView = new JScrollPane(tree);

    //TreePanel contains the tree views to toggle between
    treePanel.add(treeView,key);

    //Collection also contains references to the tree views.
    //Used to setup listeners, current selection values, and attribute lists
    //The Tree views are not accessible from treePanel. Therefore, they
    //are saved separately (a reference).
    treeMap.put(key,tree);
  }




  /*
   *    Method initializeMenu
   *        Creates the main main bar for the Justification Tool
   *
   */
  private JMenuBar initializeMenu() {

    JMenuBar menuBar = new JMenuBar();
    JMenu file = new JMenu("File");
    JMenu edit = new JMenu("Edit");
    JMenu view = new JMenu("View");
    JMenu detail = new JMenu("Detail");


    JMenuItem fileExit = new JMenuItem("Exit");

    fileExit.addActionListener(new ActionListener(){

        public void actionPerformed(ActionEvent e) {

            String command = e.getActionCommand();

            if ("Exit".equals(command)) dispose();
        }
    });

    file.add(fileExit);
    menuBar.add(file);

    JMenuItem editFindString = new JMenuItem("Find String");

    edit.add(editFindString);
    // not implemented
    editFindString.setEnabled(false);
    menuBar.add(edit);

    JMenuItem viewOutline = new JCheckBoxMenuItem("Justification as Outline");
    JMenuItem viewGraph = new JCheckBoxMenuItem("Justification as Graph");
    JMenuItem viewCandidates = new JCheckBoxMenuItem("Candidates");

    ButtonGroup group = new ButtonGroup();

    view.add(viewOutline);
    viewOutline.setSelected(true);

    //In Tree view mode, connect the displayed JTree as a list
    //selection listener.  Note, the same jlist is used to represent
    //the different tree and graph views.  If we switch to a graph view
    //the current JTree must be disconnect or else it will receive
    viewOutline.addActionListener(new ActionListener(){

        public void actionPerformed(ActionEvent event){

            ModelDisplayMode = TreeMode;
            updateDisplay();
        }
    });

    group.add(viewOutline);

    view.add(viewGraph);

    viewGraph.addActionListener(new ActionListener(){

        public void actionPerformed(ActionEvent event){
            ModelDisplayMode = GraphMode;
            updateDisplay();
        }
    });

    group.add(viewGraph);

    //not implemented
    view.add(viewCandidates);
    viewCandidates.setEnabled(false);

    menuBar.add(view);

    // Group the radio buttons.
    group = new ButtonGroup();
    DetailListener detailListener = new DetailListener();

    JMenuItem detailAttributes = new JRadioButtonMenuItem("By Attributes");
    detailAttributes.setActionCommand(AttributeLevel);
    detailAttributes.addActionListener(detailListener);
    group.add(detailAttributes);

    JMenuItem detailPropositions = new JRadioButtonMenuItem("By Propositions");
    detailPropositions.setActionCommand(PropositionLevel);
    detailPropositions.addActionListener(detailListener);
    detailPropositions.setSelected(true);


    group.add(detailPropositions);

    JMenuItem detailAssumptions = new JRadioButtonMenuItem("By Assumptions");
    detailAssumptions.setActionCommand(AssumptionLevel);
    detailAssumptions.addActionListener(detailListener);
    group.add(detailAssumptions);

    detail.add(detailAttributes);
    detail.add(detailPropositions);
    detail.add(detailAssumptions);
    menuBar.add(detail);

    return menuBar;
  }



  /*    Method updateDisplay:
   *        updates the tree view, clause support and attribute list panes
   *
   */

  private void updateDisplay(){

    createDataList();           //Update jlist with data of the current tree view
                                //Update Tree View
    treeManager.show(treePanel, ModelDisplayMode+ModelDisplayDetailLevel);
  }




  /*
   *    Method: DetailListener
   *        The tree view pane shows L2's chain of reasoning.
   *        This tree view can display simplified veiws of this chain
   *        of reasoning. Therefore, the DetailListener listens when
   *        displayed detail level changes
   *
   */


  private class DetailListener implements ActionListener {

    public void actionPerformed(ActionEvent e) {

      String command = e.getActionCommand();

      if (AttributeLevel.equals(command)) {

        ModelDisplayDetailLevel = AttributeLevel;

      } else if (PropositionLevel.equals(command)) {

        ModelDisplayDetailLevel = PropositionLevel;

      } else if (AssumptionLevel.equals(command)) {

        ModelDisplayDetailLevel = AssumptionLevel;
      }

      updateDisplay();
    }
  }






/*  Method: getPipleline
 *          Gets LivingstoneEngine Interface.
 *          Verifies that L2 is still in the same state when root proposition
 *          was selected for explanation.
 */

  protected ScenarioInterface getPipeline() throws DataModelStateError {
    try {
      return dataModelState.getPipeline();
    } catch (DataModelStateError e) {

        throw new DataModelStateError("Engine out of Sync. Unable to Create Inference Tree for "+propositionToExplain);
    }
  }






 /*
  * Class: Proposition
  *     Customizes an AttributeAssignment into a propositional representation.
  *     Attribute assignment does not return a logically correct representation
  *     of a proposition. For example the current time is represented as Integer.MAX_VALUE.
  *
  *
  */


  private class Proposition extends AttributeAssignment implements Comparable{

    private String propositionalValue = "unknown";
    private String currentTime = null;
    private int id = 0;

    public Proposition(String L2Proposition) throws DataModelStateError,L2NativeException{

        super(L2Proposition);
        
        if(isUndefined()) { 
          throw new RuntimeException(this.getName()+" has not been defined or assigned a value");
        }

        if(!isRecognized(getPipeline())){ throw new L2NativeException("Livingstone does not recognize: "+L2Proposition);}

        if(isTrue(getPipeline())) propositionalValue = "true";
        else if(isFalse(getPipeline())) propositionalValue = "false";

        currentTime = Integer.toString(getPipeline().getHistoryStartStop()[1]);
    }

    public String getPropValue() {return propositionalValue;}

    public void setID( int id ) {this.id = id;}

    public int getID() {return this.id;}

    public int compareTo(Object o){

        return this.getPropString().compareTo(o.toString());
    }

  
    public boolean isUndefined() { return this.getValue().equalsIgnoreCase("<undefined>"); }

    public String toString(){

        String proposition=null;
        int i;

        String name = getName();

        proposition = ((i=getPropString().indexOf("#"))==-1)? currentTime
                                                            : getPropString().substring(0,i);

        proposition += "#";
        proposition += (i=name.indexOf("test.")) == -1?name:name.substring(i+5);

        proposition += "=";
        proposition += getValue();
        proposition += " "+"("+propositionalValue+")";

        return(proposition);
    }


    public boolean equals(Object obj) {
        int i;
        if(obj instanceof Proposition) {
            Proposition p = (Proposition)obj;

            return((p.getName().equals(this.getName())) && (p.getTime()==this.getTime()) &&
                   (p.getValue().equals(this.getValue())) &&
                   (p.getPropValue().equals(this.getPropValue())));
        }
        else return false;
    }
  }




  /*    Class InferenceModel
   *        Represents three data models for Livingstone's Chain of reasoning
   *        Inference Model - contains L2's complete data model representation of
   *                         its chain of reasoning for a specific proposition passed
   *                         in for JUSTIFICATION
   *
   *        Assumption Model - contains all assumptions L2 used to build the inference model
   *
   *        Attribute Model - a simplified version of L2's inference model. It attempts to
   *                          remove extraneous assignments from the tree.
   *
   *                          Example: Variable X has a domain of [0 1 2]
   *                                   Assume variable X = 1
   *                                   Then attribute assignments describing x != 0 and x != 2
   *                                   can be removed from the inference tree
   *
   */

  private class InferenceModel extends DefaultTreeModel{

   private DefaultTreeModel assumptionModel = null;
   private DefaultTreeModel attributeModel = null;


   public InferenceModel(DefaultMutableTreeNode propositionToExplain) throws DataModelStateError, L2NativeException{

        super(propositionToExplain);

        createNodes(propositionToExplain);
    }



    /*  Method: getAssumptionModel()
     *      Creates an Assumption data model from the inference model(main model)
     *
     */
    public DefaultTreeModel  getAssumptionModel(){

        DefaultMutableTreeNode assumptionRoot = new DefaultMutableTreeNode(((DefaultMutableTreeNode)root).getUserObject());

        assumptionModel = new DefaultTreeModel(assumptionRoot);

        DefaultMutableTreeNode child = null;

        createAssumptionModel(root,assumptionRoot);

        return assumptionModel;
    }



    /*  Method: getAttributeModel
     *     create a data model where the attributes are mentioned
     *     only once in the collection. Simplified version of the
     *     main Inference Model.
     */
    public DefaultTreeModel getAttributeModel(){

        DefaultMutableTreeNode top = new DefaultMutableTreeNode(((DefaultMutableTreeNode)root).getUserObject());

        attributeModel = new DefaultTreeModel(top);

        createAttributeModel((DefaultMutableTreeNode)root,top);

        return attributeModel;
    }



    /*  Method: createAttributeModel
     *      Builds an attribute model from the Inference data model
     *
     */
    private void createAttributeModel(DefaultMutableTreeNode attributeNode,DefaultMutableTreeNode node){

        Vector supportingNodes = new Vector();

        filterPropositions(attributeNode, supportingNodes);

        for(int i=0; i < supportingNodes.size(); i++){

            DefaultMutableTreeNode attributeChild = (DefaultMutableTreeNode)supportingNodes.elementAt(i);
            DefaultMutableTreeNode propositionChild = (DefaultMutableTreeNode)attributeChild.clone();
            createAttributeModel(attributeChild,propositionChild);
            node.add(propositionChild);
        }

    }



    /*
     *  Method: filterPropostions
     *      This method attempts to simplify L2's chain of reasoning.
     *      It attaempts to parse out extraneous information, so that attribute
     *      assignments are displayed/mentioned once in the tree view.
     *
     *      If the  inferred proposition (proposition to explain) is false, and it
     *      is mentioned in the supporting clause, then add the supporting clause propostion.
     *      It is assumed the supporting clause proposition is true.
     *
     */
    private void filterPropositions(DefaultMutableTreeNode inferenceNode,Vector supportNodes) {

        Proposition inferenceProp = (Proposition)inferenceNode.getUserObject();
        DefaultMutableTreeNode child = null;

	for (Enumeration children = inferenceNode.children(); children.hasMoreElements();) {

            child = (DefaultMutableTreeNode)children.nextElement();
            Proposition supportProp = (Proposition)(child.getUserObject());

            if (inferenceProp.getName().equals(supportProp.getName())) {

                if (inferenceProp.getPropValue().equals("false")) {
                    supportNodes.add(child);;
                }
                else filterPropositions(child,supportNodes);

            }
            else if (supportProp.getPropValue().equals("false")
                                &&
                    (attributeFoundAtNextLevel(child))) {

                    for (Enumeration grandChildren = child.children(); grandChildren.hasMoreElements();)
                        supportNodes.add(grandChildren.nextElement());

            }
            else supportNodes.add(child);

	 }
    }



    /*
     *  Method: createNodes
     *      Create Livingstone's inference model.
     *      Repeatedly calss Livingstone in order to completely re-create
     *      Livingstones chain of reasoning for the particular proposition
     *      It recursively calls L2, so that L2 can explain each node in the inference tree.
     *      Hence, a full explanation of the initial proposition is compiled.
     */
    private void createNodes(DefaultMutableTreeNode node) throws DataModelStateError,L2NativeException{
        Vector supportingClause = null;
        DefaultMutableTreeNode childNode = null;
        Proposition prop  = (Proposition) node.getUserObject();

        supportingClause = getJustification(prop.getPropString(), getPipeline());

        for(int i=0; i < supportingClause.size(); i++){
            childNode = new DefaultMutableTreeNode(new Proposition((String)supportingClause.elementAt(i)));
            createNodes(childNode);
            node.add(childNode);

        }
    }



    /*  Method: getJustification
     *      Calls L2. Finds the supporting clause (positive and negative antecedents)
     *      for the proposition needs "explanation"
     *
     *
     */
    private Vector getJustification(String proposition, ScenarioInterface L2) {

        Vector supportingClause = new Vector();

        JustificationStruct struct = null;

        try {
        struct = L2.justificationStruct(proposition);
        } catch (L2NativeException err) {};

        if (struct==null) {
        System.err.println("\nCouldn't find justification for "+proposition);
        }
        else{
            supportingClause.addAll(struct.positive_antecedents);
            supportingClause.addAll(struct.negative_antecedents);
        }

        return supportingClause;
    }



  /*
   *    Method: attributeFoundAtNextLevel
   *        Decides if the attribute is also a member of the
   *        supporting clause used to explain the attribute.
   */

  private boolean attributeFoundAtNextLevel(DefaultMutableTreeNode node) {

    String attributeName = ((Proposition)node.getUserObject()).getName();

    for (Enumeration children = node.children(); children.hasMoreElements();) {

      DefaultMutableTreeNode child = (DefaultMutableTreeNode)children.nextElement();
      if (((Proposition)child.getUserObject()).getName().equals(attributeName))
        return true;
    }

    return false;
  }




  /*
   *    Method: createAssumptionModel
   *        identifies all assumptions used to build the complete inference
   *        tree/model
   */
  private void createAssumptionModel(TreeNode node,DefaultMutableTreeNode root){
    DefaultMutableTreeNode child;

    for (Enumeration children = node.children() ; children.hasMoreElements() ;) {

        if( (child =(DefaultMutableTreeNode) children.nextElement()).isLeaf()){
               root.add((DefaultMutableTreeNode)child.clone());
        }
        else createAssumptionModel(child,root);
    }
  }
 }





  /*    Class ClauseSupportPane
   *        Simple JTextArea with JList listening capabilities.
   *        WhenEver there is a JList selection/de-selection,
   *        JList sends an event to ClauseSupportPane.
   *        JList gets its selection events from three sources:
   *        the userl, JTree, and Graph(openjgraph component)
   */

  public class ClauseSupportPane extends TextFrame implements ListSelectionListener{

    public ClauseSupportPane(int x, int y,String title){

        super (x,y,title);
     }


     public void valueChanged(ListSelectionEvent e){

        if(e.getValueIsAdjusting()) {return;}

        MyJList list = (MyJList)e.getSource();

        PropositionListMap map = (PropositionListMap)treeMap.get(ModelDisplayMode+ModelDisplayDetailLevel);
        int selectedKey= list.getSelectedIndex();

        Object nodes[] = map.getMappedValues(selectedKey);

        this.setText("");

        if(selectedKey== -1 || nodes.length == 0) {return;}

        DefaultMutableTreeNode selectNode = null;
        if(nodes[0] instanceof VisualVertex){
            VisualVertex visualVertex = (VisualVertex)nodes[0];
            VertexImpl vertex = (VertexImpl)visualVertex.getVertex();
            selectNode = (DefaultMutableTreeNode)vertex.getObject();
        }
        else{ selectNode = (DefaultMutableTreeNode)nodes[0];}

        this.append(selectNode.toString());
        this.append((selectNode.getChildCount()==0)
                                    ?" (Command/Observation)\n":"\n");

        for(Enumeration children = selectNode.children(); children.hasMoreElements();){

            DefaultMutableTreeNode child = (DefaultMutableTreeNode)children.nextElement();
            this.append("    "+child.toString()+"\n");
        }
     }
  }



  /*
   *    Class MyCellRenderer
   *        Overrides the CellRenderer for JLIST
   *        MyCellRenderer uses different icons to indicate supporting clauses,
   *        attribute assignments, root nodes, and assumptions
   *
   */
  private class MyCellRenderer extends JLabel implements ListCellRenderer {


     // This is the only method defined by ListCellRenderer.
     // We just reconfigure the JLabel each time we're called.

     private boolean isSelected = false;
     private int TextOffset = 20;

     public MyCellRenderer(){ super(); }

     /* Method getListCellRendererComponent
      *     Build the Graphical representation for each list value
      *     based on its selection status and whether it supports
      *     another proposition.
      *
      *     Graphs and Trees present different APIs, therefore each must
      *     be considered as a separate case.
      */
     public Component getListCellRendererComponent(
       JList list,
       Object value,            // value to display
       int index,               // cell index
       boolean isSelected,      // is the cell selected
       boolean cellHasFocus)    // the list and the cell have the focus

     {
         boolean isLeafNode =false;
         boolean isRootNode =false;
         boolean isSupportClauseProp =false;
         String s=null;
        try{
        //Note: Encapsulation of Graph Objects
        //VisualVertex contains an instance of VertexImpl
        //VertexImpl contains an instance of DefaultMutableTreeNode
        //DefaultMutableTreeNode contains an instance of Proposition

         if(ModelDisplayMode.equals(GraphMode)){

            MyVisualGraph vgraph = (MyVisualGraph)treeMap.get(ModelDisplayMode+ModelDisplayDetailLevel);
            Object nodeObjects[] = vgraph.getMappedValues(index);

            if(nodeObjects ==null) {return null;}

            //Get LastPathComponent (similiar to a JTree)
            VisualVertex selectVisualVertex = vgraph.getSelectedVertex();

            VertexImpl selectVertex = (selectVisualVertex==null)?null:(VertexImpl)selectVisualVertex.getVertex();
            DefaultMutableTreeNode selectNode= (selectVertex==null)?null:(DefaultMutableTreeNode)selectVertex.getObject();
            Proposition selectProp = (selectNode == null)?null:(Proposition)selectNode.getUserObject();

            VisualVertex rootVisualVertex = vgraph.getRoot();
            VertexImpl rootVertex = (VertexImpl)rootVisualVertex.getVertex();
            DefaultMutableTreeNode rootNode = (DefaultMutableTreeNode)(rootVertex.getObject());
            Proposition rootProp = (rootNode ==null)?null:(Proposition)rootNode.getUserObject();

            if(isSelected || selectProp ==null) {isSupportClauseProp = false;}
            else {

                for(int i= 0; !isSupportClauseProp && (i < nodeObjects.length); i++){

                    VisualVertex currentVisualVertex = (VisualVertex)nodeObjects[i];
                    VisualVertex parentVisualVertex = vgraph.getParent(currentVisualVertex);

                    VertexImpl parentVertex = parentVisualVertex==null?null:(VertexImpl)parentVisualVertex.getVertex();
                    DefaultMutableTreeNode  parentNode = parentVertex==null? null : (DefaultMutableTreeNode)parentVertex.getObject();
                    Proposition parentProp = parentNode == null? null : (Proposition)parentNode.getUserObject();
                    isSupportClauseProp = (parentProp==null)?false: parentProp.equals(selectProp);

                }
            }

            VisualVertex currentVisualVertex = (VisualVertex)nodeObjects[0];
            VertexImpl currentVertex = (VertexImpl)currentVisualVertex.getVertex();
            DefaultMutableTreeNode currentNode = (DefaultMutableTreeNode)currentVertex.getObject();
            Proposition currentProp = (Proposition)currentNode.getUserObject();

            isRootNode = rootProp==null? false : rootProp.equals(currentProp);

            isLeafNode = vgraph.isLeaf((VisualVertex)nodeObjects[0]);
         }
         else if (ModelDisplayMode.equals(TreeMode)){

            MyJTree tree = (MyJTree)treeMap.get(ModelDisplayMode+ModelDisplayDetailLevel);

            Object nodeObjects[] = tree.getMappedValues(index);

            TreePath selectPath = tree.getSelectionPath();
            DefaultMutableTreeNode selectNode = (selectPath==null)?null:(DefaultMutableTreeNode)selectPath.getLastPathComponent();
            Proposition selectProp = (selectNode ==  null)?null:(Proposition)selectNode.getUserObject();

            Object rootProp = ((DefaultMutableTreeNode)tree.getModel().getRoot()).getUserObject();

            if(isSelected) { isSupportClauseProp = false; }
            else{
                for(int i=0; !isSupportClauseProp && (i < nodeObjects.length); i++){
                    DefaultMutableTreeNode node = (DefaultMutableTreeNode)nodeObjects[i];
                    DefaultMutableTreeNode parent = (DefaultMutableTreeNode)node.getParent();
                    isSupportClauseProp = (parent==null?false:parent.getUserObject().equals(selectProp));
                }
            }

            //An assumption will be a leaf node. The same assumption can be used L2
            //in different locations of the inference tree
            isLeafNode = ((DefaultMutableTreeNode)nodeObjects[0]).getChildCount() == 0;

            //The proposition to explain can only occur once in the inference tree.(At the Root)
            isRootNode = ((DefaultMutableTreeNode)nodeObjects[0]).getUserObject().equals( rootProp );

         }

         s =  value.toString();

         if(isRootNode) setIcon(rootIcon);
         else if (isSupportClauseProp) setIcon(clausePropIcon);
         else if(isLeafNode) setIcon(leafIcon);
         else setIcon(propIcon);

         setText(index +")  "+s);

         if(this.isSelected =isSelected){
           setForeground(Color.white);
         }else{
           setForeground(Color.black);
         }

         setEnabled(list.isEnabled());
	 setFont(list.getFont());

         }catch(Exception e){e.printStackTrace();}
         return this;
     }


    /*  Method: paint
     *      This method is overridden so that the ICONs background is not painted.
     *
     */
    public void paint(Graphics g) {
	Color            bColor = getParent().getBackground();
	Icon             currentI = getIcon();

        if(currentI != null) setIconTextGap(TextOffset-currentI.getIconWidth());

	if(isSelected)
	    bColor = Color.black;


	g.setColor(bColor);

	if(currentI != null && getText() != null) {
	    int offset = (currentI.getIconWidth() + getIconTextGap());

            //GetWidth() returns the width of the longest list entry ,
            //Therefore, a selected item is painted to that maximum instead
            //of the length of the current list entry
	    g.fillRect(offset, 0, getWidth()  - offset-2,getHeight()-1 );
	}
	super.paint(g);
    }
 }




/*  Class PropositionMap
 *     A collection that maps a proposition string to set of logically equivalent
 *     Object instances. For this implementation, Different nodes in a JTree or Graph
 *     represent the same proposition.  Assumptions can occur in multiple
 *     locations in an inference tree.
 *
 */

  class PropositionMap extends TreeMap{

    private HashMap indexMap = new HashMap();

    private int listIndex = 0;

    public PropositionMap() {super();}

    public void clear(){
        listIndex = 0;
        indexMap.clear();
        super.clear();
    }

    public Object[] getOrderedValues(){

        Set keys = this.getKeySet();
        Object objects[] = new Object[keys.size()];
        Iterator iterator = keys.iterator();

        for(int i =0; i < objects.length; i++){
            Integer key = (Integer)iterator.next();
            Object values[] = this.getMappedValues(key.intValue());
            objects[i] = values[0];
        }

        return objects;
    }
    public int getMappedIndex(String value){

        Integer index = (Integer)indexMap.get(value);
        return  ( index==null?-1:index.intValue() );
    }


    public Object[] getMappedValues(int index) {

        Integer key = new Integer(index);

        Vector v = (Vector)this.get(key);
        if(v == null) { return new Object[0];}

        Object nodes[]= new Object[v.size()];

        for(int i = 0 ; i < v.size(); i++)
            nodes[i] = v.elementAt(i);

        return (nodes);
    }


    public Set getKeySet() { return this.keySet(); }


    public void put(Object object){

        Integer index = null;

        if((index= (Integer)indexMap.get(object.toString())) == null){
            index = new Integer(listIndex++);
            this.put(index, new Vector());
            indexMap.put(object.toString(),index);
        }

        Vector v = (Vector)this.get(index);

        v.addElement(object);
    }

  }




/*  Class MyJlist
 *      JList with  Tree Selection and Graph Selection listening
 *      capabilities.  Whenever the state of the currently displayed tree changes
 *      (could be a OUTLINE or GRAPH tree), it must also update to the corresponding state.
 *
 *
 */
  class MyJList extends JList implements PropertyChangeListener,ListSelectionListener{

    private PropertyChangeSupport propertyChangeSupport = new PropertyChangeSupport(this);

    private boolean ignore = false;


    public MyJList() {super();}

    public  MyJList(DefaultListModel list) {
        super (list);
        setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        this.addListSelectionListener(this);
    }



    public void addPropertyListener(PropertyChangeListener l){

        propertyChangeSupport.addPropertyChangeListener(l);
    }



    public void removePropertyListener(PropertyChangeListener l){

        propertyChangeSupport.removePropertyChangeListener(l);
    }



    /*  method valueChanged
     *      fires a PropertyChangeEvent whenever the JList selection changes.
     *      Though there are several JTrees and VisualGraphs listening.
     *      Each decides whether the event corresponds to its tree/graph
     *
     *      Note: if the JList is in transition to a new Mode, several ListSelection
     *      events may be generated.  The PropertyChangeListeners only need to
     *      know when the JList is in its new Mode. Therefore, only one is PropertyChange event
     *      will be generated.
     */

    public void valueChanged(ListSelectionEvent e){

        //Check if JList is still in transition to a new Mode
        if(ignore || e.getValueIsAdjusting()) return;

        ignore = true;

        Integer index = new Integer(this.getSelectedIndex());
        propertyChangeSupport.firePropertyChange("selection",null,index.toString());

       //Need to repaint JList. JList only repaints the selected entry.
       //But It needs to repaint the newly marked entries indicating
       //immediate support propositions(clause supporting the selected attribute assignment).
        this.repaint();
        ignore = false;
    }


    public void clear(){

        ignore = true;
        this.clearSelection();
        ((DefaultListModel) this.getModel()).clear();
        ignore= false;
    }




    public void setModelFromOrderedList(Object objects[]){

        DefaultListModel model = (DefaultListModel)this.getModel();

        model.clear();

        if(objects == null) return;

        for(int i=0; i < objects.length; i++){
            model.addElement(objects[i].toString());
        }
    }





    /*  Method: propertyChange
     *       Listens for Graph/JTree Selection events
     */
    public void propertyChange(PropertyChangeEvent e){

        if(ignore) return;
        ignore = true;

        int selectedIndex = -1;

        if(e.getSource() instanceof MyVisualGraph){
            MyVisualGraph vgraph = (MyVisualGraph)e.getSource();
            selectedIndex = vgraph.getSelectedIndex();


        }
        else if(e.getSource() instanceof MyJTree){
            MyJTree tree = (MyJTree)e.getSource();
            selectedIndex = tree.getSelectedIndex();
        }

        if(selectedIndex == -1) clearSelection();
        else this.setSelectedIndex(selectedIndex);

        ignore = false;
        this.repaint();
    }


    public void setSelectionIndex(int index){
        ignore =true;
        this.setSelectedIndex(index);
        ignore = false;
    }

  }





/*  class MyJtree
 *      JTree with JList and JTree selection capabilities.
 *      Updates it's VIEW by listening for events from a corresponding JList
 */

 private class MyJTree extends JTree implements PropertyChangeListener,
                                                PropositionListMap,
                                                TreeSelectionListener{

    private boolean ignore = false;

    private String listenKey =null;

    private PropertyChangeSupport propertyChangeSupport = new PropertyChangeSupport(this);

    private PropositionMap propMap = null;

    public MyJTree(TreeModel newModel) {

        super(newModel);
        setUI((TreeUI)UIManager.getUI(this));
        setCellRenderer(new MyTreeCellRenderer());
        addTreeSelectionListener(this);
        buildPropMap();
    }



    public void buildPropMap(){

        propMap = new PropositionMap();

        DefaultMutableTreeNode parent = (DefaultMutableTreeNode)this.getModel().getRoot();

        Enumeration nodes = parent.breadthFirstEnumeration();

        while(nodes.hasMoreElements()){

            propMap.put(nodes.nextElement());
         }
    }


    public Object[] getOrderedValues(){
        return this.propMap.getOrderedValues();
    }


    public Object[] getMappedValues(int index){
        return this.propMap.getMappedValues(index);
    }


    public int getSelectedIndex(){

        TreePath path = this.getSelectionPath();

        DefaultMutableTreeNode selectNode = (path == null)? null
                             :(DefaultMutableTreeNode)path.getLastPathComponent();

        return this.getMappedIndex(selectNode);

    }


    public int getMappedIndex(Object value){

        return (value == null)? -1
                    :propMap.getMappedIndex(value.toString());
    }

    public void setModel(TreeModel newModel){

        super.setModel(newModel);
        buildPropMap();
    }


    /*  Method: setListenKey
     *      Sets the ListenKey. The ListenKey is just a unique identifier
     *      for the JTree.  When it is listening for events from the JLIST, it
     *      must distinguish which events are meant for this JTree.  It uses
     *      the key(which is composed of the ModelDisplayType & ModelDisplayDetailLevel)
     *      to determine if it should listen or ignore an event.
     */
    public void setListenKey(String key){
        this.listenKey = key;
    }



    public void addPropertyListener(PropertyChangeListener l){

        propertyChangeSupport.addPropertyChangeListener(l);
    }



    public void removePropertyListener(PropertyChangeListener l){

        propertyChangeSupport.removePropertyChangeListener(l);
    }



    /*  Method getUIClassID
     *      Uniquely identifies this class to the UIManager
     */
    public String getUIClassID(){ return "MyJTree"; }



    //When A new selection is made by user, the GUI must be
    //updated in order to correctly show the highlighted path from the
    //root to the selected node.
    public void valueChanged(TreeSelectionEvent e) {

        if(ignore) { return; }

        TreePath selectPath = this.getSelectionPath();

        Object object = (selectPath==null)?null:selectPath.getLastPathComponent();

        propertyChangeSupport.firePropertyChange(
                            new PropertyChangeEvent(this, "selection", "", object));


        this.repaint();
    }



    /*  Method: valueChanged
     *      Update the tree view when the JList's view changes
     */
    public void propertyChange(PropertyChangeEvent e){

        //does this JList event correspond to this JTree?
        if(!listenKey.equals(ModelDisplayMode+ModelDisplayDetailLevel)) {return;}

        ignore = true;

        MyJList list = (MyJList)e.getSource();

        //Selection was Toggled Off
        clearSelection();

        selectAllMatchingNodes(list.getSelectedIndex());

        ignore = false;

        this.repaint();
    }



    public void selectAllMatchingNodes(int selectedIndex){

        if(selectedIndex == -1) return;

        TreePath rootToSelectedNodePath = null;

        Object selectedNodes[] = propMap.getMappedValues(selectedIndex);

        int minDepth = Integer.MAX_VALUE;

        TreePath path = null;

        for(int i = selectedNodes.length-1; i >= 0; i--){

            TreeNode []nodes = ((DefaultTreeModel)this.getModel()).getPathToRoot((TreeNode)selectedNodes[i]);
            path = new TreePath(nodes);
            if(nodes.length < minDepth){
                rootToSelectedNodePath = path;
            }
            this.addSelectionPath(path);
        }
        this.setSelectionPath(rootToSelectedNodePath);
    }

}


/*  Class MyTreeCellRenderer
 *      Represents the Visual representation of node in the JTree
 *      Each node is displayed according to its role in the inference.
 *      The node can be a proposition to explain (root node), an assumption,
 *      or a member attribute assigment of a supporting clause.
 */
  private class MyTreeCellRenderer extends JLabel implements TreeCellRenderer{

    /** Font used if the string to be displayed isn't a font. */
    protected Font             defaultFont = null;


    /** Color to use for the background when selected. */
    protected final Color SelectedBackgroundColor = Color.black;

   /** Whether or not the item that was last configured is selected. */
    protected boolean            selected;

   /** Is this tree node represent an Identical proposition as the
    *  currently select proposition */
    protected boolean           isRelated;

    protected boolean           hasFocus;

    public MyTreeCellRenderer()
    {

	try {
	    defaultFont = new Font("SansSerif", 0, 12);
	} catch (Exception e) {}

    }



    /** Method getTreeCellRendererComponent
      *  This is messaged from JTree whenever it needs to get the size
      *  of the component or it wants to draw it.
      *
      */
    public Component getTreeCellRendererComponent(JTree tree, Object value,
					  boolean selected, boolean expanded,
					  boolean leaf, int row,

                                          boolean hasFocus) {

	Font    font;
        Object  selectedPathNodes[]=null;
        boolean containedInSelectedPath = false;
	String  stringValue = tree.convertValueToText(value, selected,
					   expanded, leaf, row, hasFocus);

        MyJTree myjtree = (MyJTree)tree;
        DefaultMutableTreeNode currentNode = (DefaultMutableTreeNode)value;
        DefaultTreeSelectionModel model = (DefaultTreeSelectionModel) tree.getSelectionModel();
        isRelated = false;
        //Highlight all ancestors of selected node
        if(model.getSelectionPath() !=null) {
            selectedPathNodes = model.getSelectionPath().getPath();

            for(int i =0; i < selectedPathNodes.length && !containedInSelectedPath;i++)
                containedInSelectedPath = (currentNode == selectedPathNodes[i]);

            if(!containedInSelectedPath){
                Object selectNode = selectedPathNodes[selectedPathNodes.length-1];
                int selectIndex = myjtree.getMappedIndex(selectNode.toString());
                Object selectNodes[] = myjtree.getMappedValues(selectIndex);

                for(int i=0; !isRelated && i < selectNodes.length; i++)
                    isRelated = (selectNodes[i]==currentNode);
            }
        }

	/* Set the text. */
	setText(stringValue);
	/* Tooltips used by the tree. */
	setToolTipText(stringValue);

	/* Set the image. */
	if(leaf)
	    setIcon(leafIcon);
	else
	    setIcon(null);


	Proposition     userObject = (Proposition)((DefaultMutableTreeNode)value)
	                                .getUserObject();

        if(selected || isRelated)
            setForeground(Color.white);
        else if(containedInSelectedPath)
            setForeground(Color.red);
	else
	    setForeground(Color.black);

	 setFont(defaultFont);


	/* Update the selected flag for the next paint. */
	this.selected = selected;

	return this;
    }



    /**  Method paint
      *     paint is subclassed to draw the background correctly.  JLabel
      *     currently does not allow backgrounds other than white, and it
      *     will also fill behind the icon.  Something that isn't desirable.
      */
    public void paint(Graphics g) {
	Color            bColor;
	Icon             currentI = getIcon();

	if(selected || isRelated)
	    bColor = SelectedBackgroundColor;
	else if(getParent() != null)
	    /* Pick background color up from parent (which will come from
	       the JTree we're contained in). */
	    bColor = getParent().getBackground();
	else
	    bColor = getBackground();

	g.setColor(bColor);

	if(currentI != null && getText() != null) {
	    int offset = (currentI.getIconWidth() + getIconTextGap());

	    g.fillRect(offset, 0, getWidth()  - offset,getHeight() );
	}
	else
	   g.fillRect(0, 0, getWidth()-1, getHeight()-1);

	super.paint(g);
    }
  }





public class MyVisualGraph extends VisualGraph implements PropertyChangeListener,
                                                          PropositionListMap{

    //Current subgraph that is highlighted
    private Graph subGraph = null;

    private String listenKey = null;

    private PropositionMap propMap = new PropositionMap();;

    private Vector selectedSet = new Vector();

    private PropertyChangeSupport propertyChange = null;

    private Color defaultColor = new Color(0,225,255);

    private VisualVertex currentSelectVertex = null;

    private VisualEdgePainter edgeHighLighter = new AlternateVisualEdgePainter();

    private boolean ignore = false;

    public MyVisualGraph() {super(); propertyChange = new PropertyChangeSupport(this);}


    public MyVisualGraph(Graph graph) {
        super(graph);
        propertyChange = new PropertyChangeSupport(this);
        buildPropMap();
        updateVisualGraph();
    }


    private void updateVisualGraph(){

        Iterator iterator = propMap.getKeySet().iterator();

        while(iterator.hasNext()){
            Integer index = (Integer)iterator.next();

            Object vv[]=propMap.getMappedValues(index.intValue());

            for(int i =0; i < vv.length; i++){
                ((VisualVertex)vv[i]).setLabel(index.toString());
            }
        }

        this.layout();
    }


    private void buildPropMap(){

        Enumeration vertices = this.breadthFirstSearch();

        while(vertices.hasMoreElements()){
            propMap.put(vertices.nextElement());
        }
    }


    public Object[] getOrderedValues(){
        return this.propMap.getOrderedValues();
    }


    public Object[] getMappedValues(int index){
        return this.propMap.getMappedValues(index);
    }

    public int getSelectedIndex(){

        int index = (currentSelectVertex== null)? -1
                    :this.propMap.getMappedIndex(currentSelectVertex.toString());

        return index;
    }


    public VisualVertex getSelectedVertex() {return currentSelectVertex;}


    public VisualVertex getParent(VisualVertex visualVertex){
        Vertex parent = null;
        TreeImpl tree = (TreeImpl)this.getGraph();
        Vertex child =  visualVertex.getVertex();

        try{parent = tree.getParent(child); }
        catch(GraphException e) {e.printStackTrace();}

        return (parent==null ?null: this.getVisualVertex(parent));
    }


    public boolean isLeaf(VisualVertex visualVertex){
        boolean isleaf = false;

        TreeImpl tree = (TreeImpl)this.getGraph();

        try{ isleaf = tree.isLeaf(visualVertex.getVertex()); }
        catch(GraphException e) {e.printStackTrace();}

        return isleaf;
    }


    public VisualVertex getRoot(){

        TreeImpl tree = (TreeImpl)this.getGraph();

        return (tree== null? null : this.getVisualVertex(tree.getRoot()));
    }





    public void clearSelection() {this.setSelectedVertex((VisualVertex)null);}


    public void selectRelatedVertices(VisualVertex vertex, Color highLightColor){

        if(vertex == null) return;

        int index = propMap.getMappedIndex(vertex.toString());
        Object vertices[] = propMap.getMappedValues(index);

        for(int i=0; i < vertices.length; i++){
            ((VisualVertex)vertices[i]).setFillcolor(highLightColor);
        }
    }


    public void setSelectedVertex(VisualVertex newVertex) {

        selectRelatedVertices(currentSelectVertex, defaultColor);

         //Remove HighLight from path
        this.emphasizeSubGraph(subGraph,null,null);

        subGraph=null;

        if(newVertex == currentSelectVertex || newVertex==null) {
            currentSelectVertex = null;
        }
        else{
            currentSelectVertex = newVertex;

            selectRelatedVertices(currentSelectVertex, Color.red);

            highLightSelectionPath();
        }

        propertyChange.firePropertyChange(new PropertyChangeEvent(this,"graph","",currentSelectVertex));

        repaint();

    }


    public void addVisualGraphListener(PropertyChangeListener listener){

        propertyChange.addPropertyChangeListener(listener);
    }


    public void removeVisualGraphListener(PropertyChangeListener listener){

        propertyChange.removePropertyChangeListener(listener);
    }


    public Enumeration breadthFirstSearch(){

        Vector v = new Vector();

        TreeImpl tree = (TreeImpl)this.getGraph();

        BreadthFirstTraversal traversal = new BreadthFirstTraversal(tree);

        tree.setTraversal(traversal);

        List list = tree.traverse(tree.getRoot());

        Iterator iterator = list.iterator();

        while(iterator.hasNext()){

            v.add( this.getVisualVertex((Vertex)iterator.next()) );
        }

        return ( v.elements() );
    }



    public void highLightSelectionPath(){

        if(currentSelectVertex == null) { return; }

        TreeImpl tree = (TreeImpl)this.getGraph();

        try{
            subGraph = tree.getRootToVertexPath(currentSelectVertex.getVertex());
        }catch(Exception error) {error.printStackTrace();}

        this.emphasizeSubGraph(subGraph,null,null,null,edgeHighLighter);
    }



  public void propertyChange(PropertyChangeEvent e){

    if(ignore) return;

    if(!listenKey.equals(ModelDisplayMode+ModelDisplayDetailLevel)) {return;}

    ignore = true;

    MyJList list = (MyJList)e.getSource();

    int selectedIndex = list.getSelectedIndex();

    Object visualVertex[] = propMap.getMappedValues(selectedIndex);

    this.setSelectedVertex(selectedIndex==-1?null:(VisualVertex)visualVertex[0]);

    ignore =false;
  }


  public void setListenKey(String key){

      this.listenKey = key;
  }


  public String getListenKey() { return this.listenKey; }


  public class AlternateVisualEdgePainter extends VisualEdgePainterImpl {

        private transient BasicStroke origStroke = null;
        private transient BasicStroke fatStroke;


        public void paint( VisualGraphComponent component, Graphics2D g2d ) {

            Color originalColor = component.getOutlinecolor();
            component.setOutlinecolor( Color.red );

            // Set the stroke for edges in thesubgraph to be double the stroke
            // of the default stroke.
            if( this.origStroke == null ) {

                this.origStroke = new BasicStroke();
                this.fatStroke = new BasicStroke( origStroke.getLineWidth() * 2 );
            }
            g2d.setStroke( this.fatStroke );
            super.paint( component, g2d );

            // Restore the stroke to the default
            g2d.setStroke( this.origStroke );
            component.setOutlinecolor( originalColor );
        }
  }
}



public class MyGraphPanelState extends GraphPanelNormalState {

    public MyGraphPanelState( GraphPanel gpanel ) {
        super( gpanel );
    }

    public MyGraphPanelState() {
        this(null);
    }


     public GraphPanelState mousePressed(MouseEvent e) {return this;}

     public GraphPanelState mouseReleased(MouseEvent e) {return this;}

     public GraphPanelState mouseClicked(MouseEvent e){

        this.previous_x = e.getX();
        this.previous_y = e.getY();

        targetvertex = gpanel.getVisualGraph().getNode(this.previous_x, this.previous_y );

        if(targetvertex==null) return this;
        if(gpanel.getVisualGraph() instanceof MyVisualGraph){

            ((MyVisualGraph)gpanel.getVisualGraph()).setSelectedVertex(targetvertex);
        }
        // Notify the VisualGraphComponent of the event
        informTargetVisualGraphComponentOfMouseEvent(e);
        return this;

     }
}

    public interface PropositionListMap{

        public int getSelectedIndex();
        public Object[] getOrderedValues();
        public Object[] getMappedValues(int key);

    }

}
