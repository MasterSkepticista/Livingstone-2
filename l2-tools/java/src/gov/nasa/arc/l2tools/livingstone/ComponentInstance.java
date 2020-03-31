//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone;

/**
 * Insert the type's description here.
 * Creation date: (3/9/2000 5:26:45 PM)
 * @author: Andrew Bachmann
 */
public class ComponentInstance extends gov.nasa.arc.l2tools.livingstone.DataObject {
  public final static String tagName = "componentinstance";
/**
 * ComponentInstance constructor comment.
 */
public ComponentInstance() {
	super(tagName);
}
/**
 * ComponentInstance constructor comment.
 * @param name java.lang.String
 */
public ComponentInstance(String name) {
	super(name);
}
/**
 * Creation date: (4/4/2000 2:19:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType
 * @param name java.lang.String
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType findAttributeType(String name) 
throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object child=children.nextElement();
		if (child instanceof gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType) {
			if (((gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType)child).get("name").equals(name)) {
				return (gov.nasa.arc.l2tools.livingstone.componentinstance.AttributeType)child;
			}
		}
	}
	return null;
}
/**
 * Creation date: (4/4/2000 2:19:32 PM)
 * @author: Andrew Bachmann
 * 
 * @return gov.nasa.arc.l2tools.livingstone.componentinstance.Component
 * @param name java.lang.String
 * @exception gov.nasa.arc.l2tools.Exception The exception description.
 */
public gov.nasa.arc.l2tools.livingstone.componentinstance.Component findComponent(String name) 
throws gov.nasa.arc.l2tools.Exception {
	java.util.Enumeration children=getChildren().elements();
	while (children.hasMoreElements()) {
		java.lang.Object child=children.nextElement();
		if (child instanceof gov.nasa.arc.l2tools.livingstone.componentinstance.Component) {
			if (((gov.nasa.arc.l2tools.livingstone.componentinstance.Component)child).get("name").equals(name)) {
				return (gov.nasa.arc.l2tools.livingstone.componentinstance.Component)child;
			}
		}
	}
	return null;
}
}
