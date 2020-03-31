//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:40:02 AM)
 * @author: Andrew Bachmann
 */
public class NegationExpression extends Expression {
	public final static String tagName = "ex:not";
	private Expression subexpression = null;
/**
 * NegationExpression constructor comment.
 */
public NegationExpression() {
	super(tagName);
}
/**
 * NegationExpression constructor comment.
 * @param name java.lang.String
 */
public NegationExpression(String name) {
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
	setSubExpression(expression);
}
/**
 *
 * Creation date: (3/24/2000 5:35:45 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.constraint.Expression
 */
public Expression getSubExpression() {
	return subexpression;
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
		if (getChildren().size() != 1) {
			String errorString="unexpected child error: expression should have exactly one child";
			throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
		}
		java.lang.Object expression=getChildren().firstElement();
		if (expression instanceof Expression) {
			cacheExpression((Expression)expression);
		} else {
			String errorString2="unexpected child error: the child should be an expression";
			throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString2);
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a negation expression");
		throw error;
	}
}
/**
 * refreshExpression method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	try {
		getSubExpression().compile();

		gov.nasa.arc.l2tools.livingstone.code.logic.NotExpression not
		  = new gov.nasa.arc.l2tools.livingstone.code.logic.NotExpression();
		not.setExpression(getSubExpression().getExpression());
		setExpression(not);
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a negation expression");
		throw error;
	}
}
/**
 *
 * Creation date: (3/24/2000 5:35:45 PM)
 * @author: Andrew Bachmann
 * 
 * @param newSubExpression gov.nasa.arc.l2tools.livingstone.code.constraint.Expression
 */
protected void setSubExpression(Expression newSubExpression) {
	subexpression = newSubExpression;
}
}
