//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools.historyTable;

/* Exception when createNodes finds no nodes */
public class JustificationCreateNodesError extends Exception {
  public JustificationCreateNodesError() { super(); }
  public JustificationCreateNodesError(String str) { super(str); }
}
