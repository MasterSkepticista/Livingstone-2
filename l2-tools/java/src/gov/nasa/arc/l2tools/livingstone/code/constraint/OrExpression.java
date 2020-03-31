//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:39:22 AM)
 * @author: Andrew Bachmann
 */
public class OrExpression extends Expression {
	public final static String tagName = "ex:or";
  // compilation caches
	private javax.swing.DefaultListModel expressions = new javax.swing.DefaultListModel();
/**
 * OrExpression constructor comment.
 */
public OrExpression() {
	super(tagName);
}
/**
 * OrExpression constructor comment.
 * @param name java.lang.String
 */
public OrExpression(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/13/2000 3:41:22 PM)
 * @author: Andrew Bachmann
 * 
 * @param expression gov.nasa.arc.l2tools.livingstone.code.constraint.Expression
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheExpression(Expression expression) throws gov.nasa.arc.l2tools.Exception {
	expression.cache();
	getExpressions().addElement(expression);
}
/**
 *
 * Creation date: (3/24/2000 5:22:29 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getExpressions() {
	return expressions;
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		java.util.Enumeration children = getChildren().elements();
		while (children.hasMoreElements()) {
			java.lang.Object child = children.nextElement();
			if (child instanceof Expression) {
				cacheExpression((Expression)child);
			} else {
				//String errorString="unexpected child error: a child should be an expression";
				//throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
			}
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in an or expression");
		throw error;
	}
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	try {
		setExpression(new gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression());
		java.util.Enumeration expressions=getExpressions().elements();
		while (expressions.hasMoreElements()) {
			java.lang.Object expression=expressions.nextElement();
			if (expression instanceof Expression) {
				((Expression)expression).compile();
				setExpression(getExpression().destructiveDisjunction(((Expression)expression).getExpression()));
			} else {
				String errorString="compiler error: expressions are corrupted";
				throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
			}
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in an and expression");
		throw error;
	}
}
/**
 *
 * Creation date: (3/24/2000 5:22:29 PM)
 * @author: Andrew Bachmann
 * 
 * @param newExpressions javax.swing.DefaultListModel
 */
protected void setExpressions(javax.swing.DefaultListModel newExpressions) {
	expressions = newExpressions;
}
}
