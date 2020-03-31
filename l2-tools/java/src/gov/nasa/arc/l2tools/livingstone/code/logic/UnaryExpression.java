//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/7/2000 11:39:21 AM)
 * @author: Andrew Bachmann
 */
public abstract class UnaryExpression extends BasicExpression {
/**
 * TermExpression constructor comment.
 */
public UnaryExpression() {
	super();
}
/**
 * Creation date: (4/8/2000 9:48:33 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.AbstractClauses
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses clauses) 
throws gov.nasa.arc.l2tools.Exception {
	gov.nasa.arc.l2tools.livingstone.componentinstance.Clause clause
	  = clauses.makeClause();
	buildInstance(clause);
	return clauses;
}
/**
 * OR(A/(NOT A),B/(NOT B))=>(OR A/(NOT A) B/(NOT B))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(UnaryExpression expression) {
	OrExpression or=new OrExpression();
	or.getSubExpressions().addElement(this);
	or.getSubExpressions().addElement(expression);
	return or;
}
/**
 * pushDisjunctionsIn method comment.
 */
protected BasicExpression pushDisjunctionsIn() {
	AndExpression and=new AndExpression();
	OrExpression or=new OrExpression();
	and.getSubExpressions().addElement(or);
	or.getSubExpressions().addElement(this);
	return and;
}
}
