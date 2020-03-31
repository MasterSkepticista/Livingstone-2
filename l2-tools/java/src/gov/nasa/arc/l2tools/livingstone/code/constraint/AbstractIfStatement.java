//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/23/2000 8:00:29 PM)
 * @author: Andrew Bachmann
 */
public abstract class AbstractIfStatement extends Statement {
  // compilation caches
	private Expression test = null;
	private Statement then = null;
/**
 * AbstractIfStatement constructor comment.
 */
public AbstractIfStatement() {
	super();
}
/**
 * AbstractIfStatement constructor comment.
 * @param name java.lang.String
 */
public AbstractIfStatement(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/23/2000 11:47:48 AM)
 * @author: Andrew Bachmann
 * 
 * @param expression gov.nasa.arc.l2tools.livingstone.code.constraint.Expression
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheTest(Expression expression) throws gov.nasa.arc.l2tools.Exception {
	expression.cache();
	setTest(expression);
}
/**
 *
 * Creation date: (3/23/2000 11:47:48 AM)
 * @author: Andrew Bachmann
 * 
 * @param expression gov.nasa.arc.l2tools.livingstone.code.constraint.Expression
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheThen(Statement statement) throws gov.nasa.arc.l2tools.Exception {
	statement.cache();
	setThen(statement);
}
/**
 *
 * Creation date: (3/23/2000 11:44:27 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.constraint.Expression
 */
public Expression getTest() {
	return test;
}
/**
 *
 * Creation date: (3/23/2000 11:48:32 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.constraint.Statement
 */
public Statement getThen() {
	return then;
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	if (getChildren().size() < 2) {
		String errorString="unexpected child error: statement must have a 'test' expression and a 'then' statement";
		errorString+="\n  statement has too few children";
		throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
	}
	// cache children
	java.lang.Object object;
	object=getChildren().elementAt(0);
	if (object instanceof Expression) {
		cacheTest((Expression)object);
	} else {
		String errorString2="unexpected child error: statement must have a 'test' expression as the first child.";
		throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString2);
	}
	object=getChildren().elementAt(1);
	if (object instanceof Statement) {
		cacheThen((Statement)object);
	} else {
		String errorString3="unexpected child error: statement must have a 'then' statement as the second child.";
		throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString3);
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	// if A then B => A implies B => ~A or B
	setExpression(new gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression());

	// test
	getTest().compile();
	setExpression(getExpression().destructiveDisjunction(getTest().getExpression().negation()));

	// then
	getThen().compile();
	setExpression(getExpression().destructiveDisjunction(getThen().getExpression()));
}
/**
 *
 * Creation date: (3/23/2000 11:44:27 AM)
 * @author: Andrew Bachmann
 * 
 * @param newTest gov.nasa.arc.l2tools.livingstone.constraint.Expression
 */
protected void setTest(Expression newTest) {
	test = newTest;
}
/**
 *
 * Creation date: (3/23/2000 11:48:32 AM)
 * @author: Andrew Bachmann
 * 
 * @param newThen gov.nasa.arc.l2tools.livingstone.code.constraint.Statement
 */
protected void setThen(Statement newThen) {
	then = newThen;
}
}
