//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Creation date: (4/4/2000 12:32:57 PM)
 * @author: Andrew Bachmann
 */
public class Attribute extends gov.nasa.arc.l2tools.livingstone.componentinstance.Particle {
	public final static String tagName = "ci:attribute";	
	private gov.nasa.arc.l2tools.livingstone.ComponentType componentType = null;
	private Component component = null;
	private java.lang.String name = null;
	private java.lang.String type = null;
	private gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType abstractAttributeType = null;
	private Component instantiatedComponent = null;
/**
 * ComponentClause constructor comment.
 */
public Attribute() {
	super(tagName);
}
/**
 * Creation date: (4/8/2000 7:00:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @param name java.lang.String
 */
public Attribute(Component component, gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType type, String name) 
throws gov.nasa.arc.l2tools.Exception {
	this();
	setName(name);
	setComponent(component);
	setAbstractAttributeType(type);
	type.compile();
	setType(type.getFullName());
	insertAttribute(component);
}
/**
 * Creation date: (4/8/2000 7:00:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @param name java.lang.String
 */
public Attribute(Component component, gov.nasa.arc.l2tools.livingstone.ComponentType type, String name, Component instantiatedComponent) 
throws gov.nasa.arc.l2tools.Exception {
	this();
	setName(name);
	setComponent(component);
	setInstantiatedComponent(instantiatedComponent);
	setComponentType(type);
	type.compile();
	setType(type.getName());
//  jim requested no structural attributes in the output
//	insertAttribute(component);
}
/**
 * ComponentClause constructor comment.
 * @param name java.lang.String
 */
public Attribute(String name) {
	super(name);
}
/**
 * Creation date: (4/9/2000 1:21:44 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType
 */
public gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType getAbstractAttributeType() {
	return abstractAttributeType;
}
/**
 * Creation date: (4/8/2000 6:11:00 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
public Component getComponent() {
	return component;
}
/**
 * Creation date: (4/7/2000 11:08:53 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public gov.nasa.arc.l2tools.livingstone.ComponentType getComponentType() {
	return componentType;
}
/**
 * Creation date: (4/9/2000 2:52:43 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
public Component getInstantiatedComponent() {
	return instantiatedComponent;
}
/**
 * Creation date: (4/9/2000 5:22:14 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 * Creation date: (4/9/2000 1:07:09 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getType() {
	return type;
}
/**
 * Creation date: (5/9/2000 4:40:53 PM)
 * @author: Andrew Bachmann
 * 
 * @param component gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
private void insertAttribute(Component component) throws gov.nasa.arc.l2tools.Exception {
	if ((component.getChildren().size()>0) &&
		  (component.getChildren().getElementAt(0) instanceof StateVector)) {
		component.getChildren().insertElementAt(this,1);
  } else {
		component.getChildren().insertElementAt(this,0);
  }
}
/**
 * search for a Attribute with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute
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
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Attribute searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Attribute)object.searchByName(Attribute.class,name);
}
/**
 * Creation date: (4/9/2000 1:21:44 PM)
 * @author: Andrew Bachmann
 * 
 * @param newAbstractAttributeType gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType
 */
protected void setAbstractAttributeType(gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType newAbstractAttributeType) {
	abstractAttributeType = newAbstractAttributeType;
}
/**
 * Creation date: (4/8/2000 6:11:00 PM)
 * @author: Andrew Bachmann
 * 
 * @param newComponent gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
protected void setComponent(Component newComponent) {
	component = newComponent;
}
/**
 * Creation date: (4/7/2000 11:08:53 AM)
 * @author: Andrew Bachmann
 * 
 * @param newComponentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
protected void setComponentType(gov.nasa.arc.l2tools.livingstone.ComponentType newComponentType) {
	componentType = newComponentType;
}
/**
 * Creation date: (4/9/2000 2:52:43 PM)
 * @author: Andrew Bachmann
 * 
 * @param newInstantiatedComponent gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 */
protected void setInstantiatedComponent(Component newInstantiatedComponent) {
	instantiatedComponent = newInstantiatedComponent;
}
/**
 * Creation date: (4/9/2000 5:22:14 AM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
	set("name",newName);
}
/**
 * Creation date: (4/9/2000 1:07:09 PM)
 * @author: Andrew Bachmann
 * 
 * @param newType java.lang.String
 */
protected void setType(java.lang.String newType) {
	type = newType;
	set("type",newType);
}
}
