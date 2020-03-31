//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/4/2000 4:11:56 PM)
 * @author: Andrew Bachmann
 */
public class NotExpression extends UnaryExpression {
	private LogicExpression expression;
/**
 * NotExpression constructor comment.
 */
public NotExpression() {
	super();
}
/**
 * Creation date: (4/8/2000 11:59:21 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 * @param clause gov.nasa.arc.l2tools.livingstone.componentinstance.Clause
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Clause buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clause clause) 
throws gov.nasa.arc.l2tools.Exception {
	EqualityExpression expression=(EqualityExpression)getExpression();
	gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm term
	  = new gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm(clause);
	expression.buildInstance(term);
	term.setSign(false);
	return clause;
}
/**
 * copy method comment.
 */
public LogicExpression copy() {
	NotExpression newNot = new NotExpression();
	newNot.setExpression(getExpression().copy());
	return newNot;
}
/**
 * negation method comment.
 */
public LogicExpression destructiveNegation() {
	return getExpression();
}
/**
 * OR((NOT A),(AND X (NOT Y) (OR Z Q)) => (AND (OR (NOT A) X) (OR (NOT A) (NOT Y)) (OR (NOT A) (OR Z Q)))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(AndExpression expression) {
	AndExpression and=new AndExpression();
	java.util.Enumeration subExpressions=expression.getSubExpressions().elements();
	while (subExpressions.hasMoreElements()) {
		BasicExpression subExpression=(BasicExpression)subExpressions.nextElement();
		OrExpression or=new OrExpression();
		or.getSubExpressions().addElement(this);
		or.getSubExpressions().addElement(subExpression);
		and.getSubExpressions().addElement(or);
	}
	return and;
}
/**
 * OR((NOT A),(OR B (NOT C) (AND D Y)))=>(OR (NOT A) B (NOT C) (AND D Y))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(OrExpression expression) {
	BasicExpression or=new OrExpression();
	java.util.Enumeration subExpressions=expression.getSubExpressions().elements();
	while (subExpressions.hasMoreElements()) {
		BasicExpression subExpression=(BasicExpression)subExpressions.nextElement();
		((OrExpression)or).getSubExpressions().addElement(subExpression);
	}
	or=((OrExpression)or).merge(this);
	return or;
}
/**
 * Creation date: (4/8/2000 10:40:55 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 * @param gov.nasa.arc.l2tools.livingstone.componentinstance.Component component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
protected BasicExpression expandAndInstantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	NotExpression not=new NotExpression();
	not.setExpression(getExpression().expandAndInstantiate(component));
	return not;
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression flatten() {
	return this;
}
/**
 * Creation date: (4/5/2000 2:14:24 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.LogicExpression
 */
public LogicExpression getExpression() {
	return expression;
}
/**
 * negation method comment.
 */
public LogicExpression negation() {
	return getExpression().copy();
}
/**
 * pushNegationIn method comment.
 */
protected BasicExpression pushNegationIn() throws NegationPushFailure {
	return (BasicExpression)getExpression();
}
/**
 * pushNegationsIn method comment.
 */
protected BasicExpression pushNegationsIn() {
	try {
		// let's try to push this negation in
		return ((BasicExpression)getExpression()).pushNegationIn().pushNegationsIn();
	} catch (NegationPushFailure x) {
		// didn't work, it's as far in as it goes
		return this;
	}
}
/**
 * Creation date: (4/5/2000 2:14:24 PM)
 * @author: Andrew Bachmann
 * 
 * @param newExpression gov.nasa.arc.l2tools.livingstone.logic.LogicExpression
 */
public void setExpression(LogicExpression newExpression) {
	expression = newExpression;
}
/**
 * substitute method comment.
 */
public LogicExpression substitute(gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner)
throws gov.nasa.arc.l2tools.Exception {
	NotExpression not=new NotExpression();
	not.setExpression(getExpression().substitute(relation,values,relationOwner));
	return not;
}
/**
 * Creation date: (4/5/2000 4:02:35 PM)
 * @author: Andrew Bachmann
 * 
 */
public String toString() {
	if (getExpression()==null)
		return "~???";
	else
		return "~"+getExpression().toString();
}
}
