//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (5/1/2000 3:34:38 AM)
 * @author: Andrew Bachmann
 */
public abstract class BooleanExpression extends BasicExpression {
/**
 * TrueExpression constructor comment.
 */
public BooleanExpression() {
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
	return null;
}
/**
 * Creation date: (4/8/2000 9:48:33 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses clauses) 
throws gov.nasa.arc.l2tools.Exception {
	return null;
}
/**
 * copy method comment.
 */
public LogicExpression copy() {
	return this;
}
/**
 * OR(A,(AND X (NOT Y) (OR Z Q)) => (AND (OR A X) (OR A (NOT Y)) (OR A (OR Z Q)))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected abstract BasicExpression distribute(BasicExpression expression);
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
	return this;
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression flatten() {
	return this;
}
/**
 * Creation date: (4/5/2000 4:47:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected BasicExpression pushDisjunctionsIn() {
	return this;
}
/**
 * Creation date: (4/5/2000 4:47:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected BasicExpression pushNegationIn() throws NegationPushFailure {
	return this;
}
/**
 * Creation date: (4/5/2000 4:47:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected BasicExpression pushNegationsIn() {
	return this;
}
/**
 * substitute method comment.
 */
public LogicExpression substitute(gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner) 
throws gov.nasa.arc.l2tools.Exception {
	return this;
}
/**
 * substitute method comment.
 */
public LogicExpression substitute(javax.swing.DefaultListModel variables, javax.swing.DefaultListModel values, String relationOwner) 
throws gov.nasa.arc.l2tools.Exception {
	return this;
}
}
