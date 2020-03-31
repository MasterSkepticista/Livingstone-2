//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (5/1/2000 3:35:15 AM)
 * @author: Andrew Bachmann
 */
public class FalseExpression extends BooleanExpression {
/**
 * FalseExpression constructor comment.
 */
public FalseExpression() {
	super();
}
/**
 * destructiveConjunction method comment.
 */
public LogicExpression disjunction(LogicExpression expression) {
	return expression;
}
/**
 * OR(FALSE,(AND/OR X Y Z)) => (AND/OR X Y Z)
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(BasicExpression expression) {
	return expression;
}
/**
 * negation method comment.
 */
public LogicExpression negation() {
	return new TrueExpression();
}
}
