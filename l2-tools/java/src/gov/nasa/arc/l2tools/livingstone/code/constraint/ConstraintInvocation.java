//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:41:20 AM)
 * @author: Andrew Bachmann
 */
public class ConstraintInvocation extends Expression {
	public final static String tagName = "ex:rel";
	private java.lang.String name = null;
	private javax.swing.DefaultListModel arguments = new javax.swing.DefaultListModel();
/**
 * ConstraintInvocation constructor comment.
 */
public ConstraintInvocation() {
	super(tagName);
}
/**
 * ConstraintInvocation constructor comment.
 * @param name java.lang.String
 */
public ConstraintInvocation(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/27/2000 1:53:53 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getArguments() {
	return arguments;
}
/**
 *
 * Creation date: (3/27/2000 1:53:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 * refreshCache method comment.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		// cache attributes
		if ((get("name")!=null) && (get("name")!="")) {
			setName(get("name"));
		} else {
			String errorString = "illegal anonymous error: each invocation must have a name";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
		}
		if ((get("arguments")!=null) && (get("arguments")!="")) {
			java.util.StringTokenizer argumentTokenizer=new java.util.StringTokenizer(get("arguments"));
			while (argumentTokenizer.hasMoreTokens()) {
				getArguments().addElement(argumentTokenizer.nextToken());
			}
		}
			
		// check for children
		if (!getChildren().isEmpty()) {
			System.out.println("Ignoring children of constraint invocation");
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a constraint invocation");
		if (getName()!=null) error.concatMessage(" of constraint '"+getName()+"'");
		throw error;
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	gov.nasa.arc.l2tools.livingstone.code.logic.RelationExpression relation
	  = new gov.nasa.arc.l2tools.livingstone.code.logic.RelationExpression();
	relation.setName(getName());
	java.util.Enumeration arguments=getArguments().elements();
	while (arguments.hasMoreElements()) {
		relation.getArguments().addElement(arguments.nextElement());
	}
	setExpression(relation);
}
/**
 *
 * Creation date: (3/27/2000 1:53:53 PM)
 * @author: Andrew Bachmann
 * 
 * @param newArguments javax.swing.DefaultListModel
 */
protected void setArguments(javax.swing.DefaultListModel newArguments) {
	arguments = newArguments;
}
/**
 *
 * Creation date: (3/27/2000 1:53:32 PM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
}
}
