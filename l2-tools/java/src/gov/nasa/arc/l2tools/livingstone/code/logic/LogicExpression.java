//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/5/2000 2:10:46 PM)
 * @author: Andrew Bachmann
 */
public abstract class LogicExpression implements gov.nasa.arc.l2tools.livingstone.code.Instantiable {
	private java.lang.Integer line = null;
/**
 * AbstractLogicExpression constructor comment.
 */
public LogicExpression() {
	super();
}
/**
 * destructiveConjunction method comment.
 */
public LogicExpression conjunction(FalseExpression expression) {
	return expression;
}
/**
 * conjunction method comment.
 */
public LogicExpression conjunction(LogicExpression expression) {
	AndExpression and = new AndExpression();
	and.getSubExpressions().addElement(this);
	and.getSubExpressions().addElement(expression);
	return and;
}
/**
 * copy method comment.
 */
public abstract LogicExpression copy();
/**
 * destructiveConjunction method comment.
 */
public LogicExpression destructiveConjunction(LogicExpression expression) {
	return conjunction(expression);
}
/**
 * conjunction method comment.
 */
public LogicExpression destructiveDisjunction(LogicExpression expression) {
	return disjunction(expression);
}
/**
 * negation method comment.
 */
public LogicExpression destructiveNegation() {
	return negation();
}
/**
 * conjunction method comment.
 */
public LogicExpression disjunction(LogicExpression expression) {
	OrExpression or = new OrExpression();
	or.getSubExpressions().addElement(this);
	or.getSubExpressions().addElement(expression);
	return or;
}
/**
 * conjunction method comment.
 */
public LogicExpression disjunction(TrueExpression expression) {
	return expression;
}
/**
 * Creation date: (4/8/2000 10:40:55 PM)
 * @author: Andrew Bachmann
 *
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
protected abstract BasicExpression expandAndInstantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception;
/**
 * Creation date: (6/28/2000 3:36:11 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.Integer
 */
public java.lang.Integer getLine() {
	return line;
}
/**
 * Creation date: (4/8/2000 9:48:33 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void instantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
  try {
	  expandAndInstantiate(component).pushNegationsIn().pushDisjunctionsIn().buildInstance(new gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses(component));
  } catch (gov.nasa.arc.l2tools.Exception e) {
	  e.concatMessage("\n  ");
		if (getLine()!=null) {
			e.concatMessage("line "+getLine()+": ");
		}
		e.concatMessage("while instantiating an expression");
	  throw e;
  }
}
/**
 * Creation date: (4/8/2000 9:48:33 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Transition
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void instantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Transition transition) 
throws gov.nasa.arc.l2tools.Exception {
	try {
		BasicExpression expression=expandAndInstantiate(transition.getComponent());
		transition.setExpandedAndInstantiatedExpression(expression);
		((BasicExpression)expression.negation()).pushNegationsIn().pushDisjunctionsIn().buildInstance(new gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses(transition));
  } catch (gov.nasa.arc.l2tools.Exception e) {
	  e.concatMessage("\n  ");
		if (getLine()!=null) {
			e.concatMessage("line "+getLine()+": ");
		}
		e.concatMessage("while instantiating an expression");
	  throw e;
  }
}
/**
 * negation method comment.
 */
public LogicExpression negation() {
	NotExpression not = new NotExpression();
	not.setExpression(this);
	return not;
}
/**
 * Creation date: (6/28/2000 3:36:11 PM)
 * @author: Andrew Bachmann
 * 
 * @param newLine java.lang.Integer
 */
public void setLine(java.lang.Integer newLine) {
	line = newLine;
}
/**
 * substitute method comment.
 */
public abstract LogicExpression substitute(gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner) 
throws gov.nasa.arc.l2tools.Exception;
}
