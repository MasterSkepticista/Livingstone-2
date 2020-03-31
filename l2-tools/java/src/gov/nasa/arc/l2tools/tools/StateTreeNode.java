//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import java.util.StringTokenizer;
import java.util.Enumeration;
import java.util.Vector;

public class StateTreeNode extends DefaultMutableTreeNode {
  
  private static MyComparator comp = new MyComparator();
  private String value = null;
  private Vector ordered = new Vector();

  public StateTreeNode(Object obj, boolean hasChildren) { super(obj, hasChildren); }

  public String getName() { return getUserObject().toString(); }

  public void setValue(String v) { value = v; }

  public StateTreeNode addAssignment(DefaultTreeModel treeModel, StringTokenizer st) {
    String next = (String)st.nextElement();
    if(st.hasMoreElements()) {
      StateTreeNode branch = getBranch(treeModel, next);
      return branch.addAssignment(treeModel, st);
    } else {
      return makeLeaf(treeModel, next);
    }
  }

  public StateTreeNode getBranch(DefaultTreeModel treeModel, String name) {
    int index = java.util.Collections.binarySearch(ordered, name, comp);
    if(index < 0) {
      StateTreeNode branch = new StateTreeNode(name, true);
      treeModel.insertNodeInto(branch, this, -1-index);
      ordered.add(-1-index, branch.getName());
      return branch;
    } else {
      return (StateTreeNode)getChildAt(index);
    }
  }

  public StateTreeNode makeLeaf(DefaultTreeModel treeModel, String name) {
    String pair[] = LivingstoneEngine2.breakAtSubstring(name, "=");
    int index = java.util.Collections.binarySearch(ordered, pair[0], comp);
    if(index < 0) {
      StateTreeNode leaf = new StateTreeNode(pair[0], false);
      leaf.setValue(pair[1]);
      treeModel.nodeChanged(leaf);
      treeModel.insertNodeInto(leaf, this, -1-index);
      ordered.add(-1-index, leaf.getName());
      return leaf;
    } else {
      StateTreeNode leaf = (StateTreeNode)getChildAt(index);
      leaf.setValue(pair[1]);
      treeModel.nodeChanged(leaf);
      return leaf;
    }
  }

  public String toString() {
    if(value == null) return getUserObject().toString();
    else return getUserObject().toString() + "=" + value;
  }

  public void clearValues() {
    value = null;
    if(getAllowsChildren()) {
      for(Enumeration enum = children(); enum.hasMoreElements();) {
	StateTreeNode child = (StateTreeNode)enum.nextElement();
	child.clearValues();
      }
    }
  }

  public void removeAllChildren() {
    ordered = new Vector();
    super.removeAllChildren();
  }

}
