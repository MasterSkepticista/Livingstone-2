//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code;

/**
 * Insert the type's description here.
 * Creation date: (3/27/2000 2:19:40 PM)
 * @author: Andrew Bachmann
 */
public abstract class AbstractAttributeType extends CodeParticle
implements Instantiable {
	// compilation caches
	private java.lang.String name = null;
	private javax.swing.DefaultListModel constraints = new javax.swing.DefaultListModel();
	private javax.swing.DefaultListModel relations = new javax.swing.DefaultListModel();
	private javax.swing.DefaultListModel values = new javax.swing.DefaultListModel();
	private javax.swing.DefaultListModel attributes = new javax.swing.DefaultListModel();
/**
 * AbstractAttributeType constructor comment.
 */
public AbstractAttributeType() {
	super();
}
/**
 * AbstractAttributeType constructor comment.
 * @param name java.lang.String
 */
public AbstractAttributeType(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/10/2000 6:32:19 PM)
 * @author: Andrew Bachmann
 * 
 */
private void cacheAttribute(gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute attribute) 
  throws gov.nasa.arc.l2tools.Exception {
	// cache itself
	attribute.cache();

	// check for duplicate name
	java.util.Enumeration attributes=getAttributes().elements();
	while (attributes.hasMoreElements()) {
		java.lang.Object oldAttribute=attributes.nextElement();
		if (!(oldAttribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)) {
			String errorString="cache error: attributes corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
		} else if (attribute.getName()==((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)oldAttribute).getName()) {
			String errorString="illegal duplicate error: a attribute may only occur once";
			errorString+="\n  '"+attribute.getName()+"' occurs more than once";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
		}
	}

	// add to cache
	getAttributes().addElement(attribute);
}
/**
 *
 * Creation date: (3/10/2000 6:32:19 PM)
 * @author: Andrew Bachmann
 * 
 * @param relation gov.nasa.arc.l2tools.livingstone.code.Relation
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheRelation(gov.nasa.arc.l2tools.livingstone.code.Relation relation) 
  throws gov.nasa.arc.l2tools.Exception {
	// cache itself
	relation.cache();

	// check for duplicate name
	java.util.Enumeration relations=getRelations().elements();
	while (relations.hasMoreElements()) {
		java.lang.Object oldRelation=relations.nextElement();
		if (!(oldRelation instanceof gov.nasa.arc.l2tools.livingstone.code.Relation)) {
			String errorString="cache error: relations corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
		} else if (relation.prototypesEqual((gov.nasa.arc.l2tools.livingstone.code.Relation)oldRelation)) {
			String errorString="illegal duplicate error: a relation may only occur once";
			errorString+="\n  '"+relation.getName()+"' occurs more than once";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
		}
	}

	// add to cache
	getRelations().addElement(relation);
}
/**
 *
 * Creation date: (3/10/2000 6:32:19 PM)
 * @author: Andrew Bachmann
 * 
 * @param group gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
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
			getConstraints().addElement(statement);
		} else {
			String errorString="cache error: constraints corrupted in statement group";
			throw new CacheError(errorString);
		}
	}
}
/**
 *
 * Creation date: (3/10/2000 6:32:19 PM)
 * @author: Andrew Bachmann
 * 
 * @param value gov.nasa.arc.l2tools.livingstone.code.AbstractValue
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void cacheValue(gov.nasa.arc.l2tools.livingstone.code.AbstractValue value) 
  throws gov.nasa.arc.l2tools.Exception {
  // cache itself
  value.cache();

  // check for duplicate name
  java.util.Enumeration values=getValues().elements();
	while (values.hasMoreElements()) {
		java.lang.Object oldValue=values.nextElement();
		if (!(oldValue instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractValue)) {
			String errorString="cache error: values corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
		} else if (value.getName()==((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)oldValue).getName()) {
			String errorString="illegal duplicate error: a value may only occur once";
			errorString+="\n  '"+value.getName()+"' occurs more than once";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalDuplicateError(errorString);
		}
	}

	// add to cache
	getValues().addElement(value);
}
/**
 *
 * Creation date: (3/23/2000 12:19:37 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void compileAttributes() throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration attributes=getAttributes().elements();
	while (attributes.hasMoreElements()) {
		java.lang.Object attribute=attributes.nextElement();
		if (attribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
			((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute).compile();
		} else {
			String errorString="compiler error: attributes corrupted";
			throw new CompilerError(errorString);
		}
	}		
}
/**
 *
 * Creation date: (3/23/2000 12:19:37 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void compileConstraints() throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration constraints=getConstraints().elements();
	while (constraints.hasMoreElements()) {
		java.lang.Object constraint=constraints.nextElement();
		if (constraint instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.Statement) {
			gov.nasa.arc.l2tools.livingstone.code.constraint.Statement statement 
			  = (gov.nasa.arc.l2tools.livingstone.code.constraint.Statement)constraint;
			statement.compile();
			if (getExpression()==null) {
				setExpression(statement.getExpression().copy());
			} else {
				getExpression().destructiveConjunction(statement.getExpression());
			}
		} else {
			String errorString="compiler error: constraints are corrupted";
			throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
		}
	}
}
/**
 *
 * Creation date: (3/23/2000 12:19:37 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void compileRelations() throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration relations=getRelations().elements();
	while (relations.hasMoreElements()) {
		java.lang.Object relation=relations.nextElement();
		if (relation instanceof gov.nasa.arc.l2tools.livingstone.code.Relation) {
			((gov.nasa.arc.l2tools.livingstone.code.Relation)relation).compile();
		} else {
			String errorString="compiler error: relations are corrupted";
			throw new CompilerError(errorString);
		}
	}
}
/**
 *
 * Creation date: (3/23/2000 12:19:37 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void compileValues() throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration values=getValues().elements();
	while (values.hasMoreElements()) {
		java.lang.Object value=values.nextElement();
		if (value instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractValue) {
			((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)value).compile();
		} else {
			String errorString="compiler error: values are corrupted";
			throw new CompilerError(errorString);
		}
	}
}
/**
 * Creation date: (4/9/2000 3:51:18 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.Relation
 * @param name java.lang.String
 */
public Relation findRelation(String name) throws gov.nasa.arc.l2tools.Exception {
	// safe not sorry
	compile();
	// look in my relations field
	java.util.Enumeration relations=getRelations().elements();
	while (relations.hasMoreElements()) {
		Relation relation=(Relation)relations.nextElement();
		if (name.equals(relation.getName()))
		  return relation;
	}
	return null;
}
/**
 *
 * Creation date: (3/28/2000 1:14:27 PM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getAttributes() {
	return attributes;
}
/**
 *
 * Creation date: (3/24/2000 10:34:47 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getConstraints() {
	return constraints;
}
/**
 * Creation date: (4/4/2000 3:31:10 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public String getFullName() throws gov.nasa.arc.l2tools.Exception {
	gov.nasa.arc.l2tools.io.TreeObject parent=getParent();
	String fullName=getName();
	while (parent instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
		((gov.nasa.arc.l2tools.livingstone.ComponentType)parent).compile();
		fullName=((gov.nasa.arc.l2tools.livingstone.ComponentType)parent).getName()+"."+fullName;
		parent=((gov.nasa.arc.l2tools.livingstone.ComponentType)parent).getParent();
	}
	return fullName;
}
/**
 *
 * Creation date: (3/10/2000 4:28:40 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 *
 * Creation date: (3/24/2000 10:35:01 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getRelations() {
	return relations;
}
/**
 *
 * Creation date: (3/24/2000 10:35:12 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getValues() {
	return values;
}
/**
 * Creation date: (4/9/2000 5:43:42 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType
 * @param name java.lang.String
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType instantiate(String name, gov.nasa.arc.l2tools.livingstone.componentinstance.Component component) 
throws gov.nasa.arc.l2tools.Exception {
	// just in case
	compile();
	// check to see if its already done
	gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType attributeType
	  = component.getComponentInstance().findAttributeType(getFullName());
	if (attributeType!=null) return attributeType;
	// create the type
	attributeType = new gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType(component.getComponentInstance(),this);
	// members
	String members=null;
	java.util.Enumeration values=getValues().elements();
	while (values.hasMoreElements()) {
		AbstractValue value=(AbstractValue)values.nextElement();
		if (members==null) members=value.getName();
		else members+=" "+value.getName();
	}
	if ((members!=null) && (members!=""))
		attributeType.set("members",members);
	// done
	return attributeType;
}
/**
 *
 * Creation date: (3/24/2000 2:14:48 PM)
 * @author: Andrew Bachmann
 * 
 * @return boolean
 * @param relation gov.nasa.arc.l2tools.livingstone.AbstractAttributeType
 */
public boolean prototypesEqual(AbstractAttributeType attributeType) {
	if (getName()==null) return false;
	// TODO: check argument list
	return (getName().equals(attributeType.getName()));
}
/**
 *
 * Creation date: (3/10/2000 4:17:47 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	// cache attributes
	if ((get("name") != null) && (get("name") != "")) {
		setName(get("name"));
	}
	
	// cache children
	java.util.Enumeration children = getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object child = children.nextElement();
		if (child instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractValue) {
			cacheValue((gov.nasa.arc.l2tools.livingstone.code.AbstractValue)child);
		} else
		if (child instanceof gov.nasa.arc.l2tools.livingstone.code.Relation) {
			cacheRelation((gov.nasa.arc.l2tools.livingstone.code.Relation)child);
		} else
		if (child instanceof gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup) {
			cacheStatementGroup((gov.nasa.arc.l2tools.livingstone.code.constraint.StatementGroup)child);
		} else 
		if (child instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
			cacheAttribute((gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)child);
		} else {
			//String errorString="unexpected child error: a child should be one of: value, relation, statement group";
			//throw new UnexpectedChildError(errorString);
		}
	}
}
/**
 * compile method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.Exception {
	compileValues();
	compileConstraints();
	compileRelations();
	compileAttributes();
}
/**
 *
 * Creation date: (3/28/2000 1:14:27 PM)
 * @author: Andrew Bachmann
 * 
 * @param newAttributes javax.swing.DefaultListModel
 */
protected void setAttributes(javax.swing.DefaultListModel newAttributes) {
	attributes = newAttributes;
}
/**
 *
 * Creation date: (3/24/2000 10:34:47 AM)
 * @author: Andrew Bachmann
 * 
 * @param newConstraints javax.swing.DefaultListModel
 */
protected void setConstraints(javax.swing.DefaultListModel newConstraints) {
	constraints = newConstraints;
}
/**
 *
 * Creation date: (3/10/2000 4:28:40 PM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
}
/**
 *
 * Creation date: (3/24/2000 10:35:01 AM)
 * @author: Andrew Bachmann
 * 
 * @param newRelations javax.swing.DefaultListModel
 */
protected void setRelations(javax.swing.DefaultListModel newRelations) {
	relations = newRelations;
}
/**
 *
 * Creation date: (3/24/2000 10:35:12 AM)
 * @author: Andrew Bachmann
 * 
 * @param newValues javax.swing.DefaultListModel
 */
protected void setValues(javax.swing.DefaultListModel newValues) {
	values = newValues;
}
}
