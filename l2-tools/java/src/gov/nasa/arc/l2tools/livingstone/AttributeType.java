//
// * See the file "l2-tools/disclaimers-and-notices.txt" for 
// * information on usage and redistribution of this file, 
// * and for a DISCLAIMER OF ALL WARRANTIES.
//
package gov.nasa.arc.l2tools.livingstone;

/**
 * Insert the type's description here.
 * Creation date: (2/10/2000 5:39:10 PM)
 * @author: Andrew Bachmann
 */
public class AttributeType extends gov.nasa.arc.l2tools.livingstone.code.AbstractAttributeType {
	public final static String tagName = "attributetype";
/**
 * AttributeType constructor comment.
 */
public AttributeType() {
	super(tagName);
}
/**
 * AttributeType constructor comment.
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
		// cache attributes
		if ((get("name") == null) || (get("name") == "")) {
			String errorString = "illegal anonymous error: each attribute type must have a name";
			throw new gov.nasa.arc.l2tools.livingstone.code.IllegalAnonymousError(errorString);
		}
		super.refreshCache();
		if (!getAttributes().isEmpty()) {
			//String errorString2 = "unexpected child error: attribute(s) not allowed";
			//throw new gov.nasa.arc.l2tools.livingstone.code.UnexpectedChildError(errorString2);
		}
	} catch (gov.nasa.arc.l2tools.Exception error) {
		error.concatMessage("\n 	");
		if (getLine()!=null) {
			error.concatMessage("line "+getLine()+": ");
		}
		error.concatMessage("in toplevel attribute type");
		if (getName()!=null) {
			error.concatMessage(": "+getName());
		}
		throw error;
	}
}
/**
 * search for a AttributeType with value <i>value</i> for key <i>key</i>
 * Creation date: (10/24/2000 4:05:22 PM)
 * @return gov.nasa.arc.l2tools.livingstone.AttributeType
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
 * @return gov.nasa.arc.l2tools.livingstone.AttributeType
 * @param name java.lang.String
 * @param object gov.nasa.arc.l2tools.DataObject
 */
public static AttributeType searchByName(String name, gov.nasa.arc.l2tools.DataObject object) {
	return (AttributeType)object.searchByName(AttributeType.class,name);
}
}
