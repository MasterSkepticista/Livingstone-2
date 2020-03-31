//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/6/2000 11:23:07 AM)
 * @author: Andrew Bachmann
 */
public abstract class AbstractTransition extends gov.nasa.arc.l2tools.livingstone.code.CodeParticle
implements gov.nasa.arc.l2tools.livingstone.code.Instantiable {
	public final static String tagName = "ct:transition";
	private java.lang.String name = null;
	private javax.swing.DefaultListModel fromStates = new javax.swing.DefaultListModel();
	private javax.swing.DefaultListModel toStates = new javax.swing.DefaultListModel();
	private javax.swing.DefaultListModel statements = new javax.swing.DefaultListModel();
	private gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression expandedAndInstantiatedExpression = null;
/**
 * CT_Transition constructor comment.
 */
public AbstractTransition() {
	super(tagName);
}
/**
 * CT_Transition constructor comment.
 * @param name java.lang.String
 */
public AbstractTransition(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/13/2000 3:58:16 PM)
 * @author: Andrew Bachmann
 * 
 * @param group gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheStatementGroup(gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup group) 
	throws gov.nasa.arc.l2tools.Exception {
	group.cache();

	java.util.Enumeration statements=group.getStatements().elements();
	while (statements.hasMoreElements()) {
		getStatements().addElement(statements.nextElement());
	}
}
/**
 *
 * Creation date: (3/13/2000 3:41:22 PM)
 * @author: Andrew Bachmann
 * 
 * @param statement gov.nasa.arc.l2tools.livingstone.code.constraint.Statement
 */
private void compileStatement(gov.nasa.arc.l2tools.livingstone.code.constraint.Statement statement) 
  throws gov.nasa.arc.l2tools.Exception {
	statement.compile();
	if (statement.getExpression()!=null) {
		getExpression().destructiveConjunction(statement.getExpression());
	}
}
/**
 * Creation date: (5/31/2000 9:23:57 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression
 */
public gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression getExpandedAndInstantiatedExpression() {
	return expandedAndInstantiatedExpression;
}
/**
 *
 * Creation date: (3/23/2000 7:13:33 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getFromStates() {
	return fromStates;
}
/**
 *
 * Creation date: (3/23/2000 7:10:28 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 *
 * Creation date: (3/23/2000 7:25:04 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getStatements() {
	return statements;
}
/**
 *
 * Creation date: (3/23/2000 7:15:01 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getToStates() {
	return toStates;
}
/**
 * Creation date: (4/8/2000 9:55:20 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Transition
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Transition instantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	// compile ourselves
	compile();
  // make instantiated class
	gov.nasa.arc.l2tools.livingstone.componentinstance.Transition transition
	  = new gov.nasa.arc.l2tools.livingstone.componentinstance.Transition(component);
	// cross check with state vector
	StateVector stateVector=component.getComponentType().getStateVector();
	if (stateVector==null) {
		gov.nasa.arc.l2tools.livingstone.SyntaxError error
		  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: transition found in a context without a state vector");
		throw error;
	}
	java.util.Enumeration states;
	// name
	if ((getName()!=null) && (getName()!="")) {
		transition.set("name",getName());
	}
	// from states
	String from=null;
	java.util.Enumeration fromStates=getFromStates().elements();
	states=stateVector.getAttributes().elements();
	while (fromStates.hasMoreElements() && states.hasMoreElements()) {
		String fromState=(String)fromStates.nextElement();
		if (!fromState.equals("*")) {
			String stateVar=(String)states.nextElement();
			gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute attribute=component.findAttribute(stateVar);
			if (attribute==null) {
				gov.nasa.arc.l2tools.livingstone.code.CompilerError error
				  = new gov.nasa.arc.l2tools.livingstone.code.CompilerError("compiler error: state variable attribute not found in component");
				throw error;
			}		
			java.util.Enumeration values=attribute.getAbstractAttributeType().getValues().elements();
			boolean found=false;
			while (values.hasMoreElements()) {
				if (fromState.equals(((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)values.nextElement()).getName())) {
					// the value is a possible value
					found=true;
					break;
				}
			}
			if (found==false) {
				gov.nasa.arc.l2tools.livingstone.SyntaxError error
				  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: transition 'from' state is not in the domain of the state variable");
				error.concatMessage("\n  the value '"+fromState+"'");
				error.concatMessage("\n  for state variable '"+stateVar+"'");
				if (attribute.getAbstractAttributeType().getCompiledFlag()) {
					error.concatMessage(" of type '"+attribute.getAbstractAttributeType().getName()+"'");
				}
				throw error;
			}
		}
		if (from==null) from=fromState;
		else from+=" "+fromState;
	}
	if (fromStates.hasMoreElements()) {
		gov.nasa.arc.l2tools.livingstone.SyntaxError error
		  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: transition has more 'from' states than the state vector");
		throw error;
	}
	transition.set("from",from);
	// to states
	String to=null;
	java.util.Enumeration toStates=getToStates().elements();
	states=stateVector.getAttributes().elements();
	while (toStates.hasMoreElements() && states.hasMoreElements()) {
		String toState=(String)toStates.nextElement();
		if (!toState.equals("+")) {
			String stateVar=(String)states.nextElement();
			gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute attribute=component.findAttribute(stateVar);
			if (attribute==null) {
				gov.nasa.arc.l2tools.livingstone.code.CompilerError error
				  = new gov.nasa.arc.l2tools.livingstone.code.CompilerError("compiler error: state variable attribute not found in component");
				throw error;
			}
			java.util.Enumeration values=attribute.getAbstractAttributeType().getValues().elements();
			boolean found=false;
			while (values.hasMoreElements()) {
				if (toState.equals(((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)values.nextElement()).getName())) {
					// the value is a possible value
					found=true;
					break;
				}
			}
			if (found==false) {
				gov.nasa.arc.l2tools.livingstone.SyntaxError error
				  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: transition 'to' state is not in the domain of the state variable");
				error.concatMessage("\n  the value '"+toState+"'");
				error.concatMessage("\n  for state variable '"+stateVar+"'");
				if (attribute.getAbstractAttributeType().getCompiledFlag()) {
					error.concatMessage(" of type '"+attribute.getAbstractAttributeType().getName()+"'");
				}
				throw error;
			}
		}
		if (to==null) to=toState;
		else to+=" "+toState;
	}
	if (toStates.hasMoreElements()) {
		gov.nasa.arc.l2tools.livingstone.SyntaxError error
		  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: transition has more 'to' states than the state vector");
		throw error;
	}
	transition.set("to",to);
	// expression
	if (getExpression()!=null) {
		getExpression().instantiate(transition);
		// cache the negated expression for the frame axioms
		setExpandedAndInstantiatedExpression(transition.getExpandedAndInstantiatedExpression());
	}
	return transition;
}
/**
 * refreshCache method comment.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	// cache attributes
	if ((get("name")!=null) && (get("name")!="")) {
		setName(get("name"));
	}
	if ((get("from")!=null) && (get("from")!="")) {
		java.util.StringTokenizer fromTokenizer=new java.util.StringTokenizer(get("from"));
		while (fromTokenizer.hasMoreTokens()) {
			getFromStates().addElement(fromTokenizer.nextToken());
		}
	}
	if ((get("to")!=null) && (get("to")!="")) {
		java.util.StringTokenizer toTokenizer=new java.util.StringTokenizer(get("to"));
		while (toTokenizer.hasMoreTokens()) {
			getToStates().addElement(toTokenizer.nextToken());
		}
	}
	// cache children
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object child=children.nextElement();
		if (child instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup) {
			cacheStatementGroup((gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup)child);
		} else {
			//String errorString="unexpected child error: a child should be a statement group";
			//throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
		}
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration statements=getStatements().elements();
	while (statements.hasMoreElements()) {
		java.lang.Object statement=statements.nextElement();
		if (statement instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.Statement) {
			compileStatement((gov.nasa.arc.l2tools.livingstone.code.constraint.Statement)statement);
		} else {
			String errorString="compiler error: statements are corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
		}
	}
}



/**
 * Creation date: (5/31/2000 9:23:57 AM)
 * @author: Andrew Bachmann
 * 
 * @param newExpandedAndInstantiatedExpression gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression
 */
public void setExpandedAndInstantiatedExpression(gov.nasa.arc.l2tools.livingstone.code.logic.BasicExpression newExpandedAndInstantiatedExpression) {
	expandedAndInstantiatedExpression = newExpandedAndInstantiatedExpression;
}
/**
 *
 * Creation date: (3/23/2000 7:13:33 PM)
 * @author: Andrew Bachmann
 * 
 * @param newFromStates javax.swing.DefaultListModel
 */
protected void setFromStates(javax.swing.DefaultListModel newFromStates) {
	fromStates = newFromStates;
}
/**
 *
 * Creation date: (3/23/2000 7:10:28 PM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
}
/**
 *
 * Creation date: (3/23/2000 7:25:04 PM)
 * @author: Andrew Bachmann
 * 
 * @param newStatements javax.swing.DefaultListModel
 */
protected void setStatements(javax.swing.DefaultListModel newStatements) {
	statements = newStatements;
}
/**
 *
 * Creation date: (3/23/2000 7:15:01 PM)
 * @author: Andrew Bachmann
 * 
 * @param newToStates javax.swing.DefaultListModel
 */
protected void setToStates(javax.swing.DefaultListModel newToStates) {
	toStates = newToStates;
}
}
