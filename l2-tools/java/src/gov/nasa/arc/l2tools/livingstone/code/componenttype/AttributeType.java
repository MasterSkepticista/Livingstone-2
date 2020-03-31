//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone.code.componenttype;

/**
 * Insert the type's description here.
 * Creation date: (3/3/2000 3:15:17 PM)
 * @author: Andrew Bachmann
 */
public class AttributeType extends gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType {
	public final static String tagName = "ct:attributetype";
/**
 * CT_AttributeType constructor comment.
 */
public AttributeType() {
	super(tagName);
}
/**
 * CT_AttributeType constructor comment.
 * @param name java.lang.String
 */
public AttributeType(String name) {
	super(name);
}
/**
 *
 * Creation date: (3/10/2000 4:17:47 PM)
 * @author: Andrew Bachmann
 * 
 */
public void refreshCache() throws gov.nasa.arc.l2tools.Exception {
	try {
		if ((get("name") == null) || (get("name") == "")) {
			if (getParent() instanceof gov.nasa.arc.l2tools.livingstone.ComponentType) {
				setName(Integer.toString(((gov.nasa.arc.l2tools.livingstone.ComponentType)getParent()).getAttributeTypes().getSize()));
			} else {
				String errorString="cache error: component type attribute type has parent that is not a component type";
				throw new gov.nasa.arc.l2tools.livingstone.code.CacheError(errorString);
			}
		}
		super.refreshCache();
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n  ");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in component attribute type");
		if (getName()!=null) {
			error.concatMessage(": "+getName());
		}
		throw error;
	}
}
/**
 * search for a AttributeType with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType
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
 * @return gov.nasa.arc.l2tools.livingstone.code.componenttype.AttributeType
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static AttributeType searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (AttributeType)object.searchByName(AttributeType.class,name);
}
}
