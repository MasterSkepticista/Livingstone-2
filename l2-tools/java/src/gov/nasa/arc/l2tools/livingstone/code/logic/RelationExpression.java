//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/5/2000 1:53:10 PM)
 * @author: Andrew Bachmann
 */
public class RelationExpression extends LogicExpression {
	private java.lang.String name = null;
	private javax.swing.DefaultListModel arguments = new javax.swing.DefaultListModel();
/**
 * RelationExpression constructor comment.
 */
public RelationExpression() {
	super();
}
/**
 * copy method comment.
 */
public LogicExpression copy() {
	RelationExpression relation = new RelationExpression();
	relation.setName(getName());
	java.util.Enumeration arguments=getArguments().elements();
	while (arguments.hasMoreElements()) {
		relation.getArguments().addElement((String)arguments.nextElement());
	}	
	return relation;
}
/**
 * Creation date: (4/8/2000 10:40:55 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
protected BasicExpression expandAndInstantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	try {
		int dotLocation=getName().lastIndexOf('.');
		String relationOwner=null; 
		String relationName=getName();
		gov.nasa.arc.l2tools.livingstone.ComponentType componentType=null;
		gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType attributeType=null;
		if (dotLocation != -1) {
			relationOwner=getName().substring(0,dotLocation);
			relationName=getName().substring(dotLocation+1);
			gov.nasa.arc.l2tools.livingstone.code.componenttype.Name name
			  = new gov.nasa.arc.l2tools.livingstone.code.componenttype.Name(relationOwner);
			gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute attribute
			  = name.instantiate(component);
			componentType=attribute.getComponentType();
			attributeType=attribute.getAbstractAttributeType();
		} else {
			componentType=component.getComponentType();
		}
		gov.nasa.arc.l2tools.livingstone.code.Relation relation=null;
		LogicExpression result=null;
		if (attributeType!=null) {
			relation=attributeType.findRelation(relationName);
			if (relation==null) {
				String errorString="syntax error: unable to find relation '"+relationName+"'";
				errorString+="\n  in attribute type '"+attributeType.getFullName()+"'";
				throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
			}
		} else if (componentType!=null) {
			relation=componentType.findRelation(relationName);
			if (relation==null) {
				String errorString="syntax error: unable to find relation '"+relationName+"'";
				errorString+="\n  in component type '"+componentType.getName()+"'";
				throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
			}
		} else {
			String errorString="compiler error: unable to find relation '"+getName()+"'";
			throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
		}
		result=relation.getExpression().substitute(relation,getArguments(),relationOwner);		
		return result.expandAndInstantiate(component);
	} catch (gov.nasa.arc.l2tools.Exception exception) {
		exception.concatMessage("\n  in relation expression: "+getName());
		throw exception;
	} catch (RuntimeException exception) {
		throw new gov.nasa.arc.l2tools.Exception(exception.toString());
	}
}
/**
 * Creation date: (4/5/2000 2:36:54 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getArguments() {
	return arguments;
}
/**
 * Creation date: (4/5/2000 2:36:34 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 * Creation date: (4/5/2000 2:36:54 PM)
 * @author: Andrew Bachmann
 * 
 * @param newArguments javax.swing.DefaultListModel
 */
protected void setArguments(javax.swing.DefaultListModel newArguments) {
	arguments = newArguments;
}
/**
 * Creation date: (4/5/2000 2:36:34 PM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
public void setName(java.lang.String newName) {
	name = newName;
}
/**
 * substitute method comment.
 */
public LogicExpression substitute(gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner) 
throws gov.nasa.arc.l2tools.Exception {
	RelationExpression newRelation=new RelationExpression();
	// make prefix
	String prefix;
	if (relationOwner!=null) 
	  prefix=relationOwner+".";
	else
	  prefix="";
	// remove the relation identifier from replacement
	if (getName().indexOf('.')==-1) {
		// no variable, the new name is made by simply adding the prefix
		newRelation.setName(prefix+getName());
	} else {
		// perform a substitution on the relation identifier
		newRelation.setName(substituteVariable(getName(),relation.getArguments(),values,prefix));
	}
	// check arguments for substitution
	java.util.Enumeration arguments=getArguments().elements();
	while (arguments.hasMoreElements()) {
		String argument=(String)arguments.nextElement();
		newRelation.getArguments().addElement(substituteVariable(argument,relation.getArguments(),values,prefix));
	}
	return newRelation;
}
/**
 * Creation date: (5/23/2000 3:26:52 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 * @param identifier java.lang.String
 * @param arguments javax.swing.DefaultListModel
 * @param values javax.swing.DefaultListModel
 * @param prefix java.lang.String
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public String substituteVariable(String identifier, javax.swing.DefaultListModel arguments, javax.swing.DefaultListModel values, String prefix) 
throws gov.nasa.arc.l2tools.Exception {
	int identifierDot=identifier.indexOf('.');
	// remove variable from the front of the identifier
	String variable;
	if (identifierDot==-1) {
		variable=identifier;
	} else {
		variable=identifier.substring(0,identifierDot);
	}
	// check for the variable in the arguments
	int argNum;
	for (argNum=0;(argNum<arguments.getSize());argNum++) {
		String argName=((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)arguments.getElementAt(argNum)).getName();
		if (variable.equals(argName)) {
			break;
		}
	}
	// if no variable found, prefix the identifier and return it
	if (argNum==arguments.getSize()) {
		return prefix+identifier;
	}
	// otherwise, proceed to replace it
	// check that there is a value for this variable
	if (argNum>=values.getSize()) {
		String errorString="syntax error: call has too few arguments";
		throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
	}
	// replace the variable with the value
	if (identifierDot==-1) {
		return (String)values.elementAt(argNum);
	} else {
		return (String)values.elementAt(argNum)+identifier.substring(identifierDot);
	}
}
/**
 * Creation date: (4/5/2000 4:02:35 PM)
 * @author: Andrew Bachmann
 * 
 */
public String toString() {
	String result=getName()+"(";
	java.util.Enumeration arguments=getArguments().elements();
	while (arguments.hasMoreElements()) {
		java.lang.Object argument=arguments.nextElement();
		result+=argument.toString();
		if (arguments.hasMoreElements()) {
			result+=",";
		}
	}
	result+=")";
	return result;
}
}
