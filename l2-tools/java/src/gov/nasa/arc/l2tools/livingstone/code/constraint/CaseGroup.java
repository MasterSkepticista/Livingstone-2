//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:38:11 AM)
 * @author: Andrew Bachmann
 */
public class CaseGroup extends Statement {
	public final static String tagName = "ex:case";
  // compilation caches
  private java.lang.String key;
	private javax.swing.DefaultListModel statements = new javax.swing.DefaultListModel();
	private javax.swing.DefaultListModel names = new javax.swing.DefaultListModel();
/**
 * CaseGroup constructor comment.
 */
public CaseGroup() {
	super(tagName);
}
/**
 * CaseGroup constructor comment.
 * @param name java.lang.String
 */
public CaseGroup(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/13/2000 3:41:22 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheStatement(Statement statement) throws gov.nasa.arc.l2tools.Exception {
	statement.cache();	
	getStatements().addElement(statement);
}
/**
 *
 * Creation date: (3/13/2000 3:41:22 PM)
 * @author: Andrew Bachmann
 * 
 * @param statement gov.nasa.arc.l2tools.livingstone.Exception
 */
private void compileStatement(Statement statement) throws gov.nasa.arc.l2tools.Exception {
	statement.compile();
	java.util.Enumeration names=getNames().elements();
	while (names.hasMoreElements()) {
		java.lang.Object name=names.nextElement();
		if (name instanceof String) {
			gov.nasa.arc.l2tools.livingstone.code.logic.EqualityExpression equality
			  = new gov.nasa.arc.l2tools.livingstone.code.logic.EqualityExpression();
			equality.setArgument1(getKey());
			equality.setArgument2((String)name);
			gov.nasa.arc.l2tools.livingstone.code.logic.NotExpression not
			  = new gov.nasa.arc.l2tools.livingstone.code.logic.NotExpression();
			not.setExpression(equality);
			gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression or
			  = new gov.nasa.arc.l2tools.livingstone.code.logic.OrExpression();
			or.destructiveDisjunction(not);
			or.destructiveDisjunction(statement.getExpression());
		  setExpression(getExpression().destructiveConjunction(or));
		}
	}
}
/**
 *
 * Creation date: (3/14/2000 2:55:37 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getKey() {
	return key;
}
/**
 *
 * Creation date: (3/23/2000 12:03:37 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getNames() {
	return names;
}
/**
 *
 * Creation date: (3/23/2000 10:13:55 AM)
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
		// structural check
		if (getParent()==null) {
			String errorString="cache error: no parent for case group";
			throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
		}
		// cache parent attribute
		if (getParent().get("key")==null) {
			String errorString2="cache error: case group found parent switch without key";
			throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString2);
		}
		setKey(getParent().get("key"));
		// cache attributes
		if (get("names")==null) {
			String errorString="cache error: case group found without names";
			throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
		}
		java.util.StringTokenizer nameTokenizer=new java.util.StringTokenizer(get("names"));
		while (nameTokenizer.hasMoreTokens()) {
			String nameToken=nameTokenizer.nextToken();
			getNames().addElement(nameToken);
		}
		// cache children
		java.util.Enumeration children = getChildren().elements();
		while (children.hasMoreElements()) {
			java.lang.Object child = children.nextElement();
			if (child instanceof Statement) {
				cacheStatement((Statement)child);
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
		error.concatMessage("in a case group");
		if (getKey()!=null) error.concatMessage(" with key '"+getKey()+"'");
		if (getNames()!=null) error.concatMessage(" with names: "+getNames().toString());
		throw error;
	}
}	
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	setExpression(new gov.nasa.arc.l2tools.livingstone.code.logic.AndExpression());
	// compile statements
	java.util.Enumeration statements=getStatements().elements();
	while (statements.hasMoreElements()) {
		java.lang.Object statement=statements.nextElement();
		if (statement instanceof Statement) {
			compileStatement((Statement)statement);
		} else {
			String errorString="compiler error: statements corrupted in case group";
			throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
		}
	}
}
/**
 *
 * Creation date: (3/14/2000 2:55:37 PM)
 * @author: Andrew Bachmann
 * 
 * @param newKey java.lang.String
 */
private void setKey(java.lang.String newKey) {
	key = newKey;
}
/**
 *
 * Creation date: (3/23/2000 12:03:37 PM)
 * @author: Andrew Bachmann
 * 
 * @param newNames javax.swing.DefaultListModel
 */
private void setNames(javax.swing.DefaultListModel newNames) {
	names = newNames;
}
/**
 *
 * Creation date: (3/23/2000 10:13:55 AM)
 * @author: Andrew Bachmann
 * 
 * @param newStatements javax.swing.DefaultListModel
 */
private void setStatements(javax.swing.DefaultListModel newStatements) {
	statements = newStatements;
}
}
