//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Creation date: (4/4/2000 10:53:34 AM)
 * @author: Andrew Bachmann
 */
public class StateVector extends gov.nasa.arc.l2tools.livingstone.componentinstance.Particle  {
	public final static String tagName = "ci:statevector";
	private Component component = null;
	private int length = 0;
	private gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector stateVectorDeclaration = null;
/**
 * ComponentClause constructor comment.
 */
public StateVector() {
	super(tagName);
}
/**
 * Creation date: (4/8/2000 7:00:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
public StateVector(Component component) {
	this();
	setComponent(component);
	component.getChildren().insertElementAt(this,0);
}
/**
 * Creation date: (4/8/2000 7:00:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
public StateVector(Component component, gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector stateVector) {
	this();
	setComponent(component);
	setStateVectorDeclaration(stateVector);
	component.getChildren().insertElementAt(this,0);
}
/**
 * ComponentClause constructor comment.
 * @param name java.lang.String
 */
public StateVector(String name) {
	super(name);
}
/**
 * Creation date: (4/8/2000 6:11:00 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
public Component getComponent() {
	return component;
}
/**
 * Creation date: (4/4/2000 9:21:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return int
 */
public int getLength() {
	return length;
}
/**
 * Creation date: (4/8/2000 9:07:28 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector
 */
public gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector getStateVectorDeclaration() {
	return stateVectorDeclaration;
}
/**
 * Creation date: (4/8/2000 6:11:00 PM)
 * @author: Andrew Bachmann
 * 
 * @param newComponent gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
protected void setComponent(Component newComponent) {
	component = newComponent;
}
/**
 * Creation date: (4/4/2000 9:21:32 PM)
 * @author: Andrew Bachmann
 * 
 * @param newLength int
 */
public void setLength(int newLength) {
	length = newLength;
}
/**
 * Creation date: (4/8/2000 9:07:28 PM)
 * @author: Andrew Bachmann
 * 
 * @param newStateVectorDeclaration gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector
 */
protected void setStateVectorDeclaration(gov.nasa.arc.l2tools.livingstone.code.componenttype.StateVector newStateVectorDeclaration) {
	stateVectorDeclaration = newStateVectorDeclaration;
}
}
