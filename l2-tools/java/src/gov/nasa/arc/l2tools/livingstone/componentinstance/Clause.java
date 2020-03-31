//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Insert the type's description here.
 * Creation date: (3/13/2000 3:44:38 PM)
 * @author: Andrew Bachmann
 */
public class Clause extends gov.nasa.arc.l2tools.livingstone.componentinstance.Particle {
	public final static String tagName = "ci:clause";
	private Component component = null;
	private Clauses clauses = null;
/**
 * Clause constructor comment.
 */
public Clause() {
	super(tagName);
}
/**
 * Creation date: (5/23/2000 11:47:24 AM)
 * @author: Andrew Bachmann
 * 
 * @param clauses gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 */
public Clause(Clauses clauses) {
	this();
	setClauses(clauses);
	if (getClauses().getTransition()!=null) {
		getClauses().getTransition().getChildren().addElement(this);
	} else if (getClauses().getComponent()!=null) {
		getClauses().getComponent().getChildren().addElement(this);
	}
}
/**
 * Clause constructor comment.
 * @param name java.lang.String
 */
public Clause(String name) {
	super(name);
}
/**
 * Creation date: (4/8/2000 9:46:24 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 */
public Clauses getClauses() {
	return clauses;
}
/**
 * Creation date: (4/8/2000 9:46:24 PM)
 * @author: Andrew Bachmann
 * 
 * @param newClauses gov.nasa.arc.l2tools.livingstone.componentinstance.ComponentClauses
 */
protected void setClauses(Clauses newClauses) {
	clauses = newClauses;
}
/**
 * Extremely useful toString method to help with debugging
 * Shows element name, attributes, and their values
 * Creation date: (2/25/2000 10:54:23 AM)
 * @author: Andrew Bachmann
 */
public String toString() {
	String result = new String("");
	result+="(";
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		EqualityTerm term=(EqualityTerm)children.nextElement();
		if (!term.getSign()) result+="~";
		result+=term.getString();
		if (children.hasMoreElements())
			result+=" v ";
	}
	result+=")";
	return result;
}
}
