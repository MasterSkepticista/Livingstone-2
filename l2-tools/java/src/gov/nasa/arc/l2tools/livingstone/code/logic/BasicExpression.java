//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/6/2000 11:25:46 AM)
 * @author: Andrew Bachmann
 */
public abstract class BasicExpression extends LogicExpression {
/**
 * BasicExpression constructor comment.
 */
public BasicExpression() {
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
public abstract gov.nasa.arc.l2tools.livingstone.componentinstance.Clause buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clause clause) 
throws gov.nasa.arc.l2tools.Exception;
/**
 * Creation date: (4/8/2000 9:48:33 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public abstract gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses clauses) 
throws gov.nasa.arc.l2tools.Exception;
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(BasicExpression expression) {
	throw new RuntimeException("unimplemented distribution operation between:\n  "+toString()+"\n  "+expression.toString());
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected abstract BasicExpression flatten();
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected void flatten(AndExpression result) {
	result.getSubExpressions().addElement(flatten());
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected void flatten(OrExpression result) {
	result.getSubExpressions().addElement(flatten());
}
/**
 * Creation date: (4/8/2000 9:48:33 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.DefaultTransition
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void instantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.DefaultTransition transition) 
throws gov.nasa.arc.l2tools.Exception {
	pushNegationsIn().pushDisjunctionsIn().buildInstance(new gov.nasa.arc.l2tools.livingstone.componentinstance.Clauses(transition));
}
/**
 * Creation date: (4/5/2000 4:47:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected abstract BasicExpression pushDisjunctionsIn();
/**
 * Creation date: (4/5/2000 4:47:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected abstract BasicExpression pushNegationIn() throws NegationPushFailure ;
/**
 * Creation date: (4/5/2000 4:47:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
protected abstract BasicExpression pushNegationsIn();
/**
 * Like your usual conjunctive normal form, except that you might get back a simple term
 * or an and expression with the possibility of terms as direct subexpressions
 *
 * Creation date: (4/5/2000 4:21:34 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 */
public BasicExpression toShortConjunctiveNormalForm() {
	return pushNegationsIn().pushDisjunctionsIn();
}
}
