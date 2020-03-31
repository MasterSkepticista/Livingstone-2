//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Creation date: (3/10/2000 2:24:14 PM)
 * @author: Andrew Bachmann
 */
public class Transition extends gov.nasa.arc.l2tools.livingstone.componentinstance.Particle {
	public final static String tagName = "ci:transition";
	private Component component = null;
	private gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression expandedAndInstantiatedExpression = null;
/**
 * ComponentClause constructor comment.
 */
public Transition() {
	super(tagName);
}
/**
 * Creation date: (4/8/2000 7:00:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
public Transition(Component component) {
	this();
	setComponent(component);
	component.getChildren().addElement(this);
}
/**
 * ComponentClause constructor comment.
 * @param name java.lang.String
 */
public Transition(String name) {
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
 * Creation date: (5/31/2000 9:23:57 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression
 */
public gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression getExpandedAndInstantiatedExpression() {
	return expandedAndInstantiatedExpression;
}
/**
 * search for a Transition with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Transition
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Transition search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (Transition)object.search(Transition.class,key,value);
}
/**
 * search for a Transition named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Transition
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Transition searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Transition)object.searchByName(Transition.class,name);
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
 * Creation date: (5/31/2000 9:23:57 AM)
 * @author: Andrew Bachmann
 * 
 * @param newExpandedAndInstantiatedExpression gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression
 */
public void setExpandedAndInstantiatedExpression(gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression newExpandedAndInstantiatedExpression) {
	expandedAndInstantiatedExpression = newExpandedAndInstantiatedExpression;
}
}
