//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/5/2000 2:20:47 PM)
 * @author: Andrew Bachmann
 */
public abstract class NaryExpression extends BasicExpression {
	private java.util.Vector subExpressions = new java.util.Vector();
/**
 * NaryExpression constructor comment.
 */
public NaryExpression() {
	super();
}
/**
 * OR((AND/OR X Y Z),TRUE/FALSE) => OR(TRUE/FALSE,(AND/OR X Y Z))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(BooleanExpression expression) {
	return expression.distribute(this);
}
/**
 * OR((AND/OR X Y Z),X) => OR(X,(AND/OR X Y Z))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(EqualityExpression expression) {
	return expression.distribute(this);
}
/**
 * OR((AND/OR X Y Z),(NOT X)) => OR((NOT X),(AND/OR X Y Z))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(NotExpression expression) {
	return expression.distribute(this);
}
/**
 * Creation date: (4/5/2000 2:19:14 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.util.Vector
 */
public java.util.Vector getSubExpressions() {
	return subExpressions;
}
/**
 * Creation date: (4/6/2000 3:37:43 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.NaryExpression
 * @param expression gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
public NaryExpression merge(BasicExpression expression) {
	getSubExpressions().addElement(expression);
	return this;
}
/**
 *
 * This is not a complete de morgan, because de morgan's law on an expression
 * will not only introduce negations in terms and change the operator or/and
 * but will also introduce a negation to encapsulate the whole.  That's why
 * it is "notDeMorgan" instead of "deMorgan"
 *
 * Creation date: (4/6/2000 11:07:31 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.NaryExpression
 */
protected abstract NaryExpression notDeMorgan();
/**
 * pushNegationIn method comment.
 */
protected BasicExpression pushNegationIn() throws NegationPushFailure {
	return notDeMorgan();
}
/**
 * pushNegationsIn method comment.
 */
protected BasicExpression pushNegationsIn() {
	for (int i=0;(i<getSubExpressions().size());i++) {
		BasicExpression expression=(BasicExpression)getSubExpressions().elementAt(i);
		getSubExpressions().setElementAt(expression.pushNegationsIn(),i);
	}
	return this;
}
/**
 * Creation date: (4/5/2000 2:19:14 PM)
 * @author: Andrew Bachmann
 * 
 * @param newSubExpressions java.util.Vector
 */
protected void setSubExpressions(java.util.Vector newSubExpressions) {
	subExpressions = newSubExpressions;
}
}
