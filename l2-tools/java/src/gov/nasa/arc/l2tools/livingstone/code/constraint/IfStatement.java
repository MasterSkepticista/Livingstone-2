//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:35:34 AM)
 * @author: Andrew Bachmann
 *
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public class IfStatement extends AbstractIfStatement {
	public final static String tagName = "ex:if";
  // compilation caches
	private Statement elseS = null;
/**
 * IfStatement constructor comment.
 */
public IfStatement() {
	super(tagName);
}
/**
 * IfStatement constructor comment.
 * @param name java.lang.String
 */
public IfStatement(String name) {
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
private void cacheElse(Statement statement) throws gov.nasa.arc.l2tools.Exception {
	statement.cache();
	setElse(statement);
}
/**
 *
 * Creation date: (3/23/2000 11:48:32 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.constraint.Statement
 */
public Statement getElse() {
	return elseS;
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		if (getChildren().size() > 3) {
			String errorString="unexpected child error: statement may have a 'test' expression, 'then' statement, and 'else statement'";
			errorString+="\n  statement has too many children";
			throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
		}
		// cache children
		super.refreshCache();
		// cache else part if it exists
		if (getChildren().size()==3) {
			java.lang.Object object=getChildren().elementAt(2);
			if (object instanceof Statement) {
				cacheElse((Statement)object);
			} else {
				String errorString2="unexpected child error: statement may only have an 'else' expression as the third child.";
				throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString2);
			}
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in an if statement");
		throw error;
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
 	try {
		// do the usual if part
		super.refreshExpression();	
		
		// ifs with else statements are equivalent to two ifs
		// do the other if now: if A then B else C => if ~A then C => A or C
		// then join the old clauses and the new clauses together
		if (getElse()!=null) {
			getElse().compile();
			gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression or
			  = new gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression();
			or.destructiveDisjunction(getTest().getExpression());
			or.destructiveDisjunction(getElse().getExpression());
			setExpression(getExpression().destructiveConjunction(or));
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in an if statement");
		throw error;
	}
}
/**
 *
 * Creation date: (3/23/2000 11:48:32 AM)
 * @author: Andrew Bachmann
 * 
 * @param newElse gov.nasa.arc.l2tools.livingstone.code.constraint.Statement
 */
protected void setElse(Statement newElse) {
	elseS = newElse;
}
}
