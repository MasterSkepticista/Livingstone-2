//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.tools;

// Exception thrown when an attempt is made to invoke
// stanleyViewer commands and there is no Stanley Viewer
public class StanleyViewerExistsError extends Exception {
  public StanleyViewerExistsError() { super(); }
  public StanleyViewerExistsError(String str) { super(str); }
}
