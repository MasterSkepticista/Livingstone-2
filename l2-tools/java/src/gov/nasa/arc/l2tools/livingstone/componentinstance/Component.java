//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.componentinstance;

/**
 * Insert the type's description here.
 * Creation date: (3/10/2000 2:24:14 PM)
 * @author: Andrew Bachmann
 */
public class Component extends gov.nasa.arc.l2tools.livingstone.componentinstance.Particle {
	public final static String tagName = "ci:component";
	private java.lang.String name = null;
	private gov.nasa.arc.l2tools.livingstone.ComponentInstance componentInstance = null;
	private gov.nasa.arc.l2tools.livingstone.ComponentType componentType = null;
	private java.lang.String type = null;
/**
 * Component constructor comment.
 */
public Component() {
	super(tagName);
}
/**
 * Component constructor comment.
 * @param name java.lang.String
 */
public Component(String name) {
	super(name);
}
/**
 * Creation date: (4/8/2000 6:24:33 PM)
 * @author: Andrew Bachmann
 * 
 * @param componentInstance gov.nasa.arc.l2tools.livingstone.ComponentInstance
 * @exception gov.nasa.arc.l2tools.Exception 
 */
public Component(String name, gov.nasa.arc.l2tools.livingstone.ComponentInstance componentInstance, gov.nasa.arc.l2tools.livingstone.ComponentType componentType) 
throws gov.nasa.arc.l2tools.Exception {
	this();
	setComponentInstance(componentInstance);
	setComponentType(componentType);
	componentType.compile();
	setType(componentType.getName());
	setName(name);
	componentInstance.getChildren().insertElementAt(this,0);
}
/**
 * Creation date: (9/14/2000 1:55:59 PM)
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public void checkVariableReferences() throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration attributes=getComponentType().getAttributes().elements();
	while (attributes.hasMoreElements()) {
		java.lang.Object attribute=attributes.nextElement();
		if (attribute instanceof gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute) {
			gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute codeAttribute
			  = (gov.nasa.arc.l2tools.livingstone.code.componenttype.Attribute)attribute;
			codeAttribute.compile();
			Attribute attributeInstance = findAttribute(codeAttribute.getName());
			if (attributeInstance == null) {
        if (getComponentInstance().findComponent(name+"."+codeAttribute.getName()) == null) {
          System.out.print("\nwarning: attribute '"+codeAttribute.getName()+"'");
          System.out.print(" in component '"+getName()+"' of type '"+getType()+"' was not referenced.");
        }
			}					                 
		}
	}
}
/**
 * Creation date: (4/4/2000 2:19:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Attribute
 * @param name java.lang.String
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public Attribute findAttribute(String name) 
throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object child=children.nextElement();
		if (child instanceof Attribute) {
			if (((Attribute)child).getName().equals(name)) {
				return (Attribute)child;
			}
		}
	}
	return null;
}
/**
 * Creation date: (4/8/2000 6:26:27 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.ComponentInstance
 */
public gov.nasa.arc.l2tools.livingstone.ComponentInstance getComponentInstance() {
	return componentInstance;
}
/**
 * Creation date: (4/8/2000 8:58:36 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.ComponentType
 */
public gov.nasa.arc.l2tools.livingstone.ComponentType getComponentType() {
	return componentType;
}
/**
 * Creation date: (4/7/2000 4:20:33 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getName() {
	return name;
}
/**
 * Creation date: (4/8/2000 9:19:12 PM)
 * @author: Andrew Bachmann
 * 
 * @return java.lang.String
 */
public java.lang.String getType() {
	return type;
}
/**
 * search for a Component with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @param key java.lang.String
 * @param value java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Component search(String key, String value, gov.nasa.arc.l2tools.DataObject object) {
	return (Component)object.search(Component.class,key,value);
}
/**
 * search for a Component named <i>name</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static Component searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (Component)object.searchByName(Component.class,name);
}
/**
 * Creation date: (4/8/2000 6:26:27 PM)
 * @author: Andrew Bachmann
 * 
 * @param newComponentInstance gov.nasa.arc.l2tools.livingstone.ComponentInstance
 */
protected void setComponentInstance(gov.nasa.arc.l2tools.livingstone.ComponentInstance newComponentInstance) {
	componentInstance = newComponentInstance;
}
/**
 * Creation date: (4/8/2000 8:58:36 PM)
 * @author: Andrew Bachmann
 * 
 * @param newComponentType gov.nasa.arc.l2tools.livingstone.ComponentType
 */
protected void setComponentType(gov.nasa.arc.l2tools.livingstone.ComponentType newComponentType) {
	componentType = newComponentType;
	setType(componentType.getName());
}
/**
 * Creation date: (4/7/2000 4:20:33 PM)
 * @author: Andrew Bachmann
 * 
 * @param newName java.lang.String
 */
protected void setName(java.lang.String newName) {
	name = newName;
	set("name",newName);
}
/**
 * Creation date: (4/8/2000 9:19:12 PM)
 * @author: Andrew Bachmann
 * 
 * @param newType java.lang.String
 */
protected void setType(java.lang.String newType) {
	type = newType;
	set("type",newType);
}
}
