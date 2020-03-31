//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools.historyTable;

/* Exception thrown an attempt is made to access an engine through a
   DataModelState and the state is no longer current */
public class DataModelStateError extends Exception {
  public DataModelStateError() { super(); }
  public DataModelStateError(String str) { super(str); }
}
