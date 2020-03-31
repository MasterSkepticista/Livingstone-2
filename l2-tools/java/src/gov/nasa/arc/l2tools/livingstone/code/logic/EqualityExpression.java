//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.logic;

/**
 * Creation date: (4/5/2000 1:53:22 PM)
 * @author: Andrew Bachmann
 */
public class EqualityExpression extends UnaryExpression {
	private java.lang.String argument1 = null;
	private java.lang.String argument2 = null;
/**
 * EqualityExpression constructor comment.
 */
public EqualityExpression() {
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
public gov.nasa.arc.l2tools.livingstone.componentinstance.Clause buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.Clause clause) 
throws gov.nasa.arc.l2tools.Exception {
	gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm term
	  = new gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm(clause);
	buildInstance(term);
	return clause;
}
/**
 * Creation date: (4/9/2000 12:09:28 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm
 * @param term gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm buildInstance(gov.nasa.arc.l2tools.livingstone.componentinstance.EqualityTerm term) 
throws gov.nasa.arc.l2tools.Exception {
	String prefix="";
	// for an extended form
	// term.getClause().getClauses().getComponent().getName()+"."
// for a short form
//	int separator=name.indexOf('.');
//	if (separator!=-1)
//	  name=name.substring(separator+1);
	term.setArgument1(prefix+getArgument1());
	term.setArgument2(prefix+getArgument2());
	return term;
}
/**
 * copy method comment.
 */
public LogicExpression copy() {
	EqualityExpression equality = new EqualityExpression();
	equality.setArgument1(getArgument1());
	equality.setArgument2(getArgument2());
	return equality;
}
/**
 * OR(A,(AND X (NOT Y) (OR Z Q)) => (AND (OR A X) (OR A (NOT Y)) (OR A (OR Z Q)))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(AndExpression expression) {
	AndExpression and=new AndExpression();
	java.util.Enumeration subExpressions=expression.getSubExpressions().elements();
	while (subExpressions.hasMoreElements()) {
		BasicExpression subExpression=(BasicExpression)subExpressions.nextElement();
		OrExpression or=new OrExpression();
		or.getSubExpressions().addElement(this);
		or.getSubExpressions().addElement(subExpression);
		and.getSubExpressions().addElement(or);
	}
	return and;
}
/**
 * OR(A,(OR B (NOT C) (AND D Y)))=>(OR A B (NOT C) (AND D Y))
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression distribute(OrExpression expression) {
	BasicExpression or=new OrExpression();
	java.util.Enumeration subExpressions=expression.getSubExpressions().elements();
	while (subExpressions.hasMoreElements()) {
		BasicExpression subExpression=(BasicExpression)subExpressions.nextElement();
		((OrExpression)or).getSubExpressions().addElement(subExpression);
	}
	or=((OrExpression)or).merge(this);
	return or;
}
/**
 * Creation date: (4/8/2000 10:40:55 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.BasicExpression
 * @param gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
protected BasicExpression expandAndInstantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	gov.nasa.arc.l2tools.livingstone.code.componenttype.Name name1
	  = new gov.nasa.arc.l2tools.livingstone.code.componenttype.Name(getArgument1());
	gov.nasa.arc.l2tools.livingstone.code.componenttype.Name name2
	  = new gov.nasa.arc.l2tools.livingstone.code.componenttype.Name(getArgument2());
  gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute att1
		= name1.instantiate(component);
  gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute att2
		= name2.instantiate(component);
	if (name1.isAttribute()) {
		if (name2.isAttribute()) {
			if (att1.getComponentType()!=null) {
				// attribute 1 is a componenttype
				if (att2.getComponentType()!=null) {
					// attribute 2 is a componenttype
					if (att2.getComponentType()!=att1.getComponentType()) {
						gov.nasa.arc.l2tools.livingstone.SyntaxError error
						  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: attributes in equality are not both components of the same type:");
						error.concatMessage("\n  component '"+getArgument1()+"' of type '"+att1.getComponentType().getName()+"'");
						error.concatMessage("\n  component '"+getArgument2()+"' of type '"+att2.getComponentType().getName()+"'");
						throw error;
					} else {
						// we have two component types of the same type
						return expandComponentEquality(att1.getComponentType()).expandAndInstantiate(component);
					}
				}
				if (att2.getAbstractAttributeType()!=null) {
					// attribute 2 is an attribute type
					gov.nasa.arc.l2tools.livingstone.SyntaxError error
					  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: attributes in equality are not both components of the same type:");
					error.concatMessage("\n  component '"+getArgument1()+"' of type '"+att1.getComponentType().getName()+"'");
					error.concatMessage("\n  attribute '"+getArgument2()+"' of type '"+att2.getAbstractAttributeType().getName()+"'");
					throw error;
				}
				// attribute 2 is not an attribute (it's a value?)
				gov.nasa.arc.l2tools.livingstone.code.CompilerError error
				  = new gov.nasa.arc.l2tools.livingstone.code.CompilerError("compiler error: second attribute in comparison has no type:");
				error.concatMessage("\n  attribute '"+getArgument2()+"'");
				throw error;
			}
			if (att1.getAbstractAttributeType()!=null) {
				// attribute 1 is an attribute type
				if (att2.getComponentType()!=null) {
					// attribute 2 is a componenttype
					gov.nasa.arc.l2tools.livingstone.SyntaxError error
					  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: attributes in equality are not both components of the same type:");
					error.concatMessage("\n  attribute '"+getArgument1()+"' of type '"+att1.getAbstractAttributeType().getName()+"'");
					error.concatMessage("\n  component '"+getArgument2()+"' of type '"+att2.getComponentType().getName()+"'");
					throw error;
				}
				if (att2.getAbstractAttributeType()!=null) {
					// attribute 2 is an attribute type
					if (att2.getAbstractAttributeType()!=att1.getAbstractAttributeType()) {
						gov.nasa.arc.l2tools.livingstone.SyntaxError error
						  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: attributes in equality are not both attribute values of the same type:");
						error.concatMessage("\n  attribute '"+getArgument1()+"' of type '"+att1.getAbstractAttributeType().getName()+"'");
						error.concatMessage("\n  attribute '"+getArgument2()+"' of type '"+att2.getAbstractAttributeType().getName()+"'");
						throw error;
					} else {
						// we have two attribute types of the same type
						return this;
					}
				}
				// attribute 2 is not an attribute (it's a value?)
				gov.nasa.arc.l2tools.livingstone.code.CompilerError error
				  = new gov.nasa.arc.l2tools.livingstone.code.CompilerError("compiler error: second attribute in comparison has no type:");
				error.concatMessage("\n  attribute '"+getArgument2()+"'");
				throw error;
				
			}
			// attribute 1 is not an attribute (it's a value?)
			gov.nasa.arc.l2tools.livingstone.code.CompilerError error
			  = new gov.nasa.arc.l2tools.livingstone.code.CompilerError("compiler error: first attribute in comparison has no type:");
			error.concatMessage("\n  attribute '"+getArgument1()+"'");
			throw error;
		} else {
			// attribute 2 is a value
			if (att1.getComponentType()!=null) {
				// attribute 1 is a componenttype
				gov.nasa.arc.l2tools.livingstone.SyntaxError error
				  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: attributes in equality are not both components of the same type:");
				error.concatMessage("\n  component '"+getArgument1()+"' of type '"+att1.getComponentType().getName()+"'");
				error.concatMessage("\n  attribute value '"+getArgument2()+"' of unknown type");
				throw error;
			}
			if (att1.getAbstractAttributeType()!=null) {
				// attribute 1 is an attribute value
				java.util.Enumeration values=att1.getAbstractAttributeType().getValues().elements();
				while (values.hasMoreElements()) {
					if (getArgument2().equals(((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)values.nextElement()).getName())) {
						// the value is a possible value
						return this;
					}
				}
				// the value is not in the domain of this attribute value
				gov.nasa.arc.l2tools.livingstone.SyntaxError error
				  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: value is not in the domain of the attribute type:");
				error.concatMessage("\n  attribute value '"+getArgument1()+"' of type '"+att1.getAbstractAttributeType().getName()+"'");
				error.concatMessage("\n  attribute value '"+getArgument2()+"' is not in the domain");
				throw error;
			}
			// attribute 1 is not an attribute (it's a value?)
			gov.nasa.arc.l2tools.livingstone.code.CompilerError error
			  = new gov.nasa.arc.l2tools.livingstone.code.CompilerError("compiler error: first attribute in comparison has no type:");
			error.concatMessage("\n  attribute '"+getArgument1()+"'");
			throw error;
		}
	} else {
		// attribute 1 is a value
		if (name2.isAttribute()) {
			// attribute 2 is not a value
			if (att2.getComponentType()!=null) {
				// attribute 2 is a componenttype
				gov.nasa.arc.l2tools.livingstone.SyntaxError error
				  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: attributes in equality are not both components of the same type:");
				error.concatMessage("\n  attribute value '"+getArgument1()+"' of unknown type");
				error.concatMessage("\n  component '"+getArgument2()+"' of type '"+att2.getComponentType().getName()+"'");
				throw error;
			}
			if (att2.getAbstractAttributeType()!=null) {
				// attribute 2 is an attribute value
				java.util.Enumeration values=att2.getAbstractAttributeType().getValues().elements();
				while (values.hasMoreElements()) {
					if (getArgument1().equals(((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)values.nextElement()).getName())) {
						// the value is a possible value
						return this;
					}
				}
				// the value is not in the domain of this attribute value
				gov.nasa.arc.l2tools.livingstone.SyntaxError error
				  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: value is not in the domain of the attribute type:");
				error.concatMessage("\n  attribute value '"+getArgument1()+"' is not in the domain");
				error.concatMessage("\n  attribute value '"+getArgument2()+"' of type '"+att2.getAbstractAttributeType().getName()+"'");
				throw error;
			}
			// attribute 2 is not an attribute (it's a value?)
			gov.nasa.arc.l2tools.livingstone.code.CompilerError error
			  = new gov.nasa.arc.l2tools.livingstone.code.CompilerError("compiler error: second attribute in comparison has no type:");
			error.concatMessage("\n  attribute '"+getArgument1()+"'");
			throw error;
		} else {
			// two values is an improper form
			gov.nasa.arc.l2tools.livingstone.SyntaxError error
			  = new gov.nasa.arc.l2tools.livingstone.SyntaxError("syntax error: attributes in equality are both values or unidentifiable attributes:");
			error.concatMessage("\n  value/attribute? '"+getArgument1()+"' of unknown type");
			error.concatMessage("\n  value/attribute? '"+getArgument2()+"' of unknown type");
			throw error;
		}
	}
}
/**
 * Creation date: (4/9/2000 5:26:28 AM)
 * @author: Andrew Bachmann
 *
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void expandComponentAttributeEqualities(AndExpression and, gov.nasa.arc.l2tools.livingstone.ComponentType attributeType, String name1, String name2) 
throws gov.nasa.arc.l2tools.Exception {
	while (attributeType!=null) {
		java.util.Enumeration attributes=attributeType.getAttributes().elements();
		while (attributes.hasMoreElements()) {
			java.lang.Object attribute=attributes.nextElement();
			if (attribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
				expandComponentAttributeEquality(and,(gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute,name1,name2);
			}
		}
		attributeType=attributeType.getSuperComponentType();
	}
}
/**
 * Creation date: (5/16/2000 3:22:22 PM)
 * @author: Andrew Bachmann
 * 
 * @param and gov.nasa.arc.l2tools.livingstone.code.logic.AndExpression
 * @param attribute gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute
 * @param name1 java.lang.String
 * @param name2 java.lang.String
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void expandComponentAttributeEquality(AndExpression and, gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attribute, String name1, String name2) 
throws gov.nasa.arc.l2tools.Exception {
	if (attribute.getTypeType() instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
		expandComponentAttributeEqualities(and,(gov.nasa.arc.l2tools.livingstone.ComponentType)attribute.getTypeType(),
			name1+"."+attribute.getName(),name2+"."+attribute.getName());
	} else {
		EqualityExpression equality = new EqualityExpression();
		if (!attribute.getName().equals("this")) name1+="."+attribute.getName();
		if (!attribute.getName().equals("this")) name2+="."+attribute.getName();
		equality.setArgument1(name1);
		equality.setArgument2(name2);
		and.getSubExpressions().addElement(equality);
	}
}
/**
 * Creation date: (4/9/2000 5:26:28 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.logic.AndExpression
 * @param attributeType gov.nasa.arc.l2tools.livingstone.ComponentType
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private AndExpression expandComponentEquality(gov.nasa.arc.l2tools.livingstone.ComponentType attributeType) 
throws gov.nasa.arc.l2tools.Exception {
	AndExpression and=new AndExpression();
	expandComponentAttributeEqualities(and,attributeType,getArgument1(),getArgument2());
	return and;
}
/**
 * Creation date: (4/6/2000 2:43:09 PM)
 * @author: Andrew Bachmann
 */
protected BasicExpression flatten() {
	return this;
}
/**
 * Creation date: (4/5/2000 2:09:31 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getArgument1() {
	return argument1;
}
/**
 * Creation date: (4/5/2000 2:09:44 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getArgument2() {
	return argument2;
}
/**
 * pushNegationIn method comment.
 */
protected BasicExpression pushNegationIn() throws NegationPushFailure {
	throw new NegationPushFailure();
}
/**
 * pushNegationsIn method comment.
 */
protected BasicExpression pushNegationsIn() {
	return this;
}
/**
 * Creation date: (4/5/2000 2:09:31 PM)
 * @author: Andrew Bachmann
 * 
 * @param newArgument1 java.lang.String
 */
public void setArgument1(java.lang.String newArgument1) {
	argument1 = newArgument1;
}
/**
 * Creation date: (4/5/2000 2:09:44 PM)
 * @author: Andrew Bachmann
 * 
 * @param newArgument2 java.lang.String
 */
public void setArgument2(java.lang.String newArgument2) {
	argument2 = newArgument2;
}
/**
 * substitute method comment.
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.logic.LogicExpression
 * @param relation gov.nasa.arc.l2tools.livingstone.code.Relation
 * @param values javax.swing.DefaultListModel
 * @param relationOwner java.lang.String
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public LogicExpression substitute(gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner) 
throws gov.nasa.arc.l2tools.Exception {
	EqualityExpression equality = new EqualityExpression();
	equality.setArgument1(substituteArgument(getArgument1(),relation,values,relationOwner));
	equality.setArgument2(substituteArgument(getArgument2(),relation,values,relationOwner));
	return equality;
}
/**
 * Creation date: (5/19/2000 10:51:09 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 * @param argument java.lang.String
 * @param relation gov.nasa.arc.l2tools.livingstone.code.Relation
 * @param values javax.swing.DefaultListModel
 * @param relationOwner java.lang.String
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private String substituteArgument(String argument, gov.nasa.arc.l2tools.livingstone.code.Relation relation, javax.swing.DefaultListModel values, String relationOwner) throws gov.nasa.arc.l2tools.Exception {
	// no need to look
	if (argument.equals("this")) {
		return relationOwner;
	}
	// chop off attribute accessors from the variable name
	int dotLocation=argument.indexOf('.'); // used to be: lastIndexOf('.'); but why??
	String argumentBase=argument;
	String argumentExtension="";
	if (dotLocation != -1) {
		argumentBase=argument.substring(0,dotLocation);
		argumentExtension=argument.substring(dotLocation);
	}
  // look for argument in variables
	int argNum;
	for (argNum=0;(argNum<relation.getArguments().getSize());argNum++) {
		if (argumentBase.equals(((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)relation.getArguments().get(argNum)).getName()))
			break;
	}
	// if we found it, do the substitution
	if (argNum<relation.getArguments().getSize()) {
		if (argNum>values.getSize()) {
			String errorString="syntax error: call has too few arguments";
			throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
		}
		return (String)values.getElementAt(argNum)+argumentExtension;
	}
	// guess if we need a prefix
	if (relation.getParent() instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
		gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType type =
			(gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)relation.getParent();
		// if it is a possible value, it is fine
		java.util.Enumeration possibleValues=type.getValues().elements();
		while (possibleValues.hasMoreElements()) {
			java.lang.Object value=possibleValues.nextElement();
			if (((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)value).get("name").equals(argument)) {
				return argument;
			}
		}
		// otherwise, hope it's a value for somebody else
		// this will be checked in the expansion
		return argument;
	} else if (relation.getParent() instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
		gov.nasa.arc.l2tools.livingstone.ComponentType type =
			(gov.nasa.arc.l2tools.livingstone.ComponentType)relation.getParent();
		// if it is an attribute, we'll just prefix it
                java.util.StringTokenizer argumentTokens=new java.util.StringTokenizer(argumentBase,".");
                while (argumentTokens.hasMoreTokens()) {
                    String argumentIdent=argumentTokens.nextToken();
                    gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attributeDeclaration
                     = type.findAttributeDeclaration(argumentIdent);
                    if (attributeDeclaration == null) {
                        // couldn't find it, hope it's a value
                        // this will be checked in the expansion
                        return argument; 
                    }
                    // compile
                    attributeDeclaration.compile();
                    // get the type conveniently
                    attributeDeclaration.compileType();
                    // instantiate the type
                    if (attributeDeclaration.getTypeType() instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
                        if (argumentTokens.hasMoreTokens()) {
                            // there shouldn't be any attribute types here.
                            String errorString="syntax error: '"+relation.getName()+"' is defined on an attribute type '"+attributeDeclaration.getName()+"'";
                            throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
                        } else { // we are in the clear
                            break;
                        }
                    } else if (attributeDeclaration.getTypeType() instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
                        // now the type of that attribute is available for us to continue resolution
                        type = (gov.nasa.arc.l2tools.livingstone.ComponentType)attributeDeclaration.getTypeType();
                    } else if (attributeDeclaration.getTypeType() == null) {
                        String errorString="compiler error: attribute '"+attributeDeclaration.getName()+"' in relation '"+relation.getName()+"' has no type'";
                        throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
                    }
                }
                return (relationOwner==null ? argument : relationOwner+"."+argument);
	}	else {
		String errorString="compiler error: relation '"+relation.getName()+"' has no valid parent";
		throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);		
	}		
}
/**
 * Creation date: (4/5/2000 4:02:35 PM)
 * @author: Andrew Bachmann
 * 
 */
public String toString() {
	return "("+getArgument1()+"="+getArgument2()+")";
}
}
