//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Insert the type's description here.
 * Creation date: (3/10/2000 8:54:51 AM)
 * @author: Andrew Bachmann
 */
public class AttributeType extends gov.nasa.arc.l2tools.livingstone.componentinstance.Particle {
	public final static String tagName = "ci:attributetype";
	private gov.nasa.arc.l2tools.livingstone.ComponentInstance componentInstance = null;
	private gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType attributeType = null;
	private java.lang.String name = null;
/**
 * AttributeType constructor comment.
 */
public AttributeType() {
	super(tagName);
}
/**
 * Creation date: (4/8/2000 6:09:13 PM)
 * @author: Andrew Bachmann
 * 
 * @param componentInstance gov.nasa.arc.l2tools.livingstone.ComponentInstance
 * @param attributeType gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType
 * @exception gov.nasa.arc.l2tools.Exception
 */
public AttributeType(gov.nasa.arc.l2tools.livingstone.ComponentInstance componentInstance, gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType attributeType) 
throws gov.nasa.arc.l2tools.Exception {
	this();
	attributeType.compile();
	setName(attributeType.getFullName());
	setComponentInstance(componentInstance);
	setAttributeType(attributeType);
	componentInstance.getChildren().insertElementAt(this,0);
}
/**
 * AttributeType constructor comment.
 * @param name java.lang.String
 */
public AttributeType(String name) {
	super(name);
}
/**
 * Creation date: (4/9/2000 5:51:15 AM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType
 */
public gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType getAttributeType() {
	return attributeType;
}
/**
 * Creation date: (4/8/2000 6:08:40 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.ComponentInstance
 */
public gov.nasa.arc.l2tools.livingstone.ComponentInstance getComponentInstance() {
	return componentInstance;
}
/**
 * Creation date: (4/9/2000 5:51:53 AM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 * search for a AttributeType with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static AttributeType search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (AttributeType)object.search(AttributeType.class,key,value);
}
/**
 * search for a AttributeType named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static AttributeType searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (AttributeType)object.searchByName(AttributeType.class,name);
}
/**
 * Creation date: (4/9/2000 5:51:15 AM)
 * @author: Andrew Bachmann
 * 
 * @param newAttributeType gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType
 */
protected void setAttributeType(gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType newAttributeType) {
	attributeType = newAttributeType;
}
/**
 * Creation date: (4/8/2000 6:08:40 PM)
 * @author: Andrew Bachmann
 * 
 * @param newComponentInstance gov.nasa.arc.l2tools.livingstone.ComponentInstance
 */
protected void setComponentInstance(gov.nasa.arc.l2tools.livingstone.ComponentInstance newComponentInstance) {
	componentInstance = newComponentInstance;
}
/**
 * Creation date: (4/9/2000 5:51:53 AM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
	set("name",newName);
}
}
