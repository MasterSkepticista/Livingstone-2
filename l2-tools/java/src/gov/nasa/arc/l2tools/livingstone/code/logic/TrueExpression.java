//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (5/1/2000 3:35:03 AM)
 * @author: Andrew Bachmann
 */
public class TrueExpression extends BooleanExpression {
/**
 * TrueExpression constructor comment.
 */
public TrueExpression() {
	super();
}
/**
 * destructiveConjunction method comment.
 */
public LogicExpression conjunction(LogicExpression expression) {
	return expression;
}
/**
 * OR(TRUE,(AND/OR X Y Z)) => TRUE
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(BasicExpression expression) {
	return this;
}
/**
 * negation method comment.
 */
public LogicExpression negation() {
	return new FalseExpression();
}
}
