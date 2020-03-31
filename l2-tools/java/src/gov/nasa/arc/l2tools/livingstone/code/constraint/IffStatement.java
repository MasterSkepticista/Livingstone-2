//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:35:41 AM)
 * @author: Andrew Bachmann
 *
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public class IffStatement extends AbstractIfStatement {
	public final static String tagName = "ex:iff";
/**
 * IffStatement constructor comment.
 */
public IffStatement() {
	super(tagName);
}
/**
 * IffStatement constructor comment.
 * @param name java.lang.String
 */
public IffStatement(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		if (getChildren().size() > 2) {
			String errorString2="unexpected child error: statement must have only a 'test' expression and a 'then' statement";
			errorString2+="\n  statement has too many children";
			throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString2);
		}
		// cache children
		super.refreshCache();
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in an iff statement");
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
		
		// iff statements are equivalent to two ifs
		// do the other if now: iff A then B => if B then A => B implies A => ~B or A
		// then join the old clauses and the new clauses together
		gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression or
		  = new gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression();
		or.destructiveDisjunction(getThen().getExpression().negation());
		or.destructiveDisjunction(getTest().getExpression());
		setExpression(getExpression().destructiveConjunction(or));
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in an iff statement");
		throw error;
	}
}
}
