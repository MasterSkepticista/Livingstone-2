//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Creation date: (4/8/2000 6:10:36 PM)
 * @author: Andrew Bachmann
 */
public class Clauses {
	private Component component = null;
	private Transition transition = null;
/**
 * Creation date: (4/8/2000 7:00:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
public Clauses(Component component) {
	setComponent(component);
}
/**
 * Creation date: (4/8/2000 7:00:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param transition gov.nasa.arc.l2tools.livingstone.componentinstance.Transition
 */
public Clauses(Transition transition) {
	this(transition.getComponent());
	setTransition(transition);
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
 * Creation date: (4/8/2000 6:11:00 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Transition
 */
public Transition getTransition() {
	return transition;
}
/**
 * Creation date: (4/8/2000 11:55:22 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 */
public Clause makeClause() {
	return new Clause(this);
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
 * Creation date: (4/8/2000 6:11:00 PM)
 * @author: Andrew Bachmann
 * 
 * @param newTransition gov.nasa.arc.l2tools.livingstone.transitioninstance.Transition
 */
protected void setTransition(Transition newTransition) {
	transition = newTransition;
}
}
