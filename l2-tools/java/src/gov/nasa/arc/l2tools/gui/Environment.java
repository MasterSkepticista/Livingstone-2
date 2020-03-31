//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.gui;

/**
 * Creation date: (9/27/2000 5:30:38 PM)
 * @author: Andrew Bachmann
 */
public class Environment {
	private Boolean GUI=Boolean.TRUE;
/**
 * SkunkWorksEnvironment constructor comment.
 */
public Environment() {
	super();
}
/**
 * Creation date: (9/22/2000 4:09:54 PM)
 * @return Boolean
 */
public Boolean isGUI() {
	return GUI;
}
/**
 * Creation date: (9/22/2000 4:09:54 PM)
 * @param newGUI Boolean
 */
protected void setGUI(Boolean newGUI) {
	GUI = newGUI;
}
}
