//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:34:54 AM)
 * @author: Andrew Bachmann
 */
public class StatementGroup extends Statement {
	public final static String tagName = "ex:group";
  // compilation caches
	private javax.swing.DefaultListModel statements = new javax.swing.DefaultListModel();
/**
 * StatementGroup constructor comment.
 */
public StatementGroup() {
	super(tagName);
}
/**
 * StatementGroup constructor comment.
 * @param name java.lang.String
 */
public StatementGroup(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/13/2000 3:41:22 PM)
 * @author: Andrew Bachmann
 * 
 * @param statement gov.nasa.arc.l2tools.livingstone.code.constraint.Statement
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheStatement(Statement statement) throws gov.nasa.arc.l2tools.Exception {
	statement.cache();
	getStatements().addElement(statement);
}
/**
 *
 * Creation date: (3/23/2000 9:48:40 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getStatements() {
	return statements;
}
/**
 *
 * Creation date: (3/10/2000 5:31:14 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		java.util.Enumeration children = getChildren().elements();
		while (children.hasMoreElements()) {
			java.lang.Object child = children.nextElement();
			if (child instanceof Statement) {
				cacheStatement(((Statement)child));
			} else {
				//String errorString="unexpected child error: a child should be a statement";
				//throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
			}
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a statement group");
		throw error;
	}
}
/**
 * refreshExpression method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	try {
		setExpression(new gov.nasa.arc.l2tools.livingstone.code.logic.AndExpression());
		java.util.Enumeration statements=getStatements().elements();
		while (statements.hasMoreElements()) {
			java.lang.Object statement=statements.nextElement();
			if (statement instanceof Statement) {
				((Statement)statement).compile();
				setExpression(getExpression().destructiveConjunction(((Statement)statement).getExpression()));
			}
		}	
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a statement group");
		throw error;
	}
}
/**
 *
 * Creation date: (3/23/2000 9:48:40 AM)
 * @author: Andrew Bachmann
 * 
 * @param newStatements javax.swing.DefaultListModel
 */
protected void setStatements(javax.swing.DefaultListModel newStatements) {
	statements = newStatements;
}
}
