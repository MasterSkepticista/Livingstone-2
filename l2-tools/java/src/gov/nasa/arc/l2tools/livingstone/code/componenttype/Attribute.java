//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/3/2000 3:16:50 PM)
 * @author: Andrew Bachmann
 */
public class Attribute extends gov.nasa.arc.l2tools.livingstone.code.CodeParticle 
implements gov.nasa.arc.l2tools.livingstone.code.Instantiable {
	public final static String tagName = "ct:attribute";
	private java.lang.String name = null;
	private java.lang.String type = null;
	private java.lang.String value = null;
	private javax.swing.DefaultListModel modifiers = new javax.swing.DefaultListModel();
	private gov.nasa.arc.l2tools.livingstone.DataObject typeType = null;
/**
 * Attribute constructor comment.
 */
public Attribute() {
	super(tagName);
}
/**
 * Attribute constructor comment.
 * @param name java.lang.String
 */
public Attribute(String name) {
	super(name);
}
/**
 * Creation date: (4/4/2000 2:29:02 PM)
 * @author: Andrew Bachmann
 * 
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void compileType() throws gov.nasa.arc.l2tools.Exception {
	// just in case
	compile();
	
	// null means no type, inconsistent
	if (getType()==null) {
		String errorString="illegal anonymous error: no type specified";
		throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
	}
	// find it
	gov.nasa.arc.l2tools.io.TreeObject parent=getParent();
	gov.nasa.arc.l2tools.livingstone.DataObject typeType=null;
	if (parent instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
		typeType=((gov.nasa.arc.l2tools.livingstone.ComponentType)parent).findType(getType());
	} else
	if (parent instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
		typeType=(gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)parent;
	} else {
		String errorString2="compiler error: attribute has no parent to resolve type in";
		throw new gov.nasa.arc.l2tools.livingstone.code.CompilerError(errorString2);
	}
	
	// not here? bail
	if (typeType==null) {
		String errorString3="find error: type '"+getType()+"' for attribute '"+getName()+"' is undefined";
		throw new gov.nasa.arc.l2tools.Exception(errorString3);
	}
	// not a component type? bail
	if (!(typeType instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) &&
		  !(typeType instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)) {
		String errorString2="syntax error: type '"+getType()+"' for attribute '"+getName()+"' is not a component type or attribute type";
		throw new gov.nasa.arc.l2tools.livingstone.SyntaxError(errorString2);
	}
	// compile it
	if (typeType instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
		((gov.nasa.arc.l2tools.livingstone.ComponentType)typeType).compile();
	} else
	if (typeType instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
		((gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)typeType).compile();
	}
	// now that we know it is okay, attach a pointer to it
	setTypeType((gov.nasa.arc.l2tools.livingstone.DataObject)typeType);
}
/**
 *
 * Creation date: (3/24/2000 9:36:27 AM)
 * @author: Andrew Bachmann
 * 
 * @return javax.swing.DefaultListModel
 */
public javax.swing.DefaultListModel getModifiers() {
	return modifiers;
}
/**
 *
 * Creation date: (3/24/2000 9:35:16 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 *
 * Creation date: (3/24/2000 9:35:41 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getType() {
	return type;
}
/**
 * Creation date: (4/4/2000 2:28:31 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.DataObject
 */
public gov.nasa.arc.l2tools.livingstone.DataObject getTypeType() {
	return typeType;
}
/**
 *
 * Creation date: (3/24/2000 9:36:05 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getValue() {
	return value;
}
/**
 * refreshCache method comment.
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		// cache attributes
		if ((get("name")!=null) && (get("name")!="")) {
			setName(get("name"));
		} else {
			String errorString = "illegal anonymous error: each attribute must have a name";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
		}
		if (getParent() instanceof gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType) {
			setType(((gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType)getParent()).getName());
		} else if ((get("type")!=null) && (get("type")!="")) {
			setType(get("type"));
		} else {
			String errorString = "illegal anonymous error: each attribute must have a type";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
		} 
		if ((get("value")!=null) && (get("value")!="")) {
			setValue(get("value"));
		}
		if ((get("modifiers")!=null) && (get("modifiers")!="")) {
			java.util.StringTokenizer modifierTokenizer=new java.util.StringTokenizer(get("modifiers"));
			while (modifierTokenizer.hasMoreTokens()) {
				getModifiers().addElement(modifierTokenizer.nextToken());
			}
		}
	
		// check for children
		if (!getChildren().isEmpty()) {
			System.out.println("Ignoring children of attribute/argument");
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in a attribute/argument");
		if (getName()!=null) error.concatMessage(" named '"+getName()+"'");
		if (getType()!=null) error.concatMessage(" of type '"+getType()+"'");
		throw error;
	}
}
/**
 * refreshClauses method comment.
 */
public void refreshExpression() throws gov.nasa.arc.l2tools.livingstone.code.CompilerError {}
/**
 * search for a Attribute with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Attribute search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (Attribute)object.search(Attribute.class,key,value);
}
/**
 * search for a Attribute named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Attribute searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Attribute)object.searchByName(Attribute.class,name);
}
/**
 *
 * Creation date: (3/24/2000 9:36:27 AM)
 * @author: Andrew Bachmann
 * 
 * @param newModifiers javax.swing.DefaultListModel
 */
protected void setModifiers(javax.swing.DefaultListModel newModifiers) {
	modifiers = newModifiers;
}
/**
 *
 * Creation date: (3/24/2000 9:35:16 AM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
}
/**
 *
 * Creation date: (3/24/2000 9:35:41 AM)
 * @author: Andrew Bachmann
 * 
 * @param newType java.lang.String
 */
protected void setType(java.lang.String newType) {
	type = newType;
}
/**
 * Creation date: (4/4/2000 2:28:31 PM)
 * @author: Andrew Bachmann
 * 
 * @param newTypeType gov.nasa.arc.l2tools.livingstone.DataObject
 */
protected void setTypeType(gov.nasa.arc.l2tools.livingstone.DataObject newTypeType) {
	typeType = newTypeType;
}
/**
 *
 * Creation date: (3/24/2000 9:36:05 AM)
 * @author: Andrew Bachmann
 * 
 * @param newValue java.lang.String
 */
protected void setValue(java.lang.String newValue) {
	value = newValue;
}
}
