//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code;

/**
 * Insert the type's description here.
 * Creation date: (3/23/2000 7:33:47 PM)
 * @author: Andrew Bachmann
 */
public abstract class AbstractRelation extends CodeParticle {
  // compilation caches
	private java.lang.String name;
	private javax.swing.DefaultListModel arguments = new javax.swing.DefaultListModel();
	private javax.swing.DefaultListModel statements = new javax.swing.DefaultListModel();
/**
 * Method constructor comment.
 */
public AbstractRelation() {
	super();
}
/**
 * Method constructor comment.
 * @param name java.lang.String
 */
public AbstractRelation(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/10/2000 6:32:19 PM)
 * @author: Andrew Bachmann
 * 
 */
private void cacheArgument(gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute argument) 
  throws gov.nasa.arc.l2tools.Exception {
	// cache itself
	argument.cache();

	// check for duplicate name
	java.util.Enumeration arguments=getArguments().elements();
	while (arguments.hasMoreElements()) {
		java.lang.Object oldArgument=arguments.nextElement();
		if (!(oldArgument instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)) {
			String errorString="cache error: arguments corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
		} else if (argument.getName()==((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)oldArgument).getName()) {
			String errorString="illegal duplicate error: an argument (attribute) may only occur once";
			errorString+="\n  '"+argument.getName()+"' occurs more than once";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
		}
	}

	// add to cache
	getArguments().addElement(argument);
}
/**
 *
 * Creation date: (3/10/2000 6:32:19 PM)
 * @author: Andrew Bachmann
 * 
 */
private void cacheStatementGroup(gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup group) 
  throws gov.nasa.arc.l2tools.Exception {
  // cache itself
	group.cache();

	// add each statement to one large list of constraints
	// (this munges the constraints from all the groups)
	java.util.Enumeration statements=group.getStatements().elements();
	while (statements.hasMoreElements()) {
		java.lang.Object statement=statements.nextElement();
		if (statement instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.Statement) {
			getStatements().addElement(statement);
		} else {
			String errorString="cache error: statements corrupted in statement group";
			throw new CacheError(errorString);
		}
	}
}
/**
 *
 * Creation date: (3/23/2000 9:39:22 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getArguments() {
	return arguments;
}
/**
 *
 * Creation date: (3/13/2000 3:54:04 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 *
 * Creation date: (3/23/2000 9:39:49 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getStatements() {
	return statements;
}
/**
 *
 * Creation date: (3/24/2000 2:14:48 PM)
 * @author: Andrew Bachmann
 * 
 * @return boolean
 * @param relation gov.nasa.arc.l2tools.livingstone.AbstractRelation
 */
public boolean prototypesEqual(AbstractRelation relation) {
	if (getName()==null) {
		if (relation.getName()!=null) {
			return false;
		}
	} else if (!(getName().equals(relation.getName()))) {
		return false;
	}
	// TODO: check argument list for disparity
	return true;
}
/**
 *
 * Creation date: (3/10/2000 7:09:36 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		// cache attributes
		if ((get("name") != null) && (get("name")!="")) {
			setName(get("name"));
		} 
		
		// cache children
		java.util.Enumeration children = getChildren().elements();
		while (children.hasMoreElements()) {
			java.lang.Object child = children.nextElement();
			if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
				cacheArgument((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)child);
			} else if (child instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup) {
				cacheStatementGroup((gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup)child);
			} else {
				//String errorString="unexpected child error: a child should be one of: attribute, statement group";
				//throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString);
			}
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n	 ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in abstract relation");
		if (getName()!=null) {
			error.concatMessage(": "+getName());
		}
		throw error;
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
			gov.nasa.arc.l2tools.livingstone.code.constraint.Statement st
			  = (gov.nasa.arc.l2tools.livingstone.code.constraint.Statement)statement;
			st.compile();
			if (getExpression()==null) {
				setExpression(st.getExpression().copy());
			} else {
				getExpression().destructiveConjunction(st.getExpression());
			}
		} else {
			String errorString="compiler error: statements are corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
		}
	}
}



/**
 *
 * Creation date: (3/23/2000 9:39:22 AM)
 * @author: Andrew Bachmann
 * 
 * @param newArguments javax.swing.DefaultListModel
 */
protected void setArguments(javax.swing.DefaultListModel newArguments) {
	arguments = newArguments;
}
/**
 *
 * Creation date: (3/13/2000 3:54:04 PM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
}
/**
 *
 * Creation date: (3/23/2000 9:39:49 AM)
 * @author: Andrew Bachmann
 * 
 * @param newStatements javax.swing.DefaultListModel
 */
protected void setStatements(javax.swing.DefaultListModel newStatements) {
	statements = newStatements;
}
}
