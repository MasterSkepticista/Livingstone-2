//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Creation date: (4/9/2000 4:44:17 AM)
 * @author: Andrew Bachmann
 */
public class Name {
	private java.util.Vector idents = new java.util.Vector();
	private boolean attribute = true;
	private boolean complex = false;
/**
 * Name constructor comment.
 */
public Name(String name) {
	java.util.StringTokenizer nameTokenizer=new java.util.StringTokenizer(name,".");
	if (nameTokenizer.countTokens()>1) setComplex(true);
	while (nameTokenizer.hasMoreTokens()) {
		getIdents().addElement(nameTokenizer.nextToken());
	}
}
/**
 * Creation date: (4/9/2000 4:44:49 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.util.Vector
 */
public java.util.Vector getIdents() {
	return idents;
}
/**
 * Creation date: (4/9/2000 4:48:44 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute instantiate(gov.nasa.arc.l2tools.livingstone.componentinstance.Component component)
throws gov.nasa.arc.l2tools.Exception {
	gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute attribute=null;
	String name=component.getName();
	java.util.Enumeration idents=getIdents().elements();
	while (idents.hasMoreElements()) {
		String ident=(String)idents.nextElement();
		name+="."+ident;
		attribute=component.findAttribute(ident);
		if (attribute!=null) {
			// already instantiated, see if we can follow it to a component
			component=attribute.getInstantiatedComponent();
			if (component==null) {
				// check to see if we are done traversing
				if (idents.hasMoreElements()) {
					// looks like there is more to go
					// one more is okay
					idents.nextElement();
					// two is not
					if (idents.hasMoreElements()) {
						String errorString="syntax error: bad attribute/constant usage at: "+name;
						throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString);
					}
				}
			}
		} else {
			// let's find out what kind of attribute it is by finding the declaration
			Attribute attributeDeclaration=component.getComponentType().findAttributeDeclaration(ident);
			if (attributeDeclaration==null) {
				// must be an enumerated type instance
				setAttribute(false);
				return null;
			} else {
				// compile
				attributeDeclaration.compile();
				// get the type conveniently
				attributeDeclaration.compileType();
				// instantiate the type
				if (attributeDeclaration.getTypeType() instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
					gov.nasa.arc.l2tools.livingstone.ComponentType type
					  = (gov.nasa.arc.l2tools.livingstone.ComponentType)attributeDeclaration.getTypeType();
					gov.nasa.arc.l2tools.livingstone.componentinstance.Component oldComponent=component;
					// follow to the new component
					component=type.instantiate(name,component.getComponentInstance());
					// but put the attribute under the old one
					attribute=new gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute(oldComponent,type,ident,component);
				} else
				if (attributeDeclaration.getTypeType() instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
					gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType type
					  = (gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)attributeDeclaration.getTypeType();
					type.instantiate(name,component);
					attribute=new gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute(component,type,ident);
					// instantiate the attribute type constraints into the attribute's component
					gov.nasa.arc.l2tools.livingstone.code.Relation relation=new gov.nasa.arc.l2tools.livingstone.code.Relation();
					Attribute thisAtt=new Attribute(); thisAtt.setName("this"); relation.getArguments().addElement(thisAtt);
					javax.swing.DefaultListModel vals=new javax.swing.DefaultListModel();
					vals.addElement(ident);
					if (type.getExpression()!=null) {
						type.getExpression().substitute(relation,vals,null).instantiate(component);
					}
				} else {
					String errorString="compiler error: getTypeType corrupted for attributeDeclaration";
					throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString);
				}
			}
		} 
	}
	setAttribute(true);
	return attribute;
}
/**
 * Creation date: (4/9/2000 5:13:56 AM)
 * @author: Andrew Bachmann
 * 
 * @return boolean
 */
public boolean isAttribute() {
	return attribute;
}
/**
 * Creation date: (4/9/2000 5:14:16 AM)
 * @author: Andrew Bachmann
 * 
 * @return boolean
 */
public boolean isComplex() {
	return complex;
}
/**
 * Creation date: (4/9/2000 5:13:56 AM)
 * @author: Andrew Bachmann
 * 
 * @param newAttribute boolean
 */
protected void setAttribute(boolean newAttribute) {
	attribute = newAttribute;
}
/**
 * Creation date: (4/9/2000 5:14:16 AM)
 * @author: Andrew Bachmann
 * 
 * @param newComplex boolean
 */
protected void setComplex(boolean newComplex) {
	complex = newComplex;
}
/**
 * Creation date: (4/9/2000 4:44:49 AM)
 * @author: Andrew Bachmann
 * 
 * @param newIdents java.util.Vector
 */
protected void setIdents(java.util.Vector newIdents) {
	idents = newIdents;
}
/**
 * Creation date: (4/9/2000 8:54:50 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public String toString() {
	return "Name: "+getIdents().toString();
}
}
