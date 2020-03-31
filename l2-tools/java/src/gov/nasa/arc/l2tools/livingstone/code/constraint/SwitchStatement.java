//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.constraint;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:35:53 AM)
 * @author: Andrew Bachmann
 */
public class SwitchStatement extends Statement {
	public final static String tagName = "ex:switch";
  // compilation caches
	private java.lang.String key;
	private javax.swing.DefaultListModel caseGroups = new javax.swing.DefaultListModel();
/**
 * SwitchStatement constructor comment.
 */
public SwitchStatement() {
	super(tagName);
}
/**
 * SwitchStatement constructor comment.
 * @param name java.lang.String
 */
public SwitchStatement(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/13/2000 4:10:30 PM)
 * @author: Andrew Bachmann
 * 
 * @param group gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheCaseGroup(CaseGroup caseGroup) throws gov.nasa.arc.l2tools.Exception {
	caseGroup.cache();
	getCaseGroups().addElement(caseGroup);
}
/**
 *
 * Creation date: (3/13/2000 4:10:30 PM)
 * @author: Andrew Bachmann
 * 
 * @param group gov.nasa.arc.l2tools.livingstone.code.constraint.CaseGroup
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void compileCaseGroup(CaseGroup caseGroup) 
  throws gov.nasa.arc.l2tools.Exception {
  caseGroup.compile();
	setExpression(getExpression().destructiveConjunction(caseGroup.getExpression()));
}
/**
 *
 * Creation date: (3/23/2000 9:59:20 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getCaseGroups() {
	return caseGroups;
}
/**
 *
 * Creation date: (3/13/2000 4:11:58 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getKey() {
	return key;
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
		// cache attributes
		if ((get("key") != null) && (get("key") != "")) {
			setKey(get("key"));
		} else {
			String errorString="syntax error: statement must have a key";
			throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
		}

		// cache children
		java.util.Enumeration children = getChildren().elements();
		while (children.hasMoreElements()) {
			java.lang.Object child = children.nextElement();
			if (child instanceof CaseGroup) {
				cacheCaseGroup(((CaseGroup)child));			
			} else {
				//String errorString="unexpected child error: switch statement has a child that is not a case group";
				//throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
			}
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a switch statement");
		if (getKey()!=null) error.concatMessage(" with a key '"+getKey()+"'");
		throw error;
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	try {
		setExpression(new gov.nasa.arc.l2tools.livingstone.code.logic.AndExpression());
		java.util.Enumeration caseGroups=getCaseGroups().elements();
		while (caseGroups.hasMoreElements()) {
			java.lang.Object caseGroup=caseGroups.nextElement();
			if (caseGroup instanceof CaseGroup) {
				compileCaseGroup((CaseGroup)caseGroup);
			} else {
				String errorString="compiler error: case groups are corrupted";
				throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
			}
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a switch statement");
		if (getKey()!=null) error.concatMessage(" with a key '"+getKey()+"'");
		throw error;
	}
}
/**
 *
 * Creation date: (3/23/2000 9:59:20 AM)
 * @author: Andrew Bachmann
 * 
 * @param newCaseGroups javax.swing.DefaultListModel
 */
protected void setCaseGroups(javax.swing.DefaultListModel newCaseGroups) {
	caseGroups = newCaseGroups;
}
/**
 *
 * Creation date: (3/13/2000 4:11:58 PM)
 * @author: Andrew Bachmann
 * 
 * @param newKey java.lang.String
 */
protected void setKey(java.lang.String newKey) {
	key = newKey;
}
}
